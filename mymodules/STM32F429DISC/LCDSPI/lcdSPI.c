#include "py/obj.h"
#include "py/runtime.h"
#include "py/builtin.h"
#include "lcdSPI.h"

SPI_HandleTypeDef hspi5;

uint16_t ILI9341_SCREEN_HEIGHT	= 240;
uint16_t ILI9341_SCREEN_WIDTH	= 320;


void ILI9341_Write_Command(uint8_t Command)
{
	HAL_GPIO_WritePin(LCD_CS_PORT, LCD_CS_PIN, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(LCD_DC_PORT, LCD_DC_PIN, GPIO_PIN_RESET);
	HAL_SPI_Transmit(&hspi5, &Command, 1, 100);
	HAL_GPIO_WritePin(LCD_CS_PORT, LCD_CS_PIN, GPIO_PIN_SET);
}

void ILI9341_Write_Data(uint8_t Data)
{
	HAL_GPIO_WritePin(LCD_DC_PORT, LCD_DC_PIN, GPIO_PIN_SET);
	HAL_GPIO_WritePin(LCD_CS_PORT, LCD_CS_PIN, GPIO_PIN_RESET);
	HAL_SPI_Transmit(&hspi5, &Data, 1, 100);
	HAL_GPIO_WritePin(LCD_CS_PORT, LCD_CS_PIN, GPIO_PIN_SET);
}

void ILI9341_Set_Rotation(uint8_t Rotation)
{

	uint8_t screen_rotation = Rotation;

	ILI9341_Write_Command(0x36);
	HAL_Delay(1);

	switch(screen_rotation)
	{
		case SCREEN_VERTICAL:
			ILI9341_Write_Data(0x40|0x08);
			ILI9341_SCREEN_WIDTH = 240;
			ILI9341_SCREEN_HEIGHT = 320;
			break;
		case SCREEN_HORIZONTAL:
			ILI9341_Write_Data(0x20|0x08);
			ILI9341_SCREEN_WIDTH  = 320;
			ILI9341_SCREEN_HEIGHT = 240;
			break;
		case SCREEN_VERTICAL_INV:
			ILI9341_Write_Data(0x80|0x08);
			ILI9341_SCREEN_WIDTH  = 240;
			ILI9341_SCREEN_HEIGHT = 320;
			break;
		case SCREEN_HORIZONTAL_INV:
			ILI9341_Write_Data(0x40|0x80|0x20|0x08);
			ILI9341_SCREEN_WIDTH  = 320;
			ILI9341_SCREEN_HEIGHT = 240;
			break;
		default:
			//EXIT IF SCREEN ROTATION NOT VALID!
			break;
	}
}

void ILI9341_Set_Address(uint16_t X1, uint16_t Y1, uint16_t X2, uint16_t Y2)
{

	ILI9341_Write_Command(0x2A);
	ILI9341_Write_Data(X1>>8);
	ILI9341_Write_Data(X1);
	ILI9341_Write_Data(X2>>8);
	ILI9341_Write_Data(X2);

	ILI9341_Write_Command(0x2B);
	ILI9341_Write_Data(Y1>>8);
	ILI9341_Write_Data(Y1);
	ILI9341_Write_Data(Y2>>8);
	ILI9341_Write_Data(Y2);

	ILI9341_Write_Command(0x2C);
}








STATIC mp_obj_t init(mp_obj_t rotation) {
	uint8_t _rotation = mp_obj_get_int(rotation);
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	__HAL_RCC_SPI5_CLK_ENABLE();
    	__HAL_RCC_GPIOF_CLK_ENABLE();
	__HAL_RCC_GPIOC_CLK_ENABLE();
	__HAL_RCC_GPIOD_CLK_ENABLE();

	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_2, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13, GPIO_PIN_RESET);
	GPIO_InitStruct.Pin = GPIO_PIN_2;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
	GPIO_InitStruct.Pin = GPIO_PIN_13;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);
  
	GPIO_InitStruct.Pin = GPIO_PIN_7|GPIO_PIN_9;
    	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    	GPIO_InitStruct.Pull = GPIO_NOPULL;
    	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStruct.Alternate = GPIO_AF5_SPI5;
    	HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);	
	hspi5.Instance = SPI5;
	hspi5.Init.Mode = SPI_MODE_MASTER;
	hspi5.Init.Direction = SPI_DIRECTION_1LINE;
	hspi5.Init.DataSize = SPI_DATASIZE_8BIT;
	hspi5.Init.CLKPolarity = SPI_POLARITY_LOW;
	hspi5.Init.CLKPhase = SPI_PHASE_1EDGE;
	hspi5.Init.NSS = SPI_NSS_SOFT;
	hspi5.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
	hspi5.Init.FirstBit = SPI_FIRSTBIT_MSB;
	hspi5.Init.TIMode = SPI_TIMODE_DISABLE;
	hspi5.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
	hspi5.Init.CRCPolynomial = 10;
	HAL_SPI_Init(&hspi5);	

	//SOFTWARE RESET
	ILI9341_Write_Command(0x01);
	HAL_Delay(10);

	//POWER CONTROL A
	ILI9341_Write_Command(0xCB);
	ILI9341_Write_Data(0x39);
	ILI9341_Write_Data(0x2C);
	ILI9341_Write_Data(0x00);
	ILI9341_Write_Data(0x34);
	ILI9341_Write_Data(0x02);

	//POWER CONTROL B
	ILI9341_Write_Command(0xCF);
	ILI9341_Write_Data(0x00);
	ILI9341_Write_Data(0xC1);
	ILI9341_Write_Data(0x30);

	//DRIVER TIMING CONTROL A
	ILI9341_Write_Command(0xE8);
	ILI9341_Write_Data(0x85);
	ILI9341_Write_Data(0x00);
	ILI9341_Write_Data(0x78);

	//DRIVER TIMING CONTROL B
	ILI9341_Write_Command(0xEA);
	ILI9341_Write_Data(0x00);
	ILI9341_Write_Data(0x00);

	//POWER ON SEQUENCE CONTROL
	ILI9341_Write_Command(0xED);
	ILI9341_Write_Data(0x64);
	ILI9341_Write_Data(0x03);
	ILI9341_Write_Data(0x12);
	ILI9341_Write_Data(0x81);

	//PUMP RATIO CONTROL
	ILI9341_Write_Command(0xF7);
	ILI9341_Write_Data(0x20);

	//POWER CONTROL,VRH[5:0]
	ILI9341_Write_Command(0xC0);
	ILI9341_Write_Data(0x23);

	//POWER CONTROL,SAP[2:0];BT[3:0]
	ILI9341_Write_Command(0xC1);
	ILI9341_Write_Data(0x10);

	//VCM CONTROL
	ILI9341_Write_Command(0xC5);
	ILI9341_Write_Data(0x3E);
	ILI9341_Write_Data(0x28);

	//VCM CONTROL 2
	ILI9341_Write_Command(0xC7);
	ILI9341_Write_Data(0x86);

	//MEMORY ACCESS CONTROL
	ILI9341_Write_Command(0x36);
	ILI9341_Write_Data(0x48);

	//PIXEL FORMAT
	ILI9341_Write_Command(0x3A);
	ILI9341_Write_Data(0x55);

	//FRAME RATIO CONTROL, STANDARD RGB COLOR
	ILI9341_Write_Command(0xB1);
	ILI9341_Write_Data(0x00);
	ILI9341_Write_Data(0x18);

	//DISPLAY FUNCTION CONTROL
	ILI9341_Write_Command(0xB6);
	ILI9341_Write_Data(0x08);
	ILI9341_Write_Data(0x82);
	ILI9341_Write_Data(0x27);

	//3GAMMA FUNCTION DISABLE
	ILI9341_Write_Command(0xF2);
	ILI9341_Write_Data(0x00);

	//GAMMA CURVE SELECTED
	ILI9341_Write_Command(0x26);
	ILI9341_Write_Data(0x01);

	//POSITIVE GAMMA CORRECTION
	ILI9341_Write_Command(0xE0);
	ILI9341_Write_Data(0x0F);
	ILI9341_Write_Data(0x31);
	ILI9341_Write_Data(0x2B);
	ILI9341_Write_Data(0x0C);
	ILI9341_Write_Data(0x0E);
	ILI9341_Write_Data(0x08);
	ILI9341_Write_Data(0x4E);
	ILI9341_Write_Data(0xF1);
	ILI9341_Write_Data(0x37);
	ILI9341_Write_Data(0x07);
	ILI9341_Write_Data(0x10);
	ILI9341_Write_Data(0x03);
	ILI9341_Write_Data(0x0E);
	ILI9341_Write_Data(0x09);
	ILI9341_Write_Data(0x00);

	//NEGATIVE GAMMA CORRECTION
	ILI9341_Write_Command(0xE1);
	ILI9341_Write_Data(0x00);
	ILI9341_Write_Data(0x0E);
	ILI9341_Write_Data(0x14);
	ILI9341_Write_Data(0x03);
	ILI9341_Write_Data(0x11);
	ILI9341_Write_Data(0x07);
	ILI9341_Write_Data(0x31);
	ILI9341_Write_Data(0xC1);
	ILI9341_Write_Data(0x48);
	ILI9341_Write_Data(0x08);
	ILI9341_Write_Data(0x0F);
	ILI9341_Write_Data(0x0C);
	ILI9341_Write_Data(0x31);
	ILI9341_Write_Data(0x36);
	ILI9341_Write_Data(0x0F);

	//EXIT SLEEP
	ILI9341_Write_Command(0x11);
	HAL_Delay(120);

	//TURN ON DISPLAY
	ILI9341_Write_Command(0x29);

	//STARTING ROTATION
	ILI9341_Set_Rotation(_rotation);
	
	return mp_obj_new_int(1);
}
STATIC MP_DEFINE_CONST_FUN_OBJ_1(lcdSPI_init_obj, init);

