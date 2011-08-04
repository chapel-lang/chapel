//
// Shared code for different mem implementations in mem-*/chpl_*_mem.c
//
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <limits.h>
#include "chpl-comm.h"
#define CHPL_MEM_C
#include "chpl-mem.h"
#include "chplmemtrack.h"
#include "chplrt.h"
#include "chpltypes.h"
#include "error.h"


//
// The compiler generates a separate array of descriptions for the
// allocation types it defines.  Indices into that compiler-generated
// array conceptually start after the CHPL_RT_MD_NUM enum value in
// chpl-mem.h).  This is that compiler-generated array.
//
extern const char* chpl_mem_descs[];


int heapInitialized = 0;


static void
confirm(void* memAlloc, chpl_mem_descInt_t description, int32_t lineno, 
        chpl_string filename) {
  if (!memAlloc) {
    const int messageSize = 1024;
    char message[messageSize];
    snprintf(message, messageSize, "Out of memory allocating \"%s\"",
             chpl_mem_descString(description));
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


const char* chpl_mem_descString(chpl_mem_descInt_t mdi) {
  //
  // These description strings correspond to the memory allocation types
  // named by the chpl_mem_rtMemDesc_t enumerated type in chpl-mem.h.
  //
  static const char* rt_memDescs[] = {
    "unknown",
    "chapel runtime code data",
    "chapel execution command buffer",
    "array elements",
    "set wide string",
    "get wide string",
    "private broadcast data",
    "remote fork data",
    "remote fork non-blocking data",
    "remote GPC data",
    "remote GPC header addr",
    "remote GPC header",
    "remote GPC copy of data",
    "remote GPC fork data",
    "active message fork data",
    "active message fork arg",
    "active message fork large data",
    "active message fork non-blocking data",
    "active message fork non-blocking arg",
    "active message fork non-blocking large data",
    "remote fork arg",
    "command buffer",
    "pvm list of nodes",
    "pvm spawn thing",
    "private objects array",
    "garbage collection heap",
    "garbage collection space pointer",
    "glom strings data",
    "string copy data",
    "string concat data",
    "string strided select data",
    "config arg copy data",
    "config table data",
    "locale name buffer",
    "serial flag",
    "task descriptor",
    "task descriptor link",               // reserved for future use
    "task stack",                         // reserved for future use
    "mutex",
    "lock report data",
    "task pool descriptor",
    "task list descriptor",
    "arrays for localesPerRealm",
    "thread private data",
    "thread callee function pointer and argument",
    "thread list descriptor",
    "io buffer or bytes",
    "gmp data",
  };

  if (mdi < CHPL_RT_MD_NUM)
    return rt_memDescs[mdi];
  else
    return chpl_mem_descs[mdi-CHPL_RT_MD_NUM];
}


void chpl_mem_init(void) {
  chpl_mem_layerInit();
  heapInitialized = 1;
}


void chpl_mem_exit(void) {
  chpl_mem_layerExit();
}


void chpl_mem_actualSharedHeap(void** start_p, size_t* size_p) {
  chpl_mem_layerActualSharedHeap(start_p, size_p);
}


void* chpl_mem_allocMany(size_t number, size_t size,
                         chpl_mem_descInt_t description,
                         int32_t lineno, chpl_string filename) {
  size_t chunk = computeChunkSize(number, size, false, lineno, filename);
  void* memAlloc = chpl_mem_layerAlloc(chunk, lineno, filename);
  if (chunk != 0) {
    confirm(memAlloc, description, lineno, filename);
  }
  chpl_track_malloc(memAlloc, chunk, number, size, description,
                    lineno, filename);
  return memAlloc;
}


void* chpl_mem_realloc(void* memAlloc, size_t number, size_t size, 
                       chpl_mem_descInt_t description,
                       int32_t lineno, chpl_string filename) {
  size_t newChunk = computeChunkSize(number, size, true, lineno, filename);
  void* moreMemAlloc;

  if (!newChunk) {
    chpl_mem_free(memAlloc, lineno, filename);
    return NULL;
  }

  if (!heapInitialized) {
    chpl_error("chpl_mem_realloc called before the heap is initialized",
               lineno, filename);
  }

  chpl_track_realloc1(memAlloc, number, size, description, lineno, filename);

  moreMemAlloc = chpl_mem_layerRealloc(memAlloc, newChunk, lineno, filename);

  confirm(moreMemAlloc, description, lineno, filename);

  chpl_track_realloc2(moreMemAlloc, newChunk, memAlloc, number, size,
                      description, lineno, filename);

  return moreMemAlloc;
}


void chpl_mem_free(void* memAlloc, int32_t lineno, chpl_string filename) {
  if (memAlloc != NULL) {
    chpl_track_free(memAlloc, lineno, filename);
    chpl_mem_layerFree(memAlloc, lineno, filename);
  }
}
