#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include <pthread.h>

void * f(void * x) {
  return (void *)((size_t)x + 34);
}

int main() {
  void * ret[1];
  pthread_t th;
  int r = pthread_create(&th, 0, f, (void *)12);
  assert(r == 0);
  pthread_join(th, ret);
  if (ret[0] == (void *)46) {
    printf("OK\n");
    return 0;
  } else {
    printf("NG\n");
    return 1;
  }
}

