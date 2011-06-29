#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

/* The API */
#include "qthread/qthread.h"

/* System Headers */
#include <stdio.h>
#include <stdlib.h>            /* for malloc() and abort() */
#ifdef HAVE_MALLOC_H
# include <malloc.h>               /* for memalign() */
#endif
#include <limits.h>              /* for INT_MAX */
#include <qthread/qthread-int.h> /* for UINT8_MAX */
#include <string.h>              /* for memset() */
#if !HAVE_MEMCPY
# define memcpy(d, s, n)  bcopy((s), (d), (n))
# define memmove(d, s, n) bcopy((s), (d), (n))
#endif
#include <sys/time.h>
#include <sys/resource.h>
#if (defined(QTHREAD_SHEPHERD_PROFILING) || defined(QTHREAD_LOCK_PROFILING))
# include <qthread/qtimer.h>
#endif
#ifdef QTHREAD_USE_PTHREADS
# include <pthread.h>
#endif
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

#ifdef QTHREAD_RCRTOOL
# include "rcrtool/qt_rcrtool.h"
extern QTHREAD_FASTLOCK_TYPE rcrtool_lock;
#endif

#if !(defined(HAVE_GCC_INLINE_ASSEMBLY) &&              \
    (QTHREAD_SIZEOF_ALIGNED_T == 4 ||                   \
     (QTHREAD_ASSEMBLY_ARCH != QTHREAD_POWERPC32 &&     \
      QTHREAD_ASSEMBLY_ARCH != QTHREAD_SPARCV9_32))) && \
    !defined(QTHREAD_MUTEX_INCREMENT)
# warning QTHREAD_MUTEX_INCREMENT not defined. It probably should be.
# define QTHREAD_MUTEX_INCREMENT 1
#endif

/* shared globals (w/ futurelib) */
pthread_key_t shepherd_structs;
qlib_t        qlib               = NULL;
int           qaffinity          = 1;
qt_mpool      generic_lock_pool  = NULL;
qt_mpool      generic_queue_pool = NULL;

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

