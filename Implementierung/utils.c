#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "utils.h"

const char* usage_msg =
    "Usage: \n"
    "   %s <number> [options]       Convert the bits of <number> to complex\n"
    "                                   number in carthesian form\n"
    "   %s <real>,<imag> [options]  Convert Complex number <real> + <imag>i\n"
    "                                   to its binary form in basis -1+i\n"
    "                                   (Don't put a space anywhere within\n"
    "                                   <real>,<imag>)\n"
    "   %s -h | --help              Show this help message\n"
    "   %s -t                       Run test\n"
    "   %s -p                       Runs performance tests for all functions\n"
    "\nOptions: \n"
    "   -V <implementation>    Specifies which implementation to be used.\n"
    "                          Could be 0 or 1 for to_bmpi or\n"
    "                          0, 1 or 2 for to_carthesian. Default is 0.\n"
    "   -B[<iteration>]        The program will be benchmarked when set."
    "<iteration>\n"
    "                          specifies how many times the program will be "
    "executed\n"
    "                          (Don't put a space between -B and <iteration>)\n"
    "\n";

int generate_random_int(int min, int max) {
    return min + rand() % (max - min + 1);
}

// Generate a random number with length len 
// The number's most significant bit will always be 1
unsigned __int128 random_number(int len) {
    unsigned __int128 num = 1;
    for (int i = 0; i < len - 1; i++) {
        if (generate_random_int(1, 100) < 50) {
            num <<= 1;
        } else {
            num <<= 1;
            num++;
        }
    }
    return num;
}

// count the number of leading zeros of binary representation of n
int leading_zero_count(unsigned __int128 n) {
    int len = 0;
    while (n > 0) {
        n >>= 1;
        len++;
    }
    return 128 - len;
}

// plot signed int128 as decimal number
// https://github.com/DoctorManhattan123/int128/blob/main/int128_lib.h
void print_int128(__int128 x) {
    if (x < 0) {
        putchar('-');
        x = -x;
    }
    print_uint128((unsigned __int128)x);
}

// print unsigned int128 as decimal number
void print_uint128(unsigned __int128 x) {
    if (x > 9)
        print_uint128(x / 10);
    putchar(x % 10 + '0');
}

// print signed int128 as binary String
void print_uint128_as_binary(unsigned __int128 x) {
    unsigned __int128 mask1 = 1;
    int lzc = leading_zero_count(x);
    int digit;
    char buf[129] = {0};
    for (int i = 0; i < 128; i++) {
        digit = x & mask1;
        buf[127 - i] = (char)(digit + '0');
        x >>= 1;
    }
    buf[128] = '\0';
    printf("%s\n", buf + lzc);
}

/***
 * parse decimal String into signed int128, Example: 12345(positive), -12345(negative)
 * @param s input decimal String
 * @return __int128 number
 */
__int128 str_to_int128(char* s) {
    __int128 res = 0;
    char* str = s;
    bool _signed = false;
    // handle negative number
    if (str[0] == '-') {
        _signed = true;
        str = s + 1;
    }
    int len = strlen(str);
    if (len > 39) {
        fprintf(stderr, "Input number is too %s, try a %s one!\n",
                _signed ? "small" : "larger", _signed ? "large" : "smaller");
        exit(EXIT_FAILURE);
    } else if (len == 39) {
        if (_signed && strcmp(str, MIN_VALUE_STR) > 0) {
            fprintf(stderr, "Input number is too small, try a larger one!\n");
            exit(EXIT_FAILURE);

        } else if ((!_signed) && strcmp(str, MAX_VALUE_STR) > 0) {
            fprintf(stderr, "Input number is too large, try a smaller one!\n");
            exit(EXIT_FAILURE);
        }
    }

    for (int i = 0; i < len; i++) {
        unsigned __int128 tmp = str[i];
        // in case not between '0' and '9'
        if (tmp < '0' || tmp > '9') {
            fprintf(stderr, "Not a valid number!\n");
            exit(EXIT_FAILURE);
        }
        tmp -= '0';
        for (int j = 0; j < len - i - 1; j++) {
            tmp *= 10;
        }
        res += tmp;
    }
    return _signed ? -res : res;
}

/***
 * parse decimal String into unsigned int128, Example: 12345
 * @param s input decimal String
 * @return unsigned __int128 number
 */
