#include "py/obj.h"
#include "py/objlist.h"
#include "py/runtime.h"
#include "py/builtin.h"


SDRAM_HandleTypeDef hsdram1;	

STATIC mp_obj_t init(void) {
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	FMC_SDRAM_TimingTypeDef SdramTiming = {0};
	FMC_SDRAM_CommandTypeDef my_command;
	uint32_t refreshrate;
	
    	__HAL_RCC_FMC_CLK_ENABLE();
	
	GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3
						  |GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_11|GPIO_PIN_12
						  |GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStruct.Alternate = GPIO_AF12_FMC;
	HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = GPIO_PIN_0;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStruct.Alternate = GPIO_AF12_FMC;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_4|GPIO_PIN_5
						  |GPIO_PIN_8|GPIO_PIN_15;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStruct.Alternate = GPIO_AF12_FMC;
	HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10
						  |GPIO_PIN_11|GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14
						  |GPIO_PIN_15|GPIO_PIN_0|GPIO_PIN_1;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStruct.Alternate = GPIO_AF12_FMC;
	HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_14
						  |GPIO_PIN_15|GPIO_PIN_0|GPIO_PIN_1;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStruct.Alternate = GPIO_AF12_FMC;
	HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = GPIO_PIN_5|GPIO_PIN_6;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStruct.Alternate = GPIO_AF12_FMC;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
	
	hsdram1.Instance = FMC_SDRAM_DEVICE;
	/* hsdram1.Init */
	hsdram1.Init.SDBank = FMC_SDRAM_BANK2;
	hsdram1.Init.ColumnBitsNumber = FMC_SDRAM_COLUMN_BITS_NUM_8;
	hsdram1.Init.RowBitsNumber = FMC_SDRAM_ROW_BITS_NUM_12;
	hsdram1.Init.MemoryDataWidth = FMC_SDRAM_MEM_BUS_WIDTH_16;
	hsdram1.Init.InternalBankNumber = FMC_SDRAM_INTERN_BANKS_NUM_4;
	hsdram1.Init.CASLatency = FMC_SDRAM_CAS_LATENCY_2;
	hsdram1.Init.WriteProtection = FMC_SDRAM_WRITE_PROTECTION_DISABLE;
	hsdram1.Init.SDClockPeriod = FMC_SDRAM_CLOCK_PERIOD_2;
	hsdram1.Init.ReadBurst = FMC_SDRAM_RBURST_DISABLE;
	hsdram1.Init.ReadPipeDelay = FMC_SDRAM_RPIPE_DELAY_0;
	/* SdramTiming */
	SdramTiming.LoadToActiveDelay = 2;
	SdramTiming.ExitSelfRefreshDelay = 7;
	SdramTiming.SelfRefreshTime = 4;
	SdramTiming.RowCycleDelay = 6;
	SdramTiming.WriteRecoveryTime = 2;
	SdramTiming.RPDelay = 2;
	SdramTiming.RCDDelay = 2;	
	
	HAL_SDRAM_Init(&hsdram1, &SdramTiming);
	
	my_command.CommandMode = FMC_SDRAM_CMD_CLK_ENABLE;
	my_command.CommandTarget = FMC_SDRAM_CMD_TARGET_BANK2;
	my_command.AutoRefreshNumber = 0;
	my_command.ModeRegisterDefinition = 0;
	HAL_SDRAM_SendCommand(&hsdram1, &my_command, 100);
	HAL_Delay(1);
	my_command.CommandMode = FMC_SDRAM_CMD_PALL;
	HAL_SDRAM_SendCommand(&hsdram1, &my_command, 100);
	my_command.CommandMode = FMC_SDRAM_CMD_AUTOREFRESH_MODE;
	my_command.AutoRefreshNumber = 3;
	HAL_SDRAM_SendCommand(&hsdram1, &my_command, 100);
	my_command.CommandMode = FMC_SDRAM_CMD_LOAD_MODE;
	my_command.ModeRegisterDefinition = 0b000000100000;
	HAL_SDRAM_SendCommand(&hsdram1, &my_command, 100);
	
	refreshrate = (uint32_t)(90000000*0.064/4096); 
	HAL_SDRAM_ProgramRefreshRate(&hsdram1, refreshrate);
	
    	return mp_obj_new_int(1);
}
STATIC MP_DEFINE_CONST_FUN_OBJ_0(sdram_init_obj, init);

STATIC mp_obj_t write8b(mp_obj_t pAddress, mp_obj_t pSrcBuffer, mp_obj_t BufferSize) {
	mp_obj_t *items;
	mp_uint_t len;
	mp_obj_get_array(pAddress, &len, &items);
	uint32_t _pAddress;
	uint32_t _pAddressArr[2];
	for(mp_uint_t i = 0; i < len; i++){
		_pAddressArr[i] = mp_obj_get_int(items[i]);
	}
	_pAddress = (0xFFFF0000 & (_pAddressArr[0]<<16))| (0x0000FFFF & _pAddressArr[1]);
	uint32_t _bufferSize = mp_obj_get_int(BufferSize);
	uint8_t* _pSrcBuffer = (uint8_t*)m_malloc(_bufferSize*sizeof(uint8_t));
	mp_obj_get_array(pSrcBuffer, &len, &items);
	for(mp_uint_t i = 0; i < len; i++){
		_pSrcBuffer[i] = mp_obj_get_int(items[i]);
	}
	HAL_SDRAM_Write_8b(&hsdram1, (uint32_t*) (_pAddress), _pSrcBuffer, _bufferSize);
	m_free(_pSrcBuffer);
    	return mp_obj_new_int(1);
}
STATIC MP_DEFINE_CONST_FUN_OBJ_3(sdram_write8b_obj, write8b);

