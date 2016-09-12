#ifdef HAVE_CONFIG_H
# include "config.h"                   /* for _GNU_SOURCE */
#endif

#include <stdlib.h>
#include <stdio.h>
#include <limits.h>                    /* for INT_MIN & friends (according to C89) */
#include <float.h>                     /* for DBL_EPSILON (according to C89) */
#include <math.h>                      /* for fabs() */
#include <assert.h>

#include <sys/time.h>                  /* for gettimeofday() */
#include <time.h>                      /* for gettimeofday() */

#include <qthread/qutil.h>
#include "argparsing.h"

struct timeval start, stop;

int main(int   argc,
         char *argv[])
{
    aligned_t *ui_array;
    double    *d_array;
    size_t     len = 1000000, i;

    assert(qthread_initialize() == QTHREAD_SUCCESS);

    CHECK_VERBOSE();
    NUMARG(len, "TEST_LEN");

    ui_array = (aligned_t *)calloc(len, sizeof(aligned_t));
    for (i = 0; i < len; i++) {
        ui_array[i] = random();
    }
    iprintf("ui_array generated...\n");
    gettimeofday(&start, NULL);
    qutil_aligned_qsort(ui_array, len);
    gettimeofday(&stop, NULL);
    iprintf("done sorting, checking correctness...\n");
    for (i = 0; i < len - 1; i++) {
        if (ui_array[i] > ui_array[i + 1]) {
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
            fprintf(stderr, "out of order at %lu: %lu > %lu\n",
                    (unsigned long)i, (unsigned long)ui_array[i],
                    (unsigned long)ui_array[i + 1]);
            abort();
        }
    }
    iprintf("sorting %lu aligned_ts took: %f seconds\n", (unsigned long)len,
            (stop.tv_sec + (stop.tv_usec * 1.0e-6)) - (start.tv_sec +
                                                       (start.tv_usec *
                                                        1.0e-6)));
    free(ui_array);

    d_array = (double *)calloc(len, sizeof(double));
    for (i = 0; i < len; i++) {
        d_array[i] = random() / (double)RAND_MAX * 10;
    }
    iprintf("d_array generated...\n");
    gettimeofday(&start, NULL);
    qutil_qsort(d_array, len);
    gettimeofday(&stop, NULL);
    iprintf("done sorting, checking correctness...\n");
    for (i = 0; i < len - 1; i++) {
        if (fabs(d_array[i] - d_array[i + 1]) >
            fabs(d_array[i] + d_array[i + 1]) * FLT_EPSILON) {
            /* equal */
        } else if (d_array[i] > d_array[i + 1]) {
            fprintf(stderr, "out of order at %lu: %f > %f\n",
                    (unsigned long)i, d_array[i], d_array[i + 1]);
            abort();
        }
    }
    iprintf("sorting %lu doubles took: %f seconds\n", (unsigned long)len,
            (stop.tv_sec + (stop.tv_usec * 1.0e-6)) - (start.tv_sec +
                                                       (start.tv_usec *
                                                        1.0e-6)));
    free(d_array);

    return 0;
}

/* vim:set expandtab */
