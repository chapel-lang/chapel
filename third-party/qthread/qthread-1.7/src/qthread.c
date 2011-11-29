#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

/* The API */
#include "qthread/qthread.h"

/* System Headers */
#include <stdio.h>
#include <stdlib.h>            /* for malloc() and abort() */
#include <stdarg.h>            /* for va_list, va_start() and va_end() */
#ifdef HAVE_MALLOC_H
# include <malloc.h>               /* for memalign() */
#endif
#include <limits.h>              /* for INT_MAX */
#include <qthread/qthread-int.h> /* for UINT8_MAX */
#include <string.h>              /* for memset() */
#ifdef QTHREAD_GUARD_PAGES
#include <unistd.h>            /* for getpagesize() */
#endif
#if !HAVE_MEMCPY
# define memcpy(d, s, n)  bcopy((s), (d), (n))
# define memmove(d, s, n) bcopy((s), (d), (n))
#endif
#include <sys/time.h>
#include <sys/resource.h>
#if (defined(QTHREAD_SHEPHERD_PROFILING) || defined(QTHREAD_LOCK_PROFILING))
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

/* Internal Headers */
#include "qt_mpool.h"
#include "qt_atomics.h"
#include "qthread_expect.h"

#include "qthread/futurelib.h"
#include "qthread_innards.h"
#include "qthread_prefetch.h"
#include "futurelib_innards.h"
#ifdef QTHREAD_USE_ROSE_EXTENSIONS
# include "qt_barrier.h"
# include "qthread/feb_barrier.h"   /* for barrier in parallel region defination */
# include "qt_arrive_first.h"
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
#include "qt_locks.h"
#include "qt_io.h"
#include "qt_debug.h"
#ifdef QTHREAD_MULTINODE
# include "net/net.h"
#endif

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
pthread_key_t shepherd_structs;
qlib_t        qlib      = NULL;
int           qaffinity = 1;

struct qt_cleanup_funcs_s {
    void                       (*func)(void);
    struct qt_cleanup_funcs_s *next;
} *qt_cleanup_funcs = NULL, *qt_cleanup_early_funcs = NULL;

#ifdef QTHREAD_COUNT_THREADS
static aligned_t             threadcount;
static aligned_t             maxconcurrentthreads;
static double                avg_concurrent_threads;
static aligned_t             avg_count;
static aligned_t             concurrentthreads;
static QTHREAD_FASTLOCK_TYPE concurrentthreads_lock;

# define QTHREAD_COUNT_THREADS_BINCOUNTER(TYPE, BIN) qthread_internal_incr(&qlib->TYPE ## _stripes[(BIN)], &qlib->TYPE ## _stripes_locks[(BIN)], 1)
#else
# define QTHREAD_COUNT_THREADS_BINCOUNTER(TYPE, BIN) do { } while(0)
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
static QINLINE qthread_t *qthread_thread_new(qthread_f             f,
                                             const void           *arg,
                                             size_t                arg_size,
                                             void                 *ret,
                                             qthread_shepherd_id_t shepherd);
static QINLINE void        qthread_thread_free(qthread_t *t);
static qthread_shepherd_t *qthread_find_active_shepherd(qthread_shepherd_id_t *l,
                                                        unsigned int          *d);
static QINLINE void qthread_enqueue(qthread_queue_t *q,
                                    qthread_t       *t);
#ifdef QTHREAD_USE_ROSE_EXTENSIONS
void       walkSyncTaskList(void);
syncvar_t *getSyncTaskVar(void);
void qthread_task_free(qthread_t *);
#endif

#if defined(UNPOOLED_QTHREAD_T) || defined(UNPOOLED)
# define ALLOC_QTHREAD(shep) (qthread_t *)malloc(sizeof(qthread_t) + qlib->qthread_argcopy_size + qlib->qthread_tasklocal_size)
# define FREE_QTHREAD(t)     free(t)
#else
static qt_mpool generic_qthread_pool = NULL;
static QINLINE qthread_t *ALLOC_QTHREAD(qthread_shepherd_t *shep)
{                      /*{{{ */
    qthread_t *tmp = (qthread_t *)qt_mpool_alloc(shep
                                                 ? (shep->qthread_pool)
                                                 : generic_qthread_pool);

    if (tmp != NULL) {
        tmp->creator_ptr = shep;
    }
    return tmp;
}                      /*}}} */

static QINLINE void FREE_QTHREAD(qthread_t *t)
{                      /*{{{ */
    qt_mpool_free(t->creator_ptr ? (t->creator_ptr->qthread_pool) :
                  generic_qthread_pool, t);
}                      /*}}} */

#endif /* if defined(UNPOOLED_QTHREAD_T) || defined(UNPOOLED) */

#if defined(UNPOOLED_STACKS) || defined(UNPOOLED)
# ifdef QTHREAD_GUARD_PAGES
static QINLINE void *ALLOC_STACK(qthread_shepherd_t *shep)
{                      /*{{{ */
    uint8_t *tmp = valloc(qlib->qthread_stack_size + sizeof(struct qthread_runtime_data_s) + (2 * getpagesize()));

    assert(tmp != NULL);
    if (tmp == NULL) {
        return NULL;
    }
    if (mprotect(tmp, getpagesize(), PROT_NONE) != 0) {
        perror("mprotect in ALLOC_STACK (1)");
    }
    if (mprotect(tmp + qlib->qthread_stack_size + getpagesize(), getpagesize(), PROT_NONE) != 0) {
        perror("mprotect in ALLOC_STACK (2)");
    }
    return tmp + getpagesize();
}                      /*}}} */

static QINLINE void FREE_STACK(qthread_shepherd_t *shep,
                               void               *t)
{                      /*{{{ */
    uint8_t *tmp = t;

    assert(t);
    tmp -= getpagesize();
    if (mprotect(tmp, getpagesize(), PROT_READ | PROT_WRITE) != 0) {
        perror("mprotect in FREE_STACK (1)");
    }
    if (mprotect
            (tmp + qlib->qthread_stack_size + getpagesize(), getpagesize(),
            PROT_READ | PROT_WRITE) != 0) {
        perror("mprotect in FREE_STACK (2)");
    }
    free(tmp);
}                      /*}}} */

# else /* ifdef QTHREAD_GUARD_PAGES */
#  define ALLOC_STACK(shep)   malloc(qlib->qthread_stack_size + sizeof(struct qthread_runtime_data_s))
#  define FREE_STACK(shep, t) free(t)
# endif /* ifdef QTHREAD_GUARD_PAGES */
#else /* if defined(UNPOOLED_STACKS) || defined(UNPOOLED) */
static qt_mpool generic_stack_pool = NULL;
# ifdef QTHREAD_GUARD_PAGES
static QINLINE void *ALLOC_STACK(qthread_shepherd_t *shep)
{                      /*{{{ */
    uint8_t *tmp = qt_mpool_alloc(shep ? (shep->stack_pool) : generic_stack_pool);

    assert(tmp);
    if (tmp == NULL) {
        return NULL;
    }
    if (mprotect(tmp, getpagesize(), PROT_NONE) != 0) {
        perror("mprotect in ALLOC_STACK (1)");
    }
    if (mprotect
            (tmp + qlib->qthread_stack_size + getpagesize(), getpagesize(),
            PROT_NONE) != 0) {
        perror("mprotect in ALLOC_STACK (2)");
    }
    return tmp + getpagesize();
}                      /*}}} */

static QINLINE void FREE_STACK(qthread_shepherd_t *shep,
                               void               *t)
{                      /*{{{ */
    uint8_t *tmp = t;

    assert(t);
    tmp -= getpagesize();
    if (mprotect(tmp, getpagesize(), PROT_READ | PROT_WRITE) != 0) {
        perror("mprotect in FREE_STACK (1)");
    }
    if (mprotect
            (tmp + qlib->qthread_stack_size + getpagesize(), getpagesize(),
            PROT_READ | PROT_WRITE) != 0) {
        perror("mprotect in FREE_STACK (2)");
    }
    qt_mpool_free(shep ? (shep->stack_pool) : generic_stack_pool, tmp);
}                      /*}}} */

# else /* ifdef QTHREAD_GUARD_PAGES */
#  define ALLOC_STACK(shep)   qt_mpool_alloc(shep ? (shep->stack_pool) : generic_stack_pool)
#  define FREE_STACK(shep, t) qt_mpool_free(shep ? (shep->stack_pool) : generic_stack_pool, t)
# endif /* ifdef QTHREAD_GUARD_PAGES */
#endif  /* if defined(UNPOOLED_STACKS) || defined(UNPOOLED) */

#if !defined(UNPOOLED_ADDRSTAT) && !defined(UNPOOLED)
qt_mpool generic_addrstat_pool = NULL;
#endif

/* guaranteed to be between 0 and 128, using the first parts of addr that are
 * significant */
unsigned int QTHREAD_LOCKING_STRIPES = 128;

#ifdef QTHREAD_DEBUG
enum qthread_debug_levels debuglevel = 0;
QTHREAD_FASTLOCK_TYPE     output_lock;

int qthread_debuglevel(int d)
{
    if (d >= 0) { debuglevel = d; }
    return debuglevel;
}

#else
int qthread_debuglevel(int Q_UNUSED d)
{
    return 0;
}

#endif /* ifdef QTHREAD_DEBUG */

#include "qt_profiling.h"

#ifndef QTHREAD_NO_ASSERTS
void *shep0arg = NULL;
#endif

static QINLINE void alloc_rdata(qthread_shepherd_t *me,
                                qthread_t          *t)
{
    void *stack = ALLOC_STACK(me);

    assert(stack);
#ifdef QTHREAD_GUARD_PAGES
    t->rdata = (struct qthread_runtime_data_s *)(((uint8_t *)stack) + getpagesize() + qlib->qthread_stack_size);
#else
    t->rdata = (struct qthread_runtime_data_s *)(((uint8_t *)stack) + qlib->qthread_stack_size);
#endif
    t->rdata->stack        = stack;
    t->rdata->shepherd_ptr = me;
    t->rdata->blockedon    = NULL;
#ifdef QTHREAD_USE_VALGRIND
    t->rdata->valgrind_stack_id = VALGRIND_STACK_REGISTER(stack, qlib->qthread_stack_size);
#endif
#ifdef QTHREAD_USE_ROSE_EXTENSIONS
# ifdef QTHREAD_OMP_AFFINITY
    t->rdata->child_affinity = OMP_NO_CHILD_TASK_AFFINITY;
# endif
    t->rdata->openmpTaskRetVar = NULL;
    t->rdata->taskWaitLock     = SYNCVAR_EMPTY_INITIALIZER;
#endif
}

#ifdef QTHREAD_RCRTOOL
static int          rcr_gate  = 0;
static volatile int rcr_ready = 0;
#endif

/* the qthread_shepherd() is the pthread responsible for actually
 * executing the work units
 *
 * this function is the workhorse of the library: this is the function that
 * gets spawned several times and runs all the qthreads. */
#ifdef QTHREAD_MAKECONTEXT_SPLIT
static void *qthread_shepherd(void *arg);
static void *qthread_shepherd_wrapper(unsigned int high,
                                      unsigned int low)
{                      /*{{{ */
    qthread_shepherd_t *me =
        (qthread_shepherd_t *)((((uintptr_t)high) << 32) | low);

    qthread_debug(SHEPHERD_DETAILS, "high(%x), low(%x): me = %p\n",
                  high, low, me);
    return qthread_shepherd(me);
}

#endif /* ifdef QTHREAD_MAKECONTEXT_SPLIT */

