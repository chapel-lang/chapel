#include <stdio.h>
#include "returnRecord.h"

R foo(void) {
  R myR;

  myR.a = 1;
  myR.b = 2;
  myR.c = 3;
  myR.d = 4;
  myR.e = 5;
  myR.f = 6;
  myR.g = 7;
  myR.h = 8;
  myR.i = 9;
  myR.j = 10;
  myR.k = 11;

  return myR;
}

#ifdef STANDALONE
void main() {
  R myR = foo();
  printf("myR.j is: %d\n", myR.j);
}
#endif

