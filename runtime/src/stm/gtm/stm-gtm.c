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

    // commit phase 2 -- will always return success.
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

    // aborts always succeeds, local or otherwise. 
    if (tx->numremlocales > -1) { 
      gtm_comm_tx_abort(tx); 
    }
    gtm_tx_abort(tx);

    // rollback to last checkpoint
    if (tx->env != NULL) {
      chpl_stm_tx_env_p txenvptr = &tx->env;
      gtm_tx_cleanup(tx); 
      longjmp(tx->env, 1);
    }
  }
  chpl_error("STM Error: cannot abort transaction.", 0, 0);
}

typedef union __wrapper_t {
  gtm_word_t w;
  uint32_t hw[2];
  uint8_t b[GTMWORDSIZE];
} wrapper_t;

int gtm_tx_load(chpl_stm_tx_t* tx, void* dstaddr, void* srcaddr, size_t size) {
  int align;
  gtm_word_p saddr, daddr;
  wrapper_t dval;
  uint8_t* dbyteaddr;

  if ((align  = (gtm_word_t) srcaddr & (GTMWORDSIZE - 1)) != 0) {
    saddr = (gtm_word_p) ((gtm_word_t) srcaddr & ~(gtm_word_t) (GTMWORDSIZE - 1));
    if (gtm_tx_load_word(tx, &dval.w, saddr) == TX_FAIL)
      return TX_FAIL; 
    dbyteaddr = (uint8_t*) dstaddr;
    for (; align < sizeof(gtm_word_t) && size > 0; align++, size--)
      *dbyteaddr++ = dval.b[align];
    daddr = (gtm_word_p) dbyteaddr;
  } else {
    saddr = (gtm_word_p) srcaddr;
    daddr = (gtm_word_p) dstaddr; 
  }

  while (size >= GTMWORDSIZE) { 
    if (gtm_tx_load_word(tx, daddr++, saddr++) == TX_FAIL) 
      return TX_FAIL;
    size -= GTMWORDSIZE;
  }

  if (size > 0) {
    if (gtm_tx_load_word(tx, &dval.w, saddr) == TX_FAIL) 
      return TX_FAIL; 
    dbyteaddr = (uint8_t*) daddr;
    for (align = 0; size > 0; align++, size--)
      *dbyteaddr++ = dval.b[align];
  }
  return TX_OK;
} 

int gtm_tx_load_wrap(chpl_stm_tx_p tx, void* dstaddr, void* srcaddr, size_t size) {
  if (size >= GTMWORDSIZE) {
    return gtm_tx_load(tx, dstaddr, srcaddr, size); 
  } else if (size == 4 && ((gtm_word_t) srcaddr & (gtm_word_t) 0x03) != 0) {
    chpl_error("STM error: unaligned 32-bit load", 0, 0);
  } else if (size == 4) {
    wrapper_t dval;
    int status; 
    dval.w = 0;
    // attempting a 4-byte load operation. first, read in the entire 
    // 8-byte word by aligning the srcaddr to the nearest 8-byte word 
    // boundary. we do this by masking the lower order three bits from
    // srcaddr. next, determine which half-word was originally requested 
    // by using bit 3 of the address and return its value. 
    status = gtm_tx_load_word(tx, &dval.w, (void*) ((gtm_word_t) srcaddr & ~(gtm_word_t) 0x07));
    *(gtm_word_p)dstaddr = dval.hw[((gtm_word_t) srcaddr & (gtm_word_t) 0x04) >> 2];
    return status; 
  } else {
    chpl_error("STM error: tx load must be multiple of 4 or 8 bytes", 0, 0);
  }
  return TX_FAIL;
} 

void chpl_stm_tx_load(chpl_stm_tx_p tx, void* dstaddr, void* srcaddr, size_t size, int ln, chpl_string fn) {
  assert(tx != NULL);
  assert(dstaddr != NULL && srcaddr != NULL);

  if (size == 0) return;
  
  GTM_Safe(tx, gtm_tx_load_wrap(tx, dstaddr, srcaddr, size));
}

int gtm_tx_store(chpl_stm_tx_p tx, void* srcaddr, void* dstaddr, size_t size) {
  int align;
  gtm_word_p saddr, daddr;

  if ((align  = (gtm_word_t) dstaddr & (GTMWORDSIZE - 1)) != 0) {   
    chpl_error("STM Error: write underflow error", 0, 0);
  } else {
    saddr = (gtm_word_p) srcaddr;
    daddr = (gtm_word_p) dstaddr; 
  }

  while (size >= GTMWORDSIZE) { 
    if (gtm_tx_store_word(tx, saddr++, daddr++, ~(gtm_word_t) 0) == TX_FAIL) 
      return TX_FAIL;
    size -= GTMWORDSIZE;
  }

  if (size > 0) {
    chpl_error("STM Error: write overflow error", 0, 0);
  }

  return TX_OK;
}

int gtm_tx_store_wrap(chpl_stm_tx_p tx, void* srcaddr, void* dstaddr, size_t size) {
  if (size >= GTMWORDSIZE) {
    return gtm_tx_store(tx, srcaddr, dstaddr, size); 
  } else if (size == 4 && ((gtm_word_t) dstaddr & (gtm_word_t) 0x03) != 0) {
    chpl_error("STM error: unaligned 32-bit store", 0, 0);
  } else if (size == 4) {
    wrapper_t sval, mask;
    int status;
    sval.w = mask.w = 0;
    sval.hw[((gtm_word_t) dstaddr & (gtm_word_t) 0x04) >> 2] = *(uint32_t*) srcaddr;
    mask.hw[((gtm_word_t) dstaddr & (gtm_word_t) 0x04) >> 2] = ~(uint32_t) 0;
    status = gtm_tx_store_word(tx, &sval.w, (void*) ((gtm_word_t) dstaddr & ~(gtm_word_t) 0x07), mask.w);      
    return status; 
  } else {
    chpl_error("STM error: tx store must be multiple of 4 or 8 bytes", 0, 0);
  }
  return TX_FAIL;
}

void chpl_stm_tx_store(chpl_stm_tx_p tx, void* srcaddr, void* dstaddr, size_t size, int ln, chpl_string fn) {
  assert(tx != NULL);
  assert(dstaddr != NULL && srcaddr != NULL && size != 0);
  
  if (size == 0) return;

  GTM_Safe(tx, gtm_tx_store_wrap(tx, srcaddr, dstaddr, size));
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
