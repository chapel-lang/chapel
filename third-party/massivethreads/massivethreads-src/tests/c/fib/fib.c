#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct fib_arg {
  pthread_t tid;
  int n;
  int x;
} fib_arg, * fib_arg_t;

void * fib(void * a_) {
  fib_arg_t a = a_;
  int n = a->n;
  if (n < 2) {
    a->x = 1;
  } else {
    fib_arg c[2] = { { 0, n - 1, 0 }, 
		     { 0, n - 2, 0 } };
    pthread_create(&c[0].tid, NULL, fib, &c[0]);
    pthread_create(&c[1].tid, NULL, fib, &c[1]);
    pthread_join(c[0].tid, NULL);
    pthread_join(c[1].tid, NULL);
    a->x = c[0].x + c[1].x;
  }
  return a;
}

int main(int argc, char ** argv) {
  int n = (argc > 1 ? atoi(argv[1]) : 10);
  fib_arg a = { 0, n, 0 };
  pthread_create(&a.tid, NULL, fib, &a);
  pthread_join(a.tid, NULL);
  printf("fib(%d) = %d\n", a.n, a.x);
  return 0;
}
