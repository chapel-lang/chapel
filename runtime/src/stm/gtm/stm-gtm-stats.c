#include <stdio.h>
#include <string.h>
#include "chplrt.h"
#include "chplcomm.h"
#include "chpl_mem.h"
#include "chplsys.h"
#include "chpltasks.h"
#include "chpltimers.h"
#include "chplstm.h"
#include "error.h"
#include "stm-gtm.h"

static chpl_mutex_t gtmStatsTxAbortLock;
unsigned long numAbort;
_real64 durAbort;
unsigned long numCommAbort;
_real64 durCommAbort;

static chpl_mutex_t gtmStatsTxCommitPh1Lock;
unsigned long numCommitPh1;
_real64 durCommitPh1;
unsigned long numCommCommitPh1;
_real64 durCommCommitPh1;

static chpl_mutex_t gtmStatsTxCommitPh2Lock;
unsigned long numCommitPh2;
_real64 durCommitPh2;
unsigned long numCommCommitPh2;
_real64 durCommCommitPh2;

static chpl_mutex_t gtmStatsTxLoadLock;
unsigned long numLoad;
_real64 durLoad;

static chpl_mutex_t gtmStatsTxGetLock;
unsigned long numGet;
_real64 durGet;
unsigned long numCommGet;
_real64 durCommGet;

static chpl_mutex_t gtmStatsTxStoreLock;
unsigned long numStore;
_real64 durStore;

static chpl_mutex_t gtmStatsTxPutLock;
unsigned long numPut;
_real64 durPut;
unsigned long numCommPut;
_real64 durCommPut;

static chpl_mutex_t gtmStatsTxForkLock;
unsigned long numFork;
_real64 durFork;
unsigned long numCommFork;
_real64 durCommFork;

void gtm_stats_init () {
  CHPL_MUTEX_INIT(&gtmStatsTxAbortLock);
  CHPL_MUTEX_INIT(&gtmStatsTxCommitPh1Lock);
  CHPL_MUTEX_INIT(&gtmStatsTxCommitPh2Lock);
  CHPL_MUTEX_INIT(&gtmStatsTxLoadLock);
  CHPL_MUTEX_INIT(&gtmStatsTxGetLock);
  CHPL_MUTEX_INIT(&gtmStatsTxStoreLock);
  CHPL_MUTEX_INIT(&gtmStatsTxPutLock);
  CHPL_MUTEX_INIT(&gtmStatsTxForkLock);
}

#define tottime(numcalls, ttime) (numcalls ? ttime / 1.0e+6 : 0)
#define avgtime(numcalls, ttime) (numcalls ? ttime / (numcalls * 1.0e+6) : 0)

void gtm_stats_exit () {
  _real64 totaltime = 0;
  totaltime = tottime(numAbort, durAbort) + tottime(numCommAbort, durCommAbort) +
    tottime(numCommitPh1, durCommitPh1) + tottime(numCommCommitPh1, durCommCommitPh1) + 
    tottime(numCommitPh2, durCommitPh2) + tottime(numCommCommitPh2, durCommCommitPh2) +
    tottime(numLoad, durLoad) + tottime(numStore, durStore) +
    tottime(numGet, durGet) + tottime(numCommGet, durCommGet) +
    tottime(numPut, durPut) + tottime(numCommPut, durCommPut) +
    tottime(numFork, durFork) + tottime(numCommFork, durCommFork);

  fprintf(stdout, "[L%d %.4e] (%lu %.3e %lu %.3e) (%lu %.3e %lu %.3e) "
	  "(%lu %.3e %lu %.3e) (%lu %.3e %lu %.3e) (%lu %.3e %lu %.3e) "
	  "(%lu %.3e %lu %.3e) (%lu %.3e %lu %.3e)\n", 
	  MYLOCALE, totaltime, numAbort, tottime(numAbort, durAbort),
	  numCommAbort, tottime(numCommAbort, durCommAbort),
	  numCommitPh1, tottime(numCommitPh1, durCommitPh1), 
	  numCommCommitPh1, tottime(numCommCommitPh1, durCommCommitPh1), 
	  numCommitPh2, tottime(numCommitPh2, durCommitPh2), 
	  numCommCommitPh2, tottime(numCommCommitPh2, durCommCommitPh2),
	  numLoad, tottime(numLoad, durLoad), 
	  numStore, tottime(numStore, durStore),
	  numGet, tottime(numGet, durGet),
	  numCommGet, tottime(numCommGet, durCommGet),
	  numPut, tottime(numPut, durPut), 
	  numCommPut, tottime(numCommPut, durCommPut),
	  numFork, tottime(numFork, durFork), 
	  numCommFork, tottime(numCommFork, durCommFork));
}

