#include <stdlib.h>

#include <sys/time.h>
#include <unistd.h>

#include <qthread/qtimer.h>

#include "qt_alloc.h"

struct qtimer_s {
  struct timeval start, stop;
};

void API_FUNC qtimer_start(qtimer_t q) { gettimeofday(&(q->start), NULL); }

unsigned long API_FUNC qtimer_fastrand(void) {
  struct timeval s;

  gettimeofday(&(s), NULL);
  return (unsigned long)(s.tv_usec);
}

double qtimer_wtime(void) {
  struct timeval s;

  gettimeofday(&(s), NULL);
  return s.tv_sec + (s.tv_usec * 1e-6);
}

double qtimer_res(void) { return 1.0 / sysconf(_SC_CLK_TCK); }

void API_FUNC qtimer_stop(qtimer_t q) { gettimeofday(&(q->stop), NULL); }

double API_FUNC qtimer_secs(qtimer_t q) {
  return (q->stop.tv_sec + q->stop.tv_usec * 1e-6) -
         (q->start.tv_sec + q->start.tv_usec * 1e-6);
}

qtimer_t API_FUNC qtimer_create() {
  return qt_calloc(1, sizeof(struct qtimer_s));
}

void API_FUNC qtimer_destroy(qtimer_t q) { FREE(q, sizeof(struct qtimer_s)); }

/* vim:set expandtab: */
