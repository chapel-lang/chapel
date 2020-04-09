#include <stdio.h>                     /* for printf() */
#include <stdlib.h>                    /* for strtol() */
#include <assert.h>                    /* for assert() */
#include <qthread/qthread.h>
#include <qthread/qloop.h>
#include <qthread/qtimer.h>
#include "argparsing.h"

static size_t NUM_TASKS, NUM_ELEMS;
static size_t rvs_count, rvs_size;

static size_t    *rvs           = NULL;
static aligned_t *aligned_elems = NULL;
static syncvar_t *syncvar_elems = NULL;

static void aligned_visit(const size_t startat,
                          const size_t stopat,
                          void        *args_)
{
    if (startat < NUM_TASKS) {
        qthread_readFE(NULL, aligned_elems + rvs[startat]);
    } else {
        qthread_writeEF_const(aligned_elems + rvs[startat], 42);
    }
}

static void syncvar_visit(const size_t startat,
                          const size_t stopat,
                          void        *args_)
{
    if (startat < NUM_TASKS) {
        qthread_syncvar_readFE(NULL, syncvar_elems + rvs[startat]);
    } else {
        qthread_syncvar_writeEF_const(syncvar_elems + rvs[startat], 42);
    }
}

int main(int   argc,
         char *argv[])
{
    unsigned int shepherds = 1;
    unsigned int workers   = 1;

    /* setup */
    assert(qthread_initialize() == QTHREAD_SUCCESS);

    CHECK_VERBOSE();
    NUMARG(NUM_TASKS, "NUM_TASKS");
    NUMARG(NUM_ELEMS, "NUM_ELEMS");

    shepherds = qthread_num_shepherds();
    workers   = qthread_num_workers();

    rvs_count = 2 * NUM_TASKS;
    rvs_size  = sizeof(size_t) * rvs_count;

    rvs = malloc(rvs_size);
    assert(rvs);

    for (size_t i = 0; i < NUM_TASKS; i++) {
        rvs[NUM_TASKS + i] = rvs[i] = rand() % NUM_ELEMS;
    }
    for (size_t i = NUM_TASKS; i < rvs_count; ++i) {
        size_t tmp;
        size_t j = NUM_TASKS + (rand() % NUM_TASKS);
        tmp    = rvs[j];
        rvs[j] = rvs[i];
        rvs[i] = tmp;
    }

    fprintf(stderr, "feb time num_tasks num_elems block_size num_sheps workers workers_per_shep\n");

    // Aligned FEB
    {
        qtimer_t timer = qtimer_create();
        aligned_elems = malloc(sizeof(aligned_t) * NUM_ELEMS);
        assert(aligned_elems);

        qtimer_start(timer); // Warning: this style relies on spawn caching
        qt_loop(0, rvs_count, aligned_visit, NULL);
        qtimer_stop(timer);

        printf("aligned_t %g %u %u %u %u %u %u\n", qtimer_secs(timer),
               (unsigned)NUM_TASKS, (unsigned)NUM_ELEMS, (unsigned)rvs_count,
               (unsigned)shepherds, (unsigned)workers,
               (unsigned)(workers / shepherds));

        free(aligned_elems);
        qtimer_destroy(timer);
    }

    // Syncvar FEB
    {
        qtimer_t timer = qtimer_create();
        size_t   i;
        syncvar_elems = malloc(sizeof(syncvar_t) * NUM_ELEMS);
        assert(syncvar_elems);
        for (i = 0; i < NUM_ELEMS; i++) {
            syncvar_elems[i] = SYNCVAR_INITIALIZER;
        }

        qtimer_start(timer); // Warning: this style relies on spawn caching
        qt_loop(0, rvs_count, syncvar_visit, NULL);
        qtimer_stop(timer);

        printf("syncvar_t %g %u %u %u %u %u %u\n", qtimer_secs(timer),
               (unsigned)NUM_TASKS, (unsigned)NUM_ELEMS, (unsigned)rvs_count,
               (unsigned)shepherds, (unsigned)workers,
               (unsigned)(workers / shepherds));

        free(syncvar_elems);
        qtimer_destroy(timer);
    }

    free(rvs);

    return 0;
}

/* vim:set expandtab */
