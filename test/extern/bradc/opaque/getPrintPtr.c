#include <stdio.h>
#include <stdlib.h>
#include "getPrintPtr.h"

int* getPtr(void) {
  int* ptr = (int*)malloc(sizeof(int));
  *ptr = 12345;
  return ptr;
}
  
void printPtr(int* ptr) {
  printf("%d\n", *ptr);
}
