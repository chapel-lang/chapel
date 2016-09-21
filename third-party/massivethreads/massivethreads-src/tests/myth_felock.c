/* 
 * myth_felock.c --- test full empty 
 */

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include <myth/myth.h>

myth_felock_t fe[1];
long count;

typedef struct {
  myth_thread_t tid;
  long n;
} arg_t;

void * incrementer(void * arg_) {
  arg_t * arg = (arg_t *)arg_;
  long n = arg->n;
  long i;
  for (i = 0; i < n; i++) {
    /* wait until full and lock it */
    myth_felock_wait_and_lock(fe, 1);
    long c = count;
    /* mark it's empty */
    myth_felock_mark_and_signal(fe, 0);
    /* wait until empty and lock it */
    myth_felock_wait_and_lock(fe, 0);
    count = c + 1;
    /* mark it's full */
    myth_felock_mark_and_signal(fe, 1);
  }
  return 0;
}

int main(int argc, char ** argv) {
  long n_threads = (argc > 1 ? atol(argv[1]) : 100);
  long n = (argc > 2 ? atol(argv[2]) : 1000);
  long i;
  arg_t arg = { 0, n };
  arg_t * args = (arg_t *)calloc(sizeof(arg_t), n_threads);

  myth_felock_init(fe, 0);
  for (i = 0; i < n_threads; i++) {
    args[i] = arg;
    args[i].tid = myth_create(incrementer, &args[i]);
  }
  const long init_count = 123;
  count = init_count;
  myth_felock_wait_and_lock(fe, 0);
  myth_felock_mark_and_signal(fe, 1);
  for (i = 0; i < n_threads; i++) {
    myth_join(args[i].tid, 0);
  }
  assert(count == n_threads * n + init_count);
  printf("OK\n");
  return 0;
}
