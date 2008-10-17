//
// This file can hold shared code between the different comm
// implementation files.
//
#include "chplrt.h"
#include "chplcomm.h"
#include "chplmem.h"

#include <stdint.h>

int32_t _localeID = -1;
int32_t _numLocales = -1;
int32_t chpl_numPrivateObjects = 0;
void** chpl_privateObjects;

void chpl_setPrivateClass(void* v) {
  chpl_numPrivateObjects += 1;
  if (chpl_numPrivateObjects == 1) {
    chpl_privateObjects = chpl_malloc(chpl_numPrivateObjects, sizeof(void*), "private objects array", 0, "");
  } else {
    chpl_privateObjects = chpl_realloc(chpl_privateObjects, chpl_numPrivateObjects, sizeof(void*), "private objects array", 0, "");
  }
  chpl_privateObjects[chpl_numPrivateObjects-1] = v;
}

extern void* chpl_getPrivateClass(int32_t i) {
  return chpl_privateObjects[i];
}

