#ifndef _stm_gtm_h_
#define _stm_gtm_h_

#include <setjmp.h> 

//
// GTM specific macros
//

#define TX_OK 1
#define TX_FAIL 0

#define GTM_Safe(tmptx, fncall) do {		\
    if (fncall != TX_OK) {			\
      chpl_stm_tx_abort(tmptx);			\
    }						\
  } while (0)

#define RWSETSIZE 4096

#define NOLOCALE -1

enum { TX_IDLE = 0, TX_ACTIVE, TX_COMMIT };

extern int32_t chpl_localeID, chpl_numLocales;   // see src/chplcomm.c
#define MYLOCALE chpl_localeID
#define NLOCALES chpl_numLocales

typedef uintptr_t gtm_word_t;
typedef gtm_word_t* gtm_word_p;
#define GTMWORDSIZE sizeof(gtm_word_t)

typedef jmp_buf chpl_stm_tx_env_t;
#define chpl_stm_setjmp(txenv) setjmp(txenv)

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

typedef struct __chpl_stm_tx_t {
  int32_t status;            // TX_IDLE, TX_LACTIVE, etc.
  int32_t nestlevel;         // flat nesting  
  int32_t srclocale;         // locale that called tx_create 
  int32_t txlocale;          // locale that called outermost tx_begin
  int32_t numremlocales;     // number of remote locales tx has state
  int32_t* remlocales;       // list of locales on which tx has state
  read_set_t readset;               
  write_set_t writeset;      
  gtm_word_t timestamp;      // commit timestamp
  chpl_stm_tx_env_t env;     // stores program execution state
} chpl_stm_tx_t;


//
// global metadata and lock operations 
//
#define OWNED_MASK             0x01                 // 1 bit
#define LOCK_GET_OWNED(lock)   (lock & OWNED_MASK)
#define LOCK_ARRAY_SIZE        (1 << 20)            // 2^20 = 1M 
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
// internal interface
//

void gtm_tx_initialize(chpl_stm_tx_t* tx, int32_t srclocale);
void gtm_tx_cleanup(chpl_stm_tx_t* tx);

int gtm_tx_commitPh1(chpl_stm_tx_t* tx);
int gtm_tx_commitPh2(chpl_stm_tx_t* tx);
void gtm_tx_abort(chpl_stm_tx_t* tx);

int gtm_tx_load(chpl_stm_tx_t* tx, void* dstaddr, void* srcaddr, size_t size);
int gtm_tx_load_wrap(chpl_stm_tx_t* tx, void* dstaddr, void* srcaddr, size_t size);
int gtm_tx_load_word(chpl_stm_tx_t* tx, gtm_word_p dstaddr, gtm_word_p srcaddr);

int gtm_tx_store(chpl_stm_tx_t* tx, void* srcaddr, void* dstaddr, size_t size);
int gtm_tx_store_wrap(chpl_stm_tx_t* tx, void* srcaddr, void*  dstaddr, size_t size);
int gtm_tx_store_word(chpl_stm_tx_t* tx, gtm_word_p srcaddr, gtm_word_p dstaddr, gtm_word_t mask);

void gtm_comm_init(void);
void gtm_comm_exit(void);

int gtm_comm_tx_commitPh1(chpl_stm_tx_t* tx);
int gtm_comm_tx_commitPh2(chpl_stm_tx_t* tx);
void gtm_comm_tx_abort(chpl_stm_tx_t* tx);

int gtm_comm_tx_get(chpl_stm_tx_t* tx, void* dstaddr, int32_t srclocale, void* srcaddr, size_t size);
int gtm_comm_tx_put(chpl_stm_tx_t* tx, void* srcaddr, int32_t dstlocale, void* dstaddr, size_t size);

int gtm_comm_tx_fork(chpl_stm_tx_t* tx, int tgtlocale, chpl_fn_int_t fid, void *arg, int arg_size);

#endif
