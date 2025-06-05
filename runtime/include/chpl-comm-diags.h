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

#ifndef _chpl_comm_diags_h_
#define _chpl_comm_diags_h_

#include <stdarg.h>
#include <stdio.h>

#include "chpl-atomics.h"
#include "chpl-comm.h"
#include "error.h"

#ifdef __cplusplus
extern "C" {
#endif

////////////////////
//
// Public
//

extern int chpl_verbose_comm;     // set via startVerboseComm
extern int chpl_verbose_comm_stacktrace;

extern int chpl_comm_diagnostics; // set via startCommDiagnostics
extern int chpl_comm_diags_print_unstable;

#define CHPL_COMM_DIAGS_VARS_ALL(MACRO) \
  MACRO(get) \
  MACRO(get_nb) \
  MACRO(put) \
  MACRO(put_nb) \
  MACRO(test_nb) \
  MACRO(wait_nb) \
  MACRO(try_nb) \
  MACRO(amo) \
  MACRO(execute_on) \
  MACRO(execute_on_fast) \
  MACRO(execute_on_nb) \
  MACRO(cache_get_hits) \
  MACRO(cache_get_misses) \
  MACRO(cache_put_hits) \
  MACRO(cache_put_misses) \
  MACRO(cache_num_prefetches) \
  MACRO(cache_num_page_readaheads) \
  MACRO(cache_prefetch_unused) \
  MACRO(cache_prefetch_waited) \
  MACRO(cache_readahead_unused) \
  MACRO(cache_readahead_waited)

typedef struct _chpl_commDiagnostics {
#define _COMM_DIAGS_DECL(cdv) uint64_t cdv;
  CHPL_COMM_DIAGS_VARS_ALL(_COMM_DIAGS_DECL)
#undef _COMM_DIAGS_DECL
} chpl_commDiagnostics;

void chpl_comm_startVerbose(chpl_bool, chpl_bool, int32_t, int32_t);
void chpl_comm_stopVerbose(int32_t, int32_t);
void chpl_comm_startVerboseHere(chpl_bool, chpl_bool, int32_t, int32_t);
void chpl_comm_stopVerboseHere(int32_t, int32_t);

void chpl_comm_startDiagnostics(chpl_bool, int32_t, int32_t);
void chpl_comm_stopDiagnostics(int32_t, int32_t);
void chpl_comm_startDiagnosticsHere(chpl_bool, int32_t, int32_t);
void chpl_comm_stopDiagnosticsHere(int32_t, int32_t);
void chpl_comm_resetDiagnosticsHere(void);
void chpl_comm_getDiagnosticsHere(chpl_commDiagnostics *cd);


////////////////////
//
// Private
//
typedef struct _chpl_atomic_commDiagnostics {
#define _COMM_DIAGS_DECL_ATOMIC(cdv) chpl_atomic_uint_least64_t cdv;
  CHPL_COMM_DIAGS_VARS_ALL(_COMM_DIAGS_DECL_ATOMIC)
#undef _COMM_DIAGS_DECL_ATOMIC
} chpl_atomic_commDiagnostics;

extern chpl_atomic_commDiagnostics chpl_comm_diags_counters;

static inline
void chpl_comm_diags_init(void) {
#define _COMM_DIAGS_INIT(cdv) \
        atomic_init_uint_least64_t(&chpl_comm_diags_counters.cdv, 0);
  CHPL_COMM_DIAGS_VARS_ALL(_COMM_DIAGS_INIT);
#undef _COMM_DIAGS_INIT
}

static inline
void chpl_comm_diags_reset(void) {
#define _COMM_DIAGS_RESET(cdv) \
        atomic_store_uint_least64_t(&chpl_comm_diags_counters.cdv, 0);
 CHPL_COMM_DIAGS_VARS_ALL(_COMM_DIAGS_RESET);
#undef _COMM_DIAGS_RESET
}

static inline
void chpl_comm_diags_copy(chpl_commDiagnostics* cd) {
#define _COMM_DIAGS_COPY(cdv) \
        cd->cdv = atomic_load_uint_least64_t(&chpl_comm_diags_counters.cdv);
  CHPL_COMM_DIAGS_VARS_ALL(_COMM_DIAGS_COPY);
#undef _COMM_DIAGS_COPY
}

extern chpl_bool chpl_task_setCommDiagsTemporarilyDisabled(chpl_bool);
extern chpl_bool chpl_task_getCommDiagsTemporarilyDisabled(void);

#define chpl_comm_diags_verbose_printf(is_unstable, format, ...)   \
  do {                                                             \
    if (chpl_verbose_comm                                          \
        && !chpl_task_getCommDiagsTemporarilyDisabled()            \
        && (!is_unstable || chpl_comm_diags_print_unstable)) {     \
      char* stack = NULL;                                          \
      if (chpl_verbose_comm_stacktrace) {                          \
        stack = chpl_stack_unwind_to_string(' ');                  \
      }                                                            \
      if (stack != NULL) {                                         \
        printf("%d: " format " <%s>\n", chpl_nodeID, __VA_ARGS__, stack); \
        chpl_mem_free(stack, 0, 0);                                \
      } else {                                                     \
        printf("%d: " format "\n", chpl_nodeID, __VA_ARGS__);      \
      }                                                            \
    }                                                              \
  } while(0)

#define chpl_comm_diags_verbose_rdma(op, node, size, ln, fn, commid)     \
  chpl_comm_diags_verbose_printf(false,                                  \
                                 "%s:%d: remote %s, node %d, %zu bytes, " \
                                 "commid %d",                            \
                                 chpl_lookupFilename(fn), ln, op,        \
                                 (int) node, size, (int) commid)

#define chpl_comm_diags_verbose_rdmaStrd(op, node, ln, fn, commid)      \
  chpl_comm_diags_verbose_printf(false,                                 \
                                 "%s:%d: remote strided %s, node %d, "  \
                                 "commid %d",                           \
                                 chpl_lookupFilename(fn), ln, op,       \
                                 (int) node, (int) commid)

#define chpl_comm_diags_verbose_amo(op, node, ln, fn)                   \
  chpl_comm_diags_verbose_printf(true,                                  \
                                 "%s:%d: remote %s, node %d",           \
                                 chpl_lookupFilename(fn), ln, op,       \
                                 (int) node)

#define chpl_comm_diags_verbose_executeOn(kind, node, ln, fn)           \
  chpl_comm_diags_verbose_printf(false,                                 \
                                 "%s:%d: remote %-*sexecuteOn, node %d", \
                                 chpl_lookupFilename(fn), ln,           \
                                 ((int) strlen(kind)                    \
                                  + ((strlen(kind) == 0) ? 0 : 1)),     \
                                 kind, (int) node)

#define chpl_comm_diags_incr(_ctr)                                           \
  do {                                                                       \
    if (chpl_comm_diagnostics &&                                             \
        !chpl_task_getCommDiagsTemporarilyDisabled()) {                      \
      chpl_atomic_uint_least64_t* ctrAddr = &chpl_comm_diags_counters._ctr;       \
      (void) atomic_fetch_add_explicit_uint_least64_t(ctrAddr, 1,            \
                                                      chpl_memory_order_relaxed); \
    }                                                                        \
  } while(0)

#ifdef __cplusplus
}
#endif

#endif
