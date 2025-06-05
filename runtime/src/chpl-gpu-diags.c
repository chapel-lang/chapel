/*
 * Copyright 2020-2025 Hewlett Packard Enterprise Development LP
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
#include "chpl-comm-internal.h"
#include "chpl-gpu-diags.h"
#include "chpl-comm-diags.h"
#include "chpl-mem-consistency.h"
#include "error.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

int chpl_verbose_gpu = 0;
int chpl_verbose_gpu_stacktrace = 0;
int chpl_gpu_diagnostics = 0;
int chpl_gpu_diags_print_unstable = 0;

chpl_atomic_int_least16_t chpl_gpu_diags_disable_flag;
chpl_atomic_gpuDiagnostics chpl_gpu_diags_counters;

static pthread_once_t bcastPrintUnstable_once = PTHREAD_ONCE_INIT;


static
void broadcast_print_unstable(void) {
  chpl_comm_bcast_rt_private(chpl_gpu_diags_print_unstable);
}


void chpl_gpu_startVerbose(chpl_bool stacktrace,
                           chpl_bool print_unstable,
                           int32_t lineno,
                           int32_t filename) {
  chpl_gpu_diags_print_unstable = (print_unstable == true);
  chpl_verbose_gpu_stacktrace = (stacktrace == true);
  if (pthread_once(&bcastPrintUnstable_once, broadcast_print_unstable) != 0) {
    chpl_internal_error("pthread_once(&bcastPrintUnstable_once) failed");
  }

  if (chpl_verbose_gpu == 1) {
    chpl_warning("verbose gpu was already started", lineno, filename);
  }
  chpl_verbose_gpu = 1;
  chpl_comm_bcast_rt_private(chpl_verbose_gpu);
  chpl_comm_bcast_rt_private(chpl_verbose_gpu_stacktrace);
}


void chpl_gpu_stopVerbose(int32_t lineno, int32_t filename) {
  if (chpl_verbose_gpu == 0) {
    chpl_warning("verbose gpu was never started", lineno, filename);
  }
  chpl_verbose_gpu = 0;
  chpl_comm_bcast_rt_private(chpl_verbose_gpu);
}


void chpl_gpu_startVerboseHere(chpl_bool stacktrace,
                               chpl_bool print_unstable,
                               int32_t lineno,
                               int32_t filename) {
  chpl_gpu_diags_print_unstable = (print_unstable == true);
  chpl_verbose_gpu_stacktrace = (stacktrace == true);

  if (chpl_verbose_gpu == 1) {
    chpl_warning("verbose gpu was already started", lineno, filename);
  }
  chpl_verbose_gpu = 1;
}


void chpl_gpu_stopVerboseHere(int32_t lineno, int32_t filename) {
  if (chpl_verbose_gpu == 0) {
    chpl_warning("verbose gpu was never started", lineno, filename);
  }
  chpl_verbose_gpu = 0;
}


void chpl_gpu_startDiagnostics(chpl_bool print_unstable,
                               int32_t lineno,
                               int32_t filename) {
  chpl_gpu_diags_print_unstable = (print_unstable == true);

  if (pthread_once(&bcastPrintUnstable_once, broadcast_print_unstable) != 0) {
    chpl_internal_error("pthread_once(&bcastPrintUnstable_once) failed");
  }

  // Make sure that there are no pending GPU operations.
  chpl_rmem_consist_release(lineno, filename);

  if (chpl_gpu_diagnostics == 1) {
    chpl_warning("gpu diagnostics was already started", lineno, filename);
  }
  chpl_gpu_diagnostics = 1;
  chpl_comm_bcast_rt_private(chpl_gpu_diagnostics);
}


void chpl_gpu_stopDiagnostics(int32_t lineno, int32_t filename) {
  // Make sure that there are no pending GPU operations.
  chpl_rmem_consist_release(lineno, filename);

  if (chpl_gpu_diagnostics == 0) {
    chpl_warning("gpu diagnostics was never started", lineno, filename);
  }
  chpl_gpu_diagnostics = 0;
  chpl_comm_bcast_rt_private(chpl_gpu_diagnostics);
}


void chpl_gpu_startDiagnosticsHere(chpl_bool print_unstable,
                                   int32_t lineno,
                                   int32_t filename) {
  chpl_gpu_diags_print_unstable = (print_unstable == true);

  // Make sure that there are no pending GPU operations.
  chpl_rmem_consist_release(lineno, filename);

  if (chpl_gpu_diagnostics == 1) {
    chpl_warning("gpu diagnostics was already started", lineno, filename);
  }
  chpl_gpu_diagnostics = 1;
}


void chpl_gpu_stopDiagnosticsHere(int32_t lineno, int32_t filename) {
  // Make sure that there are no pending GPU operations.
  chpl_rmem_consist_release(lineno, filename);

  if (chpl_gpu_diagnostics == 0) {
    chpl_warning("gpu diagnostics was never started", lineno, filename);
  }
  chpl_gpu_diagnostics = 0;
}


void chpl_gpu_resetDiagnosticsHere(void) {
  chpl_gpu_diags_reset();
}


void chpl_gpu_getDiagnosticsHere(chpl_gpuDiagnostics *cd) {
  chpl_gpu_diags_copy(cd);
}
