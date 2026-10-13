# zeno_lcd -- ILI9488 8-bit parallel display + 4-wire resistive touch for Zeno OS

Native MicroPython C port of the working Nopnop `esp-idf-parallel-tft`
driver (ILI9488, register-mode 8080 bus, ESP32-S3), exposed as two small
Python modules: `lcd` and `touch`.

**Start here:** `docs/ASSESSMENT.md` (what was actually found in your
source files), then `docs/INTEGRATION.md` (how to build it), then
`docs/VALIDATION.md` (how to test it in stages on real hardware -- nothing
in this delivery has been run on real hardware; see
`docs/LIMITATIONS.md`).

## Layout

```
zeno_lcd/
  zeno_lcd_config.h     -- every pin/geometry/touch constant, sourced from your sdkconfig
  zeno_pure.h            -- hardware-independent logic (unit-tested, see tests/)
  lcd_bus.{h,c}          -- register-mode 8080 bus (replaces lcd_com.c's bus half)
  ili9488_panel.{h,c}    -- ILI9488 init table + drawing primitives
  touch_drv.{h,c}        -- 4-wire resistive touch acquisition (replaces lcd_com.c's touch half)
  touch_calib.{h,c}      -- affine calibration + NVS persistence
  modzlcd.c              -- "lcd" MicroPython module
  modztouch.c            -- "touch" MicroPython module
  zeno_mp_err.h           -- esp_err_t -> OSError helper
  micropython.cmake      -- USER_C_MODULES build glue
  py/
    zeno_calibrate.py     -- 5-point calibration helper (Stage C)
    zeno_lcd_test.py       -- display test, all corners/rotations (Stage A/D)
    zeno_touch_test.py     -- touch test, raw + calibrated (Stage B/D)
    zeno_minimal_test.py   -- smallest smoke test (Stage D)
  tests/
    host_test_pure.c      -- runs on a desktop compiler, no ESP-IDF needed
  docs/
    ASSESSMENT.md          -- source-based findings (Phase One)
    FILE_MAPPING.md         -- original file/function -> new file/function
    GPIO_MAPPING.md         -- pin tables, from your sdkconfig
    INTEGRATION.md          -- build-system wiring
    CALIBRATION.md          -- calibration design + usage
    VALIDATION.md           -- staged hardware test plan
    LIMITATIONS.md          -- what's verified vs. assumed, read this
```

## Quick start

```
cd micropython/ports/esp32
idf.py -D MICROPY_BOARD=<your_board> \
       -D USER_C_MODULES=/absolute/path/to/zeno_lcd/micropython.cmake \
       build flash monitor
```
```python
>>> import zeno_minimal_test
lcd: 480 x 320 rotation 0
touch calibrated: False
touch.raw(): (0, 0, 0, False)
zeno_minimal_test: OK
>>> import zeno_calibrate
>>> zeno_calibrate.run()
>>> import zeno_touch_test
>>> zeno_touch_test.run()
```

## Host-side unit tests (no device needed)

```
cd tests
cc -std=c11 -Wall -Wextra -I.. -o host_test_pure host_test_pure.c -lm
./host_test_pure
```
This actually runs in this environment and prints `ALL TESTS PASSED`.
