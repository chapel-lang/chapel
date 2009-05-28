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

#define MEM_DIAGNOSIS 0
static int memDiagnosisFlag = 0;

#define CHPL_DEBUG_MEMTRACK 0
#define PRINTF(s) do if (CHPL_DEBUG_MEMTRACK) \
                       { printf("%s%s\n", __func__, s); fflush(stdout); } while (0)

typedef char str80_t[81];

typedef struct _memTableEntry { /* table entry */
  size_t number;
  size_t size;
  char* description;
  void* memAlloc;
  struct _memTableEntry* nextInBucket;
} memTableEntry;

#define HASHSIZE 1019

static memTableEntry* memTable[HASHSIZE];

_Bool memfinalstat = false;
_Bool memstat = false;
_Bool memtrace = false;
_Bool memtrack = false;
static _Bool memfinalstatSet = false;
static _Bool memstatSet = false;
static _Bool memtraceSet = false;
static _Bool memtrackSet = false;

static int64_t memmaxValue = 0;
static int64_t memthresholdValue = 1;
static FILE* memlog = NULL;
static size_t totalMem = 0;         /* total memory currently allocated */
static size_t totalTrackedMem = 0;  /* total memory being tracked */
static size_t maxMem = 0;           /* maximum total memory during run  */

static _Bool alreadyPrintingStat = false;

#ifndef LAUNCHER
static chpl_mutex_t memtrack_lock;
static chpl_mutex_t memstat_lock;
static chpl_mutex_t memtrace_lock;
#endif

static unsigned hash(void* memAlloc);
static void updateMaxMem(void);
static memTableEntry* removeBucketEntry(void* address);
static memTableEntry* lookupMemoryNoLock(void* memAlloc);
static memTableEntry* lookupMemory(void* memAlloc);


/* hashing function */
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


static inline void updateMaxMem(void) {
  if (totalTrackedMem > maxMem)
    maxMem = totalTrackedMem;
}


static memTableEntry* lookupMemoryNoLock(void* memAlloc) {
  memTableEntry* memEntry = NULL;
  memTableEntry* found = NULL;
  unsigned hashValue = hash(memAlloc);

  for (memEntry = memTable[hashValue];
       memEntry != NULL;
       memEntry = memEntry->nextInBucket) {

    if (memEntry->memAlloc == memAlloc) {
      found = memEntry;
      break;
    }
  }
  return found;
}


static memTableEntry* removeBucketEntry(void* address) {
  unsigned hashValue = hash(address);
  memTableEntry* thisBucketEntry = memTable[hashValue];
  memTableEntry* deletedBucket = NULL;

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
  if (deletedBucket == NULL) {
    chpl_internal_error("Hash table entry has disappeared unexpectedly!");
  }
  return deletedBucket;
}


void initMemTable(void) {
  chpl_mutex_init(&memtrace_lock);
  chpl_mutex_init(&memstat_lock);
  chpl_mutex_init(&memtrack_lock);
  if (memtrack) {
    int i;
    for (i = 0; i < HASHSIZE; i++) {
      memTable[i] = NULL;
    }
  }
}

void setMemmax(int64_t value) {
  memmaxValue = value;
  setMemstat();
}


void setMemstat(void) {
  memstatSet = true;
  memtrackSet = true;
}


void setMemfinalstat(void) {
  memfinalstatSet = true;
  memtrackSet = true;
}


void setMemtrack(void) {
  memtrackSet = true;
}


void setMemthreshold(int64_t value) {
  if (!memlog && !memfinalstatSet) {
    chpl_error("--memthreshold useless when used without --memtrace or --memfinalstat", 0, 0);
  }
  memthresholdValue = value >= 0 ? value : -value;
  if (memthresholdValue < 0)
    memthresholdValue = INT64_MAX;
}


void setMemtrace(char* memlogname) {
  memtraceSet = true;
  if (memlogname) {
    if (strcmp(memlogname, "-")) {
      memlog = fopen(memlogname, "w");
      if (!memlog)
        chpl_error(chpl_glom_strings(3, "Unable to open \"", memlogname, "\""), 0, 0);
    } else
      memlog = stdout;
  }
}


static void increaseMemStat(size_t chunk, int32_t lineno, chpl_string filename) {
  chpl_mutex_lock(&memstat_lock);
  totalMem += chunk;
  if (memmaxValue && (totalMem > memmaxValue)) {
    chpl_mutex_unlock(&memstat_lock);
    chpl_error("Exceeded memory limit", lineno, filename);
  }
  totalTrackedMem += chunk;
  updateMaxMem();
  chpl_mutex_unlock(&memstat_lock);
}


