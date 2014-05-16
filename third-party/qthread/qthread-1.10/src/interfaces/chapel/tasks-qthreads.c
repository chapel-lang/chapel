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
#include "chplcgfns.h" // for chpl_ftable()
#include "chpl-comm.h" // for chpl_localeID
#include "chpl-locale-model.h" // for sublocale information
#include "chpl-tasks.h"
#include "config.h"   // for chpl_config_get_value()
#include "error.h"    // for chpl_warning()
#include "arg.h"      // for blockreport and taskreport
#include "signal.h"   // for signal
#include "chplexit.h" // for chpl_exit_any()
#include <stdio.h>
#include <stdlib.h> // for setenv()
#include <assert.h>
#include <inttypes.h>
#include <errno.h>
#include <sys/time.h>
#include <unistd.h>
#include <sched.h>

#include "qthread/qthread.h"
#include "qthread/qtimer.h"
#include "qt_feb.h" // for blockreporting
#include "qt_syncvar.h" // for blockreporting
#include "qt_hash.h" /* for qt_key_t */
#include "qt_atomics.h"      /* for SPINLOCK_BODY() */
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

// aka chpl_task_list_p
struct chpl_task_list {
    chpl_fn_p        fun;
    void            *arg;
    chpl_string      filename;
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
    while (l != s->lockers_out) SPINLOCK_BODY();
}

void chpl_sync_unlock(chpl_sync_aux_t *s)
{
    PROFILE_INCR(profile_sync_unlock, 1);

    qthread_incr(&s->lockers_out, 1);
}

static inline void about_to_block(int32_t     lineno,
                                  chpl_string filename)
{
    chpl_qthread_tls_t * data = chpl_qthread_get_tasklocal();
    assert(data);

    data->lock_lineno   = lineno;
    data->lock_filename = filename;
}

void chpl_sync_waitFullAndLock(chpl_sync_aux_t *s,
                               int32_t          lineno,
                               chpl_string      filename)
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
                                chpl_string      filename)
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
            fprintf(stderr, "Waiting at: %s:%d (task %s:%d)\n", rep->lock_filename, rep->lock_lineno, rep->task_filename, rep->task_lineno);
        } else if (rep->lock_lineno == 0 && rep->lock_filename) {
            fprintf(stderr, "Waiting for more work (line 0? file:%s) (task %s:%d)\n", rep->lock_filename, rep->task_filename, rep->task_lineno);
        } else if (rep->lock_lineno == 0) {
            fprintf(stderr, "Waiting for dependencies (uninitialized task %s:%d)\n", rep->task_filename, rep->task_lineno);
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

void chpl_task_init(void)
{
    int32_t   numThreadsPerLocale;
    int32_t   commMaxThreads;
    int32_t   hwpar;
    size_t    callStackSize;
    pthread_t initer;
    char      newenv_stack[100] = { 0 };

    // Set up available hardware parallelism.

    // Experience has shown that we hardly ever win by using more than
    // one PU per core, so default to that.  If this was explicitly
    // set by the user we won't override it, however.
    (void) setenv("QT_WORKER_UNIT", "core", 0);

    // Determine the thread count.  CHPL_RT_NUM_THREADS_PER_LOCALE has
    // the highest precedence but we limit it to the number of PUs.
    // QTHREAD_HWPAR has the next precedence.  We don't impose the
    // same limit on it, so it can be used to overload the hardware.
    // In either case the number of threads can be no greater than any
    // maximum imposed by the comm layer.  This limit is imposed
    // silently.
    numThreadsPerLocale = chpl_task_getenvNumThreadsPerLocale();
    commMaxThreads = chpl_comm_getMaxThreads();
    hwpar = 0;
    if (numThreadsPerLocale != 0) {
        int32_t numPUsPerLocale;

	hwpar = numThreadsPerLocale;

        numPUsPerLocale = chpl_numCoresOnThisLocale();
        if (0 < numPUsPerLocale && numPUsPerLocale < hwpar) {
            if (2 == verbosity) {
                printf("QTHREADS: Reduced numThreadsPerLocale=%d to %d "
                       "to prevent oversubscription of the system.\n",
                       hwpar, numPUsPerLocale);
            }

            // Do not oversubscribe the system, use all available resources.
            hwpar = numPUsPerLocale;
        }

        if (0 < commMaxThreads && commMaxThreads < hwpar) {
	    hwpar = commMaxThreads;
	}
    } else {
	if (0 < commMaxThreads) {
	    hwpar = qt_internal_get_env_num("HWPAR", 0, 0);
	    if (commMaxThreads < hwpar) {
		hwpar = commMaxThreads;
	    }
	}
    }

    if (hwpar > 0) {
	char newenv[100];

        // Unset relevant Qthreads environment variables.  Currently
        // QTHREAD_HWPAR has precedence over the QTHREAD_NUM_* ones,
        // but that isn't documented and may not be true forever, so
        // we unset them all.
        qt_internal_unset_envstr("HWPAR");
        qt_internal_unset_envstr("NUM_SHEPHERDS");
        qt_internal_unset_envstr("NUM_WORKERS_PER_SHEPHERD");

        // Set environment variable for Qthreads
        snprintf(newenv, sizeof(newenv), "%i", (int)hwpar);
        setenv("QT_HWPAR", newenv, 1);
    }

    // Precedence (high-to-low):
    // 1) Chapel minimum
    // 2) QTHREAD_STACK_SIZE
    // In practice we never get to #2, because the Chapel minimum is
    // always > 0, but we cover that case as a backstop.
    callStackSize = chpl_task_getMinCallStackSize();
    if (callStackSize <= 0)
        callStackSize = 1024 * 1024 * sizeof(size_t);
    snprintf(newenv_stack, 99, "%zu", callStackSize);
    setenv("QT_STACK_SIZE", newenv_stack, 1);

    // Turn on informative Qthreads setting messages with Chapel's verbose flag
    if (verbosity == 2) {
        setenv("QT_INFO", "1", 1);
    }

    pthread_create(&initer, NULL, initializer, NULL);
    while (chpl_qthread_done_initializing == 0) SPINLOCK_BODY();

    // Now that Qthreads is up and running, make sure that the number
    // of workers is less than any comm layer limit.  This is mainly
    // checking that the default thread count without QTHREAD_HWPAR
    // being set is within any comm layer limit, because we can't
    // determine that default ahead of time.  Secondarily, it's a
    // sanity check on the thread count versus comm limit logic
    // above.
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

    qthread_debug(CHAPEL_CALLS, "[%d] begin chpl_task_callMain()\n", chpl_localeID);

#ifdef QTHREAD_MULTINODE
    qthread_debug(CHAPEL_BEHAVIOR, "[%d] calling spr_unify\n", chpl_localeID);
    int const rc = spr_unify();
    assert(SPR_OK == rc);
#endif /* QTHREAD_MULTINODE */

    qthread_fork_syncvar(chapel_wrapper, &wrapper_args, &exit_ret);
    qthread_syncvar_readFF(NULL, &exit_ret);

    qthread_debug(CHAPEL_BEHAVIOR, "[%d] main task finished\n", chpl_localeID);
    qthread_debug(CHAPEL_CALLS, "[%d] end chpl_task_callMain()\n", chpl_localeID);
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
                             chpl_string       filename)
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
