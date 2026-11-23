// Universally Consistent AVX (UCAVX).

// --- PLEASE LEAVE THIS SECTION UNTOUCHED ---
// UCAVX comes with absolutely NO WARRENTY.
// UCAVX comes with the MIT-0 license, copyright 2026 Brian Billman. https://opensource.org/license/mit-0
// Originally written by Brian Billman.
// Original repository at: github.com/brian-catcow-b/universally_consistent_avx
// Do no evil.
// --- END SECTION ---

// --- GENERAL DOCUMETATION ---
// -- MACROS DECLARABLE BEFORE INCLUDING THIS HEADER --
// UCAVX_COMPILE - Specify that this translation unit should compile UCAVX function definitions.
//                 DEFAULT: Off.
// UCAVX_INLINE - Specify that every UCAVX function be inlined with the default
//                implementation being `__attribute__((always_inline)) static inline'.
//                DEFAULT: Off.
//                SEE ALSO: UCAVX_INLINE_IMPL_OVERRIDE.
// UCAVX_INLINE_IMPL_OVERRIDE - define the implementation for inlining UCAVX functions.

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
#else
#undef UCAVX_INLINE
#define UCAVX_COMPILE
#ifndef UCAVX_INLINE_IMPL_OVERRIDE
#define UCAVX_INLINE __attribute__((always_inline)) static inline
#else
#define UCAVX_INLINE UCAVX_INLINE_IMPL_OVERRIDE
#endif // UCAVX_INLINE_IMPL_OVERRIDE
#endif // UCAVX_INLINE

#include <stdint.h>
#include <immintrin.h>

#ifdef UCAVX_ULTRA_DEBUG
#include <stdio.h>
#define UCAVX_ULTRA_DEBUG_PRINT(str, ...) printf(str "\n", __VA_ARGS__)
#else
#define UCAVX_ULTRA_DEBUG_PRINT(str, ...)
#endif


// fn-like macros start

// expects type* dst, type* a, type* b, size_t len in scope
#define UCAVX_DEF_BINOP_ARBLEN(num_avx_bits, type, type_short, avx_binop, scalar_binop_expr_a_i_b_i) \
    for (size_t i = 0; (i + 1) * ((num_avx_bits/8)/sizeof(type)) <= len; i++) \
    { \
        __m##num_avx_bits##type_short a_i; \
        for (size_t j = 0; j < ((num_avx_bits/8)/sizeof(type)); j++) \
        { \
            a_i[j] = a[i*((num_avx_bits/8)/sizeof(type)) + j]; \
        } \
        __m##num_avx_bits##type_short b_i; \
        for (size_t j = 0; j < ((num_avx_bits/8)/sizeof(type)); j++) \
        { \
            b_i[j] = b[i*((num_avx_bits/8)/sizeof(type)) + j]; \
        } \
        __m##num_avx_bits##type_short dst_i = avx_binop(a_i, b_i); \
        for (size_t j = 0; j < ((num_avx_bits/8)/sizeof(type)); j++) \
        { \
            dst[i*((num_avx_bits/8)/sizeof(type)) + j] = dst_i[j]; \
        } \
    } \
    /* arblen leftovers */ \
    const size_t leftover = len % ((num_avx_bits/8)/sizeof(type)); \
    const size_t leftover_section_start = len - leftover; \
    for (size_t i = 0; i < leftover; i++) \
    { \
        const type a_i = a[leftover_section_start + i]; \
        const type b_i = b[leftover_section_start + i]; \
        dst[leftover_section_start + i] = scalar_binop_expr_a_i_b_i; \
    } do {} while(0)

// expects type* dst, type* a, size_t len in scope
#define UCAVX_DEF_UNARYOP_ARBLEN(num_avx_bits, type, type_short, avx_unaryop, scalar_unaryop_expr_a_i) \
    for (size_t i = 0; (i + 1) * ((num_avx_bits/8)/sizeof(type)) <= len; i++) \
    { \
        __m##num_avx_bits##type_short a_i; \
        for (size_t j = 0; j < ((num_avx_bits/8)/sizeof(type)); j++) \
        { \
           a_i[j] = a[i*((num_avx_bits/8)/sizeof(type)) + j]; \
        } \
        __m##num_avx_bits##type_short dst_i = avx_unaryop(a_i); \
        for (size_t j = 0; j < ((num_avx_bits/8)/sizeof(type)); j++) \
        { \
            dst[i*((num_avx_bits/8)/sizeof(type)) + j] = dst_i[j]; \
        } \
    } \
    /* arblen leftovers */ \
    const size_t leftover = len % ((num_avx_bits/8)/sizeof(type)); \
    const size_t leftover_section_start = len - leftover; \
    for (size_t i = 0; i < leftover; i++) \
    { \
        const type a_i = a[leftover_section_start + i]; \
        dst[leftover_section_start + i] = scalar_unaryop_expr_a_i; \
    } do {} while(0)

