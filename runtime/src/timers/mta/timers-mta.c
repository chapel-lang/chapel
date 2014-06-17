#include "chplrt.h"
#include "chpltimers.h"
#include <machine/runtime.h>
#include <sys/mta_task.h>

_real64 chpl_now_time(void) {
#pragma mta fence
  return ((_real64)mta_get_clock(0)/mta_clock_freq())*1.0e+6;
}
