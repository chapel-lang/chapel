/*
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

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>


void chpl_comm_startVerbose() {
  chpl_verbose_comm = 1;
  chpl_comm_diags_disable();
  chpl_comm_bcast_rt_private(chpl_verbose_comm);
  chpl_comm_diags_enable();
}


void chpl_comm_stopVerbose() {
  chpl_verbose_comm = 0;
  chpl_comm_diags_disable();
  chpl_comm_bcast_rt_private(chpl_verbose_comm);
  chpl_comm_diags_enable();
}


void chpl_comm_startVerboseHere() {
  chpl_verbose_comm = 1;
}


void chpl_comm_stopVerboseHere() {
  chpl_verbose_comm = 0;
}


void chpl_comm_startDiagnostics() {
  // Make sure that there are no pending communication operations.
  chpl_rmem_consist_release(0, 0);

  chpl_comm_diagnostics = 1;
  chpl_comm_diags_disable();
  chpl_comm_bcast_rt_private(chpl_comm_diagnostics);
  chpl_comm_diags_enable();
}


void chpl_comm_stopDiagnostics() {
  // Make sure that there are no pending communication operations.
  chpl_rmem_consist_release(0, 0);

  chpl_comm_diagnostics = 0;
  chpl_comm_diags_disable();
  chpl_comm_bcast_rt_private(chpl_comm_diagnostics);
  chpl_comm_diags_enable();
}


void chpl_comm_startDiagnosticsHere() {
  // Make sure that there are no pending communication operations.
  chpl_rmem_consist_release(0, 0);

  chpl_comm_diagnostics = 1;
}


void chpl_comm_stopDiagnosticsHere() {
  // Make sure that there are no pending communication operations.
  chpl_rmem_consist_release(0, 0);

  chpl_comm_diagnostics = 0;
}


void chpl_comm_resetDiagnosticsHere() {
  chpl_comm_diags_reset();
}


void chpl_comm_getDiagnosticsHere(chpl_commDiagnostics *cd) {
  chpl_comm_diags_copy(cd);
}
