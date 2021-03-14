/*
 * Copyright © 2011-2013 Inria.  All rights reserved.
 * Copyright © 2011 Cisco Systems, Inc.  All rights reserved.
 * See COPYING in top-level directory.
 */

#include <hwloc.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

/* testing of adding/replacing/removing distance matrices
   with set_distance_matrix or the environment,
   grouping with/without accuracy
 */


int main(void)
{
  hwloc_topology_t topology;
  unsigned indexes[16];
  float distances[16*16];
  unsigned i, j;
  unsigned depth;
  unsigned width;

  /* default 2*8*1 */
  hwloc_topology_init(&topology);
  hwloc_topology_set_synthetic(topology, "node:2 core:8 pu:1");
  hwloc_topology_load(topology);
  depth = hwloc_topology_get_depth(topology);
  assert(depth == 4);
  width = hwloc_get_nbobjs_by_depth(topology, 0);
  assert(width == 1);
  width = hwloc_get_nbobjs_by_depth(topology, 1);
  assert(width == 2);
  width = hwloc_get_nbobjs_by_depth(topology, 2);
  assert(width == 16);
  width = hwloc_get_nbobjs_by_depth(topology, 3);
  assert(width == 16);
  hwloc_topology_destroy(topology);

  /* 2*8*1 and group 8cores as 2*2*2 */
  hwloc_topology_init(&topology);
  hwloc_topology_set_synthetic(topology, "node:2 core:8 pu:1");
  for(i=0; i<16; i++) {
    indexes[i] = i;
    for(j=0; j<16; j++)
      if (i==j)
        distances[i+16*j] = distances[j+16*i] = 3;
      else if (i/2==j/2)
        distances[i+16*j] = distances[j+16*i] = 5;
      else if (i/4==j/4)
        distances[i+16*j] = distances[j+16*i] = 7;
      else
        distances[i+16*j] = distances[j+16*i] = 9;
  }
  assert(!hwloc_topology_set_distance_matrix(topology, HWLOC_OBJ_CORE, 16, indexes, distances));
  hwloc_topology_load(topology);
  depth = hwloc_topology_get_depth(topology);
  assert(depth == 6);
  width = hwloc_get_nbobjs_by_depth(topology, 0);
  assert(width == 1);
  width = hwloc_get_nbobjs_by_depth(topology, 1);
  assert(width == 2);
  width = hwloc_get_nbobjs_by_depth(topology, 2);
  assert(width == 4);
  width = hwloc_get_nbobjs_by_depth(topology, 3);
  assert(width == 8);
  width = hwloc_get_nbobjs_by_depth(topology, 4);
  assert(width == 16);
  hwloc_topology_destroy(topology);

  /* play with accuracy */
  distances[0] = 2.9; /* diagonal, instead of 3 (0.0333% error) */
  distances[1] = 5.1; distances[16] = 5.2; /* smallest group, instead of 5 (0.02% error) */
  hwloc_topology_init(&topology);
  hwloc_topology_set_synthetic(topology, "node:2 core:8 pu:1");
  assert(!hwloc_topology_set_distance_matrix(topology, HWLOC_OBJ_CORE, 16, indexes, distances));
  putenv("HWLOC_GROUPING_ACCURACY=0.1"); /* ok */
  hwloc_topology_load(topology);
  depth = hwloc_topology_get_depth(topology);
  assert(depth == 6);
  hwloc_topology_destroy(topology);

  hwloc_topology_init(&topology);
  hwloc_topology_set_synthetic(topology, "node:2 core:8 pu:1");
  assert(!hwloc_topology_set_distance_matrix(topology, HWLOC_OBJ_CORE, 16, indexes, distances));
  putenv("HWLOC_GROUPING_ACCURACY=try"); /* ok */
  hwloc_topology_load(topology);
  depth = hwloc_topology_get_depth(topology);
  assert(depth == 6);
  hwloc_topology_destroy(topology);

  hwloc_topology_init(&topology);
  hwloc_topology_set_synthetic(topology, "node:2 core:8 pu:1");
  assert(!hwloc_topology_set_distance_matrix(topology, HWLOC_OBJ_CORE, 16, indexes, distances));
  putenv("HWLOC_GROUPING_ACCURACY=0.01"); /* too small, cannot group */
  hwloc_topology_load(topology);
  depth = hwloc_topology_get_depth(topology);
  assert(depth == 4);
  hwloc_topology_destroy(topology);

  hwloc_topology_init(&topology);
  hwloc_topology_set_synthetic(topology, "node:2 core:8 pu:1");
  assert(!hwloc_topology_set_distance_matrix(topology, HWLOC_OBJ_CORE, 16, indexes, distances));
  putenv("HWLOC_GROUPING_ACCURACY=0"); /* full accuracy, cannot group */
  hwloc_topology_load(topology);
  depth = hwloc_topology_get_depth(topology);
  assert(depth == 4);
  hwloc_topology_destroy(topology);

  /* revert to default 2*8*1 */
  hwloc_topology_init(&topology);
  hwloc_topology_set_synthetic(topology, "node:2 core:8 pu:1");
  assert(!hwloc_topology_set_distance_matrix(topology, HWLOC_OBJ_CORE, 16, indexes, distances));
  assert(!hwloc_topology_set_distance_matrix(topology, HWLOC_OBJ_CORE, 0, NULL, NULL));
  hwloc_topology_load(topology);
  depth = hwloc_topology_get_depth(topology);
  assert(depth == 4);
  width = hwloc_get_nbobjs_by_depth(topology, 0);
  assert(width == 1);
  width = hwloc_get_nbobjs_by_depth(topology, 1);
  assert(width == 2);
  width = hwloc_get_nbobjs_by_depth(topology, 2);
  assert(width == 16);
  width = hwloc_get_nbobjs_by_depth(topology, 3);
  assert(width == 16);
  hwloc_topology_destroy(topology);

  /* default 2*4*4 */
  hwloc_topology_init(&topology);
  hwloc_topology_set_synthetic(topology, "node:2 core:4 pu:4");
  hwloc_topology_load(topology);
  depth = hwloc_topology_get_depth(topology);
  assert(depth == 4);
  width = hwloc_get_nbobjs_by_depth(topology, 0);
  assert(width == 1);
  width = hwloc_get_nbobjs_by_depth(topology, 1);
  assert(width == 2);
  width = hwloc_get_nbobjs_by_depth(topology, 2);
  assert(width == 8);
  width = hwloc_get_nbobjs_by_depth(topology, 3);
  assert(width == 32);
  hwloc_topology_destroy(topology);

  /* 2*4*4 and group 4cores as 2*2 */
  hwloc_topology_init(&topology);
  hwloc_topology_set_synthetic(topology, "node:2 core:4 pu:4");
  putenv("HWLOC_Core_DISTANCES=0,1,2,3,4,5,6,7:4*2");
  hwloc_topology_load(topology);
  depth = hwloc_topology_get_depth(topology);
  assert(depth == 5);
  width = hwloc_get_nbobjs_by_depth(topology, 0);
  assert(width == 1);
  width = hwloc_get_nbobjs_by_depth(topology, 1);
  assert(width == 2);
  width = hwloc_get_nbobjs_by_depth(topology, 2);
  assert(width == 4);
  width = hwloc_get_nbobjs_by_depth(topology, 3);
  assert(width == 8);
  width = hwloc_get_nbobjs_by_depth(topology, 4);
  assert(width == 32);
  hwloc_topology_destroy(topology);

  /* 2*4*4 and group 4cores as 2*2 and 4PUs as 2*2 */
  hwloc_topology_init(&topology);
  hwloc_topology_set_synthetic(topology, "node:2 core:4 pu:4");
  putenv("HWLOC_PU_DISTANCES=0-31:16*2");
  hwloc_topology_load(topology);
  depth = hwloc_topology_get_depth(topology);
  assert(depth == 6);
  width = hwloc_get_nbobjs_by_depth(topology, 0);
  assert(width == 1);
  width = hwloc_get_nbobjs_by_depth(topology, 1);
  assert(width == 2);
  width = hwloc_get_nbobjs_by_depth(topology, 2);
  assert(width == 4);
  width = hwloc_get_nbobjs_by_depth(topology, 3);
  assert(width == 8);
  width = hwloc_get_nbobjs_by_depth(topology, 4);
  assert(width == 16);
  width = hwloc_get_nbobjs_by_depth(topology, 5);
  assert(width == 32);
  hwloc_topology_destroy(topology);

  /* replace previous core distances with useless ones (grouping as the existing numa nodes) */
  /* 2*4*4 and group 4PUs as 2*2 */
  hwloc_topology_init(&topology);
  hwloc_topology_set_synthetic(topology, "node:2 core:4 pu:4");
  putenv("HWLOC_Core_DISTANCES=0-7:2*4");
  hwloc_topology_load(topology);
  depth = hwloc_topology_get_depth(topology);
  assert(depth == 5);
  width = hwloc_get_nbobjs_by_depth(topology, 0);
  assert(width == 1);
  width = hwloc_get_nbobjs_by_depth(topology, 1);
  assert(width == 2);
  width = hwloc_get_nbobjs_by_depth(topology, 2);
  assert(width == 8);
  width = hwloc_get_nbobjs_by_depth(topology, 3);
  assert(width == 16);
  width = hwloc_get_nbobjs_by_depth(topology, 4);
  assert(width == 32);
  hwloc_topology_destroy(topology);

  /* clear everything */
  /* default 2*4*4 */
  hwloc_topology_init(&topology);
  hwloc_topology_set_synthetic(topology, "node:2 core:4 pu:4");
  putenv("HWLOC_Core_DISTANCES=none");
  putenv("HWLOC_PU_DISTANCES=none");
  hwloc_topology_load(topology);
  depth = hwloc_topology_get_depth(topology);
  assert(depth == 4);
  width = hwloc_get_nbobjs_by_depth(topology, 0);
  assert(width == 1);
  width = hwloc_get_nbobjs_by_depth(topology, 1);
  assert(width == 2);
  width = hwloc_get_nbobjs_by_depth(topology, 2);
  assert(width == 8);
  width = hwloc_get_nbobjs_by_depth(topology, 3);
  assert(width == 32);
  hwloc_topology_destroy(topology);

  /* buggy tests */
  hwloc_topology_init(&topology);
  hwloc_topology_set_synthetic(topology, "node:2 core:4 pu:4");
  assert(hwloc_topology_set_distance_matrix(topology, HWLOC_OBJ_CORE, 16, NULL, NULL) < 0);
  assert(hwloc_topology_set_distance_matrix(topology, HWLOC_OBJ_CORE, 0, indexes, NULL) < 0);
  indexes[1] = 0;
  assert(hwloc_topology_set_distance_matrix(topology, HWLOC_OBJ_CORE, 16, indexes, distances) < 0);
  hwloc_topology_destroy(topology);

  return 0;
}
