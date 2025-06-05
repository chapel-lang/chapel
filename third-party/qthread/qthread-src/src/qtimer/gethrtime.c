#include <stdlib.h>

#include <unistd.h>

#include <qthread/qtimer.h>

#include "qt_alloc.h"

struct qtimer_s {
  hrtime_t start;
  hrtime_t stop;
};

void API_FUNC qtimer_start(qtimer_t q) { q->start = gethrtime(); }

unsigned long API_FUNC qtimer_fastrand(void) {
  return (unsigned long)(gethrtime());
}

void API_FUNC qtimer_stop(qtimer_t q) { q->stop = gethrtime(); }

double qtimer_wtime(void) { return ((double)gethrtime()) * 1e-9; }

double qtimer_res(void) { return 1.0 / sysconf(_SC_CLK_TCK); }

double API_FUNC qtimer_secs(qtimer_t q) {
  return ((double)(q->stop - q->start)) * 1e-9;
}

qtimer_t API_FUNC qtimer_create() {
  return qt_calloc(1, sizeof(struct qtimer_s));
}

void API_FUNC qtimer_destroy(qtimer_t q) { FREE(q, sizeof(struct qtimer_s)); }

/* vim:set expandtab: */
