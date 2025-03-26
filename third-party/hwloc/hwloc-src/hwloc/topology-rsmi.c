/*
 * Copyright © 2012-2022 Inria.  All rights reserved.
 * Copyright (c) 2020, Advanced Micro Devices, Inc. All rights reserved.
 * Written by Advanced Micro Devices,
 * See COPYING in top-level directory.
 */

#include "private/autogen/config.h"
#include "hwloc.h"
#include "hwloc/plugins.h"

/* private headers allowed for convenience because this plugin is built within hwloc */
#include "private/misc.h"
#include "private/debug.h"

#include <rocm_smi/rocm_smi.h>

static int
hwloc__rsmi_add_xgmi_bandwidth(hwloc_topology_t topology,
                               unsigned nbobjs, hwloc_obj_t *objs, hwloc_uint64_t *bws)
{
  void *handle;
  int err;

  handle = hwloc_backend_distances_add_create(topology, "XGMIBandwidth",
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

static int
hwloc__rsmi_add_xgmi_hops(hwloc_topology_t topology,
                          unsigned nbobjs, hwloc_obj_t *objs, hwloc_uint64_t *hops)
{
  void *handle;
  int err;

  handle = hwloc_backend_distances_add_create(topology, "XGMIHops",
                                              HWLOC_DISTANCES_KIND_FROM_OS|HWLOC_DISTANCES_KIND_MEANS_LATENCY,
                                              0);
  if (!handle)
    goto out;

  err = hwloc_backend_distances_add_values(topology, handle, nbobjs, objs, hops, 0);
  if (err < 0)
    goto out;
  /* arrays are now attached to the handle */
  objs = NULL;
  hops = NULL;

  err = hwloc_backend_distances_add_commit(topology, handle, 0 /* don't group GPUs */);
  if (err < 0)
    goto out;

  return 0;

 out:
  free(objs);
  free(hops);
  return -1;
}

/*
 * Get the name of the GPU
 *
 * dv_ind		(IN) The device index
 * device_name	(OUT) Name of GPU devices
 * size			(OUT) Size of the name
 */
static int get_device_name(uint32_t dv_ind, char *device_name, unsigned int size)
{
  rsmi_status_t rsmi_rc = rsmi_dev_name_get(dv_ind, device_name, size);

  if (rsmi_rc != RSMI_STATUS_SUCCESS) {
    if (HWLOC_SHOW_ALL_ERRORS()) {
      const char *status_string;
      rsmi_rc = rsmi_status_string(rsmi_rc, &status_string);
      fprintf(stderr, "hwloc/rsmi: GPU(%u): Failed to get name: %s\n", (unsigned)dv_ind, status_string);
    }
    return -1;
  }
  return 0;
}

/*
 * Get the PCI Info of the GPU
 *
 * dv_ind  (IN) The device index
 * bdfid   (OUT) PCI Info of GPU devices
 */
static int get_device_pci_info(uint32_t dv_ind, uint64_t *bdfid)
{
  rsmi_status_t rsmi_rc = rsmi_dev_pci_id_get(dv_ind, bdfid);

  if (rsmi_rc != RSMI_STATUS_SUCCESS) {
    if (HWLOC_SHOW_ALL_ERRORS()) {
      const char *status_string;
      rsmi_rc = rsmi_status_string(rsmi_rc, &status_string);
      fprintf(stderr, "hwloc/rsmi: GPU(%u): Failed to get PCI Info: %s\n", (unsigned)dv_ind, status_string);
    }
    return -1;
  }
  return 0;
}

/*
 * Get the PCI link speed of the GPU
 *
 * dv_ind    (IN) The device index
 * linkspeed (OUT) PCI link speed of GPU devices
 */
static int get_device_pci_linkspeed(uint32_t dv_ind, float *linkspeed)
{
  rsmi_pcie_bandwidth_t bandwidth;
  uint64_t lanespeed_raw; // T/s
  uint64_t lanespeed; // (bits/s)
  uint32_t lanes;
  rsmi_status_t rsmi_rc = rsmi_dev_pci_bandwidth_get(dv_ind, &bandwidth);

  if (rsmi_rc != RSMI_STATUS_SUCCESS) {
    return -1;
  }
  lanespeed_raw = bandwidth.transfer_rate.frequency[bandwidth.transfer_rate.current]; // T/s
  lanespeed = lanespeed_raw <= 5000000000 ? (lanespeed_raw * 8)/10 : (lanespeed_raw * 128)/130; // bits/s
  lanes = bandwidth.lanes[bandwidth.transfer_rate.current];
  *linkspeed = (lanespeed * lanes) / 8000000000; // GB/s
  return 0;
}

/*
 * Get the Unique ID of the GPU
 *
 * dv_ind  (IN) The device index
 * buffer  (OUT) Unique ID of GPU devices
 */
static int get_device_unique_id(uint32_t dv_ind, char *buffer)
{
  uint64_t id;
  rsmi_status_t rsmi_rc = rsmi_dev_unique_id_get(dv_ind, &id);

  if (rsmi_rc != RSMI_STATUS_SUCCESS) {
    return -1;
  }
  sprintf(buffer, "%llx", (unsigned long long) id);
  return 0;
}

/*
 * Get the serial number of the GPU
 *
 * dv_ind  (IN) The device index
 * serial  (OUT) Serial number of GPU devices
 * size    (IN) Length of the caller provided buffer
 */
static int get_device_serial_number(uint32_t dv_ind, char *serial, unsigned int size)
{
  rsmi_status_t rsmi_rc = rsmi_dev_serial_number_get(dv_ind, serial, size);

  if (rsmi_rc != RSMI_STATUS_SUCCESS) {
    return -1;
  }
  return 0;
}

/*
 * Get the XGMI hive id of the GPU
 *
 * dv_ind  (IN) The device index
 * hive_id (OUT) The XGMI hive id of GPU devices
 */
static int get_device_xgmi_hive_id(uint32_t dv_ind, char *buffer)
{
  uint64_t hive_id;
  rsmi_status_t rsmi_rc = rsmi_dev_xgmi_hive_id_get(dv_ind, &hive_id);

  if (rsmi_rc != RSMI_STATUS_SUCCESS) {
    if (HWLOC_SHOW_ALL_ERRORS()) {
      const char *status_string;
      rsmi_rc = rsmi_status_string(rsmi_rc, &status_string);
      fprintf(stderr, "hwloc/rsmi: GPU(%u): Failed to get hive id: %s\n", (unsigned)dv_ind, status_string);
    }
    return -1;
  }
  sprintf(buffer, "%llx", (unsigned long long) hive_id);
  return 0;
}

/*
 * Get the IO Link type of the GPU
 *
 * dv_ind_src  (IN)  The source device index
 * dv_ind_dst  (IN)  The destination device index
 * type        (OUT) The type of IO Link
 */
static int get_device_io_link_type(uint32_t dv_ind_src, uint32_t dv_ind_dst,
                                   RSMI_IO_LINK_TYPE *type, uint64_t *hops)
{
  rsmi_status_t rsmi_rc = rsmi_topo_get_link_type(dv_ind_src, dv_ind_dst,
                                                  hops, type);

  if (rsmi_rc != RSMI_STATUS_SUCCESS) {
    if (HWLOC_SHOW_ALL_ERRORS()) {
      const char *status_string;
      rsmi_rc = rsmi_status_string(rsmi_rc, &status_string);
      fprintf(stderr, "hwloc/rsmi: GPU(%u): Failed to get link type: %s\n", (unsigned)dv_ind_src, status_string);
    }
    return -1;
  }
  return 0;
}

static int
hwloc_rsmi_discover(struct hwloc_backend *backend, struct hwloc_disc_status *dstatus)
{
  /*
   * This backend uses the underlying OS.
   * However we don't enforce topology->is_thissystem so that
   * we may still force use this backend when debugging with !thissystem.
   */

  struct hwloc_topology *topology = backend->topology;
  enum hwloc_type_filter_e filter;
  hwloc_obj_t *osdevs = NULL, *osdevs2 = NULL;
  hwloc_uint64_t *xgmi_bws = NULL, *xgmi_hops = NULL;
  uint64_t memory;
  int got_xgmi_bws = 0;
  rsmi_version_t version;
  rsmi_status_t ret;
  int may_shutdown;
  unsigned nb, i, j;

  assert(dstatus->phase == HWLOC_DISC_PHASE_IO);

  hwloc_topology_get_type_filter(topology, HWLOC_OBJ_OS_DEVICE, &filter);
  if (filter == HWLOC_TYPE_FILTER_KEEP_NONE)
    return 0;

  ret = rsmi_init(0);
  if (RSMI_STATUS_SUCCESS != ret) {
    if (HWLOC_SHOW_ALL_ERRORS()) {
      const char *status_string;
      rsmi_status_string(ret, &status_string);
      fprintf(stderr, "hwloc/rsmi: Failed to initialize with rsmi_init(): %s\n", status_string);
    }
    return 0;
  }

  rsmi_version_get(&version);

  ret = rsmi_num_monitor_devices(&nb);
  if (RSMI_STATUS_SUCCESS != ret || !nb) {
    if (RSMI_STATUS_SUCCESS != ret && HWLOC_SHOW_ALL_ERRORS()) {
      const char *status_string;
      rsmi_status_string(ret, &status_string);
      fprintf(stderr, "hwloc/rsmi: Failed to get number of devices with rsmi_num_monitor_devices(): %s\n", status_string);
    }
    goto out;
  }

  osdevs = malloc(nb *sizeof(*osdevs));
  xgmi_bws = calloc(nb*nb, sizeof(*xgmi_bws));
  osdevs2 = malloc(nb *sizeof(*osdevs2));
  xgmi_hops = calloc(nb*nb, sizeof(*xgmi_hops));
  if (!osdevs || !xgmi_bws || !osdevs2 || !xgmi_hops)
    goto out;

  for (i=0; i<nb; i++) {
    uint64_t bdfid = 0;
    hwloc_obj_t osdev, parent;
    char buffer[64];
    char *xgmi_peers, *xgmi_peers_ptr;

    osdev = hwloc_alloc_setup_object(topology, HWLOC_OBJ_OS_DEVICE, HWLOC_UNKNOWN_INDEX);
    snprintf(buffer, sizeof(buffer), "rsmi%u", i);
    osdev->name = strdup(buffer);
    osdev->subtype = strdup("RSMI");
    osdev->depth = HWLOC_TYPE_DEPTH_UNKNOWN;
    osdev->attr->osdev.type = HWLOC_OBJ_OSDEV_GPU;

    hwloc_obj_add_info(osdev, "Backend", "RSMI");
    hwloc_obj_add_info(osdev, "GPUVendor", "AMD");

    buffer[0] = '\0';
    if (get_device_name(i, buffer, sizeof(buffer)) == -1)
      buffer[0] = '\0';
    hwloc_obj_add_info(osdev, "GPUModel", buffer);

    buffer[0] = '\0';
    if ((get_device_serial_number(i, buffer, sizeof(buffer)) == 0) && buffer[0])
      hwloc_obj_add_info(osdev, "AMDSerial", buffer);

    buffer[0] = '\0';
    if (get_device_unique_id(i, buffer) == 0)
      hwloc_obj_add_info(osdev, "AMDUUID", buffer);

    buffer[0] = '\0';
    if (get_device_xgmi_hive_id(i, buffer) == 0)
      hwloc_obj_add_info(osdev, "XGMIHiveID", buffer);

    ret = rsmi_dev_memory_total_get(i, RSMI_MEM_TYPE_VRAM, &memory);
    if (ret == RSMI_STATUS_SUCCESS) {
      char tmp[64];
      snprintf(tmp, sizeof(tmp), "%llu", (unsigned long long)memory/1024);
      hwloc_obj_add_info(osdev, "RSMIVRAMSize", tmp);
    }
    ret = rsmi_dev_memory_total_get(i, RSMI_MEM_TYPE_VIS_VRAM, &memory);
    if (ret == RSMI_STATUS_SUCCESS) {
      char tmp[64];
      snprintf(tmp, sizeof(tmp), "%llu", (unsigned long long)memory/1024);
      hwloc_obj_add_info(osdev, "RSMIVisibleVRAMSize", tmp);
    }
    ret = rsmi_dev_memory_total_get(i, RSMI_MEM_TYPE_GTT, &memory);
    if (ret == RSMI_STATUS_SUCCESS) {
      char tmp[64];
      snprintf(tmp, sizeof(tmp), "%llu", (unsigned long long)memory/1024);
      hwloc_obj_add_info(osdev, "RSMIGTTSize", tmp);
    }
    /* there's also rsmi_dev_memory_usage_get() to get what's currently used in these memories */

    xgmi_peers = malloc(nb*15+1);  /* "rsmi" + unsigned int + space = 15 chars max, + ending \0 */
    if (xgmi_peers) {
      xgmi_peers[0] = '\0';
      xgmi_peers_ptr = xgmi_peers;
      for (j=0; j<nb; j++) {
        RSMI_IO_LINK_TYPE type;
        uint64_t hops;
        if (i == j)
          continue;
        if ((get_device_io_link_type(i, j, &type, &hops) == 0) &&
            (type == RSMI_IOLINK_TYPE_XGMI)) {
          xgmi_peers_ptr += sprintf(xgmi_peers_ptr, "rsmi%u ", j);
          xgmi_bws[i*nb+j] = 100000; /* TODO: verify the XGMI version before putting 100GB/s here? */
          xgmi_hops[i*nb+j] = hops;
          got_xgmi_bws = 1;
        }
      }
      if (xgmi_peers[0] != '\0')
        hwloc_obj_add_info(osdev, "XGMIPeers", xgmi_peers);
      free(xgmi_peers);
    }

    parent = NULL;
    if (get_device_pci_info(i, &bdfid) == 0) {
      unsigned domain, device, bus, func;
      domain = (bdfid>>32) & 0xffffffff;
      bus = ((bdfid & 0xffff)>>8) & 0xff;
      device = ((bdfid & 0xff)>>3) & 0x1f;
      func = bdfid & 0x7;
      parent = hwloc_pci_find_parent_by_busid(topology, domain, bus, device, func);
      if (parent && parent->type == HWLOC_OBJ_PCI_DEVICE)
        get_device_pci_linkspeed(i, &parent->attr->pcidev.linkspeed);
      if (!parent)
        parent = hwloc_get_root_obj(topology);
    }

    hwloc_insert_object_by_parent(topology, parent, osdev);
    osdevs[i] = osdevs2[i] = osdev;
  }

  if (hwloc_topology_get_flags(topology) & HWLOC_TOPOLOGY_FLAG_NO_DISTANCES)
    got_xgmi_bws = 0;

  if (got_xgmi_bws) {
    /* add very high artifical values on the diagonal since local is faster than remote.
     * XGMI links are 100GB/s, so use 1TB/s for local, it somehow matches the HBM.
     */
    for(i=0; i<nb; i++)
      xgmi_bws[i*nb+i] = 1000000;

    hwloc__rsmi_add_xgmi_bandwidth(topology, nb, osdevs, xgmi_bws);

    hwloc__rsmi_add_xgmi_hops(topology, nb, osdevs2, xgmi_hops);

    /* don't free arrays, they were given to the distance internals */
    osdevs = NULL;
    xgmi_bws = NULL;
    osdevs2 = NULL;
    xgmi_hops = NULL;
  }

 out:
  may_shutdown = 0;
  if (version.major > 3 || (version.major == 3 && version.minor > 3)) {
    may_shutdown = 1;
  } else {
    /* old RSMI libs didn't have refcounting, we don't want to shutdown what the app may be using.
     * don't shutdown unless required for valgrind testing etc.
     */
    char *env = getenv("HWLOC_RSMI_SHUTDOWN");
    if (env)
      may_shutdown = 1;
  }
  if (may_shutdown)
    rsmi_shut_down();

  free(osdevs);
  free(xgmi_bws);
  free(osdevs2);
  free(xgmi_hops);
  return 0;
}

static struct hwloc_backend *
hwloc_rsmi_component_instantiate(struct hwloc_topology *topology,
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
  backend->discover = hwloc_rsmi_discover;
  return backend;
}

static struct hwloc_disc_component hwloc_rsmi_disc_component = {
  "rsmi",
  HWLOC_DISC_PHASE_IO,
  HWLOC_DISC_PHASE_GLOBAL,
  hwloc_rsmi_component_instantiate,
  10, /* after pci */
  1,
  NULL
};

static int
hwloc_rsmi_component_init(unsigned long flags)
{
  if (flags)
    return -1;
  if (hwloc_plugin_check_namespace("rsmi", "hwloc_backend_alloc") < 0)
    return -1;
  return 0;
}

#ifdef HWLOC_INSIDE_PLUGIN
HWLOC_DECLSPEC extern const struct hwloc_component hwloc_rsmi_component;
#endif

const struct hwloc_component hwloc_rsmi_component = {
  HWLOC_COMPONENT_ABI,
  hwloc_rsmi_component_init, NULL,
  HWLOC_COMPONENT_TYPE_DISC,
  0,
  &hwloc_rsmi_disc_component
};
