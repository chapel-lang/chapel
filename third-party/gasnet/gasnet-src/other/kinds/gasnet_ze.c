/*   $Source: bitbucket.org:berkeleylab/gasnet.git/other/kinds/gasnet_ze.c $
 * Description: GASNet Memory Kinds Implementation for oneAPI Level Zero
 * Copyright (c) 2022, The Regents of the University of California
 * Terms of use are as specified in license.txt
 */

#define GASNETI_NEED_GASNET_MK_H 1
#include <gasnet_internal.h>
#include <gasnet_kinds_internal.h>

#if GASNET_HAVE_MK_CLASS_ZE // Else empty

#include <level_zero/ze_api.h>

GASNETI_IDENT(gasneti_IdentString_MKClassZE, "$GASNetMKClassZE: 1 $");

//
// Class-specific MK type and functions
//

typedef struct my_MK_s {
  GASNETI_MK_COMMON // Class-indep prefix

  // TODO: This is an initial/experimental design.
  // For a final design we must determine which of these is *actually* needed.
  ze_device_handle_t        device;
  ze_context_handle_t       context;
  uint32_t                  ordinal;
} *my_MK_t;

//
// Error checking/reporting support
//
GASNETI_COLD
const char* gasneti_mk_ze_strerror(unsigned int result) {
  const char *errname = NULL;
  const char *errdesc = NULL;

  switch ((ze_result_t) result) {
    #define GASNETI_ZE_RESULT(name,desc) \
      case name: errname=#name; errdesc = desc; break;
    GASNETI_ZE_RESULT(ZE_RESULT_SUCCESS,
                      "[Core] success")
    GASNETI_ZE_RESULT(ZE_RESULT_NOT_READY,
                      "[Core] synchronization primitive not signaled")
    GASNETI_ZE_RESULT(ZE_RESULT_ERROR_DEVICE_LOST,
                      "[Core] device hung, reset, was removed, or driver update occurred")
    GASNETI_ZE_RESULT(ZE_RESULT_ERROR_OUT_OF_HOST_MEMORY,
                      "[Core] insufficient host memory to satisfy call")
    GASNETI_ZE_RESULT(ZE_RESULT_ERROR_OUT_OF_DEVICE_MEMORY,
                      "[Core] insufficient device memory to satisfy call")
    GASNETI_ZE_RESULT(ZE_RESULT_ERROR_MODULE_BUILD_FAILURE,
                      "[Core] error occurred when building module, see build log for details")
    GASNETI_ZE_RESULT(ZE_RESULT_ERROR_MODULE_LINK_FAILURE,
                      "[Core] error occurred when linking modules, see build log for details")
    GASNETI_ZE_RESULT(ZE_RESULT_ERROR_DEVICE_REQUIRES_RESET,
                      "[Core] device requires a reset")
    GASNETI_ZE_RESULT(ZE_RESULT_ERROR_DEVICE_IN_LOW_POWER_STATE,
                      "[Core] device currently in low power state")
    GASNETI_ZE_RESULT(ZE_RESULT_EXP_ERROR_DEVICE_IS_NOT_VERTEX,
                      "[Core, Expoerimental] device is not represented by a fabric vertex")
    GASNETI_ZE_RESULT(ZE_RESULT_EXP_ERROR_VERTEX_IS_NOT_DEVICE,
                      "[Core, Experimental] fabric vertex does not represent a device")
    GASNETI_ZE_RESULT(ZE_RESULT_EXP_ERROR_REMOTE_DEVICE,
                      "[Core, Expoerimental] fabric vertex represents a remote device or subdevice")
    GASNETI_ZE_RESULT(ZE_RESULT_ERROR_INSUFFICIENT_PERMISSIONS,
                      "[Sysman] access denied due to permission level")
    GASNETI_ZE_RESULT(ZE_RESULT_ERROR_NOT_AVAILABLE,
                      "[Sysman] resource already in use and simultaneous access not allowed or resource was removed")
    GASNETI_ZE_RESULT(ZE_RESULT_ERROR_DEPENDENCY_UNAVAILABLE,
                      "[Tools] external required dependency is unavailable or missing")
    GASNETI_ZE_RESULT(ZE_RESULT_WARNING_DROPPED_DATA,
                      "[Tools] data may have been dropped")
    GASNETI_ZE_RESULT(ZE_RESULT_ERROR_UNINITIALIZED,
                      "[Validation] driver is not initialized")
    GASNETI_ZE_RESULT(ZE_RESULT_ERROR_UNSUPPORTED_VERSION,
                      "[Validation] generic error code for unsupported versions")
    GASNETI_ZE_RESULT(ZE_RESULT_ERROR_UNSUPPORTED_FEATURE,
                      "[Validation] generic error code for unsupported features")
    GASNETI_ZE_RESULT(ZE_RESULT_ERROR_INVALID_ARGUMENT,
                      "[Validation] generic error code for invalid arguments")
    GASNETI_ZE_RESULT(ZE_RESULT_ERROR_INVALID_NULL_HANDLE,
                      "[Validation] handle argument is not valid")
    GASNETI_ZE_RESULT(ZE_RESULT_ERROR_HANDLE_OBJECT_IN_USE,
                      "[Validation] object pointed to by handle still in-use by device")
    GASNETI_ZE_RESULT(ZE_RESULT_ERROR_INVALID_NULL_POINTER,
                      "[Validation] pointer argument may not be nullptr")
    GASNETI_ZE_RESULT(ZE_RESULT_ERROR_INVALID_SIZE,
                      "[Validation] size argument is invalid (e.g., must not be zero)")
    GASNETI_ZE_RESULT(ZE_RESULT_ERROR_UNSUPPORTED_SIZE,
                      "[Validation] size argument is not supported by the device (e.g., too large)")
    GASNETI_ZE_RESULT(ZE_RESULT_ERROR_UNSUPPORTED_ALIGNMENT,
                      "[Validation] alignment argument is not supported by the device (e.g., too small)")
    GASNETI_ZE_RESULT(ZE_RESULT_ERROR_INVALID_SYNCHRONIZATION_OBJECT,
                      "[Validation] synchronization object in invalid state")
    GASNETI_ZE_RESULT(ZE_RESULT_ERROR_INVALID_ENUMERATION,
                      "[Validation] enumerator argument is not valid")
    GASNETI_ZE_RESULT(ZE_RESULT_ERROR_UNSUPPORTED_ENUMERATION,
                      "[Validation] enumerator argument is not supported by the device")
    GASNETI_ZE_RESULT(ZE_RESULT_ERROR_UNSUPPORTED_IMAGE_FORMAT,
                      "[Validation] image format is not supported by the device")
    GASNETI_ZE_RESULT(ZE_RESULT_ERROR_INVALID_NATIVE_BINARY,
                      "[Validation] native binary is not supported by the device")
    GASNETI_ZE_RESULT(ZE_RESULT_ERROR_INVALID_GLOBAL_NAME,
                      "[Validation] global variable is not found in the module")
    GASNETI_ZE_RESULT(ZE_RESULT_ERROR_INVALID_KERNEL_NAME,
                      "[Validation] kernel name is not found in the module")
    GASNETI_ZE_RESULT(ZE_RESULT_ERROR_INVALID_FUNCTION_NAME,
                      "[Validation] function name is not found in the module")
    GASNETI_ZE_RESULT(ZE_RESULT_ERROR_INVALID_GROUP_SIZE_DIMENSION,
                      "[Validation] group size dimension is not valid for the kernel or device")
    GASNETI_ZE_RESULT(ZE_RESULT_ERROR_INVALID_GLOBAL_WIDTH_DIMENSION,
                      "[Validation] global width dimension is not valid for the kernel or device")
    GASNETI_ZE_RESULT(ZE_RESULT_ERROR_INVALID_KERNEL_ARGUMENT_INDEX,
                      "[Validation] kernel argument index is not valid for kernel")
    GASNETI_ZE_RESULT(ZE_RESULT_ERROR_INVALID_KERNEL_ARGUMENT_SIZE,
                      "[Validation] kernel argument size does not match kernel")
    GASNETI_ZE_RESULT(ZE_RESULT_ERROR_INVALID_KERNEL_ATTRIBUTE_VALUE,
                      "[Validation] value of kernel attribute is not valid for the kernel or device")
    GASNETI_ZE_RESULT(ZE_RESULT_ERROR_INVALID_MODULE_UNLINKED,
                      "[Validation] module with imports needs to be linked before kernels can be created from it.")
    GASNETI_ZE_RESULT(ZE_RESULT_ERROR_INVALID_COMMAND_LIST_TYPE,
                      "[Validation] command list type does not match command queue type")
    GASNETI_ZE_RESULT(ZE_RESULT_ERROR_OVERLAPPING_REGIONS,
                      "[Validation] copy operations do not support overlapping regions of memory")
    GASNETI_ZE_RESULT(ZE_RESULT_WARNING_ACTION_REQUIRED,
                      "[Sysman] an action is required to complete the desired operation")
    GASNETI_ZE_RESULT(ZE_RESULT_ERROR_UNKNOWN,
                      "[Core] unknown or internal error")
    GASNETI_ZE_RESULT(ZE_RESULT_FORCE_UINT32, NULL)
    #undef GASNETI_ZE_RESULT
    default: (void)0;
  }
  if (!errname) {
    return gasneti_dynsprintf("UNRECOGNIZED(0x%x)", result);
  } else if (errdesc) {
    return gasneti_dynsprintf("%s: %s", errname, errdesc);
  } else {
    return errname;
  }
}
#define gasneti_check_zecall(op) do {                \
   ze_result_t _result = (op);                       \
    if_pf (_result != ZE_RESULT_SUCCESS ) {          \
      gasneti_fatalerror("%s returned %s", #op,      \
                         gasneti_mk_ze_strerror((unsigned int)_result)); \
    }                                                \
  } while (0)

