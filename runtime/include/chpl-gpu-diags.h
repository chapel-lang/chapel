/*
 * Copyright 2020-2022 Hewlett Packard Enterprise Development LP
 * Copyright 2004-2019 Cray Inc.
 * Other additional copyright holders may be indicated within.
 *
 * The entirety of this work is licensed under the Apache License,
 * Version 2.0 (the "License"); you may not use this file except
 * in compliance with the License.
 *
 * You may obtain a copy of the License at * *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef _chpl_gpu_diags_h_
#define _chpl_gpu_diags_h_

#include <stdarg.h>
#include <stdio.h>

#include "chpl-atomics.h"
#include "chpl-comm.h"
//#include "chpl-gpu.h"
#include "error.h"

#ifdef __cplusplus
extern "C" {
#endif

////////////////////
//
// Public
//

extern int chpl_verbose_gpu;     // set via startVerboseComm
extern int chpl_verbose_gpu_stacktrace;

extern int chpl_gpu_diagnostics; // set via startCommDiagnostics
extern int chpl_gpu_diags_print_unstable;

#define CHPL_GPU_DIAGS_VARS_ALL(MACRO) \
  MACRO(kernel_launch)


typedef struct _chpl_gpuDiagnostics {
#define _GPU_DIAGS_DECL(cdv) uint64_t cdv;
  CHPL_GPU_DIAGS_VARS_ALL(_GPU_DIAGS_DECL)
#undef _GPU_DIAGS_DECL
} chpl_gpuDiagnostics;

void chpl_gpu_startVerbose(chpl_bool, chpl_bool);
void chpl_gpu_stopVerbose(void);
void chpl_gpu_startVerboseHere(chpl_bool, chpl_bool);
void chpl_gpu_stopVerboseHere(void);

void chpl_gpu_startDiagnostics(chpl_bool);
void chpl_gpu_stopDiagnostics(void);
void chpl_gpu_startDiagnosticsHere(chpl_bool);
void chpl_gpu_stopDiagnosticsHere(void);
void chpl_gpu_resetDiagnosticsHere(void);
void chpl_gpu_getDiagnosticsHere(chpl_gpuDiagnostics *cd);


////////////////////
//
// Private
//
typedef struct _chpl_atomic_gpuDiagnostics {
#define _GPU_DIAGS_DECL_ATOMIC(cdv) atomic_uint_least64_t cdv;
  CHPL_GPU_DIAGS_VARS_ALL(_GPU_DIAGS_DECL_ATOMIC)
#undef _GPU_DIAGS_DECL_ATOMIC
} chpl_atomic_gpuDiagnostics;

extern chpl_atomic_gpuDiagnostics chpl_gpu_diags_counters;
extern atomic_int_least16_t chpl_gpu_diags_disable_flag;

static inline
void chpl_gpu_diags_init(void) {
#define _GPU_DIAGS_INIT(cdv) \
        atomic_init_uint_least64_t(&chpl_gpu_diags_counters.cdv, 0);
  CHPL_GPU_DIAGS_VARS_ALL(_GPU_DIAGS_INIT);
#undef _GPU_DIAGS_INIT
  atomic_init_int_least16_t(&chpl_gpu_diags_disable_flag, 0);
}

static inline
void chpl_gpu_diags_reset(void) {
#define _GPU_DIAGS_RESET(cdv) \
        atomic_store_uint_least64_t(&chpl_gpu_diags_counters.cdv, 0);
 CHPL_GPU_DIAGS_VARS_ALL(_GPU_DIAGS_RESET);
#undef _GPU_DIAGS_RESET
}

static inline
void chpl_gpu_diags_copy(chpl_gpuDiagnostics* cd) {
#define _GPU_DIAGS_COPY(cdv) \
        cd->cdv = atomic_load_uint_least64_t(&chpl_gpu_diags_counters.cdv);
  CHPL_GPU_DIAGS_VARS_ALL(_GPU_DIAGS_COPY);
#undef _GPU_DIAGS_COPY
}

static inline
void chpl_gpu_diags_disable(void) {
  (void) atomic_fetch_add_int_least16_t(&chpl_gpu_diags_disable_flag, 1);
}

static inline
void chpl_gpu_diags_enable(void) {
  (void) atomic_fetch_add_int_least16_t(&chpl_gpu_diags_disable_flag, -1);
}

static inline
int chpl_gpu_diags_is_enabled(void) {
  return (atomic_load_int_least16_t(&chpl_gpu_diags_disable_flag) <= 0);
}

#define chpl_gpu_diags_verbose_printf(is_unstable, device_id, format, ...)   \
  do {                                                             \
    if (chpl_verbose_gpu                                          \
        && chpl_gpu_diags_is_enabled()                            \
        && (!is_unstable || chpl_gpu_diags_print_unstable)) {     \
      char* stack = NULL;                                          \
      if (chpl_verbose_gpu_stacktrace) {                          \
        stack = chpl_stack_unwind_to_string(' ');                  \
      }                                                            \
      if (stack != NULL) {                                         \
        printf("%d (gpu %d): " format " <%s>\n", chpl_nodeID, (int) device_id, \
               __VA_ARGS__, stack);                                \
        chpl_mem_free(stack, 0, 0);                                \
      } else {                                                     \
        printf("%d (gpu %d): " format "\n", chpl_nodeID, (int) device_id, \
               __VA_ARGS__);      \
      }                                                            \
    }                                                              \
  } while(0)

#define chpl_gpu_diags_verbose_launch(ln, fn, device_id)     \
  chpl_gpu_diags_verbose_printf(false, device_id,          \
                                "%s:%d: kernel launch",    \
                                chpl_lookupFilename(fn), ln)

#define chpl_gpu_diags_incr(_ctr)                                           \
  do {                                                                       \
    if (chpl_gpu_diagnostics && chpl_gpu_diags_is_enabled()) {             \
      atomic_uint_least64_t* ctrAddr = &chpl_gpu_diags_counters._ctr;       \
      (void) atomic_fetch_add_explicit_uint_least64_t(ctrAddr, 1,            \
                                                      memory_order_relaxed); \
    }                                                                        \
  } while(0)

#ifdef __cplusplus
}
#endif

#endif
