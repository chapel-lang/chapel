/*
 * Copyright © 2009 CNRS
 * Copyright © 2009-2010 inria.  All rights reserved.
 * Copyright © 2009-2010 Université Bordeaux 1
 * Copyright © 2011 Cisco Systems, Inc.  All rights reserved.
 * See COPYING in top-level directory.
 */

#include <hwloc.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

int main(void)
{
  hwloc_topology_t topology;
  hwloc_bitmap_t cpuset;
  hwloc_obj_t obj;

  hwloc_topology_init(&topology);
  hwloc_topology_load(topology);
  hwloc_topology_check(topology);
  cpuset = hwloc_bitmap_alloc();
  hwloc_bitmap_set(cpuset, 0);
  obj = hwloc_topology_insert_misc_object_by_cpuset(topology, cpuset, "test");
  hwloc_bitmap_free(cpuset);
  hwloc_topology_insert_misc_object_by_parent(topology, obj, "test2");
  hwloc_topology_check(topology);
  hwloc_topology_destroy(topology);

  return 0;
}
