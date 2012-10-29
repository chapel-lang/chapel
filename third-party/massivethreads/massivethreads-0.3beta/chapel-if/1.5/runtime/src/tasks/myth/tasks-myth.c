/**************************************************************************
  Copyright (c) 2004-2011, Cray Inc.  (See LICENSE file for more details)
**************************************************************************/

#define _GNU_SOURCE

#ifdef __OPTIMIZE__
// Turn assert() into a no op if the C compiler defines the macro above.
#define NDEBUG
#endif

#include "chpl-mem.h"
#include "chpl-comm.h"
#include "chplcast.h"
#include "chplrt.h"
#include "chpl-tasks.h"
#include "config.h"
#include "error.h"
#include <assert.h>
#include <stdint.h>
#include <sys/resource.h>
#include <unistd.h>
#include <sched.h>
#include <stdio.h>
#include <dlfcn.h>

#include <unistd.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <stdio.h>

#include "myth.h"

typedef union{
	int flag;
	char size[128];
}thread_local_flag;

static int tasking_layer_active=0;
static int worker_in_cs_beforeinit=0;
static thread_local_flag* worker_in_cs;

static int is_worker_in_cs(void)
{
	if (tasking_layer_active){
		int rank=myth_get_worker_num();
		return worker_in_cs[rank].flag;
	}
	else return worker_in_cs_beforeinit;
}

static void worker_enter_cs(void)
{
	assert(!is_worker_in_cs());
	if (tasking_layer_active){
		int rank=myth_get_worker_num();
		worker_in_cs[rank].flag=1;
	}
	else worker_in_cs_beforeinit=1;
}

static void worker_exit_cs(void)
{
	assert(is_worker_in_cs());
	if (tasking_layer_active){
		int rank=myth_get_worker_num();
		worker_in_cs[rank].flag=0;
	}
	else worker_in_cs_beforeinit=0;
}

// Sync variables
void chpl_sync_lock(chpl_sync_aux_t *s)
{
	//Simple mutex lock
	assert(!is_worker_in_cs());
	myth_felock_lock(s->lock);
}
void chpl_sync_unlock(chpl_sync_aux_t *s)
{
	assert(!is_worker_in_cs());
	//Simple mutex unlock
	myth_felock_unlock(s->lock);
}

void chpl_sync_waitFullAndLock(chpl_sync_aux_t *s,
                                  int32_t lineno, chpl_string filename)
{
	assert(!is_worker_in_cs());
	//wait until F/E bit is empty, and acquire lock
	myth_felock_wait_lock(s->lock,1);
}

