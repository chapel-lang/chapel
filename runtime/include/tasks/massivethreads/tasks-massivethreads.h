#ifndef _tasks_myth_h_
#define _tasks_myth_h_

#define CHPL_COMM_YIELD_TASK_WHILE_POLLING

#include "chpl-threads.h"

#define MYTH_IS_PTHREAD 0

#if MYTH_IS_PTHREAD
typedef struct myth_felock {
  pthread_mutex_t mutex[1];
  pthread_cond_t cond[2];
  int status;
} myth_felock_t;
#else
#include "myth/myth.h"
#endif

typedef struct {
  myth_felock_t felock[1];
  volatile int is_full;
} chpl_sync_aux_t;

// type (and default value) used to communicate task identifiers
// between C code and Chapel code in the runtime.
typedef intptr_t chpl_taskID_t;
#define chpl_nullTaskID 0

#endif
