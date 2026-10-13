# Build-system integration

This is delivered as a self-contained **MicroPython `USER_C_MODULES`
component** (see `docs/ASSESSMENT.md`, "Missing information" -- no Zeno OS
source tree was supplied to wire into a specific existing backend). It
does not touch the ESP32 port's own files, boot code, partition table, or
any other driver.

Determined from the actual `micropython` repo (`ports/esp32/`,
`py/usermod.cmake`, `examples/usercmodule/cexample/micropython.cmake`):
the ESP32 port is CMake/`idf.py`-based (not the legacy Make port), and
user C modules are wired in purely via a `-D USER_C_MODULES=<path(s)>`
cmake variable pointing at one or more `micropython.cmake` files, each of
which declares an `INTERFACE` library and links it into the `usermod`
target. No changes to `ports/esp32/main/CMakeLists.txt` or any other
MicroPython file are needed or made.

## Steps

1. Copy (or symlink) this whole `zeno_lcd/` directory somewhere your
   Zeno OS build tree already reaches, e.g.
   `micropython/ports/esp32/zeno_modules/zeno_lcd/`. Nothing inside it
   needs editing for a basic bring-up; `zeno_lcd_config.h` already has
   every value pulled from your sdkconfig (adjust it if you rewire
   anything).

2. Build with:
   ```
   cd micropython/ports/esp32
   idf.py -D MICROPY_BOARD=<your_board> \
          -D USER_C_MODULES=/absolute/path/to/zeno_lcd/micropython.cmake \
          build
   ```
   If Zeno OS already sets `USER_C_MODULES` for other modules, `cmake`
   accepts a `;`-separated list -- append this path rather than
   overwriting the variable.

3. From Python:
   ```python
   import lcd, touch
   lcd.init(rotation=0)
   touch.init()
   ```

## What `micropython.cmake` does (and nothing more)

- Adds `lcd_bus.c`, `ili9488_panel.c`, `touch_drv.c`, `touch_calib.c`,
  `modzlcd.c`, `modztouch.c` to an `INTERFACE` library.
- Adds this directory to the include path.
- Links the `driver` (GPIO), `esp_adc`, and `nvs_flash` ESP-IDF component
  libraries into that `INTERFACE` library via
  `idf_component_get_property(... COMPONENT_LIB)`, the standard way an
  IDF-based `USER_C_MODULES` component picks up component dependencies
  without a full `idf_component_register()` of its own (user C modules
  aren't components; they're folded into MicroPython's own `main`
  component, which already indirectly depends on these, so these
  `target_link_libraries()` calls are a belt-and-suspenders safety net
  more than a strict requirement -- see the comment in the file).
- Finally, `target_link_libraries(usermod INTERFACE usermod_zeno_lcd)` --
  the one line every `examples/usercmodule/*/micropython.cmake` file in
  the MicroPython repo ends with.

## Required ESP-IDF components

All already part of a stock ESP32 MicroPython build: `driver` (GPIO),
`esp_adc`, `nvs_flash`, `esp_rom`, `freertos`. No `idf_component.yml`
additions, no `sdkconfig.defaults` additions. (`nvs_flash_init()` itself
is assumed already called once at boot by the existing MicroPython
firmware -- it normally is, for Wi-Fi's NVS use; see
`docs/LIMITATIONS.md` if that's not the case in your Zeno OS build.)

## What is intentionally NOT changed

- `ports/esp32/main/CMakeLists.txt`, `mpconfigport.h`, boot scripts,
  frozen manifest, partition table, flash size: untouched.
- No `idf.py menuconfig` / `sdkconfig` changes required (this reads its
  pin/geometry constants from its own `zeno_lcd_config.h`, not from
  `sdkconfig.h`, so it doesn't need the *display's* Kconfig options the
  original Nopnop `main/Kconfig.projbuild` defined -- those belonged to
  the standalone demo app, not to a reusable driver).
