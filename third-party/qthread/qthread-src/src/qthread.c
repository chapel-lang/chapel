#ifdef HAVE_CONFIG_H
# include "config.h"
#endif


/******************************************************/
/* The API                                            */
/******************************************************/
#include "qthread/qthread.h"

/******************************************************/
/* System Headers                                     */
/******************************************************/
#include <stdio.h>
#include <stdlib.h>              /* for malloc() and abort() */
#include <stdarg.h>              /* for va_list, va_start() and va_end() */
#include <limits.h>              /* for INT_MAX */
#include <qthread/qthread-int.h> /* for UINT8_MAX */
#include <string.h>              /* for memset() */
#include <unistd.h>              /* for getpagesize() */
#if !HAVE_MEMCPY
# define memcpy(d, s, n)  bcopy((s), (d), (n))
# define memmove(d, s, n) bcopy((s), (d), (n))
#endif
#include <sys/time.h>
#include <sys/resource.h>
#include <pthread.h>
#ifdef HAVE_SCHED_H
# include <sched.h>
#endif
#ifdef QTHREAD_USE_VALGRIND
# include <valgrind/memcheck.h>
#endif
#include <sys/types.h>
#include <sys/mman.h>
#include <errno.h>
#include <signal.h>

/******************************************************/
/* Public Headers                                     */
/******************************************************/
#include "qthread/cacheline.h"
#if (defined(QTHREAD_SHEPHERD_PROFILING) || defined(QTHREAD_FEB_PROFILING))
# include "qthread/qtimer.h"
#endif
#include "qthread/barrier.h"

/******************************************************/
/* Internal Headers                                   */
/******************************************************/
#include "qt_mpool.h"
#include "qt_atomics.h"
#include "qt_expect.h"
#include "qt_asserts.h"
#include "qthread_innards.h"
#include "qt_prefetch.h"
#include "qt_barrier.h"
#ifdef QTHREAD_OMP_AFFINITY
# include "omp_affinity.h"
#endif
#include "qt_qthread_struct.h"
#include "qt_qthread_mgmt.h"
#include "qt_shepherd_innards.h"
#include "qt_blocking_structs.h"
#include "qt_addrstat.h"
#include "qt_threadqueues.h"
#include "qt_threadqueue_scheduler.h"
#include "qt_affinity.h"
#include "qt_io.h"
#include "qt_debug.h"
#include "qt_envariables.h"
#include "qt_queue.h"
#include "qt_feb.h"
#include "qt_syncvar.h"
#include "qt_spawncache.h"
#ifdef QTHREAD_MULTINODE
# include "qt_multinode_innards.h"
#endif
#include "qt_alloc.h"
#include "qt_teams.h"
#ifdef QTHREAD_USE_EUREKAS
# include "qt_eurekas.h"
#endif /* QTHREAD_USE_EUREKAS */
#include "qt_subsystems.h"
#include "qt_output_macros.h"
#include "qt_int_log.h"


#if !(defined(HAVE_GCC_INLINE_ASSEMBLY) &&              \
    (QTHREAD_SIZEOF_ALIGNED_T == 4 ||                   \
     (QTHREAD_ASSEMBLY_ARCH != QTHREAD_POWERPC32 &&     \
      QTHREAD_ASSEMBLY_ARCH != QTHREAD_SPARCV9_32))) && \
    !defined(QTHREAD_ATOMIC_CAS) &&                     \
    !defined(QTHREAD_MUTEX_INCREMENT)
# warning QTHREAD_MUTEX_INCREMENT not defined. It probably should be.
# define QTHREAD_MUTEX_INCREMENT 1
#endif

#ifdef QTHREAD_PERFORMANCE
#define WKR_DBG 1
#include "qthread/performance.h"
#include "qthread/logging.h"
#endif //ifdef QTHREAD_PERFORMANCE

/* Shared Globals */
qlib_t qlib      = NULL;
int    qaffinity = 1;
QTHREAD_FASTLOCK_ATTRVAR;

struct qt_cleanup_funcs_s {
    void                       (*func)(void);
    struct qt_cleanup_funcs_s *next;
} *qt_cleanup_funcs    = NULL, *qt_cleanup_early_funcs = NULL,
*qt_cleanup_late_funcs = NULL;

#ifdef QTHREAD_COUNT_THREADS
aligned_t             threadcount;
aligned_t             maxconcurrentthreads;
double                avg_concurrent_threads;
aligned_t             maxeffconcurrentthreads;
double                avg_eff_concurrent_threads;
aligned_t             effconcurrentthreads;
aligned_t             concurrentthreads;
QTHREAD_FASTLOCK_TYPE concurrentthreads_lock;
QTHREAD_FASTLOCK_TYPE effconcurrentthreads_lock;
#endif

#ifdef QTHREAD_GUARD_PAGES
int GUARD_PAGES = 1;
#else
#define GUARD_PAGES 0
#endif

/* Internal Prototypes */
#ifdef QTHREAD_MAKECONTEXT_SPLIT
static void qthread_wrapper(unsigned int high,
                            unsigned int low);
#else
static void qthread_wrapper(void *ptr);
#endif

static QINLINE void qthread_makecontext(qt_context_t *const c,
                                        void *const         stack,
                                        const size_t        stacksize,
                                        void                (*func)(void),
                                        const void *const   arg,
                                        qt_context_t *const returnc);
static QINLINE qthread_t *qthread_thread_new(qthread_f   f,
                                             const void *arg,
                                             size_t      arg_size,
                                             void       *ret,
                                             qt_team_t  *team,
                                             int         team_leader);

/*Make method externally available for the scheduler; to be used when agg tasks*/
void qthread_thread_free(qthread_t *t);


#if defined(UNPOOLED_QTHREAD_T) || defined(UNPOOLED)
# define ALLOC_QTHREAD()     (qthread_t *)MALLOC(sizeof(qthread_t) + sizeof(void *) + qlib->qthread_tasklocal_size)
# define ALLOC_BIG_QTHREAD() (qthread_t *)MALLOC(sizeof(qthread_t) + qlib->qthread_argcopy_size + qlib->qthread_tasklocal_size)
# define FREE_QTHREAD(t)     FREE(t, sizeof(qthread_t) + sizeof(void *) + qlib->qthread_tasklocal_size)
# define FREE_BIG_QTHREAD(t) FREE(t, sizeof(qthread_t) + qlib->qthread_argcopy_size + qlib->qthread_tasklocal_size)
#else /* if defined(UNPOOLED_QTHREAD_T) || defined(UNPOOLED) */
qt_mpool generic_qthread_pool     = NULL;
qt_mpool generic_big_qthread_pool = NULL;
# define ALLOC_QTHREAD()     (qthread_t *)qt_mpool_alloc(generic_qthread_pool)
# define ALLOC_BIG_QTHREAD() (qthread_t *)qt_mpool_alloc(generic_big_qthread_pool)
# define FREE_QTHREAD(t)     qt_mpool_free(generic_qthread_pool, t)
# define FREE_BIG_QTHREAD(t) qt_mpool_free(generic_big_qthread_pool, t)
#endif /* if defined(UNPOOLED_QTHREAD_T) || defined(UNPOOLED) */

#if defined(UNPOOLED_STACKS) || defined(UNPOOLED)
# ifdef QTHREAD_GUARD_PAGES
static QINLINE void *ALLOC_STACK(void)
{                      /*{{{ */
    if (GUARD_PAGES) {
        uint8_t *tmp = qt_internal_aligned_alloc(qlib->qthread_stack_size + sizeof(struct qthread_runtime_data_s) + (2 * getpagesize()), getpagesize());

        assert(tmp != NULL);
        if (tmp == NULL) {
            return NULL;
        }
        ALLOC_SCRIBBLE(tmp, qlib->qthread_stack_size + sizeof(struct qthread_runtime_data_s) + (2 * getpagesize()));
        if (mprotect(tmp, getpagesize(), PROT_NONE) != 0) {
            perror("mprotect in ALLOC_STACK (1)");
        }
        if (mprotect(tmp + qlib->qthread_stack_size + getpagesize(), getpagesize(), PROT_NONE) != 0) {
            perror("mprotect in ALLOC_STACK (2)");
        }
        return tmp + getpagesize();
    } else {
        return MALLOC(qlib->qthread_stack_size + sizeof(struct qthread_runtime_data_s));
    }
}                      /*}}} */

static QINLINE void FREE_STACK(void *t)
{                      /*{{{ */
    if (GUARD_PAGES) {
        uint8_t *tmp = t;

        assert(t);
        tmp -= getpagesize();
        if (mprotect(tmp, getpagesize(), PROT_READ | PROT_WRITE) != 0) {
            perror("mprotect in FREE_STACK (1)");
        }
        if (mprotect(tmp + qlib->qthread_stack_size + getpagesize(),
                    getpagesize(),
                    PROT_READ | PROT_WRITE) != 0) {
            perror("mprotect in FREE_STACK (2)");
        }
        FREE(tmp, qlib->qthread_stack_size + sizeof(struct qthread_runtime_data_s) + (2 * getpagesize()));
    } else {
        FREE(t, qlib->qthread_stack_size); /* XXX: this size seems wrong */
    }
}                      /*}}} */

# else /* ifdef QTHREAD_GUARD_PAGES */
#  define ALLOC_STACK() MALLOC(qlib->qthread_stack_size + sizeof(struct qthread_runtime_data_s))
#  define FREE_STACK(t) FREE(t, qlib->qthread_stack_size) /* XXX: this size seems wrong */
# endif /* ifdef QTHREAD_GUARD_PAGES */
#else /* if defined(UNPOOLED_STACKS) || defined(UNPOOLED) */
static qt_mpool generic_stack_pool = NULL;
# ifdef QTHREAD_GUARD_PAGES
static QINLINE void *ALLOC_STACK(void)
{                      /*{{{ */
    if (GUARD_PAGES) {
        uint8_t *tmp = qt_mpool_alloc(generic_stack_pool);

        assert(tmp);
        if (tmp == NULL) {
            return NULL;
        }
        if (mprotect(tmp, getpagesize(), PROT_NONE) != 0) {
            perror("mprotect in ALLOC_STACK (1)");
        }
        if (mprotect(tmp + qlib->qthread_stack_size + getpagesize(),
                    getpagesize(),
                    PROT_NONE) != 0) {
            perror("mprotect in ALLOC_STACK (2)");
        }
        return tmp + getpagesize();
    } else {
        return qt_mpool_alloc(generic_stack_pool);
    }
}                      /*}}} */

static QINLINE void FREE_STACK(void *t)
{                      /*{{{ */
    if (GUARD_PAGES) {
        assert(t);
        t = (uint8_t*)t - getpagesize();
        if (mprotect(t, getpagesize(), PROT_READ | PROT_WRITE) != 0) {
            perror("mprotect in FREE_STACK (1)");
        }
        if (mprotect(((uint8_t*)t) + qlib->qthread_stack_size + getpagesize(),
                    getpagesize(),
                    PROT_READ | PROT_WRITE) != 0) {
            perror("mprotect in FREE_STACK (2)");
        }
    }
    qt_mpool_free(generic_stack_pool, t);
}                      /*}}} */

# else /* ifdef QTHREAD_GUARD_PAGES */
#  define ALLOC_STACK() qt_mpool_alloc(generic_stack_pool)
#  define FREE_STACK(t) qt_mpool_free(generic_stack_pool, t)
# endif /* ifdef QTHREAD_GUARD_PAGES */
#endif  /* if defined(UNPOOLED_STACKS) || defined(UNPOOLED) */

#if defined(UNPOOLED)
# define ALLOC_RDATA() (struct qthread_runtime_data_s *)MALLOC(sizeof(struct qthread_runtime_data_s));
# define FREE_RDATA(r) FREE(r, sizeof(struct qthread_runtime_data_s))
#else
static qt_mpool generic_rdata_pool = NULL;
# define ALLOC_RDATA() (struct qthread_runtime_data_s *)qt_mpool_alloc(generic_rdata_pool)
# define FREE_RDATA(r) qt_mpool_free(generic_rdata_pool, (r))
#endif /* if defined(UNPOOLED) */

#ifdef NEED_RLIMIT
# define RLIMIT_TO_NORMAL(thr) do {                                              \
        qthread_debug(THREAD_DETAILS,                                            \
                      "t(%p:%u): setting stack size limits back to normal...\n", \
                      (thr), (thr)->thread_id);                                  \
        if (!((thr)->flags & QTHREAD_REAL_MCCOY)) {                              \
            struct rlimit rlp;                                                   \
            rlp.rlim_cur = qlib->master_stack_size;                              \
            rlp.rlim_max = qlib->max_stack_size;                                 \
            qassert(setrlimit(RLIMIT_STACK, &rlp), 0);                           \
        }                                                                        \
} while (0)
# define RLIMIT_TO_TASK(thr)   do {                                                                         \
        struct rlimit rlp;                                                                                  \
        qthread_debug(THREAD_DETAILS,                                                                       \
                      "t(%p:%u): setting stack size limits... hopefully we don't currently exceed them!\n", \
                      (thr), (thr)->thread_id);                                                             \
        if ((thr)->flags & QTHREAD_REAL_MCCOY) {                                                            \
            rlp.rlim_cur = qlib->master_stack_size;                                                         \
        } else {                                                                                            \
            rlp.rlim_cur = qlib->qthread_stack_size;                                                        \
        }                                                                                                   \
        rlp.rlim_max = qlib->max_stack_size;                                                                \
        qassert(setrlimit(RLIMIT_STACK, &rlp), 0);                                                          \
} while (0)
#else /* ifdef NEED_RLIMIT */
# define RLIMIT_TO_NORMAL(thr) do { } while(0)
# define RLIMIT_TO_TASK(thr)   do { } while(0)
#endif /* ifdef NEED_RLIMIT */

#ifdef QTHREAD_DEBUG
enum qthread_debug_levels debuglevel = NO_DEBUG_OUTPUT;
QTHREAD_FASTLOCK_TYPE     output_lock;

int API_FUNC qthread_debuglevel(int d)
{
    if (d >= 0) {
        debuglevel = (enum qthread_debug_levels)((1 << d) - 1);
    }
    return (int)debuglevel;
}

#else
int API_FUNC qthread_debuglevel(int Q_UNUSED d)
{
    return 0;
}

#endif /* ifdef QTHREAD_DEBUG */

#include "qt_profiling.h"

#ifndef QTHREAD_NO_ASSERTS
int   qthread_library_initialized = 0;
void *shep0arg                    = NULL;
#endif

static QINLINE void alloc_rdata(qthread_shepherd_t *me,
                                qthread_t          *t)
{   /*{{{*/
    void                          *stack = NULL;
    struct qthread_runtime_data_s *rdata;

    if (t->flags & QTHREAD_SIMPLE) {
        rdata = t->rdata = ALLOC_RDATA();
    } else {
        stack = ALLOC_STACK();
        assert(stack);
        if (GUARD_PAGES) {
            rdata = t->rdata = (struct qthread_runtime_data_s *)(((uint8_t *)stack) + getpagesize() + qlib->qthread_stack_size);
        } else {
            rdata = t->rdata = (struct qthread_runtime_data_s *)(((uint8_t *)stack) + qlib->qthread_stack_size);
        }
    }
    rdata->tasklocal_size = 0;
    rdata->criticalsect   = 0;
    rdata->stack          = stack;
    rdata->shepherd_ptr   = me;
    rdata->blockedon.io   = NULL;
#ifdef QTHREAD_USE_VALGRIND
    if (stack) {
        rdata->valgrind_stack_id = VALGRIND_STACK_REGISTER(stack, qlib->qthread_stack_size);
    }
#endif
#ifdef QTHREAD_PERFORMANCE
    rdata->performance_data = NULL;
    if(qtperf_should_instrument_qthreads){
      QTPERF_ASSERT(qtperf_qthreads_group != NULL);
      rdata->performance_data = qtperf_create_perfdata(qtperf_qthreads_group);
    }
#endif
} /*}}}*/


/* the qthread_master() function is the loop responsible for actually
 * executing the work units
 *
 * this function is the workhorse of the library: this is the function that
 * gets spawned several times and runs all the qthreads. */
#ifdef QTHREAD_MAKECONTEXT_SPLIT
static void *qthread_master(void *arg);
static void *qthread_master_wrapper(unsigned int high,
                                    unsigned int low)
{                      /*{{{ */
    qthread_shepherd_t *me =
        (qthread_shepherd_t *)((((uintptr_t)high) << 32) | low);

    qthread_debug(SHEPHERD_DETAILS, "high(%x), low(%x): me = %p\n",
                  high, low, me);
    return qthread_master(me);
}

#endif /* ifdef QTHREAD_MAKECONTEXT_SPLIT */
#include <time.h>
extern volatile int *allowed_workers;

