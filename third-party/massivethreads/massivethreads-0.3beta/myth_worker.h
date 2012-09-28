#ifndef MYTH_WORKER_H_
#define MYTH_WORKER_H_

#include <pthread.h>
#include <time.h>

#include "myth_sched.h"
#include "myth_io.h"
#include "myth_log.h"
#include "myth_wsqueue.h"

//A structure describing an environment for executing a thread
//(scheduler, worker thread, runqueue, etc...)
//Each worker thread have one of them

typedef struct myth_running_env
{
	//The following entries are only accessed from the owner
	struct myth_thread *this_thread;//Currently executing thread
#ifdef MYTH_SPLIT_STACK_DESC
	myth_freelist_t freelist_desc;//Freelist of thread descriptor
	myth_freelist_t freelist_stack;//Freelist of stack
#else
	myth_freelist_t freelist_ds;//Freelis
#endif
	int log_buf_size;
	int log_count;
	myth_internal_lock_t log_lock;
	struct myth_log_entry *log_data;
	struct myth_prof_data prof_data;
	pid_t tid;//an ID of the worker thread
	struct myth_sched sched;	//Scheduler descriptor
	//The following entries may be read from other worker threads
	pthread_t worker;
	int rank;
	//The following entries may be written by other worker threads
	//Appropriate synchronization is required
	myth_thread_queue runnable_q;//Runqueue
	//Reference to Global free list
#ifdef MYTH_SPLIT_STACK_DESC
	myth_freelist_t *freelist_desc_g;//Freelist of thread descriptor
	myth_freelist_t *freelist_stack_g;//Freelist of stack
#endif
	struct myth_io_struct_perenv io_struct;//I/O-related data structure. See myth_io_struct.h
	int exit_flag;
	//-1:Main thread, must not be terminated at the scheduling loop
	//0:Currently application is running
	//1:Application is terminated. Worker thread should exit scheduling loop and terminate itself
}__attribute__((aligned(CACHE_LINE_SIZE))) myth_running_env,*myth_running_env_t;

typedef struct myth_thread* (*myth_steal_func_t)(int);
extern myth_steal_func_t g_myth_steal_func;

//Thread index
extern int g_thread_index;
extern myth_running_env_t g_envs;
//Number of worker threads
extern int g_worker_thread_num;
//Barrier for worker threads
extern pthread_barrier_t g_worker_barrier;

#endif /* MYTH_WORKER_H_ */
