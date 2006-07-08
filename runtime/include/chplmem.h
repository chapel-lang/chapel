#ifndef _chplmem_H_
#define _chplmem_H_

#include <stddef.h>
#include "arg.h"
#include "chpltypes.h"


void initMemTable(void);
void printMemTable(_int64 threshold);
void resetMemStat(void);
void startTrackingMem(void);
void printMemStat(void);
void printFinalMemStat(void);

void setMemmax(_int64 value);
void setMemstat(void);
void setMemtrack(void);
void setMemthreshold(_int64 value);
void setMemtrace(char* memLogname);

#define _chpl_alloc( size, description) _chpl_malloc( 1, size, description)
void* _chpl_malloc( size_t number, size_t size, char* description);
void* _chpl_calloc( size_t number, size_t size, char* description);
void* _chpl_realloc( void* ptr, size_t number, size_t size, char* description);
void  _chpl_free( void* ptr);

#endif
