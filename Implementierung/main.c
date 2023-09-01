#include <getopt.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "btoc.h"
#include "ctob.h"
#include "test.h"
#include "performance.h"
#include "utils.h"

int main(int argc, char** argv) {
    bool benchmarked = false;
    const char* progname = argv[0];
    if (argc == 1) {
        print_usage(progname);
        exit(EXIT_SUCCESS);
    }

    char* complex_arg = extract_complex_number_arg(argc, argv);
    argc = complex_arg ? argc - 1 : argc;

    // index for the implementation to be used
    int imp = 0;
    // counter for iteration used in benchmark
    unsigned __int128 iterations = 1;

    static struct option long_options[] = {{"help", no_argument, NULL, 'h'}, {NULL, 0, NULL, 0}};
    const char* short_options = "V:B::pth";
    int opt;

    while ((opt = getopt_long(argc, argv, short_options, long_options, NULL)) != -1) {
        switch (opt) {
            case 0:
                print_usage(progname);
                return EXIT_FAILURE;
            case 'V':
                if (optarg != NULL) {
                    char* endptr;
                    imp = strtol(optarg, &endptr, 10);
                    if (endptr == optarg || *endptr != '\0') {
                        fprintf(stderr, "%s could not be converted to integer\n", optarg);
                        print_usage(progname);
                        return EXIT_FAILURE;
                    }
                    // 2 Implementations ctob / 5 Implementations btoc
                    if (imp < 0 || imp > 2) {
                        fprintf(
                            stderr,
                            "The implementation with index %d does not exist\n", imp);
                        print_usage(progname);
                        return EXIT_FAILURE;
                    }
                } else {
                    fprintf(stdout, "The default implementation V0 is used\n");
                }
                break;
            case 'B':
                benchmarked = true;
                if (optarg != NULL) {
                    if (optarg[0] == '-') {
                        fprintf(stderr, "Number of iteration must be positive!\n");
                        exit(EXIT_FAILURE);
                    }
                    iterations = str_to_uint128(optarg);
                    fprintf(stdout, "The iteration of %s is used\n", optarg);
                } else {
                    fprintf(stdout, "The default iteration of 1 is used\n");
                }
                break;
            case 'h':
                print_usage(progname);
                return EXIT_FAILURE;
            case 't':
                test();
                return EXIT_SUCCESS;
            case 'p':
                performance();
                return EXIT_SUCCESS;
            default:
                print_usage(progname);
                return EXIT_FAILURE;
        }
    }

    // we have a complex number as argument in the form of a,b
    if (complex_arg) {
        __int128 real, imag;
        parse_complex_number_arg(complex_arg, &real, &imag);

        unsigned __int128 bm1pi = 0;

        struct timespec start;
        clock_gettime(CLOCK_MONOTONIC, &start);

        // call convert function
        switch (imp) {
            case 0:
                for (unsigned __int128 ite = 0; ite < iterations; ite++) {
                    bm1pi = to_bm1pi(real, imag);
                }
                break;
            case 1:
                for (unsigned __int128 ite = 0; ite < iterations; ite++) {
                    bm1pi = to_bm1pi_V1(real, imag);
                }
                break;
            default:
                fprintf(stderr, "The implementation for to_bm1pi with index %d does not exist\n", imp);
                return EXIT_FAILURE;
        }

        struct timespec end;
        clock_gettime(CLOCK_MONOTONIC, &end);

        double time_ns = 1e9 * (end.tv_sec - start.tv_sec) +
                         (end.tv_nsec - start.tv_nsec);
        double time = end.tv_sec - start.tv_sec +
                      1e-9 * (end.tv_nsec - start.tv_nsec);
        if (benchmarked) {
            printf("Average time per iteration: %lfs (%lfns)\n", time / (double)iterations, time_ns / (double)iterations);
            printf("Total time for %f iterations: %fs (%fns)\n", (double)iterations, time, time_ns);
        }

        // print the result as binary and decimal
        printf("Result as decimal: \n");
        print_uint128(bm1pi);
        putchar('\n');
        printf("Result as binary: \n");
        print_uint128_as_binary(bm1pi);
        putchar('\n');
    } else {
        if (optind >= argc) {
            printf("%s: Missing positional arguments\n", progname);
            print_usage(progname);
            return EXIT_FAILURE;
        } else if (optind == argc - 1) {
            // feed first argument to unsigned __int128 bm1pi
            // decimal input
            unsigned __int128 bm1pi = str_to_uint128(argv[optind]);

            __int128 real;
            __int128 imag;

            struct timespec start;
            clock_gettime(CLOCK_MONOTONIC, &start);

            // call convert function
            switch (imp) {
                case 0:
                    for (unsigned __int128 ite = 0; ite < iterations; ite++) {
                        to_carthesian(bm1pi, &real, &imag);
                    }
                    break;
                case 1:
                    for (unsigned __int128 ite = 0; ite < iterations; ite++) {
                        to_carthesian_V1(bm1pi, &real, &imag);
                    }
                    break;
                case 2:
                    for (unsigned __int128 ite = 0; ite < iterations; ite++) {
                        to_carthesian_V2(bm1pi, &real, &imag);
                    }
                    break;
                default:
                    fprintf(stderr, "The implementation for to_carthesian with index %d does not exist\n", imp);
                    return EXIT_FAILURE;
            }

            struct timespec end;
            clock_gettime(CLOCK_MONOTONIC, &end);
            double time_ns = 1e9 * (end.tv_sec - start.tv_sec) +
                             (end.tv_nsec - start.tv_nsec);
            double time = end.tv_sec - start.tv_sec +
                          1e-9 * (end.tv_nsec - start.tv_nsec);
            if (benchmarked) {
                printf("Average time per iteration: %lfs (%lfns)\n", time / (double)iterations, time_ns / (double)iterations);
                printf("Total time for %f iterations: %fs (%fns)\n", (double)iterations, time, time_ns);
            }
            // plot the result in the form of (real) + (imag) i
            print_complex_number(real, imag);
            putchar('\n');

        }
        // too many positional args if the remaining number > 1
        else {
            printf("%s: Too many positional arguments\n", progname);
            print_usage(progname);
            return EXIT_FAILURE;
        }
    }
    return EXIT_SUCCESS;
}
