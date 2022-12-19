#include "py/obj.h"
#include "py/runtime.h"
#include "py/builtin.h"
#include "stm32f429i_discovery.h"
#include "stm32f429i_discovery_gyroscope.h"
#include "gyro.h"
#include "l3gd20.h"

//I2C_HandleTypeDef hi2c3;

STATIC mp_obj_t init(void) {
	uint8_t data;	
/*	GPIO_InitTypeDef GPIO_InitStruct = {0};

    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();
    GPIO_InitStruct.Pin = GPIO_PIN_9;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF4_I2C3;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
    GPIO_InitStruct.Pin = GPIO_PIN_8;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF4_I2C3;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
	__HAL_RCC_I2C3_CLK_ENABLE();

	hi2c3.Instance = I2C3;
	hi2c3.Init.ClockSpeed = 100000;
	hi2c3.Init.DutyCycle = I2C_DUTYCYCLE_2;
	hi2c3.Init.OwnAddress1 = 0;
	hi2c3.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
	hi2c3.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
	hi2c3.Init.OwnAddress2 = 0;
	hi2c3.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
	hi2c3.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
	HAL_I2C_Init(&hi2c3);*/
	data = BSP_GYRO_Init();
	return mp_obj_new_int(data);
}
STATIC MP_DEFINE_CONST_FUN_OBJ_0(gyro_init_obj, init);

STATIC mp_obj_t read_id(void) {
	uint8_t data;
    	data = BSP_GYRO_ReadID();

	return mp_obj_new_int(data);
}
STATIC MP_DEFINE_CONST_FUN_OBJ_0(gyro_read_id_obj, read_id);

STATIC mp_obj_t get_x(void) {
	float data[3];
    	BSP_GYRO_GetXYZ(data);

    	return mp_obj_new_float(data[0]);
}
STATIC MP_DEFINE_CONST_FUN_OBJ_0(gyro_get_x_obj, get_x);

STATIC mp_obj_t get_y(void) {
	float data[3];
    	BSP_GYRO_GetXYZ(data);

    	return mp_obj_new_float(data[1]);
}
STATIC MP_DEFINE_CONST_FUN_OBJ_0(gyro_get_y_obj, get_y);

STATIC mp_obj_t get_z(void) {
	float data[3];
    	BSP_GYRO_GetXYZ(data);

    	return mp_obj_new_float(data[2]);
}
STATIC MP_DEFINE_CONST_FUN_OBJ_0(gyro_get_z_obj, get_z);

STATIC const mp_rom_map_elem_t gyro_module_globals_table[] = {
	{ MP_ROM_QSTR(MP_QSTR___name__), MP_ROM_QSTR(MP_QSTR_gyro) },
	{ MP_ROM_QSTR(MP_QSTR_init), MP_ROM_PTR(&gyro_init_obj) },
	{ MP_ROM_QSTR(MP_QSTR_read_id), MP_ROM_PTR(&gyro_read_id_obj) },
	{ MP_ROM_QSTR(MP_QSTR_get_x), MP_ROM_PTR(&gyro_get_x_obj) },
	{ MP_ROM_QSTR(MP_QSTR_get_y), MP_ROM_PTR(&gyro_get_y_obj) },
	{ MP_ROM_QSTR(MP_QSTR_get_z), MP_ROM_PTR(&gyro_get_z_obj) },
};
STATIC MP_DEFINE_CONST_DICT(gyro_module_globals, gyro_module_globals_table);

const mp_obj_module_t gyro_cmodule = {
	.base = { &mp_type_module },
	.globals = (mp_obj_dict_t*)&gyro_module_globals,
};

//#if MODULE_GYRO_ENABLED
MP_REGISTER_MODULE(MP_QSTR_gyro, gyro_cmodule);
//#endif
