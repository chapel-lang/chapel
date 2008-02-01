#include <stdio.h>
#include <stdint.h>
#include "chplmem.h"
#include "chplmem_tests.h"
#include "chplrt.h"
#include "chpltypes.h"


void _memtest_printMemTable(int threshold, int32_t lineno, _string filename) {
  printMemTable(threshold, lineno, filename);
}


void _memtest_printMemStat(int32_t lineno, _string filename) {
  printMemStat(lineno, filename);
}


void _memtest_resetMemStat(int32_t lineno, _string filename) {
  resetMemStat();
}


void _memtest_allocAndFree(int32_t lineno, _string filename) {
  int64_t* i;
  _chpl_bool* b;
  _real64* f;

  resetMemStat();

  i = (int64_t*) _chpl_malloc(1, sizeof(int64_t), "int64_t", lineno, filename);
  fprintf(stdout, "malloc'd an int\n");
  printMemStat(lineno, filename);
  b = (_chpl_bool*) _chpl_malloc(1, sizeof(_chpl_bool), "_chpl_bool", lineno, filename);
  fprintf(stdout, "malloc'd a bool\n");
  printMemStat(lineno, filename);

  f = (_real64*) _chpl_calloc(1, sizeof(_real64), "_real64", lineno, filename);
  *f = 99.9;
  fprintf(stdout, "calloc'd a real\n");
  printMemStat(lineno, filename);

  _chpl_free(i, lineno, filename);
  _chpl_free(b, lineno, filename);
  fprintf(stdout, "freed the int and the bool\n");
  printMemStat(lineno, filename);

  f = _chpl_realloc(f, 10, sizeof(_real64), "_real64", lineno, filename);
  fprintf(stdout, "realloc'd 10 times the real\n");
  printMemStat(lineno, filename);
  
  _chpl_free(f, lineno, filename);
  fprintf(stdout, "freed the real\n");
  printMemStat(lineno, filename);
}


void _memtest_freedMalloc(int32_t lineno, _string filename) {
  int64_t* freedInt = (int64_t*) _chpl_malloc(1, sizeof(int64_t), "int64_t", lineno, filename);
  _real64* unfreedFloat = (_real64*) _chpl_malloc(1, sizeof(_real64), "_real64", lineno, filename);
  *unfreedFloat = 99.9;
  
  printMemTable(0, lineno, filename);
  _chpl_free(freedInt, lineno, filename);
  fprintf(stdout, "And this is the memory hash table after the int "
          "variable has been freed:\n");
  printMemTable(0, lineno, filename);
}


void _memtest_freedWithoutMalloc(int32_t lineno, _string filename) {
  int64_t cantFreeMe = 13;
  _chpl_free(&cantFreeMe, lineno, filename);
}


void _memtest_reallocWithoutMalloc(int32_t lineno, _string filename) {
  int64_t cantReallocMe = 13;
  _chpl_realloc(&cantReallocMe, 3, sizeof(int64_t*), "int64_t", lineno, filename);
}


void _memtest_reallocZeroSize(int32_t lineno, _string filename) {
  int64_t* makeMeZero = (int64_t*) _chpl_malloc(1, sizeof(int64_t), "int64_t", lineno, filename);
  printMemTable(0, lineno, filename);
  _chpl_realloc(makeMeZero, 0, sizeof(int64_t*), "int64_t", lineno, filename);
  fprintf(stdout, "After a call to realloc with size equal to 0:\n");
  printMemTable(0, lineno, filename);
}


void _memtest_mallocOutOfMemory(int32_t lineno, _string filename) {
  while (1) {
    int i = 100000;
    _chpl_malloc(i, sizeof(int64_t), "malloc test", lineno, filename);
  }
}


/* This test caused the "impossible" to happen when run with valgrind 2.1.2, 
   and ultimately crashed my desktop.  A bug report was submitted.  */ 
void _memtest_reallocOutOfMemory(int32_t lineno, _string filename) {
  int i = 100000;
  int64_t* biggerAndBigger = NULL;
  
  while (1) {
    biggerAndBigger = (int64_t*) 
      _chpl_realloc(biggerAndBigger, i, sizeof(int64_t), "int64_t", lineno, filename);
    i = i + 100000;
  }
}
