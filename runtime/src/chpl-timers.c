// chpl-timers.c
//
#include "chplrt.h"

#include "chpltimers.h"

#include <time.h>   // For struct tm.

_timevalue chpl_null_timevalue(void) {
  _timevalue ret;
  ret.tv_sec = 0;
  ret.tv_usec = 0;
  return ret;
}

_timevalue chpl_now_timevalue(void) {
  _timevalue ret;
  gettimeofday(&ret, NULL);
  return ret;
}

int64_t chpl_timevalue_seconds(_timevalue t) { return t.tv_sec; }
int64_t chpl_timevalue_microseconds(_timevalue t) { return t.tv_usec; }

void chpl_timevalue_parts(_timevalue t, int32_t* seconds, int32_t* minutes, int32_t* hours, int32_t* mday, int32_t* month, int32_t* year, int32_t* wday, int32_t* yday, int32_t* isdst)
{
  struct tm localt;
  localtime_r(&t.tv_sec, &localt);
  if( seconds ) *seconds = localt.tm_sec;
  if( minutes ) *minutes = localt.tm_min;
  if( hours ) *hours = localt.tm_hour;
  if( mday ) *mday = localt.tm_mday;
  if( month ) *month = localt.tm_mon;
  if( year ) *year = localt.tm_year;
  if( wday ) *wday = localt.tm_wday;
  if( yday ) *yday = localt.tm_yday;
  if( isdst ) *isdst = localt.tm_isdst;
}
