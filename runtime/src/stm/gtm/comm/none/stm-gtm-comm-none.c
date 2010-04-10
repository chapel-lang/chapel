#include "chplrt.h"
#include "chpl_mem.h"
#include "chplstm.h"
#include "stm-gtm.h"

void gtm_comm_tx_abort(chpl_stm_tx_p tx) { }

int gtm_comm_tx_commitPh1(chpl_stm_tx_p tx) { return SUCCESS; }

int gtm_comm_tx_commitPh2(chpl_stm_tx_p tx) { return SUCCESS; }

int gtm_comm_tx_get(chpl_stm_tx_p tx, void* dstaddr, int32_t tgtLoc, void* srcaddr, size_t size) { return 0; }

int gtm_comm_tx_put(chpl_stm_tx_p tx, void* srcaddr, int32_t tgtLoc, void* dstddr, size_t size) { return 0; }