static void *qthread_master(void *arg)
{
    qthread_worker_t   *me_worker = (qthread_worker_t *)arg;
    qthread_shepherd_t *me        = (qthread_shepherd_t *)me_worker->shepherd;
    qthread_shepherd_id_t     my_id = me->shepherd_id;
    qt_context_t              my_context;
    qt_threadqueue_t         *threadqueue;
#ifdef QTHREAD_LOCAL_PRIORITY
    qt_threadqueue_t         *localpriorityqueue;
#endif /* ifdef QTHREAD_LOCAL_PRIORITY */
    qt_threadqueue_private_t *localqueue = NULL;
    qthread_t                *t;
    qthread_t               **current;
    int                       done = 0;

#ifdef QTHREAD_SHEPHERD_PROFILING
    me->total_time = qtimer_create();
    qtimer_t idle = qtimer_create();
#endif

    qthread_debug(SHEPHERD_DETAILS, "alive! me = %p\n", me);
    assert(me != NULL);
    assert(me->shepherd_id <= qlib->nshepherds);
    qthread_debug(SHEPHERD_FUNCTIONS, "id(%u): forked with arg %p\n", my_id, arg);
#ifndef QTHREAD_NO_ASSERTS
    if ((shep0arg != NULL) && (my_id == 0)) {
        if (arg != shep0arg) {
            print_error("arg = %p, shep0arg = %p\n", arg, shep0arg);
        }
        assert(arg == shep0arg);
        shep0arg = NULL;
    }
#endif

    /*******************************************************************************/
    /* Initialize myself                                                           */
    /*******************************************************************************/
    TLS_SET(shepherd_structs, arg);
#ifdef QTHREAD_USE_SPAWNCACHE
    localqueue = qt_init_local_spawncache();
#endif
#ifdef QTHREAD_USE_EUREKAS
    qt_eureka_worker_init();
#endif /* QTHREAD_USE_EUREKAS */

    current = &(me_worker->current);

    if (qaffinity && (me->node != UINT_MAX)) {
        qt_affinity_set(me_worker, qlib->nworkerspershep);
    }


    /*******************************************************************************/
    /* Workhorse Loop                                                              */
    /*******************************************************************************/

    threadqueue = me->ready;
#ifdef QTHREAD_LOCAL_PRIORITY
    localpriorityqueue = me->local_priority_queue;
    assert(localpriorityqueue);
#endif /* ifdef QTHREAD_LOCAL_PRIORITY */
    assert(threadqueue);
#ifdef QTHREAD_PERFORMANCE
    QTPERF_WORKER_ENTER_STATE(qthread_internal_getworker()->performance_data, WKR_IDLE);
#endif /*  ifdef QTHREAD_PERFORMANCE  */
    while (!done) {
#ifdef QTHREAD_SHEPHERD_PROFILING
        qtimer_start(idle);
#endif
        qthread_debug(SHEPHERD_DETAILS, "id(%i): fetching a thread from my queue...\n", my_id);

        while (!QTHREAD_CASLOCK_READ_UI(me_worker->active)) {
            SPINLOCK_BODY();
        }
#ifdef QTHREAD_LOCAL_PRIORITY
        t = qt_scheduler_get_thread(threadqueue, localpriorityqueue, localqueue, QTHREAD_CASLOCK_READ_UI(me->active));
#else
        t = qt_scheduler_get_thread(threadqueue, localqueue, QTHREAD_CASLOCK_READ_UI(me->active));
#endif /* ifdef QTHREAD_LOCAL_PRIORITY */
        assert(t);
#ifdef QTHREAD_SHEPHERD_PROFILING
        qtimer_stop(idle);
        me->idle_count++;
        me->idle_time += qtimer_secs(idle);
        if (me->idle_maxtime < qtimer_secs(idle)) {
            me->idle_maxtime = qtimer_secs(idle);
        }
#endif


        qthread_debug(THREAD_DETAILS,
                      "id(%u): dequeued thread %p: id %d/state %d\n",
                      my_id, t, t->thread_id, t->thread_state);

        // Process input preconditions if this is a NASCENT thread
        if (t->thread_state == QTHREAD_STATE_NASCENT) {
            assert(0 && "All preconditions should be satisfied before reaching the main scheduling loop");
            if (qthread_check_feb_preconds(t) == 1) { continue; }
        }

        if (t->thread_state == QTHREAD_STATE_TERM_SHEP) {
#ifdef QTHREAD_SHEPHERD_PROFILING
            if ((my_id != 0)) {
                qtimer_stop(me->total_time);
            }
#endif
            done = 1;
            qthread_thread_free(t); /* free qthread data structures */
        } else {
            /* yielded only happens for the first thread */
            assert((t->thread_state == QTHREAD_STATE_NEW) ||
                   (t->thread_state == QTHREAD_STATE_RUNNING) ||
                   (t->thread_state == QTHREAD_STATE_YIELDED &&
                    t->flags & QTHREAD_REAL_MCCOY));

            assert(t->f != NULL || t->flags & QTHREAD_REAL_MCCOY);
            if (t->rdata == NULL) {
                alloc_rdata(me, t);
            } else {
                assert(t->rdata->shepherd_ptr != NULL);
                if (t->rdata->shepherd_ptr != me) {
                    t->rdata->shepherd_ptr = me;
                }
            }

            if ((t->target_shepherd != NO_SHEPHERD) && (t->target_shepherd != my_id) &&
                QTHREAD_CASLOCK_READ_UI(qlib->shepherds[t->target_shepherd].active)) {
                /* send this thread home */
                qthread_debug(THREAD_DETAILS,
                              "id(%u): thread %u going back home to shep %u\n",
                              my_id, t->thread_id, t->target_shepherd);
                t->rdata->shepherd_ptr = &qlib->shepherds[t->target_shepherd];
                assert(t->rdata->shepherd_ptr->ready != NULL);
                qt_threadqueue_enqueue(qlib->shepherds[t->target_shepherd].ready, t);
            } else if (!QTHREAD_CASLOCK_READ_UI(me->active)) {
                qthread_debug(THREAD_DETAILS,
                              "id(%u): skipping thread exec because I've been disabled!\n",
                              my_id);
                if ((t->target_shepherd == NO_SHEPHERD) || (t->target_shepherd == my_id)) {
                    /* send to the closest shepherd */
                    assert(me->sorted_sheplist);
                    assert(me->shep_dists);
                    t->rdata->shepherd_ptr = qthread_find_active_shepherd(me->sorted_sheplist, me->shep_dists);
                } else {
                    /* find a shepherd somewhere near the preferred shepherd
                     *
                     * Note: if the preferred shep was active, we would have sent
                     * this thread home above */
                    t->rdata->shepherd_ptr = qthread_find_active_shepherd(qlib->shepherds[t->target_shepherd].sorted_sheplist, qlib->shepherds[t->target_shepherd].shep_dists);
                }
                assert(t->rdata->shepherd_ptr);
                if (t->rdata->shepherd_ptr == NULL) {
                    qthread_debug(THREAD_DETAILS,
                                  "id(%u): a new home for thread %i could not be found!\n",
                                  my_id, t->thread_id);
                    t->rdata->shepherd_ptr = me;
                }
                qthread_debug(THREAD_DETAILS,
                              "id(%u): rescheduling thread %i on %i\n",
                              my_id, t->thread_id, t->rdata->shepherd_ptr->shepherd_id);
                assert(t->rdata->shepherd_ptr->ready != NULL);
                qt_threadqueue_enqueue(t->rdata->shepherd_ptr->ready, t);
            } else {           /* me->active */
#ifdef QTHREAD_SHEPHERD_PROFILING
                if (t->thread_state == QTHREAD_STATE_NEW) {
                    me->num_threads++;
                }
#endif

                *current = t;

#ifdef HAVE_NATIVE_MAKECONTEXT
                getcontext(&my_context);
#endif
                qthread_debug(THREAD_DETAILS, "id(%u): about to exec thread. shepherd context is %p\n", my_id, &my_context);
                qthread_exec(t, &my_context);

                t = *current; // necessary for direct-swap sanity
                *current = NULL; // neessary for "queue sanity"
#ifdef QTHREAD_USE_EUREKAS
                *current = NULL; // necessary for eureka sanity
#endif /* QTHREAD_USE_EUREKAS */

                qthread_debug(THREAD_DETAILS, "id(%u): back from qthread_exec, state is %i\n", my_id, t->thread_state);
                /* now clean up, based on the thread's state */
                switch (t->thread_state) {
                    case QTHREAD_STATE_MIGRATING:
                        qthread_debug(THREAD_DETAILS | AFFINITY_DETAILS | SHEPHERD_DETAILS,
                                      "id(%u): thread %u migrating to shep %u\n",
                                      my_id, t->thread_id,
                                      t->target_shepherd);
                        t->thread_state        = QTHREAD_STATE_RUNNING;
                        t->rdata->shepherd_ptr = &qlib->shepherds[t->target_shepherd];
#ifdef QTHREAD_PERFORMANCE
                        QTPERF_QTHREAD_ENTER_STATE(t->rdata->performance_data, QTHREAD_STATE_RUNNING);
#endif /* ifdef QTHREAD_PERFORMANCE */
                        assert(t->rdata->shepherd_ptr->ready != NULL);
                        qt_threadqueue_enqueue(t->rdata->shepherd_ptr->ready, t);
                        break;
                    default:
                        qthread_debug(ALWAYS_OUTPUT, "id(%u): thread in state %i; that's illegal!\n", my_id, t->thread_state);
                        assert(0);
                        break;

                    case QTHREAD_STATE_YIELDED_NEAR: /* reschedule it */
                        t->thread_state = QTHREAD_STATE_RUNNING;
#ifdef QTHREAD_PERFORMANCE
                        QTPERF_QTHREAD_ENTER_STATE(t->rdata->performance_data, QTHREAD_STATE_RUNNING);
#endif /*  ifdef QTHREAD_PERFORMANCE */
                        qthread_debug(THREAD_DETAILS | SHEPHERD_DETAILS,
                                      "id(%u): thread %i yielded near; rescheduling\n",
                                      my_id, t->thread_id);
#ifdef QTHREAD_USE_SPAWNCACHE
                        if (!qt_spawncache_yield(t))
#endif
                        {
#ifdef QTHREAD_LOCAL_PRIORITY
                            qthread_t *f = qt_scheduler_get_thread(threadqueue, localpriorityqueue, NULL, 
                                                                   QTHREAD_CASLOCK_READ_UI(me->active));
#else
                            qthread_t *f = qt_scheduler_get_thread(threadqueue, NULL, 
                                                                   QTHREAD_CASLOCK_READ_UI(me->active));
#endif /* ifdef QTHREAD_LOCAL_PRIORITY */                          
                            qt_threadqueue_enqueue(me->ready, t);
                            qt_threadqueue_enqueue(me->ready, f);
                        }
                        break;
                    case QTHREAD_STATE_YIELDED: /* reschedule it */
                        t->thread_state = QTHREAD_STATE_RUNNING;
#ifdef QTHREAD_PERFORMANCE
                        QTPERF_QTHREAD_ENTER_STATE(t->rdata->performance_data, QTHREAD_STATE_RUNNING);
#endif /* ifdef QTHREAD_PERFORMANCE */
                        qthread_debug(THREAD_DETAILS | SHEPHERD_DETAILS,
                                      "id(%u): thread %i yielded; rescheduling\n",
                                      my_id, t->thread_id);
                        assert(me->ready != NULL);
                        qt_threadqueue_enqueue_yielded(me->ready, t);
                        break;

                    case QTHREAD_STATE_QUEUE:
                        {
                            qthread_queue_t q = t->rdata->blockedon.queue;
                            qthread_debug(THREAD_DETAILS | SHEPHERD_DETAILS,
                                    "id(%u): thread tid=%i(%p) entering user queue (q=%p, type=%u)\n",
                                    my_id, t->thread_id, t, q, q->type);
                            assert(q);
                            qthread_queue_internal_enqueue(q, t);
                            break;
                        }
                    case QTHREAD_STATE_FEB_BLOCKED: /* unlock the related FEB address locks, and re-arrange memory to be correct */
                    {
                        qthread_addrstat_t *m = t->rdata->blockedon.addr;
                        qthread_debug(THREAD_DETAILS | FEB_DETAILS | SHEPHERD_DETAILS,
                                      "id(%u): thread tid=%i(%p) blocked on FEB (m=%p, EFQ=%p)\n",
                                      my_id, t->thread_id, t, m, m->EFQ);
                        QTHREAD_FASTLOCK_UNLOCK(&(m->lock));
                        break;
                    }

                    case QTHREAD_STATE_PARENT_YIELD:
                        t->thread_state = QTHREAD_STATE_PARENT_BLOCKED;
#ifdef QTHREAD_PERFORMANCE
                        QTPERF_QTHREAD_ENTER_STATE(t->rdata->performance_data, QTHREAD_STATE_PARENT_BLOCKED);
#endif /* ifdef QTHREAD_PERFORMANCE */
                        break;

                    case QTHREAD_STATE_PARENT_BLOCKED:
                        qthread_debug(THREAD_DETAILS, "id(%u): thread in state %i; that's illegal!\n", my_id, t->thread_state);
                        assert(0);
                        break;

                    case QTHREAD_STATE_PARENT_UNBLOCKED:
                        qthread_debug(THREAD_DETAILS, "id(%u): thread in state %i; that's illegal!\n", my_id, t->thread_state);
                        assert(0);
                        break;
                    case QTHREAD_STATE_SYSCALL:
                        t->thread_state = QTHREAD_STATE_RUNNING;
#ifdef QTHREAD_PERFORMANCE
                        QTPERF_QTHREAD_ENTER_STATE(t->rdata->performance_data, QTHREAD_STATE_RUNNING);
#endif /*  ifdef QTHREAD_PERFORMANCE */
                        qthread_debug(THREAD_DETAILS | IO_DETAILS | SHEPHERD_DETAILS,
                                      "id(%u): thread %i made a syscall\n",
                                      my_id, t->thread_id);
                        qt_blocking_subsystem_enqueue(t->rdata->blockedon.io);
                        break;
#ifdef QTHREAD_USE_EUREKAS
                    case QTHREAD_STATE_ASSASSINATED:
                        qthread_debug(THREAD_DETAILS | SHEPHERD_DETAILS,
                                      "id(%u): thread %i assassinated\n",
                                      my_id, t->thread_id);
                        qthread_internal_assassinate(t);
                        /* now, we're done cleaning, so we can unblock the assassination signal */
                        {
                            sigset_t iset;
                            qassert(sigemptyset(&iset), 0);
                            qassert(sigaddset(&iset, QT_ASSASSINATE_SIGNAL), 0);
                            qassert(sigaddset(&iset, QT_EUREKA_SIGNAL), 0);
                            qassert(sigprocmask(SIG_UNBLOCK, &iset, NULL), 0);
                        }
                        break;
#endif /* QTHREAD_USE_EUREKAS */
                    case QTHREAD_STATE_TERMINATED:
                        qthread_debug(THREAD_DETAILS | SHEPHERD_DETAILS,
                                      "id(%u): thread %i terminated\n",
                                      my_id, t->thread_id);
                        /* we can remove the stack etc. */
                        Q_PREFETCH(threadqueue);
                        qthread_thread_free(t);
                        break;
                }
            }
        }
    }
#ifdef QTHREAD_PERFORMANCE
    QTPERF_WORKER_ENTER_STATE(qthread_internal_getworker()->performance_data, WKR_SHEPHERD);
#endif /* ifdef QTHREAD_PERFORMANCE  */

#ifdef QTHREAD_SHEPHERD_PROFILING
    qtimer_destroy(idle);
#endif
    qthread_debug(SHEPHERD_DETAILS, "id(%u): wkr(%u): finished\n",
                  my_id, me_worker->worker_id);
    pthread_exit(NULL);
    return NULL;
}                      /*}}} */

/* By default allow merging of tasks of the same kind
 * Rely on the limitation put on max number of tasks 
 * allowed to be aggregated from a queue 
 * (relative to available work per worker) */
int qthread_default_agg_cost (int count, qthread_f* f, void **arg){   
    if(f[count] != f[0])
        return qlib->max_c+1;
    return qlib->max_c-1;
}

void qthread_default_agg_f (int count, qthread_f* f, void** arg, void** ret, uint16_t flags){
    int i;
    for(i=count-1; i>=0; i--){
        qthread_call_method(f[i], arg[i], ret[i], flags);
    }
}

int API_FUNC qthread_init(qthread_shepherd_id_t nshepherds)
{                      /*{{{ */
    char newenv[100];

    snprintf(newenv, 99, "%i", (int)nshepherds);
    qassert(setenv("QT_NUM_SHEPHERDS", newenv, 1), 0);
    qassert(setenv("QT_NUM_WORKERS_PER_SHEPHERD", "1", 1), 0);
    return qthread_initialize();
}                      /*}}} */

/**
 *
 *
 *
 * Use these functions to initialize the qthreads environment before using any
 * other qthread functions. The <b>qthread_init()</b> function is deprecated in
 * favor of <b>qthread_initialize()</b> which will attempt to auto-detect the
 * correct number of shepherds for the system. The number of shepherds can be
 * forcibly specified with the environment variable QTHREAD_NUM_SHEPHERDS. The
 * <b>qthread_init()</b> function is a wrapper around
 * <b>qthread_initialize()</b> that simply exports the <b>QTHREAD_NUM_SHEPHERDS</b>
 * environment variable. If <b>QTHREAD_NUM_SHEPHERDS</b> is 0 or unset, the library
 * will attempt to guess the correct number of shepherds, defaulting to a single
 * shepherd if no information about the system could be found. Shepherds will
 * attempt to pin themselves to processors using whatever CPU affinity libraries
 * are available.
 * qthread_initialize_agg is another wrapper, receiving the method that can aggregate tasks.
 * By default, qthreads provide such a method for spawned tasks marked as AGGREGABLE.

 * @name qthread_initialize
 *
 * @synopsis
 *
 * @return int On success, the system is ready to fork threads and 0 is
 *         returned. On error, an non-zero error code is returned.
 *
 * @error ENOMEM Not enough memory could be allocated.
 */

