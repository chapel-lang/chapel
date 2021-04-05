/*   $Source: bitbucket.org:berkeleylab/gasnet.git/other/kinds/gasnet_cuda_uva.c $
 * Description: GASNet Memory Kinds Implementation for CUDA UVA devices
 * Copyright (c) 2020, The Regents of the University of California
 * Terms of use are as specified in license.txt
 */

#define GASNETI_NEED_GASNET_MK_H 1
#include <gasnet_internal.h>
#include <gasnet_kinds_internal.h>

#if GASNET_HAVE_MK_CLASS_CUDA_UVA // Else empty

#include <cuda.h>

GASNETI_IDENT(gasneti_IdentString_MKClassCUDAUVA, "$GASNetMKClassCUDAUVA: 1 $");

//
// Class-specific MK type and functions
//

typedef struct my_MK_s {
  GASNETI_MK_COMMON // Class-indep prefix

  CUcontext       ctx;
  CUdevice        dev;
  int             use_sync_memops;
} *my_MK_t;

// Wrapper and format for use of cuGetErrorName()
const char *_gasneti_cuerror_name(CUresult res) {
  static const char *unknown = "UNKNOWN";
  const char *errorname;
  if (cuGetErrorName(res, &errorname)) errorname = unknown;
  return errorname;
}
#define GASNETI_CURESULT_FMT         "%s(%d)"
#define GASNETI_CURESULT_STRING(res) _gasneti_cuerror_name(res),(res)

//
// Error checking/reporting wrapper
//
#define gasneti_check_cudacall(op) do {              \
    CUresult _retval = (op);                                \
    if_pf (_retval) {                                       \
      gasneti_fatalerror("%s returned "GASNETI_CURESULT_FMT,#op,GASNETI_CURESULT_STRING(_retval));\
    }                                                       \
  } while (0)

static void gasneti_MK_Destroy_cuda_uva(
            gasneti_MK_t                     i_mk,
            gex_Flags_t                      flags)
{
  my_MK_t mk = (my_MK_t) i_mk;
  gasneti_check_cudacall(cuCtxSetCurrent(NULL));
  gasneti_check_cudacall(cuDevicePrimaryCtxRelease(mk->dev));
  gasneti_free_mk(i_mk);
}

static int gasneti_MK_Segment_Create_cuda_uva(
            gasneti_Segment_t                *i_segment_p,
            gasneti_MK_t                     i_mk,
            void *                           addr,
            uintptr_t                        size,
            gex_Flags_t                      flags)
{
  my_MK_t kind = (my_MK_t) i_mk;
  CUdeviceptr dptr;
  CUresult result;
  void * to_free = NULL;
  int retval = GASNET_OK;

  gasneti_check_cudacall(cuCtxPushCurrent(kind->ctx));

  // TODO:
  // Might want additional care with respect to error returns from the CUDA device API.
  // In particular, any call "may also return error codes from previous, asynchronous launches."
  // Presently, we try to always provide the specific CUDA error code as we fatalerror.

  if (addr) { // Client-allocated
    dptr = (CUdeviceptr)addr;

    // cuPointerGetAttributes available since CUDA 7.0
    unsigned int mem_type = 0;
    unsigned int is_managed = 0;
    CUcontext ctx = NULL;
    void * ptrs[3] = { (void*)&mem_type, (void*)&is_managed, (void*)&ctx };
    CUpointer_attribute attrs[3] = { CU_POINTER_ATTRIBUTE_MEMORY_TYPE,
                                     CU_POINTER_ATTRIBUTE_IS_MANAGED,
                                     CU_POINTER_ATTRIBUTE_CONTEXT };

    result = cuPointerGetAttributes(3, attrs, ptrs, dptr);
    if (result) {
      gasneti_fatalerror("Failed to query pointer attributes of client-allocated memory: "
                         GASNETI_CURESULT_FMT, GASNETI_CURESULT_STRING(result));
    }

    if (mem_type != CU_MEMORYTYPE_DEVICE) {
      gasneti_fatalerror("Invalid call to gex_Segment_Create(CUDA_UVA) with non-device memory");
    }
    if (is_managed) {
      gasneti_fatalerror("Invalid call to gex_Segment_Create(CUDA_UVA) with managed memory");
    }

    // We currently accept memory allocated by *any* context for the same device.
    // TODO: should we be more strict by checking equality of contexts instead of devices?
    CUdevice dev;
    if ((result = cuCtxPushCurrent(ctx)) ||
        (result = cuCtxGetDevice(&dev))  ||
        (result = cuCtxPopCurrent(&ctx))) {
      gasneti_fatalerror("Failed to query CUDA device of client-allocated memory: "
                         GASNETI_CURESULT_FMT, GASNETI_CURESULT_STRING(result));
    } else if (dev != kind->dev) {
      gasneti_fatalerror("gex_Segment_Create(CUDA_UVA) with memory associated with wrong device");
    }
  } else { // GASNet-allocated
    result = cuMemAlloc(&dptr, size);

    if (result == CUDA_ERROR_OUT_OF_MEMORY) {
      retval = GASNET_ERR_RESOURCE;
      goto out;
    } else if (result != CUDA_SUCCESS) {
      gasneti_fatalerror("cuMemAlloc() returned unexpected failure: "
                         GASNETI_CURESULT_FMT, GASNETI_CURESULT_STRING(result));
    }

    addr = to_free = (void *) dptr;
  }

  if (kind->use_sync_memops) {
    int one = 1;
    gasneti_check_cudacall(cuPointerSetAttribute(&one, CU_POINTER_ATTRIBUTE_SYNC_MEMOPS, dptr));
  }

  gasneti_Client_t client = i_mk->_client;
  gex_MK_t e_mk = gasneti_export_mk(i_mk);
  gasneti_Segment_t i_segment = gasneti_alloc_segment(client, addr, size, e_mk, flags);
  i_segment->_opaque_mk_use = to_free;

  *i_segment_p = i_segment;

out:
  {
    CUcontext prev_ctx;
    gasneti_check_cudacall(cuCtxPopCurrent(&prev_ctx));
    gasneti_assert(prev_ctx == kind->ctx);
  }
  return retval;
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
      the_impl.mk_class     = GEX_MK_CLASS_CUDA_UVA;
      the_impl.mk_name      = "CUDA_UVA";
      the_impl.mk_sizeof    = sizeof(struct my_MK_s);

      the_impl.mk_destroy   = &gasneti_MK_Destroy_cuda_uva;
      the_impl.mk_segment_create
                            = &gasneti_MK_Segment_Create_cuda_uva;

      gasneti_sync_writes();
      result = &the_impl;
    }
    gasneti_mutex_unlock(&lock);
  }

  gasneti_assert(result);
  return result;
}

