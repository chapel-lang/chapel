#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

/* System Headers */
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

/* Public Headers */
#include "qthread/qthread.h"
#include "qthread/cacheline.h"

/* Internal Headers */
#include "qt_visibility.h"
#include "qthread_innards.h"           /* for qlib (only used in steal_chunksize) */
#include "qt_shepherd_innards.h"
#include "qt_qthread_struct.h"
#include "qthread_asserts.h"
#include "qthread_prefetch.h"
#include "qt_threadqueues.h"
#include "qt_envariables.h"
#include "qt_debug.h"

/* Data Structures */
struct _qt_threadqueue_node {
    struct _qt_threadqueue_node *next;
    struct _qt_threadqueue_node *prev;
    uintptr_t                    stealable;
    qthread_t                   *value;
} /* qt_threadqueue_node_t */;

struct _qt_threadqueue {
    qt_threadqueue_node_t *head;
    qt_threadqueue_node_t *tail;
    long                   qlength;
    long                   qlength_stealable;                   /* number of stealable tasks on queue - stop steal attempts
                                                                 * that will fail because tasks cannot be moved - 4/1/11 AKP
                                                                 */
#ifdef STEAL_PROFILE
    aligned_t steal_amount_stolen;
    aligned_t steal_called;
    aligned_t steal_attempted;
    aligned_t steal_failed;
    aligned_t steal_successful;
#endif

    QTHREAD_TRYLOCK_TYPE qlock;
} /* qt_threadqueue_t */;

static aligned_t steal_disable   = 0;
static long      steal_chunksize = 0;

#ifdef STEAL_PROFILE
# define STEAL_CALLED(shep)     qthread_incr( & ((shep)->steal_called), 1)
# define STEAL_ATTEMPTED(shep)  qthread_incr( & ((shep)->steal_attempted), 1)
# define STEAL_SUCCESSFUL(shep) qthread_incr( & ((shep)->steal_successful), 1)
# define STEAL_FAILED(shep)     qthread_incr( & ((shep)->steal_failed), 1)
#else
# define STEAL_CALLED(shep)     do {} while(0)
# define STEAL_ATTEMPTED(shep)  do {} while(0)
# define STEAL_SUCCESSFUL(shep) do {} while(0)
# define STEAL_FAILED(shep)     do {} while(0)
#endif

// Forward declarations
qt_threadqueue_node_t INTERNAL *qt_threadqueue_dequeue_steal(qt_threadqueue_t *h,
                                                             qt_threadqueue_t *v);

void INTERNAL qt_threadqueue_enqueue_multiple(qt_threadqueue_t      *q,
                                              qt_threadqueue_node_t *first);

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

#ifdef QTHREAD_PARANOIA
static inline void sanity_check_queue(qt_threadqueue_t *q)
{
    qt_threadqueue_node_t *cursor          = q->head;
    size_t                 count_stealable = 0, count_total = 0;

    assert((q->head == NULL) || q->qlength);
    assert(q->qlength_stealable <= q->qlength);
    assert((q->head && q->tail) || (!q->head && !q->tail));

    while (cursor) {
        count_total++;
        count_stealable += cursor->stealable;
        cursor           = cursor->next;
    }
    assert(count_total == q->qlength);
    assert(count_stealable == q->qlength_stealable);
}

# define PARANOIA_ONLY(x) x
#else /* ifndef QTHREAD_NO_ASSERTS */
# define PARANOIA_ONLY(x)
#endif /* ifndef QTHREAD_NO_ASSERTS */

/* Memory Management */
#if defined(UNPOOLED_QUEUES) || defined(UNPOOLED)
# define ALLOC_THREADQUEUE() (qt_threadqueue_t *)MALLOC(sizeof(qt_threadqueue_t))
# define FREE_THREADQUEUE(t) FREE(t, sizeof(qt_threadqueue_t))
# define ALLOC_TQNODE()      (qt_threadqueue_node_t *)MALLOC(sizeof(qt_threadqueue_node_t))
# define FREE_TQNODE(t)      FREE(t, sizeof(qt_threadqueue_node_t))
void INTERNAL qt_threadqueue_subsystem_init(void)
{
    steal_chunksize = qt_internal_get_env_num("STEAL_CHUNK", 0, 0);
}