int API_FUNC qthread_initialize_agg(int(*agg_cost) (int count, qthread_f* f, void **arg),
                                    qthread_agg_f agg_f, int max_c)
{
    int r = qthread_initialize();
    if(agg_cost != NULL)
        qlib->agg_cost = agg_cost;
    if(agg_f != NULL)
        qlib->agg_f    = agg_f;
    if(max_c >=0)
        qlib->max_c = max_c;
    return r;
}

int API_FUNC qthread_initialize(void)
{                      /*{{{ */
    int                   r;
    size_t                i;
    uint_fast8_t          print_info      = 0;
    uint_fast8_t          need_sync       = 1;
    qthread_shepherd_id_t nshepherds      = 0;
    qthread_worker_id_t   nworkerspershep = 0;
    size_t                hw_par          = 0;
    extern unsigned int QTHREAD_LOCKING_STRIPES;
    //qtlog(1,"qthread_initialize");
    print_info = qt_internal_get_env_num("INFO", 0, 1);

    QTHREAD_FASTLOCK_SETUP();
#ifdef QTHREAD_DEBUG
    QTHREAD_FASTLOCK_INIT(output_lock);
    {
        unsigned long dl = qt_internal_get_env_num("DEBUG_LEVEL", 0, 0);
        debuglevel = (enum qthread_debug_levels)((1 << dl) - 1);
    }
#endif /* ifdef QTHREAD_DEBUG */

    qthread_debug(CORE_CALLS, "began.\n");
    if (qlib != NULL) {
        qthread_debug(CORE_DETAILS, "redundant call\n");
        return QTHREAD_SUCCESS;
    }
    qlib = (qlib_t)MALLOC(sizeof(struct qlib_s));
    qassert_ret(qlib, QTHREAD_MALLOC_ERROR);

#if defined(QTHREAD_MUTEX_INCREMENT) || (QTHREAD_ASSEMBLY_ARCH == QTHREAD_POWERPC32)
    qlib->atomic_locks = MALLOC(sizeof(QTHREAD_FASTLOCK_TYPE) * QTHREAD_LOCKING_STRIPES);
    qassert_ret(qlib->atomic_locks, QTHREAD_MALLOC_ERROR);
    for (i = 0; i < QTHREAD_LOCKING_STRIPES; i++) {
        QTHREAD_FASTLOCK_INIT(qlib->atomic_locks[i]);
    }
#endif

    qt_internal_alignment_init();
    qt_hash_initialize_subsystem();

    qt_topology_init(&nshepherds,
                     &nworkerspershep,
                     &hw_par);

    if ((nshepherds == 1) && (nworkerspershep == 1)) {
        need_sync = 0;
    }
    QTHREAD_LOCKING_STRIPES = 2 << (QT_INT_LOG(nshepherds * nworkerspershep) + 1);
    qthread_debug(CORE_BEHAVIOR, "there will be %u shepherd(s)\n", (unsigned)nshepherds);

#ifdef QTHREAD_COUNT_THREADS
    threadcount                = 1;
    maxconcurrentthreads       = 1;
    maxeffconcurrentthreads    = 1;
    concurrentthreads          = 1;
    effconcurrentthreads       = 1;
    avg_concurrent_threads     = 1;
    avg_eff_concurrent_threads = 1;
    QTHREAD_FASTLOCK_INIT(concurrentthreads_lock);
    QTHREAD_FASTLOCK_INIT(effconcurrentthreads_lock);
#endif

#ifdef CAS_STEAL_PROFILE
    qlib->cas_steal_profile =
        qt_internal_aligned_alloc(sizeof(uint64_strip_t) * nshepherds * nworkerspershep, 64);
#endif

    /* initialize the kernel threads and scheduler */
    TLS_INIT(shepherd_structs);
#ifdef QTHREAD_USE_SPAWNCACHE
    qt_spawncache_init();
#endif
    qlib->nshepherds        = nshepherds;
    qlib->nworkerspershep   = nworkerspershep;
    qlib->nshepherds_active = nshepherds;
    qlib->shepherds         = (qthread_shepherd_t *) qt_calloc(nshepherds,
                                                               sizeof(qthread_shepherd_t));
    qlib->threadqueues      = (qt_threadqueue_t **)MALLOC(nshepherds * sizeof(qt_threadqueue_t *));
#ifdef QTHREAD_LOCAL_PRIORITY
    qlib->local_priority_queues = (qt_threadqueue_t **)MALLOC(nshepherds * sizeof(qt_threadqueue_t *));
#endif /* ifdef QTHREAD_LOCAL_PRIORITY */
    qassert_ret(qlib->shepherds, QTHREAD_MALLOC_ERROR);
#ifdef QTHREAD_MUTEX_INCREMENT
    QTHREAD_FASTLOCK_INIT(qlib->nshepherds_active_lock);
    QTHREAD_FASTLOCK_INIT(qlib->nworkers_active_lock);
#endif

    qt_mpool_subsystem_init();

    qlib->qthread_stack_size = qt_internal_get_env_num("STACK_SIZE",
                                                       QTHREAD_DEFAULT_STACK_SIZE,
                                                       QTHREAD_DEFAULT_STACK_SIZE);
    qthread_debug(CORE_DETAILS, "qthread stack size: %u\n", qlib->qthread_stack_size);
#ifdef QTHREAD_GUARD_PAGES
    GUARD_PAGES = qt_internal_get_env_bool("GUARD_PAGES", 1);
#endif
    if (GUARD_PAGES) {
        if (print_info) {
            print_status("Guard Pages Enabled\n");
        }
        /* round stack size to nearest page */
        if (qlib->qthread_stack_size % pagesize) {
            qlib->qthread_stack_size +=
                pagesize - (qlib->qthread_stack_size % pagesize);
        }
    }
    if (print_info) {
        print_status("Using %u byte stack size.\n", qlib->qthread_stack_size);
    }


    qlib->max_thread_id  = 1;
    qlib->max_unique_id  = 1;
    qlib->sched_shepherd = 0;
    QTHREAD_FASTLOCK_INIT(qlib->max_thread_id_lock);
    QTHREAD_FASTLOCK_INIT(qlib->max_unique_id_lock);
    QTHREAD_FASTLOCK_INIT(qlib->sched_shepherd_lock);
    {
        struct rlimit rlp;

        qassert(getrlimit(RLIMIT_STACK, &rlp), 0);
        qthread_debug(CORE_DETAILS, "stack sizes ... cur: %u max: %u\n", rlp.rlim_cur, rlp.rlim_max);
        if ((rlp.rlim_cur == RLIM_INFINITY) || (rlp.rlim_cur == 0)) {
            if (rlp.rlim_cur == 0) {
                qthread_debug(CORE_DETAILS, "correcting stack cur (you have a broken OS)...\n");
            }
            rlp.rlim_cur = 8 * 1024 * 1024;
        }
        qlib->master_stack_size = rlp.rlim_cur;
        if (rlp.rlim_max < rlp.rlim_cur) {
            qthread_debug(CORE_DETAILS, "correcting stack max (you have a broken OS)...\n");
            rlp.rlim_max = rlp.rlim_cur;
        }
        qlib->max_stack_size = rlp.rlim_max;
    }

    /* initialize the shepherds as having no affinity */
    for (i = 0; i < nshepherds; i++) {
        qlib->shepherds[i].node            = -1;
        qlib->shepherds[i].shep_dists      = NULL;
        qlib->shepherds[i].sorted_sheplist = NULL;
        qlib->shepherds[i].workers = (qthread_worker_t *) qt_calloc(nworkerspershep,
                                                                    sizeof(qthread_worker_t));
        qassert_ret(qlib->shepherds[i].workers, QTHREAD_MALLOC_ERROR);
    }
    qaffinity = qt_internal_get_env_bool("AFFINITY", 1);
    qthread_debug(AFFINITY_DETAILS, "qaffinity = %i\n", qaffinity);
#ifndef QTHREAD_NO_ASSERTS
    qthread_library_initialized = 1;
    MACHINE_FENCE;
#endif
    {
        int ret = qt_affinity_gendists(qlib->shepherds, nshepherds);
        if (ret != QTHREAD_SUCCESS) {
            qthread_debug(AFFINITY_DETAILS, "gendists returned non-success (%i)\n", (int)ret);
            return ret;
        }
        assert(qlib->shepherds[0].sorted_sheplist);
        assert(qlib->shepherds[0].shep_dists);
    }

    // Set task argument buffer size
    qlib->qthread_argcopy_size = qt_internal_get_env_num("ARGCOPY_SIZE", ARGCOPY_DEFAULT, 0);
    qthread_debug(CORE_DETAILS, "qthread task argcopy size: %u\n", (unsigned)qlib->qthread_argcopy_size);

    // Set task-local data size
    qlib->qthread_tasklocal_size = qt_internal_get_env_num("TASKLOCAL_SIZE",
                                                           TASKLOCAL_DEFAULT,
                                                           sizeof(void *));
    qthread_debug(CORE_DETAILS, "qthread task-local size: %u\n", qlib->qthread_tasklocal_size);

#ifndef UNPOOLED
    generic_qthread_pool     = qt_mpool_create_aligned(sizeof(qthread_t) + sizeof(void *) + qlib->qthread_tasklocal_size, qthread_cacheline());
    generic_big_qthread_pool = qt_mpool_create(sizeof(qthread_t) + qlib->qthread_argcopy_size + qlib->qthread_tasklocal_size);
    if (GUARD_PAGES) {
        generic_stack_pool =
            qt_mpool_create_aligned(qlib->qthread_stack_size + sizeof(struct qthread_runtime_data_s) +
                                    (2 * getpagesize()), getpagesize());
    } else {
        generic_stack_pool = qt_mpool_create_aligned(qlib->qthread_stack_size + sizeof(struct qthread_runtime_data_s), QTHREAD_STACK_ALIGNMENT);     // stacks on most platforms must be 16-byte aligned (or less)
    }
    generic_rdata_pool = qt_mpool_create(sizeof(struct qthread_runtime_data_s));
#endif /* ifndef UNPOOLED */
    initialize_hazardptrs();
    qt_internal_teams_init();
    qthread_queue_subsystem_init();
    qt_feb_subsystem_init(need_sync);
    qt_syncvar_subsystem_init(need_sync);
    qt_threadqueue_subsystem_init();
    qt_blocking_subsystem_init();

/* Set up agg methods*/
    qlib->agg_cost = qthread_default_agg_cost;
    qlib->agg_f    = qthread_default_agg_f;
/* initialize the shepherd structures */
    for (i = 0; i < nshepherds; i++) {
        qthread_debug(SHEPHERD_DETAILS, "setting up shepherd %lu (%p)\n", i, &qlib->shepherds[i]);
        qlib->shepherds[i].shepherd_id = (qthread_shepherd_id_t)i;
        QTHREAD_CASLOCK_INIT(qlib->shepherds[i].active, 1);
        qlib->shepherds[i].ready = qt_threadqueue_new();
#ifdef QTHREAD_LOCAL_PRIORITY
        qlib->shepherds[i].local_priority_queue = qt_threadqueue_new();
#endif /* ifdef QTHREAD_LOCAL_PRIORITY */
        qassert_ret(qlib->shepherds[i].ready, QTHREAD_MALLOC_ERROR);
        qlib->threadqueues[i] = qlib->shepherds[i].ready;
#ifdef QTHREAD_LOCAL_PRIORITY
        qlib->local_priority_queues[i] = qlib->shepherds[i].local_priority_queue;
#endif /* ifdef QTHREAD_LOCAL_PRIORITY */
#ifdef QTHREAD_FEB_PROFILING
# ifdef QTHREAD_MUTEX_INCREMENT
        qlib->shepherds[i].uniqueincraddrs = qt_hash_create(need_sync);
# endif
        qlib->shepherds[i].uniquelockaddrs = qt_hash_create(need_sync);
        qlib->shepherds[i].uniquefebaddrs  = qt_hash_create(need_sync);
#endif

        qthread_debug(SHEPHERD_DETAILS, "shepherd %i set up (%p)\n", i, &qlib->shepherds[i]);
    }
    qthread_debug(SHEPHERD_DETAILS, "done setting up shepherds.\n");

/* now, transform the current main context into a qthread,
 * and make the main thread a shepherd (shepherd 0).
 * What will happen is this:
 * qlib->mccoy_thread represents the original execution thread, and so will
 * receive a context based on the current execution state.
 * qlib->master_context is a context for the new shepherd that will be
 * created (shepherd 0).
 * qlib->master_stack is a stack for that shepherd, and is huge, because
 * the shepherd expects a "standard" size stack. The mccoy_thread, as it is
 * for the *current* thread, also expects a full-size stack. The point of
 * this weirdness is so that the current thread can block the same way that
 * a qthread can. */
    qthread_debug(SHEPHERD_DETAILS, "allocating shep0\n");
    qlib->mccoy_thread = qthread_thread_new(NULL, NULL, 0, NULL, NULL, 0);
    qthread_debug(CORE_DETAILS, "mccoy thread = %p\n", qlib->mccoy_thread);
    qassert_ret(qlib->mccoy_thread, QTHREAD_MALLOC_ERROR);

    qthread_debug(CORE_DETAILS, "master_context = %p\n", &(qlib->master_context));
    qthread_debug(CORE_DETAILS, "master_stack_size = %u\n", (unsigned)(qlib->master_stack_size));
    qlib->master_stack = qt_internal_aligned_alloc(qlib->master_stack_size, QTHREAD_STACK_ALIGNMENT);
    qassert_ret(qlib->master_stack, QTHREAD_MALLOC_ERROR);
    qthread_debug(CORE_DETAILS, "master_stack = %p\n", qlib->master_stack);
#ifdef QTHREAD_USE_VALGRIND
    qlib->valgrind_masterstack_id =
        VALGRIND_STACK_REGISTER(qlib->master_stack, qlib->master_stack_size);
#endif

/* the context will have its own stack ptr */
    qlib->mccoy_thread->thread_state = QTHREAD_STATE_YIELDED;                    /* avoid re-launching */
    qlib->mccoy_thread->flags        = QTHREAD_REAL_MCCOY | QTHREAD_UNSTEALABLE; /* i.e. this is THE parent thread */
    assert(qlib->mccoy_thread->rdata == NULL);

    qlib->mccoy_thread->rdata = MALLOC(sizeof(struct qthread_runtime_data_s));

#ifdef QTHREAD_PERFORMANCE
    if(qtperf_should_instrument_qthreads){
      QTPERF_ASSERT(qtperf_qthreads_group != NULL);
      qlib->mccoy_thread->rdata->performance_data = qtperf_create_perfdata(qtperf_qthreads_group);
    }
    QTPERF_QTHREAD_ENTER_STATE(qlib->mccoy_thread->rdata->performance_data, QTHREAD_STATE_YIELDED);
#endif  /* endif QTHREAD_PERFORMANCE */
    assert(qlib->mccoy_thread->rdata != NULL);
    qlib->mccoy_thread->rdata->shepherd_ptr   = &(qlib->shepherds[0]);
    qlib->mccoy_thread->rdata->stack          = NULL;
    qlib->mccoy_thread->rdata->tasklocal_size = 0;

    qthread_debug(CORE_DETAILS, "enqueueing mccoy thread\n");
    TLS_SET(shepherd_structs, (qthread_shepherd_t *)&(qlib->shepherds[0].workers[0]));
    qt_threadqueue_enqueue(qlib->shepherds[0].ready, qlib->mccoy_thread);
    qassert(getcontext(&(qlib->mccoy_thread->rdata->context)), 0);
/* now build the context for the shepherd 0 */
    qthread_debug(CORE_DETAILS, "calling qthread_makecontext\n");
    qlib->shepherds[0].workers[0].worker   = pthread_self();
    qlib->shepherds[0].workers[0].shepherd = &qlib->shepherds[0];
    QTHREAD_CASLOCK_INIT(qlib->shepherds[0].workers[0].active, 1);
    qthread_debug(CORE_DETAILS, "initialized caslock 0,0 %p\n", &qlib->shepherds[0].workers[0].active);
    qlib->shepherds[0].workers[0].worker_id = 0;
    qlib->shepherds[0].workers[0].unique_id = qthread_internal_incr(&(qlib->max_unique_id),
                                                                    &qlib->max_unique_id_lock, 1);
#ifdef QTHREAD_PERFORMANCE
    if(qtperf_should_instrument_workers){
      QTPERF_ASSERT(qtperf_workers_group != NULL);
      qlib->shepherds[0].workers[0].performance_data = qtperf_create_perfdata(qtperf_workers_group);
      QTPERF_ASSERT(qlib->shepherds[0].workers[0].performance_data != NULL);
    }
#endif
    qthread_makecontext(&(qlib->master_context), qlib->master_stack,
                        qlib->master_stack_size,
#ifdef QTHREAD_MAKECONTEXT_SPLIT
                        (void (*)(void))qthread_master_wrapper,
#else
                        (void (*)(void))qthread_master,
#endif
                        &(qlib->shepherds[0].workers[0]),
                        &(qlib->mccoy_thread->rdata->context));
#ifndef QTHREAD_NO_ASSERTS
    shep0arg = &(qlib->shepherds[0].workers[0]);
#endif
/* this launches shepherd 0 */
    qthread_debug(CORE_DETAILS | SHEPHERD_DETAILS, "launching shepherd 0\n");
#ifdef QTHREAD_USE_VALGRIND
    VALGRIND_CHECK_MEM_IS_ADDRESSABLE(&qlib->mccoy_thread->rdata->context, sizeof(qt_context_t));
    VALGRIND_CHECK_MEM_IS_ADDRESSABLE(&(qlib->master_context), sizeof(qt_context_t));
    VALGRIND_MAKE_MEM_DEFINED(&qlib->mccoy_thread->rdata->context, sizeof(qt_context_t));
    VALGRIND_MAKE_MEM_DEFINED(&(qlib->master_context), sizeof(qt_context_t));
#endif
    qthread_debug(CORE_DETAILS, "calling swapcontext into master_context\n");
#ifdef HAVE_NATIVE_MAKECONTEXT
    qassert(swapcontext(&qlib->mccoy_thread->rdata->context, &(qlib->master_context)), 0);
#else
    qassert(qt_swapctxt(&qlib->mccoy_thread->rdata->context, &(qlib->master_context)), 0);
#endif
    qthread_debug(CORE_DETAILS, "back from master_context\n");

    assert(hw_par > 0);
    qlib->nworkers_active = hw_par;

/* spawn the shepherds */
    /*  XXX: where does WKR_DBG come from? */
#ifdef QTHREAD_PERFORMANCE
    qtlogargs(WKR_DBG, "nshepherds = %i, nworkerspershep = %i", nshepherds, nworkerspershep);
#endif /*  ifdef QTHREAD_PERFORMANCE */
    for (i = 0; i < nshepherds; ++i) {
        qthread_worker_id_t j;
        qthread_debug(SHEPHERD_DETAILS,
                      "forking workers for shepherd %i (%p)\n", i,
                      &qlib->shepherds[i]);
        for (j = 0; j < nworkerspershep; ++j) {
            qlib->shepherds[i].workers[j].nostealbuffer = qt_calloc(STEAL_BUFFER_LENGTH,
                                                                    sizeof(qthread_t *));
            qlib->shepherds[i].workers[j].stealbuffer = qt_calloc(STEAL_BUFFER_LENGTH,
                                                                  sizeof(qthread_t *));
            # ifdef QTHREAD_PERFORMANCE
            QTPERF_ASSERT(((qtperf_should_instrument_workers != 0 &&  (qtperf_workers_group != NULL)) ||
                           (qtperf_should_instrument_workers == 0 && (qtperf_workers_group == NULL))));
            // only add perfdata struct if we're watching, and if it
            // hasn't been done (which should be the case for all but
            // the master thread)
            if(qtperf_should_instrument_workers && qlib->shepherds[i].workers[j].performance_data == NULL){
              QTPERF_ASSERT(qtperf_workers_group != NULL);
              qlib->shepherds[i].workers[j].performance_data = qtperf_create_perfdata(qtperf_workers_group);
              QTPERF_ASSERT(qlib->shepherds[i].workers[j].performance_data != NULL);
              QTPERF_WORKER_ENTER_STATE(qlib->shepherds[i].workers[j].performance_data, WKR_INIT);
              qtlogargs(WKR_DBG, "created performance data for worker %i, shepherd %lu\n", j, i);
            }
# endif /* ifdef QTHREAD_PERFORMANCE */

            if ((i == 0) && (j == 0)) {
                continue;                       // original pthread becomes shep 0 worker 0
            }
            qlib->shepherds[i].workers[j].shepherd  = &qlib->shepherds[i];
            qlib->shepherds[i].workers[j].worker_id = j;
            qlib->shepherds[i].workers[j].unique_id = qthread_internal_incr(&(qlib->max_unique_id),
                                                                            &qlib->max_unique_id_lock, 1);
            qlib->shepherds[i].workers[j].packed_worker_id = j + (i * nworkerspershep);

            if ((j * nshepherds) + i + 1 > hw_par) {
                qthread_debug(CORE_DETAILS, "deactivate shep %i's worker %i\n", (int)i, (int)j);
                QTHREAD_CASLOCK_INIT(qlib->shepherds[i].workers[j].active, 0);
            } else {
                qthread_debug(CORE_DETAILS, "activate shep %i's worker %i\n", (int)i, (int)j);
                QTHREAD_CASLOCK_INIT(qlib->shepherds[i].workers[j].active, 1);
            }
            qthread_debug(CORE_DETAILS, "initialized caslock %i,%i %p\n", i, j, &qlib->shepherds[i].workers[j].active);
            qlib->shepherds[i].workers[j].shepherd = &qlib->shepherds[i];
            if ((r = pthread_create(&qlib->shepherds[i].workers[j].worker, NULL,
                                    qthread_master, &qlib->shepherds[i].workers[j])) != 0) {
                print_error("qthread_init: pthread_create() failed (%d): %s\n", r, strerror(errno));
                return QTHREAD_THIRD_PARTY_ERROR;
            }
            qthread_debug(SHEPHERD_DETAILS, "spawned shep %i worker %i\n", (int)i, (int)j);
        }
    }
    
#ifdef QTHREAD_PERFORMANCE
    QTPERF_ASSERT(qtperf_check_invariants());
#endif /* ifdef QTHREAD_PERFORMANCE */
    
    qthread_debug(CORE_DETAILS, "calling atexit\n");
    atexit(qthread_finalize);

    qthread_debug(CORE_DETAILS, "calling component init functions\n");
    qt_barrier_internal_init();

#ifdef QTHREAD_MULTINODE
    if (NULL != qt_internal_get_env_str("MULTINODE", NULL)) {
        qthread_multinode_initialize();
    }
#endif

    qthread_debug(CORE_DETAILS, "finished.\n");
    return QTHREAD_SUCCESS;
}                      /*}}} */

