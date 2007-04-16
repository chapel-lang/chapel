#include <stdlib.h>
#include "chplexit.h"
#include "chplrt.h"
#include "gdb.h"

#undef exit

// must be non-static to avoid dead-code elim. when compiling -O3
void gdbShouldBreakHere(void) {
}

static void cleanup_for_exit(void) {
  removeGDBFile();
}

void _chpl_exit(int status) {
  if (status != 0) {
    gdbShouldBreakHere();
  }
  cleanup_for_exit();
  exit(status);
}


