#ifndef _threads_pthreads_h_
#define _threads_pthreads_h_

#include <pthread.h>
#include <stdint.h>

//
// This is the type used to communicate thread identifiers between the
// threading layer and its users.  Thread identifiers are 64-bit ints
// and should be negative to allow distinguishing them from task
// identifiers when the latter are positive and an ID could be either
// one or the other.
//
typedef int64_t threadlayer_threadID_t;
#define threadlayer_nullThreadID INT64_MIN

typedef pthread_mutex_t threadlayer_mutex_t;

#endif // _threads_pthreads_h_
