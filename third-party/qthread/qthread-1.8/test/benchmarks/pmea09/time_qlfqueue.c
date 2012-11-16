#ifdef HAVE_CONFIG_H
# include "config.h"
#endif
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#ifdef HAVE_CPROPS
# include <cprops/linked_list.h>
#endif
#include <qthread/qthread.h>
#include <qthread/qloop.h>
#include <qthread/qlfqueue.h>
#include <qthread/qtimer.h>
#include "argparsing.h"

#define ELEMENT_COUNT   10000
#define THREAD_COUNT    128

#ifdef HAVE_CPROPS
static aligned_t cpqueuer(void *arg)
{
    cp_list *q = (cp_list *)arg;
    size_t i;

    for (i = 0; i < ELEMENT_COUNT; i++) {
        if (cp_list_append(q, (void *)(intptr_t)qthread_id()) == NULL) {
            fprintf(stderr, "%i'th cp_list_append(q, %p) failed!\n", (int)i,
                    (void *)(intptr_t)qthread_id());
            perror("cp_list_append");
            exit(-2);
        }
    }
    return 0;
}

static aligned_t cpdequeuer(void *arg)
{
    cp_list *q = (cp_list *)arg;
    size_t i;

    for (i = 0; i < ELEMENT_COUNT; i++) {
        while (cp_list_remove_head(q) == NULL) {
            qthread_yield();
        }
    }
    return 0;
}

#endif /* ifdef HAVE_CPROPS */

static aligned_t queuer(void *arg)
{
    qlfqueue_t *q = (qlfqueue_t *)arg;
    size_t i;

    for (i = 0; i < ELEMENT_COUNT; i++) {
        if (qlfqueue_enqueue(q, (void *)(intptr_t)qthread_id()) !=
            QTHREAD_SUCCESS) {
            fprintf(stderr, "qlfqueue_enqueue(q, %p) failed!\n",
                    (void *)(intptr_t)qthread_id());
            exit(-2);
        }
    }
    return 0;
}

static aligned_t dequeuer(void *arg)
{
    qlfqueue_t *q = (qlfqueue_t *)arg;
    size_t i;

    for (i = 0; i < ELEMENT_COUNT; i++) {
        while (qlfqueue_dequeue(q) == NULL) {
            qthread_yield();
        }
    }
    return 0;
}

#ifdef HAVE_CPROPS
static void loop_cpqueuer(const size_t startat, const size_t stopat,
                          void *arg)
{                                      /*{{{ */
    size_t i;
    cp_list *q = (cp_list *)arg;

    for (i = startat; i < stopat; i++) {
        if (cp_list_append(q, (void *)(intptr_t)qthread_id()) == NULL) {
            fprintf(stderr, "cp_list_append(q, %p) failed!\n",
                    (void *)(intptr_t)qthread_id());
            exit(-2);
        }
    }
}                                      /*}}} */

static void loop_cpdequeuer(const size_t startat, const size_t stopat,
                            void *arg)
{                                      /*{{{ */
    size_t i;
    cp_list *q = (cp_list *)arg;

    for (i = startat; i < stopat; i++) {
        if (cp_list_remove_head(q) == NULL) {
            fprintf(stderr, "cp_list_remove_head(q, %p) failed!\n",
                    (void *)(intptr_t)qthread_id());
            exit(-2);
        }
    }
}                                      /*}}} */

#endif /* ifdef HAVE_CPROPS */

static void loop_queuer(const size_t startat, const size_t stopat, void *arg)
{                                      /*{{{ */
    size_t i;
    qlfqueue_t *q = (qlfqueue_t *)arg;
    void *me = (void *)(uintptr_t)qthread_id();

    for (i = startat; i < stopat; i++) {
        if (qlfqueue_enqueue(q, me) != QTHREAD_SUCCESS) {
            fprintf(stderr, "qlfqueue_enqueue(q, %p) failed!\n", me);
            exit(-2);
        }
    }
}                                      /*}}} */

