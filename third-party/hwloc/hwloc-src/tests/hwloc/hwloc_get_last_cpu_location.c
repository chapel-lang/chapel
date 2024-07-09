/*
 * Copyright © 2011-2020 Inria.  All rights reserved.
 * Copyright © 2011 Université Bordeaux.  All rights reserved.
 * See COPYING in top-level directory.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>

#include "hwloc.h"

hwloc_topology_t topology;
const struct hwloc_topology_support *support;

static int checkprocincluded;

/* check that a bound process execs on a non-empty cpuset included in the binding */
static int check(hwloc_const_cpuset_t set, int flags)
{
  hwloc_cpuset_t last;
  int ret;

  printf("  binding\n");
  ret = hwloc_set_cpubind(topology, set, flags);
  if (ret)
    return 0;

  printf("  getting cpu location\n");
  last = hwloc_bitmap_alloc();
  ret = hwloc_get_last_cpu_location(topology, last, flags);
  assert(!ret);
  assert(!hwloc_bitmap_iszero(last));

  if (flags == HWLOC_CPUBIND_THREAD || checkprocincluded) {
    printf("  checking inclusion\n");
    assert(hwloc_bitmap_isincluded(last, set));
  }

  hwloc_bitmap_free(last);
  return 0;
}

static int checkall(hwloc_const_cpuset_t set)
{
  if (support->cpubind->get_thisthread_last_cpu_location) {
    printf(" with HWLOC_CPUBIND_THREAD...\n");
    check(set, HWLOC_CPUBIND_THREAD);
  }
  if (support->cpubind->get_thisproc_last_cpu_location) {
    printf(" with HWLOC_CPUBIND_PROCESS...\n");
    check(set, HWLOC_CPUBIND_PROCESS);
  }
  if (support->cpubind->get_thisthread_last_cpu_location || support->cpubind->get_thisproc_last_cpu_location) {
    printf(" with flags 0...\n");
    check(set, 0);
  }
  return 0;
}

static int has_unexpected_threads(void)
{
#ifdef HWLOC_LINUX_SYS
  struct stat stbuf;
  int err = stat("/proc/self/task", &stbuf);
  if (!err && stbuf.st_nlink > 3) {
    printf("program has multiple threads, disabling process-wide binding/cpulocation checks.\n");
    return 1;
  }
#endif
  /* if the problem ever occurs on !Linux,
   * we'll use HWLOC_TEST_DONTCHECK_PROC_CPULOCATION=1 until detecting it here
   */
  return 0;
}

int main(void)
{
  unsigned depth;
  hwloc_obj_t obj;

  checkprocincluded = !has_unexpected_threads()
    && getenv("HWLOC_TEST_DONTCHECK_PROC_CPULOCATION") == NULL;

  hwloc_topology_init(&topology);
  hwloc_topology_load(topology);

  support = hwloc_topology_get_support(topology);

  /* check at top level */
  printf("testing at top level\n");
  obj = hwloc_get_root_obj(topology);
  checkall(obj->cpuset);

  depth = hwloc_topology_get_depth(topology);
  /* check at intermediate level if it exists */
  if (depth >= 3) {
    printf("testing at depth %u\n", (depth-1)/2);
    obj = NULL;
    while ((obj = hwloc_get_next_obj_by_depth(topology, (depth-1)/2, obj)) != NULL)
      checkall(obj->cpuset);
  }

  /* check at bottom level */
  printf("testing at bottom level\n");
  obj = NULL;
  while ((obj = hwloc_get_next_obj_by_type(topology, HWLOC_OBJ_PU, obj)) != NULL)
    checkall(obj->cpuset);

  hwloc_topology_destroy(topology);
  return 0;
}
