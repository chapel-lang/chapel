/*
 * Copyright 2020-2021 Hewlett Packard Enterprise Development LP
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

//
// PMI2-based out-of-band support for the OFI-based Chapel comm layer.
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


#define PMI2_SUCCESS 0
#define PMI2_ID_NULL -1
#define PMI2_MAX_VALLEN 1024

extern int PMI2_Initialized(void);
extern int PMI2_Init(int* spawned, int* size, int* rank, int* appnum);
extern int PMI2_Finalize(void);

extern int PMI2_KVS_Put(const char key[], const char value[])
           __attribute__((weak));
extern int PMI2_KVS_Fence(void)
           __attribute__((weak));
extern int PMI2_KVS_Get(const char* jobid, int src_pmi_id,
                        const char key[], char value[],
                        int maxvalue, int* vallen)
           __attribute__((weak));

#define PMI2_CHK(expr) CHK_EQ_TYPED(expr, PMI2_SUCCESS, int, "d")


//
// We prefer PMI_Barrier(), PMI_Allgather(), and PMI_Bcast() to the
// KVS interface, when both are available.  This is often the case
// on HPE Cray EX systems, for example.
//
#define PMI_SUCCESS 0

typedef int PMI_BOOL;
#define PMI_TRUE     1
#define PMI_FALSE    0

extern int PMI_Barrier(void)
           __attribute__((weak));
extern int PMI_Allgather(void*, void*, int)
           __attribute__((weak));
extern int PMI_Bcast(void*, int)
           __attribute__((weak));

#define PMI_CHK(expr) CHK_EQ_TYPED(expr, PMI_SUCCESS, int, "d")


void chpl_comm_ofi_oob_init(void) {
  //
  // If we don't have all or none of PMI_{Allgather,Barrier,Bcast) and
  // all or none of PMI2_KVS_{Fence,Get,Put}, or we're missing both,
  // something strange is going on.
  //
  CHK_TRUE((PMI_Allgather == NULL) == (PMI_Bcast == NULL)
           && ((PMI2_KVS_Fence == NULL) == (PMI2_KVS_Get == NULL)
               && (PMI2_KVS_Get == NULL) == (PMI2_KVS_Put == NULL))
           && (PMI_Allgather != NULL || PMI2_KVS_Fence != NULL));

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
  if (PMI_Barrier != NULL) {
    DBG_PRINTF(DBG_OOB, "OOB barrier via PMI");
    PMI_CHK(PMI_Barrier());
  } else {
    DBG_PRINTF(DBG_OOB, "OOB barrier via KVS");
    PMI2_CHK(PMI2_KVS_Fence());
  }
}


static void allgather_kvs(const void* mine, void* all, size_t size,
                          size_t chunk_off, size_t chunk_size);

void chpl_comm_ofi_oob_allgather(const void* mine, void* all, size_t size) {
  if (PMI_Allgather != NULL) {
    DBG_PRINTF(DBG_OOB, "OOB allgather via PMI: %zd", size);

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
  } else {
    // Fall back to KVS-based allgather.
    DBG_PRINTF(DBG_OOB, "OOB allgather via KVS: %zd", size);

    //
    // There is a limit on the maximum size of a KVS value, so we may
    // need to do this in multiple chunks.
    //
    size_t size_remaining = size;
    size_t chunk_size_max = (PMI2_MAX_VALLEN - 1) / 2; // unencoded max chunk
    while (size_remaining > chunk_size_max) {
      allgather_kvs(mine, all, size, size - size_remaining, chunk_size_max);
      size_remaining -= chunk_size_max;
    }

    allgather_kvs(mine, all, size, size - size_remaining, size_remaining);
  }
}


static inline void encode_kvs(char* enc, const char* raw, size_t size);
static inline void decode_kvs(char* raw, const char* enc, size_t size);

static
void allgather_kvs(const void* mine, void* all, size_t size,
                   size_t chunk_off, size_t chunk_size) {
  //
  // Key values need to be NUL-terminated printable strings.
  //
  size_t chunk_size_enc = 2 * chunk_size + 1;

  char* enc;
  CHK_SYS_MALLOC_SZ(enc, 1, chunk_size_enc);
  encode_kvs(enc, (const char*) mine + chunk_off, chunk_size);

  char key[64];
  const char* key_fmt = "ChplAllgthr%d_N%d";
  static int key_cntr;

  key_cntr++;
  CHK_TRUE(snprintf(key, sizeof(key), key_fmt, key_cntr, chpl_nodeID)
           < sizeof(key));

  DBG_PRINTF(DBG_OOB, "PMI2_KVS_Put(\"%s\", \"%s\")", key, enc);
  PMI2_CHK(PMI2_KVS_Put(key, enc));

  PMI2_CHK(PMI2_KVS_Fence());

  for (int node = 0; node < chpl_numNodes; node++) {
    CHK_TRUE(snprintf(key, sizeof(key), key_fmt, key_cntr, node)
             < sizeof(key));
    int chunk_size_ret;
    PMI2_CHK(PMI2_KVS_Get(NULL, PMI2_ID_NULL, key, enc,
                          chunk_size_enc, &chunk_size_ret));
    CHK_TRUE(((size_t) chunk_size_ret) == chunk_size_enc - 1);
    decode_kvs((char*) all + node * size + chunk_off, enc, chunk_size);
  }

  sys_free(enc);
}


void chpl_comm_ofi_oob_bcast(void* buf, size_t size) {
  if (PMI_Bcast != NULL) {
    DBG_PRINTF(DBG_OOB, "OOB bcast via PMI: %zd", size);
    PMI_CHK(PMI_Bcast(buf, size));
  } else {
    // Fall back to KVS-based broadcast.
    DBG_PRINTF(DBG_OOB, "OOB bcast via KVS: %zd", size);

    //
    // Key values need to be NUL-terminated printable strings.
    //
    size_t size_enc = 2 * size + 1;
    char* enc;
    CHK_SYS_MALLOC_SZ(enc, 1, size_enc);

    char key[64];
    const char* key_fmt = "ChplBcast%d";
    static int key_cntr;
    key_cntr++;
    CHK_TRUE(snprintf(key, sizeof(key), key_fmt, key_cntr, chpl_nodeID)
             < sizeof(key));

    if (chpl_nodeID == 0) {
      encode_kvs(enc, (char*) buf, size);
      DBG_PRINTF(DBG_OOB, "PMI2_KVS_Put(\"%s\", \"%s\")", key, enc);
      PMI2_CHK(PMI2_KVS_Put(key, enc));
    }

    PMI2_CHK(PMI2_KVS_Fence());

    if (chpl_nodeID != 0) {
      int size_ret;
      PMI2_CHK(PMI2_KVS_Get(NULL, PMI2_ID_NULL, key, enc, size_enc,
                            &size_ret));
      CHK_TRUE(((size_t) size_ret) == size_enc - 1);
      decode_kvs((char*) buf, enc, size);
    }

    sys_free(enc);
  }
}


static inline
void encode_kvs(char* enc, const char* raw, size_t size) {
  for (size_t i = 0; i < size; i++) {
    enc[2 * i + 0] = ((raw[i] >> (0 * 4)) & 0xf) + 'a';
    enc[2 * i + 1] = ((raw[i] >> (1 * 4)) & 0xf) + 'a';
  }
  enc[2 * size] = '\0';
}


static inline
void decode_kvs(char* raw, const char* enc, size_t size) {
  for (size_t i = 0; i < size; i++) {
    raw[i] =   ((enc[2 * i + 0] - 'a') << (0 * 4))
             | ((enc[2 * i + 1] - 'a') << (1 * 4));
  }
}
