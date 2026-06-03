/*
 * Copyright © 2020-2024 Inria.  All rights reserved.
 * See COPYING in top-level directory.
 */

#ifndef HWLOC_PORT_L0_ZET_API_H
#define HWLOC_PORT_L0_ZET_API_H

#include "ze_api.h"

extern ze_result_t zesInit(int);

typedef void * zes_driver_handle_t;
typedef void * zes_device_handle_t;

typedef ze_device_uuid_t zes_uuid_t;

extern ze_result_t zesDriverGet(uint32_t *, zes_driver_handle_t *);
extern ze_result_t zesDriverGetDeviceByUuidExp(zes_driver_handle_t, zes_uuid_t, zes_device_handle_t *, ze_bool_t *, uint32_t *);

typedef struct {
  char *vendorName;
  char *brandName;
  char *modelName;
  char *serialNumber;
  char *boardNumber;
  unsigned numSubdevices;
} zes_device_properties_t;

extern ze_result_t zesDeviceGetProperties(zes_device_handle_t, zes_device_properties_t *);

typedef struct {
  struct {
    unsigned domain, bus, device, function;
  } address;
  struct {
    unsigned gen;
    unsigned lanes;
    unsigned maxBandwidth;
  } maxSpeed;
} zes_pci_properties_t;

extern ze_result_t zesDevicePciGetProperties(zes_device_handle_t, zes_pci_properties_t *);

typedef enum _zes_mem_type_t {
    ZES_MEM_TYPE_HBM = 0,
    ZES_MEM_TYPE_DDR = 1,
    ZES_MEM_TYPE_DDR3 = 2,
    ZES_MEM_TYPE_DDR4 = 3,
    ZES_MEM_TYPE_DDR5 = 4,
    ZES_MEM_TYPE_LPDDR = 5,
    ZES_MEM_TYPE_LPDDR3 = 6,
    ZES_MEM_TYPE_LPDDR4 = 7,
    ZES_MEM_TYPE_LPDDR5 = 8,
    ZES_MEM_TYPE_SRAM = 9,
    ZES_MEM_TYPE_L1 = 10,
    ZES_MEM_TYPE_L3 = 11,
    ZES_MEM_TYPE_GRF = 12,
    ZES_MEM_TYPE_SLM = 13,
} zes_mem_type_t;

typedef void * zes_mem_handle_t;

extern ze_result_t zesDeviceEnumMemoryModules(zes_device_handle_t, uint32_t*, zes_mem_handle_t*);

typedef struct {
  zes_mem_type_t type;
  int onSubdevice;
  unsigned subdeviceId;
  uint64_t physicalSize;
} zes_mem_properties_t;

extern ze_result_t zesMemoryGetProperties(zes_mem_handle_t, zes_mem_properties_t*);

typedef struct {
  uint64_t size;
} zes_mem_state_t;

extern ze_result_t zesMemoryGetState(zes_mem_handle_t, zes_mem_state_t*);

typedef void * zes_fabric_port_handle_t;

extern ze_result_t zesDeviceEnumFabricPorts(zes_device_handle_t, uint32_t*, zes_fabric_port_handle_t*);

typedef struct _zes_fabric_port_id_t {
  uint32_t fabricId;
  uint32_t attachId;
  uint8_t portNumber;
} zes_fabric_port_id_t;

typedef struct _zes_fabric_port_speed_t {
  int64_t bitRate;
  int32_t width;
} zes_fabric_port_speed_t;

#define ZES_MAX_FABRIC_PORT_MODEL_SIZE 256
typedef struct _zes_fabric_port_properties_t {
  char model[ZES_MAX_FABRIC_PORT_MODEL_SIZE];
  int onSubdevice;
  uint32_t subdeviceId;
  zes_fabric_port_id_t portId;
  zes_fabric_port_speed_t maxRxSpeed;
  zes_fabric_port_speed_t maxTxSpeed;
} zes_fabric_port_properties_t;

extern ze_result_t zesFabricPortGetProperties(zes_fabric_port_handle_t, zes_fabric_port_properties_t *);

typedef enum _zes_fabric_port_status_t {
    ZES_FABRIC_PORT_STATUS_UNKNOWN = 0,
    ZES_FABRIC_PORT_STATUS_HEALTHY = 1,
    ZES_FABRIC_PORT_STATUS_DEGRADED = 2,
    ZES_FABRIC_PORT_STATUS_FAILED = 3,
    ZES_FABRIC_PORT_STATUS_DISABLED = 4
} zes_fabric_port_status_t;

typedef struct _zes_fabric_port_state_t {
  zes_fabric_port_status_t status;
  zes_fabric_port_id_t remotePortId;
  zes_fabric_port_speed_t rxSpeed;
  zes_fabric_port_speed_t txSpeed;
} zes_fabric_port_state_t;

ze_result_t zesFabricPortGetState(zes_fabric_port_handle_t, zes_fabric_port_state_t *);

#endif /* HWLOC_PORT_L0_ZET_API_H */
