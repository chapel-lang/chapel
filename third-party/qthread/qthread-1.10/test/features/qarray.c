#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <qthread/qthread.h>
#include <qthread/qarray.h>
#include "argparsing.h"

static unsigned int ELEMENT_COUNT = 1000;

aligned_t count = 0;
typedef struct {
    char pad[10000];
} bigobj;
typedef struct {
    char pad[41];
} offsize;

static aligned_t assign1(void *arg)
{
    *(double *)arg = 1.0;
    qthread_incr(&count, 1);
    return 0;
}

static aligned_t assignall1(void *arg)
{
    memset(arg, 1, sizeof(bigobj));
    qthread_incr(&count, 1);
    return 0;
}

static void assignoff1(const size_t startat,
                       const size_t stopat,
                       qarray * q,
                       void *arg)
{
    for (size_t i = startat; i < stopat; i++) {
        void *ptr = qarray_elem_nomigrate(q, i);

        memset(ptr, 1, sizeof(offsize));
    }
    qthread_incr(&count, stopat - startat);
}

int main(int argc,
         char *argv[])
{
    qarray *a;
    distribution_t disttypes[] = {
        FIXED_HASH, FIXED_FIELDS,
        ALL_LOCAL, ALL_RAND, ALL_LEAST,
        DIST_RAND, DIST_STRIPES, DIST_FIELDS, DIST_LEAST
    };
    const char *distnames[] = {
        "FIXED_HASH", "FIXED_FIELDS",
        "ALL_LOCAL", "ALL_RAND", "ALL_LEAST",
        "DIST_RAND", "DIST_STRIPES", "DIST_FIELDS", "DIST_LEAST"
    };
    unsigned int dt_index;
    unsigned int num_dists = sizeof(disttypes) / sizeof(distribution_t);
    unsigned int dists = (1 << num_dists) - 1;

    qthread_initialize();
    CHECK_VERBOSE();
    NUMARG(dists, "TEST_DISTS");
    NUMARG(ELEMENT_COUNT, "ELEMENT_COUNT");

    /* iterate over all the different distribution types */
    for (dt_index = 0; dt_index < num_dists; dt_index++) {
        if ((dists & (1 << dt_index)) == 0) {
            continue;
        }
        /* test a basic array of doubles */
        count = 0;
        a = qarray_create_configured(ELEMENT_COUNT, sizeof(double),
                                     disttypes[dt_index], 0, 0);
        assert(a);
        iprintf("%s: created basic array of doubles\n", distnames[dt_index]);
        qarray_iter(a, 0, ELEMENT_COUNT, assign1);
        iprintf("%s: iterated; now checking work...\n", distnames[dt_index]);
        if (count != ELEMENT_COUNT) {
            printf("count = %lu, dt_index = %u\n", (unsigned long)count,
                   dt_index);
            assert(count == ELEMENT_COUNT);
        }
        {
            size_t i;

            for (i = 0; i < ELEMENT_COUNT; i++) {
                double elem = *(double *)qarray_elem_nomigrate(a, i);

                if (elem != 1.0) {
                    printf
                        ("element %lu is %f instead of 1.0, disttype = %s\n",
                        (unsigned long)i, elem, distnames[dt_index]);
                    assert(elem == 1.0);
                }
            }
        }
        iprintf("%s: correct result!\n", distnames[dt_index]);
        qarray_destroy(a);

        /* now test an array of giant things */
        count = 0;
        a = qarray_create_configured(ELEMENT_COUNT, sizeof(bigobj),
                                     disttypes[dt_index], 0, 0);
        iprintf("%s: created array of big objects\n", distnames[dt_index]);
        qarray_iter(a, 0, ELEMENT_COUNT, assignall1);
        iprintf("%s: iterated; now checking work...\n", distnames[dt_index]);
        if (count != ELEMENT_COUNT) {
            printf("count = %lu, dt_index = %u\n", (unsigned long)count,
                   dt_index);
            // assert(count == ELEMENT_COUNT);
        }
        {
            size_t i;
            char fail = 0;

            for (i = 0; i < ELEMENT_COUNT; i++) {
                char *elem = (char *)qarray_elem_nomigrate(a, i);
                size_t j;

                for (j = 0; j < sizeof(bigobj); j++) {
                    if (elem[j] != 1) {
                        printf
                        (
                         "byte %lu of element %lu is %i instead of 1, dt_index = %u\n",
                         (unsigned long)j, (unsigned long)i, elem[j],
                         dt_index);
                        fail = 1;
                        break;
                    }
                }
            }
            assert(fail == 0);
        }
        iprintf("%s: correct result!\n", distnames[dt_index]);
        qarray_destroy(a);

        /* now test an array of weird-sized things */
        count = 0;
        a = qarray_create_configured(ELEMENT_COUNT, sizeof(offsize),
                                     disttypes[dt_index], 0, 0);
        iprintf("%s: created array of odd-sized objects\n",
                distnames[dt_index]);
        qarray_iter_loop(a, 0, ELEMENT_COUNT, assignoff1, NULL);
        iprintf("%s: iterated; now checking work...\n", distnames[dt_index]);
        if (count != ELEMENT_COUNT) {
            printf("count = %lu, dt_index = %u\n", (unsigned long)count,
                   dt_index);
            assert(count == ELEMENT_COUNT);
        }
        {
            size_t i;

            for (i = 0; i < ELEMENT_COUNT; i++) {
                char *elem = (char *)qarray_elem_nomigrate(a, i);
                size_t j;

                for (j = 0; j < sizeof(offsize); j++) {
                    if (elem[j] != 1) {
                        printf
                        (
                         "byte %lu of element %lu is %i instead of 1, dt_index = %u\n",
                         (unsigned long)j, (unsigned long)i, elem[j],
                         dt_index);
                        assert(elem[j] == 1);
                    }
                }
            }
        }
        iprintf("%s: correct result!\n", distnames[dt_index]);
        qarray_destroy(a);
    }

    return 0;
}

/* vim:set expandtab */
