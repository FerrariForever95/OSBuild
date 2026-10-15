# Smoke test. Fill in YOUR pins; none are assumed. Run line by line over the REPL.
import zenotft as t, time

PINS = dict(d=(0,0,0,0,0,0,0,0), wr=0, rd=0, rs=0, cs=0, rst=0)   # <-- EDIT: D0..D7, WR, RD, RS, CS, RST GPIOs
BL = 38

t.init(**PINS, bl=BL, width=320, height=480)       # screen goes black, backlight on
t.fill(t.color565(255, 0, 0)); time.sleep(0.5)       # red, green, blue: wrong colours -> t.invert(True)
t.fill(t.color565(0, 255, 0)); time.sleep(0.5)
t.fill(t.color565(0, 0, 255)); time.sleep(0.5)
t.fill(0)

print(t.mount())                                     # (total, used) bytes of the assets partition
t.font_load(0, "/assets/ILGH24XB.FNT")
t.text(0, 10, 10, "Zeno OS", 0xFFFF)
# t.png("/assets/logo.png"); t.jpeg("/assets/photo.jpg")

# Touch: adc_yp / adc_xm = ADC1 channel (GPIO-1) of the pins wired to the CS (Y+) and RS (X-) electrodes
t.touch_init(adc_yp=6, adc_xm=7)                     # <-- EDIT channels
for _ in range(40):                                  # press and release; note idle vs pressed numbers
    print(t.read_raw()); time.sleep_ms(100)

# After reading raw values at two known points per axis (e.g. 20 px and 300 px from the left):
# t.touch_calibration(xraw0, 20, xraw1, 300, yraw0, 20, yraw1, 460)   # add swap=True if axes are exchanged
# t.touch_config(z_min=..., jump=..., tolerance=...)                  # sensitivity tuning, any time
# while True:
#     p = t.read_touch()
#     if p: print(p)
#     time.sleep_ms(10)