static void *qthread_shepherd(void *arg)
{
#ifdef QTHREAD_MULTITHREADED_SHEPHERDS
    qthread_worker_t   *me_worker = (qthread_worker_t *)arg;
    qthread_shepherd_t *me        = (qthread_shepherd_t *)me_worker->shepherd;
#else
    qthread_shepherd_t *me = (qthread_shepherd_t *)arg;
#endif
    qt_context_t my_context;
    qthread_t   *t;
    int          done = 0;

#ifdef QTHREAD_SHEPHERD_PROFILING
    me->total_time = qtimer_create();
    qtimer_t idle = qtimer_create();
#endif

    qthread_debug(SHEPHERD_DETAILS, "alive! me = %p\n", me);
    assert(me != NULL);
    assert(me->shepherd_id <= qlib->nshepherds);
    qthread_debug(SHEPHERD_FUNCTIONS, "id(%u): forked with arg %p\n", me->shepherd_id, arg);
#ifndef QTHREAD_NO_ASSERTS
    if ((shep0arg != NULL) && (me->shepherd_id == 0)) {
        if (arg != shep0arg) {
            fprintf(stderr, "arg = %p, shep0arg = %p\n", arg, shep0arg);
        }
        assert(arg == shep0arg);
        shep0arg = NULL;
    }
#endif

    /* Initialize myself */
    pthread_setspecific(shepherd_structs, arg);

#ifdef QTHREAD_USE_ROSE_EXTENSIONS
# ifdef QTHREAD_MULTITHREADED_SHEPHERDS
    if (!me_worker) {  // }  // if worker 0 init taskWaitLock
# else
    if (!me) {                 // if sheperhd 0 init taskWaitLock
# endif
        qthread_syncvar_empty(&qlib->mccoy_thread->rdata->taskWaitLock);
    }
#endif

    if (qaffinity && (me->node != UINT_MAX)) {
#ifdef QTHREAD_MULTITHREADED_SHEPHERDS
        qt_affinity_set(me_worker);
#else
        qt_affinity_set(me);
#endif
    }

#ifdef QTHREAD_RCRTOOL
    if (rcrtoollevel) { // has cache control been turned off by an environment variable?
        // if so need to initialize -- inside of call (call also forces is to happen only once)
        // care needs to be taken so no one makes it into workhorse loop before allocation
        // has actually happened
        int gate = qthread_incr(&rcr_gate, 1);
        if(gate == 0) {
            maestro_allowed_workers();
            rcr_ready = 1;
        } else {
            while(!rcr_ready) {} // spin until ready
        }
    }
#endif /* ifdef QTHREAD_RCRTOOL */
       /* workhorse loop */
    while (!done) {
#ifdef QTHREAD_SHEPHERD_PROFILING
        qtimer_start(idle);
#endif
        qthread_debug(SHEPHERD_DETAILS, "id(%i): fetching a thread from my queue...\n", me->shepherd_id);

#ifdef QTHREAD_RCRTOOL
        if (rcrtoollevel) {                         // has cache control been turned off by an environment variable?
            if (me_worker->packed_worker_id != 0) { // never idle shepherd 0 worker 0  -- needs to be active for termination
                if (qlib->shepherds[me->shepherd_id].active_workers > maestro_current_workers(me->shepherd_id)) {
                    qthread_incr(&qlib->shepherds[me->shepherd_id].active_workers, -1);                                        // not working spinning
                    while ((qlib->shepherds[me->shepherd_id].active_workers + 1) > maestro_current_workers(me->shepherd_id)) { // A) the number of workers to be increased
                        if(done) {
                            break; // B) somebodies noticed the job is done
                        }
                    }
                    qthread_incr(&qlib->shepherds[me->shepherd_id].active_workers, 1); // back at work  -- skipped in departed workers case OK since everyone leaving
                }
            }
        }
#endif  /* ifdef QTHREAD_RCRTOOL */

        assert(me->ready);
        t = qt_threadqueue_dequeue_blocking(me->ready QMS_ARG(me_worker->active));
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
                      me->shepherd_id, t, t->thread_id, t->thread_state);

        // Process input preconds if this is a nascent thread
        if (t->thread_state == QTHREAD_STATE_NASCENT) {
            if (qthread_check_precond(t) == 1) { continue; }
        }

        if (t->thread_state == QTHREAD_STATE_TERM_SHEP) {
#ifdef QTHREAD_SHEPHERD_PROFILING
            qtimer_stop(me->total_time);
#endif
            done = 1;
#ifdef QTHREAD_RCRTOOL
            qthread_incr(&qlib->shepherds[me->shepherd_id].active_workers, -1); // not working spinning
#endif
	    qthread_thread_free(t); /* free qthread data structures */
#ifdef QTHREAD_USE_ROSE_EXTENSIONS
            qthread_task_free(t);   /* mark as ready for freeing -- may free qthread itself */
#endif
        } else {
#ifdef QTHREAD_MULTITHREADED_SHEPHERDS
            if(!*(volatile size_t *)&me_worker->active) {
                qt_threadqueue_enqueue(me->ready, t, me);
                // short stall to prevent worker from monoplizing queue
                struct timespec req = { 0 };
                req.tv_sec  = 0;
                req.tv_nsec = 10000000L;
                nanosleep(&req, &req);
            } else {
#endif
            /* yielded only happens for the first thread */
            assert((t->thread_state == QTHREAD_STATE_NEW) ||
                   (t->thread_state == QTHREAD_STATE_RUNNING) ||
                   (t->thread_state == QTHREAD_STATE_YIELDED &&
                    t->flags & QTHREAD_REAL_MCCOY));

            assert(t->f != NULL || t->flags & QTHREAD_REAL_MCCOY);
            if (t->rdata == NULL) {
                alloc_rdata(me, t);
            }
            assert(t->rdata->shepherd_ptr != NULL);
            if (t->rdata->shepherd_ptr != me) {
                // fprintf(stderr, "shepherd_ptr = %p, me = %p\n", t->rdata->shepherd_ptr, me);
                // fflush(stderr);
                t->rdata->shepherd_ptr = me;
            }

            if ((t->target_shepherd != NULL) && (t->target_shepherd != me) &&
                QTHREAD_CASLOCK_READ_UI(t->target_shepherd->active)) {
                /* send this thread home */
                qthread_debug(THREAD_DETAILS,
                              "id(%u): thread %u going back home to shep %u\n",
                              me->shepherd_id, t->thread_id,
                              t->target_shepherd->shepherd_id);
                t->rdata->shepherd_ptr = t->target_shepherd;
                assert(t->rdata->shepherd_ptr->ready != NULL);
                qt_threadqueue_enqueue(t->rdata->shepherd_ptr->ready, t, me);
            } else if (!QTHREAD_CASLOCK_READ_UI(me->active)) {
                qthread_debug(THREAD_DETAILS,
                              "id(%u): skipping thread exec because I've been disabled!\n",
                              me->shepherd_id);
                if ((t->target_shepherd == NULL) || (t->target_shepherd == me)) {
                    /* send to the closest shepherd */
                    t->rdata->shepherd_ptr = qthread_find_active_shepherd(me->sorted_sheplist, me->shep_dists);
                } else {
                    /* find a shepherd somewhere near the preferred shepherd
                     *
                     * Note: if the preferred shep was active, we would have sent
                     * this thread home above */
                    t->rdata->shepherd_ptr = qthread_find_active_shepherd(t->target_shepherd->sorted_sheplist, t->target_shepherd->shep_dists);
                }
                assert(t->rdata->shepherd_ptr);
                if (t->rdata->shepherd_ptr == NULL) {
                    qthread_debug(THREAD_DETAILS,
                                  "id(%u): a new home for thread %i could not be found!\n",
                                  me->shepherd_id, t->thread_id);
                    t->rdata->shepherd_ptr = me;
                }
                qthread_debug(THREAD_DETAILS,
                              "id(%u): rescheduling thread %i on %i\n",
                              me->shepherd_id, t->thread_id,
                              t->rdata->shepherd_ptr->shepherd_id);
                assert(t->rdata->shepherd_ptr->ready != NULL);
                qt_threadqueue_enqueue(t->rdata->shepherd_ptr->ready, t, me);
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
                getcontext(&my_context);
                qthread_debug(THREAD_DETAILS, "id(%u): shepherd context is %p\n", me->shepherd_id, &my_context);
                /* note: there's a good argument that the following should
                 * be: (*t->f)(t), however the state management would be
                 * more complex
                 */
                qthread_exec(t, &my_context);
#ifdef QTHREAD_MULTITHREADED_SHEPHERDS
                me_worker->current = NULL;
#else
                me->current = NULL;
#endif
                qthread_debug(THREAD_DETAILS, "id(%u): back from qthread_exec, state is %i\n", me->shepherd_id, t->thread_state);
                /* now clean up, based on the thread's state */
                switch (t->thread_state) {
                    case QTHREAD_STATE_MIGRATING:
                        qthread_debug(THREAD_DETAILS | AFFINITY_DETAILS,
                                      "id(%u): thread %u migrating to shep %u\n",
                                      me->shepherd_id, t->thread_id,
                                      t->target_shepherd->shepherd_id);
                        t->thread_state        = QTHREAD_STATE_RUNNING;
                        t->rdata->shepherd_ptr = t->target_shepherd;
                        assert(t->rdata->shepherd_ptr->ready != NULL);
                        qt_threadqueue_enqueue(t->rdata->shepherd_ptr->ready, t, me);
                        break;
                    default:
                        qthread_debug(THREAD_DETAILS, "id(%u): thread in state %i; that's illegal!\n", me->shepherd_id, t->thread_state);
                        assert(0);
                        break;
                    case QTHREAD_STATE_YIELDED: /* reschedule it */
                        t->thread_state = QTHREAD_STATE_RUNNING;
                        qthread_debug(THREAD_DETAILS,
                                      "id(%u): thread %i yielded; rescheduling\n",
                                      me->shepherd_id, t->thread_id);
                        assert(me->ready != NULL);
                        qt_threadqueue_enqueue_yielded(me->ready, t, me);
                        break;

                    case QTHREAD_STATE_FEB_BLOCKED: /* unlock the related FEB address locks, and re-arrange memory to be correct */
                        qthread_debug(THREAD_DETAILS | LOCK_DETAILS,
                                      "id(%u): thread %i blocked on FEB\n",
                                      me->shepherd_id, t->thread_id);
                        t->thread_state = QTHREAD_STATE_BLOCKED;
                        QTHREAD_FASTLOCK_UNLOCK(&(((qthread_addrstat_t *)(t->rdata->blockedon))->lock));
                        break;

                    case QTHREAD_STATE_BLOCKED: /* put it in the blocked queue */
                        qthread_debug(THREAD_DETAILS | LOCK_DETAILS,
                                      "id(%u): thread %i blocked on LOCK\n",
                                      me->shepherd_id, t->thread_id);
                        qthread_enqueue((qthread_queue_t *)t->rdata->blockedon->waiting, t);
                        QTHREAD_FASTLOCK_UNLOCK(&(t->rdata->blockedon->lock));
                        break;

                    case QTHREAD_STATE_SYSCALL:
                        t->thread_state = QTHREAD_STATE_RUNNING;
                        qthread_debug(THREAD_DETAILS | IO_DETAILS,
                                      "id(%u): thread %i made a syscall\n",
                                      me->shepherd_id, t->thread_id);
                        qt_blocking_subsystem_enqueue((qt_blocking_queue_node_t *)t->rdata->blockedon);
                        break;

                    case QTHREAD_STATE_TERMINATED:
                        qthread_debug(THREAD_DETAILS,
                                      "id(%u): thread %i terminated\n",
                                      me->shepherd_id, t->thread_id);
                        /* we can remove the stack etc. */ 
			qthread_thread_free(t);
			/*   and the context... */
#ifdef QTHREAD_USE_ROSE_EXTENSIONS
                        qthread_task_free(t);
#endif
                        break;
                }
            }
#ifdef QTHREAD_MULTITHREADED_SHEPHERDS
        }
#endif
        }
    }

#ifdef QTHREAD_SHEPHERD_PROFILING
    qtimer_destroy(idle);
#endif
    qthread_debug(SHEPHERD_DETAILS, "id(%u): finished\n",
                  me->shepherd_id);
    pthread_exit(NULL);
    return NULL;
}                      /*}}} */

static qthread_shepherd_t *qthread_find_active_shepherd(qthread_shepherd_id_t *l,
                                                        unsigned int          *d)
{                      /*{{{ */
    qthread_shepherd_id_t       target = 0;
    qthread_shepherd_t         *sheps  = qlib->shepherds;
    const qthread_shepherd_id_t nsheps =
        (qthread_shepherd_id_t)qlib->nshepherds;

    qthread_debug(SHEPHERD_FUNCTIONS, "l(%p): from %i sheps\n", l, (int)nsheps);
    if (l == NULL) {
        /* if l==NULL, there's no locality info, so just find the least-busy active shepherd */
        saligned_t busyness = 0;
        int        found    = 0;

        for (size_t i = 0; i < nsheps; i++) {
            if (QTHREAD_CASLOCK_READ_UI(sheps[i].active)) {
                ssize_t shep_busy_level = qt_threadqueue_advisory_queuelen(sheps[i].ready);

                if (found == 0) {
                    found = 1;
                    qthread_debug(SHEPHERD_FUNCTIONS,
                                  "l(%p): shep %i is the least busy (%i) so far\n",
                                  l, (int)i, shep_busy_level);
                    busyness = shep_busy_level;
                    target   = i;
                } else if ((shep_busy_level < busyness) ||
                           ((shep_busy_level == busyness) &&
                            (random() % 2 == 0))) {
                    qthread_debug(SHEPHERD_FUNCTIONS,
                                  "l(%p): shep %i is the least busy (%i) so far\n",
                                  l, (int)i, shep_busy_level);
                    busyness = shep_busy_level;
                    target   = i;
                }
            }
        }
        assert(found);
        if (found == 0) {
            qthread_debug(SHEPHERD_FUNCTIONS,
                          "l(%p): DID NOT FIND ANY ACTIVE SHEPHERDS!!!\n", l);
            return NULL;
        } else {
            qthread_debug(SHEPHERD_FUNCTIONS,
                          "l(%p): found bored target %i\n",
                          l, (int)target);
            return &(sheps[target]);
        }
    } else {
        /* if we have locality info, use it to identify the closest shepherd(s)
         * and if there's more than one that is equidistant, pick the least busy
         */
        qthread_shepherd_id_t alt;
        saligned_t            busyness;

        while (target < (nsheps - 1) && QTHREAD_CASLOCK_READ_UI(sheps[l[target]].active) == 0) {
            target++;
        }
        if (target >= (nsheps - 1)) {
            return NULL;
        }
        qthread_debug(SHEPHERD_FUNCTIONS,
                      "l(%p): nearest active shepherd (%i) is %i away\n",
                      l, (int)l[target], (int)d[l[target]]);
        busyness = qt_threadqueue_advisory_queuelen(sheps[l[target]].ready);
        for (alt = target + 1; alt < (nsheps - 1) && d[l[alt]] == d[l[target]];
             alt++) {
            saligned_t shep_busy_level = qt_threadqueue_advisory_queuelen(sheps[l[alt]].ready);
            if ((shep_busy_level < busyness) ||
                ((shep_busy_level == busyness) && (random() % 2 == 0))) {
                qthread_debug(SHEPHERD_FUNCTIONS,
                              "l(%p): shep %i is the least busy (%i) so far\n",
                              l, (int)d[l[alt]], shep_busy_level);
                busyness = shep_busy_level;
                target   = alt;
            }
        }
        qthread_debug(SHEPHERD_FUNCTIONS, "l(%p): found target %i\n", l, (int)target);
        return &(sheps[l[target]]);
    }
}                      /*}}} */

