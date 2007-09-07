#include <stdio.h>
#include <stdlib.h>
#include "chplcomm.h"
#include "chplexit.h"
#include "chplmem.h"
#include "chplrt.h"
#include "gdb.h"

#undef exit

// must be non-static to avoid dead-code elim. when compiling -O3

void gdbShouldBreakHere(void) {
}


static void cleanup_for_exit(void) {
  removeGDBFile();
}


static void _chpl_exit_common(int status, int all) {
  fflush(stdout);
  fflush(stderr);
  if (status != 0) {
    gdbShouldBreakHere();
  }
  cleanup_for_exit();
  if (all) {
    exitChplThreads();         // tear down the threads
  }
  if (all) {
    _chpl_comm_barrier("_chpl_comm_exit_all");
    _chpl_comm_exit_all(status);
  } else {
    _chpl_comm_exit_any(status);
  }
  exit(status);
}


void _chpl_exit_all(int status) {
  printFinalMemStat(0, 0);   // print the final memory statistics
  _chpl_exit_common(status, 1);
}


void _chpl_exit_any(int status) {
  _chpl_exit_common(status, 0);
}
