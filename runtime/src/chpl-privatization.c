#include "chplrt.h"
#include "chpl-privatization.h"
#include "chpl-mem.h"
#include "chpl-tasks.h"

static int32_t chpl_capPrivateObjects = 0;
static chpl_sync_aux_t privatizationSync;

void** chpl_privateObjects;

void chpl_privatization_init(void) {
    chpl_sync_initAux(&privatizationSync);
}

void chpl_newPrivatizedClass(void* v, int32_t pid) {
  // We need to lock around this operation so two calls in rapid succession
  // that pass the chpl_capPrivateObjects limit don't both try to create a new
  // array. If they do, one of the calls will be leaked and an invalid pointer
  // to be placed in the table.
  chpl_sync_lock(&privatizationSync);

  pid += 1;
  if (pid == 1) {
    chpl_capPrivateObjects = 8;
    // "private" means "node-private", so we can use the system allocator.
    chpl_privateObjects = chpl_mem_allocMany(chpl_capPrivateObjects, sizeof(void*), CHPL_RT_MD_COMM_PRIVATE_OBJECTS_ARRAY, 0, "");
  } else {
    if (pid > chpl_capPrivateObjects) {
      void** tmp;
      chpl_capPrivateObjects *= 2;
      tmp = chpl_mem_allocMany(chpl_capPrivateObjects, sizeof(void*), CHPL_RT_MD_COMM_PRIVATE_OBJECTS_ARRAY, 0, "");
      memcpy((void*)tmp, (void*)chpl_privateObjects, (pid-1)*sizeof(void*));
      chpl_privateObjects = tmp;
      // purposely leak old copies of chpl_privateObject to avoid the need to
      // lock chpl_getPrivatizedClass; TODO: fix with lock free data structure
    }
  }
  chpl_privateObjects[pid-1] = v;

  chpl_sync_unlock(&privatizationSync);
}


extern void* chpl_getPrivatizedClass(int32_t i) {
  return chpl_privateObjects[i];
}


