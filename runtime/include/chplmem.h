#ifndef _chplmem_H_
#define _chplmem_H_

#include <stddef.h>
#include <stdint.h>
#include "arg.h"
#include "chpltypes.h"
#include "chplthreads.h"

extern int heapInitialized;

void initHeap(void* start, size_t size);

void chpl_startMemDiagnosis(void);
void chpl_stopMemDiagnosis(void);

#define CHPL_ALLOC_PERMIT_ZERO(s,d,l,f) ((s == 0) ? 0x0 : chpl_alloc(s,d,l,f))
#define chpl_alloc(size, description, lineno, filename) \
  chpl_malloc(1, size, description, lineno, filename)
void* chpl_malloc(size_t number, size_t size, const char* description,
                   int32_t lineno, chpl_string filename);
void* chpl_realloc(void* ptr, size_t number, size_t size, 
                    const char* description, int32_t lineno, chpl_string filename);
void  chpl_free(void* ptr, int32_t lineno, chpl_string filename);

#endif
