#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <qthread/qthread.h>
#include <qthread/qloop.h>
#include <qthread/qdqueue.h>
#include <qthread/qpool.h>
#include <qthread/qtimer.h>
#include "argparsing.h"

#define ELEMENT_COUNT   10000
#define THREAD_COUNT    128

qpool *memory = NULL;
size_t objsize = 8;

static aligned_t queuer(void *arg)
{
    qdqueue_t *q = (qdqueue_t *)arg;
    size_t i;

    for (i = 0; i < ELEMENT_COUNT; i++) {
        void *tmp = qpool_alloc(memory);
        memset(tmp, 1, objsize);
        if (qdqueue_enqueue(q, tmp) != QTHREAD_SUCCESS) {
            fprintf(stderr, "qdqueue_enqueue(q, %p) failed!\n",
                    (void *)(intptr_t)qthread_id());
            exit(-2);
        }
    }
    return 0;
}

static aligned_t dequeuer(void *arg)
{
    qdqueue_t *q = (qdqueue_t *)arg;
    size_t i;
    void *ref = qpool_alloc(memory);

    memset(ref, 1, objsize);
    for (i = 0; i < ELEMENT_COUNT; i++) {
        void *tmp;
        while ((tmp = qdqueue_dequeue(q)) == NULL) {
            qthread_yield();
        }
        if (memcmp(ref, tmp, objsize)) {
            fprintf(stderr, "memory was corrupted!\n");
            exit(-3);
        }
        qpool_free(memory, tmp);
    }
    qpool_free(memory, ref);
    return 0;
}

static void loop_queuer(const size_t startat, const size_t stopat, void *arg)
{                                      /*{{{ */
    size_t i;
    qdqueue_t *q = (qdqueue_t *)arg;

    for (i = startat; i < stopat; i++) {
        void *tmp = qpool_alloc(memory);
        memset(tmp, 1, objsize);
        if (qdqueue_enqueue(q, tmp) != QTHREAD_SUCCESS) {
            fprintf(stderr, "qdqueue_enqueue(q, %p) failed!\n", (void *)tmp);
            exit(-2);
        }
    }
}                                      /*}}} */

static void loop_dequeuer(const size_t startat, const size_t stopat,
                          void *arg)
{                                      /*{{{ */
    size_t i;
    qdqueue_t *q = (qdqueue_t *)arg;
    void *ref = qpool_alloc(memory);

    memset(ref, 1, objsize);
    for (i = startat; i < stopat; i++) {
        void *tmp;
        if ((tmp = qdqueue_dequeue(q)) == NULL) {
            fprintf(stderr, "qdqueue_dequeue(%p) failed!\n", (void *)q);
            exit(-2);
        }
        if (memcmp(ref, tmp, objsize)) {
            fprintf(stderr, "memory was corrupted!\n");
            exit(-3);
        }
        qpool_free(memory, tmp);
    }
    qpool_free(memory, ref);
}                                      /*}}} */

int main(int argc, char *argv[])
{
    qdqueue_t *q;
    size_t i;
    aligned_t *rets;
    qtimer_t timer = qtimer_create();

    assert(qthread_initialize() == QTHREAD_SUCCESS);

    CHECK_VERBOSE();
    NUMARG(objsize, "TEST_OBJSIZE");

    memory = qpool_create(objsize);

    if ((q = qdqueue_create()) == NULL) {
        fprintf(stderr, "qdqueue_create() failed!\n");
        exit(-1);
    }

    /* prime the pump */
    /*qt_loop_balance(0, THREAD_COUNT * ELEMENT_COUNT, loop_queuer, q);
     * qt_loop_balance(0, THREAD_COUNT * ELEMENT_COUNT, loop_dequeuer, q);
     * if (!qdqueue_empty(q)) {
     * fprintf(stderr, "qdqueue not empty after priming!\n");
     * exit(-2);
     * } */

    qtimer_start(timer);
    qt_loop_balance(0, THREAD_COUNT * ELEMENT_COUNT, loop_queuer, q);
    qtimer_stop(timer);
    printf("loop balance enqueue: %f secs\n", qtimer_secs(timer));
    qtimer_start(timer);
    qt_loop_balance(0, THREAD_COUNT * ELEMENT_COUNT, loop_dequeuer, q);
    qtimer_stop(timer);
    printf("loop balance dequeue: %f secs\n", qtimer_secs(timer));
    if (!qdqueue_empty(q)) {
        fprintf(stderr, "qdqueue not empty after loop balance test!\n");
        exit(-2);
    }

    rets = calloc(THREAD_COUNT, sizeof(aligned_t));
    assert(rets != NULL);
    qtimer_start(timer);
    for (i = 0; i < THREAD_COUNT; i++) {
        assert(qthread_fork(dequeuer, q, &(rets[i])) == QTHREAD_SUCCESS);
    }
    for (i = 0; i < THREAD_COUNT; i++) {
        assert(qthread_fork(queuer, q, NULL) == QTHREAD_SUCCESS);
    }
    for (i = 0; i < THREAD_COUNT; i++) {
        assert(qthread_readFF(NULL, &(rets[i])) == QTHREAD_SUCCESS);
    }
    qtimer_stop(timer);
    if (!qdqueue_empty(q)) {
        fprintf(stderr, "qdqueue not empty after threaded test!\n");
        exit(-2);
    }
    printf("threaded dq test: %f secs\n", qtimer_secs(timer));
    free(rets);

    if (qdqueue_destroy(q) != QTHREAD_SUCCESS) {
        fprintf(stderr, "qdqueue_destroy() failed!\n");
        exit(-2);
    }

    iprintf("success!\n");

    return 0;
}

/* vim:set expandtab */