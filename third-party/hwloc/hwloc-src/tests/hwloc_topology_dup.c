/*
 * Copyright © 2011-2015 Inria.  All rights reserved.
 * See COPYING in top-level directory.
 */

#include <hwloc.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

int main(void)
{
  static hwloc_topology_t oldtopology, topology;
  hwloc_bitmap_t cpuset = hwloc_bitmap_alloc();
  unsigned node_indexes[3], core_indexes[6];
  float node_distances[9], core_distances[36];
  unsigned i,j;
  int err;

  for(i=0; i<3; i++) {
    node_indexes[i] = i;
    for(j=0; j<3; j++)
      node_distances[i*3+j] = (i == j ? 10.f : 20.f);
  }
  for(i=0; i<6; i++) {
    core_indexes[i] = i;
    for(j=0; j<6; j++)
      core_distances[i*6+j] = (i == j ? 4.f : 8.f);
  }

  hwloc_topology_init(&oldtopology);
  printf("building fake 'node:3 core:2 pu:4' topology\n");
  hwloc_topology_set_synthetic(oldtopology, "node:3 core:2 pu:4");
  printf("adding node and core matrices\n");
  hwloc_topology_set_distance_matrix(oldtopology, HWLOC_OBJ_NUMANODE, 3, node_indexes, node_distances);
  hwloc_topology_set_distance_matrix(oldtopology, HWLOC_OBJ_CORE, 6, core_indexes, core_distances);
  hwloc_topology_load(oldtopology);

  printf("duplicating\n");
  hwloc_topology_dup(&topology, oldtopology);
  printf("destroying the old topology\n");
  hwloc_topology_destroy(oldtopology);

  /* remove the entire third node */
  printf("removing one node\n");
  hwloc_bitmap_fill(cpuset);
  hwloc_bitmap_clr_range(cpuset, 16, 23);
  err = hwloc_topology_restrict(topology, cpuset, HWLOC_RESTRICT_FLAG_ADAPT_DISTANCES);
  assert(!err);
  printf("checking the result\n");
  assert(hwloc_get_nbobjs_by_type(topology, HWLOC_OBJ_NUMANODE) == 2);

  hwloc_topology_destroy(topology);

  hwloc_bitmap_free(cpuset);

  return 0;
}
