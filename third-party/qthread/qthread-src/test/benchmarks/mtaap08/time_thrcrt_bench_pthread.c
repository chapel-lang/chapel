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

#define NUM_THREADS     1000000
#define MAX_THREADS     400
#define THREAD_BLOCK    200

static void *qincr(void *arg)
{
    return NULL;
}

int main(int argc, char *argv[])
{
    pthread_t rets[NUM_THREADS];
    qtimer_t timer = qtimer_create();
    double cumulative_time = 0.0;
    size_t counter;

    CHECK_VERBOSE();

    for (int iteration = 0; iteration < 10; iteration++) {
        qtimer_start(timer);
        for (int i = 0; i < MAX_THREADS; i++) {
            pthread_create(&(rets[i]), NULL, qincr, NULL);
        }
        counter = MAX_THREADS;
        while (counter < NUM_THREADS) {
            for (int i = 0; i < THREAD_BLOCK; i++) {
                pthread_join(rets[i], NULL);
            }
            for (int i = 0; i < THREAD_BLOCK; i++) {
                pthread_create(&(rets[i]), NULL, qincr, &counter);
            }
            for (int i = THREAD_BLOCK; i < MAX_THREADS; i++) {
                pthread_join(rets[i], NULL);
            }
            for (int i = THREAD_BLOCK; i < MAX_THREADS; i++) {
                pthread_create(&(rets[i]), NULL, qincr, &counter);
            }
            counter += MAX_THREADS;
        }
        for (int i = 0; i < MAX_THREADS; i++) {
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