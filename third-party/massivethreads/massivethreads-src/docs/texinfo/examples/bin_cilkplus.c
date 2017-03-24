#include <stdio.h>
#include <stdlib.h>
#include <cilk/cilk.h>

long bin(int n) {
  if (n == 0) return 1;
  else {
    long x, y;
    x = cilk_spawn bin(n - 1);
    y = cilk_spawn bin(n - 1);
    cilk_sync;
    return x + y;
  }
}

int main(int argc, char ** argv) {
  int n = atoi(argv[1]);
  long x;
  x = cilk_spawn bin(n);
  cilk_sync;
  printf("bin(%d) = %ld\n", n, x);
  return 0;
}
