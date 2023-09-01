#include <stdint.h>
#include <stdio.h>

// Represents a complex number a+bi, where real = a, imag = b
struct complex_number {
    __int128 real;
    __int128 imag;
};

//Multiplication of a complex numbers and (-1 + i)
void complex_mul_base(struct complex_number* src, struct complex_number* dst) {
    __int128 r = src->real;
    __int128 i = src->imag;

    dst->real = -r - i;
    dst->imag = r - i;
}

//Calculates exponent of given base by repeated multiplication
void exponent_of_base(size_t n, struct complex_number* dst) {
    if (n == 0) {
        dst->real = 1;
        dst->imag = 0;
        return;
    }
    dst->real = -1, dst->imag = 1;
    for (size_t i = 0; i < n - 1; i++) {
        complex_mul_base(dst, dst);
    }
}