#define GET_ENV_NUM(envariable, num, dflt, zerodflt) do {                \
        char         *str = getenv(envariable);                          \
        unsigned long tmp = dflt;                                        \
        if (str && *str) {                                               \
            char *errptr;                                                \
            tmp = strtoul(str, &errptr, 0);                              \
            if (*errptr != 0) {                                          \
                fprintf(stderr, "unparsable "envariable " (%s)\n", str); \
                tmp = dflt;                                              \
            }                                                            \
            if (tmp == 0) { tmp = zerodflt; }                            \
        }                                                                \
        num = tmp;                                                       \
} while (0)

int qthread_init(qthread_shepherd_id_t nshepherds)
{                      /*{{{ */
    char newenv[100];

    snprintf(newenv, 99, "QTHREAD_NUM_SHEPHERDS=%i", (int)nshepherds);
    putenv(newenv);
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

int qthread_initialize(void)
{                      /*{{{ */
    int                   r;
    size_t                i;
    int                   need_sync       = 1;
    qthread_shepherd_id_t nshepherds      = 0;
    qthread_worker_id_t   nworkerspershep = 0;

#ifdef QTHREAD_DEBUG
    QTHREAD_FASTLOCK_INIT(output_lock);
    GET_ENV_NUM("QTHREAD_DEBUG_LEVEL", debuglevel, 0, 0);
# ifdef SST
    debuglevel = 7;
# endif
#endif /* ifdef QTHREAD_DEBUG */

    qthread_debug(CORE_CALLS, "began.\n");
    if (qlib != NULL) {
        qthread_debug(CORE_DETAILS, "redundant call\n");
        return QTHREAD_SUCCESS;
    }
    qlib = (qlib_t)malloc(sizeof(struct qlib_s));
    qassert_ret(qlib, QTHREAD_MALLOC_ERROR);

#ifdef QTHREAD_MULTINODE
    if (NULL != getenv("QTHREAD_MULTINODE")) {
        qthread_internal_net_initialize();
    }
#endif

    GET_ENV_NUM("QTHREAD_NUM_SHEPHERDS", nshepherds, 0, 0);
# ifdef QTHREAD_MULTITHREADED_SHEPHERDS
    GET_ENV_NUM("QTHREAD_NUM_WORKERS_PER_SHEPHERD", nworkerspershep, 0, 0);
    if (nworkerspershep > 0) {
        if (nshepherds == 0) {
            fprintf(stderr, "Number of shepherds not specified - number of workers may be ignored\n");
        }
    }
# else
    nworkerspershep = 1;
# endif /* ifdef QTHREAD_MULTITHREADED_SHEPHERDS */

#if defined(QTHREAD_MUTEX_INCREMENT) || (QTHREAD_ASSEMBLY_ARCH == QTHREAD_POWERPC32)
    qlib->atomic_locks = malloc(sizeof(QTHREAD_FASTLOCK_TYPE) * QTHREAD_LOCKING_STRIPES);
    qassert_ret(qlib->atomic_locks, QTHREAD_MALLOC_ERROR);
    for (i = 0; i < QTHREAD_LOCKING_STRIPES; i++) {
        QTHREAD_FASTLOCK_INIT(qlib->atomic_locks[i]);
    }
#endif

    qt_affinity_init(&nshepherds, &nworkerspershep);

    if (getenv("QTHREAD_INFO")) {
        fprintf(stderr, "Using %i Shepherds\n", (int)nshepherds);
# ifdef QTHREAD_MULTITHREADED_SHEPHERDS
        fprintf(stderr, "Using %i Workers per Shepherd\n", (int)nworkerspershep);
# endif
    }

    if ((nshepherds == 1) && (nworkerspershep == 1)) {
        need_sync = 0;
    }
    QTHREAD_LOCKING_STRIPES = 1 << ((unsigned int)(log2(nshepherds * nworkerspershep)) + 1);
    qthread_debug(CORE_BEHAVIOR, "there will be %u shepherd(s)\n", (unsigned)nshepherds);

#ifdef QTHREAD_COUNT_THREADS
    threadcount            = 0;
    maxconcurrentthreads   = 0;
    concurrentthreads      = 0;
    avg_concurrent_threads = 0;
    avg_count              = 0;
    QTHREAD_FASTLOCK_INIT(concurrentthreads_lock);
#endif

    /* initialize the FEB-like locking structures */
#ifdef QTHREAD_COUNT_THREADS
    qlib->locks_stripes = malloc(sizeof(aligned_t) * QTHREAD_LOCKING_STRIPES);
    qassert_ret(qlib->locks_stripes, QTHREAD_MALLOC_ERROR);
    qlib->febs_stripes = malloc(sizeof(aligned_t) * QTHREAD_LOCKING_STRIPES);
    qassert_ret(qlib->febs_stripes, QTHREAD_MALLOC_ERROR);
# ifdef QTHREAD_MUTEX_INCREMENT
    qlib->locks_stripes_locks = malloc(sizeof(QTHREAD_FASTLOCK_TYPE) * QTHREAD_LOCKING_STRIPES);
    qassert_ret(qlib->locks_stripes_locks, QTHREAD_MALLOC_ERROR);
    qlib->febs_stripes_locks = malloc(sizeof(QTHREAD_FASTLOCK_TYPE) * QTHREAD_LOCKING_STRIPES);
    qassert_ret(qlib->febs_stripes_locks, QTHREAD_MALLOC_ERROR);
# endif
#endif /* ifdef QTHREAD_COUNT_THREADS */
    qlib->locks = malloc(sizeof(qt_hash) * QTHREAD_LOCKING_STRIPES);
    qassert_ret(qlib->locks, QTHREAD_MALLOC_ERROR);
    qlib->FEBs = malloc(sizeof(qt_hash) * QTHREAD_LOCKING_STRIPES);
    qassert_ret(qlib->FEBs, QTHREAD_MALLOC_ERROR);
    qlib->syncvars = malloc(sizeof(qt_hash) * QTHREAD_LOCKING_STRIPES);
    qassert_ret(qlib->syncvars, QTHREAD_MALLOC_ERROR);
    for (i = 0; i < QTHREAD_LOCKING_STRIPES; i++) {
#ifdef QTHREAD_COUNT_THREADS
        qlib->locks_stripes[i] = 0;
        qlib->febs_stripes[i]  = 0;
# ifdef QTHREAD_MUTEX_INCREMENT
        QTHREAD_FASTLOCK_INIT(qlib->locks_stripes_locks[i]);
        QTHREAD_FASTLOCK_INIT(qlib->febs_stripes_locks[i]);
# endif
#endif
        qlib->locks[i] = qt_hash_create(need_sync);
        qassert_ret(qlib->locks[i], QTHREAD_MALLOC_ERROR);
        qlib->FEBs[i] = qt_hash_create(need_sync);
        qassert_ret(qlib->FEBs[i], QTHREAD_MALLOC_ERROR);
        qlib->syncvars[i] = qt_hash_create(need_sync);
        qassert_ret(qlib->syncvars[i], QTHREAD_MALLOC_ERROR);
    }

    /* initialize the kernel threads and scheduler */
    qassert(pthread_key_create(&shepherd_structs, NULL), 0);
    qlib->nshepherds        = nshepherds;
    qlib->nworkerspershep   = nworkerspershep;
    qlib->nshepherds_active = nshepherds;
    qlib->shepherds         = (qthread_shepherd_t *)calloc(nshepherds, sizeof(qthread_shepherd_t));
    qassert_ret(qlib->shepherds, QTHREAD_MALLOC_ERROR);
#ifdef QTHREAD_MUTEX_INCREMENT
    QTHREAD_FASTLOCK_INIT(qlib->nshepherds_active_lock);
#endif

    GET_ENV_NUM("QTHREAD_STACK_SIZE", qlib->qthread_stack_size, QTHREAD_DEFAULT_STACK_SIZE, QTHREAD_DEFAULT_STACK_SIZE);
    qthread_debug(CORE_DETAILS, "qthread stack size: %u\n", qlib->qthread_stack_size);
    if (getenv("QTHREAD_INFO"))
        fprintf(stderr, "Using %u byte stack size.\n", qlib->qthread_stack_size);

#ifdef QTHREAD_GUARD_PAGES
    {
        size_t pagesize = getpagesize();

        /* round stack size to nearest page */
        if (qlib->qthread_stack_size % pagesize) {
            qlib->qthread_stack_size +=
                pagesize - (qlib->qthread_stack_size % pagesize);
        }
    }
#endif
    qlib->max_thread_id  = 0;
    qlib->sched_shepherd = 0;
    QTHREAD_FASTLOCK_INIT(qlib->max_thread_id_lock);
    QTHREAD_FASTLOCK_INIT(qlib->sched_shepherd_lock);
    {
        struct rlimit rlp;

        qassert(getrlimit(RLIMIT_STACK, &rlp), 0);
        qthread_debug(CORE_DETAILS, "stack sizes ... cur: %u max: %u\n", rlp.rlim_cur, rlp.rlim_max);
        if (rlp.rlim_cur == RLIM_INFINITY) {
            qlib->master_stack_size = 8 * 1024 * 1024;
        } else {
            qlib->master_stack_size = (unsigned int)(rlp.rlim_cur);
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
        char *aff = getenv("QTHREAD_AFFINITY");

        if (aff && !strncmp(aff, "no", 3)) {
            qaffinity = 0;
        } else {
            qaffinity = 1;
        }
    }
    qthread_debug(AFFINITY_DETAILS, "qaffinity = %i\n", qaffinity);
    if ((qaffinity == 1) && ((nshepherds * nworkerspershep) > 1)) {
        int ret = qt_affinity_gendists(qlib->shepherds, nshepherds);
        if (ret != QTHREAD_SUCCESS) {
            qthread_debug(AFFINITY_DETAILS, "gendists returned non-success (%i)\n", (int)ret);
            return ret;
        }
    }

    // Set task argument buffer size
    GET_ENV_NUM("QTHREAD_ARGCOPY_SIZE", qlib->qthread_argcopy_size, ARGCOPY_DEFAULT, 0);
    qthread_debug(CORE_DETAILS, "qthread task argcopy size: %u\n", (unsigned)qlib->qthread_argcopy_size);

    // Set task-local data size
    GET_ENV_NUM("QTHREAD_TASKLOCAL_SIZE", qlib->qthread_tasklocal_size, TASKLOCAL_DEFAULT, sizeof(void *));
    qthread_debug(CORE_DETAILS, "qthread task-local size: %u\n", qlib->qthread_tasklocal_size);

#ifndef UNPOOLED
/* set up the memory pools */
    qthread_debug(CORE_DETAILS, "shepherd pools sync = %i\n", need_sync);
    for (i = 0; i < nshepherds; i++) { /*{{{ */
        /* the following SHOULD only be accessed by one thread at a time, so
         * should be quite safe unsynchronized. If things fail, though...
         * resynchronize them and see if that fixes it. */
        qlib->shepherds[i].qthread_pool = qt_mpool_create(sizeof(qthread_t) + qlib->qthread_argcopy_size + qlib->qthread_tasklocal_size);
        qlib->shepherds[i].stack_pool   =
# ifdef QTHREAD_GUARD_PAGES
            qt_mpool_create_aligned(qlib->qthread_stack_size +
                                    sizeof(struct qthread_runtime_data_s) + (2 * getpagesize()),
                                    getpagesize());
# else
            qt_mpool_create(qlib->qthread_stack_size + sizeof(struct qthread_runtime_data_s));
# endif
        qlib->shepherds[i].queue_pool = qt_mpool_create(sizeof(qthread_queue_t));
        // printf("stack_pool = %p, queue_pool = %p\n", qlib->shepherds[i].stack_pool, qlib->shepherds[i].queue_pool); fflush(stdout);
        qt_threadqueue_init_pools(&(qlib->shepherds[i].threadqueue_pools));
        qlib->shepherds[i].lock_pool     = qt_mpool_create(sizeof(qthread_lock_t));
        qlib->shepherds[i].addrres_pool  = qt_mpool_create(sizeof(qthread_addrres_t));
        qlib->shepherds[i].addrstat_pool = qt_mpool_create(sizeof(qthread_addrstat_t));
    }                      /*}}} */
/* these are used when qthread_fork() is called from a non-qthread. */
    generic_qthread_pool = qt_mpool_create(sizeof(qthread_t) + qlib->qthread_argcopy_size + qlib->qthread_tasklocal_size);
    generic_stack_pool   =
# ifdef QTHREAD_GUARD_PAGES
        qt_mpool_create_aligned(qlib->qthread_stack_size + sizeof(struct qthread_runtime_data_s) +
                                (2 * getpagesize()), getpagesize());
# else
        qt_mpool_create(sizeof(struct qthread_runtime_data_s) + qlib->qthread_stack_size);
# endif
    generic_addrstat_pool = qt_mpool_create(sizeof(qthread_addrstat_t));
#endif /* ifndef UNPOOLED */
    initialize_hazardptrs();
    qt_lock_subsystem_init();
    qt_threadqueue_subsystem_init();
    qt_blocking_subsystem_init();

/* initialize the shepherd structures */
    for (i = 0; i < nshepherds; i++) {
        qthread_debug(SHEPHERD_DETAILS, "setting up shepherd %i (%p)\n", i, &qlib->shepherds[i]);
        qlib->shepherds[i].shepherd_id = (qthread_shepherd_id_t)i;
        QTHREAD_CASLOCK_INIT(qlib->shepherds[i].active, 1);
        qlib->shepherds[i].ready = qt_threadqueue_new(&(qlib->shepherds[i]));
        qassert_ret(qlib->shepherds[i].ready, QTHREAD_MALLOC_ERROR);
#ifdef QTHREAD_LOCK_PROFILING
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
    qlib->mccoy_thread = qthread_thread_new(NULL, NULL, 0, NULL, 0);
    qthread_debug(CORE_DETAILS, "mccoy thread = %p\n", qlib->mccoy_thread);
    qassert_ret(qlib->mccoy_thread, QTHREAD_MALLOC_ERROR);

    qthread_debug(CORE_DETAILS, "master_context = %p\n", &(qlib->master_context));
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

    qlib->mccoy_thread->rdata = malloc(sizeof(struct qthread_runtime_data_s));

    assert(qlib->mccoy_thread->rdata != NULL);
    qlib->mccoy_thread->rdata->shepherd_ptr = &(qlib->shepherds[0]);
    qlib->mccoy_thread->rdata->stack        = NULL;

#ifdef QTHREAD_USE_ROSE_EXTENSIONS
    qlib->mccoy_thread->rdata->taskWaitLock = SYNCVAR_EMPTY_INITIALIZER;
#endif

    qthread_debug(CORE_DETAILS, "enqueueing mccoy thread\n");
    qt_threadqueue_enqueue(qlib->shepherds[0].ready, qlib->mccoy_thread, &(qlib->shepherds[0]));
    qassert(getcontext(&(qlib->mccoy_thread->rdata->context)), 0);
    qassert(getcontext(&(qlib->master_context)), 0);
/* now build the context for the shepherd 0 */
    qthread_debug(CORE_DETAILS, "calling qthread_makecontext\n");
#ifdef QTHREAD_MULTITHREADED_SHEPHERDS
    qlib->shepherds[0].workers[0].shepherd  = &qlib->shepherds[0];
    qlib->shepherds[0].workers[0].active    = 1;
    qlib->shepherds[0].workers[0].worker_id = 0;
#endif
    qthread_makecontext(&(qlib->master_context), qlib->master_stack,
                        qlib->master_stack_size,
#ifdef QTHREAD_MAKECONTEXT_SPLIT
                        (void (*)(void))qthread_shepherd_wrapper,
#else
                        (void (*)(void))qthread_shepherd,
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
    qthread_debug(SHEPHERD_DETAILS, "launching shepherd 0\n");
#ifdef QTHREAD_USE_VALGRIND
    VALGRIND_CHECK_MEM_IS_ADDRESSABLE(&qlib->mccoy_thread->rdata->context, sizeof(qt_context_t));
    VALGRIND_CHECK_MEM_IS_ADDRESSABLE(&(qlib->master_context), sizeof(qt_context_t));
    VALGRIND_MAKE_MEM_DEFINED(&qlib->mccoy_thread->rdata->context, sizeof(qt_context_t));
    VALGRIND_MAKE_MEM_DEFINED(&(qlib->master_context), sizeof(qt_context_t));
#endif
    qthread_debug(CORE_DETAILS, "calling swapcontext\n");
#ifdef HAVE_NATIVE_MAKECONTEXT
    qassert(swapcontext(&qlib->mccoy_thread->rdata->context, &(qlib->master_context)), 0);
#else
    qassert(qt_swapctxt(&qlib->mccoy_thread->rdata->context, &(qlib->master_context)), 0);
#endif

#ifdef QTHREAD_MULTITHREADED_SHEPHERDS
# ifdef QTHREAD_RCRTOOL
    QTHREAD_FASTLOCK_INIT(rcrtool_lock);
    GET_ENV_NUM("QTHREAD_RCRTOOL_LEVEL", rcrtoollevel, 0, 0);
    GET_ENV_NUM("QTHREAD_RCRTOOL_LOG_LEVEL", rcrtoolloglevel, 0, 0);
    if (rcrtoollevel > 0) {
        qlib->nworkers_active = nshepherds * nworkerspershep - 1;
    } else {
        qlib->nworkers_active = nshepherds * nworkerspershep;
    }
# else /* ifdef QTHREAD_RCRTOOL */
    qlib->nworkers_active = nshepherds * nworkerspershep;
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
        if (i == nshepherds - 1) {
            qlib->shepherds[i].active_workers--;                    // the daemon uses up one worker
        }
# endif
        for (j = 0; j < nworkerspershep; ++j) {
            if ((i == 0) && (j == 0)) {
                continue;                       // original pthread becomes shep 0 worker 0
            }
            qlib->shepherds[i].workers[j].shepherd = &qlib->shepherds[i];
# ifdef QTHREAD_RCRTOOL
            if (rcrtoollevel > 0) {
                if ((i == nshepherds - 1) && (j == nworkerspershep - 1)) {
                    swinfo.nshepherds      = nshepherds;
                    swinfo.nworkerspershep = nworkerspershep;
                    swinfo.worker          = &qlib->shepherds[i].workers[j];
                    swinfo.qaffinity       = qaffinity;
                    if ((r = pthread_create(&rcrToolPThreadID, NULL, rcrtoolDaemon, &swinfo)) != 0) {
                        fprintf(stderr, "qthread_init: pthread_create() failed (%d)\n", r);
                        perror("qthread_init spawning rcrTool");
                        return r;
                    }
                    continue;
                }
            }
# endif     /* ifdef QTHREAD_RCRTOOL */
            qlib->shepherds[i].workers[j].worker_id        = j;
            qlib->shepherds[i].workers[j].packed_worker_id = j + (i * nworkerspershep);
            qlib->shepherds[i].workers[j].active           = 1;
# ifndef QTHREAD_RCRTOOL
            qlib->shepherds[i].workers[j].shepherd = &qlib->shepherds[i];
# endif     /* ifndef QTHREAD_RCRTOOL */
            if ((r = pthread_create(&qlib->shepherds[i].workers[j].worker, NULL,
                                    qthread_shepherd, &qlib->shepherds[i].workers[j])) != 0) {
                fprintf(stderr, "qthread_init: pthread_create() failed (%d)\n", r);
                perror("qthread_init spawning worker");
                return r;
            }
            qthread_debug(SHEPHERD_DETAILS, "spawned shep %i worker %i\n", (int)i, (int)j);
        }
    }
#else /* ifdef QTHREAD_MULTITHREADED_SHEPHERDS */
    for (i = 1; i < nshepherds; ++i) {
        qthread_debug(SHEPHERD_DETAILS,
                      "forking shepherd %i (%p)\n", i,
                      &qlib->shepherds[i]);
        if ((r = pthread_create(&qlib->shepherds[i].shepherd, NULL, qthread_shepherd, &qlib->shepherds[i])) != 0) {
            fprintf(stderr, "qthread_init: pthread_create() failed (%d)\n", r);
            perror("qthread_init spawning shepherd");
            return r;
        }
    }
#endif /* ifdef QTHREAD_MULTITHREADED_SHEPHERDS */

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
    c->uc_stack.ss_sp = (char *)(stack) + 8;
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
 * these functions get called AFTER shepherds are torn down */
void qthread_internal_cleanup(void (*function)(void))
{   /*{{{*/
    struct qt_cleanup_funcs_s *ng = malloc(sizeof(struct qt_cleanup_funcs_s));

    assert(ng);
    ng->func         = function;
    ng->next         = qt_cleanup_funcs;
    qt_cleanup_funcs = ng;
} /*}}}*/

/* this adds a function to the list of cleanup functions to call at finalize;
 * these functions get called BEFORE shepherds are torn down */
void qthread_internal_cleanup_early(void (*function)(void))
{   /*{{{*/
    struct qt_cleanup_funcs_s *ng = malloc(sizeof(struct qt_cleanup_funcs_s));

    assert(ng);
    ng->func               = function;
    ng->next               = qt_cleanup_early_funcs;
    qt_cleanup_early_funcs = ng;
} /*}}}*/

void qthread_finalize(void)
{                      /*{{{ */
    int                   r;
    qthread_shepherd_id_t i;
    qthread_t            *t;

#ifdef QTHREAD_MULTITHREADED_SHEPHERDS
    qthread_worker_t *worker;
#endif

#ifdef STEAL_PROFILE
    qthread_steal_stat();
#endif

    if (qlib == NULL || qthread_shep() != 0 || qthread_worker(NULL) != 0) {
        return;
    } else {
        qthread_t *t = qthread_internal_self();
        if (0 == (t->flags & QTHREAD_REAL_MCCOY)) {
            return;
        }
    }

    qthread_shepherd_t *shep0 = &(qlib->shepherds[0]);

#ifdef QTHREAD_MULTITHREADED_SHEPHERDS
    worker = qthread_internal_getworker();
    if (worker && worker->packed_worker_id != 0) {                       /* Only run finalize on shepherd 0 worker 0*/
        worker->current->thread_state = QTHREAD_STATE_YIELDED; /* Otherwise, put back */
        //      qt_threadqueue_enqueue(shep0->ready, worker->current,
        //             shep0);
        return;
    }
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
                continue;
            }
# endif
            qthread_debug(SHEPHERD_DETAILS, "terminating shepherd %i worker %i\n", (int)i, j);
            t = qthread_thread_new(NULL, NULL, 0, NULL, i);
            assert(t != NULL);         /* what else can we do? */
            t->thread_state = QTHREAD_STATE_TERM_SHEP;
            t->thread_id    = (unsigned int)-1;
            qt_threadqueue_enqueue(qlib->shepherds[i].ready, t,
                                   shep0);
        }
    }
#else /* ifdef QTHREAD_MULTITHREADED_SHEPHERDS */
    for (i = 1; i < qlib->nshepherds; i++) {
        qthread_debug(SHEPHERD_DETAILS, "terminating shepherd %i\n", (int)i);
        t = qthread_thread_new(NULL, NULL, 0, NULL, i);
        assert(t != NULL);     /* what else can we do? */
        t->thread_state = QTHREAD_STATE_TERM_SHEP;
        t->thread_id    = (unsigned int)-1;
        qt_threadqueue_enqueue(qlib->shepherds[i].ready, t,
                               shep0);
    }
#endif /* ifdef QTHREAD_MULTITHREADED_SHEPHERDS */

#ifdef QTHREAD_USE_ROSE_EXTENSIONS
    qthread_debug(BARRIER_DETAILS, "destroying the global barrier\n");
    qt_global_barrier_destroy();
#endif
#ifdef QTHREAD_SHEPHERD_PROFILING
    printf
        ("QTHREADS: Shepherd 0 spent %f%% of the time idle, handling %lu threads\n",
        shep0->idle_time / qtimer_secs(shep0->total_time) * 100.0,
        (unsigned long)shep0->num_threads);
    printf
        ("QTHREADS: Shepherd 0 averaged %g secs to find a new thread, max %g secs\n",
        shep0->idle_time / shep0->idle_count,
        shep0->idle_maxtime);
    qtimer_destroy(shep0->total_time);
#endif

    qthread_debug(CORE_DETAILS, "calling early cleanup functions\n");
    while (qt_cleanup_early_funcs != NULL) {
        struct qt_cleanup_funcs_s *tmp = qt_cleanup_early_funcs;
        qt_cleanup_early_funcs = tmp->next;
        tmp->func();
        free(tmp);
    }

    /* wait for each SPAWNED shepherd to drain it's queue
     * (note: not shepherd 0, because that one wasn't spawned) */
#ifdef QTHREAD_MULTITHREADED_SHEPHERDS
    for (i = 0; i < qlib->nshepherds; i++) {
        /* With multi-threaded shepherds, do join shepherd 0 workers, but not worker 0 */
        qthread_worker_id_t j;
        qthread_shepherd_t *shep = &(qlib->shepherds[i]);
        qthread_debug(SHEPHERD_DETAILS, "waiting for shepherd %i to exit\n", (int)i);
        for (j = 0; j < qlib->nworkerspershep; j++) {
            if ((i == 0) && (j == 0)) {
                continue;  /* This leaves out shepard 0's worker 0 */
            }
# ifdef QTHREAD_RCRTOOL
            if ((rcrtoollevel > 0) && ((i == qlib->nshepherds - 1) && (j == qlib->nworkerspershep - 1))) {
                if ((r = pthread_join(rcrToolPThreadID, NULL)) != 0) {
                    fprintf(stderr, "qthread_finalize: pthread_join() of rcrTool failed (%d, or \"%s\")\n", r, strerror(r));
                    fflush(stderr);
                    abort();
                }
                continue;
            }
# endif
            qthread_debug(SHEPHERD_DETAILS, "000\n");
            if ((r = pthread_join(shep->workers[j].worker, NULL)) != 0) {
                fprintf(stderr,
                        "qthread_finalize: pthread_join() of shep %i worker %i failed (%d, or \"%s\")\n",
                        (int)i, (int)j, r, strerror(r));
                fflush(stderr);
                abort();
            }
        }
        free(qlib->shepherds[i].workers);
        if (i == 0) { continue; }
#else /* ifdef QTHREAD_MULTITHREADED_SHEPHERDS */
    for (i = 1; i < qlib->nshepherds; i++) {
        qthread_shepherd_t *shep = &(qlib->shepherds[i]);
        qthread_debug(SHEPHERD_DETAILS, "waiting for shepherd %i to exit\n", (int)i);
        if ((r = pthread_join(shep->shepherd, NULL)) != 0) {
            fprintf(stderr,
                    "qthread_finalize: pthread_join() of shep %i failed (%d, or \"%s\")\n",
                    (int)i, r, strerror(r));
            abort();
        }
#endif  /* ifdef QTHREAD_MULTITHREADED_SHEPHERDS */
        QTHREAD_CASLOCK_DESTROY(shep->active);
        qt_threadqueue_free(shep->ready);
#ifdef QTHREAD_SHEPHERD_PROFILING
        printf
            ("QTHREADS: Shepherd %i spent %f%% of the time idle, handling %lu threads\n",
            i, shep->idle_time / qtimer_secs(shep->total_time) *
            100.0, (unsigned long)shep->num_threads);
        qtimer_destroy(shep->total_time);
        printf
            ("QTHREADS: Shepherd %i averaged %g secs to find a new thread, max %g secs\n",
            i, shep->idle_time / shep->idle_count, shep->idle_maxtime);
#endif
#ifdef QTHREAD_LOCK_PROFILING
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
#endif  /* ifdef QTHREAD_LOCK_PROFILING */
    }
    qthread_debug(CORE_DETAILS, "freeing shep0's threadqueue\n");
    qt_threadqueue_free(shep0->ready);

    qthread_debug(CORE_DETAILS, "calling cleanup functions\n");
    while (qt_cleanup_funcs != NULL) {
        struct qt_cleanup_funcs_s *tmp = qt_cleanup_funcs;
        qt_cleanup_funcs = tmp->next;
        tmp->func();
        free(tmp);
    }

#ifdef QTHREAD_LOCK_PROFILING
# ifdef QTHREAD_MUTEX_INCREMENT
    printf
        ("QTHREADS: %llu increments performed (%ld unique), average %g secs, max %g secs\n",
        (unsigned long long)shep0->incr_count, qt_hash_count(shep0->uniqueincraddrs),
        (shep0->incr_count == 0) ? 0 : (shep0->incr_time / shep0->incr_count),
        shep0->incr_maxtime);
    qt_hash_destroy(shep0->uniqueincraddrs);
# endif
    printf
        ("QTHREADS: %llu locks aquired (%ld unique), average %g secs, max %g secs\n",
        (unsigned long long)shep0->aquirelock_count, qt_hash_count(shep0->uniquelockaddrs),
        (shep0->aquirelock_count == 0) ? 0 : (shep0->aquirelock_time /
                                              shep0->aquirelock_count), shep0->aquirelock_maxtime);
    qt_hash_destroy(shep0->uniquelockaddrs);
    printf
        ("QTHREADS: Blocked on a lock %llu times, average %g secs, max %g secs\n",
        (unsigned long long)shep0->lockwait_count,
        (shep0->lockwait_count == 0) ? 0 : (shep0->lockwait_time / shep0->lockwait_count),
        shep0->lockwait_maxtime);
    printf("QTHREADS: Locks held an average of %g seconds, max %g seconds\n",
           (shep0->aquirelock_count == 0) ? 0 : (shep0->hold_time / shep0->aquirelock_count),
           shep0->hold_maxtime);
    printf("QTHREADS: %ld unique addresses used with FEB, blocked %g secs\n",
           qt_hash_count(shep0->uniquefebaddrs),
           (shep0->febblock_count == 0) ? 0 : shep0->febblock_time);
    qt_hash_destroy(shep0->uniquefebaddrs);
    printf
        ("QTHREADS: %llu potentially-blocking FEB operations, average %g secs, max %g secs\n",
        (unsigned long long)shep0->febblock_count,
        (shep0->febblock_count == 0) ? 0 : (shep0->febblock_time / shep0->febblock_count),
        shep0->febblock_maxtime);
    printf
        ("QTHREADS: %llu FEB operations blocked, average wait %g secs, max %g secs\n",
        (unsigned long long)shep0->febwait_count,
        (shep0->febwait_count == 0) ? 0 : (shep0->febwait_time / shep0->febwait_count),
        shep0->febwait_maxtime);
    printf
        ("QTHREADS: %llu FEB bits emptied, stayed empty average %g secs, max %g secs\n",
        (unsigned long long)shep0->empty_count,
        (shep0->empty_count == 0) ? 0 : (shep0->empty_time /
                                         shep0->empty_count), shep0->empty_maxtime);
#endif /* ifdef QTHREAD_LOCK_PROFILING */

    for (i = 0; i < QTHREAD_LOCKING_STRIPES; i++) {
        qthread_debug(LOCK_DETAILS, "destroying lock infrastructure of shep %i\n", (int)i);
        qt_hash_destroy(qlib->locks[i]);
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
        printf("QTHREADS: bin %i used %u times for locks, %u times for FEBs\n", i,
               (unsigned int)qlib->locks_stripes[i], (unsigned int)qlib->febs_stripes[i]);
# ifdef QTHREAD_MUTEX_INCREMENT
        QTHREAD_FASTLOCK_DESTROY(qlib->locks_stripes_locks[i]);
        QTHREAD_FASTLOCK_DESTROY(qlib->febs_stripes_locks[i]);
# endif
#endif
    }
    qthread_debug(LOCK_DETAILS, "destroy lock infrastructure arrays\n");
    free(qlib->locks);
    free(qlib->FEBs);
    free(qlib->syncvars);
#if defined(QTHREAD_MUTEX_INCREMENT) || (QTHREAD_ASSEMBLY_ARCH == QTHREAD_POWERPC32)
    free((void*)qlib->atomic_locks);
#endif
#ifdef QTHREAD_COUNT_THREADS
    free(qlib->locks_stripes);
    free(qlib->febs_stripes);
# ifdef QTHREAD_MUTEX_INCREMENT
    free(qlib->locks_stripes_locks);
    free(qlib->febs_stripes_locks);
# endif
#endif

#ifdef QTHREAD_COUNT_THREADS
    printf("QTHREADS: spawned %lu threads, max concurrency %lu, avg concurrency %g\n",
           (unsigned long)threadcount, (unsigned long)maxconcurrentthreads,
           avg_concurrent_threads);
    QTHREAD_FASTLOCK_DESTROY(concurrentthreads_lock);
#endif

    qthread_debug(LOCK_DETAILS, "destroy scheduling locks\n");
    QTHREAD_FASTLOCK_DESTROY(qlib->max_thread_id_lock);
    QTHREAD_FASTLOCK_DESTROY(qlib->sched_shepherd_lock);

#ifdef QTHREAD_USE_VALGRIND
    VALGRIND_STACK_DEREGISTER(qlib->mccoy_thread->rdata->valgrind_stack_id);
    VALGRIND_STACK_DEREGISTER(qlib->valgrind_masterstack_id);
#endif
    assert(qlib->mccoy_thread->rdata->stack == NULL);
    qthread_debug(CORE_DETAILS, "destroy mccoy thread structure\n");
    free(qlib->mccoy_thread->rdata);
    FREE_QTHREAD(qlib->mccoy_thread);
    qthread_debug(CORE_DETAILS, "destroy master stack\n");
    free(qlib->master_stack);
    for (i = 0; i < qlib->nshepherds; ++i) {
        qthread_debug(AFFINITY_DETAILS, "destroy topology information on shep %i\n", (int)i);
        if (qlib->shepherds[i].shep_dists) {
            free(qlib->shepherds[i].shep_dists);
        }
        if (qlib->shepherds[i].sorted_sheplist) {
            free(qlib->shepherds[i].sorted_sheplist);
        }
    }
#ifndef UNPOOLED
    for (i = 0; i < qlib->nshepherds; ++i) {
        qthread_debug(CORE_DETAILS, "destroy shep %i qthread pool\n", (int)i);
        qt_mpool_destroy(qlib->shepherds[i].qthread_pool);
        qthread_debug(CORE_DETAILS, "destroy shep %i queue pool\n", (int)i);
        qt_mpool_destroy(qlib->shepherds[i].queue_pool);
        qthread_debug(CORE_DETAILS, "destroy shep %i threadqueue pools\n", (int)i);
        qt_threadqueue_destroy_pools(&qlib->shepherds[i].threadqueue_pools);
        qthread_debug(CORE_DETAILS, "destroy shep %i lock pool\n", (int)i);
        qt_mpool_destroy(qlib->shepherds[i].lock_pool);
        qthread_debug(CORE_DETAILS, "destroy shep %i addrres pool\n", (int)i);
        qt_mpool_destroy(qlib->shepherds[i].addrres_pool);
        qthread_debug(CORE_DETAILS, "destroy shep %i addrstat pool\n", (int)i);
        qt_mpool_destroy(qlib->shepherds[i].addrstat_pool);
        qthread_debug(CORE_DETAILS, "destroy shep %i stack pool\n", (int)i);
        qt_mpool_destroy(qlib->shepherds[i].stack_pool);
    }
    qthread_debug(CORE_DETAILS, "destroy global memory pools\n");
    qt_mpool_destroy(generic_qthread_pool);
    generic_qthread_pool = NULL;
    qt_mpool_destroy(generic_stack_pool);
    generic_stack_pool = NULL;
    qt_mpool_destroy(generic_addrstat_pool);
    generic_addrstat_pool = NULL;
#endif /* ifndef UNPOOLED */
    qthread_debug(CORE_DETAILS, "destroy global shepherd array\n");
    free(qlib->shepherds);
    qthread_debug(CORE_DETAILS, "destroy global data\n");
    free(qlib);
    qlib = NULL;
    qthread_debug(CORE_DETAILS, "destroy shepherd thread-local data\n");
    qassert(pthread_key_delete(shepherd_structs), 0);

    qthread_debug(CORE_DETAILS, "finished.\n");
    fflush(stdout);
}                      /*}}} */

#ifdef QTHREAD_USE_ROSE_EXTENSIONS
void qthread_pack_workerid(const qthread_worker_id_t w,
                           const qthread_worker_id_t newId)
{
    int shep   = w % qlib->nshepherds;
    int worker = w / qlib->nshepherds;

    assert((shep < qlib->nshepherds));
    assert((worker < qlib->nworkerspershep));
    qlib->shepherds[shep].workers[worker].packed_worker_id = newId;
}

#endif /* ifdef QTHREAD_USE_ROSE_EXTENSIONS */

int qthread_disable_worker(const qthread_worker_id_t w)
{
#ifdef QTHREAD_MULTITHREADED_SHEPHERDS
    int shep   = w % qlib->nshepherds;
    int worker = w / qlib->nshepherds;

    qassert_ret((shep < qlib->nshepherds), QTHREAD_BADARGS);
    qassert_ret((worker < qlib->nworkerspershep), QTHREAD_BADARGS);
    if ((worker == 0) & (shep == 0)) {
        /* currently, the "real mccoy" original thread cannot be migrated
         * (because I don't know what issues that could cause on all
         * architectures). For similar reasons, therefore, the original
         * shepherd cannot be disabled. One of the nice aspects of this is that
         * therefore it is impossible to disable ALL shepherds.
         *
         * ... it's entirely possible that I'm being overly cautious. This is a
         * policy based on gut feeling rather than specific issues. */
        return QTHREAD_NOT_ALLOWED;
    }
    qthread_debug(SHEPHERD_CALLS, "began on worker(%i-%i)\n", shep, worker);

    (void)QT_CAS(qlib->shepherds[shep].workers[worker].active, 1, 0);
    qlib->nworkers_active--; // decrement active count

    if (worker == 0) { qthread_disable_shepherd(shep); }

    return QTHREAD_SUCCESS;

#else /* ifdef QTHREAD_MULTITHREADED_SHEPHERDS */
    return qthread_disable_shepherd(w);
#endif /* ifdef QTHREAD_MULTITHREADED_SHEPHERDS */
}

void qthread_enable_worker(const qthread_worker_id_t w)
{                      /*{{{ */
#ifdef QTHREAD_MULTITHREADED_SHEPHERDS
    int shep   = w % qlib->nshepherds;
    int worker = w / qlib->nshepherds;

    assert(shep < qlib->nshepherds);

    if (worker == 0) { qthread_enable_shepherd(shep); }
    qthread_debug(SHEPHERD_CALLS, "began on shep(%i)\n", shep);
    if (worker < qlib->nworkerspershep) {
        qthread_internal_incr(&(qlib->nshepherds_active), &(qlib->nshepherds_active_lock), 1);
        (void)QT_CAS(qlib->shepherds[shep].workers[worker].active, 0, 1);
    }
#else /* ifdef QTHREAD_MULTITHREADED_SHEPHERDS */
    qthread_enable_shepherd(w);
#endif /* ifdef QTHREAD_MULTITHREADED_SHEPHERDS */
}                      /*}}} */

int qthread_disable_shepherd(const qthread_shepherd_id_t shep)
{                      /*{{{ */
    qassert_ret((shep < qlib->nshepherds), QTHREAD_BADARGS);
    if (shep == 0) {
        /* currently, the "real mccoy" original thread cannot be migrated
         * (because I don't know what issues that could cause on all
         * architectures). For similar reasons, therefore, the original
         * shepherd cannot be disabled. One of the nice aspects of this is that
         * therefore it is impossible to disable ALL shepherds.
         *
         * ... it's entirely possible that I'm being overly cautious. This is a
         * policy based on gut feeling rather than specific issues. */
        return QTHREAD_NOT_ALLOWED;
    }
    qthread_debug(SHEPHERD_CALLS, "began on shep(%i)\n", shep);
    qthread_internal_incr(&(qlib->nshepherds_active), &(qlib->nshepherds_active_lock), -1);
    (void)QT_CAS(qlib->shepherds[shep].active, 1, 0);
    return QTHREAD_SUCCESS;
}                      /*}}} */

void qthread_enable_shepherd(const qthread_shepherd_id_t shep)
{                      /*{{{ */
    assert(shep < qlib->nshepherds);
    qthread_debug(SHEPHERD_CALLS, "began on shep(%i)\n", shep);
    qthread_internal_incr(&(qlib->nshepherds_active), &(qlib->nshepherds_active_lock), 1);
    (void)QT_CAS(qlib->shepherds[shep].active, 0, 1);
}                      /*}}} */

qthread_t INTERNAL *qthread_internal_self(void)
{                      /*{{{ */
#ifdef QTHREAD_MULTITHREADED_SHEPHERDS
    qthread_worker_t *worker = qthread_internal_getworker();
    return worker ? worker->current : NULL;

#else
    qthread_shepherd_t *shep = qthread_internal_getshep();
    return shep ? shep->current : NULL;
#endif
}                      /*}}} */

qthread_t *qthread_self(void)
{                      /*{{{ */
#ifdef QTHREAD_MULTITHREADED_SHEPHERDS
    qthread_worker_t *worker = qthread_internal_getworker();
    return worker ? worker->current : NULL;

#else
    qthread_shepherd_t *shep = qthread_internal_getshep();
    return shep ? shep->current : NULL;
#endif
}                      /*}}} */

void *qthread_get_tasklocal(unsigned int size)
{
    qthread_t *f = qthread_internal_self();

    if (NULL != f) {
        if ((0 == f->tasklocal_size) && (size <= qlib->qthread_tasklocal_size)) {
            // Use default space
            return &f->data[qlib->qthread_argcopy_size];
        } else {
            void **data_blob = (void **)&f->data[qlib->qthread_argcopy_size];
            if (0 == f->tasklocal_size) {
                // Allocate space and copy old data
                void *tmp_data = malloc(size);
                assert(NULL != tmp_data);

                memcpy(tmp_data, data_blob, qlib->qthread_tasklocal_size);
                *data_blob = tmp_data;

                f->tasklocal_size = size;
                return *data_blob;
            } else if (size <= f->tasklocal_size) {
                // Use alloc'd data blob, no need to resize
                return *data_blob;
            } else {
                // Resize alloc'd data blob
                *data_blob = realloc(*data_blob, size);
                assert(NULL != *data_blob);

                f->tasklocal_size = size;
                return *data_blob;
            }
        }
    }

    return NULL;
}

unsigned qthread_size_tasklocal(void)
{
    const qthread_t *f = qthread_internal_self();

    assert(NULL != f);

    return f->tasklocal_size ? f->tasklocal_size : qlib->qthread_tasklocal_size;
}

size_t qthread_stackleft(void)
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

size_t qthread_readstate(const enum introspective_state type)
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

        default:
            return (size_t)(-1);
    }
}                      /*}}} */

