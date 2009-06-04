#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>

#include "chplrt.h"
#include "chplmemtrack.h"
#include "chapel_code.h"
#include "chplthreads.h"
#include "chplcomm.h"
#include "error.h"

#undef malloc
#undef calloc
#undef free

typedef struct memTableEntry_struct { /* table entry */
  size_t number;
  size_t size;
  chpl_memDescInt_t description;
  void* memAlloc;
  int32_t lineno;
  chpl_string filename;
  struct memTableEntry_struct* nextInBucket;
} memTableEntry;

#define HASHSIZE 1019

static memTableEntry* memTable[HASHSIZE];

static _Bool memreport = false;
static _Bool memstat = false;
static int64_t memmax = 0;
static _Bool memtrack = false;
static _Bool memtrackSet = false;

static size_t totalMem = 0;       /* total memory currently allocated */
static size_t maxMem = 0;         /* maximum total memory during run  */
static size_t totalAllocated = 0; /* total memory allocated */
static size_t totalFreed = 0;     /* total memory freed */

static chpl_mutex_t memtrack_lock;

static int chpl_verbose_mem = 0;               // set via startVerboseComm

static unsigned hash(void* memAlloc) {
  unsigned hashValue = 0;
  char* fakeCharPtr = (char*)&memAlloc;
  size_t i;
  for (i = 0; i < sizeof(void*); i++) {
    hashValue = *fakeCharPtr + 31 * hashValue;
    fakeCharPtr++;
  }
  return hashValue % HASHSIZE;
}


static void increaseMemStat(size_t chunk, int32_t lineno, chpl_string filename) {
  totalMem += chunk;
  totalAllocated += chunk;
  if (memmax && (totalMem > memmax)) {
    chpl_error("Exceeded memory limit", lineno, filename);
  }
  if (totalMem > maxMem)
    maxMem = totalMem;
}


static void decreaseMemStat(size_t chunk) {
  totalMem -= chunk; // > totalMem ? 0 : totalMem - chunk;
  totalFreed += chunk;
}


static void addMemTableEntry(void* memAlloc, size_t number, size_t size, chpl_memDescInt_t description, int32_t lineno, chpl_string filename) {
  unsigned hashValue;
  memTableEntry* memEntry;

  memEntry = (memTableEntry*) calloc(1, sizeof(memTableEntry));
  if (!memEntry) {
    chpl_error("memtrack fault: out of memory allocating memtrack table",
               lineno, filename);
  }

  hashValue = hash(memAlloc);
  memEntry->nextInBucket = memTable[hashValue];
  memTable[hashValue] = memEntry;
  memEntry->description = description;
  memEntry->memAlloc = memAlloc;
  memEntry->lineno = lineno;
  memEntry->filename = filename;
  memEntry->number = number;
  memEntry->size = size;
  increaseMemStat(number*size, lineno, filename);
}


static memTableEntry* removeMemTableEntry(void* address) {
  unsigned hashValue = hash(address);
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
  decreaseMemStat(deletedBucket->number * deletedBucket->size);
  return deletedBucket;
}


void chpl_initMemTable(void) {
  chpl_mutex_init(&memtrack_lock);
  if (memtrack) {
    int i;
    for (i = 0; i < HASHSIZE; i++) {
      memTable[i] = NULL;
    }
  }
}

void chpl_setMemmax(int64_t value) {
  memmax = value;
  memtrackSet = true;
}


void chpl_setMemstat(void) {
  memstat = true;
  memtrackSet = true;
}


void chpl_setMemreport(void) {
  memreport = true;
  memtrackSet = true;
}


void chpl_setMemtrack(void) {
  memtrackSet = true;
}


void chpl_startTrackingMem(void) {
    memtrack = memtrackSet;
}


uint64_t chpl_memoryUsed(int32_t lineno, chpl_string filename) {
  if (!memtrack)
    chpl_error("invalid call to memoryUsed(); rerun with --memtrack",
               lineno, filename);
  return (uint64_t)totalMem;
}


