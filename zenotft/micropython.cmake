# zenotft - MicroPython user C module (ESP32-S3, ESP-IDF 5.x)
# Build:  make USER_C_MODULES=/abs/path/to/zenotft/micropython.cmake   (in micropython/ports/esp32)

add_library(usermod_zenotft INTERFACE)

set(ZENO_DIR ${CMAKE_CURRENT_LIST_DIR})

set(ZENO_VENDORED
    ${ZENO_DIR}/vendor/lcd_com.c
    ${ZENO_DIR}/vendor/lcd_lib.c
    ${ZENO_DIR}/vendor/ili9341.c
    ${ZENO_DIR}/vendor/ili9488.c
    ${ZENO_DIR}/vendor/fontx.c
    ${ZENO_DIR}/vendor/pngle.c
    ${ZENO_DIR}/vendor/decode_png.c
    ${ZENO_DIR}/vendor/decode_jpeg_v5.c
)

target_sources(usermod_zenotft INTERFACE
    ${ZENO_DIR}/modzenotft.c
    ${ZENO_VENDORED}
)

target_include_directories(usermod_zenotft INTERFACE
    ${ZENO_DIR}
    ${ZENO_DIR}/vendor
)

# Replaces the Kconfig menu of the original project.
#  - ILI9488 driver, REGISTER I/O parallel (the only fast mode that supports shared-pin touch)
#  - CUSTOM_GPIO_CONFIG: the D0..D7/WR/RD/RS/CS/RST macros in lcd_com.c expand to the runtime
#    struct zeno_pins (filled by zenotft.init()), so no pin numbers are baked into the firmware.
#  - JD_USE_ROM: use the JPEG decoder in the S3 ROM (avoids the esp_jpeg managed component).
target_compile_definitions(usermod_zenotft INTERFACE MODULE_ZENOTFT_ENABLED=1)

# The vendored files were written for a plain ESP-IDF build; don't let their warnings fail the build.
set_source_files_properties(${ZENO_VENDORED} PROPERTIES COMPILE_OPTIONS "-Wno-error")

target_link_libraries(usermod INTERFACE usermod_zenotft)
