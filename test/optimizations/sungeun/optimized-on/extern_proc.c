#include <stdio.h>
#include "extern_proc.h"

void
foo(int x) {
  printf("in foo(): x=%d\n", x);
  fflush(stdout);
}
