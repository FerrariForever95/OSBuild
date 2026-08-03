// zeno_ili9488_bus.c
//
// STAGE 1 — Bus layer only.
//
// Scope of this file: GPIO bring-up + esp_lcd Intel-8080 (LCD_CAM) bus
// construction, and the two raw transaction primitives (tx_param / tx_color).
// It intentionally contains ZERO ILI9488 command knowledge -- no SWRESET,
// no SLPOUT, no MADCTL. That is Stage 2. This stage exists so the bus can be
// verified in isolation (correct GPIO config, correct esp_lcd bus/io handles,
// correct write-cycle waveform on a scope) before any panel-specific timing
// is layered on top.
//
// Hardware contract (fixed, not configurable at the Python level):
//   - CS is grounded in hardware. We pass cs_gpio_num = -1 to esp_lcd and
//     never touch a CS pin anywhere in this file.
//   - RD is tied HIGH in hardware (write-only bus). esp_lcd's I80 driver has
//     no concept of an RD line at all -- it is a write-only peripheral by
//     design -- so the only thing we do with RD is configure the GPIO once
//     as an output driven high, matching the physical wiring, and then never
//     touch it again for the lifetime of the object.
//   - RST and Backlight are plain GPIO outputs, driven by MicroPython-level
//     methods added in Stage 2 (reset()) and Stage 4 (backlight()). Stage 1
//     only configures their direction so pin state is deterministic from
//     construction onward (no floating RST during bring-up).

#include "py/runtime.h"
#include "py/obj.h"
#include "py/mphal.h"

#include "driver/gpio.h"
#include "esp_lcd_panel_io.h"
#include "esp_lcd_panel_vendor.h"
#include "esp_err.h"

// ---------------------------------------------------------------------
// Fixed pin map (from the target hardware description). These are build
// -time constants, not runtime Python arguments: this driver serves one
// board. If a second board variant is ever needed, these become
// Kconfig/menuconfig values, not a general-purpose kwargs surface --
// per the "no unnecessary abstraction" requirement.
// ---------------------------------------------------------------------
#define ZENO_PIN_RST   12
#define ZENO_PIN_RS    13   // D/CX
#define ZENO_PIN_WR    14
#define ZENO_PIN_RD    41
#define ZENO_PIN_D0    16
#define ZENO_PIN_D1    15
#define ZENO_PIN_D2    11
#define ZENO_PIN_D3    10
#define ZENO_PIN_D4    9
#define ZENO_PIN_D5    4
#define ZENO_PIN_D6    18
#define ZENO_PIN_D7    17
#define ZENO_PIN_BL    38   // Backlight, confirmed. Not a strapping pin on
                             // ESP32-S3 (strapping set is 0/3/45/46) and not
                             // in the octal-PSRAM-reserved range (26-32/33-37),
                             // so no conflict with typical S3 module wiring.

// Default values -- overridable from Python at construct time, since pixel
// clock and panel resolution are legitimately per-panel/per-tuning knobs,
// unlike the data/control pin map which is a hard PCB fact.
#define ZENO_DEFAULT_PCLK_HZ   10000000u   // 10 MHz start point; raised once
                                            // Stage 2/3 prove signal integrity
                                            // at higher rates on real hardware
#define ZENO_DEFAULT_WIDTH     320
#define ZENO_DEFAULT_HEIGHT    480

typedef struct _zeno_ili9488_obj_t {
    mp_obj_base_t base;
    esp_lcd_i80_bus_handle_t i80_bus;
    esp_lcd_panel_io_handle_t io_handle;
    gpio_num_t pin_rst;
    gpio_num_t pin_bl;
    uint16_t width;
    uint16_t height;
    bool bus_ready;
} zeno_ili9488_obj_t;

extern const mp_obj_type_t zeno_ili9488_type;

// ---------------------------------------------------------------------
// Internal helper: raise a Python OSError carrying the ESP-IDF error code
// and the failing call's name, so failures are diagnosable from the REPL
// instead of a bare abort.
// ---------------------------------------------------------------------
static void zeno_check(esp_err_t err, const char *what) {
    if (err != ESP_OK) {
        mp_raise_msg_varg(&mp_type_OSError, MP_ERROR_TEXT("%s failed: %d"), what, (int)err);
    }
}

