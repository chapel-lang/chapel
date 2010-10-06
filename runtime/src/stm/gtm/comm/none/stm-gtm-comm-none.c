#include "chplrt.h"
#include "chpl_mem.h"
#include "chplstm.h"
#include "stm-gtm.h"

void gtm_tx_comm_abort(chpl_stm_tx_p tx) { }

int gtm_tx_comm_commitPh1(chpl_stm_tx_p tx) { return TX_FAIL; }

int gtm_tx_comm_commitPh2(chpl_stm_tx_p tx) { return TX_FAIL; }

int gtm_tx_comm_get(chpl_stm_tx_p tx, void* dstaddr, int32_t tgtLoc, void* srcaddr, size_t size) { return TX_FAIL; }

int gtm_tx_comm_put(chpl_stm_tx_p tx, void* srcaddr, int32_t tgtLoc, void* dstddr, size_t size) { return TX_FAIL; }