// expects type* dst, type* a, type b, size_t len in scope
#define UCAVX_DEF_VSOP_ARBLEN(num_avx_bits, type, type_short, avx_vsop, scalar_vsop_expr_a_i_b) \
    for (size_t i = 0; (i + 1) * ((num_avx_bits/8)/sizeof(type)) <= len; i++) \
    { \
        __m##num_avx_bits##type_short a_i; \
        for (size_t j = 0; j < ((num_avx_bits/8)/sizeof(type)); j++) \
        { \
           a_i[j] = a[i*((num_avx_bits/8)/sizeof(type)) + j]; \
        } \
        __m##num_avx_bits##type_short bvec; \
        for (size_t j = 0; j < ((num_avx_bits/8)/sizeof(type)); j++) \
        { \
           bvec[j] = b; \
        } \
        __m##num_avx_bits##type_short dst_i = avx_vsop(a_i, bvec); \
        for (size_t j = 0; j < ((num_avx_bits/8)/sizeof(type)); j++) \
        { \
            dst[i*((num_avx_bits/8)/sizeof(type)) + j] = dst_i[j]; \
        } \
    } \
    /* arblen leftovers */ \
    const size_t leftover = len % ((num_avx_bits/8)/sizeof(type)); \
    const size_t leftover_section_start = len - leftover; \
    for (size_t i = 0; i < leftover; i++) \
    { \
        const type a_i = a[leftover_section_start + i]; \
        dst[leftover_section_start + i] = scalar_binop_expr_a_i_b; \
    } do {} while(0)

// binary operation (meaning AxA->A mapping) for 128, 256, and 512 bit avx instruction
// in-fn-body definers for arbitrary length inputs using scalar operation for leftovers

// expects double* dst, double* a, double* b, size_t len in scope
#define UCAVX_DEF_DOUBLE_128_BINOP_ARBLEN(avx128_binop, scalar_binop_expr_a_i_b_i) \
    UCAVX_DEF_BINOP_ARBLEN(128, double, d, avx128_binop, scalar_binop_expr_a_i_b_i)
// expects double* dst, double* a, double* b, size_t len in scope
#define UCAVX_DEF_DOUBLE_256_BINOP_ARBLEN(avx256_binop, scalar_binop_expr_a_i_b_i) \
    UCAVX_DEF_BINOP_ARBLEN(256, double, d, avx256_binop, scalar_binop_expr_a_i_b_i)
// expects double* dst, double* a, double* b, size_t len in scope
#define UCAVX_DEF_DOUBLE_512_BINOP_ARBLEN(avx512_binop, scalar_binop_expr_a_i_b_i) \
    UCAVX_DEF_BINOP_ARBLEN(512, double, d, avx512_binop, scalar_binop_expr_a_i_b_i)

// unary operation (meaning A->A mapping) for 128, 256, and 512 bit avx instruction
// in-fn-body definers for arbitrary length inputs using scalar operation for leftovers

// expects double* dst, double* a, double* b, size_t len in scope
#define UCAVX_DEF_DOUBLE_128_UNARYOP_ARBLEN(avx128_unaryop, scalar_unaryop_expr_a_i) \
    UCAVX_DEF_UNARYOP_ARBLEN(128, double, d, avx128_unaryop, scalar_unaryop_expr_a_i)
// expects double* dst, double* a, double* b, size_t len in scope
#define UCAVX_DEF_DOUBLE_256_UNARYOP_ARBLEN(avx256_unaryop, scalar_unaryop_expr_a_i) \
    UCAVX_DEF_UNARYOP_ARBLEN(256, double, d, avx256_unaryop, scalar_unaryop_expr_a_i)
// expects double* dst, double* a, size_t len in scope
#define UCAVX_DEF_DOUBLE_512_UNARYOP_ARBLEN(avx512_unaryop, scalar_unaryop_expr_a_i) \
    UCAVX_DEF_UNARYOP_ARBLEN(512, double, d, avx512_unaryop, scalar_unaryop_expr_a_i)

// vector scalar operation for 128, 256, and 512 bit avx instruction
// in-fn-body definers for arbitrary length inputs using scalar operation for leftovers

// expects double* dst, double* a, double b, size_t len in scope
#define UCAVX_DEF_DOUBLE_128_VSOP_ARBLEN(avx_vsop, scalar_vsop_expr_a_i_b) \
    UCAVX_DEF_VSOP_ARBLEN(128, double, d, avx_vsop, scalar_vsop_expr_a_i_b) \
