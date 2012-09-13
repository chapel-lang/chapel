#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <qthread/qthread.h>
#include <qthread/feb_barrier.h>
#include <qthread/qtimer.h>
#include "argparsing.h"

aligned_t         initme_idx = 0;
aligned_t        *initme     = NULL;
qt_feb_barrier_t *wait_on_me;

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
    size_t       threads = 1000, i;
    aligned_t   *rets;
    qtimer_t     t;
    unsigned int iter, iterations = 10;

    assert(qthread_initialize() == 0);
    t = qtimer_create();

    CHECK_VERBOSE();
    NUMARG(threads, "THREADS");
    NUMARG(iterations, "ITERATIONS");

    initme = (aligned_t *)calloc(threads, sizeof(aligned_t));
    assert(initme);

    rets = (aligned_t *)malloc(iterations * threads * sizeof(aligned_t));
    assert(rets);

    iprintf("creating the barrier for %zu threads\n", threads + 1);
    wait_on_me = qt_feb_barrier_create(threads + 1);    // all my spawnees plus me
    assert(wait_on_me);

    for (iter = 0; iter < iterations; iter++) {
        iprintf("%i: forking the threads\n", iter);
        for (i = 0; i < threads; i++) {
            qthread_fork(barrier_thread, wait_on_me, rets + (iter * threads) + i);
        }
        iprintf("%i: done forking the threads, entering the barrier\n", iter);
        qtimer_start(t);
        qt_feb_barrier_enter(wait_on_me);
        qtimer_stop(t);
        iprintf("%i: main thread exited barrier in %f seconds\n", iter, qtimer_secs(t));

        initme_idx = 0;

        for (i = 0; i < threads; i++) {
            if (initme[i] != iter + 1) {
                iprintf("initme[%i] = %i (should be %i)\n", (int)i,
                        (int)initme[i], iter + 1);
            }
            assert(initme[i] == iter + 1);
        }
    }

    iprintf("Destroying barrier...\n");
    qt_feb_barrier_destroy(wait_on_me);

    iprintf("Success!\n");

    /* this loop shouldn't be necessary... but seems to avoid crashes in rare
     * cases (in other words there must a race condition in qthread_finalize()
     * if there are outstanding threads out there) */
    for (i = 0; i < threads * 2; i++) {
        aligned_t tmp = 1;
        qthread_readFF(&tmp, rets + i);
        assert(tmp == 0);
    }
    return 0;
}

/* vim:set expandtab */
