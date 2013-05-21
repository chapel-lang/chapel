//
// Shared code for different tasking implementations in
// tasks/<tasklayer>/tasks-<tasklayer>.c
//
#include "chplrt.h"
#include "chpl-comm.h"
#include "chpl-tasks.h"
#include "error.h"

#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


int32_t chpl_task_getenvNumThreadsPerLocale(void)
{
  char*          p;
  static int     env_checked = 0;
  static int32_t num = 0;

  if (env_checked)
    return num;

  if ((p = getenv("CHPL_RT_NUM_THREADS_PER_LOCALE")) != NULL) {
    if (sscanf(p, "%" SCNi32, &num) != 1)
      chpl_warning("Cannot parse CHPL_RT_NUM_THREADS_PER_LOCALE environment "
                   "variable", 0, NULL);
    if (num < 0) {
      chpl_error("CHPL_RT_NUM_THREADS_PER_LOCALE must be >= 0", 0, NULL);
      num = 0;
    }
    else {
      int32_t lim = chpl_comm_getMaxThreads();
      if (lim > 0 && num > lim) {
        char msg[200];
        snprintf(msg, sizeof(msg),
                 "CHPL_RT_NUM_THREADS_PER_LOCALE = %" PRIi32 " is too large; "
                 "limit is %" PRIi32,
                 num, lim);
        chpl_warning(msg, 0, NULL);
        num = lim;
      }
    }
  }

  env_checked = 1;

  return num;
}


int64_t chpl_task_getenvCallStackSize(void)
{
  char*          p;
  int            scan_cnt;
  static int     env_checked = 0;
  static int64_t size = 0;
  char           units;

  if (env_checked)
    return size;

  if ((p = getenv("CHPL_RT_CALL_STACK_SIZE")) != NULL) {
    if ((scan_cnt = sscanf(p, "%" SCNi64 "%c", &size, &units)) != 1) {
      if (scan_cnt == 2 && strchr("kKmMgG", units) != NULL) {
        switch (units) {
        case 'k' : case 'K': size *= 1 << 10; break;
        case 'm' : case 'M': size *= 1 << 20; break;
        case 'g' : case 'G': size *= 1 << 30; break;
        }
      }
      else
        chpl_warning("Cannot parse CHPL_RT_CALL_STACK_SIZE environment "
                     "variable", 0, NULL);
    }

    if (size < 0) {
      chpl_error("CHPL_RT_CALL_STACK_SIZE must be >= 0", 0, NULL);
      size = 0;
    }
  }

  env_checked = 1;

  return size;
}
