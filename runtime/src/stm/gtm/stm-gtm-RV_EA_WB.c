#include <string.h>
#include "chplrt.h"
#include "chpl-comm.h"
#include "chpl_mem.h"
#include "chplsys.h"
#include "chpltasks.h"
#include "chplstm.h"
#include "error.h"
#include "stm-gtm.h"
#include "stm-gtm-atomic.h"
#include "stm-gtm-memory.h"

#define LOCK_SET_TIMESTAMP(t)       (t << 1)        // Unset WRITE bit
#define LOCK_GET_TIMESTAMP(lock)    (((unsigned long)lock) >> 1) 
#define VERSION_MAX                 (~(gtm_word_t)0 >> 1)

static inline
write_entry_t* gtm_tx_has_write_entry(chpl_stm_tx_p tx, volatile gtm_word_p lock) {
  write_entry_t* wentry;  
  int i;

  wentry = tx->writeset.entries;
  for (i = tx->writeset.numentries; i > 0; i--, wentry++) {
    if (wentry->lock == lock) 
      return wentry;
  }
  return NULL;
}

static inline
read_entry_t* gtm_tx_has_read_entry(chpl_stm_tx_p tx, volatile gtm_word_p lock) {
  read_entry_t* rentry;  
  int i;

  rentry = tx->readset.entries;
  for (i = tx->readset.numentries; i > 0; i--, rentry++) {
    if (rentry->lock == lock) 
      return rentry;
  }
  return NULL;
}

static inline
int gtm_tx_readset_validate(chpl_stm_tx_p tx) {
  read_entry_t* rentry;
  gtm_word_t lockval;
  int i;

  rentry = tx->readset.entries;
  for (i = tx->readset.numentries; i > 0; i--, rentry++) {
    lockval = ATOMIC_LOAD_MB(rentry->lock);
    if (LOCK_GET_OWNED(lockval)) {
      if (gtm_tx_has_write_entry(tx, rentry->lock) == NULL) {
	return TX_FAIL;
      }
    } else {
      if (LOCK_GET_TIMESTAMP(lockval) != rentry->version) {
	return TX_FAIL;
      }
    }
  }
  return TX_OK;
} 

static inline
int gtm_tx_resize_readset(chpl_stm_tx_p tx) {
  if (tx->readset.numentries == tx->readset.size)
    chpl_error("STM Error: cannot resize readset", 0, 0);
  return TX_OK;
}

static inline
int gtm_tx_resize_writeset(chpl_stm_tx_p tx) {
  if (tx->writeset.numentries == tx->writeset.size)
    chpl_error("STM Error: cannot resize writeset", 0, 0);
  return TX_OK;
}

int gtm_tx_commitPh1(chpl_stm_tx_t* tx) {
  gtm_word_t commitstamp;
  if (tx->writeset.numentries) {
    commitstamp = FETCH_AND_INC_CLOCK + 1;
    if (commitstamp > VERSION_MAX)
      chpl_error("STM Error: exceeded maximum version number", 0, 0);
    tx->timestamp = commitstamp;
  }
  return gtm_tx_readset_validate(tx);
}

int gtm_tx_commitPh2(chpl_stm_tx_t* tx) {
  write_entry_t* wentry;
  gtm_word_t commitstamp;
  int i;

  if (tx->writeset.numentries) {
    commitstamp = tx->timestamp;
    wentry = tx->writeset.entries;
    for (i = tx->writeset.numentries; i > 0; i--, wentry++) {
      ATOMIC_STORE(wentry->addr, wentry->value);
      if (wentry->next == NULL)
  	ATOMIC_STORE_MB(wentry->lock, LOCK_SET_TIMESTAMP(commitstamp));
    }
  }
  gtm_tx_commit_memset(tx);
  return TX_OK;
}

