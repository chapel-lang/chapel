//
// Qthreads implementation of Chapel tasking interface
//
// Copyright 2011 Sandia Corporation. Under the terms of Contract
// DE-AC04-94AL85000, there is a non-exclusive license for use of this work by
// or on behalf of the U.S. Government. Export of this program may require a
// license from the United States Government
//

// For SVID definitions (setenv)
#define _SVID_SOURCE

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include "chplrt.h"
#include "chplsys.h"
#include "tasks-qthreads.h"
#include "chplcgfns.h"
#include "chpl-comm.h"
#include "chpl-locale-model.h"
#include "chpl-tasks.h"
#include "config.h"
#include "error.h"
#include "arg.h"
#include "signal.h"
#include "chplexit.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <inttypes.h>
#include <errno.h>
#include <sys/time.h>
#include <unistd.h>
#include <sched.h>

#include "qthread/qthread.h"
#include "qthread/qtimer.h"
#include "qt_feb.h"
#include "qt_syncvar.h"
#include "qt_hash.h"
#include "qt_atomics.h"
#include "qt_shepherd_innards.h"
#include "qt_envariables.h"
#include "qt_debug.h"

#ifdef QTHREAD_MULTINODE
#include "qthread/spr.h"
#endif /* QTHREAD_MULTINODE */

#include <pthread.h>

#ifdef CHAPEL_PROFILE
# define PROFILE_INCR(counter,count) do { (void)qthread_incr(&counter,count); } while (0)

/* Tasks */
static aligned_t profile_task_yield = 0;
static aligned_t profile_task_addToTaskList = 0;
static aligned_t profile_task_processTaskList = 0;
static aligned_t profile_task_executeTasksInList = 0;
static aligned_t profile_task_freeTaskList = 0;
static aligned_t profile_task_startMovedTask = 0;
static aligned_t profile_task_getId = 0;
static aligned_t profile_task_sleep = 0;
static aligned_t profile_task_getSerial = 0;
static aligned_t profile_task_setSerial = 0;
static aligned_t profile_task_getCallStackSize = 0;
/* Sync */
static aligned_t profile_sync_lock= 0;
static aligned_t profile_sync_unlock= 0;
static aligned_t profile_sync_waitFullAndLock= 0;
static aligned_t profile_sync_waitEmptyAndLock= 0;
static aligned_t profile_sync_markAndSignalFull= 0;
static aligned_t profile_sync_markAndSignalEmpty= 0;
static aligned_t profile_sync_isFull= 0;
static aligned_t profile_sync_initAux= 0;
static aligned_t profile_sync_destroyAux= 0;

static void profile_print(void)
{
    /* Tasks */
    fprintf(stderr, "task yield: %lu\n", (unsigned long)profile_task_yield);
    fprintf(stderr, "task addToTaskList: %lu\n", (unsigned long)profile_task_addToTaskList);
    fprintf(stderr, "task processTaskList: %lu\n", (unsigned long)profile_task_processTaskList);
    fprintf(stderr, "task executeTasksInList: %lu\n", (unsigned long)profile_task_executeTasksInList);
    fprintf(stderr, "task freeTaskList: %lu\n", (unsigned long)profile_task_freeTaskList);
    fprintf(stderr, "task startMovedTask: %lu\n", (unsigned long)profile_task_startMovedTask);
    fprintf(stderr, "task getId: %lu\n", (unsigned long)profile_task_getId);
    fprintf(stderr, "task sleep: %lu\n", (unsigned long)profile_task_sleep);
    fprintf(stderr, "task getSerial: %lu\n", (unsigned long)profile_task_getSerial);
    fprintf(stderr, "task setSerial: %lu\n", (unsigned long)profile_task_setSerial);
    fprintf(stderr, "task getCallStackSize: %lu\n", (unsigned long)profile_task_getCallStackSize);
    /* Sync */
    fprintf(stderr, "sync lock: %lu\n", (unsigned long)profile_sync_lock);
    fprintf(stderr, "sync unlock: %lu\n", (unsigned long)profile_sync_unlock);
    fprintf(stderr, "sync waitFullAndLock: %lu\n", (unsigned long)profile_sync_waitFullAndLock);
    fprintf(stderr, "sync waitEmptyAndLock: %lu\n", (unsigned long)profile_sync_waitEmptyAndLock);
    fprintf(stderr, "sync markAndSignalFull: %lu\n", (unsigned long)profile_sync_markAndSignalFull);
    fprintf(stderr, "sync markAndSignalEmpty: %lu\n", (unsigned long)profile_sync_markAndSignalEmpty);
    fprintf(stderr, "sync isFull: %lu\n", (unsigned long)profile_sync_isFull);
    fprintf(stderr, "sync initAux: %lu\n", (unsigned long)profile_sync_initAux);
    fprintf(stderr, "sync destroyAux: %lu\n", (unsigned long)profile_sync_destroyAux);
}
#else
# define PROFILE_INCR(counter,count)
#endif /* CHAPEL_PROFILE */

