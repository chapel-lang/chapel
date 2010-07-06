#ifndef _lock_h_
#define _lock_h_

#include "chpltasks.h"
#include "chpl_mem.h"

typedef threadlayer_mutex_t mutex_t;
typedef threadlayer_mutex_t* mutex_p;
typedef mutex_p* mutex_pp;

void mutex_new_init(mutex_pp lockAddr);

#define mutex_init(lockAddr) mutex_new_init((mutex_pp) lockAddr)
#define mutex_lock(lockAddr)  threadlayer_mutex_lock((mutex_p) *lockAddr)
#define mutex_unlock(lockAddr) threadlayer_mutex_unlock((mutex_p) *lockAddr)

void mutex_new_init(mutex_pp lockAddr) {
  threadlayer_mutex_t* temp = chpl_malloc(1, sizeof(threadlayer_mutex_t), CHPL_RT_MD_MUTEX, 0, 0);
  *lockAddr = temp;
  threadlayer_mutex_init((threadlayer_mutex_t*) *lockAddr);
}

#endif