aligned_t *qthread_retloc(void)
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
static QINLINE qthread_t *qthread_thread_new(const qthread_f             f,
                                             const void                 *arg,
                                             size_t                      arg_size,
                                             void                       *ret,
                                             const qthread_shepherd_id_t shepherd)
{                      /*{{{ */
    qthread_t *t;

#ifndef UNPOOLED
    qthread_shepherd_t *myshep = &(qlib->shepherds[shepherd]);
#endif

    t       = ALLOC_QTHREAD(myshep);
    t->next = NULL;
    qthread_debug(THREAD_DETAILS, "t = %p\n", t);
    qassert_ret(t, NULL);

#ifdef QTHREAD_NONLAZY_THREADIDS
    /* give the thread an ID number */
    t->thread_id =
        qthread_internal_incr(&(qlib->max_thread_id),
                              &qlib->max_thread_id_lock, 1);
#else
    t->thread_id = (unsigned int)-1;
#endif

    t->thread_state    = QTHREAD_STATE_NEW;
    t->flags           = 0;
    t->target_shepherd = NULL;
    t->f               = f;
    t->arg             = (void *)arg;
    t->ret             = ret;
    t->rdata           = NULL;
#ifdef QTHREAD_USE_ROSE_EXTENSIONS
    t->task_completed  = 0;
    t->child           = NULL;
    t->sibling         = NULL;
#endif
    // should I use the builtin block for args?
    t->flags &= ~QTHREAD_HAS_ARGCOPY;
    if (arg_size > 0) {
        if (arg_size <= qlib->qthread_argcopy_size) {
            t->arg = (void *)(&t->data);
        } else {
            t->arg    = malloc(arg_size);
            t->flags |= QTHREAD_HAS_ARGCOPY;
        }
        memcpy(t->arg, arg, arg_size);
    }
    t->tasklocal_size = 0;

    qthread_debug(THREAD_DETAILS, "returning\n");
    return t;
}                      /*}}} */