void chpl_sync_waitEmptyAndLock(chpl_sync_aux_t *s,
                                   int32_t lineno, chpl_string filename)
{
	assert(!is_worker_in_cs());
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

static cpu_set_t worker_cpusets[CPU_SETSIZE];
static int available_cores=-1;

static void get_process_affinity_info(void)
{
	cpu_set_t cset;
	sched_getaffinity(getpid(),sizeof(cpu_set_t),&cset);
	int i;
	for (i=0;i<CPU_SETSIZE;i++){
		CPU_ZERO(&worker_cpusets[i]);
	}
	available_cores=0;
	for (i=0;i<CPU_SETSIZE;i++){
		if (CPU_ISSET(i,&cset)){
			CPU_SET(i,&worker_cpusets[available_cores]);
			available_cores++;
		}
	}
}

//Return the number of CPU cores
static int get_cpu_num(void)
{
	assert(available_cores>0);
	return available_cores;
}

static int32_t s_num_workers;
static uint64_t s_stack_size;

// Tasks
void chpl_task_init(int32_t numThreadsPerLocale, int32_t maxThreadsPerLocale,
                    int numCommTasks, uint64_t callStackSize)
{
	//Initialize tasking layer
	//numThreadsPerLocale and callStackSize is specified or 0(default)
	//initializing change the number of workers
	char *env;
	int n_workers;
	int i;
	s_num_workers=numThreadsPerLocale;
	s_stack_size=callStackSize;
	assert(!is_worker_in_cs());
	get_process_affinity_info();
	env=getenv("MYTH_WORKER_NUM");
	n_workers=(int)((numThreadsPerLocale>0)?numThreadsPerLocale:-1);
	if (n_workers<=0 && env){n_workers=atoi(env);}
	if (n_workers<=0){n_workers=get_cpu_num();}
	worker_in_cs=chpl_mem_allocMany(n_workers+numCommTasks, sizeof(thread_local_flag), 0, 0, "");
	for (i=0;i<n_workers+numCommTasks;i++){worker_in_cs[i].flag=0;}
	tasking_layer_active=1;
	myth_init_withparam((int)(n_workers+numCommTasks),(size_t)callStackSize);
}

int chpl_task_createCommTask(chpl_fn_p fn, void* arg) {
	const size_t stacksize_for_comm_task=8*1024*1024;
	myth_thread_option opt;
	myth_thread_t th;
	//chpl_fn_p is defined as "typedef void (*chpl_fn_p)(void*);" in chpltypes.h at line 85.
	//Since return value is always ignored, this cast is legal unless the definition is changed.
	opt.stack_size=stacksize_for_comm_task;
	opt.switch_immediately=0;
	th=myth_create_ex((void*(*)(void*))fn,arg,&opt);
	assert(th);
	myth_detach(th);
}

void chpl_task_perPthreadInit(void)
{
}

void chpl_task_exit(void)
{
	//Cleanup tasking layer
	assert(!is_worker_in_cs());
	myth_fini();
	tasking_layer_active=0;
	chpl_mem_free(worker_in_cs,0,"");
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
	//Create a new task directly
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
	//Create one task
	myth_thread_t th;
	//chpl_fn_p is defined as "typedef void (*chpl_fn_p)(void*);" in chpltypes.h at line 85.
	//So this cast is legal unless the definition is changed.
	if (is_worker_in_cs()){
		//Called from critical section by pthreads.
		myth_thread_option opt;
		opt.stack_size=0;
		opt.switch_immediately=0;
		th=myth_create_ex((void*(*)(void*))fp,a,&opt);
	}
	else{
		th=myth_create((void*(*)(void*))fp,a);
	}
	assert(th);
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
	myth_yield(1);
}

void chpl_task_sleep(int secs) {
	//sleep specified seconds
	sleep(secs);
}

//FIXME: serial state ignored
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
	return s_stack_size;
}


uint32_t chpl_task_getNumQueuedTasks(void)
{
	//return the number of queued tasks
	return 0;
}

uint32_t chpl_task_getNumRunningTasks(void)
{
	//return the number of running tasks
	return 0;
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
	return s_num_workers;
}

int32_t chpl_task_getMaxThreadsLimit(void)
{
	//return the limit of max threads
	return s_num_workers;
}

uint32_t chpl_task_getNumThreads(void)
{
	//return the number of threads (excluding a thread for comm)
	return s_num_workers;
}

uint32_t chpl_task_getNumIdleThreads(void)
{
	//return the number of idle threads
	return 0;
}

#include <pthread.h>

static int (*pthread_mutex_lock_fp) (pthread_mutex_t *)=NULL;
static int (*pthread_mutex_unlock_fp) (pthread_mutex_t *)=NULL;

int pthread_mutex_lock (pthread_mutex_t *mutex)
{
	worker_enter_cs();
	if (!pthread_mutex_lock_fp){
		pthread_mutex_lock_fp=dlsym(RTLD_NEXT,"pthread_mutex_lock");
		assert(pthread_mutex_lock_fp);
	}
	pthread_mutex_lock_fp(mutex);
	return 0;
}

int pthread_mutex_unlock (pthread_mutex_t *mutex)
{
	if (!pthread_mutex_unlock_fp){
		pthread_mutex_unlock_fp=dlsym(RTLD_NEXT,"pthread_mutex_unlock");
		assert(pthread_mutex_unlock_fp);
	}
	pthread_mutex_unlock_fp(mutex);
	worker_exit_cs();
	return 0;
}

