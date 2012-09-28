#ifndef MYTH_WORKER_FUNC_H
#define MYTH_WORKER_FUNC_H

#include <signal.h>
#include <sys/time.h>

#include "myth_worker.h"
#include "myth_malloc_wrapper.h"

#include "myth_sched.h"
#include "myth_sched_proto.h"
#include "myth_sched_func.h"

//#include "myth_log_proto.h"
#include "myth_log_func.h"

#include "myth_init.h"
#include "myth_io.h"

#include "myth_io_func.h"


static void myth_sched_loop(void);

//TLS implementations
#ifdef TLS_BY_PTHREAD
//TLS by pthread_key_XXX
extern pthread_key_t g_env_key;
static inline void myth_env_init(void)//Initialize
{(*real_pthread_key_create)(&g_env_key,NULL);}
static inline void myth_env_fini(void)//Cleanup
{(*real_pthread_key_delete)(g_env_key);}
static inline void myth_set_current_env(myth_running_env_t e)//Set worker thread descriptor
{(*real_pthread_setspecific)(g_env_key,(void*)e);}
static inline myth_running_env_t myth_get_current_env(void)//Return current worker thread descriptor
{return (myth_running_env_t)(*real_pthread_getspecific)(g_env_key);}
#elif defined TLS_BY_ELF
//TLS provided by ELF(Architecture dependent)
extern __thread int g_worker_rank;
static inline void myth_env_init(void)//Initialize
{}
static inline void myth_env_fini(void)//Cleanup
{}
static inline void myth_set_current_env(myth_running_env_t e)//Set worker thread descriptor
{g_worker_rank=e->rank;}
static inline myth_running_env_t myth_get_current_env(void)//Return current worker thread descriptor
{return &g_envs[g_worker_rank];}
#elif defined TLS_NONE
//Just a global variable. It works one worker thread only.
static inline void myth_env_init(void)//Initialize
{}
static inline void myth_env_fini(void)//Cleanup
{}
static inline void myth_set_current_env(myth_running_env_t e)//Set worker thread descriptor
{}
static inline myth_running_env_t myth_get_current_env(void)//Return current worker thread descriptor
{return &g_envs[0];}
#else
#error
#endif

#ifdef WS_TARGET_RANDOM
//Search a worker thread that seems to be busy
static inline myth_running_env_t myth_env_get_first_busy(myth_running_env_t e)
{
	//If number of worker threads == 1 , always fails
	if (g_worker_thread_num==1)return NULL;
	//Choose randomly
	int idx;
	do{
		idx=myth_random(0,g_worker_thread_num);
	}while (idx==e->rank);
	return &g_envs[idx];
}
#else

//Search a worker thread that seems to be busy
static inline myth_running_env_t myth_env_get_first_busy(myth_running_env_t e)
{
	return e->next;
}
#endif

static inline myth_running_env_t myth_env_get_randomly(void)
{
	//If number of worker threads == 1 , always fails
	if (g_worker_thread_num==1)return &g_envs[0];
	//Choose randomly
	int idx;
	idx=myth_random(0,g_worker_thread_num);
	return &g_envs[idx];
}

void myth_alrm_sighandler(int signum,siginfo_t *sinfo,void* ctx);

