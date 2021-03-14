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

pthread_mutex_t locks[PER_THREAD_INCR];

static void *qincr(void *arg)
{
    size_t incrs;

    for (incrs = 0; incrs < PER_THREAD_INCR; incrs++) {
        pthread_mutex_lock(&(locks[incrs]));
        pthread_mutex_unlock(&(locks[incrs]));
    }

    return NULL;
}

int main(int argc, char *argv[])
{
    pthread_t rets[NUM_THREADS];
    qtimer_t timer = qtimer_create();
    double cumulative_time = 0.0;

    CHECK_VERBOSE();

    for (int i = 0; i < PER_THREAD_INCR; i++) {
        pthread_mutex_init(&(locks[i]), NULL);
    }

    for (int iteration = 0; iteration < 10; iteration++) {
        qtimer_start(timer);
        for (int i = 0; i < NUM_THREADS; i++) {
            pthread_create(&(rets[i]), NULL, qincr, NULL);
        }
        for (int i = 0; i < NUM_THREADS; i++) {
            pthread_join(rets[i], NULL);
        }
        qtimer_stop(timer);
        iprintf("\ttest iteration %i: %f secs\n", iteration,
                qtimer_secs(timer));
        cumulative_time += qtimer_secs(timer);
    }
    printf("pthread time: %f\n", cumulative_time / 10.0);

    return 0;
}

/* vim:set expandtab */