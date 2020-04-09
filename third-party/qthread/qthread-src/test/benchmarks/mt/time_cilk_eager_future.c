#ifdef HAVE_CONFIG_H
# include "config.h" /* for _GNU_SOURCE */
#endif

#include <assert.h>
#include <stdio.h>
#include <cilk/cilk.h>
#include <cilk/cilk_api.h>
#include <pthread.h>
#include <qthread/qthread.h>
#include <qthread/qtimer.h>
#include "argparsing.h"

static uint64_t        *rets     = NULL;
static pthread_mutex_t *ret_sync = NULL;

static aligned_t null_task(void *args_)
{
    aligned_t d = 0;

    rets[(uintptr_t)args_] = d;
    pthread_mutex_unlock(&ret_sync[(uintptr_t)args_]);
    return d;
}

int main(int   argc,
         char *argv[])
{
    uint64_t count    = 0;
    int      par_fork = 0;

    qtimer_t timer;
    double   total_time = 0.0;

    CHECK_VERBOSE();

    NUMARG(count, "MT_COUNT");
    NUMARG(par_fork, "MT_PAR_FORK");
    assert(0 != count);

    rets     = malloc(sizeof(uint64_t) * count);
    ret_sync = malloc(sizeof(pthread_mutex_t) * count);
    for (uint64_t i = 0; i < count; i++) {
        rets[i] = 0;
        pthread_mutex_init(&ret_sync[i], NULL);
        pthread_mutex_lock(&ret_sync[i]);
    }

    timer = qtimer_create();
    qtimer_start(timer);

    if (par_fork) {
        _Cilk_for(uint64_t i = 0; i < count; i++) {
            rets[i] = _Cilk_spawn null_task((void *)(uintptr_t)i);
        }
    } else {
        for (uint64_t i = 0; i < count; i++) {
            rets[i] = _Cilk_spawn null_task((void *)(uintptr_t)i);
        }
    }

    for (uint64_t i = 0; i < count; i++) {
        pthread_mutex_lock(&ret_sync[i]);
    }

    qtimer_stop(timer);

    total_time = qtimer_secs(timer);

    qtimer_destroy(timer);

    printf("%lu %lu %f\n",
           (unsigned long)__cilkrts_get_nworkers(),
           (unsigned long)count,
           total_time);

    return 0;
}

/* vim:set expandtab */
