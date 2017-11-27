#include <stdlib.h>
#include <stdio.h>

#include "chpl-prefetch.h"

int main(int argc, char** argv) {
  int x = 8;
  int* y = &x;
  chpl_prefetch(y);
  printf("%i\n", x);
  return 0;
}
