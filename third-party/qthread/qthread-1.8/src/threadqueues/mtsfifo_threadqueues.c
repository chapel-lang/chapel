#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

/* System Headers */
#ifdef QTHREAD_CONDWAIT_BLOCKING_QUEUE
# include <pthread.h>
#endif
#include <stdlib.h>

/* Internal Headers */
#include "qthread/qthread.h"
#include "qt_macros.h"
#include "qt_visibility.h"
#include "qt_atomics.h"
#include "qt_shepherd_innards.h"
#include "qt_qthread_struct.h"
#include "qthread_asserts.h"
#include "qthread_prefetch.h"
#include "qt_threadqueues.h"
#include "qthread_innards.h" /* for qthread_internal_cleanup_early() */
#include "qt_debug.h"
#if defined(UNPOOLED_QUEUES) || defined(UNPOOLED)
# include "qt_aligned_alloc.h"
#endif

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
    pthread_mutex_t        lock;
    pthread_cond_t         notempty;
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
    *ret = (qt_threadqueue_node_t *)qthread_internal_aligned_alloc(sizeof(qt_threadqueue_node_t), 16);
    if (*ret != NULL) {
        memset(*ret, 0, sizeof(qt_threadqueue_node_t));
    }
}                                      /*}}} */

static void FREE_TQNODE(void *p)
{
    FREE_SCRIBBLE(p, sizeof(qt_threadqueue_node_t));
    qthread_internal_aligned_free(p, 16);
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
        if (pthread_mutex_init(&q->lock, NULL) != 0) {
            FREE_THREADQUEUE(q);
            return NULL;
        }
        if (pthread_cond_init(&q->notempty, NULL) != 0) {
            QTHREAD_DESTROYLOCK(&q->lock);
            FREE_THREADQUEUE(q);
            return NULL;
        }
        q->fruitless = 0;
#endif   /* ifdef QTHREAD_CONDWAIT_BLOCKING_QUEUE */
        ALLOC_TQNODE(((qt_threadqueue_node_t **)&(q->head)));
        assert(q->head != NULL);
        if (q->head == NULL) {   // if we're not using asserts, fail nicely
#ifdef QTHREAD_CONDWAIT_BLOCKING_QUEUE
            QTHREAD_DESTROYLOCK(&q->lock);
            QTHREAD_DESTROYCOND(&q->notempty);
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
        Q_PREFETCH(&(p->thread_state));
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
    QTHREAD_DESTROYLOCK(&q->lock);
    QTHREAD_DESTROYCOND(&q->notempty);
#endif
    FREE_TQNODE((qt_threadqueue_node_t *)q->head);
    FREE_THREADQUEUE(q);
}                                      /*}}} */

#ifdef QTHREAD_USE_SPAWNCACHE
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

#endif /* ifdef QTHREAD_USE_SPAWNCACHE */

void INTERNAL qt_threadqueue_enqueue(qt_threadqueue_t *restrict q,
                                     qthread_t *restrict        t)
{                                      /*{{{ */
    qt_threadqueue_node_t *tail;
    qt_threadqueue_node_t *next;
    qt_threadqueue_node_t *node;

    assert(t != NULL);
    assert(q != NULL);

    ALLOC_TQNODE(&node);
    assert(node != NULL);

    node->value = t;
    node->next  = NULL;

    while (1) {
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

    (void)qthread_incr(&q->advisory_queuelen, 1);
#ifdef QTHREAD_CONDWAIT_BLOCKING_QUEUE
    if (q->fruitless) {
        QTHREAD_LOCK(&q->lock);
        if (q->fruitless) {
            q->fruitless = 0;
            QTHREAD_BCAST(&q->notempty);
        }
        QTHREAD_UNLOCK(&q->lock);
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
                QTHREAD_LOCK(&q->lock);
                while (q->fruitless > 1000) {
                    QTHREAD_CONDWAIT(&q->notempty, &q->lock);
                }
                QTHREAD_UNLOCK(&q->lock);
            } else {
# ifdef HAVE_PTHREAD_YIELD
                pthread_yield();
# elif HAVE_SHED_YIELD
                sched_yield();
# endif
            }
#else       /* ifdef QTHREAD_CONDWAIT_BLOCKING_QUEUE */
            SPINLOCK_BODY();
#endif              /* ifdef QTHREAD_CONDWAIT_BLOCKING_QUEUE */
            continue;
        }
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
    hazardous_release_node(FREE_TQNODE, head);
    if (p != NULL) {
        (void)qthread_internal_incr_s(&q->advisory_queuelen, &q->advisory_queuelen_m, -1);
    }
    return p;
}                                      /*}}} */

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
