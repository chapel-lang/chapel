#ifndef _chplmem_tests_H_
#define _chplmem_tests_H_

void _chpl_memtest_printMemTable(void);
void _chpl_memtest_printMemStat(void);
void _chpl_memtest_resetMemStat(void);
void _chpl_memtest_allocAndFree(void);
void _chpl_memtest_freedMalloc(void);
void _chpl_memtest_freedWithoutMalloc(void);
void _chpl_memtest_reallocWithoutMalloc(void);
void _chpl_memtest_reallocZeroSize(void);
void _chpl_memtest_mallocOutOfMemory(void);
void _chpl_memtest_reallocOutOfMemory(void);

#endif
