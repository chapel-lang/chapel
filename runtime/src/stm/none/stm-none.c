#include <string.h>
#include "chplrt.h"
#include "chplcomm.h"
#include "chplexit.h"
#include "error.h"
#include "chpl_mem.h"
#include "chpltasks.h"
#include "chplstm.h"

static void stm_none_comm_register(chpl_stm_tx_p tx, int32_t locale) {
  int32_t i;

  for (i = 0; i <= tx->numremlocales; i++) {
    if (tx->remlocales[i] == locale) return;
  }
  tx->numremlocales++;
  tx->remlocales[tx->numremlocales] = locale; 
}

void chpl_stm_init() {
  chpl_stm_stats_init();
}


void chpl_stm_exit() { 
  chpl_stm_stats_exit();
}

chpl_stm_tx_env_p chpl_stm_tx_get_env(chpl_stm_tx_p tx) { return NULL; } 

chpl_stm_tx_p chpl_stm_tx_create() { 
  chpl_stm_tx_p tx = (chpl_stm_tx_p) chpl_malloc(1, sizeof(chpl_stm_tx_t), 
						 CHPL_RT_MD_STM_TX_DESCRIPTOR,
						 0, 0);

  tx->remlocales = (int32_t*) chpl_malloc(NLOCALES, sizeof(int32_t), 
					  CHPL_RT_MD_STM_TX_REMLOCALES, 0, 0);
  tx->numremlocales = -1;
  tx->counters = chpl_stm_stats_create();
  return tx;
} 

void chpl_stm_tx_destroy(chpl_stm_tx_p tx) { 
  chpl_stm_stats_destroy(tx->counters);
  chpl_free(tx, 0, 0);
}

void chpl_stm_tx_begin(chpl_stm_tx_p tx) { 
  CHPL_STM_STATS_START(tx->counters, STATS_TX_BEGIN);
}

void chpl_stm_tx_commit(chpl_stm_tx_p tx) { 
  CHPL_STM_STATS_START(tx->counters, STATS_TX_COMMITPH2);
  CHPL_STM_STATS_STOP(tx->counters, STATS_TX_COMMITPH2, tx->numremlocales);
}

void chpl_stm_tx_abort(chpl_stm_tx_p tx) { 
  CHPL_STM_STATS_START(tx->counters, STATS_TX_ABORT);
  CHPL_STM_STATS_STOP(tx->counters, STATS_TX_ABORT, tx->numremlocales);
}

void chpl_stm_tx_load(chpl_stm_tx_p tx, void* dstaddr, void* srcaddr, size_t size, int ln, chpl_string fn) { 
  assert(tx != NULL);
  assert(dstaddr != NULL && srcaddr != NULL && size > 0);
  CHPL_STM_STATS_START(tx->counters, STATS_TX_LOAD);
  memcpy(dstaddr, srcaddr, size);
  CHPL_STM_STATS_STOP(tx->counters, STATS_TX_LOAD, size);
}

void chpl_stm_tx_store(chpl_stm_tx_p tx, void* srcaddr, void* dstaddr, size_t size, int ln, chpl_string fn) { 
  assert(tx != NULL);
  assert(dstaddr != NULL && srcaddr != NULL && size > 0);
  CHPL_STM_STATS_START(tx->counters, STATS_TX_STORE);
  memcpy(dstaddr, srcaddr, size);
  CHPL_STM_STATS_STOP(tx->counters, STATS_TX_STORE, size);
}

void chpl_stm_tx_get(chpl_stm_tx_p tx, void* dstaddr, int32_t srclocale, void* srcaddr, size_t size, int ln, chpl_string fn) {
  assert(tx != NULL);
  assert(srclocale != chpl_localeID);
  assert(dstaddr != NULL && srcaddr != NULL && size > 0);
  stm_none_comm_register(tx, srclocale);
  CHPL_STM_STATS_START(tx->counters, STATS_TX_GET);
  chpl_comm_get(dstaddr, srclocale, srcaddr, size, ln, fn);
  CHPL_STM_STATS_STOP(tx->counters, STATS_TX_GET, size);
}

void chpl_stm_tx_put(chpl_stm_tx_p tx, void* srcaddr, int32_t dstlocale, void* dstaddr, size_t size, int ln, chpl_string fn) {
  assert(tx != NULL);
  assert(dstlocale != chpl_localeID);
  assert(dstaddr != NULL && srcaddr != NULL && size > 0); 
  stm_none_comm_register(tx, dstlocale);
  CHPL_STM_STATS_START(tx->counters, STATS_TX_PUT);
  chpl_comm_put(srcaddr, dstlocale, dstaddr, size, ln, fn);
  CHPL_STM_STATS_STOP(tx->counters, STATS_TX_PUT, size);
}

void chpl_stm_tx_fork(chpl_stm_tx_p tx, int dstlocale, chpl_fn_int_t fid, void *arg, size_t argsize) {
  assert(tx != NULL);
  if (dstlocale == chpl_localeID) {
    (*chpl_txftable[fid])(tx, arg);
  } else {
    stm_none_comm_register(tx, dstlocale);
    CHPL_STM_STATS_START(tx->counters, STATS_TX_FORK);
    chpl_comm_fork(dstlocale, fid, arg, argsize);
    CHPL_STM_STATS_STOP(tx->counters, STATS_TX_FORK, 0);
  }
}

void* chpl_stm_tx_malloc(chpl_stm_tx_p tx, size_t number, size_t size, chpl_memDescInt_t description, int32_t ln, chpl_string fn) { 
  void *tmp;
  CHPL_STM_STATS_START(tx->counters, STATS_TX_MALLOC);
  tmp = chpl_malloc(number, size, description, ln, fn);
  CHPL_STM_STATS_STOP(tx->counters, STATS_TX_MALLOC, size);
  return tmp;
}

void chpl_stm_tx_free(chpl_stm_tx_p tx, void* ptr, int32_t ln, chpl_string fn) { 
  CHPL_STM_STATS_START(tx->counters, STATS_TX_FREE);
  chpl_free(ptr, ln, fn);
  CHPL_STM_STATS_STOP(tx->counters, STATS_TX_FREE, 0);
}