#else /* if defined(UNPOOLED_QUEUES) || defined(UNPOOLED) */
qt_threadqueue_pools_t generic_threadqueue_pools;
# define ALLOC_THREADQUEUE() (qt_threadqueue_t *)qt_mpool_alloc(generic_threadqueue_pools.queues)
# define FREE_THREADQUEUE(t) qt_mpool_free(generic_threadqueue_pools.queues, t)

static QINLINE qt_threadqueue_node_t *ALLOC_TQNODE(void)
{                                      /*{{{ */
    return (qt_threadqueue_node_t *)qt_mpool_alloc(generic_threadqueue_pools.nodes);
}                                      /*}}} */

# define FREE_TQNODE(t) qt_mpool_free(generic_threadqueue_pools.nodes, t)

static void qt_threadqueue_subsystem_shutdown(void)
{   /*{{{*/
    qt_mpool_destroy(generic_threadqueue_pools.nodes);
    qt_mpool_destroy(generic_threadqueue_pools.queues);
} /*}}}*/

void INTERNAL qt_threadqueue_subsystem_init(void)
{   /*{{{*/
    generic_threadqueue_pools.queues = qt_mpool_create_aligned(sizeof(qt_threadqueue_t),
                                                               qthread_cacheline());
    generic_threadqueue_pools.nodes = qt_mpool_create_aligned(sizeof(qt_threadqueue_node_t),
                                                              qthread_cacheline());
    steal_chunksize = qt_internal_get_env_num("STEAL_CHUNK", 0, 0);
    qthread_internal_cleanup(qt_threadqueue_subsystem_shutdown);
} /*}}}*/

#endif /* if defined(UNPOOLED_QUEUES) || defined(UNPOOLED) */

ssize_t INTERNAL qt_threadqueue_advisory_queuelen(qt_threadqueue_t *q)
{   /*{{{*/
#if ((QTHREAD_ASSEMBLY_ARCH == QTHREAD_AMD64) ||    \
    (QTHREAD_ASSEMBLY_ARCH == QTHREAD_IA64) ||      \
    (QTHREAD_ASSEMBLY_ARCH == QTHREAD_POWERPC64) || \
    (QTHREAD_ASSEMBLY_ARCH == QTHREAD_SPARCV9_64))
    /* only works if a basic load is atomic */
    return q->qlength;

#else
    ssize_t tmp;
    QTHREAD_TRYLOCK_LOCK(&q->qlock);
    PARANOIA_ONLY(sanity_check_queue(q));
    tmp = q->qlength;
    QTHREAD_TRYLOCK_UNLOCK(&q->qlock);
    return tmp;
#endif /* if ((QTHREAD_ASSEMBLY_ARCH == QTHREAD_AMD64) || (QTHREAD_ASSEMBLY_ARCH == QTHREAD_IA64) || (QTHREAD_ASSEMBLY_ARCH == QTHREAD_POWERPC64) || (QTHREAD_ASSEMBLY_ARCH == QTHREAD_SPARCV9_64)) */
} /*}}}*/

/*****************************************/
/* functions to manage the thread queues */
/*****************************************/

static QINLINE qt_threadqueue_node_t *qthread_steal(qthread_shepherd_t *thief_shepherd);

qt_threadqueue_t INTERNAL *qt_threadqueue_new(void)
{   /*{{{*/
    qt_threadqueue_t *q = ALLOC_THREADQUEUE();

    if (q != NULL) {
        q->head              = NULL;
        q->tail              = NULL;
        q->qlength           = 0;
        q->qlength_stealable = 0;
        QTHREAD_TRYLOCK_INIT(q->qlock);
    }

    return q;
} /*}}}*/

#if defined(UNPOOLED_QTHREAD_T) || defined(UNPOOLED)
# define ALLOC_QTHREAD() MALLOC(sizeof(qthread_t) + qlib->qthread_argcopy_size + qlib->qthread_tasklocal_size)
# define FREE_QTHREAD(t) FREE(t, sizeof(qthread_t) + qlib->qthread_argcopy_size + qlib->qthread_tasklocal_size)
#else /* if defined(UNPOOLED_QTHREAD_T) ||./src/threadqueues/nemesis_threadqueues.c defined(UNPOOLED) */
extern qt_mpool generic_qthread_pool;
# define ALLOC_QTHREAD() (qthread_t *)qt_mpool_alloc(generic_qthread_pool)
# define FREE_QTHREAD(t) qt_mpool_free(generic_qthread_pool, t)
#endif /* if defined(UNPOOLED_QTHREAD_T) || defined(UNPOOLED) */

