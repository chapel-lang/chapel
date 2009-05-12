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
  chpl_bool userCode;
  void* memAlloc;

  struct _memTableEntry* nextInBucket;
  struct _memTableEntry* prevInstalled;
  struct _memTableEntry* nextInstalled;

} memTableEntry;

void decreaseMemStat(size_t chunk, chpl_bool userCode);
void increaseMemStat(size_t chunk, chpl_bool userCode,
                     int32_t lineno, chpl_string filename);
void initMemTable(void);
void installMemory(void* memAlloc, size_t number, size_t size,
                   const char* description, chpl_bool userCode);
memTableEntry* lookupMemory(void* memAlloc);
uint64_t mem_used(int32_t lineno, chpl_string filename);
void printFinalMemStat(int32_t lineno, chpl_string filename);
void printMemStat(int32_t lineno, chpl_string filename);
void printMemTable(int64_t threshold, chpl_bool userCode,
                   int32_t lineno, chpl_string filename);
void chpl_printMemTable(void);
void printToMemLog(const char* memType, size_t number, size_t size,
                   const char* description, chpl_bool userCode,
                   int32_t lineno, chpl_string filename,
                   void* memAlloc, void* moreMemAlloc);
void removeMemory(void* memAlloc, int32_t lineno, chpl_string filename);
void resetMemStat(void);
void setMemmax(int64_t value);
void setMemstat(void);
void setMemfinalstat(void);
void setMemthreshold(int64_t value);
void setMemtrace(char* memlogname);
void setMemtrack(void);
void startTrackingMem(void);
void  stopTrackingMem(void);
void updateMemory(memTableEntry* memEntry, void* oldAddress,
                  void* newAddress, size_t number, size_t size);

#endif
