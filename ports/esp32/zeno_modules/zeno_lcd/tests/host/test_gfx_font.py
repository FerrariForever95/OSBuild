# Runs inside MicroPython (unix port) against the REAL modzlcd.c + zeno_gfx.c +
# zeno_font.c + ili9488_panel.c, with mock_bus.c standing in for the GPIO bus.
import lcd
import mocklcd
import struct

lcd.init()
lcd.strict_bounds(False)   # this suite deliberately tests clip-at-the-edge behaviour

BLACK, WHITE, RED, GREEN, BLUE = 0x0000, 0xFFFF, 0xF800, 0x07E0, 0x001F
fails = 0
passed = 0


def check(cond, msg):
    global fails, passed
    if cond:
        passed += 1
    else:
        fails += 1
        print("FAIL:", msg)


def fresh(bg=BLACK):
    lcd.fill(bg)
    mocklcd.clear(bg)
    lcd.fill(bg)
    mocklcd.clear(bg)  # also resets the stats counters


def clean(msg):
    extra, oob, _ = mocklcd.stats()
    check(extra == 0 and oob == 0, "%s: window overrun=%d out-of-bounds=%d" % (msg, extra, oob))


def at(x, y):
    return mocklcd.get(x, y)


def count(c):
    return mocklcd.count(c)


lcd.init(rotation=1)
W, H = lcd.width(), lcd.height()
check((W, H) == (480, 320), "landscape size %r" % ((W, H),))

# ---------------------------------------------------------------- regression
fresh()
lcd.rect(10, 10, 20, 10, RED)
check(at(10, 10) == RED and at(29, 19) == RED and at(15, 15) == BLACK, "rect outline (old behaviour)")
lcd.rect(50, 10, 20, 10, RED, True)
check(count(RED) == (20 * 10 - 18 * 8 + 20 * 10) or at(55, 15) == RED, "rect fill=True")
check(at(55, 15) == RED, "rect fill interior")
fresh()
lcd.line(0, 0, 40, 0, GREEN)
check(count(GREEN) == 41, "line (old signature) count=%d" % count(GREEN))
lcd.circle(100, 100, 10, BLUE, True)
check(at(100, 100) == BLUE and at(100, 90) == BLUE and at(100, 111) == BLACK, "circle fill (old)")
clean("regression")

# ------------------------------------------------------------------- ellipse
fresh()
lcd.ellipse(200, 150, 60, 30, RED, fill=True)
area = count(RED)
import math
check(abs(area - math.pi * 60 * 30) / (math.pi * 60 * 30) < 0.03, "ellipse fill area %d" % area)
check(at(200, 150) == RED and at(200 + 59, 150) == RED and at(200 - 59, 150) == RED, "ellipse fill centre row")
check(at(200 + 61, 150) == BLACK and at(200, 150 + 31) == BLACK and at(200 + 55, 150 + 20) == BLACK, "ellipse fill outside")
sym = all(at(200 + dx, 150 + dy) == at(200 - dx, 150 - dy) == at(200 + dx, 150 - dy) for dx in range(0, 61, 3) for dy in range(0, 31, 3))
check(sym, "ellipse fill symmetric")
fresh()
lcd.ellipse(200, 150, 60, 30, GREEN)
check(at(260, 150) == GREEN and at(140, 150) == GREEN and at(200, 120) == GREEN and at(200, 180) == GREEN, "ellipse outline extremes")
check(at(200, 150) == BLACK and at(230, 150) == BLACK, "ellipse outline hollow")
fresh()
lcd.ellipse(100, 100, 40, 0, RED)      # degenerate -> line
check(count(RED) == 81, "ellipse ry=0 is a line (%d)" % count(RED))
lcd.ellipse(100, 200, 0, 0, RED, fill=True)
check(at(100, 200) == RED, "ellipse 0x0 is a pixel")
lcd.ellipse(-500, -500, 20, 20, RED, True)   # fully off-screen: no crash, no effect
lcd.ellipse(10, 10, 100, 80, BLUE, True)     # partially clipped
clean("ellipse")