static QINLINE void qthread_thread_free(qthread_t *t)
{                      /*{{{ */
    assert(t != NULL);

    qthread_debug(THREAD_FUNCTIONS, "t(%p): destroying thread id %i\n", t, t->thread_id);
    if (t->rdata != NULL) {
#ifdef QTHREAD_USE_VALGRIND
        VALGRIND_STACK_DEREGISTER(t->rdata->valgrind_stack_id);
#endif
        qthread_debug(THREAD_DETAILS, "t(%p): releasing stack %p to %p\n", t, t->rdata->stack, t->creator_ptr);
	FREE_STACK(t->creator_ptr, t->rdata->stack);
	t->rdata = NULL;
    }
    if (t->flags & QTHREAD_HAS_ARGCOPY) {
        assert(&t->data != t->arg);
        free(t->arg);
	t->arg = NULL;
    }
    if (t->tasklocal_size > sizeof(void *)) {
        free(*(void **)&t->data[qlib->qthread_argcopy_size]);
	*(void**)&t->data[qlib->qthread_argcopy_size] = NULL;
    }
    qthread_debug(THREAD_DETAILS, "t(%p): releasing thread handle %p\n", t, t);
#ifndef QTHREAD_USE_ROSE_EXTENSIONS
    FREE_QTHREAD(t);   // if ROSE -- thread cannot be freed until parent approves -- see qthread_task_free
#endif
}                      /*}}} */

