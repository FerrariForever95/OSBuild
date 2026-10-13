"""
zeno_minimal_test.py -- the smallest possible smoke test (deliverable #8).

Usage:
    import zeno_minimal_test
"""
import lcd
import touch

lcd.init(rotation=0)
touch.init()

lcd.fill(0x0000)
lcd.rect(0, 0, lcd.width(), lcd.height(), 0xFFFF)
lcd.fill_rect(10, 10, 40, 40, 0xF800)
lcd.circle(lcd.width() // 2, lcd.height() // 2, 30, 0x07E0, True)
lcd.pixel(5, 5, 0xFFFF)

print("lcd:", lcd.width(), "x", lcd.height(), "rotation", lcd.rotation())
print("touch calibrated:", touch.calib_active())
print("touch.raw():", touch.raw())
print("zeno_minimal_test: OK")