/* This initializes a context (c) to run the function (func) with a single
 * argument (arg). This is just a wrapper around makecontext that isolates some
 * of the portability garbage. */
static QINLINE void qthread_makecontext(qt_context_t *const c,
                                        void *const         stack,
                                        const size_t        stacksize,
                                        void                (*func)(void),
                                        const void *const   arg,
                                        qt_context_t *const returnc)
{                      /*{{{ */
#ifdef QTHREAD_MAKECONTEXT_SPLIT
    const unsigned int high = ((uintptr_t)arg) >> 32;
    const unsigned int low  = ((uintptr_t)arg) & 0xffffffff;
#endif

    assert(c != NULL);
    assert(stack != NULL);
    assert(func != NULL);
    assert(arg != NULL);
    assert(returnc != NULL);

    qassert(getcontext(c), 0); /* initialization required by makecontext API */

    /* Several other libraries that do this reserve a few words on either end
     * of the stack for some reason. To avoid problems, I'll also do this (even
     * though I have no idea why they do this). */
#ifdef INVERSE_STACK_POINTER
    c->uc_stack.ss_sp = (char *)(stack) + stacksize - 8;
#else
    c->uc_stack.ss_sp = (uint8_t *)(stack) + 8;
#endif
    c->uc_stack.ss_size = stacksize - 64;
#ifdef UCSTACK_HAS_SSFLAGS
    c->uc_stack.ss_flags = 0;
#endif
#ifdef HAVE_NATIVE_MAKECONTEXT
    /* the makecontext man page (Linux) says: set the uc_link FIRST.
     * why? no idea */
    c->uc_link = returnc;          /* NULL pthread_exit() */
#endif
#ifdef HAVE_NATIVE_MAKECONTEXT
# ifdef QTHREAD_MAKECONTEXT_SPLIT
#  ifdef EXTRA_MAKECONTEXT_ARGC
    makecontext(c, func, 3, high, low);
#  else
    makecontext(c, func, 2, high, low);
#  endif /* EXTRA_MAKECONTEXT_ARGC */
# else /* QTHREAD_MAKECONTEXT_SPLIT */
#  ifdef EXTRA_MAKECONTEXT_ARGC
    makecontext(c, func, 2, arg);
#  else
    makecontext(c, func, 1, arg);
#  endif /* EXTRA_MAKECONTEXT_ARGC */
# endif  /* QTHREAD_MAKECONTEXT_SPLIT */
    assert((void *)c->uc_link == (void *)returnc);
#else /* ifdef HAVE_NATIVE_MAKECONTEXT */
    qt_makectxt(c, func, 1, arg);
#endif /* ifdef HAVE_NATIVE_MAKECONTEXT */
}                      /*}}} */

/* this adds a function to the list of cleanup functions to call at finalize;
 * these functions get called *way* AFTER shepherds are torn down */
void INTERNAL qthread_internal_cleanup_late(void (*function)(void))
{   /*{{{*/
    struct qt_cleanup_funcs_s *ng = MALLOC(sizeof(struct qt_cleanup_funcs_s));

    assert(ng);
    ng->func              = function;
    ng->next              = qt_cleanup_late_funcs;
    qt_cleanup_late_funcs = ng;
} /*}}}*/

/* this adds a function to the list of cleanup functions to call at finalize;
 * these functions get called AFTER shepherds are torn down */
void INTERNAL qthread_internal_cleanup(void (*function)(void))
{   /*{{{*/
    struct qt_cleanup_funcs_s *ng = MALLOC(sizeof(struct qt_cleanup_funcs_s));

    assert(ng);
    ng->func         = function;
    ng->next         = qt_cleanup_funcs;
    qt_cleanup_funcs = ng;
} /*}}}*/

/* this adds a function to the list of cleanup functions to call at finalize;
 * these functions get called BEFORE shepherds are torn down */
void INTERNAL qthread_internal_cleanup_early(void (*function)(void))
{   /*{{{*/
    struct qt_cleanup_funcs_s *ng = MALLOC(sizeof(struct qt_cleanup_funcs_s));

    assert(ng);
    ng->func               = function;
    ng->next               = qt_cleanup_early_funcs;
    qt_cleanup_early_funcs = ng;
} /*}}}*/

#ifdef QTHREAD_DEBUG
static void qt_hash_print_addrstat(const qt_key_t      addr,
                                   qthread_addrstat_t *m,
                                   void               *arg)
{                                      /*{{{ */
    printf("addr: %#lx\n", (unsigned long)addr);
    QTHREAD_FASTLOCK_LOCK(&m->lock);
    if (m->EFQ) {
        qthread_addrres_t *curs = m->EFQ;
        printf("\tEFQ = ");
        while (curs) {
            if (curs->next) {
                printf("%p(%u), ", curs, curs->waiter->thread_id);
            } else {
                printf("%p(%u)\n", curs, curs->waiter->thread_id);
            }
            curs = curs->next;
        }
    }
    if (m->FEQ) {
        qthread_addrres_t *curs = m->FEQ;
        printf("\tFEQ = ");
        while (curs) {
            if (curs->next) {
                printf("%p(%u), ", curs, curs->waiter->thread_id);
            } else {
                printf("%p(%u)\n", curs, curs->waiter->thread_id);
            }
            curs = curs->next;
        }
    }
    if (m->FFQ) {
        qthread_addrres_t *curs = m->FFQ;
        printf("\tFFQ = ");
        while (curs) {
            if (curs->next) {
                printf("%p(%u), ", curs, curs->waiter->thread_id);
            } else {
                printf("%p(%u)\n", curs, curs->waiter->thread_id);
            }
            curs = curs->next;
        }
    }
    printf("\tfull = %u\n"
           "\tvalid = %u\n",
           m->full, m->valid);
    if (arg) {
        *(int *)arg += 1;
    }
    QTHREAD_FASTLOCK_UNLOCK(&m->lock);
}                                      /*}}} */

static void qt_print_addrstat(qt_key_t addr, qthread_f f, void *arg, void *retloc, unsigned int thread_id, void *tls, void *callarg)
{
    printf("addr: %#lx thread %u, arg:%p retval:%p\n", (unsigned long)addr, thread_id, arg, retloc);
    *(int *)callarg += 1;
}

static int print_FEBs(int *ct)
{
    qthread_feb_callback(qt_print_addrstat, ct);
    qthread_syncvar_callback(qt_print_addrstat, ct);
    return 0;
}

#endif /* ifdef QTHREAD_DEBUG */

