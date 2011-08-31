#include <stdlib.h>
#include <stdio.h>
#include <limits.h>                    /* for INT_MIN & friends (according to C89) */
#include <float.h>                     /* for DBL_EPSILON (according to C89) */
#include <math.h>                      /* for fabs() */
#include <assert.h>

#include <sys/time.h>                  /* for gettimeofday() */
#include <time.h>                      /* for gettimeofday() */

#include <qthread/qutil.h>

// #define ARRAYLEN 1000000000
#define ARRAYLEN 200000000

/*
 * This file tests the qutil functions
 *
 */
static int dcmp(const void *a, const void *b)
{
    if ((*(double *)a) < (*(double *)b)) {
        return -1;
    }
    if ((*(double *)a) > (*(double *)b)) {
        return 1;
    }
    return 0;
}

#define TIME_TEST(type, action, func, out, auth, arg1, len)             do { \
        printf("[test1] " action " %lu " type "s in parallel", \
               (unsigned long)len); \
        fflush(stdout); \
        gettimeofday(&start, NULL); \
        out = func(me, arg1, len, 0); \
        gettimeofday(&stop, NULL); \
        assert(out == auth); \
        printf( \
            " took: %g seconds\n", (stop.tv_sec + (stop.tv_usec * 1.0e-6)) - \
            (start.tv_sec + (start.tv_usec * 1.0e-6))); \
} while (0);
#define TIME_TEST_SIMPLE(type, action, func, out, auth, arg1, len)      do { \
        size_t i; \
        printf("[test1] " action " %lu " type "s serially", \
               (unsigned long)len); \
        fflush(stdout); \
        gettimeofday(&start, NULL); \
        for (i = 0; i < len; i++) func(out, arg1[i]); \
        gettimeofday(&stop, NULL); \
        if ((stop.tv_sec == start.tv_sec) && \
            (stop.tv_usec == start.tv_usec)) { \
            printf("WOW! instantaneous computation! (%lu) %g\n", \
                   (unsigned long)i, (double)out); abort(); \
        } \
        printf(" took:    %g seconds\n", \
               (stop.tv_sec + (stop.tv_usec * 1.0e-6)) - \
               (start.tv_sec + (start.tv_usec * 1.0e-6))); \
} while (0);

#define ADD(a, b)       a += b
#define MULT(a, b)      a *= b
#define MAX(a, b)       a = (a > b) ? a : b
#define MIN(a, b)       a = (a < b) ? a : b

