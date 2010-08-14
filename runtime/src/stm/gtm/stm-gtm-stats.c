#include <stdio.h>
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

#define TOTDUR(num, dur) (dur) // (num ? dur / 1.0e+6 : 0)
#define AVGDUR(num, dur) (num ? dur / (num * 1.0e+6) : 0)
#define AVGNUM(num, tot) (num ? (tot * 1.0) / (num * 1.0) : 0)
#define MAX(num1,num2)   (((num1) > (num2)) ? (num1) : (num2))

static chpl_mutex_t gtmStatsLock;
static chpl_mutex_t gtmStatsCommLock;

unsigned int numSuccess;
_real64 durSuccess;
_real64 durFailed;
_real64 durCreate;

unsigned int maxLocales;
unsigned int numLocales;

unsigned int numAbort;
_real64 durAbort;
_real64 durFail;
unsigned int numMAbort;
_real64 durMAbort;
_real64 durMFail;
unsigned int numCommAbort;
_real64 durCommAbort;

unsigned int numMCommitPh1;
_real64 durMCommitPh1;
unsigned int numCommitPh1;
_real64 durCommitPh1;
unsigned int numCommCommitPh1;
_real64 durCommCommitPh1;

unsigned int numMCommitPh2;
_real64 durMCommitPh2;
unsigned int numCommitPh2;
_real64 durCommitPh2;
unsigned int numCommCommitPh2;
_real64 durCommCommitPh2;

unsigned int numLoad;
_real64 durLoad;
unsigned int sizeLoad;
unsigned int maxLoad; 

unsigned int numGet;
_real64 durGet;
unsigned int sizeGet;
unsigned int maxGet;

unsigned int numCommGet;
_real64 durCommGet;

unsigned int numStore;
_real64 durStore;
unsigned int sizeStore;
unsigned int maxStore;

unsigned int numPut;
_real64 durPut;
unsigned int sizePut;
unsigned int maxPut;

unsigned int numCommPut;
_real64 durCommPut;

unsigned int numFork;
_real64 durFork;
unsigned int numCommFork;
_real64 durCommFork;

unsigned int numMalloc;
_real64 durMalloc;
unsigned int sizeMalloc;
unsigned int maxMalloc;

unsigned int numFree;
_real64 durFree;

void gtm_init_stats() {
  CHPL_MUTEX_INIT(&gtmStatsLock);
  CHPL_MUTEX_INIT(&gtmStatsCommLock);
}

