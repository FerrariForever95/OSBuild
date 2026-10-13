#include "esp_rom_sys.h"
#include "driver/gpio.h"
#include "touch_drv.h"
#include "lcd_bus.h"
#include "zeno_lcd_config.h"

static adc_oneshot_unit_handle_t s_adc1;
static bool s_inited;
static zt_config_t s_cfg = {
    .min_raw      = ZTOUCH_DEFAULT_MIN_RAW,
    .z_max        = ZTOUCH_DEFAULT_Z_MAX,
    .settle_us    = ZTOUCH_DEFAULT_SETTLE_US,
    .down_confirm = ZTOUCH_DEFAULT_DOWN_CONFIRM,
    .up_confirm   = ZTOUCH_DEFAULT_UP_CONFIRM,
};
static zt_debounce_t s_deb;

esp_err_t zl_touch_init(void)
{
    if (s_inited) {
        return ESP_OK;
    }
    adc_oneshot_unit_init_cfg_t uc = { .unit_id = ADC_UNIT_1 };
    esp_err_t e = adc_oneshot_new_unit(&uc, &s_adc1);
    if (e != ESP_OK) {
        return e;
    }
    adc_oneshot_chan_cfg_t cc = { .atten = ADC_ATTEN_DB_12, .bitwidth = ADC_BITWIDTH_DEFAULT };
    if ((e = adc_oneshot_config_channel(s_adc1, ZTOUCH_ADC_CH_YP, &cc)) != ESP_OK) return e;
    if ((e = adc_oneshot_config_channel(s_adc1, ZTOUCH_ADC_CH_XM, &cc)) != ESP_OK) return e;
    s_inited = true;
    return ESP_OK;
}

bool zl_touch_is_init(void) { return s_inited; }

void zl_touch_set_config(const zt_config_t *cfg) { if (cfg) s_cfg = *cfg; }
void zl_touch_get_config(zt_config_t *cfg)       { if (cfg) *cfg = s_cfg; }

/* touch_avr_analog(): read `n` samples, drop min+max, average the rest. */
static int avg_adc(adc_channel_t ch, int n)
{
    if (n <= 2) return 0;
    int v[16];
    if (n > 16) n = 16;
    for (int i = 0; i < n; i++) {
        int raw = 0;
        if (adc_oneshot_read(s_adc1, ch, &raw) != ESP_OK) {
            raw = 0;
        }
        v[i] = raw;
    }
    return zt_trimmed_mean(v, n);
}

/* touch_gpio(): one axis pin -> input (pulldown) or output+level. */
static void set_pin(int gpio, bool as_output, int level)
{
    gpio_reset_pin(gpio);
    gpio_config_t c = { .pin_bit_mask = 1ULL << gpio };
    if (as_output) {
        c.mode = GPIO_MODE_OUTPUT;
        gpio_config(&c);
        gpio_set_level(gpio, level);
    } else {
        c.mode = GPIO_MODE_INPUT;
        c.intr_type = GPIO_INTR_DISABLE;
        c.pull_down_en = GPIO_PULLDOWN_ENABLE;
        gpio_config(&c);
    }
}

/* Put D0/D1/RS/CS back the way zl_bus_init() left them: GPIO outputs, idle
 * HIGH (CS/RS/the data lines rest high between bus transactions). */
static void restore_lcd_pins(void)
{
    const int pins[4] = { ZTOUCH_GPIO_XP, ZTOUCH_GPIO_XM, ZTOUCH_GPIO_YP, ZTOUCH_GPIO_YM };
    for (int i = 0; i < 4; i++) {
        gpio_reset_pin(pins[i]);
        gpio_set_direction(pins[i], GPIO_MODE_OUTPUT);
        gpio_set_level(pins[i], 1);
    }
}