#ifndef QTHREAD_MULTINODE
volatile int chpl_qthread_done_initializing;
#endif

// Make qt env sizes uniform. Same as qt, but they use the literal everywhere
#define QT_ENV_S 100

// aka chpl_task_list_p
struct chpl_task_list {
    chpl_fn_p        fun;
    void            *arg;
    c_string         filename;
    int              lineno;
    chpl_task_list_p next;
};

pthread_t chpl_qthread_comm_pthread;

chpl_qthread_tls_t chpl_qthread_comm_task_tls = {
  PRV_DATA_IMPL_VAL(c_sublocid_any_val, false),
  NULL, 0, NULL, 0 };
 
 //
 
//
// structs chpl_task_prvDataImpl_t, chpl_qthread_wrapper_args_t and
// chpl_qthread_tls_t have been moved to tasks-qthreads.h
//

//
// chpl_qthread_get_tasklocal() is in tasks-qthreads.h
//

static syncvar_t exit_ret = SYNCVAR_STATIC_EMPTY_INITIALIZER;

static volatile chpl_bool canCountRunningTasks = false;

void chpl_task_yield(void)
{
    PROFILE_INCR(profile_task_yield,1);
    if (qthread_shep() == NO_SHEPHERD) {
        sched_yield();
    } else {
        qthread_yield();
    }
}

// Sync variables
void chpl_sync_lock(chpl_sync_aux_t *s)
{
    aligned_t l;

    PROFILE_INCR(profile_sync_lock, 1);

    l = qthread_incr(&s->lockers_in, 1);

    // Yield if somebody else has the lock. If the current task can't get the
    // lock then it can't make progress until the lock is released, so let
    // somebody else do some work. It might be better to yield only after X
    // number of failures to acquire the lock for cases where the lock would be
    // released "soon", but we didn't see any performance impacts of doing it
    // every time.
    //
    // A core issue is that it's possible that a task scheduled on the same
    // worker could be the one responsible for unlocking the sync var. For
    // instance if a task locked the sync var, then does a chpl_task_yield, it
    // could now be scheduled to run after the current task so this task must
    // yield in order to prevent deadlock. However, if the task that will
    // unlock is on another worker, then a spinloop would result in lower
    // latency. That tension between deadlock avoidance and latency
    // minimization is what pushes us to even consider spinning in addition to
    // yielding, and to worry about how much of it we should do.
    //
    // If real qthreads sync vars were used, it's possible this wouldn't be
    // needed.
    while (l != s->lockers_out) qthread_yield();
}

void chpl_sync_unlock(chpl_sync_aux_t *s)
{
    PROFILE_INCR(profile_sync_unlock, 1);

    // Give other tasks that might be waiting on a sync var a chance to run.
    // Currently this is every 1024 unlocks. [x % 2n == x & (2n - 1)] This
    // number was chosen with trial and error and is a good balance of
    // not hurting performance and allowing progress. Determined by modifying
    // the value over several days and examining performance and correctness
    // testing.
    //
    // It's possible that two tasks can deadlock if they are both on the same
    // worker and the running task relies on the work the second task is
    // supposed to do to make progress. The first task will never yield
    // automatically, so this forces a yield on sync var unlock every so often
    // to allow progress.
    //
    // Consider this example:
    //
    // var alreadySet: [1..10] bool;
    // while (syncVar.readXX() != 10) {
    //   for i in 1..10 {
    //     if alreadySet[i] == false then
    //       begin syncVar += 1;
    //     alreadySet[i] = true;
    //   }
    // }
    //
    // Some of the incrementing tasks might get placed on the same worker as
    // the task waiting for the syncVar to be 10. That task will never yield,
    // so the value would never get set to 10 and we'd be stuck in the loop
    //
    // qthread_yield is used over chpl_task_yield. chpl_task_yield just adds
    // calling a sched yield if there are no shepherds. If we don't have any
    // shepherds we are either setting up or tearing down in which case the
    // pthread unlock will guarantee progress. When qthread_yield() is called
    // from a non-qthread it's just a no-op.
    //
    // I believe this is still necessary even if we used real qthread sync
    // vars. As the second task would never get a chance to try and acquire the
    // lock, so other tasks wouldn't know a task is waiting on the sync var.
    if ((qthread_incr(&s->lockers_out, 1) & 0x3FF) == 0x3FF) {
        qthread_yield();
    }
}

