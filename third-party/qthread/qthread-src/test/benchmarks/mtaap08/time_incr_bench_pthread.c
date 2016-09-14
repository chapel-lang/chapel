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

aligned_t counter = 0;
pthread_mutex_t counter_lock;

static void *qincr(void *arg)
{
    aligned_t *c = (aligned_t *)arg;
    size_t incrs;

    for (incrs = 0; incrs < PER_THREAD_INCR; incrs++) {
        pthread_mutex_lock(&counter_lock);
        (*c)++;
        pthread_mutex_unlock(&counter_lock);
    }

    return NULL;
}

int main(int argc, char *argv[])
{
    pthread_t rets[NUM_THREADS];
    qtimer_t timer = qtimer_create();
    double cumulative_time = 0.0;

    pthread_mutex_init(&counter_lock, NULL);

    CHECK_VERBOSE();
    printf("this may take a while...\n");

    for (int iteration = 0; iteration < 10; iteration++) {
        counter = 0;
        qtimer_start(timer);
        for (int i = 0; i < NUM_THREADS; i++) {
            pthread_create(&(rets[i]), NULL, qincr, &counter);
        }
        for (int i = 0; i < NUM_THREADS; i++) {
            pthread_join(rets[i], NULL);
        }
        qtimer_stop(timer);
        assert(counter == (NUM_THREADS * PER_THREAD_INCR));
        iprintf("\ttest iteration %i: %f secs\n", iteration,
                qtimer_secs(timer));
        cumulative_time += qtimer_secs(timer);
    }
    printf("pthread time: %f\n", cumulative_time / 10.0);

    return 0;
}

/* vim:set expandtab */