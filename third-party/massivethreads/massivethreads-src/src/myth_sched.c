/* 
 * myth_sched.c
 */

#include <signal.h>

#include "myth_sched.h"
#include "myth_sched_func.h"

//Global variable declarations
//Global thread index
myth_running_env_t g_envs = NULL;
int g_envs_sz = 0;
//A barrier for worker thread to synchronize
pthread_barrier_t g_worker_barrier;

FILE *g_log_fp;

int g_sched_prof=0;
int g_log_worker_stat=0;

#define PAGE_ALIGN(n) ((((n)+(PAGE_SIZE)-1)/(PAGE_SIZE))*PAGE_SIZE)

/* TODO:
   this does not belong here. 
   should create myth_worker.c and move there */
#if WENV_IMPL == WENV_IMPL_PTHREAD
//TLS by pthread
pthread_key_t g_env_key;
#elif WENV_IMPL == WENV_IMPL_ELF
//TLS by GCC extension
__thread int g_worker_rank = -1;
#elif WENV_IMPL == WENV_IMPL_NONE
//Simple global variable. Works only on single worker thread
#else
#error
#endif

void myth_alrm_sighandler(int signum, siginfo_t *sinfo, void* ctx) {
  (void)signum;
  (void)sinfo;
  (void)ctx;
  myth_running_env_t env;
  int errno_bk;
  errno_bk=errno;
  env=myth_get_current_env();
  int i;
  if (env->rank==0){
    //broadcast signal
    for (i=0;i<g_attr.n_workers;i++){
      if (i!=env->rank){
	real_pthread_kill(g_envs[i].worker, SIGVTALRM);
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
#if MYTH_WRAP_SOCKIO
      myth_thread_t ret;
      ret=myth_io_polling_sig(env);
      if (ret)myth_queue_push(&env->runnable_q,ret);
#endif
    }
  errno=errno_bk;
}

