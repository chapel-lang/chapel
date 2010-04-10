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

void gtm_tx_initialize(chpl_stm_tx_p tx, int32_t srclocale) {
  tx->srclocale = srclocale;
  tx->txlocale = NOLOCALE;
  tx->status = TX_IDLE;
  tx->nestlevel = 0;
  tx->numremlocales = -1;
  tx->remlocales = (int32_t*) chpl_malloc(NLOCALES, sizeof(int32_t), CHPL_RT_MD_STM_TX_REMLOCALES, 0, 0);
  tx->readset.numentries = 0;
  tx->readset.size = RWSETSIZE;
  tx->readset.reallocate = 0;
  tx->readset.entries = (read_entry_t*) chpl_malloc(tx->readset.size, sizeof(read_entry_t), CHPL_RT_MD_STM_TX_READSET, 0, 0);
  tx->writeset.numentries = 0;
  tx->writeset.size = RWSETSIZE;
  tx->writeset.reallocate = 0;
  tx->writeset.entries = (write_entry_t*) chpl_malloc(tx->writeset.size, sizeof(write_entry_t), CHPL_RT_MD_STM_TX_WRITESET, 0, 0);
  tx->timestamp = GET_CLOCK;
}

void gtm_tx_cleanup(chpl_stm_tx_p tx) {
  tx->txlocale = NOLOCALE;
  tx->status = TX_IDLE;
  tx->nestlevel = 0;
  tx->numremlocales = -1;
  tx->readset.numentries = 0;
  tx->writeset.numentries = 0;
}

void chpl_stm_init() { 
  assert(sizeof(gtm_word_t) == sizeof(void*));
  assert(sizeof(gtm_word_t) == sizeof(atomic_t));
  assert(sizeof(gtm_word_t) == sizeof(unsigned long));
  memset((void *)locks, 0, LOCK_ARRAY_SIZE * sizeof(gtm_word_t));
  CLOCK = 0;
}

void chpl_stm_exit() { 

}

chpl_stm_tx_p chpl_stm_tx_create() { 
  chpl_stm_tx_p tx;
  
  tx = (chpl_stm_tx_p) chpl_malloc(1, sizeof(chpl_stm_tx_t), CHPL_RT_MD_STM_TX_CREATE_TD, 0 , 0);

  gtm_tx_initialize(tx, MYLOCALE);
  
  return tx;
}

void chpl_stm_tx_destroy(chpl_stm_tx_p tx) {
  assert(tx != NULL && tx->status == TX_IDLE);
  chpl_free(tx->remlocales, 0, 0);
  chpl_free(tx->readset.entries, 0, 0);
  chpl_free(tx->writeset.entries, 0, 0);
  chpl_free(tx, 0, 0);
}

jmp_buf *chpl_stm_tx_get_env(chpl_stm_tx_p tx) { 
  return (tx->status == TX_IDLE ? &tx->env : NULL); 
}

void chpl_stm_tx_begin(chpl_stm_tx_p tx) { 
  assert(tx != NULL);

  tx->nestlevel++;

  if (tx->status == TX_IDLE) {
    // begin of a new transaction
    assert(tx->nestlevel == 1 && tx->txlocale == NOLOCALE);
    tx->status = TX_ACTIVE;
    tx->txlocale = tx->srclocale;
    tx->timestamp = GET_CLOCK;
    return;
  } else if (tx->status == TX_ACTIVE) {
    // simple flat nested transaction
    assert(tx->nestlevel > 1); 
    return;
  }
  chpl_error("STM Error: cannot begin transaction here.", 0, 0);
}

void chpl_stm_tx_commit(chpl_stm_tx_p tx) { 
  assert(tx != NULL);
    
  if (tx->status == TX_ACTIVE) {
    assert(tx->srclocale == tx->txlocale && tx->srclocale == MYLOCALE);

    // flat nesting, no action required
    if (--tx->nestlevel > 0) 
      return;

    assert(tx->nestlevel == 0);
    tx->status = TX_COMMIT;

    // commit phase 1
    if (tx->numremlocales > -1) { 
      GTM_Safe(tx, gtm_comm_tx_commitPh1(tx));
    }
    GTM_Safe(tx, gtm_tx_commitPh1(tx));

    // commit phase 2
    if (tx->numremlocales > -1) { 
      GTM_Safe(tx, gtm_comm_tx_commitPh2(tx));
    }
    GTM_Safe(tx, gtm_tx_commitPh2(tx));

    // commit success, cleanup descriptor for future use
    gtm_tx_cleanup(tx);
    return;
  } 
  chpl_error("STM Error: cannot commit transaction.", 0, 0);
}

