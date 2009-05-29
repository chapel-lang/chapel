#ifndef _chplmemtrack_H_
#define _chplmemtrack_H_

#include <stdio.h>
#include <stdlib.h>

#define HASHSIZE 1019

extern _Bool memfinalstat;
extern _Bool memstat;
extern _Bool memtrace;
extern _Bool memtrack;

void initMemTable(void);
uint64_t mem_used(int32_t lineno, chpl_string filename);
void printFinalMemStat(int32_t lineno, chpl_string filename);
void printMemStat(int32_t lineno, chpl_string filename);
void printMemTable(int64_t threshold, chpl_bool aggregated, int32_t lineno, chpl_string filename);
void chpl_printMemTable(void);
void resetMemStat(void);
void setMemmax(int64_t value);
void setMemstat(void);
void setMemfinalstat(void);
void setMemthreshold(int64_t value);
void setMemtrace(char* memlogname);
void setMemtrack(void);
void startTrackingMem(void);
void  stopTrackingMem(void);

void chpl_startMemDiagnosis(void);
void chpl_stopMemDiagnosis(void);
void chpl_track_malloc(void* memAlloc, size_t chunk, size_t number, size_t size, const char* description, int32_t lineno, chpl_string filename);
void chpl_track_free(void* memAlloc, int32_t lineno, chpl_string filename);
void* chpl_track_realloc1(void* memAlloc, size_t number, size_t size, const char* description, int32_t lineno, chpl_string filename);
void chpl_track_realloc2(void* memEntry, void* moreMemAlloc, size_t newChunk, void* memAlloc, size_t number, size_t size, const char* description, int32_t lineno, chpl_string filename);

#endif
