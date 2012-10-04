#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

/* The API */
#include "qthread/qthread.h"

/* System Headers */
#include <stdio.h>
#include <stdlib.h>              /* for malloc() and abort() */
#include <stdarg.h>              /* for va_list, va_start() and va_end() */
#include <limits.h>              /* for INT_MAX */
#include <qthread/qthread-int.h> /* for UINT8_MAX */
#include <string.h>              /* for memset() */
#ifdef QTHREAD_GUARD_PAGES
# include <unistd.h>           /* for getpagesize() */
#endif
#if !HAVE_MEMCPY
# define memcpy(d, s, n)  bcopy((s), (d), (n))
# define memmove(d, s, n) bcopy((s), (d), (n))
#endif
#include <sys/time.h>
#include <sys/resource.h>
#if (defined(QTHREAD_SHEPHERD_PROFILING) || defined(QTHREAD_FEB_PROFILING))
# include <qthread/qtimer.h>
#endif
#include <pthread.h>
#ifdef HAVE_SCHED_H
# include <sched.h>
#endif
#ifdef QTHREAD_USE_VALGRIND
# include <valgrind/memcheck.h>
#endif
#ifdef QTHREAD_GUARD_PAGES
# include <sys/types.h>
# include <sys/mman.h>
#endif
#include <errno.h>
#ifdef SST
# include <ppcPimCalls.h>
#endif
#include <signal.h>

/* Internal Headers */
#include "qt_mpool.h"
#include "qt_atomics.h"
#include "qthread_expect.h"

#include "qthread/cacheline.h"
#include "qthread/futurelib.h"
#include "qthread_innards.h"
#include "qthread_prefetch.h"
#include "futurelib_innards.h"
#ifdef QTHREAD_USE_ROSE_EXTENSIONS
# include "qt_barrier.h"
# include "qthread/feb_barrier.h"   /* for barrier in parallel region defination */
# include "qt_arrive_first.h"
# include "qt_task_counter.h"
#endif
#ifdef QTHREAD_OMP_AFFINITY
# include "omp_affinity.h"
#endif
#include "qt_qthread_struct.h"
#include "qt_shepherd_innards.h"
#include "qt_blocking_structs.h"
#include "qt_addrstat.h"
#include "qt_threadqueues.h"
#include "qt_affinity.h"
#include "qt_io.h"
#include "qt_debug.h"
#include "qt_envariables.h"
#include "qt_internal_feb.h"
#include "qt_spawncache.h"
#ifdef QTHREAD_MULTINODE
# include "qt_multinode_innards.h"
#endif
#include "qt_aligned_alloc.h"

#ifdef QTHREAD_RCRTOOL
# include "maestro_sched.h"
# include "rcrtool/qt_rcrtool.h"
extern QTHREAD_FASTLOCK_TYPE rcrtool_lock;
#endif

#if !(defined(HAVE_GCC_INLINE_ASSEMBLY) &&              \
    (QTHREAD_SIZEOF_ALIGNED_T == 4 ||                   \
     (QTHREAD_ASSEMBLY_ARCH != QTHREAD_POWERPC32 &&     \
      QTHREAD_ASSEMBLY_ARCH != QTHREAD_SPARCV9_32))) && \
    !defined(QTHREAD_ATOMIC_CAS) &&                     \
    !defined(QTHREAD_MUTEX_INCREMENT)
# warning QTHREAD_MUTEX_INCREMENT not defined. It probably should be.
# define QTHREAD_MUTEX_INCREMENT 1
#endif

/* shared globals (w/ futurelib) */
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
static QINLINE void qthread_thread_free(qthread_t *t);

#ifdef QTHREAD_RCRTOOL_STAT
extern int adaptiveSetHigh;
#endif

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
    uint8_t *tmp = valloc(qlib->qthread_stack_size + sizeof(struct qthread_runtime_data_s) + (2 * getpagesize()));

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
}                      /*}}} */

static QINLINE void FREE_STACK(void *t)
{                      /*{{{ */
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
}                      /*}}} */

static QINLINE void FREE_STACK(void *t)
{                      /*{{{ */
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
    qt_mpool_free(generic_stack_pool, tmp);
}                      /*}}} */

# else /* ifdef QTHREAD_GUARD_PAGES */
#  define ALLOC_STACK() qt_mpool_alloc(generic_stack_pool)
#  define FREE_STACK(t) qt_mpool_free(generic_stack_pool, t)
# endif /* ifdef QTHREAD_GUARD_PAGES */
#endif  /* if defined(UNPOOLED_STACKS) || defined(UNPOOLED) */

#if defined(UNPOOLED)
# define ALLOC_RDATA() (struct qthread_runtime_data_s *)MALLOC(sizeof(struct qthread_runtime_data_s));
# define FREE_RDATA(r) FREE(r, sizeof(struct qthread_runtime_data_s))
# define ALLOC_TEAM()  (qt_team_t *)MALLOC(sizeof(qt_team_t))
# define FREE_TEAM(t)  FREE(t, sizeof(qt_team_t))
#else
static qt_mpool generic_rdata_pool = NULL;
# define ALLOC_RDATA() (struct qthread_runtime_data_s *)qt_mpool_alloc(generic_rdata_pool)
# define FREE_RDATA(r) qt_mpool_free(generic_rdata_pool, (r))
static qt_mpool generic_team_pool = NULL;
# define ALLOC_TEAM() (qt_team_t *)qt_mpool_alloc(generic_team_pool)
# define FREE_TEAM(t) qt_mpool_free(generic_team_pool, (t))
#endif /* if defined(UNPOOLED) */

/* guaranteed to be between 0 and 128, using the first parts of addr that are
 * significant */
unsigned int QTHREAD_LOCKING_STRIPES = 128;

#ifdef QTHREAD_DEBUG
enum qthread_debug_levels debuglevel = NO_DEBUG_OUTPUT;
QTHREAD_FASTLOCK_TYPE     output_lock;

int API_FUNC qthread_debuglevel(int d)
{
    if (d >= 0) { debuglevel = (enum qthread_debug_levels)d; }
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
#ifdef QTHREAD_GUARD_PAGES
        rdata = t->rdata = (struct qthread_runtime_data_s *)(((uint8_t *)stack) + getpagesize() + qlib->qthread_stack_size);
#else
        rdata = t->rdata = (struct qthread_runtime_data_s *)(((uint8_t *)stack) + qlib->qthread_stack_size);
#endif
    }
    rdata->tasklocal_size = 0;
    rdata->stack          = stack;
    rdata->shepherd_ptr   = me;
    rdata->blockedon.io   = NULL;
#ifdef QTHREAD_USE_VALGRIND
    if (stack) {
        rdata->valgrind_stack_id = VALGRIND_STACK_REGISTER(stack, qlib->qthread_stack_size);
    }
#endif
#if defined(QTHREAD_USE_ROSE_EXTENSIONS) && defined(QTHREAD_OMP_AFFINITY)
    rdata->child_affinity = OMP_NO_CHILD_TASK_AFFINITY;
#endif
} /*}}}*/

#ifdef QTHREAD_RCRTOOL
static int rcr_gate  = 0;
static int rcr_ready = 0;
# ifdef QTHREAD_RCRTOOL_STAT
static double totalIdleTime = 0.0; // added for RCRTOOL level >= 3 stats
static int    idleCheckin   = 0;   // added for RCRTOOL level >= 3 stats
# endif
#endif

static void hup_handler(int sig)
{   /*{{{*/
#ifdef QTHREAD_MULTITHREADED_SHEPHERDS
    qthread_worker_t *w = qthread_internal_getworker();
    // qthread_shepherd_t * s = w->shepherd;
    qthread_t *t = w->current;
#else
    qthread_shepherd_t *s = qthread_internal_getshep();
    qthread_t          *t = s->current;
#endif

    assert(sig == SIGUSR1);
    t->thread_state = QTHREAD_STATE_ASSASSINATED;
    qthread_back_to_master(t);
} /*}}}*/

#define print_status(format, ...)  printf("QTHREADS: " format, ## __VA_ARGS__)
#define print_warning(format, ...) fprintf(stderr, "QTHREADS: " format, ## __VA_ARGS__)
#define print_error(format, ...)   do { fprintf(stderr, "QTHREADS ERROR: " format, ## __VA_ARGS__); \
                                        fflush(stderr); } while (0)

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
#ifdef QTHREAD_MULTITHREADED_SHEPHERDS
    qthread_worker_t   *me_worker = (qthread_worker_t *)arg;
    qthread_shepherd_t *me        = (qthread_shepherd_t *)me_worker->shepherd;
#else
    qthread_shepherd_t *me = (qthread_shepherd_t *)arg;
#endif
    qthread_shepherd_id_t     my_id = me->shepherd_id;
    qt_context_t              my_context;
    qt_threadqueue_t         *threadqueue;
    qt_threadqueue_private_t *localqueue = NULL;
    qthread_t                *t;
    int                       done = 0;

#ifdef QTHREAD_SHEPHERD_PROFILING
    me->total_time = qtimer_create();
    qtimer_t idle = qtimer_create();
#endif
#ifdef QTHREAD_RCRTOOL
# ifdef QTHREAD_RCRTOOL_STAT
    struct timespec adaptTimeStart;
    struct timespec adaptTimeStop;
    double          time = 0;
# endif
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
    signal(SIGUSR1, hup_handler);

    if (qaffinity && (me->node != UINT_MAX)) {
#ifdef QTHREAD_MULTITHREADED_SHEPHERDS
        qt_affinity_set(me_worker);
#else
        qt_affinity_set(me);
#endif
    }

#ifdef QTHREAD_RCRTOOL
    if (rcrtoollevel > 0) { // has cache control been turned off by an environment variable?
        // if so need to initialize -- inside of call (call also forces is to happen only once)
        // care needs to be taken so no one makes it into workhorse loop before allocation
        // has actually happened
        int gate = qthread_incr(&rcr_gate, 1);
        if(gate == 0) {
            maestro_allowed_workers();
            rcr_ready = 1;
        } else {
            while(!rcr_ready) SPINLOCK_BODY();  // spin until ready
        }
    }
