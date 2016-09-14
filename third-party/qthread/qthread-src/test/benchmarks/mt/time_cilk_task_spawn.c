#ifdef HAVE_CONFIG_H
# include "config.h" /* for _GNU_SOURCE */
#endif

#include <assert.h>
#include <stdio.h>
#include <cilk/cilk.h>
#include <cilk/cilk_api.h>
#include <qthread/qthread.h>
#include <qthread/qtimer.h>
#include "argparsing.h"

// static aligned_t donecount = 0;

static aligned_t null_task(void *args_)
{
    return 0;
}

int main(int   argc,
         char *argv[])
{
    unsigned long count = 0;
    unsigned long i;
    int           par_fork = 0;

    qtimer_t timer;
    double   total_time = 0.0;

    CHECK_VERBOSE();

    NUMARG(count, "MT_COUNT");
    NUMARG(par_fork, "MT_PAR_FORK");
    assert(0 != count);

    timer = qtimer_create();

    if (par_fork) {
        qtimer_start(timer);

        _Cilk_for(i = 0; i < count; i++) {
            _Cilk_spawn null_task(NULL);
        }
    } else {
        qtimer_start(timer);

        for (i = 0; i < count; i++) {
            _Cilk_spawn null_task(NULL);
        }
    }
    _Cilk_sync;

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
