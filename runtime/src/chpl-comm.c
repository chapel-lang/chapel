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

//
// Shared code for different comm implementations in
//  comm/<commlayer>/comm-<commlayer>.c
//
#include "chplrt.h"
#include "chpl-comm.h"
#include "chpl-env.h"
#include "chpl-mem.h"
#include "chpl-mem-consistency.h"

#include <pthread.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>

int32_t chpl_nodeID = -1;
int32_t chpl_numNodes = -1;

int chpl_verbose_comm;
int chpl_comm_diagnostics;
int chpl_verbose_mem;

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


static int maxHeapSize_set;
static pthread_once_t maxHeapSize_once = PTHREAD_ONCE_INIT;
static size_t maxHeapSize;

static
void set_maxHeapSize(void)
{
  maxHeapSize = chpl_env_rt_get_size("MAX_HEAP_SIZE", 0);
}

size_t chpl_comm_getenvMaxHeapSize(void)
{
  if (!maxHeapSize_set
      && pthread_once(&maxHeapSize_once, set_maxHeapSize) != 0) {
    chpl_internal_error("pthread_once(&maxHeapSize_once) failed");
  }

  return maxHeapSize;
}


void* chpl_get_global_serialize_table(int64_t idx) {
  return chpl_global_serialize_table[idx];
}
