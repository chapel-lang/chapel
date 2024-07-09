/*
 * Copyright © 2018 Inria.  All rights reserved.
 * See COPYING in top-level directory.
 */

#include "hwloc.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <assert.h>

int main(void)
{
  hwloc_topology_t topology;
  hwloc_const_bitmap_t set;
  hwloc_bitmap_t new = hwloc_bitmap_alloc();
  int err;

  /* allow without HWLOC_TOPOLOGY_FLAG_INCLUDE_DISALLOWED should fail */
  hwloc_topology_init(&topology);
  hwloc_topology_set_synthetic(topology, "pack:5 node:3 core:2 pu:4");
  hwloc_topology_load(topology);
  err = hwloc_topology_allow(topology, NULL, NULL, HWLOC_ALLOW_FLAG_ALL);
  assert(err == -1 && errno == EINVAL);
  hwloc_topology_destroy(topology);

  hwloc_topology_init(&topology);
  hwloc_topology_set_synthetic(topology, "pack:5 node:3 core:2 pu:4");
  hwloc_topology_set_flags(topology, HWLOC_TOPOLOGY_FLAG_INCLUDE_DISALLOWED);
  hwloc_topology_load(topology);

  set = hwloc_topology_get_allowed_cpuset(topology);
  assert(set);
  assert(hwloc_bitmap_weight(set) == 120);
  assert(hwloc_bitmap_first(set) == 0);
  assert(hwloc_bitmap_last(set) == 119);

  set = hwloc_topology_get_allowed_nodeset(topology);
  assert(set);
  assert(hwloc_bitmap_weight(set) == 15);
  assert(hwloc_bitmap_first(set) == 0);
  assert(hwloc_bitmap_last(set) == 14);

  /* HWLOC_ALLOW_FLAG_ALL doesn't want any set */
  err = hwloc_topology_allow(topology, new, new, HWLOC_ALLOW_FLAG_ALL);
  assert(err == -1 && errno == EINVAL);
  /* HWLOC_ALLOW_FLAG_LOCAL_RESTRICTIONS doesn't want any set */
  err = hwloc_topology_allow(topology, new, new, HWLOC_ALLOW_FLAG_LOCAL_RESTRICTIONS);
  assert(err == -1 && errno == EINVAL);
  /* HWLOC_ALLOW_FLAG_LOCAL_RESTRICTIONS doesn't work without thissystem */
  assert(!hwloc_topology_is_thissystem(topology));
  err = hwloc_topology_allow(topology, NULL, NULL, HWLOC_ALLOW_FLAG_LOCAL_RESTRICTIONS);
  assert(err == -1 && errno == EINVAL);
  /* HWLOC_ALLOW_FLAG_CUSTOM doesn't like empty sets */
  hwloc_bitmap_zero(new);
  err = hwloc_topology_allow(topology, new, new, HWLOC_ALLOW_FLAG_CUSTOM);
  assert(err == -1 && errno == EINVAL);
  err = hwloc_topology_allow(topology, NULL, new, HWLOC_ALLOW_FLAG_CUSTOM);
  assert(err == -1 && errno == EINVAL);
  err = hwloc_topology_allow(topology, new, NULL, HWLOC_ALLOW_FLAG_CUSTOM);
  assert(err == -1 && errno == EINVAL);
  /* HWLOC_ALLOW_FLAG_CUSTOM doesn't like sets outside of the machine */
  hwloc_bitmap_set_range(new, 500, 600);
  err = hwloc_topology_allow(topology, new, new, HWLOC_ALLOW_FLAG_CUSTOM);
  assert(err == -1 && errno == EINVAL);
  err = hwloc_topology_allow(topology, NULL, new, HWLOC_ALLOW_FLAG_CUSTOM);
  assert(err == -1 && errno == EINVAL);
  err = hwloc_topology_allow(topology, new, NULL, HWLOC_ALLOW_FLAG_CUSTOM);
  assert(err == -1 && errno == EINVAL);

  /* allow a single bit */
  hwloc_bitmap_only(new, 10);
  err = hwloc_topology_allow(topology, new, new, HWLOC_ALLOW_FLAG_CUSTOM);
  assert(!err);
  set = hwloc_topology_get_allowed_cpuset(topology);
  assert(set);
  assert(hwloc_bitmap_weight(set) == 1);
  assert(hwloc_bitmap_first(set) == 10);
  assert(hwloc_bitmap_last(set) == 10);
  set = hwloc_topology_get_allowed_nodeset(topology);
  assert(set);
  assert(hwloc_bitmap_weight(set) == 1);
  assert(hwloc_bitmap_first(set) == 10);
  assert(hwloc_bitmap_last(set) == 10);

  /* restrict the topology outside of the allowed set */
  hwloc_bitmap_zero(new);
  hwloc_bitmap_set_range(new, 40, 60);
  err = hwloc_topology_restrict(topology, new, 0);
  assert(err == -1 && errno == EINVAL);
  hwloc_bitmap_zero(new);
  hwloc_bitmap_set_range(new, 12, 14);
  err = hwloc_topology_restrict(topology, new, HWLOC_RESTRICT_FLAG_BYNODESET);
  assert(err == -1 && errno == EINVAL);

  /* allow all NUMAs and more PUs */
  hwloc_bitmap_zero(new);
  hwloc_bitmap_set_range(new, 0, 20);
  err = hwloc_topology_allow(topology, new, new, HWLOC_ALLOW_FLAG_CUSTOM);
  assert(!err);
  set = hwloc_topology_get_allowed_cpuset(topology);
  assert(set);
  assert(hwloc_bitmap_weight(set) == 21);
  assert(hwloc_bitmap_first(set) == 0);
  assert(hwloc_bitmap_last(set) == 20);
  set = hwloc_topology_get_allowed_nodeset(topology);
  assert(set);
  assert(hwloc_bitmap_weight(set) == 15);
  assert(hwloc_bitmap_first(set) == 0);
  assert(hwloc_bitmap_last(set) == 14);

  /* allow less NUMAs */
  hwloc_bitmap_zero(new);
  hwloc_bitmap_set_range(new, 6, 10);
  err = hwloc_topology_allow(topology, NULL, new, HWLOC_ALLOW_FLAG_CUSTOM);
  assert(!err);
  set = hwloc_topology_get_allowed_cpuset(topology);
  assert(set);
  assert(hwloc_bitmap_weight(set) == 21);
  assert(hwloc_bitmap_first(set) == 0);
  assert(hwloc_bitmap_last(set) == 20);
  set = hwloc_topology_get_allowed_nodeset(topology);
  assert(set);
  assert(hwloc_bitmap_weight(set) == 5);
  assert(hwloc_bitmap_first(set) == 6);
  assert(hwloc_bitmap_last(set) == 10);

  /* restrict to PUs 10-30 and all NUMAs */
  hwloc_bitmap_zero(new);
  hwloc_bitmap_set_range(new, 10, 30);
  err = hwloc_topology_restrict(topology, new, 0);
  assert(!err);
  set = hwloc_topology_get_allowed_cpuset(topology);
  assert(set);
  assert(hwloc_bitmap_weight(set) == 11);
  assert(hwloc_bitmap_first(set) == 10);
  assert(hwloc_bitmap_last(set) == 20);
  set = hwloc_topology_get_allowed_nodeset(topology);
  assert(set);
  assert(hwloc_bitmap_weight(set) == 5);
  assert(hwloc_bitmap_first(set) == 6);
  assert(hwloc_bitmap_last(set) == 10);

  /* restrict to NUMA 4-7 */
  hwloc_bitmap_zero(new);
  hwloc_bitmap_set_range(new, 4, 7);
  err = hwloc_topology_restrict(topology, new, HWLOC_RESTRICT_FLAG_BYNODESET);
  assert(!err);
  set = hwloc_topology_get_allowed_cpuset(topology);
  assert(set);
  assert(hwloc_bitmap_weight(set) == 11);
  assert(hwloc_bitmap_first(set) == 10);
  assert(hwloc_bitmap_last(set) == 20);
  set = hwloc_topology_get_allowed_nodeset(topology);
  assert(set);
  assert(hwloc_bitmap_weight(set) == 2);
  assert(hwloc_bitmap_first(set) == 6);
  assert(hwloc_bitmap_last(set) == 7);

  /* allow all */
  hwloc_bitmap_fill(new);
  err = hwloc_topology_allow(topology, new, new, HWLOC_ALLOW_FLAG_CUSTOM);
  assert(!err);
  set = hwloc_topology_get_allowed_cpuset(topology);
  assert(set);
  assert(hwloc_bitmap_weight(set) == 21);
  assert(hwloc_bitmap_first(set) == 10);
  assert(hwloc_bitmap_last(set) == 30);
  set = hwloc_topology_get_allowed_nodeset(topology);
  assert(set);
  assert(hwloc_bitmap_weight(set) == 4);
  assert(hwloc_bitmap_first(set) == 4);
  assert(hwloc_bitmap_last(set) == 7);

  hwloc_topology_destroy(topology);
  hwloc_bitmap_free(new);

  return 0;
}
