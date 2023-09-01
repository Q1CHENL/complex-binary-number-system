#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "btoc.h"
#include "ctob.h"
#include "test.h"
#include "utils.h"

void perform(void (*func_to_carthesian)(__uint128_t bm1pi, __int128_t *real, __int128_t *imag),
             unsigned __int128 (*func_to_bm1pi)(__int128 real, __int128 imag));

void performance() {
    srand(time(NULL));
    perform(to_carthesian, to_bm1pi);
    perform(to_carthesian_V1, to_bm1pi_V1);
    perform(to_carthesian_V2, NULL);
}


/**
 * @brief Run func_to_carthesian and func_to_bm1pi with random 
 * input in 8, 16, ..., 128 bit with 1000.000 iterations
 * 
 * @param func_to_carthesian Function that converts binary to complex number
 * @param func_to_bm1pi Function that converts complex number to binary
 */
void perform(void (*func_to_carthesian)(__uint128_t bm1pi, __int128_t *real, __int128_t *imag),
             unsigned __int128 (*func_to_bm1pi)(__int128 real, __int128 imag)) {
    unsigned __int128 iterations = 1000000;

    for (int len = 8; len <= 128; len += 8) {
        double btoc_total_time_sec = 0;
        double btoc_total_time_nsec = 0;

        double ctob_total_time_sec = 0;
        double ctob_total_time_nsec = 0;

        for (unsigned __int128 i = 0; i < iterations; i++) {
            unsigned __int128 bm1pi = random_number(len);
            __int128 real, imag;
            struct timespec start1;
            clock_gettime(CLOCK_MONOTONIC, &start1);
            func_to_carthesian(bm1pi, &real, &imag);
            struct timespec end1;
            clock_gettime(CLOCK_MONOTONIC, &end1);
            double elapsed_nsec1 = 1e9 * (end1.tv_sec - start1.tv_sec) +
                                   (end1.tv_nsec - start1.tv_nsec);
            btoc_total_time_sec += elapsed_nsec1 * 1e-9;
            btoc_total_time_nsec += elapsed_nsec1;

            if (func_to_bm1pi) {
                struct timespec start2;
                clock_gettime(CLOCK_MONOTONIC, &start2);
                func_to_bm1pi(real, imag);
                struct timespec end2;
                clock_gettime(CLOCK_MONOTONIC, &end2);
                double elapsed_nsec2 = 1e9 * (end2.tv_sec - start2.tv_sec) +
                                       (end2.tv_nsec - start2.tv_nsec);
                ctob_total_time_sec += elapsed_nsec2 * 1e-9;
                ctob_total_time_nsec += elapsed_nsec2;
            }
        }
        if (func_to_carthesian == to_carthesian) {
            printf("Time average (%d bit):\nto_carthesian: %lfs (%lfns)\n", len,
                   btoc_total_time_sec / (double)iterations,
                   btoc_total_time_nsec / (double)iterations);

        } else if (func_to_carthesian == to_carthesian_V1) {
            printf("Time average (%d bit):\nto_carthesian_V1: %lfs (%lfns)\n",
                   len, btoc_total_time_sec / (double)iterations,
                   btoc_total_time_nsec / (double)iterations);
        } else if (func_to_carthesian == to_carthesian_V2) {
            printf("Time average (%d bit):\nto_carthesian_V2: %lfs (%lfns)\n",
                   len, btoc_total_time_sec / (double)iterations,
                   btoc_total_time_nsec / (double)iterations);
        }
        if (func_to_bm1pi == to_bm1pi) {
            printf("to_bm1pi: %lfs (%lfns)\n\n",
                   ctob_total_time_sec / (double)iterations,
                   ctob_total_time_nsec / (double)iterations);
        } else if (func_to_bm1pi == to_bm1pi_V1) {
            printf("to_bm1pi_V1: %lfs (%lfns)\n\n",
                   ctob_total_time_sec / (double)iterations,
                   ctob_total_time_nsec / (double)iterations);
        }
    }
    printf("\n====================================================================\n");
}
