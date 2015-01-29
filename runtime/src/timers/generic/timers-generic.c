#include "chplrt.h"

#include "chpltimers.h"

#include <sys/time.h>
#include <time.h>

_real64 chpl_now_time(void) {
  struct tm * now;
  _timevalue t;
  gettimeofday(&t, NULL);
  now = localtime(&t.tv_sec);
  return (_real64)(now->tm_hour)*3600.0e+6 +
    (_real64)(now->tm_min)*60.0e+6 +
    (_real64)(now->tm_sec)*1.0e+6 +
    (_real64)(t.tv_usec);
}