void INTERNAL qt_threadqueue_free(qt_threadqueue_t *q)
{   /*{{{*/
    if (q->head != q->tail) {
        qthread_t *t;
        QTHREAD_TRYLOCK_LOCK(&q->qlock);
        PARANOIA_ONLY(sanity_check_queue(q));
        while (q->head != q->tail) {
            qt_threadqueue_node_t *node = q->tail;
            if (node != NULL) {
                q->tail = node->prev;
                if (q->tail == NULL) {
                    q->head = NULL;
                } else {
                    q->tail->next = NULL;
                }
                t = node->value;
                FREE_TQNODE(node);
                FREE_QTHREAD(t);
            }
        }
        assert(q->head == NULL);
        assert(q->tail == NULL);
        q->qlength           = 0;
        q->qlength_stealable = 0;
        QTHREAD_TRYLOCK_UNLOCK(&q->qlock);
    }
    assert(q->head == q->tail);
    QTHREAD_TRYLOCK_DESTROY(q->qlock);
    FREE_THREADQUEUE(q);
} /*}}}*/

static QINLINE int qt_threadqueue_isstealable(qthread_t *t)
{   /*{{{*/
    return ((t->flags & QTHREAD_UNSTEALABLE) == 0) ? 1 : 0;
} /*}}}*/

/* enqueue at tail */
void INTERNAL qt_threadqueue_enqueue(qt_threadqueue_t *restrict q,
                                     qthread_t *restrict        t)
{   /*{{{*/
    qt_threadqueue_node_t *node;

    node = ALLOC_TQNODE();
    assert(node != NULL);

    node->value     = t;
    node->stealable = qt_threadqueue_isstealable(t);

    assert(q != NULL);
    assert(t != NULL);

    QTHREAD_TRYLOCK_LOCK(&q->qlock);
    PARANOIA_ONLY(sanity_check_queue(q));
    node->next = NULL;
    node->prev = q->tail;
    q->tail    = node;
    if (q->head == NULL) {
        q->head = node;
    } else {
        node->prev->next = node;
    }
    q->qlength++;
    q->qlength_stealable += node->stealable;
    QTHREAD_TRYLOCK_UNLOCK(&q->qlock);
} /*}}}*/

#ifdef QTHREAD_USE_SPAWNCACHE
int INTERNAL qt_threadqueue_private_enqueue(qt_threadqueue_private_t *restrict c, /* cache */
                                            qt_threadqueue_t *restrict         q, /* queue */
                                            qthread_t *restrict                t) /* thread */
{                                                                                 /*{{{*/
    assert(c != NULL &&
           ((c->head == c->tail && c->qlength < 2) ||
            (c->head != NULL && c->tail != NULL && c->qlength > 1)));
    assert(t != NULL);

    qt_threadqueue_node_t *node;

    node = ALLOC_TQNODE();
    assert(node != NULL);

    node->next      = NULL;
    node->prev      = NULL;
    node->value     = t;
    node->stealable = qt_threadqueue_isstealable(t);

    assert(c->tail == NULL || c->tail->next == NULL);
    assert(c->head == NULL || c->head->prev == NULL);
    if (c->on_deck) {
        qt_threadqueue_node_t *tmp = c->on_deck;
        // Add to the tail of the `c`
        tmp->prev = c->tail;
        c->tail   = tmp;
        if (c->head == NULL) {
            c->head = tmp;
        } else {
            tmp->prev->next = tmp;
        }
        c->qlength++;
        if (tmp->stealable) { c->qlength_stealable++; }
        assert(c->tail->next == NULL);
        assert(c->head->prev == NULL);
    }
    c->on_deck = node;

# if 0
    /* it is an idea, but so far does not appear to have any benefit (and a tiny tiny amount of harm, ~0.4%)... */
    if ((q->qlength == 0) && (qlib->nworkers_active > 1) && (c->qlength > (qlib->nworkers_active << 2))) {
        qt_threadqueue_node_t *first = c->head;
        qt_threadqueue_node_t *last  = c->tail;
        if (QTHREAD_TRYLOCK_TRY(&q->qlock)) {
            assert((q->head && q->tail) || (!q->head && !q->tail));
            assert(q->head != first);
            assert(q->tail != last);
            first->prev = q->tail;
            q->tail     = last;
            if (q->head == NULL) {
                q->head = first;
            } else {
                first->prev->next = first;
            }
            q->qlength           += c->qlength;
            q->qlength_stealable += c->qlength_stealable;
            assert(q->tail->next == NULL);
            assert(q->head->prev == NULL);
            QTHREAD_TRYLOCK_UNLOCK(&q->qlock);
            c->head    = c->tail = NULL;
            c->qlength = c->qlength_stealable = 0;
        }
    }
# endif /* if 0 */
    return 1;
} /*}}}*/