static inline void myth_setup_worker(int rank)
{
	pid_t tid;
	//Get thread ID(for debug)
	tid=syscall(SYS_gettid);
	myth_running_env_t env;
	env=&g_envs[rank];
	env->rank=rank;
	env->exit_flag=0;
	env->tid=tid;
	memset(&env->prof_data,0,sizeof(myth_prof_data));
	//Initialize allocators
	myth_flmalloc_init_worker(rank);
	myth_malloc_wrapper_init_worker(rank);
	//Initialize logger
	myth_log_worker_init(env);
	myth_set_current_env(env);
	//Initialize random seed
	myth_random_init(((unsigned)time(NULL))+rank);
	//Initialize runqueue
	myth_queue_init(&env->runnable_q);
	myth_queue_clear(&env->runnable_q);
	//Initialize freelist for thread descriptor
#ifdef MYTH_SPLIT_STACK_DESC
	myth_freelist_init(env->freelist_desc);
	myth_freelist_init(env->freelist_stack);
#if (INITIAL_STACK_ALLOC_UNIT>0)
	{
		//Allocate
		int i;
		size_t th_size;
		size_t alloc_size;
		char *th_ptr;
		th_size=sizeof(myth_thread);
		alloc_size=th_size*INITIAL_STACK_ALLOC_UNIT;
#ifdef ALLOCATE_STACK_BY_MALLOC
		th_ptr=myth_flmalloc(env->rank,alloc_size);
#else
		alloc_size+=4095;
		alloc_size&=~(0xFFF);
#ifdef MAP_STACK
		th_ptr=mmap(NULL,alloc_size,PROT_READ|PROT_WRITE,MAP_PRIVATE|MAP_ANONYMOUS|MAP_STACK,-1,0);
#else
		th_ptr=mmap(NULL,alloc_size,PROT_READ|PROT_WRITE,MAP_PRIVATE|MAP_ANONYMOUS,-1,0);
#endif
#endif
		for (i=0;i<INITIAL_STACK_ALLOC_UNIT;i++){
			myth_thread_t ret;
			ret=(myth_thread_t)th_ptr;
			myth_freelist_push(env->freelist_desc,ret);
			th_ptr+=th_size;
		}
	}
	{
		//Allocate
		int i;
		size_t th_size;
		size_t alloc_size;
		char *th_ptr;
		th_size=g_default_stack_size;
		alloc_size=th_size*INITIAL_STACK_ALLOC_UNIT;
#ifdef ALLOCATE_STACK_BY_MALLOC
		th_ptr=myth_flmalloc(env->rank,alloc_size);
#else
		alloc_size+=4095;
		alloc_size&=~(0xFFF);
#ifdef MAP_STACK
		th_ptr=mmap(NULL,alloc_size,PROT_READ|PROT_WRITE,MAP_PRIVATE|MAP_ANONYMOUS|MAP_STACK,-1,0);
#else
		th_ptr=mmap(NULL,alloc_size,PROT_READ|PROT_WRITE,MAP_PRIVATE|MAP_ANONYMOUS,-1,0);
#endif
#endif
		/*th_ptr+=th_size-sizeof(void*);
		for (i=0;i<INITIAL_STACK_ALLOC_UNIT;i++){
			void **ret;
			ret=(void**)th_ptr;
			myth_freelist_push(env->freelist_stack,ret);
			th_ptr+=th_size;
		}*/
		th_ptr+=th_size-(sizeof(void*)*2);
		for (i=0;i<STACK_ALLOC_UNIT;i++){
			void**
			ret=(void**)th_ptr;
			uintptr_t *blk_size=(uintptr_t*)(th_ptr+sizeof(void*));
			*blk_size=0;//indicates default
			myth_freelist_push(env->freelist_stack,ret);
			th_ptr+=th_size;
		}
	}
#endif
#else
	myth_freelist_init(env->freelist_ds);
#endif
#ifdef MYTH_WRAP_SOCKIO
	//Initialize I/O
	myth_io_worker_init(env,&env->io_struct);
#endif //MYTH_WRAP_SOCKIO
	env->this_thread=NULL;
	//Wait for other worker threads
	real_pthread_barrier_wait(&g_worker_barrier);
	//set signal mask
	if (env->rank!=0){
		sigset_t ss;
		sigemptyset( &ss );
		sigaddset(&ss,SIGALRM);
		real_pthread_sigmask(SIG_BLOCK,&ss,NULL);
	}
	else{
		struct sigaction newact;
		memset(&newact,0,sizeof(newact));
		newact.sa_sigaction=myth_alrm_sighandler;
		newact.sa_flags=SA_SIGINFO|SA_RESTART;
		sigaction(SIGALRM,&newact,NULL);
		sigaction(SIGVTALRM,&newact,NULL);
		//Start interval timer
		struct itimerval tv;
		memset(&tv,0,sizeof(struct itimerval));
		tv.it_value.tv_usec=10000;
		tv.it_interval.tv_usec=10000;
#ifdef MYTH_USE_ITIMER
		setitimer(ITIMER_REAL,&tv,NULL);
#endif
	}
}

