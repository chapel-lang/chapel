/*
 * Copyright 2020 Hewlett Packard Enterprise Development LP
 * Copyright 2004-2019 Cray Inc.
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
#include "chpl-mem-sys.h"  // mem layer not initialized yet, need system alloc
#include "chpl-tasks.h"
#include "chpltypes.h"
#include "chpl-comm.h"
#include "chpl-comm-internal.h"
#include "chplcgfns.h"
#include "chpl-linefile-support.h"
#include "config.h"
#include "error.h"

#include "chpl-comm-compiler-macros.h"

#include <assert.h>
#include <math.h>
#include <pthread.h>
#include <string.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>

int chpl_verbose_mem = 0;
int chpl_memTrack = 0;

static void
printMemAllocs(chpl_mem_descInt_t description, int64_t threshold,
               int32_t lineno, int32_t filename);


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

typedef struct memTableEntry_struct { /* table entry */
  size_t number;
  size_t size;
  chpl_mem_descInt_t description;
  void* memAlloc;
  int32_t lineno;
  int32_t filename;
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
static c_string memLeaksByDesc = "";
static _Bool memLeaks = false;
static size_t memMax = 0;
static size_t memThreshold = 0;
static c_string memLog = NULL;
static FILE* memLogFile = NULL;
static c_string memLeaksLog = NULL;

static size_t totalMem = 0;       /* total memory currently allocated */
static size_t maxMem = 0;         /* maximum total memory during run  */
static size_t totalAllocated = 0; /* total memory allocated */
static size_t totalFreed = 0;     /* total memory freed */
static size_t totalEntries = 0;     /* number of entries in hash table */


// We can't use a sync var for concurrency control here.  The Qthreads
// internal memory allocator shim references this memory tracking code
// via the Chapel runtime public memory layer interface.  Referring to a
// sync var here when exiting (to report memTrack results, say), after
// the tasking layer is shut down, ends up trying to create a qthread in
// the terminated Qthreads library.  Chaos results.  We also cannot use
// an atomic var, because with CHPL_ATOMICS=locks those are implemented
// by means of sync vars.  So, we use a pthread mutex.  Note that this
// is only safe if we cannot switch tasks on a pthread while holding the
// mutex and then try to lock it recursively.  Currently that is the
// case, since we do not yield while holding the mutex.
// 
static pthread_mutex_t memTrack_lockVar = PTHREAD_MUTEX_INITIALIZER;

static inline
void memTrack_lock(void) {
  (void) pthread_mutex_lock(&memTrack_lockVar);
}

