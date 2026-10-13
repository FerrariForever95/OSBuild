"""
zeno_touch_test.py -- Stage B/D touch test program.

Prints raw ADC readings and draws a dot wherever a calibrated touch is
detected. Run zeno_calibrate.run() first for the dot to land accurately;
without calibration, touch.read() still works (it falls back to clamped raw
passthrough) but will be offset/scaled incorrectly.

Usage:
    import zeno_touch_test
    zeno_touch_test.run()          # calibrated dot test, Ctrl-C to stop
    zeno_touch_test.run_raw()      # raw ADC printout only, no display needed
"""
import time
import lcd
import touch


def run_raw(duration_ms=15000):
    touch.init()
    t0 = time.ticks_ms()
    last_valid = False
    while time.ticks_diff(time.ticks_ms(), t0) < duration_ms:
        rx, ry, z, valid = touch.raw()
        if valid != last_valid:
            print("touch", "DOWN" if valid else "UP", "rx=", rx, "ry=", ry, "z=", z)
            last_valid = valid
        elif valid:
            print("rx=", rx, "ry=", ry, "z=", z)
        time.sleep_ms(50)


def run(duration_ms=20000):
    lcd.init(rotation=0)
    touch.init()
    lcd.fill(0x0000)

    t0 = time.ticks_ms()
    was_down = False
    while time.ticks_diff(time.ticks_ms(), t0) < duration_ms:
        p = touch.read()
        if p is not None:
            x, y = p
            lcd.circle(x, y, 3, 0x07FF, True)
            if not was_down:
                print("touch down at", x, y)
            was_down = True
        else:
            if was_down:
                print("touch up")
            was_down = False
        time.sleep_ms(20)

    print("zeno_touch_test.run() complete")
