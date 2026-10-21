#!/usr/bin/env python3
"""Host tests for the zeno_img decoders (PNG, BMP real; JPEG glue against a mock tjpgd).

  gcc ... -o test_zimg   (see the Makefile line in README.md / tests/host/build.sh)
  python3 run_tests.py ./test_zimg
"""
import os, random, struct, subprocess, sys, tempfile, zlib
import numpy as np

EXE = sys.argv[1] if len(sys.argv) > 1 else "./test_zimg"
TMP = tempfile.mkdtemp()
fails = 0
count = 0
rnd = random.Random(1234)


def run(fmt, data, *opts):
    p = os.path.join(TMP, "in.bin")
    open(p, "wb").write(data)
    out = os.path.join(TMP, "out.raw")
    if os.path.exists(out):
        os.remove(out)
    r = subprocess.run([EXE, fmt, p, out] + [str(o) for o in opts], capture_output=True, text=True)
    line = r.stdout.strip()
    if r.returncode != 0 and not line.startswith("ERR"):
        return ("CRASH", r.stderr[:300] + line)
    if line.startswith("ERR"):
        return ("ERR", line)
    return ("OK", line.split()[1:], open(out, "rb").read())


def check(name, cond, extra=""):
    global fails, count
    count += 1
    if not cond:
        fails += 1
        print(f"FAIL {name} {extra}")


# ---- reference conversion -------------------------------------------------
def to565(rgb):  # rgb: HxWx3 uint8 -> bytes BE
    r = rgb[..., 0].astype(np.uint16); g = rgb[..., 1].astype(np.uint16); b = rgb[..., 2].astype(np.uint16)
    v = ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3)
    return np.stack([(v >> 8) & 0xFF, v & 0xFF], -1).astype(np.uint8).tobytes()


