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

#define MAXTDS 1024
chpl_stm_tx_p gtmTxArray[2048][MAXTDS];
static chpl_mutex_t gtmTxArrayLock[MAXTDS];

void gtm_tx_init() {
  int i, j;
  
  for (j = 0; j < MAXTDS; j++) 
    CHPL_MUTEX_INIT(&gtmTxArrayLock[j]);

  for (i = 0; i < NLOCALES; i++)
    for (j = 0; j < MAXTDS; j++) 
      gtmTxArray[i][j] = NULL;
}

chpl_stm_tx_p gtm_tx_create(int32_t txid, int32_t txlocale) {
  chpl_stm_tx_p tx = (chpl_stm_tx_p) chpl_malloc(1, sizeof(chpl_stm_tx_t), 
				     CHPL_RT_MD_STM_TX_DESCRIPTOR, 0, 0);

  tx->counters = chpl_stm_stats_create();

  tx->id = txid;
  tx->locale = txlocale;
  tx->status = TX_IDLE;
  tx->nestlevel = 0;
  tx->numremlocales = -1;
  tx->remlocales = NULL; 
  tx->readset.numentries = 0;
  tx->readset.size = RWSETSIZE;
  tx->readset.reallocate = 0;
  tx->readset.entries = (read_entry_t*) chpl_malloc(tx->readset.size, sizeof(read_entry_t), CHPL_RT_MD_STM_TX_READSET, 0, 0);
  tx->writeset.numentries = 0;
  tx->writeset.size = RWSETSIZE;
  tx->writeset.reallocate = 0;
  tx->writeset.entries = (write_entry_t*) chpl_malloc(tx->writeset.size, sizeof(write_entry_t), CHPL_RT_MD_STM_TX_WRITESET, 0, 0);
  tx->timestamp = GET_CLOCK;
  tx->rollback = false;
  gtm_tx_create_cmgr(tx);
  gtm_tx_create_memset(tx);

  return tx;
}

void gtm_tx_destroy(chpl_stm_tx_p tx) {
  chpl_free(tx->readset.entries, 0, 0);
  chpl_free(tx->writeset.entries, 0, 0);
  gtm_tx_destroy_memset(tx);

  chpl_stm_stats_destroy(tx->counters);

  if (tx->id != -1 && tx->locale == MYLOCALE) {
    assert(tx->numremlocales >= 0 && tx->remlocales != NULL);
    chpl_free(tx->remlocales, 0, 0);
    CHPL_MUTEX_LOCK(&gtmTxArrayLock[tx->id]);
    gtmTxArray[tx->locale][tx->id] = NULL; 
    CHPL_MUTEX_UNLOCK(&gtmTxArrayLock[tx->id]);
  } else {
    assert(tx->numremlocales == -1 && tx->remlocales == NULL);
  }

  chpl_free(tx, 0, 0);
}

void gtm_tx_cleanup(chpl_stm_tx_p tx) {
  int i;
  tx->status = TX_IDLE;
  tx->nestlevel = 0;
  for (i = 0; i <= tx->numremlocales; i++) 
    tx->remlocales[i] = -1;
  tx->numremlocales = -1;
  tx->readset.numentries = 0;
  tx->writeset.numentries = 0;
  tx->rollback = false;
  gtm_tx_cleanup_memset(tx);
#ifdef STM_STATS_CLEAN_ON_ABORT
  chpl_stm_stats_cleanup(tx->counters, 1);
#else
  chpl_stm_stats_cleanup(tx->counters, 0);
#endif
}

void gtm_tx_comm_register(chpl_stm_tx_p tx, int32_t dstlocale) {
  int i;

  assert(tx->status == TX_ACTIVE);
  assert(dstlocale != MYLOCALE);

  if (tx->id == -1) {
    // First time we are doing a remote operation
    // Allocate and initialize remote locales array
    assert(tx->numremlocales == -1 && tx->remlocales == NULL);
    tx->remlocales = (int32_t*) chpl_malloc(NLOCALES, sizeof(int32_t), CHPL_RT_MD_STM_TX_REMLOCALES, 0, 0);
    for (i = 0; i < NLOCALES; i++) 
      tx->remlocales[i] = -1;
    
    // Get a global id for this transaction
    for (i = 0; i < MAXTDS; i++) {
      CHPL_MUTEX_LOCK(&gtmTxArrayLock[i]);
      if (gtmTxArray[tx->locale][i] == NULL) {
	gtmTxArray[tx->locale][i] = tx;
	CHPL_MUTEX_UNLOCK(&gtmTxArrayLock[i]);
	tx->id = i; 
	break;
      }
      CHPL_MUTEX_UNLOCK(&gtmTxArrayLock[i]);
    }
    if (tx->id == -1) 
      chpl_error("Run out of tx descriptors", 0, 0);
  }
 
  assert(tx->remlocales != NULL);

  // Register the remote operation
  for (i = 0; i <= tx->numremlocales; i++) {
    if (tx->remlocales[i] == dstlocale) return;
  }
  tx->numremlocales++;
  tx->remlocales[tx->numremlocales] = dstlocale; 
}

chpl_stm_tx_p gtm_tx_comm_create(int32_t txid, int32_t txlocale, int32_t txstatus) {
  chpl_stm_tx_p tx = NULL;

  assert(txid >= 0 && txid < MAXTDS);
  assert(txlocale != MYLOCALE);
  assert(txstatus != TX_IDLE);
  if (txstatus == TX_ACTIVE) { 
    if ((tx = gtmTxArray[txlocale][txid]) == NULL) {
      tx = gtm_tx_create(txid, txlocale);
      gtmTxArray[txlocale][txid] = tx;
    }
    tx->status = TX_AMACTIVE;
  } else if (txstatus == TX_COMMIT) {
    if ((tx = gtmTxArray[txlocale][txid]) == NULL) {
      chpl_error("Descriptor not found for commit", 0, 0);
    }
    tx->status = TX_AMCOMMIT;
  } else if (txstatus == TX_ABORT) {
    if ((tx = gtmTxArray[txlocale][txid]) == NULL) {
      chpl_error("Descriptor not found for abort.", 0, 0);
    }
    tx->status = TX_AMABORT;
  } else {
    chpl_error("Descriptor setup for comm operation failed.", 0, 0);
  }

  return tx;
}

void gtm_tx_comm_destroy(chpl_stm_tx_p tx) {
  int32_t txid, txlocale;
  assert(tx != NULL); 
  assert(tx->id != -1 && tx->locale != MYLOCALE);
  txid = tx->id;
  txlocale = tx->locale;
  gtm_tx_destroy(tx);
  gtmTxArray[txlocale][txid] = NULL; 
}

void gtm_tx_comm_cleanup(chpl_stm_tx_p tx) {
  assert(tx->id != -1 && tx->locale != MYLOCALE);
  gtm_tx_cleanup(tx);
}
