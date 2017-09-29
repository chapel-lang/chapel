/*
 * Copyright 2004-2017 Cray Inc.
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
// Shared code for different comm implementations in
//  comm/<commlayer>/comm-<commlayer>.c
//
#include "chplrt.h"
#include "chpl-comm.h"
#include "chpl-comm-count-calls.h"
#include "chpl-mem.h"
#include "chpl-mem-consistency.h"

#include <stdint.h>
#include <string.h>
#include <unistd.h>

int32_t chpl_nodeID = -1;
int32_t chpl_numNodes = -1;

int chpl_verbose_comm;
int chpl_comm_diagnostics;
int chpl_verbose_mem;

atomic_uint_least64_t chpl_comm_n_get_calls;
atomic_uint_least64_t chpl_comm_n_put_calls;
atomic_uint_least64_t chpl_comm_n_prefetch_calls;

void chpl_startCommDiagnostics(void); // this one implemented by comm layers
void chpl_gen_startCommDiagnostics(void); // this one implemented in chpl-comm.c
void chpl_stopCommDiagnostics(void);
void chpl_gen_stopCommDiagnostics(void);
void chpl_startCommDiagnosticsHere(void);
void chpl_gen_startCommDiagnosticsHere(void);
void chpl_stopCommDiagnosticsHere(void);
void chpl_gen_stopCommDiagnosticsHere(void);
void chpl_resetCommDiagnosticsHere(void);
void chpl_getCommDiagnosticsHere(chpl_commDiagnostics *cd);

void chpl_gen_startCommDiagnostics(void) {
  // Make sure that there are no pending communication operations.
  chpl_rmem_consist_release(0, 0);
  // And then start the comm diagnostics as usual.
  chpl_startCommDiagnostics();
}

void chpl_gen_stopCommDiagnostics(void) {
  // Make sure that there are no pending communication operations.
  chpl_rmem_consist_release(0, 0);
  // And then stop the comm diagnostics as usual.
  chpl_stopCommDiagnostics();
}

void chpl_gen_startCommDiagnosticsHere(void) {
  // Make sure that there are no pending communication operations.
  chpl_rmem_consist_release(0, 0);
  // And then start the comm diagnostics as usual.
  chpl_startCommDiagnosticsHere();
}

void chpl_gen_stopCommDiagnosticsHere(void) {
  // Make sure that there are no pending communication operations.
  chpl_rmem_consist_release(0, 0);
  // And then stop the comm diagnostics as usual.
  chpl_stopCommDiagnosticsHere();
}

void chpl_gen_resetCommDiagnosticsHere(void) {
  // Make sure that there are no pending communication operations.
  chpl_rmem_consist_release(0, 0);
  // And then reset the comm diagnostics as usual.
  chpl_resetCommDiagnosticsHere();
  // And then clear the local counters
  atomic_store_explicit_uint_least64_t(&chpl_comm_n_get_calls, 0, memory_order_seq_cst);
  atomic_store_explicit_uint_least64_t(&chpl_comm_n_put_calls, 0, memory_order_seq_cst);
  atomic_store_explicit_uint_least64_t(&chpl_comm_n_prefetch_calls, 0, memory_order_seq_cst);
}

void chpl_gen_getCommDiagnosticsHere(chpl_commDiagnostics *cd) {
  // Make sure that there are no pending communication operations.
  chpl_rmem_consist_release(0, 0);
  // And then get the comm diagnostics as usual.
  chpl_getCommDiagnosticsHere(cd);
  // And then get the local counters
  cd->get_calls = atomic_load_explicit_uint_least64_t(&chpl_comm_n_get_calls, memory_order_relaxed);
  cd->put_calls = atomic_load_explicit_uint_least64_t(&chpl_comm_n_put_calls, memory_order_relaxed);
  cd->prefetch_calls = atomic_load_explicit_uint_least64_t(&chpl_comm_n_prefetch_calls, memory_order_relaxed);
}

size_t chpl_comm_getenvMaxHeapSize(void)
{
  char*  p;
  static int    env_checked = 0;
  static size_t size = 0;

  if (env_checked)
    return size;

  if ((p = getenv("CHPL_RT_MAX_HEAP_SIZE")) != NULL) {
    //
    // The user specified a maximum size, so start with that.
    //
    int  num_scanned;
    char units;

    if ((num_scanned = sscanf(p, "%zi%c", &size, &units)) != 1) {
      if (num_scanned == 2 && strchr("kKmMgG", units) != NULL) {
        switch (units) {
        case 'k' : case 'K': size <<= 10; break;
        case 'm' : case 'M': size <<= 20; break;
        case 'g' : case 'G': size <<= 30; break;
        }
      }
      else {
        chpl_warning("Cannot parse CHPL_RT_MAX_HEAP_SIZE environment "
                     "variable; assuming 1g", 0, 0);
        size = ((size_t) 1) << 30;
      }
    }
  }

  env_checked = 1;

  return size;
}

void* chpl_get_global_serialize_table(int64_t idx) {
  return chpl_global_serialize_table[idx];
}
