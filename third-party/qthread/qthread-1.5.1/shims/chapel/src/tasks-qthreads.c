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
#define NDEBUG
#endif

#include "chplrt.h"
#include "chpltasks.h"
#include "chpl_mem.h" // for chpl_malloc(), mandatory malloc() replacement
#include "config.h" // for chpl_config_get_value()
#include "error.h" // for chpl_warning()
#include <stdio.h>
#include <stdlib.h> // for putenv()
#include <assert.h>
#include <inttypes.h>
#include <errno.h>
#include <sys/time.h>
#include <unistd.h>
#include <qthread.h>
#include <qthread/qtimer.h>

// Sync variables
void chpl_sync_lock(
    chpl_sync_aux_t * s)
{
    qthread_syncvar_readFE(NULL, &(s->is_full));
}

void chpl_sync_unlock(
    chpl_sync_aux_t * s)
{
    qthread_syncvar_fill(&(s->is_full));
}

void chpl_sync_waitFullAndLock(
    chpl_sync_aux_t *s,
    int32_t lineno,
    chpl_string filename)
{
    uint64_t is_full;
    qthread_syncvar_readFE(&is_full, &(s->is_full));
    while (is_full == 0) {
	qthread_syncvar_fill(&(s->is_full));
	qthread_syncvar_readFE(NULL, &(s->signal_full));
	qthread_syncvar_readFE(&is_full, &(s->is_full));
    }
}

void chpl_sync_waitEmptyAndLock(
    chpl_sync_aux_t * s,
    int32_t lineno,
    chpl_string filename)
{
    uint64_t is_full;
    qthread_syncvar_readFE(&is_full, &(s->is_full));
    while (is_full != 0) {
	qthread_syncvar_fill(&(s->is_full));
	qthread_syncvar_readFE(NULL, &(s->signal_empty));
	qthread_syncvar_readFE(&is_full, &(s->is_full));
    }
}

void chpl_sync_markAndSignalFull(
    chpl_sync_aux_t *s)		// and unlock
{
    qthread_syncvar_fill(&(s->signal_full));
    qthread_syncvar_writeEF_const(&(s->is_full), 1);
}

void chpl_sync_markAndSignalEmpty(
    chpl_sync_aux_t *s)		// and unlock
{
    qthread_syncvar_fill(&(s->signal_empty));
    qthread_syncvar_writeEF_const(&(s->is_full), 0);
}

chpl_bool chpl_sync_isFull(
    void * val_ptr,
    chpl_sync_aux_t *s,
    chpl_bool simple_sync_var)
{
    return s->is_full.u.s.data;
}

void chpl_sync_initAux(
    chpl_sync_aux_t *s)
{
    s->is_full = SYNCVAR_INITIALIZER;
    s->signal_empty = SYNCVAR_EMPTY_INITIALIZER;
    s->signal_full = SYNCVAR_EMPTY_INITIALIZER;
}

void chpl_sync_destroyAux(
    chpl_sync_aux_t *s)
{ }


// Tasks

void chpl_task_init(int32_t maxThreadsPerLocale, uint64_t callStackSize)
{
    //
    // If a value was specified for the call stack size config const, warn
    // the user that it's ignored on this system.
    //

    if (maxThreadsPerLocale != 0) {
	char newenv[100] = {0};
	snprintf(newenv, 99, "QTHREAD_NUM_SHEPHERDS=%i", (int)maxThreadsPerLocale);
	putenv(newenv);
    }

    if (callStackSize != 0) {
	char newenv[100] = {0};
	snprintf(newenv, 99, "QTHREAD_STACK_SIZE=%lu", (unsigned long)callStackSize);
	putenv(newenv);
    }

    qthread_initialize();
}

void chpl_task_exit(
    void)
{
    qthread_finalize();
}

void chpl_task_callMain(void (*chpl_main)(void)) {
  chpl_main();
}

void chpl_task_perPthreadInit(void)
{
}

void chpl_task_addToTaskList(chpl_fn_int_t fid,
                             void* arg,
                             chpl_task_list_p *task_list,
                             int32_t task_list_locale,
                             chpl_bool call_chpl_begin,
                             int lineno,
                             chpl_string filename) {
  chpl_task_begin(chpl_ftable[fid], arg, false, false, NULL);
}

void chpl_task_processTaskList(chpl_task_list_p task_list) { }

void chpl_task_executeTasksInList(chpl_task_list_p task_list) { }

void chpl_task_freeTaskList(chpl_task_list_p task_list) { }

static aligned_t chapel_wrapper(
    void *arg)
{
    void **rarg = (void **)arg;

    //printf("calling chpl_fn_p (%p)\n", rarg[0]);
    (*(chpl_fn_p) (rarg[0])) (rarg[1]);
    //printf("returning from chpl_fn_p (%p)\n", rarg[0]);

    chpl_free(arg, __LINE__, __FILE__);

    return 0;
}

void chpl_task_begin(
    chpl_fn_p fp,
    void *arg,
    chpl_bool ignore_serial,
    chpl_bool serial_state,
    chpl_task_list_p task_list_entry)
{
    if (!ignore_serial && chpl_task_getSerial())
	(*fp) (arg);

    else {
	// Will call the real begin statement function. Only purpose of this
	// thread is to wait on that function and coordinate the exiting
	// of the main Chapel thread.
	void **wrapper_args = chpl_malloc(2, sizeof(void *), 0, __LINE__, __FILE__);
	wrapper_args[0] = fp;
	wrapper_args[1] = arg;
	//printf("spawning to call %p\n", fp);
	qthread_fork(chapel_wrapper, wrapper_args, NULL);
    }
}

chpl_taskID_t chpl_task_getId(
    void)
{
    return (chpl_taskID_t) qthread_id();
}

void chpl_task_sleep(int secs) {
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

/* This is for manipulating thread-specific data (a single boolean value) */
chpl_bool chpl_task_getSerial(void) {
  /*chpl_bool *p = NULL;
  p = (chpl_bool*) mta_register_task_data(p);
  if (p == NULL)
    return false;
  else {
    mta_register_task_data(p); // Put back the value retrieved above.
    return *p;
  }*/
    return false;
}

void chpl_task_setSerial(chpl_bool state) {
  /*chpl_bool *p = NULL;
  p = (chpl_bool*) mta_register_task_data(p);
  if (p == NULL)
    p = (chpl_bool*) chpl_alloc(sizeof(chpl_bool), CHPL_RT_MD_SERIAL_FLAG, 0, 0);
  if (p) {
    *p = state;
    mta_register_task_data(p);
  } else
    chpl_internal_error("out of memory while creating serial state");*/
}

uint64_t chpl_task_getCallStackSize(void) { return qthread_readstate(STACK_SIZE); }

// XXX: Should probably reflect all shepherds
uint32_t chpl_task_getNumQueuedTasks(void) { return qthread_readstate(BUSYNESS); }

// XXX: Should this reflect all shepherds?
uint32_t chpl_task_getNumRunningTasks(void) { return 1; } /* 1, i.e. this one */

// not sure what the correct value should be here!
int32_t  chpl_task_getNumBlockedTasks(void) { return -1; }


// Threads

int32_t chpl_task_getMaxThreads(void) {
    return 0;
}

int32_t chpl_task_getMaxThreadsLimit(void) {
    return 0;
}

// This bookkeeping would be too expensive, so let's not and say we did.
uint32_t chpl_task_getNumThreads(void) { return 1; }

// There seems to be some different notion of threads here
uint32_t chpl_task_getNumIdleThreads(void) { return 0; }
