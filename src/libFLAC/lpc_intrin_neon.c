#include "private/cpu.h"

#ifndef FLAC__INTEGER_ONLY_LIBRARY
#ifndef FLAC__NO_ASM
#if defined FLAC__CPU_AARCH64 && FLAC__HAS_NEONINTRIN
#include "private/lpc.h"
#include "FLAC/assert.h"
#include "FLAC/format.h"
#include <arm_neon.h>

inline float32x4_t shufffleVector(float32x4_t vec)
{
    float32x2_t hi = vget_high_f32(vec);
    float32x2_t lo = vget_low_f32(vec);
    float32x2x2_t qr0_z = vzip_f32(hi, lo);
    return vcombine_f32(qr0_z.val[0], qr0_z.val[1]);
}

inline float32x4_t shufffleVector_2103(float32x4_t vec)
{
    float32_t *tempPtr;
    float32_t temp = vgetq_lane_f32(vec, 0);

    vec = vcopyq_laneq_f32(vec, 0, vec, 2);
    tempPtr = &temp;
    vst1q_lane_f32(tempPtr, vec, 2);
    return vec;
}

void FLAC__lpc_compute_autocorrelation_intrin_neon_lag_4(const FLAC__real data[], uint32_t data_len, uint32_t lag, FLAC__real autoc[])
{
    int i;
    int limit = data_len - 4;
    float32x4_t sum0 = vdupq_n_f32(0.0f);

    (void)lag;
    FLAC__ASSERT(lag <= 4);
    FLAC__ASSERT(lag <= data_len);

    for (i = 0; i <= limit; i++)
    {
        float32x4_t d, d0;
        d0 = vld1q_f32(data + i);
        d = shufffleVector(d0);
        sum0 = vaddq_f32(sum0, vmulq_f32(d0, d));
    }

    {
        float32x4_t d0 = vdupq_n_f32(0.0f);
        limit++;
        if (limit < 0)
            limit = 0;

        for (i = data_len - 1; i >= limit; i--)
        {
            float32x4_t d = vld1q_lane_f32(data + i, vdupq_n_f32(0.0f), 0);

            d = shufffleVector(d);

            d0 = shufffleVector_2103(d0);
            d0 = vcopyq_laneq_f32(d0, 0, d, 0);
            sum0 = vaddq_f32(sum0, vmulq_f32(d, d0));
        }
    }
    vst1q_f32(autoc, sum0);
}

void FLAC__lpc_compute_autocorrelation_intrin_neon_lag_8(const FLAC__real data[], uint32_t data_len, uint32_t lag, FLAC__real autoc[])
{
    int i;
    int limit = data_len - 8;
    float32x4_t sum0 = vdupq_n_f32(0.0f);
    float32x4_t sum1 = vdupq_n_f32(0.0f);

    (void)lag;
    FLAC__ASSERT(lag <= 8);
    FLAC__ASSERT(lag <= data_len);

    for (i = 0; i <= limit; i++)
    {
        float32x4_t d, d0, d1;
        d0 = vld1q_f32(data + i);
        d1 = vld1q_f32(data + i + 4);
        d = shufffleVector(d0);
        sum0 = vaddq_f32(sum0, vmulq_f32(d0, d));
        sum1 = vaddq_f32(sum1, vmulq_f32(d1, d));
    }

    {
        float32x4_t d0 = vdupq_n_f32(0.0f);
        float32x4_t d1 = vdupq_n_f32(0.0f);
        limit++;
        if (limit < 0)
            limit = 0;

        for (i = data_len - 1; i >= limit; i--)
        {
            float32x4_t d = vld1q_lane_f32(data + i, vdupq_n_f32(0.0f), 0);
            d = shufffleVector(d);

            d1 = shufffleVector_2103(d1);
            d0 = shufffleVector_2103(d0);

            d1 = vcopyq_laneq_f32(d1, 0, d0, 0);
            d0 = vcopyq_laneq_f32(d0, 0, d, 0);

            sum1 = vaddq_f32(sum1, vmulq_f32(d, d1));
            sum0 = vaddq_f32(sum0, vmulq_f32(d, d0));
        }
    }
    vst1q_f32(autoc, sum0);
    vst1q_f32(autoc + 4, sum1);
}

