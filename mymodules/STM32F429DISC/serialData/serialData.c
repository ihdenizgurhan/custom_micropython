#include "py/obj.h"
#include "py/runtime.h"
#include "py/builtin.h"

UART_HandleTypeDef huart5;

void SERIAL_dataSend(uint8_t *pData, uint16_t Size, uint8_t Type)
{
	uint8_t request_start_sequence[3] = "STW";

	HAL_UART_Transmit(&huart5, request_start_sequence, 3, 100);
	HAL_Delay(1);

	HAL_UART_Transmit(&huart5, (uint8_t *)&Size, 2, 100);
	HAL_UART_Transmit(&huart5, &Type, 1, 100);
	HAL_Delay(200);

	HAL_UART_Transmit(&huart5, (uint8_t *)pData, Size, 10000);
	HAL_Delay(200);
}

void SERIAL_dataReceive(uint8_t *pData, uint16_t Size, uint8_t Type)
{
	uint8_t request_start_sequence[3] = "STR";

	HAL_UART_Transmit(&huart5, request_start_sequence, 3, 100);
	HAL_Delay(1);

	HAL_UART_Transmit(&huart5, (uint8_t *)&Size, 2, 100);
	HAL_UART_Transmit(&huart5, &Type, 1, 100);
	HAL_UART_Receive(&huart5, (uint8_t *)pData, Size, 10000);
}

STATIC mp_obj_t init(void) {
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	__HAL_RCC_UART5_CLK_ENABLE();
	__HAL_RCC_GPIOC_CLK_ENABLE();
	__HAL_RCC_GPIOD_CLK_ENABLE();
	GPIO_InitStruct.Pin = GPIO_PIN_12;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStruct.Alternate = GPIO_AF8_UART5;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
	GPIO_InitStruct.Pin = GPIO_PIN_2;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStruct.Alternate = GPIO_AF8_UART5;
	HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);
	huart5.Instance = UART5;
	huart5.Init.BaudRate = 2000000;
	huart5.Init.WordLength = UART_WORDLENGTH_8B;
	huart5.Init.StopBits = UART_STOPBITS_1;
	huart5.Init.Parity = UART_PARITY_NONE;
	huart5.Init.Mode = UART_MODE_TX_RX;
	huart5.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	huart5.Init.OverSampling = UART_OVERSAMPLING_16;
	HAL_UART_Init(&huart5);	
	return mp_obj_new_int(1);
}
STATIC MP_DEFINE_CONST_FUN_OBJ_0(serialData_init_obj, init);

STATIC mp_obj_t receive(mp_uint_t n_args, const mp_obj_t *args) {
	(void)n_args;
	mp_obj_list_t *self = MP_OBJ_TO_PTR(args[0]);
	uint16_t size = mp_obj_get_int(args[1]); 
	uint8_t type = mp_obj_get_int(args[2]); 
	if(type == 0){
		uint8_t* pData = (uint8_t*)m_malloc((self->len)*sizeof(uint8_t));
		SERIAL_dataReceive((uint8_t*) pData, size, type);
		for(mp_uint_t i = 0; i < (self->len); i++){
			self->items[i] = mp_obj_new_int(pData[i]);
		}
		m_free(pData);
	}
	else if(type == 1){
		uint16_t* pData = (uint16_t*)m_malloc((self->len)*sizeof(uint16_t));
		SERIAL_dataReceive((uint8_t*) pData, size, type);
		for(mp_uint_t i = 0; i < (self->len); i++){
			self->items[i] = mp_obj_new_int(pData[i]);
		}
		m_free(pData);
	}
	else if(type == 2){
		uint32_t* pData = (uint32_t*)m_malloc((self->len)*sizeof(uint32_t));
		SERIAL_dataReceive((uint8_t*) pData, size, type);
		for(mp_uint_t i = 0; i < (self->len); i++){
			self->items[i] = mp_obj_new_int(pData[i]);
		}
		m_free(pData);
	}
	else if(type == 3){
		float* pData = (float*)m_malloc((self->len)*sizeof(float));
		SERIAL_dataReceive((uint8_t*) pData, size, type);
		for(mp_uint_t i = 0; i < (self->len); i++){
			self->items[i] = mp_obj_new_float(pData[i]);
		}
		m_free(pData);
	}	
    	return mp_obj_new_int(1);
}
STATIC MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(serialData_receive_obj, 3, 3, receive);

STATIC mp_obj_t send(mp_uint_t n_args, const mp_obj_t *args) {
	(void)n_args;
	mp_obj_t *items;
	mp_uint_t len;
	mp_obj_get_array(args[0], &len, &items);
	uint16_t size = mp_obj_get_int(args[1]); 
	uint8_t type = mp_obj_get_int(args[2]); 
	if(type == 0){
		uint8_t* pData = (uint8_t*)m_malloc(len*sizeof(uint8_t));
		for(mp_uint_t i = 0; i < len; i++){
			pData[i] = mp_obj_get_int(items[i]);
		}
		SERIAL_dataSend((uint8_t*) pData, size, type);
		m_free(pData);
	}
	else if(type == 1){
		uint16_t* pData = (uint16_t*)m_malloc(len*sizeof(uint16_t));
		for(mp_uint_t i = 0; i < len; i++){
			pData[i] = mp_obj_get_int(items[i]);
		}
		SERIAL_dataSend((uint8_t*) pData, size, type);
		m_free(pData);
	}
	else if(type == 2){
		uint32_t* pData = (uint32_t*)m_malloc(len*sizeof(uint32_t));
		for(mp_uint_t i = 0; i < len; i++){
			pData[i] = mp_obj_get_int(items[i]);
		}
		SERIAL_dataSend((uint8_t*) pData, size, type);
		m_free(pData);
	}
	else if(type == 3){
		float* pData = (float*)m_malloc(len*sizeof(float));
		for(mp_uint_t i = 0; i < len; i++){
			pData[i] = mp_obj_get_float(items[i]);
		}
		SERIAL_dataSend((uint8_t*) pData, size, type);
		m_free(pData);
	}	
    	return mp_obj_new_int(1);
}
STATIC MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(serialData_send_obj, 3, 3, send);

STATIC const mp_rom_map_elem_t serialData_module_globals_table[] = {
    { MP_ROM_QSTR(MP_QSTR___name__), MP_ROM_QSTR(MP_QSTR_serialData) },
    { MP_ROM_QSTR(MP_QSTR_init), MP_ROM_PTR(&serialData_init_obj) },
    { MP_ROM_QSTR(MP_QSTR_receive), MP_ROM_PTR(&serialData_receive_obj) },	
    { MP_ROM_QSTR(MP_QSTR_send), MP_ROM_PTR(&serialData_send_obj) },
};
STATIC MP_DEFINE_CONST_DICT(serialData_module_globals, serialData_module_globals_table);

const mp_obj_module_t serialData_cmodule = {
    .base = { &mp_type_module },
    .globals = (mp_obj_dict_t*)&serialData_module_globals,
};

#if MODULE_SERIALDATA_ENABLED
MP_REGISTER_MODULE(MP_QSTR_serialData, serialData_cmodule);
#endif