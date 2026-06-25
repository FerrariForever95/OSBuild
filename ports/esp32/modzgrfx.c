#include "py/runtime.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

static volatile uint32_t counter = 0;
static TaskHandle_t test_task_handle = NULL;

static void test_task(void *arg) {
    while (1) {
        counter++;
    }
}

STATIC mp_obj_t start_test(void) {
    if (test_task_handle == NULL) {
        xTaskCreatePinnedToCore(
            test_task,
            "gfx_test",
            4096,
            NULL,
            5,
            &test_task_handle,
            0
        );
    }
    return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_0(start_test_obj, start_test);

STATIC mp_obj_t get_counter(void) {
    return mp_obj_new_int_from_uint(counter);
}
STATIC MP_DEFINE_CONST_FUN_OBJ_0(get_counter_obj, get_counter);

static const mp_rom_map_elem_t test_module_globals_table[] = {
    { MP_ROM_QSTR(MP_QSTR___name__), MP_ROM_QSTR(MP_QSTR_ztest) },
    { MP_ROM_QSTR(MP_QSTR_start_test), MP_ROM_PTR(&start_test_obj) },
    { MP_ROM_QSTR(MP_QSTR_counter), MP_ROM_PTR(&get_counter_obj) },
};

static MP_DEFINE_CONST_DICT(test_module_globals, test_module_globals_table);

const mp_obj_module_t ztest_user_cmodule = {
    .base = { &mp_type_module },
    .globals = (mp_obj_dict_t *)&test_module_globals,
};

MP_REGISTER_MODULE(MP_QSTR_ztest, ztest_user_cmodule);
