/*
 * Copyright © 2009 CNRS
 * Copyright © 2009-2014 Inria.  All rights reserved.
 * Copyright © 2009 Université Bordeaux
 * Copyright © 2011 Cisco Systems, Inc.  All rights reserved.
 * See COPYING in top-level directory.
 */

#include <hwloc.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

/* check that object userdata is properly initialized */

int main(void)
{
  hwloc_topology_t topology;
  unsigned depth;
  unsigned i,j, width;
  char buffer[1024];
  int err;

  /* check a synthetic topology */
  hwloc_topology_init(&topology);
  err = hwloc_topology_set_synthetic(topology, "2 3 4 5 6");
  assert(!err);
  hwloc_topology_load(topology);

  /* internal checks */

  hwloc_topology_check(topology);

  /* local checks */
  depth = hwloc_topology_get_depth(topology);
  assert(depth == 6);

  width = 1;
  for(i=0; i<6; i++) {
    /* check arities */
    assert(hwloc_get_nbobjs_by_depth(topology, i) == width);
    for(j=0; j<width; j++) {
      hwloc_obj_t obj = hwloc_get_obj_by_depth(topology, i, j);
      assert(obj);
      assert(obj->arity == (i<5 ? i+2 : 0));
    }
    width *= i+2;
  }

  err = hwloc_topology_export_synthetic(topology, buffer, sizeof(buffer), 0);
  assert(err == 75);
  err = strcmp("NUMANode:2(memory=1073741824) Package:3 L2Cache:4(size=4194304) Core:5 PU:6", buffer);
  assert(!err);

  err = hwloc_topology_export_synthetic(topology, buffer, sizeof(buffer), HWLOC_TOPOLOGY_EXPORT_SYNTHETIC_FLAG_NO_EXTENDED_TYPES|HWLOC_TOPOLOGY_EXPORT_SYNTHETIC_FLAG_NO_ATTRS);
  assert(err == 40);
  err = strcmp("NUMANode:2 Package:3 Cache:4 Core:5 PU:6", buffer);
  assert(!err);

  hwloc_topology_destroy(topology);



  hwloc_topology_init(&topology);
  err = hwloc_topology_set_flags(topology, HWLOC_TOPOLOGY_FLAG_ICACHES);
  err = hwloc_topology_set_synthetic(topology, "pack:2(indexes=3,5) numa:2(memory=256GB indexes=pack) l3u:1(size=20mb) l2:2 l1i:1(size=16kB) l1dcache:2 core:1 pu:2(indexes=l2)");
  assert(!err);
  hwloc_topology_load(topology);

  err = hwloc_topology_export_synthetic(topology, buffer, sizeof(buffer), 0);
  assert(err == 181);
  err = strcmp("Package:2 NUMANode:2(memory=274877906944 indexes=2*2:1*2) L3Cache:1(size=20971520) L2Cache:2(size=4194304) L1iCache:1(size=16384) L1dCache:2(size=32768) Core:1 PU:2(indexes=4*8:1*4)", buffer);
  assert(!err);

  assert(hwloc_get_obj_by_type(topology, HWLOC_OBJ_PACKAGE, 1)->os_index == 5);
  assert(hwloc_get_obj_by_type(topology, HWLOC_OBJ_NUMANODE, 1)->os_index == 2);
  assert(hwloc_get_obj_by_type(topology, HWLOC_OBJ_PU, 12)->os_index == 3);
  assert(hwloc_get_obj_by_type(topology, HWLOC_OBJ_PU, 13)->os_index == 11);
  assert(hwloc_get_obj_by_type(topology, HWLOC_OBJ_PU, 14)->os_index == 19);
  assert(hwloc_get_obj_by_type(topology, HWLOC_OBJ_PU, 15)->os_index == 27);
  assert(hwloc_get_obj_by_type(topology, HWLOC_OBJ_PU, 16)->os_index == 4);
  assert(hwloc_get_obj_by_type(topology, HWLOC_OBJ_PU, 17)->os_index == 12);
  assert(hwloc_get_obj_by_type(topology, HWLOC_OBJ_PU, 18)->os_index == 20);
  assert(hwloc_get_obj_by_type(topology, HWLOC_OBJ_PU, 19)->os_index == 28);

  hwloc_topology_destroy(topology);




  hwloc_topology_init(&topology);
  err = hwloc_topology_set_synthetic(topology, "pack:2 core:2 pu:2(indexes=0,4,2,6,1,5,3,7)");
  assert(!err);
  hwloc_topology_load(topology);

  err = hwloc_topology_export_synthetic(topology, buffer, sizeof(buffer), 0);
  assert(err == 42);
  err = strcmp("Package:2 Core:2 PU:2(indexes=4*2:2*2:1*2)", buffer);
  assert(!err);

  assert(hwloc_get_obj_by_type(topology, HWLOC_OBJ_PU, 0)->os_index == 0);
  assert(hwloc_get_obj_by_type(topology, HWLOC_OBJ_PU, 1)->os_index == 4);
  assert(hwloc_get_obj_by_type(topology, HWLOC_OBJ_PU, 2)->os_index == 2);
  assert(hwloc_get_obj_by_type(topology, HWLOC_OBJ_PU, 3)->os_index == 6);
  assert(hwloc_get_obj_by_type(topology, HWLOC_OBJ_PU, 4)->os_index == 1);
  assert(hwloc_get_obj_by_type(topology, HWLOC_OBJ_PU, 5)->os_index == 5);
  assert(hwloc_get_obj_by_type(topology, HWLOC_OBJ_PU, 6)->os_index == 3);
  assert(hwloc_get_obj_by_type(topology, HWLOC_OBJ_PU, 7)->os_index == 7);

  hwloc_topology_destroy(topology);




  hwloc_topology_init(&topology);
  err = hwloc_topology_set_synthetic(topology, "pack:2 core:2 pu:2(indexes=0,4,2,6,1,3,5,7)");
  assert(!err);
  hwloc_topology_load(topology);

  err = hwloc_topology_export_synthetic(topology, buffer, sizeof(buffer), 0);
  assert(err == 46);
  err = strcmp("Package:2 Core:2 PU:2(indexes=0,4,2,6,1,3,5,7)", buffer);
  assert(!err);

  assert(hwloc_get_obj_by_type(topology, HWLOC_OBJ_PU, 0)->os_index == 0);
  assert(hwloc_get_obj_by_type(topology, HWLOC_OBJ_PU, 1)->os_index == 4);
  assert(hwloc_get_obj_by_type(topology, HWLOC_OBJ_PU, 2)->os_index == 2);
  assert(hwloc_get_obj_by_type(topology, HWLOC_OBJ_PU, 3)->os_index == 6);
  assert(hwloc_get_obj_by_type(topology, HWLOC_OBJ_PU, 4)->os_index == 1);
  assert(hwloc_get_obj_by_type(topology, HWLOC_OBJ_PU, 5)->os_index == 3);
  assert(hwloc_get_obj_by_type(topology, HWLOC_OBJ_PU, 6)->os_index == 5);
  assert(hwloc_get_obj_by_type(topology, HWLOC_OBJ_PU, 7)->os_index == 7);

  hwloc_topology_destroy(topology);

  return 0;
}
