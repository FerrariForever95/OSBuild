# Source assessment (Phase One)

Everything below was read directly out of the two supplied artifacts:
`esp-idf-parallel-tft-main.zip` (Nopnop `esp-idf-parallel-tft` repo) and the
supplied `sdkconfig`. Nothing here is inferred from the Nopnop project's
other hardware profiles (the repo supports ~20 controllers; only the
ILI9488 + register-mode + custom-GPIO path that the sdkconfig actually
selects was inspected in depth).

## 1. ESP-IDF version and target
`sdkconfig`: `CONFIG_IDF_TARGET_ESP32S3=y`, `CONFIG_IDF_INIT_VERSION="5.5.2"`.
Confirmed against a fresh clone of `espressif/esp-idf` tag `v5.5.2`.

## 2. Interface mode actually built
`CONFIG_INTERFACE_REG=y` (register-mode bit-banged 8080, *not*
`CONFIG_INTERFACE_GPIO` and *not* `CONFIG_INTERFACE_I2S`/LCD_CAM+DMA). This
means the "working" driver the user wants ported is `lcd_com.c`'s
`reg_lcd_write_data()` path -- direct `GPIO.out_w1ts`/`out_w1tc` register
writes per bus cycle, no DMA, no `esp_lcd` component, no `LCD_CAM`
peripheral at all. (This supersedes an earlier, unrelated conversation in
this chat about an `LCD_CAM`+GDMA rewrite of a *different* ILI9488 project
-- that project is not the one in this zip, and this port does not reuse
any of that code.)

## 3. Controller and panel
`CONFIG_ILI9488=y`. Driver file: `components/tft_library/ili9488.c`,
`ili9488.h`. `ili9488_lcdInit()` calls `lcd_write_table()` once with
`ili9488_regValues[]` (shared command helper from `ili9341.c`, which
`ili9488.c` `#include`s to reuse `ili9341_lcdDrawPixel`,
`ili9341_lcdDrawMultiPixels`, `ili9341_lcdDrawFillRect`,
`ili9341_lcdDisplayOn/Off`, `ili9341_lcdInversionOn/Off`).

## 4. Pin mapping (from `sdkconfig`, `CONFIG_CUSTOM_GPIO_CONFIG=y`)
| Signal | GPIO | sdkconfig symbol |
|---|---|---|
| D0 | 8  | `CONFIG_D0_GPIO` |
| D1 | 15 | `CONFIG_D1_GPIO` |
| D2 | 11 | `CONFIG_D2_GPIO` |
| D3 | 10 | `CONFIG_D3_GPIO` |
| D4 | 9  | `CONFIG_D4_GPIO` |
| D5 | 4  | `CONFIG_D5_GPIO` |
| D6 | 18 | `CONFIG_D6_GPIO` |
| D7 | 17 | `CONFIG_D7_GPIO` |
| RD | 41 | `CONFIG_RD_GPIO` |
| WR | 14 | `CONFIG_WR_GPIO` |
| RS/DC | 2 | `CONFIG_RS_GPIO` |
| CS | 1 | `CONFIG_CS_GPIO` |
| RESET | 12 | `CONFIG_RESET_GPIO` |

`CONFIG_WIDTH=320`, `CONFIG_HEIGHT=480`, `CONFIG_OFFSETX=0`,
`CONFIG_OFFSETY=0`, `# CONFIG_INVERSION is not set`.

These are carried unchanged into `zeno_lcd_config.h`.

## 5. Touch electrode mapping (from `sdkconfig` + `4-line_resistance_touch_screen.txt`)
`CONFIG_ENABLE_TOUCH=y`, electrode choice bits:
`CONFIG_XP_GPIO_D0=y`, `CONFIG_XM_GPIO_RS=y`, `CONFIG_YP_GPIO_CS=y`,
`CONFIG_YM_GPIO_D1=y`. ADC channels: `CONFIG_ADC_CHANNEL_YP=0`,
`CONFIG_ADC_CHANNEL_XM=1`.

Checked against `components/soc/esp32s3/include/soc/adc_channel.h`:
ADC1 channel 0 = GPIO1, ADC1 channel 1 = GPIO2. So
`CONFIG_ADC_CHANNEL_YP=0` is GPIO1, and GPIO1 is `CONFIG_CS_GPIO` -- i.e.
the YP electrode's ADC tap is on the **CS** pin, matching
`CONFIG_YP_GPIO_CS=y` exactly. Likewise `CONFIG_ADC_CHANNEL_XM=1` is
GPIO2, which is `CONFIG_RS_GPIO`, matching `CONFIG_XM_GPIO_RS=y` exactly.
This is also exactly what the original `touch_getx()`/`touch_gety()`
algorithm needs: `touch_getx()` drives XP/XM as outputs and samples the
**YP** electrode; `touch_gety()` drives YP/YM as outputs and samples the
**XM** electrode. XP (D0/GPIO8) and YM (D1/GPIO15) are only ever driven as
digital outputs in this algorithm and are never read by an ADC, so they
have no ADC channel configured -- correctly.

