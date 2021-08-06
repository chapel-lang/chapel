/*
 * Copyright 2020-2021 Hewlett Packard Enterprise Development LP
 * Copyright 2004-2019 Cray Inc.
 * Other additional copyright holders may be indicated within.  * 
 * The entirety of this work is licensed under the Apache License,
 * Version 2.0 (the "License"); you may not use this file except
 * in compliance with the License.
 * 
 * You may obtain a copy of the License at
 * 
 *     http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "sys_basic.h"
#include "chplrt.h"
#include "chpl-mem.h"
#include "chpl-gpu.h"
#include "error.h"

#ifdef HAS_GPU_LOCALE

#include <cuda.h>
#include <cuda_runtime.h>
#include <assert.h>

 #define CHPL_GPU_DEBUG  // TODO: adjust Makefile for this

static void CHPL_GPU_LOG(const char *str, ...) {
#ifdef CHPL_GPU_DEBUG
  va_list args;
  va_start(args, str);
  vfprintf(stdout, str, args);
  va_end(args);
  fflush(stdout);
#endif
}

static void chpl_gpu_cuda_check(int err, const char* file, int line) {
  if(err != CUDA_SUCCESS) {
    const int msg_len = 80;
    char msg[msg_len];

    snprintf(msg, msg_len,
             "%s:%d: Error calling CUDA function. (Code: %d)",
             file, line, err);
    
    chpl_internal_error(msg);
  }
}

#define CUDA_CALL(call) do {\
  chpl_gpu_cuda_check((int)call, __FILE__, __LINE__);\
} while(0);

void chpl_gpu_init() {
  CUdevice    device;
  CUcontext   context;
  int         devCount;

  CHPL_GPU_LOG("Initializing GPU\n");

  // CUDA initialization
  CUDA_CALL(cuInit(0));

  CUDA_CALL(cuDeviceGetCount(&devCount));

  CUDA_CALL(cuDeviceGet(&device, 0));

  // Create driver context
  CUDA_CALL(cuCtxCreate(&context, 0, device));
  CUDA_CALL(cuDevicePrimaryCtxRetain(&context, device));

  CUcontext cuda_context = NULL;
  cuCtxGetCurrent(&cuda_context);
  if (cuda_context == NULL) {
    CHPL_GPU_LOG("No context for init\n");
  }
}

static void* chpl_gpu_getKernel(const char* fatbinFile, const char* kernelName) {
  CUmodule    cudaModule;
  CUfunction  function;

  //read in fatbin and store in buffer
  char * buffer = 0;
  long length;
  FILE * f = fopen (fatbinFile, "rb");

  if (f)
  {
    fseek (f, 0, SEEK_END);
    length = ftell (f);
    fseek (f, 0, SEEK_SET);
    buffer = (char* )chpl_malloc (length);
    if (buffer)
    {
      fread (buffer, 1, length, f);
    }
    fclose (f);
  }

  // Create module for object
  CUDA_CALL(cuModuleLoadData(&cudaModule, buffer));

  // Get kernel function
  CUDA_CALL(cuModuleGetFunction(&function, cudaModule, kernelName));

  return (void*)function;
}

static void chpl_gpu_check_device_ptr(void* ptr) {
  /*CUdeviceptr base;*/
  /*size_t size;*/
  /*CUDA_CALL(cuMemGetAddressRange(&base, &size, ((CUdeviceptr)ptr)));*/

  /*assert(base);*/
  /*assert(size > 0);*/
  assert(chpl_gpu_is_device_ptr(ptr));
}

bool chpl_gpu_is_device_ptr(void* ptr) {
  unsigned int res;
  
  // We call CUDA_CALL later, because we want to treat some error codes
  // separately
  CUresult ret_val = cuPointerGetAttribute(&res, CU_POINTER_ATTRIBUTE_MEMORY_TYPE,
                                           (CUdeviceptr)ptr);

  if (ret_val == CUDA_SUCCESS) {
    return res == CU_MEMORYTYPE_DEVICE;
  }
  else if (ret_val == CUDA_ERROR_INVALID_VALUE ||
           ret_val == CUDA_ERROR_NOT_INITIALIZED ||
           ret_val == CUDA_ERROR_DEINITIALIZED) {
    return false;  // this is a cpu pointer that CUDA doesn't even know about
  }

  // there must be an error in calling the cuda function. report that.
  CUDA_CALL(ret_val);

  return false;
}

size_t chpl_gpu_get_alloc_size(void* ptr) {
  CUdeviceptr base;
  size_t size;
  CUDA_CALL(cuMemGetAddressRange(&base, &size, (CUdeviceptr)ptr));

  return size;
}

