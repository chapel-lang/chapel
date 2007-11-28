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

#define _chpl_write_EF(x,y,lineno,filename) \
  do {if (_chpl_sync_wait_empty_and_lock(&((x)->sync_aux),lineno,filename) == 0) { \
        (x)->value = (y); \
        _chpl_sync_mark_and_signal_full(&((x)->sync_aux)); \
        _chpl_sync_unlock(&((x)->sync_aux));} \
      else _printError("invalid mutex", lineno, filename); \
     } while (0)

#define _chpl_write_FF(x,y,lineno,filename) \
  do {if (_chpl_sync_wait_full_and_lock(&((x)->sync_aux),lineno,filename) == 0) { \
        (x)->value = (y); \
        _chpl_sync_mark_and_signal_full(&((x)->sync_aux)); \
        _chpl_sync_unlock(&((x)->sync_aux));} \
      else _printError("invalid mutex", lineno, filename); \
     } while (0)

#define _chpl_write_XF(x,y) \
  do {if (_chpl_sync_lock(&((x)->sync_aux)) == 0) { \
        (x)->value = (y); \
        _chpl_sync_mark_and_signal_full(&((x)->sync_aux)); \
        _chpl_sync_unlock(&((x)->sync_aux));} \
     } while (0)

#define _chpl_write_XE0(x)  \
  do {if (_chpl_sync_lock(&((x)->sync_aux)) == 0) { \
        (x)->value = 0; \
        _chpl_sync_mark_and_signal_empty(&((x)->sync_aux)); \
        _chpl_sync_unlock(&((x)->sync_aux));} \
     } while (0)

// The following macros take advantage of the fact that the various pthread functions
// normally return 0.
#define _chpl_read_FE(x,lineno,filename) \
  (_chpl_sync_wait_full_and_lock(&((x)->sync_aux),lineno,filename) \
     ? _printError("invalid mutex", lineno, filename) \
     : (x)->value | \
       _chpl_sync_mark_and_signal_empty(&((x)->sync_aux)) | \
       _chpl_sync_unlock(&((x)->sync_aux)))

#define _chpl_read_FF(x,lineno,filename) \
  (_chpl_sync_wait_full_and_lock(&((x)->sync_aux),lineno,filename) \
     ? _printError("invalid mutex", lineno, filename) \
     : (x)->value | \
       _chpl_sync_mark_and_signal_full(&((x)->sync_aux) | \
       _chpl_sync_unlock(&((x)->sync_aux)))

#define _chpl_read_XX(x) \
  (_chpl_sync_lock(&((x)->sync_aux)) ? 0 : \
     (x)->value | _chpl_sync_unlock(&((x)->sync_aux)))

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
