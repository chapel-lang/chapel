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

void gtm_comm_tx_abort(chpl_stm_tx_p tx) { 
  chpl_error("FIXME: gtm_comm_tx_abort", 0, 0);
}

int gtm_comm_tx_commitPh1(chpl_stm_tx_p tx) { 
  chpl_error("FIXME: gtm_comm_tx_commitPh1", 0, 0);
  return TX_OK;
}

int gtm_comm_tx_commitPh2(chpl_stm_tx_p tx) { 
  chpl_error("FIXME: gtm_comm_tx_commitPh2", 0, 0);
  return TX_OK;
}

int gtm_comm_tx_get(chpl_stm_tx_p tx, void* dstaddr, int32_t srclocale, void* srcaddr, size_t size) { 
  chpl_error("FIXME: gtm_comm_tx_get", 0, 0);
  return TX_OK; 
}

int gtm_comm_tx_put(chpl_stm_tx_p tx, void* srcaddr, int32_t dstlocale, void* dstddr, size_t size) { 
  chpl_error("FIXME: gtm_comm_tx_put", 0, 0);
  return TX_OK; 
}

int gtm_comm_tx_fork(chpl_stm_tx_p tx, int tgtlocale, chpl_fn_int_t fid, void *arg, int arg_size) {
  chpl_error("FIXME: gtm_comm_tx_fork", 0, 0);
  return TX_OK;
}
