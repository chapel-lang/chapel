
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include <pthread.h>

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
    pthread_t tid;
    int r = pthread_create(&tid, 0, f, cargs);
    assert(r == 0);
    f(cargs + 1);
    pthread_join(tid, 0);
    arg->r = cargs[0].r + cargs[1].r;
  }
  return 0;
}

int main(int argc, char ** argv) {
  long nthreads = (argc > 1 ? atol(argv[1]) : 100);
  arg_t arg[1] = { { 0, nthreads, 0 } };
  pthread_t tid;
  int r = pthread_create(&tid, 0, f, arg);
  assert(r == 0);
  pthread_join(tid, 0);
  if (arg->r == (nthreads - 1) * nthreads / 2) {
    printf("OK\n");
    return 0;
  } else {
    printf("NG\n");
    return 1;
  }
}

