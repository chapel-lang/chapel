#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include <myth/myth.h>

void * f(void * x) {
  return (void *)((size_t)x + 34);
}

int main() {
  void * ret[1];
  myth_thread_t th = myth_create(f, (void *)12);
  myth_join(th, ret);
  if (ret[0] == (void *)46) {
    printf("OK\n");
    return 0;
  } else {
    printf("NG\n");
    return 1;
  }
}

