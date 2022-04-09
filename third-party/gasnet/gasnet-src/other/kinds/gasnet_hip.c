/*   $Source: bitbucket.org:berkeleylab/gasnet.git/other/kinds/gasnet_hip.c $
 * Description: GASNet Memory Kinds Implementation for HIP devices
 * Copyright (c) 2021, The Regents of the University of California
 * Terms of use are as specified in license.txt
 */

#define GASNETI_NEED_GASNET_MK_H 1
#include <gasnet_internal.h>
#include <gasnet_kinds_internal.h>

#if GASNET_HAVE_MK_CLASS_HIP // Else empty

#include <hip/hip_runtime_api.h>

GASNETI_IDENT(gasneti_IdentString_MKClassHIP, "$GASNetMKClassHIP: 1 $");

#if GASNETI_HIP_PLATFORM_AMD
  GASNETI_IDENT(gasneti_IdentString_HIPPlatform, "$GASNetHIPPlatform: AMD $");
#elif GASNETI_HIP_PLATFORM_NVIDIA
  GASNETI_IDENT(gasneti_IdentString_HIPPlatform, "$GASNetHIPPlatform: NVIDIA $");
#else
  #error Unknown HIP Platform
#endif

//
// Class-specific MK type and functions
//

typedef struct my_MK_s {
  GASNETI_MK_COMMON // Class-indep prefix

  hipCtx_t           ctx;
  hipDevice_t        dev;
} *my_MK_t;

// Format for use of hipGetErrorName()
#define GASNETI_HIPRESULT_FMT        "%s(%d)"
#define GASNETI_HIPRESULT_ARG(res)   hipGetErrorName(res),(res)

//
// Error checking/reporting wrapper
//
#define gasneti_check_hipcall(op) do {              \
    hipError_t _retval = (op);                      \
    if_pf (_retval) {                               \
      gasneti_fatalerror("%s returned "GASNETI_HIPRESULT_FMT,#op,GASNETI_HIPRESULT_ARG(_retval));\
    }                                               \
  } while (0)

static gasneti_mk_impl_t *get_impl(void);

// Convenience wrappers around hip{Get,Set}Device()
#define GASNETI_SAVE_AND_SET_HIP_DEVICE(kind, prev) do { \
    gasneti_check_hipcall( hipGetDevice(&(prev)) );         \
    if ((prev) != (kind)->dev) {                            \
      gasneti_check_hipcall( hipSetDevice((kind)->dev) );   \
    }                                                       \
  } while (0)
#define GASNETI_RESTORE_HIP_DEVICE(kind, prev) do { \
    if ((prev) != (kind)->dev) {                            \
      gasneti_check_hipcall( hipSetDevice(prev) );          \
    }                                                       \
  } while (0)

static const char *gasneti_formatmk_hip(gasneti_MK_t i_mk)
{
  my_MK_t kind = (my_MK_t) i_mk;
  return gasneti_dynsprintf("HIP(gex_hipDevice=%i)", (int)kind->dev);
}

//
// Class-specific MK_Create
//
int gasneti_MK_Create_hip(
            gasneti_MK_t                     *i_memkind_p,
            gasneti_Client_t                 client,
            const gex_MK_Create_args_t       *args,
            gex_Flags_t                      flags)
{
  hipDevice_t dev = args->gex_args.gex_class_hip.gex_hipDevice;
  GASNETI_TRACE_PRINTF(O,("gex_MK_Create: class=HIP gex_hipDevice=%d", dev));

  if (dev < 0) {
    // This is always treated as programmer error
    gasneti_fatalerror("gex_MK_Create called with negative hipDevice_t=%i", dev);
  }

#if PLATFORM_OS_LINUX && GASNET_CONDUIT_IBV
 #if GASNETI_HIP_PLATFORM_NVIDIA
  // Look for NVIDIA GDR support
  const char *filename = "/sys/kernel/mm/memory_peers/nv_mem/version";
 #else
  // Look for AMD GDR support (AMD Kernel Fusion Driver == amdkfd).
  const char *filename = "/sys/kernel/mm/memory_peers/amdkfd/version";
 #endif
  if (access(filename, F_OK)) {
    // TODO: gracefully fall back to "reference implementation",
    // once one is available, rather than failing.
    GASNETI_RETURN_ERRR(BAD_ARG,"GEX_MK_CLASS_HIP: kernel lacks GPUDirect RDMA support");
  }
#endif

  // Obtain the primary context for the given device, initializing if needed
  hipCtx_t ctx;
  hipError_t res = hipDevicePrimaryCtxRetain(&ctx, dev);
  if (res == hipErrorNotInitialized) {
    int initRes = hipInit(0);
    if (initRes == hipSuccess) {
      res = hipDevicePrimaryCtxRetain(&ctx, dev);
    } else if (initRes == hipErrorNoDevice) {
      GASNETI_RETURN_ERRR(BAD_ARG,"GEX_MK_CLASS_HIP: no HIP devices found");
    } else {
      const char *msg = gasneti_dynsprintf("GEX_MK_CLASS_HIP: hipInit() returned %i", initRes);
      GASNETI_RETURN_ERRR(BAD_ARG,msg);
    }
  }

  // Failed to obtain the primary context, try to reason out why
  // TODO: explicit diagnosis of more failure cases
  if_pf (res != hipSuccess) {
    const char *why = "unknown failure";
    if (res == hipErrorInvalidDevice) {
      int dev_count;
      if (hipGetDeviceCount(&dev_count)) {
        why = "hipGetDeviceCount() failed";
      } else if (! dev_count) {
        why = "no HIP devices found";
      } else {
        why = gasneti_dynsprintf("invalid hipDevice_t=%i (%d devices found)", dev, dev_count);
      }
    } else {
      why = gasneti_dynsprintf("hipDevicePrimaryCtxRetain() returned %i", res);
    }
    const char *msg = gasneti_dynsprintf("GEX_MK_CLASS_HIP: %s", why);
    GASNETI_RETURN_ERRR(BAD_ARG,msg);
  }

  my_MK_t result = (my_MK_t) gasneti_alloc_mk(client, get_impl(), flags);
  result->dev = dev;
  result->ctx = ctx;

#if 0 // TODO: enable if HIP has equivalent of CU_POINTER_ATTRIBUTE_SYNC_MEMOPS
  result->use_sync_memops = gasneti_getenv_yesno_withdefault("GASNET_USE_HIP_SYNC_MEMOPS", 1);
#endif

  *i_memkind_p = (gasneti_MK_t) result;
  return GASNET_OK;
}

