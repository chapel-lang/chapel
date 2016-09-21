/* 
 * pth_yield.c
 */

#define _GNU_SOURCE 1
#include <pthread.h>

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>


typedef struct {
  long a;
  long b;
  long r;
} arg_t;
 
void * f(void * arg_) {
  arg_t * arg = (arg_t *)arg_;
  long a = arg->a, b = arg->b;
  if (b - a == 1) {
    int i;
    for (i = 0; i < 10; i++) {
      pthread_yield();
    }
    arg->r = a;
  } else {
    long c = (a + b) / 2;
    arg_t cargs[2] = { { a, c, 0 }, { c, b, 0 } };
    pthread_t tid;
    pthread_create(&tid, 0, f, cargs);
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
  pthread_create(&tid, 0, f, arg);
  pthread_join(tid, 0);
  if (arg->r == (nthreads - 1) * nthreads / 2) {
    printf("OK\n");
    return 0;
  } else {
    printf("NG\n");
    return 1;
  }
}

