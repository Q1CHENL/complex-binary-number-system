#ifndef BASE_POWERS_H
#define BASE_POWERS_H

#include <inttypes.h>

struct complex_number {
    __int128 real;
    __int128 imag;
};

struct complex_number64 {
    __int64_t real;
    __int64_t imag;
};

extern struct complex_number64 powers64[];
extern struct complex_number powers128[];

#endif  // BASE_POWERS_H