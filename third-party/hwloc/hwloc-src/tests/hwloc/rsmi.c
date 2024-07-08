/*
 * Copyright © 2012-2020 Inria.  All rights reserved.
 * Copyright (c) 2020, Advanced Micro Devices, Inc. All rights reserved.
 * Written by Advanced Micro Devices,
 * See COPYING in top-level directory.
 */

#include <stdio.h>
#include <assert.h>
#include <rocm_smi/rocm_smi.h>

#include "hwloc.h"
#include "hwloc/rsmi.h"

/* check the RSMI helpers */

int main(void)
{
  hwloc_topology_t topology;
  rsmi_status_t ret;
  unsigned count, i;
  int err = 0;

  rsmi_init(0);

  ret = rsmi_num_monitor_devices(&count);
  if (RSMI_STATUS_SUCCESS != ret || !count) {
    rsmi_shut_down();
    if (RSMI_STATUS_SUCCESS != ret) {
      fprintf(stderr, "Failed to get device count\n");
      return -1;
    } else {
      fprintf(stderr, "No GPU available\n");
      return 0;
    }	    
  }
  printf("rsmi_num_monitor_devices found %u devices\n", count);

  hwloc_topology_init(&topology);
  hwloc_topology_set_io_types_filter(topology, HWLOC_TYPE_FILTER_KEEP_IMPORTANT);
  hwloc_topology_load(topology);

  for (i=0; i<count; i++) {
    hwloc_bitmap_t set;
    hwloc_obj_t osdev, osdev2, ancestor;
    const char *value;

    osdev = hwloc_rsmi_get_device_osdev(topology, i);
    assert(osdev);
    osdev2 = hwloc_rsmi_get_device_osdev_by_index(topology, i);
    assert(osdev2 == osdev);

    ancestor = hwloc_get_non_io_ancestor_obj(topology, osdev);

    printf("found OSDev %s\n", osdev->name);
    err = strncmp(osdev->name, "rsmi", 4);
    assert(!err);
    assert(atoi(osdev->name+4) == (int) i);

    value = hwloc_obj_get_info_by_name(osdev, "Backend");
    err = strcmp(value, "RSMI");
    assert(!err);

    assert(osdev->attr->osdev.type == HWLOC_OBJ_OSDEV_GPU);

    value = hwloc_obj_get_info_by_name(osdev, "GPUModel");
    printf("found OSDev model %s\n", value);

    set = hwloc_bitmap_alloc();
    err = hwloc_rsmi_get_device_cpuset(topology, i, set);
    if (err < 0) {
      printf("failed to get cpuset for device %u\n", i);
    } else {
      char *cpuset_string = NULL;
      hwloc_bitmap_asprintf(&cpuset_string, set);
      printf("got cpuset %s for device %u\n", cpuset_string, i);
      free(cpuset_string);
      if (hwloc_bitmap_isequal(hwloc_topology_get_complete_cpuset(topology), hwloc_topology_get_topology_cpuset(topology)))
        /* only compare if the topology is complete, otherwise things can be significantly different */
        assert(hwloc_bitmap_isequal(set, ancestor->cpuset));
    }
    hwloc_bitmap_free(set);
  }

  hwloc_topology_destroy(topology);

  rsmi_shut_down();

  return err;
}