#endif /* ifdef QTHREAD_RCRTOOL */

    /*******************************************************************************/
    /* Workhorse Loop                                                              */
    /*******************************************************************************/

    threadqueue = me->ready;
    assert(threadqueue);

    while (!done) {
#ifdef QTHREAD_SHEPHERD_PROFILING
        qtimer_start(idle);
#endif
        qthread_debug(SHEPHERD_DETAILS, "id(%i): fetching a thread from my queue...\n", my_id);

#ifdef QTHREAD_RCRTOOL
        if (rcrtoollevel > 1) {                     // has cache control been turned off by an environment variable?
            if (me_worker->packed_worker_id != 0) { // never idle shepherd 0 worker 0  -- needs to be active for termination
                maestro_allowed_workers();
                if (qlib->shepherds[my_id].active_workers > maestro_current_workers(my_id)) {
# ifdef QTHREAD_RCRTOOL_STAT
                    clock_gettime(CLOCK_MONOTONIC, &adaptTimeStart);
# endif
                    int active  = qthread_incr(&qlib->shepherds[my_id].active_workers, -1);
                    int current = maestro_current_workers(my_id);
                    while ((active > current) &&
                           (!active > 1) // never throttle the last active worker
                           ) {
                        COMPILER_FENCE; // is the fence enough of a slowdown to reduce
                        // pressure on me->active_workers ????
                        active  = me->active_workers + 1;
                        current = allowed_workers[my_id]; // use array don't recompute
                    }
                    MACHINE_FENCE;
                    qthread_incr(&qlib->shepherds[my_id].active_workers, 1);

# ifdef QTHREAD_RCRTOOL_STAT
                    clock_gettime(CLOCK_MONOTONIC, &adaptTimeStop);
                    time += (adaptTimeStop.tv_sec + adaptTimeStop.tv_nsec * 1e-9) - (adaptTimeStart.tv_sec + adaptTimeStart.tv_nsec * 1e-9);
# endif
                }
            }
        }
#endif  /* ifdef QTHREAD_RCRTOOL */
#ifdef QTHREAD_MULTITHREADED_SHEPHERDS
        while (!QTHREAD_CASLOCK_READ_UI(me_worker->active)) {
            SPINLOCK_BODY();
        }
#endif
        t = qt_scheduler_get_thread(threadqueue, localqueue, QTHREAD_CASLOCK_READ_UI(me->active));
        assert(t);
#ifdef QTHREAD_SHEPHERD_PROFILING
        qtimer_stop(idle);
        me->idle_count++;
        me->idle_time += qtimer_secs(idle);
        if (me->idle_maxtime < qtimer_secs(idle)) {
            me->idle_maxtime = qtimer_secs(idle);
        }
#endif

#ifdef QTHREAD_USE_ROSE_EXTENSIONS
qt_run:
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
#ifdef QTHREAD_RCRTOOL
            if (rcrtoollevel > 0) {
                qthread_incr(&qlib->shepherds[my_id].active_workers, -1); // not working spinning
            }
#endif
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

#ifdef QTHREAD_MULTITHREADED_SHEPHERDS
                me_worker->current = t;
#else
                me->current = t;
#endif
#ifdef HAVE_NATIVE_MAKECONTEXT
                getcontext(&my_context);
#endif
                qthread_debug(THREAD_DETAILS, "id(%u): shepherd context is %p, current = %p\n", my_id, &my_context);
                /* note: there's a good argument that the following should
                 * be: (*t->f)(t), however the state management would be
                 * more complex
                 */
                qthread_exec(t, &my_context);
#ifdef QTHREAD_DEBUG
# ifdef QTHREAD_MULTITHREADED_SHEPHERDS
                me_worker->current = NULL;
# else
                me->current = NULL;
# endif
#endif
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
                        assert(t->rdata->shepherd_ptr->ready != NULL);
                        qt_threadqueue_enqueue(t->rdata->shepherd_ptr->ready, t);
                        break;
                    default:
                        qthread_debug(THREAD_DETAILS, "id(%u): thread in state %i; that's illegal!\n", my_id, t->thread_state);
                        assert(0);
                        break;

                    case QTHREAD_STATE_YIELDED_NEAR: /* reschedule it */
                        t->thread_state = QTHREAD_STATE_RUNNING;
                        qthread_debug(THREAD_DETAILS | SHEPHERD_DETAILS,
                                      "id(%u): thread %i yielded near; rescheduling\n",
                                      my_id, t->thread_id);
#ifdef QTHREAD_USE_SPAWNCACHE
                        if (!qt_spawncache_yield(t))
#endif
                        {
                            qthread_t *f = qt_scheduler_get_thread(threadqueue, NULL,
                                                                   QTHREAD_CASLOCK_READ_UI(me->active));
                            qt_threadqueue_enqueue(me->ready, t);
                            qt_threadqueue_enqueue(me->ready, f);
                        }
                        break;
                    case QTHREAD_STATE_YIELDED: /* reschedule it */
                        t->thread_state = QTHREAD_STATE_RUNNING;
                        qthread_debug(THREAD_DETAILS | SHEPHERD_DETAILS,
                                      "id(%u): thread %i yielded; rescheduling\n",
                                      my_id, t->thread_id);
                        assert(me->ready != NULL);
                        qt_threadqueue_enqueue_yielded(me->ready, t);
                        break;

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
                        break;

                    case QTHREAD_STATE_PARENT_BLOCKED:
                        qthread_debug(THREAD_DETAILS, "id(%u): thread in state %i; that's illegal!\n", my_id, t->thread_state);
                        assert(0);
                        break;

                    case QTHREAD_STATE_PARENT_UNBLOCKED:
#ifdef QTHREAD_USE_ROSE_EXTENSIONS
                        {
                            qthread_t *parent = t->parent;
                            qthread_thread_free(t);
                            t               = parent;
                            t->thread_state = t->prev_thread_state;
                            goto qt_run;
                        }
#else
                        qthread_debug(THREAD_DETAILS, "id(%u): thread in state %i; that's illegal!\n", my_id, t->thread_state);
                        assert(0);
                        break;
#endif              /* ifdef QTHREAD_USE_ROSE_EXTENSIONS */
                    case QTHREAD_STATE_SYSCALL:
                        t->thread_state = QTHREAD_STATE_RUNNING;
                        qthread_debug(THREAD_DETAILS | IO_DETAILS | SHEPHERD_DETAILS,
                                      "id(%u): thread %i made a syscall\n",
                                      my_id, t->thread_id);
                        qt_blocking_subsystem_enqueue(t->rdata->blockedon.io);
                        break;

                    case QTHREAD_STATE_ASSASSINATED:
                        qthread_debug(THREAD_DETAILS | SHEPHERD_DETAILS,
                                      "id(%u): thread %i assassinated\n",
                                      my_id, t->thread_id);
                        /* need to clean up return value */
                        if (t->ret) {
                            if (t->flags & QTHREAD_RET_IS_SYNCVAR) {
                                qassert(qthread_syncvar_fill((syncvar_t *)t->ret), QTHREAD_SUCCESS);
                            } else {
                                qthread_debug(FEB_DETAILS, "tid %u assassinated, filling retval (%p)\n", t->thread_id, t->ret);
                                qassert(qthread_fill((aligned_t *)t->ret), QTHREAD_SUCCESS);
                            }
                        }
                        /* we can remove the stack etc. */
                        qthread_thread_free(t);
                        /* now, we're done cleaning, so we can unblock the assassination signal */
                        {
                            sigset_t iset;
                            qassert(sigemptyset(&iset), 0);
                            qassert(sigaddset(&iset, SIGUSR1), 0);
                            qassert(sigprocmask(SIG_UNBLOCK, &iset, NULL), 0);
                        }
                        break;

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

#ifdef QTHREAD_SHEPHERD_PROFILING
    qtimer_destroy(idle);
#endif
#ifdef QTHREAD_MULTITHREADED_SHEPHERDS
    qthread_debug(SHEPHERD_DETAILS, "id(%u): wkr(%u): finished\n",
                  my_id, me_worker->worker_id);
#else
    qthread_debug(SHEPHERD_DETAILS, "id(%u): finished\n",
                  my_id);
#endif
#ifdef QTHREAD_RCRTOOL_STAT
    if (rcrtoollevel > 2) {
        totalIdleTime += time;
        if (--idleCheckin == 1) { // next to last thread checkin (other than
                                  // master and I don't see how to include master
                                  // thread time (it calls qthread_finalize/exit
                                  // and never completes the workhorse loop
            printf("\tTotal Idle time = %10f (%10f)\n",
                   totalIdleTime,
                   totalIdleTime / ((qlib->nworkerspershep * qlib->nshepherds) - 2)); // rcrthread + master missing
        }

        if (rcrtoollevel > 2) {
            printf("\tIdle time (%d) = %10f\n", me_worker->packed_worker_id, time);
        }
    }
#endif /* ifdef QTHREAD_RCRTOOL_STAT */
    pthread_exit(NULL);
    return NULL;
}                      /*}}} */

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
 *
 * @name qthread_initialize
 *
 * @synopsis
 *
 * @return int On success, the system is ready to fork threads and 0 is
 *         returned. On error, an non-zero error code is returned.
 *
 * @error ENOMEM Not enough memory could be allocated.
 */

