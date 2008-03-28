#ifndef _chpl_cfg_threads_h_
#define _chpl_cfg_threads_h_

// bogus mutex type (no mutexes)
typedef int chpl_mutex_t;

typedef _Bool chpl_sync_aux_t;       // only needs to store the full/empty bit
typedef chpl_sync_aux_t chpl_single_aux_t;

#endif
