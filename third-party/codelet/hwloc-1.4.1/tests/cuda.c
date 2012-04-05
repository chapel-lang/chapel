/*
 * Copyright © 2010 inria.  All rights reserved.
 * Copyright © 2011 Cisco Systems, Inc.  All rights reserved.
 * See COPYING in top-level directory.
 */

#include <stdio.h>
#include <assert.h>
#include <cuda.h>
#include <hwloc.h>
#include <hwloc/cuda.h>

/* check the CUDA Driver API helpers */

int main(void)
{
  hwloc_topology_t topology;
  CUresult cres;
  CUdevice device;
  int count, i;
  int err;

  cres = cuInit(0);
  if (cres != CUDA_SUCCESS) {
    printf("cuInit failed %d\n", cres);
    return 0;
  }

  cres = cuDeviceGetCount(&count);
  if (cres != CUDA_SUCCESS) {
    printf("cuDeviceGetCount failed %d\n", cres);
    return 0;
  }
  printf("cuDeviceGetCount found %d devices\n", count);

  hwloc_topology_init(&topology);
  hwloc_topology_load(topology);

  for(i=0; i<count; i++) {
    hwloc_bitmap_t set;

    cres = cuDeviceGet(&device, i);
    if (cres != CUDA_SUCCESS) {
      printf("failed to get device %d\n", i);
      continue;
    }

    set = hwloc_bitmap_alloc();
    err = hwloc_cuda_get_device_cpuset(topology, device, set);
    if (err < 0) {
      printf("failed to get cpuset for device %d\n", i);
    } else {
      char *cpuset_string = NULL;
      hwloc_bitmap_asprintf(&cpuset_string, set);
      printf("got cpuset %s for device %d\n", cpuset_string, i);
      free(cpuset_string);
    }
    hwloc_bitmap_free(set);
  }

  hwloc_topology_destroy(topology);

  return 0;
}
