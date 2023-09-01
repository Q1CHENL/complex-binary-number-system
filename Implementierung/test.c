#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "btoc.h"
#include "ctob.h"
#include "utils.h"

void print_checkline(bool failed, __int128 real, __int128 imag, unsigned __int128 bm1pi) {
    printf("%s", failed ? "Failed: " : "Passed: ");
    print_complex_number(real, imag);
    printf("%s", failed ? " != " : " == ");
    print_uint128(bm1pi);
    putchar('\n');
}

// Check if to_bm1pi(_V1) converts the complex number to its correct binary representation
void print_tobm1pi_check(char* real_str, char* imag_str, char* result_str, int* fail_count, int version) {
    bool failed;
    __int128 real = str_to_int128(real_str);
    __int128 imag = str_to_int128(imag_str);
    unsigned __int128 complex = 0;
    if (version == 0) {
        complex = to_bm1pi(real, imag);
    }
    if (version == 1) {
        complex = to_bm1pi_V1(real, imag);
    }
    unsigned __int128 result = str_to_uint128(result_str);
    failed = complex != result;
    *fail_count += failed;

    print_checkline(failed, real, imag, result);
    if (failed) {
        printf("Expected: ");
        print_uint128(result);
        printf("    Got: ");
        print_uint128(complex);
        putchar('\n');
    }
}

//Check if to_carthesian converts a number(binary form represents a complex number)
//to the correct complex number
void print_tocarthesian_check(char* bm1pi, char* real_res, char* imag_res, int* fail_count, int version) {
    __int128 real, imag;
    bool failed;
    if (version == 0) {
        to_carthesian(str_to_uint128(bm1pi), &real, &imag);
    }
    if (version == 1) {
        to_carthesian_V1(str_to_uint128(bm1pi), &real, &imag);
    }
    if (version == 2) {
        to_carthesian_V2(str_to_uint128(bm1pi), &real, &imag);
    }
    failed = real != str_to_int128(real_res) || imag != str_to_int128(imag_res);
    *fail_count += failed;
    printf("%s", failed ? "Failed: " : "Passed: ");
    print_uint128(str_to_uint128(bm1pi));
    printf("%s", failed ? " != " : " == ");
    print_complex_number(real, imag);
    putchar('\n');
    if (failed) {
        printf("Expected: ");
        print_complex_number(str_to_int128(real_res), str_to_int128(imag_res));
        printf("    Got: ");
        print_complex_number(real, imag);
        putchar('\n');
    }
}