static inline void about_to_block(int32_t  lineno,
                                  c_string filename)
{
    chpl_qthread_tls_t * data = chpl_qthread_get_tasklocal();
    assert(data);

    data->lock_lineno   = lineno;
    data->lock_filename = filename;
}

void chpl_sync_waitFullAndLock(chpl_sync_aux_t *s,
                               int32_t          lineno,
                               c_string         filename)
{
    PROFILE_INCR(profile_sync_waitFullAndLock, 1);

    if (blockreport) { about_to_block(lineno, filename); }
    chpl_sync_lock(s);
    while (s->is_full == 0) {
        chpl_sync_unlock(s);
        qthread_syncvar_readFE(NULL, &(s->signal_full));
        chpl_sync_lock(s);
    }
}

void chpl_sync_waitEmptyAndLock(chpl_sync_aux_t *s,
                                int32_t          lineno,
                                c_string         filename)
{
    PROFILE_INCR(profile_sync_waitEmptyAndLock, 1);

    if (blockreport) { about_to_block(lineno, filename); }
    chpl_sync_lock(s);
    while (s->is_full != 0) {
        chpl_sync_unlock(s);
        qthread_syncvar_readFE(NULL, &(s->signal_empty));
        chpl_sync_lock(s);
    }
}

void chpl_sync_markAndSignalFull(chpl_sync_aux_t *s)         // and unlock
{
    PROFILE_INCR(profile_sync_markAndSignalFull, 1);

    qthread_syncvar_fill(&(s->signal_full));
    s->is_full = 1;
    chpl_sync_unlock(s);
}

void chpl_sync_markAndSignalEmpty(chpl_sync_aux_t *s)         // and unlock
{
    PROFILE_INCR(profile_sync_markAndSignalEmpty, 1);

    qthread_syncvar_fill(&(s->signal_empty));
    s->is_full = 0;
    chpl_sync_unlock(s);
}

chpl_bool chpl_sync_isFull(void            *val_ptr,
                           chpl_sync_aux_t *s)
{
    PROFILE_INCR(profile_sync_isFull, 1);

    return s->is_full;
}

void chpl_sync_initAux(chpl_sync_aux_t *s)
{
    PROFILE_INCR(profile_sync_initAux, 1);

    s->lockers_in   = 0;
    s->lockers_out  = 0;
    s->is_full      = 0;
    s->signal_empty = SYNCVAR_EMPTY_INITIALIZER;
    s->signal_full  = SYNCVAR_EMPTY_INITIALIZER;
}

void chpl_sync_destroyAux(chpl_sync_aux_t *s)
{
    PROFILE_INCR(profile_sync_destroyAux, 1);
}

static void chapel_display_thread(qt_key_t     addr,
                                  qthread_f    f,
                                  void        *arg,
                                  void        *retloc,
                                  unsigned int thread_id,
                                  void        *tls,
                                  void        *callarg)
{
    chpl_qthread_tls_t *rep = (chpl_qthread_tls_t *)tls;

    if (rep) {
        if ((rep->lock_lineno > 0) && rep->lock_filename) {
            fprintf(stderr, "Waiting at: %s:%zu (task %s:%zu)\n", rep->lock_filename, rep->lock_lineno, rep->task_filename, rep->task_lineno);
        } else if (rep->lock_lineno == 0 && rep->lock_filename) {
            fprintf(stderr, "Waiting for more work (line 0? file:%s) (task %s:%zu)\n", rep->lock_filename, rep->task_filename, rep->task_lineno);
        } else if (rep->lock_lineno == 0) {
            fprintf(stderr, "Waiting for dependencies (uninitialized task %s:%zu)\n", rep->task_filename, rep->task_lineno);
        }
        fflush(stderr);
    }
}

