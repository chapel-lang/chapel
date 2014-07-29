#include "chplrt.h"

#include "chpl_rt_utils_static.h"
#include "chpl-comm.h"
#include "chplexit.h"
#include "chpl-mem.h"
#include "chplmemtrack.h"
#include "gdb.h"

#include <stdio.h>
#include <stdlib.h>

#undef exit

static void chpl_exit_common(int status, int all) {
  fflush(stdout);
  fflush(stderr);
  if (status != 0) {
    gdbShouldBreakHere();
  }
  chpl_comm_pre_task_exit(all);
  if (all) {
    chpl_task_exit();
    chpl_reportMemInfo();
  }
  chpl_mem_exit();
  chpl_comm_exit(all, status);
  exit(status);
}


void chpl_exit_all(int status) {
  chpl_exit_common(status, 1);
}


void chpl_exit_any(int status) {
  chpl_exit_common(status, 0);
}
