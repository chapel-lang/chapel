#include <sys/time.h>
#include <stdio.h>
#include "task_group.h"

double cur_time() {
  struct timeval tp[1];
  gettimeofday(tp, NULL);
  return tp->tv_sec + 1.0E-6 * tp->tv_usec;
}

int fib(int n) {
  if (n < 2) return 1;
  else {
    task_group tg; int x, y;
    tg.run([=,&x] { x = fib(n - 1); });
    y = fib(n - 2);
    tg.wait();
    return x + y;
  }
}

int main(int argc, char ** argv) {
  int n = (argc > 1 ? atoi(argv[1]) : 35);
  while (1) {
    double t0 = cur_time();
    int x = fib(n);
    double t1 = cur_time();
    printf("fib(%d) = %d in %.3f sec\n", n, x, t1 - t0);
  }
}
