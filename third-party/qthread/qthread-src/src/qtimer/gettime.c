#include <stdint.h>
#include <stdlib.h>
#include <time.h>

#include <qthread/hash.h> /* for qt_hash64() */
#include <qthread/qtimer.h>

#include "qt_alloc.h"
#include "qt_asserts.h"

struct qtimer_s {
  struct timespec start, stop;
};

void qtimer_start(qtimer_t q) {
  assert(q);
  qassert(clock_gettime(CLOCK_MONOTONIC, &(q->start)), 0);
}

unsigned long API_FUNC qtimer_fastrand(void) {
  struct timespec s;
  static aligned_t volatile state = GOLDEN_RATIO;
  aligned_t volatile tmp; // this volatile is to prevent the compiler from
                          // optimizing tmp out of existence

  qassert(clock_gettime(CLOCK_MONOTONIC, &(s)), 0);
  state = tmp = qt_hash_bytes(&s.tv_nsec, sizeof(s.tv_nsec), state);
  return tmp;
}

void API_FUNC qtimer_stop(qtimer_t q) {
  assert(q);
  qassert(clock_gettime(CLOCK_MONOTONIC, &(q->stop)), 0);
}

double qtimer_wtime(void) {
  struct timespec s;

  qassert(clock_gettime(CLOCK_MONOTONIC, &(s)), 0);
  return s.tv_sec + (s.tv_nsec * 1e-9);
}

double qtimer_res(void) {
  struct timespec s;
  qassert(clock_getres(CLOCK_MONOTONIC, &s), 0);
  return s.tv_sec + (s.tv_nsec * 1e-9);
}

double API_FUNC qtimer_secs(qtimer_t q) {
  assert(q);
  return (q->stop.tv_sec + q->stop.tv_nsec * 1e-9) -
         (q->start.tv_sec + q->start.tv_nsec * 1e-9);
}

qtimer_t API_FUNC qtimer_create(void) {
  qtimer_t ret = qt_calloc(1, sizeof(struct qtimer_s));

  assert(ret);
  return ret;
}

void API_FUNC qtimer_destroy(qtimer_t q) {
  assert(q);
  FREE(q, sizeof(struct qtimer_s));
}

/* vim:set expandtab: */
