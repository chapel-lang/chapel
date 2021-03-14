#include <stdio.h>
#include <sys/time.h>
#include <time.h>
#include <stdlib.h>
#include "time_iterate.h"

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


static void assign(int *A, int n) {
  int i;

  for (i=0; i<n; i++) {
    A[i] = i;
  }
}

static const int size = 100000000;
static const int ntrials = 100;

void c_trial() {
  int* A;
  int i;
  double starttime, endtime;
  A = (int*)malloc(size*sizeof(int));
  if (A == NULL) {
    printf("malloc failed\n");
  }
  starttime = now_time();
  for (i=0; i<ntrials; i++) {
    assign(A, size);
  }
  endtime = now_time();
  printf("C    wrote %d elements %d times in %f seconds\n", size, ntrials, (endtime-starttime)/(1000*1000));
  free(A);
}
