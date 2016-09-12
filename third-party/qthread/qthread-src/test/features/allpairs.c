#ifdef HAVE_CONFIG_H
# include "config.h"                   /* for _GNU_SOURCE */
#endif
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <qthread/qthread.h>
#include <qthread/allpairs.h>
#include "argparsing.h"

size_t ASIZE = 1026;

aligned_t hamming = (aligned_t)-1;

static void assigni(const size_t startat,
                    const size_t stopat,
                    qarray * q,
                    void *arg)
{
    int *ptr = (int *)qarray_elem_nomigrate(q, startat);

    for (size_t i = 0; i < (stopat - startat); i++) {
        ptr[i] = (i + startat);
    }
}

static void assignrand(const size_t startat,
                       const size_t stopat,
                       qarray * q,
                       void *arg)
{
    int *ptr = (int *)qarray_elem_nomigrate(q, startat);

    for (size_t i = 0; i < (stopat - startat); i++) {
        ptr[i] = random();
    }
}

#if 0                                  /* unused */
static void printout(int *restrict * restrict out)
{
    size_t i;

    for (i = 0; i < ASIZE; i++) {
        size_t j;

        for (j = 0; j < ASIZE; j++) {
            if (out[i][j] == -1) {
                printf("       _ ");
            } else {
                printf("%8i ", out[i][j]);
            }
            assert(out[i][j] == out[j][i]);
        }
        printf("\n");
    }
}

#endif /* if 0 */

static void mult(const int *inta,
                 const int *intb,
                 int *restrict out)
{
    assert(*out == -1);
    *out = (*inta) * (*intb);
}

static void hammingdist(const int *inta,
                        const int *intb)
{
    unsigned int ham = *inta ^ *intb;
    aligned_t hamdist = 0;

    while (ham != 0) {
        hamdist += ham & 1;
        ham >>= 1;
    }
    if (hamming > hamdist) {
        qthread_lock(&hamming);
        if (hamming > hamdist) {
            hamming = hamdist;
        }
        qthread_unlock(&hamming);
    }
}

int main(int argc,
         char *argv[])
{
    qarray *a1, *a2;
    int **out;
    size_t i;

    assert(qthread_initialize() == QTHREAD_SUCCESS);
    CHECK_VERBOSE();
    NUMARG(ASIZE, "TEST_ASIZE");
    iprintf("ASIZE: %i\n", (int)ASIZE);

    iprintf("%i threads\n", qthread_num_shepherds());

    a1 = qarray_create_tight(ASIZE, sizeof(int));
    a2 = qarray_create_tight(ASIZE, sizeof(int));
    qarray_iter_loop(a1, 0, ASIZE, assigni, NULL);
    qarray_iter_loop(a2, 0, ASIZE, assigni, NULL);

    out = (int **)calloc(ASIZE, sizeof(int *));
    assert(out);
    for (i = 0; i < ASIZE; i++) {
        size_t j;
        out[i] = (int *)calloc(sizeof(int), ASIZE);
        assert(out[i]);
        for (j = 0; j < ASIZE; j++) {
            out[i][j] = -1;
        }
    }

    qt_allpairs_output(a1, a2, (dist_out_f)mult, (void **)out, sizeof(int));
    /*if (verbose) {
     * printout(out);
     * } */
    for (i = 0; i < ASIZE; i++) {
        free(out[i]);
    }
    free(out);

    /* trial #2 */
    qarray_iter_loop(a1, 0, ASIZE, assignrand, NULL);
    qarray_iter_loop(a2, 0, ASIZE, assignrand, NULL);

    qt_allpairs(a1, a2, (dist_f)hammingdist);

    iprintf("minimum hamming distance = %lu\n", (unsigned long)hamming);

    qarray_destroy(a1);
    qarray_destroy(a2);

    return 0;
}

/* vim:set expandtab */
