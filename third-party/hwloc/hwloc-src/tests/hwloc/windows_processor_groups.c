/*
 * Copyright © 2021 Inria.  All rights reserved.
 * See COPYING in top-level directory.
 */

#include "private/autogen/config.h"
#include "hwloc.h"
#include "hwloc/windows.h"

#include <assert.h>

int main(void)
{
  hwloc_topology_t topology;
  hwloc_bitmap_t set;
  int nr, i, err;
  char *s;

  hwloc_topology_init(&topology);
  hwloc_topology_load(topology);

  set = hwloc_bitmap_alloc();
  assert(set);

  /* check invalid flag */
  nr = hwloc_windows_get_nr_processor_groups(topology, 1);
  assert(nr < 0);
  assert(errno == EINVAL);

  nr = hwloc_windows_get_nr_processor_groups(topology, 0);
  assert(nr >= 1);
  printf("found %d groups\n", nr);

  /* check invalid flag */
  err = hwloc_windows_get_processor_group_cpuset(topology, 0, set, 1);
  assert(err < 0);
  assert(errno == EINVAL);
  /* check invalid index */
  err = hwloc_windows_get_processor_group_cpuset(topology, nr, set, 0);
  assert(err < 0);
  assert(errno == ENOENT);

  for(i=0; i<nr; i++) {
    hwloc_bitmap_zero(set);
    err = hwloc_windows_get_processor_group_cpuset(topology, i, set, 0);
    assert(err == 0);
    assert(!hwloc_bitmap_iszero(set));
    err = hwloc_bitmap_asprintf(&s, set);
    assert(err >= 0);
    printf("processor group #%d has cpuset %s\n", i, s);
    free(s);
  }

  hwloc_bitmap_free(set);
  hwloc_topology_destroy(topology);
  return 0;
}
