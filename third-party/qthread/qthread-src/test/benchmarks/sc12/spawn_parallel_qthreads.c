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

static void par_null_task(size_t start,
                          size_t stop,
                          void  *args_)
{}

static void par_null_task2(size_t start,
                          size_t stop,
                          void  *args_)
{
    for (size_t i=start; i<stop; ++i) {
    }
}

static aligned_t donecount = 0;

static aligned_t null_task(void *args_)
{
    return qthread_incr(&donecount, 1);
}

static void par_spawn(size_t start,
                          size_t stop,
                          void  *args_)
{
    for (size_t i=start; i<stop; ++i) {
        qthread_spawn(null_task, NULL, 0, NULL, 0, NULL, NO_SHEPHERD, QTHREAD_SPAWN_SIMPLE);
    }
}

int main(int   argc,
         char *argv[])
{
    uint64_t count      = 1048576;
    uint64_t loop_style = 1;

    qtimer_t timer;
    double   total_time = 0.0;

    CHECK_VERBOSE();

    NUMARG(loop_style, "LOOP_STYLE");
    NUMARG(count, "MT_COUNT");
    assert(0 != count);

    assert(qthread_initialize() == 0);

    timer = qtimer_create();

    qqloop_handle_t *l = NULL;
    switch (loop_style) {
	case 1:
	    qtimer_start(timer);
	    qt_loop(0, count, par_null_task, NULL);
	    qtimer_stop(timer);
	    break;
	case 2:
	    qtimer_start(timer);
	    qt_loop_balance(0, count, par_null_task2, NULL);
	    qtimer_stop(timer);
	    break;
	case 3:
	    qtimer_start(timer);
	    l = qt_loop_queue_create(CHUNK, 0, count, 1, par_null_task2, NULL);
	    qt_loop_queue_run(l);
	    qtimer_stop(timer);
	    break;
	case 4:
	    qtimer_start(timer);
	    l = qt_loop_queue_create(GUIDED, 0, count, 1, par_null_task2, NULL);
	    qt_loop_queue_run(l);
	    qtimer_stop(timer);
	    break;
	case 5:
	    qtimer_start(timer);
	    l = qt_loop_queue_create(FACTORED, 0, count, 1, par_null_task2, NULL);
	    qt_loop_queue_run(l);
	    qtimer_stop(timer);
	    break;
	case 6:
	    qtimer_start(timer);
	    l = qt_loop_queue_create(TIMED, 0, count, 1, par_null_task2, NULL);
	    qt_loop_queue_run(l);
	    qtimer_stop(timer);
	    break;
	case 7:
	    qtimer_start(timer);
	    qt_loop_balance(0, count, par_spawn, NULL);
	    while (donecount != count) {
		qthread_yield();
	    }
	    qtimer_stop(timer);
	    break;
	case 8:
	    qtimer_start(timer);
	    qt_loop_simple(0, count, par_null_task, NULL);
	    qtimer_stop(timer);
	    break;
    }

    total_time = qtimer_secs(timer);

    qtimer_destroy(timer);

    LOG_SPAWN_PARALLEL_YAML(count, total_time)
    LOG_ENV_QTHREADS_YAML()
    printf("Loop style: %lu\n", loop_style);

    return 0;
}

/* vim:set expandtab */
