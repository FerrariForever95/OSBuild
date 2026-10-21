# micropython.cmake -- wires the zeno_img user C module (png, jpeg/jpg, bmp, img)
# into the MicroPython ESP32 port.
#
#   idf.py -D MICROPY_BOARD=<board> \
#          -D USER_C_MODULES="/abs/zeno_lcd/micropython.cmake;/abs/zeno_img/micropython.cmake" \
#          build
add_library(usermod_zeno_img INTERFACE)

target_sources(usermod_zeno_img INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}/zimg_sink.c
    ${CMAKE_CURRENT_LIST_DIR}/zimg_png.c
    ${CMAKE_CURRENT_LIST_DIR}/zimg_bmp.c
    ${CMAKE_CURRENT_LIST_DIR}/zimg_jpeg.c
    ${CMAKE_CURRENT_LIST_DIR}/zimg_inflate_rom.c
    ${CMAKE_CURRENT_LIST_DIR}/modzimg.c
)

target_include_directories(usermod_zeno_img INTERFACE ${CMAKE_CURRENT_LIST_DIR})

# esp_rom: rom/tjpgd.h + rom/miniz.h (ROM JPEG and inflate). Already part of every
# ESP-IDF build; made explicit like zeno_lcd does for its own dependencies.
idf_component_get_property(zeno_img_rom_lib esp_rom COMPONENT_LIB)
if(zeno_img_rom_lib)
    target_link_libraries(usermod_zeno_img INTERFACE ${zeno_img_rom_lib})
endif()

target_link_libraries(usermod INTERFACE usermod_zeno_img)