STATIC mp_obj_t setAddress(mp_uint_t n_args, const mp_obj_t *args) {
	(void)n_args;
	uint16_t _X1 = mp_obj_get_int(args[0]); 
	uint16_t _Y1 = mp_obj_get_int(args[1]); 
	uint16_t _X2 = mp_obj_get_int(args[2]); 
	uint16_t _Y2 = mp_obj_get_int(args[3]);
	ILI9341_Write_Command(0x2A);
	ILI9341_Write_Data(_X1>>8);
	ILI9341_Write_Data(_X1);
	ILI9341_Write_Data(_X2>>8);
	ILI9341_Write_Data(_X2);

	ILI9341_Write_Command(0x2B);
	ILI9341_Write_Data(_Y1>>8);
	ILI9341_Write_Data(_Y1);
	ILI9341_Write_Data(_Y2>>8);
	ILI9341_Write_Data(_Y2);

	ILI9341_Write_Command(0x2C);
	
    	return mp_obj_new_int(1);
}
STATIC MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(lcdSPI_setAddress_obj, 4, 4, setAddress);

STATIC mp_obj_t fillScreen(mp_obj_t Color) {
	uint16_t _Color = mp_obj_get_int(Color); 
	uint8_t _data[1000];
	uint8_t pxl_msb = _Color>>8, pxl_lsb = _Color;
	uint16_t _blocksize = 1000, _lastblocksize = 0;
	uint32_t i = 0, _blockCount = 0;
	uint32_t _framesize = ILI9341_SCREEN_WIDTH*ILI9341_SCREEN_HEIGHT*2;

	for(i = 0; i < 1000; i+=2)
	{
		_data[i] = 	pxl_msb;
		_data[i+1] = pxl_lsb;
	}

	_blockCount = _framesize/_blocksize;
	_lastblocksize = (uint16_t)(_framesize % _blocksize);

	ILI9341_Set_Address(0,0,ILI9341_SCREEN_WIDTH,ILI9341_SCREEN_HEIGHT);
	HAL_GPIO_WritePin(LCD_DC_PORT, LCD_DC_PIN, GPIO_PIN_SET);
	HAL_GPIO_WritePin(LCD_CS_PORT, LCD_CS_PIN, GPIO_PIN_RESET);

	for(i=0;i<_blockCount; i++)
		HAL_SPI_Transmit(&hspi5, _data, _blocksize, 100);

	if(_lastblocksize)
		HAL_SPI_Transmit(&hspi5, _data, _lastblocksize, 100);

	HAL_GPIO_WritePin(LCD_CS_PORT, LCD_CS_PIN, GPIO_PIN_SET);
	
    	return mp_obj_new_int(1);
}
STATIC MP_DEFINE_CONST_FUN_OBJ_1(lcdSPI_fillScreen_obj, fillScreen);

