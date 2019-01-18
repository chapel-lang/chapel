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
#include "qt_alloc.h"
#include "qt_visibility.h"
#include "qthread_innards.h"           /* for qlib */
#include "qt_shepherd_innards.h"
#include "qt_qthread_struct.h"
#include "qt_qthread_mgmt.h"
#include "qt_asserts.h"
#include "qt_prefetch.h"
#include "qt_threadqueues.h"
#include "qt_envariables.h"
#include "qt_debug.h"
#ifdef QTHREAD_USE_EUREKAS
#include "qt_eurekas.h" /* for qt_eureka_check() */
#endif /* QTHREAD_USE_EUREKAS */
#include "qt_expect.h"
#include "qt_subsystems.h"

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
#endif

    QTHREAD_TRYLOCK_TYPE qlock;
} /* qt_threadqueue_t */;

static aligned_t steal_disable   = 0;
static long      steal_chunksize = 0;

#ifdef STEAL_PROFILE
# define STEAL_CALLED(shep)     qthread_incr( & ((shep)->steal_called), 1)
# define STEAL_ELECTED(shep)    qthread_incr( & ((shep)->steal_elected), 1)
# define STEAL_ATTEMPTED(shep)  qthread_incr( & ((shep)->steal_attempted), 1)
# define STEAL_SUCCESSFUL(shep) do {} while (0)
# define STEAL_FAILED(shep)     qthread_incr( & ((shep)->steal_failed), 1)
# define STEAL_AMOUNT(q, ct)    qthread_incr( & ((q)->steal_amount_stolen), ct)
#else
# define STEAL_CALLED(shep)     do {} while(0)
# define STEAL_ELECTED(shep)    do {} while(0)
# define STEAL_ATTEMPTED(shep)  do {} while(0)
# define STEAL_SUCCESSFUL(shep) do {} while(0)
# define STEAL_FAILED(shep)     do {} while(0)
# define STEAL_AMOUNT(q, ct)    do {} while(0)
#endif /* ifdef STEAL_PROFILE */

// Forward declarations
qt_threadqueue_node_t INTERNAL *qt_threadqueue_dequeue_steal(qt_threadqueue_t *h,
                                                             qt_threadqueue_t *v);

void INTERNAL qt_threadqueue_enqueue_multiple(qt_threadqueue_t      *q,
                                              qt_threadqueue_node_t *first);

qthread_t INTERNAL *qt_init_agg_task(void);
int INTERNAL        qt_keep_adding_agg_task(qthread_t *agg_task,
                                            int        max_t,
                                            int       *curr_cost,
                                            void      *q,
                                            int        lock);
void INTERNAL qt_add_first_agg_task(qthread_t             *agg_task,
                                    int                   *curr_cost,
                                    qt_threadqueue_node_t *node);

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

/*FIXME: Cannot use pools for allcations of variable size */
#define ALLOC_EXTRA(size) qt_malloc(size)
#define FREE_EXTRA(t)     qt_free(t)
#define DIV_FACTOR  4
#define MAX_ABS_AGG 64
static void      **agged_tasks_arg = NULL;
static void      **agged_tasks_ret = NULL;
static qthread_f **agged_tasks_f   = NULL;

static void init_agged_tasks(void)
{
    int                i;
    const unsigned int total_num_workers = qthread_readstate(TOTAL_WORKERS);

    agged_tasks_arg = (void **)ALLOC_EXTRA(total_num_workers * sizeof(void *) * 3);
    agged_tasks_ret = agged_tasks_arg + total_num_workers;
    agged_tasks_f   = (qthread_f **)(agged_tasks_ret + total_num_workers);

    int   dim_per_worker = (sizeof(void *) * 2 + sizeof(qthread_f)) * MAX_ABS_AGG + sizeof(int);
    char *total_mem      = ALLOC_EXTRA(dim_per_worker * total_num_workers);
    for(i = 0; i < total_num_workers; i++) {
        agged_tasks_arg[i] = (void *)(total_mem + dim_per_worker * i);
        agged_tasks_ret[i] = (void *)(total_mem + dim_per_worker * i + MAX_ABS_AGG * sizeof(void *));
        agged_tasks_f[i]   = (void *)(total_mem + dim_per_worker * i + 2 * MAX_ABS_AGG * sizeof(void *));
    }
}

