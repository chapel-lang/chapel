/*
 * Copyright © 2012-2019 Inria.  All rights reserved.
 * See COPYING in top-level directory.
 */

#include "private/autogen/config.h"
#include "hwloc.h"
#include "private/private.h"

#include <stdlib.h>

static int
hwloc_look_fake(struct hwloc_backend *backend, struct hwloc_disc_status *dstatus)
{
  hwloc_topology_t topology = backend->topology;

  assert(dstatus->phase == HWLOC_DISC_PHASE_TWEAK);

  if (getenv("HWLOC_DEBUG_FAKE_COMPONENT_TWEAK")) {
    hwloc_obj_t obj;
    int err;
    /* restrict to single (last) PU */
    obj = hwloc_get_obj_by_type(topology, HWLOC_OBJ_PU, hwloc_get_nbobjs_by_type(topology, HWLOC_OBJ_PU)-1);
    assert(obj);
    err = hwloc_topology_restrict(topology, obj->cpuset, 0);
    assert(!err);
    /* restrict to single (first) NUMA node */
    obj = hwloc_get_obj_by_type(topology, HWLOC_OBJ_NUMANODE, 0);
    assert(obj);
    err = hwloc_topology_restrict(topology, obj->nodeset, HWLOC_RESTRICT_FLAG_BYNODESET);
    assert(!err);
  }

  return 0;
}

static struct hwloc_backend *
hwloc_fake_component_instantiate(struct hwloc_topology *topology __hwloc_attribute_unused,
				 struct hwloc_disc_component *component __hwloc_attribute_unused,
				 unsigned excluded_phases __hwloc_attribute_unused,
				 const void *_data1 __hwloc_attribute_unused,
				 const void *_data2 __hwloc_attribute_unused,
				 const void *_data3 __hwloc_attribute_unused)
{
  struct hwloc_backend *backend;

  backend = hwloc_backend_alloc(topology, component);
  if (!backend)
    goto out;
  backend->discover = hwloc_look_fake;

  if (getenv("HWLOC_DEBUG_FAKE_COMPONENT"))
    printf("fake component instantiated\n");

  return backend;

 out:
  return NULL;
}

static struct hwloc_disc_component hwloc_fake_disc_component = {
  "fake",
  HWLOC_DISC_PHASE_TWEAK,
  0, /* nothing to exclude */
  hwloc_fake_component_instantiate,
  100, /* make sure it's loaded before anything conflicting excludes it */
  1,
  NULL
};

static int
hwloc_fake_component_init(unsigned long flags)
{
  if (flags)
    return -1;
  if (hwloc_plugin_check_namespace("fake", "hwloc_backend_alloc") < 0)
    return -1;
  if (getenv("HWLOC_DEBUG_FAKE_COMPONENT"))
    printf("fake component initialized\n");
  return 0;
}

static void
hwloc_fake_component_finalize(unsigned long flags)
{
  if (flags)
    return;
  if (getenv("HWLOC_DEBUG_FAKE_COMPONENT"))
    printf("fake component finalized\n");
}

HWLOC_DECLSPEC extern const struct hwloc_component hwloc_fake_component; /* never linked statically in the core */

const struct hwloc_component hwloc_fake_component = {
  HWLOC_COMPONENT_ABI,
  hwloc_fake_component_init, hwloc_fake_component_finalize,
  HWLOC_COMPONENT_TYPE_DISC,
  0,
  &hwloc_fake_disc_component
};
