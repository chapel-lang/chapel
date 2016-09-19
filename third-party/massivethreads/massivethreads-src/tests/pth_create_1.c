#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include <pthread.h>

void * f(void * x) {
  long y = (long)x;
  return (void *)(y * y);
}

#define nthreads 100
int main() {
  pthread_t th[nthreads];
  long i;
  for (i = 0; i < nthreads; i++) {
    int r = pthread_create(&th[i], 0, f, (void *)i);
    assert(r == 0);
  }
  for (i = 0; i < nthreads; i++) {
    void * ret[1];
    pthread_join(th[i], ret);
    if (ret[0] != (void *)(i * i)) {
      printf("NG\n");
      return 1;
    }
  }
  printf("OK\n");
  return 0;
}

