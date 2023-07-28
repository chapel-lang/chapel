/*
 * Copyright © 2009 CNRS
 * Copyright © 2009-2022 Inria.  All rights reserved.
 * Copyright © 2009-2011, 2013 Université Bordeaux
 * Copyright © 2014-2018 Cisco Systems, Inc.  All rights reserved.
 * Copyright © 2015      Research Organization for Information Science
 *                       and Technology (RIST). All rights reserved.
 * See COPYING in top-level directory.
 */

#include "private/autogen/config.h"
#include "hwloc.h"
#include "hwloc/helper.h"
#include "hwloc/plugins.h"

/* private headers allowed for convenience because this plugin is built within hwloc */
#include "private/debug.h"
#include "private/misc.h"

#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <assert.h>
#include <stdarg.h>
#ifdef HWLOC_LINUX_SYS
#include <dirent.h>
#endif

#include <pciaccess.h>

#ifndef PCI_HEADER_TYPE
#define PCI_HEADER_TYPE 0x0e
#endif
#ifndef PCI_HEADER_TYPE_BRIDGE
#define PCI_HEADER_TYPE_BRIDGE 1
#endif

#ifndef PCI_CLASS_DEVICE
#define PCI_CLASS_DEVICE 0x0a
#endif
#ifndef PCI_CLASS_BRIDGE_PCI
#define PCI_CLASS_BRIDGE_PCI 0x0604
#endif

#ifndef PCI_REVISION_ID
#define PCI_REVISION_ID 0x08
#endif

#ifndef PCI_SUBSYSTEM_VENDOR_ID
#define PCI_SUBSYSTEM_VENDOR_ID 0x2c
#endif
#ifndef PCI_SUBSYSTEM_ID
#define PCI_SUBSYSTEM_ID 0x2e
#endif

#ifndef PCI_PRIMARY_BUS
#define PCI_PRIMARY_BUS 0x18
#endif
#ifndef PCI_SECONDARY_BUS
#define PCI_SECONDARY_BUS 0x19
#endif
#ifndef PCI_SUBORDINATE_BUS
#define PCI_SUBORDINATE_BUS 0x1a
#endif

#ifndef PCI_CAP_ID_EXP
#define PCI_CAP_ID_EXP 0x10
#endif

#ifndef PCI_CAP_NORMAL
#define PCI_CAP_NORMAL 1
#endif

#define CONFIG_SPACE_CACHESIZE 256

#ifdef HWLOC_WIN_SYS
#error pciaccess locking currently not implemented on Windows

#elif defined HWLOC_HAVE_PTHREAD_MUTEX
/* pthread mutex if available (except on windows) */
#include <pthread.h>
static pthread_mutex_t hwloc_pciaccess_mutex = PTHREAD_MUTEX_INITIALIZER;
#define HWLOC_PCIACCESS_LOCK() pthread_mutex_lock(&hwloc_pciaccess_mutex)
#define HWLOC_PCIACCESS_UNLOCK() pthread_mutex_unlock(&hwloc_pciaccess_mutex)

#else /* HWLOC_WIN_SYS || HWLOC_HAVE_PTHREAD_MUTEX */
#error No mutex implementation available
#endif

static void
hwloc_pci_get_obj_names(hwloc_obj_t obj, struct pci_id_match *m)
{
  const char *vendorname, *devicename;
  m->vendor_id = obj->attr->pcidev.vendor_id;
  m->device_id = obj->attr->pcidev.device_id;
  pci_get_strings(m, &devicename, &vendorname, NULL, NULL);
  if (vendorname && *vendorname)
    hwloc_obj_add_info(obj, "PCIVendor", vendorname);
  if (devicename && *devicename)
    hwloc_obj_add_info(obj, "PCIDevice", devicename);
}

static void
hwloc_pci_get_names(hwloc_topology_t topology)
{
  hwloc_obj_t obj;
  struct pci_id_match m;

  /* we need the lists of PCI and bridges */
  hwloc_topology_reconnect(topology, 0);

  m.subvendor_id = PCI_MATCH_ANY;
  m.subdevice_id = PCI_MATCH_ANY;
  m.device_class = 0;
  m.device_class_mask = 0;
  m.match_data = 0;

  HWLOC_PCIACCESS_LOCK();

  obj = NULL;
  while ((obj = hwloc_get_next_pcidev(topology, obj)) != NULL)
    hwloc_pci_get_obj_names(obj, &m);

  obj = NULL;
  while ((obj = hwloc_get_next_bridge(topology, obj)) != NULL)
    if (obj->attr->bridge.upstream_type == HWLOC_OBJ_BRIDGE_PCI)
      hwloc_pci_get_obj_names(obj, &m);

  HWLOC_PCIACCESS_UNLOCK();
}

