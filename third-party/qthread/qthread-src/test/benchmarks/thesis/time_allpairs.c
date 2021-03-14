#ifdef HAVE_CONFIG_H
# include "config.h"                   /* for _GNU_SOURCE */
#endif
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <qthread/qthread.h>
#include <qthread/allpairs.h>
#include <qthread/qtimer.h>
#include "argparsing.h"

size_t ASIZE = 1026;

aligned_t hamming = (aligned_t)-1;

static void assigni(const size_t startat, const size_t stopat,
                    qarray * q, void *arg)
{
    int *ptr = qarray_elem_nomigrate(q, startat);

    for (size_t i = 0; i < (stopat - startat); i++) {
        ptr[i] = (i + startat);
    }
}

static void mult(const int *inta, const int *intb, int *restrict out)
{
    *out = (*inta) * (*intb);
}

int main(int argc, char *argv[])
{
    qarray *a1, *a2;
    int **out;
    qtimer_t timer = qtimer_create();
    double cumulative_time = 0.0;

    if (qthread_initialize() != QTHREAD_SUCCESS) {
        fprintf(stderr, "qthread library could not be initialized!\n");
        exit(EXIT_FAILURE);
    }
    CHECK_VERBOSE();
    NUMARG(ASIZE, "TEST_ASIZE");
    iprintf("threads: %i\n", qthread_num_shepherds());

    a1 = qarray_create_configured(ASIZE, sizeof(int), FIXED_HASH, 1, 1);
    a2 = qarray_create_configured(ASIZE, sizeof(int), FIXED_HASH, 1, 1);
    iprintf("segments of %u elements\n", (unsigned int)a1->segment_size);
    qarray_iter_loop(a1, 0, ASIZE, assigni, NULL);
    qarray_iter_loop(a2, 0, ASIZE, assigni, NULL);

    out = calloc(ASIZE, sizeof(int *));
    assert(out);
    for (size_t i = 0; i < ASIZE; i++) {
        out[i] = calloc(sizeof(int), ASIZE);
        assert(out[i]);
    }
    iprintf("all initialized\n");

    for (int i = 0; i < 10; i++) {
        qtimer_start(timer);
        qt_allpairs_output(a1, a2, (dist_out_f)mult, (void **)out,
                           sizeof(int));
        qtimer_stop(timer);
        cumulative_time += qtimer_secs(timer);
        iprintf("\t%i: mult time %f\n", i, qtimer_secs(timer));
    }
    printf("mult time: %f (avg)\n", cumulative_time / 10.0);
    for (size_t i = 0; i < ASIZE; i++) {
        free(out[i]);
    }
    free(out);

    qtimer_destroy(timer);

    qarray_destroy(a1);
    qarray_destroy(a2);

    return 0;
}

/* vim:set expandtab */