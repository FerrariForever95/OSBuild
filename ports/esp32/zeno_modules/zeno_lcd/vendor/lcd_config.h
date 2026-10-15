#pragma once
/* Replaces the Kconfig menu of the original project. Included first by every vendored .c file. */
#include "lcd_pins.h"
#define CONFIG_ILI9488 1
#define CONFIG_INTERFACE_REG 1
#define CONFIG_INTERFACE_GPIO 0
#define CONFIG_INTERFACE_I2S 0
#define CONFIG_CUSTOM_GPIO_CONFIG 1
#define CONFIG_D0_GPIO LCD_PIN_D0
#define CONFIG_D1_GPIO LCD_PIN_D1
#define CONFIG_D2_GPIO LCD_PIN_D2
#define CONFIG_D3_GPIO LCD_PIN_D3
#define CONFIG_D4_GPIO LCD_PIN_D4
#define CONFIG_D5_GPIO LCD_PIN_D5
#define CONFIG_D6_GPIO LCD_PIN_D6
#define CONFIG_D7_GPIO LCD_PIN_D7
#define CONFIG_WR_GPIO LCD_PIN_WR
#define CONFIG_RD_GPIO LCD_PIN_RD
#define CONFIG_RS_GPIO LCD_PIN_RS
#define CONFIG_CS_GPIO LCD_PIN_CS
#define CONFIG_RESET_GPIO LCD_PIN_RST
#define CONFIG_JD_USE_ROM 1