STATIC mp_obj_t write16b(mp_obj_t pAddress, mp_obj_t pSrcBuffer, mp_obj_t BufferSize) {
	mp_obj_t *items;
	mp_uint_t len;
	mp_obj_get_array(pAddress, &len, &items);
	uint32_t _pAddress;
	uint32_t _pAddressArr[2];
	for(mp_uint_t i = 0; i < len; i++){
		_pAddressArr[i] = mp_obj_get_int(items[i]);
	}
	_pAddress = (0xFFFF0000 & (_pAddressArr[0]<<16))| (0x0000FFFF & _pAddressArr[1]);

	uint32_t _bufferSize = mp_obj_get_int(BufferSize);
	uint16_t* _pSrcBuffer = (uint16_t*)m_malloc(_bufferSize*sizeof(uint16_t));
	mp_obj_get_array(pSrcBuffer, &len, &items);
	for(mp_uint_t i = 0; i < len; i++){
		_pSrcBuffer[i] = mp_obj_get_int(items[i]);
	}
	HAL_SDRAM_Write_16b(&hsdram1, (uint32_t*) (_pAddress), _pSrcBuffer, _bufferSize);
	m_free(_pSrcBuffer);
    	return mp_obj_new_int(1);
}
STATIC MP_DEFINE_CONST_FUN_OBJ_3(sdram_write16b_obj, write16b);

STATIC mp_obj_t write32b(mp_obj_t pAddress, mp_obj_t pSrcBuffer, mp_obj_t BufferSize) {
	mp_obj_t *items;
	mp_uint_t len;
	mp_obj_get_array(pAddress, &len, &items);
	uint32_t _pAddress;
	uint32_t _pAddressArr[2];
	for(mp_uint_t i = 0; i < len; i++){
		_pAddressArr[i] = mp_obj_get_int(items[i]);
	}
	_pAddress = (0xFFFF0000 & (_pAddressArr[0]<<16))| (0x0000FFFF & _pAddressArr[1]);

	uint32_t _bufferSize = mp_obj_get_int(BufferSize);
	uint32_t* _pSrcBuffer = (uint32_t*)m_malloc(_bufferSize*sizeof(uint32_t));
	mp_obj_get_array(pSrcBuffer, &len, &items);
	for(mp_uint_t i = 0; i < len; i++){
		_pSrcBuffer[i] = mp_obj_get_int(items[i]);
	}
	HAL_SDRAM_Write_32b(&hsdram1, (uint32_t*) (_pAddress), _pSrcBuffer, _bufferSize);
	m_free(_pSrcBuffer);
    	return mp_obj_new_int(1);
}
STATIC MP_DEFINE_CONST_FUN_OBJ_3(sdram_write32b_obj, write32b);
	
	
STATIC mp_obj_t read8b(mp_obj_t pAddress, mp_obj_t BufferSize) {
	uint32_t _bufferSize = mp_obj_get_int(BufferSize);
	mp_obj_t *items;
	mp_uint_t len;
	mp_obj_get_array(pAddress, &len, &items);
	uint32_t _pAddress;
	uint32_t _pAddressArr[2];
	for(mp_uint_t i = 0; i < len; i++){
		_pAddressArr[i] = mp_obj_get_int(items[i]);
	}
	_pAddress = (0xFFFF0000 & (_pAddressArr[0]<<16))| (0x0000FFFF & _pAddressArr[1]);
	uint8_t* _pDstBuffer = (uint8_t*)m_malloc(_bufferSize*sizeof(uint8_t));

	HAL_SDRAM_Read_8b(&hsdram1, (uint32_t*) (_pAddress), _pDstBuffer, _bufferSize);
	mp_obj_t dummy = mp_obj_new_bytearray(_bufferSize, (mp_obj_t*)_pDstBuffer);
	return dummy;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_2(sdram_read8b_obj, read8b);		
	

STATIC const mp_rom_map_elem_t sdram_module_globals_table[] = {
    	{ MP_ROM_QSTR(MP_QSTR___name__), MP_ROM_QSTR(MP_QSTR_sdram) },
    	{ MP_ROM_QSTR(MP_QSTR_init), MP_ROM_PTR(&sdram_init_obj) },
	{ MP_ROM_QSTR(MP_QSTR_write8b), MP_ROM_PTR(&sdram_write8b_obj) },
	{ MP_ROM_QSTR(MP_QSTR_write16b), MP_ROM_PTR(&sdram_write16b_obj) },
	{ MP_ROM_QSTR(MP_QSTR_write32b), MP_ROM_PTR(&sdram_write32b_obj) },
	{ MP_ROM_QSTR(MP_QSTR_read8b), MP_ROM_PTR(&sdram_read8b_obj) },
};
STATIC MP_DEFINE_CONST_DICT(sdram_module_globals, sdram_module_globals_table);

const mp_obj_module_t sdram_cmodule = {
    .base = { &mp_type_module },
    .globals = (mp_obj_dict_t*)&sdram_module_globals,
};

#if MODULE_SDRAM_ENABLED
MP_REGISTER_MODULE(MP_QSTR_sdram, sdram_cmodule);
#endif