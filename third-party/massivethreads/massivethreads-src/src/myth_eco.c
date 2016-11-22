/* 
 * myth_eco.c
 */
/* semaphre.h should come before linux/futex.h on FX10
   otherwise you get a compile-time error
   redefinition of loff_t; */
#include <semaphore.h>
#include <linux/futex.h>
#include <limits.h>
#include <assert.h>

#include "myth_config.h"

#include "myth_misc.h"
#include "myth_worker.h"
#include "myth_thread.h"
#include "myth_init.h"
#include "myth_eco.h"
#include "myth_real.h"

#include "myth_misc_func.h"
#include "myth_worker_func.h"


sleep_queue_t g_sleep_queue;
int g_eco_mode_enabled=0;
pthread_mutex_t *queue_lock;
int sleeper;
int task_num;

#if MYTH_ECO_MODE

#if MYTH_ECO_TEST
static void myth_eco_sched_loop(myth_running_env_t env) {
  //  printf("%d\n",FINISH);
  while (1) {
    //sched_yield();
    myth_thread_t next_run;
    //Get runnable thread
    next_run=myth_queue_pop(&env->runnable_q);
#if MYTH_WRAP_SOCKIO
    //If there is no runnable thread, check I/O
    if (!next_run){
      next_run=myth_io_polling(env);
    }
#endif
    //If there is no runnable thread after I/O checking, try work-stealing
    if (!next_run){
      //next_run=myth_steal_from_others(env);
      //next_run=g_myth_steal_func(env->rank);
      next_run=myth_eco_steal(env->rank);
    }
    if ((worker_cond_t)next_run == FINISH) { //next_run == FINISH
      if(env->rank != 0) {
	env->this_thread=NULL;
	return;
      } else {
	while(1) {
	  int temp = 0;
	  int j;
	  for(j = 1; j < g_attr.n_workers; j++) {
	    if(g_envs[j].c != EXITED) {
	      temp = 1;
	    }
	  }
	  if(temp == 0) return;
	}
      }
    }
    if (next_run)
      {
	//sanity check
	myth_assert(next_run->status==MYTH_STATUS_READY);
	env->this_thread=next_run;
	next_run->env=env;
	//Switch to runnable thread
#if MYTH_SCHED_LOOP_DEBUG
	myth_dprintf("myth_sched_loop:switching to thread:%p\n",next_run);
#endif
	myth_assert(next_run->status==MYTH_STATUS_READY);
	myth_swap_context(&env->sched.context, &next_run->context);
#if MYTH_SCHED_LOOP_DEBUG
	myth_dprintf("myth_sched_loop:returned from thread:%p\n",(void*)next_run);
#endif
	env->this_thread=NULL;
      }
    //Check exit flag
    if (env->exit_flag==1){
      if(env->rank == 0)
	while(1) {
	  int temp = 0;
	  int j;
	  for(j = 1; j < g_attr.n_workers; j++) {
	    if(g_envs[j].c != EXITED) {
	      temp = 1;
	    }
	  }
	  if(temp == 0) return;
	}
      env->this_thread=NULL;
#if MYTH_SCHED_LOOP_DEBUG
      myth_dprintf("env %p received exit signal,exiting\n",env);
#endif
      return;
    }
  }
}
#endif

myth_thread_t myth_eco_steal(int rank) {
  myth_running_env_t env,busy_env;
  myth_thread_t next_run=NULL;
#if MYTH_WS_PROF_DETAIL
  uint64_t t0,t1;
  t0=myth_get_rdtsc();
#endif

  //Choose a worker thread that seems to be busy
  env=&g_envs[rank];
  if(env->isSleepy == 1) {
    env->isSleepy = 0;
    busy_env = &g_envs[env->ws_target];
  } else {
    busy_env=myth_env_get_first_busy(env);
  }

  if (busy_env){
    myth_assert(busy_env!=env);
    //int ws_victim;
    //ws_victim=busy_env->rank;
    //Try to steal thread
    next_run=myth_queue_take(&busy_env->runnable_q);
    if (next_run){
#if MYTH_SCHED_LOOP_DEBUG
      myth_dprintf("env %p is stealing thread %p from %p...\n",env,steal_th,busy_env);
#endif
      myth_assert(next_run->status==MYTH_STATUS_READY);
      //Change worker thread descriptor
      next_run->env=env;
    }
  }
  if(!next_run) {
    if(busy_env->c == STEALING) {
#if MYTH_ECO_TEST
      if(env->thief_count < 3) {
	env->thief_count++;
	return 0;
      }
#endif
      myth_sleep_1();
      // This line seems not correct, it may occur infinite recursion
      //return myth_eco_steal(env->rank);
      return NULL;
    }
    else if(busy_env->c == SLEEPING) {
      MAY_BE_UNUSED int tmp = task_num;
      next_run = myth_eco_all_task_check(env);
      if(!next_run){
	myth_sleep_1();
      } else {
	return next_run;
      }
    } 
    else if(busy_env->c == RUNNING) { // victim has one task and executing
      int tmp = task_num;
      next_run = myth_eco_all_task_check(env);
      if(!next_run){
	myth_sleep_2(tmp);
      } else {
	return next_run;
      }
    } else if(busy_env->c == FINISH) {
      return (myth_thread_t)FINISH;
    }
  }

#if MYTH_WS_PROF_DETAIL
  t1=myth_get_rdtsc();
  if (g_sched_prof){
    env->prof_data.ws_attempt_count[busy_env->rank]++;
    if (next_run){
      env->prof_data.ws_hit_cycles+=t1-t0;
      env->prof_data.ws_hit_cnt++;
    }else{
      env->prof_data.ws_miss_cycles+=t1-t0;
      env->prof_data.ws_miss_cnt++;
    }
  }
#endif
#if MYTH_ECO_TEST
  env->thief_count = 0;
#endif
  return next_run;
}

