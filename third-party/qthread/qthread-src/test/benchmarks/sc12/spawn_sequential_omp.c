#ifdef HAVE_CONFIG_H
# include "config.h" /* for _GNU_SOURCE */
#endif

#include <assert.h>
#include <stdio.h>
#include <omp.h>

#include <qthread/qthread.h>
#include <qthread/qtimer.h>

#define SILENT_ARGPARSING
#include "argparsing.h"
#include "log.h"

static aligned_t null_task(void *args_)
{
    return 0;
}

int main(int   argc,
         char *argv[])
{
    uint64_t      count    = 1048576;
    unsigned long threads  = 1;

    qtimer_t timer;
    double   total_time = 0.0;

    CHECK_VERBOSE();

    NUMARG(count, "MT_COUNT");
    assert(0 != count);

#pragma omp parallel
#pragma omp single
    {
        timer   = qtimer_create();
        threads = omp_get_num_threads();

        qtimer_start(timer);
#pragma omp task untied
        for (uint64_t i = 0; i < count; i++) {
#pragma omp task untied
            null_task(NULL);
        }
#pragma omp taskwait
        qtimer_stop(timer);
    }

    total_time = qtimer_secs(timer);

    qtimer_destroy(timer);

    LOG_SPAWN_SEQUENTIAL_YAML(count, total_time)
    LOG_ENV_OMP_YAML(threads)

    return 0;
}

/* vim:set expandtab */
