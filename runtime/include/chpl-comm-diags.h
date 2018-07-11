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

#include "chpl-atomics.h"
#include "chpl-comm.h"

typedef struct _chpl_atomic_commDiagnostics {
#define _COMM_DIAGS_DECL_ATOMIC(cdv) atomic_uint_least64_t cdv;
  CHPL_COMM_DIAGS_VARS_ALL(_COMM_DIAGS_DECL_ATOMIC)
#undef _COMM_DIAGS_DECL_ATOMIC
} chpl_atomic_commDiagnostics;


static inline
void chpl_comm_diags_init(chpl_atomic_commDiagnostics* acd) {
#define _COMM_DIAGS_INIT(cdv) atomic_init_uint_least64_t(&acd->cdv, 0);
  CHPL_COMM_DIAGS_VARS_ALL(_COMM_DIAGS_INIT);
#undef _COMM_DIAGS_INIT
}

static inline
void chpl_comm_diags_reset(chpl_atomic_commDiagnostics* acd) {
#define _COMM_DIAGS_RESET(cdv) atomic_store_uint_least64_t(&acd->cdv, 0);
  CHPL_COMM_DIAGS_VARS_ALL(_COMM_DIAGS_RESET);
#undef _COMM_DIAGS_RESET
}

static inline
void chpl_comm_diags_copy(chpl_commDiagnostics* cd,
                          chpl_atomic_commDiagnostics* acd) {
#define _COMM_DIAGS_COPY(cdv) cd->cdv = atomic_load_uint_least64_t(&acd->cdv);
  CHPL_COMM_DIAGS_VARS_ALL(_COMM_DIAGS_COPY);
#undef _COMM_DIAGS_COPY
}

static inline
void chpl_comm_diags_incr(atomic_uint_least64_t* op) {
  (void) atomic_fetch_add_uint_least64_t(op, 1);
}

#endif
