#include <stdio.h>
#include <stdlib.h>

//If there are too many shifts, number is not representible in 128 Bit and error message is given out by this function
void shift_check(int shift) {
    if (shift > 128) {
        fprintf(stderr, "The given complex number's binary form need more than 128 bits!\n");
        exit(EXIT_FAILURE);
    }
}

// https://math.stackexchange.com/questions/1209190/base-conversion-how-to-convert-between-decimal-and-a-complex-base
/**
 * @brief
 * A basic, unoptimized implementation, converting complex number in cartesian form real + i(imag) to base (-1 + i) bitstring
 * @param real real part of the input complex number
 * @param imag imaginary part of the input complex number
 * @return unsigned __int128 bitstring
 */
unsigned __int128 to_bm1pi_V1(__int128 real, __int128 imag) {
    __int128 r = real;
    __int128 i = imag;
    unsigned __int128 bitstring = 0;
    unsigned __int128 mask = 1;
    int shift = 0;
    while (!(i == 0 && r == 0)) {
        __int128 tmpr = r;
        __int128 tmpi = i;
        if ((r % 2 == 0 && i % 2 == 0) || (r % 2 != 0 && i % 2 != 0)) {
            r = (tmpi - tmpr) / 2;
            i = (-1 * (tmpi + tmpr)) / 2;
        } else {
            r = (tmpi - tmpr + 1) / 2;
            i = (-1 * (tmpr + tmpi - 1)) / 2;
            bitstring = bitstring | mask;
        }
        mask <<= 1;
        // Edge case: the given complex number can not be represented within 128 bits
        shift_check(++shift);
    }
    return bitstring;
}

/**
 * @brief
 * An optimized implementation, converting complex number in cartesian form real + i(imag) to base (-1 + i) bitstring. If possible,
 * process is sped-up by dividing by higher powers of (-1 + i)
 * @param real real part of the input complex number
 * @param imag imaginary part of the input complex number
 * @return unsigned __int128 bitstring
 */
unsigned __int128 to_bm1pi(__int128 real, __int128 imag) {
    __int128 r = real;
    __int128 i = imag;
    unsigned __int128 bitstring = 0;
    unsigned __int128 mask = 1;
    int shift = 0;
    while (!(i == 0 && r == 0)) {
        __int128 tmpr = r;
        __int128 tmpi = i;
        if ((r % 2 == 0 && i % 2 == 0) || (r % 2 != 0 && i % 2 != 0)) {
            if ((i - r) % 4 == 0 && (i + r) % 4 == 0) { //Check if divisible by (-1 + i)^3
                r = (tmpi + tmpr) / 4;
                i = (tmpi - tmpr) / 4;
                mask <<= 3;
                shift += 3;
                continue;
            }
            if (r % 2 == 0 && i % 2 == 0) { //Check if divisible by (-1 + i)^2
                r = -tmpi / 2;
                i = tmpr / 2;
                mask <<= 2;
                shift += 2;
                continue;
            }

            r = (tmpi - tmpr) / 2;
            i = (-1 * (tmpi + tmpr)) / 2;
        } else { //If not divisible by higher prower, proceed like normal
            r = (tmpi - tmpr + 1) / 2;
            i = (-1 * (tmpr + tmpi - 1)) / 2;
            bitstring = bitstring | mask;
        }
        mask <<= 1;
        shift_check(++shift);
    }
    return bitstring;
}