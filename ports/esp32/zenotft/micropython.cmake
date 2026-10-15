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
target_compile_definitions(usermod_zenotft INTERFACE
    MODULE_ZENOTFT_ENABLED=1
    CONFIG_ILI9488=1
    CONFIG_INTERFACE_REG=1
    CONFIG_INTERFACE_GPIO=0
    CONFIG_INTERFACE_I2S=0
    CONFIG_CUSTOM_GPIO_CONFIG=1
    CONFIG_D0_GPIO=zeno_pins.d0
    CONFIG_D1_GPIO=zeno_pins.d1
    CONFIG_D2_GPIO=zeno_pins.d2
    CONFIG_D3_GPIO=zeno_pins.d3
    CONFIG_D4_GPIO=zeno_pins.d4
    CONFIG_D5_GPIO=zeno_pins.d5
    CONFIG_D6_GPIO=zeno_pins.d6
    CONFIG_D7_GPIO=zeno_pins.d7
    CONFIG_WR_GPIO=zeno_pins.wr
    CONFIG_RD_GPIO=zeno_pins.rd
    CONFIG_RS_GPIO=zeno_pins.rs
    CONFIG_CS_GPIO=zeno_pins.cs
    CONFIG_RESET_GPIO=zeno_pins.rst
    CONFIG_JD_USE_ROM=1
)

# The vendored files were written for a plain ESP-IDF build; don't let their warnings fail the build.
set_source_files_properties(${ZENO_VENDORED} PROPERTIES COMPILE_OPTIONS "-Wno-error")

target_link_libraries(usermod INTERFACE usermod_zenotft)