static void report_locked_threads(void)
{
    qthread_feb_callback(chapel_display_thread, NULL);
    qthread_syncvar_callback(chapel_display_thread, NULL);
}

static void SIGINT_handler(int sig)
{
    signal(sig, SIG_IGN);

    if (blockreport) {
        report_locked_threads();
    }

    if (taskreport) {
        fprintf(stderr, "Taskreport is currently unsupported by the qthreads tasking layer.\n");
        // report_all_tasks();
    }

    chpl_exit_any(1);
}

// Tasks

#ifndef QTHREAD_MULTINODE
static syncvar_t canexit            = SYNCVAR_STATIC_EMPTY_INITIALIZER;
static volatile int done_finalizing = 0;

static void *initializer(void *junk)
{
    qthread_initialize();
    MACHINE_FENCE;
    chpl_qthread_done_initializing = 1;

    qthread_syncvar_readFF(NULL, &canexit);

    qthread_finalize();
    MACHINE_FENCE;
    done_finalizing = 1;
    return NULL;
}
#endif /* ! QTHREAD_MULTINODE */

// Helper function to set a qthreads env var. This is meant to mirror setenv
// functionality, but qthreads has two environment variables for every setting:
// a QT_ and a QTHREAD_ version. We often forget to think about both so this
// wraps the overriding logic. In verbose mode it prints out if we overrode
// values, or if we were prevented from setting values because they existed
// (and override was 0.)
static void chpl_qt_setenv(char* var, char* val, int32_t override) {
    char      qt_env[QT_ENV_S]  = { 0 };
    char      qthread_env[QT_ENV_S] = { 0 };
    char      *qt_val;
    char      *qthread_val;
    chpl_bool eitherSet = false;

    strncpy(qt_env, "QT_", sizeof(qt_env));
    strncat(qt_env, var, sizeof(qt_env) - 1);

    strncpy(qthread_env, "QTHREAD_", sizeof(qthread_env));
    strncat(qthread_env, var, sizeof(qthread_env) - 1);

    qt_val = getenv(qt_env);
    qthread_val = getenv(qthread_env);
    eitherSet = (qt_val != NULL || qthread_val != NULL);

    if (override || !eitherSet) {
        if (verbosity >= 2 && override && eitherSet) {
            printf("QTHREADS: Overriding the value of %s and %s "
                   "with %s\n", qt_env, qthread_env, val);
        }
        (void) setenv(qt_env, val, 1);
        (void) setenv(qthread_env, val, 1);
    } else if (verbosity >= 2) {
        char* set_env = NULL;
        char* set_val = NULL;
        if (qt_val != NULL) {
            set_env = qt_env;
            set_val = qt_val;
        } else {
            set_env = qthread_env;
            set_val = qthread_val;
        }
        printf("QTHREADS: Not setting %s to %s because %s is set to %s and "
               "overriding was not requested\n", qt_env, val, set_env, set_val);
    }
}

// Determine the number of workers based on environment settings. If a user set
// HWPAR, they are saying they want to use HWPAR many workers, but let the
// runtime figure out the details. If they explicitly set NUM_SHEPHERDS and/or
// NUM_WORKERS_PER_SHEPHERD then they must have specific reasons for doing so.
// Returns 0 if no Qthreads env vars related to the number of threads were set,
// what HWPAR was set to if it was set, or -1 if NUM_SHEP and/or NUM_WPS were
// set since we can't figure out before Qthreads init what this will actually
// turn into without duplicating Qthreads logic (-1 is a sentinel for don't
// adjust the values, and give them as is to Qthreads.)
static int32_t chpl_qt_getenv_num_workers() {
    int32_t  hwpar;
    int32_t  num_wps;
    int32_t  num_sheps;

    hwpar = qt_internal_get_env_num("HWPAR", 0, 0);
    num_wps = qt_internal_get_env_num("NUM_WORKERS_PER_SHEPHERD", 0, 0);
    num_sheps = qt_internal_get_env_num("NUM_SHEPHERDS", 0, 0);

    if (hwpar) {
        return hwpar;
    } else if (num_wps || num_sheps) {
        return -1;
    }

    return 0;
}


