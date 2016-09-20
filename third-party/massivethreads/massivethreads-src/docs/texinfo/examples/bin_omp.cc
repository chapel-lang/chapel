#include <stdio.h>
#include <stdlib.h>

long bin(int n) {
  if (n == 0) return 1;
  else {
    long x, y;
#pragma omp task shared(x)
    x = bin(n - 1);
#pragma omp task shared(y)
    y = bin(n - 1);
#pragma omp taskwait
    return x + y;
  }
}

int main(int argc, char ** argv) {
  int n = atoi(argv[1]);
#pragma omp parallel 
#pragma omp single
  {
    long x = bin(n);
    printf("bin(%d) = %ld\n", n, x);
  }
  return 0;
}