myth_thread_t myth_eco_all_task_check(myth_running_env_t env)
{
  myth_running_env_t busy_env;
  myth_thread_t next_run=NULL;
  int i=0;
#if MYTH_WS_PROF_DETAIL
  uint64_t t0,t1;
  t0=myth_get_rdtsc();
#endif
  while(i < g_attr.n_workers){
    if(g_envs[i].c == RUNNING) {
      busy_env = &g_envs[i];
      next_run = myth_queue_take(&busy_env->runnable_q);
      if(next_run){
	next_run->env=env;
	return next_run;
      }
    }
    if(g_envs[i].c == FINISH){
      return (myth_thread_t)FINISH;
    }
    i++;
  }
  return NULL;
}

// wait
void myth_sleep_1(void) {
  int s;
  myth_running_env_t env = myth_get_current_env();
  int *my_sem = &env->my_sem;
  s = *my_sem = 2;
  if(myth_sleeper_push(my_sem, env->rank, -1) == 0) {
#if MYTH_ECO_CIRCLE_STEAL || MYTH_ECO_TEIAN_STEAL
    env->c = SLEEPING;
#endif
    while( s != 0 ) {
      futex_wait( my_sem, 2 );
      s = fetch_and_store((void *) my_sem, 2);
    }
#if MYTH_ECO_DEBUG
    printf("wake up!\n");
#endif
#if MYTH_ECO_CIRCLE_STEAL || MYTH_ECO_TEIAN_STEAL
    env->c = STEALING;
#endif

  }
}

// wait
void myth_sleep_2(int num) {
  int s;
  myth_running_env_t env = myth_get_current_env();
  int *my_sem = &env->my_sem;
  s = *my_sem = 2;
  if(myth_sleeper_push(my_sem, env->rank, num) == 0) {
#if MYTH_ECO_CIRCLE_STEAL || MYTH_ECO_TEIAN_STEAL
     env->c = SLEEPING;
#endif
    while( s != 0 ) {
      futex_wait( my_sem, 2 );
      s = fetch_and_store((void *) my_sem, 2);
    }
#if MYTH_ECO_DEBUG
    printf("wake up!\n");
#endif
    env->c = STEALING;
  }
}

void myth_go_asleep(void) {
  int s;
  myth_running_env_t env = myth_get_current_env();
  int *my_sem = &env->my_sem;
  s = *my_sem = 2;
#if MYTH_ECO_CIRCLE_STEAL || MYTH_ECO_TEIAN_STEAL
  env->c = SLEEPING;
#endif
  while(myth_sleeper_push(my_sem, env->rank, -1) != 0) {  }
  //  __sync_fetch_and_add(&sleeper,1); //atomic(sleeper++;)
  while( s != 0 ) {
    futex_wait( my_sem, 2 );
    s = fetch_and_store((void *) my_sem, 2);
  }
#if MYTH_ECO_DEBUG
  printf("wake up!\n");
#endif
#if MYTH_ECO_CIRCLE_STEAL || MYTH_ECO_TEIAN_STEAL
  env->c = STEALING;
#endif
}

int myth_wakeup_one(void) {
  int rank;
  int *my_sem;
  sleep_queue_t tmp = myth_sleeper_pop();
  if(tmp == NULL) return -1;//nobody sleeping
  my_sem = tmp->head_sem;
  rank = tmp->head_rank;
  myth_free(tmp);//free
  if(my_sem == 0) return -1;//ideally not operated
  if(__sync_fetch_and_sub(my_sem,1) != 1) {
    *my_sem = 0;
    g_envs[rank].isSleepy = 1;
    g_envs[rank].ws_target = myth_get_worker_num_body();
    if(futex_wakeup_one( (void *)my_sem ) != -1) {
      __sync_fetch_and_sub(&sleeper,1); // atomic(sleeper--;)
      //real_free(tmp);
      return 0;
    } else {
      return -1;
    }
  }
  return -1;
}

void myth_wakeup_all(void) {
  int i;
  for(i = 0; i < g_attr.n_workers;) {
    /* int s;   if((s = myth_wakeup_one()) != -1) {
      i++;
      }*/
    myth_wakeup_one();
    i++;
  }
#if MYTH_ECO_MODE_DEBUG
  printf("wakeup\n");
#endif
}

