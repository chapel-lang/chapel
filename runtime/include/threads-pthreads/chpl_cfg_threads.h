#ifndef _chpl_cfg_threads_h_
#define _chpl_cfg_threads_h_

#include <pthread.h>

// mutexes
typedef pthread_mutex_t   chpl_mutex_t;

// condition variables
typedef pthread_cond_t    chpl_condvar_t;
typedef chpl_condvar_t   *chpl_condvar_p;

typedef struct {
  volatile _Bool is_full;
  chpl_mutex_t *lock;          // need to acquire before accessing corresponding _syncvar
  chpl_condvar_p signal_full;  // wait for full; signal this when full
  chpl_condvar_p signal_empty; // wait for empty; signal this when empty
} chpl_sync_aux_t;

typedef struct {
  volatile _Bool is_full;
  chpl_mutex_t *lock;          // need to acquire before writing to corresponding _singlevar
  chpl_condvar_p signal_full;  // wait for full; signal this when full
} chpl_single_aux_t;

#endif
