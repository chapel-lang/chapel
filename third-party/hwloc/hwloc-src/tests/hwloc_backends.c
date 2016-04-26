/*
 * Copyright © 2012-2014 Inria.  All rights reserved.
 * See COPYING in top-level directory.
 */

#include <private/autogen/config.h> /* for HWLOC_WIN_SYS */
#include <hwloc.h>

#include <stdlib.h>
#include <stdio.h>
#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif
#include <string.h>
#include <errno.h>
#include <assert.h>

#if defined(HWLOC_WIN_SYS) && !defined(__CYGWIN__)
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
static inline int mkstemp(char *name)
{
  mktemp(name);
  return open(name, O_RDWR|O_CREAT, S_IRWXU);
}
#endif

/* mostly useful with valgrind, to check if backend cleanup properly */

int main(void)
{
  hwloc_topology_t topology1, topology2;
  char *xmlbuf;
  int xmlbuflen;
  char xmlfile[] = "hwloc_backends.tmpxml.XXXXXX";
  int xmlbufok = 0, xmlfileok = 0, xmlfilefd;
  hwloc_obj_t sw;
  int err;

  printf("trying to export topology to XML buffer and file for later...\n");
  hwloc_topology_init(&topology1);
  hwloc_topology_load(topology1);
  assert(hwloc_topology_is_thissystem(topology1));
  if (hwloc_topology_export_xmlbuffer(topology1, &xmlbuf, &xmlbuflen) < 0)
    printf("XML buffer export failed (%s), ignoring\n", strerror(errno));
  else
    xmlbufok = 1;
  xmlfilefd = mkstemp(xmlfile);
  if (xmlfilefd < 0 || hwloc_topology_export_xml(topology1, xmlfile) < 0)
    printf("XML file export failed (%s), ignoring\n", strerror(errno));
  else
    xmlfileok = 1;


  printf("init...\n");
  hwloc_topology_init(&topology2);
  if (xmlfileok) {
    printf("switching to xml...\n");
    assert(!hwloc_topology_set_xml(topology2, xmlfile));
  }
  if (xmlbufok) {
    printf("switching to xmlbuffer...\n");
    assert(!hwloc_topology_set_xmlbuffer(topology2, xmlbuf, xmlbuflen));
  }
  printf("switching to custom...\n");
  hwloc_topology_set_custom(topology2);
  printf("switching to synthetic...\n");
  hwloc_topology_set_synthetic(topology2, "machine:2 node:3 cache:2 pu:4");
  printf("switching sysfs fsroot to // ...\n");
  hwloc_topology_set_fsroot(topology2, "//"); /* valid path that won't be recognized as '/' */
  printf("switching sysfs fsroot to / ...\n");
  hwloc_topology_set_fsroot(topology2, "/");
  hwloc_topology_destroy(topology2);

  if (xmlfileok) {
    printf("switching to xml and loading...\n");
    hwloc_topology_init(&topology2);
    assert(!hwloc_topology_set_xml(topology2, xmlfile));
    hwloc_topology_load(topology2);
    hwloc_topology_check(topology2);
    assert(!hwloc_topology_is_thissystem(topology2));
    hwloc_topology_destroy(topology2);
  }

  if (xmlbufok) {
    printf("switching to xmlbuffer and loading...\n");
    hwloc_topology_init(&topology2);
    assert(!hwloc_topology_set_xmlbuffer(topology2, xmlbuf, xmlbuflen));
    hwloc_topology_load(topology2);
    hwloc_topology_check(topology2);
    assert(!hwloc_topology_is_thissystem(topology2));
    hwloc_topology_destroy(topology2);
  }

  printf("switching to custom and loading...\n");
  hwloc_topology_init(&topology2);
  hwloc_topology_set_custom(topology2);
  sw = hwloc_custom_insert_group_object_by_parent(topology2, hwloc_get_root_obj(topology2), 0);
  assert(sw);
  hwloc_custom_insert_topology(topology2, sw, topology1, NULL);
  hwloc_topology_load(topology2);
  hwloc_topology_check(topology2);
  assert(!hwloc_topology_is_thissystem(topology2));
  hwloc_topology_destroy(topology2);

  printf("switching to synthetic and loading...\n");
  hwloc_topology_init(&topology2);
  hwloc_topology_set_synthetic(topology2, "machine:2 node:3 cache:2 pu:4");
  hwloc_topology_load(topology2);
  hwloc_topology_check(topology2);
  assert(!hwloc_topology_is_thissystem(topology2));
  hwloc_topology_destroy(topology2);

  printf("switching sysfs fsroot to // and loading...\n");
  hwloc_topology_init(&topology2);
  err = hwloc_topology_set_fsroot(topology2, "//"); /* '//' isn't recognized as the normal fsroot on Linux, and it fails and falls back to normal topology on !Linux */
  hwloc_topology_load(topology2);
  hwloc_topology_check(topology2);
  assert(!hwloc_topology_is_thissystem(topology2) == !err);
  hwloc_topology_destroy(topology2);

  printf("switching sysfs fsroot to / and loading...\n");
  hwloc_topology_init(&topology2);
  err = hwloc_topology_set_fsroot(topology2, "/");
  hwloc_topology_load(topology2);
  hwloc_topology_check(topology2);
  assert(hwloc_topology_is_thissystem(topology2)); /* '/' is recognized as the normal fsroot on Linux, and it fails and falls back to normal topology on !Linux */
  hwloc_topology_destroy(topology2);

  printf("switching to synthetic...\n");
  hwloc_topology_init(&topology2);
  hwloc_topology_set_synthetic(topology2, "machine:2 node:3 cache:2 pu:4");
  hwloc_topology_destroy(topology2);


  if (xmlbufok)
    hwloc_free_xmlbuffer(topology1, xmlbuf);
  if (xmlfilefd >= 0) {
    unlink(xmlfile);
    close(xmlfilefd);
  }
  hwloc_topology_destroy(topology1);

  return 0;
}
