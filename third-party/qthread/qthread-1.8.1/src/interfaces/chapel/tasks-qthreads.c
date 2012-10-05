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

#ifdef __OPTIMIZE__
// Turn assert() into a no op if the C compiler defines the macro above.
# define NDEBUG
#endif

// XXX: Workaround for problems with "" escaping
#undef CHPL_TASKS_MODEL_H
#undef CHPL_THREADS_MODEL_H
#define CHPL_TASKS_MODEL_H   "tasks-qthreads.h"
#define CHPL_THREADS_MODEL_H "threads-none.h"

#include "tasks-qthreads.h"

// XXX: Workaround for problems with "" escaping
#undef CHPL_TASKS_MODEL_H
#undef CHPL_THREADS_MODEL_H
#define CHPL_TASKS_MODEL_H "tasks-qthreads.h"
#define CHPL_THREADS_MODEL_H "threads-none.h"

#include "chplrt.h"
#include "chplsys.h"
#include "chpl-tasks.h"
#include "config.h"   // for chpl_config_get_value()
#include "error.h"    // for chpl_warning()
#include "arg.h"      // for blockreport and taskreport
#include "chplexit.h" // for chpl_exit_any()
#include <stdio.h>
#include <stdlib.h> // for setenv()
#include <assert.h>
#include <inttypes.h>
#include <errno.h>
#include <sys/time.h>
#include <unistd.h>
#include "qthread/qthread.h"
#include "qthread/qtimer.h"
#include "qthread_innards.h" // not strictly necessary (yet)
#include "qt_internal_feb.h" // for blockreporting
#include "qt_atomics.h"      /* for SPINLOCK_BODY() */
#include "qt_envariables.h"

#include <pthread.h>

// aka chpl_task_list_p
struct chpl_task_list {
    chpl_fn_p        fun;
    void            *arg;
    chpl_string      filename;
    int              lineno;
    chpl_task_list_p next;
};

typedef struct task_info_s {
    chpl_string lock_filename;
    size_t      lock_lineno;
    const char *task_filename;
    size_t      task_lineno;
    chpl_bool   serial_state;
} task_info_t;

typedef struct {
    void       *fn;
    void       *args;
    chpl_string task_filename;
    int         lineno;
    chpl_bool   serial_state;
} chapel_wrapper_args_t;

// Default serial state is used outside of the tasking layer.
static chpl_bool default_serial_state = true;
static syncvar_t exit_ret             = SYNCVAR_STATIC_EMPTY_INITIALIZER;

void chpl_task_yield(void)
{
    qthread_yield();
}

// Sync variables
void chpl_sync_lock(chpl_sync_aux_t *s)
{
    aligned_t l;

    l = qthread_incr(&s->lockers_in, 1);
    while (l != s->lockers_out) SPINLOCK_BODY();
}

void chpl_sync_unlock(chpl_sync_aux_t *s)
{
    qthread_incr(&s->lockers_out, 1);
}

static inline void about_to_block(int32_t     lineno,
                                  chpl_string filename)
{
    task_info_t *data = (task_info_t *)qthread_get_tasklocal(sizeof(task_info_t));

    assert(data);
    data->lock_lineno   = lineno;
    data->lock_filename = filename;
}

void chpl_sync_waitFullAndLock(chpl_sync_aux_t *s,
                               int32_t          lineno,
                               chpl_string      filename)
{
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
    qthread_syncvar_fill(&(s->signal_full));
    s->is_full = 1;
    chpl_sync_unlock(s);
}

void chpl_sync_markAndSignalEmpty(chpl_sync_aux_t *s)         // and unlock
{
    qthread_syncvar_fill(&(s->signal_empty));
    s->is_full = 0;
    chpl_sync_unlock(s);
}

chpl_bool chpl_sync_isFull(void            *val_ptr,
                           chpl_sync_aux_t *s,
                           chpl_bool        simple_sync_var)
{
    if(simple_sync_var) {
        return qthread_syncvar_status(&(s->signal_full));
    } else {
        return s->is_full;
    }
}

void chpl_sync_initAux(chpl_sync_aux_t *s)
{
    s->lockers_in   = 0;
    s->lockers_out  = 0;
    s->is_full      = 0;
    s->signal_empty = SYNCVAR_EMPTY_INITIALIZER;
    s->signal_full  = SYNCVAR_EMPTY_INITIALIZER;
}

void chpl_sync_destroyAux(chpl_sync_aux_t *s)
{ }

