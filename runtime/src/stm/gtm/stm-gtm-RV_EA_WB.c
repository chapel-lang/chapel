#include <string.h>
#include "chplrt.h"
#include "chplcomm.h"
#include "chpl_mem.h"
#include "chplsys.h"
#include "chpltasks.h"
#include "chplstm.h"
#include "error.h"
#include "stm-gtm.h"
#include "stm-gtm-atomic.h"

#define LOCK_SET_ADDR_OWNED         ((gtm_word_t)OWNED_MASK) // WRITE bit 
#define LOCK_SET_TIMESTAMP(t)       (t << 1)        // Unset WRITE bit
#define LOCK_GET_TIMESTAMP(lock)    (((unsigned long)lock) >> 1) 
#define VERSION_MAX                 (~(gtm_word_t)0 >> 1)

static inline
write_entry_t* gtm_tx_has_write_lock(chpl_stm_tx_p tx, volatile gtm_word_p lock) {
  write_entry_t* wentry;  
  wentry = tx->writeset.entries;
  for (int i = tx->writeset.numentries; i > 0; i--, wentry++) 
    if (wentry->lock == lock) return wentry;
  return NULL;
}

static inline
int gtm_tx_readset_validate(chpl_stm_tx_p tx) {
  return TX_OK;
} 

static inline
int gtm_tx_readset_reallocate(chpl_stm_tx_p tx) {
  if (tx->readset.numentries == tx->readset.size)
    chpl_error("STM Error: Reallocate readset", 0, 0);
  return TX_OK;
}

int gtm_tx_commitPh1(chpl_stm_tx_t* tx) {
  return TX_OK;
}

int gtm_tx_commitPh2(chpl_stm_tx_t* tx) {

  return TX_OK;
}

void gtm_tx_abort(chpl_stm_tx_t* tx) {

}



int gtm_tx_load_word(chpl_stm_tx_t* tx, gtm_word_p dstaddr, gtm_word_p srcaddr) {
  gtm_word_p lock;
  gtm_word_t lockval, lockval2, version;
  read_entry_t* rentry;
  write_entry_t* wentry;
  
  lock = GET_LOCK(srcaddr);
  lockval = ATOMIC_LOAD_MB(lock);
 restart:
  if (LOCK_GET_OWNED(lockval)) { 
    if ((wentry = gtm_tx_has_write_lock(tx, lock))) {
      // this transaction already owns the lock
      while (1) {
	if (wentry->addr == srcaddr) {
	  *dstaddr = wentry->value;
	  return TX_OK;
	}
	if (wentry->next == NULL) {
	  *dstaddr = ATOMIC_LOAD_MB(srcaddr);
	  return TX_OK;
	}
	wentry = wentry->next;
      }
    } else {  
      // another transaction owns the lock
      return TX_FAIL;
    }
  } else {
    // lock is free
    lockval2 = ATOMIC_LOAD_MB(lock);
    if (lockval != lockval2) {
      lockval = lockval2;
      goto restart;
    }    
    version = LOCK_GET_TIMESTAMP(lock);
    // this validation is an overkill
    if (gtm_tx_readset_validate(tx) == TX_FAIL) {
      return TX_FAIL;
    }
    lockval = ATOMIC_LOAD_MB(lock);
    if (lockval != lockval2) goto restart;
  }

  if (gtm_tx_readset_reallocate(tx)) return TX_FAIL;

  // why add to the readset if this is going to fail ?
  // ideally do the validate later and add to readset later
  rentry = &tx->readset.entries[tx->readset.numentries++];
  rentry->version = version;
  rentry->lock = lock;

  return TX_OK;
}
 
int gtm_tx_store_word(chpl_stm_tx_t* tx, gtm_word_p srcaddr, gtm_word_p dstaddr) {
  return TX_OK;
}
