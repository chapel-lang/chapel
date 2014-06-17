#ifndef _CHPL_SYS_H_
#define _CHPL_SYS_H_

#include "chpltypes.h" // for chpl_string

#include <stdint.h>

uint64_t chpl_bytesPerLocale(void);
int64_t chpl_numCoresOnThisLocale(void);

//
// returns default values of max threads set according to the
// communication and threading runtimes; if neither set a max, then 0
// is returned; otherwise the minimum max is returned.
//
int32_t chpl_maxThreads(void);

//
// returns upper limit of values of max threads set according to the
// communication and threading runtimes; if neither set a limit, then 0
// is returned; otherwise the minimum limit is returned.
//
int32_t chpl_maxThreadsLimit(void);

//
// returns the name of a locale via uname -n or the like
//
chpl_string chpl_nodeName(void);

#endif
