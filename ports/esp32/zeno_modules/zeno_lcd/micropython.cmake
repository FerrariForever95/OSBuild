# lcd - MicroPython user C module (ESP32-S3, ESP-IDF 5.x). Only used for the USER_C_MODULES route;
# the esp32_common.cmake route lists the same sources directly.
add_library(usermod_lcd INTERFACE)
set(ZENO_DIR ${CMAKE_CURRENT_LIST_DIR})
set(ZENO_VENDORED
    ${ZENO_DIR}/vendor/lcd_com.c ${ZENO_DIR}/vendor/lcd_lib.c ${ZENO_DIR}/vendor/ili9341.c
    ${ZENO_DIR}/vendor/ili9488.c ${ZENO_DIR}/vendor/fontx.c ${ZENO_DIR}/vendor/pngle.c
    ${ZENO_DIR}/vendor/decode_png.c ${ZENO_DIR}/vendor/decode_jpeg_v5.c)
target_sources(usermod_lcd INTERFACE ${ZENO_DIR}/modzlcd.c ${ZENO_VENDORED})
target_include_directories(usermod_lcd INTERFACE ${ZENO_DIR} ${ZENO_DIR}/vendor)
set_source_files_properties(${ZENO_VENDORED} PROPERTIES COMPILE_OPTIONS "-Wno-error")
target_link_libraries(usermod INTERFACE usermod_lcd)