int INTERNAL qt_threadqueue_private_enqueue_yielded(qt_threadqueue_private_t *restrict q,
                                                    qthread_t *restrict                t)
{   /*{{{*/
    assert(q != NULL &&
           ((q->head == q->tail && q->qlength < 2) ||
            (q->head != NULL && q->tail != NULL && q->qlength > 1)));
    assert(t != NULL);

    qt_threadqueue_node_t *node;

    node = ALLOC_TQNODE();
    assert(node != NULL);

    // node->next      = NULL;
    node->prev      = NULL;
    node->value     = t;
    node->stealable = qt_threadqueue_isstealable(t);

    assert(q->tail == NULL || q->tail->next == NULL);
    assert(q->head == NULL || q->head->prev == NULL);
    // Add to the head of the `q`
    node->next = q->head;
    q->head    = node;
    if (q->tail == NULL) {
        q->tail = node;
    } else {
        node->next->prev = node;
    }
    q->qlength++;
    if (node->stealable) { q->qlength_stealable++; }

    assert(q->tail->next == NULL);
    assert(q->head->prev == NULL);

    return 1;
} /*}}}*/

#endif /* ifdef QTHREAD_USE_SPAWNCACHE */

/* yielded threads enqueue at head */
void INTERNAL qt_threadqueue_enqueue_yielded(qt_threadqueue_t *restrict q,
                                             qthread_t *restrict        t)
{   /*{{{*/
    qt_threadqueue_node_t *node;

    node = ALLOC_TQNODE();
    assert(node != NULL);

    node->value     = t;
    node->stealable = qt_threadqueue_isstealable(t);

    assert(q != NULL);
    assert(t != NULL);

    QTHREAD_TRYLOCK_LOCK(&q->qlock);
    PARANOIA_ONLY(sanity_check_queue(q));
    node->prev = NULL;
    node->next = q->head;
    q->head    = node;
    if (q->tail == NULL) {
        q->tail = node;
    } else {
        node->next->prev = node;
    }
    q->qlength++;
    if (node->stealable) { q->qlength_stealable++; }
    QTHREAD_TRYLOCK_UNLOCK(&q->qlock);
} /*}}}*/