unsigned __int128 str_to_uint128(char* s) {
    if (s[0] == '-') {
        fprintf(stderr, "str_to_uint128: argument must be positive!\n");
        exit(EXIT_FAILURE);
    }
    unsigned __int128 res = 0;
    int len = strlen(s);
    if (len > 39 || (len == 39 && strcmp(s, UMAX_VALUE_STR) > 0)) {
        fprintf(stderr, "Input number is too large, try a smaller number!\n");
        exit(EXIT_FAILURE);
    }

    char str[len + 1];
    strncpy(str, s, len + 1);
    for (int i = 0; i < len; i++) {
        unsigned __int128 tmp = str[i];
        // in case not between '0' and '9'
        if (tmp < '0' || tmp > '9') {
            fprintf(stderr, "Not a valid number!\n");
            exit(EXIT_FAILURE);
        }
        tmp -= '0';
        for (int j = 0; j < len - i - 1; j++) {
            tmp *= 10;
        }
        res += tmp;
    }
    return res;
}

// find the index of ',' in arg
int comma(char* arg) {
    for (int i = 0; arg[i] != '\0'; i++) {
        if (arg[i] == ',')
            return i;
    }
    return 0;
}

// print the helper message
void print_usage(const char* progname) {
    fprintf(stderr, usage_msg, progname, progname, progname, progname, progname);
}

// helper function: check if number exceeds its boundary len_bound
void length_check(char* number, char* number_bound, int len, int len_bound, char* part_str, char* compared) {
    if (len > len_bound || (len == len_bound && strcmp(number, number_bound) > 0)) {
        fprintf(stderr, "%s part is too %s\n", part_str, compared);
        exit(EXIT_FAILURE);
    }
}

// helper function: check if real or imag part exceeds its boundaries
void check_part(char* part, char* upper_bound, char* lower_bound, int upper_len_bound, int lower_len_bound, char* part_str) {
    bool part_signed = part[0] == '-' ? true : false;
    int part_len = strlen(part);
    if (part_signed) {
        length_check(part, lower_bound, part_len, lower_len_bound, part_str, "small!");
    } else {
        length_check(part, upper_bound, part_len, upper_len_bound, part_str, "large!");
    }
}

// check if real and imag exceeds their lower or upper boundaries
void boundary_check(char* real, char* imag) {
    check_part(real, REAL_MAX_STR, REAL_MIN_STR, 19, 21, "Real");
    check_part(imag, IMAG_MAX_STR, IMAG_MIN_STR, 20, 21, "Imag");
}

/**
 * @brief Get the complex number input e.g -1,2 or 13,9
 *
 * @param argc number of arguments including program name
 * @param argv arguments
 * @return char* the complex number argument itself e.g "-1,2"
 */
char* extract_complex_number_arg(int argc, char** argv) {
    int found = 0;
    char* complex_arg = NULL;
    for (int i = 1; i < argc; i++) {
        char* curr_arg = argv[i];
        // return the first amakrg in argv that contains a ','
        if (comma(curr_arg)) {
            if (++found > 1) {
                fprintf(stderr, "Can only process one complex number!\n");
                exit(EXIT_FAILURE);
            }
            complex_arg = curr_arg;
            // remove found element(ptr) from argv
            memmove(argv + i, argv + i + 1, (argc - i - 1) * sizeof(char*));
        }
    }
    // complex number argument not found
    return complex_arg;
}

/**
 * @brief parse and split the given complex number into real and imag part
 *
 * @param complex_arg a complex number a + bi in the form "a,b"
 * @param real used to store the real part
 * @param imag used to store the imagine part
 */
void parse_complex_number_arg(char* complex_arg,
                              __int128* real,
                              __int128* imag) {
    char* real_part = strtok(complex_arg, ",");
    char* imag_part = strtok(NULL, ",");
    boundary_check(real_part, imag_part);
    *real = str_to_int128(real_part);
    *imag = str_to_int128(imag_part);
}

/***
 * plot complex number in the form of (real) + (imag) i
 * @param real signed int128 real number
 * @param imag signed int128 imaginary number
 */
void print_complex_number(__int128 real, __int128 imag) {
    putchar('(');
    print_int128(real);
    printf(") + (");
    print_int128(imag);
    printf(")i");
}
