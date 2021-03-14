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
#define LOCK_COUNT      1000000

aligned_t counters[LOCK_COUNT] = { 0 };

pthread_mutex_t counter_locks[LOCK_COUNT];

static void *qincr(void *arg)
{
    aligned_t id = (aligned_t)(uintptr_t)arg;
    size_t incrs;

    for (incrs = 0; incrs < LOCK_COUNT; incrs++) {
        pthread_mutex_lock(&(counter_locks[incrs]));
        while (counters[incrs] != id) {
            pthread_mutex_unlock(&(counter_locks[incrs]));
            pthread_mutex_lock(&(counter_locks[incrs]));
        }
        counters[incrs]++;
        pthread_mutex_unlock(&(counter_locks[incrs]));
    }

    return NULL;
}

int main(int argc, char *argv[])
{
    pthread_t rets[NUM_THREADS];
    qtimer_t timer = qtimer_create();
    double cumulative_time = 0.0;

    CHECK_VERBOSE();

    for (int i = 0; i < LOCK_COUNT; i++) {
        pthread_mutex_init(&(counter_locks[i]), NULL);
    }
    for (int iteration = 0; iteration < 10; iteration++) {
        memset(counters, 0, sizeof(aligned_t) * LOCK_COUNT);
        qtimer_start(timer);
        for (int i = 0; i < NUM_THREADS; i++) {
            pthread_create(&(rets[i]), NULL, qincr, (void *)(intptr_t)(i));
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
