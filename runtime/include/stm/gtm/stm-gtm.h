#ifndef _stm_gtm_h_
#define _stm_gtm_h_

#include <setjmp.h> 

#include "stm-gtm-memory.h"
#include "stm-gtm-cmgr.h"
#include "stm-gtm-comm.h"

//
// GTM specific macros
//

extern void* chpl_heapStart;   // see src/chpl_mem.c
extern size_t chpl_heapSize; 

#define GTM_COMM_COMBINED_COMMIT 1

enum {TX_FAIL = 0, TX_OK, TX_BUSY};

#define GTM_Safe(tmptx, fncall) do {		\
    if (fncall != TX_OK) {			\
      chpl_stm_tx_abort(tmptx);			\
    }						\
  } while (0)

#define RWSETSIZE 1024

#define NOLOCALE -1

enum { TX_IDLE = 0, TX_ACTIVE, TX_COMMIT, TX_ABORT, TX_AMACTIVE, TX_AMCOMMIT, TX_AMABORT };

typedef uintptr_t gtm_word_t;
typedef gtm_word_t* gtm_word_p;
#define GTMWORDSIZE sizeof(gtm_word_t)

typedef jmp_buf chpl_stm_tx_env_t;

#define chpl_stm_tx_set_env(txenv) setjmp(txenv)

//
// read set entry and read set
//

typedef struct __readEntry_t {
  gtm_word_t version;
  gtm_word_p lock;
} read_entry_t;

typedef struct __readSet_t {
  read_entry_t *entries;
  int numentries;
  size_t size;
  int reallocate;
} read_set_t;

//
// write set entry and write set
//

typedef struct __writeEntry_t {
  volatile gtm_word_p addr;
  gtm_word_t value;
  gtm_word_t version;
  gtm_word_t mask;
  volatile gtm_word_p lock;
  struct __writeEntry_t *next;
} write_entry_t;

typedef struct __writeSet_t {
  write_entry_t *entries;
  int numentries;
  int size;
  int reallocate;
} write_set_t;

//
// transaction descriptor
//

typedef struct __mfix {
  read_entry_t rentries[RWSETSIZE];
  write_entry_t wentries[RWSETSIZE];
  int32_t remlocales[1024];
  memset_t memset;
  tx_generic_t genericbuf;
  tx_get_t getbuf;
  tx_getdata_t getdatabuf;
  tx_put_t putbuf;
  tx_fork_t forkbuf;
} mfix_t; 

typedef struct __chpl_stm_tx_t {
  int32_t id;
  int32_t status;            // TX_IDLE, TX_LACTIVE, etc.
  int32_t nestlevel;         // flat nesting  
  int32_t locale;            // locale that called tx_create 
  int32_t numremlocales;     // number of remote locales tx has state
  int32_t* remlocales;       // list of locales on which tx has state
  read_set_t readset;               
  write_set_t writeset;      
  gtm_word_t timestamp;      // commit timestamp
  chpl_bool rollback;
  chpl_stm_tx_env_t env;     // stores program execution state
  memset_t* memset;          // tracks memory allocated / freed 
  chpl_stm_stats_p counters;     // timers and counters
  cmgr_t cmgr;
  mfix_t mfix; 
} chpl_stm_tx_t;


//
// global metadata and lock operations 
//
#define OWNED_MASK             0x01                 // 1 bit
#define LOCK_GET_OWNED(lock)   (lock & OWNED_MASK)
#define LOCK_ARRAY_SIZE        (1 << 24)            // 2^20 = 1M 
#define LOCK_MASK              (LOCK_ARRAY_SIZE - 1)
#define LOCK_SHIFT             ((sizeof(gtm_word_t) == 4) ? 2 : 3)
#define LOCK_IDX(addr)         (((gtm_word_t)addr >> LOCK_SHIFT) & LOCK_MASK)
#define GET_LOCK(addr)         ((gtm_word_p) &locks[LOCK_IDX(addr)])
#define CLOCK                  (gclock)
#define GET_CLOCK              ATOMIC_LOAD_MB(&CLOCK)
#define FETCH_AND_INC_CLOCK    ATOMIC_FETCH_AND_INC_MB(&CLOCK)

static volatile gtm_word_t locks[LOCK_ARRAY_SIZE];

static volatile gtm_word_t gclock;

//
// internal interface for descriptor management (purely local)
//

void gtm_tx_init(void);
chpl_stm_tx_t* gtm_tx_create(int32_t txid, int32_t txlocale);
void gtm_tx_destroy(chpl_stm_tx_t* tx);
void gtm_tx_cleanup(chpl_stm_tx_t* tx);

chpl_stm_tx_t* gtm_tx_comm_create(int32_t txid, int32_t txlocale, int32_t txstatus); 
void gtm_tx_comm_register(chpl_stm_tx_t* tx, int32_t dstlocale); 
void gtm_tx_comm_destroy(chpl_stm_tx_t* tx);
void gtm_tx_comm_cleanup(chpl_stm_tx_t* tx);

//
// internal interface for transactional operations (purely local)
//

int gtm_tx_commitPh1(chpl_stm_tx_t* tx);
int gtm_tx_commitPh2(chpl_stm_tx_t* tx);
void gtm_tx_abort(chpl_stm_tx_t* tx);
int gtm_tx_load(chpl_stm_tx_t* tx, void* dstaddr, void* srcaddr, size_t size);
int gtm_tx_load_wrap(chpl_stm_tx_t* tx, void* dstaddr, void* srcaddr, size_t size);
int gtm_tx_load_word(chpl_stm_tx_t* tx, gtm_word_p dstaddr, gtm_word_p srcaddr);
int gtm_tx_store(chpl_stm_tx_t* tx, void* srcaddr, void* dstaddr, size_t size);
int gtm_tx_store_wrap(chpl_stm_tx_t* tx, void* srcaddr, void*  dstaddr, size_t size);
int gtm_tx_store_word(chpl_stm_tx_t* tx, gtm_word_p srcaddr, gtm_word_p dstaddr, gtm_word_t mask);

//
// internal interface for transactional operations (purely remote)
//

int gtm_tx_comm_commitPh1(chpl_stm_tx_t* tx);
int gtm_tx_comm_commitPh2(chpl_stm_tx_t* tx);
void gtm_tx_comm_abort(chpl_stm_tx_t* tx);
int gtm_tx_comm_get(chpl_stm_tx_t* tx, void* dstaddr, int32_t srclocale, void* srcaddr, size_t size);
int gtm_tx_comm_put(chpl_stm_tx_t* tx, void* srcaddr, int32_t dstlocale, void* dstaddr, size_t size);
int gtm_tx_comm_fork(chpl_stm_tx_t* tx, int32_t dstlocale, chpl_fn_int_t fid, void *arg, size_t argsize);
int gtm_tx_comm_fork_fast(chpl_stm_tx_t* tx, int32_t dstlocale, chpl_fn_int_t fid, void *arg, size_t argsize);

#endif
