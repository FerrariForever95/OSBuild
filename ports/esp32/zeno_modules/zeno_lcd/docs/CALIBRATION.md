# Touch calibration

## Why the original scheme was replaced

The original (`main/main.c`, `TouchCalibration()`/`ConvertCoordinate()`)
samples raw min/max separately on each axis and rescales each axis
independently:

```
screen_x = (raw_x - raw_x_min) * width  / (raw_x_max - raw_x_min)
screen_y = (raw_y - raw_y_min) * height / (raw_y_max - raw_y_min)
```

This is a 2-point, per-axis affine fit with the cross terms (how much X
raw motion bleeds into Y screen position, and vice versa) fixed at zero.
It can only correct a constant offset and a per-axis scale. It cannot
correct any skew/rotation between the touch overlay's electrodes and the
LCD's pixel grid -- which is one of the most common real-world sources of
exactly the symptom described ("functional but has a small positional
error"): the touch film is very rarely bonded perfectly square to the
glass, so raw-X motion usually has at least a small raw-Y component too.

## The new scheme

`zt_cal_solve()` (`zeno_pure.h`) fits a full 2D affine transform by least
squares:

```
screen_x = a*raw_x + b*raw_y + c
screen_y = d*raw_x + e*raw_y + f
```

over `n >= 3` calibration points. With exactly 2 independent-axis points
this degrades to the same accuracy as the original (it still recovers a
correct 2-point mapping; it just doesn't yet have enough information to
fit `b`/`d`). With the 5 points `py/zeno_calibrate.py` collects by default
(4 corners + center), it fits offset, scale, rotation, and skew in one
pass, and also reports an RMS and max residual in screen pixels so you can
see how good the fit actually is -- the original reported no error metric
at all.

A fit is rejected (`zt_cal_solve()` returns `false`,
`touch.calib_solve()` raises `ValueError`) if the points are
collinear/degenerate (the X-Y covariance determinant underflows a
tolerance relative to the point spread) rather than silently producing
nonsense coefficients.

## Collecting calibration points

`py/zeno_calibrate.py`'s `run()`:
1. Draws a small circle at 5 known screen coordinates in turn (4 corners,
   inset 24px from the edges so a fingertip can actually land on them,
   plus the center).
2. At each target, waits for any already-in-progress touch to release,
   then averages 8 raw `touch.raw()` samples where `valid` is true.
3. Feeds `(raw_x, raw_y, screen_x, screen_y)` into
   `touch.calib_add_point()`.
4. After all 5, calls `touch.calib_solve()` (raises if it can't fit) and
   `touch.calib_save()` (persists to NVS under namespace `zlcd_touch`,
   key `affine`).

Run it once per physical unit:
```python
import zeno_calibrate
rms, max_err = zeno_calibrate.run()
print("rms px:", rms, "max px:", max_err)
```

`touch.init()` automatically calls the native equivalent of
`touch.calib_load_nvs()` at startup; if no calibration has ever been
saved, `touch.read()` falls back to raw-passthrough-and-clamp (see
`zl_calib_apply()` in `touch_calib.c`) rather than refusing to return
anything, so a freshly flashed unit still produces *some* touch
coordinates (just uncalibrated ones) before `zeno_calibrate.run()` is
ever called.

## MicroPython calibration API

| Function | Purpose |
|---|---|
| `touch.calib_begin()` | clear the in-progress point buffer |
| `touch.calib_add_point(raw_x, raw_y, screen_x, screen_y)` | record one point (max 9) |
| `touch.calib_solve()` | fit and install; returns `(rms_px, max_err_px)`; raises `ValueError` if it can't fit |
| `touch.calib_save()` | persist the active fit to NVS |
| `touch.calib_clear()` | drop the active fit (back to raw passthrough) |
| `touch.calib_active()` | `True`/`False` |
| `touch.raw()` | `(rx, ry, z, valid)` -- for calibration tooling and Stage B/C debugging |

## Measuring accuracy (Stage C)

Use `zeno_touch_test.run()` after calibrating: it draws a dot at
`touch.read()`'s reported position on every touch, so the gap between
where you actually touch and where the dot lands is directly visible.
`zeno_calibrate.run()`'s returned `rms_px`/`max_err_px` is the *fit*
residual against the 5 calibration points themselves, not an independent
accuracy measurement -- for a real accuracy number, calibrate, then touch
several *different* points (not the calibration points) and compare
`touch.read()`'s output to where you actually touched, per Stage C in
`VALIDATION.md`.

## What has NOT been measured

No physical hardware was available while writing this port, so no actual
RMS/max-error numbers from real touches are reported anywhere in this
delivery. Every accuracy claim above is about what the affine model is
mathematically capable of correcting versus the original's 2-point model,
not a measurement. `tests/host_test_pure.c`'s calibration tests confirm
`zt_cal_solve()`/`zt_cal_apply()` are numerically correct (identity fit,
known offset+scale fit, degenerate-input rejection, out-of-bounds
clamping), not that touch accuracy on your panel is good.
