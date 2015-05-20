/*
 * Copyright 2004-2015 Cray Inc.
 * Other additional copyright holders may be indicated within.
 * 
 * The entirety of this work is licensed under the Apache License,
 * Version 2.0 (the "License"); you may not use this file except
 * in compliance with the License.
 * 
 * You may obtain a copy of the License at
 * 
 *     http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "chplrt.h"

#include "chplmemtrack.h"
#include "chpl-mem.h"
#include "chpl-mem-desc.h"
#include "chpl-tasks.h"
#include "chpltypes.h"
#include "chpl-comm.h"
#include "chplcgfns.h"
#include "config.h"
#include "error.h"

#include "chpl-comm-compiler-macros.h"

#include <assert.h>
#include <string.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>


//
// This is in the modules, in MemTracking.chpl.
//
extern void chpl_memTracking_returnConfigVals(chpl_bool* memTrack,
                                              chpl_bool* memStats,
                                              chpl_bool* memLeaksByType,
                                              c_string* memLeaksByDesc,
                                              chpl_bool* memLeaks,
                                              size_t* memMax,
                                              size_t* memThreshold,
                                              c_string* memLog,
                                              c_string* memLeaksLog);

chpl_bool chpl_memTrack = false;

#undef malloc
#undef calloc
#undef free

typedef struct memTableEntry_struct { /* table entry */
  size_t number;
  size_t size;
  chpl_mem_descInt_t description;
  void* memAlloc;
  int32_t lineno;
  c_string filename;
  struct memTableEntry_struct* nextInBucket;
} memTableEntry;

#define NUM_HASH_SIZE_INDICES 20

static int hashSizes[NUM_HASH_SIZE_INDICES] = { 1543, 3079, 6151, 12289, 24593, 49157, 98317,
                                                196613, 393241, 786433, 1572869, 3145739,
                                                6291469, 12582917, 25165843, 50331653,
                                                100663319, 201326611, 402653189, 805306457 };
static int hashSizeIndex = 0;
static int hashSize = 0;

static memTableEntry** memTable = NULL;

static _Bool memStats = false;
static _Bool memLeaksByType = false;
static c_string memLeaksByDesc = NULL;
static _Bool memLeaks = false;
static size_t memMax = 0;
static size_t memThreshold = 0;
static c_string memLog = "";
static FILE* memLogFile = NULL;
static c_string memLeaksLog = "";

static size_t totalMem = 0;       /* total memory currently allocated */
static size_t maxMem = 0;         /* maximum total memory during run  */
static size_t totalAllocated = 0; /* total memory allocated */
static size_t totalFreed = 0;     /* total memory freed */
static size_t totalEntries = 0;     /* number of entries in hash table */

static chpl_sync_aux_t memTrack_sync;


void chpl_setMemFlags(void) {
  chpl_bool local_memTrack = false;

  //
  // Get the values of the memTracking config consts from the module.
  // The runtime also has the execution-time settings for these same
  // config consts, but if their default values are changed at compile
  // time only the emitted code for the module reflects that; the
  // runtime doesn't know about it.  So, we need to get the values
  // from the module.
  //
  // We use local_memTrack here so that the module code doesn't store
  // directly to our flag and effectively turn on memory tracking too
  // early.  In the first version of this code I passed &chpl_memTrack
  // itself, and for comm=gasnet we ended up tracking an extra 2 bytes
  // of space the comm layer allocated as a result of our own call to
  // chpl_memTracking_returnConfigVals().
  //
  chpl_memTracking_returnConfigVals(&local_memTrack,
                                    &memStats,
                                    &memLeaksByType,
                                    &memLeaksByDesc,
                                    &memLeaks,
                                    &memMax,
                                    &memThreshold,
                                    &memLog,
                                    &memLeaksLog);

  if (local_memTrack
      || memStats
      || memLeaksByType
      || memLeaksByDesc
      || memLeaks
      || memMax > 0
      || strcmp(memLeaksLog, "") != 0) {
    chpl_memTrack = true;
  }

  if (strcmp(memLog, "") == 0) {
    memLogFile = stdout;
  } else {
    if (chpl_numNodes == 1) {
      memLogFile = fopen(memLog, "w");
    } else {
      char* filename = (char*)malloc((strlen(memLog)+10)*sizeof(char));
      sprintf(filename, "%s.%" FORMAT_c_nodeid_t, memLog, chpl_nodeID);
      memLogFile = fopen(filename, "w");
      free(filename);
    }
  }

  if (chpl_memTrack) {
    chpl_sync_initAux(&memTrack_sync);
    hashSizeIndex = 0;
    hashSize = hashSizes[hashSizeIndex];
    memTable = calloc(hashSize, sizeof(memTableEntry*));
  }
}