# ---------------------------------------------------------------- round rect
fresh()
lcd.round_rect(20, 20, 100, 60, 12, RED, fill=True)
check(at(20, 20) == BLACK and at(119, 20) == BLACK and at(20, 79) == BLACK and at(119, 79) == BLACK, "rrect fill corners cut")
check(at(32, 20) == RED and at(20, 32) == RED and at(70, 50) == RED, "rrect fill edges/interior")
check(at(20, 50) == RED and at(119, 50) == RED and at(70, 79) == RED, "rrect fill extreme edges")
# r bigger than half the short side is clamped (pill shape), never overruns
fresh()
lcd.round_rect(20, 20, 100, 30, 999, RED, True)
check(at(70, 35) == RED and at(20, 20) == BLACK, "rrect radius clamp")
fresh()
lcd.round_rect(20, 20, 100, 60, 0, RED, True)
check(count(RED) == 6000, "rrect r=0 == fill_rect (%d)" % count(RED))
fresh()
lcd.round_rect(20, 20, 100, 60, 12, GREEN)
check(at(50, 20) == GREEN and at(50, 79) == GREEN and at(20, 50) == GREEN and at(119, 50) == GREEN, "rrect outline edges")
check(at(70, 50) == BLACK and at(20, 20) == BLACK, "rrect outline hollow / corner cut")
# outline arc must be 4-connected-ish: every outline pixel in the top-left corner zone has a neighbour
ok = True
for y in range(20, 32):
    for x in range(20, 32):
        if at(x, y) == GREEN:
            nb = sum(1 for dx, dy in ((1, 0), (-1, 0), (0, 1), (0, -1), (1, 1), (-1, -1), (1, -1), (-1, 1))
                     if at(x + dx, y + dy) == GREEN)
            ok = ok and nb >= 2
check(ok, "rrect outline corner is connected")
lcd.round_rect(-30, -30, 60, 60, 10, RED, True)
lcd.round_rect(440, 300, 100, 100, 10, RED)
lcd.round_rect(10, 10, 0, 5, 3, RED)
clean("round_rect")

# ------------------------------------------------------------------ triangle
fresh()
lcd.triangle(50, 50, 150, 60, 90, 140, RED, fill=True)
area = count(RED)
exact = abs((150 - 50) * (140 - 50) - (90 - 50) * (60 - 50)) / 2
check(abs(area - exact) / exact < 0.06, "triangle area %d vs %d" % (area, exact))
check(at(50, 50) == RED and at(150, 60) == RED and at(90, 140) == RED, "triangle vertices filled")
check(at(100, 80) == RED and at(40, 40) == BLACK and at(160, 140) == BLACK, "triangle inside/outside")
fresh()
lcd.triangle(50, 50, 150, 60, 90, 140, GREEN)
check(at(50, 50) == GREEN and at(150, 60) == GREEN and at(90, 140) == GREEN and at(100, 80) == BLACK, "triangle outline")
fresh()
lcd.triangle(10, 10, 60, 10, 35, 60, RED, True)    # flat top
check(count(RED) > 1000 and at(10, 10) == RED and at(60, 10) == RED, "triangle flat top")
fresh()
lcd.triangle(10, 60, 35, 10, 60, 60, RED, True)    # flat bottom, unsorted input order
check(count(RED) > 1000 and at(10, 60) == RED and at(60, 60) == RED and at(35, 10) == RED, "triangle flat bottom")
lcd.triangle(5, 5, 5, 5, 5, 5, RED, True)
lcd.triangle(10, 10, 100, 10, 50, 10, RED, True)   # degenerate: one row
lcd.triangle(-100, -100, 600, 20, 100, 900, BLUE, True)
clean("triangle")

# ------------------------------------------------------------------- polygon
fresh()
U = [(20, 20), (40, 20), (40, 60), (60, 60), (60, 20), (80, 20), (80, 80), (20, 80)]   # a "U" (concave)
lcd.polygon(U, RED, fill=True)
check(at(30, 50) == RED and at(70, 50) == RED and at(50, 75) == RED, "polygon concave arms/base")
check(at(50, 40) == BLACK, "polygon concave notch is empty")
check(at(20, 80) == RED and at(80, 80) == RED and at(20, 20) == RED, "polygon corners (outline closes bottom row)")
fresh()
lcd.polygon([100, 100, 160, 100, 130, 160], GREEN)      # flat list form, outline
check(at(100, 100) == GREEN and at(160, 100) == GREEN and at(130, 160) == GREEN and at(130, 120) == BLACK, "polygon flat list outline")
try:
    lcd.polygon([1, 2, 3], RED)
    check(False, "odd flat list should raise")
except ValueError:
    check(True, "odd flat list ValueError")
try:
    lcd.polygon([(i, i) for i in range(65)], RED)
    check(False, "65 points should raise")
