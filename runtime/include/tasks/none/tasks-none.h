#ifndef _tasks_none_h_
#define _tasks_none_h_

typedef chpl_bool chpl_sync_aux_t; // only needs to store the full/empty bit

// type (and default value) used to communicate task identifiers
// between C code and Chapel code in the runtime.
typedef int64_t chpl_taskID_t;
#define chpl_nullTaskID 0

#endif
