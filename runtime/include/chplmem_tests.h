#ifndef _chplmem_tests_H_
#define _chplmem_tests_H_

void _chpl_memtest_printMemTable(int threshold, _int32 lineno, _string filename);
void _chpl_memtest_printMemStat(_int32 lineno, _string filename);
void _chpl_memtest_resetMemStat(_int32 lineno, _string filename);
void _chpl_memtest_allocAndFree(_int32 lineno, _string filename);
void _chpl_memtest_freedMalloc(_int32 lineno, _string filename);
void _chpl_memtest_freedWithoutMalloc(_int32 lineno, _string filename);
void _chpl_memtest_reallocWithoutMalloc(_int32 lineno, _string filename);
void _chpl_memtest_reallocZeroSize(_int32 lineno, _string filename);
void _chpl_memtest_mallocOutOfMemory(_int32 lineno, _string filename);
void _chpl_memtest_reallocOutOfMemory(_int32 lineno, _string filename);

#endif
