#include "py/runtime.h"
#include "py/obj.h"
#include "py/binary.h"
#include "racer_core.h"

// Define Python object structure for racer state handle if needed, 
// or keep a static game instance for lightweight embedding.
typedef struct {
    mp_obj_base_t base;
    rc_game_t game;
    uint8_t fb[RC_ROW_BYTES * RC_H];
} racer_obj_t;

const mp_obj_type_t racer_type;

// racer.Racer([seed]) -> creates a game instance
static mp_obj_t racer_make_new(const mp_obj_type_t *type, size_t n_args, size_t n_kw, const mp_obj_t *args) {
    mp_arg_check_num(n_args, n_kw, 0, 1, false);
    uint32_t seed = (n_args > 0) ? (uint32_t)mp_obj_get_int(args[0]) : 0;
    
    racer_obj_t *self = m_new_obj(racer_obj_t);
    self->base.type = &racer_type;
    rc_reset(&self->game, seed);
    memset(self->fb, 0, sizeof(self->fb));
    return MP_OBJ_FROM_PTR(self);
}

// self.update(dt, steer)
static mp_obj_t racer_obj_update(mp_obj_t self_in, mp_obj_t dt_obj, mp_obj_t steer_obj) {
    racer_obj_t *self = MP_OBJ_TO_PTR(self_in);
    float dt = mp_obj_get_float(dt_obj);
    int steer = mp_obj_get_int(steer_obj);
    
    rc_update(&self->game, dt, steer);
    return mp_obj_new_int(self->game.state);
}
static MP_DEFINE_FUN_OBJ_3(racer_obj_update_obj, racer_obj_update);

// buf, dist, speed = self.render()
static mp_obj_t racer_obj_render(mp_obj_t self_in) {
    racer_obj_t *self = MP_OBJ_TO_PTR(self_in);
    
    rc_render(self->fb, &self->game);
    
    // Zero-copy bytearray view of the render frame buffer
    mp_obj_t py_buf = mp_obj_new_bytearray_by_ref(sizeof(self->fb), self->fb);
    
    mp_obj_t tuple[3] = {
        py_buf,
        mp_obj_new_float(self->game.dist),
        mp_obj_new_float(self->game.v)
    };
    return mp_obj_new_tuple(3, tuple);
}
static MP_DEFINE_FUN_OBJ_1(racer_obj_render_obj, racer_obj_render);

static const mp_rom_map_elem_t racer_locals_dict_table[] = {
    { MP_ROM_QSTR(MP_QSTR_update), MP_ROM_PTR(&racer_obj_update_obj) },
    { MP_ROM_QSTR(MP_QSTR_render), MP_ROM_PTR(&racer_obj_render_obj) },
};
static MP_DEFINE_CONST_DICT(racer_locals_dict, racer_locals_dict_table);

const mp_obj_type_t racer_type = {
    { &mp_type_type },
    .name = MP_QSTR_Racer,
    .make_new = racer_make_new,
    .locals_dict = (mp_obj_dict *)&racer_locals_dict,
};

// Module-level definitions
static const mp_rom_map_elem_t racer_module_globals_table[] = {
    { MP_ROM_QSTR(MP_QSTR___name__), MP_ROM_QSTR(MP_QSTR_racer) },
    { MP_ROM_QSTR(MP_QSTR_Racer), MP_ROM_PTR(&racer_type) },
    { MP_ROM_QSTR(MP_QSTR_WIDTH), MP_ROM_INT(RC_W) },
    { MP_ROM_QSTR(MP_QSTR_HEIGHT), MP_ROM_INT(RC_H) },
};
static MP_DEFINE_CONST_DICT(racer_module_globals, racer_module_globals_table);

const mp_obj_module_t mp_module_racer = {
    .base = { &mp_type_module },
    .globals = (mp_obj_dict_t *)&racer_module_globals,
};

MP_REGISTER_MODULE(MP_QSTR_racer, mp_module_racer);
