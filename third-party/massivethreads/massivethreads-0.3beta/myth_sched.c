//#include <stdio.h>
//#include <stdlib.h>
//#include <string.h>
//#include <stdarg.h>
//#include <assert.h>
//#include <errno.h>
//#include <unistd.h>
//#include <sys/syscall.h>
//#include <sys/types.h>
#include <signal.h>
#include "myth_config.h"

#include "myth_wsqueue.h"
#include "myth_sched.h"

#include "myth_wsqueue_func.h"
#include "myth_sched_func.h"

//Global variable declarations
//Global thread index
int g_thread_index=0;
myth_running_env_t g_envs=NULL;
//The number of worker threads
int g_worker_thread_num=0;
//A barrier for worker thread to synchronize
pthread_barrier_t g_worker_barrier;

FILE *g_log_fp;

int g_sched_prof=0;
int g_log_worker_stat=0;

#define PAGE_ALIGN(n) ((((n)+(PAGE_SIZE)-1)/(PAGE_SIZE))*PAGE_SIZE)

size_t g_default_stack_size=PAGE_ALIGN(DEF_STACK_SIZE);

#ifdef TLS_BY_PTHREAD
//TLS by pthread
pthread_key_t g_env_key;
#elif defined TLS_BY_ELF
//TLS by GCC extension
__thread int g_worker_rank;
#elif defined TLS_NONE
//Simple global variable. Works only on single worker thread
#else
#error
#endif

void myth_alrm_sighandler(int signum,siginfo_t *sinfo,void* ctx)
{
	myth_running_env_t env;
	int errno_bk;
	errno_bk=errno;
	env=myth_get_current_env();
	int i;
	if (env->rank==0){
		//broadcast signal
		for (i=0;i<g_worker_thread_num;i++){
			if (i!=env->rank){
				real_pthread_kill(g_envs[i].worker,SIGVTALRM);
			}
		}
	}
#if 0
	int ret;
	char str[100];
	strcpy(str,"1 SIGALRM IO:X Q:X\n");
	//check for I/O
	if (!env->io_checking_flag){
		str[13]='O';
		//Do I/O checking
	}
	if (!myth_queue_is_operating(&env->runnable_q)){
		str[17]='O';
	}
	str[0]='0'+(char)env->rank;
	ret=write(1,str,strlen(str));
#endif
	if (!myth_queue_is_operating(&env->runnable_q))
	{
#ifdef MYTH_WRAP_SOCKIO
		myth_thread_t ret;
		ret=myth_io_polling_sig(env);
		if (ret)myth_queue_push(&env->runnable_q,ret);
#endif
	}
	errno=errno_bk;
}

