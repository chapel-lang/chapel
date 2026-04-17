/*
 * Copyright © 2013-2024 Inria.  All rights reserved.
 * Copyright (c) 2020, Advanced Micro Devices, Inc. All rights reserved.
 * Written by Advanced Micro Devices,
 * See COPYING in top-level directory.
 */

#ifndef HWLOC_PORT_RSMI_RSMI_H
#define HWLOC_PORT_RSMI_RSMI_H

/* we need to replace any RSMI-related #define that configure may have put in private/autogen/config.h */
#ifndef HWLOC_CONFIGURE_H
#error rocm_smi.h must be included after private/autogen/config.h
#endif

typedef int rsmi_status_t;
#define RSMI_STATUS_SUCCESS 0

typedef struct { int major; int minor; int patch; } rsmi_version_t;

typedef int RSMI_IO_LINK_TYPE;
#define RSMI_IOLINK_TYPE_XGMI 2

#define RSMI_MAX_NUM_FREQUENCIES 32

/**
 * @brief This structure holds information about clock frequencies.
 */
typedef struct {
  /**
   * The number of supported frequencies
   */
  uint32_t num_supported;

  /**
   * The current frequency index
   */
  uint32_t current;

  /**
   * List of frequencies.
   * Only the first num_supported frequencies are valid.
   */
  uint64_t frequency[RSMI_MAX_NUM_FREQUENCIES];
} rsmi_frequencies_t;
typedef rsmi_frequencies_t rsmi_frequencies;

/**
 * @brief This structure holds information about the possible PCIe
 * bandwidths. Specifically, the possible transfer rates and their
 * associated numbers of lanes are stored here.
 */
typedef struct {
  /**
   * Transfer rates (T/s) that are possible
   */
  rsmi_frequencies_t transfer_rate;

  /**
   * List of lanes for corresponding transfer rate.
   * Only the first num_supported bandwidths are valid.
   */
  uint32_t lanes[RSMI_MAX_NUM_FREQUENCIES];
} rsmi_pcie_bandwidth_t;

typedef enum {
  RSMI_MEM_TYPE_VRAM,
  RSMI_MEM_TYPE_VIS_VRAM,
  RSMI_MEM_TYPE_GTT,
} rsmi_memory_type_t;

rsmi_status_t rsmi_init(uint64_t init_flags);
rsmi_status_t rsmi_shut_down(void);
rsmi_status_t rsmi_version_get(rsmi_version_t *);
rsmi_status_t rsmi_status_string(rsmi_status_t, const char **string);
rsmi_status_t rsmi_num_monitor_devices(uint32_t *num_devices);
rsmi_status_t rsmi_dev_pci_id_get(uint32_t dv_ind, uint64_t *bdfid);
rsmi_status_t rsmi_dev_name_get(uint32_t dv_ind, char *name, size_t len);
rsmi_status_t rsmi_dev_serial_number_get(uint32_t dv_ind, char *serial_num, uint32_t len);
rsmi_status_t rsmi_dev_unique_id_get(uint32_t dv_ind, uint64_t *id);
rsmi_status_t rsmi_dev_pci_bandwidth_get(uint32_t dv_ind, rsmi_pcie_bandwidth_t *bandwidth);
rsmi_status_t rsmi_dev_partition_id_get(uint32_t dv_ind, uint32_t *partition_id);
rsmi_status_t rsmi_dev_xgmi_hive_id_get(uint32_t dv_ind, uint64_t *hive_id);
rsmi_status_t rsmi_topo_get_link_type(uint32_t dv_ind_src, uint32_t dv_ind_dst,
                                      uint64_t *hops, RSMI_IO_LINK_TYPE *type);
rsmi_status_t rsmi_dev_memory_total_get(uint32_t dv_ind, rsmi_memory_type_t mem_type,
                                        uint64_t *total);

#endif /* HWLOC_PORT_RSMI_RSMI_H */
