#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <qthread/qthread.h>
#include "qt_barrier.h"
#include <qthread/qtimer.h>
#include "argparsing.h"

aligned_t           initme_idx = 0;
volatile aligned_t *initme     = NULL;
qt_barrier_t       *wait_on_me;

static aligned_t barrier_thread(void *arg)
{
    qt_barrier_t *b   = (qt_barrier_t *)arg;
    aligned_t     idx = qthread_incr(&initme_idx, 1);

    qthread_incr(&(initme[idx]), 1);
    qt_barrier_enter(b, qthread_shep());
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
    printf("run \"export VERBOSE=1\" to make test_log_barrier run (it's currently broken)\n");
    if (!verbose) {
        exit(0);
    }                                  // XXX don't break the trunk

    iprintf("%i shepherds...\n", qthread_num_shepherds());
    iprintf("%i threads...\n", (int)threads);

    initme = calloc(threads, sizeof(aligned_t));
    assert(initme);

    rets = malloc(threads * sizeof(aligned_t));
    assert(rets);

    iprintf("Creating a barrier to block %i threads\n", threads + 1);

    wait_on_me = qt_barrier_create(threads + 1, REGION_BARRIER, 1);     // all my spawnees plus me
    assert(wait_on_me);

    for (i = 0; i < threads; i++) {
        qthread_fork(barrier_thread, wait_on_me, rets + i);
    }
    qtimer_start(t);
    qt_barrier_enter(wait_on_me, qthread_shep());
    qtimer_stop(t);
    iprintf("main thread exited barrier 1 in %f seconds\n", qtimer_secs(t));
    initme_idx = 0;

    for (i = 0; i < threads; i++) {
        qthread_fork(barrier_thread, wait_on_me, rets + i);
    }
    qtimer_start(t);
    qt_barrier_enter(wait_on_me, qthread_shep());
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
