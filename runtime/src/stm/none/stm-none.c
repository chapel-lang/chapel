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

chpl_stm_tx_p chpl_stm_tx_create_local(void) { return NULL; }

void chpl_stm_tx_destroy_local(chpl_stm_tx_p tx) { }

void chpl_stm_tx_begin_local(chpl_stm_tx_p tx) { }

void chpl_stm_tx_commit_local(chpl_stm_tx_p tx) { }

void chpl_stm_tx_abort_local(chpl_stm_tx_p tx) { } 

chpl_stm_tx_p chpl_stm_tx_create() { return NULL; } 

void chpl_stm_tx_destroy(chpl_stm_tx_p tx) { }

void chpl_stm_tx_begin(chpl_stm_tx_p tx) { }

void chpl_stm_tx_abort(chpl_stm_tx_p tx) { }

void chpl_stm_tx_commit(chpl_stm_tx_p tx) { }

jmp_buf *chpl_stm_tx_get_env(chpl_stm_tx_p tx) { return NULL; } 

void chpl_stm_tx_load(chpl_stm_tx_p tx, void* dstaddr, void* srcaddr, size_t size, int lineno, chpl_string filename) { 
  memcpy(dstaddr, srcaddr, size);
}

void chpl_stm_tx_store(chpl_stm_tx_p tx, void* srcaddr, void* dstaddr, size_t size, int lineno, chpl_string filename) { 
  memcpy(dstaddr, srcaddr, size);
}

void chpl_stm_tx_get(chpl_stm_tx_p tx, void* dstaddr, proc_t locale, void* srcaddr, size_t size, int lineno, chpl_string filename) {
  chpl_comm_get(dstaddr, locale, srcaddr, size, lineno, filename);
}

void chpl_stm_tx_put(chpl_stm_tx_p tx, void* srcaddr, proc_t locale, void* dstaddr, size_t size, int lineno, chpl_string filename) {
  chpl_comm_put(srcaddr, locale, dstaddr, size, lineno, filename);
}

void chpl_stm_tx_malloc(chpl_stm_tx_p tx, void* ptr, size_t number, size_t size, chpl_memDescInt_t description, int32_t lineno, chpl_string filename) { 
  ptr = chpl_malloc(number, size, CHPL_RT_MD_STM_TX_MALLOC, lineno, filename);
}

void chpl_stm_tx_free(chpl_stm_tx_p tx, void* ptr, int32_t lineno, chpl_string filename) { 
  chpl_free(ptr, lineno, filename);
}
