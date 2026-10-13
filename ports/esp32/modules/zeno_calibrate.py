"""
zeno_calibrate.py -- interactive touch-calibration helper for Zeno OS.

Draws a small target at each of 5 points (4 corners + center, inset from the
edges so a finger can actually land on them), asks the user to tap each one,
and feeds the raw readings to the native affine solver in `touch`.

Usage (from the MicroPython REPL or a Zeno OS app):

    import zeno_calibrate
    zeno_calibrate.run()

Run this once per physical unit (touch panels vary enough from one assembly
to the next that calibration data does not transfer between units). Results
are saved to NVS by touch.calib_save(), so this only needs to run again if
the touch overlay is removed/reseated or accuracy drifts over time.
"""
import time
import lcd
import touch

_MARK_R = 10
_INSET = 24
_SAMPLES_PER_POINT = 8
_BG = 0x0000
_TARGET = 0x07FF   # cyan
_OK = 0x07E0       # green


def _targets():
    w, h = lcd.width(), lcd.height()
    return [
        (_INSET, _INSET),
        (w - 1 - _INSET, _INSET),
        (w // 2, h // 2),
        (_INSET, h - 1 - _INSET),
        (w - 1 - _INSET, h - 1 - _INSET),
    ]


def _wait_for_release():
    # Original Nopnop behaviour: drain any touch already in progress before
    # starting a new measurement, so a held-over tap doesn't bleed into the
    # next point's average.
    while touch.read() is not None:
        time.sleep_ms(10)


def _sample_point(sx, sy):
    lcd.fill(_BG)
    lcd.circle(sx, sy, _MARK_R, _TARGET, True)

    _wait_for_release()

    xs = []
    ys = []
    while len(xs) < _SAMPLES_PER_POINT:
        rx, ry, _z, valid = touch.raw()
        if valid:
            xs.append(rx)
            ys.append(ry)
        time.sleep_ms(15)

    lcd.circle(sx, sy, _MARK_R, _OK, True)
    _wait_for_release()
    return sum(xs) / len(xs), sum(ys) / len(ys)


def run():
    """Run the 5-point calibration and save it. Returns (rms_px, max_err_px)."""
    touch.calib_begin()
    for (sx, sy) in _targets():
        rx, ry = _sample_point(sx, sy)
        touch.calib_add_point(int(rx), int(ry), sx, sy)

    rms, maxerr = touch.calib_solve()
    touch.calib_save()

    lcd.fill(_BG)
    lcd.display(True)
    return rms, maxerr
