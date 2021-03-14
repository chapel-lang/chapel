#include <stdio.h>
#include <sys/time.h>
#include <time.h>
#include <stdlib.h>
#include <stdint.h>
#include "forVersusWhilePerf.h"

static double now_time(void) {
  struct tm * now;
  struct timeval t;
  gettimeofday(&t, NULL);
  now = localtime(&t.tv_sec);
  return (double)(now->tm_hour)*3600.0e+6 +
    (double)(now->tm_min)*60.0e+6 +
    (double)(now->tm_sec)*1.0e+6 +
    (double)(t.tv_usec);
}

int64_t numIters = 1000000;
int64_t ntrials = 10000;

int64_t for_loop()
{
  int64_t result = 0;
  int64_t i, j;
  for (i = 0; i < ntrials; i++) {
    for (j = 0; j < numIters; j++) {
      if (j % 2 == 1) {
        result = result + 3;
      } else {
        result = result / 2;
      }
    }
  }
  return result;
}

int64_t while_loop()
{
  int64_t result2 = 0;
  int64_t i = 0, j;
  while (i < ntrials) {
    j = 0;
    while (j < numIters) {
      if (j % 2 == 1) {
        result2 = result2 + 3;
      } else {
        result2 = result2 / 2;
      }
      j++;
    }
    i++;
  }
  return result2;
}

int64_t c_trial() {
  double starttimeFor, endtimeFor;
  double starttimeWhile, endtimeWhile;
  starttimeFor = now_time();
  int64_t result = for_loop();
  endtimeFor = now_time();
  starttimeWhile = now_time();
  int64_t result2 = while_loop();
  endtimeWhile = now_time();

  if (result != result2) {
    printf("These results should have matched, got %ld and %ld\n", (long int)result, (long int)result2);
  } else {
    printf("C verification successful\n");
  }
  printf("C for loop took %f seconds for %ld iterations %ld times\n", (endtimeFor-starttimeFor)/(1000*1000), (long int)numIters, (long int)ntrials);
  printf("C while loop took %f seconds for %ld iterations %ld times\n", (endtimeWhile-starttimeWhile)/(1000*1000), (long int)numIters, (long int)ntrials);

  return result;
}
