#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

/* System Headers */
#include <pthread.h>

#include <stdio.h>
#include <stdlib.h>

/* Internal Headers */
#include "qthread/qthread.h"
#include "qt_visibility.h"
#include "qthread_innards.h"
#include "qthread_asserts.h"
#include "qthread_prefetch.h"
#include "qt_threadqueues.h"
#include "qt_qthread_struct.h"

/* This thread queueing uses the NEMESIS lock-free queue protocol from
 * http://www.mcs.anl.gov/~buntinas/papers/ccgrid06-nemesis.pdf
 * Note: it is NOT SAFE to use with multiple de-queuers, it is ONLY safe to use
 * with multiple enqueuers and a single de-queuer. */

/* Data Structures */
typedef struct {
    void *volatile next;
    char           data[];
} NEMESIS_entry;

typedef struct {
    /* The First Cacheline */
    void *volatile head;
    void *volatile tail;
    uint8_t        pad1[CACHELINE_WIDTH - (2 * sizeof(void *))];
    /* The Second Cacheline */
    void *volatile shadow_head;
    /* the following is for estimating a queue's "busy" level, and is not
     * guaranteed accurate (that would be a race condition) */
    volatile saligned_t advisory_queuelen;
    qthread_shepherd_t *creator_ptr;
    uint8_t             pad2[CACHELINE_WIDTH - sizeof(void *) - sizeof(saligned_t) -
                             sizeof(qthread_shepherd_t *)];
}
NEMESIS_queue Q_ALIGNED (
                         CACHELINE_WIDTH);

struct _qt_threadqueue {
    NEMESIS_queue q;
    /* the following is for estimating a queue's "busy" level, and is not
     * guaranteed accurate (that would be a race condition) */
    volatile saligned_t advisory_queuelen;
    qthread_shepherd_t *creator_ptr;
#ifdef QTHREAD_CONDWAIT_BLOCKING_QUEUE
    volatile uint32_t   frustration;
    pthread_cond_t      trigger;
    pthread_mutex_t     trigger_lock;
#endif
} /* qt_threadqueue_t */;

/* Memory Management */
#if defined(UNPOOLED_QUEUES) || defined(UNPOOLED)
# define ALLOC_THREADQUEUE(shep) (qt_threadqueue_t *)calloc(1, sizeof(qt_threadqueue_t))
# define FREE_THREADQUEUE(t)     free(t)
void INTERNAL qt_threadqueue_subsystem_init(void) {}
#else /* if defined(UNPOOLED_QUEUES) || defined(UNPOOLED) */
qt_threadqueue_pools_t generic_threadqueue_pools = { NULL, NULL };

static QINLINE qt_threadqueue_t *ALLOC_THREADQUEUE(qthread_shepherd_t *shep)
{                                      /*{{{ */
    qt_threadqueue_t *tmp =
        (qt_threadqueue_t *)qt_mpool_alloc(shep
                                           ? (shep->threadqueue_pools.queues)
                                           : generic_threadqueue_pools.queues);

    if (tmp != NULL) {
        tmp->creator_ptr = shep;
    }
    return tmp;
}                                      /*}}} */

static QINLINE void FREE_THREADQUEUE(qt_threadqueue_t *t)
{                                      /*{{{ */
    qt_mpool_free(t->
                  creator_ptr ? (t->creator_ptr->threadqueue_pools.
                                 queues) : generic_threadqueue_pools.queues,
                  t);
}                                      /*}}} */

static void qt_threadqueue_subsystem_shutdown(void)
{
    qt_mpool_destroy(generic_threadqueue_pools.queues);
}

void INTERNAL qt_threadqueue_subsystem_init(void)
{
    generic_threadqueue_pools.queues = qt_mpool_create(sizeof(qt_threadqueue_t));
    qthread_internal_cleanup_early(qt_threadqueue_subsystem_shutdown);
}

void INTERNAL qt_threadqueue_init_pools(qt_threadqueue_pools_t *p)
{                                      /*{{{ */
    assert(p);
    p->nodes  = NULL;
    p->queues = qt_mpool_create(sizeof(qt_threadqueue_t));
}                                      /*}}} */

void INTERNAL qt_threadqueue_destroy_pools(qt_threadqueue_pools_t *p)
{                                      /*{{{ */
    assert(p);
    assert(p->queues);
    qt_mpool_destroy(p->queues);
}                                      /*}}} */

#endif /* if defined(UNPOOLED_QUEUES) || defined(UNPOOLED) */

/* Thankfully, NEMESIS does not suffer from the ABA problem. */

