#include <stdio.h>
#include <stdlib.h>

long bin(int n) {
  if (n == 0) return 1;
  else {
    long x, y;
    x = _Cilk_spawn bin(n - 1);
    y = _Cilk_spawn bin(n - 1);
    _Cilk_sync;
    return x + y;
  }
}

int main(int argc, char ** argv) {
  int n = atoi(argv[1]);
  long x;
  x = _Cilk_spawn bin(n);
  _Cilk_sync;
  printf("bin(%d) = %ld\n", n, x);
  return 0;
}
