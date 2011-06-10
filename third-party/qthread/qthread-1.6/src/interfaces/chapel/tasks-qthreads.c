//
// Qthreads implementation of Chapel tasking interface
//
// Copyright 2011 Sandia Corporation. Under the terms of Contract
// DE-AC04-94AL85000, there is a non-exclusive license for use of this work by
// or on behalf of the U.S. Government. Export of this program may require a
// license from the United States Government
//

// For SVID definitions (putenv)
#define _SVID_SOURCE

#ifdef __OPTIMIZE__
// Turn assert() into a no op if the C compiler defines the macro above.
# define NDEBUG
#endif

#include "chplrt.h"
#include "tasks-qthreads.h"
#include "chpl-tasks.h"
#include "chpl_mem.h" // for chpl_malloc(), mandatory malloc() replacement
#include "config.h"   // for chpl_config_get_value()
#include "error.h"    // for chpl_warning()
#include <stdio.h>
#include <stdlib.h> // for putenv()
#include <assert.h>
#include <inttypes.h>
#include <errno.h>
#include <sys/time.h>
#include <unistd.h>
#include "qthread/qthread.h"
#include "qthread/qtimer.h"
#include "qthread_innards.h" // not strictly necessary (yet)

#include <pthread.h>

void chpl_task_yield(void)
{
    /* fprintf(stdout, "In qthread's yield" );*/
    qthread_yield();
}

// Sync variables
void chpl_sync_lock(chpl_sync_aux_t *s)
{
    qthread_syncvar_readFE(NULL, &(s->is_full));
}

void chpl_sync_unlock(chpl_sync_aux_t *s)
{
    qthread_syncvar_fill(&(s->is_full));
}

void chpl_sync_waitFullAndLock(chpl_sync_aux_t *s,
                               int32_t          lineno,
                               chpl_string      filename)
{
    uint64_t is_full;

    qthread_syncvar_readFE(&is_full, &(s->is_full));
    while (is_full == 0) {
        qthread_syncvar_fill(&(s->is_full));
        qthread_syncvar_readFE(NULL, &(s->signal_full));
        qthread_syncvar_readFE(&is_full, &(s->is_full));
    }
}

void chpl_sync_waitEmptyAndLock(chpl_sync_aux_t *s,
                                int32_t          lineno,
                                chpl_string      filename)
{
    uint64_t is_full;

    qthread_syncvar_readFE(&is_full, &(s->is_full));
    while (is_full != 0) {
        qthread_syncvar_fill(&(s->is_full));
        qthread_syncvar_readFE(NULL, &(s->signal_empty));
        qthread_syncvar_readFE(&is_full, &(s->is_full));
    }
}

void chpl_sync_markAndSignalFull(chpl_sync_aux_t *s)         // and unlock
{
    qthread_syncvar_fill(&(s->signal_full));
    qthread_syncvar_writeEF_const(&(s->is_full), 1);
}

void chpl_sync_markAndSignalEmpty(chpl_sync_aux_t *s)         // and unlock
{
    qthread_syncvar_fill(&(s->signal_empty));
    qthread_syncvar_writeEF_const(&(s->is_full), 0);
}

chpl_bool chpl_sync_isFull(void            *val_ptr,
                           chpl_sync_aux_t *s,
                           chpl_bool        simple_sync_var)
{
    return s->is_full.u.s.data;
}

void chpl_sync_initAux(chpl_sync_aux_t *s)
{
    s->is_full      = SYNCVAR_INITIALIZER;
    s->signal_empty = SYNCVAR_EMPTY_INITIALIZER;
    s->signal_full  = SYNCVAR_EMPTY_INITIALIZER;
}

void chpl_sync_destroyAux(chpl_sync_aux_t *s)
{ }

// Tasks

static volatile int done_initializing = 0;
static syncvar_t    canexit           = SYNCVAR_STATIC_EMPTY_INITIALIZER;
static volatile int done_finalizing   = 0;

static void *initializer(void *junk)
{
    qthread_initialize();
    COMPILER_FENCE;
    done_initializing = 1;

    qthread_syncvar_readFF(NULL, &canexit);

    // qthread_finalize();
    COMPILER_FENCE;
    done_finalizing = 1;
    return NULL;
}

