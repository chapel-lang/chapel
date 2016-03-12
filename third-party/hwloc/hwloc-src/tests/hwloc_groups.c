/*
 * Copyright © 2011-2014 Inria.  All rights reserved.
 * See COPYING in top-level directory.
 */

#include <hwloc.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

/* intensive testing of two grouping cases (2+1 and 2+2+1) */

int main(void)
{
  hwloc_topology_t topology;
  hwloc_obj_t obj;
  unsigned indexes[5];
  float distances[5*5];
  unsigned depth;
  unsigned width;

  /* group 3 numa nodes as 1 group of 2 and 1 on the side */
  hwloc_topology_init(&topology);
  hwloc_topology_set_synthetic(topology, "node:3 pu:1");
  indexes[0] = 0; indexes[1] = 1; indexes[2] = 2;
  distances[0] = 1; distances[1] = 4; distances[2] = 4;
  distances[3] = 4; distances[4] = 1; distances[5] = 2;
  distances[6] = 4; distances[7] = 2; distances[8] = 1;
  hwloc_topology_set_distance_matrix(topology, HWLOC_OBJ_PU, 3, indexes, distances);
  hwloc_topology_load(topology);
  /* 2 groups at depth 1 */
  depth = hwloc_get_type_depth(topology, HWLOC_OBJ_GROUP);
  assert(depth == 1);
  width = hwloc_get_nbobjs_by_depth(topology, depth);
  assert(width == 1);
  /* 3 node at depth 2 */
  depth = hwloc_get_type_depth(topology, HWLOC_OBJ_NUMANODE);
  assert(depth == 2);
  width = hwloc_get_nbobjs_by_depth(topology, depth);
  assert(width == 3);
  /* find the root obj */
  obj = hwloc_get_root_obj(topology);
  assert(obj->arity == 2);
  /* check its children */
  assert(obj->children[0]->type == HWLOC_OBJ_NUMANODE);
  assert(obj->children[0]->depth == 2);
  assert(obj->children[0]->arity == 1);
  assert(obj->children[1]->type == HWLOC_OBJ_GROUP);
  assert(obj->children[1]->depth == 1);
  assert(obj->children[1]->arity == 2);
  hwloc_topology_destroy(topology);

  /* group 5 packages as 2 group of 2 and 1 on the side, all of them below a common node object */
  hwloc_topology_init(&topology);
  hwloc_topology_set_synthetic(topology, "node:1 pack:5 pu:1");
  indexes[0] = 0; indexes[1] = 1; indexes[2] = 2; indexes[3] = 3; indexes[4] = 4;
  distances[ 0] = 1; distances[ 1] = 2; distances[ 2] = 4; distances[ 3] = 4; distances[ 4] = 4;
  distances[ 5] = 2; distances[ 6] = 1; distances[ 7] = 4; distances[ 8] = 4; distances[ 9] = 4;
  distances[10] = 4; distances[11] = 4; distances[12] = 1; distances[13] = 4; distances[14] = 4;
  distances[15] = 4; distances[16] = 4; distances[17] = 4; distances[18] = 1; distances[19] = 2;
  distances[20] = 4; distances[21] = 4; distances[22] = 4; distances[23] = 2; distances[24] = 1;
  hwloc_topology_set_distance_matrix(topology, HWLOC_OBJ_PACKAGE, 5, indexes, distances);
  hwloc_topology_load(topology);
  /* 1 node at depth 1 */
  depth = hwloc_get_type_depth(topology, HWLOC_OBJ_NUMANODE);
  assert(depth == 1);
  width = hwloc_get_nbobjs_by_depth(topology, depth);
  assert(width == 1);
  /* 2 groups at depth 2 */
  depth = hwloc_get_type_depth(topology, HWLOC_OBJ_GROUP);
  assert(depth == 2);
  width = hwloc_get_nbobjs_by_depth(topology, depth);
  assert(width == 2);
  /* 5 packages at depth 3 */
  depth = hwloc_get_type_depth(topology, HWLOC_OBJ_PACKAGE);
  assert(depth == 3);
  width = hwloc_get_nbobjs_by_depth(topology, depth);
  assert(width == 5);
  /* find the node obj */
  obj = hwloc_get_root_obj(topology);
  assert(obj->arity == 1);
  obj = obj->children[0];
  assert(obj->type == HWLOC_OBJ_NUMANODE);
  assert(obj->arity == 3);
  /* check its children */
  assert(obj->children[0]->type == HWLOC_OBJ_GROUP);
  assert(obj->children[0]->depth == 2);
  assert(obj->children[0]->arity == 2);
  assert(obj->children[1]->type == HWLOC_OBJ_PACKAGE);
  assert(obj->children[1]->depth == 3);
  assert(obj->children[1]->arity == 1);
  assert(obj->children[2]->type == HWLOC_OBJ_GROUP);
  assert(obj->children[2]->depth == 2);
  assert(obj->children[2]->arity == 2);
  hwloc_topology_destroy(topology);

  return 0;
}