int API_FUNC qthread_initialize(void)
{                      /*{{{ */
    int                   r;
    size_t                i;
    uint_fast8_t          print_info      = 0;
    uint_fast8_t          need_sync       = 1;
    qthread_shepherd_id_t nshepherds      = 0;
    qthread_worker_id_t   nworkerspershep = 0;
    size_t                hw_par          = 0;

    QTHREAD_FASTLOCK_SETUP();
#ifdef QTHREAD_DEBUG
    QTHREAD_FASTLOCK_INIT(output_lock);
    {
        unsigned long dl = qt_internal_get_env_num("DEBUG_LEVEL", 0, 0);
# ifdef SST
        dl = 7;
# endif
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

    nshepherds = qt_internal_get_env_num("NUM_SHEPHERDS", 0, 0);
#ifdef QTHREAD_MULTITHREADED_SHEPHERDS
    nworkerspershep = qt_internal_get_env_num("NUM_WORKERS_PER_SHEPHERD", 0, 0);
    if (nworkerspershep > 0) {
        if (nshepherds == 0) {
            print_warning("Number of shepherds not specified - number of workers may be ignored\n");
        }
    }
#else
    nworkerspershep = 1;
#endif  /* ifdef QTHREAD_MULTITHREADED_SHEPHERDS */

    hw_par = qt_internal_get_env_num("HWPAR", nshepherds * nworkerspershep, nshepherds * nworkerspershep);
    if ((hw_par != 0) && (nshepherds != 0) && (nworkerspershep != 0)) {
        if (hw_par != (nshepherds * nworkerspershep)) {
            print_warning("Shepherd/worker parallelism directly specified (%u/%u); ignoring HWPAR (%u)\n",
                          (unsigned)nshepherds, (unsigned)nworkerspershep, (unsigned)hw_par);
            hw_par = nshepherds * nworkerspershep;
        }
    }

#if defined(QTHREAD_MUTEX_INCREMENT) || (QTHREAD_ASSEMBLY_ARCH == QTHREAD_POWERPC32)
    qlib->atomic_locks = MALLOC(sizeof(QTHREAD_FASTLOCK_TYPE) * QTHREAD_LOCKING_STRIPES);
    qassert_ret(qlib->atomic_locks, QTHREAD_MALLOC_ERROR);
    for (i = 0; i < QTHREAD_LOCKING_STRIPES; i++) {
        QTHREAD_FASTLOCK_INIT(qlib->atomic_locks[i]);
    }
#endif

    qthread_internal_alignment_init();
    qt_mpool_subsystem_init();
    qt_affinity_init(&nshepherds, &nworkerspershep);
    qt_hash_initialize_subsystem();

    if (hw_par != 0) {
        if ((hw_par < nshepherds) || (hw_par == 1)) {
            nworkerspershep = 1;
            nshepherds      = hw_par;
        } else if (hw_par > (nshepherds * nworkerspershep)) {
            nworkerspershep = (hw_par / nshepherds);
            if ((hw_par % nshepherds) != 0) {
                nworkerspershep++;
            }
        } else {
            nworkerspershep = hw_par / nshepherds;
            if (hw_par % nshepherds > 0) {
                nworkerspershep++;
            }
        }
    } else {
        hw_par = nshepherds * nworkerspershep;
    }

    print_info = qt_internal_get_env_num("INFO", 0, 1);
    if (print_info) {
        print_status("Using %i Shepherds\n", (int)nshepherds);
#ifdef QTHREAD_MULTITHREADED_SHEPHERDS
        print_status("Using %i Workers per Shepherd\n", (int)nworkerspershep);
#endif
    }

    if ((nshepherds == 1) && (nworkerspershep == 1)) {
        need_sync = 0;
    }
    QTHREAD_LOCKING_STRIPES = 2 << ((unsigned int)(log2(nshepherds * nworkerspershep)) + 1);
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
        qthread_internal_aligned_alloc(sizeof(uint64_strip_t) * nshepherds * nworkerspershep, 64);
#endif

    /* initialize the FEB-like locking structures */
#ifdef QTHREAD_COUNT_THREADS
    qlib->febs_stripes = MALLOC(sizeof(aligned_t) * QTHREAD_LOCKING_STRIPES);
    qassert_ret(qlib->febs_stripes, QTHREAD_MALLOC_ERROR);
# ifdef QTHREAD_MUTEX_INCREMENT
    qlib->febs_stripes_locks = MALLOC(sizeof(QTHREAD_FASTLOCK_TYPE) * QTHREAD_LOCKING_STRIPES);
    qassert_ret(qlib->febs_stripes_locks, QTHREAD_MALLOC_ERROR);
# endif
#endif /* ifdef QTHREAD_COUNT_THREADS */
    qlib->FEBs = MALLOC(sizeof(qt_hash) * QTHREAD_LOCKING_STRIPES);
    qassert_ret(qlib->FEBs, QTHREAD_MALLOC_ERROR);
    qlib->syncvars = MALLOC(sizeof(qt_hash) * QTHREAD_LOCKING_STRIPES);
    qassert_ret(qlib->syncvars, QTHREAD_MALLOC_ERROR);
    for (i = 0; i < QTHREAD_LOCKING_STRIPES; i++) {
#ifdef QTHREAD_COUNT_THREADS
        qlib->febs_stripes[i] = 0;
# ifdef QTHREAD_MUTEX_INCREMENT
        QTHREAD_FASTLOCK_INIT(qlib->febs_stripes_locks[i]);
# endif
#endif
        qlib->FEBs[i] = qt_hash_create(need_sync);
        qassert_ret(qlib->FEBs[i], QTHREAD_MALLOC_ERROR);
        qlib->syncvars[i] = qt_hash_create(need_sync);
        qassert_ret(qlib->syncvars[i], QTHREAD_MALLOC_ERROR);
    }

    /* initialize the kernel threads and scheduler */
    TLS_INIT(shepherd_structs);
#ifdef QTHREAD_USE_SPAWNCACHE
    qt_spawncache_init();
#endif
    qlib->nshepherds        = nshepherds;
    qlib->nworkerspershep   = nworkerspershep;
    qlib->nshepherds_active = nshepherds;
    qlib->shepherds         = (qthread_shepherd_t *)calloc(nshepherds, sizeof(qthread_shepherd_t));
    qlib->threadqueues      = (qt_threadqueue_t **)MALLOC(nshepherds * sizeof(qt_threadqueue_t *));
    qassert_ret(qlib->shepherds, QTHREAD_MALLOC_ERROR);
#ifdef QTHREAD_MUTEX_INCREMENT
    QTHREAD_FASTLOCK_INIT(qlib->nshepherds_active_lock);
# ifdef QTHREAD_MULTITHREADED_SHEPHERDS
    QTHREAD_FASTLOCK_INIT(qlib->nworkers_active_lock);
# endif
#endif

    qlib->qthread_stack_size = qt_internal_get_env_num("STACK_SIZE",
                                                       QTHREAD_DEFAULT_STACK_SIZE,
                                                       QTHREAD_DEFAULT_STACK_SIZE);
    qthread_debug(CORE_DETAILS, "qthread stack size: %u\n", qlib->qthread_stack_size);
    if (print_info) {
        print_status("Using %u byte stack size.\n", qlib->qthread_stack_size);
    }

#ifdef QTHREAD_RCRTOOL_STAT
    if (rcrtoollevel > 0) {
        idleCheckin = (nshepherds * nworkerspershep) - 1; // don't wait for RCR thread
    }
#endif

#ifdef QTHREAD_GUARD_PAGES
    {
        /* round stack size to nearest page */
        if (qlib->qthread_stack_size % pagesize) {
            qlib->qthread_stack_size +=
                pagesize - (qlib->qthread_stack_size % pagesize);
        }
    }
#endif

#ifdef TEAM_PROFILE
    qlib->team_create  = 0;
    qlib->team_destroy = 0;

    qlib->team_subteam_create  = 0;
    qlib->team_subteam_destroy = 0;

    qlib->team_sinc_create  = 0;
    qlib->team_sinc_destroy = 0;

    qlib->team_subteams_sinc_create  = 0;
    qlib->team_subteams_sinc_destroy = 0;

    qlib->team_leader_start = 0;
    qlib->team_leader_stop  = 0;

    qlib->team_watcher_start = 0;
    qlib->team_watcher_stop  = 0;
#endif /* ifdef TEAM_PROFILE */

    qlib->max_thread_id  = 1;
    qlib->max_unique_id  = 1;
    qlib->max_team_id    = 2; /* team 1 is reserved for the default team */
    qlib->team_count     = 0; /* count of existing teams */
    qlib->sched_shepherd = 0;
    QTHREAD_FASTLOCK_INIT(qlib->max_thread_id_lock);
    QTHREAD_FASTLOCK_INIT(qlib->max_team_id_lock);
    QTHREAD_FASTLOCK_INIT(qlib->team_count_lock);
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
#ifdef QTHREAD_MULTITHREADED_SHEPHERDS
        qlib->shepherds[i].workers = (qthread_worker_t *)calloc(nworkerspershep, sizeof(qthread_worker_t));
        qassert_ret(qlib->shepherds[i].workers, QTHREAD_MALLOC_ERROR);
#endif
    }
    {
        const char *aff = qt_internal_get_env_str("AFFINITY", "yes");

        if (aff && !strncmp(aff, "no", 3)) {
            qaffinity = 0;
        } else {
            qaffinity = 1;
        }
    }
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
    generic_stack_pool       =
# ifdef QTHREAD_GUARD_PAGES
        qt_mpool_create_aligned(qlib->qthread_stack_size + sizeof(struct qthread_runtime_data_s) +
                                (2 * getpagesize()), getpagesize());
# else
        qt_mpool_create_aligned(sizeof(struct qthread_runtime_data_s) + qlib->qthread_stack_size, 16); // stacks on most platforms must be 16-byte aligned (or less)
# endif
    generic_rdata_pool = qt_mpool_create(sizeof(struct qthread_runtime_data_s));
    generic_team_pool  = qt_mpool_create(sizeof(qt_team_t));
#endif /* ifndef UNPOOLED */
    initialize_hazardptrs();
    qt_feb_subsystem_init();
    qt_threadqueue_subsystem_init();
    qt_blocking_subsystem_init();

/* initialize the shepherd structures */
    for (i = 0; i < nshepherds; i++) {
        qthread_debug(SHEPHERD_DETAILS, "setting up shepherd %i (%p)\n", i, &qlib->shepherds[i]);
        qlib->shepherds[i].shepherd_id = (qthread_shepherd_id_t)i;
        QTHREAD_CASLOCK_INIT(qlib->shepherds[i].active, 1);
        qlib->shepherds[i].ready = qt_threadqueue_new();
        qassert_ret(qlib->shepherds[i].ready, QTHREAD_MALLOC_ERROR);
        qlib->threadqueues[i] = qlib->shepherds[i].ready;
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
    qlib->master_stack = calloc(1, qlib->master_stack_size);
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

    assert(qlib->mccoy_thread->rdata != NULL);
    qlib->mccoy_thread->rdata->shepherd_ptr   = &(qlib->shepherds[0]);
    qlib->mccoy_thread->rdata->stack          = NULL;
    qlib->mccoy_thread->rdata->tasklocal_size = 0;

    qthread_debug(CORE_DETAILS, "enqueueing mccoy thread\n");
#ifdef QTHREAD_MULTITHREADED_SHEPHERDS
    TLS_SET(shepherd_structs, (qthread_shepherd_t *)&(qlib->shepherds[0].workers[0]));
#else
    TLS_SET(shepherd_structs, &(qlib->shepherds[0])); // for hazardptrs
#endif
    qt_threadqueue_enqueue(qlib->shepherds[0].ready, qlib->mccoy_thread);
    qassert(getcontext(&(qlib->mccoy_thread->rdata->context)), 0);
    qassert(getcontext(&(qlib->master_context)), 0);
/* now build the context for the shepherd 0 */
    qthread_debug(CORE_DETAILS, "calling qthread_makecontext\n");
#ifdef QTHREAD_MULTITHREADED_SHEPHERDS
    qlib->shepherds[0].workers[0].shepherd = &qlib->shepherds[0];
    QTHREAD_CASLOCK_INIT(qlib->shepherds[0].workers[0].active, 1);
    qthread_debug(CORE_DETAILS, "initialized caslock 0,0 %p\n", &qlib->shepherds[0].workers[0].active);
    qlib->shepherds[0].workers[0].worker_id = 0;
    qlib->shepherds[0].workers[0].unique_id = qthread_internal_incr(&(qlib->max_unique_id),
                                                                    &qlib->max_unique_id_lock, 1);
#endif
    qthread_makecontext(&(qlib->master_context), qlib->master_stack,
                        qlib->master_stack_size,
#ifdef QTHREAD_MAKECONTEXT_SPLIT
                        (void (*)(void))qthread_master_wrapper,
#else
                        (void (*)(void))qthread_master,
#endif
#ifdef QTHREAD_MULTITHREADED_SHEPHERDS
                        &(qlib->shepherds[0].workers[0]),
#else
                        &(qlib->shepherds[0]),
#endif
                        &(qlib->mccoy_thread->rdata->context));
#ifndef QTHREAD_NO_ASSERTS
# ifdef QTHREAD_MULTITHREADED_SHEPHERDS
    shep0arg = &(qlib->shepherds[0].workers[0]);
# else
    shep0arg = &(qlib->shepherds[0]);
# endif
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

#ifdef QTHREAD_MULTITHREADED_SHEPHERDS
# ifdef QTHREAD_RCRTOOL
    QTHREAD_FASTLOCK_INIT(rcrtool_lock);
    rcrtoollevel    = qt_internal_get_env_num("RCRTOOL_LEVEL", 0, 0);
    rcrtoolloglevel = qt_internal_get_env_num("RCRTOOL_LOG_LEVEL", 0, 0);
    if (rcrtoollevel > 0) {
        qlib->nworkers_active = nshepherds * nworkerspershep - 1;
    } else {
        qlib->nworkers_active = nshepherds * nworkerspershep;
    }
# else /* ifdef QTHREAD_RCRTOOL */
    assert(hw_par > 0);
    qlib->nworkers_active = hw_par;
# endif /* ifdef QTHREAD_RCRTOOL */
#endif  /* QTHREAD_MULTITHREADED_SHEPHERDS */

/* spawn the shepherds */
#ifdef QTHREAD_MULTITHREADED_SHEPHERDS
    for (i = 0; i < nshepherds; ++i) {
        qthread_worker_id_t j;
        qthread_debug(SHEPHERD_DETAILS,
                      "forking workers for shepherd %i (%p)\n", i,
                      &qlib->shepherds[i]);
# ifdef QTHREAD_RCRTOOL
        qlib->shepherds[i].active_workers = nworkerspershep;         // race? between creation and workhorse loop putting guys to sleep??? akp 6/10/11
        if (rcrtoollevel > 0) {
            if (i == nshepherds - 1) {
                qlib->shepherds[i].active_workers--;                // the daemon uses up one worker
            }
        }
# endif
        for (j = 0; j < nworkerspershep; ++j) {
            qlib->shepherds[i].workers[j].nostealbuffer = calloc(STEAL_BUFFER_LENGTH,
                                                                 sizeof(qthread_t *));
            qlib->shepherds[i].workers[j].stealbuffer = calloc(STEAL_BUFFER_LENGTH,
                                                               sizeof(qthread_t *));
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
# ifdef QTHREAD_RCRTOOL
            if (rcrtoollevel > 0) {
                if ((i == nshepherds - 1) && (j == nworkerspershep - 1)) {
                    swinfo.nshepherds      = nshepherds;
                    swinfo.nworkerspershep = nworkerspershep;
                    swinfo.worker          = &qlib->shepherds[i].workers[j];
                    swinfo.qaffinity       = qaffinity;
                    if ((r = pthread_create(&rcrToolPThreadID, NULL, rcrtoolDaemon, &swinfo)) != 0) {
                        print_error("qthread_init: pthread_create() failed (%d): %s\n", r, strerror(errno));
                        return QTHREAD_THIRD_PARTY_ERROR;
                    }
                    if (rcrtoollevel > 0) { continue; }
                }
            }
# endif     /* ifdef QTHREAD_RCRTOOL */
            if ((r = pthread_create(&qlib->shepherds[i].workers[j].worker, NULL,
                                    qthread_master, &qlib->shepherds[i].workers[j])) != 0) {
                print_error("qthread_init: pthread_create() failed (%d): %s\n", r, strerror(errno));
                return QTHREAD_THIRD_PARTY_ERROR;
            }
            qthread_debug(SHEPHERD_DETAILS, "spawned shep %i worker %i\n", (int)i, (int)j);
        }
    }
#else /* ifdef QTHREAD_MULTITHREADED_SHEPHERDS */
    for (i = 1; i < nshepherds; ++i) {
        qthread_debug(SHEPHERD_DETAILS,
                      "forking shepherd %i (%p)\n", i,
                      &qlib->shepherds[i]);
        if ((r = pthread_create(&qlib->shepherds[i].shepherd, NULL, qthread_master, &qlib->shepherds[i])) != 0) {
            print_error("qthread_init: pthread_create() failed (%d): %s\n", r, strerror(errno));
            return QTHREAD_THIRD_PARTY_ERROR;
        }
    }
#endif /* ifdef QTHREAD_MULTITHREADED_SHEPHERDS */
    qthread_steal_disable();

    qthread_debug(CORE_DETAILS, "calling atexit\n");
    atexit(qthread_finalize);

    qthread_debug(CORE_DETAILS, "calling component init functions\n");
    qt_feb_barrier_internal_init();
#ifdef QTHREAD_USE_ROSE_EXTENSIONS
# ifdef QTHREAD_MULTITHREADED_SHEPHERDS
#  ifdef QTHREAD_RCRTOOL
    if (rcrtoollevel > 0) {
        qt_global_barrier_init(qlib->nshepherds * qlib->nworkerspershep - 1, 0);
    } else
#  endif
    qt_global_barrier_init(qlib->nshepherds * qlib->nworkerspershep, 0);
    qt_global_arrive_first_init(qthread_num_workers() - 1, 0);
# else
    qt_global_barrier_init(nshepherds, 0);
    qt_global_arrive_first_init(nshepherds - 1, 0);
# endif
#endif /* ifdef QTHREAD_USE_ROSE_EXTENSIONS */

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

static void qt_hash_call_cb(const qt_key_t      addr,
                            qthread_addrstat_t *m,
                            void               *arg)
{
    QTHREAD_FASTLOCK_LOCK(&m->lock);
    for (qthread_addrres_t *curs = m->EFQ; curs != NULL; curs = curs->next) {
        qthread_t *waiter = curs->waiter;
        void      *tls;
        if (waiter->rdata->tasklocal_size <= qlib->qthread_tasklocal_size) {
            if (waiter->flags & QTHREAD_BIG_STRUCT) {
                tls = &waiter->data[qlib->qthread_argcopy_size];
            } else {
                tls = waiter->data;
            }
        } else {
            if (waiter->flags & QTHREAD_BIG_STRUCT) {
                tls = *(void **)&waiter->data[qlib->qthread_argcopy_size];
            } else {
                tls = *(void **)&waiter->data[0];
            }
        }
        ((qt_feb_callback_f)arg)((void *)addr, waiter->f, waiter->arg, waiter->ret, waiter->thread_id, tls);
        curs = curs->next;
    }
    for (qthread_addrres_t *curs = m->FEQ; curs != NULL; curs = curs->next) {
        qthread_t *waiter = curs->waiter;
        void      *tls;
        if (waiter->rdata->tasklocal_size <= qlib->qthread_tasklocal_size) {
            if (waiter->flags & QTHREAD_BIG_STRUCT) {
                tls = &waiter->data[qlib->qthread_argcopy_size];
            } else {
                tls = waiter->data;
            }
        } else {
            if (waiter->flags & QTHREAD_BIG_STRUCT) {
                tls = *(void **)&waiter->data[qlib->qthread_argcopy_size];
            } else {
                tls = *(void **)&waiter->data[0];
            }
        }
        ((qt_feb_callback_f)arg)((void *)addr, waiter->f, waiter->arg, waiter->ret, waiter->thread_id, tls);
    }
    for (qthread_addrres_t *curs = m->FFQ; curs != NULL; curs = curs->next) {
        qthread_t *waiter = curs->waiter;
        void      *tls;
        if (waiter->rdata->tasklocal_size <= qlib->qthread_tasklocal_size) {
            if (waiter->flags & QTHREAD_BIG_STRUCT) {
                tls = &waiter->data[qlib->qthread_argcopy_size];
            } else {
                tls = waiter->data;
            }
        } else {
            if (waiter->flags & QTHREAD_BIG_STRUCT) {
                tls = *(void **)&waiter->data[qlib->qthread_argcopy_size];
            } else {
                tls = *(void **)&waiter->data[0];
            }
        }
        ((qt_feb_callback_f)arg)((void *)addr, waiter->f, waiter->arg, waiter->ret, waiter->thread_id, tls);
    }
    QTHREAD_FASTLOCK_UNLOCK(&m->lock);
}

void INTERNAL qthread_print_FEB_callback(qt_feb_callback_f cb)
{
    for (unsigned int i = 0; i < QTHREAD_LOCKING_STRIPES; i++) {
        qt_hash_callback(qlib->FEBs[i],
                         (qt_hash_callback_fn)qt_hash_call_cb, cb);
    }
}

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

int print_FEBs(int *ct);
int print_FEBs(int *ct)
{
    for (unsigned int i = 0; i < QTHREAD_LOCKING_STRIPES; i++) {
        qt_hash_callback(qlib->FEBs[i],
                         (qt_hash_callback_fn)qt_hash_print_addrstat, ct);
    }
    return 0;
}

#endif /* ifdef QTHREAD_DEBUG */

void API_FUNC qthread_finalize(void)
{                      /*{{{ */
    int                   r;
    qthread_shepherd_id_t i;
    qthread_t            *t;

#ifdef QTHREAD_MULTITHREADED_SHEPHERDS
    qthread_worker_t *worker;
#endif

    /* Sanity check */
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

    // Wait for all team structures to be reclaimed.
    while (qlib->team_count) {
#ifdef QTHREAD_DEBUG
        unsigned int ct = qlib->team_count;
#endif
        qthread_yield();
#ifdef QTHREAD_DEBUG
        if (ct != qlib->team_count) {
            printf("waiting for %u teams...\n", (unsigned int)qlib->team_count);
        }
#endif
    }

    qthread_shepherd_t *shep0 = &(qlib->shepherds[0]);

#ifdef QTHREAD_RCRTOOL_STAT
    if (rcrtoollevel > 0) {
        print_status("RCR resource contenton managment limited thread count %d times\n", adaptiveSetHigh);
    }
#endif
#ifdef QTHREAD_MULTITHREADED_SHEPHERDS
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
        //      qt_threadqueue_enqueue(shep0->ready, worker->current);
        return; // AKP 11/2/11 I think that is if statement catches the case that exit is called within a
        // parallel region so a random stream reaches here.  we return rather than requeue because we just
        // want to exit (something bad happened) [my speculation]
    }
#endif /* ifdef QTHREAD_MULTITHREADED_SHEPHERDS */
#ifdef TEAM_PROFILE
    qt_team_profile();
#endif

    qthread_debug(CORE_CALLS, "began.\n");

    /* rcm - probably need to put a "turn off the library flag" here, but,
     * the programmer can ensure that no further threads are forked for now
     */

    /* enqueue the termination thread sentinal */
#ifdef QTHREAD_SHEPHERD_PROFILING
    qtimer_stop(shep0->total_time);
#endif

#ifdef QTHREAD_MULTITHREADED_SHEPHERDS
    for (i = 0; i < qlib->nshepherds; i++) {
        qthread_worker_id_t j;
        for (j = 0; j < qlib->nworkerspershep; j++) {
            if ((i == 0) && (j == 0)) {
                continue;   /* None for shepard 0's worker 0 */
            }
# ifdef QTHREAD_RCRTOOL
            if ((rcrtoollevel > 0) && ((i == qlib->nshepherds - 1) && (j == qlib->nworkerspershep - 1))) {
                // Tell RCRTool thread to stop
                rcrToolContinue = 0;
            }
# endif
            qthread_debug(SHEPHERD_DETAILS, "terminating worker %i:%i\n", (int)i, (int)j);
            t = qthread_thread_new(NULL, NULL, 0, NULL, NULL, 0);
            assert(t != NULL);         /* what else can we do? */
            t->thread_state = QTHREAD_STATE_TERM_SHEP;
            t->thread_id    = QTHREAD_NON_TASK_ID;
            t->flags        = QTHREAD_UNSTEALABLE;
            qt_threadqueue_enqueue(qlib->shepherds[i].ready, t);
            if (!QTHREAD_CASLOCK_READ_UI(qlib->shepherds[i].workers[j].active)) {
                qthread_debug(SHEPHERD_DETAILS, "re-enabling worker %i:%i, so he can exit\n", (int)i, (int)j);
                (void)QT_CAS(qlib->shepherds[i].workers[j].active, 0, 1);
            }
        }
    }
#else /* ifdef QTHREAD_MULTITHREADED_SHEPHERDS */
    for (i = 1; i < qlib->nshepherds; i++) {
        qthread_debug(SHEPHERD_DETAILS, "terminating shepherd %i\n", (int)i);
        t = qthread_thread_new(NULL, NULL, 0, NULL, NULL, 0);
        assert(t != NULL);     /* what else can we do? */
        t->thread_state = QTHREAD_STATE_TERM_SHEP;
        t->thread_id    = QTHREAD_NON_TASK_ID;
        t->flags        = QTHREAD_UNSTEALABLE;
        qt_threadqueue_enqueue(qlib->shepherds[i].ready, t);
    }
#endif /* ifdef QTHREAD_MULTITHREADED_SHEPHERDS */

#ifdef QTHREAD_USE_ROSE_EXTENSIONS
    qthread_debug(BARRIER_DETAILS, "destroying the global barrier\n");
    qt_global_barrier_destroy();
#endif
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
#ifdef QTHREAD_MULTITHREADED_SHEPHERDS
    for (i = 0; i < qlib->nshepherds; i++) {
        /* With multi-threaded shepherds, do join shepherd 0 workers, but not worker 0 */
        qthread_worker_id_t j;
        qthread_shepherd_t *shep = &(qlib->shepherds[i]);
        qthread_debug(SHEPHERD_DETAILS, "waiting for shepherd %i to exit\n", (int)i);
        for (j = 0; j < qlib->nworkerspershep; j++) {
            if ((i == 0) && (j == 0)) {
                continue;  /* This leaves out shepherd 0's worker 0 */
            }
# ifdef QTHREAD_RCRTOOL
            if ((rcrtoollevel > 0) && ((i == qlib->nshepherds - 1) && (j == qlib->nworkerspershep - 1))) {
                if ((r = pthread_join(rcrToolPThreadID, NULL)) != 0) {
                    print_error("qthread_finalize: pthread_join() of rcrTool failed (%d, or \"%s\")\n", r, strerror(r));
                    abort();
                }
                continue;
            }
# endif
            if ((r = pthread_join(shep->workers[j].worker, NULL)) != 0) {
                print_error("qthread_finalize: pthread_join() of shep %i worker %i failed (%d, or \"%s\")\n",
                            (int)i, (int)j, r, strerror(r));
                abort();
            }
        }
        if (i == 0) { continue; }
#else /* ifdef QTHREAD_MULTITHREADED_SHEPHERDS */
    for (i = 1; i < qlib->nshepherds; i++) {
        qthread_shepherd_t *shep = &(qlib->shepherds[i]);
        qthread_debug(SHEPHERD_DETAILS, "waiting for shepherd %i to exit\n", (int)i);
        if ((r = pthread_join(shep->shepherd, NULL)) != 0) {
            print_error("qthread_finalize: pthread_join() of shep %i failed (%d, or \"%s\")\n",
                        (int)i, r, strerror(r));
            abort();
        }
#endif  /* ifdef QTHREAD_MULTITHREADED_SHEPHERDS */
    }
    /**********************************************************************/
    qthread_debug(SHEPHERD_BEHAVIOR | CORE_BEHAVIOR, "******* Now running with only ONE thread! *******\n");
    /**********************************************************************/
#ifdef QTHREAD_MULTITHREADED_SHEPHERDS
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
#else /* ifdef QTHREAD_MULTITHREADED_SHEPHERDS */
    for (i = 1; i < qlib->nshepherds; i++) {
        qthread_shepherd_t *shep = &(qlib->shepherds[i]);
#endif  /* ifdef QTHREAD_MULTITHREADED_SHEPHERDS */
        QTHREAD_CASLOCK_DESTROY(shep->active);
        qt_threadqueue_free(shep->ready);

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

    for (i = 0; i < QTHREAD_LOCKING_STRIPES; i++) {
        qthread_debug(FEB_DETAILS, "destroying feb infrastructure of shep %i\n", (int)i);
        qt_hash_destroy_deallocate(qlib->FEBs[i],
                                   (qt_hash_deallocator_fn)
                                   qthread_addrstat_delete);
        qt_hash_destroy_deallocate(qlib->syncvars[i],
                                   (qt_hash_deallocator_fn)
                                   qthread_addrstat_delete);

#if defined(QTHREAD_MUTEX_INCREMENT) || (QTHREAD_ASSEMBLY_ARCH == QTHREAD_POWERPC32)
        QTHREAD_FASTLOCK_DESTROY(qlib->atomic_locks[i]);
#endif
#ifdef QTHREAD_COUNT_THREADS
        print_status("bin %i used %u times for FEBs\n", i, (unsigned int)qlib->febs_stripes[i]);
# ifdef QTHREAD_MUTEX_INCREMENT
        QTHREAD_FASTLOCK_DESTROY(qlib->febs_stripes_locks[i]);
# endif
#endif
    }
#ifdef QTHREAD_MUTEX_INCREMENT
    QTHREAD_FASTLOCK_DESTROY(qlib->nshepherds_active_lock);
# ifdef QTHREAD_MULTITHREADED_SHEPHERDS
    QTHREAD_FASTLOCK_DESTROY(qlib->nworkers_active_lock);
# endif
#endif
    qthread_debug(CORE_DETAILS, "calling late cleanup functions\n");
    while (qt_cleanup_late_funcs != NULL) {
        struct qt_cleanup_funcs_s *tmp = qt_cleanup_late_funcs;
        qt_cleanup_late_funcs = tmp->next;
        tmp->func();
        FREE(tmp, sizeof(struct qt_cleanup_funcs_s));
    }
    qthread_debug(FEB_DETAILS, "destroy feb infrastructure arrays\n");
    FREE(qlib->FEBs, sizeof(qt_hash) * QTHREAD_LOCKING_STRIPES);
    FREE(qlib->syncvars, sizeof(qt_hash) * QTHREAD_LOCKING_STRIPES);
#if defined(QTHREAD_MUTEX_INCREMENT) || (QTHREAD_ASSEMBLY_ARCH == QTHREAD_POWERPC32)
    FREE((void *)qlib->atomic_locks, sizeof(QTHREAD_FASTLOCK_TYPE) * QTHREAD_LOCKING_STRIPES);
#endif
#ifdef QTHREAD_COUNT_THREADS
    FREE(qlib->febs_stripes, sizeof(aligned_t) * QTHREAD_LOCKING_STRIPES);
# ifdef QTHREAD_MUTEX_INCREMENT
    FREE(qlib->febs_stripes_locks, sizeof(QTHREAD_FASTLOCK_TYPE) * QTHREAD_LOCKING_STRIPES);
# endif
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
    QTHREAD_FASTLOCK_DESTROY(qlib->max_team_id_lock);
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
    qt_mpool_destroy(generic_team_pool);
    generic_team_pool = NULL;
#endif /* ifndef UNPOOLED */
    qthread_debug(CORE_DETAILS, "destroy global shepherd array\n");
    FREE(qlib->shepherds, qlib->nshepherds * sizeof(qthread_shepherd_t));
    FREE(qlib->threadqueues, qlib->nshepherds * sizeof(qt_threadqueue_t *));
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

#ifdef QTHREAD_MULTITHREADED_SHEPHERDS
    qthread_worker_t *worker = qthread_internal_getworker();
    if (worker == NULL) {
        // this may also be null, but in the slow path, the logic is sound
        return TLS_GET(IO_task_struct);
    } else {
        return worker->current;
    }

#else /* ifdef QTHREAD_MULTITHREADED_SHEPHERDS */
    qthread_shepherd_t *shep = qthread_internal_getshep();
    if (shep == NULL) {
        // this may also be null, but in the slow path, the logic is sound
        return TLS_GET(IO_task_struct);
    } else {
        return shep->current;
    }
#endif /* ifdef QTHREAD_MULTITHREADED_SHEPHERDS */
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
                *data_blob = realloc(*data_blob, size);
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

        case BUSYNESS:
        {
            qthread_shepherd_t *shep = qthread_internal_getshep();
            if (shep == NULL) {
                return (size_t)(-1);
            } else {
                return qt_threadqueue_advisory_queuelen(shep->ready);
            }
        }
        case ACTIVE_SHEPHERDS:
            return (size_t)(qlib->nshepherds_active);

        case TOTAL_SHEPHERDS:
            return (size_t)(qlib->nshepherds);

        case ACTIVE_WORKERS:
#ifdef QTHREAD_MULTITHREADED_SHEPHERDS
            return (size_t)(qlib->nworkers_active);

#else
            return (size_t)(qlib->nshepherds_active);
#endif

        case TOTAL_WORKERS:
#ifdef QTHREAD_MULTITHREADED_SHEPHERDS
            return (size_t)(qlib->nworkerspershep * qlib->nshepherds);

#else
            return (size_t)(qlib->nshepherds);
#endif

        case CURRENT_WORKER:
#ifdef QTHREAD_MULTITHREADED_SHEPHERDS
            {
                qthread_worker_t *worker = (qthread_worker_t *)TLS_GET(shepherd_structs);

                return worker ? (worker->worker_id) : NO_WORKER;
            }
#else
            return 0;
#endif  /* ifdef QTHREAD_MULTITHREADED_SHEPHERDS */

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

#define TEAM_EUREKA_EXIT(team_id)          (((saligned_t)team_id) > 1)
#define TEAM_EUREKA_EUREKA(team_id)        (((saligned_t)team_id) < 0)
#define TEAM_EUREKA_ID(team_id)            (team_id)
#define TEAM_EUREKA_SIGNAL_EXIT(team_id)   (team_id)
#define TEAM_EUREKA_SIGNAL_EUREKA(team_id) ((saligned_t)(-team_id))

static aligned_t qt_team_watcher(void *args_)
{   /*{{{*/
    aligned_t    code   = 0;
    qt_team_id_t myteam = qt_team_id();

    qt_team_t *team = (qt_team_t *)args_;

    assert(team);

    aligned_t *parent_eureka = team->parent_eureka;
    assert(parent_eureka);

    qthread_debug(FEB_DETAILS, "watcher (tid %u) of team %u filling watcher_started (%p)\n", qthread_id(), myteam, &team->watcher_started);
    qthread_fill(&team->watcher_started);
#ifdef TEAM_PROFILE
    qthread_incr(&qlib->team_watcher_start, 1);
#endif

    do {
        qthread_debug(FEB_DETAILS, "team %u's watcher (tid %u) waiting for a eureka or a team exit (%p)\n", myteam, qthread_id(), parent_eureka);
        qthread_readFF(&code, parent_eureka);

        if (TEAM_EUREKA_EXIT(code)) {
            if (myteam == TEAM_EUREKA_ID(code)) {
                // Reset the FEB and exit
                qthread_debug(FEB_DETAILS, "team %u's watcher (tid %u) preparing to exit, emptying parent's eureka (%p)\n", myteam, qthread_id(), parent_eureka);
                qthread_empty(parent_eureka);
                break;
            } else {
                // Yield control of the resource while waiting for FEB to be reset
                qthread_yield();
            }
        } else if (TEAM_EUREKA_EUREKA(code)) {
            assert("Error: hard-eureka not implemented" && 0);
        } else {
            assert("Error: watcher received code 0 or 1" && 0);
        }
    } while (1);

#ifdef TEAM_PROFILE
    qthread_incr(&qlib->team_watcher_stop, 1);
#endif

    return 0;
} /*}}}*/

#ifdef TEAM_PROFILE
void qt_team_profile(void)
{   /*{{{*/
    fprintf(stderr, "\n======== Teams Profile ========\n\n");
    fprintf(stderr, "Teams:\n");
    fprintf(stderr, "%8lu create\n",
            (unsigned long)qlib->team_create);
    fprintf(stderr, "%8lu destroy\n",
            (unsigned long)qlib->team_destroy);
    fprintf(stderr, "%8lu teams were not destroyed\n",
            (unsigned long)(qlib->team_create - qlib->team_destroy));

    fprintf(stderr, "\nSubteams:\n");
    fprintf(stderr, "%8lu subteam create\n",
            (unsigned long)qlib->team_subteam_create);
    fprintf(stderr, "%8lu subteam destroy\n",
            (unsigned long)qlib->team_subteam_destroy);
    fprintf(stderr, "%8lu subteams were not destroyed\n",
            (unsigned long)(qlib->team_subteam_create -
                            qlib->team_subteam_destroy));

    fprintf(stderr, "\nSincs:\n");
    fprintf(stderr, "%8lu sinc_create\n",
            (unsigned long)qlib->team_sinc_create);
    fprintf(stderr, "%8lu sinc_destroy\n",
            (unsigned long)qlib->team_sinc_destroy);
    fprintf(stderr, "%8lu sincs were not destroyed\n",
            (unsigned long)(qlib->team_sinc_create -
                            qlib->team_sinc_destroy));

    fprintf(stderr, "\nSubteams Sincs:\n");
    fprintf(stderr, "%8lu subteams_sinc_create\n",
            (unsigned long)qlib->team_subteams_sinc_create);
    fprintf(stderr, "%8lu subteams_sinc_destroy\n",
            (unsigned long)qlib->team_subteams_sinc_destroy);
    fprintf(stderr, "%8lu subteams sncs were not destroyed\n",
            (unsigned long)(qlib->team_subteams_sinc_create -
                            qlib->team_subteams_sinc_destroy));

    fprintf(stderr, "\nLeaders:\n");
    fprintf(stderr, "%8lu leader_start\n",
            (unsigned long)qlib->team_leader_start);
    fprintf(stderr, "%8lu leader_stop\n",
            (unsigned long)qlib->team_leader_stop);
    fprintf(stderr, "%8lu leaders did not stop\n",
            (unsigned long)(qlib->team_leader_start -
                            qlib->team_leader_stop));

    fprintf(stderr, "\nWatchers:\n");
    fprintf(stderr, "%8lu watcher_start\n",
            (unsigned long)qlib->team_watcher_start);
    fprintf(stderr, "%8lu watcher_stop\n",
            (unsigned long)qlib->team_watcher_stop);
    fprintf(stderr, "%8lu watchers did not stop\n",
            (unsigned long)(qlib->team_watcher_start -
                            qlib->team_watcher_stop));
    fprintf(stderr, "\n");

    fprintf(stderr, "Total current team count %8lu\n",
            (unsigned long)(qlib->team_count));
} /*}}}*/

#endif /* ifdef TEAM_PROFILE */

/* Returns the team id. If there is no team structure associated with the task,
 * it is considered to be in the default team with id 1. */
qt_team_id_t API_FUNC qt_team_id(void)
{   /*{{{*/
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
} /*}}}*/

/* Returns the parent team id. If there is no team structure associated with
 * the task, it is considered to be in the default team with id, and to have
 * no parent (id 0). */
qt_team_id_t API_FUNC qt_team_parent_id(void)
{   /*{{{*/
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
} /*}}}*/

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

#ifdef QTHREAD_USE_ROSE_EXTENSIONS
    t->id                    = 0;
    t->currentParallelRegion = NULL;
    t->task_counter          = 0;
    t->parent                = NULL;
    t->prev_thread_state     = QTHREAD_STATE_ILLEGAL;
#endif

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

static QINLINE void qthread_thread_free(qthread_t *t)
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
        free(t->arg); // I don't record the size of this anywhere, so I can't scribble it
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
// in Rose only code -- call function with rose argument list
// pulled from HPCToolkit externals

int in_qthread_fence(void *addr);
extern void *qthread_fence1;
extern void *qthread_fence2;

# define MONITOR_ASM_LABEL(name)             \
    __asm__ __volatile__ (".globl " # name); \
    __asm__ __volatile__ (# name ":")
#endif /* ifdef QTHREAD_ALLOW_HPCTOOLKIT_STACK_UNWINDING */

// This is called in `qthread_wrapper()` immediately after each team task
// returns.
static QINLINE void qthread_internal_teamfinish(qt_team_t *team,
                                                uint8_t    flags)
{   /*{{{*/
    assert(team != NULL);

    if (QTHREAD_TEAM_LEADER & flags) {
        // 1. This task is a sub/team leader

        if (QTHREAD_NON_TEAM_ID == team->parent_id) {
            // 1.1. This task is a team leader
            assert(team->sinc);
            assert(team->subteams_sinc);
            assert(NULL == team->parent_eureka);
            assert(NULL == team->parent_subteams_sinc);

            // Wait for all participants on team sinc after submitting to
            // team sinc for the leader
            qt_sinc_submit(team->sinc, NULL);
            qt_sinc_wait(team->sinc, NULL);

            // Wait for all participants on team subteams sinc
            qt_sinc_submit(team->subteams_sinc, NULL);
            qt_sinc_wait(team->subteams_sinc, NULL);

            // Clean up
            qt_sinc_destroy(team->sinc);
            team->sinc = NULL;
            qt_sinc_destroy(team->subteams_sinc);
            team->subteams_sinc = NULL;
            qthread_debug(FEB_DETAILS, "tid %u killing team %u, filling my own eureka (%p)\n", qthread_id(), team->team_id, &team->eureka);
            qthread_fill(&team->eureka);

            FREE_TEAM(team);

            qthread_internal_incr(&(qlib->team_count), &qlib->team_count_lock, -1);

#ifdef TEAM_PROFILE
            qthread_incr(&qlib->team_destroy, 1);
#endif
        } else {
            // 1.2. This task is a subteam leader
            assert(team->sinc);
            assert(team->subteams_sinc);

            if (QTHREAD_DEFAULT_TEAM_ID == team->parent_id) {
                // 1.2.1. This is a subteam of the default team
                assert(NULL == team->parent_eureka);
                assert(NULL == team->parent_subteams_sinc);

                // Wait for all participants on team sinc after subtmitting
                // to team sinc - there is no watcher
                qt_sinc_submit(team->sinc, NULL);
                qt_sinc_wait(team->sinc, NULL);

                // Wait for all participants on team subteams sinc
                qt_sinc_submit(team->subteams_sinc, NULL);
                qt_sinc_wait(team->subteams_sinc, NULL);

                // Clean up
                qt_sinc_destroy(team->sinc);
                team->sinc = NULL;
                qt_sinc_destroy(team->subteams_sinc);
                team->subteams_sinc = NULL;
            } else {
                // 1.2.2. This is a subteam of a non-default team
                assert(team->parent_eureka);
                assert(team->parent_subteams_sinc);

                // Wait for all participants on team sinc after subtmitting
                // to team sinc for leader and watcher; reset team sinc
                // for watcher
                qt_sinc_submit(team->sinc, NULL);
                qt_sinc_submit(team->sinc, NULL);
                qt_sinc_wait(team->sinc, NULL);
                qt_sinc_reset(team->sinc, 1);

                // Wait for all participants on team subteams sinc
                qt_sinc_submit(team->subteams_sinc, NULL);
                qt_sinc_wait(team->subteams_sinc, NULL);

                // Signal watcher to exit and wait for it
                qthread_debug(FEB_DETAILS, "tid %u killing team %u signalling team %u's watcher (%p)\n", qthread_id(), team->team_id, team->parent_id, team->parent_eureka);
                qthread_writeEF_const(team->parent_eureka,
                                      TEAM_EUREKA_SIGNAL_EXIT(team->team_id));
                qt_sinc_wait(team->sinc, NULL);

                // Submit to parent team subteams sinc
                qt_sinc_submit(team->parent_subteams_sinc, NULL);

                // Clean up
                qt_sinc_destroy(team->sinc);
                team->sinc = NULL;
                qt_sinc_destroy(team->subteams_sinc);
                team->subteams_sinc = NULL;

                team->parent_eureka        = NULL;
                team->parent_subteams_sinc = NULL;
            }

            qthread_debug(FEB_DETAILS, "tid %u killing team %u, filling my own eureka (%p)\n", qthread_id(), team->team_id, &team->eureka);
            qthread_fill(&team->eureka);

            FREE_TEAM(team);

            qthread_internal_incr(&(qlib->team_count), &qlib->team_count_lock, -1);

#ifdef TEAM_PROFILE
            qthread_incr(&qlib->team_subteam_destroy, 1);
#endif
        }

#ifdef TEAM_PROFILE
        qthread_incr(&qlib->team_sinc_destroy, 1);
        qthread_incr(&qlib->team_subteams_sinc_destroy, 1);
        qthread_incr(&qlib->team_leader_stop, 1);
#endif
    } else {
        // 2. This task is not a sub/team leader: a watcher or participant
        assert(team);

        // Submit to the team sinc
        qt_sinc_submit(team->sinc, NULL);
    }
} /*}}}*/

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
            qthread_debug(FEB_DETAILS, "tid %u emptying team %u's watcher_started (%p)\n", t->thread_id, t->team->team_id, &t->team->watcher_started);
            qthread_empty(&t->team->watcher_started);
            qthread_fork(qt_team_watcher, t->team, NULL);
            qthread_readFF(NULL, &t->team->watcher_started);
        }
    }

    if (t->ret) {
        qthread_debug(THREAD_DETAILS, "tid %u, with flags %u, handling retval\n", t->thread_id, t->flags);
        if (t->flags & QTHREAD_RET_IS_SINC) {
            if (t->flags & QTHREAD_RET_IS_VOID_SINC) {
                (t->f)(t->arg);
                if (NULL != t->team) { qthread_internal_teamfinish(t->team, t->flags); }
                qt_sinc_submit((qt_sinc_t *)t->ret, NULL);
            } else {
                aligned_t retval = (t->f)(t->arg);
                if (NULL != t->team) { qthread_internal_teamfinish(t->team, t->flags); }
                qt_sinc_submit((qt_sinc_t *)t->ret, &retval);
            }
        } else if (t->flags & QTHREAD_RET_IS_SYNCVAR) {
            /* this should avoid problems with irresponsible return values */
            uint64_t retval = INT64TOINT60((t->f)(t->arg));
            if (NULL != t->team) { qthread_internal_teamfinish(t->team, t->flags); }
            qassert(qthread_syncvar_writeEF_const((syncvar_t *)t->ret, retval), QTHREAD_SUCCESS);
        } else {
            aligned_t retval = (t->f)(t->arg);
            if (NULL != t->team) { qthread_internal_teamfinish(t->team, t->flags); }
            qthread_debug(FEB_DETAILS, "tid %u filling retval (%p)\n", t->thread_id, t->ret);
            qassert(qthread_writeEF_const((aligned_t *)t->ret, retval), QTHREAD_SUCCESS);
        }
    } else {
        assert(t->f);
        (t->f)(t->arg);
        if (NULL != t->team) { qthread_internal_teamfinish(t->team, t->flags); }
    }

    t->thread_state = QTHREAD_STATE_TERMINATED;

#ifdef QTHREAD_USE_ROSE_EXTENSIONS
    if ((t->parent != NULL) && (t->ret == NULL)) {
        qthread_t *parent              = t->parent;
        aligned_t *parent_task_counter = &(parent->task_counter);
        aligned_t  newval, oldval, test;
        oldval = *parent_task_counter;
        while (1) {
            assert(tcount_get_children(oldval) > 0);
            aligned_t waiting  = tcount_get_waiting(oldval);
            aligned_t children = tcount_get_children(oldval) - 1;
            newval = tcount_create(waiting, children);
            // atomic decrement cannot be used because
            // the get_waiting() mask must be preserved
            test = qthread_cas(parent_task_counter, oldval, newval);
            if (test == oldval) { break; }
            oldval = test;
        }
        if (newval == tcount_finished_state) {
            while(parent->thread_state != QTHREAD_STATE_PARENT_BLOCKED) SPINLOCK_BODY();
            t->thread_state = QTHREAD_STATE_PARENT_UNBLOCKED;
        }
    }
#endif /* ifdef QTHREAD_USE_ROSE_EXTENSIONS */

#ifdef QTHREAD_COUNT_THREADS
    QTHREAD_FASTLOCK_LOCK(&effconcurrentthreads_lock);
    effconcurrentthreads--;
    QTHREAD_FASTLOCK_UNLOCK(&effconcurrentthreads_lock);
    QTHREAD_FASTLOCK_LOCK(&concurrentthreads_lock);
    assert(concurrentthreads > 0);
    concurrentthreads--;
    QTHREAD_FASTLOCK_UNLOCK(&concurrentthreads_lock);
#endif
    if (t->flags & QTHREAD_FUTURE) {
        future_exit();
    }
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
#ifdef NEED_RLIMIT
    struct rlimit rlp;
#endif

    assert(t != NULL);
    assert(c != NULL);

    if ((t->flags & QTHREAD_SIMPLE) == 0) {
        if (t->thread_state == QTHREAD_STATE_NEW) {
            qthread_debug(SHEPHERD_DETAILS,
                          "t(%p), c(%p): type is QTHREAD_THREAD_NEW!\n",
                          t, c);
            t->thread_state = QTHREAD_STATE_RUNNING;

            qassert(getcontext(&t->rdata->context), 0); /* puts the current context into t->rdata->context */
            qthread_makecontext(&t->rdata->context, t->rdata->stack, qlib->qthread_stack_size,
                                (void (*)(void))qthread_wrapper, t, c);
#ifdef HAVE_NATIVE_MAKECONTEXT
        } else {
            t->rdata->context.uc_link = c; /* NULL pthread_exit() */
#endif
        }

        t->rdata->return_context = c;

#ifdef NEED_RLIMIT
        qthread_debug(SHEPHERD_DETAILS,
                      "t(%p): setting stack size limits... hopefully we don't currently exceed them!\n",
                      t);
        if (t->flags & QTHREAD_REAL_MCCOY) {
            rlp.rlim_cur = qlib->master_stack_size;
        } else {
            rlp.rlim_cur = qlib->qthread_stack_size;
        }
        rlp.rlim_max = qlib->max_stack_size;
        qassert(setrlimit(RLIMIT_STACK, &rlp), 0);
#endif  /* ifdef NEED_RLIMIT */

        qthread_debug(SHEPHERD_DETAILS,
                      "t(%p): executing swapcontext(%p, %p)...\n", t, t->rdata->return_context, &t->rdata->context);
        /* return_context (aka "c") is being written over with the current context */
#ifdef QTHREAD_USE_VALGRIND
        VALGRIND_CHECK_MEM_IS_ADDRESSABLE(&t->rdata->context, sizeof(qt_context_t));
        VALGRIND_CHECK_MEM_IS_ADDRESSABLE(t->rdata->return_context, sizeof(qt_context_t));
        VALGRIND_MAKE_MEM_DEFINED(&t->rdata->context, sizeof(qt_context_t));
        VALGRIND_MAKE_MEM_DEFINED(t->rdata->return_context, sizeof(qt_context_t));
#endif
#ifdef HAVE_NATIVE_MAKECONTEXT
        qassert(swapcontext(t->rdata->return_context, &t->rdata->context), 0);
#else
        qassert(qt_swapctxt(t->rdata->return_context, &t->rdata->context), 0);
#endif
#ifdef NEED_RLIMIT
        qthread_debug(SHEPHERD_DETAILS,
                      "t(%p): setting stack size limits back to normal...\n",
                      t);
        if (!(t->flags & QTHREAD_REAL_MCCOY)) {
            rlp.rlim_cur = qlib->master_stack_size;
            qassert(setrlimit(RLIMIT_STACK, &rlp), 0);
        }
#endif
    } else {
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

    qthread_debug(SHEPHERD_DETAILS, "t(%p): finished, t->thread_state = %i\n", t, t->thread_state);
}                      /*}}} */

/* this function yields thread t to the master kernel thread */
void API_FUNC qthread_yield_(int k)
{                      /*{{{ */
    assert(qthread_library_initialized);
    qthread_t *t = qthread_internal_self();

    if (t != NULL) {
        qthread_debug(THREAD_CALLS,
                      "tid %u yielding...\n", t->thread_id);
        if (k) {
            t->thread_state = QTHREAD_STATE_YIELDED_NEAR;
        } else {
            t->thread_state = QTHREAD_STATE_YIELDED;
        }
        qthread_back_to_master(t);
        qthread_debug(THREAD_BEHAVIOR, "tid %u resumed.\n",
                      t->thread_id);
    }
}                      /*}}} */

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

#if defined(QTHREAD_DEBUG) || !defined(QTHREAD_MULTITHREADED_SHEPHERDS)
    const qthread_shepherd_id_t max_sheps = qlib->nshepherds;
#endif

    if (me) {
        assert(me->rdata);
        myshep = me->rdata->shepherd_ptr;
    } else {
        myshep = NULL;
    }

    /* Step 1: Check arguments */
    qthread_debug(THREAD_CALLS,
                  "f(%p), arg(%p), arg_size(%z), rt(%s), ret(%p), pt(%s), np(%z), pc(%p), ts(%u), %s, %s, %s\n",
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
                  ((feature_flag & QTHREAD_SPAWN_FUTURE) ? "future" : "qthread"),
                  ((feature_flag & QTHREAD_SPAWN_SIMPLE) ? "simple" : "full"));
    assert(qlib);
    /* Step 2: Pick a destination */
    if (target_shep != NO_SHEPHERD) {
        dest_shep = target_shep % qlib->nshepherds;
    } else {
#ifdef QTHREAD_MULTITHREADED_SHEPHERDS
        if (QTHREAD_LIKELY(myshep)) {
            dest_shep = myshep->shepherd_id; // rely on work-stealing
        } else {
            dest_shep = 0;
        }
#else
        if (QTHREAD_LIKELY(myshep)) {
            dest_shep               = myshep->sched_shepherd++;
            myshep->sched_shepherd *= (max_sheps >= (dest_shep + 1));
        } else {
            dest_shep = (qthread_shepherd_id_t)qthread_internal_incr_mod(&qlib->sched_shepherd,
                                                                         max_sheps,
                                                                         &qlib->sched_shepherd_lock);
        }
#endif  /* ifdef QTHREAD_MULTITHREADED_SHEPHERDS */
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
        new_team = ALLOC_TEAM();
        assert(new_team);

        // Initialize new team values
        new_team->team_id         = qthread_internal_incr(&(qlib->max_team_id), &qlib->max_team_id_lock, 1);
        new_team->eureka          = QTHREAD_NON_TASK_ID;
        new_team->watcher_started = 0;
        new_team->sinc            = qt_sinc_create(0, NULL, NULL, 1);
        assert(new_team->sinc);
        new_team->subteams_sinc = qt_sinc_create(0, NULL, NULL, 1);
        assert(new_team->subteams_sinc);
        new_team->parent_id            = QTHREAD_NON_TEAM_ID;
        new_team->parent_eureka        = NULL;
        new_team->parent_subteams_sinc = NULL;
        new_team->flags                = 0;

        // Empty new team FEBs
        qthread_debug(FEB_DETAILS, "tid %i emptying NEW team %u's eureka (%p)\n", me ? ((int)me->thread_id) : -1, new_team->team_id, &new_team->eureka);
        qthread_empty(&new_team->eureka);

#ifdef TEAM_PROFILE
        qthread_incr(&qlib->team_sinc_create, 1);
        qthread_incr(&qlib->team_subteams_sinc_create, 1);
        qthread_incr(&qlib->team_create, 1);
#endif
    } else if (feature_flag & QTHREAD_SPAWN_NEW_SUBTEAM) {
        // Task will participate in a new subteam
        qthread_internal_incr(&(qlib->team_count), &qlib->team_count_lock, 1);
        team_leader = 1;

        // Allocate new team structure
        new_team = ALLOC_TEAM();
        assert(new_team);

        // Initialize new team values
        new_team->team_id         = qthread_internal_incr(&(qlib->max_team_id), &qlib->max_team_id_lock, 1);
        new_team->eureka          = QTHREAD_NON_TASK_ID;
        new_team->watcher_started = 0;
        new_team->sinc            = qt_sinc_create(0, NULL, NULL, 1);
        assert(new_team->sinc);
        new_team->subteams_sinc = qt_sinc_create(0, NULL, NULL, 1);
        assert(new_team->subteams_sinc);
        new_team->parent_id            = QTHREAD_DEFAULT_TEAM_ID;
        new_team->parent_eureka        = NULL;
        new_team->parent_subteams_sinc = NULL;
        new_team->flags                = 0;

        // Empty new team FEBs
        qthread_debug(FEB_DETAILS, "tid %i emptying SUB team %u's eureka (%p)\n", me ? ((int)me->thread_id) : -1, new_team->team_id, &new_team->eureka);
        qthread_empty(&new_team->eureka);

        if (curr_team) {
            new_team->parent_id     = curr_team->team_id;
            new_team->parent_eureka = &curr_team->eureka;
            assert(new_team->parent_eureka);
            new_team->parent_subteams_sinc = curr_team->subteams_sinc;
            assert(new_team->parent_subteams_sinc);

            // Notify the parent of the new subteam
            qt_sinc_expect(new_team->parent_subteams_sinc, 1);
        }

#ifdef TEAM_PROFILE
        qthread_incr(&qlib->team_sinc_create, 1);
        qthread_incr(&qlib->team_subteams_sinc_create, 1);
        qthread_incr(&qlib->team_subteam_create, 1);
#endif
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
        t->preconds     = preconds;
        qthread_debug(THREAD_BEHAVIOR, "npreconds=%u, preconds[0]=%u\n", (unsigned int)npreconds, (unsigned int)(uintptr_t)((aligned_t **)preconds)[0]);
        assert(((aligned_t **)preconds)[0] == (aligned_t *)(uintptr_t)npreconds);
    }
#ifdef QTHREAD_USE_ROSE_EXTENSIONS
    t->id = target_shep;  /* used in barrier and arrive_first, NOT the
                           * thread-id may be extraneous in both when parallel
                           * region barriers in place (not will to pull it now
                           * maybe later) akp */
#endif
    if (QTHREAD_UNLIKELY(feature_flag & QTHREAD_SPAWN_FUTURE)) {
        t->flags |= QTHREAD_FUTURE;
    }
    if (feature_flag & QTHREAD_SPAWN_SIMPLE) {
        t->flags |= QTHREAD_SIMPLE;
    }
    qthread_debug(THREAD_BEHAVIOR, "new-tid %u shep %u\n", t->thread_id, dest_shep);
#ifdef QTHREAD_USE_ROSE_EXTENSIONS
    if (me != NULL) {
        t->currentParallelRegion = me->currentParallelRegion; // saved in shepherd
        if ((ret == NULL) && (feature_flag & QTHREAD_SPAWN_PARENT)) {
            aligned_t next, previous, test;
            t->parent = me;
            previous  = me->task_counter;
            while(1) {
                assert(tcount_get_waiting(previous) == 0);
                next = tcount_create(0, tcount_get_children(previous) + 1);
                // atomic increment cannot be used because
                // the get_waiting() mask must be preserved
                test = qthread_cas(&me->task_counter, previous, next);
                if (test == previous) { break; }
                previous = test;
            }
        }
    }
#endif /* ifdef QTHREAD_USE_ROSE_EXTENSIONS */
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
            qthread_steal_enable();
            qt_threadqueue_enqueue(qlib->threadqueues[dest_shep], t);
        }
    }
    return QTHREAD_SUCCESS;
} /*}}}*/

int API_FUNC qthread_fork(qthread_f   f,
                          const void *arg,
                          aligned_t  *ret)
{   /*{{{*/
    qthread_debug(THREAD_CALLS, "f(%p), arg(%p), ret(%p)\n", f, arg, ret);
    return qthread_spawn(f, arg, 0, ret, 0, NULL, NO_SHEPHERD, 0);
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

int API_FUNC qthread_fork_track_syncvar_copyargs_to(qthread_f             f,
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
                         QTHREAD_SPAWN_PARENT |
                         QTHREAD_SPAWN_RET_SYNCVAR_T);
} /*}}}*/

int API_FUNC qthread_fork_track_syncvar_copyargs(qthread_f   f,
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
                         QTHREAD_SPAWN_PARENT |
                         QTHREAD_SPAWN_RET_SYNCVAR_T);
}                      /*}}} */

