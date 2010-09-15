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

#define AVGNUM(num, tot) (num ? (tot * 1.0) / (num * 1.0) : 0)
#define MAX(num1,num2)   (((num1) > (num2)) ? (num1) : (num2))

int printStmStats;    // hack to print stats only when we collect them

static chpl_mutex_t stmStatsLock;
static chpl_mutex_t stmStatsCommLock;

unsigned int numSuccess;
_real64 durSuccess;
_real64 durFailed;
_real64 durCreate;

unsigned int numFail;
_real64 durFail;
unsigned int numMFail;
_real64 durMFail;

unsigned int maxLocales;
unsigned int numLocales;

unsigned int numAbort;
_real64 durAbort;
unsigned int numMAbort;
_real64 durMAbort;
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

void chpl_startStmStats() {
  chpl_stm_stats = 1;
  chpl_comm_broadcast_private(3 /* &chpl_stm_stats */, sizeof(int));
}

void chpl_stopStmStats() {
  chpl_stm_stats = 0;
  chpl_comm_broadcast_private(3 /* &chpl_stm_stats */, sizeof(int));
}

void chpl_stm_stats_init() {
  CHPL_MUTEX_INIT(&stmStatsLock);
  CHPL_MUTEX_INIT(&stmStatsCommLock);
}

