/* 
 * myth_sched.h
 */
#pragma once
#ifndef _MYTH_SCHED_H_
#define _MYTH_SCHED_H_
#include <stdint.h>

#include "myth/myth.h"
#include "myth_config.h"

#include "myth_context.h"

typedef struct myth_running_env * myth_running_env_t;

//Thread scheduler
typedef struct myth_sched {
  void *stack;//Call stack
  myth_context context;//Scheduler context
} myth_sched, * myth_sched_t;

//External Global variables
extern int g_log_worker_stat;
extern int g_sched_prof;


//Cancel constants, set as the same as those of pthreads
#define MYTH_CANCEL_DEFERRED PTHREAD_CANCEL_DEFERRED
#define MYTH_CANCEL_ENABLE PTHREAD_CANCEL_ENABLE
#define MYTH_CANCEL_DISABLE PTHREAD_CANCEL_DISABLE
#define MYTH_CANCELED PTHREAD_CANCELED


static inline void myth_env_init(void);
static inline void myth_env_fini(void);
static inline void myth_set_current_env(myth_running_env_t e);
static inline myth_running_env_t myth_get_current_env(void);
static inline void init_myth_thread_struct(myth_running_env_t env,myth_thread_t th);
static inline myth_running_env_t myth_env_get_first_busy(myth_running_env_t e);
MYTH_CTX_CALLBACK void myth_create_1(void *arg1,void *arg2,void *arg3);
static inline int myth_create_ex_body(myth_thread_t * id,
				      myth_thread_attr_t * attr,
				      myth_func_t func, void *arg);
MYTH_CTX_CALLBACK void myth_yield_ex_1(void * arg1, void * arg2, void * arg3);
static inline int myth_yield_ex_body(int yield_opt);
static inline int myth_yield_body(void);
static inline void myth_join_1(myth_running_env_t e,myth_thread_t th,void **result);
MYTH_CTX_CALLBACK void myth_join_2(void *arg1,void *arg2,void *arg3);
MYTH_CTX_CALLBACK void myth_join_3(void *arg1,void *arg2,void *arg3);
static inline int myth_join_body(myth_thread_t th,void **result);
static inline int myth_detach_body(myth_thread_t th);
static void myth_sched_loop(void);
MYTH_CTX_CALLBACK void myth_entry_point_1(void *arg1,void *arg2,void *arg3);
MYTH_CTX_CALLBACK void myth_entry_point_2(void *arg1,void *arg2,void *arg3);
static void __attribute__((unused)) myth_entry_point(void);
static inline void myth_entry_point_cleanup(myth_thread_t this_thread);
static inline void myth_log_start_body(void);
static inline void myth_log_pause_body(void);
static inline void myth_sched_prof_start_body(void);
static inline void myth_sched_prof_pause_body(void);

static inline myth_thread_t get_new_myth_thread_struct_desc(myth_running_env_t env);
static inline void *get_new_myth_thread_struct_stack(myth_running_env_t env,size_t size_in_bytes);
static inline void init_myth_thread_struct(myth_running_env_t env,myth_thread_t th);
static inline void free_myth_thread_struct_desc(myth_running_env_t e,myth_thread_t th);
static inline void free_myth_thread_struct_stack(myth_running_env_t e,myth_thread_t th);



#endif //_MYTH_H_
