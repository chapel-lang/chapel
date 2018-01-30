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
#include "chpl-thread-local-storage.h"
#include "chpl-privatization.h"
#include "chpl-atomics.h"
#include "chpl-mem.h"
#include "chpl-tasks.h"
#include "chpl-qsbr.h"
#include <pthread.h>
#include <stdbool.h>

/*
  TODO: Make calls to check for TLS 'unlikely' as it only happens once.
*/

static chpl_sync_aux_t privatizationSync;

void** chpl_privateObjects = NULL;
static int64_t chpl_capPrivateObjects = 0;

void chpl_privatization_init(void) {
    chpl_sync_initAux(&privatizationSync);
    
    // Initialize...
    chpl_capPrivateObjects = 1024;
    chpl_privateObjects =
        chpl_mem_allocManyZero(chpl_capPrivateObjects, sizeof(void *),
                           CHPL_RT_MD_COMM_PRV_OBJ_ARRAY, 0, 0);
}

static inline int64_t max(int64_t a, int64_t b) {
  return a > b ? a : b;
}

// Note that this function can be called in parallel and more notably it can be
// called with non-monotonic pid's. e.g. this may be called with pid 27, and
// then pid 2, so it has to ensure that the privatized array has at least pid+1
// elements. Be __very__ careful if you have to update it.
void chpl_newPrivatizedClass(void* v, int64_t pid) {  
  chpl_sync_lock(&privatizationSync);
  // if we're out of space, double (or more) the array size
  if (pid >= chpl_capPrivateObjects) {
    void** tmp;
    void** old;
    int64_t oldCap;

    oldCap = chpl_capPrivateObjects;
    chpl_capPrivateObjects = 1.5*max(pid, oldCap);

    tmp = chpl_mem_allocManyZero(chpl_capPrivateObjects, sizeof(void *),
                                 CHPL_RT_MD_COMM_PRV_OBJ_ARRAY, 0, 0);
    chpl_memcpy((void*)tmp, (void*)chpl_privateObjects, (oldCap)*sizeof(void*));
    old = chpl_privateObjects;
    chpl_privateObjects = tmp;

    void **storage = chpl_mem_allocManyZero(1, sizeof(void *), 0, 0, 0);
    storage[0] = old;
    chpl_qsbr_defer_deletion(storage, 1, true);
  }

  chpl_privateObjects[pid] = v;

  chpl_sync_unlock(&privatizationSync);
}

void chpl_clearPrivatizedClass(int64_t i) {
  chpl_sync_lock(&privatizationSync);
  chpl_privateObjects[i] = NULL;
  chpl_sync_unlock(&privatizationSync);
}

// Used to check for leaks of privatized classes
int64_t chpl_numPrivatizedClasses(void) {
  int64_t ret = 0;
  chpl_sync_lock(&privatizationSync);
  for (int64_t i = 0; i < chpl_capPrivateObjects; i++) {
    if (chpl_privateObjects[i])
      ret++;
  }
  chpl_sync_unlock(&privatizationSync);
  return ret;
}
 
// Clean up all resources for memory checkers.
void chpl_privatization_exit(void) {
  // Destroy last instance
  chpl_mem_free(chpl_privateObjects, 0, 0);
  chpl_privateObjects = NULL;
}