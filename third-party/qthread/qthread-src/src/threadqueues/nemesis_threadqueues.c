#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

/* System Headers */
#include <pthread.h>

#include <stdio.h>
#include <stdlib.h>

/* API Headers */
#include "qthread/qthread.h"

/* Internal Headers */
#include "qthread_innards.h"           /* for qlib */
#include "qt_macros.h"
#include "qt_visibility.h"
#include "qt_asserts.h"
#include "qt_prefetch.h"
#include "qt_threadqueues.h"
#include "qt_envariables.h"
#include "qt_qthread_struct.h"
#include "qt_debug.h"
#ifdef QTHREAD_USE_EUREKAS
#include "qt_eurekas.h"
#endif /* QTHREAD_USE_EUREKAS */
#include "qt_subsystems.h"
#include "qt_qthread_mgmt.h"             /* for qthread_thread_free() */

/* This thread queueing uses the NEMESIS lock-free queue protocol from
 * http://www.mcs.anl.gov/~buntinas/papers/ccgrid06-nemesis.pdf
 * Note: it is NOT SAFE to use with multiple de-queuers, it is ONLY safe to use
 * with multiple enqueuers and a single de-queuer. */

int num_spins_before_condwait;
#ifdef QTHREAD_OVERSUBSCRIPTION
#define DEFAULT_SPINCOUNT 300
#else
#define DEFAULT_SPINCOUNT 300000
#endif

/* Data Structures */
struct _qt_threadqueue_node {
    struct _qt_threadqueue_node *next;
    qthread_t                   *thread;
};

typedef struct {
    /* The First Cacheline */
    void   *head;
    void   *tail;
    uint8_t pad1[CACHELINE_WIDTH - (2 * sizeof(void *))];
    /* The Second Cacheline */
    void *shadow_head;
    /* the following is for estimating a queue's "busy" level, and is not
     * guaranteed accurate (that would be a race condition) */
    saligned_t nemesis_advisory_queuelen;
    uint8_t    pad2[CACHELINE_WIDTH - sizeof(void *) - sizeof(saligned_t)];
}
NEMESIS_queue Q_ALIGNED (CACHELINE_WIDTH);