// Sets up and returns the amount of hardware parallelism to use, limited to
// maxThreads. Returns -1 if we did not setup parallelism because a user
// explicitly requested a specific layout from qthreads.
static int32_t setupAvailableParallelism(int32_t maxThreads) {
    int32_t   numThreadsPerLocale;
    int32_t   qtEnvThreads;
    int32_t   hwpar;
    char      newenv_workers[QT_ENV_S] = { 0 };

    // Experience has shown that Qthreads generally performs best with
    // num_workers = numCores (and thus worker_unit = core) but if the user has
    // explicitly requested more threads through the chapel or Qthread env
    // vars, we override the default.
    numThreadsPerLocale = chpl_task_getenvNumThreadsPerLocale();
    qtEnvThreads = chpl_qt_getenv_num_workers();
    hwpar = 0;

    // User set chapel level env var (CHPL_RT_NUM_THREADS_PER_LOCALE)
    // This is limited to the number of logical CPUs on the node.
    if (numThreadsPerLocale != 0) {
        int32_t numPUsPerLocale;

        hwpar = numThreadsPerLocale;

        numPUsPerLocale = chpl_getNumLogicalCpus(true);
        if (0 < numPUsPerLocale && numPUsPerLocale < hwpar) {
            if (verbosity >= 2) {
                printf("QTHREADS: Reduced numThreadsPerLocale=%d to %d "
                       "to prevent oversubscription of the system.\n",
                       hwpar, numPUsPerLocale);
            }

            // Do not oversubscribe the system, use all available resources.
            hwpar = numPUsPerLocale;
        }
    }
    // User set qthreads level env var
    // (HWPAR or (NUM_SHEPHERDS and NUM_WORKERS_PER_SHEPHERD))
    else if (qtEnvThreads != 0) {
        hwpar = qtEnvThreads;
    }
    // User did not set chapel or qthreads vars -- our default
    else {
        hwpar = chpl_getNumPhysicalCpus(true);
    }

    // hwpar will only be <= 0 if the user set QT_NUM_SHEPHERDS and/or
    // QT_NUM_WORKERS_PER_SHEPHERD in which case we assume as "expert" user and
    // don't impose any thread limits or set worker_unit.
    if (hwpar > 0) {
        // Limit the parallelism to the maximum imposed by the comm layer.
        if (0 < maxThreads && maxThreads < hwpar) {
            hwpar = maxThreads;
        }

        // If there is more parallelism requested than the number of cores, set the
        // worker unit to pu, otherwise core.
        if (hwpar > chpl_getNumPhysicalCpus(true)) {
          chpl_qt_setenv("WORKER_UNIT", "pu", 0);
        } else {
          chpl_qt_setenv("WORKER_UNIT", "core", 0);
        }

        // Unset relevant Qthreads environment variables.
        qt_internal_unset_envstr("HWPAR");
        qt_internal_unset_envstr("NUM_SHEPHERDS");
        qt_internal_unset_envstr("NUM_WORKERS_PER_SHEPHERD");

        snprintf(newenv_workers, sizeof(newenv_workers), "%i", (int)hwpar);
        if (THREADQUEUE_POLICY_TRUE == qt_threadqueue_policy(SINGLE_WORKER)) {
            chpl_qt_setenv("NUM_SHEPHERDS", newenv_workers, 1);
            chpl_qt_setenv("NUM_WORKERS_PER_SHEPHERD", "1", 1);
        } else {
            chpl_qt_setenv("HWPAR", newenv_workers, 1);
        }
    }
    return hwpar;
}

