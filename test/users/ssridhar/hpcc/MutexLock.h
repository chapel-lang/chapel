#ifndef _lock_h_
#define _lock_h_

#include "chpltasks.h"
#include "chpl_mem.h"

typedef chpl_mutex_t mutex_t;
typedef chpl_mutex_t* mutex_p;
typedef mutex_p* mutex_pp;

#define mutex_init(lockAddr) mutex_alloc_init((mutex_pp) lockAddr)
#define mutex_lock(lockAddr) CHPL_MUTEX_LOCK((mutex_p) *lockAddr)
#define mutex_unlock(lockAddr) CHPL_MUTEX_UNLOCK((mutex_p) *lockAddr)

void mutex_alloc_init(mutex_pp lockAddr) {
  chpl_mutex_t* temp = chpl_malloc(1, sizeof(chpl_mutex_t), CHPL_RT_MD_CHAPEL_CODE, 0, 0);
  *lockAddr = temp;
  CHPL_MUTEX_INIT((chpl_mutex_t*) *lockAddr);
}

#endif


