#include <stdio.h>
#include <stdlib.h>
#include "chpl_rt_utils_static.h"
#include "chplcomm.h"
#include "chplexit.h"
#include "chpl_mem.h"
#include "chplmemtrack.h"
#include "chplrt.h"
#include "gdb.h"

#undef exit

static void chpl_exit_common(int status, int all) {
  fflush(stdout);
  fflush(stderr);
  if (status != 0) {
    gdbShouldBreakHere();
  }
  if (all) {
    chpl_comm_barrier("chpl_comm_exit_all");
    chpl_task_exit();
    if (!_runInGDB())
      chpl__autoDestroyRuntimeGlobals();
    chpl_reportMemInfo();
    chpl_comm_exit_all(status);
  } else {
    chpl_comm_exit_any(status);
  }
  exit(status);
}


void chpl_exit_all(int status) {
  chpl_exit_common(status, 1);
}


void chpl_exit_any(int status) {
  chpl_exit_common(status, 0);
}
