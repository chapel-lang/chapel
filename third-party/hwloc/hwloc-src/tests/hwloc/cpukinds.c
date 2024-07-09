/*
 * Copyright © 2020 Inria.  All rights reserved.
 * See COPYING in top-level directory.
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "hwloc.h"

int main(void)
{
  hwloc_topology_t topology;
  int efficiency;
  hwloc_bitmap_t cpuset;
  struct hwloc_info_s info, *infosp;
  unsigned nr_infos;
  int err;

  err = hwloc_topology_init(&topology);
  assert(!err);
  err = hwloc_topology_set_synthetic(topology, "pack:4 pu:3");
  assert(!err);
  err = hwloc_topology_load(topology);
  assert(!err);

  cpuset = hwloc_bitmap_alloc();
  assert(cpuset);

  /* no cpukinds */

  /* get_nr with invalid flags */
  err = hwloc_cpukinds_get_nr(topology, 1 /* invalid flag */);
  assert(err == -1);
  assert(errno == EINVAL);
  /* get_nr */
  err = hwloc_cpukinds_get_nr(topology, 0);
  assert(err == 0);

  /* get_info with invalid flags */
  err = hwloc_cpukinds_get_info(topology, 0, NULL, NULL, NULL, NULL, 1 /* invalid flag */);
  assert(err == -1);
  assert(errno == EINVAL);
  /* get kind, non-existing */
  err = hwloc_cpukinds_get_info(topology, 0, NULL, NULL, NULL, NULL, 0);
  assert(err == -1);
  assert(errno == ENOENT);

  /* get_by_cpuset with no cpuset */
  err = hwloc_cpukinds_get_by_cpuset(topology, NULL, 0);
  assert(err == -1);
  assert(errno == EINVAL);
  /* get_by_cpuset with empty cpuset */
  hwloc_bitmap_zero(cpuset);
  err = hwloc_cpukinds_get_by_cpuset(topology, cpuset, 0);
  assert(err == -1);
  assert(errno == EINVAL);
  /* get_by_cpuset with invalid flags */
  hwloc_bitmap_zero(cpuset);
  hwloc_bitmap_set_range(cpuset, 0, 11);
  err = hwloc_cpukinds_get_by_cpuset(topology, cpuset, 1 /* invalid flag */);
  assert(err == -1);
  assert(errno == EINVAL);
  /* get_by_cpuset, non-existing */
  hwloc_bitmap_zero(cpuset);
  hwloc_bitmap_set_range(cpuset, 0, 11);
  err = hwloc_cpukinds_get_by_cpuset(topology, cpuset, 0);
  assert(err == -1);
  assert(errno == ENOENT);

  /* register with no cpuset */
  err = hwloc_cpukinds_register(topology, NULL, 0, 0, NULL, 0);
  assert(err == -1);
  assert(errno == EINVAL);
  /* register with empty cpuset */
  hwloc_bitmap_zero(cpuset);
  err = hwloc_cpukinds_register(topology, cpuset, 0, 0, NULL, 0);
  assert(err == -1);
  assert(errno == EINVAL);
  /* register with invalid flags */
  hwloc_bitmap_set_range(cpuset, 0, 5);
  err = hwloc_cpukinds_register(topology, cpuset, 0, 0, NULL, 1 /* invalid flag */);
  assert(err == -1);
  assert(errno == EINVAL);

  /* PU 0-5 (first two packages) are big and efficient */
  hwloc_bitmap_zero(cpuset);
  hwloc_bitmap_set_range(cpuset, 0, 5);
  info.name = (char*) "CoreType";
  info.value = (char*) "BigCore";
  err = hwloc_cpukinds_register(topology, cpuset, 1000, 1, &info, 0);
  assert(!err);
  /* PU 6-9 (third package) are small and less efficient */
  hwloc_bitmap_zero(cpuset);
  hwloc_bitmap_set_range(cpuset, 6, 8);
  info.name = (char*) "CoreType";
  info.value = (char*) "SmallCore";
  err = hwloc_cpukinds_register(topology, cpuset, 10, 1, &info, 0);
  assert(!err);

  /* check those kinds */
  err = hwloc_cpukinds_get_nr(topology, 0);
  assert(err == 2);
  /* first one should be second registered one */
  hwloc_bitmap_zero(cpuset);
  err = hwloc_cpukinds_get_info(topology, 0, cpuset, &efficiency, &nr_infos, &infosp, 0);
  assert(!err);
  assert(hwloc_bitmap_weight(cpuset) == 3);
  assert(hwloc_bitmap_first(cpuset) == 6);
  assert(hwloc_bitmap_last(cpuset) == 8);
  assert(efficiency == 0);
  assert(nr_infos == 1);
  assert(!strcmp(infosp[0].name, "CoreType"));
  assert(!strcmp(infosp[0].value, "SmallCore"));
  /* second one should be first registered one */
  hwloc_bitmap_zero(cpuset);
  err = hwloc_cpukinds_get_info(topology, 1, cpuset, &efficiency, &nr_infos, &infosp, 0);
  assert(!err);
  assert(hwloc_bitmap_weight(cpuset) == 6);
  assert(hwloc_bitmap_first(cpuset) == 0);
  assert(hwloc_bitmap_last(cpuset) == 5);
  assert(efficiency == 1);
  assert(nr_infos == 1);
  assert(!strcmp(infosp[0].name, "CoreType"));
  assert(!strcmp(infosp[0].value, "BigCore"));
  /* try without getting any info too */
  err = hwloc_cpukinds_get_info(topology, 1, NULL, NULL, NULL, NULL, 0);
  assert(!err);
  /* third one is invalid */
  err = hwloc_cpukinds_get_info(topology, 2, NULL, NULL, NULL, NULL, 0);
  assert(err == -1);
  assert(errno == ENOENT);

  /* get_by_cpuset */
  hwloc_bitmap_zero(cpuset);
  hwloc_bitmap_set_range(cpuset, 1, 3);
  err = hwloc_cpukinds_get_by_cpuset(topology, cpuset, 0);
  assert(err == 1);
  hwloc_bitmap_zero(cpuset);
  hwloc_bitmap_set_range(cpuset, 7, 8);
  err = hwloc_cpukinds_get_by_cpuset(topology, cpuset, 0);
  assert(err == 0);
  /* get_by_cpuset across kinds */
  hwloc_bitmap_zero(cpuset);
  hwloc_bitmap_set_range(cpuset, 5, 6);
  err = hwloc_cpukinds_get_by_cpuset(topology, cpuset, 0);
  assert(err == -1);
  assert(errno == EXDEV);
  /* get_by_cpuset outside of kinds */
  hwloc_bitmap_zero(cpuset);
  hwloc_bitmap_set_range(cpuset, 9, 11);
  err = hwloc_cpukinds_get_by_cpuset(topology, cpuset, 0);
  assert(err == -1);
  assert(errno == ENOENT);

  /* PU 5-9 (part of packages #1 and #3, and all of #2) have some features */
  hwloc_bitmap_zero(cpuset);
  hwloc_bitmap_set_range(cpuset, 5, 10);
  info.name = (char*) "Features";
  info.value = (char*) "this, that and those";
  err = hwloc_cpukinds_register(topology, cpuset, -1, 1, &info, 0);
  assert(!err);

  /* check those kinds */
  err = hwloc_cpukinds_get_nr(topology, 0);
  assert(err == 4);
  /* WARNING: the order is now implementation dependent, since we cannot order by efficiency here */
  /* first one is 6-8 SmallCore Features */
  hwloc_bitmap_zero(cpuset);
  err = hwloc_cpukinds_get_info(topology, 0, cpuset, &efficiency, &nr_infos, &infosp, 0);
  assert(!err);
  assert(hwloc_bitmap_weight(cpuset) == 3);
  assert(hwloc_bitmap_first(cpuset) == 6);
  assert(hwloc_bitmap_last(cpuset) == 8);
  assert(efficiency == -1);
  assert(nr_infos == 2);
  assert(!strcmp(infosp[0].name, "CoreType"));
  assert(!strcmp(infosp[0].value, "SmallCore"));
  assert(!strcmp(infosp[1].name, "Features"));
  assert(!strcmp(infosp[1].value, "this, that and those"));
  /* second one is 0-4 BigCore */
  hwloc_bitmap_zero(cpuset);
  err = hwloc_cpukinds_get_info(topology, 1, cpuset, &efficiency, &nr_infos, &infosp, 0);
  assert(!err);
  assert(hwloc_bitmap_weight(cpuset) == 5);
  assert(hwloc_bitmap_first(cpuset) == 0);
  assert(hwloc_bitmap_last(cpuset) == 4);
  assert(efficiency == -1);
  assert(nr_infos == 1);
  assert(!strcmp(infosp[0].name, "CoreType"));
  assert(!strcmp(infosp[0].value, "BigCore"));
  /* third one is 5-5 BigCore Features */
  hwloc_bitmap_zero(cpuset);
  err = hwloc_cpukinds_get_info(topology, 2, cpuset, &efficiency, &nr_infos, &infosp, 0);
  assert(!err);
  assert(hwloc_bitmap_weight(cpuset) == 1);
  assert(hwloc_bitmap_first(cpuset) == 5);
  assert(hwloc_bitmap_last(cpuset) == 5);
  assert(efficiency == -1);
  assert(nr_infos == 2);
  assert(!strcmp(infosp[0].name, "CoreType"));
  assert(!strcmp(infosp[0].value, "BigCore"));
  assert(!strcmp(infosp[1].name, "Features"));
  assert(!strcmp(infosp[1].value, "this, that and those"));
  /* fourth one is 9-10 Features */
  hwloc_bitmap_zero(cpuset);
  err = hwloc_cpukinds_get_info(topology, 3, cpuset, &efficiency, &nr_infos, &infosp, 0);
  assert(!err);
  assert(hwloc_bitmap_weight(cpuset) == 2);
  assert(hwloc_bitmap_first(cpuset) == 9);
  assert(hwloc_bitmap_last(cpuset) == 10);
  assert(efficiency == -1);
  assert(nr_infos == 1);
  assert(!strcmp(infosp[0].name, "Features"));
  assert(!strcmp(infosp[0].value, "this, that and those"));
  /* fifth one is invalid */
  err = hwloc_cpukinds_get_info(topology, 4, NULL, NULL, NULL, NULL, 0);
  assert(err == -1);
  assert(errno == ENOENT);
  
  /* get_by_cpuset */
  hwloc_bitmap_zero(cpuset);
  hwloc_bitmap_set_range(cpuset, 1, 3);
  err = hwloc_cpukinds_get_by_cpuset(topology, cpuset, 0);
  assert(err == 1);
  hwloc_bitmap_zero(cpuset);
  hwloc_bitmap_set_range(cpuset, 7, 8);
  err = hwloc_cpukinds_get_by_cpuset(topology, cpuset, 0);
  assert(err == 0);
  hwloc_bitmap_zero(cpuset);
  hwloc_bitmap_set_range(cpuset, 5, 5);
  err = hwloc_cpukinds_get_by_cpuset(topology, cpuset, 0);
  assert(err == 2);
  /* get_by_cpuset across kinds */
  hwloc_bitmap_zero(cpuset);
  hwloc_bitmap_set_range(cpuset, 10, 11);
  err = hwloc_cpukinds_get_by_cpuset(topology, cpuset, 0);
  assert(err == -1);
  assert(errno == EXDEV);
  /* get_by_cpuset outside of kinds */
  hwloc_bitmap_zero(cpuset);
  hwloc_bitmap_set_range(cpuset, 11, 11);
  err = hwloc_cpukinds_get_by_cpuset(topology, cpuset, 0);
  assert(err == -1);
  assert(errno == ENOENT);

  /* now force efficiency of those kinds */
  hwloc_bitmap_zero(cpuset);
  hwloc_bitmap_set_range(cpuset, 0, 4);
  err = hwloc_cpukinds_register(topology, cpuset, 1000, 0, NULL, 0);
  assert(!err);
  hwloc_bitmap_zero(cpuset);
  hwloc_bitmap_set_range(cpuset, 5, 5);
  err = hwloc_cpukinds_register(topology, cpuset, 100, 0, NULL, 0);
  assert(!err);
  hwloc_bitmap_zero(cpuset);
  hwloc_bitmap_set_range(cpuset, 6, 8);
  err = hwloc_cpukinds_register(topology, cpuset, 10, 0, NULL, 0);
  assert(!err);
  hwloc_bitmap_zero(cpuset);
  hwloc_bitmap_set_range(cpuset, 9, 10);
  err = hwloc_cpukinds_register(topology, cpuset, 1, 0, NULL, 0);
  assert(!err);
  
  /* check those kinds */
  err = hwloc_cpukinds_get_nr(topology, 0);
  assert(err == 4);
  /* first one is 9-10 Features */
  hwloc_bitmap_zero(cpuset);
  err = hwloc_cpukinds_get_info(topology, 0, cpuset, &efficiency, &nr_infos, &infosp, 0);
  assert(!err);
  assert(hwloc_bitmap_weight(cpuset) == 2);
  assert(hwloc_bitmap_first(cpuset) == 9);
  assert(hwloc_bitmap_last(cpuset) == 10);
  assert(efficiency == 0);
  assert(nr_infos == 1);
  assert(!strcmp(infosp[0].name, "Features"));
  assert(!strcmp(infosp[0].value, "this, that and those"));
  /* second one is 6-8 SmallCore Features */
  hwloc_bitmap_zero(cpuset);
  err = hwloc_cpukinds_get_info(topology, 1, cpuset, &efficiency, &nr_infos, &infosp, 0);
  assert(!err);
  assert(hwloc_bitmap_weight(cpuset) == 3);
  assert(hwloc_bitmap_first(cpuset) == 6);
  assert(hwloc_bitmap_last(cpuset) == 8);
  assert(efficiency == 1);
  assert(nr_infos == 2);
  assert(!strcmp(infosp[0].name, "CoreType"));
  assert(!strcmp(infosp[0].value, "SmallCore"));
  assert(!strcmp(infosp[1].name, "Features"));
  assert(!strcmp(infosp[1].value, "this, that and those"));
  /* third one is 5-5 BigCore Features */
  hwloc_bitmap_zero(cpuset);
  err = hwloc_cpukinds_get_info(topology, 2, cpuset, &efficiency, &nr_infos, &infosp, 0);
  assert(!err);
  assert(hwloc_bitmap_weight(cpuset) == 1);
  assert(hwloc_bitmap_first(cpuset) == 5);
  assert(hwloc_bitmap_last(cpuset) == 5);
  assert(efficiency == 2);
  assert(nr_infos == 2);
  assert(!strcmp(infosp[0].name, "CoreType"));
  assert(!strcmp(infosp[0].value, "BigCore"));
  assert(!strcmp(infosp[1].name, "Features"));
  assert(!strcmp(infosp[1].value, "this, that and those"));
  /* fourth one is 0-4 BigCore */
  hwloc_bitmap_zero(cpuset);
  err = hwloc_cpukinds_get_info(topology, 3, cpuset, &efficiency, &nr_infos, &infosp, 0);
  assert(!err);
  assert(hwloc_bitmap_weight(cpuset) == 5);
  assert(hwloc_bitmap_first(cpuset) == 0);
  assert(hwloc_bitmap_last(cpuset) == 4);
  assert(efficiency == 3);
  assert(nr_infos == 1);
  assert(!strcmp(infosp[0].name, "CoreType"));
  assert(!strcmp(infosp[0].value, "BigCore"));
  /* fifth one is invalid */
  err = hwloc_cpukinds_get_info(topology, 4, NULL, NULL, NULL, NULL, 0);
  assert(err == -1);
  assert(errno == ENOENT);

  /* restrict to remove PU 5-6 (third kind and part of second), PU 11 (no kind) and PU 0-2 (part of fourth kind) */
  hwloc_bitmap_zero(cpuset);
  hwloc_bitmap_set_range(cpuset, 3, 10);
  hwloc_bitmap_clr_range(cpuset, 5, 6);
  err = hwloc_topology_restrict(topology, cpuset, 0);
  assert(!err);

  /* check remaining kinds */
  err = hwloc_cpukinds_get_nr(topology, 0);
  assert(err == 3);
  /* first one is 9-10 Features */
  hwloc_bitmap_zero(cpuset);
  err = hwloc_cpukinds_get_info(topology, 0, cpuset, &efficiency, &nr_infos, &infosp, 0);
  assert(!err);
  assert(hwloc_bitmap_weight(cpuset) == 2);
  assert(hwloc_bitmap_first(cpuset) == 9);
  assert(hwloc_bitmap_last(cpuset) == 10);
  assert(efficiency == 0);
  assert(nr_infos == 1);
  assert(!strcmp(infosp[0].name, "Features"));
  assert(!strcmp(infosp[0].value, "this, that and those"));
  /* second one is 7-8 SmallCore Features */
  hwloc_bitmap_zero(cpuset);
  err = hwloc_cpukinds_get_info(topology, 1, cpuset, &efficiency, &nr_infos, &infosp, 0);
  assert(!err);
  assert(hwloc_bitmap_weight(cpuset) == 2);
  assert(hwloc_bitmap_first(cpuset) == 7);
  assert(hwloc_bitmap_last(cpuset) == 8);
  assert(efficiency == 1);
  assert(nr_infos == 2);
  assert(!strcmp(infosp[0].name, "CoreType"));
  assert(!strcmp(infosp[0].value, "SmallCore"));
  assert(!strcmp(infosp[1].name, "Features"));
  assert(!strcmp(infosp[1].value, "this, that and those"));
  /* third one is 3-4 BigCore */
  hwloc_bitmap_zero(cpuset);
  err = hwloc_cpukinds_get_info(topology, 2, cpuset, &efficiency, &nr_infos, &infosp, 0);
  assert(!err);
  assert(hwloc_bitmap_weight(cpuset) == 2);
  assert(hwloc_bitmap_first(cpuset) == 3);
  assert(hwloc_bitmap_last(cpuset) == 4);
  assert(efficiency == 2);
  assert(nr_infos == 1);
  assert(!strcmp(infosp[0].name, "CoreType"));
  assert(!strcmp(infosp[0].value, "BigCore"));
  /* fourth one is invalid */
  err = hwloc_cpukinds_get_info(topology, 3, NULL, NULL, NULL, NULL, 0);
  assert(err == -1);
  assert(errno == ENOENT);

  hwloc_bitmap_free(cpuset);
  hwloc_topology_destroy(topology);
  return 0;
}