/* dequeue at tail */
qthread_t INTERNAL *qt_scheduler_get_thread(qt_threadqueue_t         *q,
                                            qt_threadqueue_private_t *qc,
                                            uint_fast8_t              active)
{   /*{{{*/
    qthread_shepherd_t *my_shepherd = qthread_internal_getshep();
    qthread_t          *t;
    qthread_worker_id_t worker_id = NO_WORKER;

    assert(q != NULL);
    assert(my_shepherd);
    assert(my_shepherd->ready == q);
    assert(my_shepherd->sorted_sheplist);

    while (1) {
        qt_threadqueue_node_t *node = NULL;

        if (qc && (qc->on_deck != NULL)) {
            assert(qc->tail == NULL || qc->tail->next == NULL);
            assert(qc->head == NULL || qc->head->prev == NULL);
            node        = qc->on_deck;
            qc->on_deck = NULL;
            assert(node->next == NULL);
            assert(node->prev == NULL);
            if (qc->qlength > 0) {
                // Push remaining items onto the real queue
                qt_threadqueue_node_t *first = qc->head;
                qt_threadqueue_node_t *last  = qc->tail;
                assert(last->next == NULL);
                assert(first->prev == NULL);
                /* Note: I tried doing the this code with a TRY rather than a
                 * LOCK and performance of UTS suffered (slightly). */
#if 0
                if (QTHREAD_TRYLOCK_TRY(&q->qlock)) {
                    assert((q->head && q->tail) || (!q->head && !q->tail));
                    assert(q->head != first);
                    assert(q->tail != last);
                    first->prev = q->tail;
                    q->tail     = last;
                    if (q->head == NULL) {
                        q->head = first;
                    } else {
                        first->prev->next = first;
                    }
                    q->qlength           += qc->qlength;
                    q->qlength_stealable += qc->qlength_stealable;
                    assert(q->tail->next == NULL);
                    assert(q->head->prev == NULL);
                    QTHREAD_TRYLOCK_UNLOCK(&q->qlock);
                    qc->head    = qc->tail = NULL;
                    qc->qlength = qc->qlength_stealable = 0;
                } else {
                    // Refill on-deck
                    qc->on_deck       = qc->head;
                    qc->head          = qc->head->next;
                    qc->on_deck->next = NULL;
                    if (qc->head) {
                        qc->head->prev = NULL;
                    }
                    qc->qlength--;
                    qc->qlength_stealable -= qc->on_deck->stealable;
                }
#else           /* if 0 */
                QTHREAD_TRYLOCK_LOCK(&q->qlock);
                PARANOIA_ONLY(sanity_check_queue(q));
                assert(q->head != first);
                assert(q->tail != last);
                first->prev = q->tail;
                q->tail     = last;
                if (q->head == NULL) {
                    q->head = first;
                } else {
                    first->prev->next = first;
                }
                q->qlength           += qc->qlength;
                q->qlength_stealable += qc->qlength_stealable;
                assert(q->tail->next == NULL);
                assert(q->head->prev == NULL);
                QTHREAD_TRYLOCK_UNLOCK(&q->qlock);
                if (steal_disable) { steal_disable = 0; }
                qc->head    = qc->tail = NULL;
                qc->qlength = qc->qlength_stealable = 0;
#endif          /* if 0 */
            }
        } else if (q->head) {
            QTHREAD_TRYLOCK_LOCK(&q->qlock);
            PARANOIA_ONLY(sanity_check_queue(q));
            node = q->tail;
            if (node != NULL) {
                q->tail = node->prev;
                if (q->tail == NULL) {
                    q->head = NULL;
                } else {
                    q->tail->next = NULL;
                }
                q->qlength--;
                if (node->stealable) { q->qlength_stealable--; }
            }
            QTHREAD_TRYLOCK_UNLOCK(&q->qlock);
        }

        if ((node == NULL) && my_shepherd->stealing) {
            if (worker_id == NO_WORKER) {
                worker_id = qthread_worker(NULL);
            }
            if ((my_shepherd->shepherd_id == 0) && (worker_id == 0)) {
                while (my_shepherd->stealing == 1) SPINLOCK_BODY();  // no sense contending for the lock
            } else {
                while (my_shepherd->stealing) SPINLOCK_BODY();  // no sense contending for the lock
            }
            continue;
        }

        if ((node == NULL) && (active)) {
            if (qlib->nshepherds > 1) {
                if (!steal_disable) {
                    node = qthread_steal(my_shepherd);
                } else {
                    while (NULL == q->head) SPINLOCK_BODY();
                    continue;
                }
            }
        }
        if (node) {
            t = node->value;
            FREE_TQNODE(node);
            if ((t->flags & QTHREAD_REAL_MCCOY)) { // only needs to be on worker 0 for termination
                if (worker_id == NO_WORKER) {
                    worker_id = qthread_worker(NULL);
                }
                switch(worker_id) {
                    case NO_WORKER:
                        QTHREAD_TRAP(); // should never happen
                        abort();
                        continue; // keep looking
                    case 0:
                        if (my_shepherd->stealing) { my_shepherd->stealing = 0; }
                        return(t);

                    default:
                        /* McCoy thread can only run on worker 0 */
                        my_shepherd->stealing = 2; // no stealing
                        MACHINE_FENCE;
                        qt_threadqueue_enqueue_yielded(q, t);
                        t = NULL;
                        continue; // keep looking
                }
            } else {
                break;
            }
        }
    }
    return (t);
} /*}}}*/

