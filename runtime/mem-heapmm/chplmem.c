#include <stdint.h>
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

hmm_descriptor chpl_heap = {0};
int heapInitialized = 0;

chpl_mutex_t _malloc_lock;

void initHeap(void* start, size_t size) {
  if (!start || !size)
    chpl_error("Must have a shared segment", 0, 0);
  chpl_mutex_init(&_malloc_lock);
  hmm_init(&chpl_heap);
  hmm_new_chunk(&chpl_heap, start, size/HMM_ADDR_ALIGN_UNIT);
  heapInitialized = 1;
}


static void
confirm(void* memAlloc, const char* description, int32_t lineno, 
        chpl_string filename) {
  if (!memAlloc) {
    char message[1024];
    sprintf(message, "Out of memory allocating \"%s\"", description);
    chpl_error(message, lineno, filename);
  }
}


static size_t computeChunkSize(size_t number, size_t size, int zeroOK, 
                               int32_t lineno, chpl_string filename) {
  size_t chunk = number * size;
  long long bigChunk = (long long)number * (long long)size;
  if (bigChunk != chunk) {
    chpl_error("Attempting to allocate > max(size_t) bytes of memory", lineno,
               filename);
  }
  if (chunk == 0 && zeroOK == 0) {
    chpl_internal_error("Attempting to allocate 0 bytes of memory");
  }
  return chunk;
}


#define MEM_DIAGNOSIS 0
static int chpl_memDiagnosisFlag = 0;

void chpl_startMemDiagnosis() {
  chpl_memDiagnosisFlag = 1;
}

void chpl_stopMemDiagnosis() {
  chpl_memDiagnosisFlag = 0;
}


void* chpl_malloc(size_t number, size_t size, const char* description,
                  chpl_bool userCode, int32_t lineno, chpl_string filename) {
  size_t chunk = computeChunkSize(number, size, 0, lineno, filename);
  size_t numBlocks = chunk / HMM_ADDR_ALIGN_UNIT;
  void* memAlloc;
  if (!heapInitialized) {
    chpl_error("chpl_malloc called before heap is initialized", lineno, filename);
  }

  if (chunk % HMM_ADDR_ALIGN_UNIT != 0)
    numBlocks += 1;

  chpl_mutex_lock(&_malloc_lock);
  memAlloc = hmm_alloc(&chpl_heap, numBlocks);
  chpl_mutex_unlock(&_malloc_lock);
  confirm(memAlloc, description, lineno, filename);

  if (memtrace) {
    printToMemLog(number, size, description, "malloc", memAlloc, NULL);
  } 
  if (memtrack) {
    installMemory(memAlloc, number, size, description);
    if (memstat) {
      increaseMemStat(chunk, lineno, filename);
    }
  }
#if MEM_DIAGNOSIS
  if (chpl_memDiagnosisFlag)
    printf("MD- %d:%p chpl_malloc(%lu, %lu, \"%s\", %d, %s)\n",
           chpl_localeID, memAlloc, (unsigned long)number,
           (unsigned long)size, description, lineno, filename);
#endif

  return memAlloc;
}


void chpl_free(void* memAlloc, int32_t lineno, chpl_string filename) {
  if (memAlloc != (void*)0x0) {
    if (memtrace) {
      if (memtrack) {
        memTableEntry* memEntry;
        memEntry = lookupMemory(memAlloc);
        printToMemLog(memEntry->number, memEntry->size, memEntry->description, "free", memAlloc, NULL);
      } else
        printToMemLog(0, 0, "", "free", memAlloc, NULL);
    }

    if (memtrack) {
      if (memstat) {
        memTableEntry* memEntry;
        size_t chunk;
        memEntry = lookupMemory(memAlloc);
        if (memEntry) {
          chunk = memEntry->number * memEntry->size;
          decreaseMemStat(chunk);
        }
      }
      removeMemory(memAlloc, lineno, filename);
    }
    if (!heapInitialized) {
      chpl_error("chpl_free called before the heap is initialized", lineno, filename);
    }
#if MEM_DIAGNOSIS
    if (chpl_memDiagnosisFlag)
      printf("MD- %d:%p chpl_free(%d, %s)\n", chpl_localeID, memAlloc, lineno, filename);
#endif

    chpl_mutex_lock(&_malloc_lock);
    hmm_free(&chpl_heap, memAlloc);
    chpl_mutex_unlock(&_malloc_lock);
  }
}


void* chpl_realloc(void* memAlloc, size_t number, size_t size, 
                    const char* description, int32_t lineno, chpl_string filename) {
  size_t newChunk = computeChunkSize(number, size, 1, lineno, filename);
  size_t numBlocks = newChunk / HMM_ADDR_ALIGN_UNIT;
  memTableEntry* memEntry = NULL;
  int success;
  void* moreMemAlloc;
  if (newChunk % HMM_ADDR_ALIGN_UNIT != 0)
    numBlocks += 1;

  if (!heapInitialized) {
    chpl_error("chpl_realloc called before the heap is initialized", lineno, filename);
  }

  if (!memAlloc && newChunk != 0) {
    memAlloc = chpl_malloc(1, newChunk, description, lineno, filename);
    return memAlloc;
  }

  if (newChunk == 0) {
    chpl_free(memAlloc, lineno, filename);
    return NULL;
  }

  if (memtrack) {
    memEntry = lookupMemory(memAlloc);
    if (!memEntry && (memAlloc != NULL)) {
      char* message;
      message = chpl_glom_strings(3, "Attempting to realloc memory for ",
                                  description, " that wasn't allocated");
      chpl_error(message, lineno, filename);
    }
  }

  chpl_mutex_lock(&_malloc_lock);
  success = !hmm_resize(&chpl_heap, memAlloc, numBlocks);
  chpl_mutex_unlock(&_malloc_lock);

  if (success) {
    moreMemAlloc = memAlloc;
    confirm(moreMemAlloc, description, lineno, filename);
  } else {
    size_t trueSize = hmm_true_size(memAlloc)*HMM_ADDR_ALIGN_UNIT;
    size_t copySize = newChunk < trueSize ? newChunk : trueSize;
    moreMemAlloc = chpl_malloc(number, size, description, lineno, filename);
    confirm(moreMemAlloc, description, lineno, filename);
    memcpy(moreMemAlloc, memAlloc, copySize);
  }

  if (memtrack) { 
    if (memAlloc != NULL) {
      if (memEntry) {
        if (memstat) {
          size_t oldChunk;
          oldChunk = memEntry->number * memEntry->size;
          decreaseMemStat(oldChunk);
        }
        updateMemory(memEntry, memAlloc, moreMemAlloc, number, size);
      }
    } else {
      installMemory(moreMemAlloc, number, size, description);
    }
    if (memstat) {
      increaseMemStat(newChunk, lineno, filename);
    }
  }
  if (memtrace) {
    printToMemLog(number, size, description, "realloc", memAlloc, 
                  moreMemAlloc);
  }

  /* If the allocator failed to realloc the memory and instead new memory
     was allocated, then free the old memory. */
  if (!success) {
    chpl_mutex_lock(&_malloc_lock);
    hmm_free(&chpl_heap, memAlloc);
    chpl_mutex_unlock(&_malloc_lock);
  }
  return moreMemAlloc;
}
