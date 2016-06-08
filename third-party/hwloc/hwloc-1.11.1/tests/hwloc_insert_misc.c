/*
 * Copyright © 2009 CNRS
 * Copyright © 2009-2014 Inria.  All rights reserved.
 * Copyright © 2009-2010 Université Bordeaux
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
  hwloc_topology_t topology, reload;
  hwloc_bitmap_t cpuset;
  hwloc_obj_t obj;
  char *buf1, *buf2;
  int buflen1, buflen2, err;

  err = hwloc_topology_init(&topology);
  assert(!err);
  err = hwloc_topology_load(topology);
  assert(!err);
  hwloc_topology_check(topology);

  /* insert by cpuset below root */
  cpuset = hwloc_bitmap_alloc();
  hwloc_bitmap_copy(cpuset, hwloc_topology_get_topology_cpuset(topology));
  obj = hwloc_topology_insert_misc_object_by_cpuset(topology, cpuset, "test by cpuset under root");
  assert(obj);
  obj = hwloc_topology_insert_misc_object_by_cpuset(topology, cpuset, "test2 by cpuset under root");
  assert(obj);
  hwloc_bitmap_free(cpuset);

  /* insert by cpuset below first available PU */
  cpuset = hwloc_bitmap_alloc();
  hwloc_bitmap_set(cpuset, hwloc_bitmap_first(hwloc_topology_get_topology_cpuset(topology)));
  obj = hwloc_topology_insert_misc_object_by_cpuset(topology, cpuset, "test by cpuset under first available PU");
  assert(obj);
  obj = hwloc_topology_insert_misc_object_by_cpuset(topology, cpuset, "test2 by cpuset under first available PU");
  assert(obj);
  hwloc_bitmap_free(cpuset);

  /* insert by parent below root */
  obj = hwloc_get_root_obj(topology);
  obj = hwloc_topology_insert_misc_object_by_parent(topology, obj, "test by parent below root");
  assert(obj);

  /* insert by parent below previous Misc */
  obj = hwloc_topology_insert_misc_object_by_parent(topology, obj, "test by parent below previous Misc");
  assert(obj);

  /* insert by parent below first PU */
  obj = hwloc_get_obj_by_type(topology, HWLOC_OBJ_PU, 0);
  obj = hwloc_topology_insert_misc_object_by_parent(topology, obj, "test by parent below first PU");
  assert(obj);

  hwloc_topology_check(topology);

  /* export, reimport and check things are in consistent state */
  err = hwloc_topology_export_xmlbuffer(topology, &buf1, &buflen1);
  assert(!err);
  err = hwloc_topology_init(&reload);
  assert(!err);
  err = hwloc_topology_set_xmlbuffer(reload, buf1, buflen1);
  assert(!err);
  err = hwloc_topology_load(reload);
  assert(!err);
  hwloc_topology_check(reload);
  err = hwloc_topology_export_xmlbuffer(reload, &buf2, &buflen2);
  assert(!err);
  assert(buflen1 == buflen2);
  err = strcmp(buf1, buf2);
  assert(!err);
  hwloc_free_xmlbuffer(reload, buf2);
  hwloc_topology_destroy(reload);
  hwloc_free_xmlbuffer(topology, buf1);

  hwloc_topology_destroy(topology);

  return 0;
}
