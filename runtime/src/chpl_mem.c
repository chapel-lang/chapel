//
// Shared code for different mem implementations in mem-*/chpl_*_mem.c
//
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#define CHPL_MEM_C
#include "chpl_mem.h"
#include "chplmemtrack.h"
#include "chplrt.h"
#include "chpltypes.h"
#include "error.h"


int heapInitialized = 0;


static void
confirm(void* memAlloc, chpl_memDescInt_t description, int32_t lineno, 
        chpl_string filename) {
  if (!memAlloc) {
    const int messageSize = 1024;
    char message[messageSize];
    snprintf(message, messageSize, "Out of memory allocating \"%s\"",
             chpl_memDescString(description));
    chpl_error(message, lineno, filename);
  }
}


static size_t
computeChunkSize(size_t number, size_t size, chpl_bool zeroOK, 
                 int32_t lineno, chpl_string filename) {
  if (number == 0 && size == 0 && !zeroOK) {
    chpl_internal_error("Attempting to allocate 0 bytes of memory");
  } else if (number > 0 && size > SIZE_MAX/number) {
    chpl_error("Attempting to allocate > max(size_t) bytes of memory", lineno, filename);
  }
  return number * size;
}


const char* chpl_memDescString(chpl_memDescInt_t mdi) {
  if (mdi < CHPL_RT_MD_NUM)
    return chpl_rt_memDescs[mdi];
#ifndef LAUNCHER
  else
    return chpl_memDescs[mdi-CHPL_RT_MD_NUM];
#else
  return 0;
#endif
}


void chpl_initHeap(void* start, size_t size) {
  chpl_md_initHeap(start, size);
  heapInitialized = 1;
}


void* chpl_malloc(size_t number, size_t size, chpl_memDescInt_t description,
                  int32_t lineno, chpl_string filename) {
  size_t chunk = computeChunkSize(number, size, false, lineno, filename);
  void* memAlloc = chpl_md_malloc(chunk, lineno, filename);
  if (chunk != 0) {
    confirm(memAlloc, description, lineno, filename);
  }
  chpl_track_malloc(memAlloc, chunk, number, size, description, lineno, filename);
  return memAlloc;
}


void chpl_free(void* memAlloc, int32_t lineno, chpl_string filename) {
  if (memAlloc != NULL) {
    chpl_track_free(memAlloc, lineno, filename);
    chpl_md_free(memAlloc, lineno, filename);
  }
}


void* chpl_realloc(void* memAlloc, size_t number, size_t size, 
                   chpl_memDescInt_t description, int32_t lineno, chpl_string filename) {
  size_t newChunk = computeChunkSize(number, size, true, lineno, filename);
  void* moreMemAlloc;

  if (!newChunk) {
    chpl_free(memAlloc, lineno, filename);
    return NULL;
  }

  if (!heapInitialized) {
    chpl_error("chpl_realloc called before the heap is initialized", lineno, filename);
  }

  chpl_track_realloc1(memAlloc, number, size, description, lineno, filename);

  moreMemAlloc = chpl_md_realloc(memAlloc, newChunk, lineno, filename);

  confirm(moreMemAlloc, description, lineno, filename);

  chpl_track_realloc2(moreMemAlloc, newChunk, memAlloc, number, size, description, lineno, filename);

  return moreMemAlloc;
}
