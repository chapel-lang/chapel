/* 
 * myth_join_counter.c --- test join_counter
 */

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include <myth/myth.h>

myth_join_counter_t j[1];

typedef struct {
  myth_thread_t tid;
  long x;
  long y;
  long n_producers;
} arg_t;

long f(long x) { 
  return (x + 1) * (x + 1); 
}

void * producer(void * arg_) {
  arg_t * arg = (arg_t *)arg_;
  arg->y = f(arg->x);
  myth_join_counter_dec(j);
  return 0;
}

void * consumer(void * arg_) {
  arg_t * arg = (arg_t *)arg_;
  myth_join_counter_wait(j);
  long i;
  for (i = 1; i < arg->n_producers; i++) {
    assert(arg[i].y == f(arg[i].x));
  }
  return 0;
}

int main(int argc, char ** argv) {
  long n_producers = (argc > 1 ? atol(argv[1]) : 1000);
  long i;
  arg_t arg = { 0, -1, -1, n_producers };
  arg_t * args = (arg_t *)calloc(sizeof(arg_t), (n_producers + 1));

  myth_join_counter_init(j, 0, n_producers);
  for (i = 0; i < n_producers + 1; i++) {
    args[i] = arg;
    args[i].x = i;
    if (i == 0) {
      args[i].tid = myth_create(consumer, &args[i]);
    } else {
      args[i].tid = myth_create(producer, &args[i]);
    }
  }
  for (i = 0; i < n_producers + 1; i++) {
    myth_join(args[i].tid, 0);
  }
  printf("OK\n");
  return 0;
}