void gtm_exit_stats() {
  unsigned int numMSum, numSum, numCommSum; 
  _real64 durMSum, durSum, durCommSum;

  if (!printStmStats) return;

  numMSum = //numMAbort + 
    numMCommitPh1 + numMCommitPh2 + 
    numLoad + numStore + numMalloc + numFree;

  durMSum = //TOTDUR(numMAbort, durMAbort) + 
    TOTDUR(numMCommitPh1, durMCommitPh1) + 
    TOTDUR(numMCommitPh2, durMCommitPh2) + 
    TOTDUR(numLoad, durLoad) + 
    TOTDUR(numStore, durStore) +
    TOTDUR(numMalloc, durMalloc) +
    TOTDUR(numFree, durFree);

  numSum = //numAbort + 
    numCommitPh1 + numCommitPh2 + 
    numGet + numPut + numFork;

  durSum = //TOTDUR(numAbort, durAbort) + 
    TOTDUR(numCommitPh1, durCommitPh1) + 
    TOTDUR(numCommitPh2, durCommitPh2) + 
    TOTDUR(numGet, durGet) + 
    TOTDUR(numPut, durPut) + 
    TOTDUR(numFork, durFork);

  numCommSum = //numCommAbort + 
    numCommCommitPh1 + numCommCommitPh2 +
    numCommGet + numCommPut + numCommFork;

  durCommSum = //TOTDUR(numCommAbort, durCommAbort) +
    TOTDUR(numCommCommitPh1, durCommCommitPh1) + 
    TOTDUR(numCommCommitPh2, durCommCommitPh2) +
    TOTDUR(numCommGet, durCommGet) +
    TOTDUR(numCommPut, durCommPut) +
    TOTDUR(numCommFork, durCommFork);

  fprintf(stdout, "-%dLn%d- %8d %8d %8d %8d %8d %8d %8d %8d\n",
	  NLOCALES, MYLOCALE, numSuccess, numSuccess,
	  numMSum, numSum, numCommSum, 
	  numMAbort + numAbort, numMAbort, numAbort);   

  fprintf(stdout, "-%dLd%d- %.2e %.2e %.2e %.2e %.2e %.2e %.2e %.2e\n", 
	  NLOCALES, MYLOCALE, durSuccess, durCreate, 
	  durMSum, durSum, durCommSum, durFailed, durMFail, durFail);

  fprintf(stdout, "-%dMn%d- %8d %8d %8d %8d %8d %8d %8d\n",
	  NLOCALES, MYLOCALE, numLoad, numStore, numMalloc, 
	  numFree, numMCommitPh1, numMCommitPh2, numMAbort);

  fprintf(stdout, "-%dMd%d- %.2e %.2e %.2e %.2e %.2e %.2e %.2e\n",
	  NLOCALES, MYLOCALE, TOTDUR(numLoad, durLoad),
	  TOTDUR(numStore, durStore),
	  TOTDUR(numMalloc, durMalloc),
	  TOTDUR(numFree, durFree),
	  TOTDUR(numMCommitPh1, durMCommitPh1),
          TOTDUR(numMCommitPh2, durMCommitPh2),
	  TOTDUR(numMAbort, durMAbort));

  fprintf(stdout, "-%dGn%d- %8d %8d %8d %8d %8d %8d\n", 
	  NLOCALES, MYLOCALE, numGet, numPut, numFork, 
	  numCommitPh1, numCommitPh2, numAbort);

  fprintf(stdout, "-%dGd%d- %.2e %.2e %.2e %.2e %.2e %.2e\n",
	  NLOCALES, MYLOCALE, TOTDUR(numGet, durGet),
	  TOTDUR(numPut, durPut),
	  TOTDUR(numFork, durFork), 
	  TOTDUR(numCommitPh1, durCommitPh1),
	  TOTDUR(numCommitPh2, durCommitPh2),
	  TOTDUR(numAbort, durAbort));

  fprintf(stdout, "-%dCn%d- %8d %8d %8d %8d %8d %8d\n", 
	  NLOCALES, MYLOCALE, numCommGet, numCommPut, numCommFork, 
	  numCommCommitPh1, numCommCommitPh2, numCommAbort);

  fprintf(stdout, "-%dCd%d- %.2e %.2e %.2e %.2e %.2e %.2e\n",
	  NLOCALES, MYLOCALE, TOTDUR(numCommGet, durCommGet),
	  TOTDUR(numCommPut, durCommPut),
	  TOTDUR(numCommFork, durCommFork), 
	  TOTDUR(numCommCommitPh1, durCommCommitPh1),
	  TOTDUR(numCommCommitPh2, durCommCommitPh2),
	  TOTDUR(numCommAbort, durCommAbort));

  fprintf(stdout, "-%dSa%d- %8.2f %8.2f %8.2f %8.2f %8.2f %8.2f\n",
	  NLOCALES, MYLOCALE, AVGNUM(numLoad, sizeLoad), 
	  AVGNUM(numStore, sizeStore), AVGNUM(numMalloc, sizeMalloc), 
	  AVGNUM(numGet, sizeGet), AVGNUM(numPut, sizePut), 
	  AVGNUM(numCommitPh2, numLocales));

  fprintf(stdout, "-%dSc%d- %8.2f %8.2f %8.2f %8.2f %8.2f %8.2f\n",
	  NLOCALES, MYLOCALE, AVGNUM(numSuccess, sizeLoad), 
	  AVGNUM(numSuccess, sizeStore), AVGNUM(numSuccess, sizeMalloc), 
	  AVGNUM(numSuccess, sizeGet), AVGNUM(numSuccess, sizePut), 
	  AVGNUM(numCommitPh2, numLocales));

  fprintf(stdout, "-%dSm%d- %8d %8d %8d %8d %8d %8d\n",
	  NLOCALES, MYLOCALE, maxLoad, maxStore, 
	  maxMalloc, maxGet, maxPut, maxLocales);
}

