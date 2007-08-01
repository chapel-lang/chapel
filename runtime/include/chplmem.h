#ifndef _chplmem_H_
#define _chplmem_H_

#include <stddef.h>
#include "arg.h"
#include "chpltypes.h"
#include "chplthreads.h"

extern _chpl_mutex_t _memtrack_lock;
extern _chpl_mutex_t _memstat_lock;
extern _chpl_mutex_t _memtrace_lock;

void initMemTable(void);
void printMemTable(_int64 threshold, _int32 lineno, _string filename);
void resetMemStat(void);
void startTrackingMem(void);
void printMemStat(_int32 lineno, _string filename);
void printFinalMemStat(_int32 lineno, _string filename);

void setMemmax(_int64 value);
void setMemstat(void);
void setMemtrack(void);
void setMemthreshold(_int64 value);
void setMemtrace(char* memLogname);

#define _chpl_alloc(size, description, lineno, filename) \
  _chpl_malloc(1, size, description, lineno, filename)
void* _chpl_malloc(size_t number, size_t size, const char* description,
                   _int32 lineno, _string filename);
void* _chpl_calloc(size_t number, size_t size, const char* description,
                   _int32 lineno, _string filename);
void* _chpl_realloc(void* ptr, size_t number, size_t size, 
                    const char* description, _int32 lineno, _string filename);
void  _chpl_free(void* ptr, _int32 lineno, _string filename);

_uint64 _mem_used(_int32 lineno, _string filename);

#endif