// ---------------------------------------------------------------------
// __init__(*, pclk_hz=10_000_000, width=320, height=480)
//
// All pins -- including backlight (GPIO38) -- are now fixed board facts,
// consistent with RST/RS/WR/RD/D0-D7. Only the genuinely per-panel tuning
// knobs (pixel clock, resolution) remain as constructor kwargs.
// ---------------------------------------------------------------------
static mp_obj_t zeno_ili9488_make_new(const mp_obj_type_t *type,
                                       size_t n_args, size_t n_kw,
                                       const mp_obj_t *all_args) {
    enum { ARG_pclk_hz, ARG_width, ARG_height };
    static const mp_arg_t allowed_args[] = {
        { MP_QSTR_pclk_hz,   MP_ARG_KW_ONLY | MP_ARG_INT,  {.u_int = ZENO_DEFAULT_PCLK_HZ} },
        { MP_QSTR_width,     MP_ARG_KW_ONLY | MP_ARG_INT,  {.u_int = ZENO_DEFAULT_WIDTH} },
        { MP_QSTR_height,    MP_ARG_KW_ONLY | MP_ARG_INT,  {.u_int = ZENO_DEFAULT_HEIGHT} },
    };
    mp_arg_val_t args[MP_ARRAY_SIZE(allowed_args)];
    mp_arg_parse_all_kw_array(n_args, n_kw, all_args,
                               MP_ARRAY_SIZE(allowed_args), allowed_args, args);

    zeno_ili9488_obj_t *self = mp_obj_malloc(zeno_ili9488_obj_t, &zeno_ili9488_type);
    self->pin_rst   = (gpio_num_t)ZENO_PIN_RST;
    self->pin_bl    = (gpio_num_t)ZENO_PIN_BL;
    self->width     = (uint16_t)args[ARG_width].u_int;
    self->height    = (uint16_t)args[ARG_height].u_int;
    self->bus_ready = false;

    // --- RST: output, idle HIGH (chip not held in reset by default) ---
    gpio_config_t rst_cfg = {
        .pin_bit_mask = 1ULL << self->pin_rst,
        .mode = GPIO_MODE_OUTPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE,
    };
    zeno_check(gpio_config(&rst_cfg), "gpio_config(RST)");
    gpio_set_level(self->pin_rst, 1);

    // --- Backlight: output, default OFF until the caller explicitly turns
    // it on (Stage 4). Leaving it off by default avoids flashing garbage
    // GRAM contents at power-up before init() has run. ---
    gpio_config_t bl_cfg = {
        .pin_bit_mask = 1ULL << self->pin_bl,
        .mode = GPIO_MODE_OUTPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE,
    };
    zeno_check(gpio_config(&bl_cfg), "gpio_config(BL)");
    gpio_set_level(self->pin_bl, 0);

    // --- RD: output, permanently HIGH. Configured once, never touched
    // again anywhere in this driver -- esp_lcd's I80 bus driver has no
    // awareness of RD at all (it is write-only), so this is purely to
    // match the physical hardware contract and ensure the pin isn't left
    // floating at boot. ---
    gpio_config_t rd_cfg = {
        .pin_bit_mask = 1ULL << ZENO_PIN_RD,
        .mode = GPIO_MODE_OUTPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE,
    };
    zeno_check(gpio_config(&rd_cfg), "gpio_config(RD)");
    gpio_set_level((gpio_num_t)ZENO_PIN_RD, 1);

    // --- I80 (LCD_CAM) bus: WR + DC + 8 data lines. No RD field exists in
    // esp_lcd_i80_bus_config_t -- confirmed against the ESP-IDF 5.5
    // reference: RD is entirely outside this peripheral's scope. ---
    esp_lcd_i80_bus_config_t bus_config = {
        .clk_src = LCD_CLK_SRC_PLL160M,
        .dc_gpio_num = ZENO_PIN_RS,
        .wr_gpio_num = ZENO_PIN_WR,
        .data_gpio_nums = {
            ZENO_PIN_D0, ZENO_PIN_D1, ZENO_PIN_D2, ZENO_PIN_D3,
            ZENO_PIN_D4, ZENO_PIN_D5, ZENO_PIN_D6, ZENO_PIN_D7,
        },
        .bus_width = 8,
        // One full frame's worth of 16bpp pixel data as the largest single
        // DMA-backed transaction we'll ever issue in Stage 3's blit(). This
        // avoids fragmenting a full-screen fill into multiple transactions.
        .max_transfer_bytes = (size_t)self->width * (size_t)self->height * sizeof(uint16_t),
        // IDF 5.3+ field name. The reference project's psram_trans_align /
        // sram_trans_align pair (valid through IDF ~5.2) was removed by the
        // time of IDF 5.5 -- using dma_burst_size is required to compile
        // and link cleanly against the target toolchain.
        .dma_burst_size = 64,
    };
    zeno_check(esp_lcd_new_i80_bus(&bus_config, &self->i80_bus), "esp_lcd_new_i80_bus");

    esp_lcd_panel_io_i80_config_t io_config = {
        .cs_gpio_num = -1,   // CS grounded in hardware; nothing to drive
        .pclk_hz = (uint32_t)args[ARG_pclk_hz].u_int,
        .trans_queue_depth = 10,
        .dc_levels = {
            .dc_idle_level = 0,
            .dc_cmd_level = 0,
            .dc_dummy_level = 0,
            .dc_data_level = 1,
        },
        .lcd_cmd_bits = 8,
        .lcd_param_bits = 8,
        .flags = {
            .swap_color_bytes = false,  // revisited in Stage 3 once we fix
                                         // the wire pixel format (RGB565 vs
                                         // the ILI9488's native 6-6-6/3-byte
                                         // format in 18bpp mode)
        },
    };
    esp_err_t io_err = esp_lcd_new_panel_io_i80(self->i80_bus, &io_config, &self->io_handle);
    if (io_err != ESP_OK) {
        // Bus was created but IO device wasn't -- clean up so a retry from
        // the REPL doesn't leak the bus handle.
        esp_lcd_del_i80_bus(self->i80_bus);
        zeno_check(io_err, "esp_lcd_new_panel_io_i80");
    }

    self->bus_ready = true;
    return MP_OBJ_FROM_PTR(self);
}

