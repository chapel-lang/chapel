/*
 * Copyright © 2020-2023 Inria.  All rights reserved.
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

static int
get_tier(hwloc_obj_t obj)
{
  const char *value = hwloc_obj_get_info_by_name(obj, "MemoryTier");
  if (value)
    return atoi(value);
  else
    return -1;
}

static void
check_subtypes(hwloc_topology_t topo,
               const char *rootnsubtype, int rootntier,
               const char *pack1n1subtype, int pack1n1tier,
               const char *pack1n2subtype, int pack1n2tier,
               const char *pack2n1subtype, int pack2n1tier,
               const char *pack2n2subtype, int pack2n2tier)
{
  hwloc_obj_t root, rootn, pack1n1, pack1n2, pack2n1, pack2n2;
  root = hwloc_get_root_obj(topo);
  rootn = root->memory_first_child;
  pack1n1 = root->first_child->memory_first_child;
  pack1n2 = root->first_child->memory_first_child->next_sibling;
  pack2n1 = root->last_child->memory_first_child;
  pack2n2 = root->last_child->memory_first_child->next_sibling;

  printf("  Expected %s(%d) %s(%d) %s(%d) %s(%d) %s(%d)\n",
         rootnsubtype, rootntier,
         pack1n1subtype, pack1n1tier,
         pack1n2subtype, pack1n2tier,
         pack2n1subtype, pack2n1tier,
         pack2n2subtype, pack2n2tier);
  printf("  Found    %s(%d) %s(%d) %s(%d) %s(%d) %s(%d)\n",
         rootn->subtype, get_tier(rootn),
         pack1n1->subtype, get_tier(pack1n1),
         pack1n2->subtype, get_tier(pack1n2),
         pack2n1->subtype, get_tier(pack2n1),
         pack2n2->subtype, get_tier(pack2n2));

  if (rootnsubtype) {
    assert(rootn->subtype);
    assert(!strcmp(rootnsubtype, rootn->subtype));
  } else {
    assert(!rootn->subtype);
  }
  assert(rootntier == get_tier(rootn));

  if (pack1n1subtype) {
    assert(pack1n1->subtype);
    assert(!strcmp(pack1n1subtype, pack1n1->subtype));
  } else {
    assert(!pack1n1->subtype);
  }
  assert(pack1n1tier == get_tier(pack1n1));

  if (pack1n2subtype) {
    assert(pack1n2->subtype);
    assert(!strcmp(pack1n2subtype, pack1n2->subtype));
  } else {
    assert(!pack1n2->subtype);
  }
  assert(pack1n2tier == get_tier(pack1n2));

  if (pack2n1subtype) {
    assert(pack2n1->subtype);
    assert(!strcmp(pack2n1subtype, pack2n1->subtype));
  } else {
    assert(!pack2n1->subtype);
  }
  assert(pack2n1tier == get_tier(pack2n1));

  if (pack2n2subtype) {
    assert(pack2n2->subtype);
    assert(!strcmp(pack2n2subtype, pack2n2->subtype));
  } else {
    assert(!pack2n2->subtype);
  }
  assert(pack2n2tier == get_tier(pack2n2));
}

int
main(void)
{
  hwloc_topology_t topology, new;
  hwloc_obj_t root, rootn, pack1n1, pack1n2, pack2n1, pack2n2;
  char *xmlbuffer;
  int buflen;
  int err;

  printf("creating topology...\n");
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

  printf("checking subtypes and tiers aren't set\n");
  check_subtypes(topology, NULL, -1, NULL, -1, NULL, -1, NULL, -1, NULL, -1);

  hwloc_topology_export_xmlbuffer(topology, &xmlbuffer, &buflen, 0);

  putenv((char*)"HWLOC_MEMTIERS_REFRESH=1");

  printf("checking NVM and SPM subtypes are set on XML reload\n");
  printf("UNKNOWN should be before SPM and NVM in tiers since we have no BW\n");
  err = hwloc_topology_init(&new);
  assert(!err);
  err = hwloc_topology_set_xmlbuffer(new, xmlbuffer, buflen);
  assert(!err);
  err = hwloc_topology_load(new);
  assert(!err);
  check_subtypes(new, "NVM", 2, NULL, 0, "SPM", 1, NULL, 0, "SPM", 1);
  hwloc_topology_destroy(new);

  printf("checking HBM subtypes are set on XML reload if HWLOC_MEMTIERS_GUESS=spm_is_hbm\n");
  putenv((char*)"HWLOC_MEMTIERS_GUESS=spm_is_hbm");
  err = hwloc_topology_init(&new);
  assert(!err);
  err = hwloc_topology_set_xmlbuffer(new, xmlbuffer, buflen);
  assert(!err);
  err = hwloc_topology_load(new);
  assert(!err);
  check_subtypes(new, "NVM", 2, NULL, 0, "HBM", 1, NULL, 0, "HBM", 1);
  printf("UNKNOWN should be before HBM and NVM in tiers since we have no BW\n");
  hwloc_topology_destroy(new);

  printf("checking HBM subtypes are set on XML reload if HWLOC_MEMTIERS_GUESS=node0_is_dram,spm_is_hbm\n");
  putenv((char*)"HWLOC_MEMTIERS_GUESS=node0_is_dram,spm_is_hbm");
  err = hwloc_topology_init(&new);
  assert(!err);
  err = hwloc_topology_set_xmlbuffer(new, xmlbuffer, buflen);
  assert(!err);
  err = hwloc_topology_load(new);
  assert(!err);
  check_subtypes(new, "NVM", 2, "DRAM", 1, "HBM", 0, "DRAM", 1, "HBM", 0);
  printf("DRAM should be before SPM and NVM in tiers since we have no BW\n");
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
  check_subtypes(new, "NVM", 2, "DRAM", 1, "HBM", 0, "DRAM", 1, "HBM", 0);
  printf("HBM should be before DRAM and NVM in tiers since we have BW\n");
  hwloc_topology_destroy(new);

  printf("checking DRAM and HBM subtypes aren't set anymore on XML reload if HWLOC_MEMTIERS_GUESS=none\n");
  putenv((char*)"HWLOC_MEMTIERS_GUESS=none");
  err = hwloc_topology_init(&new);
  assert(!err);
  err = hwloc_topology_set_xmlbuffer(new, xmlbuffer, buflen);
  assert(!err);
  err = hwloc_topology_load(new);
  assert(!err);
  check_subtypes(new, "NVM", 2, NULL, 1, "SPM", 0, NULL, 1, "SPM", 0);
  printf("SPM should be before UNKNOWN and NVM in tiers since we have BW\n");
  hwloc_topology_destroy(new);

  printf("checking that no subtypes and tiers are back on XML reload if disabling memory attributes (which disables tiers too)\n");
  putenv((char*)"HWLOC_MEMTIERS_GUESS=spm_is_hbm");
  err = hwloc_topology_init(&new);
  assert(!err);
  err = hwloc_topology_set_flags(new, HWLOC_TOPOLOGY_FLAG_NO_MEMATTRS);
  assert(!err);
  err = hwloc_topology_set_xmlbuffer(new, xmlbuffer, buflen);
  assert(!err);
  err = hwloc_topology_load(new);
  assert(!err);
  check_subtypes(new, NULL, -1, NULL, -1, NULL, -1, NULL, -1, NULL, -1);
  hwloc_topology_destroy(new);

  printf("breaking BW values\n");
  add_local_bw(topology, rootn, 100);
  add_local_bw(topology, pack2n1, 10000);
  add_local_bw(topology, pack2n2, 1000);

  hwloc_topology_export_xmlbuffer(topology, &xmlbuffer, &buflen, 0);

  printf("checking DRAM and HBM subtypes aren't set anymore on XML reload\n");
  printf("one tier per node since kinds and BW are mixed\n");
  putenv((char*)"HWLOC_MEMTIERS_GUESS=default");
  err = hwloc_topology_init(&new);
  assert(!err);
  err = hwloc_topology_set_xmlbuffer(new, xmlbuffer, buflen);
  assert(!err);
  err = hwloc_topology_load(new);
  assert(!err);
  check_subtypes(new, "NVM", 4, NULL, 2, "SPM", 1, NULL, 0, "SPM", 3);
  hwloc_topology_destroy(new);

  printf("checking broken BW can be ignored if HWLOC_MEMTIERS_GUESS=spm_is_hbm\n");
  printf("one tier per node since kinds and BW are mixed\n");
  putenv((char*)"HWLOC_MEMTIERS_GUESS=spm_is_hbm");
  err = hwloc_topology_init(&new);
  assert(!err);
  err = hwloc_topology_set_xmlbuffer(new, xmlbuffer, buflen);
  assert(!err);
  err = hwloc_topology_load(new);
  assert(!err);
  check_subtypes(new, "NVM", 4, NULL, 2, "HBM", 1, NULL, 0, "HBM", 3);
  hwloc_topology_destroy(new);

  printf("checking guessing can be disabled if HWLOC_MEMTIERS_GUESS=none\n");
  printf("one tier per node since kinds and BW are mixed\n");
  putenv((char*)"HWLOC_MEMTIERS_GUESS=none");
  err = hwloc_topology_init(&new);
  assert(!err);
  err = hwloc_topology_set_xmlbuffer(new, xmlbuffer, buflen);
  assert(!err);
  err = hwloc_topology_load(new);
  assert(!err);
  check_subtypes(new, "NVM", 4, NULL, 2, "SPM", 1, NULL, 0, "SPM", 3);
  hwloc_topology_destroy(new);

  printf("checking all tier stuff can be disabled if HWLOC_MEMTIERS=none\n");
  putenv((char*)"HWLOC_MEMTIERS=none");
  err = hwloc_topology_init(&new);
  assert(!err);
  err = hwloc_topology_set_xmlbuffer(new, xmlbuffer, buflen);
  assert(!err);
  err = hwloc_topology_load(new);
  assert(!err);
  check_subtypes(new, NULL, -1, NULL, -1, NULL, -1, NULL, -1, NULL, -1);
  hwloc_topology_destroy(new);

  printf("checking everything can be overwritten with HWLOC_MEMTIERS\n");
  putenv((char*)"HWLOC_MEMTIERS=0x5=HBM;0x12=SPM;0x8=DRAM");
  err = hwloc_topology_init(&new);
  assert(!err);
  err = hwloc_topology_set_xmlbuffer(new, xmlbuffer, buflen);
  assert(!err);
  err = hwloc_topology_load(new);
  assert(!err);
  check_subtypes(new, "SPM", 1, "HBM", 0, "SPM", 1, "HBM", 0, "DRAM", 2);
  hwloc_topology_destroy(new);

  printf("checking everything can be overwritten with HWLOC_MEMTIERS with invalid types\n");
  putenv((char*)"HWLOC_MEMTIERS=0x14=foo;0xb=HBM");
  err = hwloc_topology_init(&new);
  assert(!err);
  err = hwloc_topology_set_xmlbuffer(new, xmlbuffer, buflen);
  assert(!err);
  err = hwloc_topology_load(new);
  assert(!err);
  check_subtypes(new, NULL, 0, "HBM", 1, "HBM", 1, NULL, 0, "HBM", 1);
  hwloc_topology_destroy(new);

  free(xmlbuffer);

  hwloc_topology_destroy(topology);
  return 0;
}

