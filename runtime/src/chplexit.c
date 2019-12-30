/*
 * Copyright 2004-2020 Cray Inc.
 * Other additional copyright holders may be indicated within.
 * 
 * The entirety of this work is licensed under the Apache License,
 * Version 2.0 (the "License"); you may not use this file except
 * in compliance with the License.
 * 
 * You may obtain a copy of the License at
 * 
 *     http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "chplrt.h"

#include "chpl_rt_utils_static.h"
#include "chpl-comm.h"
#include "chplexit.h"
#include "chpl-mem.h"
#include "chplmemtrack.h"
#include "chpl-topo.h"
#include "gdb.h"

#include <stdio.h>
#include <stdlib.h>

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
  chpl_comm_exit(all, status);
  if (all) {
    chpl_mem_exit();
    chpl_topo_exit();
  }
  exit(status);
}


void chpl_exit_all(int status) {
  chpl_exit_common(status, 1);
}


void chpl_exit_any(int status) {
  chpl_exit_common(status, 0);
}
