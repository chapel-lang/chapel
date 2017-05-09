/*
 * Copyright 2004-2017 Cray Inc.
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

#ifndef _chpl_privatization_h_
#define _chpl_privatization_h_
#ifndef LAUNCHER
#include <stdint.h>

#include "chplrt.h"

#include "chpltypes.h"
#include "chpl-mem.h"
#include "chpl-tasks.h"

//
// Runtime support for storing/getting privatized objects. Implementation is
// done inside a header for performance: getPrivatizedClass can be called
// frequently, so putting it in a header allows the backend to fully optimize.
//

static int64_t chpl_capPrivateObjects = 0;
static chpl_sync_aux_t privatizationSync;
static void** chpl_privateObjects = NULL;

static void chpl_privatization_init(void) {
  chpl_sync_initAux(&privatizationSync);
}

static inline int64_t max(int64_t a, int64_t b) {
  return a > b ? a : b;
}

// Note that this function can be called in parallel and more notably it can be
// called with non-monotonic pid's. e.g. this may be called with pid 27, and
// then pid 2, so it has to ensure that the privatized array has at least pid+1
// elements. Be __very__ careful if you have to update it.
static inline void chpl_newPrivatizedClass(void* v, int64_t pid) {
  chpl_sync_lock(&privatizationSync);

  // initialize array to a default size
  if (chpl_privateObjects == NULL) {
    chpl_capPrivateObjects = 2*max(pid, 4);
    chpl_privateObjects =
        chpl_mem_allocManyZero(chpl_capPrivateObjects, sizeof(void *),
                           CHPL_RT_MD_COMM_PRV_OBJ_ARRAY, 0, 0);
  } else {
    // if we're out of space, double (or more) the array size
    if (pid >= chpl_capPrivateObjects) {
      void** tmp;
      int64_t oldCap;

      oldCap = chpl_capPrivateObjects;
      chpl_capPrivateObjects = 2*max(pid, oldCap);

      tmp = chpl_mem_allocManyZero(chpl_capPrivateObjects, sizeof(void *),
                                   CHPL_RT_MD_COMM_PRV_OBJ_ARRAY, 0, 0);
      chpl_memcpy((void*)tmp, (void*)chpl_privateObjects, (oldCap)*sizeof(void*));
      chpl_privateObjects = tmp;
      // purposely leak old copies of chpl_privateObject to avoid the need to
      // lock chpl_getPrivatizedClass; TODO: fix with lock free data structure
    }
  }
  chpl_privateObjects[pid] = v;

  chpl_sync_unlock(&privatizationSync);
}

static inline void* chpl_getPrivatizedClass(int64_t i) {
  return chpl_privateObjects[i];
}

static inline void chpl_clearPrivatizedClass(int64_t i) {
  chpl_sync_lock(&privatizationSync);
  chpl_privateObjects[i] = NULL;
  chpl_sync_unlock(&privatizationSync);
}

// Used to check for leaks of privatized classes
static inline int64_t chpl_numPrivatizedClasses(void) {
  int64_t ret = 0;
  chpl_sync_lock(&privatizationSync);
  for (int64_t i = 0; i < chpl_capPrivateObjects; i++) {
    if (chpl_privateObjects[i])
      ret++;
  }
  chpl_sync_unlock(&privatizationSync);
  return ret;
}

#endif // LAUNCHER
#endif // _chpl_privatization_h_