static const char *gasneti_formatmk_ze(gasneti_MK_t i_mk)
{
  my_MK_t kind = (my_MK_t) i_mk;
  return gasneti_dynsprintf("ZE(gex_zeDevice=%p, gex_zeContext=%p, gex_zeMemoryOrdinal=%u)",
                            (void*)kind->device, (void*)kind->context, (unsigned int)kind->ordinal);
}

static gasneti_mk_impl_t *get_impl(void);

//
// Class-specific MK_Create
//
int gasneti_MK_Create_ze(
            gasneti_MK_t                     *i_memkind_p,
            gasneti_Client_t                 client,
            const gex_MK_Create_args_t       *args,
            gex_Flags_t                      flags)
{
  gasneti_static_assert(sizeof(ze_device_handle_t) == sizeof(void*));
  gasneti_static_assert(sizeof(ze_context_handle_t) ==  sizeof(void*));
  
  // TODO: validation
  // is Device valid?
  // is Context valid?
  // is are Device and Context from the same Driver?
  // is MemoryOrdinal valid for this Device?

  my_MK_t result = (my_MK_t) gasneti_alloc_mk(client, get_impl(), flags);
  result->device = args->gex_args.gex_class_ze.gex_zeDevice;
  result->context = args->gex_args.gex_class_ze.gex_zeContext;
  result->ordinal = args->gex_args.gex_class_ze.gex_zeMemoryOrdinal;

  *i_memkind_p = (gasneti_MK_t) result;
  return GASNET_OK;
}