void API_FUNC qthread_finalize(void)
{                      /*{{{ */
    int                   r;
    qthread_shepherd_id_t i;
    qthread_t            *t;
    qthread_worker_t *worker;

    /***********************************************************************/
    /* Sanity check - THIS *MUST* BE THE FIRST CODE RUN IN QTHREAD_FINALIZE
     *
     * Those who insert other code before this will discover that bad things
     * happen, and will be forced to re-invent this code, badly. */
    /***********************************************************************/
    if ((qlib == NULL) || (qthread_shep() != 0) || (qthread_worker(NULL) != 0)) {
        /* in essence, qthread_finalize can easily be called by threads
         * external to qthreads (via the atexit() setup); if that happens, we
         * need to ignore it. */
        return;
    } else {
        /* also, if a thread other than worker0, shep0, real-mccoy calls it, we
         * should ignore that too. */
        qthread_t *t = qthread_internal_self();
        if (0 == (t->flags & QTHREAD_REAL_MCCOY)) {
            return;
        }
    }
    qthread_debug(CORE_CALLS, "began.\n");
    /***********************************************************************/


    qt_internal_teams_reclaim();

    qthread_shepherd_t *shep0 = &(qlib->shepherds[0]);

# ifdef STEAL_PROFILE
    qthread_steal_stat();
# endif
# ifdef CAS_STEAL_PROFILE
    qthread_cas_steal_stat();
    // outstanding threads remain
    // free(qlib->cas_steal_profile);
    qlib->cas_steal_profile = NULL;
# endif
    worker = qthread_internal_getworker();
    if (worker && (worker->packed_worker_id != 0)) {           /* Only run finalize on shepherd 0 worker 0*/
        worker->current->thread_state = QTHREAD_STATE_YIELDED; /* Otherwise, put back */
#ifdef QTHREAD_PERFORMANCE
        QTPERF_QTHREAD_ENTER_STATE(worker->current->rdata->performance_data, QTHREAD_STATE_YIELDED);
#endif /*  ifdef QTHREAD_PERFORMANCE */
        //      qt_threadqueue_enqueue(shep0->ready, worker->current);
        return; // AKP 11/2/11 I think that is if statement catches the case that exit is called within a
        // parallel region so a random stream reaches here.  we return rather than requeue because we just
        // want to exit (something bad happened) [my speculation]
    }

    /* enqueue the termination thread sentinal */
#ifdef QTHREAD_SHEPHERD_PROFILING
    qtimer_stop(shep0->total_time);
#endif

    for (i = 0; i < qlib->nshepherds; i++) {
        qthread_worker_id_t j;
        for (j = 0; j < qlib->nworkerspershep; j++) {
            if ((i == 0) && (j == 0)) {
                continue;   /* None for shepard 0's worker 0 */
            }
            qthread_debug(SHEPHERD_DETAILS, "terminating worker %i:%i\n", (int)i, (int)j);
            t = qthread_thread_new(NULL, NULL, 0, NULL, NULL, 0);
            assert(t != NULL);         /* what else can we do? */
            t->thread_state = QTHREAD_STATE_TERM_SHEP;
#ifdef QTHREAD_PERFORMANCE
            QTPERF_QTHREAD_ENTER_STATE(t->rdata->performance_data, QTHREAD_STATE_TERM_SHEP);
#endif /*  ifdef QTHREAD_PERFORMANCE */
            t->thread_id    = QTHREAD_NON_TASK_ID;
            t->flags        = QTHREAD_UNSTEALABLE;
            qt_threadqueue_enqueue(qlib->shepherds[i].ready, t);
            if (!QTHREAD_CASLOCK_READ_UI(qlib->shepherds[i].workers[j].active)) {
                qthread_debug(SHEPHERD_DETAILS, "re-enabling worker %i:%i, so he can exit\n", (int)i, (int)j);
                (void)QT_CAS(qlib->shepherds[i].workers[j].active, 0, 1);
            }
        }
    }

#ifdef QTHREAD_SHEPHERD_PROFILING
    print_status("Shepherd 0 spent %f%% of the time idle, handling %lu threads\n",
                 shep0->idle_time / qtimer_secs(shep0->total_time) * 100.0,
                 (unsigned long)shep0->num_threads);
    print_status("Shepherd 0 averaged %g secs to find a new thread, max %g secs\n",
                 shep0->idle_time / shep0->idle_count,
                 shep0->idle_maxtime);
    qtimer_destroy(shep0->total_time);
#endif

    qthread_debug(CORE_DETAILS, "calling early cleanup functions\n");
    while (qt_cleanup_early_funcs != NULL) {
        struct qt_cleanup_funcs_s *tmp = qt_cleanup_early_funcs;
        qt_cleanup_early_funcs = tmp->next;
        tmp->func();
        FREE(tmp, sizeof(struct qt_cleanup_funcs_s));
    }
    qthread_debug(CORE_DETAILS, "done calling early cleanup functions\n");

    /* wait for each SPAWNED shepherd to drain it's queue
     * (note: not shepherd 0, because that one wasn't spawned) */
    /**********************************************************************
     *                   DO NOT FREE MEMORY YET!
     **********************************************************************
     * When some shepherds are still alive, they may be attempting to steal,
     * and this is a race condition to see if they access free'd memory. */
    for (i = 0; i < qlib->nshepherds; i++) {
        /* With multi-threaded shepherds, do join shepherd 0 workers, but not worker 0 */
        qthread_worker_id_t j;
        qthread_shepherd_t *shep = &(qlib->shepherds[i]);
        qthread_debug(SHEPHERD_DETAILS, "waiting for shepherd %i to exit\n", (int)i);
        for (j = 0; j < qlib->nworkerspershep; j++) {
            if ((i == 0) && (j == 0)) {
                continue;  /* This leaves out shepherd 0's worker 0 */
            }
            if ((r = pthread_join(shep->workers[j].worker, NULL)) != 0) {
                print_error("qthread_finalize: pthread_join() of shep %i worker %i failed (%d, or \"%s\")\n",
                            (int)i, (int)j, r, strerror(r));
                abort();
            }
        }
        if (i == 0) { continue; }
    }
    /**********************************************************************/
    qthread_debug(SHEPHERD_BEHAVIOR | CORE_BEHAVIOR, "******* Now running with only ONE thread! *******\n");
    /**********************************************************************/
    for (i = 0; i < qlib->nshepherds; i++) {
        /* With multi-threaded shepherds, do join shepherd 0 workers, but not worker 0 */
        qthread_worker_id_t j;
        qthread_shepherd_t *shep = &(qlib->shepherds[i]);
        qthread_debug(SHEPHERD_DETAILS, "destroying shepherd %i's worker memory \n", (int)i);
        for (j = 0; j < qlib->nworkerspershep; j++) {
            if ((i == 0) && (j == 0)) {
                continue;  /* This leaves out shepherd 0's worker 0 */
            }
            FREE(shep->workers[j].nostealbuffer, STEAL_BUFFER_LENGTH * sizeof(qthread_t *));
            FREE(shep->workers[j].stealbuffer, STEAL_BUFFER_LENGTH * sizeof(qthread_t *));
        }
        if (i == 0) {
            FREE(shep0->workers[0].nostealbuffer, STEAL_BUFFER_LENGTH * sizeof(qthread_t *));
            FREE(shep0->workers[0].stealbuffer, STEAL_BUFFER_LENGTH * sizeof(qthread_t *));
        }
        FREE(qlib->shepherds[i].workers, qlib->nworkerspershep * sizeof(qthread_worker_t));
        if (i == 0) { continue; }
        QTHREAD_CASLOCK_DESTROY(shep->active);
        qt_threadqueue_free(shep->ready);
#ifdef QTHREAD_LOCAL_PRIORITY
        qt_threadqueue_free(shep->local_priority_queue);
#endif /* ifdef QTHREAD_LOCAL_PRIORITY */

#ifdef QTHREAD_DEBUG
        {
            int ct = 0;
            print_FEBs(&ct);
            if (ct != 0) { printf("ct = %i\n", ct); }
            while (ct != 0) ;
        }
#endif

#ifdef QTHREAD_SHEPHERD_PROFILING
        print_status("Shepherd %i spent %f%% of the time idle, handling %lu threads\n",
                     i,
                     shep->idle_time / qtimer_secs(shep->total_time) * 100.0,
                     (unsigned long)shep->num_threads);
        qtimer_destroy(shep->total_time);
        print_status("Shepherd %i averaged %g secs to find a new thread, max %g secs\n",
                     i,
                     shep->idle_time / shep->idle_count,
                     shep->idle_maxtime);
#endif
#ifdef QTHREAD_FEB_PROFILING
# ifdef QTHREAD_MUTEX_INCREMENT
        QTHREAD_ACCUM_MAX(shep0->incr_maxtime, shep->incr_maxtime);
        shep0->incr_time  += shep->incr_time;
        shep0->incr_count += shep->incr_count;
# endif
        QTHREAD_ACCUM_MAX(shep0->aquirelock_maxtime,
                          shep->aquirelock_maxtime);
        shep0->aquirelock_time  += shep->aquirelock_time;
        shep0->aquirelock_count += shep->aquirelock_count;
        QTHREAD_ACCUM_MAX(shep0->lockwait_maxtime,
                          shep->lockwait_maxtime);
        shep0->lockwait_time  += shep->lockwait_time;
        shep0->lockwait_count += shep->lockwait_count;
        QTHREAD_ACCUM_MAX(shep0->hold_maxtime, shep->hold_maxtime);
        shep0->hold_time += shep->hold_time;
        QTHREAD_ACCUM_MAX(shep0->febblock_maxtime,
                          shep->febblock_maxtime);
        shep0->febblock_time  += shep->febblock_time;
        shep0->febblock_count += shep->febblock_count;
        QTHREAD_ACCUM_MAX(shep0->febwait_maxtime,
                          shep->febwait_maxtime);
        shep0->febwait_time  += shep->febwait_time;
        shep0->febwait_count += shep->febwait_count;
        QTHREAD_ACCUM_MAX(shep0->empty_maxtime, shep->empty_maxtime);
        shep0->empty_time  += shep->empty_time;
        shep0->empty_count += shep->empty_count;
        qthread_debug(CORE_DETAILS, "destroying hashes\n");
# ifdef QTHREAD_MUTEX_INCREMENT
        qt_hash_callback(shep->uniqueincraddrs,
                         qthread_unique_collect, shep0->uniqueincraddrs);
        qt_hash_destroy(shep->uniqueincraddrs);
# endif
        qt_hash_callback(shep->uniquelockaddrs,
                         qthread_unique_collect, shep0->uniquelockaddrs);
        qt_hash_destroy(shep->uniquelockaddrs);
        qt_hash_callback(shep->uniquefebaddrs,
                         qthread_unique_collect, shep0->uniquefebaddrs);
        qt_hash_destroy(shep->uniquefebaddrs);
#endif  /* ifdef QTHREAD_FEB_PROFILING */
    }
    qthread_debug(CORE_DETAILS, "freeing shep0's threadqueue\n");
    qt_threadqueue_free(shep0->ready);

    qthread_debug(CORE_DETAILS, "calling cleanup functions\n");
    while (qt_cleanup_funcs != NULL) {
        struct qt_cleanup_funcs_s *tmp = qt_cleanup_funcs;
        qt_cleanup_funcs = tmp->next;
        tmp->func();
        FREE(tmp, sizeof(struct qt_cleanup_funcs_s));
    }

#ifdef QTHREAD_FEB_PROFILING
# ifdef QTHREAD_MUTEX_INCREMENT
    print_status("%llu increments performed (%ld unique), average %g secs, max %g secs\n",
                 (unsigned long long)shep0->incr_count, qt_hash_count(shep0->uniqueincraddrs),
                 (shep0->incr_count == 0) ? 0 : (shep0->incr_time / shep0->incr_count),
                 shep0->incr_maxtime);
    qt_hash_destroy(shep0->uniqueincraddrs);
# endif
    print_status("%ld unique addresses used with FEB, blocked %g secs\n",
                 qt_hash_count(shep0->uniquefebaddrs),
                 (shep0->febblock_count == 0) ? 0 : shep0->febblock_time);
    qt_hash_destroy(shep0->uniquefebaddrs);
    print_status("%llu potentially-blocking FEB operations, average %g secs, max %g secs\n",
                 (unsigned long long)shep0->febblock_count,
                 (shep0->febblock_count == 0) ? 0 : (shep0->febblock_time / shep0->febblock_count),
                 shep0->febblock_maxtime);
    print_status("%llu FEB operations blocked, average wait %g secs, max %g secs\n",
                 (unsigned long long)shep0->febwait_count,
                 (shep0->febwait_count == 0) ? 0 : (shep0->febwait_time / shep0->febwait_count),
                 shep0->febwait_maxtime);
    print_status("%llu FEB bits emptied, stayed empty average %g secs, max %g secs\n",
                 (unsigned long long)shep0->empty_count,
                 (shep0->empty_count == 0) ? 0 : (shep0->empty_time /
                                                  shep0->empty_count), shep0->empty_maxtime);
#endif /* ifdef QTHREAD_FEB_PROFILING */

#ifdef LOCK_FREE_FEBS
    extern unsigned int QTHREAD_LOCKING_STRIPES;
    QTHREAD_LOCKING_STRIPES = 1;
#elif defined(QTHREAD_MUTEX_INCREMENT) || (QTHREAD_ASSEMBLY_ARCH == QTHREAD_POWERPC32)
    extern unsigned int QTHREAD_LOCKING_STRIPES;
    for (i = 0; i < QTHREAD_LOCKING_STRIPES; i++) {
        QTHREAD_FASTLOCK_DESTROY(qlib->atomic_locks[i]);
    }
#endif
#ifdef QTHREAD_MUTEX_INCREMENT
    QTHREAD_FASTLOCK_DESTROY(qlib->nshepherds_active_lock);
    QTHREAD_FASTLOCK_DESTROY(qlib->nworkers_active_lock);
#endif
#ifdef QTHREAD_COUNT_THREADS
    print_status("spawned %lu threads, max realized concurrency %lu, avg realized concurrency %g\n",
                 (unsigned long)threadcount, (unsigned long)maxeffconcurrentthreads,
                 avg_eff_concurrent_threads);
    print_status("max theoretical concurrency %lu, avg theoretical concurrency %g\n",
                 (unsigned long)maxconcurrentthreads, avg_concurrent_threads);
    QTHREAD_FASTLOCK_DESTROY(concurrentthreads_lock);
    QTHREAD_FASTLOCK_DESTROY(effconcurrentthreads_lock);
#endif

    qthread_debug(CORE_DETAILS, "destroy scheduling locks\n");
    QTHREAD_FASTLOCK_DESTROY(qlib->max_thread_id_lock);
    QTHREAD_FASTLOCK_DESTROY(qlib->max_unique_id_lock);
    QTHREAD_FASTLOCK_DESTROY(qlib->sched_shepherd_lock);

#ifdef QTHREAD_USE_VALGRIND
    VALGRIND_STACK_DEREGISTER(qlib->mccoy_thread->rdata->valgrind_stack_id);
    VALGRIND_STACK_DEREGISTER(qlib->valgrind_masterstack_id);
#endif
    assert(qlib->mccoy_thread->rdata->stack == NULL);
    if (qlib->mccoy_thread->rdata->tasklocal_size > 0) {
        FREE(*(void **)&qlib->mccoy_thread->data[0], qlib->mccoy_thread->rdata->tasklocal_size);
    }
    qthread_debug(CORE_DETAILS, "destroy mccoy thread structure\n");
    FREE(qlib->mccoy_thread->rdata, sizeof(struct qthread_runtime_data_s));
    FREE_QTHREAD(qlib->mccoy_thread);
    qthread_debug(CORE_DETAILS, "destroy master stack\n");
    FREE(qlib->master_stack, qlib->master_stack_size);
    qthread_debug(CORE_DETAILS, "calling late cleanup functions\n");
    while (qt_cleanup_late_funcs != NULL) {
        struct qt_cleanup_funcs_s *tmp = qt_cleanup_late_funcs;
        qt_cleanup_late_funcs = tmp->next;
        tmp->func();
        FREE(tmp, sizeof(struct qt_cleanup_funcs_s));
    }
#if defined(QTHREAD_MUTEX_INCREMENT) || (QTHREAD_ASSEMBLY_ARCH == QTHREAD_POWERPC32)
    FREE((void *)qlib->atomic_locks, sizeof(QTHREAD_FASTLOCK_TYPE) * QTHREAD_LOCKING_STRIPES);
#endif

    for (i = 0; i < qlib->nshepherds; ++i) {
        qthread_debug(AFFINITY_DETAILS, "destroy topology information on shep %i\n", (int)i);
        if (qlib->shepherds[i].shep_dists) {
            FREE(qlib->shepherds[i].shep_dists, (qlib->nshepherds - 1) * sizeof(unsigned int));
        }
        if (qlib->shepherds[i].sorted_sheplist) {
            FREE(qlib->shepherds[i].sorted_sheplist, (qlib->nshepherds - 1) * sizeof(qthread_shepherd_id_t));
        }
    }

#ifndef UNPOOLED
    qthread_debug(CORE_DETAILS, "destroy global memory pools\n");
    qt_mpool_destroy(generic_qthread_pool);
    generic_qthread_pool = NULL;
    qt_mpool_destroy(generic_big_qthread_pool);
    generic_big_qthread_pool = NULL;
    qt_mpool_destroy(generic_stack_pool);
    generic_stack_pool = NULL;
    qt_mpool_destroy(generic_rdata_pool);
    generic_rdata_pool = NULL;
#endif /* ifndef UNPOOLED */
    qthread_debug(CORE_DETAILS, "destroy global shepherd array\n");
    FREE(qlib->shepherds, qlib->nshepherds * sizeof(qthread_shepherd_t));
    FREE(qlib->threadqueues, qlib->nshepherds * sizeof(qt_threadqueue_t *));
#ifdef QTHREAD_LOCAL_PRIORITY
    FREE(qlib->local_priority_queues, qlib->nshepherds * sizeof(qt_threadqueue_t *));
#endif /* ifdef QTHREAD_LOCAL_PRIORITY */
    qthread_debug(CORE_DETAILS, "destroy global data\n");
    FREE(qlib, sizeof(struct qlib_s));
    qlib = NULL;
    qthread_debug(CORE_DETAILS, "destroy shepherd thread-local data\n");
    TLS_DELETE(shepherd_structs);

#ifndef QTHREAD_NO_ASSERTS
    MACHINE_FENCE;
    qthread_library_initialized = 0;
    MACHINE_FENCE;
#endif
    qthread_debug(CORE_DETAILS, "finished.\n");
    fflush(stdout);
}                      /*}}} */

qthread_t INTERNAL *qthread_internal_self(void)
{                      /*{{{ */
    extern TLS_DECL(qthread_t *, IO_task_struct);

    qthread_worker_t *worker = qthread_internal_getworker();
    if (worker == NULL) {
        // this may also be null, but in the slow path, the logic is sound
        return TLS_GET(IO_task_struct);
    } else {
        return worker->current;
    }

}                      /*}}} */

void API_FUNC *qthread_get_tasklocal(unsigned int size)
{   /*{{{*/
    qthread_t *f = qthread_internal_self();

    qthread_debug(THREAD_CALLS, "size=%u\n", size);
    if (NULL != f) {
        const unsigned int tl_sz = f->rdata->tasklocal_size;
        qthread_debug(THREAD_DETAILS, "tasklocal_size=%u, global tasklocal_size=%u\n", tl_sz, qlib->qthread_tasklocal_size);
        if ((0 == tl_sz) && (size <= qlib->qthread_tasklocal_size)) {
            // Use default space
            if (f->flags & QTHREAD_BIG_STRUCT) {
                return &f->data[qlib->qthread_argcopy_size];
            } else {
                return &f->data;
            }
        } else {
            void **data_blob;
            if (f->flags & QTHREAD_BIG_STRUCT) {
                data_blob = (void **)&f->data[qlib->qthread_argcopy_size];
            } else {
                data_blob = (void **)&f->data[0];
            }
            if (0 == tl_sz) {
                qthread_debug(THREAD_DETAILS, "Allocate space and copy old data\n");
                void *tmp_data = MALLOC(size);
                assert(NULL != tmp_data);

                memcpy(tmp_data, data_blob, qlib->qthread_tasklocal_size);
                *data_blob = tmp_data;

                qthread_debug(THREAD_DETAILS, "set tlsize = %u\n", size);
                f->rdata->tasklocal_size = size;
                return *data_blob;
            } else if (size <= tl_sz) {
                qthread_debug(THREAD_DETAILS, "Use alloc'd data blob, no need to resize\n");
                return *data_blob;
            } else {
                qthread_debug(THREAD_DETAILS, "Resize alloc'd data blob to %u from %u\n", size, f->rdata->tasklocal_size);
                *data_blob = qt_realloc(*data_blob, size);
                assert(NULL != *data_blob);

                qthread_debug(THREAD_DETAILS, "set tlsize = %u\n", size);
                f->rdata->tasklocal_size = size;
                return *data_blob;
            }
        }
    }

    return NULL;
} /*}}}*/

unsigned API_FUNC qthread_size_tasklocal(void)
{   /*{{{*/
    const qthread_t *f = qthread_internal_self();

    assert(NULL != f);
    assert(NULL != f->rdata);

    qthread_debug(THREAD_DETAILS, "internal tasklocal_size=%u\n", f->rdata->tasklocal_size);
    return f->rdata->tasklocal_size ? f->rdata->tasklocal_size : qlib->qthread_tasklocal_size;
} /*}}}*/

API_FUNC void* qthread_tos(void)
{
    const qthread_t *f = qthread_internal_self();

    return f->rdata->stack;
}


API_FUNC void* qthread_bos(void)
{
    const qthread_t *f = qthread_internal_self();

    return f->rdata->stack + qlib->qthread_stack_size;
}

size_t API_FUNC qthread_stackleft(void)
{                      /*{{{ */
    const qthread_t *f = qthread_internal_self();

    if ((f != NULL) && (f->rdata->stack != NULL)) {
        assert((size_t)&f > (size_t)f->rdata->stack &&
               (size_t)&f < ((size_t)f->rdata->stack + qlib->qthread_stack_size));
#ifdef STACK_GROWS_DOWN
        /* not tested */
        assert(((size_t)(f->rdata->stack) + qlib->qthread_stack_size) -
               (size_t)(&f) < qlib->qthread_stack_size);
        return ((size_t)(f->rdata->stack) + qlib->qthread_stack_size) -
               (size_t)(&f);

#else
        assert((size_t)(&f) - (size_t)(f->rdata->stack) <
               qlib->qthread_stack_size);
        return (size_t)(&f) - (size_t)(f->rdata->stack);
#endif
    } else {
        return 0;
    }
}                      /*}}} */