//Cleanup a worker thread
static inline void myth_cleanup_worker(int rank)
{
	//synchronize
	real_pthread_barrier_wait(&g_worker_barrier);
	myth_running_env_t env;
	env=myth_get_current_env();
	//cleanup timer
	if (rank==0){
		//Stop interval timer
		struct itimerval tv;
		memset(&tv,0,sizeof(struct itimerval));
		tv.it_value.tv_usec=0;
		tv.it_interval.tv_usec=0;
		setitimer(ITIMER_REAL,&tv,NULL);
	}
	//Release scheduler's stack
	if (env->sched.stack)
		myth_free(env->sched.stack,0);
#ifdef MYTH_WRAP_SOCKIO
	//Release I/O
	myth_io_worker_fini(env,&env->io_struct);
#endif
	//Release runqueue
	myth_queue_fini(&env->runnable_q);
	//Release thread descriptor of current thread
	if (env->this_thread)free_myth_thread_struct_desc(env,env->this_thread);			//Found in a freelist-ds
	//Count freelist entry
#ifdef MYTH_FL_PROF
	int fl_num=0;
	void **ret;
	while (1){
		myth_freelist_pop(env->freelist_stack,ret);
		if (!ret)break;
		fl_num++;
	}
	if (fl_num)
		fprintf(stderr,"Rank : %d freelist stack entry : %d\n",rank,fl_num);
#endif
	//Release allocator
	myth_flmalloc_fini_worker(rank);
	myth_malloc_wrapper_fini_worker(rank);
	//finalize logger
	myth_log_worker_fini(env);
}

//Execute worker thread scheduling loop
static inline void myth_worker_start_ex_body(int rank)
{
	myth_setup_worker(rank);
	myth_running_env_t env;
	env=myth_get_current_env();
	env->sched.stack=NULL;
	//Call thread scheduler
	myth_sched_loop();
	myth_cleanup_worker(rank);
}

MYTH_CTX_CALLBACK void myth_startpoint_init_ex_1(void *arg1,void *arg2,void *arg3)
{
	myth_running_env_t env=(myth_running_env_t)arg1;
	myth_thread_t this_th=(myth_thread_t)arg2;
	//Add to runqueue
	myth_queue_push(&env->runnable_q,this_th);
}

//Execute worker thread scheduling loop introducing current context as a new thread
static inline void myth_startpoint_init_ex_body(int rank)
{
	myth_setup_worker(rank);
	myth_running_env_t env;
	env=myth_get_current_env();
	env->exit_flag = -1;
	myth_thread_t this_th;
	//Allocate thread descriptor for current thread
#ifdef MYTH_SPLIT_STACK_DESC
	this_th=get_new_myth_thread_struct_desc(env);
	this_th->stack=NULL;
#else
	this_th=get_new_myth_thread_struct_desc(env);
#endif
#if defined MYTH_ENABLE_THREAD_ANNOTATION && defined MYTH_COLLECT_LOG
	sprintf(this_th->annotation_str,"%p(main)",this_th);
#endif
	//Set worker thread descrptor
	this_th->env = env;
	//Initialize context for scheduler
	env->sched.stack=myth_malloc(SCHED_STACK_SIZE);
	myth_make_context_voidcall(&env->sched.context,myth_sched_loop,(void*)(((char*)env->sched.stack)+SCHED_STACK_SIZE-sizeof(void*)),SCHED_STACK_SIZE-sizeof(void*));
	//Switch to scheduler
	myth_swap_context_withcall(&this_th->context, &env->sched.context,
		myth_startpoint_init_ex_1, env, this_th, NULL);
}

MYTH_CTX_CALLBACK void myth_startpoint_exit_ex_1(void *arg1,void *arg2,void *arg3)
{
	myth_thread_t th=arg1;
	intptr_t rank=(intptr_t)arg2;
	myth_running_env_t target=&g_envs[rank];
	th->env=target;
	while (!myth_queue_trypass(&target->runnable_q,th)){
		target=myth_env_get_randomly();
		th->env=target;
	}
}

//Return the context introduced by myth_startpoint_init_ex to the original pthread
static inline void myth_startpoint_exit_ex_body(int rank)
{
	//Guarantee running the thread on the initial worker
	myth_running_env_t env;
	//Get current environment
	env=myth_get_current_env();
	//Set exit flag
	myth_notify_workers_exit();
	//If running on a different worker, switch context
	while (env->rank != rank) {
		intptr_t rank_ = rank;
		myth_thread_t th;
		th = env->this_thread;
		myth_swap_context_withcall(&th->context, &env->sched.context, 
      myth_startpoint_exit_ex_1, (void*)th, (void*) rank_, NULL);
		myth_rwbarrier();
		//Obtain worker thread descriptor again, because env may be changed
		env=th->env;
	}
	//Cleanup
	myth_cleanup_worker(rank);
}