except ValueError:
    check(True, "too many points ValueError")
lcd.polygon([], RED)
lcd.polygon([(5, 5)], RED, True)
lcd.polygon([(5, 5), (30, 30)], RED, True)
star = []
for i in range(10):
    r = 60 if i % 2 == 0 else 25
    a = -math.pi / 2 + i * math.pi / 5
    star.append((200 + int(r * math.cos(a)), 150 + int(r * math.sin(a))))
lcd.polygon(star, BLUE, True)
check(at(200, 150) == BLUE, "star centre filled")
clean("polygon")

# ------------------------------------------------------- arc / pie / ring
fresh()
lcd.pie(200, 150, 60, 0, 90, RED)         # 3 o'clock -> 6 o'clock = lower-right on screen
check(at(230, 180) == RED and at(260 - 1, 150) == RED, "pie 0..90 lower right filled")
check(at(170, 120) == BLACK and at(230, 120) == BLACK and at(170, 180) == BLACK, "pie 0..90 other quadrants empty")
fresh()
lcd.pie(200, 150, 60, 90, 0, RED)         # wraps: 270-degree sweep (reflex angle)
check(at(170, 120) == RED and at(230, 120) == RED and at(170, 180) == RED, "pie 90..0 reflex three quadrants")
check(at(230, 180) == BLACK, "pie 90..0 hole quadrant empty")
fresh()
lcd.pie(200, 150, 60, 0, 360, RED)
full = count(RED)
check(abs(full - math.pi * 60 * 60) / (math.pi * 60 * 60) < 0.04, "pie 360 area %d" % full)
fresh()
lcd.ring(200, 150, 40, 60, GREEN)
check(at(200, 150) == BLACK and at(200 + 50, 150) == GREEN and at(200, 150 - 50) == GREEN and at(200 + 70, 150) == BLACK, "ring annulus")
ring_area = count(GREEN)
check(abs(ring_area - math.pi * (60 * 60 - 40 * 40)) / (math.pi * 2000) < 0.06, "ring area %d" % ring_area)
fresh()
lcd.ring(200, 150, 40, 60, GREEN, start=180, end=360)    # upper half only
check(at(200, 150 - 50) == GREEN and at(200, 150 + 50) == BLACK, "ring half gauge")
fresh()
lcd.arc(200, 150, 60, 0, 90, BLUE)                       # 1px arc
n = count(BLUE)
check(40 < n < 140, "thin arc pixel count %d" % n)
check(at(260, 150) == BLUE or at(259, 150) == BLUE, "arc starts at 3 o'clock")
check(at(200, 210) == BLUE or at(200, 209) == BLUE, "arc ends at 6 o'clock")
check(at(200, 150) == BLACK and at(140, 150) == BLACK, "arc other side empty")
fresh()
lcd.arc(200, 150, 60, 0, 360, RED, thick=6)
check(at(200 + 57, 150) == RED and at(200 + 62, 150) == BLACK and at(200 + 50, 150) == BLACK, "arc thick=6 band")
lcd.arc(200, 150, 60, 10.5, 99.25, RED, 3)               # float angles accepted
lcd.arc(200, 150, 60, 45, 45, RED)                       # zero sweep: nothing
lcd.pie(-100, -100, 30, 0, 360, RED)
lcd.pie(5, 5, 300, 0, 360, BLUE)
clean("arc/pie/ring")

# ------------------------------------------------------ thick line, bezier
fresh()
lcd.line(50, 100, 150, 100, RED, 5)
check(all(at(100, 100 + d) == RED for d in (-2, -1, 0, 1, 2)) and at(100, 103) == BLACK and at(100, 97) == BLACK, "thick horizontal line")
fresh()
lcd.line(50, 50, 150, 150, RED, 6)
check(at(100, 100) == RED and at(100, 94) == BLACK and at(100, 106) == BLACK or at(100, 95) == BLACK, "thick diagonal line")
fresh()
lcd.line(100, 100, 100, 100, RED, 9)
check(at(100, 100) == RED and at(104, 100) == RED, "zero-length thick line is a dot")
lcd.line(10, 10, 60, 40, BLUE, 1)
lcd.line(10, 10, 60, 40, BLUE, 0)
clean("thick line")
fresh()
lcd.bezier(20, 200, 60, 20, 160, 20, 200, 200, WHITE)
check(at(20, 200) == WHITE and at(200, 200) == WHITE and count(WHITE) > 150, "cubic bezier endpoints/length")
lcd.qbezier(250, 200, 300, 100, 350, 200, GREEN)
check(at(250, 200) == GREEN and at(350, 200) == GREEN and count(GREEN) > 60, "quadratic bezier")
clean("bezier")

