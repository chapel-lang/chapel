#ifndef _threads_pthreads_h_
#define _threads_pthreads_h_

#include <pthread.h>


// type (and default value) used to communicate thread identifiers
// between C code and Chapel code in the runtime.
typedef pthread_t chpl_threadID_t;
#define chpl_nullThreadID 0


// Mutexes

typedef pthread_mutex_t      threadlayer_mutex_t;
typedef threadlayer_mutex_t* threadlayer_mutex_p;


// Sync variables

typedef pthread_cond_t chpl_condvar_t;

typedef struct {
  chpl_condvar_t* signal_full;  // wait for full; signal this when full
  chpl_condvar_t* signal_empty; // wait for empty; signal this when empty
} threadlayer_sync_aux_t;


// Single variables

typedef struct {
  chpl_condvar_t* signal_full;  // wait for full; signal this when full
} threadlayer_single_aux_t;

#endif // _threads_pthreads_h_
