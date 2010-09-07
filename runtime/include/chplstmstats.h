#ifndef _chplstmstats_h_
#define _chplstmstats_h_

#define STM_STATS_CLEAN_ON_ABORT 1

enum {
  STATS_TX_BEGIN = 1,
  STATS_TX_COMMITPH1,
  STATS_TX_COMMITPH2,
  STATS_TX_ABORT,
  STATS_TX_LOAD,
  STATS_TX_STORE,
  STATS_TX_MALLOC,
  STATS_TX_FREE,
  STATS_TX_GET,
  STATS_TX_PUT,
  STATS_TX_FORK,
  STATS_TX_COMM_COMMITPH1,
  STATS_TX_COMM_COMMITPH2,
  STATS_TX_COMM_ABORT,
  STATS_TX_COMM_GET,
  STATS_TX_COMM_PUT,
  STATS_TX_COMM_FORK
};

typedef struct __chpl_stm_stats_t {
  void *tx;
  _real64 createtime;           // time transaction was created
  _real64 begintime;            // time transaction started
  _real64 starttime;            // time current operation started
  _real64 durCreate;            // time between createtime and first begin
  unsigned int numAbort;
  _real64 durAbort;
  _real64 durFail;
  unsigned int numMAbort;
  _real64 durMAbort;
  _real64 durMFail;
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
} chpl_stm_stats_t;

typedef chpl_stm_stats_t* chpl_stm_stats_p;

void chpl_stm_stats_init(void);
void chpl_stm_stats_exit(void);
chpl_stm_stats_p chpl_stm_stats_create(void);
void chpl_stm_stats_destroy(chpl_stm_stats_p);
void chpl_stm_stats_cleanup(chpl_stm_stats_p, int);
void chpl_stm_stats_start(chpl_stm_stats_p, int);
void chpl_stm_stats_stop(chpl_stm_stats_p, int, int status, int size);

#define CHPL_STM_STATS_START(sptr, op)			\
  if (chpl_stm_stats) chpl_stm_stats_start(sptr, op)

#define CHPL_STM_STATS_STOP(sptr, op, value) \
  if (chpl_stm_stats) chpl_stm_stats_stop(sptr, op, 1, value)

#define CHPL_STM_COMM_STATS_START(sptr, op) \
  if (chpl_stm_stats) chpl_stm_stats_start(sptr, op) 

#define CHPL_STM_COMM_STATS_STOP(sptr, op, status) \
  if (chpl_stm_stats) chpl_stm_stats_stop(sptr, op, status, 0)

#endif
