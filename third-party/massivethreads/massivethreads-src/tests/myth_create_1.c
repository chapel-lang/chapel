#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include <myth/myth.h>

void * f(void * x) {
  long i = (long)x;
  return (void *)(i * i);
}

myth_thread_t parent_th;
myth_thread_t th[1000];
void * ret[1000];

//#define nthreads 100
int main(int argc, char ** argv) {
  int nthreads = (argc > 1 ? atoi(argv[1]) : 100);
  long i;
  parent_th = myth_self();
  for (i = 0; i < nthreads; i++) {
    th[i] = myth_create(f, (void *)i);
    myth_join(th[i], &ret[i]);
    if (ret[i] != (void *)(i * i)) {
      printf("NG\n");
      return 1;
    }
  }
#if 0
  for (i = 0; i < nthreads; i++) {
    void * ret[1];
    myth_join(th[i], ret);
    if (ret[0] != (void *)(i * i)) {
      printf("NG\n");
      return 1;
    }
  }
#endif
  printf("OK\n");
  return 0;
}