static void setupCallStacks(int32_t hwpar) {
    size_t callStackSize;

    // If the user compiled with no stack checks (either explicitly or
    // implicitly) turn off qthread guard pages. TODO there should also be a
    // chpl level env var backing this at runtime (can be the same var.)
    if (CHPL_STACK_CHECKS == 0) {
        chpl_qt_setenv("GUARD_PAGES", "false", 0);
    }

    // Precedence (high-to-low):
    // 1) Chapel environment (CHPL_RT_CALL_STACK_SIZE)
    // 2) QTHREAD_STACK_SIZE
    // 3) Chapel default
    if ((callStackSize = chpl_task_getEnvCallStackSize()) > 0 ||
        (qt_internal_get_env_num("STACK_SIZE", 0, 0) == 0 &&
         (callStackSize = chpl_task_getDefaultCallStackSize()) > 0)) {
        char newenv_stack[QT_ENV_S];
        snprintf(newenv_stack, sizeof(newenv_stack), "%zu", callStackSize);
        chpl_qt_setenv("STACK_SIZE", newenv_stack, 1);

        // Qthreads sets up memory pools expecting the item_size to be small.
        // Stacks are allocated in this manner too, but our default stack size
        // is quite large, so we limit the max memory allocated for a pool. We
        // default to a multiple of callStackSize and hwpar, with the thought
        // that available memory is generally proportional to the amount of
        // parallelism. For some architectures, this isn't true so we set a max
        // upper bound. And if the callStackSize is small, we don't want to
        // limit all qthreads pool allocations to a small value, so we have a
        // lower bound as well. Note that qthread stacks are slightly larger
        // than specified to store a book keeping structure and possibly guard
        // pages, so we thrown an extra MB.
        if (hwpar > 0) {
            const size_t oneMB = 1024 * 1024;
            const size_t allocSizeLowerBound =  33 * oneMB;
            const size_t allocSizeUpperBound = 513 * oneMB;
            size_t maxPoolAllocSize;
            char newenv_alloc[QT_ENV_S];

            maxPoolAllocSize = 2 * hwpar * callStackSize + oneMB;
            if (maxPoolAllocSize < allocSizeLowerBound) {
                maxPoolAllocSize = allocSizeLowerBound;
            } else if (maxPoolAllocSize > allocSizeUpperBound) {
                maxPoolAllocSize = allocSizeUpperBound;
            }
            snprintf(newenv_alloc, sizeof(newenv_alloc), "%zu", maxPoolAllocSize);
            chpl_qt_setenv("MAX_POOL_ALLOC_SIZE", newenv_alloc, 0);
        }
    }
}

void chpl_task_init(void)
{
    int32_t   commMaxThreads;
    int32_t   hwpar;
    pthread_t initer;

    commMaxThreads = chpl_comm_getMaxThreads();

    // Setup hardware parallelism, the stack size, and stack guards
    hwpar = setupAvailableParallelism(commMaxThreads);
    setupCallStacks(hwpar);

    if (verbosity >= 2) { chpl_qt_setenv("INFO", "1", 0); }

    // Initialize qthreads
    pthread_create(&initer, NULL, initializer, NULL);
    while (chpl_qthread_done_initializing == 0) SPINLOCK_BODY();

    // Now that Qthreads is up and running, do a sanity check and make sure
    // that the number of workers is less than any comm layer limit. This is
    // mainly need for the case where a user set QT_NUM_SHEPHERDS and/or
    // QT_NUM_WORKERS_PER_SHEPHERD in which case we don't impose any limits on
    // the number of threads qthreads creates beforehand
    assert(0 == commMaxThreads || qthread_num_workers() < commMaxThreads);

    if (blockreport || taskreport) {
        if (signal(SIGINT, SIGINT_handler) == SIG_ERR) {
            perror("Could not register SIGINT handler");
        }
    }
}

void chpl_task_exit(void)
{
#ifdef CHAPEL_PROFILE
    profile_print();
#endif /* CHAPEL_PROFILE */

#ifdef QTHREAD_MULTINODE
#else
    if (qthread_shep() == NO_SHEPHERD) {
        /* sometimes, tasking is told to shutdown even though it hasn't been
         * told to start yet */
        if (chpl_qthread_done_initializing == 1) {
            qthread_syncvar_fill(&canexit);
            while (done_finalizing == 0) SPINLOCK_BODY();
        }
    } else {
        qthread_syncvar_fill(&exit_ret);
    }
#endif /* QTHREAD_MULTINODE */
}

static aligned_t chapel_wrapper(void *arg)
{
    chpl_qthread_wrapper_args_t *rarg = arg;
    chpl_qthread_tls_t * data = chpl_qthread_get_tasklocal();

    data->task_filename = rarg->task_filename;
    data->task_lineno = rarg->lineno;
    data->chpl_data = rarg->chpl_data;
    data->lock_filename = NULL;
    data->lock_lineno = 0;

    if (rarg->countRunning) {
        chpl_taskRunningCntInc(0, NULL);
    }

    (*(chpl_fn_p)(rarg->fn))(rarg->args);

    if (rarg->countRunning) {
        chpl_taskRunningCntDec(0, NULL);
    }

    return 0;
}

