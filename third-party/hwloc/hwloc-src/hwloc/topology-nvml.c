/*
 * Copyright © 2012-2022 Inria.  All rights reserved.
 * See COPYING in top-level directory.
 */

#include "private/autogen/config.h"
#include "hwloc.h"
#include "hwloc/plugins.h"

/* private headers allowed for convenience because this plugin is built within hwloc */
#include "private/misc.h"
#include "private/debug.h"

#include <nvml.h>


#ifdef NVML_NVLINK_MAX_LINKS

static unsigned
hwloc__nvml_get_peer_gpu_by_pci(nvmlPciInfo_t peer, unsigned nb, nvmlPciInfo_t *gpus)
{
  unsigned i;
  for(i=0; i<nb; i++)
    if (gpus[i].domain == peer.domain && gpus[i].bus == peer.bus && gpus[i].device == peer.device)
      return i;
  return (unsigned)-1;
}

static hwloc_obj_t
hwloc__nvml_get_peer_obj_by_pci(struct hwloc_topology *topology, hwloc_obj_t gpu, nvmlPciInfo_t peer_bdf)
{
  hwloc_obj_t obj;

  /* we want the exact object here because we'll use it PCI class below
   * (we can't use hwloc_pci_find_parent_by_busid() which is enough for inserting OSdev by locality).
   */
  obj = hwloc_pci_find_by_busid(topology, peer_bdf.domain, peer_bdf.bus, peer_bdf.device, 0);
  if (!obj) {
    enum hwloc_type_filter_e pfilter;
    hwloc_topology_get_type_filter(topology, HWLOC_OBJ_PCI_DEVICE, &pfilter);
    /* we need PCI devices to be filtered-in */
    if (pfilter != HWLOC_TYPE_FILTER_KEEP_NONE) {
      static int warned = 0;
      if (!warned && HWLOC_SHOW_ALL_ERRORS())
        fprintf(stderr, "hwloc failed to find NVLink peer %04x:%02x:%02x\n",
                peer_bdf.domain, peer_bdf.bus, peer_bdf.device);
      warned = 1;
    } else {
      static int warned = 0;
      if (!warned)
        hwloc_debug("hwloc failed to find NVLink peer %04x:%02x:%02x because PCI devices are filtered-out\n",
                    peer_bdf.domain, peer_bdf.bus, peer_bdf.device);
      warned = 1;
    }
    return NULL;
  }

  /* We want a non-PCI bridge.
   * On POWER8/9, it's class 0680 vendor 1014 (IBM) model 04ea prog-if 00.
   * For NVSwitch, it's class 0680 with prog-if 01 vendor 10de (NVIDIA).
   * Baseclass 0x06 is enough to avoid GPUs (baseclass 0x03),
   * and that's needed because some GPUs may be hidden from us because of cgroups.
   */
  if (obj->type != HWLOC_OBJ_PCI_DEVICE || (obj->attr->pcidev.class_id >> 8 != 0x06))
    return NULL;

  switch (obj->attr->pcidev.vendor_id) {
  case 0x1014: {
    /* IBM OpenCAPI port, return the CPU object. */
    if (!getenv("HWLOC_NVML_USE_OPENCAPI_LOCALITY")) {
      /* OpenCAPI Bridge PCI locality is wrong on POWER8 (equal to the entire machine).
       * Both POWER8 and POWER9 have correct GPU locality, use that one instead.
       * This will only break if PCI and NVLink are not connected to the same location, unlikely.
       */
      obj = gpu;
    }
    /* return a CPU side parent */
    while (!obj->cpuset)
      obj = obj->parent;
    return obj;
  }
  case 0x10de: {
    /* NVIDIA NVSwitch, return the PCI object, we don't have anything better.
     * Mark it as subtype NVSwitch so that the core doesn't remove it.
     */
    if (!obj->subtype)
      obj->subtype = strdup("NVSwitch");
    return obj;
  }
  default: {
    static int warned = 0;
    if (!warned && HWLOC_SHOW_ALL_ERRORS())
      fprintf(stderr, "hwloc failed to recognize NVLink peer %04x:%02x:%02x class %04x vendor %04x device %04x\n",
              peer_bdf.domain, peer_bdf.bus, peer_bdf.device,
              obj->attr->pcidev.class_id, obj->attr->pcidev.vendor_id, obj->attr->pcidev.device_id);
    warned = 1;
    return NULL;
  }
  }
}

