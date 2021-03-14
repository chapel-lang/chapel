#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

/* System Headers */
#include <stdlib.h>

/* Internal Headers */
#include "qthread_innards.h"           /* for qlib */
#include "qthread/qthread.h"
#include "qt_macros.h"
#include "qt_visibility.h"
#include "qt_atomics.h"
#include "qt_shepherd_innards.h"
#include "qt_qthread_struct.h"
#include "qt_asserts.h"
#include "qt_prefetch.h"
#include "qt_threadqueues.h"
#include "qt_debug.h"
#if defined(UNPOOLED_QUEUES) || defined(UNPOOLED)
#include "qt_alloc.h"
#endif
#ifdef QTHREAD_USE_EUREKAS
#include "qt_eurekas.h"
#endif /* QTHREAD_USE_EUREKAS */
#include "qt_subsystems.h"

/* Data Structures */
struct _qt_threadqueue_node {
    struct _qt_threadqueue_node *next;
    qthread_t                   *value;
} /* qt_threadqueue_node_t */;

struct _qt_threadqueue {
    qt_threadqueue_node_t *head;
    qt_threadqueue_node_t *tail;
#ifdef QTHREAD_CONDWAIT_BLOCKING_QUEUE
    aligned_t              fruitless;
    QTHREAD_COND_DECL(trigger);
#endif                          /* CONDWAIT */
    /* the following is for estimating a queue's "busy" level, and is not
     * guaranteed accurate (that would be a race condition) */
    saligned_t advisory_queuelen;
} /* qt_threadqueue_t */;

/* Memory Management */
#if defined(UNPOOLED_QUEUES) || defined(UNPOOLED)
# define ALLOC_THREADQUEUE() (qt_threadqueue_t *)MALLOC(sizeof(qt_threadqueue_t))
# define FREE_THREADQUEUE(t) FREE(t, sizeof(qt_threadqueue_t))
static QINLINE void ALLOC_TQNODE(qt_threadqueue_node_t **ret)
{                                      /*{{{ */
    *ret = (qt_threadqueue_node_t *) qt_internal_aligned_alloc(sizeof(qt_threadqueue_node_t), 16);
    if (*ret != NULL) {
        memset(*ret, 0, sizeof(qt_threadqueue_node_t));
    }
}                                      /*}}} */

static void FREE_TQNODE(void *p)
{
    FREE_SCRIBBLE(p, sizeof(qt_threadqueue_node_t));
    qt_internal_aligned_free(p, 16);
}

void INTERNAL qt_threadqueue_subsystem_init(void) {}
#else /* if defined(UNPOOLED_QUEUES) || defined(UNPOOLED) */
qt_threadqueue_pools_t generic_threadqueue_pools;
# define ALLOC_THREADQUEUE() (qt_threadqueue_t *)qt_mpool_alloc(generic_threadqueue_pools.queues)
# define FREE_THREADQUEUE(t) qt_mpool_free(generic_threadqueue_pools.queues, t)

static QINLINE void ALLOC_TQNODE(qt_threadqueue_node_t **ret)
{                                      /*{{{ */
    *ret = (qt_threadqueue_node_t *)qt_mpool_alloc(generic_threadqueue_pools.nodes);
    if (*ret != NULL) {
        memset(*ret, 0, sizeof(qt_threadqueue_node_t));
    }
}                                      /*}}} */

static void FREE_TQNODE(void *p)
{
    qt_mpool_free(generic_threadqueue_pools.nodes, p);
}

static void qt_threadqueue_subsystem_shutdown(void)
{
    qt_mpool_destroy(generic_threadqueue_pools.nodes);
    qt_mpool_destroy(generic_threadqueue_pools.queues);
}

void INTERNAL qt_threadqueue_subsystem_init(void)
{
    generic_threadqueue_pools.nodes  = qt_mpool_create_aligned(sizeof(qt_threadqueue_node_t), 16);
    generic_threadqueue_pools.queues = qt_mpool_create(sizeof(qt_threadqueue_t));
    qthread_internal_cleanup(qt_threadqueue_subsystem_shutdown);
}

#endif /* if defined(UNPOOLED_QUEUES) || defined(UNPOOLED) */

