#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

/* System Headers */
#include <pthread.h>

#include <stdio.h>
#include <stdlib.h>

/* Internal Headers */
#include "qthread/qthread.h"
#include "qt_macros.h"
#include "qt_visibility.h"
#include "qthread_asserts.h"
#include "qthread_prefetch.h"
#include "qt_threadqueues.h"
#include "qt_qthread_struct.h"
#include "qt_atomics.h"
#include "qthread_innards.h" /* for qthread_internal_cleanup_early() */

/* Note: this queue is SAFE to use with multiple de-queuers, with the caveat
 * that if you have multiple dequeuer's, you'll need to solve the ABA problem.
 * (single dequeuer == no ABA problem) Also, yielding changes behavior a bit in
 * the multiple-dequeuer case.
 */

/* Data Structures */
struct _qt_threadqueue {
    qthread_t *stack;
    /* the following is for estimating a queue's "busy" level, and is not
     * guaranteed accurate (that would be a race condition) */
    saligned_t      advisory_queuelen;
#ifdef QTHREAD_CONDWAIT_BLOCKING_QUEUE
    uint32_t        frustration;
    pthread_cond_t  trigger;
    pthread_mutex_t trigger_lock;
#endif
} /* qt_threadqueue_t */;

struct _qt_threadqueue_private {} /* qt_threadqueue_private_t */;

/* Memory Management */
#if defined(UNPOOLED_QUEUES) || defined(UNPOOLED)
# define ALLOC_THREADQUEUE() (qt_threadqueue_t *)calloc(1, sizeof(qt_threadqueue_t))
# define FREE_THREADQUEUE(t) free(t)
void INTERNAL qt_threadqueue_subsystem_init(void) {}
#else /* if defined(UNPOOLED_QUEUES) || defined(UNPOOLED) */
qt_threadqueue_pools_t generic_threadqueue_pools = { NULL, NULL };
# define ALLOC_THREADQUEUE() (qt_threadqueue_t *)qt_mpool_cached_alloc(generic_threadqueue_pools.queues)
# define FREE_THREADQUEUE(t) qt_mpool_cached_alloc(generic_threadqueue_pools.queues)

static void qt_threadqueue_subsystem_shutdown(void)
{
    qt_mpool_destroy(generic_threadqueue_pools.queues);
}

void INTERNAL qt_threadqueue_subsystem_init(void)
{
    generic_threadqueue_pools.queues = qt_mpool_create(sizeof(qt_threadqueue_t));
    qthread_internal_cleanup(qt_threadqueue_subsystem_shutdown);
}

#endif /* if defined(UNPOOLED_QUEUES) || defined(UNPOOLED) */

/* Thankfully, a basic LIFO stack does not suffer from the ABA problem. */

qt_threadqueue_t INTERNAL *qt_threadqueue_new(void)
{   /*{{{*/
    qt_threadqueue_t *q = ALLOC_THREADQUEUE();

    qassert_ret(q != NULL, NULL);

    q->stack             = NULL;
    q->advisory_queuelen = 0;
#ifdef QTHREAD_CONDWAIT_BLOCKING_QUEUE
    q->frustration = 0;
    {
        pthread_mutexattr_t ma;
        qassert(pthread_mutexattr_init(&ma), 0);
        qassert(pthread_mutexattr_setpshared(&ma, PTHREAD_PROCESS_PRIVATE),
                   0);
        qassert(pthread_mutex_init(&q->trigger_lock, &ma), 0);
        qassert(pthread_mutexattr_destroy(&ma), 0);
    }
    {
        pthread_condattr_t ca;
        qassert(pthread_condattr_init(&ca), 0);
        qassert(pthread_condattr_setpshared(&ca, PTHREAD_PROCESS_PRIVATE),
                   0);
        qassert(pthread_cond_init(&q->trigger, &ca), 0);
        qassert(pthread_condattr_destroy(&ca), 0);
    }
#endif /* ifdef QTHREAD_CONDWAIT_BLOCKING_QUEUE */

    return q;
} /*}}}*/

void INTERNAL qt_threadqueue_free(qt_threadqueue_t *q)
{   /*{{{*/
    assert(q);
    while (qt_threadqueue_dequeue(q)) ;
#ifdef QTHREAD_CONDWAIT_BLOCKING_QUEUE
    QTHREAD_DESTROYLOCK(&q->lock);
    QTHREAD_DESTROYCOND(&q->notempty);
#endif
    FREE_THREADQUEUE(q);
} /*}}}*/

