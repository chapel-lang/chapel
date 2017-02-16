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
#include "qt_shepherd_innards.h"
#include "qt_qthread_struct.h"
#include "qt_asserts.h"
#include "qt_prefetch.h"
#include "qt_threadqueues.h"
#include "qt_debug.h"
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
    QTHREAD_FASTLOCK_TYPE  head_lock;
    QTHREAD_FASTLOCK_TYPE  tail_lock;
    QTHREAD_FASTLOCK_TYPE  advisory_queuelen_m;
    /* the following is for estimating a queue's "busy" level, and is not
     * guaranteed accurate (that would be a race condition) */
    saligned_t advisory_queuelen;
} /* qt_threadqueue_t */;

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
{   /*{{{*/
    qt_mpool_destroy(generic_threadqueue_pools.nodes);
    qt_mpool_destroy(generic_threadqueue_pools.queues);
} /*}}}*/

void INTERNAL qt_threadqueue_subsystem_init(void)
{   /*{{{*/
    generic_threadqueue_pools.nodes  = qt_mpool_create(sizeof(qt_threadqueue_node_t));
    generic_threadqueue_pools.queues = qt_mpool_create(sizeof(qt_threadqueue_t));
    qthread_internal_cleanup(qt_threadqueue_subsystem_shutdown);
} /*}}}*/
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
        q->advisory_queuelen = 0;
        q->head              = ALLOC_TQNODE();
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
    QTHREAD_FASTLOCK_DESTROY(q->head_lock);
    QTHREAD_FASTLOCK_DESTROY(q->tail_lock);
    QTHREAD_FASTLOCK_DESTROY(q->advisory_queuelen_m);
    FREE_TQNODE((qt_threadqueue_node_t *)(q->head));
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
    (void)qthread_internal_incr_s(&q->advisory_queuelen, &q->advisory_queuelen_m, 1);
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

#ifdef QTHREAD_USE_EUREKAS
    qt_eureka_disable();
#endif /* QTHREAD_USE_EUREKAS */
    while ((p = qt_threadqueue_dequeue(q)) == NULL) {
#ifdef QTHREAD_USE_EUREKAS
        qt_eureka_check(1);
#endif /* QTHREAD_USE_EUREKAS */
        SPINLOCK_BODY();
    }
    return p;
}                                      /*}}} */

/* walk queue removing all tasks matching this description */
void INTERNAL qt_threadqueue_filter(qt_threadqueue_t       *q,
                                    qt_threadqueue_filter_f f)
{   /*{{{*/
    QTHREAD_FASTLOCK_LOCK(&q->head_lock);
    {
        qt_threadqueue_node_t  *curs = q->head->next;
        qt_threadqueue_node_t **ptr  = &q->head->next;

        while (curs) {
            qthread_t *t = curs->value;
            switch(f(t)) {
                case IGNORE_AND_CONTINUE: // ignore, move on
                    ptr  = &curs->next;
                    curs = curs->next;
                    break;
                case IGNORE_AND_STOP: // ignore, stop looking
                    curs = NULL;
                    continue;
                case REMOVE_AND_CONTINUE: // remove, move on
                {
                    qt_threadqueue_node_t *tmp = curs;
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
                    FREE_TQNODE(tmp);
                    break;
                }
                case REMOVE_AND_STOP: // remove, stop looking
                {
#ifdef QTHREAD_USE_EUREKAS
                    qthread_internal_assassinate(t);
#endif /* QTHREAD_USE_EUREKAS */
                    if (curs->next == NULL) {
                        /* this is clever: since 'next' is the first field, its
                         * address is the address of the entire structure */
                        q->tail = (qt_threadqueue_node_t *)ptr;
                    }
                    *ptr = curs->next;
                    FREE_TQNODE(curs);
                    curs = NULL;
                    continue;
                }
            }
        }
    }
    QTHREAD_FASTLOCK_UNLOCK(&q->head_lock);
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