# ------------------------------------------------------------------ gradient
fresh()
c0, c1 = lcd.color565(255, 0, 0), lcd.color565(0, 0, 255)
check((c0, c1) == (0xF800, 0x001F), "color565")
lcd.gradient(10, 10, 100, 50, c0, c1)
check(at(50, 10) == c0 and at(50, 59) == c1 and at(50, 30) not in (c0, c1) and at(9, 10) == BLACK and at(50, 60) == BLACK, "vertical gradient")
fresh()
lcd.gradient(10, 10, 100, 50, c0, c1, vertical=False)
check(at(10, 30) == c0 and at(109, 30) == c1 and at(60, 30) not in (c0, c1), "horizontal gradient")
check(at(60, 10) == at(60, 59), "horizontal gradient rows identical")
lcd.gradient(-50, -50, 100, 100, c0, c1, False)     # clipped: colour index stays anchored to the full rect
lcd.gradient(-50, -50, 100, 100, c0, c1, True)
lcd.gradient(440, 300, 100, 100, c0, c1, False)
lcd.gradient(0, 0, 0, 5, c0, c1)
clean("gradient")
check(lcd.blend(0xF800, 0x001F, 0) == 0xF800 and lcd.blend(0xF800, 0x001F, 255) == 0x001F and lcd.blend(0, 0xFFFF, 255) == 0xFFFF, "blend ends")

# ---------------------------------------------------------------- blit_mono
fresh()
bits = bytes([0b10100000, 0b01000000, 0b10100000])        # 3x3 checker, 1 byte/row
lcd.blit_mono(10, 10, 3, 3, bits, WHITE)
check(at(10, 10) == WHITE and at(11, 10) == BLACK and at(11, 11) == WHITE and count(WHITE) == 5, "blit_mono transparent")
fresh(BLUE)
lcd.blit_mono(10, 10, 3, 3, bits, WHITE, bg=RED)
check(at(10, 10) == WHITE and at(11, 10) == RED and at(13, 10) == BLUE, "blit_mono opaque")
fresh()
lcd.blit_mono(10, 10, 3, 3, bits, WHITE, scale=3)
check(count(WHITE) == 5 * 9 and at(10, 10) == WHITE and at(12, 12) == WHITE and at(13, 10) == BLACK, "blit_mono scale=3")
try:
    lcd.blit_mono(0, 0, 8, 2, b"\x00", WHITE)
    check(False, "short buffer must raise")
except ValueError:
    check(True, "blit_mono size check")
lcd.blit_mono(-5, -5, 3, 3, bits, WHITE, bg=RED)       # clipped
lcd.blit_mono(478, 318, 3, 3, bits, WHITE, bg=RED)
clean("blit_mono")

# --------------------------------------------------------------------- fonts
names = lcd.fonts()
check(names == ("mono12", "mono16", "sans16", "sansbold24", "digits48"), "fonts() %r" % (names,))
for fid in range(len(names)):
    h, asc, first, last, bpp = lcd.font_info(fid)
    check(h > 0 and 0 < asc <= h and first <= 32 and last >= 58 and bpp == 4, "font_info %d" % fid)
check(lcd.font_info(lcd.FONT_MONO16)[:2] == (19, 15) or lcd.font_info(lcd.FONT_MONO16)[0] > 10, "mono16 metrics")

fresh()
endx = lcd.text(10, 10, "Hello", WHITE, BLACK)
w, h = lcd.text_size("Hello")
check(endx == 10 + w, "text end x %d == 10 + %d" % (endx, w))
check(h == lcd.font_info(lcd.FONT_MONO16)[0], "text_size height")
check(count(WHITE) > 50, "text drew foreground pixels")
check(count(BLACK) > 0, "opaque text cell has background")
# monospace: width == chars * advance
check(lcd.text_size("iiii")[0] == lcd.text_size("MMMM")[0], "mono16 is monospaced")
check(lcd.text_size("iiii", font=lcd.FONT_SANS16)[0] < lcd.text_size("MMMM", font=lcd.FONT_SANS16)[0], "sans16 is proportional")
# anti-aliasing: opaque text on a bg shows in-between colours, transparent only fg
fresh()
lcd.text(10, 10, "Hag", WHITE, BLACK, font=lcd.FONT_SANS24B)
tw, th = lcd.text_size("Hag", font=lcd.FONT_SANS24B)
cols = set()
for yy in range(10, 10 + th):
    for xx in range(10, 10 + tw):
        cols.add(at(xx, yy))
