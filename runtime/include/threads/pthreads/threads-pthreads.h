#ifndef _threads_pthreads_h_
#define _threads_pthreads_h_

#include <pthread.h>
#include <stdint.h>


//
// This is the type (and default value) used to communicate thread
// identifiers between the threading layer and its users.  Thread
// identifiers are 64-bit negative integers, to allow distinguishing
// them from task identifiers when the latter are positive and an ID
// could be either one or the other.
//
typedef int64_t threadlayer_threadID_t;
#define threadlayer_nullThreadID INT64_MIN


// Mutexes

typedef pthread_mutex_t      threadlayer_mutex_t;
typedef threadlayer_mutex_t* threadlayer_mutex_p;

#endif // _threads_pthreads_h_
