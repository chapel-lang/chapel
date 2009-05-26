#ifndef _chplmem_H_
#define _chplmem_H_

#include <stddef.h>
#include <stdint.h>
#include "arg.h"
#include "chpltypes.h"
#include "chplthreads.h"

void chpl_initHeap(void* start, size_t size);

void chpl_startMemDiagnosis(void);
void chpl_stopMemDiagnosis(void);

#define CHPL_ALLOC_PERMIT_ZERO(s,d,u,l,f) ((s == 0) ? NULL : chpl_alloc(s,d,u,l,f))
#define chpl_alloc(size, description, userCode, lineno, filename) \
  chpl_malloc(1, size, description, userCode, lineno, filename)
void* chpl_malloc(size_t number, size_t size, const char* description,
                  chpl_bool userCode, int32_t lineno, chpl_string filename);
void* chpl_realloc(void* ptr, size_t number, size_t size, 
                   const char* description, int32_t lineno, chpl_string filename);
void  chpl_free(void* ptr, chpl_bool userCode,
                int32_t lineno, chpl_string filename);

extern int heapInitialized;
void confirm(void*, const char*, int32_t, chpl_string);
size_t computeChunkSize(size_t, size_t, chpl_bool, int32_t, chpl_string);
void track_malloc(void*, size_t, size_t, size_t, const char*, chpl_bool, int32_t, chpl_string);
void track_free(void*, chpl_bool, int32_t, chpl_string);
void track_realloc(size_t, void*, void*, size_t, size_t, const char*, int32_t, chpl_string);

#endif
