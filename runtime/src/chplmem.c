#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "chplmem.h"
#include "chplrt.h"
#include "chpltypes.h"
#include "error.h"

#undef malloc
#undef calloc
#undef free
#undef realloc

#define HASHSIZE 1019

typedef struct _memTableEntry { /* table entry */
  size_t number;
  size_t size;
  char* description;
  void* memAlloc;

  struct _memTableEntry* nextInBucket;
  struct _memTableEntry* prevInstalled;
  struct _memTableEntry* nextInstalled;

} memTableEntry;


/* hash table */
static memTableEntry* memTable[HASHSIZE];
static memTableEntry* first = NULL;
static memTableEntry* last = NULL;


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


static int memstat = 0;
static int memtrack = 0;
static int memthreshold = 0;
static _integer64 memmaxValue = 0;
static _integer64 memthresholdValue = 0;
static FILE* memlog = NULL;
static size_t totalMem = 0;  /* total memory currently allocated */
static size_t maxMem = 0;    /* maximum total memory during run  */


void initMemTable(void) {
  if (memtrack) {
    int i;
    for (i = 0; i < HASHSIZE; i++) {
      memTable[i] = NULL;
    }
  }
}


void setMemmax(_integer64 value) {
  memmaxValue = value;
  setMemstat();
}


void setMemstat(void) {
  memstat = 1;
  memtrack = 1;
}


void setMemtrack(void) {
  memtrack = 1;
}


void setMemthreshold(_integer64 value) {
  if (!memlog) {
    char* message = "--memthreshold useless when used without --memtrace";
    printError(message);
  }
  memthreshold = 1;
  memthresholdValue = value;
}


void setMemtrace(char* memlogname) {
  if (memlogname) {
    memlog = fopen(memlogname, "w");
    if (!memlog) {
      char* message = _glom_strings(3, "Unable to open \"", memlogname, 
                                    "\"");
      printError(message);
    }
  } 
}


static void updateMaxMem(void) {
  if (totalMem > maxMem) {
    maxMem = totalMem;
  }
}


static void increaseMemStat(size_t chunk) {
  totalMem += chunk;
  if (memmaxValue && (totalMem > memmaxValue)) {
      char* message = "Exceeded memory limit";
      printError(message);
    }
  updateMaxMem();
}


static void decreaseMemStat(size_t chunk) {
  totalMem -= chunk;
  updateMaxMem();
}


void resetMemStat(void) {
  totalMem = 0;
  maxMem = 0;
}

static int alreadyPrintingStat = 0;

void printMemStat(void) {
  if (memstat) {
    fprintf(stdout, "totalMem=%u, maxMem=%u\n", 
            (unsigned)totalMem, (unsigned)maxMem);
    alreadyPrintingStat = 1;
  } else {
    char* message = "printMemStat() only works with the --memstat flag";
    printError(message);
  }
}


void printFinalMemStat(void) {
  if (!alreadyPrintingStat && memstat) {
    fprintf(stdout, "Final Memory Statistics:  ");
    printMemStat();
  }
}


void printMemTable(void) {
  if (!memtrack) {
    char* message = "printMemTable() only works with the --memtrack flag";
    printError(message);
  }
  memTableEntry* memEntry = NULL;
  fprintf(stdout, "\n");

  int numberWidth   = 9;
  int addressWidth  = 12;
  int precision     = 8;

  char* size        = "Size:";
  char* bytes       = "(bytes)"; 
  char* number      = "Number:";
  char* total       = "Total:";
  char* address     = "Address:";
  char* description = "Description:";
  char* line40      = "========================================";   

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
    fprintf(stdout, "%-*u%-*u%-*u%#-*.*x%-s\n", 
            numberWidth, (unsigned)memEntry->size, 
            numberWidth, (unsigned)memEntry->number, 
            numberWidth, (unsigned)chunk, 
            addressWidth, precision, (unsigned)(intptr_t)memEntry->memAlloc, 
            memEntry->description);
  }
  fprintf(stdout, "\n");
}


static memTableEntry* lookupMemory(void* memAlloc) {
  memTableEntry* memEntry = NULL;
  unsigned hashValue = hash(memAlloc);

  for (memEntry = memTable[hashValue]; 
       memEntry != NULL; 
       memEntry = memEntry->nextInBucket) {

    if (memEntry->memAlloc == memAlloc) {
      return memEntry;
    }
  }
  return NULL;
}


static void installMemory(void* memAlloc, size_t number, size_t size, 
                   char* description) {
  unsigned hashValue;
  memTableEntry* memEntry = lookupMemory(memAlloc);

  if (!memEntry) { 
    memEntry = (memTableEntry*) calloc(1, sizeof(memTableEntry));
    if (!memEntry) {
      char* message = _glom_strings(3, "Out of memory allocating table entry "
                                    "for \"", description, "\"");
      printError(message);
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
      printError(message);
    }
    strcpy(memEntry->description, description);
    memEntry->memAlloc = memAlloc;
  }  
  memEntry->number = number;
  memEntry->size = size;
}