/* enqueue multiple (from steal) */
void INTERNAL qt_threadqueue_enqueue_multiple(qt_threadqueue_t      *q,
                                              qt_threadqueue_node_t *first)
{   /*{{{*/
    qt_threadqueue_node_t *last;
    size_t                 addCnt = 1;

    assert(first != NULL);
    assert(q != NULL);

    last = first;
    while (last->next) {
        last = last->next;
        addCnt++;
    }

    QTHREAD_TRYLOCK_LOCK(&q->qlock);
    PARANOIA_ONLY(sanity_check_queue(q));
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
    QTHREAD_TRYLOCK_UNLOCK(&q->qlock);
} /*}}}*/

/* dequeue stolen threads at head, skip yielded threads */
qt_threadqueue_node_t INTERNAL *qt_threadqueue_dequeue_steal(qt_threadqueue_t *h,
                                                             qt_threadqueue_t *v)
{                                      /*{{{ */
    qt_threadqueue_node_t *node;
    qt_threadqueue_node_t *first     = NULL;
    qt_threadqueue_node_t *last      = NULL;
    long                   amtStolen = 0;
    long                   desired_stolen;

    if (steal_chunksize == 0) {
        desired_stolen = v->qlength_stealable / 2;
    } else {
        desired_stolen = steal_chunksize;
    }

    assert(h != NULL);
    assert(v != NULL);

    if (desired_stolen == 0) { desired_stolen = 1; }

    if (!QTHREAD_TRYLOCK_TRY(&v->qlock)) {
        return NULL;
    }
    PARANOIA_ONLY(sanity_check_queue(v));
    while (v->qlength_stealable > 0 && amtStolen < desired_stolen) {
        node = (qt_threadqueue_node_t *)v->head;
        do {
            // Find next stealable node (if one exists)
            while (node) {
                if (!node->stealable) {
                    node = node->next;
                } else {
                    break;
                }
            }
            if (node != NULL) {
                qt_threadqueue_node_t *first_stolen = node;
                qt_threadqueue_node_t *last_stolen  = node;

                amtStolen++;

                // Adjust queue length(s)
                v->qlength--;
                v->qlength_stealable--;

                // Find next unstealable node, or amount we want to steal
                qt_threadqueue_node_t *next_to_steal = last_stolen->next;
                while (amtStolen < desired_stolen && next_to_steal) {
                    if (!next_to_steal->stealable) {
                        break;
                    } else {
                        last_stolen = next_to_steal;

                        amtStolen++;

                        // Adjust queue length(s)
                        v->qlength--;
                        v->qlength_stealable--;

                        next_to_steal = next_to_steal->next;
                    }
                }

                // Patch up the victim queue
                if (first_stolen == v->head) {
                    v->head = last_stolen->next;
                } else {
                    first_stolen->prev->next = last_stolen->next;
                }
                if (last_stolen == v->tail) {
                    v->tail = first_stolen->prev;
                } else {
                    last_stolen->next->prev = first_stolen->prev;
                }

                node = last_stolen->next;

                // Update steal list (first & last)
                first_stolen->prev = last_stolen->next = NULL;
                if (first == NULL) {
                    first = first_stolen;
                    last  = last_stolen;
                } else {
                    last->next         = first_stolen;
                    first_stolen->prev = last;
                    last               = last_stolen;
                }
            } else {
                break;
            }
        } while (v->qlength_stealable > 0 && amtStolen < desired_stolen);
        break;
    }
    QTHREAD_TRYLOCK_UNLOCK(&v->qlock);
#ifdef STEAL_PROFILE                   // should give mechanism to make steal profiling optional
    qthread_incr(&v->steal_amount_stolen, amtStolen);
#endif

    return (first);
}                                      /*}}} */

/*  Steal work from another shepherd's queue
 *  Returns the work stolen
 */
