/*
 * Copyright 2004-2018 Cray Inc.
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
#include "chpl-gen-includes.h"
#include "comm-ofi-internal.h"

static struct commDiags_atomic comm_diagnostics;
static int chpl_comm_diags_enabled = 1; // for masking diags

struct commDiags_atomic *chpl_comm_ofi_getCommDiags() {
  return &comm_diagnostics;
}

void chpl_comm_ofi_commDiagsInc(atomic_uint_least64_t *val) {
  if (chpl_comm_diagnostics && chpl_comm_diags_enabled) {
     atomic_fetch_add_uint_least64_t(val, 1);
  }
}

/*
 * comm diagnostics support
 *
 * Would be good to make this uniform over all comm layers
 */

void chpl_startVerboseComm() {
  chpl_verbose_comm = 1;
  chpl_comm_diags_enabled = 0;
  chpl_comm_broadcast_private(0 /* &chpl_verbose_comm */, sizeof(int),
                              -1 /*typeIndex: unused*/);
  chpl_comm_diags_enabled = 1;
}

void chpl_stopVerboseComm() {
  chpl_verbose_comm = 0;
  chpl_comm_diags_enabled = 0;
  chpl_comm_broadcast_private(0 /* &chpl_verbose_comm */, sizeof(int),
                              -1 /*typeIndex: unused*/);
  chpl_comm_diags_enabled = 1;
}

void chpl_startVerboseCommHere() {
  chpl_verbose_comm = 1;
}

void chpl_stopVerboseCommHere() {
  chpl_verbose_comm = 1;
}

void chpl_startCommDiagnostics() {
  chpl_comm_diagnostics = 1;
  chpl_comm_diags_enabled = 0;
  chpl_comm_broadcast_private(1 /* &chpl_comm_diagnostics */, sizeof(int),
                              -1 /*typeIndex: unused*/);
  chpl_comm_diags_enabled = 1;
}

void chpl_stopCommDiagnostics() {
  chpl_comm_diagnostics = 0;
  chpl_comm_diags_enabled = 0;
  chpl_comm_broadcast_private(1 /* &chpl_comm_diagnostics */, sizeof(int),
                              -1 /*typeIndex: unused*/);
  chpl_comm_diags_enabled = 1;
}

void chpl_startCommDiagnosticsHere() {
  chpl_comm_diagnostics = 1;
}

void chpl_stopCommDiagnosticsHere() {
  chpl_comm_diagnostics = 0;
}

void chpl_resetCommDiagnosticsHere() {
  atomic_store_uint_least64_t(&comm_diagnostics.get, 0);
  atomic_store_uint_least64_t(&comm_diagnostics.get_nb, 0);
  atomic_store_uint_least64_t(&comm_diagnostics.put, 0);
  atomic_store_uint_least64_t(&comm_diagnostics.put_nb, 0);
  atomic_store_uint_least64_t(&comm_diagnostics.test_nb, 0);
  atomic_store_uint_least64_t(&comm_diagnostics.wait_nb, 0);
  atomic_store_uint_least64_t(&comm_diagnostics.try_nb, 0);
  atomic_store_uint_least64_t(&comm_diagnostics.execute_on, 0);
  atomic_store_uint_least64_t(&comm_diagnostics.execute_on_fast, 0);
  atomic_store_uint_least64_t(&comm_diagnostics.execute_on_nb, 0);
}

void chpl_getCommDiagnosticsHere(chpl_commDiagnostics *cd) {
  cd->get = atomic_load_uint_least64_t(&comm_diagnostics.get);
  cd->get_nb = atomic_load_uint_least64_t(&comm_diagnostics.get_nb);
  cd->put = atomic_load_uint_least64_t(&comm_diagnostics.put);
  cd->put_nb = atomic_load_uint_least64_t(&comm_diagnostics.put_nb);
  cd->test_nb = atomic_load_uint_least64_t(&comm_diagnostics.test_nb);
  cd->wait_nb = atomic_load_uint_least64_t(&comm_diagnostics.wait_nb);
  cd->try_nb = atomic_load_uint_least64_t(&comm_diagnostics.try_nb);
  cd->execute_on = atomic_load_uint_least64_t(&comm_diagnostics.execute_on);
  cd->execute_on_fast = atomic_load_uint_least64_t(&comm_diagnostics.execute_on_fast);
  cd->execute_on_nb = atomic_load_uint_least64_t(&comm_diagnostics.execute_on_nb);
}
