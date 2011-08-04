#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "chpl-comm.h"
#include "chpl-mem.h"
#include "chplmemtrack.h"
#include "chplrt.h"
#include "chpltypes.h"
#include "error.h"
#include "dlmalloc.h"

static mspace chpl_heap;

static void* saved_heap_start;
static size_t saved_heap_size;


void chpl_mem_layerInit(void) {
  chpl_comm_desired_shared_heap(&saved_heap_start, &saved_heap_size);
  if (!saved_heap_start || !saved_heap_size)
    chpl_error("Must have a shared segment", 0, 0);
  chpl_heap = create_mspace_with_base(saved_heap_start, saved_heap_size, 1);
}


void chpl_mem_layerExit(void) { }


void chpl_mem_layerActualSharedHeap(void** start_p, size_t* size_p) {
  *start_p = saved_heap_start;
  *size_p  = saved_heap_size;
}


void* chpl_mem_layerAlloc(size_t size, int32_t lineno, chpl_string filename) {
  if (!heapInitialized) {
    chpl_error("chpl_mem_allocMany called before the heap is initialized",
               lineno, filename);
  }
  return mspace_malloc(chpl_heap, size);
}


void* chpl_mem_layerRealloc(void* memAlloc, size_t newChunk,
                      int32_t lineno, chpl_string filename) {
  return mspace_realloc(chpl_heap, memAlloc, newChunk);
}


void chpl_mem_layerFree(void* memAlloc, int32_t lineno, chpl_string filename) {
  if (!heapInitialized) {
    chpl_error("chpl_mem_free called before the heap is initialized",
               lineno, filename);
  }
  mspace_free(chpl_heap, memAlloc);
}
