/* 
 * myth_thraed.h ---
 *  data structure related to thread and
 *  must be included by the user
 */
#ifndef MYTH_THREAD_H_
#define MYTH_THREAD_H_

#include <stdlib.h>

#include "myth/myth.h"

#include "myth_config.h"
#include "myth_context.h"
#include "myth_tls.h"

/* Thread status constants */
typedef enum {
  // Executable
  MYTH_STATUS_READY = 0,
  // Blocked
  MYTH_STATUS_BLOCKED = 1,
  // Execution finished. But you have to obtain lock before release
  MYTH_STATUS_FREE_READY = 2,
  //Execution finished. Feel free to release
  MYTH_STATUS_FREE_READY2 = 3,
} myth_status_t;

/* Thread descriptor */
// typedef 
struct myth_thread {
  struct myth_thread * next;
  // A thread which is waiting for this
  struct myth_thread* join_thread;
  myth_func_t entry_func;
  // Return value
  void *result;
  // Context
  myth_context context;
  // Pointer to stack
  void *stack;
  size_t stack_size;
  // Pointer to worker thread
  struct myth_running_env* env;
  // Lock
  myth_spinlock_t lock;
  // Status
  volatile myth_status_t status;
  uint8_t detached;
  uint8_t cancelled;
  uint8_t cancel_enabled;
#if MYTH_ENABLE_THREAD_ANNOTATION && MYTH_COLLECT_LOG
  char annotation_str[MYTH_THREAD_ANNOTATION_MAXLEN];
  int recycle_count;
#endif
#if MYTH_DESC_REUSE_CHECK
  myth_spinlock_t sanity_check;
#endif
  /* TODO: get rid of them */
  void *custom_data_ptr;
  int custom_data_size;

  myth_thread_attr_t attr;

  myth_tls_tree_t tls[1];

#if MYTH_DEBUG_JOIN_FCC
  uint64_t join_called_at;
  char * child_status_when_join_was_called;
  uint64_t finished_at;
  struct myth_thread * waiter;
  char * when_I_finished;
#endif
  
} ;

// __attribute__((aligned(CACHE_LINE_SIZE))) myth_thread, *myth_thread_t;

// typedef 
struct myth_pickle {
  struct myth_thread desc;
  size_t stack_size;
  char *stack;
} ;
// myth_pickle_t;

#endif	/* MYTH_THREAD_H_ */
