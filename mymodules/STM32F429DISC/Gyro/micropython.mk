GYRO_MOD_DIR := $(USERMOD_DIR)

SRC_USERMOD += $(GYRO_MOD_DIR)/gyro.c
SRC_USERMOD += $(GYRO_MOD_DIR)/../../../STM32CubeF4/Drivers/BSP/STM32F429I-Discovery/stm32f429i_discovery.c
SRC_USERMOD += $(GYRO_MOD_DIR)/../../../STM32CubeF4/Drivers/BSP/STM32F429I-Discovery/stm32f429i_discovery_gyroscope.c
SRC_USERMOD += $(GYRO_MOD_DIR)/../../../STM32CubeF4/Drivers/BSP/Components/l3gd20/l3gd20.c

CFLAGS_USERMOD += -I$(GYRO_MOD_DIR)/../../../STM32CubeF4/Drivers/BSP/STM32F429I-Discovery
CFLAGS_USERMOD += -I$(GYRO_MOD_DIR)/../../../STM32CubeF4/Drivers/BSP/Components/l3gd20
CFLAGS_USERMOD += -I$(GYRO_MOD_DIR)/../../../STM32CubeF4/Drivers/BSP/Components/Common
