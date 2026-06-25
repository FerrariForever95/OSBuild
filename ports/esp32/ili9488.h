/**
 * @file ili9488_new.h
 * @brief Minimal ILI9488 driver — ESP32-S3, ESP-IDF 5.5
 *        Pure GPIO bit-bang, 8080 8-bit bus. Zero esp_lcd dependency.
 *
 * PIN MAP
 *   D0=GPIO16  D1=GPIO15  D2=GPIO11  D3=GPIO10
 *   D4=GPIO9   D5=GPIO8   D6=GPIO18  D7=GPIO17
 *   RS=GPIO13  WR=GPIO14  RST=GPIO12
 *   RD=GPIO47  BL=GPIO38
 *
 * DISPLAY: 480×320, RGB565 (COLMOD=0x55), landscape default (MADCTL=0x48).
 */
#ifndef ILI9488_NEW_H
#define ILI9488_NEW_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stddef.h>
#include "esp_err.h"
#include <stdbool.h>
/* Physical panel dimensions */
#define ILI9488_W  480
#define ILI9488_H  320

/**
 * ili9488_reset() — hardware reset with empirically proven timing.
 *
 * RST=1 → 250 ms → RST=0 → 250 ms → RST=1 → 250 ms
 * BL is held LOW throughout. The panel module needs ~250 ms after
 * reset release for its gate/source drivers to stabilise. The ILI9488
 * IC itself only needs 5 ms, but the panel hardware needs more — short
 * delays produce working register reads but a white (blank) screen.
 */
void ili9488_reset(void);

/**
 * ili9488_init() — GPIO config, reset, known-good init sequence, BL on.
 *
 * Sequence (exact copy of confirmed-working Python bring-up):
 *   GPIO init → reset → SWRESET(150ms) → SLPOUT(150ms)
 *   → COLMOD(0x55) → MADCTL(0x48) → DISPON(50ms) → BL ON
 *
 * BL is enabled only after DISPON + 50 ms. Enabling BL during reset
 * causes supply sag that corrupts the init sequence.
 *
 * Returns true on success.
 */
bool ili9488_init(void);

/**
 * ili9488_write_cmd() — send one command byte (RS=0).
 */
void ili9488_write_cmd(uint8_t cmd);

/**
 * ili9488_write_data() — send cmd byte then len data bytes (RS=1 for data).
 * data may be NULL with len=0 to send a bare command (same as write_cmd).
 */
void ili9488_write_data(uint8_t cmd, const uint8_t *data, size_t len);

/**
 * ili9488_read() — read len bytes from register cmd into buf.
 *
 * Read cycle per ILI9488 §7.4:
 *   - D0-D7 switched to GPIO inputs.
 *   - One mandatory dummy RD pulse discarded (all registers).
 *   - Data sampled WHILE RD IS LOW (not after — bus released within tOH=20ns).
 *   - tRHRL ≥ 90 ns enforced between pulses.
 *   - D0-D7 restored to outputs after read.
 *
 * Expected: ili9488_read(0xD3, buf, 4) → {0x00, 0x94, 0x88, 0x88}
 */
esp_err_t ili9488_read(uint8_t cmd, uint8_t *buf, size_t len);

/**
 * ili9488_fill() — flood fill entire 480×320 display with one RGB565 colour.
 *
 * Sets window to full screen, issues RAMWR once, streams all
 * 480×320 = 153 600 pixels as big-endian RGB565 (high byte first).
 *
 * Example: ili9488_fill(0xF800) → solid red screen.
 */
void ili9488_fill(uint16_t color);

#ifdef __cplusplus
}
#endif
#endif /* ILI9488_NEW_H */
