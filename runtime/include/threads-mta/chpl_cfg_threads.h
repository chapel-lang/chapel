#ifndef _chpl_cfg_threads_h_
#define _chpl_cfg_threads_h_

// mutex type
typedef int _chpl_mutex_t;
typedef _chpl_mutex_t *_chpl_mutex_p;

typedef struct {
  sync _int64 is_full;      // also serves as lock - need to acquire before accessing corresponding _syncvar
  sync _int64 signal_full;  // wait for full; signal this when full
  sync _int64 signal_empty; // wait for empty; signal this when empty
} _chpl_sync_aux_t;

typedef struct {
  sync _int64 is_full;      // also serves as lock - need to acquire before writing to corresponding _singlevar
  sync _int64 signal_full;  // wait for full; signal this when full
} _chpl_single_aux_t;

#define _chpl_read_FE(x,lineno,filename) readfe(&((x)->value))
#define _chpl_read_FF(x,lineno,filename) readff(&((x)->value))
#define _chpl_read_XX(x) readxx(&((x)->value))
#define _chpl_write_EF(x,y,lineno,filename) writeef(&((x)->value), (y))
#define _chpl_write_FF(x,y,lineno,filename) writeff(&((x)->value), (y))
#define _chpl_write_XF(x,y) writexf(&((x)->value), (y))
#define _chpl_sync_reset(x) purge(&((x)->value))

#define _chpl_single_read_FF(x,lineno,filename) readff(&((x)->value))
#define _chpl_single_write_EF(x,y,lineno,filename) writeef(&((x)->value), (y))
#define _chpl_single_reset(x) purge(&((x)->value))

// thread-related - used in comm-gasnet
#define _chpl_thread_create(thread, attr, fun, arg) \
  (*fun)(arg)

#endif
