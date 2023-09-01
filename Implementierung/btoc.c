#include <immintrin.h>

#include "complex_mul.h"
#include "base_powers.h"

void handle_power128(unsigned __int128 bm1pi, __int128* real, __int128* imag);


/**
 * @brief
 * A basic, unoptimized implementation, converting bits of bm1pi to complex number
 * @param bm1pi the to be converted integer
 * @param real real part of the result complex number
 * @param imag imagine part of the result complex number
 * @return ** void
 */
void to_carthesian_V2(unsigned __int128 bm1pi, __int128* real, __int128* imag) {
    // zero out real and imag for addtion
    *real = 0;
    *imag = 0;
    // 000..01 mask to get the lowest bit
    unsigned __int128 mask1 = 1;
    // hold tmp result of base^n in every iteration
    struct complex_number tmp;

    // lowest bit
    int digit;
    // iterate through bm1pi from lowest to highest bit
    for (int i = 0; i < 128; i++) {
        digit = bm1pi & mask1;
        bm1pi >>= 1;
        if (!digit) continue;
        exponent_of_base(i, &tmp);
        *real += tmp.real;
        *imag += tmp.imag;
    }
}

/**
 * @brief
 * A with lookup table optimized implementation
 * parameters and return are the same as to_carthesian()
 */
void to_carthesian_V1(unsigned __int128 bm1pi, __int128* real, __int128* imag) {
    // zero out real and imag for addtion
    *real = 0;
    *imag = 0;
    // 000..01 mask to get the lowest bit
    unsigned __int128 mask1 = 1;
    // lowest bit
    int digit;
    // iterate through bm1pi from lowest to highest bit
    for (int i = 0; i < 126; i++) {
        digit = bm1pi & mask1;
        bm1pi >>= 1;
        if (!digit) continue;
        // use the lookup table to get real and imag
        *real += powers64[i].real;
        *imag += powers64[i].imag;
    }
    handle_power128(bm1pi, real, imag);
}

// Main implementation utilizing lookup table as well as simd optimization
void to_carthesian(unsigned __int128 bm1pi, __int128* real, __int128* imag) {
    __m128i cn = _mm_setzero_si128();
    // zero out real and imag for addtion
    *real = 0;
    *imag = 0;
    // 000..01 mask to get the lowest bit
    unsigned __int128 mask1 = 1;
    int digit;
    // iterate through bm1pi from lowest to highest bit
    for (int i = 0; i < 126; i++) {
        digit = bm1pi & mask1;
        bm1pi >>= 1;
        if (!digit) continue;
        // use the lookup table to get real and imag
        cn = _mm_add_epi64(cn, _mm_loadu_si128((__m128i_u*)(powers64 + i)));
    }
    *real = _mm_cvtsi128_si64(cn);
    cn = _mm_srli_si128(cn, 8);
    *imag = _mm_cvtsi128_si64(cn);
    handle_power128(bm1pi, real, imag);
}

// Function handles (-1 + i)^126 and (-1 + i)^127
void handle_power128(unsigned __int128 bm1pi, __int128* real, __int128* imag) {
    switch (bm1pi) {
        case 1:
            *real += powers128[0].real;
            *imag += powers128[0].imag;
            break;
        case 2:
            *real += powers128[1].real;
            *imag += powers128[1].imag;
            break;
        case 3:
            *real += powers128[0].real + powers128[1].real;
            *imag += powers128[0].imag + powers128[1].imag;
            break;
        default:
            break;
    }
}
