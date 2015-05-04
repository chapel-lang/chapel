/*
 * Copyright 2004-2015 Cray Inc.
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
#include "chpl-tasks.h"

static int64_t chpl_capPrivateObjects = 0;
static chpl_sync_aux_t privatizationSync;

void** chpl_privateObjects;

void chpl_privatization_init(void) {
    chpl_sync_initAux(&privatizationSync);
}

void chpl_newPrivatizedClass(void* v, int64_t pid) {
  // We need to lock around this operation so two calls in rapid succession
  // that pass the chpl_capPrivateObjects limit don't both try to create a new
  // array. If they do, one of the calls will be leaked and an invalid pointer
  // to be placed in the table.
  chpl_sync_lock(&privatizationSync);

  pid += 1;
  if (pid == 1) {
    chpl_capPrivateObjects = 8;
    // "private" means "node-private", so we can use the system allocator.
    chpl_privateObjects = chpl_mem_allocMany(chpl_capPrivateObjects, sizeof(void*), CHPL_RT_MD_COMM_PRV_OBJ_ARRAY, 0, "");
  } else {
    if (pid > chpl_capPrivateObjects) {
      void** tmp;
      chpl_capPrivateObjects *= 2;
      tmp = chpl_mem_allocMany(chpl_capPrivateObjects, sizeof(void*), CHPL_RT_MD_COMM_PRV_OBJ_ARRAY, 0, "");
      chpl_memcpy((void*)tmp, (void*)chpl_privateObjects, (pid-1)*sizeof(void*));
      chpl_privateObjects = tmp;
      // purposely leak old copies of chpl_privateObject to avoid the need to
      // lock chpl_getPrivatizedClass; TODO: fix with lock free data structure
    }
  }
  chpl_privateObjects[pid-1] = v;

  chpl_sync_unlock(&privatizationSync);
}


extern void* chpl_getPrivatizedClass(int64_t i) {
  return chpl_privateObjects[i];
}


