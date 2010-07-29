#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>
#include "chplrt.h"
#include "chplcomm.h"
#include "chpl_mem.h"
#include "chplsys.h"
#include "chpltasks.h"
#include "chpltimers.h"
#include "chplstm.h"
#include "error.h"
#include "stm-gtm.h"

#ifdef GTM_STATS

#define DURTOT(num, dur) (num ? dur / 1.0e+6 : 0)
#define DURAVG(num, dur) (num ? dur / (num * 1.0e+6) : 0)

static chpl_mutex_t gtmStatsLock;
static chpl_mutex_t gtmStatsCommLock;

_real64 durSuccess, durFailed;

unsigned int numAbort;
_real64 durAbort;
unsigned int numCommAbort;
_real64 durCommAbort;

unsigned int numCommitPh1;
_real64 durCommitPh1;
unsigned int numCommCommitPh1;
_real64 durCommCommitPh1;

unsigned int numCommitPh2;
_real64 durCommitPh2;
unsigned int numCommCommitPh2;
_real64 durCommCommitPh2;

unsigned int numLoad;
_real64 durLoad;
unsigned int sizeReadSet;

unsigned int numGet;
_real64 durGet;
unsigned int numCommGet;
_real64 durCommGet;

unsigned int numStore;
_real64 durStore;
unsigned int sizeWriteSet;

unsigned int numPut;
_real64 durPut;
unsigned int numCommPut;
_real64 durCommPut;

unsigned int numFork;
_real64 durFork;
unsigned int numCommFork;
_real64 durCommFork;

unsigned int numMalloc;
_real64 durMalloc;

unsigned int numFree;
_real64 durFree;

void gtm_init_stats() {
  CHPL_MUTEX_INIT(&gtmStatsLock);
  CHPL_MUTEX_INIT(&gtmStatsCommLock);
}

void gtm_exit_stats() {
  _real64 durSum, durCommSum;

  durSum = DURTOT(numAbort, durAbort) + 
    DURTOT(numCommitPh1, durCommitPh1) + 
    DURTOT(numCommitPh2, durCommitPh2) + 
    DURTOT(numLoad, durLoad) + 
    DURTOT(numStore, durStore) +
    DURTOT(numGet, durGet) + 
    DURTOT(numPut, durPut) + 
    DURTOT(numFork, durFork) + 
    DURTOT(numMalloc, durMalloc) +
    DURTOT(numFree, durFree);

  durCommSum = DURTOT(numCommAbort, durCommAbort) +
    DURTOT(numCommCommitPh1, durCommCommitPh1) + 
    DURTOT(numCommCommitPh2, durCommCommitPh2) +
    DURTOT(numCommGet, durCommGet) +
    DURTOT(numCommPut, durCommPut) +
    DURTOT(numCommFork, durCommFork);

  fprintf(stdout, "L%d %.3e %.3e %.3e %.3e %.3e\n", MYLOCALE, 
	  durSuccess, durSum, durCommSum, durSum + durCommSum, durFailed);

  fprintf(stdout, "Mn%d %8d %8d %8d %8d\n",
	  MYLOCALE, numLoad, numStore, numMalloc, numFree);

  fprintf(stdout, "Md%d %.2e %.2e %.2e %.2e\n",
	  MYLOCALE, DURTOT(numLoad, durLoad),
	  DURTOT(numStore, durStore),
	  DURTOT(numMalloc, durMalloc),
	  DURTOT(numFree, durFree));

  fprintf(stdout, "Gn%d %8d %8d %8d %8d %8d %8d\n", 
	  MYLOCALE, numGet, numPut, numFork, 
	  numCommitPh1, numCommitPh2, numAbort);

  fprintf(stdout, "Gd%d %.2e %.2e %.2e %.2e %.2e %.2e\n",
	  MYLOCALE, DURTOT(numGet, durGet),
	  DURTOT(numPut, durPut),
	  DURTOT(numFork, durFork), 
	  DURTOT(numCommitPh1, durCommitPh1),
	  DURTOT(numCommitPh2, durCommitPh2),
	  DURTOT(numAbort, durAbort));

  fprintf(stdout, "Cn%d %8d %8d %8d %8d %8d %8d\n", 
	  MYLOCALE, numCommGet, numCommPut, numCommFork, 
	  numCommCommitPh1, numCommCommitPh2, numCommAbort);

  fprintf(stdout, "Cd%d %.2e %.2e %.2e %.2e %.2e %.2e\n",
	  MYLOCALE, DURTOT(numCommGet, durCommGet),
	  DURTOT(numCommPut, durCommPut),
	  DURTOT(numCommFork, durCommFork), 
	  DURTOT(numCommCommitPh1, durCommCommitPh1),
	  DURTOT(numCommCommitPh2, durCommCommitPh2),
	  DURTOT(numCommAbort, durCommAbort));
}

