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
#include "qthread_innards.h"           /* for qlib (only used in steal_chunksize) */
#include "qt_shepherd_innards.h"
#include "qt_qthread_struct.h"
#include "qthread_asserts.h"
#include "qthread_prefetch.h"
#include "qt_threadqueues.h"

/* Data Structures */
struct _qt_threadqueue_node {
    volatile struct _qt_threadqueue_node *volatile next;
    volatile struct _qt_threadqueue_node *volatile prev;
    qthread_t                                     *value;
    qthread_shepherd_t                            *creator_ptr;
} /* qt_threadqueue_node_t */;

struct _qt_threadqueue {
    volatile qt_threadqueue_node_t *volatile head;
    volatile qt_threadqueue_node_t *volatile tail;
#ifdef QTHREAD_CONDWAIT_BLOCKING_QUEUE
    volatile aligned_t                       fruitless;
    pthread_mutex_t                          lock;
    pthread_cond_t                           notempty;
#endif                          /* CONDWAIT */
    /* the following is for estimating a queue's "busy" level, and is not
     * guaranteed accurate (that would be a race condition) */
    volatile saligned_t advisory_queuelen;
    qthread_shepherd_t *creator_ptr;
    /* used for the work stealing queue implementation */
    QTHREAD_FASTLOCK_TYPE qlock;
    long                  qlength;
    long                  qlength_stealable; /* number of stealable tasks on queue - stop steal attempts
                                              * that will fail because tasks cannot be moved - 4/1/11 AKP
                                              */
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
# define ALLOC_THREADQUEUE(shep) (qt_threadqueue_t *)calloc(1, sizeof(qt_threadqueue_t))
# define FREE_THREADQUEUE(t)     free(t)
static QINLINE void ALLOC_TQNODE(qt_threadqueue_node_t **ret,
                                 qthread_shepherd_t     *shep)
{                                      /*{{{ */
# ifdef HAVE_MEMALIGN
    *ret =
        (qt_threadqueue_node_t *)memalign(16, sizeof(qt_threadqueue_node_t));
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
#else /* if defined(UNPOOLED_QUEUES) || defined(UNPOOLED) */
qt_threadqueue_pools_t generic_threadqueue_pools;
static QINLINE qt_threadqueue_t *ALLOC_THREADQUEUE(qthread_shepherd_t *shep)
{                                      /*{{{ */
    qt_threadqueue_t *tmp = (qt_threadqueue_t *)qt_mpool_alloc(shep
                                                               ? (shep->threadqueue_pools.queues)
                                                               : generic_threadqueue_pools.queues);

    if (tmp != NULL) {
        tmp->creator_ptr = shep;
    }
    return tmp;
}                                      /*}}} */

static QINLINE void FREE_THREADQUEUE(qt_threadqueue_t *t)
{                                      /*{{{ */
    qt_mpool_free(t->creator_ptr ? (t->creator_ptr->threadqueue_pools.queues) :
                  generic_threadqueue_pools.queues, t);
}                                      /*}}} */

static QINLINE void ALLOC_TQNODE(qt_threadqueue_node_t **ret,
                                 qthread_shepherd_t     *shep)
{                                      /*{{{ */
    *ret = (qt_threadqueue_node_t *)qt_mpool_alloc(shep
                                                   ? (shep->threadqueue_pools.nodes)
                                                   : generic_threadqueue_pools.nodes);
    if (*ret != NULL) {
        memset(*ret, 0, sizeof(qt_threadqueue_node_t));
        (*ret)->creator_ptr = shep;
    }
}                                      /*}}} */

static QINLINE void FREE_TQNODE(qt_threadqueue_node_t *t)
{                                      /*{{{ */
    qt_mpool_free(t->creator_ptr
                  ? (t->creator_ptr->threadqueue_pools.nodes)
                  : generic_threadqueue_pools.nodes,
                  t);
}                                      /*}}} */

void INTERNAL qt_threadqueue_init_pools(qt_threadqueue_pools_t *p)
{   /*{{{*/
    p->nodes  = qt_mpool_create_aligned(sizeof(qt_threadqueue_node_t), 16);
    p->queues = qt_mpool_create(sizeof(qt_threadqueue_t));
} /*}}}*/

void INTERNAL qt_threadqueue_destroy_pools(qt_threadqueue_pools_t *p)
{   /*{{{*/
    qt_mpool_destroy(p->nodes);
    qt_mpool_destroy(p->queues);
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
#define QTHREAD_SIGNAL(l)      qassert(pthread_cond_signal(l), 0)
#define QTHREAD_CONDWAIT(c, l) qassert(pthread_cond_wait(c, l), 0)

/*****************************************/
/* functions to manage the thread queues */
/*****************************************/

static QINLINE long qthread_steal_chunksize(void);
static QINLINE void qthread_steal(void);

qt_threadqueue_t INTERNAL *qt_threadqueue_new(qthread_shepherd_t *shepherd)
{   /*{{{*/
    qt_threadqueue_t *q = ALLOC_THREADQUEUE(shepherd);

    if (q != NULL) {
        q->creator_ptr       = shepherd;
        q->head              = q->tail = NULL;
        q->qlength           = 0;
        q->qlength_stealable = 0;
        QTHREAD_FASTLOCK_INIT(q->qlock);
    }
    return q;
} /*}}}*/

void INTERNAL qt_threadqueue_free(qt_threadqueue_t *q)
{   /*{{{*/
    while (q->head != q->tail) {
        qt_threadqueue_dequeue_blocking(q, 1);
    }
    assert(q->head == q->tail);
    QTHREAD_FASTLOCK_DESTROY(q->qlock);
    FREE_THREADQUEUE(q);
} /*}}}*/

/* enqueue at tail */
void INTERNAL qt_threadqueue_enqueue(qt_threadqueue_t   *q,
                                     qthread_t          *t,
                                     qthread_shepherd_t *shep)
{   /*{{{*/
    qt_threadqueue_node_t *node;

    ALLOC_TQNODE(&node, shep);
    assert(node != NULL);

    node->value = t;

    assert(q != NULL);
    assert(t != NULL);

    QTHREAD_FASTLOCK_LOCK(&q->qlock);
    node->next = NULL;
    node->prev = q->tail;
    q->tail    = node;
    if (q->head == NULL) {
        q->head = node;
    } else {
        node->prev->next = node;
    }
    q->qlength++;
    if (!(t->flags & QTHREAD_UNSTEALABLE)) { q->qlength_stealable++; }
    QTHREAD_FASTLOCK_UNLOCK(&q->qlock);
} /*}}}*/

/* yielded threads enqueue at head */
void INTERNAL qt_threadqueue_enqueue_yielded(qt_threadqueue_t   *q,
                                             qthread_t          *t,
                                             qthread_shepherd_t *shep)
{   /*{{{*/
    qt_threadqueue_node_t *node;

    ALLOC_TQNODE(&node, shep);
    assert(node != NULL);

    node->value = t;

    assert(q != NULL);
    assert(t != NULL);

    QTHREAD_FASTLOCK_LOCK(&q->qlock);
    node->prev = NULL;
    node->next = q->head;
    q->head    = node;
    if (q->tail == NULL) {
        q->tail = node;
    } else {
        node->next->prev = node;
    }
    q->qlength++;
    if (!(t->flags & QTHREAD_UNSTEALABLE)) { q->qlength_stealable++; }
    QTHREAD_FASTLOCK_UNLOCK(&q->qlock);
} /*}}}*/

/* dequeue at tail, unlike original qthreads implementation */
qthread_t INTERNAL *qt_threadqueue_dequeue_blocking(qt_threadqueue_t *q,
                                                    size_t            active)
{   /*{{{*/
    volatile qt_threadqueue_node_t *node;
    qthread_t                      *t;

    assert(q != NULL);

    while (1) {
        QTHREAD_FASTLOCK_LOCK(&q->qlock);
        node = q->tail;
        if (node != NULL) {
            q->tail = q->tail->prev;
            if (q->tail == NULL) {
                q->head = NULL;
            } else {
                q->tail->next = NULL;
            }
            q->qlength--;
            if (!(node->value->flags & QTHREAD_UNSTEALABLE)) { q->qlength_stealable--; }
        }
        QTHREAD_FASTLOCK_UNLOCK(&q->qlock);

        if ((node == NULL) && (active)) {
            qthread_steal();
        } else {
            if (node) {                // watch out for inactive node not stealling
                t = node->value;
                FREE_TQNODE((qt_threadqueue_node_t *)node);
                if ((t->flags & QTHREAD_MUST_BE_WORKER_ZERO)) { // only needs to be on worker 0 for termination
                    switch(qthread_worker(NULL)) {
                        case NO_WORKER:
                            *(int *)0 = 0; // should never happen
                        case 0:
                            return (t);

                        default:
                            /* McCoy thread can only run on worker 0 */
                            qt_threadqueue_enqueue_yielded(q, t, t->rdata->shepherd_ptr);
                            break;
                    }
                } else {
                    break;
                }
            }
        }
    }
    return (t);
} /*}}}*/

/* enqueue multiple (from steal) */
void INTERNAL qt_threadqueue_enqueue_multiple(qt_threadqueue_t      *q,
                                              qt_threadqueue_node_t *first,
                                              qthread_shepherd_t    *shep)
{   /*{{{*/
    volatile qt_threadqueue_node_t *last;
    size_t                          addCnt = 1;

    assert(first != NULL);
    assert(q != NULL);

    last                         = first;
    last->value->target_shepherd = shep;        // Defeats default of "sending home" to original shepherd
    while (last->next) {
        last                         = last->next;
        last->value->target_shepherd = shep;    // Defeats default of "sending home" to original shepherd
        addCnt++;
    }

    QTHREAD_FASTLOCK_LOCK(&q->qlock);
    last->next  = NULL;
    first->prev = q->tail;
    q->tail     = last;
    if (q->head == NULL) {
        q->head = first;
    } else {
        first->prev->next = first;
    }
    q->qlength           += addCnt;
    q->qlength_stealable += addCnt;
    QTHREAD_FASTLOCK_UNLOCK(&q->qlock);
} /*}}}*/

/* dequeue stolen threads at head, skip yielded threads */
qt_threadqueue_node_t INTERNAL *qt_threadqueue_dequeue_steal(qt_threadqueue_t *q)
{                                      /*{{{ */
    qt_threadqueue_node_t *node;
    qt_threadqueue_node_t *first     = NULL;
    qt_threadqueue_node_t *last      = NULL;
    long                   amtStolen = 0;

    assert(q != NULL);

    QTHREAD_FASTLOCK_LOCK(&q->qlock);
    while (q->qlength_stealable > 0 && amtStolen < qthread_steal_chunksize()) {
        node = (qt_threadqueue_node_t *)q->head;
        while (node != NULL &&
               (node->value->thread_state == QTHREAD_STATE_YIELDED ||
                node->value->thread_state == QTHREAD_STATE_TERM_SHEP ||
                (node->value->flags & QTHREAD_UNSTEALABLE))
               ) {
            node = (qt_threadqueue_node_t *)node->next;
        }
        if (node != NULL) {
            if (node == q->head) {
                q->head = node->next;
            } else {
                node->prev->next = node->next;
            }
            if (node == q->tail) {
                q->tail = node->prev;
            } else {
                node->next->prev = node->prev;
            }

            q->qlength--;
            q->qlength_stealable--;

            node->prev = node->next = NULL;
            if (first == NULL) {
                first = last = node;
            } else {
                last->next = node;
                node->prev = last;
                last       = node;
            }
            amtStolen++;
        } else {
            break;
        }
    }
    QTHREAD_FASTLOCK_UNLOCK(&q->qlock);
# ifdef STEAL_PROFILE                  // should give mechanism to make steal profiling optional
    qthread_incr(&q->creator_ptr->steal_amount_stolen, amtStolen);
# endif

    return (first);
}                                      /*}}} */

/* Returns the number of tasks to steal per steal operation (chunk size) */
static QINLINE long qthread_steal_chunksize(void)
{   /*{{{*/
    static long chunksize = 0;

    if (chunksize == 0) {
        char *qsc  = getenv("QTHREAD_STEAL_CHUNKSIZE");
        char *qsce = NULL;

        if (qsc) {
            chunksize = strtol(qsc, &qsce, 0);
            if ((qsce == NULL) || (qsce == qsc)) {
                fprintf(stderr, "unparseable steal chunksize (%s)\n", qsc);
                chunksize = 1;
            }
        } else {
            chunksize = qlib->nworkerspershep;
        }
    }

    return chunksize;
} /*}}}*/

/*  Steal work from another shepherd's queue
 *    Returns the amount of work stolen
 */
static QINLINE void qthread_steal(void)
{   /*{{{*/
    int                    i;
    extern pthread_key_t   shepherd_structs;
    qt_threadqueue_node_t *first;
    qthread_shepherd_t    *victim_shepherd;
    qthread_worker_t      *worker =
        (qthread_worker_t *)pthread_getspecific(shepherd_structs);
    qthread_shepherd_t *thief_shepherd =
        (qthread_shepherd_t *)worker->shepherd;

# ifdef STEAL_PROFILE                  // should give mechanism to make steal profiling optional
    qthread_incr(&thief_shepherd->steal_called, 1);
# endif
    if (thief_shepherd->stealing) {
        return;
    } else {
# ifdef QTHREAD_OMP_AFFINITY
        if (thief_shepherd->stealing_mode == QTHREAD_STEAL_ON_ALL_IDLE) {
            for (i = 0; i < qlib->nworkerspershep; i++)
                if (thief_shepherd->workers[i].current != NULL) {
                    return;
                }
            thief_shepherd->stealing_mode = QTHREAD_STEAL_ON_ANY_IDLE;
        }
# endif
        thief_shepherd->stealing = 1;
    }
# ifdef STEAL_PROFILE                  // should give mechanism to make steal profiling optional
    qthread_incr(&thief_shepherd->steal_attempted, 1);
# endif
    for (i = 1; i < qlib->nshepherds; i++) {
        victim_shepherd =
            &qlib->shepherds[(thief_shepherd->shepherd_id + i) %
                             qlib->nshepherds];
        if (0 < victim_shepherd->ready->qlength_stealable) {
            first = qt_threadqueue_dequeue_steal(victim_shepherd->ready);
            if (first) {
                qt_threadqueue_enqueue_multiple(thief_shepherd->ready, first,
                                                thief_shepherd);
# ifdef STEAL_PROFILE                  // should give mechanism to make steal profiling optional
                qthread_incr(&thief_shepherd->steal_successful, 1);
# endif
                break;
            }
# ifdef STEAL_PROFILE                  // should give mechanism to make steal profiling optional
            else {
                qthread_incr(&thief_shepherd->steal_failed, 1);
            }
# endif
        }
        if (0 < thief_shepherd->ready->qlength) {  // work at home quit steal attempt
            break;
        }
    }
    thief_shepherd->stealing = 0;
} /*}}}*/

# ifdef STEAL_PROFILE                  // should give mechanism to make steal profiling optional
void INTERNAL qthread_steal_stat(void)
{
    int i;

    for (i = 0; i < qlib->nshepherds; i++) {
        fprintf(stdout,
                "shepherd %d - steals called %ld attempted %ld failed %ld successful %ld work stolen %ld\n",
                qlib->shepherds[i].shepherd_id,
                qlib->shepherds[i].steal_called,
                qlib->shepherds[i].steal_attempted,
                qlib->shepherds[i].steal_failed,
                qlib->shepherds[i].steal_successful,
                qlib->shepherds[i].steal_amount_stolen);
    }
}

# endif /* ifdef STEAL_PROFILE */

/* walk queue looking for a specific value  -- if found remove it (and start
 * it running)  -- if not return NULL
 */
qt_threadqueue_node_t INTERNAL *qt_threadqueue_dequeue_specific(qt_threadqueue_t *q,
                                                                void             *value)
{
    qt_threadqueue_node_t *node = NULL;
    qthread_t             *t;

    assert(q != NULL);

    QTHREAD_FASTLOCK_LOCK(&q->qlock);
    if (q->qlength > 0) {
        node = (qt_threadqueue_node_t *)q->tail;
        if (node) {
            t = (qthread_t *)node->value;
        }
        while ((node != NULL) && (t->ret != value)) {
            node = (qt_threadqueue_node_t *)node->prev;
            if (node) {
                t = (qthread_t *)node->value;
            }
        }
        if ((node != NULL)) {
            if (node != q->tail) {
                if (node == q->head) {
                    q->head = node->next;       // reset front ptr
                } else {
                    node->prev->next = node->next;
                }
                node->next->prev = node->prev;  // reset back ptr (know we're not tail
                node->next       = NULL;
                node->prev       = q->tail;
                q->tail->next    = node;
                q->tail          = node;
            }
        } else {
            node = NULL;
        }
    }
    QTHREAD_FASTLOCK_UNLOCK(&q->qlock);

    return (node);
}

/* vim:set expandtab: */
