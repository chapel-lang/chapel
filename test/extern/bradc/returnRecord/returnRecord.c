#include <stdio.h>
#include "returnRecord.h"

R foo(void) {
  R myR;

  myR.x = 3;

  return myR;
}

#ifdef STANDALONE
void main() {
  R myR = foo();
  printf("myR.x is: %d\n", myR.x);
}
#endif