// expects double* dst, double* a, double b, size_t len in scope
#define UCAVX_DEF_DOUBLE_256_VSOP_ARBLEN(avx_vsop, scalar_vsop_expr_a_i_b) \
    UCAVX_DEF_VSOP_ARBLEN(256, double, d, avx_vsop, scalar_vsop_expr_a_i_b) \
// expects double* dst, double* a, double b, size_t len in scope
#define UCAVX_DEF_DOUBLE_512_VSOP_ARBLEN(avx_vsop, scalar_vsop_expr_a_i_b) \
    UCAVX_DEF_VSOP_ARBLEN(512, double, d, avx_vsop, scalar_vsop_expr_a_i_b) \

// fn-like macros end

// TODO: determine how this crap will work
typedef uint32_t ucavx_available_avx_flags_t;

// FUNCTIONS

// ucavx<numbits>_add_d_arblen(double* dst, const double* a, const double* b, const size_t len)
// ucavx<numbits>_sub_d_arblen(double* dst, const double* a, const double* b, const size_t len)

// add_d_arblen
// dst=a+b

UCAVX_INLINE
void ucavxscalar_add_d_arblen(double* UCAVX_NONULL dst, const double* UCAVX_NONULL a, const double* UCAVX_NONULL b, const size_t len)
#ifdef UCAVX_COMPILE
{ for (size_t i = 0; i < len; i++) { dst[i] =  a[i] + b[i]; } }
#else
;
#endif

__attribute__((target("avx"))) UCAVX_INLINE
void ucavx128_add_d_arblen(double* UCAVX_NONULL dst, const double* UCAVX_NONULL a, const double* UCAVX_NONULL b, const size_t len)
#ifdef UCAVX_COMPILE
{ UCAVX_DEF_DOUBLE_128_BINOP_ARBLEN(_mm_add_pd, (a_i + b_i)); }
#else
;
#endif

__attribute__((target("avx2"))) UCAVX_INLINE
void ucavx256_add_d_arblen(double* UCAVX_NONULL dst, const double* UCAVX_NONULL a, const double* UCAVX_NONULL b, const size_t len)
#ifdef UCAVX_COMPILE
{ UCAVX_DEF_DOUBLE_256_BINOP_ARBLEN(_mm256_add_pd, (a_i + b_i)); }
#else
;
#endif

__attribute__((target("avx512f"))) UCAVX_INLINE
void ucavx512_add_d_arblen(double* UCAVX_NONULL dst, const double* UCAVX_NONULL a, const double* UCAVX_NONULL b, const size_t len)
#ifdef UCAVX_COMPILE
{ UCAVX_DEF_DOUBLE_512_BINOP_ARBLEN(_mm512_add_pd, (a_i + b_i)); }
#else
;
#endif

// sub_d_arblen
// dst=a-b

UCAVX_INLINE
void ucavxscalar_sub_d_arblen(double* UCAVX_NONULL dst, const double* UCAVX_NONULL a, const double* UCAVX_NONULL b, const size_t len)
#ifdef UCAVX_COMPILE
{ for (size_t i = 0; i < len; i++) { dst[i] =  a[i] - b[i]; } }
#else
;
#endif

__attribute__((target("avx"))) UCAVX_INLINE
void ucavx128_sub_d_arblen(double* UCAVX_NONULL dst, const double* UCAVX_NONULL a, const double* UCAVX_NONULL b, const size_t len)
#ifdef UCAVX_COMPILE
{ UCAVX_DEF_DOUBLE_128_BINOP_ARBLEN(_mm_sub_pd, (a_i - b_i)); }
#else
;
#endif

__attribute__((target("avx2"))) UCAVX_INLINE
void ucavx256_sub_d_arblen(double* UCAVX_NONULL dst, const double* UCAVX_NONULL a, const double* UCAVX_NONULL b, const size_t len)
#ifdef UCAVX_COMPILE
{ UCAVX_DEF_DOUBLE_256_BINOP_ARBLEN(_mm256_sub_pd, (a_i - b_i)); }
#else
;
#endif

__attribute__((target("avx512f"))) UCAVX_INLINE
void ucavx512_sub_d_arblen(double* UCAVX_NONULL dst, const double* UCAVX_NONULL a, const double* UCAVX_NONULL b, const size_t len)
#ifdef UCAVX_COMPILE
{ UCAVX_DEF_DOUBLE_512_BINOP_ARBLEN(_mm512_sub_pd, (a_i - b_i)); }
#else
;
#endif

#endif // UCAVX_H

