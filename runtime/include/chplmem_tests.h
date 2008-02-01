#ifndef _chplmem_tests_H_
#define _chplmem_tests_H_

#include <stdint.h>

void _memtest_printMemTable(int threshold, int32_t lineno, _string filename);
void _memtest_printMemStat(int32_t lineno, _string filename);
void _memtest_resetMemStat(int32_t lineno, _string filename);
void _memtest_allocAndFree(int32_t lineno, _string filename);
void _memtest_freedMalloc(int32_t lineno, _string filename);
void _memtest_freedWithoutMalloc(int32_t lineno, _string filename);
void _memtest_reallocWithoutMalloc(int32_t lineno, _string filename);
void _memtest_reallocZeroSize(int32_t lineno, _string filename);
void _memtest_mallocOutOfMemory(int32_t lineno, _string filename);
void _memtest_reallocOutOfMemory(int32_t lineno, _string filename);

#endif
