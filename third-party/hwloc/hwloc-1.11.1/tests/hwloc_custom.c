/*
 * Copyright © 2011-2014 Inria.  All rights reserved.
 * See COPYING in top-level directory.
 */

#include <hwloc.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

int main(void)
{
  hwloc_topology_t local, global;
  hwloc_obj_t sw1, sw2, sw11, sw12, sw21, sw22, root;
  int err;

  printf("Loading the local topology...\n");
  hwloc_topology_init(&local);
  hwloc_topology_set_synthetic(local, "node:2 pack:2 cache:1 core:2 cache:2 pu:2");
  hwloc_topology_load(local);

  printf("Try to create an empty custom topology...\n");
  hwloc_topology_init(&global);
  hwloc_topology_set_custom(global);
  err = hwloc_topology_load(global);
  assert(err == -1);
  assert(errno == EINVAL);
  hwloc_topology_destroy(global);

  printf("Creating a custom topology...\n");
  hwloc_topology_init(&global);
  hwloc_topology_set_custom(global);

  printf("Inserting the local topology into the global one...\n");
  root = hwloc_get_root_obj(global);

  sw1 = hwloc_custom_insert_group_object_by_parent(global, root, 0);
  sw11 = hwloc_custom_insert_group_object_by_parent(global, sw1, 1);
  hwloc_custom_insert_topology(global, sw11, local, NULL);
  hwloc_custom_insert_topology(global, sw11, local, NULL);
  sw12 = hwloc_custom_insert_group_object_by_parent(global, sw1, 1);
  hwloc_custom_insert_topology(global, sw12, local, NULL);
  hwloc_custom_insert_topology(global, sw12, local, NULL);

  sw2 = hwloc_custom_insert_group_object_by_parent(global, root, 0);
  sw21 = hwloc_custom_insert_group_object_by_parent(global, sw2, 1);
  hwloc_custom_insert_topology(global, sw21, local, NULL);
  hwloc_custom_insert_topology(global, sw21, local, NULL);
  hwloc_custom_insert_topology(global, sw21, local, NULL);
  sw22 = hwloc_custom_insert_group_object_by_parent(global, sw2, 1);
  hwloc_custom_insert_topology(global, sw22, local, NULL); /* only one to check that it won't get merged */

  hwloc_topology_destroy(local);

  printf("Building the global topology...\n");
  hwloc_topology_load(global);
  hwloc_topology_check(global);

  assert(hwloc_topology_get_depth(global) == 10);
  assert(hwloc_get_depth_type(global, 0) == HWLOC_OBJ_SYSTEM);
  assert(hwloc_get_nbobjs_by_type(global, HWLOC_OBJ_SYSTEM) == 1);
  assert(hwloc_get_depth_type(global, 1) == HWLOC_OBJ_GROUP);
  assert(hwloc_get_nbobjs_by_depth(global, 1) == 2);
  assert(hwloc_get_depth_type(global, 2) == HWLOC_OBJ_GROUP);
  assert(hwloc_get_nbobjs_by_depth(global, 2) == 4); /* the last group of this level shouldn't be merged */
  assert(hwloc_get_depth_type(global, 3) == HWLOC_OBJ_MACHINE);
  assert(hwloc_get_nbobjs_by_type(global, HWLOC_OBJ_MACHINE) == 8);
  assert(hwloc_get_depth_type(global, 4) == HWLOC_OBJ_NODE);
  assert(hwloc_get_nbobjs_by_type(global, HWLOC_OBJ_NODE) == 16);
  assert(hwloc_get_depth_type(global, 5) == HWLOC_OBJ_PACKAGE);
  assert(hwloc_get_nbobjs_by_type(global, HWLOC_OBJ_PACKAGE) == 32);
  assert(hwloc_get_depth_type(global, 6) == HWLOC_OBJ_CACHE);
  assert(hwloc_get_nbobjs_by_depth(global, 6) == 32);
  assert(hwloc_get_depth_type(global, 7) == HWLOC_OBJ_CORE);
  assert(hwloc_get_nbobjs_by_type(global, HWLOC_OBJ_CORE) == 64);
  assert(hwloc_get_depth_type(global, 8) == HWLOC_OBJ_CACHE);
  assert(hwloc_get_nbobjs_by_depth(global, 8) == 128);
  assert(hwloc_get_depth_type(global, 9) == HWLOC_OBJ_PU);
  assert(hwloc_get_nbobjs_by_type(global, HWLOC_OBJ_PU) == 256);

  hwloc_topology_destroy(global);

  return 0;
}