static void loop_dequeuer(const size_t startat, const size_t stopat,
                          void *arg)
{                                      /*{{{ */
    size_t i;
    qlfqueue_t *q = (qlfqueue_t *)arg;

    for (i = startat; i < stopat; i++) {
        if (qlfqueue_dequeue(q) == NULL) {
            fprintf(stderr, "qlfqueue_dequeue(%p) failed!\n", (void *)q);
            exit(-2);
        }
    }
}                                      /*}}} */

int main(int argc, char *argv[])
{
    qlfqueue_t *q;
    size_t i;
    aligned_t *rets;
    qtimer_t timer = qtimer_create();

#ifdef HAVE_CPROPS
    cp_list *cpq;
#endif

    assert(qthread_initialize() == QTHREAD_SUCCESS);

    CHECK_VERBOSE();

    if ((q = qlfqueue_create()) == NULL) {
        fprintf(stderr, "qlfqueue_create() failed!\n");
        exit(-1);
    }

    /* prime the pump */
    qt_loop_balance(0, THREAD_COUNT * ELEMENT_COUNT, loop_queuer, q);
    qt_loop_balance(0, THREAD_COUNT * ELEMENT_COUNT, loop_dequeuer, q);
    if (!qlfqueue_empty(q)) {
        fprintf(stderr, "qlfqueue not empty after priming!\n");
        exit(-2);
    }

    qtimer_start(timer);
    qt_loop_balance(0, THREAD_COUNT * ELEMENT_COUNT, loop_queuer, q);
    qtimer_stop(timer);
    printf("loop balance enqueue: %g secs (%g nsecs/enqueue)\n", qtimer_secs(timer), 1e9*qtimer_secs(timer)/(THREAD_COUNT*ELEMENT_COUNT));
    qtimer_start(timer);
    qt_loop_balance(0, THREAD_COUNT * ELEMENT_COUNT, loop_dequeuer, q);
    qtimer_stop(timer);
    printf("loop balance dequeue: %g secs (%g nsecs/dequeue)\n", qtimer_secs(timer), 1e9*qtimer_secs(timer)/(THREAD_COUNT*ELEMENT_COUNT));
    if (!qlfqueue_empty(q)) {
        fprintf(stderr, "qlfqueue not empty after loop balance test!\n");
        exit(-2);
    }
#ifdef HAVE_CPROPS
    cpq = cp_list_create();
    qtimer_start(timer);
    qt_loop_balance(0, THREAD_COUNT * ELEMENT_COUNT, loop_cpqueuer, cpq);
    qtimer_stop(timer);
    printf("loop balance cp enqueue: %f secs\n", qtimer_secs(timer));
    qtimer_start(timer);
    qt_loop_balance(0, THREAD_COUNT * ELEMENT_COUNT, loop_cpdequeuer, cpq);
    qtimer_stop(timer);
    printf("loop balance cp dequeue: %f secs\n", qtimer_secs(timer));
#endif

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
    if (!qlfqueue_empty(q)) {
        fprintf(stderr, "qlfqueue not empty after threaded test!\n");
        exit(-2);
    }
    printf("threaded lf test: %f secs\n", qtimer_secs(timer));

#ifdef HAVE_CPROPS
    qtimer_start(timer);
    for (i = 0; i < THREAD_COUNT; i++) {
        assert(qthread_fork(cpdequeuer, cpq, &(rets[i])) == QTHREAD_SUCCESS);
    }
    for (i = 0; i < THREAD_COUNT; i++) {
        assert(qthread_fork(cpqueuer, cpq, NULL) == QTHREAD_SUCCESS);
    }
    for (i = 0; i < THREAD_COUNT; i++) {
        assert(qthread_readFF(NULL, &(rets[i])) == QTHREAD_SUCCESS);
    }
    qtimer_stop(timer);
    free(rets);
    printf("threaded cp test: %f secs\n", qtimer_secs(timer));

    cp_list_destroy(cpq);
#endif /* ifdef HAVE_CPROPS */

    if (qlfqueue_destroy(q) != QTHREAD_SUCCESS) {
        fprintf(stderr, "qlfqueue_destroy() failed!\n");
        exit(-2);
    }

    iprintf("success!\n");

    return 0;
}

/* vim:set expandtab */