// ---------------------------------------------------------------------
// deinit() -- tear down IO device then bus, mirroring construction order
// in reverse (esp_lcd requires the IO device be deleted before the bus).
// ---------------------------------------------------------------------
static mp_obj_t zeno_ili9488_deinit(mp_obj_t self_in) {
    zeno_ili9488_obj_t *self = MP_OBJ_TO_PTR(self_in);
    if (self->bus_ready) {
        esp_lcd_panel_io_del(self->io_handle);
        esp_lcd_del_i80_bus(self->i80_bus);
        self->bus_ready = false;
    }
    return mp_const_none;
}
static MP_DEFINE_CONST_FUN_OBJ_1(zeno_ili9488_deinit_obj, zeno_ili9488_deinit);

// ---------------------------------------------------------------------
// _tx_param(cmd, params=None) -- raw command + short-parameter write.
// Underscore-prefixed: this is the Stage 1 verification primitive, not
// the intended end-user API. Stage 2 wraps specific, named, correctly-
// timed methods around calls like this one; end users should not be
// hand-assembling ILI9488 command bytes from Python.
// ---------------------------------------------------------------------
static mp_obj_t zeno_ili9488_tx_param(size_t n_args, const mp_obj_t *args) {
    zeno_ili9488_obj_t *self = MP_OBJ_TO_PTR(args[0]);
    mp_int_t cmd = mp_obj_get_int(args[1]);

    if (n_args == 3 && args[2] != mp_const_none) {
        mp_buffer_info_t bufinfo;
        mp_get_buffer_raise(args[2], &bufinfo, MP_BUFFER_READ);
        zeno_check(
            esp_lcd_panel_io_tx_param(self->io_handle, (int)cmd, bufinfo.buf, bufinfo.len),
            "esp_lcd_panel_io_tx_param"
        );
    } else {
        zeno_check(
            esp_lcd_panel_io_tx_param(self->io_handle, (int)cmd, NULL, 0),
            "esp_lcd_panel_io_tx_param"
        );
    }
    return mp_const_none;
}
static MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(zeno_ili9488_tx_param_obj, 2, 3, zeno_ili9488_tx_param);

// ---------------------------------------------------------------------
// _tx_color(cmd, buf) -- raw command + large DMA-backed color write.
// Same "verification primitive, not end-user API" note as _tx_param.
// ---------------------------------------------------------------------
static mp_obj_t zeno_ili9488_tx_color(mp_obj_t self_in, mp_obj_t cmd_in, mp_obj_t buf_in) {
    zeno_ili9488_obj_t *self = MP_OBJ_TO_PTR(self_in);
    mp_int_t cmd = mp_obj_get_int(cmd_in);

    mp_buffer_info_t bufinfo;
    mp_get_buffer_raise(buf_in, &bufinfo, MP_BUFFER_READ);
    zeno_check(
        esp_lcd_panel_io_tx_color(self->io_handle, (int)cmd, bufinfo.buf, bufinfo.len),
        "esp_lcd_panel_io_tx_color"
    );
    return mp_const_none;
}
static MP_DEFINE_CONST_FUN_OBJ_3(zeno_ili9488_tx_color_obj, zeno_ili9488_tx_color);