static QINLINE void qthread_enqueue(qthread_queue_t *q,
                                    qthread_t       *t)
{                      /*{{{ */
    assert(t != NULL);
    assert(q != NULL);
    assert(t->next == NULL);

    qthread_debug(THREAD_FUNCTIONS, "q(%p), t(%p): started\n", q, t);

    QTHREAD_LOCK(&q->lock);

    t->next = NULL;

    if (q->head == NULL) {         /* surely then tail is also null; no need to check */
        q->head = t;
        q->tail = t;
        QTHREAD_SIGNAL(&q->notempty);
    } else {
        q->tail->next = t;
        q->tail       = t;
    }

    qthread_debug(THREAD_DETAILS, "q(%p), t(%p): finished\n", q, t);
    QTHREAD_UNLOCK(&q->lock);
}                      /*}}} */

#ifdef QTHREAD_ALLOW_HPCTOOLKIT_STACK_UNWINDING
// in Rose only code -- call function with rose argument list
// pulled from HPCToolkit externals

int in_qthread_fence(void *addr);
extern void *qthread_fence1;
extern void *qthread_fence2;

# define MONITOR_ASM_LABEL(name)     \
    asm volatile (".globl " # name); \
    asm volatile (# name ":")
#endif /* ifdef QTHREAD_ALLOW_HPCTOOLKIT_STACK_UNWINDING */

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
    assert((size_t)&t > (size_t)t->rdata->stack &&
           (size_t)&t < ((size_t)t->rdata->stack + qlib->qthread_stack_size));
#ifdef QTHREAD_COUNT_THREADS
    QTHREAD_FASTLOCK_LOCK(&concurrentthreads_lock);
    threadcount++;
    concurrentthreads++;
    if (concurrentthreads > maxconcurrentthreads) {
        maxconcurrentthreads = concurrentthreads;
    }
    avg_concurrent_threads =
        (avg_concurrent_threads * (double)(threadcount - 1.0) / threadcount)
        + ((double)concurrentthreads / threadcount);
    QTHREAD_FASTLOCK_UNLOCK(&concurrentthreads_lock);
#endif /* ifdef QTHREAD_COUNT_THREADS */
    if (t->ret) {
        /* XXX: if this fails, we should probably do something */
        if (t->flags & QTHREAD_RET_IS_SYNCVAR) {
            /* this should avoid problems with irresponsible return values */
            qassert(qthread_syncvar_writeEF_const((syncvar_t *)t->ret,
                                                  INT64TOINT60((t->f)(t->arg))), QTHREAD_SUCCESS);
        } else {
            qassert(qthread_writeEF_const((aligned_t *)t->ret, (t->f)(t->arg)), QTHREAD_SUCCESS);
        }
    } else {
        assert(t->f);
        (t->f)(t->arg);
    }
    t->thread_state = QTHREAD_STATE_TERMINATED;

#ifdef QTHREAD_COUNT_THREADS
    QTHREAD_FASTLOCK_LOCK(&concurrentthreads_lock);
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
    qthread_back_to_master(t);
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
        t->rdata->context.uc_link = c;   /* NULL pthread_exit() */
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
#endif /* ifdef NEED_RLIMIT */

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

    assert(t != NULL);
    assert(c != NULL);

    qthread_debug(SHEPHERD_DETAILS, "t(%p): finished, t->thread_state = %i\n", t, t->thread_state);
}                      /*}}} */

/* this function yields thread t to the master kernel thread */
void qthread_yield(void)
{                      /*{{{ */
    qthread_t *t = qthread_internal_self();

    if (t != NULL) {
        qthread_debug(THREAD_CALLS,
                      "tid %u yielding...\n", t->thread_id);
        t->thread_state = QTHREAD_STATE_YIELDED;
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
typedef enum {
    ALIGNED_T,
    SYNCVAR_T,
    NO_SYNC
} synctype_t;

static int qthread_uberfork(qthread_f             f,
                            const void           *arg,
                            size_t                arg_size,
                            synctype_t            ret_type,
                            void                 *ret,
                            synctype_t            precond_type,
                            size_t                npreconds,
                            void                 *preconds,
                            qthread_shepherd_id_t target_shep,
                            uint_fast8_t          future_flag)
{   /*{{{*/
    qthread_t            *t;
    qthread_shepherd_t   *myshep = qthread_internal_getshep();
    qthread_shepherd_id_t dest_shep;

#if defined(QTHREAD_DEBUG) || !defined(QTHREAD_MULTITHREADED_SHEPHERDS)
    const qthread_shepherd_id_t max_sheps = qlib->nshepherds;
#endif

    /* Step 1: Check arguments */
    qthread_debug(THREAD_BEHAVIOR,
                  "f(%p), arg(%p), arg_size(%z), rt(%s), ret(%p), pt(%s), np(%z), pc(%p), ts(%u), %s\n",
                  f,
                  arg,
                  arg_size,
                  ((ret_type == ALIGNED_T) ? "aligned_t" : ((ret_type == SYNCVAR_T) ? "syncvar_t" : "none")),
                  ret,
                  ((precond_type == ALIGNED_T) ? "aligned_t" : ((precond_type == SYNCVAR_T) ? "syncvar_t" : "none")),
                  npreconds,
                  preconds,
                  target_shep,
                  (future_flag ? "future" : "qthread"));
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
            myshep->sched_shepherd *= (max_sheps > (dest_shep + 1));
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
            qassert_ret(precond_type != NO_SYNC, QTHREAD_BADARGS);
        } else {
            qassert_ret(preconds == NULL, QTHREAD_BADARGS);
            qassert_ret(precond_type == NO_SYNC, QTHREAD_BADARGS);
        }
#endif  /* ifdef QTHREAD_DEBUG */
    }
    /* Step 3: Allocate & init the structure */
    t = qthread_thread_new(f, arg, arg_size, (aligned_t *)ret, dest_shep);
    qassert_ret(t, QTHREAD_MALLOC_ERROR);
    if (QTHREAD_UNLIKELY(target_shep != NO_SHEPHERD)) {
        t->target_shepherd = &qlib->shepherds[dest_shep];
        t->flags          |= QTHREAD_UNSTEALABLE;
    }
    if (QTHREAD_UNLIKELY(npreconds != 0)) {
        t->thread_state = QTHREAD_STATE_NASCENT; // special non-executable state
        t->preconds     = preconds;
        t->npreconds    = npreconds;
        if (t->target_shepherd == NULL) {
            t->target_shepherd = (void *)(uintptr_t)dest_shep;
        }
    }
    t->id = target_shep;  /* used in barrier and arrive_first, NOT the
                           * thread-id may be extraneous in both when parallel
                           * region barriers in place (not will to pull it now
                           * maybe later) akp */
    if (QTHREAD_UNLIKELY(future_flag)) {
        t->flags |= QTHREAD_FUTURE;
    }
    qthread_debug(THREAD_BEHAVIOR, "new-tid %u shep %u\n", t->thread_id, target_shep);
#ifdef QTHREAD_USE_ROSE_EXTENSIONS
    qthread_t *me = qthread_internal_self();
    if (me) {
        t->currentParallelRegion = me->currentParallelRegion; // saved in shepherd
	t->sibling = me->child;  // add to parents tsk list
	t->child = NULL;
	me->child = t;
    }
#endif
    /* Step 4: Prepare the return value location (if necessary) */
#ifdef QTHREAD_USE_ROSE_EXTENSIONS
    if (!ret && (ret_type == SYNCVAR_T)){  // if no ret specified and syncvar requested
      ret = &t->ret_value;                 // use the one inside the thread
      t->ret = ret;                        // and set thread's return address
    }
#endif
    if (ret) {
        int test = QTHREAD_SUCCESS;
        switch(ret_type) {
            case SYNCVAR_T:
                t->flags |= QTHREAD_RET_IS_SYNCVAR;
                test      = qthread_syncvar_empty((syncvar_t *)ret);
                break;
            case ALIGNED_T:
                test = qthread_empty(ret);
                break;
            default: abort(); /* this should never happen */
        }
        if (QTHREAD_UNLIKELY(test != QTHREAD_SUCCESS)) {
            qthread_thread_free(t);
            return test;
        }
    }
    /* Step 5: Prepare the input preconditions (if necessary) */
    if (QTHREAD_LIKELY(!preconds) || (qthread_check_precond(t) == 0)) {
        /* Step 6: Set it going */
        qt_threadqueue_enqueue(qlib->shepherds[dest_shep].ready, t, myshep);
    }
    return QTHREAD_SUCCESS;
} /*}}}*/

int qthread_fork(qthread_f   f,
                 const void *arg,
                 aligned_t  *ret)
{   /*{{{*/
    return qthread_uberfork(f, arg, 0, ALIGNED_T, ret, NO_SYNC, 0, NULL, NO_SHEPHERD, 0);
} /*}}}*/

int qthread_fork_copyargs_precond(qthread_f   f,
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
        preconds = malloc(npreconds * sizeof(aligned_t *));
        assert(preconds != NULL);
        for (int i = 0; i < npreconds; ++i) {
            preconds[i] = va_arg(args, aligned_t *);
        }
    } else if (npreconds < 0) {
        npreconds *= -1;
        preconds   = malloc(npreconds * sizeof(aligned_t *));
        assert(preconds != NULL);
        aligned_t *tmp = va_arg(args, aligned_t *);
        for (int i = 0; i < npreconds; ++i) {
            preconds[i] = &tmp[i];
        }
    }
    va_end(args);

    return qthread_uberfork(f, arg, arg_size, SYNCVAR_T, ret, ALIGNED_T, npreconds, preconds, NO_SHEPHERD, 0);
} /*}}}*/

