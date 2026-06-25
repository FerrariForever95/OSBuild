/**
 * @file modili9488_new.c
 * @brief MicroPython bindings for ili9488_new driver.
 *
 * Python API:
 *   import ili9488
 *   ili9488.init()                  → None   raises OSError on failure
 *   ili9488.reset()                 → None
 *   ili9488.write_cmd(cmd)          → None   cmd: int 0-255
 *   ili9488.write_data(cmd, data)   → None   data: bytes or bytearray
 *   ili9488.read(cmd, length)       → bytes  length: int 1-255
 *   ili9488.fill(color)             → None   color: RGB565 int 0-65535
 *
 * qstrdefsport.h — add:
 *   Q(ili9488)
 *   Q(init)
 *   Q(reset)
 *   Q(write_cmd)
 *   Q(write_data)
 *   Q(read)
 *   Q(fill)
 */

#include "py/runtime.h"
#include "py/obj.h"
#include "py/mperrno.h"
#include "ili9488.h"

/* ── ili9488.init() ───────────────────────────────────────────── */
static mp_obj_t m_init(void)
{
    if (!ili9488_init()) {
        mp_raise_OSError(MP_ENODEV);
    }
    return mp_const_none;
}
MP_DEFINE_CONST_FUN_OBJ_0(m_init_obj, m_init);

/* ── ili9488.reset() ─────────────────────────────────────────── */
static mp_obj_t m_reset(void)
{
    ili9488_reset();
    return mp_const_none;
}
MP_DEFINE_CONST_FUN_OBJ_0(m_reset_obj, m_reset);

/* ── ili9488.write_cmd(cmd) ──────────────────────────────────── */
static mp_obj_t m_write_cmd(mp_obj_t cmd_obj)
{
    mp_int_t cmd = mp_obj_get_int(cmd_obj);
    if (cmd < 0 || cmd > 0xFF) {
        mp_raise_ValueError(MP_ERROR_TEXT("cmd must be 0-255"));
    }
    ili9488_write_cmd((uint8_t)cmd);
    return mp_const_none;
}
MP_DEFINE_CONST_FUN_OBJ_1(m_write_cmd_obj, m_write_cmd);

/* ── ili9488.write_data(cmd, data) ──────────────────────────── */
static mp_obj_t m_write_data(mp_obj_t cmd_obj, mp_obj_t data_obj)
{
    mp_int_t cmd = mp_obj_get_int(cmd_obj);
    if (cmd < 0 || cmd > 0xFF) {
        mp_raise_ValueError(MP_ERROR_TEXT("cmd must be 0-255"));
    }
    mp_buffer_info_t buf;
    mp_get_buffer_raise(data_obj, &buf, MP_BUFFER_READ);
    ili9488_write_data((uint8_t)cmd, (const uint8_t *)buf.buf, buf.len);
    return mp_const_none;
}
MP_DEFINE_CONST_FUN_OBJ_2(m_write_data_obj, m_write_data);

/* ── ili9488.read(cmd, length) → bytes ──────────────────────── */
static mp_obj_t m_read(mp_obj_t cmd_obj, mp_obj_t len_obj)
{
    mp_int_t cmd = mp_obj_get_int(cmd_obj);
    mp_int_t len = mp_obj_get_int(len_obj);
    if (cmd < 0 || cmd > 0xFF) {
        mp_raise_ValueError(MP_ERROR_TEXT("cmd must be 0-255"));
    }
    if (len < 1 || len > 255) {
        mp_raise_ValueError(MP_ERROR_TEXT("length must be 1-255"));
    }
    uint8_t buf[255];
    if (ili9488_read((uint8_t)cmd, buf, (size_t)len) != ESP_OK) {
        mp_raise_OSError(MP_ENODEV);
    }
    return mp_obj_new_bytes((const byte *)buf, (size_t)len);
}
MP_DEFINE_CONST_FUN_OBJ_2(m_read_obj, m_read);

/* ── ili9488.fill(color) ─────────────────────────────────────── */
static mp_obj_t m_fill(mp_obj_t color_obj)
{
    mp_int_t color = mp_obj_get_int(color_obj);
    if (color < 0 || color > 0xFFFF) {
        mp_raise_ValueError(MP_ERROR_TEXT("color must be 0-65535 (RGB565)"));
    }
    ili9488_fill((uint16_t)color);
    return mp_const_none;
}
MP_DEFINE_CONST_FUN_OBJ_1(m_fill_obj, m_fill);

/* ── module globals ───────────────────────────────────────────── */
static const mp_rom_map_elem_t mod_table[] = {
    { MP_ROM_QSTR(MP_QSTR___name__),   MP_ROM_QSTR(MP_QSTR_ili9488)   },
    { MP_ROM_QSTR(MP_QSTR_fill),       MP_ROM_PTR(&m_fill_obj)        },
    { MP_ROM_QSTR(MP_QSTR_init),       MP_ROM_PTR(&m_init_obj)        },
    { MP_ROM_QSTR(MP_QSTR_read),       MP_ROM_PTR(&m_read_obj)        },
    { MP_ROM_QSTR(MP_QSTR_reset),      MP_ROM_PTR(&m_reset_obj)       },
    { MP_ROM_QSTR(MP_QSTR_write_cmd),  MP_ROM_PTR(&m_write_cmd_obj)   },
    { MP_ROM_QSTR(MP_QSTR_write_data), MP_ROM_PTR(&m_write_data_obj)  },
};
static MP_DEFINE_CONST_DICT(mod_globals, mod_table);

const mp_obj_module_t modili9488_new_module = {
    .base    = { &mp_type_module },
    .globals = (mp_obj_dict_t *)&mod_globals,
};
MP_REGISTER_MODULE(MP_QSTR_ili9488, modili9488_new_module);
