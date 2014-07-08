#include <stdio.h>
#include <sys/time.h>
#include <time.h>
#include <stdlib.h>
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

static const int numIters = 1000000;
static const int ntrials = 10000;

int c_trial() {
  int i, j;
  double starttimeFor, endtimeFor;
  double starttimeWhile, endtimeWhile;
  int result = 0;
  int result2 = 0;
  starttimeFor = now_time();
  for (i = 0; i < ntrials; i++) {
    for (j = 0; j < numIters; j++) {
      if (j % 2 == 1) {
        result = result + 3;
      } else {
        result = result / 2;
      }
    }
  }
  endtimeFor = now_time();
  starttimeWhile = now_time();
  i = 0;
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
  endtimeWhile = now_time();

  if (result != result2) {
    printf("These results should have matched, got %d and %d\n", result, result2);
  } else {
    printf("C verification successful\n");
  }
  printf("C for loop took %f seconds for %d iterations %d times\n", (endtimeFor-starttimeFor)/(1000*1000), numIters, ntrials);
  printf("C while loop took %f seconds for %d iterations %d times\n", (endtimeWhile-starttimeWhile)/(1000*1000), numIters, ntrials);

  return result;
}
