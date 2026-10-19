#!/usr/bin/env python3
"""ttf2zfont.py -- convert a TrueType/OpenType font to the Zeno .zfont bitmap format.

    python3 ttf2zfont.py DejaVuSansMono.ttf 16 -o mono16.zfont
    python3 ttf2zfont.py MyFont.ttf 24 --bpp 1 --first 32 --last 255 -o my24.zfont
    python3 ttf2zfont.py Clock.ttf 48 --first 0x20 --last 0x3A -o digits48.zfont

Format: see zeno_font.h. Needs Pillow (pip install pillow).

--bpp 4  anti-aliased (16 coverage levels; best with an opaque background)
--bpp 1  crisp 1-bit rendering (smaller; looks right on transparent text)
"""
import argparse
import struct
import sys

from PIL import Image, ImageDraw, ImageFont


def build(ttf, size, bpp, first, last, index=0):
    font = ImageFont.truetype(ttf, size, index=index)
    if bpp == 1:
        font.fontmode = "1"                 # no anti-aliasing: crisp 1-bit glyphs
    ascent, descent = font.getmetrics()
    height = ascent + descent
    if height > 255:
        sys.exit("font too tall for the format (max 255 px line height)")

    entries, blob = [], bytearray()
    for cp in range(first, last + 1):
        ch = chr(cp)
        adv = int(round(font.getlength(ch)))
        bbox = font.getbbox(ch, anchor="ls")          # relative to baseline origin
        w = h = xoff = yoff = 0
        rows = b""
        if bbox and bbox[2] > bbox[0] and bbox[3] > bbox[1]:
            x0, y0, x1, y1 = bbox
            img = Image.new("L", (x1 - x0, y1 - y0), 0)
            d = ImageDraw.Draw(img)
            d.fontmode = "1" if bpp == 1 else "L"
            d.text((-x0, -y0), ch, font=font, fill=255, anchor="ls")
            xoff, yoff = x0, ascent + y0                # top of glyph from top of cell
            # keep glyph inside the cell vertically
            if yoff < 0:
                img = img.crop((0, -yoff, img.width, img.height)); yoff = 0
            if yoff + img.height > height:
                img = img.crop((0, 0, img.width, height - yoff))
            # keep glyph inside the advance horizontally (widen instead of clipping)
            if xoff < 0:
                adv += -xoff; xoff = 0
            adv = max(adv, xoff + img.width)
            w, h = img.size
            if w > 255 or adv > 255:
                sys.exit(f"glyph U+{cp:04X} too wide for the format")
            px = img.load()
            out = bytearray()
            for yy in range(h):
                if bpp == 1:
                    for bx in range(0, w, 8):
                        v = 0
                        for k in range(8):
                            if bx + k < w and px[bx + k, yy] >= 128:
                                v |= 0x80 >> k
                        out.append(v)
                else:
                    for bx in range(0, w, 2):
                        hi = (px[bx, yy] * 15 + 127) // 255
                        lo = (px[bx + 1, yy] * 15 + 127) // 255 if bx + 1 < w else 0
                        out.append((hi << 4) | lo)
            rows = bytes(out)
        if adv > 255:
            sys.exit(f"advance of U+{cp:04X} too large")
        entries.append((len(blob), w, h, adv, xoff, yoff))
        blob += rows

    count = last - first + 1
    hdr = b"ZFNT" + struct.pack("<BBBBHH4x", 1, bpp, height, ascent, first, count)
    table = b"".join(struct.pack("<IBBBbb3x", *e) for e in entries)
    return hdr + table + bytes(blob), height, ascent


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("ttf")
    ap.add_argument("size", type=int, help="pixel size (em height)")
    ap.add_argument("-o", "--out", required=True)
    ap.add_argument("--bpp", type=int, choices=(1, 4), default=4)
    ap.add_argument("--first", type=lambda s: int(s, 0), default=32)
    ap.add_argument("--last", type=lambda s: int(s, 0), default=126)
    ap.add_argument("--index", type=int, default=0, help="face index for .ttc files")
    a = ap.parse_args()
    if not (0 <= a.first <= a.last <= 0xFFFF):
        sys.exit("bad --first/--last")
    data, h, asc = build(a.ttf, a.size, a.bpp, a.first, a.last, a.index)
    with open(a.out, "wb") as f:
        f.write(data)
    print(f"{a.out}: {len(data)} bytes, line height {h}, ascent {asc}, "
          f"U+{a.first:04X}..U+{a.last:04X}, {a.bpp}bpp")


if __name__ == "__main__":
    main()
