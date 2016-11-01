
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <myth/myth.h>

typedef unsigned long long ts_t;

#define USE_RDTSC 0

#if USE_RDTSC
/* TODO : make it run on Sparc */
static inline ts_t rdtsc() {
  unsigned long long u;
  asm volatile ("rdtsc;shlq $32,%%rdx;orq %%rdx,%%rax":"=a"(u)::"%rdx");
  return u;
}
static inline ts_t cur_time() {
  return rdtsc();
}

#else
ts_t cur_time() {
  struct timespec ts[1];
  clock_gettime(CLOCK_REALTIME, ts);
  return ts->tv_sec * 1000000000 + ts->tv_nsec;
}
#endif

typedef struct {
  union {
    ts_t child_started;
    char pad0[4096];
  };
  union {
    volatile int parent_resumed;
    char pad1[4096];
  };
  union {
    volatile int child_almost_finished;
    char pad2[4096];
  };
} arg_t;
 
void * f(void * arg_) {
  arg_t * arg = (arg_t *)arg_;
  arg->child_started = cur_time();
  while (! arg->parent_resumed) { }
  arg->child_almost_finished = 1;
  return 0;
}

void wait_a_while() {
  ts_t t0 = cur_time();
  while (cur_time() - t0 < 1000) { }
}

void bench(long n) {
  long i;
  ts_t child_latency_sum = 0;
  ts_t parent_latency_sum = 0;
  ts_t join_latency_sum = 0;
  arg_t arg[1];
  
  for (i = 0; i < n; i++) {
    arg->child_started = 0;
    arg->parent_resumed = 0;
    arg->child_almost_finished = 0;

    ts_t t0 = cur_time();
    myth_thread_t c = myth_create(f, arg);
    ts_t t1 = cur_time();
    arg->parent_resumed = 1;
    while (! arg->child_almost_finished) { }
    wait_a_while();
    ts_t t2 = cur_time();
    myth_join(c, 0);
    ts_t t3 = cur_time();
    child_latency_sum += (arg->child_started - t0);
    parent_latency_sum += (t1 - t0);
    join_latency_sum += (t3 - t2);
  }

  printf("avg child latency = %.9f clocks\n", child_latency_sum / (double)n);
  printf("avg parent latency = %.9f clocks\n", parent_latency_sum / (double)n);
  printf("avg join latency = %.9f clocks\n", join_latency_sum / (double)n);

}

int main(int argc, char ** argv) {
  long n = (argc > 1 ? atol(argv[1]) : 100000);
  long i;
  for (i = 0; i < 3; i++) {
    bench(n);
  }
  return 0;
}