qt_threadqueue_t INTERNAL *qt_threadqueue_new(qthread_shepherd_t *shepherd)
{                                      /*{{{ */
    qt_threadqueue_t *q = ALLOC_THREADQUEUE(shepherd);

    qassert_ret(q != NULL, NULL);

    q->q.shadow_head     = q->q.head = q->q.tail = NULL;
    q->advisory_queuelen = 0;
#ifdef QTHREAD_CONDWAIT_BLOCKING_QUEUE
    q->frustration = 0;
    {
        pthread_mutexattr_t ma;
        qassert(pthread_mutexattr_init(&ma), 0);
        qassert(pthread_mutexattr_setpshared(&ma, PTHREAD_PROCESS_SHARED), 0);
        qassert(pthread_mutex_init(&q->trigger_lock, &ma), 0);
        qassert(pthread_mutexattr_destroy(&ma), 0);
    }
    {
        pthread_condattr_t ca;
        qassert(pthread_condattr_init(&ca), 0);
        qassert(pthread_condattr_setpshared(&ca, PTHREAD_PROCESS_SHARED), 0);
        qassert(pthread_cond_init(&q->trigger, &ca), 0);
        qassert(pthread_condattr_destroy(&ca), 0);
    }
#endif /* ifdef QTHREAD_CONDWAIT_BLOCKING_QUEUE */

    return q;
}                                      /*}}} */

void INTERNAL qt_threadqueue_free(qt_threadqueue_t *q)
{                                      /*{{{ */
    assert(q);
    while (qt_threadqueue_dequeue(q)) ;
#ifdef QTHREAD_CONDWAIT_BLOCKING_QUEUE
    QTHREAD_DESTROYLOCK(&q->trigger_lock);
    QTHREAD_DESTROYCOND(&q->trigger);
#endif
    FREE_THREADQUEUE(q);
}                                      /*}}} */

void INTERNAL qt_threadqueue_enqueue(qt_threadqueue_t *restrict   q,
                                     qthread_t *restrict          t,
                                     qthread_shepherd_t *restrict shep)
{                                      /*{{{ */
    assert(t->next == NULL);
    NEMESIS_entry *prev =
        qt_internal_atomic_swap_ptr((void *volatile *)&(q->q.tail), t);

    if (prev == NULL) {
        q->q.head = t;
    } else {
        prev->next = t;
    }
    qthread_incr(&(q->advisory_queuelen), 1);
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
}                                      /*}}} */

void INTERNAL qt_threadqueue_enqueue_yielded(qt_threadqueue_t   *q,
                                             qthread_t          *t,
                                             qthread_shepherd_t *shep)
{                                      /*{{{ */
    qt_threadqueue_enqueue(q, t, shep);
}                                      /*}}} */

ssize_t INTERNAL qt_threadqueue_advisory_queuelen(qt_threadqueue_t *q)
{                                      /*{{{ */
    assert(q);
    return q->advisory_queuelen;
}                                      /*}}} */

static inline NEMESIS_entry *qt_internal_NEMESIS_dequeue(NEMESIS_queue *q)
{                                      /*{{{ */
    NEMESIS_entry *retval = q->head;

    if ((retval != NULL) && (retval != (void *)1)) {
        if (retval->next != NULL) {
            q->head      = retval->next;
            retval->next = NULL;
        } else {
            NEMESIS_entry *old;
            q->head = NULL;
            old     = qthread_cas_ptr(&(q->tail), retval, NULL);
            if (old != retval) {
                while (retval->next == NULL) SPINLOCK_BODY();
                q->head      = retval->next;
                retval->next = NULL;
            }
        }
    }
    return retval;
}                                      /*}}} */

qthread_t INTERNAL *qt_threadqueue_dequeue(qt_threadqueue_t *q)
{                                      /*{{{ */
    NEMESIS_entry *retval = qt_internal_NEMESIS_dequeue(&q->q);

    if (retval) {
        assert(retval->next == NULL);
        qthread_incr(&(q->advisory_queuelen), -1);
    }
    return (qthread_t *)retval;
}                                      /*}}} */

qthread_t INTERNAL *qt_threadqueue_dequeue_blocking(qt_threadqueue_t *q)
{                                      /*{{{ */
    NEMESIS_entry *retval = qt_internal_NEMESIS_dequeue(&q->q);

    if (retval == NULL) {
        while (q->q.shadow_head == NULL && q->q.head == NULL) {
#ifndef QTHREAD_CONDWAIT_BLOCKING_QUEUE
            SPINLOCK_BODY();
#else
            if (qthread_incr(&q->frustration, 1) > 1000) {
                qassert(pthread_mutex_lock(&q->trigger_lock), 0);
                if (q->frustration > 1000) {
                    qassert(pthread_cond_wait(&q->trigger, &q->trigger_lock),
                            0);
                }
                qassert(pthread_mutex_unlock(&q->trigger_lock), 0);
            }
#endif      /* ifdef USE_HARD_POLLING */
        }
        retval = qt_internal_NEMESIS_dequeue(&q->q);
    }
    assert(retval);
    assert(retval->next == NULL);
    qthread_incr(&(q->advisory_queuelen), -1);
    return (qthread_t *)retval;
}                                      /*}}} */

/* vim:set expandtab: */
