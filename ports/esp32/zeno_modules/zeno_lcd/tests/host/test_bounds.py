# Regression test: out-of-bounds draw commands must print a message and be IGNORED,
# and nothing may leave the lcd stuck (leaked bus lock / endless loop).
import lcd
import mocklcd

BLACK, RED = 0x0000, 0xF800
fails = 0


def check(cond, msg):
    global fails
    if not cond:
        fails += 1
        print("FAIL:", msg)


def blank():
    lcd.fill(BLACK)
    mocklcd.clear(BLACK)


def untouched(msg):
    check(mocklcd.count(RED) == 0, msg + ": drew something")
    check(not mocklcd.locked(), msg + ": bus lock left held")


lcd.init()
check(lcd.strict_bounds() is True, "strict by default")
W, H = lcd.width(), lcd.height()

# every primitive: partly or wholly off-screen -> ignored
cases = [
    "lcd.pixel(-1, 5, RED)", "lcd.pixel(W, 5, RED)", "lcd.pixel(5, H, RED)",
    "lcd.fill_rect(-5, 5, 20, 20, RED)", "lcd.fill_rect(W - 5, 5, 20, 20, RED)",
    "lcd.rect(5, H - 5, 20, 20, RED)", "lcd.rect(5, 5, W + 1, 5, RED, True)",
    "lcd.hline(-10, 5, 50, RED)", "lcd.vline(5, -10, 50, RED)",
    "lcd.line(-5, 5, 50, 50, RED)", "lcd.line(10, 10, 10, H, RED)",
    "lcd.line(2, 2, 100, 100, RED, 9)",
    "lcd.circle(5, 5, 20, RED)", "lcd.circle(W - 3, 50, 10, RED, True)",
    "lcd.ellipse(5, 50, 20, 10, RED)", "lcd.round_rect(-1, 5, 30, 30, 5, RED)",
    "lcd.triangle(5, 5, 50, 5, 50, H + 1, RED, True)", "lcd.polygon([(5, 5), (W + 1, 5), (20, 40)], RED, True)",
    "lcd.arc(5, 50, 20, 0, 90, RED)", "lcd.pie(W - 2, 50, 20, 0, 90, RED)", "lcd.ring(50, 5, 5, 20, RED)",
    "lcd.bezier(5, 5, 50, -1, 60, 60, 80, 80, RED)", "lcd.qbezier(5, 5, 50, H, 60, 60, RED)",
    "lcd.gradient(-5, 5, 20, 20, RED, RED)", "lcd.blit(W - 1, 0, 2, 2, bytes(8))",
    "lcd.blit_mono(W - 4, 5, 8, 8, bytes(8), RED)", "lcd.text(W - 10, 5, 'hello', RED)",
    # absurd values that used to freeze the board
    "lcd.line(0, 0, 2000000000, 5, RED)", "lcd.line(-2000000000, 0, 2000000000, H, RED)",
    "lcd.circle(10, 10, 1000000000, RED, True)", "lcd.fill_rect(0, 0, 2000000000, 2000000000, RED)",
    "lcd.line(0, 0, 100, 100, RED, 1000000000)",
]
for c in cases:
    blank()
    exec(c)
    untouched(c)

# inside the screen still works
blank()
lcd.fill_rect(10, 10, 20, 20, RED)
check(mocklcd.count(RED) == 400, "in-bounds fill_rect still draws")
blank()
lcd.line(100, 100, 10, 10, RED)          # upward diagonal used to loop forever
check(mocklcd.count(RED) > 50, "upward line terminates and draws")
blank()
lcd.fill_rect(10, 10, 0, 20, RED)        # zero width: silent no-op, no error
untouched("zero width")

# bad arguments raise but must NOT leave the bus locked
for bad in ("lcd.fill_rect(1.5, 2, 3, 4, RED)", "lcd.pixel(None, 2, RED)", "lcd.circle(1, 2, 'x', RED)",
            "lcd.line(1, 2, 3, 4, None)", "lcd.fill('red')"):
    try:
        exec(bad)
    except Exception:
        pass
    check(not mocklcd.locked(), bad + ": lock leaked")
blank()
lcd.fill_rect(10, 10, 20, 20, RED)
check(mocklcd.count(RED) == 400, "drawing works after bad arguments")

# loose mode: old clip-at-edge behaviour, but still protected from absurd values
lcd.strict_bounds(False)
blank()
lcd.fill_rect(-5, 5, 20, 20, RED)
check(mocklcd.count(RED) == 15 * 20, "loose mode clips at the edge")
blank()
lcd.line(0, 0, 2000000000, 5, RED)
untouched("loose mode, absurd line")
lcd.strict_bounds(True)

print("test_bounds:", "ALL PASSED" if fails == 0 else "%d FAILED" % fails)