int qthread_fork_precond(qthread_f   f,
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
        preconds = malloc(npreconds * sizeof(aligned_t *));
        assert(preconds != NULL);
        for (int i = 0; i < npreconds; ++i) {
            preconds[i] = va_arg(args, aligned_t *);
        }
    } else if (npreconds < 0) {
        npreconds *= -1;
        preconds   = malloc(npreconds * sizeof(aligned_t *));
        assert(preconds != NULL);
        aligned_t *tmp = va_arg(args, aligned_t *);
        for (int i = 0; i < npreconds; ++i) {
            preconds[i] = &tmp[i];
        }
    }
    va_end(args);

    return qthread_uberfork(f, arg, 0, ALIGNED_T, ret, ALIGNED_T, npreconds, preconds, NO_SHEPHERD, 0);
} /*}}}*/

int qthread_fork_syncvar_copyargs_to(qthread_f             f,
                                     const void           *arg,
                                     size_t                arg_size,
                                     syncvar_t            *ret,
                                     qthread_shepherd_id_t preferred_shep)
{   /*{{{*/
    return qthread_uberfork(f, (void *const)arg, arg_size, SYNCVAR_T, ret, NO_SYNC, 0, NULL, preferred_shep, 0);
} /*}}}*/

int qthread_fork_copyargs(qthread_f   f,
                          const void *arg,
                          size_t      arg_size,
                          aligned_t  *ret)
{   /*{{{*/
    return qthread_uberfork(f, (void *const)arg, arg_size, ALIGNED_T, ret, NO_SYNC, 0, NULL, NO_SHEPHERD, 0);
}   /*}}}*/

int qthread_fork_syncvar_copyargs(qthread_f   f,
                                  const void *arg,
                                  size_t      arg_size,
                                  syncvar_t  *ret)
{                      /*{{{ */
    return qthread_uberfork(f, (void *const)arg, arg_size, SYNCVAR_T, ret, NO_SYNC, 0, NULL, NO_SHEPHERD, 0);
}                      /*}}} */

int qthread_fork_syncvar(qthread_f   f,
                         const void *arg,
                         syncvar_t  *ret)
{                      /*{{{ */
    return qthread_uberfork(f, (void *const)arg, 0, SYNCVAR_T, ret, NO_SYNC, 0, NULL, NO_SHEPHERD, 0);
}                      /*}}} */

int qthread_fork_to(qthread_f             f,
                    const void           *arg,
                    aligned_t            *ret,
                    qthread_shepherd_id_t shepherd)
{   /*{{{*/
    if ((shepherd != NO_SHEPHERD) && (shepherd >= qlib->nshepherds)) {
        shepherd %= qlib->nshepherds;
    }
    return qthread_uberfork(f, arg, 0, ALIGNED_T, ret, NO_SYNC, 0, NULL, shepherd, 0);
} /*}}}*/

int qthread_fork_precond_to(qthread_f             f,
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
        preconds = malloc(npreconds * sizeof(aligned_t *));
        assert(preconds != NULL);
        for (int i = 0; i < npreconds; ++i) {
            preconds[i] = va_arg(args, aligned_t *);
        }
    } else if (npreconds < 0) {
        npreconds *= -1;
        preconds   = malloc(npreconds * sizeof(aligned_t *));
        assert(preconds != NULL);
        aligned_t *tmp = va_arg(args, aligned_t *);
        for (int i = 0; i < npreconds; ++i) {
            preconds[i] = &tmp[i];
        }
    }
    va_end(args);

    if ((shepherd != NO_SHEPHERD) && (shepherd >= qlib->nshepherds)) {
        shepherd %= qlib->nshepherds;
    }
    return qthread_uberfork(f, arg, 0, ALIGNED_T, ret, ALIGNED_T, npreconds, preconds, shepherd, 0);
}                      /*}}} */

int qthread_fork_syncvar_to(qthread_f             f,
                            const void           *arg,
                            syncvar_t            *ret,
                            qthread_shepherd_id_t s)
{   /*{{{*/
    if ((s != NO_SHEPHERD) && (s >= qlib->nshepherds)) {
        s %= qlib->nshepherds;
    }
    return qthread_uberfork(f, arg, 0, SYNCVAR_T, ret, NO_SYNC, 0, NULL, s, 0);
} /*}}}*/

int qthread_fork_future_to(qthread_f             f,
                           const void           *arg,
                           aligned_t            *ret,
                           qthread_shepherd_id_t shepherd)
{   /*{{{*/
    if ((shepherd != NO_SHEPHERD) && (shepherd >= qlib->nshepherds)) {
        shepherd %= qlib->nshepherds;
    }
    return qthread_uberfork(f, arg, 0, ALIGNED_T, ret, NO_SYNC, 0, NULL, shepherd, 1);
} /*}}}*/

int qthread_fork_syncvar_future_to(qthread_f             f,
                                   const void           *arg,
                                   syncvar_t            *ret,
                                   qthread_shepherd_id_t shepherd)
{   /*{{{*/
    if ((shepherd != NO_SHEPHERD) && (shepherd >= qlib->nshepherds)) {
        shepherd %= qlib->nshepherds;
    }
    return qthread_uberfork(f, arg, 0, SYNCVAR_T, ret, NO_SYNC, 0, NULL, shepherd, 1);
} /*}}}*/

int qthread_fork_syncvar_future(qthread_f   f,
                                const void *arg,
                                syncvar_t  *ret)
{   /*{{{*/
    return qthread_uberfork(f, arg, 0, SYNCVAR_T, ret, NO_SYNC, 0, NULL, NO_SHEPHERD, 1);
} /*}}}*/

/*
 * This function walks the list of preconditions. When an empty variable is
 * encountered, it enqueues the "nascent" qthread in the associated FFQ. When
 * all preconditions are satisfied, the qthread state is set as "new".
 *
 * This is a modified readFF() that does not suspend the calling thread, but
 * simply enqueues the specified qthread in the FFQ associated with the target.
 */
int INTERNAL qthread_check_precond(qthread_t *t)
{
    aligned_t **these_preconds = (aligned_t **)t->preconds;

    // Process input preconds
    while (t->npreconds > 0) {
        aligned_t *this_sync = these_preconds[t->npreconds - 1];

        if (1 == qthread_feb_status(this_sync)) {
            t->npreconds--;
        } else {
            // Need to wait on this one, add to appropriate FFQ
            qthread_addrstat_t *m       = NULL;
            qthread_addrres_t  *X       = NULL;
            const int           lockbin = QTHREAD_CHOOSE_STRIPE(this_sync);
            const aligned_t    *alignedaddr;
            qthread_shepherd_t *const curshep = qthread_internal_getshep();

            QTHREAD_LOCK_UNIQUERECORD2(feb, this_sync, curshep);
            QALIGN(this_sync, alignedaddr);
            QTHREAD_COUNT_THREADS_BINCOUNTER(febs, lockbin);
            qt_hash_lock(qlib->FEBs[lockbin]);
            {
                m = (qthread_addrstat_t *)qt_hash_get_locked(qlib->FEBs[lockbin], (void *)alignedaddr);
                if (m) {
                    QTHREAD_FASTLOCK_LOCK(&m->lock);
                }
            }
            qt_hash_unlock(qlib->FEBs[lockbin]);
            qthread_debug(LOCK_DETAILS, "data structure locked\n");
            /* now m, if it exists, is locked - if m is NULL, then we're done! */
            if (m == NULL) {               /* already full! */
                t->npreconds--;
            } else if (m->full != 1) {     /* not full... so we must block */
                X = ALLOC_ADDRRES(curshep);
                if (X == NULL) {
                    QTHREAD_FASTLOCK_UNLOCK(&m->lock);
                    abort();
                    return 2; // memory allocation failure
                }
                X->addr         = NULL;
                X->waiter       = t;
                X->next         = m->FFQ;
                m->FFQ          = X;
                t->thread_state = QTHREAD_STATE_NASCENT;
                QTHREAD_FASTLOCK_UNLOCK(&m->lock);

                return 1;
            } else {
                // m->full == 1
                t->npreconds--;
                QTHREAD_FASTLOCK_UNLOCK(&m->lock);
            }
        }
    }

    // All input preconds are full
    t->thread_state = QTHREAD_STATE_NEW;
    if ((uintptr_t)t->target_shepherd < qlib->nshepherds) {
        t->target_shepherd = NULL;
    }

    return 0;
}