static int
hwloc_look_pci(struct hwloc_backend *backend, struct hwloc_disc_status *dstatus)
{
  /*
   * This backend uses the underlying OS.
   * However we don't enforce topology->is_thissystem so that
   * we may still force use this backend when debugging with !thissystem.
   */

  struct hwloc_topology *topology = backend->topology;
  enum hwloc_type_filter_e pfilter, bfilter;
  struct hwloc_obj *tree = NULL;
  int ret;
  struct pci_device_iterator *iter;
  struct pci_device *pcidev;
  struct pci_id_match m;

  m.subvendor_id = PCI_MATCH_ANY;
  m.subdevice_id = PCI_MATCH_ANY;
  m.device_class = 0;
  m.device_class_mask = 0;
  m.match_data = 0;

  hwloc_topology_get_type_filter(topology, HWLOC_OBJ_PCI_DEVICE, &pfilter);
  hwloc_topology_get_type_filter(topology, HWLOC_OBJ_BRIDGE, &bfilter);
  if (bfilter == HWLOC_TYPE_FILTER_KEEP_NONE
      && pfilter == HWLOC_TYPE_FILTER_KEEP_NONE)
    return 0;

  if (dstatus->phase == HWLOC_DISC_PHASE_ANNOTATE) {
    hwloc_pci_get_names(topology);
    return 0;
  }
  assert(dstatus->phase == HWLOC_DISC_PHASE_PCI);

  hwloc_debug("%s", "\nScanning PCI buses...\n");

  /* pciaccess isn't thread-safe. it uses a single global variable that doesn't have
   * refcounting, and is dynamically reallocated when vendor/device names are needed, etc.
   */
  HWLOC_PCIACCESS_LOCK();

  /* initialize PCI scanning */
  ret = pci_system_init();
  if (ret) {
    HWLOC_PCIACCESS_UNLOCK();
    if (HWLOC_SHOW_ALL_ERRORS())
      fprintf(stderr, "hwloc/pci: Failed to initialize libpciaccess with pci_system_init(): %d (%s)\n",
              ret, strerror(errno));
    return -1;
  }

  iter = pci_slot_match_iterator_create(NULL);

  /* iterate over devices */
  for (pcidev = pci_device_next(iter);
       pcidev;
       pcidev = pci_device_next(iter))
  {
    unsigned char config_space_cache[CONFIG_SPACE_CACHESIZE];
    hwloc_obj_type_t type;
    struct hwloc_obj *obj;
    unsigned domain, bus, dev, func;
    unsigned secondary_bus, subordinate_bus;
    unsigned device_class;
    unsigned short tmp16;
    unsigned offset;

    domain = pcidev->domain;
    bus = pcidev->bus;
    dev = pcidev->dev;
    func = pcidev->func;

#ifndef HWLOC_HAVE_32BITS_PCI_DOMAIN
    if (domain > 0xffff) {
      static int warned = 0;
      if (!warned && HWLOC_SHOW_ALL_ERRORS())
	fprintf(stderr, "hwloc/pci: Ignoring PCI device with non-16bit domain.\nPass --enable-32bits-pci-domain to configure to support such devices\n(warning: it would break the library ABI, don't enable unless really needed).\n");
      warned = 1;
      continue;
    }
#endif

    /* initialize the config space in case we fail to read it (missing permissions, etc). */
    memset(config_space_cache, 0xff, CONFIG_SPACE_CACHESIZE);
    pci_device_probe(pcidev);
    pci_device_cfg_read(pcidev, config_space_cache, 0, CONFIG_SPACE_CACHESIZE, NULL);

    /* try to read the device_class */
    device_class = pcidev->device_class >> 8;

    /* bridge or pci dev? */
    type = hwloc_pcidisc_check_bridge_type(device_class, config_space_cache);
    /* only HWLOC_OBJ_BRIDGE for bridges to-PCI */
    if (type == HWLOC_OBJ_BRIDGE) {
      if (hwloc_pcidisc_find_bridge_buses(domain, bus, dev, func,
					  &secondary_bus, &subordinate_bus,
					  config_space_cache) < 0)
	continue;
    }

    /* filtered? */
    if (type == HWLOC_OBJ_PCI_DEVICE) {
      enum hwloc_type_filter_e filter;
      hwloc_topology_get_type_filter(topology, HWLOC_OBJ_PCI_DEVICE, &filter);
      if (filter == HWLOC_TYPE_FILTER_KEEP_NONE)
	continue;
      if (filter == HWLOC_TYPE_FILTER_KEEP_IMPORTANT
	  && !hwloc_filter_check_pcidev_subtype_important(device_class))
	continue;
    } else if (type == HWLOC_OBJ_BRIDGE) {
      enum hwloc_type_filter_e filter;
      hwloc_topology_get_type_filter(topology, HWLOC_OBJ_BRIDGE, &filter);
      if (filter == HWLOC_TYPE_FILTER_KEEP_NONE)
	continue;
      /* HWLOC_TYPE_FILTER_KEEP_IMPORTANT filtered later in the core */
    }

    /* fixup SR-IOV buggy VF device/vendor IDs */
    if (0xffff == pcidev->vendor_id && 0xffff == pcidev->device_id) {
      /* SR-IOV puts ffff:ffff in Virtual Function config space.
       * The actual VF device ID is stored at a special (dynamic) location in the Physical Function config space.
       * VF and PF have the same vendor ID.
       *
       * libpciaccess just returns ffff:ffff, needs to be fixed.
       * linuxpci is OK because sysfs files are already fixed in the kernel.
       * (pciutils is OK when it uses those Linux sysfs files.)
       *
       * Reading these files is an easy way to work around the libpciaccess issue on Linux,
       * but we have no way to know if this is caused by SR-IOV or not.
       *
       * TODO:
       *  If PF has CAP_ID_PCIX or CAP_ID_EXP (offset>0),
       *  look for extended capability PCI_EXT_CAP_ID_SRIOV (need extended config space (more than 256 bytes)),
       *  then read the VF device ID after it (PCI_IOV_DID bytes later).
       *  Needs access to extended config space (needs root on Linux).
       * TODO:
       *  Add string info attributes in VF and PF objects?
       */
#ifdef HWLOC_LINUX_SYS
      /* Workaround for Linux (the kernel returns the VF device/vendor IDs). */
      char path[64];
      char value[16];
      FILE *file;
      size_t bytes_read;

      snprintf(path, sizeof(path), "/sys/bus/pci/devices/%04x:%02x:%02x.%01x/vendor",
	       domain, bus, dev, func);
      file = fopen(path, "r");
      if (file) {
	bytes_read = fread(value, 1, sizeof(value), file);
	fclose(file);
	if (bytes_read)
	  /* fixup the pciaccess struct so that pci_device_get_vendor_name() is correct later. */
          pcidev->vendor_id = strtoul(value, NULL, 16);
      }

      snprintf(path, sizeof(path), "/sys/bus/pci/devices/%04x:%02x:%02x.%01x/device",
	       domain, bus, dev, func);
      file = fopen(path, "r");
      if (file) {
	bytes_read = fread(value, 1, sizeof(value), file);
	fclose(file);
	if (bytes_read)
	  /* fixup the pciaccess struct so that pci_device_get_device_name() is correct later. */
          pcidev->device_id = strtoul(value, NULL, 16);
      }
#endif
    }

    obj = hwloc_alloc_setup_object(topology, type, HWLOC_UNKNOWN_INDEX);
    obj->attr->pcidev.domain = domain;
    obj->attr->pcidev.bus = bus;
    obj->attr->pcidev.dev = dev;
    obj->attr->pcidev.func = func;
    obj->attr->pcidev.vendor_id = pcidev->vendor_id;
    obj->attr->pcidev.device_id = pcidev->device_id;
    obj->attr->pcidev.class_id = device_class;
    obj->attr->pcidev.revision = config_space_cache[PCI_REVISION_ID];

    /* bridge specific attributes */
    if (type == HWLOC_OBJ_BRIDGE) {
      /* assumes this is a Bridge to-PCI */
      struct hwloc_bridge_attr_s *battr = &obj->attr->bridge;
      battr->upstream_type = HWLOC_OBJ_BRIDGE_PCI;
      battr->downstream_type = HWLOC_OBJ_BRIDGE_PCI;
      battr->downstream.pci.domain = domain;
      battr->downstream.pci.secondary_bus = secondary_bus;
      battr->downstream.pci.subordinate_bus = subordinate_bus;
    }

    obj->attr->pcidev.linkspeed = 0; /* unknown */
    offset = hwloc_pcidisc_find_cap(config_space_cache, PCI_CAP_ID_EXP);

    if (offset > 0 && offset + 20 /* size of PCI express block up to link status */ <= CONFIG_SPACE_CACHESIZE) {
      hwloc_pcidisc_find_linkspeed(config_space_cache, offset, &obj->attr->pcidev.linkspeed);
#ifdef HWLOC_LINUX_SYS
    } else {
      /* if not available from config-space (extended part is root-only), look in Linux sysfs files added in 4.13 */
      char path[128];
      char value[16];
      FILE *file;
      size_t bytes_read;
      float speed = 0.f;
      unsigned width = 0;
      snprintf(path, sizeof(path), "/sys/bus/pci/devices/%04x:%02x:%02x.%01x/current_link_speed",
	       domain, bus, dev, func);
      file = fopen(path, "r");
      if (file) {
	bytes_read = fread(value, 1, sizeof(value), file);
	fclose(file);
	if (bytes_read)
	  speed = hwloc_linux_pci_link_speed_from_string(value);
      }
      snprintf(path, sizeof(path), "/sys/bus/pci/devices/%04x:%02x:%02x.%01x/current_link_width",
	       domain, bus, dev, func);
      file = fopen(path, "r");
      if (file) {
	bytes_read = fread(value, 1, sizeof(value), file);
	fclose(file);
	if (bytes_read)
	  width = atoi(value);
      }
      obj->attr->pcidev.linkspeed = speed*width/8;
#endif
    }

    if (obj->type == HWLOC_OBJ_PCI_DEVICE) {
      memcpy(&tmp16, &config_space_cache[PCI_SUBSYSTEM_VENDOR_ID], sizeof(tmp16));
      obj->attr->pcidev.subvendor_id = tmp16;
      memcpy(&tmp16, &config_space_cache[PCI_SUBSYSTEM_ID], sizeof(tmp16));
      obj->attr->pcidev.subdevice_id = tmp16;
    } else {
      /* TODO:
       * bridge must lookup PCI_CAP_ID_SSVID and then look at offset+PCI_SSVID_VENDOR/DEVICE_ID
       * cardbus must look at PCI_CB_SUBSYSTEM_VENDOR_ID and PCI_CB_SUBSYSTEM_ID
       */
    }

    hwloc_debug("  %04x:%02x:%02x.%01x %04x %04x:%04x\n",
		domain, bus, dev, func,
		device_class, pcidev->vendor_id, pcidev->device_id);

    hwloc_pci_get_obj_names(obj, &m);
    hwloc_pcidisc_tree_insert_by_busid(&tree, obj);
  }

  /* finalize device scanning */
  pci_iterator_destroy(iter);
  pci_system_cleanup();
  HWLOC_PCIACCESS_UNLOCK();

  hwloc_pcidisc_tree_attach(topology, tree);

  /* no need to run another PCI phase */
  dstatus->excluded_phases |= HWLOC_DISC_PHASE_PCI;
  /* no need to run the annotate phase, we did it above */
  backend->phases &= HWLOC_DISC_PHASE_ANNOTATE;
  return 0;
}