//
// Class-specific MK_Destroy
//
static void gasneti_MK_Destroy_hip(
            gasneti_MK_t                     i_mk,
            gex_Flags_t                      flags)
{
  my_MK_t mk = (my_MK_t) i_mk;
  gasneti_check_hipcall(hipDevicePrimaryCtxRelease(mk->dev));
  gasneti_free_mk(i_mk);
}

//
// Class-specific Segment_Create
//
static int gasneti_MK_Segment_Create_hip(
            gasneti_Segment_t                *i_segment_p,
            gasneti_MK_t                     i_mk,
            void *                           addr,
            uintptr_t                        size,
            gex_Flags_t                      flags)
{
  my_MK_t kind = (my_MK_t) i_mk;
  hipError_t result;
  void * to_free = NULL;
  int retval = GASNET_OK;

  // TODO:
  // Might want additional care with respect to error returns from the HIP device API.

  int prevDevice;
  GASNETI_SAVE_AND_SET_HIP_DEVICE(kind, prevDevice);

  if (addr) { // Client-allocated
    hipPointerAttribute_t attr;
    gasneti_check_hipcall(hipPointerGetAttributes(&attr, addr));

    if (attr.memoryType != hipMemoryTypeDevice) {
      gasneti_fatalerror("Invalid call to gex_Segment_Create(HIP) with non-device memory");
    }
    if (attr.isManaged) {
      gasneti_fatalerror("Invalid call to gex_Segment_Create(HIP) with managed memory");
    }
    if (attr.device != kind->dev) {
      gasneti_fatalerror("Invalid call to gex_Segment_Create(HIP) with memory associated with wrong device");
    }
  } else { // GASNet-allocated
    result = hipMalloc(&addr, size);

    if (result == hipErrorOutOfMemory) {
      retval = GASNET_ERR_RESOURCE;
      goto out;
    } else if (result != hipSuccess) {
      gasneti_fatalerror("hipMalloc() returned unexpected failure: "
                         GASNETI_HIPRESULT_FMT, GASNETI_HIPRESULT_ARG(result));
    }

    to_free = addr;
  }

#if 0
  if (kind->use_sync_memops) {
    // TODO: if HIP has equivalent of CU_POINTER_ATTRIBUTE_SYNC_MEMOPS, apply it here
  }
#endif

  gasneti_Client_t client = i_mk->_client;
  gex_MK_t e_mk = gasneti_export_mk(i_mk);
  gasneti_Segment_t i_segment = gasneti_alloc_segment(client, addr, size, e_mk, !to_free, flags);
  i_segment->_opaque_mk_use = to_free;

  *i_segment_p = i_segment;

out:
  GASNETI_RESTORE_HIP_DEVICE(kind, prevDevice);
  return retval;
}

static void gasneti_MK_Segment_Destroy_hip(
           gasneti_Segment_t                i_segment)
{
  void *to_free = i_segment->_opaque_mk_use;
  if (to_free) {
    gasneti_check_hipcall( hipFree(to_free) );
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
      the_impl.mk_class     = GEX_MK_CLASS_HIP;
      the_impl.mk_name      = "HIP";
      the_impl.mk_sizeof    = sizeof(struct my_MK_s);

      the_impl.mk_format    = &gasneti_formatmk_hip;
      the_impl.mk_destroy   = &gasneti_MK_Destroy_hip;
      the_impl.mk_segment_create
                            = &gasneti_MK_Segment_Create_hip;
      the_impl.mk_segment_destroy
                            = &gasneti_MK_Segment_Destroy_hip;

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
#endif
