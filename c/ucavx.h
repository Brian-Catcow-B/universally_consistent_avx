// Universally Consistent AVX (UCAVX).
// UCAVX comes with absolutely NO WARRENTY.
// UCAVX comes with absolutely NO LICESNSE.
// UCAVX comes with absolutely NO COPYRIGHT.

// Free to copy, modify, and distribute.

// --- PLEASE LEAVE THIS SECTION UNTOUCHED ---
// Originally written by Brian Billman.
// Original repository at: github.com/brian-catcow-b/universally_consistent_avx
// Do no evil.
// --- END SECTION ---

#ifndef UCAVX_H
#define UCAVX_H

#define UCAVX_VERSION_MAJOR 0
#define UCAVX_VERSION_MINOR 0
#define UCAVX_VERSION_PATCH 0

//#ifndef UCAVX_EXTERN
//#ifdef __cplusplus
//#define UCAVX_EXTERN extern "C"
//#else
//#define UCAVX_EXTERN extern
//#endif
//#endif // UCAVX_EXTERN

#ifndef UCAVX_NONULL
#define UCAVX_NONULL
#endif // UCAVX_NONULL

#ifndef UCAVX_INLINE
#define UCAVX_INLINE
#endif // UCAVX_INLINE

#include <stdint.h>
#include <x86intrin.h>

// expects double* dst, double* a, double* b, size_t len in scope
#define UCAVX_DEF_DOUBLE_256_BINOP_ARBLEN(avx256_binop, scalar_binop_expr_a_i_b_i) \
    for (size_t i = 0; i * ((256/8)/sizeof(double)) < len; i++) \
    { \
        __m256d a_i = { a[i*((256/8)/sizeof(double))], a[i*((256/8)/sizeof(double)) + 1], a[i*((256/8)/sizeof(double)) + 2], a[i*((256/8)/sizeof(double)) + 3] }; \
        __m256d b_i = { b[i*((256/8)/sizeof(double))], b[i*((256/8)/sizeof(double)) + 1], b[i*((256/8)/sizeof(double)) + 2], b[i*((256/8)/sizeof(double)) + 3] }; \
        __m256d dst_i = avx256_binop(a_i, b_i); \
        for (size_t j = 0; j < ((256/8)/sizeof(double)); j++) \
        { \
            dst[i*((256/8)/sizeof(double)) + j] = dst_i[j]; \
        } \
    } \
    /* arblen leftovers */ \
    const size_t leftover_section_start = len / ((256/8)/sizeof(double)); \
    const size_t leftover = len % ((256/8)/sizeof(double)); \
    for (size_t i = 0; i < leftover; i++) \
    { \
        const double a_i = a[leftover_section_start + i]; \
        const double b_i = b[leftover_section_start + i]; \
        dst[leftover_section_start + i] = scalar_binop_expr_a_i_b_i; \
    } do {} while(0)

// TODO: determine how this crap will work
typedef uint32_t ucavx_available_avx_flags_t;

__attribute__((target("avx2")))
UCAVX_INLINE void ucavx256_add_d_arblen(double* UCAVX_NONULL dst, const double* UCAVX_NONULL a, const double* UCAVX_NONULL b, const size_t len)
{
    UCAVX_DEF_DOUBLE_256_BINOP_ARBLEN(_mm256_add_pd, (a_i + b_i));
}

// TODO: we need the context passed into this to know if we even can call avx vector instructions
// this function runs vectorized dst=a+b
__attribute__((target("avx512f")))
UCAVX_INLINE void ucavx512_add_d_arblen(double* UCAVX_NONULL dst, const double* UCAVX_NONULL a, const double* UCAVX_NONULL b, const size_t len)
{
    // TODO: preprocessor directive the cpu arch and passed flag check
    // avx512
    for (size_t i = 0; i * sizeof(double) < len; i++)
    {
        __m512d a_i = { a[i*sizeof(double)], a[i*sizeof(double) + 1], a[i*sizeof(double) + 2], a[i*sizeof(double) + 3], a[i*sizeof(double) + 4], a[i*sizeof(double) + 5], a[i*sizeof(double) + 6], a[i*sizeof(double) + 7] };
        __m512d b_i = { b[i*sizeof(double)], b[i*sizeof(double) + 1], b[i*sizeof(double) + 2], b[i*sizeof(double) + 3], b[i*sizeof(double) + 4], b[i*sizeof(double) + 5], b[i*sizeof(double) + 6], b[i*sizeof(double) + 7] };
        __m512d dst_i = _mm512_add_pd(a_i, b_i);
        for (size_t j = 0; j < 8; j++)
        {
            dst[i*sizeof(double) + j] = dst_i[j];
        }
    }
    // leftovers
    const size_t leftover_section_start = len / sizeof(double);
    const size_t leftover = len % sizeof(double);
    for (size_t i = 0; i < leftover; i++)
    {
        dst[leftover_section_start + i] = a[leftover_section_start + i] + b[leftover_section_start + i];
    }
}

#endif // UCAVX_H

