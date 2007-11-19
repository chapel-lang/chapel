#ifndef _chpl_cfg_threads_h_
#define _chpl_cfg_threads_h_

#include <stddef.h>

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
typedef _chpl_thread_t _chpl_cobegin_wkspace_t;  // temporary work space

#define _chpl_thread_create(thread, attr, fun, arg) \
  (*fun)(arg)

typedef struct {
  sync _int64 is_full;      // also serves as lock - need to acquire before accessing corresponding _syncvar
  _chpl_condvar_p cv_empty; // wait for empty; signal this when empty
  _chpl_condvar_p cv_full;  // wait for full; signal this when full
} _chpl_sync_aux_t;

#if 0
#define _chpl_mutex_new() NULL
#define _chpl_mutex_init(x) 0
#define _chpl_mutex_lock(x) /*0*/
#define _chpl_mutex_unlock(x) /*0*/
#define _chpl_mutex_destroy(x) 0
#define _chpl_condvar_new(x) NULL
#define _chpl_condvar_init(x) 0
#define _chpl_condvar_destroy(x) 0
#define _chpl_condvar_signal(x) /*0*/
#define _chpl_condvar_wait_full(x,y,z) /*0*/
#define _chpl_condvar_wait_empty(x,y,z) /*0*/
#endif

#define _chpl_write_EF(x,y) writeef(&(x), (y))
//#define _chpl_write_FE(x,y) writefe(&(x), (y))
#define _chpl_write_FF(x,y) writeff(&(x), (y))
#define _chpl_write_XF(x,y) writexf(&(x), (y))
//#define _chpl_write_XE(x,y) writexe(&(x), (y))
#define _chpl_write_XE0(x) purge(&(x))
#define _chpl_read_FE(x) readfe(&(x))
#define _chpl_read_FF(x) readff(&(x))
//#define _chpl_read_XF(x) readxf(&(x))
#define _chpl_read_XX(x) readxx(&(x))

#endif
