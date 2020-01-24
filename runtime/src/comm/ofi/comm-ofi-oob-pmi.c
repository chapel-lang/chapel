/*
 * Copyright 2004-2020 Hewlett Packard Enterprise Development LP
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
// PMI-based out-of-band support for the OFI-based Chapel comm layer.
//

#include "chplrt.h"
#include "chpl-env-gen.h"

#include "chpl-comm.h"
#include "chpl-mem.h"
#include "chpl-mem-sys.h"
#include "chpl-gen-includes.h"
#include "chplsys.h"
#include "error.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "comm-ofi-internal.h"


#define PMI_SUCCESS 0
#define PMI2_ID_NULL -1

typedef int PMI_BOOL;
#define PMI_TRUE     1
#define PMI_FALSE    0

extern int PMI2_Initialized(void);
extern int PMI2_Init(int* spawned, int* size, int* rank, int* appnum);
extern int PMI2_Finalize(void);
extern int PMI_Barrier(void);
extern int PMI_Allgather(void*, void*, int);
extern int PMI_Bcast(void*, int);


#define PMI_CHK(expr) CHK_EQ_TYPED(expr, PMI_SUCCESS, int, "d")


void chpl_comm_ofi_oob_init(void) {
  int spawned, size, rank, appnum;

  if (PMI2_Initialized() != PMI_TRUE) {
    PMI_CHK(PMI2_Init(&spawned, &size, &rank, &appnum));
    assert(spawned == 0);
    chpl_nodeID = (c_nodeid_t) rank;
    chpl_numNodes = (int32_t) size;
  }

  DBG_PRINTF(DBG_OOB, "OOB init: node %" PRI_c_nodeid_t " of %" PRId32,
             chpl_nodeID, chpl_numNodes);
}


void chpl_comm_ofi_oob_fini(void) {
  if (PMI2_Initialized() == PMI_TRUE) {
    DBG_PRINTF(DBG_OOB, "OOB finalize");
    PMI_CHK(PMI2_Finalize());
  }
}


void chpl_comm_ofi_oob_barrier(void) {
  DBG_PRINTF(DBG_OOB, "OOB barrier");
  PMI_CHK(PMI_Barrier());
}


void chpl_comm_ofi_oob_allgather(const void* mine, void* all, size_t size) {
  DBG_PRINTF(DBG_OOB, "OOB allGather: %zd", size);

  //
  // PMI doesn't provide an ordered allGather, so we build one here
  // by concatenating the node index and the payload and using that
  // to scatter the unordered PMI_Allgather() results.
  //
  typedef struct {
    int nodeID;
    uint64_t info[];
  } gather_t;

  const size_t g_size = offsetof(gather_t, info) + size;
  gather_t* g_mine;
  CHK_SYS_CALLOC_SZ(g_mine, 1, g_size);
  g_mine->nodeID = chpl_nodeID;
  memcpy(&g_mine->info, mine, size);

  gather_t* g_all;
  CHK_SYS_CALLOC_SZ(g_all, chpl_numNodes, g_size);

  PMI_CHK(PMI_Allgather(g_mine, g_all, g_size));

  for (int g_i = 0; g_i < chpl_numNodes; g_i++) {
    char* g_pa = (char*) g_all + g_i * g_size;
    int i;
    memcpy(&i, g_pa + offsetof(gather_t, nodeID), sizeof(i));
    char* p_a = (char*) all + i * size;
    memcpy(p_a, g_pa + offsetof(gather_t, info), size);
  }

  sys_free(g_all);
  sys_free(g_mine);
}


void chpl_comm_ofi_oob_bcast(void* buf, size_t size) {
  DBG_PRINTF(DBG_OOB, "OOB bcast: %zd", size);
  PMI_CHK(PMI_Bcast(buf, size));
}
