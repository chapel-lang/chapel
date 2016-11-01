/* 
 * myth_log.h
 */
#pragma once
#ifndef MYTH_LOG_H_
#define MYTH_LOG_H_

#include <stdint.h>
#include "myth_config.h"

//Special pointer describing status
#define THREAD_PTR_SCHED NULL //scheduler running
#define TRREAD_PTR_SCHED_SLEEP ((myth_thread_t)0xFFFFFFFF) //scheduler sleeping
#define THREAD_PTR_SCHED_TERM ((myth_thread_t)0xFFFFFFFE) //scheduler entering termination

typedef enum {
  MYTH_LOG_THREAD_ANNOTATION = 0,//thread annotation
  MYTH_LOG_SWITCH,//context switch
}myth_log_type_t;

typedef struct myth_log_entry {
  uint64_t tsc;			//Time stamp counter from rdtsc
  myth_log_type_t type;
  int rank;
  union{
    struct{
      struct myth_thread *th;
      int recycle_count;
    }ctx_switch;
    struct{
      struct myth_thread *th;
      int recycle_count;
      char str[MYTH_THREAD_ANNOTATION_MAXLEN];
    }annotation;
  }u;
}myth_log_entry,*myth_log_entry_t;

extern uint64_t g_tsc_base;

struct myth_running_env;
struct myth_thread;
static inline void myth_log_init(void);
static inline void myth_log_worker_init(struct myth_running_env * env);
static inline void myth_log_worker_fini(struct myth_running_env * env);
static inline void myth_log_add_context_switch(struct myth_running_env * env, struct myth_thread * th);
static inline void myth_log_add_context_switch(struct myth_running_env * env, struct myth_thread * th);


#endif /* MYTH_LOG_H_ */
