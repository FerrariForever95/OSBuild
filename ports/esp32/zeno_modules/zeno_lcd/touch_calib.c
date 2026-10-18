#include <string.h>
#include "nvs.h"
#include "nvs_flash.h"
#include "esp_log.h"
#include "touch_calib.h"

static const char *TAG = "zeno_touch_calib";
#define NVS_NS "zlcd_touch"

static double s_rx[ZCAL_MAX_POINTS], s_ry[ZCAL_MAX_POINTS];
static double s_sx[ZCAL_MAX_POINTS], s_sy[ZCAL_MAX_POINTS];
static int    s_n;
static zt_cal_t s_active;
static bool   s_have_active;

void zl_calib_begin(void) { s_n = 0; }

bool zl_calib_add_point(int raw_x, int raw_y, int screen_x, int screen_y)
{
    if (s_n >= ZCAL_MAX_POINTS) {
        return false;
    }
    s_rx[s_n] = raw_x; s_ry[s_n] = raw_y;
    s_sx[s_n] = screen_x; s_sy[s_n] = screen_y;
    s_n++;
    return true;
}

int zl_calib_count(void) { return s_n; }

esp_err_t zl_calib_solve(double *rms_px, double *maxerr_px)
{
    zt_cal_t k;
    double rms = 0, me = 0;
    if (!zt_cal_solve(s_rx, s_ry, s_sx, s_sy, s_n, &k, &rms, &me)) {
        return ESP_ERR_INVALID_STATE;
    }
    s_active = k;
    s_have_active = true;
    if (rms_px) *rms_px = rms;
    if (maxerr_px) *maxerr_px = me;
    ESP_LOGI(TAG, "calibration solved from %d points: rms=%.2fpx max=%.2fpx", s_n, rms, me);
    return ESP_OK;
}

bool zl_calib_active(const zt_cal_t **out)
{
    if (out) *out = s_have_active ? &s_active : NULL;
    return s_have_active;
}

void zl_calib_clear(void) { s_have_active = false; }

esp_err_t zl_calib_save_nvs(void)
{
    if (!s_have_active) {
        return ESP_ERR_INVALID_STATE;
    }
    nvs_handle_t h;
    esp_err_t e = nvs_open(NVS_NS, NVS_READWRITE, &h);
    if (e != ESP_OK) return e;
    e = nvs_set_blob(h, "affine", &s_active, sizeof(s_active));
    if (e == ESP_OK) e = nvs_commit(h);
    nvs_close(h);
    return e;
}

esp_err_t zl_calib_load_nvs(void)
{
    nvs_handle_t h;
    esp_err_t e = nvs_open(NVS_NS, NVS_READONLY, &h);
    if (e != ESP_OK) return e;
    size_t len = sizeof(s_active);
    zt_cal_t tmp;
    e = nvs_get_blob(h, "affine", &tmp, &len);
    nvs_close(h);
    if (e == ESP_OK && len == sizeof(tmp) && tmp.valid) {
        s_active = tmp;
        s_have_active = true;
    }
    return e;
}

void zl_calib_apply(int raw_x, int raw_y, int screen_w, int screen_h, int *x, int *y)
{
    if (s_have_active) {
        zt_cal_apply(&s_active, raw_x, raw_y, screen_w, screen_h, x, y);
        return;
    }
    int cx = raw_x, cy = raw_y;
    if (cx < 0) cx = 0;
    if (cy < 0) cy = 0;
    if (cx > screen_w - 1) cx = screen_w - 1;
    if (cy > screen_h - 1) cy = screen_h - 1;
    *x = cx;
    *y = cy;
}