STATIC mp_obj_t drawPixel(mp_obj_t Color) {
	uint16_t _Color = mp_obj_get_int(Color); 
	uint8_t _data[2] = {_Color>>8, _Color};

	HAL_GPIO_WritePin(LCD_DC_PORT, LCD_DC_PIN, GPIO_PIN_SET);
	HAL_GPIO_WritePin(LCD_CS_PORT, LCD_CS_PIN, GPIO_PIN_RESET);
	HAL_SPI_Transmit(&hspi5, _data, 2, 100);
	HAL_GPIO_WritePin(LCD_CS_PORT, LCD_CS_PIN, GPIO_PIN_SET);
	
    	return mp_obj_new_int(1);
}
STATIC MP_DEFINE_CONST_FUN_OBJ_1(lcdSPI_drawPixel_obj, drawPixel);


STATIC mp_obj_t drawImage(mp_uint_t n_args, const mp_obj_t *args) {
	(void)n_args;
	mp_obj_t *items;
	mp_uint_t len;
	mp_obj_get_array(args[0], &len, &items);
	uint8_t* pData = (uint8_t*)m_malloc(len*sizeof(uint8_t));
	for(mp_uint_t i = 0; i < len; i++){
		pData[i] = mp_obj_get_int(items[i]);
	}
	uint16_t width = mp_obj_get_int(args[1]); 
	uint16_t height = mp_obj_get_int(args[2]); 
	uint32_t size = mp_obj_get_int(args[3]);
	uint16_t xOffset = mp_obj_get_int(args[4]); 
	uint16_t yOffset = mp_obj_get_int(args[5]);
	if((xOffset + width) > ILI9341_SCREEN_WIDTH  && (yOffset + height) > ILI9341_SCREEN_HEIGHT) return mp_obj_new_int(1);

	uint16_t _blocksize = 65535, _lastblocksize = 0;	
	uint32_t i = 0, _blockCount = 0;

	if(size < 65536)_blocksize = size;
	_blockCount = size/_blocksize;
	_lastblocksize = (uint16_t)(size % _blocksize);
	
	ILI9341_Set_Address(xOffset, yOffset, xOffset + width - 1 , yOffset + height -1);
	HAL_GPIO_WritePin(LCD_DC_PORT, LCD_DC_PIN, GPIO_PIN_SET);
	HAL_GPIO_WritePin(LCD_CS_PORT, LCD_CS_PIN, GPIO_PIN_RESET);

	for(i=0;i<_blockCount; i++)
	HAL_SPI_Transmit(&hspi5, pData + (i*_blocksize) , _blocksize, 100);

	if(_lastblocksize)
	HAL_SPI_Transmit(&hspi5, pData + (i*_blocksize) , _lastblocksize, 100);

	HAL_GPIO_WritePin(LCD_CS_PORT, LCD_CS_PIN, GPIO_PIN_SET);
	m_free(pData);
    	return mp_obj_new_int(1);
}
STATIC MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(lcdSPI_drawImage_obj, 6, 6, drawImage);

