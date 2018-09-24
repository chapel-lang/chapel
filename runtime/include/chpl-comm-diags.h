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

#ifndef _chpl_comm_diags_h_
#define _chpl_comm_diags_h_

#include <stdarg.h>
#include <stdio.h>

#include "chpl-atomics.h"
#include "chpl-comm.h"

typedef struct _chpl_atomic_commDiagnostics {
#define _COMM_DIAGS_DECL_ATOMIC(cdv) atomic_uint_least64_t cdv;
  CHPL_COMM_DIAGS_VARS_ALL(_COMM_DIAGS_DECL_ATOMIC)
#undef _COMM_DIAGS_DECL_ATOMIC
} chpl_atomic_commDiagnostics;

chpl_atomic_commDiagnostics chpl_comm_diags_counters;
atomic_int_least16_t chpl_comm_diags_disable_flag;

static inline
void chpl_comm_diags_init(void) {
#define _COMM_DIAGS_INIT(cdv) \
        atomic_init_uint_least64_t(&chpl_comm_diags_counters.cdv, 0);
  CHPL_COMM_DIAGS_VARS_ALL(_COMM_DIAGS_INIT);
#undef _COMM_DIAGS_INIT
  atomic_init_int_least16_t(&chpl_comm_diags_disable_flag, 0);
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

static inline
void chpl_comm_diags_disable(void) {
  (void) atomic_fetch_add_int_least16_t(&chpl_comm_diags_disable_flag, 1);
}

static inline
void chpl_comm_diags_enable(void) {
  (void) atomic_fetch_add_int_least16_t(&chpl_comm_diags_disable_flag, -1);
}

static inline
int chpl_comm_diags_is_enabled(void) {
  return (atomic_load_int_least16_t(&chpl_comm_diags_disable_flag) <= 0);
}

static inline
void chpl_comm_diags_verbose_printf(const char*, ...)
  __attribute__((format(printf, 1, 2)));
static inline
void chpl_comm_diags_verbose_printf(const char* format, ...) {
  if (chpl_verbose_comm && chpl_comm_diags_is_enabled()) {
    char myFmt[100];
    snprintf(myFmt, sizeof(myFmt), "%d: %s\n", chpl_nodeID, format);
    va_list ap;
    va_start(ap, format);
    vfprintf(stdout, myFmt, ap);
    va_end(ap);
  }
}

#define chpl_comm_diags_incr(_ctr)                                      \
  do {                                                                  \
    if (chpl_comm_diagnostics && chpl_comm_diags_is_enabled()) {        \
      atomic_uint_least64_t* ctrAddr = &chpl_comm_diags_counters._ctr;  \
      (void) atomic_fetch_add_uint_least64_t(ctrAddr, 1);               \
    }                                                                   \
  } while(0)

#endif