static unsigned hash(void* memAlloc, int hashSize) {
  unsigned hashValue = 0;
  char* fakeCharPtr = (char*)&memAlloc;
  size_t i;
  for (i = 0; i < sizeof(void*); i++) {
    hashValue = *fakeCharPtr + 31 * hashValue;
    fakeCharPtr++;
  }
  return hashValue % hashSize;
}


static void increaseMemStat(size_t chunk, int32_t lineno, c_string filename) {
  totalMem += chunk;
  totalAllocated += chunk;
  if (memMax && (totalMem > memMax)) {
    chpl_error("Exceeded memory limit", lineno, filename);
  }
  if (totalMem > maxMem)
    maxMem = totalMem;
}


static void decreaseMemStat(size_t chunk) {
  totalMem -= chunk; // > totalMem ? 0 : totalMem - chunk;
  totalFreed += chunk;
}


static void
resizeTable(int direction) {
  memTableEntry** newMemTable = NULL;
  int newHashSizeIndex, newHashSize, newHashValue;
  int i;
  memTableEntry* me;
  memTableEntry* next;

  newHashSizeIndex = hashSizeIndex + direction;
  newHashSize = hashSizes[newHashSizeIndex];
  newMemTable = calloc(newHashSize, sizeof(memTableEntry*));

  for (i = 0; i < hashSize; i++) {
    for (me = memTable[i]; me != NULL; me = next) {
      next = me->nextInBucket;
      newHashValue = hash(me->memAlloc, newHashSize);
      me->nextInBucket = newMemTable[newHashValue];
      newMemTable[newHashValue] = me;
    }
  }

  free(memTable);
  memTable = newMemTable;
  hashSize = newHashSize;
  hashSizeIndex = newHashSizeIndex;
}


static void addMemTableEntry(void* memAlloc, size_t number, size_t size, chpl_mem_descInt_t description, int32_t lineno, c_string filename) {
  unsigned hashValue;
  memTableEntry* memEntry;

  if ((totalEntries+1)*2 > hashSize && hashSizeIndex < NUM_HASH_SIZE_INDICES-1)
    resizeTable(1);

  memEntry = (memTableEntry*) calloc(1, sizeof(memTableEntry));
  if (!memEntry) {
    chpl_error("memtrack fault: out of memory allocating memtrack table",
               lineno, filename);
  }

  hashValue = hash(memAlloc, hashSize);
  memEntry->nextInBucket = memTable[hashValue];
  memTable[hashValue] = memEntry;
  memEntry->description = description;
  memEntry->memAlloc = memAlloc;
  memEntry->lineno = lineno;
  memEntry->filename = filename; // do we want to copy this string?
  memEntry->number = number;
  memEntry->size = size;
  increaseMemStat(number*size, lineno, filename);
  totalEntries += 1;
}