static QINLINE qt_threadqueue_node_t *qthread_steal(qthread_shepherd_t *thief_shepherd)
{   /*{{{*/
    qt_threadqueue_node_t *stolen = NULL;

    assert(thief_shepherd);

    STEAL_CALLED(thief_shepherd);
    if (thief_shepherd->stealing) {
        // this means that someone else on this shepherd is already stealing; I will spin on my own queue.
        return NULL;
    } else {
#ifdef QTHREAD_OMP_AFFINITY /*{{{*/
        if (thief_shepherd->stealing_mode == QTHREAD_STEAL_ON_ALL_IDLE) {
            int i;
            for (i = 0; i < qlib->nworkerspershep; i++)
                if (thief_shepherd->workers[i].current != NULL) {
                    return NULL;
                }
            thief_shepherd->stealing_mode = QTHREAD_STEAL_ON_ANY_IDLE;
        }
#endif  /*}}}*/
        if (qthread_cas(&thief_shepherd->stealing, 0, 1) != 0) { // avoid unnecessary stealing with a CAS
            return NULL;
        }
    }
    STEAL_ATTEMPTED(thief_shepherd);

    qthread_shepherd_id_t        i               = 0;
    qthread_shepherd_t *const    shepherds       = qlib->shepherds;
    qthread_shepherd_id_t *const sorted_sheplist = thief_shepherd->sorted_sheplist;
    assert(sorted_sheplist);

    qt_threadqueue_t *myqueue = thief_shepherd->ready;
    while (stolen == NULL) {
        qt_threadqueue_t *victim_queue = shepherds[sorted_sheplist[i]].ready;
        if (0 != victim_queue->qlength_stealable) {
            stolen = qt_threadqueue_dequeue_steal(myqueue, victim_queue);
            if (stolen) {
                qt_threadqueue_node_t *surplus = stolen->next;
                if (surplus) {
                    stolen->next  = NULL;
                    surplus->prev = NULL;
                    qt_threadqueue_enqueue_multiple(myqueue, surplus);
                }
                STEAL_SUCCESSFUL(thief_shepherd);
                break;
            } else {
                STEAL_FAILED(thief_shepherd);
            }
        }
        if ((0 < myqueue->qlength) || steal_disable) {  // work at home quit steal attempt
            break;
        }

        i++;
        i *= (i < qlib->nshepherds - 1);
        if (i == 0) {
#ifdef HAVE_PTHREAD_YIELD
            pthread_yield();
#elif defined(HAVE_SCHED_YIELD)
            sched_yield();
#else
            SPINLOCK_BODY();
#endif
        }
        SPINLOCK_BODY();
    }
    thief_shepherd->stealing = 0;
    return stolen;
} /*}}}*/

#ifdef STEAL_PROFILE                   // should give mechanism to make steal profiling optional
void INTERNAL qthread_steal_stat(void)
{   /*{{{*/
    int i;

    assert(qlib);
    for (i = 0; i < qlib->nshepherds; i++) {
        fprintf(stdout,
                "QTHREADS: shepherd %d - steals called %ld attempted %ld failed %ld successful %ld work stolen %ld\n",
                qlib->shepherds[i].shepherd_id,
                qlib->shepherds[i].steal_called,
                qlib->shepherds[i].steal_attempted,
                qlib->shepherds[i].steal_failed,
                qlib->shepherds[i].steal_successful,
                qlib->shepherds[i].ready->steal_amount_stolen);
    }
} /*}}}*/

#endif  /* ifdef STEAL_PROFILE */

/* walk queue looking for a specific value  -- if found remove it (and start
 * it running)  -- if not return NULL
 */
qthread_t INTERNAL *qt_threadqueue_dequeue_specific(qt_threadqueue_t *q,
                                                    void             *value)
{   /*{{{*/
    qt_threadqueue_node_t *node = NULL;
    qthread_t             *t    = NULL;

    assert(q != NULL);

    QTHREAD_TRYLOCK_LOCK(&q->qlock);
    PARANOIA_ONLY(sanity_check_queue(q));
    if (q->qlength > 0) {
        node = (qt_threadqueue_node_t *)q->tail;
        t    = (node) ? (qthread_t *)node->value : NULL;
        while ((t != NULL) && (t->ret != value)) {
            node = (qt_threadqueue_node_t *)node->prev;
            t    = (node) ? (qthread_t *)node->value : NULL;
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
        }
    }
    QTHREAD_TRYLOCK_UNLOCK(&q->qlock);

    return (t);
} /*}}}*/

void INTERNAL qthread_steal_enable()
{   /*{{{*/
    steal_disable = 0;
}   /*}}}*/

void INTERNAL qthread_steal_disable()
{   /*{{{*/
    steal_disable = 1;
}   /*}}}*/

/* vim:set expandtab: */
