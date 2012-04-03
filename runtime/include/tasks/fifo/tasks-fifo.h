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
// Sync variables
//
typedef struct {
  volatile chpl_bool  is_full;
  chpl_thread_mutex_t lock;
} chpl_sync_aux_t;

#endif