ssize_t INTERNAL qt_threadqueue_advisory_queuelen(qt_threadqueue_t *q)
{   /*{{{*/
    return qthread_internal_atomic_read_s(&q->advisory_queuelen, &q->advisory_queuelen_m);
} /*}}}*/

/*****************************************/
/* functions to manage the thread queues */
/*****************************************/

// This lock-free algorithm borrowed from
// http://www.research.ibm.com/people/m/michael/podc-1996.pdf
// ... and modified to use hazard ptrs according to
// http://www.research.ibm.com/people/m/michael/ieeetpds-2004.pdf

qt_threadqueue_t INTERNAL *qt_threadqueue_new(void)
{                                      /*{{{ */
    qt_threadqueue_t *q = ALLOC_THREADQUEUE();

    if (q != NULL) {
#ifdef QTHREAD_CONDWAIT_BLOCKING_QUEUE
        q->fruitless = 0;
        QTHREAD_COND_INIT(q->trigger);
#endif   /* ifdef QTHREAD_CONDWAIT_BLOCKING_QUEUE */
        ALLOC_TQNODE(((qt_threadqueue_node_t **)&(q->head)));
        assert(q->head != NULL);
        if (q->head == NULL) {   // if we're not using asserts, fail nicely
#ifdef QTHREAD_CONDWAIT_BLOCKING_QUEUE
            QTHREAD_COND_DESTROY(q->trigger);
#endif
            FREE_THREADQUEUE(q);
            q = NULL;
        }
        q->tail       = q->head;
        q->tail->next = NULL;
    }
    return q;
}                                      /*}}} */

static qthread_t *qt_threadqueue_dequeue(qt_threadqueue_t *q)
{                                      /*{{{ */
    qthread_t *p = NULL;

    qt_threadqueue_node_t *head;
    qt_threadqueue_node_t *tail;
    qt_threadqueue_node_t *next_ptr;

    assert(q != NULL);
    while (1) {
        head = q->head;

        hazardous_ptr(0, head);
        if (head != q->head) {
            continue;                               // are head, tail, and next consistent?
        }

        tail     = q->tail;
        next_ptr = head->next;

        hazardous_ptr(1, next_ptr);

        if (next_ptr == NULL) {
            return NULL;                   // queue is empty
        }
        if (head == tail) { // tail is falling behind!
            (void)qt_cas((void **)&(q->tail),
                         (void *)tail,
                         next_ptr); // advance tail ptr
            continue;
        }
        // read value before CAS, otherwise another dequeue might free the next node
        p = next_ptr->value;
        if (qt_cas((void **)&(q->head),
                   (void *)head,
                   next_ptr) == head) {
            break;             // success!
        }
    }
    hazardous_release_node(FREE_TQNODE, head);
    if (p != NULL) {
        Q_PREFETCH(p);
        (void)qthread_internal_incr_s(&q->advisory_queuelen, &q->advisory_queuelen_m, -1);
    }
    return p;
}                                      /*}}} */

void INTERNAL qt_threadqueue_free(qt_threadqueue_t *q)
{                                      /*{{{ */
    while (q->head != q->tail) {
        qt_threadqueue_dequeue(q);
    }
    assert(q->head == q->tail);
#ifdef QTHREAD_CONDWAIT_BLOCKING_QUEUE
    QTHREAD_COND_DESTROY(q->trigger);
#endif
    FREE_TQNODE((qt_threadqueue_node_t *)q->head);
    FREE_THREADQUEUE(q);
}                                      /*}}} */

#ifdef QTHREAD_USE_SPAWNCACHE
qthread_t INTERNAL *qt_threadqueue_private_dequeue(qt_threadqueue_private_t *c)
{
    return NULL;
}

int INTERNAL qt_threadqueue_private_enqueue(qt_threadqueue_private_t *restrict pq,
                                            qt_threadqueue_t *restrict         q,
                                            qthread_t *restrict                t)
{
    return 0;
}

int INTERNAL qt_threadqueue_private_enqueue_yielded(qt_threadqueue_private_t *restrict q,
                                                    qthread_t *restrict                t)
{
    return 0;
}

void INTERNAL qt_threadqueue_enqueue_cache(qt_threadqueue_t         *q,
                                           qt_threadqueue_private_t *cache)
{}

void INTERNAL qt_threadqueue_private_filter(qt_threadqueue_private_t *restrict c,
                                            qt_threadqueue_filter_f            f)
{}
#endif /* ifdef QTHREAD_USE_SPAWNCACHE */

