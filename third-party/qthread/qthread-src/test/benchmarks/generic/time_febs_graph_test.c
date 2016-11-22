#include <stdio.h>                     /* for printf() */
#include <stdlib.h>                    /* for strtol() */
#include <assert.h>                    /* for assert() */
#include <qthread/qthread.h>
#include <qthread/qloop.h>
#include <qthread/qtimer.h>
#include "argparsing.h"

static size_t NUM_TASKS, NUM_ELEMS;
static size_t rvs_count, rvs_size;

static aligned_t *aligned_elems = NULL;
static syncvar_t *syncvar_elems = NULL;

static aligned_t aligned_visit(void *args_)
{
    size_t *rvs = (size_t *)args_;

    aligned_t tmp;
    size_t    i;

    for (i = 0; i < rvs_count; i++) {
        qthread_readFE(&tmp, aligned_elems + rvs[i]);
        qthread_writeF_const(aligned_elems + rvs[i], tmp + 1);
    }
    return 0;
}

static aligned_t syncvar_visit(void *args_)
{
    size_t *rvs = (size_t *)args_;

    aligned_t tmp;
    size_t    i;

    for (i = 0; i < rvs_count; i++) {
        qthread_syncvar_readFE(&tmp, syncvar_elems + rvs[i]);
        qthread_syncvar_writeF_const(syncvar_elems + rvs[i], tmp + 1);
    }
    return 0;
}

int main(int   argc,
         char *argv[])
{
    aligned_t   *rets;
    unsigned int shepherds = 1;
    unsigned int workers   = 1;

    /* setup */
    assert(qthread_initialize() == QTHREAD_SUCCESS);

    CHECK_VERBOSE();
    NUMARG(NUM_TASKS, "NUM_TASKS");
    NUMARG(NUM_ELEMS, "NUM_ELEMS");

    shepherds = qthread_num_shepherds();
    workers   = qthread_num_workers();

    rets = malloc(sizeof(aligned_t) * NUM_TASKS);
    assert(rets);

    assert(NUM_ELEMS % NUM_TASKS == 0);
    rvs_count = NUM_ELEMS / NUM_TASKS;
    rvs_size  = sizeof(long int) * rvs_count;

    size_t *rvs = malloc(rvs_size);
    assert(rvs);

    fprintf(stderr, "feb time num_tasks num_elems block_size num_sheps workers workers_per_shep\n");

    // Aligned FEB
    {
        qtimer_t timer = qtimer_create();
        size_t   i;
        aligned_elems = malloc(sizeof(aligned_t) * NUM_ELEMS);
        assert(aligned_elems);

        for (i = 0; i < NUM_TASKS; i++) {
            for (size_t j = 0; j < rvs_count; j++) {
                rvs[j] = rand() % NUM_ELEMS;
            }

            qthread_fork_copyargs(aligned_visit, rvs, rvs_size, rets + i);
        }

        qtimer_start(timer); // Warning: this style relies on spawn caching
        qthread_yield();

        for (i = 0; i < NUM_TASKS; i++) {
            qthread_readFF(NULL, rets + i);
        }
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

        for (i = 0; i < NUM_TASKS; i++) {
            for (size_t j = 0; j < rvs_count; j++) {
                rvs[j] = rand() % NUM_ELEMS;
            }

            qthread_fork_copyargs(syncvar_visit, rvs, rvs_size, rets + i);
        }

        qtimer_start(timer); // Warning: this style relies on spawn caching
        qthread_yield();

        for (i = 0; i < NUM_TASKS; i++) {
            qthread_readFF(NULL, rets + i);
        }
        qtimer_stop(timer);
        printf("syncvar %g %u %u %u %u %u %u\n", qtimer_secs(timer),
               (unsigned)NUM_TASKS, (unsigned)NUM_ELEMS, (unsigned)rvs_count,
	       (unsigned)shepherds, (unsigned)workers,
	       (unsigned)(workers / shepherds));

        free(syncvar_elems);
        qtimer_destroy(timer);
    }

    free(rvs);
    free(rets);

    return 0;
}

/* vim:set expandtab */
