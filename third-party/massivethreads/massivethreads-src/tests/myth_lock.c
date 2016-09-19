
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include <myth/myth.h>

typedef struct {
  long ninc_per_thread;
  long a;
  long b;
  long r;
  long * p;
  myth_mutex_t * m;
} arg_t;

void * f(void * arg_) {
  arg_t * arg = (arg_t *)arg_;
  long a = arg->a, b = arg->b;
  long ninc_per_thread = arg->ninc_per_thread;
#if DBG
  printf("%ld : f(%ld,%ld)\n", myth_get_worker_num(), a, b);
#endif
  if (b - a == 1) {
    int i;
    for (i = 0; i < ninc_per_thread; i++) {
      myth_mutex_lock(arg->m);
      arg->p[0]++;
      myth_mutex_unlock(arg->m);
    }
    arg->r = a;
  } else {
    long c = (a + b) / 2;
    arg_t cargs[2] = { { ninc_per_thread, a, c, 0, arg->p, arg->m }, 
		       { ninc_per_thread, c, b, 0, arg->p, arg->m } };
    myth_thread_t tid = myth_create(f, cargs);
    f(cargs + 1);
    myth_join(tid, 0);
    arg->r = cargs[0].r + cargs[1].r;
  }
  return 0;
}

myth_mutex_t m[1];

int main(int argc, char ** argv) {
  long nthreads        = (argc > 1 ? atol(argv[1]) : 50);
  long ninc_per_thread = (argc > 2 ? atol(argv[2]) : 1000);

  myth_mutex_init(m, 0);
  long p[1] = { 0 };
  arg_t arg[1] = { { ninc_per_thread, 0, nthreads, 0, p, m } };
  myth_thread_t tid = myth_create(f, arg);
  myth_join(tid, 0);

  if (arg->r == (nthreads - 1) * nthreads / 2
      && arg->p[0] == nthreads * ninc_per_thread) {
    printf("OK\n");
    return 0;
  } else {
    printf("NG: p = %ld != nthreads * ninc_per_thread = %ld\n",
	   arg->p[0], nthreads * ninc_per_thread);
    return 1;
  }
}