static void free_agged_tasks(void)
{
    FREE_EXTRA(agged_tasks_arg[0]);
    FREE_EXTRA(agged_tasks_arg);
}

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
static void qt_threadqueue_subsystem_shutdown(void)
{
    free_agged_tasks();
}

void INTERNAL qt_threadqueue_subsystem_init(void)
{
    init_agged_tasks();
    steal_chunksize = qt_internal_get_env_num("STEAL_CHUNK", 0, 0);
    qthread_internal_cleanup(qt_threadqueue_subsystem_shutdown);
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
    free_agged_tasks();
} /*}}}*/

void INTERNAL qt_threadqueue_subsystem_init(void)
{   /*{{{*/
    init_agged_tasks();
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

    qthread_debug(THREADQUEUE_CALLS, "c(%p), q(%p), t(%p:%u)\n", c, q, t, t->thread_id);
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
            assert(tmp && tmp->prev);
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
        assert(node && node->next);
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

#define QTHREAD_TASK_IS_AGGREGABLE(f) (0 &&                                                \
                                       (f &QTHREAD_SIMPLE) && !(f &QTHREAD_HAS_ARGCOPY) && \
                                       !(f &QTHREAD_BIG_STRUCT) &&                         \
                                       !(f &QTHREAD_FUTURE) && !(f &QTHREAD_REAL_MCCOY) && \
                                       !(f &QTHREAD_AGGREGATED))
// && (f & QTHREAD_AGGREGABLE) \
//               )
// (f & QTHREAD_AGGREGABLE)

qthread_t INTERNAL *qt_init_agg_task() // partly a duplicate from qthread.c
{
    qthread_t *t = ALLOC_QTHREAD();

#ifdef QTHREAD_NONLAZY_THREADIDS
    /* give the thread an ID number */
    t->thread_id =
        qthread_internal_incr(&(qlib->max_thread_id),
                              &qlib->max_thread_id_lock, 1);
    if (QTHREAD_UNLIKELY(t->thread_id == QTHREAD_NULL_TASK_ID)) {
        /* yes, this is wrapping around, but... thread_id should be prevented from being NULL */
        t->thread_id = qthread_internal_incr(&(qlib->max_thread_id),
                                             &qlib->max_thread_id_lock, 2);
    } else if (QTHREAD_UNLIKELY(t->thread_id == QTHREAD_NON_TASK_ID)) {
        /* yes, this is wrapping around, but... thread_id should be prevented from being NON */
        t->thread_id = qthread_internal_incr(&(qlib->max_thread_id),
                                             &qlib->max_thread_id_lock, 1);
    }
#else /* ifdef QTHREAD_NONLAZY_THREADIDS */
    t->thread_id = QTHREAD_NON_TASK_ID;
#endif /* ifdef QTHREAD_NONLAZY_THREADIDS */

    t->thread_state    = QTHREAD_STATE_NEW;
    t->flags           = 0;
    t->target_shepherd = NO_SHEPHERD;
    t->team            = NULL;
    t->f               = (qthread_f)qlib->agg_f; // changed function pointer type!!!
    t->arg             = NULL;                   // set later
    t->ret             = 0;
    t->rdata           = NULL;
    t->preconds        = NULL;  // use for list of f and arg
    t->flags &= ~QTHREAD_HAS_ARGCOPY;
    t->flags |= QTHREAD_SIMPLE; // will remain a simple task if all tasks it batches are simple.
    t->flags |= QTHREAD_AGGREGATED;

    int loc_id = qthread_worker(NULL);
    t->arg      = agged_tasks_arg[loc_id];
    t->ret      = agged_tasks_ret[loc_id];
    t->preconds = agged_tasks_f[loc_id];
    return t;
}

