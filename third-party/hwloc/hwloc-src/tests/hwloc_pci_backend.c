/*
 * Copyright © 2011-2012 Inria.  All rights reserved.
 * See COPYING in top-level directory.
 */

#include <hwloc.h>

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

/* check whether the PCI backend behaves as expected wrt to thissystem, XML, flags, ... */

static int get_nb_pcidev(unsigned long flags, int thissystem,
			 const char *xmlbuf, int xmlbuflen)
{
  int nb;
  hwloc_topology_t topology;

  if (thissystem)
    putenv("HWLOC_THISSYSTEM=1");
  else
    putenv("HWLOC_THISSYSTEM=0");
  hwloc_topology_init(&topology);
  hwloc_topology_set_flags(topology, flags);
  if (xmlbuf)
    hwloc_topology_set_xmlbuffer(topology, xmlbuf, xmlbuflen);
  hwloc_topology_load(topology);

  nb = hwloc_get_nbobjs_by_depth(topology, HWLOC_TYPE_DEPTH_PCI_DEVICE);

  hwloc_topology_destroy(topology);

  return nb;
}

int main(void)
{
  hwloc_topology_t topology;
  char *xmlbuf;
  int xmlbuflen;
  int nb, nbnormal, nbwhole;

  hwloc_topology_init(&topology);
  hwloc_topology_set_flags(topology, HWLOC_TOPOLOGY_FLAG_IO_DEVICES|HWLOC_TOPOLOGY_FLAG_WHOLE_IO);
  hwloc_topology_load(topology);
  if (hwloc_topology_export_xmlbuffer(topology, &xmlbuf, &xmlbuflen) < 0)
    printf("XML buffer export failed (%s), ignoring\n", strerror(errno));

  /* with HWLOC_THISSYSTEM=1 */
  nb = get_nb_pcidev(0, 1, NULL, 0);
  assert(!nb);
  nbnormal = get_nb_pcidev(HWLOC_TOPOLOGY_FLAG_IO_DEVICES, 1, NULL, 0);
  assert(nbnormal >= 0); /* may get more objects */
  nbwhole = get_nb_pcidev(HWLOC_TOPOLOGY_FLAG_IO_DEVICES|HWLOC_TOPOLOGY_FLAG_WHOLE_IO, 1, NULL, 0);
  assert(nbwhole >= nbnormal); /* will get at least as much objects */

  /* XML with with HWLOC_THISSYSTEM=1, should get as many object as a native load */
  nb = get_nb_pcidev(0, 1, xmlbuf, xmlbuflen);
  assert(!nb);
  nb = get_nb_pcidev(HWLOC_TOPOLOGY_FLAG_IO_DEVICES, 1, xmlbuf, xmlbuflen);
  assert(nb == nbnormal);
  nb = get_nb_pcidev(HWLOC_TOPOLOGY_FLAG_IO_DEVICES|HWLOC_TOPOLOGY_FLAG_WHOLE_IO, 1, xmlbuf, xmlbuflen);
  assert(nb == nbwhole);

  /* XML with with HWLOC_THISSYSTEM=0,  should get as many object as a native load */
  nb = get_nb_pcidev(0, 0, xmlbuf, xmlbuflen);
  assert(!nb);
  nb = get_nb_pcidev(HWLOC_TOPOLOGY_FLAG_IO_DEVICES, 0, xmlbuf, xmlbuflen);
  assert(nb == nbnormal);
  nb = get_nb_pcidev(HWLOC_TOPOLOGY_FLAG_IO_DEVICES|HWLOC_TOPOLOGY_FLAG_WHOLE_IO, 0, xmlbuf, xmlbuflen);
  assert(nb == nbwhole);

  /* make sure we don't use linuxpci backend, it works fine when HWLOC_THISSYSTEM=0 */
  putenv("HWLOC_COMPONENTS=-linuxpci");
  /* with HWLOC_THISSYSTEM=0, won't get any object */
  nb = get_nb_pcidev(0, 0, NULL, 0);
  assert(!nb);
  nb = get_nb_pcidev(HWLOC_TOPOLOGY_FLAG_IO_DEVICES, 0, NULL, 0);
  assert(!nb);
  nb = get_nb_pcidev(HWLOC_TOPOLOGY_FLAG_IO_DEVICES|HWLOC_TOPOLOGY_FLAG_WHOLE_IO, 0, NULL, 0);
  assert(!nb);

  hwloc_free_xmlbuffer(topology, xmlbuf);
  hwloc_topology_destroy(topology);

  return 0;
}
