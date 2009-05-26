#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "chplmem.h"
#include "chplmemtrack.h"
#include "chplrt.h"
#include "chpltypes.h"
#include "error.h"
#include "heapmm.h"

#undef malloc
#undef free
#undef realloc

static hmm_descriptor chpl_heap = {0};
static chpl_mutex_t mem_lock;

void chpl_initHeap(void* start, size_t size) {
  if (!start || !size)
    chpl_error("Must have a shared segment", 0, 0);
  chpl_mutex_init(&mem_lock);
  hmm_init(&chpl_heap);
  hmm_new_chunk(&chpl_heap, start, size/HMM_ADDR_ALIGN_UNIT);
  heapInitialized = 1;
}


void* chpl_malloc(size_t number, size_t size, const char* description,
                  chpl_bool userCode, int32_t lineno, chpl_string filename) {
  size_t chunk = computeChunkSize(number, size, false, lineno, filename);
  size_t numBlocks = chunk / HMM_ADDR_ALIGN_UNIT;
  void* memAlloc;
  if (!heapInitialized) {
    chpl_error("chpl_malloc called before heap is initialized", lineno, filename);
  }

  if (chunk % HMM_ADDR_ALIGN_UNIT != 0)
    numBlocks += 1;

  chpl_mutex_lock(&mem_lock);
  memAlloc = hmm_alloc(&chpl_heap, numBlocks);
  chpl_mutex_unlock(&mem_lock);
  confirm(memAlloc, description, lineno, filename);
  track_malloc(memAlloc, chunk, number, size, description, userCode, lineno, filename);
  return memAlloc;
}


void chpl_free(void* memAlloc, chpl_bool userCode,
               int32_t lineno, chpl_string filename) {
  track_free(memAlloc, userCode, lineno, filename);
  if (memAlloc != NULL) {
    if (!heapInitialized) {
      chpl_error("chpl_free called before the heap is initialized", lineno, filename);
    }
    chpl_mutex_lock(&mem_lock);
    hmm_free(&chpl_heap, memAlloc);
    chpl_mutex_unlock(&mem_lock);
  }
}


void* chpl_realloc(void* memAlloc, size_t number, size_t size, 
                   const char* description, int32_t lineno, chpl_string filename) {
  size_t newChunk = computeChunkSize(number, size, true, lineno, filename);
  size_t numBlocks = newChunk / HMM_ADDR_ALIGN_UNIT;
  int success;
  void* moreMemAlloc;

  if (!newChunk) {
    chpl_free(memAlloc, false, lineno, filename);
    return NULL;
  }

  if (!memAlloc) {
    memAlloc = chpl_malloc(1, newChunk, description, false, lineno, filename);
    return memAlloc;
  }

  if (newChunk % HMM_ADDR_ALIGN_UNIT != 0)
    numBlocks += 1;

  chpl_mutex_lock(&mem_lock);
  success = !hmm_resize(&chpl_heap, memAlloc, numBlocks);
  chpl_mutex_unlock(&mem_lock);

  if (success) {
    moreMemAlloc = memAlloc;
    confirm(moreMemAlloc, description, lineno, filename);
  } else {
    size_t trueSize = hmm_true_size(memAlloc)*HMM_ADDR_ALIGN_UNIT;
    size_t copySize = newChunk < trueSize ? newChunk : trueSize;
    moreMemAlloc = chpl_malloc(number, size, description, false, lineno, filename);
    confirm(moreMemAlloc, description, lineno, filename);
    memcpy(moreMemAlloc, memAlloc, copySize);
  }

  /* If the allocator failed to realloc the memory and instead new memory
     was allocated, then free the old memory. */
  if (!success) {
    chpl_mutex_lock(&mem_lock);
    hmm_free(&chpl_heap, memAlloc);
    chpl_mutex_unlock(&mem_lock);
  }

  track_realloc(newChunk, moreMemAlloc, memAlloc,
                number, size, description, lineno, filename);

  return moreMemAlloc;
}
