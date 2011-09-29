#ifdef HAVE_CONFIG_H
# include "config.h"                   /* for _GNU_SOURCE */
#endif
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <qthread/qloop.h>
#include <qthread/qtimer.h>
#include "argparsing.h"

static aligned_t threads  = 0;
static aligned_t numincrs = 1024;
static size_t *sleeplen;

static void sum(const size_t startat,
                const size_t stopat,
                void        *arg_)
{
    qthread_incr(&threads, stopat - startat);
    for (size_t i=startat; i<stopat; ++i) {
	usleep(sleeplen[i]);
    }
}

int main(int   argc,
         char *argv[])
{
    double total = 0;
    size_t numiters = 10;

    assert(qthread_initialize() == QTHREAD_SUCCESS);
    CHECK_VERBOSE();
    NUMARG(numincrs, "NUM_INCRS");
    NUMARG(numiters, "NUM_ITERS");
    iprintf("%i shepherds\n", qthread_num_shepherds());
    iprintf("%i threads\n", qthread_num_workers());

    sleeplen = malloc(sizeof(size_t) * numincrs);
    for (int i = 0; i < numincrs; ++i) {
	sleeplen[i] = qtimer_fastrand()%1000;
    }

    iprintf("version    sync    iters    time\n");
    qtimer_t timer = qtimer_create();

    for (int i = 0; i < numiters; ++i) {
	threads = 0;
        qtimer_start(timer);
        qt_loop(0, numincrs, sum, NULL);
        assert(threads == numincrs);
        qtimer_stop(timer);
        total += qtimer_secs(timer);
    }
    printf("thread per iteration syncvar %8lu %f\n", (unsigned long)numincrs,
           total / numiters);

    total   = 0;
    for (int i = 0; i < numiters; ++i) {
	threads = 0;
        qtimer_start(timer);
        qt_loop_sinc(0, numincrs, sum, NULL);
        assert(threads == numincrs);
        qtimer_stop(timer);
        total += qtimer_secs(timer);
    }
    printf("thread per iteration sinc    %8lu %f\n", (unsigned long)numincrs,
           total / numiters);

    total   = 0;
    for (int i = 0; i < numiters; ++i) {
	threads = 0;
        qtimer_start(timer);
        qt_loop_balance(0, numincrs, sum, NULL);
        assert(threads == numincrs);
        qtimer_stop(timer);
        total += qtimer_secs(timer);
    }
    printf("balanced             syncvar %8lu %f\n", (unsigned long)numincrs,
           total / numiters);

    total   = 0;
    for (int i = 0; i < numiters; ++i) {
	threads = 0;
	qtimer_start(timer);
	qt_loop_balance_sinc(0, numincrs, sum, NULL);
	assert(threads == numincrs);
        qtimer_stop(timer);
        total += qtimer_secs(timer);
    }
    printf("balanced             sinc    %8lu %f\n", (unsigned long)numincrs,
           total / numiters);

    return 0;
}

/* vim:set expandtab */