int INTERNAL qt_keep_adding_agg_task(qthread_t *agg_task,
                                     int        max_t,
                                     int       *curr_cost,
                                     void      *q,
                                     int        lock)
{
    qt_threadqueue_node_t *node         = NULL;
    qthread_t             *t            = NULL;
    int                    count        = ((int *)agg_task->preconds)[0];
    qthread_f             *list_of_f    /*= (qthread_f *)(&(((int *)agg_task->preconds)[1]))*/;
    void                 **list_of_farg /*= (void **)agg_task->arg*/;
    void                 **list_of_fret /*= (void **)agg_task->ret*/;
    int                    local_cost   = *curr_cost;
    // never getting more than what was initially allocated!

    qt_threadqueue_node_t **head_addr, *head_l = NULL;
    qt_threadqueue_node_t **tail_addr, *tail_l = NULL;
    long                   *length, *stealable, len_l = 0, ste_l = 0;

    if(lock) {
        qt_threadqueue_t *public_q = (qt_threadqueue_t *)q;
        head_addr = &(public_q->head);
        tail_addr = &(public_q->tail);
        length    = &(public_q->qlength);
        assert(*length > 0);
        stealable = &(public_q->qlength_stealable);
        int stop_adding = 0;
        if(count > 0) {
            int local_max_t = (count + *length) / qthread_readstate(TOTAL_WORKERS) / DIV_FACTOR;
            local_max_t = (local_max_t > MAX_ABS_AGG ? MAX_ABS_AGG : local_max_t);
            if(local_max_t < max_t) {
                max_t = local_max_t;
            }
            if(max_t <= count) {
                stop_adding = 1;
            }
        }

        if(!stop_adding) { // keep some tasks
            head_l = *head_addr;
            tail_l = *tail_addr;
            if(tail_l != NULL) {
                int max_allowed = max_t - count;
                while((len_l < max_allowed) && QTHREAD_TASK_IS_AGGREGABLE(tail_l->value->flags) && tail_l != head_l) {
                    len_l++;
                    if (tail_l->stealable) { ste_l++; }
                    tail_l = tail_l->prev;
                }
            }

            if(tail_l == *tail_addr) { // didn't keep anything
                assert(len_l == 0 && ste_l == 0);
                head_l = NULL;
                tail_l = NULL;
            } else if(head_l == tail_l) { // kept eveything
                head_l     = *head_addr;
                tail_l     = *tail_addr;
                *head_addr = *tail_addr = NULL; // empty q
                *length    = *stealable = 0;
            } else {   // tail_l is the new q tail, = first task which should remain there
                       // interchange tail_l and real tail using head_l as tmp
                head_l     = tail_l;
                tail_l     = *tail_addr;
                *tail_addr = head_l;
                // break links
                head_l             = (*tail_addr)->next;
                head_l->prev       = NULL;
                (*tail_addr)->next = NULL;
                *length           -= len_l;
                assert(*length > 0);
                *stealable -= ste_l;
            }
        }
        QTHREAD_TRYLOCK_UNLOCK(&public_q->qlock);
        // work on locally grabbed q from now on
        head_addr = &head_l;
        tail_addr = &tail_l;
        length    = &len_l;
        stealable = &ste_l;
    } else {
        qt_threadqueue_private_t *private_q = (qt_threadqueue_private_t *)q;
        head_addr = &(private_q->head);
        tail_addr = &(private_q->tail);
        length    = &(private_q->qlength);
        stealable = &(private_q->qlength_stealable);
    }

    while(*head_addr != NULL) {
        node = *tail_addr;
        t    = node->value;
        if(!QTHREAD_TASK_IS_AGGREGABLE(t->flags)) {
            // printf("Found non-agg task, stopping\n");
            break;
        }

        if((t->flags & QTHREAD_RET_MASK) != (agg_task->flags & QTHREAD_RET_MASK)) {
            // printf("Found task with different return value, stopping\n");
            break;
        }

        list_of_f[count]    = t->f;
        list_of_farg[count] = t->arg;
        local_cost          = (qlib->agg_cost)(count, list_of_f, list_of_farg);
        if(local_cost < qlib->max_c) {
            // cost ok => remove task from q;
            *tail_addr = node->prev;
            if (*tail_addr == NULL) {
                *head_addr = NULL;
            } else {
                (*tail_addr)->next = NULL;
            }
            *length = *length - 1;
            assert(*length >= 0);
            if (node->stealable) { *stealable = *stealable - 1; }
            // update the other info
            list_of_fret[count] = t->ret;
            count++;
            assert(node->value->rdata == NULL);
            qthread_thread_free(t);
            FREE_TQNODE(node);
        } else {
            break;
        }
        if(count + 1 > max_t) {
            // printf("Have enough tasks agg-ed(%d), stopping\n", count);
            break;
        }
    }

    *curr_cost                     = local_cost;
    ((int *)agg_task->preconds)[0] = count;

    if(lock) {
        qt_threadqueue_t *public_q = (qt_threadqueue_t *)q;
        QTHREAD_TRYLOCK_LOCK(&public_q->qlock);
        if(head_l != NULL) { // need to put tasks back into q
            tail_addr = &(public_q->tail);
            if(*tail_addr == NULL) {
                *tail_addr = tail_l;
                assert(public_q->head == NULL);
                public_q->head = head_l;
            } else {
                (*tail_addr)->next = head_l;
                head_l->prev       = (*tail_addr);
                (*tail_addr)       = tail_l;
            }
            assert(public_q->qlength >= 0);
            public_q->qlength += len_l;
            assert(public_q->qlength > 0);
            public_q->qlength_stealable += ste_l;
        }
    }
    return count;
}

