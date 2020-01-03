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
extern int PMI2_KVS_Put(const char key[], const char value[]);
extern int PMI2_KVS_Fence(void);
extern int PMI2_KVS_Get(const char* jobid, int src_pmi_id,
                        const char key[], char value[],
                        int maxvalue, int* vallen);


#define PMI2_CHK(expr) CHK_EQ_TYPED(expr, PMI2_SUCCESS, int, "d")


void chpl_comm_ofi_oob_init(void) {
  int spawned, size, rank, appnum;

  if (!PMI2_Initialized()) {
    PMI2_CHK(PMI2_Init(&spawned, &size, &rank, &appnum));
    assert(spawned == 0);
    chpl_nodeID = (c_nodeid_t) rank;
    chpl_numNodes = (int32_t) size;
  }

  DBG_PRINTF(DBG_OOB, "OOB init: node %" PRI_c_nodeid_t " of %" PRId32,
             chpl_nodeID, chpl_numNodes);
}


void chpl_comm_ofi_oob_fini(void) {
  if (PMI2_Initialized()) {
    DBG_PRINTF(DBG_OOB, "OOB finalize");
    PMI2_CHK(PMI2_Finalize());
  }
}


void chpl_comm_ofi_oob_barrier(void) {
  DBG_PRINTF(DBG_OOB, "OOB barrier");
  PMI2_CHK(PMI2_KVS_Fence());
}


static void allgather_kvs(const char* mine, char* all, size_t size,
                          size_t chunk_off, size_t chunk_size);

void chpl_comm_ofi_oob_allgather(const void* mine_v, void* all_v,
                                 size_t size) {
  DBG_PRINTF(DBG_OOB, "OOB allgather: %zd", size);

  const char* mine = (const char*) mine_v;
  char* all = (char*) all_v;

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


static inline void encode_kvs(char* enc, const char* raw, size_t size);
static inline void decode_kvs(char* raw, const char* enc, size_t size);

static
void allgather_kvs(const char* mine, char* all, size_t size,
                   size_t chunk_off, size_t chunk_size) {
  //
  // Key values need to be NUL-terminated printable strings.
  //
  size_t chunk_size_enc = 2 * chunk_size + 1;

  char* enc;
  CHK_SYS_MALLOC_SZ(enc, 1, chunk_size_enc);
  encode_kvs(enc, mine + chunk_off, chunk_size);

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
    decode_kvs(all + node * size + chunk_off, enc, chunk_size);
  }

  sys_free(enc);
}


void chpl_comm_ofi_oob_bcast(void* buf_v, size_t size) {
  DBG_PRINTF(DBG_OOB, "OOB bcast: %zd", size);

  char* buf = (char*) buf_v;

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
    encode_kvs(enc, buf, size);
    DBG_PRINTF(DBG_OOB, "PMI2_KVS_Put(\"%s\", \"%s\")", key, enc);
    PMI2_CHK(PMI2_KVS_Put(key, enc));
  }

  PMI2_CHK(PMI2_KVS_Fence());

  if (chpl_nodeID != 0) {
    int size_ret;
    PMI2_CHK(PMI2_KVS_Get(NULL, PMI2_ID_NULL, key, enc, size_enc, &size_ret));
    CHK_TRUE(((size_t) size_ret) == size_enc - 1);
    decode_kvs(buf, enc, size);
  }

  sys_free(enc);
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
