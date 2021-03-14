#ifdef HAVE_CONFIG_H
# include "config.h" /* for _GNU_SOURCE */
#endif

#include <assert.h>
#include <stdio.h>
#include <cilk/cilk.h>
#include <cilk/cilk_api.h>
#include <qthread/qthread.h>
#include <qthread/qtimer.h>

#define SILENT_ARGPARSING
#include "argparsing.h"
#include "log.h"

// static aligned_t donecount = 0;

static aligned_t null_task(void *args_)
{
    return 0;
}

int main(int   argc,
         char *argv[])
{
    unsigned long count = 1048576;
    unsigned long i;

    qtimer_t timer;
    double   total_time = 0.0;

    CHECK_VERBOSE();

    NUMARG(count, "MT_COUNT");
    assert(0 != count);

    timer = qtimer_create();
    qtimer_start(timer);
    _Cilk_for(i = 0; i < count; i++) {
        _Cilk_spawn null_task(NULL);
    }
    _Cilk_sync;
    qtimer_stop(timer);

    total_time = qtimer_secs(timer);

    qtimer_destroy(timer);

    LOG_SPAWN_PARALLEL_YAML(count, total_time)
    LOG_ENV_CILK_YAML()

    return 0;
}

/* vim:set expandtab */
