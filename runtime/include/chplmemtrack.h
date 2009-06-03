#ifndef _chplmemtrack_H_
#define _chplmemtrack_H_

#ifndef LAUNCHER

#include <stdio.h>
#include <stdlib.h>
#include "chpl_mem.h"

#define HASHSIZE 1019

void chpl_initMemTable(void);
uint64_t chpl_memoryUsed(int32_t lineno, chpl_string filename);
void chpl_printMemStat(int32_t lineno, chpl_string filename);
void chpl_printMemTable(int64_t threshold, int32_t lineno, chpl_string filename);
void chpl_reportMemInfo(void);
void chpl_setMemmax(int64_t value);
void chpl_setMemstat(void);
void chpl_setMemreport(void);
void chpl_setMemthreshold(int64_t value);
void chpl_setMemtrace(char* memlogname);
void chpl_setMemtrack(void);
void chpl_startTrackingMem(void);

void chpl_startMemDiagnosis(void);
void chpl_stopMemDiagnosis(void);
void chpl_track_malloc(void* memAlloc, size_t chunk, size_t number, size_t size, chpl_memDescInt_t description, int32_t lineno, chpl_string filename);
void chpl_track_free(void* memAlloc, int32_t lineno, chpl_string filename);
void chpl_track_realloc1(void* memAlloc, size_t number, size_t size, chpl_memDescInt_t description, int32_t lineno, chpl_string filename);
void chpl_track_realloc2(void* moreMemAlloc, size_t newChunk, void* memAlloc, size_t number, size_t size, chpl_memDescInt_t description, int32_t lineno, chpl_string filename);

#else // LAUNCHER

#define chpl_setMemmax(value)
#define chpl_setMemstat()
#define chpl_setMemreport()
#define chpl_setMemthreshold(value)
#define chpl_setMemtrace(memlogname)
#define chpl_setMemtrack()

#endif // LAUNCHER

#endif