qt_threadqueue_private_t INTERNAL *qt_threadqueue_private_create(void)
{   /*{{{*/
    return NULL;
} /*}}}*/

void INTERNAL qt_threadqueue_private_enqueue(qt_threadqueue_private_t *restrict q,
                                             qthread_t *restrict                t)
{}

void INTERNAL qt_threadqueue_private_destroy(void *q)
{   /*{{{*/
    assert(q == NULL);
} /*}}}*/

void INTERNAL qt_threadqueue_enqueue(qt_threadqueue_t *restrict q,
                                     qthread_t *restrict        t)
{   /*{{{*/
    assert(q);
    assert(t);
    assert(t->next == NULL);
    qthread_t *old, *new;

    do {
        old     = q->stack;                /* should be an atomic read */
        t->next = old;
        new     = qthread_cas_ptr(&(q->stack), old, t);
    } while (new != old);
    (void)qthread_incr(&(q->advisory_queuelen), 1);

    /* awake waiter */
#ifdef QTHREAD_CONDWAIT_BLOCKING_QUEUE
    if (q->frustration) {
        qassert(pthread_mutex_lock(&q->trigger_lock), 0);
        if (q->frustration) {
            q->frustration = 0;
            qassert(pthread_cond_signal(&q->trigger), 0);
        }
        qassert(pthread_mutex_unlock(&q->trigger_lock), 0);
    }
#endif
} /*}}}*/

void INTERNAL qt_threadqueue_enqueue_yielded(qt_threadqueue_t *restrict q,
                                             qthread_t *restrict        t)
{   /*{{{*/
    assert(q);
    assert(t);
    assert(t->next == NULL);
#ifdef QTHREAD_MULTITHREADED_SHEPHERDS
    qthread_t *top = qt_threadqueue_dequeue(q);
    qt_threadqueue_enqueue(q, t);
    if (top) {
        qt_threadqueue_enqueue(q, top);
    }
#else
    /* THIS is not safe for multiple dequeuers */
    qthread_t *cursor = q->stack;
    while (cursor && cursor->next) {
        cursor = cursor->next;
    }
    if (cursor) {
        assert(cursor->next == NULL);
        cursor->next = t;
    } else {
        qt_threadqueue_enqueue(q, t);
    }
#endif /* ifdef QTHREAD_MULTITHREADED_SHEPHERDS */
} /*}}}*/

ssize_t INTERNAL qt_threadqueue_advisory_queuelen(qt_threadqueue_t *q)
{   /*{{{*/
    assert(q);
    return q->advisory_queuelen;
} /*}}}*/

qthread_t INTERNAL *qt_threadqueue_dequeue(qt_threadqueue_t *q)
{   /*{{{*/
    qthread_t *retval = q->stack;

    if (retval != NULL) {
        qthread_t *old, *new;

        do {
            old    = retval;
            new    = retval->next;
            retval = qthread_cas_ptr(&q->stack, old, new);
        } while (retval != old && retval != NULL);
    }
    if (retval != NULL) {
        retval->next = NULL;
        (void)qthread_incr(&(q->advisory_queuelen), -1);
    }
    return retval;
} /*}}}*/

qthread_t INTERNAL *qt_threadqueue_dequeue_blocking(qt_threadqueue_t         *q,
                                                    qt_threadqueue_private_t *QUNUSED(qc),
                                                    uint_fast8_t              QUNUSED(active))
{   /*{{{*/
    qthread_t *retval = qt_threadqueue_dequeue(q);

    if (retval == NULL) {
        while (q->stack == NULL) {
#ifndef QTHREAD_CONDWAIT_BLOCKING_QUEUE
            SPINLOCK_BODY();
#else
            COMPILER_FENCE;
            if (qthread_incr(&q->frustration, 1) > 1000) {
                qassert(pthread_mutex_lock(&q->trigger_lock), 0);
                if (q->frustration > 1000) {
                    qassert(pthread_cond_wait
                                   (&q->trigger, &q->trigger_lock), 0);
                }
                qassert(pthread_mutex_unlock(&q->trigger_lock), 0);
            }
#endif      /* ifdef USE_HARD_POLLING */
        }
        retval = qt_threadqueue_dequeue(q);
    }
    assert(retval);
    assert(retval->next == NULL);
    return retval;
} /*}}}*/

/* vim:set expandtab: */
