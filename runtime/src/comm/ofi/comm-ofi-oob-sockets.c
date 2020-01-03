/*
 * Copyright 2004-2020 Cray Inc.
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
// Sockets-based (really, faked) out-of-band support for the OFI-based
// Chapel comm layer.
//

#include "chplrt.h"
#include "chpl-env-gen.h"

#include "chpl-comm.h"
#include "chpl-mem.h"
#include "chpl-gen-includes.h"
#include "chplsys.h"
#include "error.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "comm-ofi-internal.h"


void chpl_comm_ofi_oob_init(void) {
  char const* ev;

  if ((ev = getenv("SLURM_PROCID")) != NULL) {
    int ev_i;

    if (sscanf(ev, "%i", &ev_i) != 1) {
      INTERNAL_ERROR_V("SLURM_PROCID");
    }
    chpl_nodeID = ev_i;

    if ((ev = getenv("SLURM_NTASKS")) == NULL
        || sscanf(ev, "%i", &ev_i) != 1) {
      INTERNAL_ERROR_V("SLURM_NTASKS");
    }
    chpl_numNodes = ev_i;

#if 0
    // TODO: this is temporary debug code
    {
      #define _P(name) printf("%d: %s %s\n", chpl_nodeID, #name, getenv(#name))
      _P(SLURM_STEP_NODELIST);
      fflush(stdout);
      #undef _P
    }
#endif
  } else {
    INTERNAL_ERROR_V("need slurm system launcher");
  }
}


void chpl_comm_ofi_oob_fini(void) {
  if (chpl_numNodes != 1) {
    INTERNAL_ERROR_V("multi-locale fini not supported");
  }
}


void chpl_comm_ofi_oob_barrier(void) {
  if (chpl_numNodes != 1) {
    INTERNAL_ERROR_V("multi-locale barrier not supported");
  }
}


void chpl_comm_ofi_oob_allgather(const void* in, void* out, size_t len) {
  if (chpl_numNodes == 1) {
    chpl_memcpy(out, in, len);
  } else {
    INTERNAL_ERROR_V("multi-locale allgather not supported");
  }
}


void chpl_comm_ofi_oob_bcast(void* buf, size_t len) {
  if (chpl_numNodes != 1) {
    INTERNAL_ERROR_V("multi-locale bcast not supported");
  }
}