static void decreaseMemStat(size_t chunk) {
  chpl_mutex_lock(&memstat_lock);
  totalMem = chunk > totalMem ? 0 : totalMem - chunk;
  totalTrackedMem = chunk > totalTrackedMem ? 0 : totalTrackedMem - chunk;
  chpl_mutex_unlock(&memstat_lock);
}


void resetMemStat(void) {
  totalMem = 0;
  totalTrackedMem = 0;
  maxMem = 0;
}


void startTrackingMem(void) {
    memfinalstat = memfinalstatSet;
    memstat = memstatSet;
    memtrack = memtrackSet;
    memtrace = memtraceSet;
}


void stopTrackingMem(void) {
    memfinalstat = false;
    memstat = false;
    memtrack = false;
    memtrace = false;
}


uint64_t mem_used(int32_t lineno, chpl_string filename) {
  alreadyPrintingStat = true; /* hack: don't want to print final stats */
  if (!memstat)
    chpl_error(chpl_glom_strings(2, __func__, "() only works with the --memstat flag"),
               lineno, filename);
  return (uint64_t)totalMem;
}


void printMemStat(int32_t lineno, chpl_string filename) {
  if (memstat) {
    chpl_mutex_lock(&memstat_lock);
    printf("totalMem=%zu, maxMem=%zu\n", totalTrackedMem, maxMem);
    alreadyPrintingStat = true;
    chpl_mutex_unlock(&memstat_lock);
  } else
    chpl_error(chpl_glom_strings(2, __func__, "() only works with the --memstat flag"),
               lineno, filename);
}


void printFinalMemStat(int32_t lineno, chpl_string filename) {
  if (!alreadyPrintingStat && memstat) {
    printf("Final Memory Statistics:  ");
    printMemStat(lineno, filename);
  }
}


static int str80cmp(const void* str1, const void* str2) {
  return strcmp(*(str80_t*)str1, *(str80_t*)str2);
}


void printMemTable(int64_t threshold, int32_t lineno, chpl_string filename) {
  memTableEntry* memEntry = NULL;

  const int numberWidth   = 9;
  const int precision     = sizeof(uintptr_t) * 2;
  const int addressWidth  = precision + 4;
  const int descWidth     = 80-3*numberWidth-addressWidth;

  const char* size        = "Size:";
  const char* bytes       = "(bytes)";
  const char* number      = "Number:";
  const char* total       = "Total:";
  const char* address     = "Address:";
  const char* description = "Description:";
  const char* line40      = "========================================";

  int n, i;
  str80_t* table;

  if (!memtrack)
    chpl_error("The printMemTable function only works with the --memtrack flag", lineno, filename);

  printf("\n%s%s\n", line40, line40);

  printf("----------------------\n");
  printf("***Allocated Memory***\n");
  printf("----------------------\n");

  printf("%-*s%-*s%-*s%-*s%-*s\n",
         numberWidth, size,
         numberWidth, number,
         numberWidth, total,
         descWidth, description,
         addressWidth, address);

  printf("%-*s%-*s%-*s\n",
         numberWidth, bytes,
         numberWidth, "",
         numberWidth, bytes);

  printf("%s%s\n", line40, line40);

  n = 0;
  for (i = 0; i < HASHSIZE; i++) {
    for (memEntry = memTable[i]; memEntry != NULL; memEntry = memEntry->nextInBucket) {
      size_t chunk = memEntry->number * memEntry->size;
      if (chunk >= threshold) {
        n += 1;
      }
    }
  }
  table = (str80_t*)malloc(n*sizeof(str80_t));
  if (!table)
    chpl_error("out of memory printing memory table", lineno, filename);

  n = 0;
  for (i = 0; i < HASHSIZE; i++) {
    for (memEntry = memTable[i]; memEntry != NULL; memEntry = memEntry->nextInBucket) {
      size_t chunk = memEntry->number * memEntry->size;
      if (chunk >= threshold) {
        if (strlen(memEntry->description) > descWidth)
          memEntry->description[descWidth-1] = '\0';
        sprintf(table[n++],
                "%-*zu%-*zu%-*zu%-*s%#-*.*" PRIxPTR,
                numberWidth, memEntry->size,
                numberWidth, memEntry->number,
                numberWidth, chunk,
                descWidth, memEntry->description,
                addressWidth, precision, (uintptr_t)memEntry->memAlloc);
      }
    }
  }
  qsort(table, n, sizeof(str80_t), str80cmp);

  for (i = 0; i < n; i++) {
    printf("%s\n", table[i]);
  }
  putchar('\n');
}