void gtm_stats_tx_abort (_real64 duration) {
  CHPL_MUTEX_LOCK(&gtmStatsTxAbortLock);
  numAbort++;
  durAbort += duration;
  CHPL_MUTEX_UNLOCK(&gtmStatsTxAbortLock);
}

void gtm_stats_tx_comm_abort (_real64 duration) {
  CHPL_MUTEX_LOCK(&gtmStatsTxAbortLock);
  numCommAbort++;
  durCommAbort += duration;
  CHPL_MUTEX_UNLOCK(&gtmStatsTxAbortLock);
}

void gtm_stats_tx_commitPh1 (_real64 duration) {
  CHPL_MUTEX_LOCK(&gtmStatsTxCommitPh1Lock);
  numCommitPh1++;
  durCommitPh1 += duration;
  CHPL_MUTEX_UNLOCK(&gtmStatsTxCommitPh1Lock);
}

void gtm_stats_tx_comm_commitPh1 (_real64 duration) {
  CHPL_MUTEX_LOCK(&gtmStatsTxCommitPh1Lock);
  numCommCommitPh1++;
  durCommCommitPh1 += duration;
  CHPL_MUTEX_UNLOCK(&gtmStatsTxCommitPh1Lock);
}

void gtm_stats_tx_commitPh2 (_real64 duration) {
  CHPL_MUTEX_LOCK(&gtmStatsTxCommitPh2Lock);
  numCommitPh2++;
  durCommitPh2 += duration;
  CHPL_MUTEX_UNLOCK(&gtmStatsTxCommitPh2Lock);
}

void gtm_stats_tx_comm_commitPh2 (_real64 duration) {
  CHPL_MUTEX_LOCK(&gtmStatsTxCommitPh2Lock);
  numCommCommitPh2++;
  durCommCommitPh2 += duration;
  CHPL_MUTEX_UNLOCK(&gtmStatsTxCommitPh2Lock);
}

void gtm_stats_tx_load (_real64 duration) {
  CHPL_MUTEX_LOCK(&gtmStatsTxLoadLock);
  numLoad++;
  durLoad += duration;
  CHPL_MUTEX_UNLOCK(&gtmStatsTxLoadLock);
}

void gtm_stats_tx_store (_real64 duration) {
  CHPL_MUTEX_LOCK(&gtmStatsTxStoreLock);
  numStore++;
  durStore += duration;
  CHPL_MUTEX_UNLOCK(&gtmStatsTxStoreLock);
}

void gtm_stats_tx_get (_real64 duration) {
  CHPL_MUTEX_LOCK(&gtmStatsTxGetLock);
  numGet++;
  durGet += duration;
  CHPL_MUTEX_UNLOCK(&gtmStatsTxGetLock);
}

void gtm_stats_tx_comm_get (_real64 duration) {
  CHPL_MUTEX_LOCK(&gtmStatsTxGetLock);
  numCommGet++;
  durCommGet += duration;
  CHPL_MUTEX_UNLOCK(&gtmStatsTxGetLock);
}

void gtm_stats_tx_put (_real64 duration) {
  CHPL_MUTEX_LOCK(&gtmStatsTxPutLock);
  numPut++;
  durPut += duration;
  CHPL_MUTEX_UNLOCK(&gtmStatsTxPutLock);
}

void gtm_stats_tx_comm_put (_real64 duration) {
  CHPL_MUTEX_LOCK(&gtmStatsTxPutLock);
  numCommPut++;
  durCommPut += duration;
  CHPL_MUTEX_UNLOCK(&gtmStatsTxPutLock);
}

void gtm_stats_tx_fork (_real64 duration) {
  CHPL_MUTEX_LOCK(&gtmStatsTxForkLock);
  numFork++;
  durFork += duration;
  CHPL_MUTEX_UNLOCK(&gtmStatsTxForkLock);
}

void gtm_stats_tx_comm_fork (_real64 duration) {
  CHPL_MUTEX_LOCK(&gtmStatsTxForkLock);
  numCommFork++;
  durCommFork += duration;
  CHPL_MUTEX_UNLOCK(&gtmStatsTxForkLock);
}
