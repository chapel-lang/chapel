#ifndef _chpl_cfg_threads_h_
#define _chpl_cfg_threads_h_

#include <pthread.h>

// mutexes
typedef pthread_mutex_t _chpl_mutex_t;
typedef _chpl_mutex_t  *_chpl_mutex_p;

// condition variables
typedef pthread_cond_t    _chpl_condvar_t;
typedef _chpl_condvar_t  *_chpl_condvar_p;

typedef struct {
  volatile _bool is_full;
  _chpl_mutex_p lock;       // need to acquire before accessing corresponding _syncvar
  _chpl_condvar_p cv_empty; // wait for empty; signal this when empty
  _chpl_condvar_p cv_full;  // wait for full; signal this when full
} _chpl_sync_aux_t;

#define _chpl_write_EF(x,y) ((x) = (y))
//#define _chpl_write_FE(x,y) ((x) = (y))
#define _chpl_write_FF(x,y) ((x) = (y))
#define _chpl_write_XF(x,y) ((x) = (y))
//#define _chpl_write_XE(x,y) ((x) = (y))
#define _chpl_write_XE0(x) ((x) = 0)
#define _chpl_read_FE(x) ((x))
#define _chpl_read_FF(x) (x)
//#define _chpl_read_XF(x,y) (((y) = true), (x))
#define _chpl_read_XX(x) (x)

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
