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
#include "chpl-env-gen.h"

#include "chpl-comm.h"
#include "chpl-mem.h"
#include "chpl-gen-includes.h"
#include "chplsys.h"
#include "error.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>

#include "comm-ofi-internal.h"


#ifdef CHPL_TARGET_PLATFORM_CRAY_XC
#define OOB_USE_PMI
#define OOB_USE_PMI2
#elif defined(CHPL_COMM_SUBSTRATE_SOCKETS)
#define OOB_USE_SOCKETS
#else
#error "undefined out-of-band environment"
#endif

#ifdef OOB_USE_PMI
#include <pmi.h>
#define PMICHKRET(fncall, expect)                                       \
  do {                                                                  \
    if (fncall != expect) {                                             \
      chpl_internal_error(#fncall);                                     \
    }                                                                   \
  } while (0)
#define PMICHKSUCCESS(fncall) PMICHKRET(fncall, PMI_SUCCESS)
#endif


void chpl_comm_ofi_oob_init(void) {

#if defined(OOB_USE_PMI)
#if defined(OOB_USE_PMI2)

  int spawned, size, rank, appnum;

  if (PMI2_Initialized() != PMI_TRUE) {
    PMICHKSUCCESS(PMI2_Init(&spawned, &size, &rank, &appnum));
    assert(spawned == 0);
    chpl_nodeID = (int32_t) rank;
    chpl_numNodes = (int32_t) size;
  }

#else // defined(OOB_USE_PMI2)

  PMI_BOOL initialized, spawned;
  int rank, app_size;

  PMICHKSUCCESS(PMI_Initialized(&initialized));
  if (initialized != PMI_TRUE) {
    PMICHKSUCCESS(PMI_Init(&spawned));
  }

  PMICHKSUCCESS(Get_rank_in_app(&rank));
  chpl_nodeID = (c_nodeid_t) rank;

  PMICHKSUCCESS(Get_size(&app_size));
  chpl_numNodes = (int32_t) app_size;

#endif // defined(OOB_USE_PMI2)
#elif defined(OOB_USE_SOCKETS)

  char const* ev;

  if ((ev = getenv("SLURM_PROCID")) != NULL) {
    int ev_i;

    if (sscanf(ev, "%i", &ev_i) != 1) {
      chpl_internal_error("SLURM_PROCID");
    }
    chpl_nodeID = ev_i;

    if ((ev = getenv("SLURM_NTASKS")) == NULL
        || sscanf(ev, "%i", &ev_i) != 1) {
      chpl_internal_error("SLURM_NTASKS");
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
    chpl_internal_error("OOB_USE_SOCKETS requires slurm launcher");
  }

#else // defined(OOB_USE_SOCKETS)

#error "unknown OOB mode for init"

#endif // defined(OOB_USE_SOCKETS)

}


void chpl_comm_ofi_oob_fini(void) {

#if defined(OOB_USE_PMI)
#if defined(OOB_USE_PMI2)

  if (PMI2_Initialized() == PMI_TRUE) {
    PMICHKSUCCESS(PMI2_Finalize());
  }

#else // defined(OOB_USE_PMI2)

  PMI_BOOL initialized;

  PMICHKSUCCESS((PMI_Initialized(&initialized));
  if (initialized == PMI_TRUE) {
    PMICHKSUCCESS(PMI_Finalize();
  }

#endif // defined(OOB_USE_PMI2)
#elif defined(OOB_USE_SOCKETS)

  if (chpl_numNodes != 1) {
    chpl_internal_error("multi-locale OOB_USE_SOCKETS fini not supported");
  }

#else // defined(OOB_USE_SOCKETS)

#error "unknown OOB mode for fini"

#endif // defined(OOB_USE_SOCKETS)

}


void chpl_comm_ofi_oob_barrier(void) {

#if defined(OOB_USE_PMI)

  PMICHKSUCCESS(PMI_Barrier());

#elif defined(OOB_USE_SOCKETS)

  if (chpl_numNodes != 1) {
    chpl_internal_error("multi-locale OOB_USE_SOCKETS barrier not supported");
  }

#else

#error "unknown OOB mode for barrier"

#endif

}


void chpl_comm_ofi_oob_allgather(void* in, void* out, int len) {

#if defined(OOB_USE_PMI)

  PMICHKSUCCESS(PMI_Allgather(in, out, len));

#elif defined(OOB_USE_SOCKETS)

  if (chpl_numNodes == 1) {
    memcpy(out, in, len);
  } else {
    chpl_internal_error("multi-locale OOB_USE_SOCKETS allgather not supported");
  }

#else

#error "unknown OOB mode for allgather"

#endif

}
