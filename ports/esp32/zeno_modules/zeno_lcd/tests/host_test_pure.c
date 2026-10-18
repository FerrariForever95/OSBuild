/*
 * host_test_pure.c -- unit tests for zeno_pure.h that run on a desktop
 * compiler, no ESP-IDF required.
 *
 * Build & run:
 *   cc -std=c11 -Wall -Wextra -I.. -o host_test_pure host_test_pure.c -lm
 *   ./host_test_pure
 */
#include <stdio.h>
#include <math.h>
#include <assert.h>
#include "../zeno_pure.h"

static int failures = 0;
#define CHECK(cond) do { \
    if (!(cond)) { printf("FAIL %s:%d: %s\n", __FILE__, __LINE__, #cond); failures++; } \
} while (0)

static void test_lut(void)
{
    uint8_t pins[8] = { 0, 1, 2, 3, 4, 5, 6, 7 };
    uint32_t lut[256];
    uint32_t mask = zl_lut_build(pins, lut);
    CHECK(mask == 0xFF);
    CHECK(lut[0x00] == 0x00);
    CHECK(lut[0xFF] == 0xFF);
    CHECK(lut[0x01] == 0x01);
    CHECK(lut[0x80] == 0x80);
    CHECK(lut[0xA5] == 0xA5);

    uint8_t pins2[8] = { 8, 15, 11, 10, 9, 4, 18, 17 }; /* real D0-D7 pins */
    uint32_t lut2[256];
    uint32_t mask2 = zl_lut_build(pins2, lut2);
    uint32_t expect_mask = 0;
    for (int i = 0; i < 8; i++) expect_mask |= (1u << pins2[i]);
    CHECK(mask2 == expect_mask);
    /* byte 0x01 -> bit0 (D0) -> pin 8 */
    CHECK(lut2[0x01] == (1u << 8));
    /* byte 0x02 -> bit1 (D1) -> pin 15 */
    CHECK(lut2[0x02] == (1u << 15));
    /* byte 0xFF -> all pins */
    CHECK(lut2[0xFF] == expect_mask);
}

static void test_touch_combine(void)
{
    CHECK(zt_trimmed_mean((int[]){1,2,3,4}, 4) == (1+2+3+4-1-4)/2);
    CHECK(zt_trimmed_mean((int[]){100,110,120,130}, 4) == 115);
    CHECK(zt_trimmed_mean((int[]){5,5}, 2) == 0);       /* n<=2 -> 0, as original */

    CHECK(zt_axis_combine(100, 101, 2) == 201);          /* within tolerance -> sum */
    CHECK(zt_axis_combine(100, 101, 0) == 0);            /* not exactly equal -> reject */
    CHECK(zt_axis_combine(100, 200, 2) == 0);            /* far apart -> reject */

    CHECK(zt_median3(3, 1, 2) == 2);
    CHECK(zt_median3(1, 2, 3) == 2);
    CHECK(zt_median3(5, 5, 5) == 5);
}

static void test_touch_valid_and_debounce(void)
{
    zt_config_t c = { .min_raw = 100, .z_max = 4095, .settle_us = 0, .down_confirm = 2, .up_confirm = 3 };
    CHECK(zt_sample_valid(500, 500, 10, &c) == true);
    CHECK(zt_sample_valid(0, 500, 10, &c) == false);
    CHECK(zt_sample_valid(500, 0, 10, &c) == false);
    CHECK(zt_sample_valid(50, 500, 10, &c) == false);     /* below min_raw */

    zt_config_t cz = c; cz.z_max = 50;
    CHECK(zt_sample_valid(500, 500, 10, &cz) == true);
    CHECK(zt_sample_valid(500, 500, 100, &cz) == false);  /* z gate trips */

    zt_debounce_t s = {0};
    CHECK(zt_debounce_step(&s, true, 2, 3) == false);   /* 1st valid: not yet */
    CHECK(zt_debounce_step(&s, true, 2, 3) == true);    /* 2nd valid: now down */
    CHECK(zt_debounce_step(&s, false, 2, 3) == true);    /* 1 invalid: still down */
    CHECK(zt_debounce_step(&s, false, 2, 3) == true);    /* 2 invalid: still down */
    CHECK(zt_debounce_step(&s, false, 2, 3) == false);   /* 3 invalid: now up */
}

static void test_calib_identity(void)
{
    /* Raw == screen exactly: solved transform must be the identity. */
    double rx[5] = {0, 100, 0, 100, 50};
    double ry[5] = {0, 0, 100, 100, 50};
    double sx[5] = {0, 100, 0, 100, 50};
    double sy[5] = {0, 0, 100, 100, 50};
    zt_cal_t k; double rms, me;
    CHECK(zt_cal_solve(rx, ry, sx, sy, 5, &k, &rms, &me) == true);
    CHECK(fabs(k.a - 1.0) < 1e-6 && fabs(k.e - 1.0) < 1e-6);
    CHECK(fabs(k.b) < 1e-6 && fabs(k.d) < 1e-6);
    CHECK(fabs(k.c) < 1e-6 && fabs(k.f) < 1e-6);
    CHECK(rms < 1e-6 && me < 1e-6);

    int ox, oy;
    zt_cal_apply(&k, 75, 25, 320, 480, &ox, &oy);
    CHECK(ox == 75 && oy == 25);
}

static void test_calib_offset_scale(void)
{
    /* screen = 2*raw + 10 on both axes: affine fit must recover that. */
    double rx[4] = {0, 50, 0, 50};
    double ry[4] = {0, 0, 50, 50};
    double sx[4], sy[4];
    for (int i = 0; i < 4; i++) { sx[i] = 2*rx[i] + 10; sy[i] = 2*ry[i] + 10; }
    zt_cal_t k; double rms, me;
    CHECK(zt_cal_solve(rx, ry, sx, sy, 4, &k, &rms, &me) == true);
    CHECK(fabs(k.a - 2.0) < 1e-6 && fabs(k.e - 2.0) < 1e-6);
    CHECK(fabs(k.c - 10.0) < 1e-6 && fabs(k.f - 10.0) < 1e-6);
    CHECK(rms < 1e-6);
}

static void test_calib_degenerate(void)
{
    /* collinear points -> solver must refuse rather than fabricate garbage */
    double rx[3] = {0, 50, 100};
    double ry[3] = {0, 0, 0};
    double sx[3] = {0, 50, 100};
    double sy[3] = {0, 0, 0};
    zt_cal_t k; double rms, me;
    CHECK(zt_cal_solve(rx, ry, sx, sy, 3, &k, &rms, &me) == false);
    CHECK(zt_cal_solve(rx, ry, sx, sy, 2, &k, &rms, &me) == false); /* n<3 */
}

static void test_calib_clamp(void)
{
    double rx[3] = {0, 100, 0};
    double ry[3] = {0, 0, 100};
    double sx[3] = {0, 100, 0};
    double sy[3] = {0, 0, 100};
    zt_cal_t k; double rms, me;
    CHECK(zt_cal_solve(rx, ry, sx, sy, 3, &k, &rms, &me) == true);
    int x, y;
    zt_cal_apply(&k, -50, -50, 320, 480, &x, &y);
    CHECK(x == 0 && y == 0);
    zt_cal_apply(&k, 10000, 10000, 320, 480, &x, &y);
    CHECK(x == 319 && y == 479);
}

int main(void)
{
    test_lut();
    test_touch_combine();
    test_touch_valid_and_debounce();
    test_calib_identity();
    test_calib_offset_scale();
    test_calib_degenerate();
    test_calib_clamp();

    if (failures == 0) {
        printf("ALL TESTS PASSED\n");
        return 0;
    }
    printf("%d TEST(S) FAILED\n", failures);
    return 1;
}