//
// Class-specific Segment_Create
//
static int gasneti_MK_Segment_Create_ze(
            gasneti_Segment_t                *i_segment_p,
            gasneti_MK_t                     i_mk,
            void *                           addr,
            uintptr_t                        size,
            gex_Flags_t                      flags)
{
  my_MK_t kind = (my_MK_t) i_mk;
  ze_result_t result;
  void * to_free = NULL;
  const char * failure = NULL;

  if (addr) { // Client-allocated
    // check that addr properties match the kind
    ze_memory_allocation_properties_t props = {ZE_STRUCTURE_TYPE_MEMORY_ALLOCATION_PROPERTIES,};
    ze_device_handle_t device = NULL;
    gasneti_check_zecall( zeMemGetAllocProperties(kind->context, addr, &props, &device) );
    if (props.type == ZE_MEMORY_TYPE_UNKNOWN) {
      failure = "address was not allocated with the gex_zeContext passed to gex_MK_Create()";
    } else if (props.type != ZE_MEMORY_TYPE_DEVICE) {
      failure = "address is not device memory";
    } else if (device != kind->device) {
      failure = "address was not allocated from the gex_zeDevice passed to gex_MK_Create()";
    }
  } else { // GASNet-allocated
    ze_device_mem_alloc_desc_t allocDesc = {ZE_STRUCTURE_TYPE_DEVICE_MEM_ALLOC_DESC,};
    allocDesc.ordinal = kind->ordinal;
    result = zeMemAllocDevice( kind->context, &allocDesc, size, 0, kind->device, &addr );
    if (result != ZE_RESULT_SUCCESS) {
      failure = gasneti_mk_ze_strerror(result);
    }
    to_free = addr;
  }

  if (failure) {
    const char *msg = gasneti_dynsprintf("GEX_MK_CLASS_ZE: %s", failure);
    GASNETI_RETURN_ERRR(BAD_ARG, msg);
  }

  gasneti_Client_t client = i_mk->_client;
  gex_MK_t e_mk = gasneti_export_mk(i_mk);
  gasneti_Segment_t i_segment = gasneti_alloc_segment(client, addr, size, e_mk, !to_free, flags);
  i_segment->_opaque_mk_use = to_free;

  *i_segment_p = i_segment;

  return GASNET_OK;
}

