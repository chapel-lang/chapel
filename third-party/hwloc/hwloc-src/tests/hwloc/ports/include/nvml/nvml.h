/*
 * Copyright © 2013-2020 Inria.  All rights reserved.
 * See COPYING in top-level directory.
 */

#ifndef HWLOC_PORT_NVML_NVML_H
#define HWLOC_PORT_NVML_NVML_H

/* we need to replace any NVML-related #define that configure may have put in private/autogen/config.h */
#ifndef HWLOC_CONFIGURE_H
#error nvml.h must be included after private/autogen/config.h
#endif
#undef HAVE_DECL_NVMLDEVICEGETMAXPCIELINKGENERATION
#define HAVE_DECL_NVMLDEVICEGETMAXPCIELINKGENERATION 1


typedef int nvmlReturn_t;
#define NVML_SUCCESS 0

typedef struct nvmlPciInfo_st {
  unsigned int domain;
  unsigned int bus;
  unsigned int device;
} nvmlPciInfo_t;

struct nvmlDevice_st;
typedef struct nvmlDevice_st* nvmlDevice_t;

nvmlReturn_t nvmlInit(void);
nvmlReturn_t nvmlShutdown(void);
char* nvmlErrorString(nvmlReturn_t result);
nvmlReturn_t nvmlDeviceGetCount(unsigned int *deviceCount);
nvmlReturn_t nvmlDeviceGetHandleByIndex(unsigned int index, nvmlDevice_t *device);
nvmlReturn_t nvmlDeviceGetPciInfo(nvmlDevice_t device, nvmlPciInfo_t *pci);
nvmlReturn_t nvmlDeviceGetName(nvmlDevice_t device, char *name, unsigned int length);
nvmlReturn_t nvmlDeviceGetSerial(nvmlDevice_t device, char *serial, unsigned int length);
nvmlReturn_t nvmlDeviceGetUUID(nvmlDevice_t device, char *uuid, unsigned int length);
nvmlReturn_t nvmlDeviceGetMaxPcieLinkGeneration(nvmlDevice_t device, unsigned int *maxLinkGen);
nvmlReturn_t nvmlDeviceGetMaxPcieLinkWidth(nvmlDevice_t device, unsigned int *maxLinkGen);

#define NVML_NVLINK_MAX_LINKS 6
typedef unsigned nvmlEnableState_t;
#define NVML_FEATURE_ENABLED 1
nvmlReturn_t nvmlDeviceGetNvLinkState(nvmlDevice_t device, unsigned int link, nvmlEnableState_t * isActive);
nvmlReturn_t nvmlDeviceGetNvLinkVersion(nvmlDevice_t device, unsigned int  link, unsigned int* version);
nvmlReturn_t nvmlDeviceGetNvLinkRemotePciInfo(nvmlDevice_t device, unsigned int link, nvmlPciInfo_t * pci);

#endif /* HWLOC_PORT_NVML_NVML_H */
