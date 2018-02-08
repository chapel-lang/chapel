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
#else
#define OOB_USE_OWN
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
#elif defined(OOB_USE_OWN)

#error "USE_OWN OOB mode init not supported"

#else // defined(OOB_USE_OWN)

#error "unknown OOB mode for init"

#endif // defined(OOB_USE_OWN)

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
#elif defined(OOB_USE_OWN)

#error "USE_OWN OOB mode fini not supported"

#else // defined(OOB_USE_OWN)

#error "unknown OOB mode for fini"

#endif // defined(OOB_USE_OWN)

}


void chpl_comm_ofi_oob_barrier(void) {

#if defined(OOB_USE_PMI)

  PMICHKSUCCESS(PMI_Barrier());

#elif defined(OOB_USE_OWN)

#error "USE_OWN OOB mode barrier not supported"

#else

#error "unknown OOB mode for barrier"

#endif

}


void chpl_comm_ofi_oob_allgather(void* in, void* out, int len) {

#if defined(OOB_USE_PMI)

  PMICHKSUCCESS(PMI_Allgather(in, out, len));

#elif defined(OOB_USE_OWN)

#error "USE_OWN OOB mode allgather not supported"

#else

#error "unknown OOB mode for allgather"

#endif

}
