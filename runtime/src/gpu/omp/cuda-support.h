#include <cuda.h>
#include <cuda_runtime.h>

static void chpl_gpu_cuda_check_impl(int err, const char* file, int line) {
  if(err != CUDA_SUCCESS) {
    const int msg_len = 256;
    char msg[msg_len];

    snprintf(msg, msg_len,
             "%s:%d: Error calling CUDA function: (Code: %d)",
             file, line, err);

    chpl_internal_error(msg);
  }
}

#define CUDA_CALL_IMPL(call) do {\
  chpl_gpu_cuda_check_impl((int)call, __FILE__, __LINE__);\
} while(0);

// this is an internal helper only

static inline
void* chpl_gpu_getKernel(const char* fatbinData, const char* kernelName) {
  //chpl_gpu_ensure_context();

  CUmodule    cudaModule;
  CUfunction  function;

  // Create module for object
  CUDA_CALL_IMPL(cuModuleLoadData(&cudaModule, fatbinData));

  // Get kernel function
  CUDA_CALL_IMPL(cuModuleGetFunction(&function, cudaModule, kernelName));

  return (void*)function;
}

// this is part of the interface (used by the module code as an extern)
static inline
bool chpl_gpu_common_is_device_ptr(void* ptr) {
  //chpl_gpu_ensure_context();

  unsigned int res;

  // We call CUDA_CALL later, because we want to treat some error codes
  // separately
  CUresult ret_val = cuPointerGetAttribute(&res, CU_POINTER_ATTRIBUTE_MEMORY_TYPE,
                                           (CUdeviceptr)ptr);

  if (ret_val == CUDA_SUCCESS) {
    return res == CU_MEMORYTYPE_DEVICE || res == CU_MEMORYTYPE_UNIFIED;
  }
  else if (ret_val == CUDA_ERROR_INVALID_VALUE ||
           ret_val == CUDA_ERROR_NOT_INITIALIZED ||
           ret_val == CUDA_ERROR_DEINITIALIZED) {
    return false;  // this is a cpu pointer that CUDA doesn't even know about
  }

  // there must have been an error in calling the cuda function. report that.
  CUDA_CALL_IMPL(ret_val);

  return false;
}
//
// This can be used for proper reallocation
static size_t chpl_gpu_common_get_alloc_size(void* ptr) {
  CUdeviceptr base;
  size_t size;
  CUDA_CALL_IMPL(cuMemGetAddressRange(&base, &size, (CUdeviceptr)ptr));

  return size;
}

