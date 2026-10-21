# img_demo.py -- load png / jpeg / bmp and blit to the zeno lcd
import lcd, png, jpeg, bmp, img

lcd.init(rotation=1)                      # 480 x 320
W, H = lcd.width(), lcd.height()
lcd.fill(0x0000)

def show(p, x=None, y=None):
    # centre if no position given; p.buf is RGB565 big-endian, len == w*h*2
    x = (W - p.width) // 2 if x is None else x
    y = (H - p.height) // 2 if y is None else y
    lcd.blit(x, y, p.width, p.height, p.buf)

# 1. exactly what you asked for
p = png.open("/sd/logo.png")              # or "/logo.png" on flash
show(p)

# 2. big photo: let the module shrink it to fit the screen (box filter for
#    png/bmp, native 1/2 1/4 1/8 for jpeg)
p = jpeg.open("/sd/photo.jpg", fit=(W, H))
print(p, p.src_size, p.div)
show(p)

# 3. transparent PNG icon over a coloured background (0xRRGGBB, not RGB565)
p = png.open("/sd/icon.png", bg=0x203040)
show(p, 10, 10)

# 4. don't know the type? sniffed from the first bytes, not the extension
p = img.open("/sd/anything", fit=(W, H))
show(p)

# 5. header only, no RAM spent on pixels
print(png.info("/sd/logo.png"))           # (width, height)

# 6. from bytes / an already-open file
p = bmp.open(open("/sd/pic.bmp", "rb"))
q = png.decode(b"...png bytes...") if False else None
