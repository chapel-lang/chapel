#ifndef _chpl_cfg_threads_h_
#define _chpl_cfg_threads_h_

// bogus mutex type (no mutexes)
typedef int chpl_mutex_t;

typedef chpl_bool chpl_sync_aux_t; // only needs to store the full/empty bit
typedef chpl_sync_aux_t chpl_single_aux_t;

// type (and default value) used to communicate thread identifiers
// between C code and Chapel code in the runtime.
typedef int64_t chpl_threadID_t;
#define chpl_nullThreadID 0

#endif