void gtm_tx_create_stats(void* txptr) {
  chpl_stm_tx_p tx = (chpl_stm_tx_p) txptr;
  tx->counters =  (stats_t*) chpl_malloc(1, sizeof(stats_t), 
					 CHPL_RT_MD_STM_TX_STATS, 0, 0);
  tx->counters->begintime = 0;
  tx->counters->starttime = 0;
  gtm_tx_cleanup_stats(tx);
}

void gtm_tx_destroy_stats(void* txptr) {
  chpl_stm_tx_p tx = (chpl_stm_tx_p) txptr;
  assert(tx->counters != NULL);
  chpl_free(tx->counters, 0, 0);
}

void gtm_tx_cleanup_stats(void* txptr) {
  chpl_stm_tx_p tx = (chpl_stm_tx_p) txptr;
  stats_t* counters = tx->counters;

  assert(counters != NULL);
  counters->numCommitPh1 = 0;
  counters->durCommitPh1 = 0;
  counters->numCommCommitPh1 = 0;
  counters->durCommCommitPh1 = 0;
  counters->numLoad = 0;
  counters->durLoad = 0;
  counters->numStore = 0;
  counters->durStore = 0;
  counters->numGet = 0;
  counters->durGet = 0;
  counters->numCommGet = 0;
  counters->durCommGet = 0;
  counters->numPut = 0;
  counters->durPut = 0;
  counters->numCommPut = 0;
  counters->durCommPut = 0;
  counters->numFork = 0;
  counters->durFork = 0;
  counters->numCommFork = 0;
  counters->durCommFork = 0;
  counters->numMalloc = 0;;
  counters->durMalloc = 0;
  counters->numFree = 0;;
  counters->durFree = 0;
}

void gtm_tx_stats_start(void* txptr, int op) {
  chpl_stm_tx_p tx = (chpl_stm_tx_p) txptr;
  stats_t* counters = tx->counters;
  _timervalue t;
  _real64 nowtime;

  //  chpl_msg(0, "OpS%d ", op); 

  t = _now_timer(t);
  nowtime = t.tv_sec + (t.tv_usec / 1.0e+6);

  switch(op) {
  case TX_BEGIN_STATS:
    counters->begintime = nowtime;
    break;
  case TX_ABORT_STATS:
  case TX_COMM_ABORT_STATS:
  case TX_COMMITPH1_STATS:
  case TX_COMM_COMMITPH1_STATS:
  case TX_COMMITPH2_STATS:
  case TX_COMM_COMMITPH2_STATS:
  case TX_LOAD_STATS:
  case TX_STORE_STATS:
  case TX_GET_STATS:
  case TX_COMM_GET_STATS:
  case TX_PUT_STATS:
  case TX_COMM_PUT_STATS:
  case TX_FORK_STATS:
  case TX_COMM_FORK_STATS:
  case TX_MALLOC_STATS:
  case TX_FREE_STATS:   
    counters->starttime = nowtime;
    break;
  default:
    chpl_error("STM Error: Timer not defined", 0, 0);
  }
}

