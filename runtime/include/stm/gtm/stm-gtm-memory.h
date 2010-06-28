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

#endif
