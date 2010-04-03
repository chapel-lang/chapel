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
  tx->txlocale = LOC_UNKNOWN;
  tx->status = TX_IDLE;
  tx->nestlevel = 0;
  tx->remlocales = (int32_t*) chpl_malloc(NLOCS, sizeof(int32_t), CHPL_RT_MD_STM_TX_REMLOCALES, 0, 0);
  tx->readset.numentries = 0;
  tx->readset.size = RW_SET_SIZE;
  tx->readset.reallocate = 0;
  tx->readset.entries = (read_entry_t*) chpl_malloc(tx->readset.size, sizeof(read_entry_t), CHPL_RT_MD_STM_TX_READSET, 0, 0);
  tx->writeset.numentries = 0;
  tx->writeset.size = RW_SET_SIZE;
  tx->writeset.reallocate = 0;
  tx->writeset.entries = (write_entry_t*) chpl_malloc(tx->writeset.size, sizeof(write_entry_t), CHPL_RT_MD_STM_TX_WRITESET, 0, 0);
  tx->timestamp = GET_CLOCK;
}

void chpl_stm_init() { 
  assert(sizeof(chpl_stm_word_t) == sizeof(void*));
  assert(sizeof(chpl_stm_word_t) == sizeof(atomic_t));
  memset((void *)locks, 0, LOCK_ARRAY_SIZE * sizeof(chpl_stm_word_t));
  CLOCK = 0;
}

void chpl_stm_exit() { 

}

chpl_stm_tx_p chpl_stm_tx_create() { 
  chpl_stm_tx_p tx;
  
  tx = (chpl_stm_tx_p) chpl_malloc(1, sizeof(chpl_stm_tx_t), CHPL_RT_MD_STM_TX_CREATE_TD, 0 , 0);

  gtm_tx_initialize(tx, MYLOC);
  
  return tx;
}

void chpl_stm_tx_destroy(chpl_stm_tx_p tx) {
  assert(tx != NULL);
  chpl_free(tx->remlocales, 0, 0);
  chpl_free(tx->readset.entries, 0, 0);
  chpl_free(tx->writeset.entries, 0, 0);
  chpl_free(tx, 0, 0);
}

jmp_buf *chpl_stm_tx_get_env(chpl_stm_tx_p tx) { 
  assert(tx->status == TX_IDLE);
  return &tx->env; 
}

void chpl_stm_tx_begin(chpl_stm_tx_p tx) { 
  assert(tx != NULL);
  tx->nestlevel++;
  if (tx->status == TX_IDLE) {
    assert(tx->nestlevel == 1);
    tx->status = TX_ACTIVE;
  } else if (tx->status == TX_ACTIVE) {
    assert(tx->nestlevel > 1);
    assert(tx->env != NULL);
    chpl_error("FIXME: nested transactions not supported.", 0 ,0);
  } else {
    chpl_error("FIXME: cannot begin transaction here.", 0, 0);
  }
  tx->readset.numentries = 0;
  tx->writeset.numentries = 0;
}

void chpl_stm_tx_commit(chpl_stm_tx_p tx) { 


}

void chpl_stm_tx_abort(chpl_stm_tx_p tx) { 


}

void chpl_stm_tx_load(chpl_stm_tx_p tx, void* dstaddr, void* srcaddr, size_t size, int ln, chpl_string fn) {
  assert(tx != NULL);
  assert(dstaddr != NULL && srcaddr != NULL && size != 0);
  memcpy(dstaddr, srcaddr, size);
}

void chpl_stm_tx_store(chpl_stm_tx_p tx, void* srcaddr, void* dstaddr, size_t size, int ln, chpl_string fn) {
  assert(tx != NULL);
  assert(dstaddr != NULL && srcaddr != NULL && size != 0);
  memcpy(dstaddr, srcaddr, size);
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
