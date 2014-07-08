/*
 * Copyright © 2010 inria.  All rights reserved.
 * See COPYING in top-level directory.
 */

#include <stdio.h>
#include <stdlib.h>
#include <myriexpress.h>
#include <hwloc.h>
#include <hwloc/myriexpress.h>

/* check the myriexpress helpers */

int main(void)
{
  hwloc_topology_t topology;
  mx_return_t ret;
  unsigned i, count;
  int err;

  putenv("MX_ERRORS_ARE_FATAL=0");

  ret = mx_init();
  if (ret != MX_SUCCESS) {
    fprintf(stderr, "mx_init failed, %s\n", mx_strerror(ret));
    return 0;
  }

  mx_get_info(NULL, MX_NIC_COUNT, NULL, 0, &count, sizeof(count));
  if (ret != MX_SUCCESS) {
    fprintf(stderr, "mx_get_info MX_NIC_COUNT failed, %s\n", mx_strerror(ret));
    return 0;
  }

  hwloc_topology_init(&topology);
  hwloc_topology_load(topology);

  for(i=0; i<count; i++) {
    mx_endpoint_t ep;
    char *cpuset_string;
    hwloc_bitmap_t set;

    ret = mx_open_endpoint(i, MX_ANY_ENDPOINT, 0, NULL, 0, &ep);
    if (ret != MX_SUCCESS)
      continue;
    set = hwloc_bitmap_alloc();
    err = hwloc_mx_endpoint_get_device_cpuset(topology, ep, set);
    if (err < 0) {
      perror("hwloc_mx_endpoint_get_device_cpuset failed");
      return -1;
    }
    hwloc_bitmap_asprintf(&cpuset_string, set);
    printf("got cpuset %s for endpoint on board #%d\n",
	   cpuset_string, i);
    free(cpuset_string);
    hwloc_bitmap_free(set);
    mx_close_endpoint(ep);

    set = hwloc_bitmap_alloc();
    err = hwloc_mx_board_get_device_cpuset(topology, i, set);
    if (err < 0) {
      perror("hwloc_mx_board_get_device_cpuset failed");
      return -1;
    }
    hwloc_bitmap_asprintf(&cpuset_string, set);
    printf("got cpuset %s for board #%d\n",
           cpuset_string, i);
    free(cpuset_string);
    hwloc_bitmap_free(set);
  }

  hwloc_topology_destroy(topology);

  mx_finalize();

  return 0;
}
