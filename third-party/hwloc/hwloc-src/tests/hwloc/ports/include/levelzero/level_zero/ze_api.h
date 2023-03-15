/*
 * Copyright © 2020-2022 Inria.  All rights reserved.
 * See COPYING in top-level directory.
 */

#ifndef HWLOC_PORT_L0_ZE_API_H
#define HWLOC_PORT_L0_ZE_API_H

typedef int ze_result_t;
#define ZE_RESULT_SUCCESS 0

#define ZE_MAX_DEVICE_NAME 64

typedef void * ze_driver_handle_t;
typedef void * ze_device_handle_t;

extern ze_result_t zeInit(int);
extern ze_result_t zeDriverGet(uint32_t *, ze_driver_handle_t *);
extern ze_result_t zeDeviceGet(ze_driver_handle_t, uint32_t *, ze_device_handle_t *);

typedef enum _ze_device_type {
  ZE_DEVICE_TYPE_GPU = 1,
  ZE_DEVICE_TYPE_CPU = 2,
  ZE_DEVICE_TYPE_FPGA = 3,
  ZE_DEVICE_TYPE_MCA = 4,
  ZE_DEVICE_TYPE_VPU = 5
} ze_device_type_t;

#define ZE_DEVICE_PROPERTY_FLAG_INTEGRATED (1<<0)
#define ZE_DEVICE_PROPERTY_FLAG_SUBDEVICE (1<<1)

#define ZE_MAX_DEVICE_UUID_SIZE 16

typedef struct ze_device_uuid {
  uint8_t id[ZE_MAX_DEVICE_UUID_SIZE];
} ze_device_uuid_t;

typedef struct ze_device_properties {
  ze_device_type_t type;
  unsigned flags;
  uint32_t numThreadsPerEU;
  uint32_t numEUsPerSubslice;
  uint32_t numSubslicesPerSlice;
  uint32_t numSlices;
  ze_device_uuid_t uuid;
} ze_device_properties_t;

extern ze_result_t zeDeviceGetProperties(ze_device_handle_t, ze_device_properties_t *);

typedef struct ze_command_queue_group_properties {
  unsigned long flags;
  unsigned numQueues;
} ze_command_queue_group_properties_t;

extern ze_result_t zeDeviceGetCommandQueueGroupProperties(ze_driver_handle_t, uint32_t *, ze_command_queue_group_properties_t *);

extern ze_result_t zeDeviceGetSubDevices(ze_device_handle_t, uint32_t *, ze_device_handle_t*);

typedef struct ze_device_memory_properties {
  uint64_t totalSize;
  char *name;
} ze_device_memory_properties_t;

extern ze_result_t zeDeviceGetMemoryProperties(ze_device_handle_t, uint32_t *, ze_device_memory_properties_t*);

typedef struct ze_pci_address_ext {
  uint32_t domain, bus, device, function;
} ze_pci_address_ext_t;

typedef struct ze_pci_speed_ext {
  int64_t maxBandwidth;
} ze_pci_speed_ext_t;

typedef int ze_structure_type_t;

#define ZE_STRUCTURE_TYPE_PCI_EXT_PROPERTIES 0x10008

typedef struct ze_pci_ext_properties {
  ze_structure_type_t stype;
  void* pNext;
  ze_pci_address_ext_t address;
  ze_pci_speed_ext_t maxSpeed;
} ze_pci_ext_properties_t;

extern ze_result_t zeDevicePciGetPropertiesExt(ze_device_handle_t, ze_pci_ext_properties_t *);

#endif /* HWLOC_PORT_L0_ZE_API_H */
