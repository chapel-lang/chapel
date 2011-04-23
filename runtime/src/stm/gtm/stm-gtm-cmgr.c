#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#include <math.h>
#include "chplrt.h"
#include "chpl-comm.h"
#include "chpl_mem.h"
#include "chplsys.h"
#include "chpl-tasks.h"
#include "chpltimers.h"
#include "chplstm.h"
#include "error.h"
#include "stm-gtm.h"

void gtm_tx_create_cmgr(void* txptr) {
  chpl_stm_tx_p tx = (chpl_stm_tx_p) txptr;
  cmgr_t* cmgr = &tx->cmgr;
  cmgr->policy = GTM_CMGR_BACKOFF;
  srand(rand()); 
  cmgr->backoff = MIN_BACKOFF;
}

void gtm_tx_abort_cmgr(void* txptr) {
  chpl_stm_tx_p tx = (chpl_stm_tx_p) txptr;
  cmgr_t* cmgr = &tx->cmgr;
  unsigned long i, delay;

  switch (cmgr->policy) {
  case GTM_CMGR_RETRY:
    break;
  case GTM_CMGR_BACKOFF:
    delay = rand() % cmgr->backoff;
    if (cmgr->backoff < MAX_BACKOFF)
      cmgr->backoff <<= 1;
    for (i = 0; i < delay; i++) {}
    break;
  }
}
