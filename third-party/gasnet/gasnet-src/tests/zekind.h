/*   $Source: bitbucket.org:berkeleylab/gasnet.git/tests/zekind.h $
 * Description: common code for test using ZE memory kind
 * Copyright (c) 2023, The Regents of the University of California
 * Terms of use are as specified in license.txt
 */

#ifndef _ZEKIND_H
#define _ZEKIND_H

#include <gasnetex.h>
#include <gasnet_mk.h>

#if !GASNET_HAVE_MK_CLASS_ZE
  #error This header is only for use when GASNET_HAVE_MK_CLASS_ZE is defined non-zero
#endif

#include <level_zero/ze_api.h>

#define check_zecall(op) do { \
    ze_result_t _ret = op; \
    if (_ret != ZE_RESULT_SUCCESS) { \
      FATALERR("GEX_MK_CLASS_ZE: %s FAILED", #op); \
    } \
  } while (0)

// Initializes args_p for the ZE GPU device with given deviceOrdinal, if any.
// Returns number of GPU devices found
// Caller must compare to the deviceOrdinal to determine if args_p has been initialized
static int test_open_ze_device(int deviceOrdinal, gex_MK_Create_args_t *args_p) {
  uint32_t driverCount = 0;
  check_zecall( zeInit(ZE_INIT_FLAG_GPU_ONLY) );
  check_zecall( zeDriverGet(&driverCount, NULL) );
  ze_driver_handle_t *driverArray = test_calloc(driverCount, sizeof(ze_driver_handle_t));
  check_zecall( zeDriverGet(&driverCount, driverArray) );
  int found = 0;
  for (uint32_t i = 0; i < driverCount; ++i) {
    ze_driver_handle_t driver = driverArray[i];
    uint32_t deviceCount = 0;
    check_zecall( zeDeviceGet(driver, &deviceCount, NULL) );
    ze_device_handle_t *deviceArray = test_calloc(deviceCount, sizeof(ze_device_handle_t));
    check_zecall( zeDeviceGet(driver, &deviceCount, deviceArray) );

    for (uint32_t j = 0; j < deviceCount; ++j) {
      ze_device_handle_t device = deviceArray[j];
      ze_device_properties_t deviceProperties = {ZE_STRUCTURE_TYPE_DEVICE_PROPERTIES,};
      check_zecall( zeDeviceGetProperties(device, &deviceProperties) );
      if (deviceProperties.type != ZE_DEVICE_TYPE_GPU) continue;

      if (found == deviceOrdinal) {
        ze_context_handle_t context;
        ze_context_desc_t cDesc = {ZE_STRUCTURE_TYPE_CONTEXT_DESC,};
        check_zecall( zeContextCreate(driver, &cDesc, &context) );

	args_p->gex_class = GEX_MK_CLASS_ZE;
        args_p->gex_args.gex_class_ze.gex_zeDevice = device;
        args_p->gex_args.gex_class_ze.gex_zeContext = context;
        args_p->gex_args.gex_class_ze.gex_zeMemoryOrdinal = 0;
      }
      
      found += 1;
    }
    test_free(deviceArray);
  }
  test_free(driverArray);

  return found;
}

#endif // _ZEKIND_H
