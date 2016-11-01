#ifdef HAVE_CONFIG_H
# include "config.h"                   /* for _GNU_SOURCE */
#endif
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <qthread/qthread.h>
#include <qthread/qtimer.h>
#include "argparsing.h"

#include <pthread.h>

#define NUM_THREADS     10
#define PER_THREAD_INCR 1000000

aligned_t counters[PER_THREAD_INCR] = { 0 };

static aligned_t qincr(void *arg)
{
    aligned_t *c = (aligned_t *)arg;
    size_t incrs;

    for (incrs = 0; incrs < PER_THREAD_INCR; incrs++) {
        qthread_lock(&(c[incrs]));
        qthread_unlock(&(c[incrs]));
    }
    return 0;
}

int main(int argc, char *argv[])
{
    aligned_t rets[NUM_THREADS];
    qtimer_t timer = qtimer_create();
    double cumulative_time = 0.0;

    CHECK_VERBOSE();

    if (qthread_initialize() != QTHREAD_SUCCESS) {
        fprintf(stderr, "qthread library could not be initialized!\n");
        exit(EXIT_FAILURE);
    }

    for (int iteration = 0; iteration < 10; iteration++) {
        qtimer_start(timer);
        for (int i = 0; i < NUM_THREADS; i++) {
            qthread_fork(qincr, counters, &(rets[i]));
        }
        for (int i = 0; i < NUM_THREADS; i++) {
            qthread_readFF(NULL, &(rets[i]));
        }
        qtimer_stop(timer);
        iprintf("\ttest iteration %i: %f secs\n", iteration,
                qtimer_secs(timer));
        cumulative_time += qtimer_secs(timer);
    }
    printf("qthread time: %f\n", cumulative_time / 10.0);

    return 0;
}

/* vim:set expandtab */