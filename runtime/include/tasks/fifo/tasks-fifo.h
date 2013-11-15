#ifndef _tasks_fifo_h_
#define _tasks_fifo_h_

#include <stdint.h>

#include "chpl-threads.h"


//
// Because we use the task tracking table for fifo tasking, this gives
// us the ability to register the main task in the task table and
// block report.
//
#define CHPL_TASK_STD_MODULES_INITIALIZED chpl_task_stdModulesInitialized

void chpl_task_stdModulesInitialized(void);

//
// The FIFO implementation of tasking is a least-common-denominator
// version designed for general usefulness, simplicity and limited
// demands on the threading layer.
//

//
// Type (and default value) used to communicate task identifiers
// between C code and Chapel code in the runtime.
//
typedef uint64_t chpl_taskID_t;
#define chpl_nullTaskID 0

//
// Condition variables
//
typedef pthread_cond_t chpl_thread_condvar_t;


//
// Sync variables
//
typedef struct {
  volatile chpl_bool  is_full;
  chpl_thread_mutex_t lock;
  chpl_thread_condvar_t signal_full;  // wait for full; signal this when full
  chpl_thread_condvar_t signal_empty; // wait for empty; signal this when empty
  //  threadlayer_sync_aux_t tl_aux;
} chpl_sync_aux_t;


//
// The fifo tasking layer doesn't really support sublocales.
//
// Putting these interface function definitions here and marking them
// for inlining makes them cost-free at execution time.
//
#ifdef CHPL_TASK_GETSUBLOC_IMPL_DECL
#error "CHPL_TASK_GETSUBLOC_IMPL_DECL is already defined!"
#else
#define CHPL_TASK_GETSUBLOC_IMPL_DECL 1
#endif
static inline
c_sublocid_t chpl_task_getSubloc(void) {
  return 0;
}


#ifdef CHPL_TASK_SETSUBLOC_IMPL_DECL
#error "CHPL_TASK_SETSUBLOC_IMPL_DECL is already defined!"
#else
#define CHPL_TASK_SETSUBLOC_IMPL_DECL 1
#endif
static inline
void chpl_task_setSubloc(c_sublocid_t subloc) {
  // nothing to do
}


#ifdef CHPL_TASK_GETREQUESTEDSUBLOC_IMPL_DECL
#error "CHPL_TASK_GETREQUESTEDSUBLOC_IMPL_DECL is already defined!"
#else
#define CHPL_TASK_GETREQUESTEDSUBLOC_IMPL_DECL 1
#endif
static inline
c_sublocid_t chpl_task_getRequestedSubloc(void) {
  return c_sublocid_any;
}

#endif
