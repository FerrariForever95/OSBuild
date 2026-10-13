# Staged build, flash, and validation guide

None of the stages below have been run on real hardware (none was
available while writing this port -- see `docs/LIMITATIONS.md`). This is
the test plan to run them yourself, in order, with what each result
means.

## Stage 0 -- build

```
idf.py -D MICROPY_BOARD=<your_board> \
       -D USER_C_MODULES=/absolute/path/to/zeno_lcd/micropython.cmake \
       build
```
Expected: clean build. If it fails on a `LCD_CAM.*`/`REG_WRITE`/
`gpio_config_t` field name, see `docs/LIMITATIONS.md` -- every symbol used
was checked against a real ESP-IDF 5.5.2 checkout in this environment
(`soc/soc.h`'s `REG_WRITE`, `soc/gpio_reg.h`'s `GPIO_OUT_W1TS_REG`/
`GPIO_OUT_W1TC_REG`, `driver/gpio.h`'s `gpio_config_t`, `esp_adc/
adc_oneshot.h`'s types, `nvs.h`'s `nvs_handle_t` family, MicroPython's
`py/obj.h`/`py/runtime.h` macros) but could not be linked against the
real `xtensa-esp32s3-elf` toolchain in this environment (no network
access to Espressif's toolchain distribution and no toolchain
preinstalled) -- `idf.py build` here is the first time this code is
actually compiled and linked end-to-end.

## Stage A -- display hardware

Run `zeno_lcd_test.run()`.
- **Corner markers** (`_corner_markers`): confirms all 4 corners are
  addressable and the yellow border traces the full frame with no
  off-by-one gap/overlap at the edges, in all 4 rotations.
- **Color bars**: confirms RGB565 packing (`lcd_bus.c`'s
  `zl_bus_write_color()`) produces the right hues, not a byte-swapped or
  bit-shifted result.
- **Lines/shapes**: confirms `zl_draw_line()`'s horizontal/vertical fast
  path (via `zl_fill_rect()`) and diagonal Bresenham path agree visually
  at their shared endpoints, and that `zl_draw_circle()`/
  `zl_fill_circle()` are round, not egg-shaped (an egg shape usually means
  width/height are swapped somewhere).
- **Pixel scan**: a sparse dot grid -- any missing/misplaced dot points at
  a specific stuck or miswired data bit.
- **Blit**: a small gradient square -- confirms `lcd.blit()`'s raw-buffer
  path independently of the fill-color path.
- **Invert**: a brief full-screen inversion, confirming `0x20`/`0x21`
  reach the panel.

If image tears, has sparkling pixels, or individual bytes look corrupted:
suspect the WR strobe timing. Call `lcd.bus_timing(low, high)` with larger
values (defaults: 6, 2 NOP cycles) and re-run. `lcd.bus_timing()` with no
arguments returns the current `(low, high)` without changing them.

If any rotation other than 0 looks mirrored, has wrong aspect ratio, or
scrolled: that MADCTL value (`zl_madctl_table[]` in `zeno_pure.h`) is
wrong for your physical panel -- only rotation 0 (`0x98`) came from the
original driver; the other three are derived, not verified (see
`docs/ASSESSMENT.md` section 8). Compute the correct ILI9488 MADCTL byte
for your desired orientation from the panel datasheet and either edit
`zl_madctl_table[]` or always pass an explicit `madctl=` to
`lcd.init()`/`lcd.set_rotation()`.

## Stage B -- touch hardware

Run `zeno_touch_test.run_raw()`. No display needed.
- Rest a finger off the panel: expect `touch DOWN`/`UP` transitions to
  stay mostly silent (no DOWN events) between taps.
- Touch and hold: expect one `DOWN` line, then periodic `rx=`/`ry=`/`z=`
  lines while held, then one `UP` line on release.
- Confirm `rx`/`ry` move in the expected direction as you drag a finger
  across each axis (if an axis is inverted, that's a wiring fact to
  record for calibration, not a bug -- the affine fit corrects it).
- If `DOWN` never fires: lower `ZTOUCH_DEFAULT_MIN_RAW` in
  `zeno_lcd_config.h`, or call `touch.raw()` directly and watch `valid`
  alongside the raw numbers to see which gate (`rx`/`ry` too low, or the
  dual-sample-disagreement check) is rejecting the reading.
- If `DOWN` fires spuriously with no touch: raise `ZTOUCH_DEFAULT_MIN_RAW`
  and/or `ZTOUCH_DEFAULT_DOWN_CONFIRM`.

This also exercises the shared-pin switching: run it immediately after
`zeno_lcd_test.run()` (or interleave `lcd.fill_rect(...)` calls from the
REPL with touch reads) to confirm the display doesn't glitch, freeze, or
show stray pixels while touch is reading, and that drawing resumes
cleanly afterward. That's the direct test of `zl_bus_lock()` doing its
job.

## Stage C -- calibration

See `docs/CALIBRATION.md`. Run `zeno_calibrate.run()`, note the returned
`(rms_px, max_err_px)`, then run `zeno_touch_test.run()` and touch several
points you did *not* calibrate on (not just the 5 targets) -- the visual
gap between your fingertip and the drawn dot is your real-world accuracy
number. Repeat near each corner and the center; report whichever is worst.

## Stage D -- MicroPython API

`import zeno_minimal_test` exercises `lcd.init`, `touch.init`, `lcd.fill`,
`lcd.rect`, `lcd.fill_rect`, `lcd.circle`, `lcd.pixel`, `lcd.width/height/
rotation`, `touch.calib_active`, `touch.raw` in one pass and prints a
one-line `OK` at the end if nothing raised.

Additional manual checks:
- Call `lcd.init()` twice in a row: second call should re-run the full
  reset/init sequence cleanly, not crash or hang (it re-enters
  `zl_bus_init()`/`zl_panel_init()`, both idempotent).
- Call a drawing function (`lcd.pixel(0,0,0)`) **before** `lcd.init()`:
  expect a clean `OSError: lcd.init() must be called first`, not a crash.
- Call `lcd.blit(0,0,10,10,b'short')`: expect `ValueError: buffer size
  must be w*h*2`, not a wild write.
- Call `lcd.fill_rect(-5,-5,10,10,0)`: `fill_rect`/`blit` are strict and
  should raise `ValueError: ... out of bounds`; by contrast `lcd.pixel()`/
  `lcd.line()`/`lcd.circle()`/`lcd.rect()` clip silently (see each
  function's docstring/comment) -- confirm a partially-off-screen circle
  or line draws its on-screen portion without raising.

## Stage E -- Zeno OS integration

Not independently testable here (no Zeno OS source tree was supplied --
see `docs/LIMITATIONS.md`). Once wired into whatever Zeno OS calls at
startup to bring up its display backend:
- Confirm Zeno OS's existing UI renders correctly through `lcd.*` calls.
- Confirm touch input reaches Zeno OS's existing input handling via
  `touch.read()`.
- Confirm the filesystem, other applications, and the rest of the boot
  process are unaffected (this port never touches boot code, so this
  should be a non-event, but it's worth confirming once end-to-end).
- Measure rendering throughput for whatever Zeno OS's heaviest screen is
  (full-screen redraw time via `lcd.blit()` is the relevant number);
  compare against the synchronous-per-chunk fill path described in
  `lcd_bus.c`'s header comment if it's not fast enough, and consider the
  async/IRQ-driven pipeline extension mentioned there.
