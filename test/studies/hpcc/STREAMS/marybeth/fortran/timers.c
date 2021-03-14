#include <float.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>

// copied from Chapel runtime
static double _now_time(void) {
  struct tm * now;
  struct timezone tz;
  struct timeval t;
  gettimeofday(&t, &tz);
  now = localtime(&t.tv_sec);
  return (double)(now->tm_hour)*3600.0e+6 +
    (double)(now->tm_min)*60.0e+6 +
    (double)(now->tm_sec)*1.0e+6 +
    (double)(t.tv_usec);
}

static double getcurrenttime_(void) {
  return _now_time() / 1.0e+6;
}

void curtime_(double *timeVal) {
  *timeVal = getcurrenttime_();
}