// Start the main task.
//
// Warning: this method is not called within a Qthread task context. Do
// not use methods that require task context (e.g., task-local storage).
void chpl_task_callMain(void (*chpl_main)(void))
{
    const chpl_qthread_wrapper_args_t wrapper_args = 
        {chpl_main, NULL, NULL, 0, false,
         PRV_DATA_IMPL_VAL(c_sublocid_any_val, false) };

    qthread_debug(CHAPEL_CALLS, "[%d] begin chpl_task_callMain()\n", chpl_nodeID);

#ifdef QTHREAD_MULTINODE
    qthread_debug(CHAPEL_BEHAVIOR, "[%d] calling spr_unify\n", chpl_nodeID);
    int const rc = spr_unify();
    assert(SPR_OK == rc);
#endif /* QTHREAD_MULTINODE */

    qthread_fork_syncvar(chapel_wrapper, &wrapper_args, &exit_ret);
    qthread_syncvar_readFF(NULL, &exit_ret);

    qthread_debug(CHAPEL_BEHAVIOR, "[%d] main task finished\n", chpl_nodeID);
    qthread_debug(CHAPEL_CALLS, "[%d] end chpl_task_callMain()\n", chpl_nodeID);
}

void chpl_task_stdModulesInitialized(void)
{
    //
    // It's not safe to call the module code to count the main task as
    // running until after the modules have been initialized.  That's
    // when this function is called, so now count the main task.
    //
    canCountRunningTasks = true;
    chpl_taskRunningCntInc(0, NULL);
}

int chpl_task_createCommTask(chpl_fn_p fn,
                             void     *arg)
{
#ifndef QTHREAD_MULTINODE
    return pthread_create(&chpl_qthread_comm_pthread,
                          NULL, (void *(*)(void *))fn, arg);
#else
    return 0;
#endif
}

void chpl_task_addToTaskList(chpl_fn_int_t     fid,
                             void             *arg,
                             c_sublocid_t      subloc,
                             chpl_task_list_p *task_list,
                             int32_t           task_list_locale,
                             chpl_bool         is_begin_stmt,
                             int               lineno,
                             c_string          filename)
{
    qthread_shepherd_id_t const here_shep_id = qthread_shep();
    chpl_bool serial_state = chpl_task_getSerial();
    chpl_qthread_wrapper_args_t wrapper_args = 
        {chpl_ftable[fid], arg, filename, lineno, false,
         PRV_DATA_IMPL_VAL(subloc, serial_state) };

    assert(subloc != c_sublocid_none);

    PROFILE_INCR(profile_task_addToTaskList,1);

    if (serial_state) {
        syncvar_t ret = SYNCVAR_STATIC_EMPTY_INITIALIZER;
        qthread_fork_syncvar_copyargs_to(chapel_wrapper, &wrapper_args,
                                         sizeof(chpl_qthread_wrapper_args_t), &ret,
                                         here_shep_id);
        qthread_syncvar_readFF(NULL, &ret);
    } else if (subloc == c_sublocid_any) {
        qthread_fork_copyargs(chapel_wrapper, &wrapper_args,
                              sizeof(chpl_qthread_wrapper_args_t), NULL);
    } else {
        qthread_fork_copyargs_to(chapel_wrapper, &wrapper_args,
                                 sizeof(chpl_qthread_wrapper_args_t), NULL,
                                 (qthread_shepherd_id_t) subloc);
    }
}

void chpl_task_processTaskList(chpl_task_list_p task_list)
{
    PROFILE_INCR(profile_task_processTaskList,1);
}

void chpl_task_executeTasksInList(chpl_task_list_p task_list)
{
    PROFILE_INCR(profile_task_executeTasksInList,1);
}

void chpl_task_freeTaskList(chpl_task_list_p task_list)
{
    PROFILE_INCR(profile_task_freeTaskList,1);
}