static void chpl_gpu_launch_kernel_help(const char* name,
                                        int grd_dim_x,
                                        int grd_dim_y,
                                        int grd_dim_z,
                                        int blk_dim_x,
                                        int blk_dim_y,
                                        int blk_dim_z,
                                        int nargs,
                                        va_list args) {
  int i;
  void* function = chpl_gpu_getKernel("tmp/chpl__gpu.fatbin", name);
  // TODO: this should use chpl_mem_alloc
  void** kernel_params = chpl_malloc(nargs*sizeof(void*));

  assert(function);
  assert(kernel_params);

  CHPL_GPU_LOG("Kernel launcher called.\
               \n\tKernel: %s\n\tGrid: %d,%d,%d\n\t\
               Block: %d,%d,%d\n\tNumArgs: %d\n",
               name,
               grd_dim_x, grd_dim_y, grd_dim_z,
               blk_dim_x, blk_dim_y, blk_dim_z,
               nargs);

  CHPL_GPU_LOG("Creating kernel parameters\n");

  for (i=0 ; i<nargs ; i++) {
    kernel_params[i] = va_arg(args, void*);

    // TODO: we can remove this check after some point, or enable only if some
    // advanced debugging is enabled.
    chpl_gpu_check_device_ptr(*((void**)kernel_params[i]));

    CHPL_GPU_LOG("\tKernel parameter %d: %p\n", i, kernel_params[i]);
  }

  CHPL_GPU_LOG("Calling gpu function named %s\n", name);

  CUDA_CALL(cuLaunchKernel((CUfunction)function,
                           grd_dim_x, grd_dim_y, grd_dim_z,
                           blk_dim_x, blk_dim_y, blk_dim_z,
                           0,  // shared memory in bytes
                           0,  // stream ID
                           kernel_params,
                           NULL));  // extra options

  CHPL_GPU_LOG("Call returned %s\n", name);
  
  CUDA_CALL(cudaDeviceSynchronize());

  CHPL_GPU_LOG("Synchronization complete %s\n", name);

  // TODO: this should use chpl_mem_free
  chpl_free(kernel_params);
}

void chpl_gpu_copy_device_to_host(void* dst, void* src, size_t n) {
  chpl_gpu_check_device_ptr(src);

  CHPL_GPU_LOG("Copying %zu bytes from device to host\n", n);

  CUDA_CALL(cuMemcpyDtoH(dst, (CUdeviceptr)src, n));
}

void chpl_gpu_copy_host_to_device(void* dst, void* src, size_t n) {
  chpl_gpu_check_device_ptr(dst);

  CHPL_GPU_LOG("Copying %zu bytes from host to device\n", n);

  CUDA_CALL(cuMemcpyHtoD((CUdeviceptr)dst, src, n));
}

void chpl_gpu_launch_kernel(const char* name,
                            int grd_dim_x, int grd_dim_y, int grd_dim_z,
                            int blk_dim_x, int blk_dim_y, int blk_dim_z,
                            int nargs, ...) {
  va_list args;
  va_start(args, nargs);
  chpl_gpu_launch_kernel_help(name,
                              grd_dim_x, grd_dim_y, grd_dim_z,
                              blk_dim_x, blk_dim_y, blk_dim_z,
                              nargs, args);
  va_end(args);
}

void chpl_gpu_launch_kernel_flat(const char* name, int grd_dim, int blk_dim,
                                 int nargs, ...) {
  va_list args;
  va_start(args, nargs);
  chpl_gpu_launch_kernel_help(name,
                              grd_dim, 1, 1,
                              blk_dim, 1, 1,
                              nargs, args);
  va_end(args);
}

bool chpl_gpu_has_context() {
  CUcontext cuda_context = NULL;

  CUresult ret = cuCtxGetCurrent(&cuda_context);

  if (ret == CUDA_ERROR_NOT_INITIALIZED || ret == CUDA_ERROR_DEINITIALIZED) {
    return false;
  }
  else {
    return cuda_context != NULL;
  }
}

void* chpl_gpu_mem_alloc(size_t size, chpl_mem_descInt_t description,
                         int32_t lineno, int32_t filename) {
  CHPL_GPU_LOG("chpl_gpu_mem_alloc called. Size:%d file:%d line:%d\n", size,
               filename, lineno);

  CUdeviceptr ptr;
  CUDA_CALL(cuMemAlloc(&ptr,  size));
  return (void*)ptr;
}

void* chpl_gpu_mem_calloc(size_t number, size_t size,
                          chpl_mem_descInt_t description,
                          int32_t lineno, int32_t filename) {
  CHPL_GPU_LOG("chpl_gpu_mem_calloc called. Size:%d\n", size);

  CUdeviceptr ptr;
  CUDA_CALL(cuMemAlloc(&ptr,  size));
  CUDA_CALL(cuMemsetD8(ptr, 0, size));
  return (void*)ptr;
}

void* chpl_gpu_mem_realloc(void* memAlloc, size_t size,
                           chpl_mem_descInt_t description,
                           int32_t lineno, int32_t filename) {
  CHPL_GPU_LOG("chpl_gpu_mem_realloc called. Size:%d\n", size);

  chpl_gpu_check_device_ptr(memAlloc);

  size_t cur_size = chpl_gpu_get_alloc_size(memAlloc);

  if (size == cur_size) {
    return memAlloc;
  }

  // TODO we could probably do something smarter, especially for the case where
  // the new allocation size is smaller than the original allocation size.

  CUdeviceptr ptr;
  CUDA_CALL(cuMemAlloc(&ptr,  size));
  CUDA_CALL(cuMemcpyDtoD(ptr, (CUdeviceptr)memAlloc, cur_size));
  CUDA_CALL(cuMemFree((CUdeviceptr)memAlloc));

  return (void*)ptr;
}

void* chpl_gpu_mem_memalign(size_t boundary, size_t size,
                            chpl_mem_descInt_t description,
                            int32_t lineno, int32_t filename) {
  CHPL_GPU_LOG("chpl_gpu_mem_memalign called. Size:%d\n", size);
  chpl_internal_error("Not ready to allocate aligned memory on GPU, yet.");

  return NULL;
}

void chpl_gpu_mem_free(void* memAlloc, int32_t lineno, int32_t filename) {
  CHPL_GPU_LOG("chpl_gpu_mem_free called.\n");

  chpl_gpu_check_device_ptr(memAlloc);

  CUDA_CALL(cuMemFree((CUdeviceptr)memAlloc));
}

#endif // HAS_GPU_LOCALE

