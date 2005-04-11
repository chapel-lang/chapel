#include <stdio.h>
#include "chplmem.h"
#include "chplmem_tests.h"
#include "chplrt.h"
#include "chpltypes.h"


void _chpl_memtest_freedMalloc(void) {
  _integer64* freedInt = (_integer64*) _chpl_malloc(1, sizeof(_integer64), 
                                                  "_integer64");
  _float64* unfreedFloat = (_float64*) _chpl_malloc(1, sizeof(_float64), 
                                                   "_float64");
  *unfreedFloat = 99.9;
  
  printMemTable();
  _chpl_free(freedInt);
  fprintf(stdout, "And this is the memory hash table after the integer "
          "variable has been freed:\n");
  printMemTable();
}


void _chpl_memtest_freedWithoutMalloc(void) {
  _integer64 cantFreeMe = 13;
  _chpl_free(&cantFreeMe);
}


void _chpl_memtest_reallocWithoutMalloc(void) {
  _integer64 cantReallocMe = 13;
  _chpl_realloc(&cantReallocMe, 3, sizeof(_integer64*), "_integer64");
}


void _chpl_memtest_reallocZeroSize(void) {
  _integer64* makeMeZero = (_integer64*) _chpl_malloc(1, sizeof(_integer64), 
                                                      "_integer64");
  printMemTable();
  _chpl_realloc(makeMeZero, 0, sizeof(_integer64*), "_integer64");
  fprintf(stdout, "After a call to realloc with size equal to 0:\n");
  printMemTable();
}


void _chpl_memtest_mallocOutOfMemory(void) {
  while (1) {
    int i = 100000;
    _chpl_malloc(i, sizeof(_integer64), "malloc test");
  }
}


/* This test caused the "impossible" to happen when run with valgrind 2.1.2, 
   and ultimately crashed my desktop.  A bug report was submitted.  */ 
void _chpl_memtest_reallocOutOfMemory(void) {
  int i = 100000;
  _integer64* biggerAndBigger = NULL;
  
  while (1) {
    biggerAndBigger = (_integer64*) 
      _chpl_realloc(biggerAndBigger, i, sizeof(_integer64), "_integer64");
    i = i + 100000;
  }
}
