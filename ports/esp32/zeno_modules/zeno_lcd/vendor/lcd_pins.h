#pragma once
/* ---- YOUR PIN SETUP: edit only this file. GPIO numbers, compile-time constants. ----
 * 8-bit parallel bus + control lines (touch shares D0 / D1 / RS / CS: XP=D0, YM=D1, XM=RS, YP=CS).
 * RS and CS must be on ADC1 (GPIO1..10 on the S3) because they are read by the touch ADC. */
#define LCD_PIN_D0   16
#define LCD_PIN_D1   15
#define LCD_PIN_D2   11
#define LCD_PIN_D3   10
#define LCD_PIN_D4   9
#define LCD_PIN_D5   4
#define LCD_PIN_D6   18
#define LCD_PIN_D7   17
#define LCD_PIN_WR   14
#define LCD_PIN_RD   41
#define LCD_PIN_RS   13
#define LCD_PIN_CS   1
#define LCD_PIN_RST  12

#if LCD_PIN_D0 < 0 || LCD_PIN_D1 < 0 || LCD_PIN_D2 < 0 || LCD_PIN_D3 < 0 || LCD_PIN_D4 < 0 || \
    LCD_PIN_D5 < 0 || LCD_PIN_D6 < 0 || LCD_PIN_D7 < 0 || LCD_PIN_WR < 0 || LCD_PIN_RD < 0 || \
    LCD_PIN_RS < 0 || LCD_PIN_CS < 0 || LCD_PIN_RST < 0
#error "Fill in your GPIO numbers in zeno_lcd/vendor/lcd_pins.h"
#endif