static unsigned
hwloc__nvml_store_peer_obj(hwloc_obj_t obj,
                           unsigned nbgpus, unsigned *nbobjs, hwloc_obj_t *objs)
{
  unsigned i;
  /* is it already in the array? */
  for(i=nbgpus; i<*nbobjs; i++)
    if (objs[i] == obj)
      return i;
  /* append it */
  objs[*nbobjs] = obj;
  return (*nbobjs)++;
}

static int
hwloc__nvml_add_nvlink_bandwidth(hwloc_topology_t topology,
                                 unsigned nbobjs, hwloc_obj_t *objs, hwloc_uint64_t *bws)
{
  void *handle;
  int err;

  handle = hwloc_backend_distances_add_create(topology, "NVLinkBandwidth",
                                              HWLOC_DISTANCES_KIND_FROM_OS|HWLOC_DISTANCES_KIND_MEANS_BANDWIDTH,
                                              0);
  if (!handle)
    goto out;

  err = hwloc_backend_distances_add_values(topology, handle, nbobjs, objs, bws, 0);
  if (err < 0)
    goto out;
  /* arrays are now attached to the handle */
  objs = NULL;
  bws = NULL;

  err = hwloc_backend_distances_add_commit(topology, handle, 0 /* don't group GPUs */);
  if (err < 0)
    goto out;

  return 0;

 out:
  free(objs);
  free(bws);
  return -1;
}
#endif /* NVML_NVLINK_MAX_LINKS */


