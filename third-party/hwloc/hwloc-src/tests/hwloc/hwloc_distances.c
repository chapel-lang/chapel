/*
 * Copyright © 2010-2021 Inria.  All rights reserved.
 * Copyright © 2011 Cisco Systems, Inc.  All rights reserved.
 * See COPYING in top-level directory.
 */

#include "hwloc.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <assert.h>

/* test setting/retrieving distances */

static void print_distances(const struct hwloc_distances_s *distances)
{
  unsigned nbobjs = distances->nbobjs;
  unsigned i, j;

  printf("     ");
  /* column header */
  for(j=0; j<nbobjs; j++)
    printf(" % 5d", (int) distances->objs[j]->os_index);
  printf("\n");

  /* each line */
  for(i=0; i<nbobjs; i++) {
    /* row header */
    printf("% 5d", (int) distances->objs[i]->os_index);
    /* each value */
    for(j=0; j<nbobjs; j++)
      printf(" % 5d", (int) distances->values[i*nbobjs+j]);
    printf("\n");
  }
}

static void check_distances(hwloc_topology_t topology, int depth, unsigned expected)
{
  struct hwloc_distances_s *distances[2];
 unsigned nr = 0;
  int err = hwloc_distances_get_by_depth(topology, depth, &nr, distances, 0, 0);
  assert(!err);
  assert(nr == expected);
  if (!nr) {
    printf("No distance at depth %d\n", depth);
    return;
  }
  nr = 2;
  err = hwloc_distances_get_by_depth(topology, depth, &nr, distances, 0, 0);
  assert(!err);
  printf("distance matrix for depth %d:\n", depth);
  print_distances(distances[0]);
  assert(!hwloc_distances_get_name(topology, distances[0]));
  hwloc_distances_release(topology, distances[0]);
  if (nr > 1) {
    print_distances(distances[1]);
    assert(!hwloc_distances_get_name(topology, distances[1]));
    hwloc_distances_release(topology, distances[1]);
  }
}

