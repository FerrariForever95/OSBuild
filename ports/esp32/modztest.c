#include "py/runtime.h"
#include "py/obj.h"

static mp_obj_t ztest_hello(void) {
    printf("[ZTEST] Hello from native module\n");
    return mp_obj_new_int(123);
}
static MP_DEFINE_CONST_FUN_OBJ_0(ztest_hello_obj, ztest_hello);

static const mp_rom_map_elem_t ztest_module_globals_table[] = {
    { MP_ROM_QSTR(MP_QSTR___name__), MP_ROM_QSTR(MP_QSTR_ztest) },
    { MP_ROM_QSTR(MP_QSTR_hello), MP_ROM_PTR(&ztest_hello_obj) },
};

static MP_DEFINE_CONST_DICT(
    ztest_module_globals,
    ztest_module_globals_table
);

const mp_obj_module_t mp_module_ztest = {
    .base = { &mp_type_module },
    .globals = (mp_obj_dict_t *)&ztest_module_globals,
};

MP_REGISTER_MODULE(MP_QSTR_ztest, mp_module_ztest);
