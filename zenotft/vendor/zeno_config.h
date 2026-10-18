#pragma once
/* Replaces the Kconfig menu of the original project. Included first by every vendored .c file. */
#include "zeno_pins.h"
#define CONFIG_ILI9488 1
#define CONFIG_INTERFACE_REG 1
#define CONFIG_INTERFACE_GPIO 0
#define CONFIG_INTERFACE_I2S 0
#define CONFIG_CUSTOM_GPIO_CONFIG 1
#define CONFIG_D0_GPIO zeno_pins.d0
#define CONFIG_D1_GPIO zeno_pins.d1
#define CONFIG_D2_GPIO zeno_pins.d2
#define CONFIG_D3_GPIO zeno_pins.d3
#define CONFIG_D4_GPIO zeno_pins.d4
#define CONFIG_D5_GPIO zeno_pins.d5
#define CONFIG_D6_GPIO zeno_pins.d6
#define CONFIG_D7_GPIO zeno_pins.d7
#define CONFIG_WR_GPIO zeno_pins.wr
#define CONFIG_RD_GPIO zeno_pins.rd
#define CONFIG_RS_GPIO zeno_pins.rs
#define CONFIG_CS_GPIO zeno_pins.cs
#define CONFIG_RESET_GPIO zeno_pins.rst
#define CONFIG_JD_USE_ROM 1