//Initialize each worker thread
static inline void *myth_worker_thread_fn(void *args)
{
	intptr_t rank=(intptr_t)args;
#ifdef WORKER_SET_AFFINITY
	//Set affinity
	cpu_set_t cs;
	cs=myth_get_worker_cpuset(rank);
	real_pthread_setaffinity_np(real_pthread_self(),sizeof(cpu_set_t),&cs);
#endif
	if (rank == 0) {
		//setup as a main thread
		myth_startpoint_init_ex_body(rank);
	} else {
		//setup as a worker
		myth_worker_start_ex_body(rank);
	}
	return NULL;
}

extern myth_steal_func_t g_myth_steal_func;

static inline myth_thread_t myth_steal_body(void)
{
	myth_thread_t ret;
	myth_running_env_t target;
	//Choose a worker thread that seems to be busy
	target=myth_env_get_randomly();
	myth_assert(target);
	//Try to steal thread
	ret=myth_queue_take(&target->runnable_q);
	if (ret){
		if (!ret->stack){
			//Cannot be stolen because it is main thread
			while (!myth_queue_trypass(&target->runnable_q,ret)){
				//Retry changing the target
				target=myth_env_get_randomly();
			}
			return NULL;
		}
		myth_assert(ret->status==MYTH_STATUS_READY);
		//Change worker thread descriptor
		ret->env=NULL;
	}
	return ret;
}

static inline void myth_import_body(myth_thread_t th)
{
	myth_running_env_t e=myth_get_current_env();
	th->env=e;
	myth_queue_push(&e->runnable_q,th);
}

static inline void myth_ext_import_body(myth_thread_t th)
{
	myth_running_env_t target;
	do{
		target=myth_env_get_randomly();
	}
	while (!myth_queue_trypass(&target->runnable_q,th));
}

//Main loop of scheduler
static void myth_sched_loop(void)
{
	MAY_BE_UNUSED uint64_t t0,t1;
	myth_running_env_t env;
	t0=0;
	t1=0;
	env=myth_get_current_env();
	myth_log_add_context_switch(env,NULL);
#ifdef MYTH_SCHED_LOOP_DEBUG
	myth_dprintf("myth_sched_loop:entered main loop\n");
#endif
#ifdef MYTH_WS_PROF_DETAIL
	env->prof_data.ws_attempt_count=myth_malloc(sizeof(uint64_t)*g_worker_thread_num);
	{
		int i;
		for (i = 0; i < g_worker_thread_num; i++) {
			env->prof_data.ws_attempt_count[i] = 0;
		}
	}
#endif
	while (1) {
		//sched_yield();
		myth_thread_t next_run;
		//Get runnable thread
		next_run=myth_queue_pop(&env->runnable_q);
#ifdef MYTH_WRAP_SOCKIO
		//If there is no runnable thread, check I/O
		if (!next_run){
			next_run=myth_io_polling(env);
		}
#endif
		//If there is no runnable thread after I/O checking, try work-stealing
		if (!next_run){
			//next_run=myth_steal_from_others(env);
			next_run=g_myth_steal_func(env->rank);
		}
		if (next_run)
		{
			//sanity check
			myth_assert(next_run->status==MYTH_STATUS_READY);
			env->this_thread=next_run;
			next_run->env=env;
			//Switch to runnable thread
#ifdef MYTH_SCHED_LOOP_DEBUG
			myth_dprintf("myth_sched_loop:switching to thread:%p\n",next_run);
#endif
			myth_assert(next_run->status==MYTH_STATUS_READY);
			myth_swap_context(&env->sched.context, &next_run->context);
#ifdef MYTH_SCHED_LOOP_DEBUG
			myth_dprintf("myth_sched_loop:returned from thread:%p\n",(void*)next_run);
#endif
			env->this_thread=NULL;
		}
		//Check exit flag
		if (env->exit_flag==1){
			env->this_thread=NULL;
#ifdef MYTH_SCHED_LOOP_DEBUG
			myth_dprintf("env %p received exit signal,exiting\n",env);
#endif
			return;
		}
	}
}

static inline int myth_get_worker_num_body(void)
{
	myth_running_env_t e=myth_get_current_env();
	return e->rank;
}

static inline int myth_get_num_workers_body(void)
{
	return g_worker_thread_num;
}
#endif
