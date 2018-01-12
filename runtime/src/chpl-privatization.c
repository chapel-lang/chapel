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
#include "chpl-privatization.h"
#include "chpl-atomics.h"
#include "chpl-mem.h"
#include "chpl-tasks.h"

#define CHPL_PRIVATIZATION_BLOCK_SIZE 1024

static chpl_sync_aux_t writeLock;


// Each block is an array of privatized objects
typedef void ** chpl_priv_block_t;

// Each instance is a variable array of blocks
typedef struct {
  chpl_priv_block_t * blocks;
  size_t len;
} chpl_priv_instance_t;

// Manage 2 instances to switch between
chpl_priv_instance_t chpl_priv_instances[2];

// Reader Count for both instances. (MUST BE ATOMIC)
atomic_uint_least32_t reader_count[2];

// Determines current instance. (MUST BE ATOMIC)
atomic_int_least8_t currentInstanceIdx;

chpl_priv_block_t chpl_priv_block_create() {
  return chpl_mem_allocManyZero(CHPL_PRIVATIZATION_BLOCK_SIZE, sizeof(void *),
                           CHPL_RT_MD_COMM_PRV_OBJ_ARRAY, 0, 0);
}

static int acquireRead(void);
static void releaseRead(int rcIdx);
static void acquireWrite(void);
static void releaseWrite(void);
static int getCurrentInstanceIdx(void);


static int getCurrentInstanceIdx(void) {
  return atomic_load_int_least8_t(&currentInstanceIdx);
}

static int acquireRead(void) {
  int instIdx = atomic_load_int_least8_t(&currentInstanceIdx);
  atomic_fetch_add_uint_least32_t(&reader_count[instIdx], 1);

  // Check if instance changed between calls.
  if (instIdx != atomic_load_int_least8_t(&currentInstanceIdx)) {
    atomic_fetch_sub_uint_least32_t(&reader_count[instIdx], 1);
    
    // Slow-path
    while (true) {
      instIdx = atomic_load_int_least8_t(&currentInstanceIdx);
      atomic_fetch_add_uint_least32_t(&reader_count[instIdx], 1);      

      if (instIdx == atomic_load_int_least8_t(&currentInstanceIdx)) {
        break;
      }

      atomic_fetch_sub_uint_least32_t(&reader_count[instIdx], 1);
    }
  }
  return instIdx;
}

static void releaseRead(int rcIdx) {
  atomic_fetch_sub_uint_least32_t(&reader_count[rcIdx], 1);
}



static void acquireWrite(void) {
  chpl_sync_lock(&writeLock);
}


static void releaseWrite(void) {
  chpl_sync_unlock(&writeLock);
}

void chpl_privatization_init(void) {
    chpl_sync_initAux(&writeLock);
    
    // Init reader counts
    reader_count[0] = 0;
    reader_count[1] = 0;

    // Initialize first instance with a single block.
    chpl_priv_instances[0].blocks = chpl_mem_allocManyZero(1, sizeof(chpl_priv_block_t),
                           CHPL_RT_MD_COMM_PRV_OBJ_ARRAY, 0, 0);
    chpl_priv_instances[0].blocks[0] = chpl_priv_block_create();
    chpl_priv_instances[0].len = 1;
}

static inline int64_t max(int64_t a, int64_t b) {
  return a > b ? a : b;
}

// Note that this function can be called in parallel and more notably it can be
// called with non-monotonic pid's. e.g. this may be called with pid 27, and
// then pid 2, so it has to ensure that the privatized array has at least pid+1
// elements. Be __very__ careful if you have to update it.
void chpl_newPrivatizedClass(void* v, int64_t pid) {
  int blockIdx = pid / CHPL_PRIVATIZATION_BLOCK_SIZE;
  while (true) {
    int rcIdx = acquireRead();

    // If slot is not allocated yet...
    if (blockIdx >= chpl_priv_instances[rcIdx].len) {
      // Become writer...
      releaseRead(rcIdx);
      acquireWrite();

      // Check amount of blocks needing allocation...
      int instIdx = getCurrentInstanceIdx();
      int deltaBlocks = (blockIdx + 1) - chpl_priv_instances[instIdx].len;
      
      // Another writer has resized for us...
      if (deltaBlocks <= 0) {
        releaseWrite();
        continue;
      }

      // Switch instances
      int newInstIdx = !getCurrentInstanceIdx();
      // Allocate new space
      chpl_priv_block_t *newBlocks = chpl_mem_allocManyZero(blockIdx + 1, sizeof(*newBlocks),
                                   CHPL_RT_MD_COMM_PRV_OBJ_ARRAY, 0, 0);
      // Move old data to new space
      chpl_memcpy((void *) newBlocks, (void *) chpl_priv_instances[instIdx].blocks, 
                                   chpl_priv_instances[instIdx].len * sizeof(void*));
      // Fill rest of new space
      for (int i = chpl_priv_instances[instIdx].len; i <= blockIdx; i++) {
        newBlocks[i] = chpl_priv_block_create();
      }
      // Set new data and instance
      chpl_priv_instances[newInstIdx].blocks = newBlocks;
      chpl_priv_instances[newInstIdx].len = blockIdx + 1;
      atomic_store_int_least8_t(&currentInstanceIdx, newInstIdx);

      // Wait for readers to finish with old.
      int readers = 0;
      while ((readers = atomic_load_uint_least32_t(&reader_count[instIdx])) > 0) {
        chpl_task_yield();
      }

      // Delete old instance data...
      chpl_mem_free(chpl_priv_instances[instIdx].blocks, 0, 0);
      releaseWrite();
    } else {
      int idx = pid % CHPL_PRIVATIZATION_BLOCK_SIZE;
      chpl_priv_instances[rcIdx].blocks[blockIdx][idx] = v;
      releaseRead(rcIdx);
      return;
    }
  }
}

void* chpl_getPrivatizedClass(int64_t i) {
  void *ret = NULL;
  int rcIdx = acquireRead();
  int blockIdx = i / CHPL_PRIVATIZATION_BLOCK_SIZE;
  int idx = i % CHPL_PRIVATIZATION_BLOCK_SIZE;
  ret = chpl_priv_instances[rcIdx].blocks[blockIdx][idx];
  releaseRead(rcIdx);
  return ret;
}

void chpl_clearPrivatizedClass(int64_t i) {
  int rcIdx = acquireRead();
  int blockIdx = i / CHPL_PRIVATIZATION_BLOCK_SIZE;
  int idx = i % CHPL_PRIVATIZATION_BLOCK_SIZE;
  chpl_priv_instances[rcIdx].blocks[blockIdx][idx] = NULL;
  releaseRead(rcIdx);
}

// Used to check for leaks of privatized classes
int64_t chpl_numPrivatizedClasses(void) {
  int rcIdx = acquireRead();
  int64_t ret = chpl_priv_instances[rcIdx].len * CHPL_PRIVATIZATION_BLOCK_SIZE;
  releaseRead(rcIdx);
  return ret;
}
 