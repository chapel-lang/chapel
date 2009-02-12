#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "chplrt.h"
#include "chplmemtrack.h"
#include "chplthreads.h"
#include "error.h"

#undef malloc
#undef calloc
#undef free

#define CHPL_DEBUG_MEMTRACK 0

#if CHPL_DEBUG_MEMTRACK
#define PRINTF(_s) {                  \
  printf("%s\n", _s); fflush(stdout); \
}
#else
#define PRINTF(_s)
#endif


static memTableEntry* memTable[HASHSIZE];
static memTableEntry* first = NULL;
static memTableEntry* last = NULL;

_Bool memfinalstat = false;
_Bool memstat = false;
_Bool memtrace = false;
_Bool memtrack = false;
static _Bool memfinalstatSet = false;
static _Bool memstatSet = false;
static _Bool memtraceSet = false;
static _Bool memtrackSet = false;

static int64_t memmaxValue = 0;
static int64_t memthresholdValue = 0;
static FILE* memlog = NULL;
static size_t totalMem = 0;  /* total memory currently allocated */
static size_t maxMem = 0;    /* maximum total memory during run  */

static int alreadyPrintingStat = 0;

chpl_mutex_t _memtrack_lock;
chpl_mutex_t _memstat_lock;
chpl_mutex_t _memtrace_lock;

static unsigned hash(void* memAlloc);
static void updateMaxMem(void);
static memTableEntry* removeBucketEntry(void* address);
static memTableEntry* lookupMemoryNoLock(void* memAlloc);

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


static void updateMaxMem(void) {
  if (totalMem > maxMem) {
    maxMem = totalMem;
  }
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
  chpl_mutex_init(&_memtrace_lock);
  chpl_mutex_init(&_memstat_lock);
  chpl_mutex_init(&_memtrack_lock);
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
  memstatSet = true;
  memtrackSet = true;
}


void setMemtrack(void) {
  memtrackSet = true;
}


void setMemthreshold(int64_t value) {
  if (!memlog) {
    chpl_error("--memthreshold useless when used without --memtrace", 0, 0);
  }
  memthresholdValue = value;
}


void setMemtrace(char* memlogname) {
  memtraceSet = true;
  if (memlogname) {
    memlog = fopen(memlogname, "w");
    if (!memlog) {
      char* message = _glom_strings(3, "Unable to open \"", memlogname,
                                    "\"");
      chpl_error(message, 0, 0);
    }
  }
}

void increaseMemStat(size_t chunk, int32_t lineno, _string filename) {
  chpl_mutex_lock(&_memstat_lock);
  totalMem += chunk;
  if (memmaxValue && (totalMem > memmaxValue)) {
    const char* message = "Exceeded memory limit";
    chpl_mutex_unlock(&_memstat_lock);
    chpl_error(message, lineno, filename);
  }
  updateMaxMem();
  chpl_mutex_unlock(&_memstat_lock);
}


void decreaseMemStat(size_t chunk) {
  chpl_mutex_lock(&_memstat_lock);
  totalMem -= chunk;
  updateMaxMem();
  chpl_mutex_unlock(&_memstat_lock);
}


void resetMemStat(void) {
  totalMem = 0;
  maxMem = 0;
}


void startTrackingMem(void) {
    memfinalstat = memfinalstatSet;
    memstat = memstatSet;
    memtrack = memtrackSet;
    memtrace = memtraceSet;
}


uint64_t mem_used(int32_t lineno, _string filename) {
  uint64_t u;
  alreadyPrintingStat = 1; /* hack: don't want to print final stats */
  if (!memstat)
    chpl_error("memoryUsed() only works with the --memstat flag", lineno, filename);
  u = (uint64_t)totalMem;
  return u;
}


void printMemStat(int32_t lineno, _string filename) {
  if (memstat) {
    chpl_mutex_lock(&_memstat_lock);
    fprintf(stdout, "totalMem=%u, maxMem=%u\n",
            (unsigned)totalMem, (unsigned)maxMem);
    alreadyPrintingStat = 1;
    chpl_mutex_unlock(&_memstat_lock);
  } else {
    const char* message = "printMemStat() only works with the --memstat flag";
    chpl_error(message, lineno, filename);
  }
}


void printFinalMemStat(int32_t lineno, _string filename) {
  if (!alreadyPrintingStat && memstat) {
    fprintf(stdout, "Final Memory Statistics:  ");
    printMemStat(lineno, filename);
  }
}


void printMemTable(int64_t threshold, int32_t lineno, _string filename) {
  memTableEntry* memEntry = NULL;

  int numberWidth   = 9;
  int addressWidth  = 12;
  int precision     = 8;

  const char* size        = "Size:";
  const char* bytes       = "(bytes)";
  const char* number      = "Number:";
  const char* total       = "Total:";
  const char* address     = "Address:";
  const char* description = "Description:";
  const char* line40      = "========================================";

  if (!memtrack) {
    const char* message = "The printMemTable function only works with the "
      "--memtrack flag";
    chpl_error(message, lineno, filename);
  }

  fprintf(stdout, "\n");

  fprintf(stdout, "%s%s\n", line40, line40);

  fprintf(stdout, "----------------------\n");
  fprintf(stdout, "***Allocated Memory***\n");
  fprintf(stdout, "----------------------\n");

  fprintf(stdout, "%-*s%-*s%-*s%-*s%-s\n",
          numberWidth, size,
          numberWidth, number,
          numberWidth, total,
          addressWidth, address,
          description);

  fprintf(stdout, "%-*s%-*s%-*s\n",
          numberWidth, bytes,
          numberWidth, "",
          numberWidth, bytes);

  fprintf(stdout, "%s%s\n", line40, line40);

  for (memEntry = first;
       memEntry != NULL;
       memEntry = memEntry->nextInstalled) {

    size_t chunk = memEntry->number * memEntry->size;
    if (chunk > threshold) {
      fprintf(stdout, "%-*u%-*u%-*u%#-*.*x%-s\n",
              numberWidth, (unsigned)memEntry->size,
              numberWidth, (unsigned)memEntry->number,
              numberWidth, (unsigned)chunk,
              addressWidth, precision, (unsigned)(intptr_t)memEntry->memAlloc,
              memEntry->description);
    }
  }
  fprintf(stdout, "\n");
}


