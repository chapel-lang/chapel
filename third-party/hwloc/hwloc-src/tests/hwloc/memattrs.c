/*
 * Copyright © 2020-2024 Inria.  All rights reserved.
 * See COPYING in top-level directory.
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "hwloc.h"

static void
check_memattr(hwloc_topology_t topology, const char *name, hwloc_memattr_id_t id, unsigned long flags)
{
  hwloc_memattr_id_t gotid;
  const char *gotname;
  unsigned long gotflags;
  int err;

  err = hwloc_memattr_get_by_name(topology, name, &gotid);
  assert(!err);
  assert(id == gotid);

  err = hwloc_memattr_get_name(topology, id, &gotname);
  assert(!err);
  assert(!strcmp(gotname, name));

  err = hwloc_memattr_get_flags(topology, id, &gotflags);
  assert(!err);
  assert(gotflags == flags);
}

int
main(void)
{
  hwloc_topology_t topology;
  const char *gotname;
  hwloc_memattr_id_t id, id2, gotid;
  struct hwloc_location loc, locs[2];
  unsigned nrlocs, nrtgs, i;
  hwloc_uint64_t gotvalue;
  hwloc_obj_t node, targets[4], obj;
  int err;
  hwloc_bitmap_t bitmap;

  err = hwloc_topology_init(&topology);
  assert(!err);
  err = hwloc_topology_set_synthetic(topology, "node:4 pu:2");
  assert(!err);
  err = hwloc_topology_set_type_filter(topology, HWLOC_OBJ_MISC, HWLOC_TYPE_FILTER_KEEP_ALL);
  assert(!err);
  err = hwloc_topology_load(topology);
  assert(!err);
  obj = hwloc_topology_insert_misc_object(topology, hwloc_get_root_obj(topology), "myMisc");
  assert(obj);

  /* check get local nodes */
  err = hwloc_get_local_numanode_objs(topology, NULL, NULL, NULL, 0);
  assert(err < 0);
  /* get all nodes */
  loc.type = HWLOC_LOCATION_TYPE_OBJECT;
  loc.location.object = hwloc_get_root_obj(topology);
  nrtgs = 4;
  err = hwloc_get_local_numanode_objs(topology, &loc, &nrtgs, targets, HWLOC_LOCAL_NUMANODE_FLAG_ALL);
  assert(!err);
  assert(nrtgs == 4);
  assert(targets[0] == hwloc_get_obj_by_type(topology, HWLOC_OBJ_NUMANODE, 0));
  assert(targets[1] == hwloc_get_obj_by_type(topology, HWLOC_OBJ_NUMANODE, 1));
  assert(targets[2] == hwloc_get_obj_by_type(topology, HWLOC_OBJ_NUMANODE, 2));
  assert(targets[3] == hwloc_get_obj_by_type(topology, HWLOC_OBJ_NUMANODE, 3));
  /* get root local nodes (none) */
  loc.type = HWLOC_LOCATION_TYPE_OBJECT;
  loc.location.object = hwloc_get_root_obj(topology);
  nrtgs = 4;
  err = hwloc_get_local_numanode_objs(topology, &loc, &nrtgs, targets, 0);
  assert(!err);
  assert(nrtgs == 0);
  /* get root-or-smaller local nodes (all) */
  loc.type = HWLOC_LOCATION_TYPE_OBJECT;
  loc.location.object = hwloc_get_root_obj(topology);
  nrtgs = 4;
  err = hwloc_get_local_numanode_objs(topology, &loc, &nrtgs, targets, HWLOC_LOCAL_NUMANODE_FLAG_SMALLER_LOCALITY);
  assert(!err);
  assert(nrtgs == 4);
  assert(targets[0] == hwloc_get_obj_by_type(topology, HWLOC_OBJ_NUMANODE, 0));
  assert(targets[1] == hwloc_get_obj_by_type(topology, HWLOC_OBJ_NUMANODE, 1));
  assert(targets[2] == hwloc_get_obj_by_type(topology, HWLOC_OBJ_NUMANODE, 2));
  assert(targets[3] == hwloc_get_obj_by_type(topology, HWLOC_OBJ_NUMANODE, 3));
  /* get PU local nodes (none) */
  loc.type = HWLOC_LOCATION_TYPE_OBJECT;
  loc.location.object = hwloc_get_obj_by_type(topology, HWLOC_OBJ_PU, 1);
  nrtgs = 4;
  err = hwloc_get_local_numanode_objs(topology, &loc, &nrtgs, targets, 0);
  assert(!err);
  assert(nrtgs == 0);
  /* get PU-or-larger local nodes (1) */
  loc.type = HWLOC_LOCATION_TYPE_OBJECT;
  loc.location.object = hwloc_get_obj_by_type(topology, HWLOC_OBJ_PU, 7);
  nrtgs = 4;
  err = hwloc_get_local_numanode_objs(topology, &loc, &nrtgs, targets, HWLOC_LOCAL_NUMANODE_FLAG_LARGER_LOCALITY);
  assert(!err);
  assert(nrtgs == 1);
  assert(targets[0] == hwloc_get_obj_by_type(topology, HWLOC_OBJ_NUMANODE, 3));
  /* get node local nodes (1) */
  loc.type = HWLOC_LOCATION_TYPE_OBJECT;
  loc.location.object = hwloc_get_obj_by_type(topology, HWLOC_OBJ_NUMANODE, 2);
  nrtgs = 4;
  err = hwloc_get_local_numanode_objs(topology, &loc, &nrtgs, targets, 0);
  assert(!err);
  assert(nrtgs == 1);
  assert(targets[0] == hwloc_get_obj_by_type(topology, HWLOC_OBJ_NUMANODE, 2));
  /* get 2-node local nodes (0) */
  loc.type = HWLOC_LOCATION_TYPE_CPUSET;
  loc.location.cpuset = hwloc_bitmap_alloc();
  hwloc_bitmap_set_range(loc.location.cpuset, 2, 5);
  nrtgs = 4;
  err = hwloc_get_local_numanode_objs(topology, &loc, &nrtgs, targets, 0);
  assert(!err);
  assert(nrtgs == 0);
  /* get 2-node-or-larger local nodes (0) */
  nrtgs = 4;
  err = hwloc_get_local_numanode_objs(topology, &loc, &nrtgs, targets, HWLOC_LOCAL_NUMANODE_FLAG_LARGER_LOCALITY);
  assert(!err);
  assert(nrtgs == 0);
  /* get 2-node-or-smaller local nodes (2) */
  nrtgs = 4;
  err = hwloc_get_local_numanode_objs(topology, &loc, &nrtgs, targets, HWLOC_LOCAL_NUMANODE_FLAG_SMALLER_LOCALITY);
  assert(!err);
  assert(nrtgs == 2);
  assert(targets[0] == hwloc_get_obj_by_type(topology, HWLOC_OBJ_NUMANODE, 1));
  assert(targets[1] == hwloc_get_obj_by_type(topology, HWLOC_OBJ_NUMANODE, 2));
  hwloc_bitmap_free(loc.location.cpuset);

  /* check default memattrs */
  check_memattr(topology, "Capacity", HWLOC_MEMATTR_ID_CAPACITY, HWLOC_MEMATTR_FLAG_HIGHER_FIRST);
  check_memattr(topology, "Locality", HWLOC_MEMATTR_ID_LOCALITY, HWLOC_MEMATTR_FLAG_LOWER_FIRST);
  check_memattr(topology, "Bandwidth", HWLOC_MEMATTR_ID_BANDWIDTH, HWLOC_MEMATTR_FLAG_HIGHER_FIRST|HWLOC_MEMATTR_FLAG_NEED_INITIATOR);
  check_memattr(topology, "Latency", HWLOC_MEMATTR_ID_LATENCY, HWLOC_MEMATTR_FLAG_LOWER_FIRST|HWLOC_MEMATTR_FLAG_NEED_INITIATOR);

  /* check buggy convenience memattr request */
  err = hwloc_memattr_get_value(topology, HWLOC_MEMATTR_ID_CAPACITY,
                                NULL, NULL, 0,
                                &gotvalue);
  assert(err == -1); /* NULL target */
  assert(errno == EINVAL);
  obj = hwloc_get_obj_by_type(topology, HWLOC_OBJ_NUMANODE, 0);
  assert(obj);
  err = hwloc_memattr_set_value(topology, HWLOC_MEMATTR_ID_CAPACITY,
                                obj, NULL, 0,
                                34);
  assert(err == -1); /* cannot change CAPACITY */
  assert(errno == EINVAL);
  obj = hwloc_get_obj_by_type(topology, HWLOC_OBJ_PU, 0);
  assert(obj);
  err = hwloc_memattr_get_value(topology, HWLOC_MEMATTR_ID_CAPACITY,
                                obj, NULL, 0,
                                &gotvalue);
  assert(err == -1); /* no CAPACITY in PU */
  assert(errno == EINVAL);
  err = hwloc_memattr_get_value(topology, HWLOC_MEMATTR_ID_LOCALITY,
                                obj, NULL, 0,
                                &gotvalue);
  assert(!err);
  assert(gotvalue == 1); /* LOCALITY of PU is 1 */
  err = hwloc_memattr_set_value(topology, HWLOC_MEMATTR_ID_LOCALITY,
                                obj, NULL, 0,
                                23);
  assert(err == -1);
  assert(errno == EINVAL); /* cannot change LOCALITY */
  obj = hwloc_get_obj_by_type(topology, HWLOC_OBJ_MISC, 0);
  assert(obj);
  err = hwloc_memattr_get_value(topology, HWLOC_MEMATTR_ID_LOCALITY,
                                obj, NULL, 0,
                                &gotvalue);
  assert(err == -1);
  assert(errno == EINVAL); /* no LOCALITY in Misc */
  /* check convenience memattr values */
  err = hwloc_memattr_get_value(topology, HWLOC_MEMATTR_ID_CAPACITY,
                                hwloc_get_obj_by_type(topology, HWLOC_OBJ_NUMANODE, 1), NULL, 0,
                                &gotvalue);
  assert(!err);
  assert(gotvalue == 1024*1024*1024);
  err = hwloc_memattr_get_value(topology, HWLOC_MEMATTR_ID_LOCALITY,
                                hwloc_get_obj_by_type(topology, HWLOC_OBJ_NUMANODE, 2), NULL, 0,
                                &gotvalue);
  assert(!err);
  assert(gotvalue == 2);
  nrtgs = 0;
  err = hwloc_memattr_get_targets(topology, HWLOC_MEMATTR_ID_LOCALITY, NULL, 0, &nrtgs, targets, NULL);
  assert(!err);
  assert(nrtgs == 4);
  nrtgs = 4;
  err = hwloc_memattr_get_targets(topology, HWLOC_MEMATTR_ID_LOCALITY, NULL, 0, &nrtgs, targets, NULL);
  assert(!err);
  assert(nrtgs == 4);
  assert(targets[0] == hwloc_get_obj_by_type(topology, HWLOC_OBJ_NUMANODE, 0));
  assert(targets[1] == hwloc_get_obj_by_type(topology, HWLOC_OBJ_NUMANODE, 1));
  assert(targets[2] == hwloc_get_obj_by_type(topology, HWLOC_OBJ_NUMANODE, 2));
  assert(targets[3] == hwloc_get_obj_by_type(topology, HWLOC_OBJ_NUMANODE, 3));

  /* look for unexisting memattr */
  err = hwloc_memattr_get_by_name(topology, "foobarrrrr", &gotid);
  assert(err < 0);
  err = hwloc_memattr_get_name(topology, HWLOC_MEMATTR_ID_MAX, &gotname);
  assert(err < 0);

  /* (fail to) register with invalid flags */
  err = hwloc_memattr_register(topology, "foobar", 0, &id);
  assert(err < 0);
  assert(errno == EINVAL);

  /* (fail to) register with existing name */
  err = hwloc_memattr_register(topology, "Capacity", HWLOC_MEMATTR_FLAG_HIGHER_FIRST, &id);
  assert(err < 0);
  assert(errno == EBUSY);

  /********************************
   * new attribute with initiators
   */

  /* register with initiator */
  err = hwloc_memattr_register(topology, "foobar", HWLOC_MEMATTR_FLAG_LOWER_FIRST|HWLOC_MEMATTR_FLAG_NEED_INITIATOR, &id);
  assert(!err);
  /* check it */
  check_memattr(topology, "foobar", id, HWLOC_MEMATTR_FLAG_LOWER_FIRST|HWLOC_MEMATTR_FLAG_NEED_INITIATOR);

  /* check 0 target */
  nrtgs = 0;
  err = hwloc_memattr_get_targets(topology, id, NULL, 0, &nrtgs, NULL, NULL);
  assert(!err);
  assert(!nrtgs);

  /* add a new value for the first NUMA node */
  node = hwloc_get_obj_by_type(topology, HWLOC_OBJ_NUMANODE, 0);
  loc.type = HWLOC_LOCATION_TYPE_OBJECT;
  loc.location.object = hwloc_get_obj_by_type(topology, HWLOC_OBJ_PU, hwloc_get_nbobjs_by_type(topology, HWLOC_OBJ_PU)-1);
  err = hwloc_memattr_set_value(topology, id, node,
                                &loc, 0, 2345);
  assert(!err);

  /* check 1 target */
  nrtgs = 4;
  err = hwloc_memattr_get_targets(topology, id, NULL, 0, &nrtgs, targets, NULL);
  assert(!err);
  assert(nrtgs == 1);
  assert(targets[0] == node);

  /* check that value */
  err = hwloc_memattr_get_value(topology, id, node, &loc, 0, &gotvalue);
  assert(!err);
  assert(gotvalue == 2345);

  /* try with NULL or unknown initiators */
  loc.type = HWLOC_LOCATION_TYPE_OBJECT;
  loc.location.object = NULL;
  err = hwloc_memattr_get_value(topology, id, node, &loc, 0, &gotvalue);
  assert(err < 0);

  loc.type = HWLOC_LOCATION_TYPE_OBJECT;
  loc.location.object = hwloc_get_root_obj(topology);
  err = hwloc_memattr_get_value(topology, id, node, &loc, 0, &gotvalue);
  assert(err < 0);

  loc.type = HWLOC_LOCATION_TYPE_CPUSET;
  loc.location.cpuset = NULL;
  err = hwloc_memattr_get_value(topology, id, node, &loc, 0, &gotvalue);
  assert(err < 0);

  loc.type = HWLOC_LOCATION_TYPE_CPUSET;
  loc.location.cpuset = hwloc_get_root_obj(topology)->cpuset;
  err = hwloc_memattr_get_value(topology, id, node, &loc, 0, &gotvalue);
  assert(err < 0);

  /* check initiators */
  nrlocs = 0;
  err = hwloc_memattr_get_initiators(topology, id, node, 0, &nrlocs, NULL, NULL);
  assert(!err);
  assert(nrlocs == 1);
  /* try first without values */
  err = hwloc_memattr_get_initiators(topology, id, node, 0, &nrlocs, &loc, NULL);
  assert(!err);
  assert(nrlocs == 1);
  assert(loc.type == HWLOC_LOCATION_TYPE_OBJECT);
  assert(loc.location.object != NULL);
  assert(loc.location.object->type == HWLOC_OBJ_PU);
  assert(loc.location.object->logical_index == (unsigned) (hwloc_get_nbobjs_by_type(topology, HWLOC_OBJ_PU) - 1));
  /* try again with values */
  err = hwloc_memattr_get_initiators(topology, id, node, 0, &nrlocs, &loc, &gotvalue);
  assert(!err);
  assert(nrlocs == 1);
  assert(loc.type == HWLOC_LOCATION_TYPE_OBJECT);
  assert(loc.location.object != NULL);
  assert(loc.location.object->type == HWLOC_OBJ_PU);
  assert(loc.location.object->logical_index == (unsigned) (hwloc_get_nbobjs_by_type(topology, HWLOC_OBJ_PU) - 1));
  assert(gotvalue == 2345);

  /***********************************
   * new attribute without initiators
   */

  /* register without initiator */
  err = hwloc_memattr_register(topology, "barnoinit", HWLOC_MEMATTR_FLAG_HIGHER_FIRST, &id2);
  assert(!err);
  /* check it */
  check_memattr(topology, "barnoinit", id2, HWLOC_MEMATTR_FLAG_HIGHER_FIRST);

  /* check 0 target */
  nrtgs = 0;
  err = hwloc_memattr_get_targets(topology, id2, NULL, 0, &nrtgs, NULL, NULL);
  assert(!err);
  assert(!nrtgs);

  /* add a new value for the last NUMA node */
  node = hwloc_get_obj_by_type(topology, HWLOC_OBJ_NUMANODE, hwloc_get_nbobjs_by_type(topology, HWLOC_OBJ_NUMANODE)-1);
  err = hwloc_memattr_set_value(topology, id2, node,
                                NULL, 0, 3456);
  assert(!err);

  /* force refresh */
  err = hwloc_topology_refresh(topology);
  assert(!err);

  /* check 1 target */
  nrtgs = 4;
  err = hwloc_memattr_get_targets(topology, id2, NULL, 0, &nrtgs, targets, NULL);
  assert(!err);
  assert(nrtgs == 1);
  assert(targets[0] == node);

  /* check that value */
  err = hwloc_memattr_get_value(topology, id2, node, NULL, 0, &gotvalue);
  assert(!err);
  assert(gotvalue == 3456);

  /* check that initiator is ignored when getting values */
  loc.type = HWLOC_LOCATION_TYPE_OBJECT;
  loc.location.object = hwloc_get_root_obj(topology);
  gotvalue = 0;
  err = hwloc_memattr_get_value(topology, id2, node, &loc, 0, &gotvalue);
  assert(!err);
  assert(gotvalue == 3456);

  loc.type = HWLOC_LOCATION_TYPE_CPUSET;
  loc.location.cpuset = hwloc_get_root_obj(topology)->cpuset;
  gotvalue = 0;
  err = hwloc_memattr_get_value(topology, id2, node, &loc, 0, &gotvalue);
  assert(!err);
  assert(gotvalue == 3456);

  /* check no initiators are returned */
  nrlocs = 0;
  err = hwloc_memattr_get_initiators(topology, id2, node, 0, &nrlocs, NULL, NULL);
  assert(!err);
  assert(nrlocs == 0);

  /**********************************************
   * add value for both attributes for all nodes
   */
  /* use first node as initiator for first attribute */
  loc.type = HWLOC_LOCATION_TYPE_CPUSET;
  loc.location.cpuset = hwloc_get_obj_by_type(topology, HWLOC_OBJ_NUMANODE, 0)->cpuset;
  for(i=0; i<(unsigned)hwloc_get_nbobjs_by_type(topology, HWLOC_OBJ_NUMANODE); i++) {
    node = hwloc_get_obj_by_type(topology, HWLOC_OBJ_NUMANODE, i);
    /* increasing value for first attribute, best target will be first node */
    err = hwloc_memattr_set_value(topology, id, node,
                                  &loc, 0, (i+1)*10);
    assert(!err);

    /* increasing value for second attribute without initiator, best target will be last node */
    err = hwloc_memattr_set_value(topology, id2, node,
                                  NULL, 0, (i+1)*10);
    assert(!err);
  }

  /* check 4 targets for id */
  nrtgs = 4;
  err = hwloc_memattr_get_targets(topology, id, NULL, 0, &nrtgs, targets, NULL);
  assert(!err);
  assert(nrtgs == 4);
  /* first target */
  assert(targets[0] == hwloc_get_obj_by_type(topology, HWLOC_OBJ_NUMANODE, 0));
  nrlocs = 2;
  err = hwloc_memattr_get_initiators(topology, id, targets[0], 0, &nrlocs, locs, NULL);
  assert(!err);
  assert(nrlocs == 2);
  assert(locs[0].type == HWLOC_LOCATION_TYPE_OBJECT);
  assert(locs[0].location.object->type == HWLOC_OBJ_PU);
  assert(locs[0].location.object->os_index == 7);
  assert(locs[1].type == HWLOC_LOCATION_TYPE_CPUSET);
  assert(hwloc_bitmap_isequal(locs[1].location.cpuset, hwloc_get_obj_by_type(topology, HWLOC_OBJ_NUMANODE, 0)->cpuset));
  /* second target */
  assert(targets[1] == hwloc_get_obj_by_type(topology, HWLOC_OBJ_NUMANODE, 1));
  nrlocs = 2;
  err = hwloc_memattr_get_initiators(topology, id, targets[1], 0, &nrlocs, locs, NULL);
  assert(!err);
  assert(nrlocs == 1);
  assert(locs[0].type == HWLOC_LOCATION_TYPE_CPUSET);
  assert(hwloc_bitmap_isequal(locs[0].location.cpuset, hwloc_get_obj_by_type(topology, HWLOC_OBJ_NUMANODE, 0)->cpuset));
  /* third target */
  assert(targets[2] == hwloc_get_obj_by_type(topology, HWLOC_OBJ_NUMANODE, 2));
  nrlocs = 2;
  err = hwloc_memattr_get_initiators(topology, id, targets[2], 0, &nrlocs, locs, NULL);
  assert(!err);
  assert(nrlocs == 1);
  assert(locs[0].type == HWLOC_LOCATION_TYPE_CPUSET);
  assert(hwloc_bitmap_isequal(locs[0].location.cpuset, hwloc_get_obj_by_type(topology, HWLOC_OBJ_NUMANODE, 0)->cpuset));
  /* fourth target */
  assert(targets[3] == hwloc_get_obj_by_type(topology, HWLOC_OBJ_NUMANODE, 3));
  nrlocs = 2;
  err = hwloc_memattr_get_initiators(topology, id, targets[3], 0, &nrlocs, locs, NULL);
  assert(!err);
  assert(nrlocs == 1);
  assert(locs[0].type == HWLOC_LOCATION_TYPE_CPUSET);
  assert(hwloc_bitmap_isequal(locs[0].location.cpuset, hwloc_get_obj_by_type(topology, HWLOC_OBJ_NUMANODE, 0)->cpuset));

  /* check 4 targets for id with entire topology cpuset */
  nrtgs = 4;
  err = hwloc_memattr_get_targets(topology, id, &loc, 0, &nrtgs, targets, NULL);
  assert(!err);
  assert(nrtgs == 4);
  /* check 0 targets for id with entire topology cpuset */
  nrtgs = 4;
  loc.type = HWLOC_LOCATION_TYPE_CPUSET;
  loc.location.cpuset = (hwloc_bitmap_t) hwloc_topology_get_topology_cpuset(topology);
  err = hwloc_memattr_get_targets(topology, id, &loc, 0, &nrtgs, targets, NULL);
  assert(!err);
  assert(nrtgs == 0);

  /* check 4 targets for id2, no initiators to check */
  nrtgs = 4;
  err = hwloc_memattr_get_targets(topology, id2, NULL, 0, &nrtgs, targets, NULL);
  assert(!err);
  assert(nrtgs == 4);
  assert(targets[0] == hwloc_get_obj_by_type(topology, HWLOC_OBJ_NUMANODE, 3)); /* node 3 was added first */
  assert(targets[1] == hwloc_get_obj_by_type(topology, HWLOC_OBJ_NUMANODE, 0));
  assert(targets[2] == hwloc_get_obj_by_type(topology, HWLOC_OBJ_NUMANODE, 1));
  assert(targets[3] == hwloc_get_obj_by_type(topology, HWLOC_OBJ_NUMANODE, 2));

  /* now check best targets from first PU (included in first node used as initiator above) */
  loc.type = HWLOC_LOCATION_TYPE_CPUSET;
  loc.location.cpuset = hwloc_get_obj_by_type(topology, HWLOC_OBJ_PU, 0)->cpuset;
  /* check best is the first node for first attribute (lower first) */
  err = hwloc_memattr_get_best_target(topology, id, &loc, 0, &node, NULL);
  assert(!err);
  assert(node);
  assert(node->type == HWLOC_OBJ_NUMANODE);
  assert(node == hwloc_get_obj_by_type(topology, HWLOC_OBJ_NUMANODE, 0));
  /* check best is the last node for last attribute (lower first) */
  err = hwloc_memattr_get_best_target(topology, id2, &loc, 0, &node, NULL);
  assert(!err);
  assert(node);
  assert(node->type == HWLOC_OBJ_NUMANODE);
  assert(node == hwloc_get_obj_by_type(topology, HWLOC_OBJ_NUMANODE, hwloc_get_nbobjs_by_type(topology, HWLOC_OBJ_NUMANODE)-1));

  /* check best initiator for first NUMA is its own cpuset */
  err = hwloc_memattr_get_best_initiator(topology, id, node, 0, &loc, NULL);
  assert(!err);
  assert(locs[0].type == HWLOC_LOCATION_TYPE_CPUSET);
  /* check best initiator is invalid for no-initiator memattr */
  assert(hwloc_bitmap_isequal(locs[0].location.cpuset, hwloc_get_obj_by_type(topology, HWLOC_OBJ_NUMANODE, 0)->cpuset));
  err = hwloc_memattr_get_best_initiator(topology, id2, node, 0, &loc, NULL);
  assert(err == -1);
  assert(errno == EINVAL);

  /***************************
   * restrict to only 3 nodes
   */
  bitmap = hwloc_bitmap_alloc();
  hwloc_bitmap_set_range(bitmap, 0, 2);
  err = hwloc_topology_restrict(topology, bitmap, HWLOC_RESTRICT_FLAG_BYNODESET);
  assert(!err);
  hwloc_bitmap_free(bitmap);

  /* now only 3 targets for id, last target was removed */
  nrtgs = 4;
  err = hwloc_memattr_get_targets(topology, id, NULL, 0, &nrtgs, targets, NULL);
  assert(!err);
  assert(nrtgs == 3);
  /* first target unchanged */
  assert(targets[0] == hwloc_get_obj_by_type(topology, HWLOC_OBJ_NUMANODE, 0));
  nrlocs = 2;
  err = hwloc_memattr_get_initiators(topology, id, targets[0], 0, &nrlocs, locs, NULL);
  assert(!err);
  assert(nrlocs == 2);
  assert(locs[0].type == HWLOC_LOCATION_TYPE_OBJECT);
  assert(locs[0].location.object->type == HWLOC_OBJ_PU);
  assert(locs[0].location.object->os_index == 7);
  assert(locs[1].type == HWLOC_LOCATION_TYPE_CPUSET);
  assert(hwloc_bitmap_isequal(locs[1].location.cpuset, hwloc_get_obj_by_type(topology, HWLOC_OBJ_NUMANODE, 0)->cpuset));
  /* second target unchanged */
  assert(targets[1] == hwloc_get_obj_by_type(topology, HWLOC_OBJ_NUMANODE, 1));
  nrlocs = 2;
  err = hwloc_memattr_get_initiators(topology, id, targets[1], 0, &nrlocs, locs, NULL);
  assert(!err);
  assert(nrlocs == 1);
  assert(locs[0].type == HWLOC_LOCATION_TYPE_CPUSET);
  assert(hwloc_bitmap_isequal(locs[0].location.cpuset, hwloc_get_obj_by_type(topology, HWLOC_OBJ_NUMANODE, 0)->cpuset));
  /* third target unchanged */
  assert(targets[2] == hwloc_get_obj_by_type(topology, HWLOC_OBJ_NUMANODE, 2));
  nrlocs = 2;
  err = hwloc_memattr_get_initiators(topology, id, targets[2], 0, &nrlocs, locs, NULL);
  assert(!err);
  assert(nrlocs == 1);
  assert(locs[0].type == HWLOC_LOCATION_TYPE_CPUSET);
  assert(hwloc_bitmap_isequal(locs[0].location.cpuset, hwloc_get_obj_by_type(topology, HWLOC_OBJ_NUMANODE, 0)->cpuset));
  /* fourth target removed */

  /* now only 3 targets for id2, last target was removed */
  nrtgs = 4;
  err = hwloc_memattr_get_targets(topology, id2, NULL, 0, &nrtgs, targets, NULL);
  assert(!err);
  assert(nrtgs == 3);
  assert(targets[0] == hwloc_get_obj_by_type(topology, HWLOC_OBJ_NUMANODE, 0));
  assert(targets[1] == hwloc_get_obj_by_type(topology, HWLOC_OBJ_NUMANODE, 1));
  assert(targets[2] == hwloc_get_obj_by_type(topology, HWLOC_OBJ_NUMANODE, 2));

  /********************************************
   * restrict to only the second half of cores
   */
  bitmap = hwloc_bitmap_alloc();
  hwloc_bitmap_set_range(bitmap, 4, 7);
  err = hwloc_topology_restrict(topology, bitmap, 0);
  assert(!err);
  hwloc_bitmap_free(bitmap);

  /* now only 1 target for id, all cpuset initiators removed */
  nrtgs = 4;
  err = hwloc_memattr_get_targets(topology, id, NULL, 0, &nrtgs, targets, NULL);
  assert(!err);
  assert(nrtgs == 1);
  /* first target only has one initiator */
  assert(targets[0] == hwloc_get_obj_by_type(topology, HWLOC_OBJ_NUMANODE, 2)); /* CPUless node:0 became node:2 */
  nrlocs = 2;
  err = hwloc_memattr_get_initiators(topology, id, targets[0], 0, &nrlocs, locs, NULL);
  assert(!err);
  assert(nrlocs == 1);
  assert(locs[0].type == HWLOC_LOCATION_TYPE_OBJECT);
  assert(locs[0].location.object->type == HWLOC_OBJ_PU);
  assert(locs[0].location.object->os_index == 7);
  /* second, third and fourth targets removed */

  /* still only 3 targets for id2 */
  nrtgs = 4;
  err = hwloc_memattr_get_targets(topology, id2, NULL, 0, &nrtgs, targets, NULL);
  assert(!err);
  assert(nrtgs == 3);
  assert(targets[0] == hwloc_get_obj_by_type(topology, HWLOC_OBJ_NUMANODE, 2)); /* CPUless node:0 became node:2 */
  assert(targets[1] == hwloc_get_obj_by_type(topology, HWLOC_OBJ_NUMANODE, 1)); /* 0 and 1 should be reversed, but the core doesn't current reordered those, see FIXME in restrict_object_by_nodeset() */
  assert(targets[2] == hwloc_get_obj_by_type(topology, HWLOC_OBJ_NUMANODE, 0));

  hwloc_topology_destroy(topology);
  return 0;
}