void chpl_printMemStat(int32_t lineno, chpl_string filename) {
  if (!memtrack)
    chpl_error("invalid call to printMemStat(); rerun with --memtrack",
               lineno, filename);
  chpl_mutex_lock(&memtrack_lock);
  printf("=================\n");
  printf("Memory Statistics\n");
  printf("==============================================================\n");
  printf("Current Allocated Memory               %zd\n", totalMem);
  printf("Maximum Simultaneous Allocated Memory  %zd\n", maxMem);
  printf("Total Allocated Memory                 %zd\n", totalAllocated);
  printf("Total Freed Memory                     %zd\n", totalFreed);
  printf("==============================================================\n");
  chpl_mutex_unlock(&memtrack_lock);
}


static int leakedMemTableEntryCmp(const void* p1, const void* p2) {
  return *(size_t*)p2 - *(size_t*)p1;
}

static void chpl_printLeakedMemTable(void) {
  size_t* table;
  memTableEntry* me;
  int i;
  const int numberWidth   = 9;
  const int numEntries = CHPL_RT_MD_NUM+chpl_num_memDescs;

  table = (size_t*)calloc(numEntries, 3*sizeof(size_t));

  for (i = 0; i < HASHSIZE; i++) {
    for (me = memTable[i]; me != NULL; me = me->nextInBucket) {
      table[3*me->description] += me->number*me->size;
      table[3*me->description+1] += 1;
      table[3*me->description+2] = me->description;
    }
  }

  qsort(table, numEntries, 3*sizeof(size_t), leakedMemTableEntryCmp);

  printf("====================\n");
  printf("Leaked Memory Report\n");
  printf("==============================================================\n");
  printf("Number of leaked allocations\n");
  printf("           Total leaked memory (bytes)\n");
  printf("                      Description of allocation\n");
  printf("==============================================================\n");
  for (i = 0; i < 3*(CHPL_RT_MD_NUM+chpl_num_memDescs); i += 3) {
    if (table[i] > 0) {
      printf("%-*zu  %-*zu  %s\n",
             numberWidth, table[i+1],
             numberWidth, table[i],
             chpl_memDescString(table[i+2]));
    }
  }
  printf("==============================================================\n");

  free(table);
}


void chpl_reportMemInfo() {
  if (memstat) {
    printf("\n");
    chpl_printMemStat(0, 0);
  }
  if (memreport) {
    printf("\n");
    chpl_printLeakedMemTable();
  }
}


static int descCmp(const void* p1, const void* p2) {
  memTableEntry* m1 = *(memTableEntry**)p1;
  memTableEntry* m2 = *(memTableEntry**)p2;

  int val = strcmp(chpl_memDescString(m1->description), chpl_memDescString(m2->description));
  if (val == 0 && m1->filename && m2->filename)
    val = strcmp(m1->filename, m2->filename);
  if (val == 0)
    val = (m1->lineno < m2->lineno) ? -1 : ((m1->lineno > m2->lineno) ? 1 : 0);
  return val;
}


void chpl_printMemTable(int64_t threshold, int32_t lineno, chpl_string filename) {
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

  if (!memtrack)
    chpl_error("The printMemTable function only works with the --memtrack flag", lineno, filename);

  n = 0;
  filenameWidth = strlen("Allocated Memory (Bytes)");
  for (i = 0; i < HASHSIZE; i++) {
    for (memEntry = memTable[i]; memEntry != NULL; memEntry = memEntry->nextInBucket) {
      size_t chunk = memEntry->number * memEntry->size;
      if (chunk >= threshold) {
        n += 1;
        if (memEntry->filename) {
          int filenameLength = strlen(memEntry->filename);
          if (filenameLength > filenameWidth)
            filenameWidth = filenameLength;
        }
      }
    }
  }

  totalWidth = filenameWidth+numberWidth*4+descWidth+20;
  for (i = 0; i < totalWidth; i++)
    printf("=");
  printf("\n");
  printf("%-*s%-*s%-*s%-*s%-*s%-*s\n",
         filenameWidth+numberWidth, "Allocated Memory (Bytes)",
         numberWidth, "Number",
         numberWidth, "Size",
         numberWidth, "Total",
         descWidth, "Description",
         20, "Address");
  for (i = 0; i < totalWidth; i++)
    printf("=");
  printf("\n");

  table = (memTableEntry**)malloc(n*sizeof(memTableEntry*));
  if (!table)
    chpl_error("out of memory printing memory table", lineno, filename);

  n = 0;
  for (i = 0; i < HASHSIZE; i++) {
    for (memEntry = memTable[i]; memEntry != NULL; memEntry = memEntry->nextInBucket) {
      size_t chunk = memEntry->number * memEntry->size;
      if (chunk >= threshold) {
        table[n++] = memEntry;
      }
    }
  }
  qsort(table, n, sizeof(memTableEntry*), descCmp);

  loc = (char*)malloc((filenameWidth+numberWidth+1)*sizeof(char));

  for (i = 0; i < n; i++) {
    memEntry = table[i];
    if (memEntry->filename)
      sprintf(loc, "%s:%d", memEntry->filename, memEntry->lineno);
    else
      sprintf(loc, "--");
    printf("%-*s%-*zu%-*zu%-*zu%-*s%#-*.*" PRIxPTR "\n",
           filenameWidth+numberWidth, loc,
           numberWidth, memEntry->number,
           numberWidth, memEntry->size,
           numberWidth, memEntry->size*memEntry->number,
           descWidth, chpl_memDescString(memEntry->description),
           addressWidth, precision, (uintptr_t)memEntry->memAlloc);
  }
  for (i = 0; i < totalWidth; i++)
    printf("=");
  printf("\n");
  putchar('\n');

  free(table);
  free(loc);
}


