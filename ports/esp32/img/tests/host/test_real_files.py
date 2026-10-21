#!/usr/bin/env python3
"""Decode real files with test_zimg and compare PNG/BMP against Pillow (JPEG: size only, host mock)."""
import subprocess, sys, os, glob
import numpy as np
from PIL import Image
exe, folder = sys.argv[1], sys.argv[2]
bad = 0
for f in sorted(glob.glob(folder + "/*")):
    ext = f.rsplit(".", 1)[-1].lower()
    fmt = {"png": "png", "bmp": "bmp", "jpeg": "jpeg", "jpg": "jpeg"}.get(ext)
    if not fmt: continue
    out = "/tmp/o.raw"
    r = subprocess.run([exe, fmt, f, out, "0", "0", "1", "000000"], capture_output=True, text=True)
    line = r.stdout.strip()
    im = Image.open(f)
    if fmt == "jpeg":
        ok = line.startswith("OK") and tuple(map(int, line.split()[1:3])) == im.size
        print(("ok  " if ok else "FAIL"), os.path.basename(f), line, "(JPEG: size check only; mock decoder)")
        bad += not ok; continue
    rgba = np.array(im.convert("RGBA")).astype(np.int64)
    a = rgba[..., 3]
    rgb = np.where(a[..., None] == 255, rgba[..., :3], (rgba[..., :3] * a[..., None] + 127) // 255)
    r5 = ((rgb[..., 0] & 0xF8) << 8) | ((rgb[..., 1] & 0xFC) << 3) | (rgb[..., 2] >> 3)
    exp = np.stack([(r5 >> 8) & 255, r5 & 255], -1).astype(np.uint8).tobytes()
    ok = line.startswith("OK") and open(out, "rb").read() == exp
    print(("ok  " if ok else "FAIL"), os.path.basename(f), line, im.mode)
    bad += not ok
sys.exit(bad)
