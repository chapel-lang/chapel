#include <string.h>
#include "chplrt.h"
#include "chplcomm.h"
#include "chplexit.h"
#include "error.h"
#include "chpl_mem.h"
#include "chpltasks.h"
#include "chplstm.h"

void chpl_stm_init() { }

void chpl_stm_exit() { }

chpl_stm_tx_p chpl_stm_tx_create() { return NULL; } 

void chpl_stm_tx_destroy(chpl_stm_tx_p tx) { }

void chpl_stm_tx_begin(chpl_stm_tx_p tx) { }

void chpl_stm_tx_abort(chpl_stm_tx_p tx) { }

void chpl_stm_tx_commit(chpl_stm_tx_p tx) { }

chpl_stm_tx_env_p chpl_stm_tx_get_env(chpl_stm_tx_p tx) { return NULL; } 

void chpl_stm_tx_load(chpl_stm_tx_p tx, void* dstaddr, void* srcaddr, size_t size, int ln, chpl_string fn) { 
  memcpy(dstaddr, srcaddr, size);
}

void chpl_stm_tx_store(chpl_stm_tx_p tx, void* srcaddr, void* dstaddr, size_t size, int ln, chpl_string fn) { 
  memcpy(dstaddr, srcaddr, size);
}

void chpl_stm_tx_get(chpl_stm_tx_p tx, void* dstaddr, int32_t srclocale, void* srcaddr, size_t size, int ln, chpl_string fn) {
  chpl_comm_get(dstaddr, srclocale, srcaddr, size, ln, fn);
}

void chpl_stm_tx_put(chpl_stm_tx_p tx, void* srcaddr, int32_t dstlocale, void* dstaddr, size_t size, int ln, chpl_string fn) {
  chpl_comm_put(srcaddr, dstlocale, dstaddr, size, ln, fn);
}

void chpl_stm_tx_fork(chpl_stm_tx_p tx, int locale, chpl_fn_int_t fid, void *arg, int arg_size) {
  chpl_comm_fork(locale, fid, arg, arg_size);
}

void* chpl_stm_tx_malloc(chpl_stm_tx_p tx, size_t number, size_t size, chpl_memDescInt_t description, int32_t ln, chpl_string fn) { 
  return chpl_malloc(number, size, CHPL_RT_MD_STM_TX_MALLOC, ln, fn);
}

void chpl_stm_tx_free(chpl_stm_tx_p tx, void* ptr, int32_t ln, chpl_string fn) { 
  chpl_free(ptr, ln, fn);
}
