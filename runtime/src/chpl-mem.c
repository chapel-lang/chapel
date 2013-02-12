//
// Shared code for different mem implementations in mem-*/chpl_*_mem.c
//
#include "chplrt.h"

#include "chpl-comm.h"
#define CHPL_MEM_C
#include "chpl-mem.h"
#include "chplmemtrack.h"
#include "chpltypes.h"
#include "error.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <limits.h>


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
  if ((number == 0 || size == 0) && !zeroOK) {
    chpl_internal_error("Attempting to allocate 0 bytes of memory");
  } else if (number > 0 && size > SIZE_MAX/number) {
    chpl_error("Attempting to allocate > max(size_t) bytes of memory", lineno, filename);
  }
  return number * size;
}

void chpl_mem_check_pre(size_t number, size_t size, chpl_bool zeroOK,
                         chpl_mem_descInt_t description,
                         int32_t lineno, chpl_string filename) {
  if( ! heapInitialized ) {
    chpl_error("memory routine called before the heap is initialized",
               lineno, filename);
  }
  // This might chpl_error out if number==0 || size==0 or overflow.
  computeChunkSize(number, size, zeroOK, lineno, filename);
}

void chpl_mem_check_post(void* memAlloc,
                         chpl_mem_descInt_t description,
                         int32_t lineno, chpl_string filename)
{
  confirm(memAlloc, description, lineno, filename);
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
    "command buffer",
    "comm layer transmit/receive buffer",
    "comm layer transmit/receive status",
    "comm layer sent remote fork info",
    "comm layer sent non-blocking remote fork info",
    "comm layer sent remote fork large fncall arg",
    "comm layer sent non-blocking remote fork large fncall arg",
    "comm layer sent remote fork response data",
    "comm layer received remote fork info",
    "comm layer received remote fork large info",
    "comm layer received non-blocking remote fork info",
    "comm layer received non-blocking remote fork large info",
    "comm layer received remote fork large fncall arg",
    "comm layer received non-blocking remote fork large fncall arg ",
    "comm layer remote fork done flag(s)",
    "comm layer per-locale information",
    "comm layer private objects array",
    "comm layer private broadcast data",
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
    "thread private data",
    "thread list descriptor",
    "io buffer or bytes",
    "gmp data",
    "put_strd/get_strd array of strides",
    "put_strd/get_strd array of count",
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

