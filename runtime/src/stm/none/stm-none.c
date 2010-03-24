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

void chpl_stm_tx_load(chpl_stm_tx_p tx, void* dstaddr, void* srcaddr, size_t size, int ln, chpl_string fn) { 
  memcpy(dstaddr, srcaddr, size);
}

void chpl_stm_tx_store(chpl_stm_tx_p tx, void* srcaddr, void* dstaddr, size_t size, int ln, chpl_string fn) { 
  memcpy(dstaddr, srcaddr, size);
}

void chpl_stm_tx_get(chpl_stm_tx_p tx, void* dstaddr, proc_t locale, void* srcaddr, size_t size, int ln, chpl_string fn) {
  chpl_comm_get(dstaddr, locale, srcaddr, size, ln, fn);
}

void chpl_stm_tx_put(chpl_stm_tx_p tx, void* srcaddr, proc_t locale, void* dstaddr, size_t size, int ln, chpl_string fn) {
  chpl_comm_put(srcaddr, locale, dstaddr, size, ln, fn);
}

