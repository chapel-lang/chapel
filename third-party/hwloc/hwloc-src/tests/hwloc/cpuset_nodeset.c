/*
 * Copyright © 2020 Inria.  All rights reserved.
 * See COPYING in top-level directory.
 */

#include "hwloc.h"

#include <assert.h>

/* check cpuset_from/to_nodeset */

int main(void)
{
  hwloc_topology_t topology;
  hwloc_bitmap_t cpuset, nodeset;
  int err;

  err = hwloc_topology_init(&topology);
  assert(!err);
  err = hwloc_topology_set_synthetic(topology, "pack:2 [numa] l3:2 [numa] pu:2");
  assert(!err);
  err = hwloc_topology_load(topology);
  assert(!err);

  cpuset = hwloc_bitmap_alloc();
  assert(cpuset);
  nodeset = hwloc_bitmap_alloc();
  assert(nodeset);

  /* check empty sets */
  hwloc_bitmap_zero(cpuset);
  hwloc_bitmap_zero(nodeset);
  err = hwloc_cpuset_to_nodeset(topology, cpuset, nodeset);
  assert(!err);
  assert(hwloc_bitmap_iszero(cpuset));
  assert(hwloc_bitmap_iszero(nodeset));
  err = hwloc_cpuset_from_nodeset(topology, cpuset, nodeset);
  assert(!err);
  assert(hwloc_bitmap_iszero(cpuset));
  assert(hwloc_bitmap_iszero(nodeset));

  /* check full topology sets */
  hwloc_bitmap_zero(nodeset);
  err = hwloc_cpuset_to_nodeset(topology, hwloc_topology_get_topology_cpuset(topology), nodeset);
  assert(!err);
  assert(hwloc_bitmap_isequal(nodeset, hwloc_topology_get_topology_nodeset(topology)));

  hwloc_bitmap_zero(cpuset);
  err = hwloc_cpuset_from_nodeset(topology, cpuset, hwloc_topology_get_topology_nodeset(topology));
  assert(!err);
  assert(hwloc_bitmap_isequal(cpuset, hwloc_topology_get_topology_cpuset(topology)));

  /* one PU per L3 in single package */
  hwloc_bitmap_zero(cpuset);
  hwloc_bitmap_set_range(cpuset, 1, 2);
  hwloc_bitmap_zero(nodeset);
  err = hwloc_cpuset_to_nodeset(topology, cpuset, nodeset);
  assert(!err);
  assert(hwloc_bitmap_weight(nodeset) == 3);
  assert(hwloc_bitmap_isset(nodeset, 0));
  assert(hwloc_bitmap_isset(nodeset, 1));
  assert(hwloc_bitmap_isset(nodeset, 2));

  /* 1 large node + 1 of its small node */
  hwloc_bitmap_zero(nodeset);
  hwloc_bitmap_set(nodeset, 3);
  hwloc_bitmap_set(nodeset, 5);
  hwloc_bitmap_zero(cpuset);
  err = hwloc_cpuset_from_nodeset(topology, cpuset, nodeset);
  assert(!err);
  assert(hwloc_bitmap_weight(cpuset) == 4);
  assert(hwloc_bitmap_isset(cpuset, 4));
  assert(hwloc_bitmap_isset(cpuset, 5));
  assert(hwloc_bitmap_isset(cpuset, 6));
  assert(hwloc_bitmap_isset(cpuset, 7));

  /* remove PUs from last (small) node */
  err = hwloc_bitmap_copy(cpuset, hwloc_topology_get_topology_cpuset(topology));
  assert(!err);
  err = hwloc_bitmap_clr_range(cpuset, 6, 7);
  assert(!err);
  hwloc_topology_restrict(topology, cpuset, 0);
  assert(!err);
  /* remove first node and first large node, PU 0-1 are not covered by a node anymore */
  err = hwloc_bitmap_copy(nodeset, hwloc_topology_get_topology_nodeset(topology));
  assert(!err);
  err = hwloc_bitmap_clr(nodeset, 0); /* first small node */
  err = hwloc_bitmap_clr(nodeset, 2); /* first large node */
  assert(!err);
  hwloc_topology_restrict(topology, nodeset, HWLOC_RESTRICT_FLAG_BYNODESET);
  assert(!err);

  /* check empty sets */
  hwloc_bitmap_zero(cpuset);
  hwloc_bitmap_zero(nodeset);
  err = hwloc_cpuset_to_nodeset(topology, cpuset, nodeset);
  assert(!err);
  assert(hwloc_bitmap_iszero(cpuset));
  assert(hwloc_bitmap_iszero(nodeset));
  err = hwloc_cpuset_from_nodeset(topology, cpuset, nodeset);
  assert(!err);
  assert(hwloc_bitmap_iszero(cpuset));
  assert(hwloc_bitmap_iszero(nodeset));

  /* check full topology sets */
  hwloc_bitmap_zero(nodeset);
  err = hwloc_cpuset_to_nodeset(topology, hwloc_topology_get_topology_cpuset(topology), nodeset);
  assert(!err);
  assert(hwloc_bitmap_weight(nodeset) == 3);
  /* node P#0 doesn't exist anymore */
  assert(hwloc_bitmap_isset(nodeset, 1));
  /* node P#2 doesn't exist anymore */
  assert(hwloc_bitmap_isset(nodeset, 3));
  /* node P#4 has no PUs */
  assert(hwloc_bitmap_isset(nodeset, 5));

  hwloc_bitmap_zero(cpuset);
  err = hwloc_cpuset_from_nodeset(topology, cpuset, hwloc_topology_get_topology_nodeset(topology));
  assert(!err);
  assert(hwloc_bitmap_weight(cpuset) == 4);
  /* PU P#0-1 not covered anymore */
  assert(hwloc_bitmap_isset(cpuset, 2));
  assert(hwloc_bitmap_isset(cpuset, 3));
  assert(hwloc_bitmap_isset(cpuset, 4));
  assert(hwloc_bitmap_isset(cpuset, 5));
  /* PU P#6-7 don't exist anymore */

  hwloc_bitmap_free(cpuset);
  hwloc_bitmap_free(nodeset);

  hwloc_topology_destroy(topology);
  return 0;
}
