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
#include <sys/resource.h>
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


static size_t stack_size_default(void)
{
  size_t s;

#ifdef __CYGWIN__
  {
    struct rlimit rlim;
    if (getrlimit(RLIMIT_STACK, &rlim) != 0)
      chpl_internal_error("getrlimit() failed");
    if (rlim.rlim_cur == RLIM_INFINITY)
      s = (size_t) 2 << 20;    // 2 MiB
    else
      s = rlim.rlim_cur;
  }
#else
  //
  // This is the default task call stack size, in bytes, for everything
  // except Cygwin.  It was chosen because it matches a common default
  // process stack size in linux, which was inherited by the fifo
  // tasking layer, and Chapel programmers have become used to that.  In
  // the future, and as our ability to detect and report task stack
  // overflow improves, we may reduce it.
  //
  s = (size_t) 8 << 20;    // 8 MiB
#endif

  return s;
}


static size_t stack_size_max(void)
{
  size_t s;

#ifdef __CYGWIN__
  {
    struct rlimit rlim;
    if (getrlimit(RLIMIT_STACK, &rlim) != 0)
      chpl_internal_error("getrlimit() failed");
    if (rlim.rlim_max == RLIM_INFINITY)
      s = (size_t) 2 << 20;    // 2 MiB
    else
      s = rlim.rlim_max;
  }
#else
  s = SIZE_MAX;
#endif

  return s;
}


size_t chpl_task_getMinCallStackSize(void)
{
  size_t        deflt;
  size_t        max;
  char*         p;
  int           scan_cnt;
  static int    env_checked = 0;
  static size_t size = 0;
  char          units;
  char          msg[200];

  if (env_checked)
    return size;

  deflt = stack_size_default();
  max = stack_size_max();

  if ((p = getenv("CHPL_RT_CALL_STACK_SIZE")) == NULL)
    size = deflt;
  else {
    if ((scan_cnt = sscanf(p, "%zu%c", &size, &units)) != 1) {
      if (scan_cnt == 2 && strchr("kKmMgG", units) != NULL) {
        switch (units) {
        case 'k' : case 'K': size <<= 10; break;
        case 'm' : case 'M': size <<= 20; break;
        case 'g' : case 'G': size <<= 30; break;
        }
      }
      else {
        snprintf(msg, sizeof(msg),
                 "Cannot parse CHPL_RT_CALL_STACK_SIZE environment variable; "
                 "using %zd",
                 deflt);
        chpl_warning(msg, 0, NULL);
        size = deflt;
      }
    }

    if (size <= 0) {
      snprintf(msg, sizeof(msg),
               "CHPL_RT_CALL_STACK_SIZE must be > 0; using %zd",
               deflt);
      chpl_warning(msg, 0, NULL);
      size = deflt;
    }

    if (size > max) {
      snprintf(msg, sizeof(msg),
               "CHPL_RT_CALL_STACK_SIZE must be <= %zd; using %zd",
               max, max);
      chpl_warning(msg, 0, NULL);
      size = max;
    }
  }

  env_checked = 1;

  return size;
}
