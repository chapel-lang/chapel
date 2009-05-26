#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "chpl_mem.h"
#include "chplmemtrack.h"
#include "chplrt.h"
#include "chpltypes.h"
#include "error.h"
#include "dlmalloc.h"

static mspace chpl_heap;

void chpl_md_initHeap(void* start, size_t size) {
  if (!start || !size)
    chpl_error("Must have a shared segment", 0, 0);
  chpl_heap = create_mspace_with_base(start, size, 1);
}


void* chpl_md_malloc(size_t size) {
  if (!heapInitialized) {
    chpl_error("chpl_malloc called before the heap is initialized", lineno, filename);
  }
  return mspace_malloc(chpl_heap, chunk);
}


void chpl_md_free(void* memAlloc) {
  if (!heapInitialized) {
    chpl_error("chpl_free called before the heap is initialized", lineno, filename);
  }
  mspace_free(chpl_heap, memAlloc);
}


void* chpl_md_realloc(void* memAlloc, size_t newChunk) {
  return mspace_realloc(chpl_heap, memAlloc, newChunk);
}
