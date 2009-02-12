#ifndef _chplmemtrack_H_
#define _chplmemtrack_H_

#include <stdio.h>
#include <stdlib.h>

#define HASHSIZE 1019

extern _Bool memfinalstat;
extern _Bool memstat;
extern _Bool memtrace;
extern _Bool memtrack;

typedef struct _memTableEntry { /* table entry */
  size_t number;
  size_t size;
  char* description;
  void* memAlloc;

  struct _memTableEntry* nextInBucket;
  struct _memTableEntry* prevInstalled;
  struct _memTableEntry* nextInstalled;

} memTableEntry;

void decreaseMemStat(size_t chunk);
void increaseMemStat(size_t chunk, int32_t lineno, _string filename);
void initMemTable(void);
void installMemory(void* memAlloc, size_t number, size_t size,
                   const char* description);
memTableEntry* lookupMemory(void* memAlloc);
uint64_t mem_used(int32_t lineno, _string filename);
void printFinalMemStat(int32_t lineno, _string filename);
void printMemStat(int32_t lineno, _string filename);
void printMemTable(int64_t threshold, int32_t lineno, _string filename);
// calls printMemTable if --memfinalstat was specified on the command line;
// otherwise does nothing
void chpl_printMemTable(int64_t threshold, int32_t lineno, _string filename);
void printToMemLog(size_t number, size_t size, const char* description,
                   const char* memType, void* memAlloc, void* moreMemAlloc);
void removeMemory(void* memAlloc, int32_t lineno, _string filename);
void resetMemStat(void);
void setMemmax(int64_t value);
void setMemstat(void);
void setMemfinalstat(void);
void setMemthreshold(int64_t value);
void setMemtrace(char* memlogname);
void setMemtrack(void);
void startTrackingMem(void);
void updateMemory(memTableEntry* memEntry, void* oldAddress,
                  void* newAddress, size_t number, size_t size);

#endif
