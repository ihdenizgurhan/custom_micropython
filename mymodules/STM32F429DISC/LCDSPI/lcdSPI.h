#ifndef __LCDSPI_H
#define __LCDSPI_H

#define LCD_CS_PORT		GPIOC
#define LCD_CS_PIN		GPIO_PIN_2

#define LCD_DC_PORT		GPIOD
#define LCD_DC_PIN		GPIO_PIN_13

// LCD Constants

#define BURST_MAX_SIZE 	65535

// Colors
#define BLACK       0x0000
#define NAVY        0x000F
#define DARKGREEN   0x03E0
#define DARKCYAN    0x03EF
#define MAROON      0x7800
#define PURPLE      0x780F
#define OLIVE       0x7BE0
#define LIGHTGREY   0xC618
#define DARKGREY    0x7BEF
#define BLUE        0x001F
#define GREEN       0x07E0
#define CYAN        0x07FF
#define RED         0xF800
#define MAGENTA     0xF81F
#define YELLOW      0xFFE0
#define WHITE       0xFFFF
#define ORANGE      0xFD20
#define GREENYELLOW 0xAFE5
#define PINK        0xF81F

// Rotation
#define SCREEN_VERTICAL			0
#define SCREEN_HORIZONTAL		1
#define SCREEN_VERTICAL_INV		2
#define SCREEN_HORIZONTAL_INV	3

#endif