int API_FUNC qthread_fork_syncvar_copyargs_to(qthread_f             f,
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
                         QTHREAD_SPAWN_RET_SYNCVAR_T
                         );
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

int INTERNAL qthread_fork_future_to(qthread_f             f,
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
                         QTHREAD_SPAWN_FUTURE);
} /*}}}*/

int INTERNAL qthread_fork_syncvar_future_to(qthread_f             f,
                                            const void           *arg,
                                            syncvar_t            *ret,
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
                         QTHREAD_SPAWN_RET_SYNCVAR_T |
                         QTHREAD_SPAWN_FUTURE);
} /*}}}*/

int INTERNAL qthread_fork_syncvar_future(qthread_f   f,
                                         const void *arg,
                                         syncvar_t  *ret)
{   /*{{{*/
    return qthread_spawn(f,
                         arg,
                         0,
                         ret,
                         0,
                         NULL,
                         NO_SHEPHERD,
                         QTHREAD_SPAWN_RET_SYNCVAR_T |
                         QTHREAD_SPAWN_FUTURE);
} /*}}}*/

void INTERNAL qthread_back_to_master(qthread_t *t)
{                      /*{{{ */
    assert((t->flags & QTHREAD_SIMPLE) == 0);
#ifdef NEED_RLIMIT
    struct rlimit rlp;

    qthread_debug(SHEPHERD_DETAILS, "t(%p): setting stack size limits for master thread...\n", t);
    if (!(t->flags & QTHREAD_REAL_MCCOY)) {
        rlp.rlim_cur = qlib->master_stack_size;
        rlp.rlim_max = qlib->max_stack_size;
        qassert(setrlimit(RLIMIT_STACK, &rlp), 0);
    }
#endif /* ifdef NEED_RLIMIT */
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
#ifdef NEED_RLIMIT
    qthread_debug(SHEPHERD_DETAILS, "t(%p): setting stack size limits back to qthread size...\n", t);
    if (!(t->flags & QTHREAD_REAL_MCCOY)) {
        rlp.rlim_cur = qlib->qthread_stack_size;
        qassert(setrlimit(RLIMIT_STACK, &rlp), 0);
    }
#endif
}                      /*}}} */

