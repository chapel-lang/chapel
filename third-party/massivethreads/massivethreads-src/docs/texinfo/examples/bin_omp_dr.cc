#include <stdio.h>
#include <stdlib.h>
#include <tpswitch/omp_dr.h>

long bin(int n) {
  if (n == 0) return 1;
  else {
    long x, y;
    pragma_omp_task(shared(x), 
		    x = bin(n - 1));
    pragma_omp_task(shared(y), 
		    y = bin(n - 1));
    pragma_omp_taskwait;
    return x + y;
  }
}

int main(int argc, char ** argv) {
  int n = atoi(argv[1]);
  pragma_omp_parallel_single(, {
      dr_start(0);
      long x = bin(n);
      dr_stop();
      printf("bin(%d) = %ld\n", n, x);
      dr_dump();
    });
  return 0;
}

