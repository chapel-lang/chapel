#ifdef HAVE_CONFIG_H
# include "config.h" /* for _GNU_SOURCE */
#endif

#include <assert.h>
#include <stdio.h>
#include <qthread/qthread.h>
#include <qthread/qloop.h>
#include <qthread/qtimer.h>

#define SILENT_ARGPARSING
#include "argparsing.h"
#include "log.h"

static aligned_t donecount = 0;

static aligned_t null_task(void *args_)
{
    return qthread_incr(&donecount, 1);
}

int main(int   argc,
         char *argv[])
{
    uint64_t count    = 1048576;

    qtimer_t timer;
    double   total_time = 0.0;

    CHECK_VERBOSE();

    NUMARG(count, "MT_COUNT");
    assert(0 != count);

    assert(qthread_initialize() == 0);

    timer = qtimer_create();

    qtimer_start(timer);
    for (uint64_t i = 0; i < count; i++) {
        qthread_spawn(null_task, NULL, 0, NULL, 0, NULL, NO_SHEPHERD, QTHREAD_SPAWN_SIMPLE);
    }
    do {
        qthread_yield();
    } while (donecount != count);
    qtimer_stop(timer);

    total_time = qtimer_secs(timer);

    qtimer_destroy(timer);

    LOG_SPAWN_SEQUENTIAL_YAML(count, total_time)
    LOG_ENV_QTHREADS_YAML()

    return 0;
}

/* vim:set expandtab */
