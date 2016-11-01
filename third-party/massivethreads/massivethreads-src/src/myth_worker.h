/* 
 * myth_worker.h
 */
#pragma once
#ifndef MYTH_WORKER_H_
#define MYTH_WORKER_H_

#include <pthread.h>
#include <time.h>

#include "myth/myth.h"

#include "myth_config.h"
#include "myth_misc.h"
#include "myth_sched.h"
#include "myth_io.h"
#include "myth_wsqueue.h"

#if MYTH_ECO_MODE && MYTH_ECO_TEIAN_STEAL
typedef enum {
  RUNNING = 31,
  STEALING,
  SLEEPING,
  FINISH,
  EXITED,
}worker_cond_t;
#endif


//Profiling data
typedef struct myth_prof_data {
#if MYTH_CREATE_PROF
  uint64_t create_cnt,create_cycles;
  uint64_t create_cycles_tmp;
#endif
#if MYTH_CREATE_PROF_DETAIL
  uint64_t create_d_cnt,create_d_tmp;
  uint64_t create_alloc,create_switch,create_push;
#endif
#if MYTH_ENTRY_POINT_PROF
  uint64_t ep_cnt;
  uint64_t ep_cyclesA,ep_cyclesB;
  uint64_t ep_cycles_tmp;
#endif
#if MYTH_EP_PROF_DETAIL
  uint64_t ep_d_cnt,ep_d_tmp;
  uint64_t ep_join,ep_switch,ep_pop;
#endif
#if MYTH_JOIN_PROF
  uint64_t join_cnt,join_cycles;
#endif
#if MYTH_JOIN_PROF_DETAIL
  uint64_t join_d_cnt;
  uint64_t join_join,join_release;
#endif
#if MYTH_WS_PROF_DETAIL
  uint64_t ws_hit_cnt;
  uint64_t ws_hit_cycles;
  uint64_t ws_miss_cnt;
  uint64_t ws_miss_cycles;
  uint64_t *ws_attempt_count;
#endif
#if MYTH_SWITCH_PROF
  uint64_t sw_cnt;
  uint64_t sw_cycles;
  uint64_t sw_tmp;
#endif
#if MYTH_ALLOC_PROF
  uint64_t alloc_cnt;
  uint64_t malloc_cnt;
  uint64_t malloc_cycles;
  uint64_t addlist_cycles;
  uint64_t dalloc_cnt;
  uint64_t dmalloc_cnt;
  uint64_t dmalloc_cycles;
  uint64_t daddlist_cycles;
  uint64_t salloc_cnt;
  uint64_t smalloc_cnt;
  uint64_t smalloc_cycles;
  uint64_t saddlist_cycles;
#endif
#if MYTH_IO_PROF_DETAIL
  uint64_t io_succ_send_cnt,io_succ_recv_cnt;
  uint64_t io_succ_send_cycles,io_succ_recv_cycles;
  uint64_t io_block_send_cnt,io_block_recv_cnt;
  uint64_t io_block_send_cycles,io_block_recv_cycles;
  uint64_t io_epoll_miss_cycles,io_epoll_miss,io_epoll_hit_cycles,io_epoll_hit;
  uint64_t io_chk_miss_cycles,io_chk_miss,io_chk_hit_cycles,io_chk_hit;
  uint64_t io_block_syscall;
  uint64_t io_genreq;
  uint64_t io_fdmap;
  uint64_t io_rqpop;
  uint64_t io_addlist;
#endif
} myth_prof_data, *myth_prof_data_t;

//A structure describing an environment for executing a thread
//(scheduler, worker thread, runqueue, etc...)
//Each worker thread have one of them

typedef struct myth_running_env {
  //The following entries are only accessed from the owner
  struct myth_thread *this_thread;//Currently executing thread
#if MYTH_SPLIT_STACK_DESC
  myth_freelist_t freelist_desc;//Freelist of thread descriptor
  myth_freelist_t freelist_stack;//Freelist of stack
#else
  myth_freelist_t freelist_ds;//Freelis
#endif
  int log_buf_size;
  int log_count;
  myth_spinlock_t log_lock;
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
#if MYTH_SPLIT_STACK_DESC
  myth_freelist_t *freelist_desc_g;//Freelist of thread descriptor
  myth_freelist_t *freelist_stack_g;//Freelist of stack
#endif
  struct myth_io_struct_perenv io_struct;//I/O-related data structure. See myth_io_struct.h
#if MYTH_ECO_MODE
  int my_sem;
  int isSleepy;
  int ws_target;
#endif
#if MYTH_ECO_TEST
  int thief_count;
#endif
#if MYTH_ECO_TEIAN_STEAL
  worker_cond_t c;
  int finish_ready;
  int knowledge;
#endif
  int exit_flag;
  //-1:Main thread, must not be terminated at the scheduling loop
  //0:Currently application is running
  //1:Application is terminated. Worker thread should exit scheduling loop and terminate itself
} __attribute__((aligned(CACHE_LINE_SIZE))) myth_running_env;

// myth_running_env, * myth_running_env_t;

//typedef struct myth_thread* (*myth_steal_func_t)(int);
extern myth_steal_func_t g_myth_steal_func;

//Thread index
extern myth_running_env_t g_envs;
extern int g_envs_sz;
//Number of worker threads
//Barrier for worker threads
extern pthread_barrier_t g_worker_barrier;


#if WENV_IMPL == WENV_IMPL_PTHREAD
//TLS by pthread
extern pthread_key_t g_env_key;
#elif WENV_IMPL == WENV_IMPL_ELF
//TLS by GCC extension
extern __thread int g_worker_rank;
#elif WENV_IMPL == WENV_IMPL_NONE
//Simple global variable. Works only on single worker thread
#else
#error "invalid WENV_IMPL"
#endif


static void myth_sched_loop(void);

static inline void myth_env_init(void);
static inline void myth_env_fini(void);
static inline void myth_set_current_env(myth_running_env_t e);
static inline myth_running_env_t myth_get_current_env(void);
static inline myth_running_env_t myth_env_get_first_busy(myth_running_env_t e);

static inline void myth_worker_start_ex_body(int rank);
static inline void myth_startpoint_init_ex_body(int rank);
static inline void myth_startpoint_exit_ex_body(int rank);
static void *myth_worker_thread_fn(void *args);

static inline int myth_get_num_workers_body(void);

#endif /* MYTH_WORKER_H_ */
