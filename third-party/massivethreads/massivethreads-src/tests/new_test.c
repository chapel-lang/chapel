#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include <myth/myth.h>

typedef struct {
  int worker;
  double t;
} rec;

double cur_time() {
  struct timespec ts[1];
  clock_gettime(CLOCK_REALTIME, ts);
  return ts->tv_sec + ts->tv_nsec * 1.0e-9;
}

double t0;

void record(rec * R) {
  R->worker = 1 + myth_get_worker_num();
  R->t = cur_time() - t0;
}

typedef struct {
  rec A[100];
  rec B[100];
  rec C[100];
  rec D[100];
  rec E[100];
} progress;

progress G;

void * f(void * x) {
  long i = (long)x;
  record(&G.B[i]);
  return (void *)(i * i);
}

int main(int argc, char ** argv) {
  int nthreads = (argc > 1 ? atoi(argv[1]) : 5);
  myth_thread_t th[nthreads];
  long i;
  t0 = cur_time();
  for (i = 0; i < nthreads; i++) {
    record(&G.A[i]);
    th[i] = myth_create(f, (void *)i);
    record(&G.C[i]);
  }
  for (i = 0; i < nthreads; i++) {
    void * ret[1];
    record(&G.D[i]);
    myth_join(th[i], ret);
    record(&G.E[i]);
    if (ret[0] != (void *)(i * i)) {
      printf("NG\n");
      return 1;
    }
  }
  printf("OK\n");
  return 0;
}

