#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "chplmem.h"
#include "chplmemtrack.h"
#include "chplrt.h"
#include "chpltypes.h"
#include "error.h"

#undef malloc
#undef free
#undef realloc

void chpl_initHeap(void* start, size_t size) {
  if (start || size)
    chpl_error("set CHPL_MEM to a more appropriate mem type", 0, 0);
  heapInitialized = 1;
}


void* chpl_malloc(size_t number, size_t size, const char* description,
                  chpl_bool userCode, int32_t lineno, chpl_string filename) {
  size_t chunk = computeChunkSize(number, size, false, lineno, filename);
  void* memAlloc = (chunk) ? malloc(chunk) : NULL;
  if (chunk != 0) {
    confirm(memAlloc, description, lineno, filename);
  }
  track_malloc(memAlloc, chunk, number, size, description, userCode, lineno, filename);
  return memAlloc;
}


void chpl_free(void* memAlloc, chpl_bool userCode,
               int32_t lineno, chpl_string filename) {
  track_free(memAlloc, userCode, lineno, filename);
  if (memAlloc != NULL) {
    free(memAlloc);
  }
}


void* chpl_realloc(void* memAlloc, size_t number, size_t size, 
                   const char* description, int32_t lineno, chpl_string filename) {
  size_t newChunk = computeChunkSize(number, size, true, lineno, filename);
  void* moreMemAlloc;

  if (!newChunk) {
    chpl_free(memAlloc, false, lineno, filename);
    return NULL;
  }

  moreMemAlloc = realloc(memAlloc, newChunk);
  confirm(moreMemAlloc, description, lineno, filename);

  track_realloc(newChunk, moreMemAlloc, memAlloc,
                number, size, description, lineno, filename);

  return moreMemAlloc;
}
