#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "chplmem.h"
#include "chplrt.h"
#include "chpltypes.h"

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

void initMemTable(void) {
  int i;
  for (i = 0; i < HASHSIZE; i++) {
    memTable[i] = NULL;
  }
}


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


void printMemTable(void) {
  memTableEntry* memEntry = NULL;
 
  fprintf(stdout, "================\n"); 
  fprintf(stdout, "Allocated Memory\n");
  fprintf(stdout, "================\n");
  fprintf(stdout, "Size (in bytes):\tType:\n");
  fprintf(stdout, "----------------\t-----\n");

  for (memEntry = first;
       memEntry != NULL;
       memEntry = memEntry->nextInstalled) {

    size_t chunk = memEntry->number * memEntry->size;
    fprintf(stdout, "%u\t\t\t%s\n", chunk, memEntry->description);
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


void installMemory(void* memAlloc, size_t number, size_t size, 
		  char* description) {
  unsigned hashValue;
  memTableEntry* memEntry = lookupMemory(memAlloc);

  if (!memEntry) { 
    memEntry = (memTableEntry*) calloc(1, sizeof(memTableEntry));
    if (!memEntry) {
      fprintf(stderr, "***Error:  Out of memory allocating table entry for %s"
	      "***\n", description);
      exit(0);
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
    strcpy(memEntry->description, description);
    memEntry->memAlloc = memAlloc;
  }  
  memEntry->number = number;
  memEntry->size = size;
}


void removeMemory(void* memAlloc) {
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
	// there is no next, which means this one is last.  must reset last.
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
    fprintf(stderr, "***Error:  Attempting to free memory that wasn't "
	    "allocated***\n");
    exit(0);
  }
}


static void confirm(void* memAlloc, char* description) {
  if (!memAlloc) {
    fprintf(stderr, "***Error:  Out of memory allocating %s***\n", 
	    description);
    exit(0);
  }
}


void* _chpl_malloc(size_t number, size_t size, char* description) {
  size_t chunk = number * size;
  void* memAlloc = malloc(chunk);
  confirm(memAlloc, description);
  installMemory(memAlloc, number, size, description);
  return memAlloc;
}


/*
static void memzero(void* memory, size_t chunk) {
  size_t i;
  for (i = 0; i < chunk; i++) {
    *(char*)(memory + i) = 0;
  }
}
*/


void* _chpl_calloc(size_t number, size_t size, char* description) {
  void* memAlloc = calloc(number, size);
  confirm(memAlloc, description);
  installMemory(memAlloc, number, size, description);
  return memAlloc;
}


void _chpl_free(void* memAlloc) {
  removeMemory(memAlloc);
  free(memAlloc);
}


void* _chpl_realloc(void* memAlloc, size_t number, size_t size, 
		    char* description) {
  size_t chunk = number * size;
  if (!chunk) {
    _chpl_free(memAlloc);
    return NULL;
  }

  memTableEntry* memEntry = lookupMemory(memAlloc);
  if (!memEntry && (memAlloc != NULL)) {
    fprintf(stderr, "***Error:  Attempting to realloc memory for %s that "
	    "wasn't allocated***\n", description);
    exit(0);
  }
  void* moreMemAlloc = realloc(memAlloc, chunk);
  confirm(moreMemAlloc, description);
  if ((memAlloc != NULL)  && (moreMemAlloc != memAlloc)) {
    removeMemory(memAlloc);
  }
  installMemory(moreMemAlloc, number, size, description);
  return moreMemAlloc;
}