void INTERNAL qt_threadqueue_enqueue(qt_threadqueue_t *restrict q,
                                     qthread_t *restrict        t)
{                                      /*{{{ */
    qt_threadqueue_node_t *tail;
    qt_threadqueue_node_t *next;
    qt_threadqueue_node_t *node;

    assert(t != NULL);
    assert(q != NULL);
    qthread_debug(THREADQUEUE_CALLS, "q(%p), t(%p:%i): began head:%p tail:%p\n", q, t, t->thread_id, q->head, q->tail);

    ALLOC_TQNODE(&node);
    assert(node != NULL);

    node->value = t;
    node->next  = NULL;

    while (1) {
        qthread_debug(THREADQUEUE_DETAILS, "q(%p), t(%p:%i): reading q->tail\n", q, t, t->thread_id);
        tail = q->tail;

        hazardous_ptr(0, tail);
        if (tail != q->tail) {
            continue;          // are tail and next consistent?
        }

        next = tail->next;
        if (next != NULL) { // tail not pointing to last node
            (void)qt_cas((void **)&(q->tail),
                         (void *)tail,
                         next); // ABA hazard (mitigated by QCOMPOSE)
            continue;
        }
        // tail must be pointing to the last node
        if (qt_cas((void **)&(tail->next),
                   (void *)next,
                   node) == next) {
            break;                                  // success!
        }
    }
    (void)qt_cas((void **)&(q->tail),
                 (void *)tail,
                 node);
    qthread_debug(THREADQUEUE_DETAILS, "q(%p), t(%p:%i): appended head:%p nextptr:%p tail:%p\n", q, t, t->thread_id, q->head, q->head ? q->head->next : NULL, q->tail);

    (void)qthread_incr(&q->advisory_queuelen, 1);
#ifdef QTHREAD_CONDWAIT_BLOCKING_QUEUE
    if (q->fruitless) {
        QTHREAD_COND_LOCK(q->trigger);
        if (q->fruitless) {
            q->fruitless = 0;
            QTHREAD_BCAST(q->trigger);
        }
        QTHREAD_COND_UNLOCK(q->trigger);
    }
#endif
    hazardous_ptr(0, NULL); // release the ptr (avoid hazardptr resource exhaustion)
}                           /*}}} */

void qt_threadqueue_enqueue_yielded(qt_threadqueue_t *restrict q,
                                    qthread_t *restrict        t)
{   /*{{{*/
    qt_threadqueue_enqueue(q, t);
} /*}}}*/