#if defined(UNPOOLED_QTHREAD_T) || defined(UNPOOLED)
# define ALLOC_QTHREAD(shep) (qthread_t *)malloc(sizeof(qthread_t))
# define FREE_QTHREAD(t)     free(t)
#else
static qt_mpool generic_qthread_pool = NULL;
static QINLINE qthread_t *ALLOC_QTHREAD(qthread_shepherd_t *shep)
{                      /*{{{ */
    qthread_t *tmp = (qthread_t *)qt_mpool_alloc(shep
                                                 ? (shep->qthread_pool)
                                                 : generic_qthread_pool);

    if (tmp != NULL) {
        tmp->next        = NULL;
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
    char *tmp = valloc(qlib->qthread_stack_size + sizeof(struct qthread_runtime_data_s) + (2 * getpagesize()));

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
    char *tmp = t;

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
    char *tmp =
        qt_mpool_alloc(shep ? (shep->stack_pool) : generic_stack_pool);

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
    char *tmp = t;

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

    qthread_debug(ALL_DETAILS, "high(%x), low(%x): me = %p\n",
                  high, low, me);
    return qthread_shepherd(me);
}

#endif /* ifdef QTHREAD_MAKECONTEXT_SPLIT */

/* pulled from qthread_shepherd in case needed by stealing/run specific code */
extern const syncvar_t SYNCVAR_EMPTY_INITIALIZER;

static QINLINE void alloc_rdata(qthread_shepherd_t *me,
                                qthread_t          *t)
{
    void *stack = ALLOC_STACK(me);

    assert(stack);
#ifdef QTHREAD_GUARD_PAGES
    t->rdata = (struct qthread_runtime_data_s *)(((char *)stack) + getpagesize() + qlib->qthread_stack_size);
#else
    t->rdata = (struct qthread_runtime_data_s *)(((char *)stack) + qlib->qthread_stack_size);
#endif
    t->rdata->stack        = stack;
    t->rdata->shepherd_ptr = me;
    t->rdata->blockedon    = NULL;
#ifdef QTHREAD_USE_VALGRIND
    t->rdata->valgrind_stack_id = VALGRIND_STACK_REGISTER(stack, qlib->qthread_stack_size);
#endif
#ifdef QTHREAD_USE_ROSE_EXTENSIONS
    t->rdata->openmpTaskRetVar = NULL;
    //    t->rdata->taskWaitLock.u.w = 0;
    // qthread_syncvar_empty(&t->rdata->taskWaitLock);
    t->rdata->taskWaitLock = SYNCVAR_EMPTY_INITIALIZER;
#endif
}

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

    qthread_debug(ALL_DETAILS, "alive! me = %p\n", me);
    assert(me != NULL);
    assert(me->shepherd_id <= qlib->nshepherds);
    qthread_debug(ALL_FUNCTIONS, "id(%u): forked with arg %p\n", me->shepherd_id, arg);
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
    /* workhorse loop */
    while (!done) {
#ifdef QTHREAD_SHEPHERD_PROFILING
        qtimer_start(idle);
#endif
        qthread_debug(ALL_DETAILS, "id(%i): fetching a thread from my queue...\n", me->shepherd_id);
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

        if (t->thread_state == QTHREAD_STATE_TERM_SHEP) {
#ifdef QTHREAD_SHEPHERD_PROFILING
            qtimer_stop(me->total_time);
#endif
            done = 1;
            qthread_thread_free(t);
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
                fflush(stderr);
                t->rdata->shepherd_ptr = me;
            }
            assert(t->rdata->shepherd_ptr == me);

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
                qthread_debug(ALL_DETAILS,
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
                qthread_debug(ALL_DETAILS, "id(%u): back from qthread_exec, state is %i\n", me->shepherd_id, t->thread_state);
                /* now clean up, based on the thread's state */
                switch (t->thread_state) {
                    case QTHREAD_STATE_MIGRATING:
                        qthread_debug(THREAD_DETAILS,
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
                        qthread_debug(LOCK_DETAILS,
                                      "id(%u): thread %i blocked on FEB\n",
                                      me->shepherd_id, t->thread_id);
                        t->thread_state = QTHREAD_STATE_BLOCKED;
                        QTHREAD_FASTLOCK_UNLOCK(&
                                                (((qthread_addrstat_t *)(t->
                                                                         rdata->blockedon))->
                                                 lock));
                        break;

                    case QTHREAD_STATE_BLOCKED: /* put it in the blocked queue */
                        qthread_debug(LOCK_DETAILS,
                                      "id(%u): thread %i blocked on LOCK\n",
                                      me->shepherd_id, t->thread_id);
                        qthread_enqueue((qthread_queue_t *)t->rdata->blockedon->waiting, t);
                        QTHREAD_FASTLOCK_UNLOCK(&(t->rdata->blockedon->lock));
                        break;

                    case QTHREAD_STATE_SYSCALL:
                        t->thread_state = QTHREAD_STATE_RUNNING;
                        qthread_debug(THREAD_DETAILS,
                                      "id(%u): thread %i made a syscall\n",
                                      me->shepherd_id, t->thread_id);
                        qt_blocking_subsystem_enqueue((qt_blocking_queue_node_t *)t->rdata->blockedon);
                        break;

                    case QTHREAD_STATE_TERMINATED:
                        qthread_debug(THREAD_DETAILS,
                                      "id(%u): thread %i terminated\n",
                                      me->shepherd_id, t->thread_id);
                        t->thread_state = QTHREAD_STATE_DONE;
                        /* we can remove the stack and the context... */
                        qthread_thread_free(t);
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
    qthread_debug(ALL_DETAILS, "id(%u): finished\n",
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

    qthread_debug(ALL_FUNCTIONS, "l(%p): from %i sheps\n", l, (int)nsheps);
    if (l == NULL) {
        /* if l==NULL, there's no locality info, so just find the least-busy active shepherd */
        saligned_t busyness = 0;
        int        found    = 0;

        for (size_t i = 0; i < nsheps; i++) {
            if (QTHREAD_CASLOCK_READ_UI(sheps[i].active)) {
                ssize_t shep_busy_level = qt_threadqueue_advisory_queuelen(sheps[i].ready);

                if (found == 0) {
                    found = 1;
                    qthread_debug(ALL_FUNCTIONS,
                                  "l(%p): shep %i is the least busy (%i) so far\n",
                                  l, (int)i, shep_busy_level);
                    busyness = shep_busy_level;
                    target   = i;
                } else if ((shep_busy_level < busyness) ||
                           ((shep_busy_level == busyness) &&
                            (random() % 2 == 0))) {
                    qthread_debug(ALL_FUNCTIONS,
                                  "l(%p): shep %i is the least busy (%i) so far\n",
                                  l, (int)i, shep_busy_level);
                    busyness = shep_busy_level;
                    target   = i;
                }
            }
        }
        assert(found);
        if (found == 0) {
            qthread_debug(ALL_FUNCTIONS,
                          "l(%p): DID NOT FIND ANY ACTIVE SHEPHERDS!!!\n", l);
            return NULL;
        } else {
            qthread_debug(ALL_FUNCTIONS,
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
        qthread_debug(ALL_FUNCTIONS,
                      "l(%p): nearest active shepherd (%i) is %i away\n",
                      l, (int)l[target], (int)d[l[target]]);
        busyness = qt_threadqueue_advisory_queuelen(sheps[l[target]].ready);
        for (alt = target + 1; alt < (nsheps - 1) && d[l[alt]] == d[l[target]];
             alt++) {
            saligned_t shep_busy_level = qt_threadqueue_advisory_queuelen(sheps[l[alt]].ready);
            if ((shep_busy_level < busyness) ||
                ((shep_busy_level == busyness) && (random() % 2 == 0))) {
                qthread_debug(ALL_FUNCTIONS,
                              "l(%p): shep %i is the least busy (%i) so far\n",
                              l, (int)d[l[alt]], shep_busy_level);
                busyness = shep_busy_level;
                target   = alt;
            }
        }
        qthread_debug(ALL_FUNCTIONS, "l(%p): found target %i\n", l, (int)target);
        return &(sheps[l[target]]);
    }
}                      /*}}} */

int qthread_init(qthread_shepherd_id_t nshepherds)
{                      /*{{{ */
    char newenv[100] = { 0 };

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
int rcrSchedulingOff = 1;

int qthread_initialize(void)
{                      /*{{{ */
    int                   r;
    size_t                i;
    int                   need_sync  = 1;
    qthread_shepherd_id_t nshepherds = 0;

#ifdef QTHREAD_MULTITHREADED_SHEPHERDS
    qthread_worker_id_t nworkerspershep = 0;
#endif

#ifdef QTHREAD_DEBUG
    QTHREAD_FASTLOCK_INIT(output_lock);
    {
        char *qdl  = getenv("QTHREAD_DEBUG_LEVEL");
        char *qdle = NULL;

        if (qdl) {
            debuglevel = strtol(qdl, &qdle, 0);
            if ((qdle == NULL) || (qdle == qdl)) {
                fprintf(stderr, "unparseable debug level (%s)\n", qdl);
                debuglevel = 0;
            }
        } else {
            debuglevel = 0;
        }
    }
# ifdef SST
    debuglevel = 7;
# endif
#endif /* ifdef QTHREAD_DEBUG */

    qthread_debug(ALL_CALLS, "began.\n");
    if (qlib != NULL) {
        qthread_debug(ALL_DETAILS, "redundant call\n");
        return QTHREAD_SUCCESS;
    }
    qlib = (qlib_t)malloc(sizeof(struct qlib_s));
    qassert_ret(qlib, QTHREAD_MALLOC_ERROR);

#ifdef QTHREAD_USE_PTHREADS
    {
        char *qsh  = getenv("QTHREAD_NUM_SHEPHERDS");
        char *qshe = NULL;

        if (qsh) {
            nshepherds = strtol(qsh, &qshe, 0);
            if ((qshe == NULL) || (qshe == qsh)) {
                fprintf(stderr, "unparsable number of shepherds (%s)\n", qsh);
                nshepherds = 0;
            } else if (nshepherds > 0) {
                fprintf(stderr, "Forced %i Shepherds\n", (int)nshepherds);
            }
        } else {
            nshepherds = 0;
        }
# ifdef QTHREAD_MULTITHREADED_SHEPHERDS
        qsh  = getenv("QTHREAD_NUM_WORKERS_PER_SHEPHERD");
        qshe = NULL;

        if (qsh) {
            nworkerspershep = strtol(qsh, &qshe, 0);
            if ((qshe == NULL) || (qshe == qsh)) {
                fprintf(stderr, "unparsable number of workers (%s)\n", qsh);
                nworkerspershep = 1;
            } else if (nworkerspershep > 0) {
                fprintf(stderr, "Forced %i Workers per Shepherd\n", (int)nworkerspershep);
                if (nshepherds == 0) {
                    fprintf(stderr, "Number of shepherds not specified - number of workers may be ignored\n");
                }
            }
        }
# endif /* ifdef QTHREAD_MULTITHREADED_SHEPHERDS */
# ifdef QTHREAD_RCRTOOL
        qsh  = getenv("QTHREAD_RCR_SCHED_OFF");
        qshe = NULL;

        if (qsh) {
            rcrSchedulingOff = 0; // treat as True if it exists
        }
# endif /* ifdef QTHREAD_RCRTOOL */
    }
    qt_affinity_init(&nshepherds
# ifdef QTHREAD_MULTITHREADED_SHEPHERDS
                     , &nworkerspershep
# endif
                     );

    if ((nshepherds == 1)
# ifdef QTHREAD_MULTITHREADED_SHEPHERDS
       && (nworkerspershep == 1)
# endif
        ) {
        need_sync = 0;
    }
#else /* i.e. not using pthreads aka all serial. */
    nshepherds = 1;
# ifdef QTHREAD_MULTITHREADED_SHEPHERDS
    nworkerspershep = 1;
# endif
    need_sync = 0;
#endif /* ifdef QTHREAD_USE_PTHREADS */
    qthread_debug(THREAD_BEHAVIOR, "there will be %u shepherd(s)\n", (unsigned)nshepherds);

    QTHREAD_LOCKING_STRIPES = 1 << ((unsigned int)(log2(nshepherds)) + 1);

#ifdef QTHREAD_COUNT_THREADS
    threadcount            = 0;
    maxconcurrentthreads   = 0;
    concurrentthreads      = 0;
    avg_concurrent_threads = 0;
    avg_count              = 0;
    QTHREAD_FASTLOCK_INIT(concurrentthreads_lock);
#endif

    /* initialize the FEB-like locking structures */
#if defined(QTHREAD_MUTEX_INCREMENT) || (QTHREAD_ASSEMBLY_ARCH == QTHREAD_POWERPC32)
    qlib->atomic_locks = malloc(sizeof(QTHREAD_FASTLOCK_TYPE) * QTHREAD_LOCKING_STRIPES);
    qassert_ret(qlib->atomic_locks, QTHREAD_MALLOC_ERROR);
#endif
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
#if defined(QTHREAD_MUTEX_INCREMENT) || (QTHREAD_ASSEMBLY_ARCH == QTHREAD_POWERPC32)
        QTHREAD_FASTLOCK_INIT(qlib->atomic_locks[i]);
#endif
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
    qlib->nshepherds = nshepherds;
#ifdef QTHREAD_MULTITHREADED_SHEPHERDS
    qlib->nworkerspershep = nworkerspershep;
#endif
    qlib->nshepherds_active = nshepherds;
    qlib->shepherds         = (qthread_shepherd_t *)calloc(nshepherds, sizeof(qthread_shepherd_t));
    qassert_ret(qlib->shepherds, QTHREAD_MALLOC_ERROR);

    {
        char  *stacksize = getenv("QTHREAD_STACK_SIZE");
        size_t ss        = 0;

        if (stacksize && *stacksize) {
            char *eptr;

            ss = strtoul(stacksize, &eptr, 0);
            if (*eptr != 0) {
                ss = 0;
            }
        }
        if (ss != 0) {
            qlib->qthread_stack_size = ss;
        } else {
            qlib->qthread_stack_size = QTHREAD_DEFAULT_STACK_SIZE;
        }
        qthread_debug(THREAD_DETAILS, "qthread stack size: %u\n", qlib->qthread_stack_size);
    }
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
        qthread_debug(THREAD_DETAILS, "stack sizes ... cur: %u max: %u\n", rlp.rlim_cur, rlp.rlim_max);
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
    qthread_debug(ALL_DETAILS, "qaffinity = %i\n", qaffinity);
#ifdef QTHREAD_MULTITHREADED_SHEPHERDS
# define NUM_PTHREADS_REQUESTED nshepherds * nworkerspershep
#else
# define NUM_PTHREADS_REQUESTED nshepherds
#endif
    if ((qaffinity == 1) && (NUM_PTHREADS_REQUESTED > 1)) {
        int ret = qt_affinity_gendists(qlib->shepherds, nshepherds);
        if (ret != QTHREAD_SUCCESS) {
            qthread_debug(ALL_DETAILS, "gendists returned non-success (%i)\n", (int)ret);
            return ret;
        }
    }
#ifndef UNPOOLED
/* set up the memory pools */
    qthread_debug(ALL_DETAILS, "shepherd pools sync = %i\n", need_sync);
    for (i = 0; i < nshepherds; i++) { /*{{{ */
        /* the following SHOULD only be accessed by one thread at a time, so
         * should be quite safe unsynchronized. If things fail, though...
         * resynchronize them and see if that fixes it. */
        qlib->shepherds[i].qthread_pool = qt_mpool_create(sizeof(qthread_t));
        qlib->shepherds[i].stack_pool   =
# ifdef QTHREAD_GUARD_PAGES
            qt_mpool_create_aligned(qlib->qthread_stack_size +
                                    sizeof(struct qthread_runtime_data_s) + (2 * getpagesize()),
                                    getpagesize());
# else
            qt_mpool_create(qlib->qthread_stack_size + sizeof(struct qthread_runtime_data_s));
# endif
        qlib->shepherds[i].queue_pool = qt_mpool_create(sizeof(qthread_queue_t));
        qt_threadqueue_init_pools(&(qlib->shepherds[i].threadqueue_pools));
        qlib->shepherds[i].lock_pool     = qt_mpool_create(sizeof(qthread_lock_t));
        qlib->shepherds[i].addrres_pool  = qt_mpool_create(sizeof(qthread_addrres_t));
        qlib->shepherds[i].addrstat_pool = qt_mpool_create(sizeof(qthread_addrstat_t));
    }                      /*}}} */
/* these are used when qthread_fork() is called from a non-qthread. */
    generic_qthread_pool = qt_mpool_create(sizeof(qthread_t));
    generic_stack_pool   =
# ifdef QTHREAD_GUARD_PAGES
        qt_mpool_create_aligned(qlib->qthread_stack_size + sizeof(struct qthread_runtime_data_s) +
                                (2 * getpagesize()), getpagesize());
# else
        qt_mpool_create(sizeof(struct qthread_runtime_data_s) + qlib->qthread_stack_size);
# endif
    generic_queue_pool = qt_mpool_create(sizeof(qthread_queue_t));
    {
        extern qt_threadqueue_pools_t generic_threadqueue_pools;
        qt_threadqueue_init_pools(&generic_threadqueue_pools);
    }
    generic_lock_pool     = qt_mpool_create(sizeof(qthread_lock_t));
    generic_addrstat_pool = qt_mpool_create(sizeof(qthread_addrstat_t));
#endif /* ifndef UNPOOLED */
    qt_blocking_subsystem_init();

/* initialize the shepherd structures */
    for (i = 0; i < nshepherds; i++) {
        qthread_debug(ALL_DETAILS, "setting up shepherd %i (%p)\n", i, &qlib->shepherds[i]);
        qlib->shepherds[i].shepherd_id = (qthread_shepherd_id_t)i;
        QTHREAD_CASLOCK_INIT(qlib->shepherds[i].active, 1);
        qlib->shepherds[i].ready = qt_threadqueue_new(&(qlib->shepherds[i]));
        qassert_ret(qlib->shepherds[i].ready, QTHREAD_MALLOC_ERROR);
#ifdef QTHREAD_LOCK_PROFILING
# ifdef QTHREAD_MUTEX_INCREMENT
        qlib->shepherds[i].uniqueincraddrs = qt_hash_create(0);
# endif
        qlib->shepherds[i].uniquelockaddrs = qt_hash_create(0);
        qlib->shepherds[i].uniquefebaddrs  = qt_hash_create(0);
#endif

        qthread_debug(ALL_DETAILS, "shepherd %i set up (%p)\n", i, &qlib->shepherds[i]);
    }
    qthread_debug(ALL_DETAILS, "done setting up shepherds.\n");

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
    qthread_debug(ALL_DETAILS, "allocating shep0\n");
    qlib->mccoy_thread = qthread_thread_new(NULL, NULL, 0, NULL, 0);
    qthread_debug(ALL_DETAILS, "mccoy thread = %p\n", qlib->mccoy_thread);
    qassert_ret(qlib->mccoy_thread, QTHREAD_MALLOC_ERROR);

    qthread_debug(ALL_DETAILS, "master_context = %p\n", &(qlib->master_context));
    qlib->master_stack = calloc(1, qlib->master_stack_size);
    qassert_ret(qlib->master_stack, QTHREAD_MALLOC_ERROR);
    qthread_debug(ALL_DETAILS, "master_stack = %p\n", qlib->master_stack);
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

    qthread_debug(ALL_DETAILS, "enqueueing mccoy thread\n");
    qt_threadqueue_enqueue(qlib->shepherds[0].ready, qlib->mccoy_thread, &(qlib->shepherds[0]));
    qassert(getcontext(&(qlib->mccoy_thread->rdata->context)), 0);
    qassert(getcontext(&(qlib->master_context)), 0);
/* now build the context for the shepherd 0 */
    qthread_debug(ALL_DETAILS, "calling qthread_makecontext\n");
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
    qthread_debug(ALL_DETAILS, "launching shepherd 0\n");
#ifdef QTHREAD_USE_VALGRIND
    VALGRIND_CHECK_MEM_IS_ADDRESSABLE(&qlib->mccoy_thread->rdata->context, sizeof(qt_context_t));
    VALGRIND_CHECK_MEM_IS_ADDRESSABLE(&(qlib->master_context), sizeof(qt_context_t));
    VALGRIND_MAKE_MEM_DEFINED(&qlib->mccoy_thread->rdata->context, sizeof(qt_context_t));
    VALGRIND_MAKE_MEM_DEFINED(&(qlib->master_context), sizeof(qt_context_t));
#endif
    qthread_debug(ALL_DETAILS, "calling swapcontext\n");
#ifdef HAVE_NATIVE_MAKECONTEXT
    qassert(swapcontext(&qlib->mccoy_thread->rdata->context, &(qlib->master_context)), 0);
#else
    qassert(qt_swapctxt(&qlib->mccoy_thread->rdata->context, &(qlib->master_context)), 0);
#endif

#ifdef QTHREAD_RCRTOOL
    QTHREAD_FASTLOCK_INIT(rcrtool_lock);
    {
        char *qrcrtl  = getenv("QTHREAD_RCRTOOL_LEVEL");
        char *qrcrtle = NULL;

        qthread_debug(ALL_DETAILS, "qthread_init: RCRTool logging initialization.\n");
        if (qrcrtl) {
            rcrtoollevel = strtol(qrcrtl, &qrcrtle, 0);
            if ((qrcrtle == NULL) || (qrcrtle == qrcrtl)) {
                fprintf(stderr, "unparseable RCRTool logging level (%s)\n", qrcrtl);
                rcrtoollevel = 0;
            }
        } else {
            rcrtoollevel = 0;
        }
    }
#endif /* ifdef QTHREAD_RCRTOOL */

/* spawn the shepherds */
#ifdef QTHREAD_MULTITHREADED_SHEPHERDS
    for (i = 0; i < nshepherds; ++i) {
        qthread_worker_id_t j;
        qthread_debug(ALL_DETAILS,
                      "forking workers for shepherd %i (%p)\n", i,
                      &qlib->shepherds[i]);
        for (j = 0; j < nworkerspershep; ++j) {
            if ((i == 0) && (j == 0)) {
                continue;                       // original pthread becomes shep 0 worker 0
            }
            qlib->shepherds[i].workers[j].shepherd         = &qlib->shepherds[i];
            qlib->shepherds[i].workers[j].worker_id        = j;
            qlib->shepherds[i].workers[j].packed_worker_id = j + (i * nworkerspershep);
            qlib->shepherds[i].workers[j].active           = 1;
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
            if ((r = pthread_create(&qlib->shepherds[i].workers[j].worker, NULL,
                                    qthread_shepherd, &qlib->shepherds[i].workers[j])) != 0) {
                fprintf(stderr, "qthread_init: pthread_create() failed (%d)\n", r);
                perror("qthread_init spawning worker");
                return r;
            }
            qthread_debug(ALL_DETAILS, "spawned shep %i worker %i\n", (int)i, (int)j);
        }
    }
# ifdef QTHREAD_RCRTOOL
    if (rcrtoollevel > 0) {
        qlib->nworkers_active = nshepherds * nworkerspershep - 1;
    } else {
        qlib->nworkers_active = nshepherds * nworkerspershep;
    }
# else
    qlib->nworkers_active = nshepherds * nworkerspershep;
# endif
#else /* ifdef QTHREAD_MULTITHREADED_SHEPHERDS */
    for (i = 1; i < nshepherds; ++i) {
        qthread_debug(ALL_DETAILS,
                      "forking shepherd %i (%p)\n", i,
                      &qlib->shepherds[i]);
        if ((r = pthread_create(&qlib->shepherds[i].shepherd, NULL, qthread_shepherd, &qlib->shepherds[i])) != 0) {
            fprintf(stderr, "qthread_init: pthread_create() failed (%d)\n", r);
            perror("qthread_init spawning shepherd");
            return r;
        }
    }
#endif /* ifdef QTHREAD_MULTITHREADED_SHEPHERDS */

    qthread_debug(ALL_DETAILS, "calling atexit\n");
    atexit(qthread_finalize);

    qthread_debug(ALL_DETAILS, "calling component init functions\n");
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

    qthread_debug(ALL_DETAILS, "finished.\n");
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

/* this adds a function to the list of cleanup functions to call at finalize */
void qthread_internal_cleanup(void (*function)(void))
{   /*{{{*/
    struct qt_cleanup_funcs_s *ng = malloc(sizeof(struct qt_cleanup_funcs_s));

    assert(ng);
    ng->func         = function;
    ng->next         = qt_cleanup_funcs;
    qt_cleanup_funcs = ng;
} /*}}}*/

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

    if (qlib == NULL) {
        return;
    }

    qthread_shepherd_t *shep0 = &(qlib->shepherds[0]);

#ifdef QTHREAD_MULTITHREADED_SHEPHERDS
    worker = qthread_internal_getworker();
    if (worker->packed_worker_id != 0) {                       /* Only run finalize on shepherd 0 worker 0*/
        worker->current->thread_state = QTHREAD_STATE_YIELDED; /* Otherwise, put back */
        //      qt_threadqueue_enqueue(shep0->ready, worker->current,
        //             shep0);
        return;
    }
#endif

    qthread_debug(ALL_CALLS, "began.\n");

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
            qthread_debug(ALL_DETAILS, "terminating shepherd %i worker %i\n", (int)i, j);
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
        qthread_debug(ALL_DETAILS, "terminating shepherd %i\n", (int)i);
        t = qthread_thread_new(NULL, NULL, 0, NULL, i);
        assert(t != NULL);     /* what else can we do? */
        t->thread_state = QTHREAD_STATE_TERM_SHEP;
        t->thread_id    = (unsigned int)-1;
        qt_threadqueue_enqueue(qlib->shepherds[i].ready, t,
                               shep0);
    }
#endif /* ifdef QTHREAD_MULTITHREADED_SHEPHERDS */

#ifdef QTHREAD_USE_ROSE_EXTENSIONS
    qthread_debug(ALL_DETAILS, "destroying the global barrier\n");
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

    qthread_debug(ALL_DETAILS, "calling early cleanup functions\n");
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
        qthread_debug(ALL_DETAILS, "waiting for shepherd %i to exit\n", (int)i);
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
            qthread_debug(ALL_DETAILS, "000\n");
            if ((r = pthread_join(shep->workers[j].worker, NULL)) != 0) {
                fprintf(stderr,
                        "qthread_finalize: pthread_join() of shep %i worker %i failed (%d, or \"%s\")\n",
                        (int)i, (int)j, r, strerror(r));
                fflush(stderr);
                abort();
            }
        }
        if (i == 0) { continue; }
#else /* ifdef QTHREAD_MULTITHREADED_SHEPHERDS */
    for (i = 1; i < qlib->nshepherds; i++) {
        qthread_shepherd_t *shep = &(qlib->shepherds[i]);
        qthread_debug(ALL_DETAILS, "waiting for shepherd %i to exit\n", (int)i);
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
        qthread_debug(ALL_DETAILS, "destroying hashes\n");
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
    qthread_debug(ALL_DETAILS, "freeing shep0's threadqueue\n");
    qt_threadqueue_free(shep0->ready);

    qthread_debug(ALL_DETAILS, "calling cleanup functions\n");
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
        qthread_debug(ALL_DETAILS, "destroying lock infrastructure of shep %i\n", (int)i);
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
    qthread_debug(ALL_DETAILS, "destroy lock infrastructure arrays\n");
    free(qlib->locks);
    free(qlib->FEBs);
#if defined(QTHREAD_MUTEX_INCREMENT) || (QTHREAD_ASSEMBLY_ARCH == QTHREAD_POWERPC32)
    free(qlib->atomic_locks);
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

    qthread_debug(ALL_DETAILS, "destroy scheduling locks\n");
    QTHREAD_FASTLOCK_DESTROY(qlib->max_thread_id_lock);
    QTHREAD_FASTLOCK_DESTROY(qlib->sched_shepherd_lock);

#ifdef QTHREAD_USE_VALGRIND
    VALGRIND_STACK_DEREGISTER(qlib->mccoy_thread->valgrind_stack_id);
    VALGRIND_STACK_DEREGISTER(qlib->valgrind_masterstack_id);
#endif
    assert(qlib->mccoy_thread->rdata->stack == NULL);
    qthread_debug(ALL_DETAILS, "destroy mccoy thread structure\n");
    free(qlib->mccoy_thread->rdata);
    FREE_QTHREAD(qlib->mccoy_thread);
    qthread_debug(ALL_DETAILS, "destroy master stack\n");
    free(qlib->master_stack);
    for (i = 0; i < qlib->nshepherds; ++i) {
        qthread_debug(ALL_DETAILS, "destroy topology information on shep %i\n", (int)i);
        if (qlib->shepherds[i].shep_dists) {
            free(qlib->shepherds[i].shep_dists);
        }
        if (qlib->shepherds[i].sorted_sheplist) {
            free(qlib->shepherds[i].sorted_sheplist);
        }
    }
#ifndef UNPOOLED
    for (i = 0; i < qlib->nshepherds; ++i) {
        qthread_debug(ALL_DETAILS, "destroy shep %i qthread pool\n", (int)i);
        qt_mpool_destroy(qlib->shepherds[i].qthread_pool);
        qthread_debug(ALL_DETAILS, "destroy shep %i queue pool\n", (int)i);
        qt_mpool_destroy(qlib->shepherds[i].queue_pool);
        qthread_debug(ALL_DETAILS, "destroy shep %i threadqueue pools\n", (int)i);
        qt_threadqueue_destroy_pools(&qlib->shepherds[i].threadqueue_pools);
        qthread_debug(ALL_DETAILS, "destroy shep %i lock pool\n", (int)i);
        qt_mpool_destroy(qlib->shepherds[i].lock_pool);
        qthread_debug(ALL_DETAILS, "destroy shep %i addrres pool\n", (int)i);
        qt_mpool_destroy(qlib->shepherds[i].addrres_pool);
        qthread_debug(ALL_DETAILS, "destroy shep %i addrstat pool\n", (int)i);
        qt_mpool_destroy(qlib->shepherds[i].addrstat_pool);
        qthread_debug(ALL_DETAILS, "destroy shep %i stack pool\n", (int)i);
        qt_mpool_destroy(qlib->shepherds[i].stack_pool);
    }
    qthread_debug(ALL_DETAILS, "destroy global memory pools\n");
    qt_mpool_destroy(generic_qthread_pool);
    generic_qthread_pool = NULL;
    qt_mpool_destroy(generic_stack_pool);
    generic_stack_pool = NULL;
    qt_mpool_destroy(generic_queue_pool);
    generic_queue_pool = NULL;
    {
        extern qt_threadqueue_pools_t generic_threadqueue_pools;
        qt_threadqueue_destroy_pools(&generic_threadqueue_pools);
    }
    qt_mpool_destroy(generic_lock_pool);
    generic_lock_pool = NULL;
    qt_mpool_destroy(generic_addrstat_pool);
    generic_addrstat_pool = NULL;
#endif /* ifndef UNPOOLED */
    qthread_debug(ALL_DETAILS, "destroy global shepherd array\n");
    free(qlib->shepherds);
    qthread_debug(ALL_DETAILS, "destroy global data\n");
    free(qlib);
    qlib = NULL;
    qthread_debug(ALL_DETAILS, "destroy shepherd thread-local data\n");
    qassert(pthread_key_delete(shepherd_structs), 0);

    qthread_debug(ALL_DETAILS, "finished.\n");
}                      /*}}} */

#ifdef QTHREAD_MULTITHREADED_SHEPHERDS
void qthread_pack_workerid(const qthread_worker_id_t w,
                           const qthread_worker_id_t newId)
{
    int shep   = w % qlib->nshepherds;
    int worker = w / qlib->nshepherds;

    assert((shep < qlib->nshepherds));
    assert((worker < qlib->nworkerspershep));
    qlib->shepherds[shep].workers[worker].packed_worker_id = newId;
}

int qthread_disable_worker(const qthread_worker_id_t w)
{
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
    qthread_debug(ALL_CALLS, "began on worker(%i-%i)\n", shep, worker);

    (void)QT_CAS(qlib->shepherds[shep].workers[worker].active, 1, 0);
    qlib->nworkers_active--; // decrement active count

    if (worker == 0) { qthread_disable_shepherd(shep); }

    return QTHREAD_SUCCESS;
}

int qthread_enable_worker(const qthread_worker_id_t w)
{                      /*{{{ */
    int shep   = w % qlib->nshepherds;
    int worker = w / qlib->nshepherds;

    assert(shep < qlib->nshepherds);

    if (worker == 0) { qthread_enable_shepherd(shep); }
    qthread_debug(ALL_CALLS, "began on shep(%i)\n", shep);
    qthread_internal_incr(&(qlib->nshepherds_active), &(qlib->nshepherds_active_lock), 1);
    (void)QT_CAS(qlib->shepherds[shep].workers[worker].active, 0, 1);
    return QTHREAD_SUCCESS;
}                      /*}}} */

#endif /* ifdef QTHREAD_MULTITHREADED_SHEPHERDS */

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
    qthread_debug(ALL_CALLS, "began on shep(%i)\n", shep);
    qthread_internal_incr(&(qlib->nshepherds_active), &(qlib->nshepherds_active_lock), -1);
    (void)QT_CAS(qlib->shepherds[shep].active, 1, 0);
    return QTHREAD_SUCCESS;
}                      /*}}} */

void qthread_enable_shepherd(const qthread_shepherd_id_t shep)
{                      /*{{{ */
    assert(shep < qlib->nshepherds);
    qthread_debug(ALL_CALLS, "began on shep(%i)\n", shep);
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
static QINLINE qthread_t *qthread_thread_new(const qthread_f f,
                                             const void     *arg,
                                             size_t          arg_size,
                                             void           *ret,
                                             const qthread_shepherd_id_t
                                             shepherd)
{                      /*{{{ */
    qthread_t *t;

#ifndef UNPOOLED
    qthread_shepherd_t *myshep = &(qlib->shepherds[shepherd]);
#endif

    t = ALLOC_QTHREAD(myshep);
    qthread_debug(ALL_DETAILS, "t = %p\n", t);
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
#ifdef QTHREAD_OMP_AFFINITY
    t->child_affinity = OMP_NO_CHILD_TASK_AFFINITY;
#endif
    t->f     = f;
    t->arg   = (void *)arg;
    t->ret   = ret;
    t->rdata = NULL;
#ifdef QTHREAD_USE_ROSE_EXTENSIONS
    t->currentParallelRegion = NULL;
#endif
    // should I use the builtin block for args?
    t->free_arg = NO;
    if (arg_size > 0) {
        if (arg_size <= ARGCOPY_MAX) {
            t->arg = (void *)(&t->argcopy_data);
        } else {
            t->arg      = malloc(arg_size);
            t->free_arg = YES;
        }
        memcpy(t->arg, arg, arg_size);
    }

    qthread_debug(ALL_DETAILS, "returning\n");
    return t;
}                      /*}}} */

static QINLINE void qthread_thread_free(qthread_t *t)
{                      /*{{{ */
    assert(t != NULL);

    qthread_debug(ALL_FUNCTIONS, "t(%p): destroying thread id %i\n", t, t->thread_id);
    if (t->rdata != NULL) {
#ifdef QTHREAD_USE_VALGRIND
        VALGRIND_STACK_DEREGISTER(t->valgrind_stack_id);
#endif
        qthread_debug(ALL_DETAILS, "t(%p): releasing stack %p to %p\n", t, t->rdata->stack, t->creator_ptr);
        FREE_STACK(t->creator_ptr, t->rdata->stack);
    }
    qthread_debug(ALL_DETAILS, "t(%p): releasing thread handle %p\n", t, t);
    FREE_QTHREAD(t);
}                      /*}}} */

static QINLINE void qthread_enqueue(qthread_queue_t *q,
                                    qthread_t       *t)
{                      /*{{{ */
    assert(t != NULL);
    assert(q != NULL);
    assert(t->next == NULL);

    qthread_debug(ALL_FUNCTIONS, "q(%p), t(%p): started\n", q, t);

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

    qthread_debug(ALL_DETAILS, "q(%p), t(%p): finished\n", q, t);
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
            if (t->free_arg) {
                assert(&t->argcopy_data != t->arg);
                free(t->arg);
            }
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
        qthread_debug(ALL_DETAILS,
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
    qthread_debug(ALL_DETAILS,
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

    qthread_debug(ALL_DETAILS,
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
    qthread_debug(ALL_DETAILS,
                  "t(%p): setting stack size limits back to normal...\n",
                  t);
    if (!(t->flags & QTHREAD_REAL_MCCOY)) {
        rlp.rlim_cur = qlib->master_stack_size;
        qassert(setrlimit(RLIMIT_STACK, &rlp), 0);
    }
#endif

    assert(t != NULL);
    assert(c != NULL);

    qthread_debug(ALL_DETAILS, "t(%p): finished, t->thread_state = %i\n", t, t->thread_state);
}                      /*}}} */

/* this function yields thread t to the master kernel thread */
void qthread_yield(void)
{                      /*{{{ */
    qthread_t *t = qthread_internal_self();

    if (t != NULL) {
        qthread_debug(THREAD_DETAILS,
                      "tid %u yielding...\n", t->thread_id);
        t->thread_state = QTHREAD_STATE_YIELDED;
        qthread_back_to_master(t);
        qthread_debug(THREAD_DETAILS, "tid %u resumed.\n",
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
int qthread_fork(const qthread_f f,
                 const void     *arg,
                 aligned_t      *ret)
{                      /*{{{ */
    qthread_t            *t;
    qthread_shepherd_id_t shep;
    qthread_shepherd_t   *myshep = qthread_internal_getshep();

    qthread_debug(THREAD_BEHAVIOR, "f(%p), arg(%p), ret(%p)\n", f, arg, ret);
    assert(qlib);
#ifdef QTHREAD_MULTITHREADED_SHEPHERDS
    if (myshep) {
        shep = myshep->shepherd_id; // rely on work-stealing
    } else {
        shep = 0;
    }
#else
    unsigned int loopctr = 0;
    if (myshep) {
        do {
            shep = (qthread_shepherd_id_t)(myshep->sched_shepherd++);
            if (myshep->sched_shepherd == qlib->nshepherds) {
                myshep->sched_shepherd = 0;
            }
            loopctr++;
        } while (QTHREAD_CASLOCK_READ_UI(qlib->shepherds[shep].active) != 1 &&
                 loopctr <= qlib->nshepherds);
    } else {
        do {
            shep = (qthread_shepherd_id_t)qthread_internal_incr_mod(&qlib->sched_shepherd, qlib->nshepherds, &qlib->sched_shepherd_lock);
            loopctr++;
        } while (QTHREAD_CASLOCK_READ_UI(qlib->shepherds[shep].active) != 1 &&
                 loopctr <= qlib->nshepherds);
    }
    if (loopctr > qlib->nshepherds) {
        qthread_debug(THREAD_BEHAVIOR, "could not find an active shepherd\n");
        return QTHREAD_NOT_ALLOWED;
    }
#endif /* ifdef QTHREAD_MULTITHREADED_SHEPHERDS */
    t = qthread_thread_new(f, arg, 0, ret, shep);
    if (t) {
        qthread_debug(THREAD_BEHAVIOR, "new-tid %u shep %u\n",
                      t->thread_id, shep);

        if (ret) {
            int test = qthread_empty(ret);

            if (test != QTHREAD_SUCCESS) {
                qthread_thread_free(t);
                return test;
            }
        }
        qt_threadqueue_enqueue(qlib->shepherds[shep].ready, t, myshep);
        return QTHREAD_SUCCESS;
    }
    qthread_debug(THREAD_BEHAVIOR, "malloc error\n");
    return QTHREAD_MALLOC_ERROR;
}                      /*}}} */

int qthread_fork_syncvar_copyargs_to(const qthread_f             f,
                                     const void *const           arg,
                                     const size_t                arg_size,
                                     syncvar_t *const            ret,
                                     const qthread_shepherd_id_t preferred_shep)
{                      /*{{{ */
    qthread_t            *t;
    qthread_shepherd_id_t target_shep;
    qthread_shepherd_t   *myshep = qthread_internal_getshep();

    qthread_debug(THREAD_BEHAVIOR, "f(%p), arg(%p), ret(%p)\n", f, arg, ret);
    assert(qlib);
    assert(f);
#ifdef QTHREAD_MULTITHREADED_SHEPHERDS
    if (preferred_shep != NO_SHEPHERD) {
        target_shep = preferred_shep % qthread_num_shepherds();
    } else if (myshep != NULL) {
        target_shep = myshep->shepherd_id;
    } else {
        target_shep = 0;
    }
#else
    if (preferred_shep != NO_SHEPHERD) {
        target_shep = preferred_shep % qthread_num_shepherds();
    } else {
        unsigned int loopctr = 0;
        if (myshep) {          /* note: for forking from a qthread, NO LOCKS! */
            do {
                target_shep =
                    (qthread_shepherd_id_t)(myshep->sched_shepherd++);
                if (myshep->sched_shepherd == qlib->nshepherds) {
                    myshep->sched_shepherd = 0;
                }
                loopctr++;
            } while (QTHREAD_CASLOCK_READ_UI
                         (qlib->shepherds[target_shep].active) != 1 &&
                     loopctr <= qlib->nshepherds);
        } else {
            do {
                target_shep = (qthread_shepherd_id_t)
                              qthread_internal_incr_mod(&qlib->sched_shepherd,
                                                        qlib->nshepherds,
                                                        &qlib->sched_shepherd_lock);
                loopctr++;
            } while (QTHREAD_CASLOCK_READ_UI
                         (qlib->shepherds[target_shep].active) != 1 &&
                     loopctr <= qlib->nshepherds);
        }
        if (loopctr > qlib->nshepherds) {
            qthread_debug(THREAD_BEHAVIOR,
                          "could not find an active shepherd\n");
            return QTHREAD_NOT_ALLOWED;
        }
    }
#endif /* ifdef QTHREAD_MULTITHREADED_SHEPHERDS */
    t = qthread_thread_new(f, arg, arg_size, (aligned_t *)ret, target_shep);
    if (!t) {
        qthread_debug(THREAD_BEHAVIOR, "malloc error\n");
        return QTHREAD_MALLOC_ERROR;
    }
    qthread_debug(THREAD_BEHAVIOR, "new-tid %u shep %u\n", t->thread_id,
                  target_shep);
    t->flags |= QTHREAD_RET_IS_SYNCVAR;

#ifdef QTHREAD_USE_ROSE_EXTENSIONS
    qthread_t *me = qthread_internal_self();
    t->currentParallelRegion = me->currentParallelRegion; // saved in shepherd
#endif
    t->id = preferred_shep;  // used in barrier and arrive_first, NOT the thread-id
                             // may be extraneous in both when parallel region
                             // barriers in place (not will to pull it now
                             // maybe late) akp

    if (ret) {
        int test = qthread_syncvar_empty(ret);

        if (test != QTHREAD_SUCCESS) {
            qthread_thread_free(t);
            return test;
        }
    }
    qt_threadqueue_enqueue(qlib->shepherds[target_shep].ready, t, myshep);
    return QTHREAD_SUCCESS;
}                      /*}}} */

int qthread_fork_syncvar_copyargs(const qthread_f   f,
                                  const void *const arg,
                                  const size_t      arg_size,
                                  syncvar_t *const  ret)
{                      /*{{{ */
    return qthread_fork_syncvar_copyargs_to(f, arg, arg_size, ret,
                                            NO_SHEPHERD);
}                      /*}}} */

int qthread_fork_syncvar(const qthread_f   f,
                         const void *const arg,
                         syncvar_t *const  ret)
{                      /*{{{ */
    return qthread_fork_syncvar_copyargs(f, arg, 0, ret);
}                      /*}}} */

int qthread_fork_to(const qthread_f             f,
                    const void                 *arg,
                    aligned_t                  *ret,
                    const qthread_shepherd_id_t shepherd)
{                      /*{{{ */
    qthread_t          *t;
    qthread_shepherd_t *shep;

    assert(shepherd < qlib->nshepherds);
    assert(f != NULL);
    if ((shepherd >= qlib->nshepherds) || (f == NULL)) {
        return QTHREAD_BADARGS;
    }
    t = qthread_thread_new(f, arg, 0, ret, shepherd);
    qassert_ret(t, QTHREAD_MALLOC_ERROR);
    shep               = &(qlib->shepherds[shepherd]);
    t->target_shepherd = shep;
    t->flags          |= QTHREAD_UNSTEALABLE;
    qthread_debug(THREAD_BEHAVIOR,
                  "new-tid %u shep %u\n", t->thread_id,
                  shepherd);

    if (ret) {
        int test = qthread_empty(ret);

        if (test != QTHREAD_SUCCESS) {
            qthread_thread_free(t);
            return test;
        }
    }
    qt_threadqueue_enqueue(shep->ready, t,
                           qthread_internal_getshep());
    return QTHREAD_SUCCESS;
}                      /*}}} */

int qthread_fork_syncvar_to(const qthread_f             f,
                            const void *const           arg,
                            syncvar_t                  *ret,
                            const qthread_shepherd_id_t s)
{                      /*{{{ */
    qthread_t          *t;
    qthread_shepherd_t *shep;

    const qthread_shepherd_id_t shepherd = s % qthread_num_shepherds();

    if ((shepherd >= qlib->nshepherds) || (f == NULL)) {
        return QTHREAD_BADARGS;
    }
    assert(f != NULL);
    assert(shepherd < qlib->nshepherds);
    t = qthread_thread_new(f, arg, 0, ret, shepherd);
    qassert_ret(t, QTHREAD_MALLOC_ERROR);
    t->target_shepherd = shep = &(qlib->shepherds[shepherd]);
    t->flags          |= QTHREAD_UNSTEALABLE;
#ifdef QTHREAD_MULTITHREADED_SHEPHERDS
    t->id = s % (qthread_num_workers());
#else
    t->id = shepherd;
#endif
    qthread_debug(THREAD_BEHAVIOR, "new-tid %u shep %u\n", t->thread_id,
                  shepherd);

    if (ret) {
        int test = qthread_syncvar_empty(ret);

        if (test != QTHREAD_SUCCESS) {
            qthread_thread_free(t);
            return test;
        }
        t->flags |= QTHREAD_RET_IS_SYNCVAR;
    }
    if (QTHREAD_CASLOCK_READ_UI(shep->active) == 0) {
        shep =
            qthread_find_active_shepherd(shep->sorted_sheplist,
                                         shep->shep_dists);
    }
    qt_threadqueue_enqueue(shep->ready, t, qthread_internal_getshep());
    return QTHREAD_SUCCESS;
}                      /*}}} */

int qthread_fork_future_to(const qthread_f             f,
                           const void                 *arg,
                           aligned_t                  *ret,
                           const qthread_shepherd_id_t shepherd)
{                      /*{{{ */
    qthread_t          *t;
    qthread_shepherd_t *shep;

    assert(shepherd < qlib->nshepherds);
    assert(f != NULL);
    if ((shepherd >= qlib->nshepherds) || (f == NULL)) {
        return QTHREAD_BADARGS;
    }
    t = qthread_thread_new(f, arg, 0, ret, shepherd);
    qassert_ret(t, QTHREAD_MALLOC_ERROR);
    shep               = &(qlib->shepherds[shepherd]);
    t->flags          |= QTHREAD_FUTURE;
    t->target_shepherd = &(qlib->shepherds[shepherd]);
    qthread_debug(THREAD_BEHAVIOR,
                  "new-tid %u shep %u\n",
                  t->thread_id, shepherd);

    if (ret) {
        int test = qthread_empty(ret);

        if (test != QTHREAD_SUCCESS) {
            qthread_thread_free(t);
            return test;
        }
    }
    if (QTHREAD_CASLOCK_READ_UI(shep->active) == 0) {
        shep =
            qthread_find_active_shepherd(shep->sorted_sheplist,
                                         shep->shep_dists);
    }
    qt_threadqueue_enqueue(shep->ready, t, qthread_internal_getshep());
    return QTHREAD_SUCCESS;
}                      /*}}} */

int qthread_fork_syncvar_future_to(const qthread_f             f,
                                   const void                 *arg,
                                   syncvar_t                  *ret,
                                   const qthread_shepherd_id_t shepherd)
{                      /*{{{ */
    qthread_t          *t;
    qthread_shepherd_t *shep;

    assert(shepherd < qlib->nshepherds);
    assert(f != NULL);
    if ((shepherd >= qlib->nshepherds) || (f == NULL)) {
        return QTHREAD_BADARGS;
    }
    if (QTHREAD_CASLOCK_READ_UI(qlib->shepherds[shepherd].active) != 1) {
        return QTHREAD_NOT_ALLOWED;
    }
    t = qthread_thread_new(f, arg, 0, ret, shepherd);
    qassert_ret(t, QTHREAD_MALLOC_ERROR);
    shep               = &(qlib->shepherds[shepherd]);
    t->flags          |= QTHREAD_FUTURE;
    t->target_shepherd = &(qlib->shepherds[shepherd]);
    qthread_debug(THREAD_BEHAVIOR, "new-tid %u shep %u\n", t->thread_id,
                  shepherd);
    if (ret) {
        int test = qthread_syncvar_empty(ret);

        if (test != QTHREAD_SUCCESS) {
            qthread_thread_free(t);
            return test;
        }
        t->flags |= QTHREAD_RET_IS_SYNCVAR;
    }
    if (QTHREAD_CASLOCK_READ_UI(shep->active) == 0) {
        shep =
            qthread_find_active_shepherd(shep->sorted_sheplist,
                                         shep->shep_dists);
    }
    qt_threadqueue_enqueue(shep->ready, t, qthread_internal_getshep());
    return QTHREAD_SUCCESS;
}                      /*}}} */

int qthread_fork_syncvar_future(const qthread_f f,
                                const void     *arg,
                                syncvar_t      *ret)
{
    qthread_shepherd_id_t shep;
    qthread_shepherd_t   *myshep = qthread_internal_getshep();

#ifdef QTHREAD_MULTITHREADED_SHEPHERDS
    if (myshep) {
        shep = myshep->shepherd_id; // rely on work-stealing
    } else {
        shep = 0;
    }
#else
    unsigned int loopctr = 0;
    if (myshep) {              /* note: for forking from a qthread, NO LOCKS! */
        do {
            shep = (qthread_shepherd_id_t)(myshep->sched_shepherd++);
            if (myshep->sched_shepherd == qlib->nshepherds) {
                myshep->sched_shepherd = 0;
            }
            loopctr++;
        } while (QTHREAD_CASLOCK_READ_UI(qlib->shepherds[shep].active) != 1 &&
                 loopctr <= qlib->nshepherds);
    } else {
        do {
            shep = (qthread_shepherd_id_t)
                   qthread_internal_incr_mod(&qlib->sched_shepherd,
                                             qlib->nshepherds,
                                             &qlib->sched_shepherd_lock);
            loopctr++;
        } while (QTHREAD_CASLOCK_READ_UI(qlib->shepherds[shep].active) != 1 &&
                 loopctr <= qlib->nshepherds);
    }
    if (loopctr > qlib->nshepherds) {
        qthread_debug(THREAD_BEHAVIOR, "could not find an active shepherd\n");
        return QTHREAD_NOT_ALLOWED;
    }
#endif /* ifdef QTHREAD_MULTITHREADED_SHEPHERDS */
    return qthread_fork_syncvar_future_to(f, arg, ret, shep);
}

void INTERNAL qthread_back_to_master(qthread_t *t)
{                      /*{{{ */
#ifdef NEED_RLIMIT
    struct rlimit rlp;

    qthread_debug(ALL_DETAILS, "t(%p): setting stack size limits for master thread...\n", t);
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
    qthread_debug(ALL_DETAILS, "t(%p): setting stack size limits back to qthread size...\n", t);
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

    if (me->rdata->shepherd_ptr->shepherd_id == shepherd) {
        me->target_shepherd = me->rdata->shepherd_ptr;
        return QTHREAD_SUCCESS;
    }
    if (me->flags & QTHREAD_REAL_MCCOY) {
        return QTHREAD_NOT_ALLOWED;
    }
    if (me && (shepherd < qlib->nshepherds)) {
        qthread_debug(THREAD_BEHAVIOR,
                      "tid %u from shep %u to shep %u\n",
                      me->thread_id, me->rdata->shepherd_ptr->shepherd_id, shepherd);
        me->target_shepherd  = &(qlib->shepherds[shepherd]);
        me->thread_state     = QTHREAD_STATE_MIGRATING;
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
qt_barrier_t INTERNAL *qt_thread_barrier()            // get barrier active for this thread
# else
qt_feb_barrier_t INTERNAL * qt_thread_barrier()            // get barrier active for this thread
# endif
{                      /*{{{ */
    qthread_t *t = qthread_internal_self();

    return t->currentParallelRegion->barrier;
}                      /*}}} */

void INTERNAL qt_set_unstealable(void);
void INTERNAL qt_set_unstealable()
{                      /*{{{ */
    qthread_t *t = qthread_internal_self();

    t->flags |= QTHREAD_UNSTEALABLE;
}                      /*}}} */

/* These are just accessor functions */
qthread_parallel_region_t INTERNAL *qt_parallel_region() // get active parallel region
{                                                        /*{{{ */
    qthread_t *t = qthread_internal_self();

    return t->currentParallelRegion;
}                      /*}}} */

int INTERNAL qt_omp_parallel_region_create()
{                      /*{{{ */
    int                        ret = 0;
    qthread_parallel_region_t *pr  = malloc(sizeof(qthread_parallel_region_t));

    qassert_ret(pr, QTHREAD_MALLOC_ERROR);

# ifdef QTHREAD_LOG_BARRIER
#  ifdef QTHREAD_MULTITHREADED_SHEPHERDS
    qt_barrier_t *gb = qt_barrier_create(qthread_num_workers(), REGION_BARRIER, 0); // allocate barrier for region (workers)
#  else
    qt_barrier_t *gb = qt_barrier_create(qthread_num_shepherds(), REGION_BARRIER, 0); // allocate barrier for region (shepherds)
#  endif
# else
#  ifdef QTHREAD_MULTITHREADED_SHEPHERDS
    qt_feb_barrier_t *gb = qt_feb_barrier_create(qthread_num_workers()); // allocate barrier for region (workers)
#  else
    qt_feb_barrier_t *gb = qt_feb_barrier_create(qthread_num_shepherds()); // allocate barrier for region (sheperds)
#  endif
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

    return ret;
}                              /*}}} */

void INTERNAL qt_free_loop(void *lp);

void INTERNAL qt_move_to_orig()
{
    qthread_t *t = qthread_internal_self();

    t->flags |= QTHREAD_MUST_BE_WORKER_ZERO;
    qt_threadqueue_enqueue(qlib->shepherds[0].ready, t, 0); // put work back (having marked that
                                                            // it must be run by thread 0) -- put
                                                            // on queue that 0 looks at by default
    qthread_back_to_master(t);                              // return to work pile
}

void INTERNAL qt_omp_parallel_region_destroy()
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

    qthread_debug(ALL_CALLS, "tid(%u)\n",
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

unsigned INTERNAL qthread_barrier_id(void)
{                      /*{{{ */
    qthread_t *t = qthread_internal_self();

    qthread_debug(ALL_CALLS, "tid(%u)\n",
                  t ? t->id : (unsigned)-1);
    return t ? t->id : (unsigned int)-1;
}                      /*}}} */

qthread_shepherd_id_t qthread_shep(void)
{                      /*{{{ */
    qthread_shepherd_t *ret = qthread_internal_getshep();

    if (ret == NULL) {
        return NO_SHEPHERD;
    } else {
        return ret->shepherd_id;
    }
}                      /*}}} */

#ifdef QTHREAD_MULTITHREADED_SHEPHERDS
qthread_worker_id_t INTERNAL qthread_worker(qthread_shepherd_id_t *shepherd_id)
{                                      /*{{{ */
    qthread_worker_t *worker = (qthread_worker_t *)pthread_getspecific(shepherd_structs);

    if(shepherd_id != NULL) {
        *shepherd_id = worker->shepherd->shepherd_id;
    }
    return worker ? (worker->packed_worker_id) : NO_WORKER;
}                                      /*}}} */

#endif /* ifdef QTHREAD_MULTITHREADED_SHEPHERDS */

int qthread_shep_ok(void)
{                      /*{{{ */
    qthread_shepherd_t *ret = qthread_internal_getshep();

    if (ret == NULL) {
        return QTHREAD_PTHREAD_ERROR;
    } else {
        return QTHREAD_CASLOCK_READ_UI(ret->active);
    }
}                      /*}}} */

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

#ifdef QTHREAD_MULTITHREADED_SHEPHERDS
/* returns the number of shepherds actively scheduling work */
qthread_worker_id_t qthread_num_workers(void)
{                      /*{{{ */
    return (qthread_worker_id_t)(qlib->nworkers_active);
}                      /*}}} */

#endif

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

void INTERNAL qthread_getTaskListLock(void)
{                      /*{{{ */
    qthread_t *t = qthread_internal_self();

    assert(t);
    qthread_syncvar_writeEF_const(&t->rdata->taskWaitLock, 1);
}                      /*}}} */

void INTERNAL qthread_releaseTaskListLock(void)
{                      /*{{{ */
    qthread_t *t = qthread_internal_self();

    assert(t);
    qthread_syncvar_readFE(NULL, &t->rdata->taskWaitLock);
}                      /*}}} */

taskSyncvar_t INTERNAL *qthread_getTaskRetVar(void)
{                      /*{{{ */
    qthread_t *t = qthread_internal_self();

    assert(t);
    return t->rdata->openmpTaskRetVar;
}                      /*}}} */

void INTERNAL qthread_setTaskRetVar(taskSyncvar_t *v)
{                      /*{{{ */
    qthread_t *t = qthread_internal_self();

    assert(t);
    t->rdata->openmpTaskRetVar = v;
}                      /*}}} */

#endif /* ifdef QTHREAD_USE_ROSE_EXTENSIONS */

#if defined(QTHREAD_MUTEX_INCREMENT) || \
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
