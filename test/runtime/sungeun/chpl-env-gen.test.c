#include <stdio.h>
#include <string.h>
#include "chpl-env-gen.h"

int
main(int arc, char* argv[]) {
#include "chpl-env-gen.test.gen.c"
  return 0;
}
