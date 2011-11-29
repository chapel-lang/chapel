#ifndef _tasks_mta_h_
#define _tasks_mta_h_

#include <stdint.h>

// type (and default value) used to communicate task identifiers
// between C code and Chapel code in the runtime.
typedef int64_t chpl_taskID_t;
#define chpl_nullTaskID 0

//
// Sync variables
//
typedef struct {
  sync int64_t is_full;      // also serves as lock - need to acquire before accessing corresponding _syncvar
  sync int64_t signal_full;  // wait for full; signal this when full
  sync int64_t signal_empty; // wait for empty; signal this when empty
} chpl_sync_aux_t;

//
// Architectural intrinsics
//
#define chpl_read_FE(x) readfe(&((x)->value))
#define chpl_read_FF(x) readff(&((x)->value))
#define chpl_read_XX(x) readxx(&((x)->value))
#define chpl_write_EF(x,y) writeef(&((x)->value), (y))
#define chpl_write_FF(x,y) writeff(&((x)->value), (y))
#define chpl_write_XF(x,y) writexf(&((x)->value), (y))
#define chpl_sync_reset(x) purge(&((x)->value))

#define chpl_single_read_FF(x)    chpl_sync_read_FF(x)
#define chpl_single_read_XX(x)    chpl_sync_read_XX(x)
#define chpl_single_write_EF(x,y) chpl_sync_write_EF(x,y)
#define chpl_single_reset(x)      chpl_sync_reset(x)

#endif
