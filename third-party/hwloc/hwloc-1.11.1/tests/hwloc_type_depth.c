/*
 * Copyright © 2009 CNRS
 * Copyright © 2009-2014 Inria.  All rights reserved.
 * Copyright © 2009-2010 Université Bordeaux
 * Copyright © 2011 Cisco Systems, Inc.  All rights reserved.
 * See COPYING in top-level directory.
 */

#include <hwloc.h>

#include <stdio.h>
#include <assert.h>

/* check topo_get_type{,_or_above,_or_below}_depth()
 * and hwloc_get_depth_type()
 */

#define SYNTHETIC_TOPOLOGY_DESCRIPTION "machine:3 group:2 group:2 core:3 cache:2 cache:2 2"

int main(void)
{
  hwloc_topology_t topology;

  hwloc_topology_init(&topology);
  hwloc_topology_set_synthetic(topology, SYNTHETIC_TOPOLOGY_DESCRIPTION);
  hwloc_topology_load(topology);

  assert(hwloc_topology_get_depth(topology) == 8);

  assert(hwloc_get_depth_type(topology, 0) == HWLOC_OBJ_SYSTEM);
  assert(hwloc_get_depth_type(topology, 1) == HWLOC_OBJ_MACHINE);
  assert(hwloc_get_depth_type(topology, 2) == HWLOC_OBJ_GROUP);
  assert(hwloc_get_depth_type(topology, 3) == HWLOC_OBJ_GROUP);
  assert(hwloc_get_depth_type(topology, 4) == HWLOC_OBJ_CORE);
  assert(hwloc_get_depth_type(topology, 5) == HWLOC_OBJ_CACHE);
  assert(hwloc_get_depth_type(topology, 6) == HWLOC_OBJ_CACHE);
  assert(hwloc_get_depth_type(topology, 7) == HWLOC_OBJ_PU);

  assert(hwloc_get_type_depth(topology, HWLOC_OBJ_MACHINE) == 1);
  assert(hwloc_get_type_depth(topology, HWLOC_OBJ_CORE) == 4);
  assert(hwloc_get_type_depth(topology, HWLOC_OBJ_PU) == 7);

  assert(hwloc_get_type_depth(topology, HWLOC_OBJ_NUMANODE) == HWLOC_TYPE_DEPTH_UNKNOWN);
  assert(hwloc_get_type_or_above_depth(topology, HWLOC_OBJ_NUMANODE) == 3);
  assert(hwloc_get_type_or_below_depth(topology, HWLOC_OBJ_NUMANODE) == 4);
  assert(hwloc_get_type_depth(topology, HWLOC_OBJ_PACKAGE) == HWLOC_TYPE_DEPTH_UNKNOWN);
  assert(hwloc_get_type_or_above_depth(topology, HWLOC_OBJ_PACKAGE) == 3);
  assert(hwloc_get_type_or_below_depth(topology, HWLOC_OBJ_PACKAGE) == 4);
  assert(hwloc_get_type_depth(topology, HWLOC_OBJ_CACHE) == HWLOC_TYPE_DEPTH_MULTIPLE);
  assert(hwloc_get_type_or_above_depth(topology, HWLOC_OBJ_CACHE) == HWLOC_TYPE_DEPTH_MULTIPLE);
  assert(hwloc_get_type_or_below_depth(topology, HWLOC_OBJ_CACHE) == HWLOC_TYPE_DEPTH_MULTIPLE);

  hwloc_topology_destroy(topology);

  return EXIT_SUCCESS;
}
