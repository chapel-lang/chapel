#ifndef _stm_gtm_memory_h_
#define _stm_gtm_memory_h_

typedef struct _memset_entry_t {
  void* addr;                    // address of memory
  size_t size;                   // size allocated
  struct _memset_entry_t* next;  // pointer to next memory block
} memset_entry_t;

typedef struct _memset_t {
  memset_entry_t* allocated;     // malloc'ed memory, free on abort
  memset_entry_t* freed;         // freed memory, free upon commit
} memset_t;

//
// internal interface for memory management (purely local)
//

void gtm_tx_create_memset(void*);
void gtm_tx_destroy_memset(void*);
void gtm_tx_cleanup_memset(void*);
void gtm_tx_commit_memset(void*);
void gtm_tx_abort_memset(void*);
void* gtm_tx_malloc_memset(void*, size_t, size_t, chpl_memDescInt_t, int32_t, chpl_string);
void gtm_tx_free_memset(void*, void*, int32_t, chpl_string);

#endif