size_t API_FUNC qthread_readstate(const enum introspective_state type)
{                      /*{{{ */
    switch (type) {
        case STACK_SIZE:
            return qlib->qthread_stack_size;

        case RUNTIME_DATA_SIZE:
             return sizeof(struct qthread_runtime_data_s);

        case BUSYNESS:
        {
            qthread_shepherd_t *shep = qthread_internal_getshep();
            if (shep == NULL) {
                return (size_t)(-1);
            } else {
                qthread_debug(CORE_DETAILS, "shep %u: queuelen: %u\n", (unsigned)shep->shepherd_id, qt_threadqueue_advisory_queuelen(shep->ready));
                return qt_threadqueue_advisory_queuelen(shep->ready) + 1;
            }
        }
        case NODE_BUSYNESS:
        {
            size_t sum = 0;
            const qthread_shepherd_t *sheps = qlib->shepherds;
            for (qthread_shepherd_id_t s=0; s < qlib->nshepherds; s++) {
                qthread_debug(CORE_DETAILS, "shep %u busyness %u\n", (unsigned)s, (unsigned)qt_threadqueue_advisory_queuelen(sheps[s].ready));
                sum += qt_threadqueue_advisory_queuelen(sheps[s].ready);
                const qthread_worker_t *wkrs = sheps[s].workers;
                for (qthread_worker_id_t w=0; w<qlib->nworkerspershep; w++) {
                    qthread_debug(CORE_DETAILS, "shep %u wkr %u current %p\n", (unsigned)s, (unsigned)w, wkrs[w].current);
                    sum += (wkrs[w].current != NULL);
                }
            }
            return sum;
        }
        case WORKER_OCCUPATION:
        {
            size_t count = 0;
            const qthread_shepherd_t *sheps = qlib->shepherds;
            for (qthread_shepherd_id_t s=0; s<qlib->nshepherds; s++) {
                const qthread_worker_t *wkrs = sheps[s].workers;
                for (qthread_worker_id_t w=0; w<qlib->nworkerspershep; w++) {
                    count += (wkrs[w].current != NULL);
                }
            }
            return count;
        }
        case ACTIVE_SHEPHERDS:
            return (size_t)(qlib->nshepherds_active);

        case TOTAL_SHEPHERDS:
            return (size_t)(qlib->nshepherds);

        case ACTIVE_WORKERS:
            return (size_t)(qlib->nworkers_active);

        case TOTAL_WORKERS:
            return (size_t)(qlib->nworkerspershep * qlib->nshepherds);

        case CURRENT_SHEPHERD:
            return qthread_shep();

        case CURRENT_WORKER:
            {
                qthread_worker_t *worker = (qthread_worker_t *)TLS_GET(shepherd_structs);

                return worker ? (worker->worker_id) : NO_WORKER;
            }

        case CURRENT_UNIQUE_WORKER:
            {
                qthread_worker_t *worker = (qthread_worker_t *)TLS_GET(shepherd_structs);

                return worker ? (worker->unique_id - 1) : NO_WORKER;
            }

        case CURRENT_TEAM:
            if (NULL != qlib) {
                qthread_t *self = qthread_internal_self();

                if ((NULL != self) && (NULL != self->team)) {
                    return self->team->team_id;
                } else {
                    return 1;
                }
            } else {
                return 1;
            }

        case PARENT_TEAM:
            if (NULL != qlib) {
                qthread_t *self = qthread_internal_self();

                if ((NULL != self) && (NULL != self->team)) {
                    return self->team->parent_id;
                } else {
                    return 0;
                }
            } else {
                return 0;
            }

        default:
            return (size_t)(-1);
    }
}                      /*}}} */

aligned_t API_FUNC *qthread_retloc(void)
{                      /*{{{ */
    qthread_t *me = qthread_internal_self();

    if (me) {
        return me->ret;
    } else {
        return NULL;
    }
}                      /*}}} */

/************************************************************/
/* functions to manage thread stack allocation/deallocation */
/************************************************************/
static QINLINE qthread_t *qthread_thread_new(const qthread_f f,
                                             const void     *arg,
                                             size_t          arg_size,
                                             void           *ret,
                                             qt_team_t      *team,
                                             int             team_leader)
{                      /*{{{ */
    qthread_t *t;

    if ((arg_size > 0) && (arg_size <= qlib->qthread_argcopy_size)) {
        t = ALLOC_BIG_QTHREAD();
    } else {
        t = ALLOC_QTHREAD();
    }
    qthread_debug(THREAD_DETAILS, "t = %p\n", t);

    t->f     = f;
    t->arg   = (void *)arg;
    t->ret   = ret;
    t->rdata = NULL;
    t->team  = team;


#ifdef QTHREAD_NONLAZY_THREADIDS
    /* give the thread an ID number */
    t->thread_id =
        qthread_internal_incr(&(qlib->max_thread_id),
                              &qlib->max_thread_id_lock, 1);
    switch (t->thread_id) {
        case QTHREAD_NULL_TASK_ID:
            /* yes, this is wrapping around, but... thread_id should be prevented from being NULL */
            t->thread_id = qthread_internal_incr(&(qlib->max_thread_id),
                                                 &qlib->max_thread_id_lock, 2);
            break;
        case QTHREAD_NON_TASK_ID:
            /* yes, this is wrapping around, but... thread_id should be prevented from being NON */
            t->thread_id = qthread_internal_incr(&(qlib->max_thread_id),
                                                 &qlib->max_thread_id_lock, 1);
            break;
    }
#else /* ifdef QTHREAD_NONLAZY_THREADIDS */
    t->thread_id = QTHREAD_NON_TASK_ID;
#endif /* ifdef QTHREAD_NONLAZY_THREADIDS */

    t->target_shepherd = NO_SHEPHERD;

    // should I use the builtin block for args?
    if (arg_size > 0) {
        if (arg_size <= qlib->qthread_argcopy_size) {
            t->arg   = (void *)(&t->data);
            t->flags = QTHREAD_BIG_STRUCT;
        } else {
            t->arg   = MALLOC(arg_size);
            t->flags = QTHREAD_HAS_ARGCOPY;
        }
        memcpy(t->arg, arg, arg_size);
    } else {
        t->flags = 0;
    }

    // am I the team leader?
    if (team_leader) {
        t->flags |= QTHREAD_TEAM_LEADER;
    }

    t->thread_state = QTHREAD_STATE_NEW;

    qthread_debug(THREAD_DETAILS, "returning\n");
    return t;
}                      /*}}} */


void qthread_thread_free(qthread_t *t)
{                      /*{{{ */
    assert(t != NULL);

    qthread_debug(THREAD_FUNCTIONS, "t(%p): destroying thread id %i\n", t, t->thread_id);
    if (t->rdata != NULL) {
        if (t->rdata->tasklocal_size > 0) {
            qthread_debug(THREAD_DETAILS, "t(%p,%i): destroying %u bytes of task-local storage\n", t, t->thread_id, t->rdata->tasklocal_size);
            if (t->flags & QTHREAD_BIG_STRUCT) {
                FREE(*(void **)&t->data[qlib->qthread_argcopy_size], t->rdata->tasklocal_size);
                *(void **)&t->data[qlib->qthread_argcopy_size] = NULL;
            } else {
                FREE(*(void **)&t->data[0], t->rdata->tasklocal_size);
                *(void **)&t->data[0] = NULL;
            }
        }
#ifdef QTHREAD_USE_VALGRIND
        VALGRIND_STACK_DEREGISTER(t->rdata->valgrind_stack_id);
#endif
        if (t->flags & QTHREAD_SIMPLE) {
            qthread_debug(THREAD_DETAILS, "t(%p): releasing rdata %p\n", t, t->rdata);
            FREE_RDATA(t->rdata);
        } else {
            assert(t->rdata->stack);
            qthread_debug(THREAD_DETAILS, "t(%p): releasing stack %p\n", t, t->rdata->stack);
            FREE_STACK(t->rdata->stack);
        }

        t->rdata = NULL;
    }
    if (t->flags & QTHREAD_HAS_ARGCOPY) {
        assert(&t->data != t->arg);
        qt_free(t->arg); // I don't record the size of this anywhere, so I can't scribble it
        t->arg = NULL;
    }
    qthread_debug(THREAD_DETAILS, "t(%p): releasing thread handle %p\n", t, t);
    if (t->flags & QTHREAD_BIG_STRUCT) {
        FREE_BIG_QTHREAD(t);
    } else {
        FREE_QTHREAD(t);
    }
}                      /*}}} */

#ifdef QTHREAD_ALLOW_HPCTOOLKIT_STACK_UNWINDING

int in_qthread_fence(void *addr);
extern void *qthread_fence1;
extern void *qthread_fence2;

