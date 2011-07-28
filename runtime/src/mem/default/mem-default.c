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

#undef malloc
#undef free
#undef realloc

void chpl_mem_layerInit(void) {
  void* start;
  size_t size;

  chpl_comm_desired_shared_heap(&start, &size);
  if (start || size)
    chpl_error("set CHPL_MEM to a more appropriate mem type", 0, 0);
}


void chpl_mem_layerExit(void) { }


void chpl_mem_layerActualSharedHeap(void** start_p, size_t* size_p) {
  *start_p = NULL;
  *size_p  = 0;
}


void* chpl_mem_layerAlloc(size_t chunk, int32_t lineno, chpl_string filename) {
  return (chunk) ? malloc(chunk) : NULL;
}


void* chpl_mem_layerRealloc(void* memAlloc, size_t newChunk,
                      int32_t lineno, chpl_string filename) {
  return realloc(memAlloc, newChunk);
}


void chpl_mem_layerFree(void* memAlloc, int32_t lineno, chpl_string filename) {
  free(memAlloc);
}
