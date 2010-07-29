#ifndef _stm_gtm_stats_h_
#define _stm_gtm_stats_h_

enum {
  TX_BEGIN_STATS = 1,
  TX_COMMITPH1_STATS,
  TX_COMMITPH2_STATS,
  TX_ABORT_STATS,
  TX_LOAD_STATS,
  TX_STORE_STATS,
  TX_GET_STATS,
  TX_PUT_STATS,
  TX_COMM_COMMITPH1_STATS,
  TX_COMM_COMMITPH2_STATS,
  TX_COMM_ABORT_STATS,
  TX_COMM_GET_STATS,
  TX_COMM_PUT_STATS,
  TX_FORK_STATS,
  TX_COMM_FORK_STATS,
  TX_MALLOC_STATS,
  TX_FREE_STATS
};

typedef struct __stats_t {
  _real64 starttime;            // time the ongoing started
  _real64 begintime;            // time the transaction started
  _real64 lastAbort;            // time of the last abort
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
} stats_t;

void gtm_init_stats(void);
void gtm_exit_stats(void);
void gtm_tx_create_stats(void*);
void gtm_tx_destroy_stats(void*);
void gtm_tx_cleanup_stats(void*);
void gtm_tx_stats_start(void*, int);
void gtm_tx_stats_stop(void*, int, int status);

#define GTM_TX_STATS_START(tx, op) \
  gtm_tx_stats_start(tx, op) 
#define GTM_TX_STATS_STOP(tx, op) \
  gtm_tx_stats_stop(tx, op, TX_OK)
#define GTM_TX_COMM_STATS_START(tx, op) \
  gtm_tx_stats_start(tx, op) 
#define GTM_TX_COMM_STATS_STOP(tx, op, status) \
  gtm_tx_stats_stop(tx, op, status)

#endif
