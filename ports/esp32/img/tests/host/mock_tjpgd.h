/* Stand-in for ROM rom/tjpgd.h, host tests only. Same names/signatures as the
 * ROM API that zimg_jpeg.c uses; it does NOT decode JPEG data (see mock_tjpgd.c). */
#ifndef MOCK_TJPGD_H
#define MOCK_TJPGD_H
#include <stdint.h>
typedef enum { JDR_OK = 0, JDR_INTR, JDR_INP, JDR_MEM1, JDR_MEM2, JDR_PAR, JDR_FMT1, JDR_FMT2, JDR_FMT3 } JRESULT;
typedef struct { uint16_t left, right, top, bottom; } JRECT;
typedef struct JDEC {
    unsigned int dctr;
    uint16_t width, height;
    void *device;
    unsigned int (*infunc)(struct JDEC *, uint8_t *, unsigned int);
    uint8_t scale;
} JDEC;
JRESULT jd_prepare(JDEC *jd, unsigned int (*infunc)(JDEC *, uint8_t *, unsigned int),
                   void *pool, unsigned int sz_pool, void *dev);
JRESULT jd_decomp(JDEC *jd, unsigned int (*outfunc)(JDEC *, void *, JRECT *), uint8_t scale);
#endif