static memTableEntry* removeMemTableEntry(void* address) {
  unsigned hashValue = hash(address, hashSize);
  memTableEntry* thisBucketEntry = memTable[hashValue];
  memTableEntry* deletedBucket = NULL;

  if (!thisBucketEntry)
    return NULL;

  if (thisBucketEntry->memAlloc == address) {
    memTable[hashValue] = thisBucketEntry->nextInBucket;
    deletedBucket = thisBucketEntry;
  } else {
    for (thisBucketEntry = memTable[hashValue];
         thisBucketEntry != NULL;
         thisBucketEntry = thisBucketEntry->nextInBucket) {

      memTableEntry* nextBucketEntry = thisBucketEntry->nextInBucket;

      if (nextBucketEntry && nextBucketEntry->memAlloc == address) {
        thisBucketEntry->nextInBucket = nextBucketEntry->nextInBucket;
        deletedBucket = nextBucketEntry;
      }
    }
  }
  if (deletedBucket) {
    decreaseMemStat(deletedBucket->number * deletedBucket->size);
    totalEntries -= 1;
    if (totalEntries*8 < hashSize && hashSizeIndex > 0)
      resizeTable(-1);
  }
  return deletedBucket;
}


uint64_t chpl_memoryUsed(int32_t lineno, c_string filename) {
  if (!chpl_memTrack) {
    chpl_warning("invalid call to memoryUsed(); rerun with --memTrack",
                 lineno, filename);
    return 0;
  }

  return (uint64_t)totalMem;
}


void chpl_printMemAllocStats(int32_t lineno, c_string filename) {
  if (!chpl_memTrack) {
    chpl_warning("invalid call to printMemAllocStats(); rerun with --memTrack",
                 lineno, filename);
    return;
  }

  chpl_sync_lock(&memTrack_sync);
  fprintf(memLogFile, "=================\n");
  fprintf(memLogFile, "Memory Statistics\n");
  if (chpl_numNodes == 1) {
    fprintf(memLogFile, "==============================================================\n");
    fprintf(memLogFile, "Current Allocated Memory               %zd\n", totalMem);
    fprintf(memLogFile, "Maximum Simultaneous Allocated Memory  %zd\n", maxMem);
    fprintf(memLogFile, "Total Allocated Memory                 %zd\n", totalAllocated);
    fprintf(memLogFile, "Total Freed Memory                     %zd\n", totalFreed);
    fprintf(memLogFile, "==============================================================\n");
  } else {
    int i;
    fprintf(memLogFile, "==============================================================\n");
    fprintf(memLogFile, "Locale\n");
    fprintf(memLogFile, "           Current Allocated Memory\n");
    fprintf(memLogFile, "                      Maximum Simultaneous Allocated Memory\n");
    fprintf(memLogFile, "                                 Total Allocated Memory\n");
    fprintf(memLogFile, "                                            Total Freed Memory\n");
    fprintf(memLogFile, "==============================================================\n");
    for (i = 0; i < chpl_numNodes; i++) {
      static size_t m1, m2, m3, m4;
      chpl_gen_comm_get(&m1, i, &totalMem, sizeof(size_t), -1 /* broke for hetero */, 1, lineno, filename);
      chpl_gen_comm_get(&m2, i, &maxMem, sizeof(size_t), -1 /* broke for hetero */, 1, lineno, filename);
      chpl_gen_comm_get(&m3, i, &totalAllocated,  sizeof(size_t), -1 /* broke for hetero */, 1, lineno, filename);
      chpl_gen_comm_get(&m4, i, &totalFreed, sizeof(size_t), -1 /*broke for hetero */, 1, lineno, filename);
      fprintf(memLogFile, "%-9d  %-9zu  %-9zu  %-9zu  %-9zu\n", i, m1, m2, m3, m4);
    }
    fprintf(memLogFile, "==============================================================\n");
  }
  chpl_sync_unlock(&memTrack_sync);
}



static int memTableEntryCmp(const void* p1, const void* p2) {
  return *(size_t*)p2 - *(size_t*)p1;
}


