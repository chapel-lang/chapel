#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <qthread/qthread.h>
#include "argparsing.h"

aligned_t threads_in = 0;
aligned_t awoke = 0;
int THREADS_ENQUEUED = 100;

qthread_queue_t the_queue;

static aligned_t tobequeued(void *arg)
{
    qthread_incr(&threads_in, 1);
    //iprintf("\tOne thread about to join the queue...\n");
    qthread_queue_join(the_queue);
    //iprintf("\tAwoke from the queue! %p\n", qthread_retloc());
    qthread_incr(&awoke, 1);

    return 1;
}

int main(int   argc,
         char *argv[])
{
    aligned_t return_value = 0;
    int status, ret;

    CHECK_VERBOSE(); // part of the testing harness; toggles iprintf() output
    NUMARG(THREADS_ENQUEUED, "THREADS_ENQUEUED");

    status = qthread_initialize();
    assert(status == QTHREAD_SUCCESS);

    iprintf("%i shepherds...\n", qthread_num_shepherds());
    iprintf("  %i threads total\n", qthread_num_workers());

    iprintf("Creating the queue...\n");
    the_queue = qthread_queue_create(QTHREAD_QUEUE_MULTI_JOIN_LENGTH, 0);
    assert(the_queue);

    iprintf("---------------------------------------------------------\n");
    iprintf("\tSINGLE THREAD TEST\n\n");

    iprintf("1/4 Spawning thread to be queued...\n");
    status = qthread_fork(tobequeued, NULL, &return_value);
    assert(status == QTHREAD_SUCCESS);

    iprintf("2/4 Waiting for thread to queue itself...\n");
    while(qthread_queue_length(the_queue) != 1) qthread_yield();
    assert(qthread_readstate(NODE_BUSYNESS) == 1);

    iprintf("3/4 Releasing the queue...\n");
    qthread_queue_release_all(the_queue);

    ret = qthread_readFF(NULL, &return_value);
    assert(ret == QTHREAD_SUCCESS);

    assert(threads_in == 1);
    assert(awoke == 1);
    assert(qthread_queue_length(the_queue) == 0);
    assert(qthread_readstate(NODE_BUSYNESS) == 1);
    iprintf("4/4 Test passed!\n");

    iprintf("---------------------------------------------------------\n");
    iprintf("\tMULTI THREAD TEST\n\n");

    threads_in = 0;
    awoke = 0;
    aligned_t *retvals = malloc(sizeof(aligned_t) * THREADS_ENQUEUED);
    iprintf("1/6 Spawning %u threads to be queued...\n", THREADS_ENQUEUED);
    for (int i=0; i<THREADS_ENQUEUED; i++) {
        status = qthread_fork(tobequeued, NULL, retvals + i);
        assert(status == QTHREAD_SUCCESS);
    }

    iprintf("2/6 Waiting for %u threads to queue themselves...\n", THREADS_ENQUEUED);
    while(qthread_queue_length(the_queue) != THREADS_ENQUEUED) qthread_yield();
    assert(threads_in == THREADS_ENQUEUED);
    assert(qthread_readstate(NODE_BUSYNESS) == 1);

    iprintf("3/6 Releasing a single thread...\n");
    qthread_queue_release_one(the_queue);

    iprintf("4/6 Waiting for that thread to exit\n");
    while (awoke == 0) qthread_yield();

    assert(qthread_queue_length(the_queue) == (THREADS_ENQUEUED - 1));
    assert(qthread_readstate(NODE_BUSYNESS) == 1);

    iprintf("5/6 Releasing the rest of the threads...\n");
    qthread_queue_release_all(the_queue);

    for (int i=0; i<THREADS_ENQUEUED; i++) {
        ret = qthread_readFF(NULL, retvals + i);
        assert(ret == QTHREAD_SUCCESS);
    }

    assert(qthread_queue_length(the_queue) == 0);
    assert(qthread_readstate(NODE_BUSYNESS) == 1);

    iprintf("6/6 Test passed!\n");
    free(retvals);
    return EXIT_SUCCESS;
}

/* vim:set expandtab */
