#ifndef _chpl_cfg_threads_h_
#define _chpl_cfg_threads_h_

#include <pthread.h>

//
// The generic implementation provides the functions in the threading
// interface.
//
#define GENERIC_THREADING_SATISFIES_INTERFACE 1

//
// Declare required types and bring in chpl_threads_generic.h.
//

typedef pthread_mutex_t chpl_mutex_t;

// type (and default value) used to communicate thread identifiers
// between C code and Chapel code in the runtime.
typedef pthread_t chpl_threadID_t;
#define chpl_nullThreadID 0

typedef pthread_cond_t chpl_condvar_t;

typedef struct {
  chpl_condvar_t* signal_full;  // wait for full; signal this when full
  chpl_condvar_t* signal_empty; // wait for empty; signal this when empty
} threadlayer_sync_aux_t;

typedef struct {
  chpl_condvar_t* signal_full;  // wait for full; signal this when full
} threadlayer_single_aux_t;

#include "chpl_threads_generic.h"

#endif // _chpl_cfg_threads_h_