static inline
void memTrack_unlock(void) {
  (void) pthread_mutex_unlock(&memTrack_lockVar);
}



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

  chpl_memTrack = (local_memTrack
                   || memStats
                   || memLeaksByType
                   || (memLeaksByDesc && strcmp(memLeaksByDesc, ""))
                   || memLeaks
                   || memMax > 0
                   || memLeaksLog != NULL);
  

  if (!memLog) {
    memLogFile = stdout;
  } else {
    if (chpl_numNodes == 1) {
      memLogFile = fopen(memLog, "w");
    } else {
      char* filename = (char*)sys_malloc((strlen(memLog)+10)*sizeof(char));
      sprintf(filename, "%s.%" PRI_c_nodeid_t, memLog, chpl_nodeID);
      memLogFile = fopen(filename, "w");
      sys_free(filename);
    }
  }

  if (chpl_memTrack) {
    hashSizeIndex = 0;
    hashSize = hashSizes[hashSizeIndex];
    memTable = sys_calloc(hashSize, sizeof(memTableEntry*));
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


static void increaseMemStat(size_t chunk, int32_t lineno, int32_t filename) {
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
  newMemTable = sys_calloc(newHashSize, sizeof(memTableEntry*));

  for (i = 0; i < hashSize; i++) {
    for (me = memTable[i]; me != NULL; me = next) {
      next = me->nextInBucket;
      newHashValue = hash(me->memAlloc, newHashSize);
      me->nextInBucket = newMemTable[newHashValue];
      newMemTable[newHashValue] = me;
    }
  }

  sys_free(memTable);
  memTable = newMemTable;
  hashSize = newHashSize;
  hashSizeIndex = newHashSizeIndex;
}

static void addMemTableEntry(void *memAlloc, size_t number, size_t size,
                             chpl_mem_descInt_t description, int32_t lineno,
                             int32_t filename) {
  unsigned hashValue;
  memTableEntry* memEntry;

  if ((totalEntries+1)*2 > hashSize && hashSizeIndex < NUM_HASH_SIZE_INDICES-1)
    resizeTable(1);

  memEntry = (memTableEntry*) sys_calloc(1, sizeof(memTableEntry));
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
  memEntry->filename = filename;
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


uint64_t chpl_memoryUsed(int32_t lineno, int32_t filename) {
  if (!chpl_memTrack) {
    chpl_warning("invalid call to memoryUsed(); rerun with --memTrack",
                 lineno, filename);
    return 0;
  }

  return (uint64_t)totalMem;
}


void chpl_printMemAllocStats(int32_t lineno, int32_t filename) {
  if (!chpl_memTrack) {
    chpl_warning("invalid call to printMemAllocStats(); rerun with --memTrack",
                 lineno, filename);
    return;
  }

  //
  // To reduce the likelihood of corrupted output in multilocale runs,
  // print everything into an internal buffer just large enough to hold
  // it all, then send that to the memory log file in a single call.
  // Also in multi-locale runs, prefix each line with a node-specific
  // string to allow sorting the output by node ID for easier reading.
  //

  //
  // First, construct the line prefix.
  //
  const int nodeWidth = (int) lrint(ceil(log10((double) chpl_numNodes)));
  char prefixBuf[15 + nodeWidth + 1]; // room for "memStats: node N"
  if (chpl_numNodes == 1) {
    snprintf(prefixBuf, sizeof(prefixBuf), "memStats:");
  } else {
    snprintf(prefixBuf, sizeof(prefixBuf), "memStats: node %*d",
             nodeWidth, chpl_nodeID);
  }

  //
  // Take a pre-run through the descriptions and values to figure
  // out how long each line will need to be.
  //
  static const struct {
    const char* desc;
    size_t* val;
  } descsVals[] = {
    { "Allocated Now:", &totalMem },
    { "Allocation High Water Mark:", &maxMem },
    { "Sum of Allocations:", &totalAllocated },
    { "Sum of Frees:", &totalFreed },
  };
  const int nDescsVals = sizeof(descsVals) / sizeof(descsVals[0]);

  int descWidth = 0;
  int memWidth = 0;

  for (int i = 0; i < nDescsVals; i++) {
    const int thisDescWidth = strlen(descsVals[i].desc);
    if (thisDescWidth > descWidth)
      descWidth = thisDescWidth;
    const int thisMemWidth =
                (*descsVals[i].val == 0)
                ? 1
                : (int) lrint(ceil(log10((double) *descsVals[i].val)));
    if (thisMemWidth > memWidth)
      memWidth = thisMemWidth;
  }

  //
  // Now finally, size the buffer, print the information, and send it
  // to the memory log file.
  //
  char buf[4 * (strlen(prefixBuf) + 1 + descWidth + 1 + memWidth + 1) + 1];
  size_t len;

  memTrack_lock();

  len = 0;
  for (int i = 0; i < nDescsVals; i++) {
    len += snprintf(buf + len, sizeof(buf) - len,
                    "%s %-*s %*zd\n",
                    prefixBuf,
                    descWidth, descsVals[i].desc,
                    memWidth, *descsVals[i].val);
  }

  memTrack_unlock();

  fputs(buf, memLogFile);
}



static int memTableEntryCmp(const void* p1, const void* p2) {
  return *(size_t*)p2 - *(size_t*)p1;
}


static void printMemAllocsByType(_Bool forLeaks,
                                 int32_t lineno, int32_t filename) {
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

  table = (size_t*)sys_calloc(numEntries, 3*sizeof(size_t));

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

  sys_free(table);
}


void chpl_printMemAllocsByType(int32_t lineno, int32_t filename) {
  printMemAllocsByType(false /* forLeaks */, lineno, filename);
}


static chpl_mem_descInt_t
find_desc(const char* descString)
{
  const int numEntries = CHPL_RT_MD_NUM+chpl_mem_numDescs;
  for (chpl_mem_descInt_t i = 0; i < numEntries; ++i)
    if (! strcmp(descString, chpl_mem_descString(i)))
      return i;
  return -1;
}


// Print only those memTable entries that match the given descriptor string and
// exceed the given threshold.
// If no match is found, all entries (exceeding the given threshold) are printed.
void chpl_printMemAllocsByDesc(const char* descString, int64_t threshold,
                               int32_t lineno, int32_t filename)
{
  chpl_mem_descInt_t description = find_desc(descString);
  printMemAllocs(description, threshold, lineno, filename);
}


static int descCmp(const void* p1, const void* p2) {
  memTableEntry* m1 = *(memTableEntry**)p1;
  memTableEntry* m2 = *(memTableEntry**)p2;
  c_string m1Filename;
  c_string m2Filename;

  int val = strcmp(chpl_mem_descString(m1->description), chpl_mem_descString(m2->description));
  if (val == 0 && m1->filename && m2->filename) {
    m1Filename = chpl_lookupFilename(m1->filename);
    m2Filename = chpl_lookupFilename(m2->filename);
    val = strcmp(m1Filename, m2Filename);
  }
  if (val == 0)
    val = (m1->lineno < m2->lineno) ? -1 : ((m1->lineno > m2->lineno) ? 1 : 0);
  return val;
}


// If description is -1, print all entries; otherwise print only those with the
// matching CHPL_RT_MD_ descriptor.
// Print only those entries exceeding threshold.
void chpl_printMemAllocs(int64_t threshold, int32_t lineno, int32_t filename) {
  printMemAllocs(-1, threshold, lineno, filename);
}

static void
printMemAllocs(chpl_mem_descInt_t description, int64_t threshold,
               int32_t lineno, int32_t filename) {
  const int numberWidth   = 9;
  const int precision     = sizeof(uintptr_t) * 2;
  const int addressWidth  = precision+4;
  const int descWidth     = 33;
  int filenameWidth       = strlen("Allocated Memory (Bytes)");
  int totalWidth;
  int filenameLength;

  memTableEntry* memEntry;
  c_string memEntryFilename;
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
      if (description != -1 && memEntry->description != description)
        continue;
      n += 1;
      if (memEntry->filename) {
        memEntryFilename = chpl_lookupFilename(memEntry->filename);
        filenameLength = strlen(memEntryFilename);
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

  table = (memTableEntry**)sys_malloc(n*sizeof(memTableEntry*));
  if (!table)
    chpl_error("out of memory printing memory table", lineno, filename);

  n = 0;
  for (i = 0; i < hashSize; i++) {
    for (memEntry = memTable[i]; memEntry != NULL; memEntry = memEntry->nextInBucket) {
      size_t chunk = memEntry->number * memEntry->size;
      if (chunk < threshold)
        continue;
      if (description != -1 && memEntry->description != description)
        continue;
      table[n++] = memEntry;
    }
  }
  qsort(table, n, sizeof(memTableEntry*), descCmp);

  loc = (char*)sys_malloc((filenameWidth+numberWidth+1)*sizeof(char));

  for (i = 0; i < n; i++) {
    memEntry = table[i];
    if (memEntry->filename) {
      memEntryFilename = chpl_lookupFilename(memEntry->filename);
      sprintf(loc, "%s:%" PRId32, memEntryFilename, memEntry->lineno);
    } else {
      sprintf(loc, "--");
    }
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

  sys_free(table);
  sys_free(loc);
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
  if (memLeaksByDesc && strcmp(memLeaksByDesc, "")) {
    fprintf(memLogFile, "\n");
    chpl_printMemAllocsByDesc(memLeaksByDesc, memThreshold, 0, 0);
  }
  if (memLeaks) {
    fprintf(memLogFile, "\n");
    printMemAllocs(-1, memThreshold, 0, 0);
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
                       int32_t lineno, int32_t filename) {
  if (number * size > memThreshold) {
    if (chpl_memTrack && chpl_mem_descTrack(description)) {
      memTrack_lock();
      addMemTableEntry(memAlloc, number, size, description, lineno, filename);
      memTrack_unlock();
    }
    if (chpl_verbose_mem) {
      fprintf(memLogFile, "%" PRI_c_nodeid_t ": %s:%" PRId32
                          ": allocate %zuB of %s at %p\n",
              chpl_nodeID, (filename ? chpl_lookupFilename(filename) : "--"),
              lineno, number * size, chpl_mem_descString(description),
              memAlloc);
    }
  }
}


void chpl_track_free(void* memAlloc, int32_t lineno, int32_t filename) {
  memTableEntry* memEntry = NULL;
  if (chpl_memTrack) {
    memTrack_lock();
    memEntry = removeMemTableEntry(memAlloc);
    if (memEntry) {
      if (chpl_verbose_mem) {
        fprintf(memLogFile, "%" PRI_c_nodeid_t ": %s:%" PRId32
                            ": free %zuB of %s at %p\n",
                chpl_nodeID, (filename ? chpl_lookupFilename(filename) : "--"),
                lineno, memEntry->number * memEntry->size,
                chpl_mem_descString(memEntry->description), memAlloc);
      }
      sys_free(memEntry);
    }
    memTrack_unlock();
  } else if (chpl_verbose_mem && !memEntry) {
    fprintf(memLogFile, "%" PRI_c_nodeid_t ": %s:%" PRId32 ": free at %p\n",
            chpl_nodeID, (filename ? chpl_lookupFilename(filename) : "--"),
            lineno, memAlloc);
  }
}


void chpl_track_realloc_pre(void* memAlloc, size_t size,
                         chpl_mem_descInt_t description,
                         int32_t lineno, int32_t filename) {
  memTableEntry* memEntry = NULL;

  if (chpl_memTrack && size > memThreshold) {
    memTrack_lock();
    if (memAlloc) {
      memEntry = removeMemTableEntry(memAlloc);
      if (memEntry)
        sys_free(memEntry);
    }
    memTrack_unlock();
  }
}


void chpl_track_realloc_post(void* moreMemAlloc,
                         void* memAlloc, size_t size,
                         chpl_mem_descInt_t description,
                         int32_t lineno, int32_t filename) {
  if (size > memThreshold) {
    if (chpl_memTrack && chpl_mem_descTrack(description)) {
      memTrack_lock();
      addMemTableEntry(moreMemAlloc, 1, size, description, lineno, filename);
      memTrack_unlock();
    }
    if (chpl_verbose_mem) {
      fprintf(memLogFile, "%" PRI_c_nodeid_t ": %s:%" PRId32
                          ": reallocate %zuB of %s at %p -> %p\n",
              chpl_nodeID, (filename ? chpl_lookupFilename(filename) : "--"),
              lineno, size, chpl_mem_descString(description), memAlloc,
              moreMemAlloc);
    }
  }
}

void chpl_startVerboseMem() {
  chpl_verbose_mem = 1;
  chpl_comm_bcast_rt_private(chpl_verbose_mem);
}

void chpl_stopVerboseMem() {
  chpl_verbose_mem = 0;
  chpl_comm_bcast_rt_private(chpl_verbose_mem);
}

void chpl_startVerboseMemHere() {
  chpl_verbose_mem = 1;
}

void chpl_stopVerboseMemHere() {
  chpl_verbose_mem = 0;
}
