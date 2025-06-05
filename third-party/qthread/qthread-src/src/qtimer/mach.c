#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>

#include <mach/mach_time.h>

#include "qt_alloc.h"
#include "qt_atomics.h"      /* for SPINLOCK_BODY() */
#include "qthread/qthread.h" /* for aligned_t */
#include "qthread/qtimer.h"

static aligned_t inited = 0;
static double conversion = 0.0;

struct qtimer_s {
  uint64_t start, stop;
};

void API_FUNC qtimer_start(qtimer_t q) {
  if (inited == 0) {
    if (qthread_cas(&inited, 0, 1) == 0) {
      mach_timebase_info_data_t info;
      kern_return_t err = mach_timebase_info(&info);

      // Convert the timebase into seconds
      if (err == 0) {
        conversion = 1e-9 * (double)info.numer / (double)info.denom;
      }
      MACHINE_FENCE;
      inited = 2;
    } else {
      while (inited == 1) SPINLOCK_BODY();
    }
  }
  q->start = mach_absolute_time();
}

unsigned long API_FUNC qtimer_fastrand(void) {
  return (unsigned long)(mach_absolute_time());
}

void API_FUNC qtimer_stop(qtimer_t q) { q->stop = mach_absolute_time(); }

double qtimer_wtime(void) { return conversion * mach_absolute_time(); }

double qtimer_res(void) { return 1.0 / sysconf(_SC_CLK_TCK); }

double API_FUNC qtimer_secs(qtimer_t q) {
  uint64_t difference = q->stop - q->start;

  return conversion * (double)difference;
}

qtimer_t API_FUNC qtimer_create(void) {
  return qt_calloc(1, sizeof(struct qtimer_s));
}

void API_FUNC qtimer_destroy(qtimer_t q) { FREE(q, sizeof(struct qtimer_s)); }

/* vim:set expandtab: */
