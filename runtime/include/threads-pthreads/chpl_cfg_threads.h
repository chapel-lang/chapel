#ifndef _chpl_cfg_threads_h_
#define _chpl_cfg_threads_h_

#include <pthread.h>

// mutexes
typedef pthread_mutex_t _chpl_mutex_t;
typedef _chpl_mutex_t  *_chpl_mutex_p;

// condition variables
typedef pthread_cond_t    _chpl_condvar_t;
typedef _chpl_condvar_t  *_chpl_condvar_p;

// thread-related
typedef void* (*_chpl_threadfp_t)(void*);  // function pointer
typedef void* _chpl_threadarg_t;           // function argument
typedef pthread_attr_t _chpl_thread_attr_t;// thread attributes
typedef pthread_t _chpl_thread_t;          // thread handle
typedef struct {                           // temporary work space
  _chpl_thread_t thread;                   // thread handle for join/wait
  int            error;                    // to store fork error code
} _chpl_cobegin_wkspace_t;

#define _chpl_thread_create(thread, attr, fun, arg) \
  pthread_create(thread, attr, fun, arg)

#endif
