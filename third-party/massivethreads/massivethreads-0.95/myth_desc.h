#ifndef MYTH_DESC_H_
#define MYTH_DESC_H_

#include <stdio.h>
#include <stddef.h>
#include <sys/mman.h>

#include "myth_context.h"
#include "myth_wsqueue.h"
#include "myth_internal_lock.h"

#include "myth_worker.h"

//A thread function
typedef void*(*myth_func_t)(void*);

//Thread status constants
typedef enum
{
	MYTH_STATUS_READY=0,//Executable
	MYTH_STATUS_BLOCKED=1,//Blocked
	MYTH_STATUS_FREE_READY=2,//Execution finished. But you have to obtain lock before release
	MYTH_STATUS_FREE_READY2=3,//Execution finished. Feel free to release
}myth_status_t;

//Thread descriptor
typedef struct myth_thread
{
	struct myth_thread* join_thread;//8//A thread which is waiting for this
	myth_func_t entry_func;
	void *result;//16//Return value
	myth_context context;//24//Context
	void *stack;//32//Pointer to stack
	struct myth_running_env* env;//40//Pointer to worker thread
	//struct myth_pickle *pickle_ptr;
	myth_internal_lock_t lock;//48//Lock
	myth_queue_data queue_data;//48//Data for runqueue
	volatile myth_status_t status;//56//Status
	uint8_t detached;//57
	uint8_t cancelled;//58
	uint8_t cancel_enabled;//59
#if defined MYTH_ENABLE_THREAD_ANNOTATION && defined MYTH_COLLECT_LOG
	char annotation_str[MYTH_THREAD_ANNOTATION_MAXLEN];
	int recycle_count;
#endif
#ifdef MYTH_DESC_REUSE_CHECK
	myth_internal_lock_t sanity_check;
#endif
	void *custom_data_ptr;
	int custom_data_size;
}__attribute__((aligned(CACHE_LINE_SIZE))) myth_thread,*myth_thread_t;

#ifndef MYTH_THREAD_OPTION_DEFINED
#define MYTH_THREAD_OPTION_DEFINED
typedef struct myth_thread_option{
	size_t stack_size;
	int switch_immediately;
	size_t custom_data_size;
	void *custom_data;
}myth_thread_option,*myth_thread_option_t;
#endif

static inline myth_thread_t myth_context_to_thread(myth_running_env_t env,myth_context_t ctx)
{
	if (ctx==&env->sched.context){return NULL;}//scheduler
	char *ctx_ptr=(char*)ctx;
	return (myth_thread_t)(&ctx_ptr[-offsetof(struct myth_thread,context)]);
}

static inline void myth_desc_join_set(myth_thread_t thread,myth_thread_t wait_thread)
{
	thread->join_thread=wait_thread;
}

static inline void myth_desc_set_not_runnable(myth_thread_t thread)
{
	thread->status=MYTH_STATUS_BLOCKED;
}

static inline int myth_desc_is_runnable(myth_thread_t thread)
{
	return thread->status==MYTH_STATUS_READY;
}

static inline int myth_desc_is_finished(myth_thread_t thread)
{
	return thread->status>=MYTH_STATUS_FREE_READY;
}

static inline void myth_desc_set_detached(myth_thread_t th)
{
	th->detached=1;
}

typedef struct myth_pickle
{
	struct myth_thread desc;
	size_t stack_size;
	char *stack;
}myth_pickle,*myth_pickle_t;

#endif /* MYTH_DESC_H_ */
