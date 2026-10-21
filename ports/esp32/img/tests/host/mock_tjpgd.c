/* Host mock of ROM TJpgDec: parses SOI..SOF only (through the real input
 * callback, so reader/skip glue is exercised), then "decodes" by emitting a
 * deterministic pattern in 16x16 MCU rectangles, honouring scale and using a
 * floor()ed descaled size so the repack path in zimg_jpeg.c is exercised. */
#include <string.h>
#include "mock_tjpgd.h"

JRESULT jd_prepare(JDEC *jd, unsigned int (*in)(JDEC *, uint8_t *, unsigned int),
                   void *pool, unsigned int sz, void *dev)
{
    (void)pool;
    if (sz < 3100) return JDR_MEM1;
    memset(jd, 0, sizeof(*jd));
    jd->device = dev; jd->infunc = in;
    uint8_t b[4];
    if (in(jd, b, 2) != 2 || b[0] != 0xFF || b[1] != 0xD8) return JDR_FMT1;
    for (;;) {
        uint8_t m;
        do { if (in(jd, &m, 1) != 1) return JDR_INP; } while (m != 0xFF);
        do { if (in(jd, &m, 1) != 1) return JDR_INP; } while (m == 0xFF);
        if (m == 0xDA || m == 0xD9) return JDR_FMT1;
        if (in(jd, b, 2) != 2) return JDR_INP;
        unsigned len = (b[0] << 8) | b[1];
        if (m == 0xC2) return JDR_FMT3;           /* progressive */
        if (m == 0xC0) {
            uint8_t s[5];
            if (in(jd, s, 5) != 5) return JDR_INP;
            jd->height = (uint16_t)((s[1] << 8) | s[2]);
            jd->width = (uint16_t)((s[3] << 8) | s[4]);
            return JDR_OK;
        }
        if (in(jd, NULL, len - 2) != len - 2) return JDR_INP;   /* exercises skip */
    }
}

JRESULT jd_decomp(JDEC *jd, unsigned int (*out)(JDEC *, void *, JRECT *), uint8_t scale)
{
    unsigned sw = jd->width >> scale, sh = jd->height >> scale;   /* floor */
    unsigned mcu = 16u >> scale;
    uint8_t buf[16 * 16 * 3];
    for (unsigned y = 0; y < sh; y += mcu) {
        for (unsigned x = 0; x < sw; x += mcu) {
            unsigned rw = (x + mcu <= sw) ? mcu : sw - x, rh = (y + mcu <= sh) ? mcu : sh - y;
            uint8_t *p = buf;
            for (unsigned j = 0; j < rh; j++)
                for (unsigned i = 0; i < rw; i++, p += 3) {
                    unsigned ox = x + i, oy = y + j;
                    p[0] = (uint8_t)(ox * 3 + oy); p[1] = (uint8_t)(oy * 5); p[2] = (uint8_t)(ox ^ oy);
                }
            JRECT r = {(uint16_t)x, (uint16_t)(x + rw - 1), (uint16_t)y, (uint16_t)(y + rh - 1)};
            if (!out(jd, buf, &r)) return JDR_INTR;
        }
    }
    return JDR_OK;
}