One inconsistency worth flagging rather than silently resolving: the task
brief's own prose (section 4) lists "LCD D0 / XP -> ESP32-S3 GPIO8" as one
of the "additional ADC connections," alongside "LCD RS / XM -> GPIO2."
The second of those matches the sdkconfig (GPIO2 = ADC1 ch1 = XM, as
above); the first does not -- GPIO8 is ADC1 **channel 7**, not channel 0
or 1, and the sdkconfig only configures channels 0 and 1 (for YP/CS and
XM/RS). Nothing in the sdkconfig or the original driver's algorithm ever
samples an ADC channel on GPIO8/D0/XP. This port follows the sdkconfig and
the original algorithm (the two declared sources of truth) rather than
the brief's prose list, and treats D0/GPIO8 as digital-drive-only for
touch, matching `touch_gpio()`'s actual usage in `lcd_com.c`. If GPIO8
genuinely carries a second, separate ADC tap on real hardware that the
sdkconfig doesn't reflect, that needs to come from the user directly --
it is not something to guess at.

## 6. Touch acquisition algorithm
`components/tft_library/lcd_com.c`:
- `touch_gpio(pin, out)`: `gpio_reset_pin()` + `gpio_config()` the pin as
  either a digital output (set to the requested level) or input.
- `touch_avr_analog(ch)`: reads `AVERAGE_TIME` (4) raw ADC samples, drops
  the min and the max, averages the remaining 2 (`touch_filter_s`-style
  trimmed mean). Requires `NUM_SAMPLES=2` >= 3 to be meaningful; with
  `AVERAGE_TIME=4` it drops 2 of 4.
- `touch_getx()`/`touch_gety()`: take `NUM_SAMPLES` (2) independent calls
  to `touch_avr_analog()`, and only accept the reading if the two calls
  agree within `COMP_TOLERANCE` (2) counts, else return 0 (treated as "no
  touch" / unstable).
- `touch_getz()`: cross-switches the electrodes once more and reads the
  absolute difference between the two ADC channels as a pressure proxy.
- `touch_getxyz()`: calls the above three in sequence, each time
  re-running `touch_gpio()` on all four electrode pins from scratch.

Ported into `touch_drv.c`'s `zl_touch_read_raw()`, same sequence and same
constants (`zeno_lcd_config.h`: `ZTOUCH_NUM_SAMPLES`, `ZTOUCH_COMP_TOLERANCE`,
`ZTOUCH_AVERAGE_TIME`), same electrode-to-ADC wiring per phase
(X: drive XP high/XM low, sample YP's channel; Y: drive YP high/YM low,
sample XM's channel; Z: YP input/YM high/XP low/XM input, z=|ADC(YP)-ADC(XM)|
-- confirmed line-for-line against `touch_getx()`/`touch_gety()`/
`touch_getz()` in `lcd_com.c`), but pins are reconfigured once per
axis-switch (3 times per read) rather than reset-from-scratch on every
individual ADC sample, and the two ADC channels are configured once at
`touch.init()` instead of on every read -- see `touch_drv.h`'s header
comment for why. One deliberate behavior change: `touch_getxyz()` leaves
all four electrode pins as GPIO outputs driven LOW when it's done;
`zl_touch_read_raw()` leaves them as GPIO outputs driven HIGH, matching
the idle-HIGH convention `lcd_bus_init()` uses for CS/RS/data lines
between bus transactions (two of the four touch electrodes -- YP and XM
-- are the CS and RS pins). This doesn't change behavior (every bus
transaction drives CS/RS explicitly at its start regardless of the prior
idle level) but keeps the pins in a single consistent idle state.

`touch_getz()`'s caller, `touch_getxy()`, rejects a reading if
`z == 1023` exactly. `ADC_BITWIDTH_DEFAULT` on ESP32-S3 is 12-bit
(0-4095) -- confirmed in `adc_oneshot_chan_cfg_t` in both the original's
`touch_interface_cfg()` and the real `esp_adc/adc_oneshot.h`. `1023` is a
10-bit-ADC "max value" constant that does not correspond to anything
special on a 12-bit reading; it looks like a leftover from an older
(classic ESP32, 10/12-bit-switchable ADC) version of this driver and, as
written, essentially never fires on this hardware. This port replaces it
with an explicit, configurable `z_max` gate (`zt_sample_valid()` in
`zeno_pure.h`) that defaults to **disabled**
(`ZTOUCH_DEFAULT_Z_MAX = 4095`, i.e. "accept any Z") rather than
reproducing a magic number that was not actually doing anything -- see
`docs/CALIBRATION.md` and Stage B of `VALIDATION.md` for how to tune or
re-enable a real pressure gate on actual hardware if desired.