// Class-specific create
int gasneti_MK_Create_cuda_uva(
            gasneti_MK_t                     *i_memkind_p,
            gasneti_Client_t                 client,
            const gex_MK_Create_args_t       *args,
            gex_Flags_t                      flags)
{
  CUdevice dev = args->gex_args.gex_class_cuda_uva.gex_CUdevice;
  GASNETI_TRACE_PRINTF(O,("gex_MK_Create: class=CUDA_UVA gex_CUdevice=%d", dev));

  if (dev < 0) {
    // This is always treated as programmer error
    gasneti_fatalerror("gex_MK_Create called with negative CUdevice=%i", dev);
  }

#if PLATFORM_OS_LINUX && GASNET_CONDUIT_IBV
  // Look for GDR support.
  // Adapted from the GDR checking logic in Open MPI.
  if (access("/sys/kernel/mm/memory_peers/nv_mem/version", F_OK)) {
    // TODO: gracefully fall back to cuMemcpy() "reference implementation",
    // once one is available, rather than failing.
    GASNETI_RETURN_ERRR(BAD_ARG,"GEX_MK_CLASS_CUDA_UVA: kernel lacks GPUDirect RDMA support");
  }
#endif

  // Obtain the primary context for the given device, initializing if needed
  CUcontext ctx;
  CUresult res = cuDevicePrimaryCtxRetain(&ctx, dev);
  if (res == CUDA_ERROR_NOT_INITIALIZED) {
    int initRes = cuInit(0);
    if (initRes == CUDA_SUCCESS) {
      res = cuDevicePrimaryCtxRetain(&ctx, dev);
    } else if (initRes == CUDA_ERROR_NO_DEVICE) {
      GASNETI_RETURN_ERRR(BAD_ARG,"GEX_MK_CLASS_CUDA_UVA: no CUDA devices found");
    } else {
      const char *errorname;
      if (cuGetErrorName(initRes, &errorname)) errorname = "UNKNOWN";
      const char *msg = gasneti_dynsprintf("GEX_MK_CLASS_CUDA_UVA: cuInit() returned %s(%i)", errorname, initRes);
      GASNETI_RETURN_ERRR(BAD_ARG,msg);
    }
  }

  // Failed to obtain the primary context, try to reason out why
  // TODO: explicit diagnosis of more failure cases
  if_pf (res != CUDA_SUCCESS) {
    const char *why = "unknown failure";
    if (res == CUDA_ERROR_INVALID_DEVICE) {
      int dev_count;
      if (cuDeviceGetCount(&dev_count)) {
        why = "cuDeviceGetCount() failed";
      } else if (! dev_count) {
        why = "no CUDA devices found";
      } else {
        why = gasneti_dynsprintf("invalid CUdevice=%i (%d devices found)", dev, dev_count);
      }
    } else {
      const char *errorname;
      if (cuGetErrorName(res, &errorname)) errorname = "UNKNOWN";
      why = gasneti_dynsprintf("cuDevicePrimaryCtxRetain() returned %s(%i)", errorname ,res);
    }
    const char *msg = gasneti_dynsprintf("GEX_MK_CLASS_CUDA_UVA: %s", why);
    GASNETI_RETURN_ERRR(BAD_ARG,msg);
  }

  int isUVA;
  if (cuDeviceGetAttribute(&isUVA, CU_DEVICE_ATTRIBUTE_UNIFIED_ADDRESSING, dev)) {
    GASNETI_RETURN_ERRR(BAD_ARG,"GEX_MK_CLASS_CUDA_UVA: failed to query CUDA device for UVA support");
  }
  if (!isUVA) {
    GASNETI_RETURN_ERRR(BAD_ARG,"GEX_MK_CLASS_CUDA_UVA: passed context for a non-UVA device");
  }

  my_MK_t result = (my_MK_t) gasneti_alloc_mk(client, get_impl(), flags);
  result->dev = dev;
  result->ctx = ctx;

  // TODO: could be a per-device setting?
  // TODO: is '1' the best default?
  result->use_sync_memops = gasneti_getenv_yesno_withdefault("GASNET_USE_CUDA_SYNC_MEMOPS", 1);

  *i_memkind_p = (gasneti_MK_t) result;
  return GASNET_OK;
}

#endif
