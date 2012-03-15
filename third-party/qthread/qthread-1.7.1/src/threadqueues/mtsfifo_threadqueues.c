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
#include "qt_shepherd_innards.h"
#include "qt_qthread_struct.h"
#include "qthread_asserts.h"
#include "qthread_prefetch.h"
#include "qt_threadqueues.h"
#include "qthread_innards.h" /* for qthread_internal_cleanup_early() */

/* Data Structures */
struct _qt_threadqueue_node {
    struct _qt_threadqueue_node *next;
    qthread_t                   *value;
} /* qt_threadqueue_node_t */;

typedef struct _qt_threadqueue_node qt_threadqueue_node_t;

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

struct _qt_threadqueue_private {} /* qt_threadqueue_private_t */;

/* Memory Management */
#if defined(UNPOOLED_QUEUES) || defined(UNPOOLED)
# define ALLOC_THREADQUEUE() (qt_threadqueue_t *)calloc(1, sizeof(qt_threadqueue_t))
# define FREE_THREADQUEUE(t) free(t)
static QINLINE void ALLOC_TQNODE(qt_threadqueue_node_t **ret)
{                                      /*{{{ */
# ifdef HAVE_MEMALIGN
    *ret = (qt_threadqueue_node_t *)memalign(16, sizeof(qt_threadqueue_node_t));
# elif defined(HAVE_POSIX_MEMALIGN)
    qassert(posix_memalign((void **)ret, 16, sizeof(qt_threadqueue_node_t)),
            0);
# else
    *ret = calloc(1, sizeof(qt_threadqueue_node_t));
    return;
# endif
    memset(*ret, 0, sizeof(qt_threadqueue_node_t));
}                                      /*}}} */

# define FREE_TQNODE(t) free(t)
void INTERNAL qt_threadqueue_subsystem_init(void) {}
#else /* if defined(UNPOOLED_QUEUES) || defined(UNPOOLED) */
qt_threadqueue_pools_t generic_threadqueue_pools;
# define ALLOC_THREADQUEUE() (qt_threadqueue_t *)qt_mpool_cached_alloc(generic_threadqueue_pools.queues)
# define FREE_THREADQUEUE(t) qt_mpool_cached_free(generic_threadqueue_pools.queues, t)

static QINLINE void ALLOC_TQNODE(qt_threadqueue_node_t **ret)
{                                      /*{{{ */
    *ret = (qt_threadqueue_node_t *)qt_mpool_cached_alloc(generic_threadqueue_pools.nodes);
    if (*ret != NULL) {
        memset(*ret, 0, sizeof(qt_threadqueue_node_t));
    }
}                                      /*}}} */

# define FREE_TQNODE(t) qt_mpool_cached_free(generic_threadqueue_pools.nodes, t)

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

#define QTHREAD_INITLOCK(l) do { if (pthread_mutex_init(l, NULL) != 0) { return QTHREAD_PTHREAD_ERROR; } } while(0)
#define QTHREAD_LOCK(l)     qassert(pthread_mutex_lock(l), 0)
#define QTHREAD_UNLOCK(l)   qassert(pthread_mutex_unlock(l), 0)
// #define QTHREAD_DESTROYLOCK(l) do { int __ret__ = pthread_mutex_destroy(l); if (__ret__ != 0) fprintf(stderr, "pthread_mutex_destroy(%p) returned %i (%s)\n", l, __ret__, strerror(__ret__)); assert(__ret__ == 0); } while (0)
#define QTHREAD_DESTROYLOCK(l) qassert(pthread_mutex_destroy(l), 0)
#define QTHREAD_DESTROYCOND(l) qassert(pthread_cond_destroy(l), 0)
#define QTHREAD_SIGNAL(l)      qassert(pthread_cond_signal(l), 0)
#define QTHREAD_CONDWAIT(c, l) qassert(pthread_cond_wait(c, l), 0)

/*****************************************/
/* functions to manage the thread queues */
/*****************************************/

// This lock-free algorithm borrowed from
// http://www.research.ibm.com/people/m/michael/podc-1996.pdf