check(len(cols) > 6, "opaque 4bpp text is anti-aliased (%d colours)" % len(cols))
fresh()
lcd.text(10, 10, "Hag", WHITE, font=lcd.FONT_SANS24B)          # transparent
cols = set()
for yy in range(10, 10 + th):
    for xx in range(10, 10 + tw):
        cols.add(at(xx, yy))
check(cols == {BLACK, WHITE}, "transparent text only fg/bg (%r)" % (cols,))
# transparent text must not erase what is underneath
fresh(BLUE)
lcd.text(10, 10, "Hag", WHITE, font=lcd.FONT_SANS24B)
check(at(10 + tw + 5, 12) == BLUE and count(WHITE) > 40, "transparent text keeps background")
# scale
fresh()
lcd.text(10, 10, "I", WHITE, font=lcd.FONT_MONO12)
n1 = count(WHITE)
fresh()
lcd.text(10, 10, "I", WHITE, font=lcd.FONT_MONO12, scale=3)
n3 = count(WHITE)
check(n3 == n1 * 9, "scale=3 -> 9x transparent pixels (%d vs %d)" % (n3, n1))
check(lcd.text_size("Hi", scale=3)[0] == 3 * lcd.text_size("Hi")[0], "text_size scales")
# newline + multi-line size
fresh()
lcd.text(10, 10, "ab\ncd", WHITE, BLACK)
w2, h2 = lcd.text_size("ab\ncd")
check(h2 == 2 * lcd.text_size("a")[1] and w2 == lcd.text_size("ab")[0], "multi-line size")
# unknown char falls back to '?', utf-8 doesn't crash, bytes accepted
fresh()
e1 = lcd.text(10, 10, "é€\u00ff", WHITE, BLACK)
check(e1 == 10 + 3 * lcd.text_size("?")[0], "non-covered code points fall back to '?'")
lcd.text(10, 40, b"bytes ok", WHITE, BLACK)
lcd.text(10, 60, "bad \xff\xfe utf8", WHITE, BLACK)
lcd.text(10, 80, "", WHITE, BLACK)
# digits font
fresh()
dx = lcd.text(0, 0, "12:34", WHITE, BLACK, font=lcd.FONT_DIGITS48)
check(dx == lcd.text_size("12:34", font=lcd.FONT_DIGITS48)[0] and dx > 100, "digits48 renders %d" % dx)
# clipping: partially / fully off-screen text never overruns or goes out of bounds
fresh()
lcd.text(-20, -8, "clipped", WHITE, BLACK, font=lcd.FONT_SANS24B)
lcd.text(470, 310, "clipped", WHITE, BLACK, font=lcd.FONT_SANS24B, scale=2)
lcd.text(-1000, 5, "gone", WHITE, BLACK)
lcd.text(5, 5000, "gone", WHITE, BLACK)
lcd.text(-30, -30, "tr", WHITE, font=lcd.FONT_SANS24B, scale=4)
lcd.text(460, 300, "tr", WHITE, font=lcd.FONT_SANS24B, scale=4)
clean("text clipping")
# a glyph clipped on the left must show the same pixels as the unclipped one, shifted
fresh()
lcd.text(100, 100, "W", WHITE, BLACK, font=lcd.FONT_MONO16)
ref = [[at(100 + i, 100 + j) for i in range(12)] for j in range(20)]
fresh()
lcd.text(-3, 100, "W", WHITE, BLACK, font=lcd.FONT_MONO16)
got = [[at(i, 100 + j) for i in range(9)] for j in range(20)]
check(all(got[j][i] == ref[j][i + 3] for j in range(20) for i in range(9)), "left-clipped glyph pixels line up")
# argument validation
for bad, exc in ((lambda: lcd.text(0, 0, "x", WHITE, scale=0), ValueError),
                 (lambda: lcd.text(0, 0, "x", WHITE, scale=17), ValueError),
                 (lambda: lcd.text(0, 0, "x", WHITE, font=99), ValueError),
                 (lambda: lcd.text(0, 0, "x", WHITE, font=b"nope"), ValueError),
                 (lambda: lcd.text(0, 0, 5, WHITE), TypeError),
                 (lambda: lcd.text(0, 0), TypeError),
                 (lambda: lcd.ellipse(1, 2, 3), TypeError)):
    try:
        bad()
        check(False, "expected %s" % exc.__name__)
    except exc:
        check(True, "arg validation")

