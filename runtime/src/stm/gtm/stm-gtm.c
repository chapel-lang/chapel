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
#include "stm-gtm-memory.h"

void chpl_stm_init() { 
  assert(sizeof(gtm_word_t) == sizeof(void*));
  assert(sizeof(gtm_word_t) == sizeof(atomic_t));
  assert(sizeof(gtm_word_t) == sizeof(unsigned long));
  memset((void *)locks, 0, LOCK_ARRAY_SIZE * sizeof(gtm_word_t));
  CLOCK = 0;
  gtm_tx_init();
}

void chpl_stm_exit() { 
}

chpl_stm_tx_p chpl_stm_tx_create() { 
  return gtm_tx_create(-1, MYLOCALE);
}

void chpl_stm_tx_destroy(chpl_stm_tx_p tx) {
  assert(tx != NULL && tx->status == TX_IDLE);
  gtm_tx_destroy(tx);
}

jmp_buf *chpl_stm_tx_get_env(chpl_stm_tx_p tx) { 
  return (tx->status == TX_IDLE ? &tx->env : NULL); 
}

void chpl_stm_tx_begin(chpl_stm_tx_p tx) { 
  assert(tx != NULL);

  tx->nestlevel++;
  if (tx->status == TX_IDLE) {
    // begin of a new transaction
    assert(tx->nestlevel == 1);
    tx->status = TX_ACTIVE;
    tx->timestamp = GET_CLOCK;
    return;
  } else if (tx->status == TX_ACTIVE) {
    // simple flat nested transaction
    assert(tx->nestlevel > 1); 
    return;
  }

  chpl_error("Transaction start failed.", __LINE__, __FILE__);
}

void chpl_stm_tx_commit(chpl_stm_tx_p tx) { 
  assert(tx != NULL);
   
  if (tx->status == TX_ACTIVE) {
    assert(tx->locale == MYLOCALE);
    // flat nesting, no action required
    if (--tx->nestlevel > 0) 
      return;
    assert(tx->nestlevel == 0);
    tx->status = TX_COMMIT;
    // commit phase 1
    if (tx->numremlocales > -1) { 
      GTM_Safe(tx, gtm_tx_comm_commitPh1(tx));
    }
    GTM_Safe(tx, gtm_tx_commitPh1(tx));
    // commit phase 2
    if (tx->numremlocales > -1) { 
      GTM_Safe(tx, gtm_tx_comm_commitPh2(tx));
    }
    GTM_Safe(tx, gtm_tx_commitPh2(tx));
    return;
  }

  chpl_error("Transaction commit failed.", __LINE__, __FILE__);
}