void chpl_task_startMovedTask(chpl_fn_p      fp,
                              void          *arg,
                              c_sublocid_t   subloc,
                              chpl_taskID_t  id,
                              chpl_bool      serial_state)
{
    assert(subloc != c_sublocid_none);
    assert(id == chpl_nullTaskID);

    chpl_qthread_wrapper_args_t wrapper_args = 
        {fp, arg, NULL, 0, canCountRunningTasks,
         PRV_DATA_IMPL_VAL(subloc, serial_state) };


    PROFILE_INCR(profile_task_startMovedTask,1);

    if (subloc == c_sublocid_any) {
        qthread_fork_copyargs(chapel_wrapper, &wrapper_args,
                              sizeof(chpl_qthread_wrapper_args_t), NULL);
    } else {
        qthread_fork_copyargs_to(chapel_wrapper, &wrapper_args,
                                 sizeof(chpl_qthread_wrapper_args_t), NULL,
                                 (qthread_shepherd_id_t) subloc);
    }
}

//
// chpl_task_getSubloc() is in tasks-qthreads.h
//

//
// chpl_task_setSubloc() is in tasks-qthreads.h
//

//
// chpl_task_getRequestedSubloc() is in tasks-qthreads.h
//


// Returns '(unsigned int)-1' if called outside of the tasking layer.
chpl_taskID_t chpl_task_getId(void)
{
    PROFILE_INCR(profile_task_getId,1);

    return (chpl_taskID_t)qthread_id();
}

void chpl_task_sleep(int secs)
{
    sleep(secs); // goes into the syscall interception system
}

/* The get- and setSerial() methods assume the beginning of the task-local
 * data segment holds a chpl_bool denoting the serial state. */
chpl_bool chpl_task_getSerial(void)
{
    chpl_qthread_tls_t * data = chpl_qthread_get_tasklocal();

    PROFILE_INCR(profile_task_getSerial,1);

    return data->chpl_data.prvdata.serial_state;
}

void chpl_task_setSerial(chpl_bool state)
{
    chpl_qthread_tls_t * data = chpl_qthread_get_tasklocal();
    data->chpl_data.prvdata.serial_state = state;

    PROFILE_INCR(profile_task_setSerial,1);
}

uint32_t chpl_task_getMaxPar(void) {
    //
    // We assume here that the caller (in the LocaleModel module code)
    // is interested in the number of workers on the whole node, and
    // will decide itself how much parallelism to create across and
    // within sublocales, if there are any.
    //
    return (uint32_t) qthread_num_workers();
}

c_sublocid_t chpl_task_getNumSublocales(void)
{
    // FIXME: What we really want here is the number of NUMA
    // sublocales we are supporting.  For now we use the number of
    // shepherds as a proxy for that.

#ifdef CHPL_LOCALE_MODEL_NUM_SUBLOCALES
#if CHPL_LOCALE_MODEL_NUM_SUBLOCALES < 0
#error Forced number of sublocales cannot be negative.
#endif
    if (CHPL_LOCALE_MODEL_NUM_SUBLOCALES == 0) {
        return 0;
    } else {
        c_sublocid_t num_sublocs = (c_sublocid_t) qthread_num_shepherds();
        return ((num_sublocs < CHPL_LOCALE_MODEL_NUM_SUBLOCALES)
                ? num_sublocs
                : CHPL_LOCALE_MODEL_NUM_SUBLOCALES);
    }
#else
    return (c_sublocid_t) qthread_num_shepherds();
#endif
}

size_t chpl_task_getCallStackSize(void)
{
    PROFILE_INCR(profile_task_getCallStackSize,1);

    return qthread_readstate(STACK_SIZE);
}

// XXX: Should probably reflect all shepherds
uint32_t chpl_task_getNumQueuedTasks(void)
{
    return qthread_readstate(NODE_BUSYNESS);
}

uint32_t chpl_task_getNumRunningTasks(void)
{
    chpl_internal_error("chpl_task_getNumRunningTasks() called");
    return 1;
}

int32_t chpl_task_getNumBlockedTasks(void)
{
    // This isn't accurate, but in the absence of better information
    // it's the best we can do.
    return 0;
}

// Threads

uint32_t chpl_task_getNumThreads(void)
{
    return (uint32_t)qthread_num_workers();
}

// Ew. Talk about excessive bookkeeping.
uint32_t chpl_task_getNumIdleThreads(void)
{
    return 0;
}

/* vim:set expandtab: */
