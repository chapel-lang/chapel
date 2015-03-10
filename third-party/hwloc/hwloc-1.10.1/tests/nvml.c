/*
 * Copyright © 2012 Inria.  All rights reserved.
 * See COPYING in top-level directory.
 */

#include <stdio.h>
#include <assert.h>
#include <nvml.h>
#include <hwloc.h>
#include <hwloc/nvml.h>

/* check the NVML helpers */

int main(void)
{
  hwloc_topology_t topology;
  nvmlReturn_t nvres;
  nvmlDevice_t device;
  unsigned count, i;
  int err;

  nvres = nvmlInit();
  if (nvres != NVML_SUCCESS) {
    printf("nvmlInit failed %d\n", nvres);
    return 0;
  }

  nvres = nvmlDeviceGetCount(&count);
  if (nvres != NVML_SUCCESS) {
    printf("nvmlDeviceGetCount failed %d\n", nvres);
    return 0;
  }
  printf("nvmlDeviceGetCount found %d devices\n", count);

  hwloc_topology_init(&topology);
  hwloc_topology_set_flags(topology, HWLOC_TOPOLOGY_FLAG_IO_DEVICES);
  hwloc_topology_load(topology);

  for(i=0; i<count; i++) {
    hwloc_bitmap_t set;
    hwloc_obj_t osdev, osdev2, ancestor;
    const char *value;

    nvres = nvmlDeviceGetHandleByIndex(i, &device);
    if (nvres != NVML_SUCCESS) {
      printf("nvmlDeviceGetHandleByIndex failed %d\n", nvres);
      continue;
    }

    osdev = hwloc_nvml_get_device_osdev(topology, device);
    assert(osdev);
    osdev2 = hwloc_nvml_get_device_osdev_by_index(topology, i);
    assert(osdev2 == osdev);

    ancestor = hwloc_get_non_io_ancestor_obj(topology, osdev);

    printf("found OSDev %s\n", osdev->name);
    err = strncmp(osdev->name, "nvml", 4);
    assert(!err);
    assert(atoi(osdev->name+4) == (int) i);

    value = hwloc_obj_get_info_by_name(osdev, "Backend");
    err = strcmp(value, "NVML");
    assert(!err);

    assert(osdev->attr->osdev.type == HWLOC_OBJ_OSDEV_GPU);

    value = hwloc_obj_get_info_by_name(osdev, "GPUModel");
    printf("found OSDev model %s\n", value);

    set = hwloc_bitmap_alloc();
    err = hwloc_nvml_get_device_cpuset(topology, device, set);
    if (err < 0) {
      printf("failed to get cpuset for device %d\n", i);
    } else {
      char *cpuset_string = NULL;
      hwloc_bitmap_asprintf(&cpuset_string, set);
      printf("got cpuset %s for device %d\n", cpuset_string, i);
      free(cpuset_string);
      assert(hwloc_bitmap_isequal(set, ancestor->cpuset));
    }
    hwloc_bitmap_free(set);
  }

  hwloc_topology_destroy(topology);

  nvmlShutdown();

  return 0;
}