void chpl_task_init(int32_t  maxThreadsPerLocale,
                    uint64_t callStackSize)
{
    //
    // If a value was specified for the call stack size config const, warn
    // the user that it's ignored on this system.
    //
    pthread_t initer;

    /*if (maxThreadsPerLocale != 0) {
     *  char newenv[100] = { 0 };
     *  snprintf(newenv, 99, "QTHREAD_NUM_SHEPHERDS=%i", (int)maxThreadsPerLocale);
     *  putenv(newenv);
     * }*/

    if (callStackSize != 0) {
        char newenv[100] = { 0 };
        snprintf(newenv, 99, "QTHREAD_STACK_SIZE=%lu", (unsigned long)callStackSize);
        putenv(newenv);
    }

    pthread_create(&initer, NULL, initializer, NULL);
    while (done_initializing == 0) ;
}

void chpl_task_exit(void)
{
    qthread_syncvar_fill(&canexit);
    while (done_finalizing == 0) ;
}

void chpl_task_callMain(void (*chpl_main)(void))
{
    chpl_main();
}

void chpl_task_perPthreadInit(void)
{}

void chpl_task_addToTaskList(chpl_fn_int_t     fid,
                             void             *arg,
                             chpl_task_list_p *task_list,
                             int32_t           task_list_locale,
                             chpl_bool         call_chpl_begin,
                             int               lineno,
                             chpl_string       filename)
{
    chpl_task_begin(chpl_ftable[fid], arg, false, false, NULL);
}

void chpl_task_processTaskList(chpl_task_list_p task_list) { }

void chpl_task_executeTasksInList(chpl_task_list_p task_list) { }

void chpl_task_freeTaskList(chpl_task_list_p task_list) { }

static aligned_t chapel_wrapper(void *arg)
{
    void **rarg = (void **)arg;

    (*(chpl_fn_p)(rarg[0]))(rarg[1]);

    return 0;
}

void chpl_task_begin(chpl_fn_p        fp,
                     void            *arg,
                     chpl_bool        ignore_serial,
                     chpl_bool        serial_state,
                     chpl_task_list_p task_list_entry)
{
    if (!ignore_serial && chpl_task_getSerial()) {
        (*fp)(arg);
    } else {
        // Will call the real begin statement function. Only purpose of this
        // thread is to wait on that function and coordinate the exiting
        // of the main Chapel thread.
        void *const wrapper_args[2] = { fp, arg };
        qthread_fork_syncvar_copyargs(chapel_wrapper, wrapper_args, sizeof(void *) * 2, NULL);
    }
}

chpl_taskID_t chpl_task_getId(void)
{
    return (chpl_taskID_t)qthread_id();
}

void chpl_task_sleep(int secs)
{
    qtimer_t t = qtimer_create();

    qtimer_start(t); // record begin-timestamp
    qthread_yield();
    qtimer_stop(t); // record wake-timestamp

    while (qtimer_secs(t) < secs) { // check difference
        qthread_yield();
        qtimer_stop(t); // record new wake-timestamp
    }
    qtimer_destroy(t);
}

/* This is for manipulating task-specific data (a single boolean value) */
chpl_bool chpl_task_getSerial(void)
{
    /*chpl_bool *p = NULL;
     * p = (chpl_bool*) mta_register_task_data(p);
     * if (p == NULL)
     * return false;
     * else {
     * mta_register_task_data(p); // Put back the value retrieved above.
     * return *p;
     * }*/
    return false;
}

// XXX: this implies task-specific data
void chpl_task_setSerial(chpl_bool state)
{
    /*chpl_bool *p = NULL;
     * p = (chpl_bool*) mta_register_task_data(p);
     * if (p == NULL)
     * p = (chpl_bool*) chpl_alloc(sizeof(chpl_bool), CHPL_RT_MD_SERIAL_FLAG, 0, 0);
     * if (p) {
     * *p = state;
     * mta_register_task_data(p);
     * } else
     * chpl_internal_error("out of memory while creating serial state");*/
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

// XXX: what does this mean?
int32_t chpl_task_getMaxThreads(void)
{
    return 0;
}

// XXX: what's the difference between this and the previous function?
int32_t chpl_task_getMaxThreadsLimit(void)
{
    return 0;
}

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
