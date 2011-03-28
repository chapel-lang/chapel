//
// Malloc / Free based on TinySTM and TL2 implementation
//

#include "chplrt.h"
#include "chpl-comm.h"
#include "chpl_mem.h"
#include "chplsys.h"
#include "chpltasks.h"
#include "chplstm.h"
#include "error.h"
#include "stm-gtm.h"
#include "stm-gtm-memory.h"

void gtm_tx_create_memset(void* txptr) {
  chpl_stm_tx_p tx = (chpl_stm_tx_p) txptr;
  memset_t* mem  = (memset_t*) chpl_malloc(1, sizeof(memset_t), CHPL_RT_MD_STM_TX_MEMSET, __LINE__, __FILE__);
  mem->allocated = mem->freed = NULL;
  tx->memset = mem;
}

void gtm_tx_destroy_memset(void* txptr) {
  chpl_stm_tx_p tx = (chpl_stm_tx_p) txptr;
  chpl_free(tx->memset, 0, 0);
}

void gtm_tx_cleanup_memset(void* txptr) {
  chpl_stm_tx_p tx = (chpl_stm_tx_p) txptr;
  assert(tx->memset->allocated == NULL);
  assert(tx->memset->freed == NULL);
}

void* gtm_tx_malloc_memset(void* txptr, size_t number, size_t size, chpl_memDescInt_t description, int32_t ln, chpl_string fn) {
  chpl_stm_tx_p tx = (chpl_stm_tx_p) txptr;
  memset_entry_t* entry = (memset_entry_t*) chpl_malloc(1, sizeof(memset_entry_t), CHPL_RT_MD_STM_TX_MEMSET_ENTRY, ln, fn);
  size = (size + 7) & ~(size_t)0x07;
  entry->addr = (void *) chpl_malloc(number, size, description, ln, fn);
  entry->next = tx->memset->allocated; 
  tx->memset->allocated = entry;
  return entry->addr;
}

void gtm_tx_free_memset(void* txptr, void* addr, int32_t ln, chpl_string fn) {
  chpl_stm_tx_p tx = (chpl_stm_tx_p) txptr;
  memset_entry_t* entry = (memset_entry_t*) chpl_malloc(1, sizeof(memset_entry_t), CHPL_RT_MD_STM_TX_MEMSET_ENTRY, ln, fn);
  
  assert(((gtm_word_t) addr & (gtm_word_t) 0x07) == 0);

  entry->addr = addr;
  entry->next = tx->memset->freed; 
  tx->memset->freed = entry;
}

void gtm_tx_commit_memset(void* txptr) {
  chpl_stm_tx_p tx = (chpl_stm_tx_p) txptr;
  memset_t* memset = tx->memset;
  memset_entry_t* cur;
  memset_entry_t* next;

  if (memset->allocated) {
    cur = memset->allocated;
    while (cur) {
      next = cur->next;
      chpl_free(cur, 0, 0);
      cur = next;
    }
    memset->allocated = NULL;
  } 

  if (memset->freed) {
    cur = memset->freed;
    while (cur) {
      next = cur->next;
      chpl_free(cur->addr, 0, 0);
      chpl_free(cur, 0, 0); 
      cur = next;
    }
    memset->freed = NULL;
  }
}

void gtm_tx_abort_memset(void* txptr) {
  chpl_stm_tx_p tx = (chpl_stm_tx_p) txptr;
  memset_t* memset = tx->memset;
  memset_entry_t* cur;
  memset_entry_t* next;

  if (memset->allocated) {
    cur = memset->allocated;
    while (cur) {
      next = cur->next;
      chpl_free(cur->addr, 0, 0);
      chpl_free(cur, 0, 0);
      cur = next;
    }
    memset->allocated = NULL;
  } 

  if (memset->freed) {
    cur = memset->freed;
    while (cur) {
      next = cur->next;
      chpl_free(cur, 0, 0); 
      cur = next;
    }
    memset->freed = NULL;
  }
}
