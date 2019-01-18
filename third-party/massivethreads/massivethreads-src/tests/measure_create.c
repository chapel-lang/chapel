
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <myth/myth.h>

typedef struct {
  long a;
  long b;
  long r;
} arg_t;
 
void * f(void * arg_) {
  arg_t * arg = (arg_t *)arg_;
  long a = arg->a, b = arg->b;
  if (b - a == 1) {
    arg->r = a;
  } else {
    long c = (a + b) / 2;
    arg_t cargs[2] = { { a, c, 0 }, { c, b, 0 } };
    myth_thread_t tid = myth_create(f, cargs);
    f(cargs + 1);
    myth_join(tid, 0);
    arg->r = cargs[0].r + cargs[1].r;
  }
  return 0;
}

double cur_time() {
  struct timespec ts[1];
  clock_gettime(CLOCK_REALTIME, ts);
  return ts->tv_sec + ts->tv_nsec * 1.0e-9;
}

int main(int argc, char ** argv) {
  long nthreads = (argc > 1 ? atol(argv[1]) : 100000);
  arg_t arg[1] = { { 0, nthreads, 0 } };
  long i;
  for (i = 0; i < 3; i++) {
    double t0 = cur_time();
    myth_thread_t tid = myth_create(f, arg);
    myth_join(tid, 0);
    double t1 = cur_time();
    double dt = t1 - t0;
    if (arg->r == (nthreads - 1) * nthreads / 2) {
      printf("OK\n");
      printf("%ld thread creation/join in %.9f sec (%.3f per sec)\n",
	     nthreads, dt, nthreads / dt);
    } else {
      printf("NG\n");
      return 1;
    }
  }
  return 0;
}

