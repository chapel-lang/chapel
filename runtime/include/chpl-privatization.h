#ifndef _chpl_privatization_h_
#define _chpl_privatization_h_
#ifndef LAUNCHER
#include <stdint.h>
#include "chpltypes.h"

extern void** chpl_privateObjects; // privatized array and domain objects

extern void chpl_privatization_init(void);

extern void chpl_newPrivatizedClass(void*, int64_t);
extern void* chpl_getPrivatizedClass(int64_t);

#endif // LAUNCHER
#endif // _chpl_privatization_h_
