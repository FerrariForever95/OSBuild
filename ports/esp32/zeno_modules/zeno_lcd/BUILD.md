# lcd (zeno_lcd) - build and flash

**Status: written against the repo source, NOT compiled or run. Expect to fix a few build errors.**

## 1. Back up, then change the partition table
The new table moves the MicroPython filesystem (`vfs`), so flashing wipes it. Copy your OS files off first (e.g. `mpremote cp -r : ./backup`).
Copy `partitions-16MiB-zeno.csv` to `micropython/ports/esp32/` and add `sdkconfig.zeno` to the board build
(`SDKCONFIG_DEFAULTS` or append to the board sdkconfig). Compare offsets with your current table first
(`esp32.Partition.find()` in the REPL) and adjust `factory` if your firmware is larger than 3 MB.

## 2. Build
```
cd micropython/ports/esp32
make BOARD=ESP32_GENERIC_S3 BOARD_VARIANT=SPIRAM_OCT USER_C_MODULES=/abs/path/zeno_lcd/micropython.cmake
```
(use your own board/variant names; first build after adding the module: `make clean`).
Check `ports/esp32/main/CMakeLists.txt`: the IDF component list must include **spiffs** and **esp_adc** (+ driver, esp_rom, hal, soc, vfs). Add any that are missing.

## 3. Assets (fonts, PNG, JPEG)
Put your PNG/JPEG files in `assets_extra/` (flat, names <= ~20 chars; SPIFFS has no folders), then
`IDF_PATH=... tools/make_assets.sh /dev/ttyACM0` builds and flashes the `assets` partition at 0x310000.
Fonts in `fonts/` are the repo FONTX2 files; check their licenses before shipping.

## 4. First run
See `zeno_test.py`. Order: init -> mount -> font_load -> touch_init -> read_raw (idle vs pressed) -> touch_calibration -> read_touch.

## Notes
- Pins are compile-time constants in `vendor/lcd_pins.h` (build fails until filled in). `lcd.init()` takes only width/height/invert. There is no backlight handling: wire it to 3V3 or drive it yourself.
- Touch needs ADC1 only (GPIO1..10, channel = GPIO-1) and must not coexist with `machine.ADC` on ADC1. If boot aborts with an ADC driver "CONFLICT" message, the legacy and new ADC drivers are both linked: that is the cause.
- JPEG uses the S3 ROM decoder (CONFIG_JD_USE_ROM). If the build says it is unsupported, enable the external tjpgd (esp_jpeg component) instead. Baseline JPEG only.
- Vendored code: MIT, nopnop2002/esp-idf-parallel-tft; keep vendor/LICENSE.nopnop2002-esp-idf-parallel-tft. Only patches: vendor/lcd_com.c, the legacy I2S header include guarded.
