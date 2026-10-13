/*
 * touch_calib.h -- affine 5-point touch calibration + NVS persistence.
 *
 * Replaces the original two-point, per-axis-separable TouchCalibration()/
 * ConvertCoordinate() in main.c. The original could only correct a constant
 * offset and a per-axis scale; it could not correct the small positional
 * error the user described if that error includes any skew/rotation between
 * the touch panel's axes and the LCD's axes (very common: the touch overlay
 * is rarely glued on perfectly square with the glass). An affine fit
 * (zt_cal_t / zt_cal_solve() in zeno_pure.h) corrects offset, scale,
 * rotation AND skew in one pass, and degrades gracefully to two-point
 * accuracy if the installer only collects 2-3 points.
 */
#ifndef ZENO_TOUCH_CALIB_H
#define ZENO_TOUCH_CALIB_H

#include <stdint.h>
#include <stdbool.h>
#include "esp_err.h"
#include "zeno_pure.h"

#define ZCAL_MAX_POINTS 9

void zl_calib_begin(void);                              /* clear point buffer */
bool zl_calib_add_point(int raw_x, int raw_y, int screen_x, int screen_y);
int  zl_calib_count(void);
/* Solve + install as the active calibration. rms/maxerr (screen px) filled
 * in on success, for the caller to report to the user. */
esp_err_t zl_calib_solve(double *rms_px, double *maxerr_px);

bool zl_calib_active(const zt_cal_t **out);
void zl_calib_clear(void);                               /* back to identity/raw */

esp_err_t zl_calib_save_nvs(void);
esp_err_t zl_calib_load_nvs(void);

/* raw -> clamped screen coords using the active calibration, or straight
 * passthrough+clamp if none is active (so callers always get *something*). */
void zl_calib_apply(int raw_x, int raw_y, int screen_w, int screen_h, int *x, int *y);

#endif