void FLAC__lpc_compute_autocorrelation_intrin_neon_lag_12(const FLAC__real data[], uint32_t data_len, uint32_t lag, FLAC__real autoc[])
{
    int i;
    int limit = data_len - 12;
    float32x4_t sum0 = vdupq_n_f32(0.0f);
    float32x4_t sum1 = vdupq_n_f32(0.0f);
    float32x4_t sum2 = vdupq_n_f32(0.0f);

    (void)lag;
    FLAC__ASSERT(lag <= 12);
    FLAC__ASSERT(lag <= data_len);

    for (i = 0; i <= limit; i++)
    {
        float32x4_t d, d0, d1, d2;
        d0 = vld1q_f32(data + i);
        d1 = vld1q_f32(data + i + 4);
        d1 = vld1q_f32(data + i + 8);
        d = shufffleVector(d0);
        sum0 = vaddq_f32(sum0, vmulq_f32(d0, d));
        sum1 = vaddq_f32(sum1, vmulq_f32(d1, d));
        sum2 = vaddq_f32(sum2, vmulq_f32(d2, d));
    }

    {
        float32x4_t d0 = vdupq_n_f32(0.0f);
        float32x4_t d1 = vdupq_n_f32(0.0f);
        float32x4_t d2 = vdupq_n_f32(0.0f);
        limit++;
        if (limit < 0)
            limit = 0;

        for (i = data_len - 1; i >= limit; i--)
        {
            float32x4_t d = vld1q_lane_f32(data + i, vdupq_n_f32(0.0f), 0);
            d = shufffleVector(d);

            d2 = shufffleVector_2103(d2);
            d1 = shufffleVector_2103(d1);
            d0 = shufffleVector_2103(d0);

            d2 = vcopyq_laneq_f32(d2, 0, d1, 0);
            d1 = vcopyq_laneq_f32(d1, 0, d0, 0);
            d0 = vcopyq_laneq_f32(d0, 0, d, 0);

            sum2 = vaddq_f32(sum2, vmulq_f32(d, d2));
            sum1 = vaddq_f32(sum1, vmulq_f32(d, d1));
            sum0 = vaddq_f32(sum0, vmulq_f32(d, d0));
        }
    }
    vst1q_f32(autoc, sum0);
    vst1q_f32(autoc + 4, sum1);
    vst1q_f32(autoc + 8, sum2);
}

void FLAC__lpc_compute_autocorrelation_intrin_neon_lag_16(const FLAC__real data[], uint32_t data_len, uint32_t lag, FLAC__real autoc[])
{
    int i;
    int limit = data_len - 16;
    float32x4_t sum0 = vdupq_n_f32(0.0f);
    float32x4_t sum1 = vdupq_n_f32(0.0f);
    float32x4_t sum2 = vdupq_n_f32(0.0f);
    float32x4_t sum3 = vdupq_n_f32(0.0f);

    (void)lag;
    FLAC__ASSERT(lag <= 16);
    FLAC__ASSERT(lag <= data_len);

    for (i = 0; i <= limit; i++)
    {
        float32x4_t d, d0, d1, d2, d3;
        d0 = vld1q_f32(data + i);
        d1 = vld1q_f32(data + i + 4);
        d1 = vld1q_f32(data + i + 8);
        d1 = vld1q_f32(data + i + 12);
        d = shufffleVector(d0);
        sum0 = vaddq_f32(sum0, vmulq_f32(d0, d));
        sum1 = vaddq_f32(sum1, vmulq_f32(d1, d));
        sum2 = vaddq_f32(sum2, vmulq_f32(d2, d));
        sum3 = vaddq_f32(sum3, vmulq_f32(d3, d));
    }

    {
        float32x4_t d0 = vdupq_n_f32(0.0f);
        float32x4_t d1 = vdupq_n_f32(0.0f);
        float32x4_t d2 = vdupq_n_f32(0.0f);
        float32x4_t d3 = vdupq_n_f32(0.0f);
        limit++;
        if (limit < 0)
            limit = 0;

        for (i = data_len - 1; i >= limit; i--)
        {
            float32x4_t d = vld1q_lane_f32(data + i, vdupq_n_f32(0.0f), 0);
            d = shufffleVector(d);

            d3 = shufffleVector_2103(d3);
            d2 = shufffleVector_2103(d2);
            d1 = shufffleVector_2103(d1);
            d0 = shufffleVector_2103(d0);

            d3 = vcopyq_laneq_f32(d3, 0, d2, 0);
            d2 = vcopyq_laneq_f32(d2, 0, d1, 0);
            d1 = vcopyq_laneq_f32(d1, 0, d0, 0);
            d0 = vcopyq_laneq_f32(d0, 0, d, 0);

            sum3 = vaddq_f32(sum3, vmulq_f32(d, d3));
            sum2 = vaddq_f32(sum2, vmulq_f32(d, d2));
            sum1 = vaddq_f32(sum1, vmulq_f32(d, d1));
            sum0 = vaddq_f32(sum0, vmulq_f32(d, d0));
        }
    }
    vst1q_f32(autoc, sum0);
    vst1q_f32(autoc + 4, sum1);
    vst1q_f32(autoc + 8, sum2);
    vst1q_f32(autoc + 12, sum3);
}
#endif /* FLAC__CPU_AARCH64 && FLAC__HAS_ARCH64INTRIN */
#endif /* FLAC__NO_ASM */
#endif /* FLAC__INTEGER_ONLY_LIBRARY */