static void printMemAllocsByType(_Bool forLeaks,
                                 int32_t lineno, c_string filename) {
  size_t* table;
  memTableEntry* me;
  int i;
  const int numberWidth   = 9;
  const int numEntries = CHPL_RT_MD_NUM+chpl_mem_numDescs;

  if (!chpl_memTrack) {
    chpl_warning("invalid call to printMemAllocsByType(); rerun with "
                 "--memTrack",
                 lineno, filename);
    return;
  }

  table = (size_t*)calloc(numEntries, 3*sizeof(size_t));

  for (i = 0; i < hashSize; i++) {
    for (me = memTable[i]; me != NULL; me = me->nextInBucket) {
      table[3*me->description] += me->number*me->size;
      table[3*me->description+1] += 1;
      table[3*me->description+2] = me->description;
    }
  }

  qsort(table, numEntries, 3*sizeof(size_t), memTableEntryCmp);

  if (forLeaks) {
    fprintf(memLogFile, "====================\n");
    fprintf(memLogFile, "Leaked Memory Report\n");
    fprintf(memLogFile, "==============================================================\n");
    fprintf(memLogFile, "Number of leaked allocations\n");
    fprintf(memLogFile, "           Total leaked memory (bytes)\n");
  }
  else {
    fprintf(memLogFile, "================================\n");
    fprintf(memLogFile, "Memory Allocation Report by Type\n");
    fprintf(memLogFile, "==============================================================\n");
    fprintf(memLogFile, "Number of allocations\n");
    fprintf(memLogFile, "           Total allocated bytes\n");
  }

  fprintf(memLogFile, "                      Description of allocation\n");
  fprintf(memLogFile, "==============================================================\n");
  for (i = 0; i < 3*(CHPL_RT_MD_NUM+chpl_mem_numDescs); i += 3) {
    if (table[i] > 0) {
      fprintf(memLogFile, "%-*zu  %-*zu  %s\n",
             numberWidth, table[i+1],
             numberWidth, table[i],
             chpl_mem_descString(table[i+2]));
    }
  }
  fprintf(memLogFile, "==============================================================\n");

  free(table);
}


void chpl_printMemAllocsByType(int32_t lineno, c_string filename) {
  printMemAllocsByType(false /* forLeaks */, lineno, filename);
}


static chpl_mem_descInt_t
find_desc(const char* descString)
{
  for (chpl_mem_descInt_t i = 0; i < chpl_mem_numDescs; ++i)
    if (! strcmp(descString, chpl_mem_descString(i)))
      return i;
  return -1;
}


// Print only those memTable entries that match the given descriptor string and
// exceed the given threshold.
// If no match is found, all entries (exceeding the given threshold) are printed.
void chpl_printMemAllocsByDesc(const char* descString, int64_t threshold,
                               int32_t lineno, c_string filename)
{
  chpl_mem_descInt_t description = find_desc(descString);
  chpl_printMemAllocs(description, threshold, lineno, filename);
}


static int descCmp(const void* p1, const void* p2) {
  memTableEntry* m1 = *(memTableEntry**)p1;
  memTableEntry* m2 = *(memTableEntry**)p2;

  int val = strcmp(chpl_mem_descString(m1->description), chpl_mem_descString(m2->description));
  if (val == 0 && m1->filename && m2->filename)
    val = strcmp(m1->filename, m2->filename);
  if (val == 0)
    val = (m1->lineno < m2->lineno) ? -1 : ((m1->lineno > m2->lineno) ? 1 : 0);
  return val;
}


