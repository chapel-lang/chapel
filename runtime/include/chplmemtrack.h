#ifndef _chplmemtrack_H_
#define _chplmemtrack_H_

#ifndef LAUNCHER

#include "chpltypes.h"  // for chpl_string
#include "chpl-mem-desc.h"

#include <stdio.h>
#include <stdlib.h>


// Memory tracking activated?
extern chpl_bool chpl_memTrack;

void chpl_setMemFlags(void);
uint64_t chpl_memoryUsed(int32_t lineno, chpl_string filename);
void chpl_printMemStat(int32_t lineno, chpl_string filename);
void chpl_printMemTable(int64_t threshold, int32_t lineno, chpl_string filename);
void chpl_reportMemInfo(void);
void chpl_track_malloc(void* memAlloc, size_t number, size_t size,
                       chpl_mem_descInt_t description,
                       int32_t lineno, chpl_string filename);
void chpl_track_free(void* memAlloc, int32_t lineno, chpl_string filename);
void chpl_track_realloc_pre(void* memAlloc, size_t size,
                         chpl_mem_descInt_t description,
                         int32_t lineno, chpl_string filename);
void chpl_track_realloc_post(void* moreMemAlloc,
                         void* memAlloc, size_t size,
                         chpl_mem_descInt_t description,
                         int32_t lineno, chpl_string filename);

void chpl_startVerboseMem(void);
void chpl_stopVerboseMem(void);
void chpl_startVerboseMemHere(void);
void chpl_stopVerboseMemHere(void);

#else // LAUNCHER

#define chpl_setMemmax(value)
#define chpl_setMemstat()
#define chpl_setMemreport()
#define chpl_setMemtrack()

#endif // LAUNCHER

#endif
