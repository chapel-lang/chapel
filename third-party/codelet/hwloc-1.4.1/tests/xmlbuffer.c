/*
 * Copyright © 2010 inria.  All rights reserved.
 * Copyright © 2011 Cisco Systems, Inc.  All rights reserved.
 * See COPYING in top-level directory.
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <hwloc.h>

static int one_test(void)
{
  hwloc_topology_t topology;
  int size1, size2;
  char *buf1, *buf2;
  int err = 0;

  hwloc_topology_init(&topology);
  hwloc_topology_set_flags(topology, HWLOC_TOPOLOGY_FLAG_WHOLE_IO);
  hwloc_topology_load(topology);
  assert(hwloc_topology_is_thissystem(topology));
  hwloc_topology_export_xmlbuffer(topology, &buf1, &size1);
  hwloc_topology_destroy(topology);
  printf("exported to buffer %p length %d\n", buf1, size1);

  hwloc_topology_init(&topology);
  assert(!hwloc_topology_set_xmlbuffer(topology, buf1, size1));
  hwloc_topology_load(topology);
  assert(!hwloc_topology_is_thissystem(topology));
  hwloc_topology_export_xmlbuffer(topology, &buf2, &size2);
  hwloc_topology_destroy(topology);
  printf("re-exported to buffer %p length %d\n", buf2, size2);

  if (strcmp(buf1, buf2)) {
    printf("### First exported buffer is:\n");
    printf("%s", buf1);
    printf("### End of first export buffer\n");
    printf("### Second exported buffer is:\n");
    printf("%s", buf2);
    printf("### End of second export buffer\n");
    err = 1;
  }

  hwloc_free_xmlbuffer(topology, buf1);
  hwloc_free_xmlbuffer(topology, buf2);

  return err;
}

int main(void)
{
  int err;

  printf("using default import and export\n");
  putenv("HWLOC_NO_LIBXML_IMPORT=0");
  putenv("HWLOC_NO_LIBXML_EXPORT=0");
  err = one_test();
  if (err < 0)
    return err;

  printf("using minimalistic import and default export\n");
  putenv("HWLOC_NO_LIBXML_IMPORT=1");
  putenv("HWLOC_NO_LIBXML_EXPORT=0");
  err = one_test();
  if (err < 0)
    return err;

  printf("using default import and minimalistic export\n");
  putenv("HWLOC_NO_LIBXML_IMPORT=0");
  putenv("HWLOC_NO_LIBXML_EXPORT=1");
  err = one_test();
  if (err < 0)
    return err;

  printf("using minimalistic import and export\n");
  putenv("HWLOC_NO_LIBXML_IMPORT=1");
  putenv("HWLOC_NO_LIBXML_EXPORT=1");
  err = one_test();
  if (err < 0)
    return err;

  return 0;
}