void chpl_stm_stats_exit() {
  unsigned int numMSum, numSum, numCommSum; 
  _real64 durMSum, durSum, durCommSum;

  if (!printStmStats) return;

  numMSum = numLoad + numStore + numMalloc + numFree +
    numMCommitPh1 + numMCommitPh2;

  durMSum = durLoad + durStore + durMalloc + durFree + 
    durMCommitPh1 + durMCommitPh2;

  numSum = numGet + numPut + numFork + numCommitPh1 + numCommitPh2;

  durSum = durGet + durPut + durFork + durCommitPh1 + durCommitPh2;

  numCommSum = numCommGet + numCommPut + numCommFork + 
    numCommCommitPh1 + numCommCommitPh2;

  durCommSum = durCommGet + durCommPut + durCommFork + 
    durCommCommitPh1 + durCommCommitPh2;

  fprintf(stdout, "-%dLn%d- %8d %8d %8d %8d %8d %8d %8d %8d\n",
	  NLOCALES, MYLOCALE, numSuccess, numSuccess,
	  numMSum, numSum, numCommSum, 
	  numMAbort + numAbort, numMFail, numFail);   

  fprintf(stdout, "-%dLd%d- %.2e %.2e %.2e %.2e %.2e %.2e %.2e %.2e\n", 
	  NLOCALES, MYLOCALE, durSuccess, durCreate, 
	  durMSum, durSum, durCommSum, durFailed, durMFail, durFail);

  fprintf(stdout, "-%dMn%d- %8d %8d %8d %8d %8d %8d %8d\n",
	  NLOCALES, MYLOCALE, numLoad, numStore, numMalloc, numFree, 
	  numMCommitPh1, numMCommitPh2, numMAbort);

  fprintf(stdout, "-%dMd%d- %.2e %.2e %.2e %.2e %.2e %.2e %.2e\n",
	  NLOCALES, MYLOCALE, durLoad, durStore, durMalloc,durFree,
	  durMCommitPh1, durMCommitPh2, durMAbort);

  fprintf(stdout, "-%dGn%d- %8d %8d %8d %8d %8d %8d\n", 
	  NLOCALES, MYLOCALE, numGet, numPut, numFork, 
	  numCommitPh1, numCommitPh2, numAbort);

  fprintf(stdout, "-%dGd%d- %.2e %.2e %.2e %.2e %.2e %.2e\n",
	  NLOCALES, MYLOCALE, durGet, durPut, durFork, 
	  durCommitPh1, durCommitPh2, durAbort);

  fprintf(stdout, "-%dCn%d- %8d %8d %8d %8d %8d %8d\n", 
	  NLOCALES, MYLOCALE, numCommGet, numCommPut, numCommFork, 
	  numCommCommitPh1, numCommCommitPh2, numCommAbort);

  fprintf(stdout, "-%dCd%d- %.2e %.2e %.2e %.2e %.2e %.2e\n",
	  NLOCALES, MYLOCALE, durCommGet, durCommPut, durCommFork, 
	  durCommCommitPh1, durCommCommitPh2, durCommAbort);

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

chpl_stm_stats_p chpl_stm_stats_create() {
  chpl_stm_stats_t* counters;
  _timervalue t;
  _real64 nowtime;

  if (!chpl_stm_stats) return;

  printStmStats = 1;

  t = _now_timer(t);
  nowtime = t.tv_sec + (t.tv_usec / 1.0e+6);

  counters = (chpl_stm_stats_t*) chpl_malloc(1, sizeof(chpl_stm_stats_t), 
					     CHPL_RT_MD_STM_STATS, 0, 0);
  counters->createtime = nowtime; 
  counters->durCreate = 0;
  counters->begintime = 0;
  counters->starttime = 0;
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
  
  chpl_stm_stats_cleanup(counters, 1);

  return counters;
}

void chpl_stm_stats_destroy(chpl_stm_stats_p counters) {
  if (!chpl_stm_stats) return;
  chpl_free(counters, 0, 0);
}

void chpl_stm_stats_cleanup(chpl_stm_stats_p counters, int cleanall) {
  if (!chpl_stm_stats) return;

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

void chpl_stm_stats_start(chpl_stm_stats_p counters, int op) {
  _timervalue t;
  _real64 nowtime;

  t = _now_timer(t);
  nowtime = t.tv_sec + (t.tv_usec / 1.0e+6);

  switch(op) {
  case STATS_TX_BEGIN:
    counters->begintime = nowtime;
    if (counters->durCreate == 0)
      counters->durCreate = nowtime - counters->createtime;
    break;
  case STATS_TX_ABORT:
  case STATS_TX_COMM_ABORT:   
  case STATS_TX_COMMITPH1:
  case STATS_TX_COMM_COMMITPH1:
  case STATS_TX_COMMITPH2:
  case STATS_TX_COMM_COMMITPH2:
  case STATS_TX_LOAD:
  case STATS_TX_STORE:
  case STATS_TX_GET:
  case STATS_TX_COMM_GET:
  case STATS_TX_PUT:
  case STATS_TX_COMM_PUT:
  case STATS_TX_FORK:
  case STATS_TX_COMM_FORK:
  case STATS_TX_MALLOC:
  case STATS_TX_FREE:   
    counters->starttime = nowtime;
    break;
  default:
    chpl_error("STM Error: Timer not defined", 0, 0);
  }
}

void chpl_stm_stats_stop(chpl_stm_stats_p counters, int op, int status, int value) {
  _timervalue t;
  _real64 nowtime;

  if (status != 1) return;
 
  t = _now_timer(t);
  nowtime = t.tv_sec + (t.tv_usec / 1.0e+6);

  switch(op) {
  case STATS_TX_ABORT:
    if (value > -1) {
      counters->numAbort++;
      counters->durAbort += nowtime - counters->starttime;   
      counters->durFail += nowtime - counters->begintime;
    } else { 
      counters->numMAbort++;
      counters->durMAbort += nowtime - counters->starttime;   
      counters->durMFail += nowtime - counters->begintime;
    }
    break;
  case STATS_TX_COMM_ABORT:
    counters->numCommAbort++;
    counters->durCommAbort += nowtime - counters->starttime;   
    break;
  case STATS_TX_COMMITPH1:
    counters->numCommitPh1++;
    counters->durCommitPh1 += nowtime - counters->starttime;
    break;
  case STATS_TX_COMM_COMMITPH1:
    counters->numCommCommitPh1++;
    counters->durCommCommitPh1 += nowtime - counters->starttime;
    break;
  case STATS_TX_COMMITPH2:
    CHPL_MUTEX_LOCK(&stmStatsLock);

    // collect abort stats
    numAbort += counters->numAbort;
    durAbort += counters->durAbort;
    numMAbort += counters->numMAbort;
    durMAbort += counters->durMAbort; 

    // collect commit stats
    if (value > -1) {
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
    numLocales += value + 1;
    maxLocales = MAX(maxLocales, value+1);

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
    numFail += (counters->numAbort) ? 1 : 0; 
    durFail += counters->durFail; 
    numMFail += (counters->numMAbort) ? 1 : 0; 
    durMFail += counters->durMFail;
    
    CHPL_MUTEX_UNLOCK(&stmStatsLock);
    break;
  case STATS_TX_COMM_COMMITPH2:
    CHPL_MUTEX_LOCK(&stmStatsCommLock);
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
    CHPL_MUTEX_UNLOCK(&stmStatsCommLock);
    break;
  case STATS_TX_LOAD:
    counters->numLoad++;
    counters->durLoad += nowtime - counters->starttime;
    counters->sizeLoad += value; 
    counters->maxLoad = MAX(counters->maxLoad, value);
    break;
  case STATS_TX_STORE:
    counters->numStore++;
    counters->durStore += nowtime - counters->starttime;
    counters->sizeStore += value;
    counters->maxStore = MAX(counters->maxStore, value);
    break;
  case STATS_TX_GET:
    counters->numGet++;
    counters->durGet += nowtime - counters->starttime;
    counters->sizeGet += value;
    counters->maxGet = MAX(counters->maxGet, value);
    break;
  case STATS_TX_COMM_GET:
    counters->numCommGet++;
    counters->durCommGet += nowtime - counters->starttime;
    break;
  case STATS_TX_PUT:
    counters->numPut++;
    counters->durPut += nowtime - counters->starttime;
    counters->sizePut += value;
    counters->maxPut = MAX(counters->maxPut, value);
    break;
  case STATS_TX_COMM_PUT:
    counters->numCommPut++;
    counters->durCommPut += nowtime - counters->starttime;
    break;
  case STATS_TX_FORK:
    counters->numFork++;
    counters->durFork += nowtime - counters->starttime;
    break;
  case STATS_TX_COMM_FORK:
    counters->numCommFork++;
    counters->durCommFork += nowtime - counters->starttime;
    break;
  case STATS_TX_MALLOC:
    counters->numMalloc++;
    counters->durMalloc += nowtime - counters->starttime;
    counters->sizeMalloc += value;
    counters->maxMalloc = MAX(counters->maxMalloc, value);
    break;
  case STATS_TX_FREE:
    counters->numFree++;
    counters->durFree += nowtime - counters->starttime;
    break;
  default:
    chpl_error("STM Error: Timer not defined", 0, 0);
  }
}