static void chapel_display_thread(void        *addr,
                                  qthread_f    f,
                                  void        *arg,
                                  void        *retloc,
                                  unsigned int thread_id,
                                  void        *tls)
{
    task_info_t *rep = (task_info_t *)tls;

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
    qthread_print_FEB_callback(chapel_display_thread);
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

static int       done_initializing = 0;
static syncvar_t canexit           = SYNCVAR_STATIC_EMPTY_INITIALIZER;
static int       done_finalizing   = 0;

static void *initializer(void *junk)
{
    qthread_initialize();
    MACHINE_FENCE;
    done_initializing = 1;

    qthread_syncvar_readFF(NULL, &canexit);

    qthread_finalize();
    MACHINE_FENCE;
    done_finalizing = 1;
    return NULL;
}

void chpl_task_init(int32_t  numThreadsPerLocale,
                    int32_t  maxThreadsPerLocale,
                    int      numCommTasks,
                    uint64_t callStackSize)
{
    pthread_t initer;
    char      newenv_sheps[100] = { 0 };
    char      newenv_stack[100] = { 0 };

    // Set up available hardware parallelism
    if (0 < numThreadsPerLocale) {
        // We are assuming the user wants to constrain the hardware
        // resources used during this run of the application.

        // Unset relevant Qthreads environment variables
        qt_internal_unset_envstr("HWPAR");
        qt_internal_unset_envstr("NUM_SHEPHERDS");
        qt_internal_unset_envstr("NUM_WORKERS_PER_SHEPHERD");

        if (chpl_numCoresOnThisLocale() < numThreadsPerLocale) {
            // Do not oversubscribe the system, use all available resources.
            numThreadsPerLocale = chpl_numCoresOnThisLocale();

            if (2 == verbosity) {
                printf("QTHREADS: Ignored --numThreadsPerLocale=%d to prevent oversubsription of the system.\n", numThreadsPerLocale);
            }
        }

        // Set environment variable for Qthreads
        snprintf(newenv_sheps, 99, "%i", (int)numThreadsPerLocale);
        setenv("QT_HWPAR", newenv_sheps, 1);
    } else if (qt_internal_get_env_str("HWPAR", NULL) ||
               qt_internal_get_env_str("NUM_SHEPHERDS", NULL) ||
               qt_internal_get_env_str("NUM_WORKERS_PER_SHEPHERD", NULL)) {
        // Assume the user wants has manually set the desired Qthreads
        // environment variables.
    } else {
        // Default to using all hardware resources.
        numThreadsPerLocale = chpl_numCoresOnThisLocale();
        snprintf(newenv_sheps, 99, "%i", (int)numThreadsPerLocale);
        setenv("QT_HWPAR", newenv_sheps, 1);
    }

    // Precendence (high-to-low):
    // 1) --callStackSize option
    // 2) QTHREAD_STACK_SIZE
    // 3) Chapel default
    if (callStackSize != 0) {
        snprintf(newenv_stack, 99, "%lu", (unsigned long)callStackSize);
        setenv("QT_STACK_SIZE", newenv_stack, 1);
    } else if (qt_internal_get_env_str("STACK_SIZE", NULL) == NULL) {
        callStackSize = 32 * 1024 * sizeof(size_t);
        snprintf(newenv_stack, 99, "%lu", (unsigned long)callStackSize);
        setenv("QT_STACK_SIZE", newenv_stack, 1);
    }

    // Turn on informative Qthreads setting messages with Chapel's verbose flag
    if (verbosity == 2) {
        setenv("QT_INFO", "1", 1);
    }

    pthread_create(&initer, NULL, initializer, NULL);
    while (done_initializing == 0) SPINLOCK_BODY();

    if (blockreport || taskreport) {
        if (signal(SIGINT, SIGINT_handler) == SIG_ERR) {
            perror("Could not register SIGINT handler");
        }
    }
}

void chpl_task_exit(void)
{
    if (qthread_shep() == NO_SHEPHERD) {
        /* sometimes, tasking is told to shutdown even though it hasn't been
         * told to start yet */
        if (done_initializing == 1) {
            qthread_syncvar_fill(&canexit);
            while (done_finalizing == 0) SPINLOCK_BODY();
        }
    } else {
        qthread_syncvar_fill(&exit_ret);
    }
}

static aligned_t chapel_wrapper(void *arg)
{
    chapel_wrapper_args_t *rarg = arg;
    task_info_t           *data = (task_info_t *)qthread_get_tasklocal(sizeof(task_info_t));

    if (NULL != data) {
        data->serial_state = rarg->serial_state;
        data->task_filename = rarg->task_filename;
        data->task_lineno = rarg->lineno;
        data->lock_filename = NULL;
        data->lock_lineno = 0;
    } else {
        default_serial_state = rarg->serial_state;
    }

    (*(chpl_fn_p)(rarg->fn))(rarg->args);

    return 0;
}

void chpl_task_callMain(void (*chpl_main)(void))
{
    const chpl_bool serial_state = false;

    chpl_task_setSerial(serial_state);

    const chapel_wrapper_args_t wrapper_args = { chpl_main, NULL, NULL, 0, serial_state };

    qthread_fork_syncvar(chapel_wrapper, &wrapper_args, &exit_ret);
    qthread_syncvar_readFF(NULL, &exit_ret);
}

int chpl_task_createCommTask(chpl_fn_p fn,
                             void     *arg)
{
    pthread_t polling_thread;

    return pthread_create(&polling_thread, NULL, (void *(*)(void *))fn, arg);
}

void chpl_task_addToTaskList(chpl_fn_int_t     fid,
                             void             *arg,
                             chpl_task_list_p *task_list,
                             int32_t           task_list_locale,
                             chpl_bool         call_chpl_begin,
                             int               lineno,
                             chpl_string       filename)
{
    struct chpl_task_list tasklist = { chpl_ftable[fid], arg, filename, lineno, NULL };

    chpl_task_begin(chpl_ftable[fid], arg, false, chpl_task_getSerial(), &tasklist);
}

void chpl_task_processTaskList(chpl_task_list_p task_list) { }

void chpl_task_executeTasksInList(chpl_task_list_p task_list) { }

void chpl_task_freeTaskList(chpl_task_list_p task_list) { }

void chpl_task_begin(chpl_fn_p        fp,
                     void            *arg,
                     chpl_bool        ignore_serial,
                     chpl_bool        serial_state,
                     chpl_task_list_p ltask)
{
    chapel_wrapper_args_t wrapper_args = { fp, arg, NULL, 0, serial_state };

    if (ltask) {
        wrapper_args.task_filename = ltask->filename;
        wrapper_args.lineno        = ltask->lineno;
    }
    if (!ignore_serial && serial_state) {
        syncvar_t ret = SYNCVAR_STATIC_EMPTY_INITIALIZER;
        qthread_fork_syncvar_copyargs_to(chapel_wrapper, &wrapper_args,
                                         sizeof(chapel_wrapper_args_t), &ret,
                                         qthread_shep());
        qthread_syncvar_readFF(NULL, &ret);
    } else {
        // Will call the real begin statement function. Only purpose of this
        // thread is to wait on that function and coordinate the exiting
        // of the main Chapel thread.
        qthread_fork_syncvar_copyargs(chapel_wrapper, &wrapper_args,
                                      sizeof(chapel_wrapper_args_t), NULL);
    }
}

// Returns '(unsigned int)-1' if called outside of the tasking layer.
chpl_taskID_t chpl_task_getId(void)
{
    return (chpl_taskID_t)qthread_id();
}

void chpl_task_sleep(int secs)
{
    qtimer_t t = qtimer_create();

    qtimer_start(t);     // record begin-timestamp
    qthread_yield();
    qtimer_stop(t);     // record wake-timestamp

    while (qtimer_secs(t) < secs) {     // check difference
        qthread_yield();
        qtimer_stop(t);     // record new wake-timestamp
    }
    qtimer_destroy(t);
}

/* The get- and setSerial() methods assume the beginning of the task-local
 * data segment holds a chpl_bool denoting the serial state. */
chpl_bool chpl_task_getSerial(void)
{
    chpl_bool *state = (chpl_bool *)qthread_get_tasklocal(sizeof(chpl_bool));

    return state == NULL ? default_serial_state : *state;
}

void chpl_task_setSerial(chpl_bool state)
{
    task_info_t *data = (task_info_t *)qthread_get_tasklocal(sizeof(task_info_t));

    if (NULL != data) {
        data->serial_state = state;
    } else {
        default_serial_state = state;
    }
}

uint64_t chpl_task_getCallStackSize(void)
{
    return qthread_readstate(STACK_SIZE);
}

// XXX: Should probably reflect all shepherds
uint32_t chpl_task_getNumQueuedTasks(void)
{
    return qthread_readstate(BUSYNESS);
}

uint32_t chpl_task_getNumRunningTasks(void)
{
#ifdef QTHREAD_MULTITHREADED_SHEPHERDS
    return (uint32_t)qthread_num_workers();

#else
    return (uint32_t)qthread_num_shepherds();
#endif
}                                                         /* 1, i.e. this one */

// XXX: not sure what the correct value should be here!
int32_t chpl_task_getNumBlockedTasks(void)
{
    return -1;
}

// Threads

uint32_t chpl_task_getNumThreads(void)
{
#ifdef QTHREAD_MULTITHREADED_SHEPHERDS
    return (uint32_t)qthread_num_workers();

#else
    return (uint32_t)qthread_num_shepherds();
#endif
}

// Ew. Talk about excessive bookkeeping.
uint32_t chpl_task_getNumIdleThreads(void)
{
    return 0;
}

/* vim:set expandtab: */
