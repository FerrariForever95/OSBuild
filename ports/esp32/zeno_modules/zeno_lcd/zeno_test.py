# Smoke test. Pins live in vendor/lcd_pins.h (compile-time), so there is nothing to pass to init().
import lcd, time

lcd.init(width=320, height=480)                       # screen goes black
lcd.fill(lcd.color565(255, 0, 0)); time.sleep(0.5)    # red, green, blue: wrong colours -> lcd.invert(True)
lcd.fill(lcd.color565(0, 255, 0)); time.sleep(0.5)
lcd.fill(lcd.color565(0, 0, 255)); time.sleep(0.5)
lcd.fill(0)

print(lcd.mount())                                    # (total, used) bytes of the assets partition
lcd.font_load(0, "/assets/ILGH24XB.FNT")
lcd.text(0, 10, 10, "Zeno OS", 0xFFFF)
# lcd.png("/assets/logo.png"); lcd.jpeg("/assets/photo.jpg")

lcd.touch_init()                                      # ADC channels derived from CS / RS pins (GPIO-1)
for _ in range(40):                                   # press and release; note idle vs pressed numbers
    print(lcd.read_raw()); time.sleep_ms(100)

# After reading raw values at two known points per axis (e.g. 20 px and 300 px from the left):
# lcd.touch_calibration(xraw0, 20, xraw1, 300, yraw0, 20, yraw1, 460)   # add swap=True if axes are exchanged
# lcd.touch_config(z_min=..., jump=..., tolerance=...)                  # sensitivity tuning, any time
# while True:
#     p = lcd.read_touch()
#     if p: print(p)
#     time.sleep_ms(10)