void gtm_tx_abort(chpl_stm_tx_t* tx) {
  write_entry_t* wentry;
  gtm_word_t version;
  int i;
  
  wentry = tx->writeset.entries;
  for (i = tx->writeset.numentries; i > 0; i--, wentry++) {
    assert(i > 0);
    version = LOCK_SET_TIMESTAMP(wentry->version);
    if (i == 1)
      ATOMIC_STORE_MB(wentry->lock, version);
    else
      ATOMIC_STORE(wentry->lock, version);
  }
  gtm_tx_abort_memset(tx);
  gtm_tx_cleanup(tx);
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
    if ((wentry = gtm_tx_has_write_entry(tx, lock)) != NULL) {
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
    } 
    // another transaction owns the lock
    return TX_FAIL;
  } else {
    // lock is free
    *dstaddr = ATOMIC_LOAD_MB(srcaddr);
    lockval2 = ATOMIC_LOAD_MB(lock);
    if (lockval != lockval2) {
      lockval = lockval2;
      goto restart;
    }    
    version = LOCK_GET_TIMESTAMP(lockval);
    //    if (gtm_tx_readset_validate(tx) == TX_FAIL) return TX_FAIL;
    lockval = ATOMIC_LOAD_MB(lock);
    if (lockval != lockval2) goto restart;
  }

  if (gtm_tx_resize_readset(tx) == TX_FAIL) return TX_FAIL;

  // FIXME why add to the readset if this is going to fail ?
  // ideally do the validate later and add to readset later
  rentry = &tx->readset.entries[tx->readset.numentries++];
  rentry->version = version;
  rentry->lock = lock;

  return TX_OK;
}
 
int gtm_tx_store_word(chpl_stm_tx_t* tx, gtm_word_p srcaddr, gtm_word_p dstaddr, gtm_word_t mask) {
  gtm_word_p lock;
  gtm_word_t lockval, version, value;
  read_entry_t* rentry;
  write_entry_t* prev;
  write_entry_t* wentry;

  assert(mask != 0);

  value = *srcaddr; 
  lock = GET_LOCK(dstaddr);
 restart:
  prev = NULL;
  lockval = ATOMIC_LOAD_MB(lock);
  if (LOCK_GET_OWNED(lockval)) {
    if ((prev = gtm_tx_has_write_entry(tx, lock)) != NULL) {
      // this transaction already owns the lock
      while (1) {
	if (prev->addr == dstaddr) {
	  if (mask != ~(gtm_word_t) 0) {
	    if (prev->mask == 0) 
	      prev->value = ATOMIC_LOAD_MB(dstaddr);
	    value = (prev->value & ~mask) | (value & mask); 
	  } 
	  prev->value = value;
	  prev->mask |= mask;
	  return TX_OK;
	}
	if (prev->next == NULL) 
	  break;
	prev = prev->next;
      }
      if (gtm_tx_resize_writeset(tx) == TX_FAIL)
	return TX_FAIL;
      wentry = &tx->writeset.entries[tx->writeset.numentries];
      prev->next = wentry;
      version = prev->version;
    } else {
      // another transaction owns the lock
      return TX_FAIL;
    }
  } else {
    // lock is free
    version = LOCK_GET_TIMESTAMP(lockval);
    if ((rentry = gtm_tx_has_read_entry(tx, lock)) != NULL) {
      if (rentry->version < version)
        return TX_FAIL;
    }
    if (gtm_tx_resize_writeset(tx) == TX_FAIL) return TX_FAIL;
    wentry = &tx->writeset.entries[tx->writeset.numentries];
    if (ATOMIC_CAS_MB(lock, lockval, (gtm_word_t) OWNED_MASK) == 0)
      goto restart;
  }

  wentry->addr = dstaddr;
  if (mask != ~(gtm_word_t) 0) 
    value = (ATOMIC_LOAD_MB(dstaddr) & ~mask) | (value & mask); 
  wentry->value = value;
  wentry->mask = mask;
  wentry->version = version;
  wentry->lock = lock;
  wentry->next = NULL;
  tx->writeset.numentries++;
  return TX_OK;
}