/* to avoid ABA reinsertion trouble, each pointer in the queue needs to have a
 * monotonically increasing counter associated with it. The counter doesn't
 * need to be huge, just big enough to avoid trouble. We'll
 * just claim 4, to be conservative. Thus, a qt_threadqueue_node_t must be at least
 * 16 bytes. */
/* Technically, ABA protection is only necessary if there are more than one
 * dequeuer AND if you aren't using hazard pointers. However, this queue is
 * only used in single dequeuer situations. */
#define NO_ABA_PROTECTION 1 /* only necessary for multiple-dequeuers */
#if defined(QTHREAD_USE_VALGRIND) || NO_ABA_PROTECTION
# define QPTR(x)        (x)
# define QCTR(x)        0
# define QCOMPOSE(x, y) (x)
#else
# define QCTR_MASK (15)
# define QPTR(x)        ((qt_threadqueue_node_t *)(((uintptr_t)(x)) & ~(uintptr_t)QCTR_MASK))
# define QCTR(x)        (((uintptr_t)(x)) &QCTR_MASK)
# define QCOMPOSE(x, y) (void *)(((uintptr_t)QPTR(x)) | ((QCTR(y) + 1) &QCTR_MASK))
#endif

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
        assert(QPTR(q->head) != NULL);
        if (QPTR(q->head) == NULL) {   // if we're not using asserts, fail nicely
#ifdef QTHREAD_CONDWAIT_BLOCKING_QUEUE
            QTHREAD_DESTROYLOCK(&q->lock);
            QTHREAD_DESTROYCOND(&q->notempty);
#endif
            FREE_THREADQUEUE(q);
            q = NULL;
        }
        q->tail             = q->head;
        QPTR(q->tail)->next = NULL;
    }
    return q;
}                                      /*}}} */

void INTERNAL qt_threadqueue_free(qt_threadqueue_t *q)
{                                      /*{{{ */
    while (QPTR(q->head) != QPTR(q->tail)) {
        qt_threadqueue_dequeue(q);
    }
    assert(QPTR(q->head) == QPTR(q->tail));
#ifdef QTHREAD_CONDWAIT_BLOCKING_QUEUE
    QTHREAD_DESTROYLOCK(&q->lock);
    QTHREAD_DESTROYCOND(&q->notempty);
#endif
    FREE_TQNODE((qt_threadqueue_node_t *)QPTR(q->head));
    FREE_THREADQUEUE(q);
}                                      /*}}} */

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
{                                      /*{{{ */
    qt_threadqueue_node_t *tail;
    qt_threadqueue_node_t *next;
    qt_threadqueue_node_t *node;

    assert(t != NULL);
    assert(q != NULL);

    ALLOC_TQNODE(&node);
    assert(node != NULL);
    assert(QCTR(node) == 0);           // node MUST be aligned

    node->value = t;
    // set to null without disturbing the ctr
    node->next = (qt_threadqueue_node_t *)(uintptr_t)QCTR(node->next);

    while (1) {
        tail = q->tail;
        next = QPTR(tail)->next;
        COMPILER_FENCE;
        if (tail == q->tail) {        // are tail and next consistent?
            if (QPTR(next) == NULL) { // was tail pointing to the last node?
                if (qt_cas((void **)&(QPTR(tail)->next),
                           (void *)next,
                           QCOMPOSE(node, next)) == next) {
                    break;             // success!
                }
            } else {                   // tail not pointing to last node
                (void)qt_cas((void **)&(q->tail),
                             (void *)tail,
                             QCOMPOSE(next, tail));
            }
        }
    }
    (void)qt_cas((void **)&(q->tail),
                 (void *)tail,
                 QCOMPOSE(node, tail));
    (void)qthread_incr(&q->advisory_queuelen, 1);
#ifdef QTHREAD_CONDWAIT_BLOCKING_QUEUE
    if (q->fruitless) {
        QTHREAD_LOCK(&q->lock);
        if (q->fruitless) {
            q->fruitless = 0;
            QTHREAD_SIGNAL(&q->notempty);
        }
        QTHREAD_UNLOCK(&q->lock);
    }
#endif
}                                      /*}}} */