void INTERNAL qthread_back_to_master2(qthread_t *t)
{                      /*{{{ */
    assert((t->flags & QTHREAD_SIMPLE) == 0);
#ifdef NEED_RLIMIT
    struct rlimit rlp;

    qthread_debug(SHEPHERD_DETAILS, "t(%p): setting stack size limits for master thread...\n", t);
    if (!(t->flags & QTHREAD_REAL_MCCOY)) {
        rlp.rlim_cur = qlib->master_stack_size;
        rlp.rlim_max = qlib->max_stack_size;
        qassert(setrlimit(RLIMIT_STACK, &rlp), 0);
    }
#endif /* ifdef NEED_RLIMIT */
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

#ifdef QTHREAD_USE_ROSE_EXTENSIONS
/* These are just accessor functions */
# ifdef QTHREAD_LOG_BARRIER
qt_barrier_t *qt_thread_barrier()            // get barrier active for this thread
# else
qt_feb_barrier_t * qt_thread_barrier()            // get barrier active for this thread
# endif
{                      /*{{{ */
    return qt_parallel_region()->barrier;
}                      /*}}} */
# ifdef QTHREAD_USE_ROSE_EXTENSIONS
/* These are just accessor functions */
#  ifdef QTHREAD_LOG_BARRIER
qt_barrier_t *qt_thread_barrier_resize(size_t size)  // resize barrier for current parallel region
#  else
qt_feb_barrier_t * qt_thread_barrier_resize(size_t size) // resize barrier for current parallel region
#  endif
{                      /*{{{ */
#  ifdef QTHREAD_LOG_BARRIER
    qt_barrier_destroy(qt_parallel_region()->barrier);
    qt_parallel_region()->barrier = qt_barrier_create(size, REGION_BARRIER, 0);
#  else
    qt_feb_barrier_destroy(qt_parallel_region()->barrier);
    qt_parallel_region()->barrier = qt_feb_barrier_create(size);
#  endif /* ifdef QTHREAD_LOG_BARRIER */
    return qt_parallel_region()->barrier;
}                      /*}}} */
# endif /* ifdef QTHREAD_USE_ROSE_EXTENSIONS */

void INTERNAL qt_set_unstealable(void);
void INTERNAL qt_set_unstealable()
{                      /*{{{ */
    qthread_t *t = qthread_internal_self();

    t->flags |= QTHREAD_UNSTEALABLE;
}                      /*}}} */

/* These are just accessor functions */
qthread_parallel_region_t *qt_parallel_region() // get active parallel region
{                                               /*{{{ */
    qthread_t *t = qthread_internal_self();

    return t->currentParallelRegion;
}                      /*}}} */

struct qqloop_step_handle_t *qt_loop_rose_queue_create(int64_t start,
                                                       int64_t stop,
                                                       int64_t incr);
// number of loops to allow concurrently active - unfinished because someone
// is late starting is the normal reason more than 1 or 2
// lowered when code to actually protect against reuse in place AKP 1/26/12
# define QTHREAD_NUM_LOOP_STRUCT 16

int qt_omp_parallel_region_create()
{                      /*{{{ */
    int                        ret = 0;
    int                        workers;
    qthread_parallel_region_t *pr = MALLOC(sizeof(qthread_parallel_region_t));

    qassert_ret(pr, QTHREAD_MALLOC_ERROR);

    workers = qthread_num_workers();

# ifdef QTHREAD_LOG_BARRIER
    qt_barrier_t *gb = qt_barrier_create(workers, REGION_BARRIER, 0); // allocate barrier for region (shepherds)
# else
    qt_feb_barrier_t *gb = qt_feb_barrier_create(workers); // allocate barrier for region (sheperds)
# endif /* ifdef QTHREAD_LOG_BARRIER */

    qthread_t *t = qthread_internal_self();
    if (t->currentParallelRegion != NULL) { // we have nested parallelism
        ret = 1;
    }
    pr->last                 = t->currentParallelRegion;
    t->currentParallelRegion = pr;
    pr->barrier              = gb;
    pr->forLoop              = NULL;
    pr->loopList             = NULL;

    // create and initialize a fixed number of loop structures that
    // will be used as a circular buffer to handle omp parallel for
    // loops -- the number of different active for loops is specified
    // by QTHREAD_NUM_LOOP_STRUCT which should be turned into an
    // environment variable -- also we should check for the limit
    // being hit and fail with a message rather than just giving
    // wrong answers which may now be the case AKP 7/26/11

    // Protection against reuse working AKP 1/26/12
    pr->clsSize           = QTHREAD_NUM_LOOP_STRUCT;
    pr->currentLoopNum    = MALLOC(sizeof(int) * workers);
    pr->currentLoopStruct = MALLOC(sizeof(struct qqloop_step_handle_t *) * pr->clsSize);
    int i;
    for(i = 0; i < qthread_num_workers(); i++) {
        pr->currentLoopNum[i] = 0;
    }
    for(i = 0; i < QTHREAD_NUM_LOOP_STRUCT; i++) {
        pr->currentLoopStruct[i] = qt_loop_rose_queue_create(0, 0, 0);
    }
    return ret;
}                              /*}}} */

void INTERNAL qt_free_loop(void *lp);

void qt_omp_parallel_region_destroy()
{      /*{{{ */
# if 0 // race condition on cleanup - commented out until found - akp 3/16/11
       // it looks like one thread reaches cleanup code before completing loop
       // I thought it was related to threads moving (affinity reduces the
       // likelyhood of the hang -- but does not make it go away) but have not
       // found it.
    qthread_shepherd_t *myshep = qthread_internal_getshep();
    if (!pr) { return; }
    qt_free_loop(pr->loopList);

    if (pr->barrier) {
#  ifdef QTHREAD_LOG_BARRIER
        qt_barrier_destroy(pr->barrier);
#  else
        qt_feb_barrier_destroy(pr->barrier);
#  endif
    }
    myshep->currentParallelRegion = pr->last;

    FREE(pr);
# endif /* if 0 */

    qthread_t *t = qthread_internal_self();

    qthread_parallel_region_t *pr = t->currentParallelRegion;
    t->currentParallelRegion = pr->last;
}                                      /*}}} */

#endif /* ifdef QTHREAD_USE_ROSE_EXTENSIONS */

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

#ifdef QTHREAD_USE_ROSE_EXTENSIONS
unsigned API_FUNC qthread_barrier_id(void)
{                      /*{{{ */
    assert(qthread_library_initialized);
    qthread_t *t = qthread_internal_self();

    qthread_debug(THREAD_CALLS, "tid(%u)\n",
                  t ? t->id : QTHREAD_NON_TASK_ID);
    if (t && (t->id == NO_SHEPHERD)) {
        return qthread_internal_getshep()->shepherd_id;
    }
    return t ? t->id : QTHREAD_NON_TASK_ID;
}                      /*}}} */

#endif /* ifdef QTHREAD_USE_ROSE_EXTENSIONS */

/* these two functions are helper functions for futurelib
 * (nobody else gets to have 'em!) */
unsigned int INTERNAL qthread_isfuture(void)
{                      /*{{{ */
    qthread_t *t = qthread_internal_self();

    return t ? (t->flags & QTHREAD_FUTURE) : 0;
}                      /*}}} */

void INTERNAL qthread_assertfuture(qthread_t *t)
{                      /*{{{ */
    t->flags |= QTHREAD_FUTURE;
}                      /*}}} */

void INTERNAL qthread_assertnotfuture(void)
{                      /*{{{ */
    qthread_t *t = qthread_internal_self();

    t->flags &= ~QTHREAD_FUTURE;
}                      /*}}} */

#ifdef QTHREAD_USE_ROSE_EXTENSIONS
# ifdef __INTEL_COMPILER
#  pragma warning (disable:1418)
# endif

int INTERNAL qthread_forCount(int inc)
{                                    /*{{{ */
    qthread_t *t = qthread_internal_self();

    assert(t);
    return (t->rdata->forCount += inc);
}                                    /*}}} */

void qthread_parent_yield_state()
{
    qthread_t *t = qthread_internal_self();

    assert(t);
    t->prev_thread_state = t->thread_state;
    t->thread_state      = QTHREAD_STATE_PARENT_YIELD;
    qthread_back_to_master(t);
}

aligned_t *qthread_task_counter(void)
{                      /*{{{ */
    qthread_t *t = qthread_internal_self();

    assert(t);
    return &t->task_counter;
}                      /*}}} */

#endif /* ifdef QTHREAD_USE_ROSE_EXTENSIONS */

/* vim:set expandtab: */
