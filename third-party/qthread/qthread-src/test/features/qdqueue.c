#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <qthread/qthread.h>
#include <qthread/qdqueue.h>
#include "argparsing.h"

static unsigned int ELEMENT_COUNT = 1000;
static unsigned int THREAD_COUNT = 128;

static aligned_t queuer(void *arg)
{
    qdqueue_t *q = (qdqueue_t *)arg;
    size_t i;

    for (i = 0; i < ELEMENT_COUNT; i++) {
        if (qdqueue_enqueue(q, (void *)((intptr_t)qthread_id() + 1)) !=
            QTHREAD_SUCCESS) {
            fprintf(stderr, "qdqueue_enqueue(q, %p) failed!\n",
                    (void *)((intptr_t)qthread_id() + 1));
            exit(-2);
        }
    }
    return 0;
}

static aligned_t dequeuer(void *arg)
{
    qdqueue_t *q = (qdqueue_t *)arg;
    size_t i;

    for (i = 0; i < ELEMENT_COUNT; i++) {
        while (qdqueue_dequeue(q) == NULL) {
            qthread_yield();
        }
    }
    return 0;
}

static aligned_t spawn_dequeuers(void *arg)
{
    for (size_t i = 0; i < THREAD_COUNT; i++) {
        assert(qthread_fork(dequeuer, arg, NULL) == QTHREAD_SUCCESS);
    }
    return 0;
}

int main(int argc,
         char *argv[])
{
    qdqueue_t *q;
    size_t i;

    assert(qthread_initialize() == QTHREAD_SUCCESS);
    CHECK_VERBOSE();
    NUMARG(ELEMENT_COUNT, "ELEMENT_COUNT");
    NUMARG(THREAD_COUNT, "THREAD_COUNT");

    if ((q = qdqueue_create()) == NULL) {
        fprintf(stderr, "qdqueue_create() failed!\n");
        exit(-1);
    }

    if (qdqueue_enqueue(q, (void *)((intptr_t)qthread_id() + 1)) != 0) {
        fprintf(stderr, "qdqueue_enqueue() failed!\n");
        exit(-1);
    }

    {
        void *ret;

        if ((ret = qdqueue_dequeue(q)) != (void *)((intptr_t)qthread_id() +
            1)) {
            fprintf(stderr, "qdqueue_dequeue() failed! %p\n", ret);
            exit(-1);
        }
    }

    for (i = 0; i < ELEMENT_COUNT; i++) {
        if (qdqueue_enqueue(q, (void *)(intptr_t)(i + 1)) != 0) {
            fprintf(stderr, "qdqueue_enqueue(q,%i) failed!\n", (int)i);
            exit(-1);
        }
    }
    for (i = 0; i < ELEMENT_COUNT; i++) {
        if (qdqueue_dequeue(q) != (void *)(intptr_t)(i + 1)) {
            fprintf(stderr, "qdqueue_dequeue() failed, didn't equal %i!\n",
                    (int)i);
            exit(-1);
        }
    }
    if (!qdqueue_empty(q)) {
        fprintf(stderr, "qdqueue not empty after ordering test!\n");
        exit(-1);
    }
    iprintf("ordering test succeeded\n");

    aligned_t ret;
    assert(qthread_fork_new_team(spawn_dequeuers, q, &ret) == QTHREAD_SUCCESS);
    iprintf("dequeuers forked\n");
    for (i = 0; i < THREAD_COUNT; i++) {
        assert(qthread_fork_to(queuer, q, NULL, i%qthread_num_shepherds()) == QTHREAD_SUCCESS);
    }
    assert(qthread_readFF(NULL, &ret) == QTHREAD_SUCCESS);
    if (!qdqueue_empty(q)) {
        fprintf(stderr, "qdqueue not empty after threaded test!\n");
        exit(-2);
    }
    iprintf("threaded test succeeded\n");

    if (qdqueue_destroy(q) != QTHREAD_SUCCESS) {
        fprintf(stderr, "qdqueue_destroy() failed!\n");
        exit(-2);
    }

    iprintf("success!\n");

    return 0;
}

/* vim:set expandtab */
