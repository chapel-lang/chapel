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

#include "chplrt.h"
#include "chpl-privatization.h"
#include "chpl-mem.h"
#include "chpl-atomics.h"

static int64_t chpl_capPrivateObjects = 0;
static atomic_spinlock_t lock;

chpl_privateObject_t* chpl_privateObjects = NULL;

void chpl_privatization_init(void) {
  atomic_init_spinlock_t(&lock);
}

static inline int64_t max(int64_t a, int64_t b) {
  return a > b ? a : b;
}

// Note that this function can be called in parallel and more notably it can be
// called with non-monotonic pid's. e.g. this may be called with pid 27, and
// then pid 2, so it has to ensure that the privatized array has at least pid+1
// elements. Be __very__ careful if you have to update it.
void chpl_newPrivatizedClass(void* v, int64_t pid) {
  atomic_lock_spinlock_t(&lock);

  // initialize array to a default size
  if (chpl_privateObjects == NULL) {
    chpl_capPrivateObjects = 2*max(pid, 4);
    chpl_privateObjects =
        chpl_mem_allocManyZero(chpl_capPrivateObjects,
                               sizeof(chpl_privateObject_t),
                               CHPL_RT_MD_COMM_PRV_OBJ_ARRAY, 0, 0);
  } else {
    // if we're out of space, double (or more) the array size
    if (pid >= chpl_capPrivateObjects) {
      chpl_privateObject_t* tmp;
      int64_t oldCap;

      oldCap = chpl_capPrivateObjects;
      chpl_capPrivateObjects = 2*max(pid, oldCap);

      tmp = chpl_mem_allocManyZero(chpl_capPrivateObjects,
                                   sizeof(chpl_privateObject_t),
                                   CHPL_RT_MD_COMM_PRV_OBJ_ARRAY, 0, 0);
      chpl_memcpy((void*)tmp, (void*)chpl_privateObjects,
                  (oldCap)*sizeof(chpl_privateObject_t));
      chpl_privateObjects = tmp;
      // purposely leak old copies of chpl_privateObject to avoid the need to
      // lock chpl_getPrivatizedClass; TODO: fix with lock free data structure
    }
  }
  chpl_privateObjects[pid].obj = v;

  atomic_unlock_spinlock_t(&lock);
}

void chpl_clearPrivatizedClass(int64_t i) {
  atomic_lock_spinlock_t(&lock);
  chpl_privateObjects[i].obj = NULL;
  atomic_unlock_spinlock_t(&lock);
}

// Used to check for leaks of privatized classes
int64_t chpl_numPrivatizedClasses(void) {
  int64_t ret = 0;
  atomic_lock_spinlock_t(&lock);
  for (int64_t i = 0; i < chpl_capPrivateObjects; i++) {
    if (chpl_privateObjects[i].obj)
      ret++;
  }
  atomic_unlock_spinlock_t(&lock);
  return ret;
}
