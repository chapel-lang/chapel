#ifndef _chpl_cfg_threads_h_
#define _chpl_cfg_threads_h_

// bogus mutex type (no mutexes)
typedef int _chpl_mutex_t;
typedef _chpl_mutex_t *_chpl_mutex_p;

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
  sync _int64 signal_empty; // wait for empty; signal this when empty
  sync _int64 signal_full;  // wait for full; signal this when full
} _chpl_sync_aux_t;

#define _chpl_write_EF(x,y,lineno,filename) writeef(&((x)->value), (y))
#define _chpl_write_FF(x,y,lineno,filename) writeff(&((x)->value), (y))
#define _chpl_write_XF(x,y) writexf(&((x)->value), (y))
#define _chpl_write_XE0(x) purge(&((x)->value))
#define _chpl_read_FE(x,lineno,filename) readfe(&((x)->value))
#define _chpl_read_FF(x,lineno,filename) readff(&((x)->value))
#define _chpl_read_XX(x) readxx(&((x)->value))

#endif