void chpl_printMemTable(void) {
  memTableEntry* memEntry = NULL;
  int i;

  const int numberWidth   = 11;

  const char* size        = "Total Size";
  const char* bytes       = "(bytes) ";
  const char* number      = "Number ";
  const char* items       = "of items";
  const char* description = "Description";
  const char* line40      = "========================================";

  _Bool printHeadings     = false;

  stopTrackingMem();


  for (i = 0; i < HASHSIZE; i++) {
    for (memEntry = memTable[i]; memEntry != NULL; memEntry = memEntry->nextInBucket) {
      size_t chunk = memEntry->number * memEntry->size;
      if (chunk >= memthresholdValue) {
#ifndef LAUNCHER
        chpl____mem_alloc_add(memEntry->description, chunk);
#endif
        printHeadings = true;
      }
    }
  }
  if (printHeadings) {
    char format[64];
    printf("\n%s%s\n", line40, line40);
    printf("---------------------\n");
    printf("*** Leaked Memory ***\n");
    printf("---------------------\n");
    printf("%*s  %*s  %s\n",
           numberWidth, number,
           numberWidth, size,
           description);
    printf("%*s  %*s\n",
           numberWidth, items,
           numberWidth, bytes);
    printf("%s%s\n", line40, line40);
    snprintf(format, sizeof(format), "%%%d%s", numberWidth, PRIu64);
#ifndef LAUNCHER
    chpl____mem_alloc_print(format);
#endif
    putchar('\n');
  }
}


static memTableEntry* lookupMemory(void* memAlloc) {
  memTableEntry* found = NULL;
  PRINTF("");
  chpl_mutex_lock(&memtrack_lock);

  found = lookupMemoryNoLock(memAlloc);

  chpl_mutex_unlock(&memtrack_lock);
  PRINTF(" done");
  return found;
}


static void installMemory(void* memAlloc, size_t number, size_t size, const char* description) {
  unsigned hashValue;
  memTableEntry* memEntry;
  PRINTF("");
  chpl_mutex_lock(&memtrack_lock);
  memEntry = lookupMemoryNoLock(memAlloc);

  if (!memEntry) {
    memEntry = (memTableEntry*) calloc(1, sizeof(memTableEntry));
    if (!memEntry) {
      char* message = chpl_glom_strings(3, "Out of memory allocating table entry for \"",
                                        description, "\"");
      chpl_error(message, 0, 0);
    }

    hashValue = hash(memAlloc);
    memEntry->nextInBucket = memTable[hashValue];
    memTable[hashValue] = memEntry;

    memEntry->description = (char*) malloc((strlen(description) + 1));
    if (!memEntry->description) {
      char* message = chpl_glom_strings(3, "Out of memory allocating table entry for \"",
                                        description, "\"");
      chpl_error(message, 0, 0);
    }
    strcpy(memEntry->description, description);
    memEntry->memAlloc = memAlloc;
  }
  memEntry->number = number;
  memEntry->size = size;
  chpl_mutex_unlock(&memtrack_lock);
  PRINTF(" done");
}


static void updateMemory(memTableEntry* memEntry, void* oldAddress, void* newAddress,
                         size_t number, size_t size) {
  unsigned newHashValue;
  PRINTF("");
  chpl_mutex_lock(&memtrack_lock);
  newHashValue = hash(newAddress);

  /* Rehash on the new memory location.  */
  removeBucketEntry(oldAddress);
  memEntry->nextInBucket = memTable[newHashValue];
  memTable[newHashValue] = memEntry;

  memEntry->memAlloc = newAddress;
  memEntry->number = number;
  memEntry->size = size;
  chpl_mutex_unlock(&memtrack_lock);
  PRINTF(" done");
}

