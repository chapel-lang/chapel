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
#include "qt_debug.h"

/* Note: this queue is SAFE to use with multiple de-queuers, with the caveat
 * that if you have multiple dequeuer's, you'll need to solve the ABA problem.
 * (single dequeuer == no ABA problem) Also, yielding changes behavior a bit in
 * the multiple-dequeuer case.
 */

/* Data Structures */
struct _qt_threadqueue_node {
    struct _qt_threadqueue_node *next;
    qthread_t                   *thread;
};

struct _qt_threadqueue {
    qt_threadqueue_node_t *stack;
    /* the following is for estimating a queue's "busy" level, and is not
     * guaranteed accurate (that would be a race condition) */
    saligned_t      advisory_queuelen;
#ifdef QTHREAD_CONDWAIT_BLOCKING_QUEUE
    uint32_t        frustration;
    pthread_cond_t  trigger;
    pthread_mutex_t trigger_lock;
#endif
} /* qt_threadqueue_t */;

/* Memory Management */
#if defined(UNPOOLED_QUEUES) || defined(UNPOOLED)
# define ALLOC_THREADQUEUE() (qt_threadqueue_t *)MALLOC(sizeof(qt_threadqueue_t))
# define FREE_THREADQUEUE(t) FREE(t, sizeof(qt_threadqueue_t))
# define ALLOC_TQNODE()      (qt_threadqueue_node_t *)MALLOC(sizeof(qt_threadqueue_node_t))
# define FREE_TQNODE(t)      FREE(t, sizeof(qt_threadqueue_node_t))
void INTERNAL qt_threadqueue_subsystem_init(void) {}
#else /* if defined(UNPOOLED_QUEUES) || defined(UNPOOLED) */
qt_threadqueue_pools_t generic_threadqueue_pools = { NULL, NULL };
# define ALLOC_THREADQUEUE() (qt_threadqueue_t *)qt_mpool_alloc(generic_threadqueue_pools.queues)
# define FREE_THREADQUEUE(t) qt_mpool_free(generic_threadqueue_pools.queues, t)
# define ALLOC_TQNODE()      (qt_threadqueue_node_t *)qt_mpool_alloc(generic_threadqueue_pools.nodes)
# define FREE_TQNODE(t)      qt_mpool_free(generic_threadqueue_pools.nodes, t)

static void qt_threadqueue_subsystem_shutdown(void)
{
    qt_mpool_destroy(generic_threadqueue_pools.queues);
    qt_mpool_destroy(generic_threadqueue_pools.nodes);
}

void INTERNAL qt_threadqueue_subsystem_init(void)
{
    generic_threadqueue_pools.queues = qt_mpool_create(sizeof(qt_threadqueue_t));
    generic_threadqueue_pools.nodes  = qt_mpool_create_aligned(sizeof(qt_threadqueue_node_t), sizeof(void *));
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

static qthread_t *qt_threadqueue_dequeue(qt_threadqueue_t *q)
{   /*{{{*/
    qt_threadqueue_node_t *retval = q->stack;

    if (retval != NULL) {
        qt_threadqueue_node_t *old, *new;

#ifdef QTHREAD_MULTITHREADED_SHEPHERDS
# error This dequeue function is not safe! retval may be freed before we dereference it to find the next ptr. Need to use hazardptrs.
#endif
        do {
            old    = retval;
            new    = retval->next;
            retval = qthread_cas_ptr(&q->stack, old, new);
        } while (retval != old && retval != NULL);
    }
    if (retval != NULL) {
        qthread_t *t = retval->thread;
        FREE_TQNODE(retval);
        (void)qthread_incr(&(q->advisory_queuelen), -1);
        return t;
    } else {
        return NULL;
    }
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

#ifdef QTHREAD_USE_SPAWNCACHE
int INTERNAL qt_threadqueue_private_enqueue(qt_threadqueue_private_t *restrict pq,
                                            qt_threadqueue_t *restrict         q,
                                            qthread_t *restrict                t)
{   /*{{{*/
    return 0;
} /*}}}*/

int INTERNAL qt_threadqueue_private_enqueue_yielded(qt_threadqueue_private_t *restrict q,
                                                    qthread_t *restrict                t)
{   /*{{{*/
    return 0;
} /*}}}*/

#endif /* ifdef QTHREAD_USE_SPAWNCACHE */

void INTERNAL qt_threadqueue_enqueue(qt_threadqueue_t *restrict q,
                                     qthread_t *restrict        t)
{   /*{{{*/
    qt_threadqueue_node_t *old, *new;
    qt_threadqueue_node_t *node;

    assert(q);
    assert(t);

    qthread_debug(THREADQUEUE_CALLS, "q(%p), t(%p->%u)\n", q, t, t->thread_id);

    node = ALLOC_TQNODE();
    assert(node != NULL);
    node->thread = t;
    node->next   = NULL;

    old = q->stack;                    /* should be an atomic read */
    do {
        node->next = old;
        new        = qthread_cas_ptr(&(q->stack), old, node);
        if (new != old) {
            old = new;
        } else {
            break;
        }
    } while (1);
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

#ifdef QTHREAD_MULTITHREADED_SHEPHERDS
    qthread_t *top = qt_threadqueue_dequeue(q);
    qt_threadqueue_enqueue(q, t);
    if (top) {
        qt_threadqueue_enqueue(q, top);
    }
#else
    /* THIS is not safe for multiple dequeuers */
    qt_threadqueue_node_t *cursor = q->stack;
    if (cursor) {
        qt_threadqueue_node_t *node;
        while (cursor->next) {
            cursor = cursor->next;
        }
        assert(cursor->next == NULL);
        /* alloc the node */
        node = ALLOC_TQNODE();
        assert(node != NULL);
        node->thread = t;
        node->next   = NULL;
        /* append the node */
        cursor->next = node;
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

qthread_t INTERNAL *qt_scheduler_get_thread(qt_threadqueue_t         *q,
                                            qt_threadqueue_private_t *QUNUSED(qc),
                                            uint_fast8_t              QUNUSED(active))
{   /*{{{*/
    qthread_t *retval = qt_threadqueue_dequeue(q);

    qthread_debug(THREADQUEUE_CALLS, "q(%p)\n", q);
    if (retval == NULL) {
        while (q->stack == NULL) {
#ifndef QTHREAD_CONDWAIT_BLOCKING_QUEUE
            SPINLOCK_BODY();
#else
            COMPILER_FENCE;
            if (qthread_incr(&q->frustration, 1) > 1000) {
                qassert(pthread_mutex_lock(&q->trigger_lock), 0);
                if (q->frustration > 1000) {
                    qassert(pthread_cond_wait(&q->trigger, &q->trigger_lock), 0);
                }
                qassert(pthread_mutex_unlock(&q->trigger_lock), 0);
            }
#endif      /* ifdef USE_HARD_POLLING */
        }
        retval = qt_threadqueue_dequeue(q);
    }
    assert(retval);
    assert(retval->next == NULL);
    qthread_debug(THREADQUEUE_BEHAVIOR, "found thread %u (%p); q(%p)\n", retval->thread_id, retval, q);
    return retval;
} /*}}}*/

/* some place-holder functions */
void INTERNAL qthread_steal_stat(void)
{}

void INTERNAL qthread_steal_enable(void)
{}

void INTERNAL qthread_steal_disable(void)
{}

void INTERNAL qthread_cas_steal_stat(void)
{}

/* vim:set expandtab: */
