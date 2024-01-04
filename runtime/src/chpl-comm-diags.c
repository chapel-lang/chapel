/*
 * Copyright 2020-2024 Hewlett Packard Enterprise Development LP
 * Copyright 2004-2019 Cray Inc.
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

//
// Communication diagnostics support.
//

#include "chplrt.h"
#include "chpl-env-gen.h"

#include "chpl-comm.h"
#include "chpl-comm-diags.h"
#include "chpl-comm-internal.h"
#include "chpl-mem-consistency.h"
#include "error.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

int chpl_verbose_comm = 0;
int chpl_verbose_comm_stacktrace = 0;
int chpl_comm_diagnostics = 0;
int chpl_comm_diags_print_unstable = 0;

chpl_atomic_commDiagnostics chpl_comm_diags_counters;

static pthread_once_t bcastPrintUnstable_once = PTHREAD_ONCE_INIT;


static
void broadcast_print_unstable(void) {
  chpl_bool prevDisabled = chpl_task_setCommDiagsTemporarilyDisabled(true);
  chpl_comm_bcast_rt_private(chpl_comm_diags_print_unstable);
  (void)chpl_task_setCommDiagsTemporarilyDisabled(prevDisabled);
}


void chpl_comm_startVerbose(chpl_bool stacktrace,
                            chpl_bool print_unstable,
                            int32_t lineno,
                            int32_t filename) {
  chpl_comm_diags_print_unstable = (print_unstable == true);
  chpl_verbose_comm_stacktrace = (stacktrace == true);
  if (pthread_once(&bcastPrintUnstable_once, broadcast_print_unstable) != 0) {
    chpl_internal_error("pthread_once(&bcastPrintUnstable_once) failed");
  }

  if (chpl_verbose_comm == 1) {
    chpl_warning("verbose comm was already started", lineno, filename);
  }
  chpl_verbose_comm = 1;

  chpl_bool prevDisabled = chpl_task_setCommDiagsTemporarilyDisabled(true);
  chpl_comm_bcast_rt_private(chpl_verbose_comm);
  chpl_comm_bcast_rt_private(chpl_verbose_comm_stacktrace);
  (void)chpl_task_setCommDiagsTemporarilyDisabled(prevDisabled);
}


void chpl_comm_stopVerbose(int32_t lineno,
                           int32_t filename) {

  if (chpl_verbose_comm == 0) {
    chpl_warning("verbose comm was never started", lineno, filename);
  }
  chpl_verbose_comm = 0;

  chpl_bool prevDisabled = chpl_task_setCommDiagsTemporarilyDisabled(true);
  chpl_comm_bcast_rt_private(chpl_verbose_comm);
  (void)chpl_task_setCommDiagsTemporarilyDisabled(prevDisabled);
}


void chpl_comm_startVerboseHere(chpl_bool stacktrace,
                                chpl_bool print_unstable,
                                int32_t lineno,
                                int32_t filename) {
  chpl_comm_diags_print_unstable = (print_unstable == true);
  chpl_verbose_comm_stacktrace = (stacktrace == true);

  if (chpl_verbose_comm == 1) {
    chpl_warning("verbose comm was already started", lineno, filename);
  }
  chpl_verbose_comm = 1;
}


void chpl_comm_stopVerboseHere(int32_t lineno, int32_t filename) {

  if (chpl_verbose_comm == 0) {
    chpl_warning("verbose comm was never started", lineno, filename);
  }
  chpl_verbose_comm = 0;
}


void chpl_comm_startDiagnostics(chpl_bool print_unstable,
                                int32_t lineno,
                                int32_t filename) {
  chpl_comm_diags_print_unstable = (print_unstable == true);

  if (pthread_once(&bcastPrintUnstable_once, broadcast_print_unstable) != 0) {
    chpl_internal_error("pthread_once(&bcastPrintUnstable_once) failed");
  }

  // Make sure that there are no pending communication operations.
  chpl_rmem_consist_release(lineno, filename);

  if (chpl_comm_diagnostics == 1) {
    chpl_warning("comm diagnostics was already started", lineno, filename);
  }
  chpl_comm_diagnostics = 1;

  chpl_bool prevDisabled = chpl_task_setCommDiagsTemporarilyDisabled(true);
  chpl_comm_bcast_rt_private(chpl_comm_diagnostics);
  (void)chpl_task_setCommDiagsTemporarilyDisabled(prevDisabled);
}


void chpl_comm_stopDiagnostics(int32_t lineno, int32_t filename) {
  // Make sure that there are no pending communication operations.
  chpl_rmem_consist_release(lineno, filename);

  if (chpl_comm_diagnostics == 0) {
    chpl_warning("comm diagnostics was never started", lineno, filename);
  }
  chpl_comm_diagnostics = 0;

  chpl_bool prevDisabled = chpl_task_setCommDiagsTemporarilyDisabled(true);
  chpl_comm_bcast_rt_private(chpl_comm_diagnostics);
  (void)chpl_task_setCommDiagsTemporarilyDisabled(prevDisabled);
}


void chpl_comm_startDiagnosticsHere(chpl_bool print_unstable,
                                    int32_t lineno,
                                    int32_t filename) {
  chpl_comm_diags_print_unstable = (print_unstable == true);

  // Make sure that there are no pending communication operations.
  chpl_rmem_consist_release(lineno, filename);

  if (chpl_comm_diagnostics == 1) {
    chpl_warning("comm diagnostics was already started", lineno, filename);
  }
  chpl_comm_diagnostics = 1;
}


void chpl_comm_stopDiagnosticsHere(int32_t lineno, int32_t filename) {
  // Make sure that there are no pending communication operations.
  chpl_rmem_consist_release(lineno, filename);

  if (chpl_comm_diagnostics == 0) {
    chpl_warning("comm diagnostics was never started", lineno, filename);
  }
  chpl_comm_diagnostics = 0;
}


void chpl_comm_resetDiagnosticsHere(void) {
  chpl_comm_diags_reset();
}


void chpl_comm_getDiagnosticsHere(chpl_commDiagnostics *cd) {
  chpl_comm_diags_copy(cd);
}