void myth_wakeup_all_force(void)
{
	int i;
	for(i = 0; i < g_attr.n_workers; i++) {
		int *my_sem=&g_envs[i].my_sem;
		*my_sem = 0;
		g_envs[i].isSleepy = 0;
		futex_wakeup_one( (void *)my_sem );
	}
}

void myth_eco_init(void) {
  int i;
  char *env;
  env = getenv("MYTH_ECO_MODE");
  if (env){
    g_eco_mode_enabled=atoi(env);
  }
  sleeper = 0;
  queue_lock = myth_malloc(sizeof(pthread_mutex_t));
  //  thread_sem = myth_malloc(sizeof(int) * g_attr.n_workers);
  g_sleep_queue = NULL;// = myth_malloc(sizeof(struct sleep_queue));
  for(i = 0; i < g_attr.n_workers; i++) {
    g_envs[i].my_sem = 0;
#if MYTH_ECO_TEIAN_STEAL
    g_envs[i].knowledge = 0;
#endif
  }
  real_pthread_mutex_init(queue_lock, NULL);
  //  MAX_TRY = g_attr.n_workers * 5;
  //  g_sleep_queue->next = NULL;
  //  g_sleep_queue->tail = g_sleep_queue;
  //  g_sleep_queue->head_sem = NULL;
  //  g_sleep_queue->head_rank = g_attr.n_workers+1;
#if MYTH_ECO_TEIAN_STEAL
  task_num = 0;
  g_envs[0].c = RUNNING;
  for(i = 1; i < g_attr.n_workers; i++) g_envs[i].c = STEALING;
  for(i = 0; i < g_attr.n_workers; i++) g_envs[i].finish_ready=0;
#endif
}
void myth_eco_des(void) {
  myth_free(queue_lock);
}

int myth_sleeper_push(int *sem, int rank,int num) {
  int rem = sleeper;
  //lock
  real_pthread_mutex_lock(queue_lock);
  if(num != -1) {
    if(num != task_num) {
	//unlock
	real_pthread_mutex_unlock(queue_lock);
	return -1;
    }      
  }
  if(!__sync_bool_compare_and_swap(&sleeper,rem,rem+1)) { //atomic(sleeper++;)
    //unlock
    real_pthread_mutex_unlock(queue_lock);
    return -1;
  }
  if(g_envs[rank].exit_flag == 1) {
    //unlock
    real_pthread_mutex_unlock(queue_lock);
    return -1;
  }
  
  sleep_queue_t tmp = myth_malloc(sizeof(struct sleep_queue));
  tmp->head_sem = sem;
  tmp->head_rank = rank;
  tmp->tail = tmp;
  tmp->next = NULL;
  if(g_sleep_queue == NULL) {
    g_sleep_queue = tmp;
    g_sleep_queue->tail = tmp;
  } else if(g_sleep_queue->tail == NULL) {
    //real_free(g_sleep_queue);
    g_sleep_queue = tmp;
    g_sleep_queue->tail = tmp;
  }else {
    g_sleep_queue->tail->next = tmp;
    g_sleep_queue->tail = tmp;
  }

  //unlock
  real_pthread_mutex_unlock(queue_lock);
  return 0;
}

sleep_queue_t myth_sleeper_pop(void) {
  //lock
  real_pthread_mutex_lock(queue_lock);

  if(g_sleep_queue == NULL) {
    real_pthread_mutex_unlock(queue_lock);
    return NULL;
  }

  // struct sleep_queue tmp = *g_sleep_queue;// copy data
  sleep_queue_t address = g_sleep_queue;//copy address
  if(g_sleep_queue->next == NULL){
    g_sleep_queue = NULL;
  } else {
    g_sleep_queue->next->tail = g_sleep_queue->tail;
    g_sleep_queue = g_sleep_queue->next;
  }

  //unlock
  real_pthread_mutex_unlock(queue_lock);

  return address;
}

int futex_wait( void *futex, int comparand ) {
  int r = syscall( SYS_futex, futex, FUTEX_WAIT, comparand, NULL);
  //  int e = errno;
  return r;
}

int futex_wakeup_one( void *futex ) {
  int r = syscall( SYS_futex, futex, FUTEX_WAKE, 1);
  return r;
}

int futex_wakeup_n( void *futex, int n ) {
  int r = syscall( SYS_futex, futex, FUTEX_WAKE, n);
  return r;
}

int futex_wakeup_all( void *futex ) {
  int r = syscall( SYS_futex, futex, FUTEX_WAKE, INT_MAX);
  return r;
}

int fetch_and_store(volatile void *ptr, int addend) {
  int result;
#if MYTH_ARCH == MYTH_ARCH_i386 || MYTH_ARCH == MYTH_ARCH_amd64 || MYTH_ARCH == MYTH_ARCH_amd64_knc
  __asm__ __volatile__("lock\nxadd" "" " %0,%1"
		       : "=r"(result),"=m"(*(volatile int*)ptr)
		       : "0"(addend), "m"(*(volatile int*)ptr)
		       : "memory");
#else
  result = __sync_fetch_and_add((int*)ptr, addend);
#endif
  return result;
}
#endif