qthread_t INTERNAL *qt_scheduler_get_thread(qt_threadqueue_t         *q,
                                            qt_threadqueue_private_t *QUNUSED(qc),
                                            uint_fast8_t              QUNUSED(active))
{                                      /*{{{ */
    qthread_t *p = NULL;

    qt_threadqueue_node_t *head;
    qt_threadqueue_node_t *tail;
    qt_threadqueue_node_t *next_ptr;

    assert(q != NULL);
    qthread_debug(THREADQUEUE_CALLS, "q(%p): began\n", q);
#ifdef QTHREAD_USE_EUREKAS
    qt_eureka_disable();
#endif /* QTHREAD_USE_EUREKAS */
    qthread_debug(THREADQUEUE_DETAILS, "q(%p): head=%p next_ptr=%p tail=%p\n", q, q->head, q->head ? q->head->next : NULL, q->tail);
    while (1) {
        head = q->head;

        hazardous_ptr(0, head);
        if (head != q->head) {
            continue;                         // are head, tail, and next consistent?
        }

        tail     = q->tail;
        next_ptr = head->next;

        hazardous_ptr(1, next_ptr);

        if (next_ptr == NULL) { // queue is empty
#ifdef QTHREAD_CONDWAIT_BLOCKING_QUEUE
            if (qthread_internal_incr(&q->fruitless, &q->fruitless_m, 1) > 1000) {
# ifdef QTHREAD_USE_EUREKAS
                qt_eureka_check(0);
# endif /* QTHREAD_USE_EUREKAS */
                QTHREAD_COND_LOCK(q->trigger);
                while (q->fruitless > 1000) {
                    QTHREAD_COND_WAIT(q->trigger);
                }
                QTHREAD_COND_UNLOCK(q->trigger);
# ifdef QTHREAD_USE_EUREKAS
                qt_eureka_disable();
# endif /* QTHREAD_USE_EUREKAS */
            } else {
# ifdef QTHREAD_USE_EUREKAS
                qt_eureka_check(0);
# endif /* QTHREAD_USE_EUREKAS */
# ifdef HAVE_PTHREAD_YIELD
                pthread_yield();
# elif HAVE_SHED_YIELD
                sched_yield();
# endif
# ifdef QTHREAD_USE_EUREKAS
                qt_eureka_disable();
# endif /* QTHREAD_USE_EUREKAS */
            }
#else       /* ifdef QTHREAD_CONDWAIT_BLOCKING_QUEUE */
# ifdef QTHREAD_USE_EUREKAS
            qt_eureka_check(1);
# endif /* QTHREAD_USE_EUREKAS */
            SPINLOCK_BODY();
#endif              /* ifdef QTHREAD_CONDWAIT_BLOCKING_QUEUE */
            continue;
        }
        qthread_debug(THREADQUEUE_DETAILS, "q(%p): next_ptr = %p\n", q, next_ptr);
        if (head == tail) {     // tail is falling behind
            (void)qt_cas((void **)&(q->tail),
                         (void *)tail,
                         next_ptr);     // advance tail ptr
        }
        // read value before CAS, otherwise another dequeue might free the next node
        p = next_ptr->value;
        if (qt_cas((void **)&(q->head),
                   (void *)head,
                   next_ptr) == head) {
            break;                     // success!
        }
    }
    qthread_debug(THREADQUEUE_DETAILS, "q(%p): found a thread! p=%p:%i\n", q, p, p->thread_id);
    hazardous_release_node(FREE_TQNODE, head);
    if (p != NULL) {
        (void)qthread_internal_incr_s(&q->advisory_queuelen, &q->advisory_queuelen_m, -1);
    }
    return p;
}                                      /*}}} */

/* walk queue removing all tasks matching this description */
void INTERNAL qt_threadqueue_filter(qt_threadqueue_t       *q,
                                    qt_threadqueue_filter_f f)
{   /*{{{*/
    qt_threadqueue_node_t *curs, **ptr;

    qthread_debug(THREADQUEUE_CALLS, "q(%p), f(%p): began head:%p next:%p tail:%p\n", q, f, q->head, q->head ? q->head->next : NULL, q->tail);

    assert(q != NULL);
    do {
        curs = q->head;
        if (curs == NULL) { return; }
        hazardous_ptr(0, curs);
        COMPILER_FENCE;
    } while (curs != q->head);
    ptr  = &curs->next;
    curs = curs->next;
    hazardous_ptr(1, curs);
    while (curs) {
        qthread_t *t = curs->value;
        switch (f(t)) {
            case IGNORE_AND_CONTINUE: // ignore, move on
                hazardous_ptr(0, curs);
                ptr  = &curs->next;
                curs = curs->next;
                hazardous_ptr(1, curs);
                continue;
            case IGNORE_AND_STOP: // ignore, stop looking
                return;

            case REMOVE_AND_CONTINUE: // remove, move on
            {
                qt_threadqueue_node_t *freeme = curs;

#ifdef QTHREAD_USE_EUREKAS
                qthread_internal_assassinate(t);
#endif /* QTHREAD_USE_EUREKAS */
                if (curs->next == NULL) {
                    /* this is clever: since 'next' is the first field, its
                     * address is the address of the entire structure */
                    q->tail = (qt_threadqueue_node_t *)ptr;
                }
                *ptr = curs->next;
                curs = curs->next;
                hazardous_ptr(1, curs);
                hazardous_release_node(FREE_TQNODE, freeme);
            }
                continue;
            case REMOVE_AND_STOP: // remove, stop looking
#ifdef QTHREAD_USE_EUREKAS
                qthread_internal_assassinate(t);
#endif /* QTHREAD_USE_EUREKAS */
                if (curs->next == NULL) {
                    /* this is clever: since 'next' is the first field, its
                     * address is the address of the entire structure */
                    q->tail = (qt_threadqueue_node_t *)ptr;
                }
                *ptr = curs->next;
                hazardous_release_node(FREE_TQNODE, curs);
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
