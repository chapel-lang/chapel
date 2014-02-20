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


//
// This is the default task call stack size, in bytes.  It was
// chosen because it matches a common default process stack size in
// linux, which was inherited by the fifo tasking layer, and Chapel
// programmers have become used to that.  In the future, and as our
// ability to detect and report task stack overflow improves, we may
// reduce it.
//
#define DEFAULT_CALL_STACK_SIZE ((size_t) 8 << 20)


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


size_t chpl_task_getMinCallStackSize(void)
{
  char*         p;
  int           scan_cnt;
  static int    env_checked = 0;
  static size_t size = 0;
  char          units;

  if (env_checked)
    return size;

  if ((p = getenv("CHPL_RT_CALL_STACK_SIZE")) == NULL)
    size = DEFAULT_CALL_STACK_SIZE;
  else {
    if ((scan_cnt = sscanf(p, "%zu%c", &size, &units)) != 1) {
      if (scan_cnt == 2 && strchr("kKmMgG", units) != NULL) {
        switch (units) {
        case 'k' : case 'K': size <<= 10; break;
        case 'm' : case 'M': size <<= 20; break;
        case 'g' : case 'G': size <<= 30; break;
        }
      }
      else
        chpl_warning("Cannot parse CHPL_RT_CALL_STACK_SIZE environment "
                     "variable", 0, NULL);
    }

    if (size <= 0) {
      chpl_error("CHPL_RT_CALL_STACK_SIZE must be > 0", 0, NULL);
      size = DEFAULT_CALL_STACK_SIZE;
    }
  }

  env_checked = 1;

  return size;
}