// If description is -1, print all entries; otherwise print only those with the
// matching CHPL_RT_MD_ descriptor.
// Print only those entries exceeding threshold.
void chpl_printMemAllocs(chpl_mem_descInt_t description, int64_t threshold,
                         int32_t lineno, c_string filename) {
  const int numberWidth   = 9;
  const int precision     = sizeof(uintptr_t) * 2;
  const int addressWidth  = precision+4;
  const int descWidth     = 33;
  int filenameWidth       = strlen("Allocated Memory (Bytes)");
  int totalWidth;

  memTableEntry* memEntry;
  int n, i;
  char* loc;
  memTableEntry** table;

  if (!chpl_memTrack) {
    chpl_warning("invalid call to printMemAllocs(); rerun with --memTrack",
                 lineno, filename);
    return;
  }

  n = 0;
  filenameWidth = strlen("Allocated Memory (Bytes)");
  for (i = 0; i < hashSize; i++) {
    for (memEntry = memTable[i]; memEntry != NULL; memEntry = memEntry->nextInBucket) {
      size_t chunk = memEntry->number * memEntry->size;
      if (chunk < threshold)
        continue;
      if (description != -1 && memEntry->description == description)
        continue;
      n += 1;
      if (memEntry->filename) {
        int filenameLength = strlen(memEntry->filename);
        if (filenameLength > filenameWidth)
          filenameWidth = filenameLength;
      }
    }
  }

  totalWidth = filenameWidth+numberWidth*4+descWidth+20;
  for (i = 0; i < totalWidth; i++)
    fprintf(memLogFile, "=");
  fprintf(memLogFile, "\n");
  fprintf(memLogFile, "%-*s%-*s%-*s%-*s%-*s%-*s\n",
         filenameWidth+numberWidth, "Allocated Memory (Bytes)",
         numberWidth, "Number",
         numberWidth, "Size",
         numberWidth, "Total",
         descWidth, "Description",
         20, "Address");
  for (i = 0; i < totalWidth; i++)
    fprintf(memLogFile, "=");
  fprintf(memLogFile, "\n");

  table = (memTableEntry**)malloc(n*sizeof(memTableEntry*));
  if (!table)
    chpl_error("out of memory printing memory table", lineno, filename);

  n = 0;
  for (i = 0; i < hashSize; i++) {
    for (memEntry = memTable[i]; memEntry != NULL; memEntry = memEntry->nextInBucket) {
      size_t chunk = memEntry->number * memEntry->size;
      if (chunk < threshold)
        continue;
      if (description != -1 && memEntry->description == description)
        continue;
      table[n++] = memEntry;
    }
  }
  qsort(table, n, sizeof(memTableEntry*), descCmp);

  loc = (char*)malloc((filenameWidth+numberWidth+1)*sizeof(char));

  for (i = 0; i < n; i++) {
    memEntry = table[i];
    if (memEntry->filename)
      sprintf(loc, "%s:%"PRId32, memEntry->filename, memEntry->lineno);
    else
      sprintf(loc, "--");
    fprintf(memLogFile, "%-*s%-*zu%-*zu%-*zu%-*s%#-*.*" PRIxPTR "\n",
           filenameWidth+numberWidth, loc,
           numberWidth, memEntry->number,
           numberWidth, memEntry->size,
           numberWidth, memEntry->size*memEntry->number,
           descWidth, chpl_mem_descString(memEntry->description),
           addressWidth, precision, (uintptr_t)memEntry->memAlloc);
  }
  for (i = 0; i < totalWidth; i++)
    fprintf(memLogFile, "=");
  fprintf(memLogFile, "\n");
  putchar('\n');

  free(table);
  free(loc);
}


void chpl_reportMemInfo() {
  if (memStats) {
    fprintf(memLogFile, "\n");
    chpl_printMemAllocStats(0, 0);
  }
  if (memLeaksByType) {
    fprintf(memLogFile, "\n");
    printMemAllocsByType(true /* forLeaks */, 0, 0);
  }
  if (memLeaksByDesc) {
    fprintf(memLogFile, "\n");
    chpl_printMemAllocsByDesc(memLeaksByDesc, memThreshold, 0, 0);
  }
  if (memLeaks) {
    fprintf(memLogFile, "\n");
    chpl_printMemAllocs(-1, memThreshold, 0, 0);
  }
  if (memLogFile && memLogFile != stdout)
    fclose(memLogFile);
  if (memLeaksLog && strcmp(memLeaksLog, "")) {
    memLogFile = fopen(memLeaksLog, "a");
    fprintf(memLogFile, "\nCompiler Command : %s\n", chpl_compileCommand);
    fprintf(memLogFile, "Execution Command: %s\n\n", chpl_executionCommand);
    chpl_printMemAllocStats(0, 0);
    fprintf(memLogFile, "\n");
    printMemAllocsByType(true /* forLeaks */, 0, 0);
    fclose(memLogFile);
  }
}