static void gasneti_MK_Segment_Destroy_ze(
           gasneti_Segment_t                i_segment)
{
  void *to_free = i_segment->_opaque_mk_use;
  if (to_free) {
    my_MK_t kind = (my_MK_t) gasneti_import_mk_nonhost(i_segment->_kind);
    gasneti_check_zecall( zeMemFree(kind->context, to_free) );
  }
}

//
// Class-specific "impl(ementation)": constants and function pointers.
//
// Due to lack of designated initializers in GASNet's required C99 subset, we
// address the fragility as the structure grows or changes by lazy explicit
// initialization.
static gasneti_mk_impl_t *get_impl(void) {
  // Static storage duration ensures these are zero-initialized
  static gasneti_mk_impl_t the_impl;
  static gasneti_mk_impl_t *result;

  if (!result) {
    static gasneti_mutex_t lock = GASNETI_MUTEX_INITIALIZER;
    gasneti_mutex_lock(&lock);
    if (!result) {
      the_impl.mk_class     = GEX_MK_CLASS_ZE;
      the_impl.mk_name      = "ZE";
      the_impl.mk_sizeof    = sizeof(struct my_MK_s);

      the_impl.mk_format    = &gasneti_formatmk_ze;
      the_impl.mk_destroy   = NULL; // No class-specific MK_Destroy needed
      the_impl.mk_segment_create
                            = gasneti_MK_Segment_Create_ze;
      the_impl.mk_segment_destroy
                            = gasneti_MK_Segment_Destroy_ze;

      gasneti_sync_writes();
      result = &the_impl;
    }
    gasneti_mutex_unlock(&lock);
  } else {
    gasneti_sync_reads();
  }

  gasneti_assert(result);
  return result;
}

// Determine the kind's device ordinal, used by at least libfabric.
// This is basic device enumeration, terminated when a matching device is found.
// This works because enumeration calls are guaranteed to return the same
// handle for each device every time.
// If `gpu_only` is non-zero, skips devices with `type != ZE_DEVICE_TYPE_GPU`.
// Returns -1 is there is no match found. 
int gasneti_mk_ze_device_ordinal(void *device_handle_arg, int gpu_only)
{
  ze_device_handle_t theDevice = (ze_device_handle_t) device_handle_arg;

  uint32_t driverCount = 0;
  gasneti_check_zecall( zeDriverGet(&driverCount, NULL) );

  ze_driver_handle_t *driverArray = gasneti_malloc(driverCount * sizeof(ze_driver_handle_t));
  gasneti_check_zecall( zeDriverGet(&driverCount, driverArray) );

  ze_device_handle_t *deviceArray = NULL;

  int count = 0;
  int result = -1;
  for (uint32_t i = 0; i < driverCount; ++i) {
    ze_driver_handle_t driver = driverArray[i];
    uint32_t deviceCount = 0;
    gasneti_check_zecall( zeDeviceGet(driver, &deviceCount, NULL) );
    deviceArray = gasneti_realloc(deviceArray, deviceCount * sizeof(ze_device_handle_t));
    gasneti_check_zecall( zeDeviceGet(driver, &deviceCount, deviceArray) );

    for (uint32_t j = 0; j < deviceCount; ++j) {
      ze_device_handle_t currDevice = deviceArray[j];

      // Only count GPU devices in our enumeration
      // This is needed because, in general, we don't control the zeInit() arguments
      ze_device_properties_t deviceProperties = {ZE_STRUCTURE_TYPE_DEVICE_PROPERTIES,};
      gasneti_check_zecall( zeDeviceGetProperties(currDevice, &deviceProperties) );
      if (gpu_only && (deviceProperties.type != ZE_DEVICE_TYPE_GPU)) continue;

      if (currDevice == theDevice) {
        result = count;
        goto done;
      }

      ++count;
    }
  }

done:
  gasneti_free(deviceArray);
  gasneti_free(driverArray);

  return result;
}

#endif
