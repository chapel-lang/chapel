#ifndef _chpl_cfg_threads_h_
#define _chpl_cfg_threads_h_

// bogus mutex type (no mutexes)
typedef int _chpl_mutex_t;
typedef _chpl_mutex_t *_chpl_mutex_p;

// bogus condition variable type (no condition variables)
typedef int _chpl_condvar_t;
typedef _chpl_condvar_t *_chpl_condvar_p;

// thread-related
typedef void* (*_chpl_threadfp_t)(void*, int);  // function pointer
typedef void* _chpl_threadarg_t;           // function argument
typedef int _chpl_thread_attr_t;           // bogus thread attributes
typedef int _chpl_thread_t;                // bogus thread handle
typedef struct {                           // temporary work space
  _chpl_thread_t thread;                   // thread handle for join/wait
  int            error;                    // to store fork error code
} _chpl_cobegin_wkspace_t;

#define _chpl_thread_create(thread, attr, fun, arg) \
  (*fun)(arg)

#endif