# ---------------------------------------------------- user .zfont (hand built)
def make_zfont(glyphs, first, height=8, ascent=7, bpp=1, adv=8, blob_extra=b""):
    """glyphs: list of (w, h, xoff, yoff, rows_bytes) for first..first+len-1"""
    table, blob = b"", b""
    for (w, h, xo, yo, rows) in glyphs:
        table += struct.pack("<IBBBbb3x", len(blob), w, h, adv, xo, yo)
        blob += rows
    hdr = b"ZFNT" + struct.pack("<BBBBHH4x", 1, bpp, height, ascent, first, len(glyphs))
    return hdr + table + blob + blob_extra


stripe = bytes([0xFF, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0xFF])    # hollow 8x8 box
blank = (0, 0, 0, 0, b"")
f = make_zfont([blank, (8, 8, 0, 0, stripe)], 0x20)       # ' ' and '!'
fresh()
ex = lcd.text(20, 20, "!", WHITE, BLACK, font=f)
check(ex == 28, "custom font advance")
check(at(20, 20) == WHITE and at(27, 27) == WHITE and at(23, 23) == BLACK and at(21, 25) == BLACK and at(20, 25) == WHITE, "custom 1bpp glyph pixels")
check(count(WHITE) == 28, "custom glyph lit pixel count %d" % count(WHITE))
fresh()
lcd.text(20, 20, "! !", WHITE, BLACK, font=memoryview(f))     # memoryview accepted
check(count(WHITE) == 56 and at(36, 20) == WHITE, "custom font via memoryview, space advance")
check(lcd.font_info(f) == (8, 7, 0x20, 0x21, 1), "custom font_info %r" % (lcd.font_info(f),))
# corrupt entries must not read out of range: offset points past the blob
bad = make_zfont([blank, (8, 8, 0, 0, stripe[:3])], 0x20)       # claims 8 rows, only 3 present
fresh()
lcd.text(20, 20, "!", WHITE, BLACK, font=bad)
check(count(WHITE) == 0, "truncated glyph data is drawn blank, not read out of bounds")
for junk in (b"", b"ZFNT", f[:10], f[:20], b"XXXX" + f[4:], f[:4] + b"\x02" + f[5:], f[:5] + b"\x03" + f[6:]):
    try:
        lcd.text(0, 0, "!", WHITE, font=junk)
        check(False, "junk font must raise")
    except ValueError:
        check(True, "junk font rejected")
clean("custom fonts")

# a 4bpp custom font: nibble order and blending
glyph4 = bytes([0xF0, 0x0F, 0x80])      # 3 wide (2 bytes/row), 1.5 rows -> use 2 rows of 2 bytes
g4 = make_zfont([(3, 2, 0, 0, bytes([0xF0, 0x80, 0x0F, 0x00]))], 0x41, height=2, ascent=2, bpp=4, adv=3)
fresh()
lcd.text(10, 10, "A", WHITE, BLACK, font=g4)
check(at(10, 10) == WHITE and at(11, 10) == BLACK and at(12, 10) == lcd.blend(BLACK, WHITE, 8 * 17)
      and at(10, 11) == BLACK and at(11, 11) == WHITE and at(12, 11) == BLACK, "custom 4bpp nibble order + blend")

# ------------------------------------------------------------ transaction cost
fresh()
_, _, t0 = mocklcd.stats()
lcd.ellipse(200, 150, 100, 100, RED, True)
_, _, t1 = mocklcd.stats()
check(t1 - t0 < 4 * 200 * 5, "fill_ellipse transaction count sane (%d)" % (t1 - t0))
fresh()
_, _, t0 = mocklcd.stats()
lcd.text(0, 0, "Hello world", WHITE, BLACK, font=lcd.FONT_MONO16)
_, _, t1 = mocklcd.stats()
check(t1 - t0 < 11 * 60, "opaque text costs few bus transactions (%d)" % (t1 - t0))

print("passed %d, failed %d" % (passed, fails))
if fails:
    raise SystemExit(1)
print("ALL HOST TESTS PASSED")
