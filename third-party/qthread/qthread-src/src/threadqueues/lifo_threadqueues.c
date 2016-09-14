#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

/* System Headers */
#include <stdio.h>
#include <stdlib.h>

/* Internal Headers */
#include "qthread_innards.h"           /* for qlib */
#include "qthread/qthread.h"
#include "qt_macros.h"
#include "qt_visibility.h"
#include "qt_asserts.h"
#include "qt_prefetch.h"
#include "qt_threadqueues.h"
#include "qt_qthread_struct.h"
#include "qt_atomics.h"
#include "qt_debug.h"
#ifdef QTHREAD_USE_EUREKAS
#include "qt_eurekas.h"
#endif /* QTHREAD_USE_EUREKAS */
#include "qt_subsystems.h"

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
    saligned_t advisory_queuelen;
#ifdef QTHREAD_CONDWAIT_BLOCKING_QUEUE
    uint32_t   frustration;
    QTHREAD_COND_DECL(trigger)
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
    QTHREAD_COND_INIT(q->trigger);
#endif /* ifdef QTHREAD_CONDWAIT_BLOCKING_QUEUE */

    return q;
} /*}}}*/

static qthread_t *qt_threadqueue_dequeue(qt_threadqueue_t *q)
{   /*{{{*/
    qt_threadqueue_node_t *retval = q->stack;

    if (retval != NULL) {
        qt_threadqueue_node_t *old, *new;

#ifdef QTHREAD_LIFO_MULTI_DEQUEUER
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
    QTHREAD_COND_DESTROY(q->trigger);
#endif
    FREE_THREADQUEUE(q);
} /*}}}*/

#ifdef QTHREAD_USE_SPAWNCACHE
qthread_t INTERNAL *qt_threadqueue_private_dequeue(qt_threadqueue_private_t *c)
{   /*{{{*/
    return NULL;
} /*}}}*/

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

void INTERNAL qt_threadqueue_enqueue_cache(qt_threadqueue_t         *q,
                                           qt_threadqueue_private_t *cache)
{}

void INTERNAL qt_threadqueue_private_filter(qt_threadqueue_private_t *restrict c,
                                            qt_threadqueue_filter_f            f)
{}
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
        QTHREAD_COND_LOCK(q->trigger);
        if (q->frustration) {
            q->frustration = 0;
            QTHREAD_COND_SIGNAL(q->trigger);
        }
        QTHREAD_COND_UNLOCK(q->trigger);
    }
#endif
} /*}}}*/

void INTERNAL qt_threadqueue_enqueue_yielded(qt_threadqueue_t *restrict q,
                                             qthread_t *restrict        t)
{   /*{{{*/
    assert(q);
    assert(t);

#ifdef QTHREAD_LIFO_MULTI_DEQUEUER
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
        (void)qthread_incr(&(q->advisory_queuelen), 1);
    } else {
        qt_threadqueue_enqueue(q, t);
    }
#endif /* ifdef QTHREAD_LIFO_MULTI_DEQUEUER */
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
#ifdef QTHREAD_USE_EUREKAS
    qt_eureka_disable();
#endif /* QTHREAD_USE_EUREKAS */
    qthread_t *retval = qt_threadqueue_dequeue(q);

    qthread_debug(THREADQUEUE_CALLS, "q(%p)\n", q);
    if (retval == NULL) {
#ifdef QTHREAD_USE_EUREKAS
        qt_eureka_check(0);
#endif /* QTHREAD_USE_EUREKAS */
        while (q->stack == NULL) {
#ifndef QTHREAD_CONDWAIT_BLOCKING_QUEUE
            SPINLOCK_BODY();
#else
            COMPILER_FENCE;
            if (qthread_incr(&q->frustration, 1) > 1000) {
                QTHREAD_COND_LOCK(q->trigger);
                if (q->frustration > 1000) {
                    QTHREAD_COND_WAIT(q->trigger);
                }
                QTHREAD_COND_UNLOCK(q->trigger);
            }
#endif      /* ifdef USE_HARD_POLLING */
        }
#ifdef QTHREAD_USE_EUREKAS
        qt_eureka_disable();
#endif /* QTHREAD_USE_EUREKAS */
        retval = qt_threadqueue_dequeue(q);
    }
    assert(retval);
    qthread_debug(THREADQUEUE_BEHAVIOR, "found thread %u (%p); q(%p)\n", retval->thread_id, retval, q);
    return retval;
} /*}}}*/

/* walk queue removing all tasks matching this description */
void INTERNAL qt_threadqueue_filter(qt_threadqueue_t       *q,
                                    qt_threadqueue_filter_f f)
{   /*{{{*/
    qt_threadqueue_node_t *curs, **ptr;

    assert(q != NULL);

    curs = q->stack;
    ptr  = &q->stack;
    while (curs) {
        qthread_t *t = curs->thread;
        switch (f(t)) {
            case IGNORE_AND_CONTINUE: // ignore, move on
                ptr  = &curs->next;
                curs = curs->next;
                break;
            case IGNORE_AND_STOP: // ignore, stop looking
                return;

            case REMOVE_AND_CONTINUE: // remove, move on
            {
                qt_threadqueue_node_t *freeme = curs;

#ifdef QTHREAD_USE_EUREKAS
                qthread_internal_assassinate(t);
#endif /* QTHREAD_USE_EUREKAS */
                *ptr = curs->next;
                curs = curs->next;
                FREE_TQNODE(freeme);
                break;
            }
            case REMOVE_AND_STOP: // remove, stop looking;
#ifdef QTHREAD_USE_EUREKAS
                qthread_internal_assassinate(t);
#endif /* QTHREAD_USE_EUREKAS */
                *ptr = curs->next;
                FREE_TQNODE(curs);
                return;
        }
    }
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

qthread_shepherd_id_t INTERNAL qt_threadqueue_choose_dest(qthread_shepherd_t * curr_shep)
{
    qthread_shepherd_id_t dest_shep_id = 0;

    if (curr_shep) {
        dest_shep_id               = curr_shep->sched_shepherd++;
        curr_shep->sched_shepherd *= (qlib->nshepherds > (dest_shep_id + 1));
    } else {
        dest_shep_id = 
            (qthread_shepherd_id_t)qthread_internal_incr_mod(
                &qlib->sched_shepherd,
                qlib->nshepherds,
                &qlib->sched_shepherd_lock);
    }

    return dest_shep_id;
}

qthread_t INTERNAL * qt_threadqueue_dequeue_specific(qt_threadqueue_t * q,
                                                     void             * value)
{
    return NULL;
}

size_t INTERNAL qt_threadqueue_policy(const enum threadqueue_policy policy)
{
    switch (policy) {
        case SINGLE_WORKER:
            return THREADQUEUE_POLICY_TRUE;
        default:
            return THREADQUEUE_POLICY_UNSUPPORTED;
    }
}

/* vim:set expandtab: */
