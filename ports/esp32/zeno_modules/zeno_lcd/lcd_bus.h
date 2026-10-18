/*
 * lcd_bus.h  --  8-bit parallel "register mode" 8080 bus
 *
 * Port of Nopnop esp-idf-parallel-tft components/tft_library/lcd_com.c
 * (CONFIG_INTERFACE_REG path: reg_lcd_write_data + lcd_write_*).
 *
 * Locking rule: zl_bus_lock()/zl_bus_unlock() guard the shared pins
 * (D0, D1, RS, CS are also the touch electrodes). Every function below that
 * is marked [LOCK] must be called with the lock held. The lock is a
 * NON-BLOCKING try-lock on purpose: under MicroPython a blocking wait while
 * holding the GIL could deadlock against a thread sleeping in mp_hal_delay_ms().
 */
#ifndef ZENO_LCD_BUS_H
#define ZENO_LCD_BUS_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "esp_err.h"

/* Configure pins exactly like lcd_interface_cfg() (CS,RS,WR,RD,D0-D7,RESET),
 * build the byte->GPIO LUT, create the mutex. Idempotent. Does NOT pulse RESET. */
esp_err_t zl_bus_init(void);
bool      zl_bus_is_init(void);

bool zl_bus_lock(void);     /* try-lock; false if busy */
void zl_bus_unlock(void);

void zl_bus_set_timing(uint8_t low_nops, uint8_t high_nops);
void zl_bus_get_timing(uint8_t *low_nops, uint8_t *high_nops);

void zl_bus_reset_level(int level);                       /* RESET pin (not shared) */

/* [LOCK] == lcd_write_comm_byte(): CS low, RS low, 1 byte, CS high */
void zl_bus_write_cmd(uint8_t cmd);
/* [LOCK] == lcd_write_data_byte()/lcd_write_addr(): CS low, RS high, n bytes, CS high */
void zl_bus_write_data(const uint8_t *d, size_t n);
/* [LOCK] one data transaction of npix copies of an RGB565 colour, hi byte first
 *        (replaces the original's malloc()+lcd_write_color() per column). */
void zl_bus_write_color(uint16_t color, uint32_t npix);
/* [LOCK] one data transaction of raw bytes (already in wire order, MSB first). */
void zl_bus_write_bytes(const uint8_t *d, size_t n);
/* [LOCK] restore idle levels CS=1, RS=1, WR=1 (pins must already be outputs). */
void zl_bus_idle(void);

#endif