void chpl_track_malloc(void* memAlloc, size_t number, size_t size,
                       chpl_mem_descInt_t description,
                       int32_t lineno, c_string filename) {
  if (number * size > memThreshold) {
    if (chpl_memTrack && chpl_mem_descTrack(description)) {
      chpl_sync_lock(&memTrack_sync);
      addMemTableEntry(memAlloc, number, size, description, lineno, filename);
      chpl_sync_unlock(&memTrack_sync);
    }
    if (chpl_verbose_mem) {
      fprintf(memLogFile,
              "%" FORMAT_c_nodeid_t ": %s:%" PRId32
              ": allocate %zuB of %s at %p\n",
              chpl_nodeID, (filename ? filename : "--"), lineno,
              number*size, chpl_mem_descString(description), memAlloc);
    }
  }
}


void chpl_track_free(void* memAlloc, int32_t lineno, c_string filename) {
  memTableEntry* memEntry = NULL;
  if (chpl_memTrack) {
    chpl_sync_lock(&memTrack_sync);
    memEntry = removeMemTableEntry(memAlloc);
    if (memEntry) {
      if (chpl_verbose_mem) {
        fprintf(memLogFile,
                "%" FORMAT_c_nodeid_t ": %s:%" PRId32
                ": free %zuB of %s at %p\n",
                chpl_nodeID, (filename ? filename : "--"), lineno,
                memEntry->number*memEntry->size,
                chpl_mem_descString(memEntry->description), memAlloc);
      }
      free(memEntry);
    }
    chpl_sync_unlock(&memTrack_sync);
  } else if (chpl_verbose_mem && !memEntry) {
    fprintf(memLogFile,
            "%" FORMAT_c_nodeid_t ": %s:%" PRId32 ": free at %p\n",
            chpl_nodeID, (filename ? filename : "--"), lineno, memAlloc);
  }
}


void chpl_track_realloc_pre(void* memAlloc, size_t size,
                         chpl_mem_descInt_t description,
                         int32_t lineno, c_string filename) {
  memTableEntry* memEntry = NULL;

  if (chpl_memTrack && size > memThreshold) {
    chpl_sync_lock(&memTrack_sync);
    if (memAlloc) {
      memEntry = removeMemTableEntry(memAlloc);
      if (memEntry)
        free(memEntry);
    }
    chpl_sync_unlock(&memTrack_sync);
  }
}


void chpl_track_realloc_post(void* moreMemAlloc,
                         void* memAlloc, size_t size,
                         chpl_mem_descInt_t description,
                         int32_t lineno, c_string filename) {
  if (size > memThreshold) {
    if (chpl_memTrack && chpl_mem_descTrack(description)) {
      chpl_sync_lock(&memTrack_sync);
      addMemTableEntry(moreMemAlloc, 1, size, description, lineno, filename);
      chpl_sync_unlock(&memTrack_sync);
    }
    if (chpl_verbose_mem) {
      fprintf(memLogFile,
              "%" FORMAT_c_nodeid_t ": %s:%" PRId32
              ": reallocate %zuB of %s at %p -> %p\n",
              chpl_nodeID, (filename ? filename : "--"), lineno,
              size, chpl_mem_descString(description), memAlloc, moreMemAlloc);
    }
  }
}

void chpl_startVerboseMem() {
  chpl_verbose_mem = 1;
  chpl_comm_broadcast_private(2 /* &chpl_verbose_mem */, sizeof(int),
                              -1 /* typeIndex: broke for hetero */);
}

void chpl_stopVerboseMem() {
  chpl_verbose_mem = 0;
  chpl_comm_broadcast_private(2 /* &chpl_verbose_mem */, sizeof(int),
                              -1 /* typeIndex: broke for hetero */);
}

void chpl_startVerboseMemHere() {
  chpl_verbose_mem = 1;
}

void chpl_stopVerboseMemHere() {
  chpl_verbose_mem = 0;
}