void gtm_tx_create_stats(void* txptr) {
  chpl_stm_tx_p tx = (chpl_stm_tx_p) txptr;
  stats_t* counters;
  _timervalue t;
  _real64 nowtime;

  if (!chpl_stm_stats) return;

  printStmStats = 1;

  t = _now_timer(t);
  nowtime = t.tv_sec + (t.tv_usec / 1.0e+6);

  tx->counters =  (stats_t*) chpl_malloc(1, sizeof(stats_t), 
					 CHPL_RT_MD_STM_TX_STATS, 0, 0);
  counters = tx->counters;
  counters->createtime = nowtime; 
  counters->durCreate = 0;
  counters->begintime = 0;
  counters->starttime = 0;

  counters->numremlocales = -1;

  counters->numAbort = 0;
  counters->durAbort = 0;
  counters->durFail = 0;
  counters->numMAbort = 0;
  counters->durMAbort = 0;
  counters->durMFail = 0;
  counters->numCommAbort = 0;
  counters->durCommAbort = 0;

  counters->maxLoad = 0;
  counters->maxStore = 0;
  counters->maxGet = 0;
  counters->maxPut = 0;
  counters->maxMalloc = 0;
  
  gtm_tx_cleanup_stats(tx, 1);
}

void gtm_tx_destroy_stats(void* txptr) {
  chpl_stm_tx_p tx = (chpl_stm_tx_p) txptr;

  if (!chpl_stm_stats) return;

  assert(tx->counters != NULL);
  chpl_free(tx->counters, 0, 0);
}

void gtm_tx_cleanup_stats(void* txptr, int cleanall) {
  chpl_stm_tx_p tx = (chpl_stm_tx_p) txptr;
  stats_t* counters;

  if (!chpl_stm_stats) return;

  counters = tx->counters;
  assert(counters != NULL);

  counters->sizeLoad = 0;
  counters->sizeStore = 0;
  counters->sizeGet = 0;
  counters->sizePut = 0;
  counters->sizeMalloc = 0;

  if (cleanall) {
    counters->numCommitPh1 = 0;
    counters->numCommCommitPh1 = 0;

    counters->durCommitPh1 = 0;
    counters->durCommCommitPh1 = 0;

    counters->numLoad = 0;
    counters->numStore = 0;
    counters->numGet = 0;
    counters->numCommGet = 0;
    counters->numPut = 0;
    counters->numCommPut = 0;
    counters->numFork = 0;
    counters->numCommFork = 0;
    counters->numMalloc = 0;;
    counters->numFree = 0;

    counters->durLoad = 0;
    counters->durStore = 0;
    counters->durGet = 0;
    counters->durCommGet = 0;
    counters->durPut = 0;
    counters->durCommPut = 0;
    counters->durFork = 0;
    counters->durCommFork = 0;
    counters->durMalloc = 0;
    counters->durFree = 0;
  }
}