void gtm_tx_stats_stop(void* txptr, int op, int status) {
  chpl_stm_tx_p tx = (chpl_stm_tx_p) txptr;
  stats_t* counters = tx->counters;
  _timervalue t;
  _real64 nowtime;

  //  chpl_msg(0, "OpE%d ", op); 

  if (status != TX_OK) return;
  
  t = _now_timer(t);
  nowtime = t.tv_sec + (t.tv_usec / 1.0e+6);

  switch(op) {
  case TX_ABORT_STATS:
    CHPL_MUTEX_LOCK(&gtmStatsLock);
    durFailed += nowtime - counters->begintime;
    numAbort++;
    durAbort += nowtime - counters->starttime;   
    CHPL_MUTEX_UNLOCK(&gtmStatsLock);
    break;
  case TX_COMM_ABORT_STATS:
    CHPL_MUTEX_LOCK(&gtmStatsCommLock);
    numCommAbort++;
    durCommAbort += nowtime - counters->starttime;   
    CHPL_MUTEX_UNLOCK(&gtmStatsCommLock);
    break;
  case TX_COMMITPH1_STATS:
    counters->numCommitPh1++;
    counters->durCommitPh1 += nowtime - counters->starttime;
    break;
  case TX_COMM_COMMITPH1_STATS:
    counters->numCommCommitPh1++;
    counters->durCommCommitPh1 += nowtime - counters->starttime;
    break;
  case TX_COMMITPH2_STATS:
    CHPL_MUTEX_LOCK(&gtmStatsLock);
    durSuccess += nowtime - counters->begintime;
    numCommitPh1 += counters->numCommitPh1;
    durCommitPh1 += counters->durCommitPh1;
    numCommitPh2++;
    durCommitPh2 += nowtime - counters->starttime;
    numLoad += counters->numLoad;
    durLoad += counters->durLoad;
    numStore += counters->numStore;
    durStore += counters->durStore;
    numGet += counters->numGet;
    durGet += counters->durGet;
    numPut += counters->numPut;
    durPut += counters->durPut;
    numFork += counters->numFork;
    durFork += counters->durFork;
    numMalloc += counters->numMalloc;
    durMalloc += counters->durMalloc;
    numFree += counters->numFree;
    durFree += counters->durFree;
    CHPL_MUTEX_UNLOCK(&gtmStatsLock);
    break;
  case TX_COMM_COMMITPH2_STATS:
    CHPL_MUTEX_LOCK(&gtmStatsCommLock);
    numCommCommitPh1 += counters->numCommCommitPh1;
    durCommCommitPh1 += counters->durCommCommitPh1;
    numCommCommitPh2++;
    durCommCommitPh2 += nowtime - counters->starttime;
    numCommGet += counters->numCommGet;
    durCommGet += counters->durCommGet;
    numCommPut += counters->numCommPut;
    durCommPut += counters->durCommPut;
    numCommFork += counters->numCommFork;
    durCommFork += counters->durCommFork;
    CHPL_MUTEX_UNLOCK(&gtmStatsCommLock);
    break;
  case TX_LOAD_STATS:
    counters->numLoad++;
    counters->durLoad += nowtime - counters->starttime;
    break;
  case TX_STORE_STATS:
    counters->numStore++;
    counters->durStore += nowtime - counters->starttime;
    break;
  case TX_GET_STATS:
    counters->numGet++;
    counters->durGet += nowtime - counters->starttime;
    break;
  case TX_COMM_GET_STATS:
    counters->numCommGet++;
    counters->durCommGet += nowtime - counters->starttime;
    break;
  case TX_PUT_STATS:
    counters->numPut++;
    counters->durPut += nowtime - counters->starttime;
    break;
  case TX_COMM_PUT_STATS:
    counters->numCommPut++;
    counters->durCommPut += nowtime - counters->starttime;
    break;
  case TX_FORK_STATS:
    counters->numFork++;
    counters->durFork += nowtime - counters->starttime;
    break;
  case TX_COMM_FORK_STATS:
    counters->numCommFork++;
    counters->durCommFork += nowtime - counters->starttime;
    break;
  case TX_MALLOC_STATS:
    counters->numMalloc++;
    counters->durMalloc += nowtime - counters->starttime;
    break;
  case TX_FREE_STATS:
    counters->numFree++;
    counters->durFree += nowtime - counters->starttime;
    break;
  default:
    chpl_error("STM Error: Timer not defined", 0, 0);
  }
}

#else

void gtm_init_stats(void) { }
void gtm_exit_stats(void) { }
void gtm_tx_create_stats(void* txpr) { }
void gtm_tx_destroy_stats(void* txpr)  { }
void gtm_tx_cleanup_stats(void* txpr) { }
void gtm_tx_stats_start(void* txpr, int op) { }
void gtm_tx_stats_stop(void* txpr, int op, int status) { }

#endif