static struct hwloc_backend *
hwloc_pci_component_instantiate(struct hwloc_topology *topology,
				struct hwloc_disc_component *component,
				unsigned excluded_phases __hwloc_attribute_unused,
				const void *_data1 __hwloc_attribute_unused,
				const void *_data2 __hwloc_attribute_unused,
				const void *_data3 __hwloc_attribute_unused)
{
  struct hwloc_backend *backend;

  backend = hwloc_backend_alloc(topology, component);
  if (!backend)
    return NULL;
  backend->discover = hwloc_look_pci;

#ifdef HWLOC_SOLARIS_SYS
  if ((uid_t)0 != geteuid())
    backend->phases &= ~HWLOC_DISC_PHASE_PCI;
#endif

  return backend;
}

static struct hwloc_disc_component hwloc_pci_disc_component = {
  "pci",
  HWLOC_DISC_PHASE_PCI | HWLOC_DISC_PHASE_ANNOTATE,
  HWLOC_DISC_PHASE_GLOBAL,
  hwloc_pci_component_instantiate,
  20,
  1,
  NULL
};

static int
hwloc_pci_component_init(unsigned long flags)
{
  if (flags)
    return -1;
  if (hwloc_plugin_check_namespace("pci", "hwloc_backend_alloc") < 0)
    return -1;
  return 0;
}

#ifdef HWLOC_INSIDE_PLUGIN
HWLOC_DECLSPEC extern const struct hwloc_component hwloc_pci_component;
#endif

const struct hwloc_component hwloc_pci_component = {
  HWLOC_COMPONENT_ABI,
  hwloc_pci_component_init, NULL,
  HWLOC_COMPONENT_TYPE_DISC,
  0,
  &hwloc_pci_disc_component
};
