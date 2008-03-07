#ifndef _CHPL_SYS_H_
#define _CHPL_SYS_H_

#include <stdint.h>

uint64_t _bytesPerLocale(void);
int32_t _coresPerLocale(void);

//
// returns default values of max threads set according to the
// communication and threading runtimes; if neither set a max, then 0
// is returned; otherwise the minimum max is returned.
//
int32_t _maxThreads(void);

//
// returns upper limit of values of max threads set according to the
// communication and threading runtimes; if neither set a limit, then 0
// is returned; otherwise the minimum limit is returned.
//
int32_t _maxThreadsLimit(void);

#endif
