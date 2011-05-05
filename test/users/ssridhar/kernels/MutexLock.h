#ifndef _lock_h_
#define _lock_h_

#include "chpltypes.h"
#include "chpl-threads.h"
#include "chpl_mem.h"

typedef chpl_thread_mutex_t mutex_t;
typedef chpl_thread_mutex_t* mutex_p;
typedef mutex_p* mutex_pp;

void mutex_new_init(mutex_pp lockAddr);

#define mutex_init(lockAddr) mutex_new_init((mutex_pp) lockAddr)
#define mutex_lock(lockAddr)  chpl_thread_mutexLock((mutex_p) *lockAddr)
#define mutex_unlock(lockAddr) chpl_thread_mutexUnlock((mutex_p) *lockAddr)

void mutex_new_init(mutex_pp lockAddr) {
  chpl_thread_mutex_t* temp = chpl_malloc(1, sizeof(chpl_thread_mutex_t), CHPL_RT_MD_MUTEX, 0, 0);
  *lockAddr = temp;
  chpl_thread_mutexInit((chpl_thread_mutex_t*) *lockAddr);
}

#endif