int main(void)
{
  hwloc_topology_t topology;
  struct hwloc_distances_s *distances[2];
  hwloc_obj_t objs[16];
  hwloc_uint64_t values[16*16], value1, value2;
  hwloc_distances_add_handle_t handle;
  int topodepth;
  unsigned i, j, k, nr;
  int err;

  hwloc_topology_init(&topology);
  hwloc_topology_set_synthetic(topology, "node:4 core:4 pu:1");
  hwloc_topology_load(topology);

  nr = 0;
  err = hwloc_distances_get(topology, &nr, distances, 0, 0);
  assert(!err);
  assert(!nr);
  if (!nr)
    printf("No distance\n");

  printf("\nInserting NUMA distances\n");
  for(i=0; i<4; i++)
    objs[i] = hwloc_get_obj_by_type(topology, HWLOC_OBJ_NUMANODE, i);
  /* matrix 2*2 */
  for(i=0; i<16; i++)
    values[i] = 8;
  values[0+4*1] = 4;
  values[1+4*0] = 4;
  values[2+4*3] = 4;
  values[3+4*2] = 4;
  for(i=0; i<4; i++)
    values[i+4*i] = 1;
  handle = hwloc_distances_add_create(topology, NULL,
                                      HWLOC_DISTANCES_KIND_MEANS_LATENCY|HWLOC_DISTANCES_KIND_FROM_USER,
                                      0);
  assert(handle);
  err = hwloc_distances_add_values(topology, handle, 4, objs, values, 0);
  assert(!err);
  err = hwloc_distances_add_commit(topology, handle,
                                   HWLOC_DISTANCES_ADD_FLAG_GROUP);
  assert(!err);

  err = hwloc_topology_refresh(topology);
  assert(!err);

  topodepth = hwloc_topology_get_depth(topology);
  assert(topodepth == 5);
  check_distances(topology, 0, 0);
  check_distances(topology, 1, 0);
  check_distances(topology, 2, 0);
  check_distances(topology, 3, 0);
  check_distances(topology, 4, 0);
  check_distances(topology, HWLOC_TYPE_DEPTH_NUMANODE, 1);

  /* check numa distances */
  printf("Checking NUMA distances\n");
  nr = 1;
  err = hwloc_distances_get_by_type(topology, HWLOC_OBJ_NUMANODE, &nr, distances, 0, 0);
  assert(!err);
  assert(nr == 1);
  assert(distances[0]);
  assert(distances[0]->objs);
  assert(distances[0]->values);
  assert(distances[0]->kind == (HWLOC_DISTANCES_KIND_MEANS_LATENCY|HWLOC_DISTANCES_KIND_FROM_USER));
  /* check helpers */
  assert(hwloc_distances_obj_index(distances[0], hwloc_get_obj_by_type(topology, HWLOC_OBJ_NUMANODE, 2)) == 2);
  err = hwloc_distances_obj_pair_values(distances[0],
					hwloc_get_obj_by_type(topology, HWLOC_OBJ_NUMANODE, 1),
					hwloc_get_obj_by_type(topology, HWLOC_OBJ_NUMANODE, 2),
					&value1, &value2);
  assert(!err);
  assert(value1 == values[1*4+2]);
  assert(value2 == values[2*4+1]);
  /* check helpers on errors */
  assert(hwloc_distances_obj_index(distances[0], hwloc_get_obj_by_type(topology, HWLOC_OBJ_PU, 0)) == -1);
  err = hwloc_distances_obj_pair_values(distances[0],
					hwloc_get_obj_by_type(topology, HWLOC_OBJ_PU, 1),
					hwloc_get_obj_by_type(topology, HWLOC_OBJ_PU, 2),
					&value1, &value2);
  assert(err == -1);
  /* check that some random values are ok */
  assert(distances[0]->values[0] == 1); /* diagonal */
  assert(distances[0]->values[4] == 4); /* same group */
  assert(distances[0]->values[6] == 8); /* different group */
  assert(distances[0]->values[9] == 8); /* different group */
  assert(distances[0]->values[10] == 1); /* diagonal */
  assert(distances[0]->values[14] == 4); /* same group */
  hwloc_distances_release(topology, distances[0]);

  printf("\nInserting PU distances\n");
  /* matrix 4*2*2 */
  for(i=0; i<16; i++)
    objs[i] = hwloc_get_obj_by_type(topology, HWLOC_OBJ_PU, i);
  for(i=0; i<256; i++)
    values[i] = 8;
  for(i=0; i<4; i++) {
    for(j=0; j<4; j++)
      for(k=0; k<4; k++)
      values[i*64+i*4+16*j+k] = 4;
    values[i*64+i*4+1] = 2;
    values[i*64+i*4+16] = 2;
    values[i*64+i*4+2*16+3] = 2;
    values[i*64+i*4+3*16+2] = 2;
  }
  for(i=0; i<16; i++)
    values[i+16*i] = 1;
  handle = hwloc_distances_add_create(topology, NULL,
                                      HWLOC_DISTANCES_KIND_MEANS_LATENCY|HWLOC_DISTANCES_KIND_FROM_USER,
                                      0);
  assert(handle);
  err = hwloc_distances_add_values(topology, handle, 16, objs, values, 0);
  assert(!err);
  err = hwloc_distances_add_commit(topology, handle,
                                   HWLOC_DISTANCES_ADD_FLAG_GROUP);
  assert(!err);

  topodepth = hwloc_topology_get_depth(topology);
  assert(topodepth == 6);
  check_distances(topology, 0, 0);
  check_distances(topology, 1, 0);
  check_distances(topology, 2, 0);
  check_distances(topology, 3, 0);
  check_distances(topology, 4, 0);
  check_distances(topology, 5, 1);
  check_distances(topology, HWLOC_TYPE_DEPTH_NUMANODE, 1);

  /* check PU distances */
  printf("Checking PU distances\n");
  nr = 1;
  err = hwloc_distances_get_by_type(topology, HWLOC_OBJ_PU, &nr, distances, 0, 0);
  assert(!err);
  assert(nr == 1);
  assert(distances[0]);
  assert(distances[0]->values);
  assert(distances[0]->kind == (HWLOC_DISTANCES_KIND_MEANS_LATENCY|HWLOC_DISTANCES_KIND_FROM_USER));
  /* check that some random values are ok */
  assert(distances[0]->values[0] == 1); /* diagonal */
  assert(distances[0]->values[1] == 2); /* same group */
  assert(distances[0]->values[3] == 4); /* same biggroup */
  assert(distances[0]->values[15] == 8); /* different biggroup */
  assert(distances[0]->values[250] == 8); /* different biggroup */
  assert(distances[0]->values[253] == 4); /* same group */
  assert(distances[0]->values[254] == 2); /* same biggroup */
  assert(distances[0]->values[255] == 1); /* diagonal */
  hwloc_distances_release(topology, distances[0]);

  printf("\nInserting 2nd PU distances\n");
  /* matrix 4*1 */
  for(i=0; i<4; i++)
    objs[i] = hwloc_get_obj_by_type(topology, HWLOC_OBJ_PU, i);
  for(i=0; i<16; i++)
    values[i] = 3;
  for(i=0; i<4; i++)
    values[i+4*i] = 7;
  handle = hwloc_distances_add_create(topology, NULL,
                                      HWLOC_DISTANCES_KIND_MEANS_BANDWIDTH|HWLOC_DISTANCES_KIND_FROM_USER,
                                      0);
  assert(handle);
  err = hwloc_distances_add_values(topology, handle, 4, objs, values, 0);
  assert(!err);
  err = hwloc_distances_add_commit(topology, handle,
                                   HWLOC_DISTANCES_ADD_FLAG_GROUP);
  assert(!err);

  topodepth = hwloc_topology_get_depth(topology);
  assert(topodepth == 6);
  check_distances(topology, 0, 0);
  check_distances(topology, 1, 0);
  check_distances(topology, 2, 0);
  check_distances(topology, 3, 0);
  check_distances(topology, 4, 0);
  check_distances(topology, 5, 2);
  check_distances(topology, HWLOC_TYPE_DEPTH_NUMANODE, 1);

  /* check PU distances */
  printf("Checking 2nd PU distances\n");
  nr = 2;
  err = hwloc_distances_get_by_type(topology, HWLOC_OBJ_PU, &nr, distances, 0, 0);
  assert(!err);
  assert(nr == 2);
  assert(distances[1]);
  assert(distances[1]->values);
  assert(distances[1]->kind == (HWLOC_DISTANCES_KIND_MEANS_BANDWIDTH|HWLOC_DISTANCES_KIND_FROM_USER));
  /* check that some random values are ok */
  assert(distances[1]->values[0] == 7); /* diagonal */
  assert(distances[1]->values[1] == 3); /* other */
  assert(distances[1]->values[3] == 3); /* other */
  assert(distances[1]->values[15] == 7); /* diagonal */
  hwloc_distances_release(topology, distances[0]);
  hwloc_distances_release(topology, distances[1]);

  /* inserting heterogeneous distance */
  printf("\nInserting heterogeneous distances\n");
  objs[0] = hwloc_get_obj_by_type(topology, HWLOC_OBJ_NUMANODE, 0);
  objs[1] = hwloc_get_obj_by_type(topology, HWLOC_OBJ_CORE, 1);
  objs[2] = hwloc_get_obj_by_depth(topology, 1, 0);
  for(i=0; i<3; i++) {
    for(j=0; j<3; j++)
      values[i*3+j] = 10;
    values[i*3+i] = 5;
  }
  handle = hwloc_distances_add_create(topology, NULL,
                                      HWLOC_DISTANCES_KIND_MEANS_BANDWIDTH|HWLOC_DISTANCES_KIND_FROM_USER,
                                      0);
  assert(handle);
  err = hwloc_distances_add_values(topology, handle, 3, objs, values, 0);
  assert(!err);
  err = hwloc_distances_add_commit(topology, handle,
                                   0);
  assert(!err);

  /* check distances by kind */
  nr = 2;
  err = hwloc_distances_get(topology, &nr, distances, HWLOC_DISTANCES_KIND_MEANS_BANDWIDTH, 0);
  assert(!err);
  assert(nr == 2);
  hwloc_distances_release(topology, distances[0]);
  assert(distances[1]->objs[0]->type == HWLOC_OBJ_NUMANODE);
  assert(distances[1]->objs[0]->logical_index == 0);
  assert(distances[1]->objs[1]->type == HWLOC_OBJ_CORE);
  assert(distances[1]->objs[1]->logical_index == 1);
  assert(distances[1]->objs[2]->type == HWLOC_OBJ_GROUP);
  assert(distances[1]->objs[2]->logical_index == 0);
  assert(distances[1]->kind == (HWLOC_DISTANCES_KIND_MEANS_BANDWIDTH|HWLOC_DISTANCES_KIND_FROM_USER|HWLOC_DISTANCES_KIND_HETEROGENEOUS_TYPES));
  hwloc_distances_release(topology, distances[1]);
  nr = 2;
  err = hwloc_distances_get(topology, &nr, distances, HWLOC_DISTANCES_KIND_MEANS_LATENCY|HWLOC_DISTANCES_KIND_FROM_OS, 0);
  assert(!err);
  assert(nr == 0);
  nr = 2;
  err = hwloc_distances_get(topology, &nr, distances, HWLOC_DISTANCES_KIND_MEANS_LATENCY|HWLOC_DISTANCES_KIND_FROM_USER, 0);
  assert(!err);
  assert(nr == 2);
  hwloc_distances_release(topology, distances[0]);
  hwloc_distances_release(topology, distances[1]);

  /* check distances by name */
  nr = 0;
  err = hwloc_distances_get_by_name(topology, NULL, &nr, distances, 0);
  assert(!err);
  assert(nr == 4);
  nr = 0;
  err = hwloc_distances_get_by_name(topology, "nomatch", &nr, distances, 0);
  assert(!err);
  assert(nr == 0);

  /* removing one PU distance */
  printf("Removing the 2nd PU distances\n");
  nr = 2;
  err = hwloc_distances_get_by_type(topology, HWLOC_OBJ_PU, &nr, distances, 0, 0);
  assert(!err);
  assert(nr == 2);
  hwloc_distances_release(topology, distances[0]);
  hwloc_distances_release_remove(topology, distances[1]);
  nr = 0;
  err = hwloc_distances_get_by_type(topology, HWLOC_OBJ_PU, &nr, distances, 0, 0);
  assert(!err);
  assert(nr == 1);

  /* remove distances */
  printf("Removing distances\n");
  /* remove both PU distances */
  err = hwloc_distances_remove_by_type(topology, HWLOC_OBJ_PU);
  assert(!err);
  nr = 0;
  err = hwloc_distances_get_by_type(topology, HWLOC_OBJ_PU, &nr, distances, 0, 0);
  assert(!err);
  assert(!nr);
  nr = 0;
  err = hwloc_distances_get_by_type(topology, HWLOC_OBJ_NUMANODE, &nr, distances, 0, 0);
  assert(!err);
  assert(nr == 1);
  /* remove all distances */
  err = hwloc_distances_remove(topology);
  assert(!err);
  nr = 0;
  err = hwloc_distances_get(topology, &nr, distances, 0, 0);
  assert(!err);
  assert(!nr);
  nr = 0;
  err = hwloc_distances_get_by_type(topology, HWLOC_OBJ_PU, &nr, distances, 0, 0);
  assert(!err);
  assert(!nr);
  nr = 0;
  err = hwloc_distances_get_by_type(topology, HWLOC_OBJ_NUMANODE, &nr, distances, 0, 0);
  assert(!err);
  assert(!nr);

  /* add a bandwidth distance that we'll modify */
  printf("\nInserting NUMA bandwidths\n");
  for(i=0; i<4; i++)
    objs[i] = hwloc_get_obj_by_type(topology, HWLOC_OBJ_NUMANODE, i);
  /* matrix 2*2 */
  for(i=0; i<16; i++)
    values[i] = 0;
  values[0+4*1] = 100;
  values[1+4*0] = 100;
  values[2+4*3] = 200;
  values[3+4*2] = 100;
  for(i=0; i<4; i++)
    values[i+4*i] = 1000;
  handle = hwloc_distances_add_create(topology, NULL,
                                      HWLOC_DISTANCES_KIND_MEANS_BANDWIDTH|HWLOC_DISTANCES_KIND_FROM_USER,
                                      0);
  assert(handle);
  err = hwloc_distances_add_values(topology, handle, 4, objs, values, 0);
  assert(!err);
  err = hwloc_distances_add_commit(topology, handle,
                                   HWLOC_DISTANCES_ADD_FLAG_GROUP);
  assert(!err);

  nr = 1;
  err = hwloc_distances_get_by_type(topology, HWLOC_OBJ_NUMANODE, &nr, distances, HWLOC_DISTANCES_KIND_MEANS_BANDWIDTH, 0);
  assert(!err);
  assert(nr == 1);
  printf("make it links\n");
  err = hwloc_distances_transform(topology, distances[0], HWLOC_DISTANCES_TRANSFORM_LINKS, NULL, 0);
  assert(!err);
  assert(distances[0]->nbobjs == 4);
  assert(distances[0]->values[0] == 0);
  assert(distances[0]->values[1] == 1);
  assert(distances[0]->values[2] == 0);
  assert(distances[0]->values[3] == 0);
  assert(distances[0]->values[4] == 1);
  assert(distances[0]->values[5] == 0);
  assert(distances[0]->values[6] == 0);
  assert(distances[0]->values[7] == 0);
  assert(distances[0]->values[8] == 0);
  assert(distances[0]->values[9] == 0);
  assert(distances[0]->values[10] == 0);
  assert(distances[0]->values[11] == 1);
  assert(distances[0]->values[12] == 0);
  assert(distances[0]->values[13] == 0);
  assert(distances[0]->values[14] == 2);
  assert(distances[0]->values[15] == 0);
  printf("remove 2 objects\n");
  distances[0]->objs[0] = NULL;
  distances[0]->objs[1] = NULL;
  err = hwloc_distances_transform(topology, distances[0], HWLOC_DISTANCES_TRANSFORM_REMOVE_NULL, NULL, 0);
  assert(!err);
  assert(distances[0]->nbobjs == 2);
  assert(distances[0]->values[0] == 0);
  assert(distances[0]->values[1] == 1);
  assert(distances[0]->values[2] == 2);
  assert(distances[0]->values[3] == 0);

  hwloc_distances_release(topology, distances[0]);

  hwloc_topology_destroy(topology);

  return 0;
}
