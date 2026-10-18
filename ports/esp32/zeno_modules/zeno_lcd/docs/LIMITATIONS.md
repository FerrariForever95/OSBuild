# Limitations, unresolved assumptions, and missing inputs

## Not built or run on real hardware
No ESP32-S3 device and no `xtensa-esp32s3-elf` toolchain were available in
the environment this was written in (network access was restricted to a
fixed domain allowlist that does not include Espressif's toolchain
distribution). What *was* done instead, concretely:
- Every ESP-IDF header path, struct field name, macro, and function
  signature this code uses was checked against a real, freshly cloned
  `espressif/esp-idf` tag `v5.5.2` tree (the exact version in the supplied
  `sdkconfig`) -- including `soc/soc.h` (`REG_WRITE`), `soc/gpio_reg.h`
  (`GPIO_OUT_W1TS_REG`/`GPIO_OUT_W1TC_REG`), `driver/gpio.h`
  (`gpio_config_t`'s actual field set), `esp_adc/adc_oneshot.h`'s types
  and functions, `nvs.h`'s `nvs_handle_t`/`nvs_open`/`nvs_set_blob`/
  `nvs_get_blob`, and `soc/adc_channel.h`'s GPIO-to-ADC-channel map (used
  to verify the touch ADC wiring claims in `docs/ASSESSMENT.md`).
- Every MicroPython macro/function used (`MP_DEFINE_CONST_FUN_OBJ_*`,
  `mp_arg_parse_all`, `MP_ARG_*`, `mp_get_buffer_raise`,
  `mp_hal_delay_ms`, `MP_REGISTER_MODULE`) was checked against a real
  `micropython` repo clone, and the `USER_C_MODULES`/`micropython.cmake`
  integration mechanism described in `docs/INTEGRATION.md` was confirmed
  against the actual `py/usermod.cmake` and
  `examples/usercmodule/cexample/micropython.cmake` in that repo, not
  assumed from memory.
- `tests/host_test_pure.c` actually compiles and runs (`cc -std=c11 ... &&
  ./host_test_pure` -> `ALL TESTS PASSED`) against every hardware-
  independent function in `zeno_pure.h`: the byte->GPIO-mask lookup table
  builder, the trimmed-mean and dual-sample-agreement touch filters, the
  debounce state machine, and the affine calibration solver (identity
  case, known offset+scale case, degenerate/collinear rejection,
  out-of-bounds clamping).
- A `-fsyntax-only` compile of the ESP-IDF-facing files was attempted
  against the real IDF 5.5.2 headers plus a `sdkconfig.h` mechanically
  generated from the supplied `sdkconfig` and the real
  `FreeRTOSConfig.h`. It got as far as `xtensa_config.h` requiring
  `sys/reent.h`, a newlib header that ships inside the
  `xtensa-esp32s3-elf` toolchain itself (not in the ESP-IDF source tree),
  which is the point where a real cross-compiler becomes unavoidable.

**`idf.py build` in your actual environment is the first real compile and
link of this code.** Run Stage 0 of `docs/VALIDATION.md` before anything
else.

## Backlight pin not confirmed
`ZLCD_PIN_BL = 38` in `zeno_lcd_config.h` has no corresponding symbol
anywhere in the supplied `sdkconfig` or Nopnop repo -- the original driver
never drives a backlight pin at all. `38` was carried over only from the
pre-existing `moclcd` MicroPython module referenced earlier in this
conversation, as a placeholder. This driver does not currently use it for
anything (no backlight API is exposed); confirm the real pin (or that
backlight is wired directly to a fixed-on rail with no GPIO) before adding
one.

## MADCTL for rotations 1-3 is derived, not verified
Only rotation 0 (`0x98`) is an exact, verified value (hard-coded in the
original `ili9488.c`). The other three entries in `zl_madctl_table[]`
(`zeno_pure.h`) were computed by keeping the ML|BGR bits and varying
MY/MX/MV the way ILI9488's MADCTL register is documented to work, but
were never checked against this specific physical panel. See Stage A of
`VALIDATION.md`.

## WR timing margin is a guess, not a measurement
The original's WR pulse width came entirely from incidental `-Og`
function-call overhead around per-pin `gpio_set_level()` calls -- not a
value anyone chose deliberately. This port's direct-register-write path
is almost certainly *faster* per byte than that, which could violate the
ILI9488's minimum WR pulse width if the NOP counts
(`ZLCD_WR_LOW_NOPS_DEFAULT`/`..._HIGH_NOPS_DEFAULT`, both currently
guesses) are too low. `lcd.bus_timing()` exists specifically so this can
be tuned on real hardware (Stage A) without a rebuild.

## Touch thresholds are defaults, not calibrated-in values
`ZTOUCH_DEFAULT_MIN_RAW`, `..._Z_MAX`, `..._SETTLE_US`,
`..._DOWN_CONFIRM`, `..._UP_CONFIRM` in `zeno_lcd_config.h` are reasonable
starting points, not measurements from this specific touch panel. Expect
to tune `MIN_RAW` and the confirm counts during Stage B.

## The GPIO8/ADC "discrepancy" (see `docs/ASSESSMENT.md` section 5)
The task brief's own prose lists GPIO8 (D0/XP) as an "ADC connection,"
which doesn't match either the sdkconfig (only ADC channels 0 and 1 are
configured, mapping to GPIO1/CS/YP and GPIO2/RS/XM) or the original
algorithm (which never samples XP's electrode). This port follows the
sdkconfig and the original algorithm. If real hardware actually has a
second ADC tap on GPIO8 that the sdkconfig doesn't reflect, that's new
information this port doesn't have and would need from you directly.

## No Zeno OS source tree was supplied
Per the task's own instruction ("produce a self-contained native driver
component and precise integration instructions" when the OS tree isn't
available), this is delivered as a standalone `USER_C_MODULES` component
exposing `lcd`/`touch`, not wired into any existing Zeno OS display
backend, scheduler, or application. `docs/INTEGRATION.md` is the
integration instructions; Stage E of `VALIDATION.md` is what to check
once you do wire it in.

## Concurrency model
`zl_bus_lock()` is a FreeRTOS mutex with a **non-blocking try-lock**
(`xSemaphoreTake(..., 0)`), chosen specifically so a drawing call from one
thread can never deadlock against `mp_hal_delay_ms()` running on another
thread while holding the GIL. The practical effect: if two Python threads
genuinely race to draw/touch at the same instant, the loser gets an
`OSError: lcd bus busy` / a rejected touch read rather than blocking. This
was a design choice appropriate for a single-display, mostly-one-thread-
draws UI; if Zeno OS's architecture needs multiple threads drawing
concurrently and expects them to simply queue rather than fail, that
would need revisiting (e.g. a bounded blocking wait instead of `0`).

## No gamma table
The original `ili9488_regValues[]` has its `0xE0`/`0xE1` (positive/
negative gamma) entries commented out; this port's `reg_after_madctl[]`
matches that (also absent), so default panel gamma is used, same as the
working original.