struct _qt_threadqueue {
    NEMESIS_queue q;
    /* the following is for estimating a queue's "busy" level, and is not
     * guaranteed accurate (that would be a race condition) */
    saligned_t advisory_queuelen;
#ifdef QTHREAD_CONDWAIT_BLOCKING_QUEUE
    uint32_t   frustration;
    QTHREAD_COND_DECL(trigger);
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
{   /*{{{*/
    qt_mpool_destroy(generic_threadqueue_pools.queues);
    qt_mpool_destroy(generic_threadqueue_pools.nodes);
} /*}}}*/

void INTERNAL qt_threadqueue_subsystem_init(void)
{   /*{{{*/

    num_spins_before_condwait = qt_internal_get_env_num("SPINCOUNT", DEFAULT_SPINCOUNT, 0);

    generic_threadqueue_pools.queues = qt_mpool_create(sizeof(qt_threadqueue_t));
    generic_threadqueue_pools.nodes  = qt_mpool_create_aligned(sizeof(qt_threadqueue_node_t), 8);
    qthread_internal_cleanup(qt_threadqueue_subsystem_shutdown);
} /*}}}*/
#endif /* if defined(UNPOOLED_QUEUES) || defined(UNPOOLED) */

/* Thankfully, NEMESIS does not suffer from the ABA problem. */

qt_threadqueue_t INTERNAL *qt_threadqueue_new(void)
{                                      /*{{{ */
    qt_threadqueue_t *q = ALLOC_THREADQUEUE();

    qassert_ret(q != NULL, NULL);

    q->q.shadow_head               = q->q.head = q->q.tail = NULL;
    q->advisory_queuelen           = 0;
    q->q.nemesis_advisory_queuelen = 0; // redundant
#ifdef QTHREAD_CONDWAIT_BLOCKING_QUEUE
    q->frustration = 0;
    QTHREAD_COND_INIT(q->trigger);
#endif /* ifdef QTHREAD_CONDWAIT_BLOCKING_QUEUE */

    return q;
}                                      /*}}} */

static inline qt_threadqueue_node_t *qt_internal_NEMESIS_dequeue(NEMESIS_queue *q)
{                                      /*{{{ */
    if (!q->shadow_head) {
        if (!q->head) {
            return NULL;
        }
        q->shadow_head = q->head;
        q->head        = NULL;
    }

    qt_threadqueue_node_t *const retval = (void *volatile)(q->shadow_head);

    if ((retval != NULL) && (retval != (void *)1)) {
        if (retval->next != NULL) {
            q->shadow_head = retval->next;
            retval->next   = NULL;
        } else {
            qt_threadqueue_node_t *old;
            q->shadow_head = NULL;
            old            = qthread_cas_ptr(&(q->tail), retval, NULL);
            if (old != retval) {
                while (retval->next == NULL) SPINLOCK_BODY();
                q->shadow_head = retval->next;
                retval->next   = NULL;
            }
        }
    }
    return retval;
}                                      /*}}} */

static inline qt_threadqueue_node_t *qt_internal_NEMESIS_dequeue_st(NEMESIS_queue *q)
{                                      /*{{{ */
    if (!q->shadow_head) {
        if (!q->head) {
            return NULL;
        }
        q->shadow_head = q->head;
        q->head        = NULL;
    }

    qt_threadqueue_node_t *const retval = (void *volatile)(q->shadow_head);

    if ((retval != NULL) && (retval != (void *)1)) {
        if (retval->next != NULL) {
            q->shadow_head = retval->next;
            retval->next   = NULL;
        } else {
            qt_threadqueue_node_t *old;
            q->shadow_head = NULL;
            if (q->tail == retval) {
                q->tail = NULL;
            }
        }
    }
    qthread_debug(THREADQUEUE_DETAILS, "nemesis q:%p head:%p tail:%p shadow_head:%p\n", q, q->head, q->tail, q->shadow_head);
    return retval;
}                                      /*}}} */

static qthread_t *qt_threadqueue_dequeue(qt_threadqueue_t *q)
{                                      /*{{{ */
    qt_threadqueue_node_t *node = qt_internal_NEMESIS_dequeue(&q->q);

    if (node) {
        qthread_t *retval = node->thread;
        assert(node->next == NULL);
        (void)qthread_incr(&(q->advisory_queuelen), -1);
        FREE_TQNODE(node);
        return retval;
    } else {
        return NULL;
    }
}                                      /*}}} */

void INTERNAL qt_threadqueue_free(qt_threadqueue_t *q)
{                                      /*{{{ */
    assert(q);
    while (1) {
        qt_threadqueue_node_t *node = qt_internal_NEMESIS_dequeue_st(&q->q);
        if (node) {
            qthread_t *retval = node->thread;
            assert(node->next == NULL);
            (void)qthread_incr(&(q->advisory_queuelen), -1);
            FREE_TQNODE(node);
            qthread_thread_free(retval);
        } else {
            break;
        }
    }
#ifdef QTHREAD_CONDWAIT_BLOCKING_QUEUE
    QTHREAD_COND_DESTROY(q->trigger);
#endif
    FREE_THREADQUEUE(q);
}                                      /*}}} */

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

void INTERNAL qthread_steal_enable() {}
void INTERNAL qthread_steal_disable() {}

#ifdef QTHREAD_PARANOIA
static void sanity_check_tq(NEMESIS_queue *q)
{   /*{{{*/
    qt_threadqueue_node_t *curs;

    assert(q);
    /*if (q->head != NULL) {
     *  assert(q->tail != NULL);
     * }*/
    if (q->shadow_head) {
        assert(q->head != q->shadow_head);
    }
    if (q->tail != NULL) {
        if (q->head == NULL) {
            assert(q->shadow_head != NULL);
        }
    }
    if ((q->head != NULL) || (q->tail != NULL)) {
        if (q->shadow_head) {
            curs = q->shadow_head;
            assert(curs->thread);
            assert(curs->thread != (void *)0x7777777777777777);
            while (curs->next) {
                curs = curs->next;
                assert(curs->thread);
                assert(curs->thread != (void *)0x7777777777777777);
            }
        }
        if (q->head) {
            curs = q->head;
            assert(curs->thread);
            assert(curs->thread != (void *)0x7777777777777777);
            while (curs->next) {
                curs = curs->next;
                assert(curs->thread);
                assert(curs->thread != (void *)0x7777777777777777);
            }
        }
    }
} /*}}}*/

#endif /* ifdef QTHREAD_PARANOIA */

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

void INTERNAL qt_threadqueue_enqueue(qt_threadqueue_t *restrict q,
                                     qthread_t *restrict        t)
{                                      /*{{{ */
    qt_threadqueue_node_t *node, *prev;

    assert(q);
    assert(t);

    PARANOIA(sanity_check_tq(&q->q));
    qthread_debug(THREADQUEUE_CALLS, "q(%p), t(%p->%u)\n", q, t, t->thread_id);

    node = ALLOC_TQNODE();
    assert(node != NULL);
    node->thread = t;
    node->next   = NULL;

    prev = qt_internal_atomic_swap_ptr((void **)&(q->q.tail), node);

    if (prev == NULL) {
        q->q.head = node;
    } else {
        prev->next = node;
    }
    PARANOIA(sanity_check_tq(&q->q));
    (void)qthread_incr(&(q->advisory_queuelen), 1);
#ifdef QTHREAD_CONDWAIT_BLOCKING_QUEUE
    /* awake waiter */
    /* Yes, this needs to be here, to prevent reading frustration being hoisted
     * to before the enqueue operations. */
    MACHINE_FENCE;
    if (q->frustration) {
        QTHREAD_COND_LOCK(q->trigger);
        if (q->frustration) {
            q->frustration = 0;
            QTHREAD_COND_SIGNAL(q->trigger);
        }
        QTHREAD_COND_UNLOCK(q->trigger);
    }
#endif /* ifdef QTHREAD_CONDWAIT_BLOCKING_QUEUE */
}                                      /*}}} */

void INTERNAL qt_threadqueue_enqueue_yielded(qt_threadqueue_t *restrict q,
                                             qthread_t *restrict        t)
{                                      /*{{{ */
    qt_threadqueue_enqueue(q, t);
}                                      /*}}} */

ssize_t INTERNAL qt_threadqueue_advisory_queuelen(qt_threadqueue_t *q)
{                                      /*{{{ */
    assert(q);
    return q->advisory_queuelen;
}                                      /*}}} */

qthread_t INTERNAL *qt_scheduler_get_thread(qt_threadqueue_t         *q,
                                            qt_threadqueue_private_t *QUNUSED(qc),
                                            uint_fast8_t              QUNUSED(active))
{                                      /*{{{ */
    int i;
#ifdef QTHREAD_USE_EUREKAS
    qt_eureka_disable();
#endif /* QTHREAD_USE_EUREKAS */
    qthread_debug(THREADQUEUE_DETAILS, "q(%p)->q {head:%p tail:%p sh:%p} q->advisory_queuelen:%u\n", q, q->q.head, q->q.tail, q->q.shadow_head, q->advisory_queuelen);
    PARANOIA(sanity_check_tq(&q->q));
    qt_threadqueue_node_t *node = qt_internal_NEMESIS_dequeue(&q->q);
    qthread_t             *retval;

    qthread_debug(THREADQUEUE_DETAILS, "q(%p)->q {head:%p tail:%p sh:%p} q->advisory_queuelen:%u\n", q, q->q.head, q->q.tail, q->q.shadow_head, q->advisory_queuelen);
    PARANOIA(sanity_check_tq(&q->q));
    if (node == NULL) {
#ifdef QTHREAD_USE_EUREKAS
        qt_eureka_check(0);
#endif /* QTHREAD_USE_EUREKAS */

#ifdef QTHREAD_CONDWAIT_BLOCKING_QUEUE
        i = num_spins_before_condwait;
        while (q->q.shadow_head == NULL && q->q.head == NULL && i > 0) {
          SPINLOCK_BODY();
          i--;
        }
#endif      /* QTHREAD_CONDWAIT_BLOCKING_QUEUE */

        while (q->q.shadow_head == NULL && q->q.head == NULL) {
#ifndef QTHREAD_CONDWAIT_BLOCKING_QUEUE
            SPINLOCK_BODY();
#else
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
        node = qt_internal_NEMESIS_dequeue(&q->q);
    }
    assert(node);
    assert(node->next == NULL);
    (void)qthread_incr(&(q->advisory_queuelen), -1);
    retval = node->thread;
    FREE_TQNODE(node);
    qthread_debug(THREADQUEUE_DETAILS, "q(%p)->q {head:%p tail:%p sh:%p} q->advisory_queuelen:%u\n", q, q->q.head, q->q.tail, q->q.shadow_head, q->advisory_queuelen);
    PARANOIA(sanity_check_tq(&q->q));
    return retval;
}                                      /*}}} */

/* walk queue removing all tasks matching this description */
void INTERNAL qt_threadqueue_filter(qt_threadqueue_t       *q,
                                    qt_threadqueue_filter_f f)
{   /*{{{*/
    NEMESIS_queue          tmp;
    qt_threadqueue_node_t *curs, *prev;

    assert(q != NULL);
    qthread_debug(THREADQUEUE_FUNCTIONS, "begin q:%p f:%p\n", q, f);

    tmp.head                      = NULL;
    tmp.tail                      = NULL;
    tmp.shadow_head               = NULL;
    tmp.nemesis_advisory_queuelen = 0;
    qthread_debug(THREADQUEUE_DETAILS, "q(%p)->q {head:%p tail:%p} q->advisory_queuelen:%u\n", q, q->q.head, q->q.tail, q->advisory_queuelen);
    PARANOIA(sanity_check_tq(&q->q));
    while ((curs = qt_internal_NEMESIS_dequeue_st(&q->q))) {
        qthread_t *t = curs->thread;
        qthread_debug(THREADQUEUE_DETAILS, "q(%p)->q {head:%p tail:%p} q->advisory_queuelen:%u\n", q, q->q.head, q->q.tail, q->advisory_queuelen);
        PARANOIA(sanity_check_tq(&tmp));
        PARANOIA(sanity_check_tq(&q->q));
        switch (f(t)) {
            case IGNORE_AND_CONTINUE: // ignore, move on
                prev = qt_internal_atomic_swap_ptr((void **)&(tmp.tail), curs);
                if (prev == NULL) {
                    tmp.head = curs;
                } else {
                    prev->next = curs;
                }
                tmp.nemesis_advisory_queuelen++;
                break;
            case IGNORE_AND_STOP: // ignore, stop looking
                prev = qt_internal_atomic_swap_ptr((void **)&(tmp.tail), curs);
                if (prev == NULL) {
                    tmp.head = curs;
                } else {
                    prev->next = curs;
                }
                tmp.nemesis_advisory_queuelen++;
                goto pushback;
            case REMOVE_AND_CONTINUE: // remove, move on
#ifdef QTHREAD_USE_EUREKAS
                qthread_internal_assassinate(t);
#endif /* QTHREAD_USE_EUREKAS */
                FREE_TQNODE(curs);
                break;
            case REMOVE_AND_STOP: // remove, stop looking
#ifdef QTHREAD_USE_EUREKAS
                qthread_internal_assassinate(t);
#endif /* QTHREAD_USE_EUREKAS */
                FREE_TQNODE(curs);
                goto pushback;
        }
    }
pushback:
    /* dequeue the rest of the queue */
    qthread_debug(THREADQUEUE_DETAILS, "q(%p)->q {head:%p tail:%p} q->advisory_queuelen:%u\n", q, q->q.head, q->q.tail, q->advisory_queuelen);
    qthread_debug(THREADQUEUE_DETAILS, "tmp {head:%p tail:%p} tmp->advisory_queuelen:%u\n", tmp.head, tmp.tail, tmp.nemesis_advisory_queuelen);
    PARANOIA(sanity_check_tq(&tmp));
    if (q->q.head) {
        prev = qt_internal_atomic_swap_ptr((void **)&(tmp.tail), q->q.head);
        if (prev == NULL) {
            tmp.head = q->q.head;
        } else {
            prev->next = q->q.head;
        }
        tmp.nemesis_advisory_queuelen += q->advisory_queuelen;
        tmp.tail                       = q->q.tail;
    }
    q->q.head            = tmp.head;
    q->q.tail            = tmp.tail;
    q->q.shadow_head     = NULL;
    q->advisory_queuelen = tmp.nemesis_advisory_queuelen;
    qthread_debug(THREADQUEUE_DETAILS, "q(%p)->q {head:%p tail:%p} q->advisory_queuelen:%u\n", q, q->q.head, q->q.tail, q->advisory_queuelen);
    PARANOIA(sanity_check_tq(&q->q));
} /*}}}*/

/* some place-holder functions */
void INTERNAL qthread_steal_stat(void) {}
void INTERNAL qthread_cas_steal_stat(void) {}

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
