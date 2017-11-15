#include <stdio.h>
#include <sys/time.h>
#include <time.h>
#include <stdlib.h>
#include "time_access.h"

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


static void assign(int **A, int n) {
  int i;
  int j;

  for (i=0; i<n; i++) {
    A[i] = (int*)malloc(n*sizeof(int));
    if (A[i] == NULL) {
      printf("malloc failed\n");
    }
    for (j=0; j<n; j++) {
      A[i][j] = i+j;
    }
  }
}

static int touch(int **A, int n) {
  int i;
  int j;
  int k=0;

  for (i=0; i<n; i++) {
    for (j=0; j<n; j++) {
      k += A[i][j];
    }
  }
  return k;
}

static const int size = 1000;
static const int ntrials = 100;

void c_trial() {
  int** A;
  int i;
  double starttime, endtime;
  int worked = 1;
  A = (int**)malloc(size*sizeof(int*));
  if (A == NULL) {
    printf("malloc failed\n");
  }
  assign(A, size);
  starttime = now_time();
  for (i=0; i<ntrials; i++) {
    if (touch(A, size) != 999000000) {
      worked = 0;
    }
  }
  endtime = now_time();
  if (!worked) {
    printf("Error, C miscalculation!\n");
  }
  printf("C    accessed [%d][%d] elements %d times in %f seconds\n", size, size,
         ntrials, (endtime-starttime)/(1000*1000));
  free(A);
}
