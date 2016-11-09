#include <stdio.h>
#include <stdlib.h>
#include <tpswitch/cilkplus_dr.h>

long bin(int n) {
  cilk_begin;
  if (n == 0) cilk_return(1);
  else {
    long x, y;
    spawn_(x = cilk_spawn bin(n - 1));
    spawn_(y = cilk_spawn bin(n - 1));
    cilk_sync_;
    cilk_return(x + y);
  }
}

int main(int argc, char ** argv) {
  cilk_begin;
  int n = atoi(argv[1]);
  dr_start(0);
  long x;
  spawn_(x = cilk_spawn bin(n));
  cilk_sync_;
  dr_stop();
  printf("bin(%d) = %ld\n", n, x);
  dr_dump();
  cilk_return(0);
}