void chpl_stm_tx_abort(chpl_stm_tx_p tx) { 
  assert(tx != NULL);

  if (tx->status == TX_ACTIVE || tx->status == TX_COMMIT) {
    assert(tx->srclocale == tx->txlocale && tx->srclocale == MYLOCALE);

    // aborts always succeed
    if (tx->numremlocales > -1) { 
      gtm_comm_tx_abort(tx);
    }
    gtm_tx_abort(tx);

    // rollback to last checkpoint
    if (tx->env != NULL) {
      chpl_stm_tx_env_p txenvptr = &tx->env;
      gtm_tx_cleanup(tx); 
      longjmp(*txenvptr, 1);
    }
  }
  chpl_error("STM Error: cannot abort transaction.", 0, 0);
}

typedef struct __convert_t {
  gtm_word_t w;
  uint8_t b[GTMWORDSIZE];
} convert_t;

int gtm_tx_load(chpl_stm_tx_t* tx, void* dstaddr, void* srcaddr, size_t size) {
  int mask;
  gtm_word_p saddr, daddr;
  convert_t sval;
  uint8_t* dbyteaddr;

  if ((mask  = (gtm_word_t) srcaddr & (GTMWORDSIZE - 1)) != 0) {
    saddr = (gtm_word_p) ((gtm_word_t) srcaddr & ~(gtm_word_t) (GTMWORDSIZE - 1));
    gtm_tx_load_word(tx, &sval.w, saddr); 
    dbyteaddr = (uint8_t*) dstaddr;
    for (; mask < sizeof(gtm_word_t) && size > 0; mask++, size--)
      *dbyteaddr++ = sval.b[mask];
    daddr = (gtm_word_p) dbyteaddr;
  } else {
    assert(mask == 0);
    saddr = (gtm_word_p) srcaddr;
    daddr = (gtm_word_p) dstaddr; 
  }

  while (size >= GTMWORDSIZE) { 
    gtm_tx_load_word(tx, daddr++, saddr++);
    size -= GTMWORDSIZE;
  }

  if (size > 0) {
    gtm_tx_load_word(tx, &sval.w, saddr); 
    dbyteaddr = (uint8_t*) daddr;
    for (mask = 0; size > 0; mask++, size--)
      *dbyteaddr++ = sval.b[mask];
  }
  return TX_OK;
} 

void chpl_stm_tx_load(chpl_stm_tx_p tx, void* dstaddr, void* srcaddr, size_t size, int ln, chpl_string fn) {
  assert(tx != NULL);
  assert(dstaddr != NULL && srcaddr != NULL);

  if (size == 0) return;
  
  GTM_Safe(tx, gtm_tx_load(tx, dstaddr, srcaddr, size));
}

int gtm_tx_store(chpl_stm_tx_p tx, void* srcaddr, void* dstaddr, size_t size) {
  return TX_OK;
}

void chpl_stm_tx_store(chpl_stm_tx_p tx, void* srcaddr, void* dstaddr, size_t size, int ln, chpl_string fn) {
  assert(tx != NULL);
  assert(dstaddr != NULL && srcaddr != NULL && size != 0);
  
  if (size == 0) return;
  
  memcpy(dstaddr, srcaddr, size);

  //  GTM_Safe(tx, gtm_tx_store(tx, srcaddr, dstaddr, size));
}

void chpl_stm_tx_get(chpl_stm_tx_p tx, void* dstaddr, int32_t srclocale, void* srcaddr, size_t size, int ln, chpl_string fn) {
  chpl_error("FIXME: chpl_stm_tx_get", ln, fn);
}

void chpl_stm_tx_put(chpl_stm_tx_p tx, void* srcaddr, int32_t dstlocale, void* dstaddr, size_t size, int ln, chpl_string fn) {
  chpl_error("FIXME: chpl_stm_tx_put", ln, fn);
}

void* chpl_stm_tx_malloc(chpl_stm_tx_p tx, size_t number, size_t size, chpl_memDescInt_t description, int32_t ln, chpl_string fn) { 
  chpl_error("FIXME: chpl_stm_tx_malloc", ln, fn);
  return NULL;
}

void chpl_stm_tx_free(chpl_stm_tx_p tx, void* ptr, int32_t ln, chpl_string fn) { 
  chpl_error("FIXME: chpl_stm_tx_free", ln, fn);
}
