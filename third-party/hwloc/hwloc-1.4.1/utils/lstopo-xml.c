/*
 * Copyright © 2009 CNRS
 * Copyright © 2009-2010 inria.  All rights reserved.
 * Copyright © 2009 Université Bordeaux 1
 * Copyright © 2009-2011 Cisco Systems, Inc.  All rights reserved.
 * See COPYING in top-level directory.
 */

#include <private/autogen/config.h>

#include <hwloc.h>
#include <string.h>

#include "lstopo.h"

void output_xml(hwloc_topology_t topology, const char *filename, int logical __hwloc_attribute_unused, int legend __hwloc_attribute_unused, int verbose_mode __hwloc_attribute_unused)
{
  if (!filename || !strcasecmp(filename, "-.xml"))
    filename = "-";

  if (hwloc_topology_export_xml(topology, filename) < 0) {
    fprintf(stderr, "Failed to export XML to %s (%s)\n", filename, strerror(errno));
    return;
  }
}
