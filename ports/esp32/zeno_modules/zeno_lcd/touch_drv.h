/*
 * touch_drv.h -- 4-wire resistive touch over the shared LCD pins.
 *
 * Port of Nopnop touch_interface_cfg()/touch_getx()/touch_gety()/
 * touch_getz()/touch_getxy() from lcd_com.c, electrode mapping and ADC
 * channels taken from the sdkconfig (see zeno_lcd_config.h).
 *
 * The original touch_gpio() called gpio_reset_pin()+gpio_config() on EVERY
 * axis switch (4 times per touch_getxyz() call) and reconfigured ADC
 * channels on every touch_avr_analog() call -- acceptable for a polling demo
 * loop, far too slow/noisy for a touch driver other MicroPython code expects
 * to call every frame. Here:
 *   - the 4 electrode GPIOs are put in a known idle state once by
 *     zl_touch_init(), not reconfigured from scratch on every call;
 *   - the two ADC channels are configured once, not re-configured per read;
 *   - zl_touch_read_raw() is the one [LOCK] function -- it is the *only*
 *     place that repurposes the shared pins, and it always leaves them back
 *     in LCD mode before returning, bus lock held throughout.
 * The electrode sequencing (which 2 pins go INPUT, which 2 go OUTPUT+level,
 * which ADC channel is sampled, dual-sample-and-compare acceptance, and the
 * z/pressure gate) is unchanged from the original.
 */
#ifndef ZENO_TOUCH_DRV_H
#define ZENO_TOUCH_DRV_H

#include <stdint.h>
#include <stdbool.h>
#include "esp_err.h"
#include "esp_adc/adc_oneshot.h"
#include "zeno_pure.h"

typedef struct {
    int rx;        /* raw X sum (0 if this read was rejected/unstable) */
    int ry;        /* raw Y sum (0 if this read was rejected/unstable) */
    int z;         /* |Y+ - X-| pressure proxy                         */
    bool valid;     /* passed zt_sample_valid() against current config  */
} zl_touch_raw_t;

esp_err_t zl_touch_init(void);      /* ADC unit + channel config; idempotent */
bool      zl_touch_is_init(void);

void zl_touch_set_config(const zt_config_t *cfg);
void zl_touch_get_config(zt_config_t *cfg);

/* [LOCK] Re-does the original's electrode dance on D0/RS/CS/D1, takes
 * NUM_SAMPLES=2 dual readings per axis exactly like touch_getx()/
 * touch_gety()/touch_getz(), restores D0/D1/RS/CS to LCD outputs before
 * returning. Must be called with the bus lock held (see lcd_bus.h);
 * returns ESP_ERR_TIMEOUT if it could not get the lock itself when used
 * standalone -- the MicroPython binding takes the lock for you. */
esp_err_t zl_touch_read_raw(zl_touch_raw_t *out);

/* Debounced touch-down/up state machine built on zl_touch_read_raw(). */
bool zl_touch_poll(int *raw_x, int *raw_y);   /* true while debounced "touched" */

#endif