# define MONITOR_ASM_LABEL(name)             \
    __asm__ __volatile__ (".globl " # name); \
    __asm__ __volatile__ (# name ":")
#endif /* ifdef QTHREAD_ALLOW_HPCTOOLKIT_STACK_UNWINDING */

void API_FUNC qthread_call_method(qthread_f f, void*arg, void* ret, uint16_t flags){
    if (ret) {
        if (flags & QTHREAD_RET_IS_SINC) {
            if (flags & QTHREAD_RET_IS_VOID_SINC) {
                (f)(arg);
                qt_sinc_submit((qt_sinc_t *)ret, NULL);
            } else {
                aligned_t retval = (f)(arg);
                qt_sinc_submit((qt_sinc_t *)ret, &retval);
            }
        } else if (flags & QTHREAD_RET_IS_SYNCVAR) {
            /* this should avoid problems with irresponsible return values */
            uint64_t retval = INT64TOINT60((f)(arg));
            qassert(qthread_syncvar_writeEF_const((syncvar_t *)ret, retval), QTHREAD_SUCCESS);
        } else {
            aligned_t retval = (f)(arg);
            qassert(qthread_writeEF_const((aligned_t *)ret, retval), QTHREAD_SUCCESS);
        }
    }
    else
        (f)(arg);
}

/* this function runs a thread until it completes or yields */
#ifdef QTHREAD_MAKECONTEXT_SPLIT
static void qthread_wrapper(unsigned int high,
                            unsigned int low)
{                      /*{{{ */
    qthread_t *t = (qthread_t *)((((uintptr_t)high) << 32) | low);

#else
static void qthread_wrapper(void *ptr)
{
    qthread_t *t = (qthread_t *)ptr;
#endif
#ifdef QTHREAD_ALLOW_HPCTOOLKIT_STACK_UNWINDING
    MONITOR_ASM_LABEL(qthread_fence1); // add label for HPCToolkit stack unwind
#endif

    if (t->thread_state == QTHREAD_STATE_YIELDED) {
        /* This means that I've direct-swapped, and need to clean up a little. */
        qthread_t *prev_t = t->rdata->blockedon.thread;
        t->thread_state = QTHREAD_STATE_RUNNING;
#ifdef QTHREAD_PERFORMANCE
        QTPERF_QTHREAD_ENTER_STATE(t->rdata->performance_data, QTHREAD_STATE_RUNNING);
#endif /*  ifdef QTHREAD_PERFORMANCE */
        qthread_debug(THREAD_DETAILS | SHEPHERD_DETAILS,
                      "thread %i yielded; rescheduling\n", t->thread_id);
        assert(prev_t->rdata);
        assert(prev_t->rdata->shepherd_ptr);
        assert(prev_t->rdata->shepherd_ptr->ready);
        assert(t->rdata);
        assert(t->rdata->shepherd_ptr);
        assert(t->rdata->shepherd_ptr->ready);
        assert(prev_t->thread_state == QTHREAD_STATE_RUNNING);
        qthread_worker_t *me_worker = (qthread_worker_t*)TLS_GET(shepherd_structs);
        me_worker->current = t;
        qt_threadqueue_enqueue_yielded(t->rdata->shepherd_ptr->ready, prev_t);
    }

#ifdef QTHREAD_USE_EUREKAS
    qt_eureka_check(0);
#endif /* QTHREAD_USE_EUREKAS */
    qthread_debug(THREAD_BEHAVIOR,
                  "tid %u executing f=%p arg=%p...\n",
                  t->thread_id, t->f, t->arg);
    if ((t->flags & QTHREAD_SIMPLE) == 0) {
        assert((size_t)&t > (size_t)t->rdata->stack &&
               (size_t)&t < ((size_t)t->rdata->stack + qlib->qthread_stack_size));
    }
#ifdef QTHREAD_COUNT_THREADS
    QTHREAD_FASTLOCK_LOCK(&effconcurrentthreads_lock);
    effconcurrentthreads++;
    if (effconcurrentthreads > maxeffconcurrentthreads) {
        maxeffconcurrentthreads = effconcurrentthreads;
    }
    avg_eff_concurrent_threads =
        (avg_eff_concurrent_threads * (double)(threadcount - 1.0) / threadcount)
        + ((double)effconcurrentthreads / threadcount);
    QTHREAD_FASTLOCK_UNLOCK(&effconcurrentthreads_lock);
#endif /* ifdef QTHREAD_COUNT_THREADS */

    if ((NULL != t->team) && (t->flags & QTHREAD_TEAM_LEADER)) {
#ifdef TEAM_PROFILE
        qthread_incr(&qlib->team_leader_start, 1);
#endif
        if (NULL != t->team->parent_eureka) {
            // This is a subteam's team-leader
            qt_internal_subteam_leader(t);
        }
    }

    assert(t->rdata);
    if(t->flags & QTHREAD_AGGREGATED){
        int count = ((int*)t->preconds)[0];
        qthread_f *list_of_f = (qthread_f*) ( & (((int*)t->preconds)[1]) );
        qthread_agg_f agg_f = (qthread_agg_f) ( t->f ) ;
        agg_f(count, list_of_f, (void**)t->arg, (void**)t->ret, t->flags);
        if (NULL != t->team) { qt_internal_teamfinish(t->team, t->flags); }
        //TODO: How to handle ret sinc flags? 
        //Temp solution: use qthread_call_method and pass task flags to the agg function.
    }
    else if (t->ret) {
        qthread_debug(THREAD_DETAILS, "tid %u, with flags %u, handling retval\n", t->thread_id, t->flags);
        if (t->flags & QTHREAD_RET_IS_SINC) {
            if (t->flags & QTHREAD_RET_IS_VOID_SINC) {
                (t->f)(t->arg);
                if (NULL != t->team) { qt_internal_teamfinish(t->team, t->flags); }
                qt_sinc_submit((qt_sinc_t *)t->ret, NULL);
            } else {
                aligned_t retval = (t->f)(t->arg);
                if (NULL != t->team) { qt_internal_teamfinish(t->team, t->flags); }
                qt_sinc_submit((qt_sinc_t *)t->ret, &retval);
            }
        } else if (t->flags & QTHREAD_RET_IS_SYNCVAR) {
            /* this should avoid problems with irresponsible return values */
            uint64_t retval = INT64TOINT60((t->f)(t->arg));
            if (NULL != t->team) { qt_internal_teamfinish(t->team, t->flags); }
            qassert(qthread_syncvar_writeEF_const((syncvar_t *)t->ret, retval), QTHREAD_SUCCESS);
        } else {
            aligned_t retval = (t->f)(t->arg);
            if (NULL != t->team) { qt_internal_teamfinish(t->team, t->flags); }
            qthread_debug(FEB_DETAILS, "tid %u filling retval (%p)\n", t->thread_id, t->ret);
            qassert(qthread_writeEF_const((aligned_t *)t->ret, retval), QTHREAD_SUCCESS);
        }
    } else {
        assert(t->f);
        (t->f)(t->arg);
        if (NULL != t->team) { qt_internal_teamfinish(t->team, t->flags); }
    }

    t->thread_state = QTHREAD_STATE_TERMINATED;
#ifdef QTHREAD_PERFORMANCE
    QTPERF_QTHREAD_ENTER_STATE(t->rdata->performance_data, QTHREAD_STATE_TERMINATED);
#endif /*  ifdef QTHREAD_PERFORMANCE */


#ifdef QTHREAD_COUNT_THREADS
    QTHREAD_FASTLOCK_LOCK(&effconcurrentthreads_lock);
    effconcurrentthreads--;
    QTHREAD_FASTLOCK_UNLOCK(&effconcurrentthreads_lock);
    QTHREAD_FASTLOCK_LOCK(&concurrentthreads_lock);
    assert(concurrentthreads > 0);
    concurrentthreads--;
    QTHREAD_FASTLOCK_UNLOCK(&concurrentthreads_lock);
#endif
    /* theoretically, we could rely on the uc_link pointer to bring us back to
     * the parent shepherd. HOWEVER, this doesn't work in lots of situations,
     * so we do it manually. A brief list of situations:
     *  1. if we're using the portable make/get/swapcontext
     *  2. if the context switch requires a stack-size modification
     *  3. if the thread has migrated (i.e. uc_link points to the original
     *  shepherd, not the current parent... theoretically, that could be
     *  changed, but getting a good uc_link is finicky)
     *
     * Thus, since doing it manually isn't a performance problem, we do it
     * manually.
     */
    qthread_debug(THREAD_BEHAVIOR, "tid %u exiting.\n",
                  t->thread_id);
#ifdef QTHREAD_ALLOW_HPCTOOLKIT_STACK_UNWINDING
    MONITOR_ASM_LABEL(qthread_fence2); // add label for HPCToolkit stack unwind
#endif
    if ((t->flags & QTHREAD_SIMPLE) == 0) {
        qthread_back_to_master2(t);
    }
}                      /*}}} */

#ifdef QTHREAD_ALLOW_HPCTOOLKIT_STACK_UNWINDING
int in_qthread_fence(void *addr)
{
    return (qthread_fence1 <= addr) && (addr >= qthread_fence2);
}

#endif

/* This function means "run thread t". The second argument (c) is a pointer
 * to the current context. */
void INTERNAL qthread_exec(qthread_t    *t,
                           qt_context_t *c)
{                      /*{{{ */
    assert(t != NULL);
    assert(c != NULL);

    if ((t->flags & QTHREAD_SIMPLE) == 0) {
        if (t->thread_state == QTHREAD_STATE_NEW) {
            qthread_debug(SHEPHERD_DETAILS,
                          "t(%p), c(%p): type is QTHREAD_THREAD_NEW!\n",
                          t, c);
            t->thread_state = QTHREAD_STATE_RUNNING;
#ifdef QTHREAD_PERFORMANCE
            QTPERF_QTHREAD_ENTER_STATE(t->rdata->performance_data, QTHREAD_STATE_RUNNING);
#endif /*  ifdef QTHREAD_PERFORMANCE */
            qthread_makecontext(&t->rdata->context,
                                t->rdata->stack, qlib->qthread_stack_size,
                                (void (*)(void))qthread_wrapper, t, c);
#ifdef HAVE_NATIVE_MAKECONTEXT
        } else {
            t->rdata->context.uc_link = c; /* NULL pthread_exit() */
#endif
        }

        t->rdata->return_context = c;

        RLIMIT_TO_TASK(t);

        qthread_debug(SHEPHERD_DETAILS,
                      "t(%p): executing swapcontext(%p, %p)...\n", t, t->rdata->return_context, &t->rdata->context);
        /* return_context (aka "c") is being written over with the current context */
#ifdef QTHREAD_USE_VALGRIND
        VALGRIND_CHECK_MEM_IS_ADDRESSABLE(&t->rdata->context, sizeof(qt_context_t));
        VALGRIND_CHECK_MEM_IS_ADDRESSABLE(t->rdata->return_context, sizeof(qt_context_t));
        VALGRIND_MAKE_MEM_DEFINED(&t->rdata->context, sizeof(qt_context_t));
        VALGRIND_MAKE_MEM_DEFINED(t->rdata->return_context, sizeof(qt_context_t));
#endif
#ifdef QTHREAD_PERFORMANCE
        QTPERF_WORKER_ENTER_STATE(qthread_internal_getworker()->performance_data, WKR_QTHREAD_ACTIVE);
#endif /*  ifdef QTHREAD_PERFORMANCE */
#ifdef HAVE_NATIVE_MAKECONTEXT
        qassert(swapcontext(t->rdata->return_context, &t->rdata->context), 0);
#else
        qassert(qt_swapctxt(t->rdata->return_context, &t->rdata->context), 0);
#endif
        RLIMIT_TO_NORMAL(t);
#ifdef QTHREAD_PERFORMANCE
        QTPERF_WORKER_ENTER_STATE(qthread_internal_getworker()->performance_data, WKR_SHEPHERD);
#endif /* ifdef QTHREAD_PERFORMANCE */
    } else {
        assert(t->thread_state == QTHREAD_STATE_NEW);
        t->thread_state = QTHREAD_STATE_RUNNING;
#ifdef QTHREAD_PERFORMANCE
        QTPERF_QTHREAD_ENTER_STATE(t->rdata->performance_data, QTHREAD_STATE_RUNNING);
#endif /* ifdef QTHREAD_PERFORMANCE */
#ifdef QTHREAD_MAKECONTEXT_SPLIT
        unsigned int high = (((uintptr_t)t) >> 32) & 0xffffffff;
        unsigned int low  = ((uintptr_t)t) & 0xffffffff;
        qthread_wrapper(high, low);
#else
        qthread_wrapper(t);
#endif
    }

    assert(t != NULL);
    assert(c != NULL);

    qthread_debug(SHEPHERD_DETAILS, "t(%p): finished, t->thread_state = %i\n", t, (int)t->thread_state);
}                      /*}}} */

/* this function yields thread t to the master kernel thread */
void API_FUNC qthread_yield_(int k)
{                      /*{{{ */
    assert(qthread_library_initialized);
    qthread_t *t = qthread_internal_self();

    if (t != NULL) {
        qthread_debug(THREAD_CALLS,
                      "tid %u yielding...\n", t->thread_id);
        switch (k) {
            case 1: // Yield-near
                t->thread_state = QTHREAD_STATE_YIELDED_NEAR;
#ifdef QTHREAD_PERFORMANCE
                QTPERF_QTHREAD_ENTER_STATE(t->rdata->performance_data, QTHREAD_STATE_YIELDED_NEAR);
#endif /*  ifdef QTHREAD_PERFORMANCE */
                break;
            case 2: // direct-yield
#ifdef QTHREAD_USE_SPAWNCACHE
                {
                    qt_threadqueue_private_t *pq = qt_spawncache_get();
                    if (pq->on_deck) {
                        qthread_t *nt = qt_threadqueue_private_dequeue(pq);
                        assert(nt);
                        if (((nt->flags & QTHREAD_SIMPLE) != 0) || (nt->thread_state != QTHREAD_STATE_NEW)) {
                            qt_spawncache_spawn(nt, t->rdata->shepherd_ptr->ready);
                            goto basic_yield;
                        }
                        /* Initialize nt's rdata */
                        alloc_rdata(t->rdata->shepherd_ptr, nt);
                        nt->thread_state = QTHREAD_STATE_YIELDED; // special indicator state for qthread_wrapper()
                        QTPERF_QTHREAD_ENTER_STATE(nt->rdata->performance_data, QTHREAD_STATE_YIELDED);
                        nt->rdata->blockedon.thread = t;
                        qthread_makecontext(&nt->rdata->context, nt->rdata->stack, qlib->qthread_stack_size, (void(*)(void))qthread_wrapper, nt, t->rdata->return_context);
                        nt->rdata->return_context = t->rdata->return_context;
                        RLIMIT_TO_TASK(t);
                        /* SWAP! */
                        qthread_debug(SHEPHERD_DETAILS,
                                "t(%p): executing swapcontext(%p, %p)...\n", t, &t->rdata->context, &nt->rdata->context);
                        QTPERF_WORKER_ENTER_STATE(qthread_internal_getworker()->performance_data, WKR_SHEPHERD);
#ifdef HAVE_NATIVE_MAKECONTEXT
                        qassert(swapcontext(&t->rdata->context, &nt->rdata->context), 0);
#else
                        qassert(qt_swapctxt(&t->rdata->context, &nt->rdata->context), 0);
#endif
                        qthread_debug(THREAD_BEHAVIOR, "tid %u resumed.\n", t->thread_id);
                        RLIMIT_TO_NORMAL(t);
                        QTPERF_WORKER_ENTER_STATE(qthread_internal_getworker()->performance_data, WKR_QTHREAD_ACTIVE);
                        return;
                    }
                }
#endif
            case 0: // general yield
#ifdef QTHREAD_USE_SPAWNCACHE
basic_yield:
#endif
                t->thread_state = QTHREAD_STATE_YIELDED;
#ifdef QTHREAD_PERFORMANCE
                QTPERF_QTHREAD_ENTER_STATE(t->rdata->performance_data, QTHREAD_STATE_YIELDED);
#endif /*  ifdef QTHREAD_PERFORMANCE */
                break;
        }
        qthread_back_to_master(t);
        qthread_debug(THREAD_BEHAVIOR, "tid %u resumed.\n",
                      t->thread_id);
    }
}                      /*}}} */

void API_FUNC qthread_flushsc(void)
{   /*{{{*/
#ifdef QTHREAD_USE_SPAWNCACHE
    qt_spawncache_flush(qlib->threadqueues[qthread_readstate(CURRENT_UNIQUE_WORKER)]);
#endif
} /*}}}*/

/***********************************************
* FORKING                                     *
***********************************************/

/**
 * Fork a thread by putting it in somebody's work queue.
 * NOTE: scheduling happens here
 *
 * These are the functions for generating a new qthread.
 *
 * Using qthread_fork() and variants:
 *
 *     The specified function will be run to completion. You can detect that a
 *     thread has finished by specifying a location to store the return value
 *     (which will be stored with a qthread_writeF call). The qthread_fork_to
 *     function spawns the thread to a specific shepherd.
 *
 *
 *
 * @param f This is a function that will be run to completion by the created
 *          qthread. (Note that f is a qthread_f and not a qthread_t.)
 * @param arg This is an argument that will be passed to the specified function.
 * @param ret This is a pointer to the location that the return value of f will
 *            be placed into.
 *
 * @return int Returns QTHREAD_SUCCESS (0) on success or a non-zero error value.
 */
#define QTHREAD_SPAWN_MASK_TEAMS (QTHREAD_SPAWN_NEW_TEAM | QTHREAD_SPAWN_NEW_SUBTEAM)

void API_FUNC qthread_chpl_reset_spawn_order(void) {
    assert(qthread_library_initialized);
    qthread_t            *me = qthread_internal_self();

    if (me) {
        assert(me->rdata);
        me->rdata->shepherd_ptr->sched_shepherd = 0;
    } else {
        qlib->sched_shepherd = 0;
        MACHINE_FENCE;
    }
}

int API_FUNC qthread_spawn(qthread_f             f,
                           const void           *arg,
                           size_t                arg_size,
                           void                 *ret,
                           size_t                npreconds,
                           void                 *preconds,
                           qthread_shepherd_id_t target_shep,
                           unsigned int          feature_flag)
{   /*{{{*/
    assert(qthread_library_initialized);
    qthread_t            *t;
    qthread_t            *me = qthread_internal_self();     // note: cannot be myshep->current on multithreaded shepherds
    qthread_shepherd_t   *myshep;
    qthread_shepherd_id_t dest_shep;

#if defined(QTHREAD_DEBUG)
    const qthread_shepherd_id_t max_sheps = qlib->nshepherds;
#endif
    qthread_shepherd_id_t save_target = target_shep;
#ifdef QTHREAD_OMP_AFFINITY
    if(target_shep == NO_SHEPHERD) {
        if (me->rdata->child_affinity != OMP_NO_CHILD_TASK_AFFINITY) {
            target_shep = me->rdata->child_affinity;
        }
    }
#endif
    if (me) {
        assert(me->rdata);
        myshep = me->rdata->shepherd_ptr;
    } else {
        myshep = NULL;
    }

    /* Step 1: Check arguments */
    qthread_debug(THREAD_CALLS,
                  "f(%p), arg(%p), arg_size(%z), rt(%s), ret(%p), pt(%s), np(%z), pc(%p), ts(%u), %s, %s\n",
                  f,
                  arg,
                  arg_size,
                  ((feature_flag & QTHREAD_SPAWN_RET_SYNCVAR_T) ? "syncvar_t" : "aligned_t"),
                  ret,
                  ((feature_flag & QTHREAD_SPAWN_PC_SYNCVAR_T) ? "syncvar_t" : "aligned_t"),
                  npreconds,
                  preconds,
                  target_shep,
                  ((feature_flag & QTHREAD_SPAWN_NEW_TEAM) ? "new_team" :
                   (feature_flag & QTHREAD_SPAWN_NEW_SUBTEAM) ? "sub_team" : "same_team"),
                  ((feature_flag & QTHREAD_SPAWN_SIMPLE) ? "simple" : "full"));
    assert(qlib);
    /* Step 2: Pick a destination */
    if (target_shep != NO_SHEPHERD) {
        dest_shep = target_shep % qlib->nshepherds;
    } else {
        dest_shep = qt_threadqueue_choose_dest(myshep);
#ifdef QTHREAD_DEBUG
        // debug moved until after destination shepherd is picked for multithreaded shepherds
        // check to make sure destination shepherd is in range (not target_shep which is
        // really worker number for multithreaded shepherds) -- AKP 08/12/11
        qassert_ret(((target_shep == NO_SHEPHERD) || (dest_shep < max_sheps)), QTHREAD_BADARGS);
        qassert_ret(f != NULL, QTHREAD_BADARGS);
        if (npreconds > 0) {
            qassert_ret(preconds != NULL, QTHREAD_BADARGS);
            // XXX: remove the following assert when preconds support syncvar_t's
            assert((feature_flag & QTHREAD_SPAWN_PC_SYNCVAR_T) == 0);
        } else {
            qassert_ret(preconds == NULL, QTHREAD_BADARGS);
            assert((feature_flag & QTHREAD_SPAWN_PC_SYNCVAR_T) == 0);
        }
#endif  /* ifdef QTHREAD_DEBUG */
    }
    qthread_debug(THREAD_BEHAVIOR, "target_shep(%i) => dest_shep(%i)\n", target_shep, dest_shep);
    /* Step 3: Allocate & init the structure */

    // Set up team
    assert(!(feature_flag & QTHREAD_SPAWN_NEW_TEAM) ||
           (feature_flag & QTHREAD_SPAWN_NEW_SUBTEAM) ||
           (feature_flag & QTHREAD_SPAWN_MASK_TEAMS));

    qt_team_t *curr_team   = (me && me->team) ? me->team : NULL;
    qt_team_t *new_team    = NULL;
    int        team_leader = -1;

    if (!(feature_flag & QTHREAD_SPAWN_MASK_TEAMS)) {
        // Spawn into the current team
        team_leader = 0;

        if (curr_team) {
            new_team = curr_team;
            qt_sinc_expect(new_team->sinc, 1);
        }
    } else if (feature_flag & QTHREAD_SPAWN_NEW_TEAM) {
        // Spawn into a new team
        qthread_internal_incr(&(qlib->team_count), &qlib->team_count_lock, 1);
        team_leader = 1;

        // Allocate new team structure
        new_team = qt_internal_team_new(ret, feature_flag, NULL, QTHREAD_NON_TEAM_ID);
        assert(new_team);
    } else if (feature_flag & QTHREAD_SPAWN_NEW_SUBTEAM) {
        // Task will participate in a new subteam
        qthread_internal_incr(&(qlib->team_count), &qlib->team_count_lock, 1);
        team_leader = 1;

        // Allocate new team structure
        new_team = qt_internal_team_new(ret, feature_flag, curr_team, curr_team ? curr_team->team_id : QTHREAD_DEFAULT_TEAM_ID);
        assert(new_team);
    } else {
        // Unsupported spawn option
        assert(0);
    }

    t = qthread_thread_new(f, arg, arg_size, (aligned_t *)ret, new_team, team_leader);
    qassert_ret(t, QTHREAD_MALLOC_ERROR);

    if (QTHREAD_UNLIKELY(target_shep != NO_SHEPHERD)) {
        t->target_shepherd = dest_shep;
        t->flags          |= QTHREAD_UNSTEALABLE;
    }
    if (QTHREAD_UNLIKELY(npreconds != 0)) {
        t->thread_state = QTHREAD_STATE_NASCENT; // special non-executable state
#ifdef QTHREAD_PERFORMANCE
        QTPERF_QTHREAD_ENTER_STATE(t->rdata->performance_data, QTHREAD_STATE_NASCENT);
#endif /*  QTHREAD_PERFORMANCE */
        t->preconds     = preconds;
        qthread_debug(THREAD_BEHAVIOR, "npreconds=%u, preconds[0]=%u\n", (unsigned int)npreconds, (unsigned int)(uintptr_t)((aligned_t **)preconds)[0]);
        assert(((aligned_t **)preconds)[0] == (aligned_t *)(uintptr_t)npreconds);
    } else {
        t->preconds = NULL;
    }
    if (feature_flag & QTHREAD_SPAWN_SIMPLE) {
        t->flags |= QTHREAD_SIMPLE;
    }
    qthread_debug(THREAD_BEHAVIOR, "new-tid %u shep %u\n", t->thread_id, dest_shep);
       /* Step 4: Prepare the return value location (if necessary) */
    if (ret) {
        int      test     = QTHREAD_SUCCESS;
        unsigned ret_type = feature_flag & (QTHREAD_SPAWN_RET_SYNCVAR_T |
                                            QTHREAD_SPAWN_RET_SINC |
                                            QTHREAD_SPAWN_RET_SINC_VOID);
        switch (ret_type) {
            case QTHREAD_SPAWN_RET_SYNCVAR_T:
                t->flags |= QTHREAD_RET_IS_SYNCVAR;
                if (qthread_syncvar_status((syncvar_t *)ret)) {
                    test = qthread_syncvar_empty((syncvar_t *)ret);
                } else {
                    test = QTHREAD_SUCCESS;
                }
                break;
            case QTHREAD_SPAWN_RET_SINC:
                t->flags |= QTHREAD_RET_IS_SINC;
                break;
            case QTHREAD_SPAWN_RET_SINC_VOID:
                t->flags |= QTHREAD_RET_IS_VOID_SINC;
                break;
            default:
                // QTHREAD_SPAWN_RET_ALIGNED
                qthread_debug(FEB_DETAILS, "tid %i emptying new thread %u's retval (%p)\n", me ? ((int)me->thread_id) : -1, t->thread_id, ret);
                test = qthread_empty(ret);
                break;
        }
        if (QTHREAD_UNLIKELY(test != QTHREAD_SUCCESS)) {
            qthread_thread_free(t);
            return test;
        }
    }
    qthread_debug(THREAD_DETAILS, "tid %i spawning new thread %u with flags %u\n", me ? ((int)me->thread_id) : -1, t->thread_id, t->flags);
    /* Step 5: Prepare the input preconditions (if necessary) */
    if (QTHREAD_LIKELY(!preconds) || (qthread_check_feb_preconds(t) == 0)) {
        /* Step 6: Set it going */
#ifdef QTHREAD_COUNT_THREADS
        QTHREAD_FASTLOCK_LOCK(&concurrentthreads_lock);
        threadcount++;
        concurrentthreads++;
        assert(concurrentthreads <= threadcount);
        if (concurrentthreads > maxconcurrentthreads) {
            maxconcurrentthreads = concurrentthreads;
        }
        avg_concurrent_threads =
            (avg_concurrent_threads * (double)(threadcount - 1.0) / threadcount)
            + ((double)concurrentthreads / threadcount);
        QTHREAD_FASTLOCK_UNLOCK(&concurrentthreads_lock);
#endif  /* ifdef QTHREAD_COUNT_THREADS */
#ifdef QTHREAD_USE_SPAWNCACHE
        if (target_shep == NO_SHEPHERD) {
            if (!qt_spawncache_spawn(t, qlib->threadqueues[dest_shep])) {
                qt_threadqueue_enqueue(qlib->threadqueues[dest_shep], t);
            }
        } else
#endif
        {
#ifdef QTHREAD_LOCAL_PRIORITY
            if (feature_flag & QTHREAD_SPAWN_LOCAL_PRIORITY)
                qt_threadqueue_enqueue(qlib->local_priority_queues[dest_shep], t);
            else
#endif /* ifdef QTHREAD_LOCAL_PRIORITY */
                qt_threadqueue_enqueue(qlib->threadqueues[dest_shep], t);

        }
    }

    if (feature_flag & QTHREAD_SPAWN_NETWORK)
        t->flags |= QTHREAD_NETWORK;

    return QTHREAD_SUCCESS;
} /*}}}*/

int API_FUNC qthread_fork(qthread_f   f,
                          const void *arg,
                          aligned_t  *ret)
{   /*{{{*/
    qthread_debug(THREAD_CALLS, "f(%p), arg(%p), ret(%p)\n", f, arg, ret);
    return qthread_spawn(f, arg, 0, ret, 0, NULL, NO_SHEPHERD, 0);
} /*}}}*/

int API_FUNC qthread_fork_net(qthread_f   f,
                          const void *arg,
                          aligned_t  *ret)
{   /*{{{*/
  qthread_debug(THREAD_CALLS, "f(%p), arg(%p), ret(%p)\n", f, arg, ret);
  return qthread_spawn(f, arg, 0, ret, 0, NULL, NO_SHEPHERD, QTHREAD_SPAWN_NETWORK);
} /*}}}*/

int API_FUNC qthread_fork_copyargs_precond(qthread_f   f,
                                           const void *arg,
                                           size_t      arg_size,
                                           syncvar_t  *ret,
                                           int         npreconds,
                                           ...)
{   /*{{{*/
    // Collect sync info
    va_list     args;
    aligned_t **preconds = NULL;

    va_start(args, npreconds);
    if (npreconds > 0) {
        preconds = MALLOC((npreconds + 1) * sizeof(aligned_t *));
        assert(preconds != NULL);
        preconds[0] = (aligned_t *)(uintptr_t)(npreconds);
        for (int i = 1; i < npreconds + 1; ++i) {
            preconds[i] = va_arg(args, aligned_t *);
        }
    } else if (npreconds < 0) {
        npreconds *= -1;
        preconds   = MALLOC((npreconds + 1) * sizeof(aligned_t *));
        assert(preconds != NULL);
        preconds[0] = (aligned_t *)(uintptr_t)npreconds;
        aligned_t **tmp = va_arg(args, aligned_t * *);
        memcpy(preconds + 1, tmp, sizeof(aligned_t *) * npreconds);
    }
    va_end(args);

    return qthread_spawn(f, arg, arg_size, ret, npreconds, preconds, NO_SHEPHERD,
                         QTHREAD_SPAWN_RET_SYNCVAR_T);
} /*}}}*/

/*
 * Duplicate code from qthreads function qthread_fork_precond
 * Needed to add the simple function marker.
 * TODO: not happy with code duplication here
 */
int API_FUNC qthread_fork_precond_simple(qthread_f   f,
                                         const void *arg,
                                         aligned_t  *ret,
                                         int         npreconds,
                                         ...)
{   /*{{{*/
    // Collect sync info
    va_list     args;
    aligned_t **preconds = NULL;

    qthread_debug(THREAD_CALLS, "f=%p, arg=%p, ret=%p, npreconds=%u\n", f, arg, ret, npreconds);
    va_start(args, npreconds);
    if (npreconds > 0) {
        preconds = (aligned_t **)MALLOC((npreconds + 1) * sizeof(aligned_t *));
        assert(preconds != NULL);
        preconds[0] = (aligned_t *)(uintptr_t)npreconds;
        for (int i = 1; i < npreconds + 1; ++i) {
            preconds[i] = va_arg(args, aligned_t *);
        }
    } else if (npreconds < 0) {
        npreconds *= -1;
        preconds   = (aligned_t **)MALLOC((npreconds + 1) * sizeof(aligned_t *));
        assert(preconds != NULL);
        preconds[0] = (aligned_t *)(uintptr_t)(npreconds);
        aligned_t **tmp = va_arg(args, aligned_t * *);
        memcpy(preconds + 1, tmp, sizeof(aligned_t *) * npreconds);
    }
    va_end(args);
    return qthread_spawn(f, arg, 0, ret, npreconds, preconds, NO_SHEPHERD,
                         QTHREAD_SPAWN_SIMPLE);
} /*}}}*/


int API_FUNC qthread_fork_precond_to(qthread_f             f,
                                     const void           *arg,
                                     aligned_t            *ret,
                                     qthread_shepherd_id_t shepherd,
                                     int                   npreconds,
                                     ...)
{                      /*{{{ */
    // Collect sync info
    va_list     args;
    aligned_t **preconds = NULL;

    va_start(args, npreconds);
    if (npreconds > 0) {
        preconds = MALLOC((npreconds + 1) * sizeof(aligned_t *));
        assert(preconds != NULL);
        preconds[0] = (aligned_t *)(uintptr_t)npreconds;
        for (int i = 1; i < npreconds + 1; ++i) {
            preconds[i] = va_arg(args, aligned_t *);
        }
    } else if (npreconds < 0) {
        npreconds *= -1;
        preconds   = MALLOC((npreconds + 1) * sizeof(aligned_t *));
        assert(preconds != NULL);
        preconds[0] = (aligned_t *)(uintptr_t)npreconds;
        aligned_t **tmp = va_arg(args, aligned_t * *);
        memcpy(preconds + 1, tmp, sizeof(aligned_t *) * npreconds);
    }
    va_end(args);

    if ((shepherd != NO_SHEPHERD) && (shepherd >= qlib->nshepherds)) {
        shepherd %= qlib->nshepherds;
    }
    return qthread_spawn(f,
                         arg,
                         0,
                         ret,
                         npreconds,
                         preconds,
                         shepherd,
                         0);
}                      /*}}} */

int API_FUNC qthread_fork_precond(qthread_f   f,
                                  const void *arg,
                                  aligned_t  *ret,
                                  int         npreconds,
                                  ...)
{   /*{{{*/
    // Collect sync info
    va_list     args;
    aligned_t **preconds = NULL;

    va_start(args, npreconds);
    if (npreconds > 0) {
        preconds = MALLOC((npreconds + 1) * sizeof(aligned_t *));
        assert(preconds != NULL);
        preconds[0] = (aligned_t *)(uintptr_t)npreconds;
        for (int i = 1; i < npreconds + 1; ++i) {
            preconds[i] = va_arg(args, aligned_t *);
        }
    } else if (npreconds < 0) {
        npreconds *= -1;
        preconds   = MALLOC((npreconds + 1) * sizeof(aligned_t *));
        assert(preconds != NULL);
        preconds[0] = (aligned_t *)(uintptr_t)npreconds;
        aligned_t **tmp = va_arg(args, aligned_t * *);
        memcpy(preconds + 1, tmp, sizeof(aligned_t *) * npreconds);
    }
    va_end(args);

    return qthread_spawn(f, arg, 0, ret, npreconds, preconds, NO_SHEPHERD, 0);
} /*}}}*/

int API_FUNC qthread_fork_copyargs_to(qthread_f             f,
                                      const void           *arg,
                                      size_t                arg_size,
                                      syncvar_t            *ret,
                                      qthread_shepherd_id_t preferred_shep)
{   /*{{{*/
    return qthread_spawn(f,
                         arg,
                         arg_size,
                         ret,
                         0,
                         NULL,
                         preferred_shep,
                         0);
} /*}}}*/

int API_FUNC qthread_fork_copyargs(qthread_f   f,
                                   const void *arg,
                                   size_t      arg_size,
                                   aligned_t  *ret)
{   /*{{{*/
    return qthread_spawn(f,
                         arg,
                         arg_size,
                         ret,
                         0,
                         NULL,
                         NO_SHEPHERD,
                         0);
}   /*}}}*/

int API_FUNC qthread_fork_syncvar_copyargs(qthread_f   f,
                                           const void *arg,
                                           size_t      arg_size,
                                           syncvar_t  *ret)
{                      /*{{{ */
    return qthread_spawn(f,
                         arg,
                         arg_size,
                         ret,
                         0,
                         NULL,
                         NO_SHEPHERD,
                         QTHREAD_SPAWN_RET_SYNCVAR_T);
}                      /*}}} */

int API_FUNC qthread_fork_syncvar_copyargs_simple(qthread_f   f,
                                                  const void *arg,
                                                  size_t      arg_size,
                                                  syncvar_t  *ret)
{                      /*{{{ */
    return qthread_spawn(f,
                         arg,
                         arg_size,
                         ret,
                         0,
                         NULL,
                         NO_SHEPHERD,
                         QTHREAD_SPAWN_SIMPLE |
                         QTHREAD_SPAWN_RET_SYNCVAR_T);
}                      /*}}} */

int API_FUNC qthread_fork_syncvar(qthread_f   f,
                                  const void *arg,
                                  syncvar_t  *ret)
{                      /*{{{ */
    return qthread_spawn(f,
                         arg,
                         0,
                         ret,
                         0,
                         NULL,
                         NO_SHEPHERD,
                         QTHREAD_SPAWN_RET_SYNCVAR_T);
}                      /*}}} */

int API_FUNC qthread_fork_to(qthread_f             f,
                             const void           *arg,
                             aligned_t            *ret,
                             qthread_shepherd_id_t shepherd)
{   /*{{{*/
    if ((shepherd != NO_SHEPHERD) && (shepherd >= qlib->nshepherds)) {
        shepherd %= qlib->nshepherds;
    }
    return qthread_spawn(f,
                         arg,
                         0,
                         ret,
                         0,
                         NULL,
                         shepherd,
                         0);
} /*}}}*/

#ifdef QTHREAD_LOCAL_PRIORITY
int API_FUNC qthread_fork_to_local_priority(qthread_f             f,
                                            const void           *arg,
                                            aligned_t            *ret,
                                            qthread_shepherd_id_t shepherd)
{   /*{{{*/
    if ((shepherd != NO_SHEPHERD) && (shepherd >= qlib->nshepherds)) {
        shepherd %= qlib->nshepherds;
    }
    return qthread_spawn(f,
                         arg,
                         0,
                         ret,
                         0,
                         NULL,
                         shepherd,
                         QTHREAD_SPAWN_LOCAL_PRIORITY);
} /*}}}*/
#endif /* ifdef QTHREAD_LOCAL_PRIORITY */

int API_FUNC qthread_fork_syncvar_to(qthread_f             f,
                                     const void           *arg,
                                     syncvar_t            *ret,
                                     qthread_shepherd_id_t s)
{   /*{{{*/
    if ((s != NO_SHEPHERD) && (s >= qlib->nshepherds)) {
        s %= qlib->nshepherds;
    }
    return qthread_spawn(f,
                         arg,
                         0,
                         ret,
                         0,
                         NULL,
                         s,
                         QTHREAD_SPAWN_RET_SYNCVAR_T);
} /*}}}*/

void INTERNAL qthread_back_to_master(qthread_t *t)
{                      /*{{{ */
    assert((t->flags & QTHREAD_SIMPLE) == 0);
    RLIMIT_TO_NORMAL(t);
#ifdef QTHREAD_PERFORMANCE
    QTPERF_WORKER_ENTER_STATE(qthread_internal_getworker()->performance_data, WKR_SHEPHERD);
#endif /*  QTHREAD_PERFORMANCE */
    /* now back to your regularly scheduled master thread */
#ifdef QTHREAD_USE_VALGRIND
    VALGRIND_CHECK_MEM_IS_ADDRESSABLE(&t->rdata->context, sizeof(qt_context_t));
    VALGRIND_CHECK_MEM_IS_ADDRESSABLE(t->rdata->return_context, sizeof(qt_context_t));
    VALGRIND_MAKE_MEM_DEFINED(&t->rdata->context, sizeof(qt_context_t));
    VALGRIND_MAKE_MEM_DEFINED(t->rdata->return_context, sizeof(qt_context_t));
#endif
#ifdef HAVE_NATIVE_MAKECONTEXT
    qassert(swapcontext(&t->rdata->context, t->rdata->return_context), 0);
#else
    qassert(qt_swapctxt(&t->rdata->context, t->rdata->return_context), 0);
#endif
    RLIMIT_TO_TASK(t);
#ifdef QTHREAD_PERFORMANCE
    QTPERF_WORKER_ENTER_STATE(qthread_internal_getworker()->performance_data, WKR_QTHREAD_ACTIVE);
#endif /*  QTHREAD_PERFORMANCE */
}                      /*}}} */

void INTERNAL qthread_back_to_master2(qthread_t *t)
{                      /*{{{ */
    assert((t->flags & QTHREAD_SIMPLE) == 0);
    RLIMIT_TO_NORMAL(t);
    //qtlog(WKR_DBG, "qthread_back_to_master2 called!!!");
#ifdef QTHREAD_PERFORMANCE
    QTPERF_WORKER_ENTER_STATE(qthread_internal_getworker()->performance_data, WKR_SHEPHERD);
#endif /*  QTHREAD_PERFORMANCE */
    /* now back to your regularly scheduled master thread */
#ifdef QTHREAD_USE_VALGRIND
    VALGRIND_CHECK_MEM_IS_ADDRESSABLE(&t->rdata->context, sizeof(qt_context_t));
    VALGRIND_CHECK_MEM_IS_ADDRESSABLE(t->rdata->return_context, sizeof(qt_context_t));
    VALGRIND_MAKE_MEM_DEFINED(&t->rdata->context, sizeof(qt_context_t));
    VALGRIND_MAKE_MEM_DEFINED(t->rdata->return_context, sizeof(qt_context_t));
#endif
#ifdef HAVE_NATIVE_MAKECONTEXT
    setcontext(t->rdata->return_context);
#else
    qt_setmctxt(&t->rdata->return_context->mc);
#endif
}                      /*}}} */

/* function to move a qthread from one shepherd to another */
int API_FUNC qthread_migrate_to(const qthread_shepherd_id_t shepherd)
{                      /*{{{ */
    assert(qthread_library_initialized);
    qthread_t *me = qthread_internal_self();

    if (me->flags & QTHREAD_REAL_MCCOY) {
        return QTHREAD_NOT_ALLOWED;
    }
    if (me->rdata->shepherd_ptr->shepherd_id == shepherd) {
        me->target_shepherd = shepherd;
        me->flags          |= QTHREAD_UNSTEALABLE;
        return QTHREAD_SUCCESS;
    }
    if (shepherd == NO_SHEPHERD) {
        me->target_shepherd = NO_SHEPHERD;
        me->flags          &= ~(uint8_t)QTHREAD_UNSTEALABLE;
        return QTHREAD_SUCCESS;
    }
    if (me && (shepherd < qlib->nshepherds)) {
        qthread_debug(THREAD_BEHAVIOR,
                      "tid %u from shep %u to shep %u\n",
                      me->thread_id, me->rdata->shepherd_ptr->shepherd_id, shepherd);
        me->target_shepherd = shepherd;
        me->thread_state    = QTHREAD_STATE_MIGRATING;
#ifdef QTHREAD_PERFORMANCE
        QTPERF_QTHREAD_ENTER_STATE(me->rdata->performance_data, QTHREAD_STATE_MIGRATING);
#endif /*  QTHREAD_PERFORMANCE */
        me->flags          |= QTHREAD_UNSTEALABLE;
        qthread_back_to_master(me);

        qthread_debug(THREAD_DETAILS,
                      "tid %u awakes on shepherd %u!\n",
                      me->thread_id, me->rdata->shepherd_ptr->shepherd_id);
        return QTHREAD_SUCCESS;
    } else {
        return QTHREAD_BADARGS;
    }
}                      /*}}} */


/* These are just accessor functions */
unsigned int API_FUNC qthread_id(void)
{                      /*{{{ */
    assert(qthread_library_initialized);
    qthread_t *t = qthread_internal_self();

    qthread_debug(THREAD_CALLS, "tid(%u)\n", t ? t->thread_id : QTHREAD_NON_TASK_ID);
#ifdef QTHREAD_NONLAZY_THREADIDS
    return t ? t->thread_id : QTHREAD_NON_TASK_ID;

#else
    if (!t) {
        return QTHREAD_NON_TASK_ID;
    }
    if (t->thread_id != QTHREAD_NON_TASK_ID) {
        return t->thread_id;
    }
    t->thread_id = qthread_internal_incr(&(qlib->max_thread_id),
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
    return t->thread_id;
#endif /* ifdef QTHREAD_NONLAZY_THREADIDS */
}                      /*}}} */



void qt_set_barrier(qt_barrier_t *bar)
{                      /*{{{ */
    qthread_t *me = qthread_internal_self();

    me->rdata->barrier = bar;
}                      /*}}} */

qt_barrier_t *qt_get_barrier()
{                      /*{{{ */
    qthread_t *me = qthread_internal_self();

    return me->rdata->barrier;
}                      /*}}} */

/* vim:set expandtab: */
