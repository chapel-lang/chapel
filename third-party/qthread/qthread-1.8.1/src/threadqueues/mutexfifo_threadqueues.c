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
#include "qthread_innards.h"           /* for qlib (only used in steal_chunksize) */
#include "qt_shepherd_innards.h"
#include "qt_qthread_struct.h"
#include "qthread_asserts.h"
#include "qthread_prefetch.h"
#include "qt_threadqueues.h"
#include "qt_debug.h"

/* Data Structures */
struct _qt_threadqueue_node {
    struct _qt_threadqueue_node *next;
    qthread_t                   *value;
} /* qt_threadqueue_node_t */;

struct _qt_threadqueue {
    qt_threadqueue_node_t *head;
    qt_threadqueue_node_t *tail;
    QTHREAD_FASTLOCK_TYPE  head_lock;
    QTHREAD_FASTLOCK_TYPE  tail_lock;
    QTHREAD_FASTLOCK_TYPE  advisory_queuelen_m;
    /* the following is for estimating a queue's "busy" level, and is not
     * guaranteed accurate (that would be a race condition) */
    saligned_t advisory_queuelen;
} /* qt_threadqueue_t */;

#if defined(AKP_DEBUG) && AKP_DEBUG
/* function added to ease debugging and tuning around queue critical sections - 4/1/11 AKP */

void qt_spin_exclusive_lock(qt_spin_exclusive_t *l)
{
    uint64_t val = qthread_incr(&l->enter, 1);

    while (val != l->exit) {} // spin waiting for my turn
}

void qt_spin_exclusive_unlock(qt_spin_exclusive_t *l)
{
    qthread_incr(&l->exit, 1); // allow next guy's turn
}

/* end of added functions - AKP */
#endif /* if AKP_DEBUG */

/* Memory Management */
#if defined(UNPOOLED_QUEUES) || defined(UNPOOLED)
# define ALLOC_THREADQUEUE() (qt_threadqueue_t *)MALLOC(sizeof(qt_threadqueue_t))
# define FREE_THREADQUEUE(t) FREE(t, sizeof(qt_threadqueue_t))
# define ALLOC_TQNODE()      (qt_threadqueue_node_t *)MALLOC(sizeof(qt_threadqueue_node_t))
# define FREE_TQNODE(t)      FREE(t, sizeof(qt_threadqueue_node_t))
void INTERNAL qt_threadqueue_subsystem_init(void) {}
#else /* if defined(UNPOOLED_QUEUES) || defined(UNPOOLED) */
qt_threadqueue_pools_t generic_threadqueue_pools;
# define ALLOC_THREADQUEUE() (qt_threadqueue_t *)qt_mpool_alloc(generic_threadqueue_pools.queues)
# define FREE_THREADQUEUE(t) qt_mpool_free(generic_threadqueue_pools.queues, t)
# define ALLOC_TQNODE()      (qt_threadqueue_node_t *)qt_mpool_alloc(generic_threadqueue_pools.nodes)
# define FREE_TQNODE(t)      qt_mpool_free(generic_threadqueue_pools.nodes, t)

static void qt_threadqueue_subsystem_shutdown(void)
{
    qt_mpool_destroy(generic_threadqueue_pools.nodes);
    qt_mpool_destroy(generic_threadqueue_pools.queues);
}

void INTERNAL qt_threadqueue_subsystem_init(void)
{
    generic_threadqueue_pools.nodes  = qt_mpool_create(sizeof(qt_threadqueue_node_t));
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

qt_threadqueue_t INTERNAL *qt_threadqueue_new(void)
{                                      /*{{{ */
    qt_threadqueue_t *q = ALLOC_THREADQUEUE();

    if (q != NULL) {
        QTHREAD_FASTLOCK_INIT(q->head_lock);
        QTHREAD_FASTLOCK_INIT(q->tail_lock);
        QTHREAD_FASTLOCK_INIT(q->advisory_queuelen_m);
        q->head = ALLOC_TQNODE();
        assert(q->head != NULL);
        if (q->head == NULL) {
            QTHREAD_FASTLOCK_DESTROY(q->advisory_queuelen_m);
            QTHREAD_FASTLOCK_DESTROY(q->head_lock);
            QTHREAD_FASTLOCK_DESTROY(q->tail_lock);
            FREE_THREADQUEUE(q);
            q = NULL;
        } else {
            q->tail        = q->head;
            q->head->next  = NULL;
            q->head->value = NULL;
        }
    }
    return q;
}                                      /*}}} */

static qthread_t *qt_threadqueue_dequeue(qt_threadqueue_t *q)
{                                      /*{{{ */
    qthread_t *p = NULL;

    qt_threadqueue_node_t *node, *new_head;

    assert(q != NULL);
    QTHREAD_FASTLOCK_LOCK(&q->head_lock);
    {
        node     = q->head;
        new_head = node->next;
        if (new_head != NULL) {
            p       = new_head->value;
            q->head = new_head;
        }
    }
    QTHREAD_FASTLOCK_UNLOCK(&q->head_lock);
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
    QTHREAD_FASTLOCK_DESTROY(q->head_lock);
    QTHREAD_FASTLOCK_DESTROY(q->tail_lock);
    QTHREAD_FASTLOCK_DESTROY(q->advisory_queuelen_m);
    FREE_TQNODE((qt_threadqueue_node_t *)(q->head));
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
    qt_threadqueue_node_t *node;

    node = ALLOC_TQNODE();
    assert(node != NULL);
    node->value = t;
    node->next  = NULL;
    QTHREAD_FASTLOCK_LOCK(&q->tail_lock);
    {
        q->tail->next = node;
        q->tail       = node;
    }
    QTHREAD_FASTLOCK_UNLOCK(&q->tail_lock);
}                                      /*}}} */

void qt_threadqueue_enqueue_yielded(qt_threadqueue_t *restrict q,
                                    qthread_t *restrict        t)
{   /*{{{*/
    qt_threadqueue_enqueue(q, t);
} /*}}}*/

/* this function is amusing, but the point is to avoid unnecessary bus traffic
 * by allowing idle shepherds to sit for a while while still allowing for
 * low-overhead for busy shepherds. This is a hybrid approach: normally, it
 * functions as a spinlock, but if it spins too much, it waits for a signal */
qthread_t INTERNAL *qt_scheduler_get_thread(qt_threadqueue_t         *q,
                                            qt_threadqueue_private_t *QUNUSED(qc),
                                            uint_fast8_t              QUNUSED(active))
{                                      /*{{{ */
    qthread_t *p = NULL;

    while ((p = qt_threadqueue_dequeue(q)) == NULL) {}
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