int test() {
    // Seed the random number generator with current time
    srand(time(NULL));
    __int128_t real;
    __int128_t imag;
    unsigned __int128 complex;
    int fail_count = 0; //count of the failed cases

    printf("==========================================PREDEFINED TEST===========================================\n");
    printf(
        "Method: check to_bm1pi() and to_carthesian() with predefined\n"
        "        values respectively, including edge cases.\n\n");
    for (int i = 0; i < 3; i++) {
        if (i == 0) {
            printf("\n[to_carthesian()]\n");
        }
        if (i == 1) {
            printf("\n[to_carthesian_V1()]\n");
        }
        if (i == 2) {
            printf("\n[to_carthesian_V2()]\n");
        }
        //Next 16 testcases are arbitrarily picked numbers with bitlengths that are multiples of 8
        print_tocarthesian_check("228", "-4", "-6", &fail_count, i);                                                                           //8 Bit
        print_tocarthesian_check("49920", "-128", "16", &fail_count, i);                                                                       //16 Bit
        print_tocarthesian_check("10154305", "-2927", "-1336", &fail_count, i);                                                                //24 Bit
        print_tocarthesian_check("3065431686", "-39321", "-56601", &fail_count, i);                                                            //32 Bit
        print_tocarthesian_check("972755564727", "-321128", "-183085", &fail_count, i);                                                        //40 Bit
        print_tocarthesian_check("144807360324496", "-8961868", "-8096312", &fail_count, i);                                                   //48 Bit
        print_tocarthesian_check("40988555181588055", "-196747252", "-141536681", &fail_count, i);                                             //56 Bit
        print_tocarthesian_check("12639701977088607897", "-416750105", "-2856390806", &fail_count, i);                                         //64 Bit
        print_tocarthesian_check("3357694348109432238544","-38441322492","-56232024128", &fail_count, i);                                      //72 Bit
        print_tocarthesian_check("678246296211846960831927", "-457056784536", "-510889145869", &fail_count, i);                                //80 Bit
        print_tocarthesian_check("156848352374657881205751158", "-8052552176465", "-9634000363709", &fail_count, i);                           //88 Bit
        print_tocarthesian_check("43242945994985478411108870072", "-111422723729734", "-99658075783258", &fail_count, i);                      //96 Bit
        print_tocarthesian_check("17004104748367570845603716699093", "-3832220498679499", "-406670775678066", &fail_count, i);                 //104 Bit
        print_tocarthesian_check("4144954125638475258552034427313317", "-27604691159396259", "2039848259148402", &fail_count, i);              //112 Bit
        print_tocarthesian_check("1005856888884758787393138896927877246", "-535294776490084511", "-47001702434955931", &fail_count, i);        //120 Bit 
        print_tocarthesian_check("216380288476396687350684715112849351387", "-6144252637082668250", "-12498372885357678381", &fail_count, i);  //128 Bit 
        //Following test cases are some edge cases and important values
        print_tocarthesian_check("0", "0", "0", &fail_count, i);                                                                               // 0
        print_tocarthesian_check(UMAX_VALUE_STR, "-7378697629483820646", "-3689348814741910323", &fail_count, i);   // All ones set (UINT128_MAX)
        print_tocarthesian_check("340282366920938463444927863358058659840", "-7378697627765833728", "-3689348813882916864", &fail_count, i);   // Upper 64 bits set
        print_tocarthesian_check("18446744073709551615", "-1717986918", "-858993459", &fail_count, i);                                         // Lower 64 bits set
        print_tocarthesian_check("9223372036854775808", "-2147483648", "-2147483648", &fail_count, i);                                         // INT32_MIN
        print_tocarthesian_check(RESULT_REAL_MIN_STR, REAL_MIN_STR, "-1229782938247303441", &fail_count, i);  // REAL_MIN
        print_tocarthesian_check(RESULT_REAL_MAX_STR, REAL_MAX_STR, "4919131752989213764", &fail_count, i);     // REAL_MAX
        print_tocarthesian_check(RESULT_IMAG_MIN_STR, "-8608480567731124087", IMAG_MIN_STR, &fail_count, i);  // IMAG_MIN
        print_tocarthesian_check(RESULT_IMAG_MAX_STR, "-2459565876494606882", IMAG_MAX_STR, &fail_count, i);   // IMAG_MAX
        print_tocarthesian_check("85070591730234615865843651857942052871", "0", "9223372036854775807", &fail_count, i);                        // INT64_MAX for imag (can't be set for real because INT64_MAX > REAL_MAX)
        print_tocarthesian_check("170141183460469231731687303715884105728", "-9223372036854775808", "-9223372036854775808", &fail_count, i);   // 128 bit input: only most significant bit is 1
    }
    for (int j = 0; j < 2; j++) {
        if (j == 0) {
            printf("\n[to_bm1pi()]\n");
        }
        if (j == 1) {
            printf("\n[to_bm1pi_V1()]\n");
        }
        //Next 16 testcases are arbitrarily picked numbers that have results with bitlengths that are multiples of 8
        print_tobm1pi_check("-4", "-6", "228", &fail_count, j);                                                                           //8 Bit
        print_tobm1pi_check("-128", "16", "49920", &fail_count, j);                                                                       //16 Bit
        print_tobm1pi_check("-2927", "-1336", "10154305", &fail_count, j);                                                                //24 Bit
        print_tobm1pi_check("-39321", "-56601", "3065431686", &fail_count, j);                                                            //32 Bit
        print_tobm1pi_check("-321128", "-183085", "972755564727", &fail_count, j);                                                        //40 Bit
        print_tobm1pi_check("-8961868", "-8096312", "144807360324496", &fail_count, j);                                                   //48 Bit
        print_tobm1pi_check("-196747252", "-141536681", "40988555181588055", &fail_count, j);                                             //56 Bit
        print_tobm1pi_check("-416750105", "-2856390806", "12639701977088607897", &fail_count, j);                                         //64 Bit
        print_tobm1pi_check("-38441322492", "-56232024128", "3357694348109432238544", &fail_count, j);                                    //72 Bit
        print_tobm1pi_check("-457056784536", "-510889145869", "678246296211846960831927", &fail_count, j);                                //80 Bit
        print_tobm1pi_check("-8052552176465", "-9634000363709", "156848352374657881205751158", &fail_count, j);                           //88 Bit
        print_tobm1pi_check("-111422723729734", "-99658075783258", "43242945994985478411108870072", &fail_count, j);                      //96 Bit
        print_tobm1pi_check("-3832220498679499", "-406670775678066", "17004104748367570845603716699093", &fail_count, j);                 //104 Bit
        print_tobm1pi_check("-27604691159396259", "2039848259148402", "4144954125638475258552034427313317", &fail_count, j);              //112 Bit
        print_tobm1pi_check("-535294776490084511", "-47001702434955931", "1005856888884758787393138896927877246", &fail_count, j);        //120 Bit 
        print_tobm1pi_check("-6144252637082668250", "-12498372885357678381", "216380288476396687350684715112849351387", &fail_count, j);  //128 Bit 

        
        //Following test cases are some edge cases and important values
        print_tobm1pi_check("0", "0", "0", &fail_count, j);                                                                               // 0
        print_tobm1pi_check("-7378697629483820646", "-3689348814741910323", UMAX_VALUE_STR, &fail_count, j);   // All bits set to 1(UINT128_MAX)
        print_tobm1pi_check("-7378697627765833728", "-3689348813882916864", "340282366920938463444927863358058659840", &fail_count, j);   // Higher 64 bits set to 1
        print_tobm1pi_check("-1717986918", "-858993459", "18446744073709551615", &fail_count, j);                                         // Lower 64 bits set to 1
        print_tobm1pi_check("2147483647", "2147483647", "267477789068788498438", &fail_count, j);                                         // INT32_MAX
        print_tobm1pi_check("-2147483648", "-2147483648", "9223372036854775808", &fail_count, j);                                         // INT32_MIN
        print_tobm1pi_check(REAL_MIN_STR, "-1229782938247303441", RESULT_REAL_MIN_STR, &fail_count, j);  // REAL_MIN
        print_tobm1pi_check(REAL_MAX_STR, "4919131752989213764", RESULT_REAL_MAX_STR, &fail_count, j);     // REAL_MAX
        print_tobm1pi_check("-8608480567731124087", IMAG_MIN_STR, RESULT_IMAG_MIN_STR, &fail_count, j);  // IMAG_MIN
        print_tobm1pi_check("-2459565876494606882", IMAG_MAX_STR, RESULT_IMAG_MAX_STR, &fail_count, j);   // IMAG_MAX
        print_tobm1pi_check("0", "9223372036854775807", "85070591730234615865843651857942052871", &fail_count, j);                        // INT64_MAX for imag (can't be set for real because INT64_MAX > REAL_MAX)
        print_tobm1pi_check("-9223372036854775808", "-9223372036854775808", "170141183460469231731687303715884105728", &fail_count, j);   // INT64_MIN (applied to both arguments as it fits in range)
    }                                              

    unsigned __int128 random_start = rand();
    // generate a random start length limit in bits
    int random_bound = generate_random_int(32, 128);
    // generate a random 128 bit start
    for (int i = 32; i < random_bound; i++) {
        if (generate_random_int(1, 100) < 50) {
            random_start <<= 1;
        } else {
            random_start <<= 1;
            random_start++;
        }
    }

    unsigned __int128 iterations = rand() / 100;
    unsigned __int128 tmp = iterations / 10;
    unsigned __int128 count = 0;
    printf("============================================RANDOM TEST=============================================\n");
    printf("Start: ");
    print_uint128(random_start);
    printf("\nIterations: ");
    print_uint128(iterations);
    printf(
        "\nMethod: feed random input to to_carthesian(), run for random iterations\n"
        "        from a random start, use the result as input for to_bm1pi(), then\n"
        "        check if the results agree to each other. Aims to conduct test on \n"
        "        a wider range.\n");
    printf("\nSome representative checks:\n");
    printf("(Left is result of to_carthesian(), right is to_bm1pi())\n\n");
    for (unsigned __int128 i = random_start; i < random_start + iterations; i++, count++) {
        bool failed;
        to_carthesian(i, &real, &imag);
        complex = to_bm1pi(real, imag);
        failed = complex != i;
        fail_count += failed;

        to_carthesian_V1(i, &real, &imag);
        complex = to_bm1pi_V1(real, imag);
        failed = complex != i;
        fail_count += failed;

        to_carthesian_V2(i, &real, &imag);
        complex = to_bm1pi(real, imag);
        failed = complex != i;
        fail_count += failed;
        if (count >= tmp) {
            print_checkline(failed, real, imag, complex);
            count = 0;
        }
    }
    printf(fail_count ? "\n%d tests failed!\n" : "\nAll tests passed!\n", fail_count);

    return 0;
}