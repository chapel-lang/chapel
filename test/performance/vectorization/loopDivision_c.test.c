#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <stdint.h>

#define A 1.0f
#define B 20.0f
#define C 25.6f
#define D 24.0f

#ifndef N
#define N 100000000
#endif
#ifndef iters
#define iters 100
#endif

#ifndef ARRAY_TYPE
#define ARRAY_TYPE int64_t
#endif

ARRAY_TYPE nums[N];

float kernel() {
  float sum = 0;
  for (long i = 0; i < N; i ++) {
      const float x = nums[i] % 3 == 0 ?
          (nums[i] % 5 == 0 ? A : B) :
          (nums[i] % 4 == 0 ? C : D);
      const float rx = 1.0f / x;
      sum += rx;
  }
  return sum;
}

void c_version(int initArray, int printTime, int printCorrectness) {
  if (initArray) {
    #ifdef seed
    srand(seed);
    #endif
    // init backwards to preseve cache
    for (long i = N-1; i >= 0; i--) {
      nums[i] = rand();
    }
  }

  float dest[iters];

  struct timespec start_time, end_time;
  clock_gettime(CLOCK_MONOTONIC_RAW, &start_time);
  for (int i = 0; i < iters; i++) {
    dest[i] = kernel();
  }
  clock_gettime(CLOCK_MONOTONIC_RAW, &end_time);
  float elapsed = (end_time.tv_sec - start_time.tv_sec)+ (end_time.tv_nsec - start_time.tv_nsec) / 1000000000.0;
  if (printTime) printf("c    Time: %f\n", elapsed);

  float sum = 0.0;
  for (int i = 0; i < iters; i++) {
    sum += dest[i];
  }
  if (printCorrectness) printf("%6.2f\n", sum);
}

int main(int argc, char **argv) {
  if (argc != 4) {
    printf("Usage: %s <initArray> <printTime> <printCorrectness>\n", argv[0]);
    return 1;
  }
  int initArray = atoi(argv[1]);
  int printTime = atoi(argv[2]);
  int printCorrectness = atoi(argv[3]);
  c_version(initArray, printTime, printCorrectness);
  return 0;
}
