#ifndef _chpl_cfg_threads_h_
#define _chpl_cfg_threads_h_

#include <stdint.h>

// mutex type
typedef int chpl_mutex_t;

typedef struct {
  sync int64_t is_full;      // also serves as lock - need to acquire before accessing corresponding _syncvar
  sync int64_t signal_full;  // wait for full; signal this when full
  sync int64_t signal_empty; // wait for empty; signal this when empty
} chpl_sync_aux_t;

typedef struct {
  sync int64_t is_full;      // also serves as lock - need to acquire before writing to corresponding _singlevar
  sync int64_t signal_full;  // wait for full; signal this when full
} chpl_single_aux_t;

#define _chpl_read_FE(x,y,lineno,filename) (x) = readfe(&((y)->value))
#define _chpl_read_FF(x,y,lineno,filename) (x) = readff(&((y)->value))
#define _chpl_read_XX(x,y) (x) = readxx(&((y)->value))
#define _chpl_write_EF(x,y,lineno,filename) writeef(&((x)->value), (y))
#define _chpl_write_FF(x,y,lineno,filename) writeff(&((x)->value), (y))
#define _chpl_write_XF(x,y) writexf(&((x)->value), (y))
#define _chpl_sync_reset(x) purge(&((x)->value))

#define _chpl_single_read_FF(x,y,lineno,filename) (x) = readff(&((y)->value))
#define _chpl_single_write_EF(x,y,lineno,filename) writeef(&((x)->value), (y))
#define _chpl_single_reset(x) purge(&((x)->value))

#endif