STATIC const mp_rom_map_elem_t lcdSPI_module_globals_table[] = {
    { MP_ROM_QSTR(MP_QSTR___name__), MP_ROM_QSTR(MP_QSTR_lcdSPI) },
    { MP_ROM_QSTR(MP_QSTR_init), MP_ROM_PTR(&lcdSPI_init_obj) },
    { MP_ROM_QSTR(MP_QSTR_setAddress), MP_ROM_PTR(&lcdSPI_setAddress_obj) },	
    { MP_ROM_QSTR(MP_QSTR_fillScreen), MP_ROM_PTR(&lcdSPI_fillScreen_obj) },
    { MP_ROM_QSTR(MP_QSTR_drawPixel), MP_ROM_PTR(&lcdSPI_drawPixel_obj) },
    { MP_ROM_QSTR(MP_QSTR_drawImage), MP_ROM_PTR(&lcdSPI_drawImage_obj) },	
};
STATIC MP_DEFINE_CONST_DICT(lcdSPI_module_globals, lcdSPI_module_globals_table);

const mp_obj_module_t lcdSPI_cmodule = {
    .base = { &mp_type_module },
    .globals = (mp_obj_dict_t*)&lcdSPI_module_globals,
};

#if MODULE_LCDSPI_ENABLED
MP_REGISTER_MODULE(MP_QSTR_lcdSPI, lcdSPI_cmodule);
#endif