void chpl_stm_tx_abort(chpl_stm_tx_p tx) { 
  assert(tx != NULL);

  if (tx->status == TX_ACTIVE || tx->status == TX_COMMIT) {
    assert(tx->locale == MYLOCALE);
    // aborts always succeeds, local or otherwise. 
    tx->status = TX_ABORT;
    if (tx->numremlocales > -1) { 
      gtm_tx_comm_abort(tx); 
    }
    gtm_tx_abort(tx);
    // rollback to last checkpoint
    if (tx->env != NULL) {
      longjmp(tx->env, 1);
    }
  } else if (tx->status == TX_AMACTIVE || tx->status == TX_AMCOMMIT) {
    assert(tx->locale != MYLOCALE);
    // for fork operations, env is set in the fork_wrapper routine in
    // stm-gtm-comm-*.c so if the transaction fails inside a transactional
    // clone spawned by the fork then rollback to the fork_wrapper.
    // for all other operations, just set the status and return 
    if (tx->env != NULL && tx->rollback) {
      longjmp(tx->env, 1);
    } else {
      tx->status = TX_AMABORT; 
      return;
    }
  } 

  chpl_error("Transaction abort failed.", __LINE__, __FILE__);
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
    dval.w = 0;
    saddr = (gtm_word_p) ((gtm_word_t) srcaddr & ~(gtm_word_t) (GTMWORDSIZE - 1));
    if (gtm_tx_load_word(tx, &dval.w, saddr++) == TX_FAIL)
      return TX_FAIL; 
    dbyteaddr = (uint8_t*) dstaddr;
    for (; align < sizeof(gtm_word_t) && size > 0; align++, size--)
      *dbyteaddr++ = dval.b[align];
    while (size >= GTMWORDSIZE) { 
      dval.w = 0;
      if (gtm_tx_load_word(tx, &dval.w, saddr++) == TX_FAIL) 
	return TX_FAIL; 
      for (align = 0; size > 0; align++, size--)
	*dbyteaddr++ = dval.b[align];
      size -= GTMWORDSIZE;
    }
    daddr = (gtm_word_p) dbyteaddr;
  } else {
    saddr = (gtm_word_p) srcaddr;
    daddr = (gtm_word_p) dstaddr; 
    while (size >= GTMWORDSIZE) { 
      if (gtm_tx_load_word(tx, daddr++, saddr++) == TX_FAIL) 
	return TX_FAIL;
      size -= GTMWORDSIZE;
    }
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
  } else if (size == 4) {
    if (((gtm_word_t) srcaddr & (gtm_word_t) 0x03) != 0) {
      chpl_error("STM error: unaligned 32-bit load", __LINE__, __FILE__);
    } else {
      wrapper_t dval;
      int status; 
      dval.w = 0;
      // attempting a 4-byte load operation. first, read in the entire 
      // 8-byte word by aligning the srcaddr to the nearest 8-byte word 
      // boundary. we do this by masking the lower order three bits from
      // srcaddr. next, determine which half-word was originally requested 
      // by using bit 3 of the address and return its value. 
      status = gtm_tx_load_word(tx, &dval.w, (gtm_word_p) ((gtm_word_t) srcaddr & ~(gtm_word_t) 0x07));
      *(uint32_t*)dstaddr = dval.hw[((gtm_word_t) srcaddr & (gtm_word_t) 0x04) >> 2];
      return status; 
    }
  } else if (size == 1) {
    if (((gtm_word_t) srcaddr & (gtm_word_t) 0x07) != 0) {
      chpl_error("STM error: unaligned 1-byte load", __LINE__, __FILE__);
    } else {
      wrapper_t dval;
      int status;
      dval.w = 0;
      status = gtm_tx_load_word(tx, &dval.w, (void*) ((gtm_word_t) srcaddr & ~(gtm_word_t) 0x07));
      *(uint8_t*)dstaddr = dval.b[(gtm_word_t) srcaddr & (gtm_word_t) 0x07];
      return status;
    }
  } else {
    chpl_error("STM error: tx load size not supported", __LINE__, __FILE__);
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
  wrapper_t sval, mask;
  int align;
  gtm_word_p saddr, daddr;
  uint8_t* sbyteaddr;

  if ((align  = (gtm_word_t) dstaddr & (GTMWORDSIZE - 1)) != 0) {
    sval.w = mask.w = 0;   
    daddr = (gtm_word_p) ((gtm_word_t) dstaddr & ~(gtm_word_t) (GTMWORDSIZE - 1));
    sbyteaddr = (uint8_t*) srcaddr;
    for (; align < sizeof(gtm_word_t) && size > 0; align++, size--) {
      sval.b[align] = *sbyteaddr++;
      mask.b[align] = 0xFF;     
    }
    if (gtm_tx_store_word(tx, &sval.w, daddr++, mask.w) == TX_FAIL) 
      return TX_FAIL;
    while (size >= GTMWORDSIZE) { 
      sval.w = mask.w = 0;
      for (align = 0; size > 0; align++, size--) {
	sval.b[align] = *(uint8_t*) sbyteaddr++;
	mask.b[align] = 0xFF;
      }
      if (gtm_tx_store_word(tx, &sval.w, daddr++, mask.w) == TX_FAIL) 
	return TX_FAIL;
      size -= GTMWORDSIZE;
    }
    saddr = (gtm_word_p) sbyteaddr;
  } else {
    saddr = (gtm_word_p) srcaddr;
    daddr = (gtm_word_p) dstaddr; 
    while (size >= GTMWORDSIZE) { 
      if (gtm_tx_store_word(tx, saddr++, daddr++, ~(gtm_word_t) 0) == TX_FAIL) 
	return TX_FAIL;
      size -= GTMWORDSIZE;
    }
  }

  if (size > 0) {
    sval.w = mask.w = 0;   
    sbyteaddr = (uint8_t*) saddr;
    for (align = 0; size > 0; align++, size--) {
      sval.b[align] = *(uint8_t*) sbyteaddr++;
      mask.b[align] = 0xFF;
    }
    if (gtm_tx_store_word(tx, &sval.w, daddr, mask.w) == TX_FAIL) 
      return TX_FAIL;
  }
  return TX_OK;
}

