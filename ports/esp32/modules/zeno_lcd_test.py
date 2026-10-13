"""
zeno_lcd_test.py -- Stage A/D display test program.

Exercises every exposed lcd.* function and both corners/edges of the panel
in every rotation, per VALIDATION.md Stage A and Stage D.

Usage:
    import zeno_lcd_test
    zeno_lcd_test.run()
"""
import time
import lcd

RED, GREEN, BLUE, WHITE, BLACK, YELLOW, CYAN = (
    0xF800, 0x07E0, 0x001F, 0xFFFF, 0x0000, 0xFFE0, 0x07FF,
)


def _corner_markers(color_bg=BLACK):
    w, h = lcd.width(), lcd.height()
    lcd.fill(color_bg)
    lcd.fill_rect(0, 0, 20, 20, RED)              # top-left
    lcd.fill_rect(w - 20, 0, 20, 20, GREEN)        # top-right
    lcd.fill_rect(0, h - 20, 20, 20, BLUE)         # bottom-left
    lcd.fill_rect(w - 20, h - 20, 20, 20, WHITE)   # bottom-right
    lcd.rect(0, 0, w, h, YELLOW)                   # full-frame border


def _color_bars():
    w, h = lcd.width(), lcd.height()
    bands = [RED, GREEN, BLUE, YELLOW, CYAN, WHITE]
    bw = max(1, w // len(bands))
    for i, c in enumerate(bands):
        x0 = i * bw
        bw_i = bw if i < len(bands) - 1 else (w - x0)
        lcd.fill_rect(x0, 0, bw_i, h, c)


def _lines_and_shapes():
    w, h = lcd.width(), lcd.height()
    lcd.fill(BLACK)
    lcd.line(0, 0, w - 1, h - 1, RED)
    lcd.line(w - 1, 0, 0, h - 1, GREEN)
    lcd.hline(0, h // 2, w, WHITE)
    lcd.vline(w // 2, 0, h, WHITE)
    lcd.circle(w // 2, h // 2, min(w, h) // 4, CYAN)
    lcd.circle(w // 4, h // 4, 20, YELLOW, True)


def _pixel_scan(step=17, color=WHITE):
    w, h = lcd.width(), lcd.height()
    lcd.fill(BLACK)
    for y in range(0, h, step):
        for x in range(0, w, step):
            lcd.pixel(x, y, color)


def _blit_test():
    w = min(32, lcd.width())
    h = min(32, lcd.height())
    buf = bytearray(w * h * 2)
    for yy in range(h):
        for xx in range(w):
            c = ((xx * 31 // w) << 11) | ((yy * 63 // h) << 5)
            i = (yy * w + xx) * 2
            buf[i] = (c >> 8) & 0xFF
            buf[i + 1] = c & 0xFF
    lcd.fill(BLACK)
    lcd.blit(4, 4, w, h, bytes(buf))


def run(pause_ms=600):
    lcd.init(rotation=0)
    for rot in range(4):
        lcd.set_rotation(rot)
        print("rotation", rot, "size", lcd.width(), "x", lcd.height())

        _corner_markers()
        time.sleep_ms(pause_ms)

        _color_bars()
        time.sleep_ms(pause_ms)

        _lines_and_shapes()
        time.sleep_ms(pause_ms)

        _pixel_scan()
        time.sleep_ms(pause_ms)

        _blit_test()
        time.sleep_ms(pause_ms)

    lcd.invert(True)
    time.sleep_ms(300)
    lcd.invert(False)

    lcd.fill(BLACK)
    print("zeno_lcd_test.run() complete")