void INTERNAL qt_add_first_agg_task(qthread_t             *agg_task,
                                    int                   *curr_cost,
                                    qt_threadqueue_node_t *node)
{
    int       *count_addr = &(((int *)agg_task->preconds)[0]);
    qthread_f *list_of_f  = (qthread_f *)(&(((int *)agg_task->preconds)[1]));

    *count_addr                   = 1;
    list_of_f[0]                  = node->value->f;
    ((void **)(agg_task->arg))[0] = node->value->arg;
    ((void **)(agg_task->ret))[0] = node->value->ret;
    // First task added defines the type of ret accepted inside this agg task
    agg_task->flags |= ((node->value->flags & QTHREAD_RET_IS_SINC) |      \
                        (node->value->flags & QTHREAD_RET_IS_VOID_SINC) | \
                        (node->value->flags & QTHREAD_RET_IS_SYNCVAR));
    assert(node->value->rdata == NULL);
    qthread_thread_free(node->value);
    FREE_TQNODE(node);
    *curr_cost = (qlib->agg_cost)(1, list_of_f, (void **)agg_task->arg);
}

/* dequeue at tail */
qthread_t INTERNAL *qt_scheduler_get_thread(qt_threadqueue_t         *q,
#ifdef QTHREAD_LOCAL_PRIORITY
                                            qt_threadqueue_t         *lpq,
#endif /* ifdef QTHREAD_LOCAL_PRIORITY */
                                            qt_threadqueue_private_t *qc,
                                            uint_fast8_t              active)
{   /*{{{*/
    qthread_shepherd_t *my_shepherd = qthread_internal_getshep();
    qthread_t          *t;
    qthread_worker_id_t worker_id = NO_WORKER;
    int                 curr_cost, max_t, ret_agg_task;

    assert(q != NULL);
    assert(my_shepherd);
    assert(my_shepherd->ready == q);
    assert(my_shepherd->sorted_sheplist);
#ifdef QTHREAD_TASK_AGGREGATION
    t = qt_init_agg_task();
#endif

#ifdef QTHREAD_USE_EUREKAS
    qt_eureka_disable();
#endif /* QTHREAD_USE_EUREKAS */
    while (1) {
        qt_threadqueue_node_t *node = NULL;
#ifdef QTHREAD_TASK_AGGREGATION
        curr_cost = 0; ret_agg_task = 0;
#endif

#ifdef QTHREAD_LOCAL_PRIORITY
            /* First check local priority queue */
        if (lpq->head) {
            QTHREAD_TRYLOCK_LOCK(&lpq->qlock);
            PARANOIA_ONLY(sanity_check_queue(lpq));
            node = lpq->tail;
            if (node != NULL) {
                assert(lpq->head);
                assert(lpq->qlength > 0);

                lpq->tail = node->prev;
                if (lpq->tail == NULL) {
                    lpq->head = NULL;
                } else {
                    lpq->tail->next = NULL;
                }
                assert(lpq->qlength > 0);
                lpq->qlength--;
                lpq->qlength_stealable -= node->stealable;
            }
            QTHREAD_TRYLOCK_UNLOCK(&lpq->qlock);
        }
        else
#endif /* ifdef QTHREAD_LOCAL_PRIORITY */

        // printf("Total number of items: %d+%d\n", (qc?(qc->on_deck?(1+qc->qlength):0):0), q->qlength);
        if (qc && (qc->on_deck != NULL)) {
            assert(qc->tail == NULL || qc->tail->next == NULL);
            assert(qc->head == NULL || qc->head->prev == NULL);
            node        = qc->on_deck;
            qc->on_deck = NULL;
            assert(node->next == NULL);
            assert(node->prev == NULL);
#ifdef QTHREAD_TASK_AGGREGATION
            if(QTHREAD_TASK_IS_AGGREGABLE(node->value->flags) && \
               ((max_t = (qc->qlength + 1 + q->qlength) / qthread_readstate(ACTIVE_WORKERS) / DIV_FACTOR) > 1)
               ) {
                max_t = (max_t > MAX_ABS_AGG ? MAX_ABS_AGG : max_t);
                assert(node->value->thread_state != QTHREAD_STATE_TERM_SHEP);
                qt_add_first_agg_task(t, &curr_cost, node);
                node = NULL;

                int *count_addr = &(((int *)t->preconds)[0]);
                int  lcount     = qt_keep_adding_agg_task(t, max_t, &curr_cost, qc, 0);
                if((qc->qlength == 0) && ((curr_cost < qlib->max_c) && (*count_addr < max_t))) {
                    // cache empty and can still add, get more from q
                    QTHREAD_TRYLOCK_LOCK(&q->qlock);
                    if(q->head) {
                        lcount = qt_keep_adding_agg_task(t, max_t, &curr_cost, q, 1);
                    }
                    QTHREAD_TRYLOCK_UNLOCK(&q->qlock);
                } else {   // done, spill remaining cache
                }
                ret_agg_task = 1;
            } else {   // no agg, spill cache
                       // t = NULL;
            }
#endif      /* ifdef QTHREAD_TASK_AGGREGATION */

            qthread_debug(THREADQUEUE_DETAILS, "q(%p), qc(%p), active(%u): qc->qlen(%u) Push remaining items onto the real queue\n", q, qc, active, qc->qlength);

            if (qc->qlength > 0) {
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
                qc->head    = qc->tail = NULL;
                qc->qlength = qc->qlength_stealable = 0;
#endif          /* if 0 */
            }
        } else if (q->head) {
            QTHREAD_TRYLOCK_LOCK(&q->qlock);
            PARANOIA_ONLY(sanity_check_queue(q));
            node = q->tail;
            if (node != NULL) {
                assert(q->head);
                assert(q->qlength > 0);

                q->tail = node->prev;
                if (q->tail == NULL) {
                    q->head = NULL;
                } else {
                    q->tail->next = NULL;
                }
                assert(q->qlength > 0);
                q->qlength--;
                q->qlength_stealable -= node->stealable;
#ifdef QTHREAD_TASK_AGGREGATION
                if(QTHREAD_TASK_IS_AGGREGABLE(node->value->flags) && \
                   ((max_t = (q->qlength) / qthread_readstate(ACTIVE_WORKERS) / DIV_FACTOR) > 1)
                   ) { // no point creating an agg task with a single simple task
                    max_t = (max_t > MAX_ABS_AGG ? MAX_ABS_AGG : max_t);
                    assert(node->value->thread_state != QTHREAD_STATE_TERM_SHEP);
                    qt_add_first_agg_task(t, &curr_cost, node);
                    node = NULL;
                    if(q->head) {
                        int lcount = qt_keep_adding_agg_task(t, max_t, &curr_cost, q, 1);
                    }
                    ret_agg_task = 1;
                } else {   // no agg, free agg task (delay)
                           // t = NULL;
                }
#endif          /* ifdef QTHREAD_TASK_AGGREGATION */
            }
            QTHREAD_TRYLOCK_UNLOCK(&q->qlock);
        }

#ifdef QTHREAD_TASK_AGGREGATION
        if(ret_agg_task) { // use t, node is NULL
            break;
        }
#endif

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
                    node = qthread_steal(my_shepherd); // TODO: same agg behavior when stealing
                } else {
                    while (NULL == q->head) SPINLOCK_BODY();
                    continue;
                }
            }
        }
        if (node) {
#ifdef QTHREAD_TASK_AGGREGATION
            qthread_thread_free(t); // free agg task; only reallocate it if mccoy found
#endif
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
#ifdef QTHREAD_TASK_AGGREGATION
                        t = qt_init_agg_task();
#endif
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

#ifdef QTHREAD_USE_SPAWNCACHE
void INTERNAL qt_threadqueue_enqueue_cache(qt_threadqueue_t         *q,
                                           qt_threadqueue_private_t *cache)
{   /*{{{*/
    assert(cache->on_deck);
    assert(cache->head == NULL || cache->qlength);

    qt_threadqueue_node_t *first = cache->on_deck;
    qt_threadqueue_node_t *last;

    if (cache->qlength) {
        first->next       = cache->head;
        cache->head->prev = first;
        last              = cache->tail;
    } else {
        last = first;
    }
    cache->qlength++;
    cache->qlength_stealable += first->stealable;
    assert(last->next == NULL);
    assert(first->prev == NULL);
    first->next    = cache->head;
    cache->head    = NULL;
    cache->tail    = NULL;
    cache->on_deck = NULL;
    QTHREAD_TRYLOCK_LOCK(&q->qlock);
    PARANOIA_ONLY(sanity_check_queue(q));
    first->prev = q->tail;
    q->tail     = last;
    if (q->head == NULL) {
        q->head = first;
    } else {
        first->prev->next = first;
    }
    q->qlength           += cache->qlength;
    q->qlength_stealable += cache->qlength_stealable;
    QTHREAD_TRYLOCK_UNLOCK(&q->qlock);
    cache->qlength           = 0;
    cache->qlength_stealable = 0;
} /*}}}*/
#endif /* ifdef QTHREAD_USE_SPAWNCACHE */

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
    STEAL_AMOUNT(v, amtStolen);

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
    STEAL_ELECTED(thief_shepherd);

    qthread_shepherd_id_t        i               = 0;
    qthread_shepherd_t *const    shepherds       = qlib->shepherds;
    qthread_shepherd_id_t *const sorted_sheplist = thief_shepherd->sorted_sheplist;
    assert(sorted_sheplist);

    qt_threadqueue_t *myqueue = thief_shepherd->ready;

#ifdef QTHREAD_LOCAL_PRIORITY
    qt_threadqueue_t *mypriorityqueue = thief_shepherd->local_priority_queue;
#endif /* ifdef QTHREAD_LOCAL_PRIORITY */
    while (stolen == NULL) {
        qt_threadqueue_t *victim_queue = shepherds[sorted_sheplist[i]].ready;
        if (0 != victim_queue->qlength_stealable) {
            STEAL_ATTEMPTED(thief_shepherd);
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
#ifdef QTHREAD_LOCAL_PRIORITY
        if ((0 < mypriorityqueue->qlength)){
          break;
        }
#endif /* ifdef QTHREAD_LOCAL_PRIORITY */
       
        if ((0 < myqueue->qlength) || steal_disable) {  // work at home quit steal attempt
            break;
        }

        i++;
        i *= (i < qlib->nshepherds - 1);
        if (i == 0) {
#ifdef QTHREAD_USE_EUREKAS
            qt_eureka_check(1);
#endif /* QTHREAD_USE_EUREKAS */
#ifdef HAVE_PTHREAD_YIELD
            pthread_yield();
#elif defined(HAVE_SCHED_YIELD)
            sched_yield();
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
                "QTHREADS: shepherd %d - steals called:%ld elected:%ld attempted:%ld(failed:%ld successful:%ld) tasks-stolen:%ld\n",
                qlib->shepherds[i].shepherd_id,
                qlib->shepherds[i].steal_called,
                qlib->shepherds[i].steal_elected,
                qlib->shepherds[i].steal_attempted,
                qlib->shepherds[i].steal_failed,
                qlib->shepherds[i].steal_attempted - qlib->shepherds[i].steal_failed,
                qlib->shepherds[i].ready->steal_amount_stolen);
    }
} /*}}}*/
#endif  /* ifdef STEAL_PROFILE */

#ifdef QTHREAD_USE_SPAWNCACHE
qthread_t INTERNAL *qt_threadqueue_private_dequeue(qt_threadqueue_private_t *c)
{   /*{{{*/
    qt_threadqueue_node_t *node = NULL;
    qthread_t             *t    = NULL;

    assert(c);
    if (c->on_deck) {
        node = c->on_deck;
        t    = node->value;
        if (c->tail) {
            // pull the tail down to the on_deck position
            qt_threadqueue_node_t *n = c->on_deck = c->tail;
            c->tail = n->prev;
            n->prev = NULL;
            if (c->head == n) {
                c->head = NULL;
            }
            c->qlength--;
            c->qlength_stealable -= n->stealable;
        } else {
            c->on_deck = NULL;
        }
        FREE_TQNODE(node);
    }
    return t;
} /*}}}*/

void INTERNAL qt_threadqueue_private_filter(qt_threadqueue_private_t *restrict c,
                                            qt_threadqueue_filter_f            f)
{   /*{{{*/
    qt_threadqueue_node_t *node = NULL;
    qthread_t             *t    = NULL;

    assert(c != NULL);

    if (c->on_deck) {
        qt_threadqueue_node_t *n = c->on_deck;
        switch(f(n->value)) {
            case IGNORE_AND_CONTINUE: // ignore, move to the next one
                break;
            case IGNORE_AND_STOP: // ignore, stop looking
                return;

            case REMOVE_AND_CONTINUE: // remove, move to the next one
            {
#ifdef QTHREAD_USE_EUREKAS
                qthread_internal_assassinate(n->value);
#endif /* QTHREAD_USE_EUREKAS */
                FREE_TQNODE(n);
                c->on_deck = NULL;
                break;
            }
            case REMOVE_AND_STOP:     // remove, stop looking
            {
#ifdef QTHREAD_USE_EUREKAS
                qthread_internal_assassinate(n->value);
#endif /* QTHREAD_USE_EUREKAS */
                FREE_TQNODE(n);
                c->on_deck = NULL;
                goto fixup_on_deck;
            }
        }
    }
    if (c->qlength > 0) {
        qt_threadqueue_node_t **lp = NULL;
        qt_threadqueue_node_t **rp = NULL;

        rp   = &c->tail;
        node = c->tail;
        if (c->head == node) {
            lp = &c->head;
        } else {
            lp = &(node->prev->next);
        }
        while (node) {
            t = (qthread_t *)node->value;
            switch (f(t)) {
                case 0: // ignore, move to the next one
                    rp   = &node->prev;
                    node = node->prev;
                    if (c->head == node) {
                        lp = &c->head;
                    } else {
                        lp = &(node->prev->next);
                    }
                    break;
                case 1: // ignore, stop looking
                    node = NULL;
                    break;
                case 2: // remove, move to the next one
                {
                    qt_threadqueue_node_t *freeme;

                    *lp = node->next;
                    *rp = node->prev;
                    c->qlength--;
                    c->qlength_stealable -= node->stealable;
                    freeme                = node;
                    node                  = node->prev;
#ifdef QTHREAD_USE_EUREKAS
                    qthread_internal_assassinate(t);
#endif /* QTHREAD_USE_EUREKAS */
                    if (c->head == node) {
                        lp = &c->head;
                    } else {
                        lp = &(node->prev->next);
                    }
                    FREE_TQNODE(freeme);
                    break;
                }
                case 3: // remove, stop looking
                    *lp = node->next;
                    *rp = node->prev;
                    c->qlength--;
                    c->qlength_stealable -= node->stealable;
#ifdef QTHREAD_USE_EUREKAS
                    qthread_internal_assassinate(t);
#endif /* QTHREAD_USE_EUREKAS */
                    FREE_TQNODE(node);
                    node = NULL;
                    break;
            }
        }
    }
fixup_on_deck:
    if ((c->on_deck == NULL) && (c->tail != NULL)) {
        // pull the tail down to the on_deck position
        qt_threadqueue_node_t *n = c->on_deck = c->tail;
        c->tail = n->prev;
        n->prev = NULL;
        if (c->head == n) {
            c->head = NULL;
        }
        c->qlength--;
        c->qlength_stealable -= n->stealable;
    }
} /*}}}*/
#endif /* ifdef QTHREAD_USE_SPAWNCACHE */

/* walk queue removing all tasks matching this description */
void INTERNAL qt_threadqueue_filter(qt_threadqueue_t       *q,
                                    qt_threadqueue_filter_f f)
{   /*{{{*/
    qt_threadqueue_node_t *node = NULL;
    qthread_t             *t    = NULL;

    assert(q != NULL);
    /* For reference:
     *
     * dequeue (and filtering) starts at the tail and proceeds to follow the prev ptrs until the head is reached.
     */

    QTHREAD_TRYLOCK_LOCK(&q->qlock);
    PARANOIA_ONLY(sanity_check_queue(q));
    if (q->qlength > 0) {
        qt_threadqueue_node_t **lp = NULL;
        qt_threadqueue_node_t **rp = NULL;

        rp   = &q->tail;
        node = q->tail;
        if (q->head == node) {
            lp = &q->head;
        } else {
            lp = &(node->prev->next);
        }
        while (node) {
            t = (qthread_t *)node->value;
            switch (f(t)) {
                case IGNORE_AND_CONTINUE: // ignore, move to the next one
                    rp   = &node->prev;
                    node = node->prev;
                    if (node) {
                        if (q->head == node) {
                            lp = &q->head;
                        } else {
                            lp = &(node->prev->next);
                        }
                    }
                    break;
                case IGNORE_AND_STOP: // ignore, stop looking
                    node = NULL;
                    break;
                case REMOVE_AND_CONTINUE: // remove, move to the next one
                {
                    qt_threadqueue_node_t *freeme;

                    *lp = node->next;
                    *rp = node->prev;
                    q->qlength--;
                    q->qlength_stealable -= node->stealable;
                    freeme                = node;
                    node                  = node->prev;
#ifdef QTHREAD_USE_EUREKAS
                    qthread_internal_assassinate(t);
#endif /* QTHREAD_USE_EUREKAS */
                    if (q->head == node) {
                        lp = &q->head;
                    } else {
                        lp = &(node->prev->next);
                    }
                    FREE_TQNODE(freeme);
                    break;
                }
                case REMOVE_AND_STOP: // remove, stop looking
                    *lp = node->next;
                    *rp = node->prev;
                    q->qlength--;
                    q->qlength_stealable -= node->stealable;
#ifdef QTHREAD_USE_EUREKAS
                    qthread_internal_assassinate(t);
#endif /* QTHREAD_USE_EUREKAS */
                    FREE_TQNODE(node);
                    node = NULL;
                    break;
            }
        }
    }
    QTHREAD_TRYLOCK_UNLOCK(&q->qlock);
} /*}}}*/

/* walk queue looking for a specific value  -- if found remove it (and start
 * it running)  -- if not return NULL
 */
qthread_t INTERNAL *qt_threadqueue_dequeue_specific(qt_threadqueue_t *q,
                                                    void             *value)
{       /*{{{*/
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
                node->next->prev = node->prev;     // reset back ptr (know we're not tail
                node->next       = NULL;
                node->prev       = q->tail;
                q->tail->next    = node;
                q->tail          = node;
            }
        }
    }
    QTHREAD_TRYLOCK_UNLOCK(&q->qlock);

    return (t);
}     /*}}}*/

void INTERNAL qthread_steal_enable()
{       /*{{{*/
    steal_disable = 0;
}     /*}}}*/

void INTERNAL qthread_steal_disable()
{       /*{{{*/
    steal_disable = 1;
}     /*}}}*/

qthread_shepherd_id_t INTERNAL qt_threadqueue_choose_dest(qthread_shepherd_t * curr_shep)
{
    if (curr_shep) {
        return curr_shep->shepherd_id;
    } else {
        return (qthread_shepherd_id_t)0;
    }
}

size_t INTERNAL qt_threadqueue_policy(const enum threadqueue_policy policy)
{
    switch (policy) {
        default:
            return THREADQUEUE_POLICY_UNSUPPORTED;
    }
}

/* vim:set expandtab: */
