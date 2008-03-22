#ifndef _chplmem_H_
#define _chplmem_H_

#include <stddef.h>
#include <stdint.h>
#include "arg.h"
#include "chpltypes.h"
#include "chplthreads.h"

typedef struct {
  char* head;
  char* current;
  char* tail;
} _chpl_meminfo_t;

extern _chpl_meminfo_t chpl_meminfo;
extern int broadcastingGlobalsStarted;
extern int whichMalloc;

extern chpl_mutex_t _memtrack_lock;
extern chpl_mutex_t _memstat_lock;
extern chpl_mutex_t _memtrace_lock;
extern chpl_mutex_t _malloc_lock;

void initMemTable(void);
void printMemTable(int64_t threshold, int32_t lineno, _string filename);
void resetMemStat(void);
void startTrackingMem(void);
void printMemStat(int32_t lineno, _string filename);
void printFinalMemStat(int32_t lineno, _string filename);

void setMemmax(int64_t value);
void setMemstat(void);
void setMemtrack(void);
void setMemthreshold(int64_t value);
void setMemtrace(char* memLogname);

#define _chpl_alloc(size, description, lineno, filename) \
  _chpl_malloc(1, size, description, lineno, filename)
void* _chpl_malloc(size_t number, size_t size, const char* description,
                   int32_t lineno, _string filename);
void* _chpl_calloc(size_t number, size_t size, const char* description,
                   int32_t lineno, _string filename);
void* _chpl_realloc(void* ptr, size_t number, size_t size, 
                    const char* description, int32_t lineno, _string filename);
void  _chpl_free(void* ptr, int32_t lineno, _string filename);

uint64_t _mem_used(int32_t lineno, _string filename);

#endif
