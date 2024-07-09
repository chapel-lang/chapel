/*
 * Copyright © 2020-2022 Inria.  All rights reserved.
 * See COPYING in top-level directory.
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "hwloc.h"

static void
add_local_bw(hwloc_topology_t topo, hwloc_obj_t node, hwloc_uint64_t bw)
{
  struct hwloc_location initiator;
  initiator.type = HWLOC_LOCATION_TYPE_CPUSET;
  initiator.location.cpuset = node->cpuset;
  hwloc_memattr_set_value(topo, HWLOC_MEMATTR_ID_BANDWIDTH, node, &initiator, 0, bw);
}

static void
add_daxtype(hwloc_topology_t topo __hwloc_attribute_unused, hwloc_obj_t node, const char *daxtype)
{
  hwloc_obj_add_info(node, "DAXType", daxtype);
}

static void
check_subtypes(hwloc_topology_t topo,
               const char *rootnsubtype,
               const char *pack1n1subtype,
               const char *pack1n2subtype,
               const char *pack2n1subtype,
               const char *pack2n2subtype)
{
  hwloc_obj_t root, rootn, pack1n1, pack1n2, pack2n1, pack2n2;
  root = hwloc_get_root_obj(topo);
  rootn = root->memory_first_child;
  pack1n1 = root->first_child->memory_first_child;
  pack1n2 = root->first_child->memory_first_child->next_sibling;
  pack2n1 = root->last_child->memory_first_child;
  pack2n2 = root->last_child->memory_first_child->next_sibling;

  if (rootnsubtype) {
    assert(rootn->subtype);
    assert(!strcmp(rootnsubtype, rootn->subtype));
  } else {
    assert(!rootn->subtype);
  }
  if (pack1n1subtype) {
    assert(pack1n1->subtype);
    assert(!strcmp(pack1n1subtype, pack1n1->subtype));
  } else {
    assert(!pack1n1->subtype);
  }
  if (pack1n2subtype) {
    assert(pack1n2->subtype);
    assert(!strcmp(pack1n2subtype, pack1n2->subtype));
  } else {
    assert(!pack1n2->subtype);
  }
  if (pack2n1subtype) {
    assert(pack2n1->subtype);
    assert(!strcmp(pack2n1subtype, pack2n1->subtype));
  } else {
    assert(!pack2n1->subtype);
  }
  if (pack2n2subtype) {
    assert(pack2n2->subtype);
    assert(!strcmp(pack2n2subtype, pack2n2->subtype));
  } else {
    assert(!pack2n2->subtype);
  }
}

int
main(void)
{
  hwloc_topology_t topology, new;
  hwloc_obj_t root, rootn, pack1n1, pack1n2, pack2n1, pack2n2;
  char *xmlbuffer;
  int buflen;
  int err;

  printf("creatin topology...\n");
  err = hwloc_topology_init(&topology);
  assert(!err);
  err = hwloc_topology_set_synthetic(topology, "[numa] pack:2 [numa] [numa] pu:2");
  assert(!err);
  err = hwloc_topology_load(topology);
  assert(!err);

  printf("marking some nodes as NVM or SPM\n");
  root = hwloc_get_root_obj(topology);
  rootn = root->memory_first_child;
  pack1n1 = root->first_child->memory_first_child;
  pack1n2 = root->first_child->memory_first_child->next_sibling;
  pack2n1 = root->last_child->memory_first_child;
  pack2n2 = root->last_child->memory_first_child->next_sibling;
  add_daxtype(topology, rootn, "NVM");
  add_daxtype(topology, pack1n2, "SPM");
  add_daxtype(topology, pack2n2, "SPM");

  printf("checking subtypes aren't set\n");
  check_subtypes(topology, NULL, NULL, NULL, NULL, NULL);

  hwloc_topology_export_xmlbuffer(topology, &xmlbuffer, &buflen, 0);

  printf("checking NVM and SPM subtypes are set on XML reload\n");
  err = hwloc_topology_init(&new);
  assert(!err);
  err = hwloc_topology_set_xmlbuffer(new, xmlbuffer, buflen);
  assert(!err);
  err = hwloc_topology_load(new);
  assert(!err);
  check_subtypes(new, "NVM", NULL, "SPM", NULL, "SPM");
  hwloc_topology_destroy(new);

  printf("checking DRAM and HBM subtypes are set on XML reload if HWLOC_MEMTIERS_GUESS=spm_is_hbm\n");
  putenv((char*)"HWLOC_MEMTIERS_GUESS=spm_is_hbm");
  err = hwloc_topology_init(&new);
  assert(!err);
  err = hwloc_topology_set_xmlbuffer(new, xmlbuffer, buflen);
  assert(!err);
  err = hwloc_topology_load(new);
  assert(!err);
  check_subtypes(new, "NVM", "DRAM", "HBM", "DRAM", "HBM");
  hwloc_topology_destroy(new);

  free(xmlbuffer);

  printf("adding correct BW values\n");
  add_local_bw(topology, rootn, 100);
  add_local_bw(topology, pack1n1, 1000);
  add_local_bw(topology, pack1n2, 10000);
  add_local_bw(topology, pack2n1, 1000);
  add_local_bw(topology, pack2n2, 10000);

  hwloc_topology_export_xmlbuffer(topology, &xmlbuffer, &buflen, 0);

  printf("checking DRAM and HBM subtypes are set on XML reload\n");
  putenv((char*)"HWLOC_MEMTIERS_GUESS=default");
  err = hwloc_topology_init(&new);
  assert(!err);
  err = hwloc_topology_set_xmlbuffer(new, xmlbuffer, buflen);
  assert(!err);
  err = hwloc_topology_load(new);
  assert(!err);
  check_subtypes(new, "NVM", "DRAM", "HBM", "DRAM", "HBM");
  hwloc_topology_destroy(new);

  printf("checking DRAM and HBM subtypes aren't set anymore on XML reload if HWLOC_MEMTIERS_GUESS=none\n");
  putenv((char*)"HWLOC_MEMTIERS_GUESS=none");
  err = hwloc_topology_init(&new);
  assert(!err);
  err = hwloc_topology_set_xmlbuffer(new, xmlbuffer, buflen);
  assert(!err);
  err = hwloc_topology_load(new);
  assert(!err);
  check_subtypes(new, NULL, NULL, NULL, NULL, NULL);
  hwloc_topology_destroy(new);

  free(xmlbuffer);

  printf("breaking BW values\n");
  add_local_bw(topology, rootn, 100);
  add_local_bw(topology, pack2n1, 10000);
  add_local_bw(topology, pack2n2, 1000);

  hwloc_topology_export_xmlbuffer(topology, &xmlbuffer, &buflen, 0);

  printf("checking DRAM and HBM subtypes aren't set anymore on XML reload\n");
  putenv((char*)"HWLOC_MEMTIERS_GUESS=default");
  err = hwloc_topology_init(&new);
  assert(!err);
  err = hwloc_topology_set_xmlbuffer(new, xmlbuffer, buflen);
  assert(!err);
  err = hwloc_topology_load(new);
  assert(!err);
  check_subtypes(new, "NVM", NULL, "SPM", NULL, "SPM");
  hwloc_topology_destroy(new);

  printf("checking broken BW can be ignored if HWLOC_MEMTIERS_GUESS=spm_is_hbm\n");
  putenv((char*)"HWLOC_MEMTIERS_GUESS=spm_is_hbm");
  err = hwloc_topology_init(&new);
  assert(!err);
  err = hwloc_topology_set_xmlbuffer(new, xmlbuffer, buflen);
  assert(!err);
  err = hwloc_topology_load(new);
  assert(!err);
  check_subtypes(new, "NVM", "DRAM", "HBM", "DRAM", "HBM");
  hwloc_topology_destroy(new);

  printf("checking everything can be disabled if HWLOC_MEMTIERS_GUESS=none\n");
  putenv((char*)"HWLOC_MEMTIERS_GUESS=none");
  err = hwloc_topology_init(&new);
  assert(!err);
  err = hwloc_topology_set_xmlbuffer(new, xmlbuffer, buflen);
  assert(!err);
  err = hwloc_topology_load(new);
  assert(!err);
  check_subtypes(new, NULL, NULL, NULL, NULL, NULL);
  hwloc_topology_destroy(new);

  free(xmlbuffer);

  hwloc_topology_destroy(topology);
  return 0;
}

