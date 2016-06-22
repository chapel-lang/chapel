/*
 * Copyright © 2011 inria.  All rights reserved.
 * Copyright © 2011 Université Bordeaux.  All rights reserved.
 * See COPYING in top-level directory.
 */

#include <stdio.h>
#include <string.h>
#include <errno.h>

#include <hwloc.h>

hwloc_topology_t topology;
const struct hwloc_topology_support *support;

/* check that a bound process execs on a non-empty cpuset included in the binding */
static int check(hwloc_const_cpuset_t set, int flags)
{
  hwloc_cpuset_t last;
  int ret;

  ret = hwloc_set_cpubind(topology, set, flags);
  if (ret)
    return 0;

  last = hwloc_bitmap_alloc();
  ret = hwloc_get_last_cpu_location(topology, last, flags);
  assert(!ret);
  assert(!hwloc_bitmap_iszero(last));
  assert(hwloc_bitmap_isincluded(last, set));

  hwloc_bitmap_free(last);
  return 0;
}

static int checkall(hwloc_const_cpuset_t set)
{
  if (support->cpubind->get_thisthread_last_cpu_location)
    check(set, HWLOC_CPUBIND_THREAD);
  if (support->cpubind->get_thisproc_last_cpu_location)
    check(set, HWLOC_CPUBIND_PROCESS);
  if (support->cpubind->get_thisthread_last_cpu_location || support->cpubind->get_thisproc_last_cpu_location)
    check(set, 0);
  return 0;
}

int main(void)
{
  unsigned depth;
  hwloc_obj_t obj;

  hwloc_topology_init(&topology);
  hwloc_topology_load(topology);

  support = hwloc_topology_get_support(topology);

  /* check at top level */
  obj = hwloc_get_root_obj(topology);
  checkall(obj->cpuset);

  depth = hwloc_topology_get_depth(topology);
  /* check at intermediate level if it exists */
  if (depth >= 3) {
    obj = NULL;
    while ((obj = hwloc_get_next_obj_by_depth(topology, (depth-1)/2, obj)) != NULL)
      checkall(obj->cpuset);
  }

  /* check at bottom level */
  obj = NULL;
  while ((obj = hwloc_get_next_obj_by_type(topology, HWLOC_OBJ_PU, obj)) != NULL)
    checkall(obj->cpuset);

  hwloc_topology_destroy(topology);
  return 0;
}
