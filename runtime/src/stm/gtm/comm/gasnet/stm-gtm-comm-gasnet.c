#include <signal.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "chplrt.h"
#include "chplcomm.h"
#include "chpl_mem.h"
#include "chplsys.h"
#include "chpltasks.h"
#include "chplstm.h"
#include "error.h"
#include "gasnet.h"
#include "comm-gasnet.h"
#include "stm-gtm.h"


void AM_tx_signal(gasnet_token_t token, void* buf, size_t nbytes, int status) {

}

void AM_tx_abort(gasnet_token_t token, void* buf, size_t nbytes) {
}

void AM_tx_commitPh1(gasnet_token_t token, void* buf, size_t nbytes) {
}

void AM_tx_commitPh2(gasnet_token_t token, void* buf, size_t nbytes) {

}

void AM_tx_getdata (gasnet_token_t token, void* buf, size_t nbytes) {

}

void AM_tx_get (gasnet_token_t token, void* buf, size_t nbytes) {

}

void AM_tx_put (gasnet_token_t token, void* buf, size_t nbytes) {

}

void gtm_comm_tx_abort(chpl_stm_tx_p tx) { }

int gtm_comm_tx_commitPh1(chpl_stm_tx_p tx) { 
  return TX_OK;}

int gtm_comm_tx_commitPh2(chpl_stm_tx_p tx) { 
  return TX_OK;
}

int gtm_comm_tx_get(chpl_stm_tx_p tx, void* dstaddr, int32_t tgtlocale, void* srcaddr, size_t size) { 
  return TX_OK; 
}

int gtm_comm_tx_put(chpl_stm_tx_p tx, void* srcaddr, int32_t tgtlocale, void* dstddr, size_t size) { 
  return TX_OK; 
}