int gtm_tx_store_wrap(chpl_stm_tx_p tx, void* srcaddr, void* dstaddr, size_t size) {
  if (size >= GTMWORDSIZE) {
    return gtm_tx_store(tx, srcaddr, dstaddr, size); 
  } else if (size == 4) {
    if (((gtm_word_t) dstaddr & (gtm_word_t) 0x03) != 0) {
      chpl_error("STM error: unaligned 32-bit store", __LINE__, __FILE__);
    } else {
      wrapper_t sval, mask;
      int status;
      sval.w = mask.w = 0;
      sval.hw[((gtm_word_t) dstaddr & (gtm_word_t) 0x04) >> 2] = *(uint32_t*) srcaddr;
      mask.hw[((gtm_word_t) dstaddr & (gtm_word_t) 0x04) >> 2] = ~(uint32_t) 0;
      status = gtm_tx_store_word(tx, &sval.w, (void*) ((gtm_word_t) dstaddr & ~(gtm_word_t) 0x07), mask.w);      
      return status;
    }
  } else if (size == 1) {
    wrapper_t sval, mask;
    int status;
    sval.w = mask.w = 0;
    sval.b[(gtm_word_t) dstaddr & (gtm_word_t) 0x07] = *(uint8_t*) srcaddr;
    mask.b[(gtm_word_t) dstaddr & (gtm_word_t) 0x07] = ~(uint8_t) 0;
    status = gtm_tx_store_word(tx, &sval.w, (void*) ((gtm_word_t) dstaddr & ~(gtm_word_t) 0x07), mask.w);      
    return status;   
  } else {
    chpl_error("STM error: tx store size not supported", __LINE__, __FILE__);
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
  assert(tx != NULL);
  assert(srclocale != MYLOCALE);

  if (size == 0) return;
  
  GTM_Safe(tx, gtm_tx_comm_get(tx, dstaddr, srclocale, srcaddr, size));
}

void chpl_stm_tx_put(chpl_stm_tx_p tx, void* srcaddr, int32_t dstlocale, void* dstaddr, size_t size, int ln, chpl_string fn) {
  assert(tx != NULL);
  assert(dstlocale != MYLOCALE);
  
  if (size == 0) return;
  
  GTM_Safe(tx, gtm_tx_comm_put(tx, srcaddr, dstlocale, dstaddr, size));
}

void chpl_stm_tx_fork(chpl_stm_tx_p tx, int dstlocale, chpl_fn_int_t fid, void *arg, size_t argsize) {
  assert(tx != NULL);

  if (dstlocale == MYLOCALE) {
    (*chpl_txftable[fid])(tx, arg);
  } else 
    GTM_Safe(tx, gtm_tx_comm_fork(tx, dstlocale, fid, arg, argsize));
}

void* chpl_stm_tx_malloc(chpl_stm_tx_p tx, size_t number, size_t size, chpl_memDescInt_t description, int32_t ln, chpl_string fn) { 
  return gtm_tx_malloc(tx, number, size, description, ln, fn);
}

void chpl_stm_tx_free(chpl_stm_tx_p tx, void* ptr, int32_t ln, chpl_string fn) { 
  gtm_tx_free(tx, ptr, ln, fn);
}
