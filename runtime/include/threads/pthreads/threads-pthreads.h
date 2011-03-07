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

#endif // _threads_pthreads_h_
