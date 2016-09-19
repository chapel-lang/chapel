#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/time.h>
#include <pthread.h>

/* Fibonacci with stack variables */

inline double curr_time(void)
{
  struct timeval tv;
  gettimeofday(&tv,NULL);
  return tv.tv_sec + (double) tv.tv_usec * 1e-6;
}

void *fib(void *args)
{
  pthread_t th1, th2;
  intptr_t arg1, arg2;
  intptr_t ret1, ret2;
  intptr_t n = (intptr_t) args;
  int status;
  
  if (n == 0 || n == 1) pthread_exit((void *) n);
  
  arg1 = n - 1;
  arg2 = n - 2;
  if (pthread_create(&th1, NULL, fib, (void*) arg1) != 0) {
    perror(NULL);
    abort();
  }
  if (pthread_create(&th2, NULL, fib, (void*) arg2) != 0) {
    perror(NULL);
    abort();
  }
  pthread_join(th1, (void**) &ret1);
  pthread_join(th2, (void**) &ret2);
  pthread_exit((void*) (ret1 + ret2));
}

int main(int argc,char **argv)
{
  double t0,t1;
  intptr_t n;
  intptr_t ret;
  pthread_t th;

  n = atoi(argv[1]);
  t0 = curr_time();
  pthread_create(&th, NULL, fib, (void *) n);
  pthread_join(th, (void **) &ret);
  t1 = curr_time();
  printf("fib(%d)=%d, took %lf sec\n", n, ret, t1 - t0);
  return 0;
}