void chpl_track_malloc(void* memAlloc, size_t chunk, size_t number, size_t size, chpl_memDescInt_t description, int32_t lineno, chpl_string filename) {
  if (memtrack) {
    chpl_mutex_lock(&memtrack_lock);
    addMemTableEntry(memAlloc, number, size, description, lineno, filename);
    chpl_mutex_unlock(&memtrack_lock);
  }
  if (chpl_verbose_mem)
    printf("%d: %s:%d: allocate %zuB of %s at %p\n", chpl_localeID, filename, lineno, number*size, chpl_memDescString(description), memAlloc);
}


void chpl_track_free(void* memAlloc, int32_t lineno, chpl_string filename) {
  memTableEntry* memEntry = NULL;

  if (memtrack) {
    chpl_mutex_lock(&memtrack_lock);
    memEntry = removeMemTableEntry(memAlloc);
    if (!memEntry)
      chpl_error("memtrack fault: invalid memory free", lineno, filename);
    if (chpl_verbose_mem)
      printf("%d: %s:%d: free %zuB of %s at %p\n", chpl_localeID, filename, lineno, memEntry->number*memEntry->size, chpl_memDescString(memEntry->description), memAlloc);
    free(memEntry);
    chpl_mutex_unlock(&memtrack_lock);
  } else if (chpl_verbose_mem) {
    printf("%d: %s:%d: free at %p\n", chpl_localeID, filename, lineno, memAlloc);
  }
}


void chpl_track_realloc1(void* memAlloc, size_t number, size_t size, chpl_memDescInt_t description, int32_t lineno, chpl_string filename) {
  memTableEntry* memEntry = NULL;

  if (memtrack) {
    chpl_mutex_lock(&memtrack_lock);
    if (memAlloc) {
      memEntry = removeMemTableEntry(memAlloc);
      if (!memEntry)
        chpl_error("memtrack fault: invalid memory realloc", lineno, filename);
      free(memEntry);
    }
    chpl_mutex_unlock(&memtrack_lock);
  }
}


void chpl_track_realloc2(void* moreMemAlloc, size_t newChunk, void* memAlloc, size_t number, size_t size, chpl_memDescInt_t description, int32_t lineno, chpl_string filename) {
  if (memtrack) {
    chpl_mutex_lock(&memtrack_lock);
    addMemTableEntry(moreMemAlloc, number, size, description, lineno, filename);
    chpl_mutex_unlock(&memtrack_lock);
  }
  if (chpl_verbose_mem)
    printf("%d: %s:%d: reallocate %zuB of %s at %p -> %p\n", chpl_localeID, filename, lineno, number*size, chpl_memDescString(description), memAlloc, moreMemAlloc);
}

void chpl_startVerboseMem() {
  chpl_verbose_mem = 1;
  chpl_comm_broadcast_private(&chpl_verbose_mem, sizeof(int));
}

void chpl_stopVerboseMem() {
  chpl_verbose_mem = 0;
  chpl_comm_broadcast_private(&chpl_verbose_mem, sizeof(int));
}

void chpl_startVerboseMemHere() {
  chpl_verbose_mem = 1;
}

void chpl_stopVerboseMemHere() {
  chpl_verbose_mem = 0;
}