void chpl_printMemTable(int64_t threshold, int32_t lineno, _string filename) {
  if (memfinalstat)
    printMemTable(threshold, lineno, filename);
}


memTableEntry* lookupMemory(void* memAlloc) {
  memTableEntry* found = NULL;
  PRINTF("lookupMemory");
  chpl_mutex_lock(&_memtrack_lock);

  found = lookupMemoryNoLock(memAlloc);

  chpl_mutex_unlock(&_memtrack_lock);
  PRINTF("lookupMemory done");
  return found;
}


void installMemory(void* memAlloc, size_t number, size_t size,
                          const char* description) {
  unsigned hashValue;
  memTableEntry* memEntry;
  PRINTF("installMemory");
  chpl_mutex_lock(&_memtrack_lock);
  memEntry = lookupMemoryNoLock(memAlloc);

  if (!memEntry) {
    memEntry = (memTableEntry*) calloc(1, sizeof(memTableEntry));
    if (!memEntry) {
      char* message = _glom_strings(3, "Out of memory allocating table entry "
                                    "for \"", description, "\"");
      chpl_error(message, 0, 0);
    }

    hashValue = hash(memAlloc);
    memEntry->nextInBucket = memTable[hashValue];
    memTable[hashValue] = memEntry;

    if (first == NULL) {
      first = memEntry;
    } else {
      last->nextInstalled = memEntry;
      memEntry->prevInstalled = last;
    }
    last = memEntry;

    memEntry->description = (char*) malloc((strlen(description) + 1)
                                           * sizeof(char));
    if (!memEntry->description) {
      char* message = _glom_strings(3, "Out of memory allocating table entry "
                                    "for \"", description, "\"");
      chpl_error(message, 0, 0);
    }
    strcpy(memEntry->description, description);
    memEntry->memAlloc = memAlloc;
  }
  memEntry->number = number;
  memEntry->size = size;
  chpl_mutex_unlock(&_memtrack_lock);
  PRINTF("installMemory done");
}


void updateMemory(memTableEntry* memEntry, void* oldAddress,
                         void* newAddress, size_t number, size_t size) {
  unsigned newHashValue;
  PRINTF("updateMemory");
  chpl_mutex_lock(&_memtrack_lock);
  newHashValue = hash(newAddress);

  /* Rehash on the new memory location.  */
  removeBucketEntry(oldAddress);
  memEntry->nextInBucket = memTable[newHashValue];
  memTable[newHashValue] = memEntry;

  memEntry->memAlloc = newAddress;
  memEntry->number = number;
  memEntry->size = size;
  chpl_mutex_unlock(&_memtrack_lock);
  PRINTF("updateMemory done");
}

void removeMemory(void* memAlloc, int32_t lineno, _string filename) {
  memTableEntry* thisBucketEntry;
  memTableEntry* memEntry;
  PRINTF("removeMemory");
  chpl_mutex_lock(&_memtrack_lock);
  memEntry = lookupMemoryNoLock(memAlloc);

  if (memEntry) {
    /* Remove the entry from the first-to-last list. */
    if (memEntry == first) {
      first = memEntry->nextInstalled;
      if (memEntry->nextInstalled) {
        memEntry->nextInstalled->prevInstalled = NULL;
      }
    } else {
      memEntry->prevInstalled->nextInstalled = memEntry->nextInstalled;
      if (memEntry->nextInstalled) {
        memEntry->nextInstalled->prevInstalled = memEntry->prevInstalled;
      } else {
        last = memEntry->prevInstalled;
      }
    }

    /* Remove the entry from the bucket list. */
    thisBucketEntry = removeBucketEntry(memAlloc);
    free(thisBucketEntry->description);
    free(thisBucketEntry);
  } else {
    const char* message = "Attempting to free memory that wasn't allocated";
    chpl_error(message, lineno, filename);
  }
  PRINTF("removeMemory done");
  chpl_mutex_unlock(&_memtrack_lock);
}


void printToMemLog(size_t number, size_t size, const char* description,
                          const char* memType, void* memAlloc, void* moreMemAlloc) {
  size_t chunk = number * size;
  chpl_mutex_lock(&_memtrace_lock);
  if (chunk >= memthresholdValue) {
    if (moreMemAlloc && (moreMemAlloc != memAlloc)) {
      fprintf(memlog, "%s called for %u items of size %u"
              " for %s:  0x%x -> 0x%x\n", memType, (unsigned)number,
              (unsigned)size, description, (unsigned)(intptr_t)memAlloc,
              (unsigned)(intptr_t)moreMemAlloc);
    } else {
      fprintf(memlog, "%s called for %u items of size %u"
              " for %s:  0x%x\n", memType, (unsigned)number, (unsigned)size,
              description, (unsigned)(intptr_t)memAlloc);
    }
  }
  chpl_mutex_unlock(&_memtrace_lock);
}

