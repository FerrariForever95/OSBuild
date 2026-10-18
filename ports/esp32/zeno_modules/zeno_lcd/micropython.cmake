# micropython.cmake -- wires the zeno_lcd user C module into the MicroPython
# ESP32 port's cmake build.
#
# USAGE (see docs/INTEGRATION.md for the full walkthrough):
#   idf.py -D MICROPY_BOARD=<your_board> \
#           -D USER_C_MODULES=/absolute/path/to/zeno_lcd/micropython.cmake \
#           build
# Or add that same path to a list in your mpconfigboard-level
# USER_C_MODULES setting if your Zeno OS build already sets one for other
# modules (USER_C_MODULES accepts a ;-separated list of such files).
#
# This file follows exactly the pattern of
# micropython/examples/usercmodule/cexample/micropython.cmake.

add_library(usermod_zeno_lcd INTERFACE)

target_sources(usermod_zeno_lcd INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}/lcd_bus.c
    ${CMAKE_CURRENT_LIST_DIR}/ili9488_panel.c
    ${CMAKE_CURRENT_LIST_DIR}/touch_drv.c
    ${CMAKE_CURRENT_LIST_DIR}/touch_calib.c
    ${CMAKE_CURRENT_LIST_DIR}/modzlcd.c
    ${CMAKE_CURRENT_LIST_DIR}/modztouch.c
)

target_include_directories(usermod_zeno_lcd INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}
)

# ESP-IDF components this module's sources call into directly:
#   driver      -- gpio.h
#   esp_adc     -- esp_adc/adc_oneshot.h
#   nvs_flash   -- nvs.h / nvs_flash.h (calibration persistence)
#   esp_rom     -- esp_rom_sys.h (esp_rom_delay_us)
# All four are already linked into the ESP-IDF MicroPython build's main
# component; this just makes the dependency explicit for anyone reusing
# zeno_lcd in a different IDF component.
idf_component_get_property(zeno_lcd_driver_lib driver COMPONENT_LIB)
idf_component_get_property(zeno_lcd_adc_lib esp_adc COMPONENT_LIB)
idf_component_get_property(zeno_lcd_nvs_lib nvs_flash COMPONENT_LIB)
if(zeno_lcd_driver_lib)
    target_link_libraries(usermod_zeno_lcd INTERFACE ${zeno_lcd_driver_lib})
endif()
if(zeno_lcd_adc_lib)
    target_link_libraries(usermod_zeno_lcd INTERFACE ${zeno_lcd_adc_lib})
endif()
if(zeno_lcd_nvs_lib)
    target_link_libraries(usermod_zeno_lcd INTERFACE ${zeno_lcd_nvs_lib})
endif()

target_link_libraries(usermod INTERFACE usermod_zeno_lcd)