void INTERNAL qthread_back_to_master(qthread_t *t)
{                      /*{{{ */
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

/* function to move a qthread from one shepherd to another */
int qthread_migrate_to(const qthread_shepherd_id_t shepherd)
{                      /*{{{ */
    qthread_t *me = qthread_internal_self();

    if (me->flags & QTHREAD_REAL_MCCOY) {
        return QTHREAD_NOT_ALLOWED;
    }
    if (me->rdata->shepherd_ptr->shepherd_id == shepherd) {
        me->target_shepherd = me->rdata->shepherd_ptr;
        me->flags          |= QTHREAD_UNSTEALABLE;
        return QTHREAD_SUCCESS;
    }
    if (shepherd == NO_SHEPHERD) {
        me->target_shepherd = NULL;
        me->flags          &= ~ (uint8_t)QTHREAD_UNSTEALABLE;
        return QTHREAD_SUCCESS;
    }
    if (me && (shepherd < qlib->nshepherds)) {
        qthread_debug(THREAD_BEHAVIOR,
                      "tid %u from shep %u to shep %u\n",
                      me->thread_id, me->rdata->shepherd_ptr->shepherd_id, shepherd);
        me->target_shepherd  = &(qlib->shepherds[shepherd]);
        me->thread_state     = QTHREAD_STATE_MIGRATING;
        me->flags           |= QTHREAD_UNSTEALABLE;
        me->rdata->blockedon = (struct qthread_lock_s *)(intptr_t)shepherd;
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
# define QTHREAD_NUM_LOOP_STRUCT 16

int qt_omp_parallel_region_create()
{                      /*{{{ */
    int                        ret = 0;
    int                        workers;
    qthread_parallel_region_t *pr = malloc(sizeof(qthread_parallel_region_t));

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

    // create and initialize a fixed number of loop sturctures that will be used as a circular buffer to handle
    // omp parallel for loops -- the number of different active for loops is specified by QTHREAD_NUM_LOOP_STRUCT
    // which should be turned into an environment variable -- also we should check for the limit being hit and
    // fail with a message rather than just giving wrong answers which may now be the case AKP 7/26/11
    pr->clsSize           = QTHREAD_NUM_LOOP_STRUCT;
    pr->currentLoopNum    = malloc(sizeof(int) * workers);
    pr->currentLoopStruct = malloc(sizeof(struct qqloop_step_handle_t *) * pr->clsSize);
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

void qt_move_to_orig()
{
    qthread_t *t = qthread_internal_self();

    t->thread_state = QTHREAD_STATE_YIELDED;
    t->flags       |= QTHREAD_MUST_BE_WORKER_ZERO;
    qt_threadqueue_enqueue(qlib->shepherds[0].ready, t, 0); // put work back (having marked that
                                                            // it must be run by thread 0) -- put
                                                            // on queue that 0 looks at by default
    qthread_back_to_master(t);                              // return to work pile
}

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

    free(pr);
# endif /* if 0 */

    qthread_t *t = qthread_internal_self();

    qthread_parallel_region_t *pr = t->currentParallelRegion;
    t->currentParallelRegion = pr->last;
}                                      /*}}} */

#endif /* ifdef QTHREAD_USE_ROSE_EXTENSIONS */

/* These are just accessor functions */
unsigned qthread_id(void)
{                      /*{{{ */
    qthread_t *t = qthread_internal_self();

    qthread_debug(THREAD_CALLS, "tid(%u)\n",
                  t ? t->thread_id : (unsigned)-1);
#ifdef QTHREAD_NONLAZY_THREADIDS
    return t ? t->thread_id : (unsigned int)-1;

#else
    if (!t) {
        return (unsigned int)-1;
    }
    if (t->thread_id != (unsigned int)-1) {
        return t->thread_id;
    }
    ((qthread_t *)t)->thread_id =
        qthread_internal_incr(&(qlib->max_thread_id),
                              &qlib->max_thread_id_lock, 1);
    return t->thread_id;
#endif /* ifdef QTHREAD_NONLAZY_THREADIDS */
}                      /*}}} */

unsigned qthread_barrier_id(void)
{                      /*{{{ */
    qthread_t *t = qthread_internal_self();

    qthread_debug(THREAD_CALLS, "tid(%u)\n",
                  t ? t->id : (unsigned)-1);
    if (t && (t->id == NO_SHEPHERD)) {
        return qthread_internal_getshep()->shepherd_id;
    }
    return t ? t->id : (unsigned int)-1;
}                      /*}}} */

qthread_shepherd_id_t qthread_shep(void)
{                      /*{{{ */
    qthread_shepherd_t *ret = qthread_internal_getshep();

    if (qlib == NULL || ret == NULL) {
        return NO_SHEPHERD;
    } else {
        return ret->shepherd_id;
    }
}                      /*}}} */

qthread_worker_id_t qthread_worker(qthread_shepherd_id_t *shepherd_id)
{                                      /*{{{ */
#ifdef QTHREAD_MULTITHREADED_SHEPHERDS
    qthread_worker_t *worker = (qthread_worker_t *)pthread_getspecific(shepherd_structs);

    if(shepherd_id != NULL) {
        *shepherd_id = worker->shepherd->shepherd_id;
    }
    return worker ? (worker->packed_worker_id) : NO_WORKER;

#else
    return qthread_shep();
#endif /* ifdef QTHREAD_MULTITHREADED_SHEPHERDS */
}                                      /*}}} */

int qthread_shep_ok(void)
{                      /*{{{ */
    qthread_shepherd_t *ret = qthread_internal_getshep();

    if (ret == NULL) {
        return QTHREAD_PTHREAD_ERROR;
    } else {
        return QTHREAD_CASLOCK_READ_UI(ret->active);
    }
}                      /*}}} */

void qthread_shep_next(qthread_shepherd_id_t *shep)
{   /*{{{*/
    /* This will mean something slightly different in a multinode world. */
    qthread_shepherd_id_t cur = *shep;

    assert(cur != NO_SHEPHERD);
    cur++;
    cur  *= cur < qlib->nshepherds;
    *shep = cur;
} /*}}}*/

void qthread_shep_prev(qthread_shepherd_id_t *shep)
{   /*{{{*/
    /* This will mean something slightly different in a multinode world. */
    qthread_shepherd_id_t cur = *shep;

    assert(cur != NO_SHEPHERD);
    if (0 == cur) {
        cur = qlib->nshepherds - 1;
    } else {
        cur--;
    }
    *shep = cur;
} /*}}}*/

void qthread_shep_next_local(qthread_shepherd_id_t *shep)
{   /*{{{*/
    /* This is node-local */
    qthread_shepherd_id_t cur = *shep;

    assert(cur != NO_SHEPHERD);
    cur++;
    cur  *= cur < qlib->nshepherds;
    *shep = cur;
} /*}}}*/

void qthread_shep_prev_local(qthread_shepherd_id_t *shep)
{   /*{{{*/
    /* This is node-local */
    qthread_shepherd_id_t cur = *shep;

    assert(cur != NO_SHEPHERD);
    if (0 == cur) {
        cur = qlib->nshepherds - 1;
    } else {
        cur--;
    }
    *shep = cur;
} /*}}}*/

unsigned int INTERNAL qthread_internal_shep_to_node(const qthread_shepherd_id_t shep)
{                      /*{{{ */
    return qlib->shepherds[shep].node;
}                      /*}}} */

/* returns the distance between two shepherds */
int qthread_distance(const qthread_shepherd_id_t src,
                     const qthread_shepherd_id_t dest)
{                      /*{{{ */
    assert(src < qlib->nshepherds);
    assert(dest < qlib->nshepherds);
    if ((src >= qlib->nshepherds) || (dest >= qlib->nshepherds)) {
        return QTHREAD_BADARGS;
    }
    if (qlib->shepherds[src].shep_dists == NULL) {
        return 0;
    } else {
        return qlib->shepherds[src].shep_dists[dest];
    }
}                      /*}}} */

/* returns a list of shepherds, sorted by their distance from this qthread;
 * if NULL, then all sheps are equidistant */
const qthread_shepherd_id_t *qthread_sorted_sheps(void)
{                      /*{{{ */
    qthread_t *t = qthread_internal_self();

    if (t == NULL) {
        return NULL;
    }
    assert(t->rdata);
    assert(t->rdata->shepherd_ptr);
    return t->rdata->shepherd_ptr->sorted_sheplist;
}                      /*}}} */

/* returns a list of shepherds, sorted by their distance from the specified shepherd;
 * if NULL, then all sheps are equidistant */
const qthread_shepherd_id_t *qthread_sorted_sheps_remote(const
                                                         qthread_shepherd_id_t
                                                         src)
{                      /*{{{ */
    assert(src < qlib->nshepherds);
    if (src >= qlib->nshepherds) {
        return NULL;
    }
    return qlib->shepherds[src].sorted_sheplist;
}                      /*}}} */

/* returns the number of shepherds actively scheduling work */
qthread_shepherd_id_t qthread_num_shepherds(void)
{                      /*{{{ */
    return (qthread_shepherd_id_t)(qlib->nshepherds_active);
}                      /*}}} */

/* returns the number of shepherds actively scheduling work */
qthread_worker_id_t qthread_num_workers(void)
{                      /*{{{ */
    return (qthread_worker_id_t)qthread_readstate(ACTIVE_WORKERS);
}                      /*}}} */

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

// check to see if task freeable 1) QTHREAD_STATE_TERMINATED 2) parent has noticed it's completion
void qthread_task_free(qthread_t * t){
  assert(t);
  int tc = qthread_incr(&t->task_completed,1);
  if (tc == 1) { // needs to be freed from both workhorse loop and taskwait
    FREE_QTHREAD(t);  // everything else is freed when QTHREAD_STATE_TERMINATED
  }
}

int INTERNAL qthread_forCount(int inc)
{                                    /*{{{ */
    qthread_t *t = qthread_internal_self();

    assert(t);
    return (t->rdata->forCount += inc);
}                                    /*}}} */

void qthread_getTaskListLock(void)
{                      /*{{{ */
    qthread_t *t = qthread_internal_self();

    assert(t);
    qthread_syncvar_writeEF_const(&t->rdata->taskWaitLock, 1);
}                      /*}}} */

void qthread_releaseTaskListLock(void)
{                      /*{{{ */
    qthread_t *t = qthread_internal_self();

    assert(t);
    qthread_syncvar_readFE(NULL, &t->rdata->taskWaitLock);
}                      /*}}} */

// get child
qthread_t * qthread_child_task(void)
{                      /*{{{ */
    qthread_t *t = qthread_internal_self();

    assert(t);
    return t->child;
}                      /*}}} */

// look at task return value -- used by parent to determine child's completion
syncvar_t * qthread_return_value(qthread_t * t)
{                      /*{{{ */
    assert(t);
    return &t->ret_value;
}                      /*}}} */

// used by parent when child complete to remove from it's child list
void qthread_remove_child(qthread_t * child)
{                      /*{{{ */
    qthread_t *t = qthread_internal_self();
    assert(t);
    assert(t->next_child == child);
    if (t->child == child) {
      t->child = t->child->sibling;
      qthread_task_free(child);// task now freeable check to see if QTHREAD_STATE_TERMINATED
    }
    
}                      /*}}} */

#endif /* ifdef QTHREAD_USE_ROSE_EXTENSIONS */

#if defined(QTHREAD_MUTEX_INCREMENT) ||             \
    (QTHREAD_ASSEMBLY_ARCH == QTHREAD_POWERPC32) || \
    (QTHREAD_ASSEMBLY_ARCH == QTHREAD_SPARCV9_32)
uint32_t qthread_incr32_(volatile uint32_t *op,
                         const int32_t      incr)
{                      /*{{{ */
    unsigned int stripe = QTHREAD_CHOOSE_STRIPE(op);
    uint32_t     retval;

    QTHREAD_LOCK_TIMER_DECLARATION(incr);

    QTHREAD_COUNT_THREADS_BINCOUNTER(atomic, stripe);
    QTHREAD_LOCK_UNIQUERECORD(incr, op, qthread_internal_self());
    QTHREAD_LOCK_TIMER_START(incr);
    QTHREAD_FASTLOCK_LOCK(&(qlib->atomic_locks[stripe]));
    retval = *op;
    *op   += incr;
    QTHREAD_FASTLOCK_UNLOCK(&(qlib->atomic_locks[stripe]));
    QTHREAD_LOCK_TIMER_STOP(incr, qthread_internal_self());
    return retval;
}                      /*}}} */

uint64_t qthread_incr64_(volatile uint64_t *op,
                         const int64_t      incr)
{                      /*{{{ */
    unsigned int stripe = QTHREAD_CHOOSE_STRIPE(op);
    uint64_t     retval;

    QTHREAD_LOCK_TIMER_DECLARATION(incr);

    QTHREAD_COUNT_THREADS_BINCOUNTER(atomic, stripe);
    QTHREAD_LOCK_UNIQUERECORD(incr, op, qthread_internal_self());
    QTHREAD_LOCK_TIMER_START(incr);
    QTHREAD_FASTLOCK_LOCK(&(qlib->atomic_locks[stripe]));
    retval = *op;
    *op   += incr;
    QTHREAD_FASTLOCK_UNLOCK(&(qlib->atomic_locks[stripe]));
    QTHREAD_LOCK_TIMER_STOP(incr, qthread_internal_self());
    return retval;
}                      /*}}} */

double qthread_dincr_(volatile double *op,
                      const double     incr)
{                      /*{{{ */
    unsigned int stripe = QTHREAD_CHOOSE_STRIPE(op);
    double       retval;

    QTHREAD_FASTLOCK_LOCK(&(qlib->atomic_locks[stripe]));
    retval = *op;
    *op   += incr;
    QTHREAD_FASTLOCK_UNLOCK(&(qlib->atomic_locks[stripe]));
    return retval;
}                      /*}}} */

float qthread_fincr_(volatile float *op,
                     const float     incr)
{                      /*{{{ */
    unsigned int stripe = QTHREAD_CHOOSE_STRIPE(op);
    float        retval;

    QTHREAD_FASTLOCK_LOCK(&(qlib->atomic_locks[stripe]));
    retval = *op;
    *op   += incr;
    QTHREAD_FASTLOCK_UNLOCK(&(qlib->atomic_locks[stripe]));
    return retval;
}                      /*}}} */

uint32_t qthread_cas32_(volatile uint32_t *operand,
                        const uint32_t     oldval,
                        const uint32_t     newval)
{                      /*{{{ */
    uint32_t     retval;
    unsigned int stripe = QTHREAD_CHOOSE_STRIPE(operand);

    QTHREAD_FASTLOCK_LOCK(&(qlib->atomic_locks[stripe]));
    retval = *operand;
    if (retval == oldval) {
        *operand = newval;
    }
    QTHREAD_FASTLOCK_UNLOCK(&(qlib->atomic_locks[stripe]));
    return retval;
}                      /*}}} */

uint64_t qthread_cas64_(volatile uint64_t *operand,
                        const uint64_t     oldval,
                        const uint64_t     newval)
{                      /*{{{ */
    uint64_t     retval;
    unsigned int stripe = QTHREAD_CHOOSE_STRIPE(operand);

    QTHREAD_FASTLOCK_LOCK(&(qlib->atomic_locks[stripe]));
    retval = *operand;
    if (retval == oldval) {
        *operand = newval;
    }
    QTHREAD_FASTLOCK_UNLOCK(&(qlib->atomic_locks[stripe]));
    return retval;
}                      /*}}} */

#endif /* if defined(QTHREAD_MUTEX_INCREMENT) || (QTHREAD_ASSEMBLY_ARCH == QTHREAD_POWERPC32) */
/* vim:set expandtab: */
