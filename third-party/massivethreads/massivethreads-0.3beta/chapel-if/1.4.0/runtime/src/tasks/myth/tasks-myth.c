/**************************************************************************
  Copyright (c) 2004-2011, Cray Inc.  (See LICENSE file for more details)
**************************************************************************/

#define _GNU_SOURCE

#ifdef __OPTIMIZE__
// Turn assert() into a no op if the C compiler defines the macro above.
#define NDEBUG
#endif

#include "chpl-mem.h"
#include "chplcast.h"
#include "chplrt.h"
#include "chpl-tasks.h"
#include "config.h"
#include "error.h"
#include <assert.h>
#include <stdint.h>
#include <sys/resource.h>
#include <unistd.h>
#include <stdio.h>

#include <dlfcn.h>

#include "myth.h"

static chpl_bool launch_next_task(void);

// Sync variables

void chpl_sync_lock(chpl_sync_aux_t *s)
{
	//Simple mutex lock
	myth_felock_lock(s->lock);
}
void chpl_sync_unlock(chpl_sync_aux_t *s)
{
	//Simple mutex unlock
	myth_felock_unlock(s->lock);
}

void chpl_sync_waitFullAndLock(chpl_sync_aux_t *s,
                                  int32_t lineno, chpl_string filename)
{
	//wait until F/E bit is empty, and acquire lock
	myth_felock_wait_lock(s->lock,1);
}

void chpl_sync_waitEmptyAndLock(chpl_sync_aux_t *s,
                                   int32_t lineno, chpl_string filename)
{
	//wait until F/E bit is empty, and acquire lock
	myth_felock_wait_lock(s->lock,0);
}

void chpl_sync_markAndSignalFull(chpl_sync_aux_t *s)
{
	//release lock and set F/E bit to full
	myth_felock_set_unlock(s->lock,1);
}

void chpl_sync_markAndSignalEmpty(chpl_sync_aux_t *s) {
	//release lock and set F/E bit to empty
	myth_felock_set_unlock(s->lock,0);
}

chpl_bool chpl_sync_isFull(void *val_ptr, chpl_sync_aux_t *s,
                            chpl_bool simple_sync_var)
{
	//return whether F/E bit is full or not
	return myth_felock_status(s->lock);
}

void chpl_sync_initAux(chpl_sync_aux_t *s)
{
	//init sync variable
	s->lock=myth_felock_create();
}

void chpl_sync_destroyAux(chpl_sync_aux_t *s)
{
	//destroy sync variable
	myth_felock_destroy(s->lock);
}

// Tasks

void chpl_task_init(int32_t numThreadsPerLocale, int32_t maxThreadsPerLocale,
                    int numCommTasks, uint64_t callStackSize)
{
	//Initialize tasking layer
	//numThreadsPerLocale and callStackSize is specified or 0(default)
	//TODO:change the number of workers
	//Set stacksize
	if (callStackSize>0){
		myth_set_def_stack_size(callStackSize);
	}
}

int chpl_task_createCommTask(chpl_fn_p fn, void* arg) {
	const size_t stacksize_for_comm_task=128*1024;
	myth_thread_option opt;
	myth_thread_t th;
	//chpl_fn_p is defined as "typedef void (*chpl_fn_p)(void*);" in chpltypes.h at line 85.
	//So this cast is legal unless the definition is changed.
	opt.stack_size=stacksize_for_comm_task;
	th=myth_create_ex((void*(*)(void*))fn,arg,&opt);
	if (th)
		myth_detach(th);
}

void chpl_task_perPthreadInit(void)
{
}

void chpl_task_exit(void)
{
	//Cleanup tasking layer
}

void chpl_task_callMain(void (*chpl_main)(void))
{
	//Call main function
	chpl_main();
}

void chpl_task_addToTaskList(chpl_fn_int_t fid,
                           void* arg,
                           chpl_task_list_p *task_list,
                           int32_t task_list_locale,
                           chpl_bool call_chpl_begin,
                           int lineno,
                           chpl_string filename) {
	//Fork a new task directly
	chpl_task_begin(chpl_ftable[fid], arg, false, false, NULL);
}

void chpl_task_processTaskList(chpl_task_list_p task_list)
{
	//Nothing to do because chpl_task_list is not used.
}

void chpl_task_executeTasksInList(chpl_task_list_p task_list)
{
	//Nothing to do because chpl_task_list is not used.
}

void chpl_task_freeTaskList(chpl_task_list_p task_list)
{
	//Nothing to do because chpl_task_list is not used.
}

void chpl_task_begin(chpl_fn_p fp, void* a, chpl_bool ignore_serial,
                chpl_bool serial_state, chpl_task_list_p task_list_entry)
{
	//Fork one task
	myth_thread_t th;
	//chpl_fn_p is defined as "typedef void (*chpl_fn_p)(void*);" in chpltypes.h at line 85.
	//So this cast is legal unless the definition is changed.
	th=myth_create((void*(*)(void*))fp,a);
	if (th)
		myth_detach(th);
}

chpl_taskID_t chpl_task_getId(void)
{
	//get task private ID
	return (chpl_taskID_t)myth_self();
}

void chpl_task_yield(void)
{
	//yield execution to other tasks
	myth_yield();
}

void chpl_task_sleep(int secs) {
	//sleep specified seconds
	sleep(secs);
}

static chpl_bool serial_state;

chpl_bool chpl_task_getSerial(void)
{
	//get dynamic serial state
	return serial_state;
}

void chpl_task_setSerial(chpl_bool new_state)
{
	//set dynamic serial state
	serial_state=new_state;
}


uint64_t chpl_task_getCallStackSize(void)
{
	//return call stack size
	return 16384;
}


uint32_t chpl_task_getNumQueuedTasks(void)
{
	//return the number of queued tasks
	return 0;
}

uint32_t chpl_task_getNumRunningTasks(void)
{
	//return the number of running tasks
	return 1;
}

int32_t  chpl_task_getNumBlockedTasks(void)
{
	//return the number of blocked tasks
	return 0;
}

// Threads
int32_t chpl_task_getMaxThreads(void)
{
	//return max threads
	return 1;
}

int32_t chpl_task_getMaxThreadsLimit(void)
{
	//return the limit of max threads
	return 1;
}

uint32_t chpl_task_getNumThreads(void)
{
	//return the number of threads
	return 1;
}

uint32_t chpl_task_getNumIdleThreads(void)
{
	//return the number of idle threads
	return 0;
}