// ---------------------------------------------------------------------
// _raw_rst(level) -- direct RST pin drive, no timing applied. Stage 1
// verification only; Stage 2's reset() applies the datasheet-timed pulse.
// ---------------------------------------------------------------------
static mp_obj_t zeno_ili9488_raw_rst(mp_obj_t self_in, mp_obj_t level_in) {
    zeno_ili9488_obj_t *self = MP_OBJ_TO_PTR(self_in);
    gpio_set_level(self->pin_rst, mp_obj_is_true(level_in) ? 1 : 0);
    return mp_const_none;
}
static MP_DEFINE_CONST_FUN_OBJ_2(zeno_ili9488_raw_rst_obj, zeno_ili9488_raw_rst);

static mp_obj_t zeno_ili9488_width(mp_obj_t self_in) {
    zeno_ili9488_obj_t *self = MP_OBJ_TO_PTR(self_in);
    return mp_obj_new_int(self->width);
}
static MP_DEFINE_CONST_FUN_OBJ_1(zeno_ili9488_width_obj, zeno_ili9488_width);

static mp_obj_t zeno_ili9488_height(mp_obj_t self_in) {
    zeno_ili9488_obj_t *self = MP_OBJ_TO_PTR(self_in);
    return mp_obj_new_int(self->height);
}
static MP_DEFINE_CONST_FUN_OBJ_1(zeno_ili9488_height_obj, zeno_ili9488_height);

static void zeno_ili9488_print(const mp_print_t *print, mp_obj_t self_in, mp_print_kind_t kind) {
    (void)kind;
    zeno_ili9488_obj_t *self = MP_OBJ_TO_PTR(self_in);
    mp_printf(print, "<ZenoILI9488 bus=%s, %ux%u>",
              self->bus_ready ? "ready" : "deinit",
              self->width, self->height);
}

static const mp_rom_map_elem_t zeno_ili9488_locals_dict_table[] = {
    { MP_ROM_QSTR(MP_QSTR_deinit),     MP_ROM_PTR(&zeno_ili9488_deinit_obj) },
    { MP_ROM_QSTR(MP_QSTR___del__),    MP_ROM_PTR(&zeno_ili9488_deinit_obj) },
    { MP_ROM_QSTR(MP_QSTR__tx_param),  MP_ROM_PTR(&zeno_ili9488_tx_param_obj) },
    { MP_ROM_QSTR(MP_QSTR__tx_color),  MP_ROM_PTR(&zeno_ili9488_tx_color_obj) },
    { MP_ROM_QSTR(MP_QSTR__raw_rst),   MP_ROM_PTR(&zeno_ili9488_raw_rst_obj) },
    { MP_ROM_QSTR(MP_QSTR_width),      MP_ROM_PTR(&zeno_ili9488_width_obj) },
    { MP_ROM_QSTR(MP_QSTR_height),     MP_ROM_PTR(&zeno_ili9488_height_obj) },
};
static MP_DEFINE_CONST_DICT(zeno_ili9488_locals_dict, zeno_ili9488_locals_dict_table);

MP_DEFINE_CONST_OBJ_TYPE(
    zeno_ili9488_type,
    MP_QSTR_ZenoILI9488,
    MP_TYPE_FLAG_NONE,
    print, zeno_ili9488_print,
    make_new, zeno_ili9488_make_new,
    locals_dict, (mp_obj_dict_t *)&zeno_ili9488_locals_dict
);

static const mp_rom_map_elem_t zeno_lcd_globals_table[] = {
    { MP_ROM_QSTR(MP_QSTR___name__),   MP_ROM_QSTR(MP_QSTR_zeno_lcd) },
    { MP_ROM_QSTR(MP_QSTR_ZenoILI9488), MP_ROM_PTR(&zeno_ili9488_type) },
};
static MP_DEFINE_CONST_DICT(zeno_lcd_globals, zeno_lcd_globals_table);

const mp_obj_module_t zeno_lcd_module = {
    .base = { &mp_type_module },
    .globals = (mp_obj_dict_t *)&zeno_lcd_globals,
};

MP_REGISTER_MODULE(MP_QSTR_zeno_lcd, zeno_lcd_module);