esp_err_t zl_touch_read_raw(zl_touch_raw_t *out)
{
    if (!s_inited) {
        return ESP_ERR_INVALID_STATE;
    }
    bool own_lock = false;
    if (!zl_bus_lock()) {
        /* Caller may already hold it (documented contract); try once more
           briefly rather than silently reading through a torn-down bus. */
        esp_rom_delay_us(200);
        if (!zl_bus_lock()) {
            return ESP_ERR_TIMEOUT;
        }
    }
    own_lock = true;

    zl_bus_idle();   /* CS/RS/WR high: isolate the LCD before repurposing pins */

    /* --- X: Y+,Y- input; X+ high, X- low; sample ADC(Y+) twice --- */
    set_pin(ZTOUCH_GPIO_YP, false, 0);
    set_pin(ZTOUCH_GPIO_YM, false, 0);
    set_pin(ZTOUCH_GPIO_XP, true, 1);
    set_pin(ZTOUCH_GPIO_XM, true, 0);
    if (s_cfg.settle_us > 0) esp_rom_delay_us((uint32_t)s_cfg.settle_us);
    int xs0 = avg_adc(ZTOUCH_ADC_CH_YP, ZTOUCH_AVERAGE_TIME);
    int xs1 = avg_adc(ZTOUCH_ADC_CH_YP, ZTOUCH_AVERAGE_TIME);
    int rx = zt_axis_combine(xs0, xs1, ZTOUCH_COMP_TOLERANCE);

    /* --- Y: Y+,Y- output; X+,X- input; sample ADC(X-) twice --- */
    set_pin(ZTOUCH_GPIO_YP, true, 1);
    set_pin(ZTOUCH_GPIO_YM, true, 0);
    set_pin(ZTOUCH_GPIO_XP, false, 0);
    set_pin(ZTOUCH_GPIO_XM, false, 0);
    if (s_cfg.settle_us > 0) esp_rom_delay_us((uint32_t)s_cfg.settle_us);
    int ys0 = avg_adc(ZTOUCH_ADC_CH_XM, ZTOUCH_AVERAGE_TIME);
    int ys1 = avg_adc(ZTOUCH_ADC_CH_XM, ZTOUCH_AVERAGE_TIME);
    int ry = zt_axis_combine(ys0, ys1, ZTOUCH_COMP_TOLERANCE);

    /* --- Z: Y+ input; Y- high; X+ low; X- input; |ADC(Y+) - ADC(X-)| --- */
    set_pin(ZTOUCH_GPIO_YP, false, 0);
    set_pin(ZTOUCH_GPIO_YM, true, 1);
    set_pin(ZTOUCH_GPIO_XP, true, 0);
    set_pin(ZTOUCH_GPIO_XM, false, 0);
    if (s_cfg.settle_us > 0) esp_rom_delay_us((uint32_t)s_cfg.settle_us);
    int z1 = 0, z2 = 0;
    adc_oneshot_read(s_adc1, ZTOUCH_ADC_CH_YP, &z1);
    adc_oneshot_read(s_adc1, ZTOUCH_ADC_CH_XM, &z2);
    int z = z1 > z2 ? z1 - z2 : z2 - z1;

    restore_lcd_pins();
    if (own_lock) {
        zl_bus_unlock();
    }

    out->rx = rx;
    out->ry = ry;
    out->z  = z;
    out->valid = zt_sample_valid(rx, ry, z, &s_cfg);
    return ESP_OK;
}

bool zl_touch_poll(int *raw_x, int *raw_y)
{
    zl_touch_raw_t r;
    if (zl_touch_read_raw(&r) != ESP_OK) {
        return zt_debounce_step(&s_deb, false, s_cfg.down_confirm, s_cfg.up_confirm);
    }
    bool touched = zt_debounce_step(&s_deb, r.valid, s_cfg.down_confirm, s_cfg.up_confirm);
    if (touched) {
        *raw_x = r.rx;
        *raw_y = r.ry;
    }
    return touched;
}