## 7. Existing calibration
`main/main.c`, functions `TouchCalibration()` and `ConvertCoordinate()`
(exact names as found; search the zip for these symbols to verify). The
existing scheme samples raw min/max X and raw min/max Y at the screen's
logical extremes and linearly rescales each axis independently:
`screen_x = (raw_x - raw_x_min) * width / (raw_x_max - raw_x_min)` (and the
equivalent for Y), i.e. a 2-point, axis-separable affine fit with the
off-diagonal (skew/rotation) terms fixed at zero. This cannot correct any
error that isn't purely "wrong scale + wrong offset, independently per
axis" -- in particular it cannot correct any skew between the touch
overlay's axes and the LCD's axes, which is a common source of exactly the
kind of small, position-dependent error described in the task ("functional
but has a small positional error"). See `docs/CALIBRATION.md` for the
replacement.

## 8. Rotation / MADCTL
`ili9488_regValues[]` in `ili9488.c` hard-codes MADCTL (`0x36`) to `0x98`
(`MY|ML|BGR`), producing portrait 320x480 with Nopnop's panel orientation.
`main.c`'s app_main calls into the TFT task; rotation beyond the single
hard-coded MADCTL value, if present in `main.c`, was not found as a
runtime-configurable path exercised by the default build -- the demo is
effectively fixed-orientation. `zl_panel_init(rotation, madctl_override)`
adds the 3 other orientations as *derived* (not hardware-verified) MADCTL
values; see the comment above `zl_madctl_table[]` in `zeno_pure.h`, and
Stage A of `VALIDATION.md` for how to confirm/replace them on real
hardware.

## 9. FreeRTOS/task/interrupt dependencies
No FreeRTOS tasks, queues, or interrupts are used by the drawing or touch
code itself -- `lcd_com.c`/`ili9341.c`/`ili9488.c` are called synchronously
from whatever task calls them (`main.c`'s `TFT` task in the demo). The only
synchronization concern is that D0/D1/RS/CS are shared between the LCD bus
and the touch electrodes, and the original demo only ever touches both from
the single `TFT` task, so it has no actual lock. Zeno OS's MicroPython
module can be called from more than one Python thread
(`_thread`)/ISR-adjacent context, so this port adds one (`lcd_bus.c`'s
`zl_bus_lock()`/`zl_bus_unlock()`, a FreeRTOS mutex) that the original did
not need. `nvs_flash` is used only by the new calibration-persistence code
(`touch_calib.c`), not by the original.

## 10. Build configuration beyond pins
`CONFIG_COMPILER_OPTIMIZATION_DEBUG=y` (`-Og`), `CONFIG_SPIRAM=y`/octal
PSRAM at 40 MHz, `CONFIG_ESP_DEFAULT_CPU_FREQ_MHZ=240`. None of these
affect the port's correctness, but the WR-strobe timing in the original
bus code has no explicit delay between setting the data bus and toggling
WR -- its real-world timing margin came entirely from `-Og` function-call
overhead around each `gpio_set_level()`. This port makes that margin an
explicit, tunable NOP count (`lcd_bus.c`, `ZLCD_WR_LOW_NOPS_DEFAULT`/
`ZLCD_WR_HIGH_NOPS_DEFAULT`) instead of relying on compiler-flag-dependent
call overhead, because a straight register-write version is fast enough
that the ILI9488's WR pulse-width minimum (fast enough to violate it) is a
real risk otherwise. See `docs/VALIDATION.md` Stage A for how to tune it on
real hardware.

## Missing information (explicitly not guessed)
- The exact runtime rotation/offset logic in `main.c` beyond the one
  hard-coded MADCTL value in `ili9488.c` -- if the Nopnop demo does expose
  a runtime rotation switch in `main.c` via some other mechanism, it was
  not identified in the supplied files as code this port needed to
  reproduce (the task brief's own illustrative API takes a `rotation=`
  argument, so `lcd.init(rotation=...)`/`lcd.set_rotation()` are provided
  regardless, built on the 4 MADCTL values above).
- The Zeno OS source tree itself was not supplied. This port is delivered
  as a self-contained `USER_C_MODULES` component (see
  `docs/INTEGRATION.md`) rather than wired into any specific Zeno OS
  display backend, because no such backend file was provided to adapt to.
