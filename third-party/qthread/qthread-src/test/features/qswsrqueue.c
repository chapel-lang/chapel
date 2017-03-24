#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <qthread/qthread.h>
#include <qthread/qswsrqueue.h>
#include <qthread/sinc.h>
#include "argparsing.h"

static size_t elementcount = 1000;
static size_t threadcount  = 128;

static aligned_t queuer(void *arg)
{
    qswsrqueue_t *q = (qswsrqueue_t *)arg;
    size_t        i;

    for (i = 0; i < elementcount; i++) {
        while (qswsrqueue_enqueue(q, (void *)(intptr_t)1) != QTHREAD_SUCCESS) {
            qthread_yield();
        }
    }
    return 0;
}

static aligned_t dequeuer(void *arg)
{
    qswsrqueue_t *q = (qswsrqueue_t *)arg;
    size_t        i;

    for (i = 0; i < elementcount; i++) {
        while (qswsrqueue_dequeue(q) == NULL) {
            qthread_yield();
        }
    }
    iprintf("dequeuer exiting\n");
    return 0;
}

int main(int   argc,
         char *argv[])
{
    qswsrqueue_t *q;
    size_t        i;

    assert(qthread_init(2) == 0);
    NUMARG(threadcount, "THREAD_COUNT");
    NUMARG(elementcount, "ELEMENT_COUNT");
    CHECK_VERBOSE();
    iprintf("%i threads\n", qthread_num_workers());

    if ((q = qswsrqueue_create(128)) == NULL) {
        fprintf(stderr, "qswsrqueue_create() failed!\n");
        exit(EXIT_FAILURE);
    }

    if (qswsrqueue_enqueue(q, (void *)(intptr_t)1) != 0) {
        fprintf(stderr, "qswsrqueue_enqueue() failed!\n");
        exit(EXIT_FAILURE);
    }

    if (qswsrqueue_dequeue(q) != (void *)(intptr_t)1) {
        fprintf(stderr, "qswsrqueue_dequeue() failed!\n");
        exit(EXIT_FAILURE);
    }

    for (i = 0; i < 100; i++) {
        if (qswsrqueue_enqueue(q, (void *)(intptr_t)(i + 1)) != 0) {
            fprintf(stderr, "qswsrqueue_enqueue(q,%i) failed!\n", (int)i);
            exit(EXIT_FAILURE);
        }
    }
    for (i = 0; i < 100; i++) {
        if (qswsrqueue_dequeue(q) != (void *)(intptr_t)(i + 1)) {
            fprintf(stderr, "qswsrqueue_dequeue() failed, didn't equal %i!\n",
                    (int)i);
            exit(EXIT_FAILURE);
        }
    }
    if (!qswsrqueue_empty(q)) {
        fprintf(stderr, "qswsrqueue not empty after ordering test!\n");
        exit(EXIT_FAILURE);
    }
    iprintf("ordering test succeeded\n");

    aligned_t ret;
    assert(qthread_fork(dequeuer, q, &ret) == QTHREAD_SUCCESS);
    iprintf("dequeuer forked\n");
    assert(qthread_fork(queuer, q, NULL) == QTHREAD_SUCCESS);
    iprintf("queuer forked\n");
    qthread_readFF(NULL, &ret);
    iprintf("dequeuer returned\n");

    if (!qswsrqueue_empty(q)) {
        fprintf(stderr, "qswsrqueue not empty after threaded test!\n");
        exit(EXIT_FAILURE);
    }
    iprintf("threaded test succeeded\n");

    if (qswsrqueue_destroy(q) != QTHREAD_SUCCESS) {
        fprintf(stderr, "qswsrqueue_destroy() failed!\n");
        exit(EXIT_FAILURE);
    }

    iprintf("success!\n");

    return 0;
}

/* vim:set expandtab */