aligned_t qmain(qthread_t * me, void *junk)
{
    unsigned int *ui_array;
    unsigned int ui_out, ui_sum_authoritative = 0, ui_mult_authoritative =
        1, ui_max_authoritative = 0, ui_min_authoritative = UINT_MAX;
    const size_t ui_len = ARRAYLEN;
    int *i_array;
    int i_out, i_sum_authoritative = 0, i_mult_authoritative =
        1, i_max_authoritative = INT_MIN, i_min_authoritative = INT_MAX;
    const size_t i_len = ARRAYLEN;
    double *d_array;
    double d_out, d_sum_authoritative = 0.0, d_mult_authoritative =
        1.0, d_max_authoritative = DBL_MIN, d_min_authoritative = DBL_MAX;
    const size_t d_len = ARRAYLEN;

    size_t i;
    struct timeval start, stop;

    ui_array = calloc(ui_len, sizeof(unsigned int));
    assert(ui_array != NULL);
    for (i = 0; i < ui_len; i++) {
        ui_array[i] = random();
        ui_sum_authoritative += ui_array[i];
        ui_mult_authoritative *= ui_array[i];
        if (ui_max_authoritative < ui_array[i]) {
            ui_max_authoritative = ui_array[i];
        }
        if (ui_min_authoritative > ui_array[i]) {
            ui_min_authoritative = ui_array[i];
        }
    }

    ui_out = 0;
    TIME_TEST_SIMPLE("unsigned int", "summing", ADD, ui_out,
                     ui_sum_authoritative, ui_array, ui_len);
    ui_out = 1;
    TIME_TEST_SIMPLE("unsigned int", "multiplying", MULT, ui_out,
                     ui_mult_authoritative, ui_array, ui_len);
    ui_out = 0;
    TIME_TEST_SIMPLE("unsigned int", "findmax()'ing", MAX, ui_out,
                     ui_max_authoritative, ui_array, ui_len);
    ui_out = UINT_MAX;
    TIME_TEST_SIMPLE("unsigned int", "findmin()'ing", MIN, ui_out,
                     ui_min_authoritative, ui_array, ui_len);
    TIME_TEST("unsigned int", "summing", qutil_uint_sum, ui_out,
              ui_sum_authoritative, ui_array, ui_len);
    TIME_TEST("unsigned int", "multiplying", qutil_uint_mult, ui_out,
              ui_mult_authoritative, ui_array, ui_len);
    TIME_TEST("unsigned int", "findmax()'ing", qutil_uint_max, ui_out,
              ui_max_authoritative, ui_array, ui_len);
    TIME_TEST("unsigned int", "findmin()'ing", qutil_uint_min, ui_out,
              ui_min_authoritative, ui_array, ui_len);

    free(ui_array);

    i_array = calloc(i_len, sizeof(int));
    assert(i_array != NULL);
    for (i = 0; i < i_len; i++) {
        i_array[i] = random() * ((random() > (RAND_MAX / 2)) ? -1 : 1);
        i_sum_authoritative += i_array[i];
        i_mult_authoritative *= i_array[i];
        if (i_max_authoritative < i_array[i]) {
            i_max_authoritative = i_array[i];
        }
        if (i_min_authoritative > i_array[i]) {
            i_min_authoritative = i_array[i];
        }
    }

    i_out = 0;
    TIME_TEST_SIMPLE("signed int", "summing", ADD, i_out, i_sum_authoritative,
                     i_array, i_len);
    i_out = 1;
    TIME_TEST_SIMPLE("signed int", "multiplying", MULT, i_out,
                     i_mult_authoritative, i_array, i_len);
    i_out = INT_MIN;
    TIME_TEST_SIMPLE("signed int", "findmax()'ing", MAX, i_out,
                     i_max_authoritative, i_array, i_len);
    i_out = INT_MAX;
    TIME_TEST_SIMPLE("signed int", "findmin()'ing", MIN, i_out,
                     i_min_authoritative, i_array, i_len);
    TIME_TEST("signed int", "summing", qutil_int_sum, i_out,
              i_sum_authoritative, i_array, i_len);
    TIME_TEST("signed int", "multiplying", qutil_int_mult, i_out,
              i_mult_authoritative, i_array, i_len);
    TIME_TEST("signed int", "findmax()'ing", qutil_int_max, i_out,
              i_max_authoritative, i_array, i_len);
    TIME_TEST("signed int", "findmin()'ing", qutil_int_min, i_out,
              i_min_authoritative, i_array, i_len);

    free(i_array);

    d_array = calloc(d_len, sizeof(double));
    assert(d_array != NULL);
    srandom(0xdeadbeef);
    for (i = 0; i < d_len; i++) {
        d_array[i] = random() / (double)RAND_MAX * 10;

        d_sum_authoritative += d_array[i];
        d_mult_authoritative *= d_array[i];
        if (d_max_authoritative < d_array[i]) {
            d_max_authoritative = d_array[i];
        }
        if (d_min_authoritative > d_array[i]) {
            d_min_authoritative = d_array[i];
        }
    }

    d_out = 0.0;
    TIME_TEST_SIMPLE("double", "summing", ADD, d_out, d_sum_authoritative,
                     d_array, d_len);
    d_out = 1.0;
    TIME_TEST_SIMPLE("double", "multiplying", MULT, d_out,
                     d_mult_authoritative, d_array, d_len);
    d_out = DBL_MIN;
    TIME_TEST_SIMPLE("double", "findmax()'ing", MAX, d_out,
                     d_max_authoritative, d_array, d_len);
    d_out = DBL_MAX;
    TIME_TEST_SIMPLE("double", "findmin()'ing", MIN, d_out,
                     d_min_authoritative, d_array, d_len);
    printf("[test1] summing %lu doubles in parallel", (unsigned long)d_len);
    fflush(stdout);
    gettimeofday(&start, NULL);
    d_out = qutil_double_sum(me, d_array, d_len, 0);
    gettimeofday(&stop, NULL);
    if (fabs(d_out - d_sum_authoritative) >
        (fabs(d_out + d_sum_authoritative) * FLT_EPSILON)) {
        printf("unexpectedly large sum delta: %g (EPSILON = %g)\n",
               fabs(d_out - d_sum_authoritative),
               (fabs(d_out + d_sum_authoritative) * FLT_EPSILON));
    }
    printf(" took: %g seconds\n",
           (stop.tv_sec + (stop.tv_usec * 1.0e-6)) - (start.tv_sec +
                                                      (start.tv_usec *
                                                       1.0e-6)));
    printf("[test1] multiplying %lu doubles in parallel",
           (unsigned long)d_len);
    fflush(stdout);
    gettimeofday(&start, NULL);
    d_out = qutil_double_mult(me, d_array, d_len, 0);
    gettimeofday(&stop, NULL);
    if (fabs(d_out - d_mult_authoritative) >
        fabs(d_out + d_mult_authoritative) * FLT_EPSILON) {
        printf("unexpectedly large mult. delta: %g (EPSILON = %g)\n",
               fabs(d_out - d_mult_authoritative),
               fabs(d_out + d_mult_authoritative) * FLT_EPSILON);
    }
    printf(" took: %g seconds\n",
           (stop.tv_sec + (stop.tv_usec * 1.0e-6)) - (start.tv_sec +
                                                      (start.tv_usec *
                                                       1.0e-6)));
    TIME_TEST("double", "findmax()'ing", qutil_double_max, d_out,
              d_max_authoritative, d_array, d_len);
    TIME_TEST("double", "findmin()'ing", qutil_double_min, d_out,
              d_min_authoritative, d_array, d_len);
    /*qutil_mergesort(me, d_array, d_len, 0);
     * for (i = 0; i < d_len-1; i++) {
     * if (d_array[i] > d_array[i+1]) {
     * printf("out of order at %i: %f > %f\n", i, d_array[i], d_array[i+1]);
     * abort();
     * }
     * } */

    printf("[test1] sorting...\n");
    gettimeofday(&start, NULL);
    qutil_qsort(me, d_array, d_len);
    gettimeofday(&stop, NULL);
    for (i = 0; i < d_len - 1; i++) {
        if (d_array[i] > d_array[i + 1]) {
            /*
             * size_t j;
             *
             * for (j = i-20; j < i+20; j++) {
             * if (j % 8 == 0)
             * printf("\n");
             * printf("[%6u]=%2.5f ", j, d_array[j]);
             * }
             * printf("\n");
             */
            printf("out of order at %lu: %f > %f\n", (unsigned long)i,
                   d_array[i], d_array[i + 1]);
            abort();
        }
    }
    printf("[test1] sorting %lu numbers took: %f seconds\n",
           (unsigned long)d_len,
           (stop.tv_sec + (stop.tv_usec * 1.0e-6)) - (start.tv_sec +
                                                      (start.tv_usec *
                                                       1.0e-6)));
    printf("[test1] resetting random array\n");
    srandom(0xdeadbeef);
    for (i = 0; i < d_len; i++) {
        d_array[i] = random() / (double)RAND_MAX * 10;
    }
    printf("[test1] libc sorting...\n");
    gettimeofday(&start, NULL);
    qsort(d_array, d_len, sizeof(double), dcmp);
    gettimeofday(&stop, NULL);
    for (i = 0; i < d_len - 1; i++) {
        if (d_array[i] > d_array[i + 1]) {
            printf("out of order at %lu: %f > %f\n", (unsigned long)i,
                   d_array[i], d_array[i + 1]);
            abort();
        }
    }
    printf("[test1] libc-sorting %lu numbers took: %f seconds\n",
           (unsigned long)d_len,
           (stop.tv_sec + (stop.tv_usec * 1.0e-6)) - (start.tv_sec +
                                                      (start.tv_usec *
                                                       1.0e-6)));
    free(d_array);
    return 0;
}

int main(int argc, char *argv[])
{
    aligned_t ret;
    unsigned long tcount = 8;

    if (argc == 2) {
        tcount = strtoul(argv[1], NULL, 10);
        if (tcount <= 0) {
            tcount = 1;
        }
        printf("[test1] using %lu threads\n", tcount);
    } else {
        fprintf(stderr, "Usage: %s [numthreads]\n", argv[0]);
        return 0;
    }

    qthread_init(tcount);
    future_init(128);
    qthread_fork(qmain, NULL, &ret);
    qthread_readFF(NULL, NULL, &ret);
    printf("[test1] now, to finalize\n");
    fflush(stdout);
    return 0;
}

/* vim:set expandtab */