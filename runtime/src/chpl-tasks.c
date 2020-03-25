/*
 * Copyright 2020 Hewlett Packard Enterprise Development LP
 * Copyright 2004-2019 Cray Inc.
 * Other additional copyright holders may be indicated within.
 * 
 * The entirety of this work is licensed under the Apache License,
 * Version 2.0 (the "License"); you may not use this file except
 * in compliance with the License.
 * 
 * You may obtain a copy of the License at
 * 
 *     http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

//
// Shared code for different tasking implementations in
// tasks/<tasklayer>/tasks-<tasklayer>.c
//
#include "chplrt.h"
#include "chpl-comm.h"
#include "chpl-tasks.h"
#include "chpl-topo.h"
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
    int32_t lim = chpl_comm_getMaxThreads();

    if (strcmp(p, "MAX_PHYSICAL") == 0) {
      num = chpl_topo_getNumCPUsPhysical(true);
      if (lim > 0 && lim < num)
        num = lim;
    }
    else if (strcmp(p, "MAX_LOGICAL") == 0) {
      num = chpl_topo_getNumCPUsLogical(true);
      if (lim > 0 && lim < num)
        num = lim;
    }
    else {
      if (sscanf(p, "%" SCNi32, &num) != 1)
        chpl_error("Cannot parse CHPL_RT_NUM_THREADS_PER_LOCALE environment "
                   "variable", 0, 0);
      if (num < 0) {
        chpl_error("CHPL_RT_NUM_THREADS_PER_LOCALE must be >= 0", 0, 0);
        num = 0;
      }
      else {
        if (lim > 0 && num > lim) {
          char msg[200];
          snprintf(msg, sizeof(msg),
                   "CHPL_RT_NUM_THREADS_PER_LOCALE = %" PRIi32 " is too large; "
                   "limit is %" PRIi32,
                   num, lim);
          chpl_warning(msg, 0, 0);
          num = lim;
        }
      }
    }
  }

  env_checked = 1;

  return num;
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
  {
    struct rlimit rlim;
    if (getrlimit(RLIMIT_STACK, &rlim) != 0)
      chpl_internal_error("getrlimit() failed");
    if (rlim.rlim_max == RLIM_INFINITY)
      s = SIZE_MAX;
    else
      s = rlim.rlim_max;
  }
#endif

  return s;
}


size_t chpl_task_getEnvCallStackSize(void)
{
  char*         p;
  int           scan_cnt;
  static int    have_size = 0;
  static size_t size = 0;
  char          units;
  char          msg[200];

  if (have_size)
    return size;

  if ((p = getenv("CHPL_RT_CALL_STACK_SIZE")) == NULL)
    size = 0;
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
        chpl_warning("Cannot parse CHPL_RT_CALL_STACK_SIZE", 0, 0);
        size = 0;
      }
    }

    if (size <= 0) {
      chpl_warning("CHPL_RT_CALL_STACK_SIZE must be > 0", 0, 0);
      size = 0;
    }

    {
      size_t max = stack_size_max();
      if (size > max) {
        snprintf(msg, sizeof(msg),
                 "CHPL_RT_CALL_STACK_SIZE must be <= %zd; using %zd",
                 max, max);
        chpl_warning(msg, 0, 0);
        size = max;
      }
    }
  }

  have_size = 1;

  return size;
}


size_t chpl_task_getDefaultCallStackSize(void)
{
  static size_t deflt;
  static int    have_deflt = 0;

  if (!have_deflt) {
#ifdef __CYGWIN__
    {
      struct rlimit rlim;
      if (getrlimit(RLIMIT_STACK, &rlim) != 0)
        chpl_internal_error("getrlimit() failed");
      if (rlim.rlim_cur == RLIM_INFINITY)
        deflt = (size_t) 2 << 20;    // 2 MiB
      else
        deflt = rlim.rlim_cur;
    }
#else
    //
    // This is the default task call stack size, in bytes, for
    // everything except Cygwin.  It was chosen because it matches a
    // common default process stack size in linux, which was inherited
    // by the fifo tasking layer, and Chapel programmers have become
    // used to that.  In the future, and as our ability to detect and
    // report task stack overflow improves, we may reduce it.
    //
    deflt = (size_t) 8 << 20;    // 8 MiB
#endif

    {
      size_t max = stack_size_max();
      if (max < deflt)
        deflt = max;
    }

    have_deflt = 1;
  }

  return deflt;
}
