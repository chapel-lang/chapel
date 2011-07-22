#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "chpl-mem.h"
#include "chplmemtrack.h"
#include "chplrt.h"
#include "chpltypes.h"
#include "error.h"

#undef malloc
#undef free
#undef realloc

void chpl_md_initHeap(void* start, size_t size) {
  if (start || size)
    chpl_error("set CHPL_MEM to a more appropriate mem type", 0, 0);
}


void* chpl_md_malloc(size_t chunk, int32_t lineno, chpl_string filename) {
  return (chunk) ? malloc(chunk) : NULL;
}


void chpl_md_free(void* memAlloc, int32_t lineno, chpl_string filename) {
  free(memAlloc);
}


void* chpl_md_realloc(void* memAlloc, size_t newChunk, int32_t lineno, chpl_string filename) {
  return realloc(memAlloc, newChunk);
}
