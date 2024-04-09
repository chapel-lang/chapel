#include <stdio.h>

static inline void printThree(double* ptr) {
  printf("%lf %lf %lf\n", ptr[0], ptr[1], ptr[2]);
}
