#include "SomeCLibrary.h"

#include <stdio.h>

void clibfn1(void) {
  printf("Hello from C library function '%s'\n", __FUNCTION__);
}

int clibfn2(int x) {
  printf("Hello from C library function '%s'\n", __FUNCTION__);
  return x + 3;
}
