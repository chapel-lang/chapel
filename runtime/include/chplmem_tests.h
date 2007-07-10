#ifndef _chplmem_tests_H_
#define _chplmem_tests_H_

void _memtest_printMemTable(int threshold, _int32 lineno, _string filename);
void _memtest_printMemStat(_int32 lineno, _string filename);
void _memtest_resetMemStat(_int32 lineno, _string filename);
void _memtest_allocAndFree(_int32 lineno, _string filename);
void _memtest_freedMalloc(_int32 lineno, _string filename);
void _memtest_freedWithoutMalloc(_int32 lineno, _string filename);
void _memtest_reallocWithoutMalloc(_int32 lineno, _string filename);
void _memtest_reallocZeroSize(_int32 lineno, _string filename);
void _memtest_mallocOutOfMemory(_int32 lineno, _string filename);
void _memtest_reallocOutOfMemory(_int32 lineno, _string filename);

#endif
