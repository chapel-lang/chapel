#ifndef _CHPL_SYS_H_
#define _CHPL_SYS_H_

#include "chpltypes.h" // for c_string

#include <stdint.h>

uint64_t chpl_bytesPerLocale(void);
size_t chpl_bytesAvailOnThisLocale(void);
int chpl_getNumPhysicalCpus(chpl_bool accessible_only);
int chpl_getNumLogicalCpus(chpl_bool accessible_only);

//
// returns the name of a locale via uname -n or the like
//
c_string chpl_nodeName(void);

#endif
