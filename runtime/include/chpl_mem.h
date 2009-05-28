#ifndef _chpl_mem_H_
#define _chpl_mem_H_

#include <stddef.h>
#include <stdint.h>
#include "arg.h"
#include "chpltypes.h"
#include "chplthreads.h"

void chpl_initHeap(void* start, size_t size);

#define CHPL_ALLOC_PERMIT_ZERO(s,d,l,f) ((s == 0) ? NULL : chpl_alloc(s,d,l,f))
#define chpl_alloc(size, description, lineno, filename) \
  chpl_malloc(1, size, description, lineno, filename)
void* chpl_malloc(size_t number, size_t size, const char* description,
                  int32_t lineno, chpl_string filename);
void* chpl_realloc(void* ptr, size_t number, size_t size, 
                   const char* description, int32_t lineno, chpl_string filename);
void  chpl_free(void* ptr, int32_t lineno, chpl_string filename);

extern int heapInitialized;

void chpl_md_initHeap(void* start, size_t size);
void* chpl_md_malloc(size_t chunk, int32_t, chpl_string);
void chpl_md_free(void* memAlloc, int32_t, chpl_string);
void* chpl_md_realloc(void* memAlloc, size_t newChunk, int32_t, chpl_string);

#endif