void qt_threadqueue_enqueue_yielded(qt_threadqueue_t *restrict q,
                                    qthread_t *restrict        t)
{   /*{{{*/
    qt_threadqueue_enqueue(q, t);
} /*}}}*/

qthread_t INTERNAL *qt_threadqueue_dequeue(qt_threadqueue_t *q)
{                                      /*{{{ */
    qthread_t *p = NULL;

    qt_threadqueue_node_t *head;
    qt_threadqueue_node_t *tail;
    qt_threadqueue_node_t *next_ptr;

    assert(q != NULL);
    while (1) {
        head     = q->head;
        tail     = q->tail;
        next_ptr = QPTR(QPTR(head)->next);
        COMPILER_FENCE;
        if (head == q->head) {              // are head, tail, and next consistent?
            if (QPTR(head) == QPTR(tail)) { // is queue empty or tail falling behind?
                if (next_ptr == NULL) {     // is queue empty?
                    return NULL;
                }
                (void)qt_cas((void **)&(q->tail),
                             (void *)tail,
                             QCOMPOSE(next_ptr, tail)); // advance tail ptr
            } else {                                    // no need to deal with tail
                // read value before CAS, otherwise another dequeue might free the next node
                p = next_ptr->value;
                if (qt_cas((void **)&(q->head),
                           (void *)head,
                           QCOMPOSE(next_ptr, head)) == head) {
                    break;             // success!
                }
            }
        }
    }
    FREE_TQNODE((qt_threadqueue_node_t *)QPTR(head));
    if (p != NULL) {
        Q_PREFETCH(&(p->thread_state));
        (void)qthread_internal_incr_s(&q->advisory_queuelen, &q->advisory_queuelen_m, -1);
    }
    return p;
}                                      /*}}} */

/* this function is amusing, but the point is to avoid unnecessary bus traffic
 * by allowing idle shepherds to sit for a while while still allowing for
 * low-overhead for busy shepherds. This is a hybrid approach: normally, it
 * functions as a spinlock, but if it spins too much, it waits for a signal */
qthread_t INTERNAL *qt_threadqueue_dequeue_blocking(qt_threadqueue_t         *q,
                                                    qt_threadqueue_private_t *QUNUSED(qc),
                                                    uint_fast8_t              QUNUSED(active))
{                                      /*{{{ */
    qthread_t *p = NULL;

    qt_threadqueue_node_t *head;
    qt_threadqueue_node_t *tail;
    qt_threadqueue_node_t *next_ptr;

    assert(q != NULL);
threadqueue_dequeue_restart:
    while (1) {
        head     = q->head;
        tail     = q->tail;
        next_ptr = QPTR(QPTR(head)->next);
        COMPILER_FENCE;
        if (head == q->head) {              // are head, tail, and next consistent?
            if (QPTR(head) == QPTR(tail)) { // is queue empty or tail falling behind?
                if (next_ptr == NULL) {     // is queue empty?
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
#endif              /* ifdef QTHREAD_CONDWAIT_BLOCKING_QUEUE */
                    goto threadqueue_dequeue_restart;
                }
                (void)qt_cas((void **)&(q->tail),
                             (void *)tail,
                             QCOMPOSE(next_ptr, tail)); // advance tail ptr
            } else {                                    // no need to deal with tail
                // read value before CAS, otherwise another dequeue might free the next node
                p = next_ptr->value;
                if (qt_cas((void **)&(q->head),
                           (void *)head,
                           QCOMPOSE(next_ptr, head)) == head) {
                    break;             // success!
                }
            }
        }
    }
    FREE_TQNODE((qt_threadqueue_node_t *)QPTR(head));
    if (p != NULL) {
        (void)qthread_internal_incr_s(&q->advisory_queuelen, &q->advisory_queuelen_m, -1);
    }
    return p;
}                                      /*}}} */

/* vim:set expandtab: */