static int
hwloc_nvml_discover(struct hwloc_backend *backend, struct hwloc_disc_status *dstatus)
{
  /*
   * This backend uses the underlying OS.
   * However we don't enforce topology->is_thissystem so that
   * we may still force use this backend when debugging with !thissystem.
   */

  struct hwloc_topology *topology = backend->topology;
  enum hwloc_type_filter_e filter;
  nvmlReturn_t ret;
  unsigned nb, i;
#ifdef NVML_NVLINK_MAX_LINKS
  unsigned nbobjs, j;
  hwloc_obj_t *objs;
  unsigned *peer_indexes;
  nvmlPciInfo_t *gpu_bdfs;
  hwloc_uint64_t *bws;
  int found_nvlinks = 0;
#endif

  assert(dstatus->phase == HWLOC_DISC_PHASE_IO);

  hwloc_topology_get_type_filter(topology, HWLOC_OBJ_OS_DEVICE, &filter);
  if (filter == HWLOC_TYPE_FILTER_KEEP_NONE)
    return 0;

  ret = nvmlInit();
  if (NVML_SUCCESS != ret) {
    if (HWLOC_SHOW_ALL_ERRORS()) {
      const char *error = nvmlErrorString(ret);
      fprintf(stderr, "hwloc/nvml: Failed to initialize with nvmlInit(): %s\n", error);
    }
    return -1;
  }
  ret = nvmlDeviceGetCount(&nb);
  if (NVML_SUCCESS != ret || !nb) {
    nvmlShutdown();
    return 0;
  }

#ifdef NVML_NVLINK_MAX_LINKS
  /* the PCI BDF of each GPU */
  gpu_bdfs = calloc(nb, sizeof(*gpu_bdfs));
  /* the nvlink matrix will require one slot per GPU and possibly additional slots for non-GPU endpoints,
   * usually one per CPU, but let's take an easy upper bound.
   */
  objs = calloc(nb * NVML_NVLINK_MAX_LINKS, sizeof(*objs));
  bws = calloc(nb * NVML_NVLINK_MAX_LINKS * nb * NVML_NVLINK_MAX_LINKS, sizeof(*bws));
  /* array to translate peer of i-th link of j-th GPU into an peer object index inside objs */
  peer_indexes = calloc(nb * NVML_NVLINK_MAX_LINKS, sizeof(*peer_indexes));
  if (!gpu_bdfs || !objs || !gpu_bdfs || !bws || !peer_indexes) {
    free(gpu_bdfs);
    free(objs);
    free(bws);
    free(peer_indexes);
    return -1;
  }
#endif

  for(i=0; i<nb; i++) {
    nvmlPciInfo_t pci;
    nvmlDevice_t device;
    hwloc_obj_t osdev, parent;
    char buffer[64];

    ret = nvmlDeviceGetHandleByIndex(i, &device);
    assert(ret == NVML_SUCCESS);

    osdev = hwloc_alloc_setup_object(topology, HWLOC_OBJ_OS_DEVICE, HWLOC_UNKNOWN_INDEX);
    snprintf(buffer, sizeof(buffer), "nvml%u", i);
    osdev->name = strdup(buffer);
    osdev->subtype = strdup("NVML");
    osdev->depth = HWLOC_TYPE_DEPTH_UNKNOWN;
    osdev->attr->osdev.type = HWLOC_OBJ_OSDEV_GPU;

    hwloc_obj_add_info(osdev, "Backend", "NVML");
    hwloc_obj_add_info(osdev, "GPUVendor", "NVIDIA Corporation");

    buffer[0] = '\0';
    ret = nvmlDeviceGetName(device, buffer, sizeof(buffer));
    hwloc_obj_add_info(osdev, "GPUModel", buffer);

    /* these may fail with NVML_ERROR_NOT_SUPPORTED on old devices */
    buffer[0] = '\0';
    ret = nvmlDeviceGetSerial(device, buffer, sizeof(buffer));
    if (buffer[0] != '\0')
      hwloc_obj_add_info(osdev, "NVIDIASerial", buffer);

    buffer[0] = '\0';
    ret = nvmlDeviceGetUUID(device, buffer, sizeof(buffer));
    if (buffer[0] != '\0')
      hwloc_obj_add_info(osdev, "NVIDIAUUID", buffer);

    parent = NULL;
    if (NVML_SUCCESS == nvmlDeviceGetPciInfo(device, &pci)) {
#ifdef NVML_NVLINK_MAX_LINKS
      gpu_bdfs[i] = pci;
#endif
      parent = hwloc_pci_find_parent_by_busid(topology, pci.domain, pci.bus, pci.device, 0);
#if HAVE_DECL_NVMLDEVICEGETMAXPCIELINKGENERATION
      if (parent && parent->type == HWLOC_OBJ_PCI_DEVICE) {
	unsigned maxwidth = 0, maxgen = 0;
	float lanespeed;
	nvmlDeviceGetMaxPcieLinkWidth(device, &maxwidth);
	nvmlDeviceGetMaxPcieLinkGeneration(device, &maxgen);
	/* PCIe Gen1 = 2.5GT/s signal-rate per lane with 8/10 encoding    = 0.25GB/s data-rate per lane
	 * PCIe Gen2 = 5  GT/s signal-rate per lane with 8/10 encoding    = 0.5 GB/s data-rate per lane
	 * PCIe Gen3 = 8  GT/s signal-rate per lane with 128/130 encoding = 1   GB/s data-rate per lane
	 */
	lanespeed = maxgen <= 2 ? 2.5 * maxgen * 0.8 : 8.0 * 128/130; /* Gbit/s per lane */
	if (lanespeed * maxwidth != 0.)
	  /* we found the max link speed, replace the current link speed found by pci (or none) */
	  parent->attr->pcidev.linkspeed = lanespeed * maxwidth / 8; /* GB/s */
      }
#endif
    }
    if (!parent)
      parent = hwloc_get_root_obj(topology);

    hwloc_insert_object_by_parent(topology, parent, osdev);
#ifdef NVML_NVLINK_MAX_LINKS
    objs[i] = osdev;
#endif
  }

#ifdef NVML_NVLINK_MAX_LINKS
  nbobjs = nb;

  /* list peer objects */
  for(i=0; i<nb; i++) {
    /* look at nvlinks */
    nvmlDevice_t device;
    nvmlPciInfo_t pci;

    ret = nvmlDeviceGetHandleByIndex(i, &device);
    assert(ret == NVML_SUCCESS);

    hwloc_debug("looking at NVLinks for NVML GPU #%u...\n", i);
    for(j=0; j<NVML_NVLINK_MAX_LINKS; j++) {
      nvmlEnableState_t isActive;

      /* mark the peer as unknown for now */
      peer_indexes[i*NVML_NVLINK_MAX_LINKS+j] = (unsigned) -1;

      ret = nvmlDeviceGetNvLinkState(device, j, &isActive);
      if (ret != NVML_SUCCESS)
        break;
      if (isActive != NVML_FEATURE_ENABLED)
        continue;
      found_nvlinks++;
      hwloc_debug("  NVLink #%u is active\n", j);

      ret = nvmlDeviceGetNvLinkRemotePciInfo(device, j, &pci);
      if (ret == NVML_SUCCESS) {
        unsigned peer_index;
        hwloc_debug("    goes to PCI %04x:%02x:%02x\n", pci.domain, pci.bus, pci.device);
        peer_index = hwloc__nvml_get_peer_gpu_by_pci(pci, nb, gpu_bdfs);
        if (peer_index == (unsigned)-1) {
          hwloc_obj_t peer_obj = hwloc__nvml_get_peer_obj_by_pci(topology, objs[i], pci);
          if (!peer_obj)
            continue;

          peer_index = hwloc__nvml_store_peer_obj(peer_obj, nb, &nbobjs, objs);
          hwloc_debug("    adding NVML peer index #%u\n", peer_index);
        } else {
          hwloc_debug("    reusing NVML peer index #%u\n", peer_index);
        }
        peer_indexes[i*NVML_NVLINK_MAX_LINKS+j] = peer_index;
      }
    }
  }
  hwloc_debug("NVML found %u GPUs within %u peers total, with %u nvlinks total\n", nb, nbobjs, found_nvlinks);

  if (hwloc_topology_get_flags(topology) & HWLOC_TOPOLOGY_FLAG_NO_DISTANCES)
    found_nvlinks = 0;

  if (found_nvlinks) {
    /* now build the matrix */
    found_nvlinks = 0; /* reset back in case the version is unknown below and the matrix remains empty */
    for(i=0; i<nb; i++) {
      nvmlDevice_t device;
      ret = nvmlDeviceGetHandleByIndex(i, &device);
      assert(ret == NVML_SUCCESS);

      for(j=0; j<NVML_NVLINK_MAX_LINKS; j++) {
        unsigned version;
        hwloc_uint64_t bw;
        unsigned peer_index = peer_indexes[i*NVML_NVLINK_MAX_LINKS+j];

        if (peer_index == (unsigned)-1)
          continue;

        /* For GPU-to-GPU link, we'll get info for both direction, while GPU-to-CPU info is unique.
         * Only store once on both sides of diagonal.
         */
        if (peer_index < i)
          continue;

        ret = nvmlDeviceGetNvLinkVersion(device, j, &version);
        if (ret != NVML_SUCCESS)
          continue;

        hwloc_debug("GPU #%u NVLink #%u has version %u\n", i, j, version);
        /* version1 = 20GB/s, version2=25GB/s */
        if (version == 1) {
          bw = 20000; /* multiple links may connect same GPUs */
        } else if (version == 2) {
          bw = 25000; /* multiple links may connect same GPUs */
        } else if (version == 3 || version == 4) {
          bw = 50000; /* multiple links may connect same GPUs */
        } else {
          static int warned = 0;
          if (!warned && HWLOC_SHOW_ALL_ERRORS())
            fprintf(stderr, "Failed to recognize NVLink version %u\n", version);
          warned = 1;
          continue;
        }

        bws[i*nbobjs+peer_index] += bw;
        bws[peer_index*nbobjs+i] += bw;
        found_nvlinks++;
      }
    }
    if (found_nvlinks) {
      /* add very high artifical values on the diagonal since local is faster than remote.
       * there are 6 link per GPU max for now, 150GB/s, use 1TB/s for local, it somehow matches the HBM.
       */
      for(i=0; i<nbobjs; i++)
        bws[i*nbobjs+i] = 1000000;

      hwloc__nvml_add_nvlink_bandwidth(topology, nbobjs, objs, bws);
      /* matrices don't need to be freed anymore */
      objs = NULL;
      bws = NULL;
    }
  }
  free(objs);
  free(bws);
  free(gpu_bdfs);
  free(peer_indexes);
#endif /* NVML_NVLINK_MAX_LINKS */

  nvmlShutdown();
  return 0;
}

static struct hwloc_backend *
hwloc_nvml_component_instantiate(struct hwloc_topology *topology,
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
  backend->discover = hwloc_nvml_discover;
  return backend;
}

static struct hwloc_disc_component hwloc_nvml_disc_component = {
  "nvml",
  HWLOC_DISC_PHASE_IO,
  HWLOC_DISC_PHASE_GLOBAL,
  hwloc_nvml_component_instantiate,
  5, /* after pci, and after cuda since likely less useful */
  1,
  NULL
};

static int
hwloc_nvml_component_init(unsigned long flags)
{
  if (flags)
    return -1;
  if (hwloc_plugin_check_namespace("nvml", "hwloc_backend_alloc") < 0)
    return -1;
  return 0;
}

#ifdef HWLOC_INSIDE_PLUGIN
HWLOC_DECLSPEC extern const struct hwloc_component hwloc_nvml_component;
#endif

const struct hwloc_component hwloc_nvml_component = {
  HWLOC_COMPONENT_ABI,
  hwloc_nvml_component_init, NULL,
  HWLOC_COMPONENT_TYPE_DISC,
  0,
  &hwloc_nvml_disc_component
};
