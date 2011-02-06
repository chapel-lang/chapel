#ifndef _threads_pthreads_h_
#define _threads_pthreads_h_

#include <pthread.h>


// type (and default value) used to communicate thread identifiers
// between the threading layer and its users.
typedef pthread_t threadlayer_threadID_t;
#define threadlayer_nullThreadID 0


// Mutexes

typedef pthread_mutex_t      threadlayer_mutex_t;
typedef threadlayer_mutex_t* threadlayer_mutex_p;


// Sync variables

typedef pthread_cond_t threadlayer_condvar_t;

typedef struct {
  threadlayer_condvar_t signal_full;  // wait for full; signal this when full
  threadlayer_condvar_t signal_empty; // wait for empty; signal this when empty
} threadlayer_sync_aux_t;

#endif // _threads_pthreads_h_
