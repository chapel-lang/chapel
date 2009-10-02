#ifndef _chpl_cfg_threads_h_
#define _chpl_cfg_threads_h_

//
// The "pthreads-minimal" threading implementation is for demonstration
// purposes only.  It shows use of the generic threading implementation
// with the minimum use of capabilities from an underlying threading
// layer.
//

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

typedef struct { } threadlayer_sync_aux_t;

typedef struct { } threadlayer_single_aux_t;

#include "chpl_threads_generic.h"

#endif // _chpl_cfg_threads_h_