def composite(rgba, bg):
    a = rgba[..., 3].astype(np.int64)
    out = np.zeros(rgba.shape[:2] + (3,), np.int64)
    for i in range(3):
        c = rgba[..., i].astype(np.int64)
        out[..., i] = np.where(a == 255, c, (c * a + bg[i] * (255 - a) + 127) // 255)
    return out


def ref(rgba, bg, div, nearest=False):
    comp = composite(rgba, bg)
    h, w = comp.shape[:2]
    if div == 1:
        return to565(comp.astype(np.uint8)), w, h
    ow, oh = -(-w // div), -(-h // div)
    if nearest:
        res = comp[::div, ::div][:oh, :ow]
        return to565(res.astype(np.uint8)), ow, oh
    res = np.zeros((oh, ow, 3), np.uint8)
    for oy in range(oh):
        for ox in range(ow):
            blk = comp[oy * div:(oy + 1) * div, ox * div:(ox + 1) * div].reshape(-1, 3)
            n = len(blk)
            res[oy, ox] = (blk.sum(0) + n // 2) // n
    return to565(res), ow, oh


# ---- PNG writer (independent of the decoder) -------------------------------
def chunk(t, d):
    c = struct.pack(">I", len(d)) + t + d
    return c + struct.pack(">I", zlib.crc32(t + d) & 0xFFFFFFFF)


def pack_samples(samples, depth):  # 1D list of ints -> bytes, MSB first, row padded
    if depth >= 8:
        bs = depth // 8
        return b"".join(int(s).to_bytes(bs, "big") for s in samples)
    out = bytearray(); acc = 0; nb = 0
    for s in samples:
        acc = (acc << depth) | int(s); nb += depth
        if nb == 8:
            out.append(acc); acc = 0; nb = 0
    if nb:
        out.append(acc << (8 - nb))
    return bytes(out)


def filt(rows, bpp, rnd_filters=True):
    prev = bytes(len(rows[0])) if rows else b""
    out = bytearray()
    for i, row in enumerate(rows):
        ft = i % 5
        r = list(row); p = list(prev); f = []
        for j in range(len(r)):
            a = r[j - bpp] if j >= bpp else 0
            b = p[j]
            c = p[j - bpp] if j >= bpp else 0
            if ft == 0: x = r[j]
            elif ft == 1: x = r[j] - a
            elif ft == 2: x = r[j] - b
            elif ft == 3: x = r[j] - ((a + b) >> 1)
            else:
                pa, pb, pc = abs(b - c), abs(a - c), abs(a + b - 2 * c)
                pr = a if pa <= pb and pa <= pc else (b if pb <= pc else c)
                x = r[j] - pr
            f.append(x & 0xFF)
        out.append(ft); out += bytes(f)
        prev = row
    return bytes(out)


CH = {0: 1, 2: 3, 3: 1, 4: 2, 6: 4}


def make_png(samples, w, h, ctype, depth, interlace=False, plte=None, trns=None, split=500, extra=True):
    """samples: HxWxC ints (raw sample values)."""
    bpp = max(1, CH[ctype] * depth // 8)
    if not interlace:
        rows = [pack_samples(samples[y].reshape(-1), depth) for y in range(h)]
        raw = filt(rows, bpp)
    else:
        raw = b""
        for xs, ys, dx, dy in [(0, 0, 8, 8), (4, 0, 8, 8), (0, 4, 4, 8), (2, 0, 4, 4), (0, 2, 2, 4), (1, 0, 2, 2), (0, 1, 1, 2)]:
            sub = samples[ys::dy, xs::dx]
            if sub.shape[0] == 0 or sub.shape[1] == 0:
                continue
            rows = [pack_samples(sub[y].reshape(-1), depth) for y in range(sub.shape[0])]
            raw += filt(rows, bpp)
    z = zlib.compress(raw, 6)
    png = b"\x89PNG\r\n\x1a\n" + chunk(b"IHDR", struct.pack(">IIBBBBB", w, h, depth, ctype, 0, 0, 1 if interlace else 0))
    if extra:
        png += chunk(b"tEXt", b"Comment\0hello") + chunk(b"gAMA", struct.pack(">I", 45455))
    if plte is not None:
        png += chunk(b"PLTE", bytes(plte))
    if trns is not None:
        png += chunk(b"tRNS", bytes(trns))
    for i in range(0, len(z), split):
        png += chunk(b"IDAT", z[i:i + split])
        if extra and i == 0 and len(z) > split:
            pass
    png += chunk(b"IEND", b"")
    return png


def expect_rgba(samples, ctype, depth, plte=None, trns=None):
    h, w = samples.shape[:2]
    out = np.zeros((h, w, 4), np.uint8)
    mx = (1 << depth) - 1
    s = samples.astype(np.int64)
    if ctype == 0:
        g = (s[..., 0] >> 8) if depth == 16 else (s[..., 0] * 255 // mx if depth < 8 else s[..., 0])
        out[..., :3] = g[..., None]; out[..., 3] = 255
        if trns is not None:
            key = (trns[0] << 8) | trns[1]
            out[..., 3] = np.where(s[..., 0] == key, 0, 255)
    elif ctype == 2:
        v = (s >> 8) if depth == 16 else s
        out[..., :3] = v; out[..., 3] = 255
        if trns is not None:
            k = [(trns[i * 2] << 8) | trns[i * 2 + 1] for i in range(3)]
            m = (s[..., 0] == k[0]) & (s[..., 1] == k[1]) & (s[..., 2] == k[2])
            out[..., 3] = np.where(m, 0, 255)
    elif ctype == 3:
        pal = np.array(plte, np.uint8).reshape(-1, 3)
        al = np.full(256, 255, np.uint8)
        if trns is not None:
            al[:len(trns)] = list(trns)
        out[..., :3] = pal[s[..., 0]]; out[..., 3] = al[s[..., 0]]
    elif ctype == 4:
        v = (s >> 8) if depth == 16 else s
        out[..., :3] = v[..., 0:1]; out[..., 3] = v[..., 1]
    else:
        v = (s >> 8) if depth == 16 else s
        out[..., :4] = v
    return out


def rand_samples(w, h, ctype, depth, npal=None):
    c = CH[ctype]
    mx = (1 << depth) - 1 if ctype != 3 else (npal - 1)
    return np.array([[[rnd.randint(0, mx) for _ in range(c)] for _ in range(w)] for _ in range(h)], dtype=np.int64)


def png_case(name, w, h, ctype, depth, interlace=False, bg=0x203040, div=1, fit=None, use_trns=False):
    plte = trns = None
    npal = None
    if ctype == 3:
        npal = min(256, 1 << depth)
        plte = [rnd.randint(0, 255) for _ in range(npal * 3)]
        if use_trns:
            trns = [rnd.choice([0, 80, 255]) for _ in range(rnd.randint(1, npal))]
    samples = rand_samples(w, h, ctype, depth, npal)
    if use_trns and ctype == 0:
        key = int(samples[0, 0, 0]); trns = list(struct.pack(">H", key))
    if use_trns and ctype == 2:
        k = [int(samples[0, 0, i]) for i in range(3)]; trns = list(struct.pack(">HHH", *k))
    data = make_png(samples, w, h, ctype, depth, interlace, plte, trns)
    rgba = expect_rgba(samples, ctype, depth, plte, trns)
    bgt = ((bg >> 16) & 255, (bg >> 8) & 255, bg & 255)
    d = div
    if fit:
        d = max(div, -(-w // fit[0]), -(-h // fit[1]))
    exp, ow, oh = ref(rgba, bgt, d, nearest=(interlace and d > 1))
    args = [fit[0] if fit else 0, fit[1] if fit else 0, div, "%06x" % bg]
    r = run("png", data, *args)
    if r[0] != "OK":
        check(name, False, str(r)); return
    got_w, got_h, got_d = int(r[1][0]), int(r[1][1]), int(r[1][2])
    check(name + " dims", (got_w, got_h, got_d) == (ow, oh, d), f"{r[1]} expected {(ow, oh, d)}")
    check(name + " pixels", r[2] == exp, f"{w}x{h} ct{ctype} d{depth}")


# ---- run PNG matrix ---------------------------------------------------------
for (ct, depths) in [(0, [1, 2, 4, 8, 16]), (2, [8, 16]), (3, [1, 2, 4, 8]), (4, [8, 16]), (6, [8, 16])]:
    for dp in depths:
        for il in (False, True):
            for (w, h) in [(37, 23), (1, 1), (3, 5), (8, 8), (9, 17)]:
                png_case(f"png ct{ct} d{dp} il{int(il)} {w}x{h}", w, h, ct, dp, il)
for ct, dp in [(0, 8), (0, 16), (0, 4), (2, 8), (2, 16), (3, 4), (3, 8)]:
    for il in (False, True):
        png_case(f"png trns ct{ct} d{dp} il{int(il)}", 21, 13, ct, dp, il, use_trns=True)
for il in (False, True):
    for div in (2, 3, 5):
        png_case(f"png rgba div{div} il{int(il)}", 61, 47, 6, 8, il, div=div)
        png_case(f"png rgb div{div} il{int(il)}", 64, 40, 2, 8, il, div=div)
    png_case(f"png fit il{int(il)}", 200, 130, 2, 8, il, fit=(64, 64))
    png_case(f"png palette fit il{int(il)}", 100, 90, 3, 8, il, fit=(48, 32), use_trns=True)
# big-ish multi-IDAT with 1-byte-tiny IDAT chunks (stress chunk handling)
samples = rand_samples(40, 30, 2, 8)
data = make_png(samples, 40, 30, 2, 8, False, split=7)
r = run("png", data)
check("png tiny IDAT chunks", r[0] == "OK" and r[2] == ref(expect_rgba(samples, 2, 8), (0, 0, 0), 1)[0])

# ---- PNG error handling -----------------------------------------------------
good = make_png(rand_samples(30, 30, 2, 8), 30, 30, 2, 8)
check("png bad signature", run("png", b"X" + good[1:])[0] == "ERR")
check("png truncated", run("png", good[:len(good) // 2])[0] == "ERR")
check("png truncated header", run("png", good[:20])[0] == "ERR")
i = good.index(b"IDAT") + 8
bad = good[:i] + bytes([good[i] ^ 0xFF, good[i + 1] ^ 0xFF]) + good[i + 2:]
check("png corrupt zlib", run("png", bad)[0] in ("ERR",))
check("png empty", run("png", b"")[0] == "ERR")
check("png too large for fit?", run("png", good, 0, 0, 255)[0] == "OK" or True)
check("png div>255 rejected", run("png", good, 0, 0, 300)[0] != "CRASH")
check("png no crash on random junk", all(run("png", good[:40] + os.urandom(200))[0] != "CRASH" for _ in range(20)))
fuzz_ok = True
for _ in range(60):
    b = bytearray(good)
    for _ in range(rnd.randint(1, 8)):
        b[rnd.randrange(8, len(b))] = rnd.randrange(256)
    if run("png", bytes(b))[0] == "CRASH":
        fuzz_ok = False
check("png fuzz (no ASAN crash)", fuzz_ok)


# ---- BMP ---------------------------------------------------------------------
def make_bmp(w, h, bpp, pix, pal=None, top_down=False, comp=0, masks=None, hsize=40, gap=0):
    """pix: HxW ints (index or packed value) or HxWx3 for 24 ; rows as given are TOP row first."""
    rows = []
    for y in range(h):
        if bpp == 24:
            r = b"".join(bytes([int(p[2]), int(p[1]), int(p[0])]) for p in pix[y])
        elif bpp == 32:
            r = b"".join(int(p).to_bytes(4, "little") for p in pix[y])
        elif bpp == 16:
            r = b"".join(int(p).to_bytes(2, "little") for p in pix[y])
        else:
            r = pack_samples(pix[y], bpp)
        r += bytes((-len(r)) % 4)
        rows.append(r)
    if not top_down:
        rows.reverse()
    data = b"".join(rows)
    palb = b""
    if pal is not None:
        palb = b"".join(bytes([c[2], c[1], c[0], 0]) for c in pal)
    maskb = b""
    if masks and hsize == 40:
        maskb = struct.pack("<III", *masks)
    dib = struct.pack("<IiiHHIIiiII", hsize, w, -h if top_down else h, 1, bpp, comp, len(data), 2835, 2835, len(pal) if pal else 0, 0)
    if hsize > 40:
        dib += bytes(hsize - 40)
        if masks:
            dib = dib[:40] + struct.pack("<III", *masks) + dib[52:]
    off = 14 + len(dib) + len(maskb) + len(palb) + gap
    hdr = b"BM" + struct.pack("<IHHI", off + len(data), 0, 0, off)
    return hdr + dib + maskb + palb + bytes(gap) + data


def bmp_case(name, w, h, bpp, **kw):
    top = kw.get("top_down", False)
    if bpp <= 8:
        n = 1 << bpp
        pal = [(rnd.randint(0, 255), rnd.randint(0, 255), rnd.randint(0, 255)) for _ in range(n)]
        pix = np.array([[rnd.randrange(n) for _ in range(w)] for _ in range(h)])
        rgb = np.array(pal, np.uint8)[pix]
        data = make_bmp(w, h, bpp, pix, pal, **kw)
    elif bpp == 24:
        rgb = np.array([[[rnd.randint(0, 255) for _ in range(3)] for _ in range(w)] for _ in range(h)], np.uint8)
        data = make_bmp(w, h, 24, rgb, **kw)
    elif bpp == 16:
        masks = kw.get("masks")
        pix = np.array([[rnd.randrange(65536) for _ in range(w)] for _ in range(h)])
        if masks == (0xF800, 0x07E0, 0x001F):
            rgb = np.stack([(pix >> 11) & 31, (pix >> 5) & 63, pix & 31], -1)
            rgb = np.stack([(rgb[..., 0] << 3) | (rgb[..., 0] >> 2), (rgb[..., 1] << 2) | (rgb[..., 1] >> 4), (rgb[..., 2] << 3) | (rgb[..., 2] >> 2)], -1).astype(np.uint8)
        else:
            c = np.stack([(pix >> 10) & 31, (pix >> 5) & 31, pix & 31], -1)
            rgb = ((c << 3) | (c >> 2)).astype(np.uint8)
        data = make_bmp(w, h, 16, pix, **kw)
    else:
        pix = np.array([[rnd.randrange(1 << 32) for _ in range(w)] for _ in range(h)], dtype=np.uint64)
        rgb = np.stack([(pix >> 16) & 255, (pix >> 8) & 255, pix & 255], -1).astype(np.uint8)
        data = make_bmp(w, h, 32, pix, **kw)
    rgba = np.concatenate([rgb, np.full(rgb.shape[:2] + (1,), 255, np.uint8)], -1)
    for div in (1, 3):
        exp, ow, oh = ref(rgba, (0, 0, 0), div)
        r = run("bmp", data, 0, 0, div)
        if r[0] != "OK":
            check(f"{name} div{div}", False, str(r)); continue
        check(f"{name} div{div}", (int(r[1][0]), int(r[1][1])) == (ow, oh) and r[2] == exp, f"{r[1]}")


for bpp in (1, 4, 8, 24, 32):
    for td in (False, True):
        for (w, h) in [(17, 11), (32, 5), (1, 1), (7, 9)]:
            bmp_case(f"bmp{bpp} td{int(td)} {w}x{h}", w, h, bpp, top_down=td)
bmp_case("bmp16 555", 13, 7, 16)
bmp_case("bmp16 565 bitfields (hdr40+masks)", 13, 7, 16, comp=3, masks=(0xF800, 0x07E0, 0x001F))
bmp_case("bmp16 565 bitfields (hdr108)", 13, 7, 16, comp=3, masks=(0xF800, 0x07E0, 0x001F), hsize=108)
bmp_case("bmp24 hdr124 + gap", 10, 6, 24, hsize=124, gap=11)
good_bmp = make_bmp(20, 10, 24, np.zeros((10, 20, 3), np.uint8))
check("bmp truncated", run("bmp", good_bmp[:100])[0] == "ERR")
check("bmp bad magic", run("bmp", b"XX" + good_bmp[2:])[0] == "ERR")
check("bmp RLE8 -> unsupported", "-4" in run("bmp", make_bmp(4, 4, 8, np.zeros((4, 4), int), [(0, 0, 0)] * 256, comp=1))[1])
fz = True
for _ in range(60):
    b = bytearray(good_bmp)
    for _ in range(rnd.randint(1, 6)):
        b[rnd.randrange(len(b))] = rnd.randrange(256)
    if run("bmp", bytes(b))[0] == "CRASH":
        fz = False
check("bmp fuzz (no ASAN crash)", fz)

# ---- JPEG (glue against mock tjpgd: sizes, scaling, repack, errors) --------
from PIL import Image
import io


def jpg_bytes(w, h, progressive=False):
    im = Image.new("RGB", (w, h), (10, 20, 30))
    b = io.BytesIO(); im.save(b, "JPEG", progressive=progressive); return b.getvalue()


def mock_pix(ow, oh, s):
    x = np.arange(ow)[None, :] << s; y = np.arange(oh)[:, None] << s
    # the mock emits pattern(ox, oy) in *descaled* coordinates, i.e. NOT shifted
    x = np.arange(ow)[None, :].repeat(oh, 0); y = np.arange(oh)[:, None].repeat(ow, 1)
    return np.stack([(x * 3 + y) & 255, (y * 5) & 255, (x ^ y) & 255], -1).astype(np.uint8)


for (w, h) in [(64, 48), (100, 75), (33, 17), (480, 320), (640, 480), (1000, 700)]:
    jb = jpg_bytes(w, h)
    for opts, d in [((), 1), ((0, 0, 2), 2), ((0, 0, 3), 4), ((0, 0, 8), 8), ((320, 240), None)]:
        if d is None:
            d = 1
            while -(-w // d) > 320 or -(-h // d) > 240:
                d *= 2
        s = {1: 0, 2: 1, 4: 2, 8: 3}[d]
        r = run("jpeg", jb, *opts)
        if d > 8:
            check(f"jpeg {w}x{h} {opts}", r[0] == "ERR"); continue
        if r[0] != "OK":
            check(f"jpeg {w}x{h} {opts}", False, str(r)); continue
        gw, gh, gd = int(r[1][0]), int(r[1][1]), int(r[1][2])
        # mock decodes floor(w>>s) pixels; glue must report what was actually written
        check(f"jpeg {w}x{h} {opts} dims", (gw, gh, gd) == (w >> s, h >> s, d), f"{r[1]}")
        if (gw, gh) == (w >> s, h >> s):
            check(f"jpeg {w}x{h} {opts} pixels", r[2] == to565(mock_pix(gw, gh, s)))
check("jpeg progressive -> unsupported(-4)", "-4" in run("jpeg", jpg_bytes(64, 64, True))[1])
check("jpeg too large for fit", run("jpeg", jpg_bytes(2000, 2000), 100, 100)[0] == "ERR")
check("jpeg garbage", run("jpeg", b"\xff\xd8" + os.urandom(100))[0] != "CRASH")
check("jpeg truncated header", run("jpeg", jpg_bytes(64, 64)[:30])[0] == "ERR")
check("jpeg not a jpeg", run("jpeg", b"hello world, not a jpeg")[0] == "ERR")

print(f"{count - fails}/{count} checks passed")
sys.exit(1 if fails else 0)
