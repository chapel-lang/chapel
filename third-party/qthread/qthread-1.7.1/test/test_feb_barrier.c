#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <qthread/qthread.h>
#include <qthread/feb_barrier.h>
#include <qthread/qtimer.h>
#include "argparsing.h"

aligned_t           initme_idx = 0;
volatile aligned_t *initme     = NULL;
qt_feb_barrier_t   *wait_on_me;

static aligned_t barrier_thread(void *arg)
{
    qt_feb_barrier_t *b   = (qt_feb_barrier_t *)arg;
    aligned_t         idx = qthread_incr(&initme_idx, 1);

    qthread_incr(&(initme[idx]), 1);
    qt_feb_barrier_enter(b);
    return 0;
}

int main(int   argc,
         char *argv[])
{
    size_t     threads = 1000, i;
    aligned_t *rets;
    qtimer_t   t;

    assert(qthread_initialize() == 0);
    t = qtimer_create();

    CHECK_VERBOSE();
    NUMARG(threads, "THREADS");

    iprintf("%i shepherds...\n", qthread_num_shepherds());
    iprintf("%i threads...\n", (int)threads);

    initme = (aligned_t *)calloc(threads, sizeof(aligned_t));
    assert(initme);

    rets = (aligned_t *)malloc(threads * sizeof(aligned_t));
    assert(rets);

    iprintf("creating the barrier for %zu threads\n", threads + 1);
    wait_on_me = qt_feb_barrier_create(threads + 1);    // all my spawnees plus me
    assert(wait_on_me);

    iprintf("forking the threads\n");
    for (i = 0; i < threads; i++) {
        qthread_fork(barrier_thread, wait_on_me, rets + i);
    }
    iprintf("done forking the threads, entering the barrier\n");
    qtimer_start(t);
    qt_feb_barrier_enter(wait_on_me);
    qtimer_stop(t);
    iprintf("main thread exited barrier 1 in %f seconds\n", qtimer_secs(t));
    initme_idx = 0;

    for (i = 0; i < threads; i++) {
        qthread_fork(barrier_thread, wait_on_me, rets + i);
    }
    qtimer_start(t);
    qt_feb_barrier_enter(wait_on_me);
    qtimer_stop(t);
    iprintf("main thread exited barrier 2 in %f seconds\n", qtimer_secs(t));

    for (i = 0; i < threads; i++) {
        if (initme[i] != 2) {
            iprintf("initme[%i] = %i (should be 2)\n", (int)i,
                    (int)initme[i]);
        }
        assert(initme[i] == 2);
    }
    iprintf("Success!\n");

    /* this loop shouldn't be necessary... but seems to avoid crashes in rare
     * cases (in other words there must a race condition in qthread_finalize()
     * if there are outstanding threads out there) */
    for (i = 0; i < threads; i++) {
        qthread_readFF(NULL, rets + i);
    }
    return 0;
}

/* vim:set expandtab */
