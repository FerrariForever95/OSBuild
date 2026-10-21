#include "py/runtime.h"
#include "py/obj.h"
#include "py/binary.h"
#include "racer_core.h"

static rc_game_t current_game;
static uint8_t frame_buffer[RC_ROW_BYTES * RC_H];

// racer.init(seed=0)
static mp_obj_t racer_init(size_t n_args, const mp_obj_t *args) {
    uint32_t seed = (n_args > 0) ? (uint32_t)mp_obj_get_int(args[0]) : 0;
    rc_reset(&current_game, seed);
    memset(frame_buffer, 0, sizeof(frame_buffer));
    return mp_const_none;
}
static MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(racer_init_obj, 0, 1, racer_init);

// state = racer.update(dt, steer)
static mp_obj_t racer_update(mp_obj_t dt_obj, mp_obj_t steer_obj) {
    float dt = (float)mp_obj_get_float(dt_obj);
    int steer = (int)mp_obj_get_int(steer_obj);
    rc_update(&current_game, dt, steer);
    return mp_obj_new_int(current_game.state);
}
static MP_DEFINE_CONST_FUN_OBJ_2(racer_update_obj, racer_update);

// buf, dist, speed = racer.render()
static mp_obj_t racer_render(void) {
    rc_render(frame_buffer, &current_game);
    
    mp_obj_t py_buf = mp_obj_new_bytearray_by_ref(sizeof(frame_buffer), frame_buffer);
    
    mp_obj_t tuple[3] = {
        py_buf,
        mp_obj_new_float(current_game.dist),
        mp_obj_new_float(current_game.v)
    };
    return mp_obj_new_tuple(3, tuple);
}
static MP_DEFINE_CONST_FUN_OBJ_0(racer_render_obj, racer_render);

static const mp_rom_map_elem_t racer_module_globals_table[] = {
    { MP_ROM_QSTR(MP_QSTR___name__), MP_ROM_QSTR(MP_QSTR_racer) },
    { MP_ROM_QSTR(MP_QSTR_init), MP_ROM_PTR(&racer_init_obj) },
    { MP_ROM_QSTR(MP_QSTR_update), MP_ROM_PTR(&racer_update_obj) },
    { MP_ROM_QSTR(MP_QSTR_render), MP_ROM_PTR(&racer_render_obj) },
    { MP_ROM_QSTR(MP_QSTR_WIDTH), MP_ROM_INT(RC_W) },
    { MP_ROM_QSTR(MP_QSTR_HEIGHT), MP_ROM_INT(RC_H) },
};
static MP_DEFINE_CONST_DICT(racer_module_globals, racer_module_globals_table);

const mp_obj_module_t mp_module_racer = {
    .base = { &mp_type_module },
    .globals = (mp_obj_dict_t *)&racer_module_globals,
};

MP_REGISTER_MODULE(MP_QSTR_racer, mp_module_racer);
