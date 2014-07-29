#ifndef _chpltimers_h_
#define _chpltimers_h_

#include "chpltypes.h"  // For _real64.

#include <sys/time.h>   // For struct timeval.

typedef struct timeval _timevalue;

_timevalue chpl_null_timevalue(void);
_timevalue chpl_now_timevalue(void);
int64_t chpl_timevalue_seconds(_timevalue t);
int64_t chpl_timevalue_microseconds(_timevalue t);
void chpl_timevalue_parts(_timevalue t, int32_t* seconds, int32_t* minutes, int32_t* hours, int32_t* mday, int32_t* month, int32_t* year, int32_t* wday, int32_t* yday, int32_t* isdst);

#ifndef LAUNCHER

_real64 chpl_now_time(void);

#endif // LAUNCHER

#endif
