#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <qthread/qthread.h>
#include "qt_barrier.h"
#include <qthread/qtimer.h>
#include "argparsing.h"

aligned_t           initme_idx = 1;
volatile aligned_t *initme     = NULL;
qt_barrier_t       *wait_on_me;

static aligned_t barrier_thread(void *arg)
{
    void **args = (void**)arg;
    qt_barrier_t *b   = (qt_barrier_t *)args[0];
    uintptr_t     id = (uintptr_t)args[1];
    aligned_t     idx = qthread_incr(&initme_idx, 1);
    qthread_shepherd_id_t shep;
    qthread_worker_id_t wkr;

    wkr = qthread_worker(&shep);
    //iprintf("I woke up on shep %u worker %u, barrier %p, using ID number %u, idx %u\n", shep, wkr, b, id, idx);
    qthread_incr(&(initme[idx]), 1);
    qt_barrier_enter(b, id);
    return 0;
}

int main(int   argc,
         char *argv[])
{
    size_t     threads, i;
    aligned_t *rets;
    qtimer_t   t;
    unsigned int iter, iterations = 10;
    double tot = 0.0;

    assert(qthread_initialize() == 0);
    t = qtimer_create();

    CHECK_VERBOSE();
    NUMARG(iterations, "ITERATIONS");

    threads = qthread_num_workers();
    iprintf("%i shepherds...\n", qthread_num_shepherds());
    iprintf("%i threads...\n", (int)threads);

    initme = calloc(threads, sizeof(aligned_t));
    assert(initme);

    rets = malloc(threads * sizeof(aligned_t));
    assert(rets);

    iprintf("Creating a barrier to block %i threads\n", threads);
    wait_on_me = qt_barrier_create(threads, REGION_BARRIER, 0);     // all my spawnees plus me
    assert(wait_on_me);

    for (iter = 0; iter < iterations; iter++) {
	iprintf("%i: forking the threads\n", iter);
	for (i = 1; i < threads; i++) {
	    void *arg[2] = {wait_on_me, (void*)(intptr_t)i};
	    qthread_spawn(barrier_thread, arg, sizeof(void*)*2, rets + i, 0, NULL, i, 0);
	}
	iprintf("%i: done forking the threads, entering the barrier\n", iter);
	qtimer_start(t);
	qt_barrier_enter(wait_on_me, 0);
	qtimer_stop(t);
	iprintf("%i: main thread exited barrier in %f seconds\n", iter, qtimer_secs(t));
	tot += qtimer_secs(t);

	// reset
	initme_idx = 1;

	// check retvals
	for (i = 1; i < threads; i++) {
	    qthread_readFF(NULL, rets + i);
	    if (initme[i] != iter + 1) {
                iprintf("initme[%i] = %i (should be %i)\n", (int)i,
                        (int)initme[i], iter + 1);
            }
            assert(initme[i] == iter + 1);
	}
    }

    iprintf("Average barrier time = %f\n", tot / iterations);

    iprintf("Destroying the barrier...\n");
    qt_barrier_destroy(wait_on_me);

    iprintf("Success!\n");

    return 0;
}

/* vim:set expandtab */
