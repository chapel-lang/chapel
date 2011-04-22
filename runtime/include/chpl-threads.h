#ifndef _chpl_threads_h_
#define _chpl_threads_h_

#include <stdint.h>

#ifdef CHPL_THREADS_MODEL_H
#include CHPL_THREADS_MODEL_H
#endif

//
// Threads are the execution vehicles for tasks.  This file declares the
// interface by which the tasking layer obtains thread services.
//

//
// Initialize the threading layer.
//
void threadlayer_init(int32_t, uint64_t, void(*)(void*), void(*)(void));

//
// Initialize the threading layer, for a secondary pthread created by,
// in all cases currently, the comm layer.
//
void threadlayer_perPthreadInit(void);

//
// Shut down the threading layer.
//
void threadlayer_exit(void);

//
// Can the thread layer create another thread?
//
chpl_bool threadlayer_can_start_thread(void);

//
// Create a new thread.
//
int threadlayer_thread_create(void*);

//
// Destroy the calling thread.  The threading layer is allowed to return
// without destroying the thread, so this function is really an advisory
// one, to say that the thread is no longer needed.  Also, the threading
// layer is actually prohibited from destroying the thread if doing so
// would leave no threads running on the processor.
//
void threadlayer_thread_destroy(void);

//
// Get the calling thread's unique identifier.
//
threadlayer_threadID_t threadlayer_thread_id(void);

//
// Yield the processor, so that some other thread can run on it.
//
void threadlayer_yield(void);

//
// Thread private data
//
// These set and get a pointer to thread private data associated with
// each thread.  This is for the use of the tasking layer itself.  If
// the threading layer also needs to store some data private to each
// thread, it must make other arrangements to do so.
//
void  threadlayer_set_thread_private_data(void*);
void* threadlayer_get_thread_private_data(void);

//
// Get the maximum number of threads that can exist.
//
uint32_t threadlayer_get_max_threads(void);

//
// Get the number of threads currently in existence.
//
uint32_t threadlayer_get_num_threads(void);

//
// Get the current thread stack size, and the limit on thread stack size.
//
uint64_t threadlayer_call_stack_size(void);
uint64_t threadlayer_call_stack_size_limit(void);

//
// Mutexes, and operations upon them.
//
typedef threadlayer_mutex_t* threadlayer_mutex_p;

void threadlayer_mutex_init(threadlayer_mutex_p);
threadlayer_mutex_p threadlayer_mutex_new(void);
void threadlayer_mutex_lock(threadlayer_mutex_p);
void threadlayer_mutex_unlock(threadlayer_mutex_p);

#endif
