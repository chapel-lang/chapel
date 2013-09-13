//
// Shared code for different comm implementations in
//  comm/<commlayer>/comm-<commlayer>.c
//
#include "chplrt.h"
#include "chpl-comm.h"
#include "chpl-mem.h"

#include <stdint.h>
#include <string.h>
#include <unistd.h>

int32_t chpl_nodeID = -1;
int32_t chpl_numNodes = -1;
int32_t chpl_numPrivateObjects = 0;
static int32_t chpl_capPrivateObjects = 0;
void** chpl_privateObjects;

int chpl_verbose_comm;
int chpl_comm_diagnostics;
int chpl_verbose_mem;

void chpl_newPrivatizedClass(void* v) {
  chpl_numPrivateObjects += 1;
  if (chpl_numPrivateObjects == 1) {
    chpl_capPrivateObjects = 8;
    // "private" means "node-private", so we can use the system allocator.
    chpl_privateObjects = chpl_mem_allocMany(chpl_capPrivateObjects, sizeof(void*), CHPL_RT_MD_COMM_PRIVATE_OBJECTS_ARRAY, 0, "");
  } else {
    if (chpl_numPrivateObjects > chpl_capPrivateObjects) {
      void** tmp;
      chpl_capPrivateObjects *= 2;
      tmp = chpl_mem_allocMany(chpl_capPrivateObjects, sizeof(void*), CHPL_RT_MD_COMM_PRIVATE_OBJECTS_ARRAY, 0, "");
      memcpy((void*)tmp, (void*)chpl_privateObjects, (chpl_numPrivateObjects-1)*sizeof(void*));
      chpl_privateObjects = tmp;
      // purposely leak old copies of chpl_privateObject to avoid the need to
      // lock chpl_getPrivatizedClass; to do: fix with lock free data structure
    }
  }
  chpl_privateObjects[chpl_numPrivateObjects-1] = v;
}

extern void* chpl_getPrivatizedClass(int32_t i) {
  return chpl_privateObjects[i];
}


