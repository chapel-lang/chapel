#include <stdio.h>
#include "chplmem.h"
#include "chplmem_tests.h"
#include "chplrt.h"
#include "chpltypes.h"


void _memtest_printMemTable(int threshold, _int32 lineno, _string filename) {
  printMemTable(threshold, lineno, filename);
}


void _memtest_printMemStat(_int32 lineno, _string filename) {
  printMemStat(lineno, filename);
}


void _memtest_resetMemStat(_int32 lineno, _string filename) {
  resetMemStat();
}


void _memtest_allocAndFree(_int32 lineno, _string filename) {
  _int64* i;
  _bool* b;
  _real64* f;

  resetMemStat();

  i = (_int64*) _chpl_malloc(1, sizeof(_int64), "_int64", lineno, filename);
  fprintf(stdout, "malloc'd an int\n");
  printMemStat(lineno, filename);
  b = (_bool*) _chpl_malloc(1, sizeof(_bool), "_bool", lineno, filename);
  fprintf(stdout, "malloc'd abool\n");
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


void _memtest_freedMalloc(_int32 lineno, _string filename) {
  _int64* freedInt = (_int64*) _chpl_malloc(1, sizeof(_int64), "_int64", lineno, filename);
  _real64* unfreedFloat = (_real64*) _chpl_malloc(1, sizeof(_real64), "_real64", lineno, filename);
  *unfreedFloat = 99.9;
  
  printMemTable(0, lineno, filename);
  _chpl_free(freedInt, lineno, filename);
  fprintf(stdout, "And this is the memory hash table after the int "
          "variable has been freed:\n");
  printMemTable(0, lineno, filename);
}


void _memtest_freedWithoutMalloc(_int32 lineno, _string filename) {
  _int64 cantFreeMe = 13;
  _chpl_free(&cantFreeMe, lineno, filename);
}


void _memtest_reallocWithoutMalloc(_int32 lineno, _string filename) {
  _int64 cantReallocMe = 13;
  _chpl_realloc(&cantReallocMe, 3, sizeof(_int64*), "_int64", lineno, filename);
}


void _memtest_reallocZeroSize(_int32 lineno, _string filename) {
  _int64* makeMeZero = (_int64*) _chpl_malloc(1, sizeof(_int64), "_int64", lineno, filename);
  printMemTable(0, lineno, filename);
  _chpl_realloc(makeMeZero, 0, sizeof(_int64*), "_int64", lineno, filename);
  fprintf(stdout, "After a call to realloc with size equal to 0:\n");
  printMemTable(0, lineno, filename);
}


void _memtest_mallocOutOfMemory(_int32 lineno, _string filename) {
  while (1) {
    int i = 100000;
    _chpl_malloc(i, sizeof(_int64), "malloc test", lineno, filename);
  }
}


/* This test caused the "impossible" to happen when run with valgrind 2.1.2, 
   and ultimately crashed my desktop.  A bug report was submitted.  */ 
void _memtest_reallocOutOfMemory(_int32 lineno, _string filename) {
  int i = 100000;
  _int64* biggerAndBigger = NULL;
  
  while (1) {
    biggerAndBigger = (_int64*) 
      _chpl_realloc(biggerAndBigger, i, sizeof(_int64), "_int64", lineno, filename);
    i = i + 100000;
  }
}
