/*
 * zeno_lcd_config.h
 *
 * Hardware configuration for the Zeno OS ILI9488 8-bit parallel display and
 * 4-wire resistive touch driver.
 *
 * SOURCE OF EVERY VALUE BELOW: the user's working sdkconfig (ESP-IDF 5.5.2,
 * Nopnop esp-idf-parallel-tft, CONFIG_INTERFACE_REG=y, CONFIG_ILI9488=y).
 * The matching CONFIG_* symbol is quoted next to each define.
 * Nothing here was invented. Items that are NOT in the sdkconfig are marked
 * "NOT FROM SDKCONFIG".
 */
#ifndef ZENO_LCD_CONFIG_H
#define ZENO_LCD_CONFIG_H

/* ---- LCD bus pins (all must be < 32: the fast path uses GPIO_OUT_W1TS/W1TC
 *      bank 0 only; enforced by _Static_assert in lcd_bus.c) ---------------- */
#define ZLCD_PIN_D0     8   /* CONFIG_D0_GPIO=8   */
#define ZLCD_PIN_D1    15   /* CONFIG_D1_GPIO=15  */
#define ZLCD_PIN_D2    11   /* CONFIG_D2_GPIO=11  */
#define ZLCD_PIN_D3    10   /* CONFIG_D3_GPIO=10  */
#define ZLCD_PIN_D4     9   /* CONFIG_D4_GPIO=9   */
#define ZLCD_PIN_D5     4   /* CONFIG_D5_GPIO=4   */
#define ZLCD_PIN_D6    18   /* CONFIG_D6_GPIO=18  */
#define ZLCD_PIN_D7    17   /* CONFIG_D7_GPIO=17  */
#define ZLCD_PIN_RD    41   /* CONFIG_RD_GPIO=41  (held HIGH, only set at init) */
#define ZLCD_PIN_WR    14   /* CONFIG_WR_GPIO=14  */
#define ZLCD_PIN_RS     2   /* CONFIG_RS_GPIO=2   (D/C) */
#define ZLCD_PIN_CS     1   /* CONFIG_CS_GPIO=1   */
#define ZLCD_PIN_RESET 12   /* CONFIG_RESET_GPIO=12 */

/* NOT FROM SDKCONFIG: the Nopnop sdkconfig has no backlight pin. GPIO 38 is
 * carried over unchanged from the existing moclcd module's header comment. */
#define ZLCD_PIN_BL    38

/* ---- Panel geometry (native, i.e. rotation 0) --------------------------- */
#define ZLCD_NATIVE_WIDTH   320   /* CONFIG_WIDTH=320   */
#define ZLCD_NATIVE_HEIGHT  480   /* CONFIG_HEIGHT=480  */
#define ZLCD_OFFSET_X       0     /* CONFIG_OFFSETX=0   */
#define ZLCD_OFFSET_Y       0     /* CONFIG_OFFSETY=0   */
/* "# CONFIG_INVERSION is not set" -> no inversion command at init. */

/* ---- Touch electrode -> GPIO mapping ------------------------------------ */
#define ZTOUCH_GPIO_XP  ZLCD_PIN_D0   /* CONFIG_XP_GPIO_D0=y */
#define ZTOUCH_GPIO_XM  ZLCD_PIN_RS   /* CONFIG_XM_GPIO_RS=y */
#define ZTOUCH_GPIO_YP  ZLCD_PIN_CS   /* CONFIG_YP_GPIO_CS=y */
#define ZTOUCH_GPIO_YM  ZLCD_PIN_D1   /* CONFIG_YM_GPIO_D1=y */
#define ZTOUCH_ADC_CH_YP 0            /* CONFIG_ADC_CHANNEL_YP=0 -> ADC1_CH0 = GPIO1 */
#define ZTOUCH_ADC_CH_XM 1            /* CONFIG_ADC_CHANNEL_XM=1 -> ADC1_CH1 = GPIO2 */

/* Acquisition constants, identical to components/tft_library/lcd_com.h */
#define ZTOUCH_NUM_SAMPLES    2   /* NUM_SAMPLES    */
#define ZTOUCH_COMP_TOLERANCE 2   /* COMP_TOLERANCE */
#define ZTOUCH_AVERAGE_TIME   4   /* AVERAGE_TIME   */

/* ---- Tunables that are NOT in the original (defaults are placeholders that
 *      must be confirmed on hardware, see VALIDATION.md stages A and B) ----- */
/* WR strobe hold, in NOP-loop iterations. The original had no explicit hold:
 * its WR timing came from gpio_set_level() call overhead at -Og. */
#define ZLCD_WR_LOW_NOPS_DEFAULT   6
#define ZLCD_WR_HIGH_NOPS_DEFAULT  2

#define ZTOUCH_DEFAULT_MIN_RAW      100   /* min (s0+s1) per axis to count as pressed */
#define ZTOUCH_DEFAULT_Z_MAX        4095  /* >=4095 disables the Z (pressure) gate    */
#define ZTOUCH_DEFAULT_SETTLE_US    20    /* original had NO explicit settle delay    */
#define ZTOUCH_DEFAULT_DOWN_CONFIRM 2     /* consecutive valid reads to report press  */
#define ZTOUCH_DEFAULT_UP_CONFIRM   3     /* consecutive invalid reads to report release */

#endif