static void updateMemory(memTableEntry* memEntry, void* oldAddress, 
                         void* newAddress, size_t number, size_t size) {
  unsigned oldHashValue = hash(oldAddress);
  unsigned newHashValue = hash(newAddress);

  /* Rehash on the new memory location.  */
  memTable[oldHashValue] = memEntry->nextInBucket;
  memEntry->nextInBucket = memTable[newHashValue];
  memTable[newHashValue] = memEntry;

  memEntry->memAlloc = newAddress;
  memEntry->number = number;
  memEntry->size = size;
}


static void removeMemory(void* memAlloc) {
  memTableEntry* memEntry = lookupMemory(memAlloc);

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
    unsigned hashValue = hash(memAlloc);    
    memTableEntry* thisBucketEntry = memTable[hashValue];
    
    if (thisBucketEntry->memAlloc == memAlloc) {
      memTable[hashValue] = thisBucketEntry->nextInBucket;
    } else {
      for (thisBucketEntry = memTable[hashValue]; 
           thisBucketEntry != NULL; 
           thisBucketEntry = thisBucketEntry->nextInBucket) {

        memTableEntry* nextBucketEntry = thisBucketEntry->nextInBucket;
        
        if (nextBucketEntry->memAlloc == memAlloc) {
          thisBucketEntry->nextInBucket = nextBucketEntry->nextInBucket;
          thisBucketEntry = nextBucketEntry;
          break;
        }
      }
    }
    free(thisBucketEntry->description);
    free(thisBucketEntry);
  } else {
    char* message = "Attempting to free memory that wasn't allocated";
    printError(message);
  }
}


static void confirm(void* memAlloc, char* description) {
  if (!memAlloc) {
    char message[1024];
    sprintf(message, "Out of memory allocating \"%s\"", description);
    printError(message);
  }
}


static void printToMemLog(size_t number, size_t size, char* description, 
                          char* memType, void* memAlloc, void* moreMemAlloc) {
  size_t chunk = number * size;
  
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
}


void* _chpl_malloc(size_t number, size_t size, char* description) {
  size_t chunk = number * size;
  void* memAlloc = malloc(chunk);
  confirm(memAlloc, description);

  if (memlog) {
    printToMemLog(number, size, description, "malloc", memAlloc, NULL);
  }
  if (memtrack) {
    installMemory(memAlloc, number, size, description);  
    if (memstat) {
      increaseMemStat(chunk);
    }
  }
  return memAlloc;
}


void* _chpl_calloc(size_t number, size_t size, char* description) {
  void* memAlloc = calloc(number, size);
  confirm(memAlloc, description);

  if (memlog) {
    printToMemLog(number, size, description, "calloc", memAlloc, NULL);
  }

  if (memtrack) {
    installMemory(memAlloc, number, size, description);
    if (memstat) {
      size_t chunk = number * size;
      increaseMemStat(chunk);
    }
  }
  return memAlloc;
}


void _chpl_free(void* memAlloc) {
  if (memtrack) {
    if (memstat) {
      memTableEntry* memEntry = lookupMemory(memAlloc);
      size_t chunk;
      if (memEntry) {
        chunk = memEntry->number * memEntry->size;
        decreaseMemStat(chunk);
      }
    }
    removeMemory(memAlloc);
  }
  free(memAlloc);
}


void* _chpl_realloc(void* memAlloc, size_t number, size_t size, 
                    char* description) {
  size_t newChunk = number * size;
  if (!newChunk) {
    _chpl_free(memAlloc);
    return NULL;
  }
  memTableEntry* memEntry;
  if (memtrack) {
    memEntry = lookupMemory(memAlloc);
    if (!memEntry && (memAlloc != NULL)) {
      char* message = _glom_strings(3, "Attempting to realloc memory for ",
                                    description, "that wasn't allocated");
      printError(message);
    }
  }
  void* moreMemAlloc = realloc(memAlloc, newChunk);
  confirm(moreMemAlloc, description);

  if (memtrack) { 
    if (memAlloc != NULL) {
      if (memEntry) {
        if (memstat) {
          size_t oldChunk = memEntry->number * memEntry->size;
          decreaseMemStat(oldChunk);
        }
        updateMemory(memEntry, memAlloc, moreMemAlloc, number, size);
      }
    } else {
      installMemory(moreMemAlloc, number, size, description);
    }
    if (memstat) {
      increaseMemStat(newChunk);
    }
  }
  if (memlog) {
    printToMemLog(number, size, description, "realloc", memAlloc, 
                  moreMemAlloc);
  }
  return moreMemAlloc;
}