void gtm_tx_stats_start(void* txptr, int op) {
  chpl_stm_tx_p tx = (chpl_stm_tx_p) txptr;
  stats_t* counters = tx->counters;
  _timervalue t;
  _real64 nowtime;

  t = _now_timer(t);
  nowtime = t.tv_sec + (t.tv_usec / 1.0e+6);

  switch(op) {
  case TX_BEGIN_STATS:
    counters->begintime = nowtime;
    if (counters->durCreate == 0)
      counters->durCreate = nowtime - counters->createtime;
    break;
  case TX_ABORT_STATS:
    counters->numremlocales = tx->numremlocales;
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

void gtm_tx_stats_stop(void* txptr, int op, int status, int size) {
  chpl_stm_tx_p tx = (chpl_stm_tx_p) txptr;
  stats_t* counters = tx->counters;
  _timervalue t;
  _real64 nowtime;

  if (status != TX_OK) return;
  
  t = _now_timer(t);
  nowtime = t.tv_sec + (t.tv_usec / 1.0e+6);

  switch(op) {
  case TX_ABORT_STATS:
    if (counters->numremlocales > -1) {
      counters->numAbort++;
      counters->durAbort += nowtime - counters->starttime;   
      counters->durFail += nowtime - counters->begintime;
    } else { 
      counters->numMAbort++;
      counters->durMAbort += nowtime - counters->starttime;   
      counters->durMFail += nowtime - counters->begintime;
    }
    break;
  case TX_COMM_ABORT_STATS:
    counters->numCommAbort++;
    counters->durCommAbort += nowtime - counters->starttime;   
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

    // collect abort stats
    numAbort += counters->numAbort;
    durAbort += counters->durAbort;
    numMAbort += counters->numMAbort;
    durMAbort += counters->durMAbort; 

    // collect commit stats
    if (tx->numremlocales > -1) {
      numCommitPh1 += counters->numCommitPh1;
      durCommitPh1 += counters->durCommitPh1;
      numCommitPh2++;
      durCommitPh2 += nowtime - counters->starttime;
    } else {
      numMCommitPh1 += counters->numCommitPh1;
      durMCommitPh1 += counters->durCommitPh1;
      numMCommitPh2++;
      durMCommitPh2 += nowtime - counters->starttime;
    }

    // collect communication stats
    numLocales += tx->numremlocales + 1;
    maxLocales = MAX(maxLocales, tx->numremlocales+1);

    // collect transactional load/store/get/put stats
    numLoad += counters->numLoad;
    durLoad += counters->durLoad;
    sizeLoad += counters->sizeLoad;
    maxLoad = MAX(maxLoad, counters->sizeLoad);
    numStore += counters->numStore;
    durStore += counters->durStore;
    sizeStore += counters->sizeStore;
    maxStore = MAX(maxStore, counters->sizeStore);
    numGet += counters->numGet;
    durGet += counters->durGet;
    sizeGet += counters->sizeGet;
    maxGet = MAX(maxGet, counters->sizeGet);
    numPut += counters->numPut;
    durPut += counters->durPut;
    sizePut += counters->sizePut;
    maxPut = MAX(maxPut, counters->sizePut);

    // collect fork stats
    numFork += counters->numFork;
    durFork += counters->durFork;

    // collect malloc / free stats
    numMalloc += counters->numMalloc;
    durMalloc += counters->durMalloc;
    sizeMalloc += counters->sizeMalloc;
    maxMalloc = MAX(maxMalloc, counters->sizeMalloc);
    numFree += counters->numFree;
    durFree += counters->durFree;

    // collect overall timing stats
    numSuccess++;
    durSuccess += nowtime - counters->begintime;
    durFailed += counters->begintime - counters->createtime - counters->durCreate;
    durCreate += counters->durCreate;
    durFail += counters->durFail; 
    durMFail += counters->durMFail;
    CHPL_MUTEX_UNLOCK(&gtmStatsLock);
    break;
  case TX_COMM_COMMITPH2_STATS:
    CHPL_MUTEX_LOCK(&gtmStatsCommLock);
    numCommAbort += counters->numCommAbort;
    durCommAbort += counters->durCommAbort;
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
    counters->sizeLoad += size; 
    counters->maxLoad = MAX(counters->maxLoad, size);
    break;
  case TX_STORE_STATS:
    counters->numStore++;
    counters->durStore += nowtime - counters->starttime;
    counters->sizeStore += size;
    counters->maxStore = MAX(counters->maxStore, size);
    break;
  case TX_GET_STATS:
    counters->numGet++;
    counters->durGet += nowtime - counters->starttime;
    counters->sizeGet += size;
    counters->maxGet = MAX(counters->maxGet, size);
    break;
  case TX_COMM_GET_STATS:
    counters->numCommGet++;
    counters->durCommGet += nowtime - counters->starttime;
    break;
  case TX_PUT_STATS:
    counters->numPut++;
    counters->durPut += nowtime - counters->starttime;
    counters->sizePut += size;
    counters->maxPut = MAX(counters->maxPut, size);
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
    counters->sizeMalloc += size;
    counters->maxMalloc = MAX(counters->maxMalloc, size);
    break;
  case TX_FREE_STATS:
    counters->numFree++;
    counters->durFree += nowtime - counters->starttime;
    break;
  default:
    chpl_error("STM Error: Timer not defined", 0, 0);
  }
}