static void removeMemory(void* memAlloc, int32_t lineno, chpl_string filename) {
  memTableEntry* thisBucketEntry;
  memTableEntry* memEntry;
  PRINTF("");
  chpl_mutex_lock(&memtrack_lock);
  memEntry = lookupMemoryNoLock(memAlloc);

  if (memEntry) {
    /* Remove the entry from the bucket list. */
    thisBucketEntry = removeBucketEntry(memAlloc);
    free(thisBucketEntry->description);
    free(thisBucketEntry);
  } else {
    chpl_error("Attempting to free memory that wasn't allocated", lineno, filename);
  }
  PRINTF(" done");
  chpl_mutex_unlock(&memtrack_lock);
}


static void printToMemLog(const char* memType, size_t number, size_t size,
                          const char* description,
                          int32_t lineno, chpl_string filename,
                          void* memAlloc, void* moreMemAlloc) {
#ifndef LAUNCHER
  size_t chunk = number * size;
  chpl_mutex_lock(&memtrace_lock);
  if (chunk >= memthresholdValue) {
    if (moreMemAlloc && (moreMemAlloc != memAlloc)) {
      fprintf(memlog, "%s called at %s:%"PRId32" on locale %"PRId32" for %zu items of size %zu for %s:  0x%p -> 0x%p\n",
              memType, filename, lineno, chpl_localeID, number, size, description,
              memAlloc, moreMemAlloc);
    } else {
      fprintf(memlog, "%s called at %s:%"PRId32" on locale %"PRId32" for %zu items of size %zu for %s:  %p\n",
              memType, filename, lineno, chpl_localeID, number, size, description, memAlloc);
    }
  }
  chpl_mutex_unlock(&memtrace_lock);
#endif
}


void
chpl_startMemDiagnosis() {
  memDiagnosisFlag = 1;
}


void
chpl_stopMemDiagnosis() {
  memDiagnosisFlag = 0;
}


void chpl_track_malloc(void* memAlloc, size_t chunk, size_t number, size_t size, const char* description, int32_t lineno, chpl_string filename) {
  if (memtrace && (!MEM_DIAGNOSIS || memDiagnosisFlag)) {
    printToMemLog("malloc", number, size, description,
                  lineno, filename, memAlloc, NULL);
  }
  if (memtrack) {
    installMemory(memAlloc, number, size, description);
    if (memstat) {
      increaseMemStat(chunk, lineno, filename);
    }
  }
}


void chpl_track_free(void* memAlloc, int32_t lineno, chpl_string filename) {
  if (memtrace) {
    if (memtrack) {
      memTableEntry* memEntry;
      memEntry = lookupMemory(memAlloc);
      if (!MEM_DIAGNOSIS || memDiagnosisFlag)
        printToMemLog("free", memEntry->number, memEntry->size,
                      memEntry->description, lineno, filename,
                      memAlloc, NULL);
    } else if (!MEM_DIAGNOSIS || memDiagnosisFlag)
      printToMemLog("free", 0, 0, "", lineno, filename, memAlloc, NULL);
  }

  if (memtrack) {
    if (memstat) {
      memTableEntry* memEntry = lookupMemory(memAlloc);
      if (memEntry)
        decreaseMemStat(memEntry->number * memEntry->size);
    }
    removeMemory(memAlloc, lineno, filename);
  }
}


void* chpl_track_realloc1(void* memAlloc, size_t number, size_t size, const char* description, int32_t lineno, chpl_string filename) {
  memTableEntry* memEntry = 0;
  if (memtrack && memAlloc != NULL) {
    memEntry = lookupMemory(memAlloc);
    if (!memEntry)
      chpl_error(chpl_glom_strings(3, "Attempting to realloc memory for ",
                                   description, " that wasn't allocated"),
                 lineno, filename);
  }
  return memEntry;
}


void chpl_track_realloc2(void* memEntryV, void* moreMemAlloc, size_t newChunk, void* memAlloc, size_t number, size_t size, const char* description, int32_t lineno, chpl_string filename) {
  memTableEntry* memEntry = (memTableEntry*)memEntryV;
  if (memtrack) { 
    if (memAlloc != NULL) {
      if (memEntry) {
        if (memstat)
          decreaseMemStat(memEntry->number * memEntry->size);
        updateMemory(memEntry, memAlloc, moreMemAlloc, number, size);
      }
    } else
      installMemory(moreMemAlloc, number, size, description);
    if (memstat)
      increaseMemStat(newChunk, lineno, filename);
  }
  if (memtrace && (!MEM_DIAGNOSIS || memDiagnosisFlag)) {
    printToMemLog("realloc", number, size, description, lineno, filename,
                  memAlloc, moreMemAlloc);
  }
}
