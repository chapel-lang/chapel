/*
 * Copyright © 2021 Inria.  All rights reserved.
 * See COPYING in top-level directory.
 */

#include "hwloc.h"

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <assert.h>

#ifndef PATH_MAX
#define PATH_MAX 1024
#endif

int main(void)
{
  const char *top_srcdir;
  char xmlpath[PATH_MAX];
  hwloc_topology_t topology;
  hwloc_obj_t tmp, obj, sda, cuda0, card0;
  int err;

  top_srcdir = getenv("HWLOC_TOP_SRCDIR");
  if (!top_srcdir) {
    fprintf(stderr, "HWLOC_TOP_SRCDIR missing in the environment.\n");
    exit(EXIT_FAILURE);
  }

  snprintf(xmlpath, sizeof(xmlpath), "%s/tests/hwloc/xml/32em64t-2n8c2t-pci-normalio.xml", top_srcdir);
  err = hwloc_topology_init(&topology);
  assert(!err);
  err = hwloc_topology_set_xml(topology, xmlpath);
  assert(!err);
  err = hwloc_topology_set_all_types_filter(topology, HWLOC_TYPE_FILTER_KEEP_ALL);
  assert(!err);
  err = hwloc_topology_load(topology);
  assert(!err);

  /* normal checks */

  /* no NUMA or package == root */
  tmp = hwloc_get_root_obj(topology);
  assert(tmp);
  obj = hwloc_get_obj_with_same_locality(topology, tmp, HWLOC_OBJ_PACKAGE, NULL, NULL, 0);
  assert(!obj);
  obj = hwloc_get_obj_with_same_locality(topology, tmp, HWLOC_OBJ_NUMANODE, NULL, NULL, 0);
  assert(!obj);

  /* NUMA == Package == L3 != PU */
  tmp = hwloc_get_obj_by_type(topology, HWLOC_OBJ_PACKAGE, 0);
  assert(tmp);
  obj = hwloc_get_obj_with_same_locality(topology, tmp, HWLOC_OBJ_PACKAGE, NULL, NULL, 0);
  assert(obj);
  assert(obj == tmp);
  obj = hwloc_get_obj_with_same_locality(topology, tmp, HWLOC_OBJ_L3CACHE, NULL, NULL, 0); 
  assert(obj);
  assert(obj->parent == tmp);
  assert(obj->type == HWLOC_OBJ_L3CACHE);
  obj = hwloc_get_obj_with_same_locality(topology, tmp, HWLOC_OBJ_NUMANODE, NULL, NULL, 0); 
  assert(obj);
  assert(obj->parent == tmp);
  assert(obj->type == HWLOC_OBJ_NUMANODE);
  obj = hwloc_get_obj_with_same_locality(topology, tmp, HWLOC_OBJ_PU, NULL, NULL, 0);
  assert(!obj);

  /* Core == L1 == L2 != Package */
  tmp = hwloc_get_obj_by_type(topology, HWLOC_OBJ_CORE, 15);
  assert(tmp);
  obj = hwloc_get_obj_with_same_locality(topology, tmp, HWLOC_OBJ_PACKAGE, NULL, NULL, 0);
  assert(!obj);
  obj = hwloc_get_obj_with_same_locality(topology, tmp, HWLOC_OBJ_L1CACHE, NULL, NULL, 0); 
  assert(obj);
  assert(obj == tmp->parent);
  assert(obj->type == HWLOC_OBJ_L1CACHE);
  obj = hwloc_get_obj_with_same_locality(topology, tmp, HWLOC_OBJ_L2CACHE, NULL, NULL, 0); 
  assert(obj);
  assert(obj == tmp->parent->parent);
  assert(obj->type == HWLOC_OBJ_L2CACHE);

  /* I/O checks */

  /* get pointers to some I/O objects */
  obj = hwloc_get_pcidev_by_busid(topology, 0, 0, 0x1f, 2);
  assert(obj);
  sda = obj->io_first_child;
  assert(sda);
  assert(!strcmp(sda->name, "sda"));

  obj = hwloc_get_pcidev_by_busid(topology, 0, 0x84, 0, 0);
  assert(obj);
  cuda0 = obj->io_first_child;
  assert(cuda0);
  assert(!strcmp(cuda0->name, "cuda0"));
  card0 = cuda0->next_sibling;
  assert(card0);
  assert(!strcmp(card0->name, "card0"));

  /* invalid flags */
  obj = hwloc_get_obj_with_same_locality(topology, sda, HWLOC_OBJ_PCI_DEVICE, NULL, NULL, 1);
  assert(!obj);
  /* invalid subtype */
  obj = hwloc_get_obj_with_same_locality(topology, sda, HWLOC_OBJ_PCI_DEVICE, "foo", NULL, 0);
  assert(!obj);
  /* invalid name */
  obj = hwloc_get_obj_with_same_locality(topology, sda, HWLOC_OBJ_PCI_DEVICE, NULL, "bar", 0);
  assert(!obj);

  /* get sda PCI parent */
  obj = hwloc_get_obj_with_same_locality(topology, sda, HWLOC_OBJ_PCI_DEVICE, NULL, NULL, 0);
  assert(obj == sda->parent);
  /* get sda from itself */
  obj = hwloc_get_obj_with_same_locality(topology, sda, HWLOC_OBJ_OS_DEVICE, NULL, NULL, 0);
  assert(obj == sda);
  /* get sda from PCI */
  obj = hwloc_get_obj_with_same_locality(topology, sda->parent, HWLOC_OBJ_OS_DEVICE, NULL, NULL, 0);
  assert(obj == sda);
  /* get PCI from PCI */
  obj = hwloc_get_obj_with_same_locality(topology, sda->parent, HWLOC_OBJ_PCI_DEVICE, NULL, NULL, 0);
  assert(obj == sda->parent);

  /* get cuda0 from card0 */
  obj = hwloc_get_obj_with_same_locality(topology, card0, HWLOC_OBJ_OS_DEVICE, "CUDA", NULL, 0);
  assert(obj == cuda0);
  obj = hwloc_get_obj_with_same_locality(topology, card0, HWLOC_OBJ_OS_DEVICE, NULL, "cuda", 0);
  assert(obj == cuda0);
  /* get card0 from cuda parent */
  obj = hwloc_get_obj_with_same_locality(topology, cuda0->parent, HWLOC_OBJ_OS_DEVICE, NULL, "card", 0);
  assert(obj == card0);

  /* invalid normal to I/O */
  tmp = hwloc_get_obj_by_type(topology, HWLOC_OBJ_PACKAGE, 0);
  assert(tmp);
  obj = hwloc_get_obj_with_same_locality(topology, tmp, HWLOC_OBJ_PCI_DEVICE, NULL, NULL, 0);
  assert(!obj);
  /* invalid I/O to normal */
  obj = hwloc_get_obj_with_same_locality(topology, cuda0->parent, HWLOC_OBJ_PACKAGE, NULL, NULL, 0);
  assert(!obj);

  hwloc_topology_destroy(topology);

  return 0;
}
