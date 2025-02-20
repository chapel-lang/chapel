#include "argparsing.h"
#include <float.h> /* for DBL_MAX, per C89 */
#include <math.h>
#include <qthread/barrier.h>
#include <qthread/qthread.h>
#include <qthread/qtimer.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

aligned_t initme_idx = 1;
aligned_t *initme = NULL;
qt_barrier_t *wait_on_me;

static aligned_t barrier_thread(void *arg) {
  void **args = (void **)arg;
  qt_barrier_t *b = (qt_barrier_t *)args[0];
  aligned_t idx = qthread_incr(&initme_idx, 1);

  qthread_incr(&(initme[idx]), 1);
  qt_barrier_enter_id(b, (uintptr_t)args[1]);
  return 0;
}

int main(int argc, char *argv[]) {
  size_t threads = 1000, i;
  aligned_t *rets;
  qtimer_t t;
  unsigned int iter, iterations = 10;
  double total_time = 0.0;
  double max_time = 0.0;
  double min_time = DBL_MAX;

  test_check(qthread_initialize() == 0);
  t = qtimer_create();

  CHECK_VERBOSE();
  NUMARG(threads, "THREADS");
  NUMARG(iterations, "ITERATIONS");

  initme = (aligned_t *)calloc(threads, sizeof(aligned_t));
  test_check(initme);

  rets = (aligned_t *)malloc(threads * sizeof(aligned_t));
  test_check(rets);

  iprintf("creating the barrier for %zu threads\n", threads);
  wait_on_me =
    qt_barrier_create(threads, REGION_BARRIER); // all my spawnees plus me
  test_check(wait_on_me);

  for (iter = 0; iter <= iterations; ++iter) {
    // iprintf("%i: forking the threads\n", iter);
    for (i = 1; i < threads; ++i) {
      void *args[2] = {wait_on_me, (void *)(uintptr_t)i};
      qthread_spawn(
        barrier_thread, args, sizeof(args), rets + i, 0, NULL, i, 0);
    }
    // iprintf("%i: done forking the threads, entering the barrier\n", iter);
    qtimer_start(t);
    qt_barrier_enter_id(wait_on_me, 0);
    qtimer_stop(t);
    if (iter > 0) {
      double tmp = qtimer_secs(t);
      iprintf("%i: main thread exited barrier in %f seconds (%u)\n",
              iter,
              tmp,
              initme_idx);
      total_time += tmp;
      if (max_time < tmp) { max_time = tmp; }
      if (min_time > tmp) { min_time = tmp; }
    }

    test_check(initme_idx == threads);
    initme_idx = 1;

    for (i = 1; i < threads; ++i) {
      qthread_readFF(NULL, rets + i);
      if (initme[i] != iter + 1) {
        iprintf(
          "initme[%i] = %i (should be %i)\n", (int)i, (int)initme[i], iter + 1);
      }
      test_check(initme[i] == iter + 1);
    }
  }

  iprintf("Max barrier time:     %f secs\n", max_time);
  iprintf("Average barrier time: %f secs\n", total_time / iterations);
  iprintf("Min barrier time:     %f secs\n", min_time);

  iprintf("Destroying barrier...\n");
  qt_barrier_destroy(wait_on_me);
  qtimer_destroy(t);

  free(rets);
  free(initme);

  iprintf("Success!\n");

  return 0;
}

/* vim:set expandtab */
