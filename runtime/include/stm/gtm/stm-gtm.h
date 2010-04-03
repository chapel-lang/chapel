#ifndef _stm_gtm_h_
#define _stm_gtm_h_

#include <setjmp.h> 

#ifdef GTM_DEBUG
#define PRINT_DEBUG(...)  printf(__VA_ARGS__); fflush(NULL)
#else 
#define PRINT_DEBUG(...)
#endif

#define SUCCESS 1
#define FAILED  0

#define RW_SET_SIZE 4096

enum { TX_IDLE = 0, TX_EMPTY, TX_ACTIVE };

enum { LOC_UNKNOWN = -1, LOC_SELF, LOC_REM }; 

extern int32_t chpl_localeID, chpl_numLocales;   // see src/chplcomm.c
#define MYLOC chpl_localeID
#define NLOCS chpl_numLocales

typedef uintptr_t chpl_stm_word_t;

typedef jmp_buf chpl_stm_tx_env_t;
#define chpl_stm_setjmp(txenv) setjmp(txenv)

//
// read set entry and read set
//

typedef struct __readEntry_t {
  chpl_stm_word_t version;
  chpl_stm_word_t *lock;
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
  volatile chpl_stm_word_t *addr;
  chpl_stm_word_t value;
  chpl_stm_word_t version;
  volatile chpl_stm_word_t *lock;
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

typedef struct __stmtxdesc_t {
  int32_t srclocale;         // locale that called tx_create 
  int32_t txlocale;          // locale that called outermost tx_begin
  int32_t *remlocales;       // locales on which tx has state
  int32_t txid;
  int status;                // TX_IDLE, TX_LACTIVE, etc.
  int nestlevel;             // flat nesting  
  read_set_t readset;           
  write_set_t writeset;
  chpl_stm_word_t timestamp;
  chpl_stm_tx_env_t env;     // stores program execution state
  // sigjmp_buf *jmp;           // pointer to program execution state
} chpl_stm_tx_t;


//
// global metadata and lock operations 
//
#define OWNED_MASK             0x01                 // 1 bit
#define LOCK_GET_OWNED(lock)   (lock & OWNED_MASK)
#define LOCK_ARRAY_SIZE        (1 << 20)            // 2^20 = 1M 
#define LOCK_MASK              (LOCK_ARRAY_SIZE - 1)
#define LOCK_SHIFT             ((sizeof(chpl_stm_word_t) == 4) ? 2 : 3)
#define LOCK_IDX(addr)         (((chpl_stm_word_t)addr >> LOCK_SHIFT) & LOCK_MASK)
#define GET_LOCK(addr)         ((chpl_stm_word_t*) &locks[LOCK_IDX(addr)])
#define CLOCK                  (gclock)
#define GET_CLOCK              ATOMIC_LOAD_MB(&CLOCK)
#define FETCH_AND_INC_CLOCK    ATOMIC_FETCH_AND_INC_MB(&CLOCK)

static volatile chpl_stm_word_t locks[LOCK_ARRAY_SIZE];

static volatile chpl_stm_word_t gclock;

//
// internal interface
//
chpl_stm_tx_t* gtm_tx_create(int32_t srclocale, int32_t txid);
void gtm_tx_initialize(chpl_stm_tx_t* tx, int32_t srclocale);
void gtm_tx_abort(chpl_stm_tx_t* tx);
int gtm_tx_twoPhaseCommit(void);
int gtm_tx_commitPh1(chpl_stm_tx_t* tx);
int gtm_tx_commitPh2(chpl_stm_tx_t* tx);
int gtm_tx_load(chpl_stm_tx_t* tx, void* dstaddr, void* srcaddr, size_t size);
int gtm_tx_store (chpl_stm_tx_t* tx, void* dstaddr, size_t size, void* srcaddr);
void gtm_comm_init(void);
void gtm_comm_exit(void);
void gtm_comm_tx_abort(chpl_stm_tx_t* tx, int32_t tgtlocale);
void gtm_comm_tx_commitPh1(chpl_stm_tx_t* tx, int32_t tgtlocale);
void gtm_comm_tx_commitPh2(chpl_stm_tx_t* tx, int32_t locale);
int gtm_comm_tx_get(chpl_stm_tx_t* tx, void* dstaddr, int32_t srclocale, void* srcaddr, size_t size);
int gtm_comm_tx_put(chpl_stm_tx_t* tx, void* srcaddr, int32_t dstlocale, void* dstaddr, size_t size);

#endif
