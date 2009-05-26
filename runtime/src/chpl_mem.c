//
// Shared code for different mem implementations in mem-*/chpl_cfg_mem.c
//
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "chpl_mem.h"
#include "chplmemtrack.h"
#include "chplrt.h"
#include "chpltypes.h"
#include "error.h"


int heapInitialized = 0;


#define MEM_DIAGNOSIS 0
static int memDiagnosisFlag = 0;


void
confirm(void* memAlloc, const char* description, int32_t lineno, 
        chpl_string filename) {
  if (!memAlloc) {
    const int messageSize = 1024;
    char message[messageSize];
    snprintf(message, messageSize, "Out of memory allocating \"%s\"", description);
    chpl_error(message, lineno, filename);
  }
}


size_t
computeChunkSize(size_t number, size_t size, chpl_bool zeroOK, 
                 int32_t lineno, chpl_string filename) {
  if (number == 0 && size == 0 && !zeroOK) {
    chpl_internal_error("Attempting to allocate 0 bytes of memory");
  } else if (number > 0 && size > SIZE_MAX/number) {
    chpl_error("Attempting to allocate > max(size_t) bytes of memory", lineno, filename);
  }
  return number * size;
}


void
track_malloc(void* addr, size_t chunk, size_t number, size_t size,
             const char* description, chpl_bool userCode, int32_t lineno, chpl_string filename) {
  if (memtrace && (!MEM_DIAGNOSIS || memDiagnosisFlag)) {
    printToMemLog("malloc", number, size, description, userCode,
                  lineno, filename, addr, NULL);
  }
  if (memtrack) {
    installMemory(addr, number, size, description, userCode);
    if (memstat) {
      increaseMemStat(chunk, userCode, lineno, filename);
    }
  }
}


void
track_free(void* addr, chpl_bool userCode, int32_t lineno, chpl_string filename) {
  if (addr != NULL) {
    if (memtrace) {
      if (memtrack) {
        memTableEntry* memEntry;
        memEntry = lookupMemory(addr);
        if (!MEM_DIAGNOSIS || memDiagnosisFlag)
          printToMemLog("free", memEntry->number, memEntry->size,
                        memEntry->description, userCode, lineno, filename,
                        addr, NULL);
      } else if (!MEM_DIAGNOSIS || memDiagnosisFlag)
        printToMemLog("free", 0, 0, "", userCode, lineno, filename, addr, NULL);
    }

    if (memtrack) {
      if (memstat) {
        memTableEntry* memEntry = lookupMemory(addr);
        if (memEntry)
          decreaseMemStat(memEntry->number * memEntry->size, userCode);
      }
      removeMemory(addr, lineno, filename);
    }
  }
}


void
track_realloc(size_t newChunk, void* moreMemAlloc,
              void* memAlloc, size_t number, size_t size, const char* description,
              int32_t lineno, chpl_string filename) {
  memTableEntry* memEntry = NULL;

  if (!heapInitialized) {
    chpl_error("chpl_realloc called before the heap is initialized", lineno, filename);
  }
  if (memtrack && memAlloc != NULL) {
    memEntry = lookupMemory(memAlloc);
    if (!memEntry)
      chpl_error(chpl_glom_strings(3, "Attempting to realloc memory for ",
                                   description, " that wasn't allocated"),
                 lineno, filename);
  }

  if (memtrack) { 
    if (memAlloc != NULL) {
      if (memEntry) {
        if (memstat)
          decreaseMemStat(memEntry->number * memEntry->size, false);
        updateMemory(memEntry, memAlloc, moreMemAlloc, number, size);
      }
    } else
      installMemory(moreMemAlloc, number, size, description, false);
    if (memstat)
      increaseMemStat(newChunk, false, lineno, filename);
  }
  if (memtrace && (!MEM_DIAGNOSIS || memDiagnosisFlag)) {
    printToMemLog("realloc", number, size, description, false, lineno, filename,
                  memAlloc, moreMemAlloc);
  }
}


void
chpl_startMemDiagnosis() {
  memDiagnosisFlag = 1;
}


void
chpl_stopMemDiagnosis() {
  memDiagnosisFlag = 0;
}
