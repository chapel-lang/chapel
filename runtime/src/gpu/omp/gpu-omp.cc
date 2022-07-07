#include "chplrt.h"
#include "gpu-cuda-target.h"
#include "dlfcn.h"
#include "chpl-mem.h"
#include "chpl-tasks.h"
#include "chpl-gpu-diags.h"
#include "chpl-linefile-support.h"

#include "omptargetplugin.h"

#include <cuda.h>
#include <cuda_runtime.h>

static void chpl_gpu_cuda_check(int err, const char* file, int line) {
  if(err != CUDA_SUCCESS) {
    const int msg_len = 256;
    char msg[msg_len];

    snprintf(msg, msg_len,
             "%s:%d: Error calling CUDA function: (Code: %d)",
             file, line, err);

    chpl_internal_error(msg);
  }
}

#define CUDA_CALL(call) do {\
  chpl_gpu_cuda_check((int)call, __FILE__, __LINE__);\
} while(0);

typedef int32_t(init_device_fn)(int32_t);
typedef int32_t(num_devices_fn)(void);
typedef void *(mem_alloc_fn)(int32_t, int64_t, void*, int32_t);
typedef void (data_exchange_fn)(int32_t, void*, int32_t, void*, int64_t);
typedef int32_t (data_delete_fn)(int32_t, void*);
typedef int32_t (is_valid_binary_fn)(__tgt_device_image*);
typedef int32_t (run_target_team_region_fn)(int32_t, void*, void**, ptrdiff_t*,
                                            int32_t, int32_t, int32_t, uint64_t);
typedef int32_t (data_submit_fn)(int32_t, void *, void *, int64_t);


typedef struct chpl_gpu_plugin_rtl_s {
  void *handle = NULL;
  init_device_fn *init_device = NULL;
  num_devices_fn *num_devices = NULL;
  mem_alloc_fn *mem_alloc = NULL;
  data_exchange_fn *data_exchange = NULL;
  data_delete_fn *data_delete = NULL;
  is_valid_binary_fn *is_valid_binary = NULL;
  run_target_team_region_fn *run_target_team_region = NULL;
  data_submit_fn *data_submit = NULL;

} chpl_gpu_plugin_rtl_t;

static chpl_gpu_plugin_rtl_t *rtl = NULL;

extern "C" {

void chpl_gpu_init() {
  if (rtl == NULL) {
    rtl = (chpl_gpu_plugin_rtl_t*)chpl_malloc(sizeof(chpl_gpu_plugin_rtl_t));

    rtl->handle = dlopen("/home/engin/code/chapel/versions/1/chapel/third-party/llvm/install/linux64-x86_64/lib/libomptarget.rtl.cuda.so", RTLD_NOW);
    assert(rtl->handle);

    rtl->num_devices = (num_devices_fn*)dlsym(rtl->handle, "__tgt_rtl_number_of_devices");
    assert(rtl->num_devices);
    //printf("%d\n", (*(rtl->num_devices))());

    rtl->mem_alloc = (mem_alloc_fn*)dlsym(rtl->handle, "__tgt_rtl_data_alloc");
    assert(rtl->mem_alloc);

    rtl->init_device = (init_device_fn*)dlsym(rtl->handle, "__tgt_rtl_init_device");
    assert(rtl->init_device);

    rtl->data_exchange = (data_exchange_fn*)dlsym(rtl->handle, "__tgt_rtl_data_exchange");
    assert(rtl->data_exchange);

    rtl->data_delete = (data_delete_fn*)dlsym(rtl->handle, "__tgt_rtl_data_delete");
    assert(rtl->data_delete);

    rtl->is_valid_binary = (is_valid_binary_fn*)dlsym(rtl->handle, "__tgt_rtl_is_valid_binary");
    assert(rtl->is_valid_binary);

    rtl->run_target_team_region = (run_target_team_region_fn*)dlsym(rtl->handle, "__tgt_rtl_run_target_team_region");
    assert(rtl->run_target_team_region);

    rtl->data_submit = (data_submit_fn*)dlsym(rtl->handle, "__tgt_rtl_data_submit");
    assert(rtl->data_submit);

    //int32_t subloc = chpl_task_getRequestedSubloc();
    (*(rtl->init_device))(0);
  }
}

void* chpl_gpu_mem_alloc(size_t size, chpl_mem_descInt_t description,
                         int32_t lineno, int32_t filename) {
  // TODO
  int32_t device_id = chpl_task_getRequestedSubloc();
    //printf("300 %d\n", device_id);
  void* res = (*(rtl->mem_alloc))(device_id, size, NULL, TARGET_ALLOC_SHARED);
  assert(res);
    //printf("400 %p\n", res);
  return res;
}

void* chpl_gpu_mem_realloc(void* memAlloc, size_t size,
                           chpl_mem_descInt_t description,
                           int32_t lineno, int32_t filename) {
  //printf("500\n");
  int32_t device_id = chpl_task_getRequestedSubloc();


  void *res = chpl_gpu_mem_alloc(size, description, lineno, filename);

  rtl->data_exchange(device_id, memAlloc, device_id, res, size);
  rtl->data_delete(device_id, memAlloc);

  return res;
}



bool chpl_gpu_is_device_ptr(void* ptr) {
  // TODO should we drop this function? I don't think libomptarget has a way of
  // doing this
  //chpl_internal_error("gpu is device ptr is not implemented yet");
  // this is all used for assertion, so maybe we can 
  return true;
}

size_t chpl_gpu_get_alloc_size(void* ptr) {
  chpl_internal_error("gpu get alloc _size is not implemented yet");
  return 0;
}

bool chpl_gpu_has_context() {
  chpl_internal_error("gpu has context is not implemented yet");
  return false;
}

typedef struct PretendKernelTy {
  void *function;
  int8_t execution_mode;
  int max_threads_per_block;
} kernel_t;

static void* chpl_gpu_getKernel(const char* fatbinData, const char* kernelName) {
  //chpl_gpu_ensure_context();

  CUmodule    cudaModule;
  CUfunction  function;

  // Create module for object
  CUDA_CALL(cuModuleLoadData(&cudaModule, fatbinData));

  // Get kernel function
  CUDA_CALL(cuModuleGetFunction(&function, cudaModule, kernelName));

  return (void*)function;
}

//void chpl_gpu_launch_kernel_help(int ln,
                                        //int32_t fn,
                                        //const char* fatbinData,
                                        //const char* name,
                                        //int grd_dim_x,
                                        //int grd_dim_y,
                                        //int grd_dim_z,
                                        //int blk_dim_x,
                                        //int blk_dim_y,
                                        //int blk_dim_z,
                                        //int nargs,
                                        //va_list args) {
void chpl_gpu_copy_host_to_device(void* dst, void* src, size_t n) {
  int32_t device_id = chpl_task_getRequestedSubloc();

  rtl->data_submit(device_id, dst, src, n);
}

void chpl_gpu_launch_kernel_help_with_tripcount(int ln,
                                        int32_t fn,
                                        const char* fatbinData,
                                        const char* name,
                                        int nargs,
                                        va_list args,
                                        int num_threads, int blk_dim) {

  //size_t fatbinSize = strlen(fatbinData);
  //size_t fatbinSize = 69096;
  //const char* fatbinEnd = fatbinData + fatbinSize;

  //printf("fatbinSize %zu\n", fatbinSize);

  //__tgt_device_image image;
  //image.ImageStart = (void*)fatbinData;
  //image.ImageEnd = (void*)fatbinEnd;
  //image.EntriesBegin = NULL;
  //image.EntriesEnd = NULL;

  //printf("is valid %d\n", rtl->is_valid_binary(&image));
  //printf("%p %p\n", __start_omp_offloading_entries, __stop_omp_offloading_entries);
  kernel_t kernel;
  kernel.function = chpl_gpu_getKernel(fatbinData, name);
  kernel.execution_mode = 1 << 1;
  kernel.max_threads_per_block = blk_dim;

  int32_t device_id = chpl_task_getRequestedSubloc();

  void** kernel_params = (void**)chpl_malloc(nargs*sizeof(void**));

  int i;
  for (i=0 ; i<nargs ; i++) {
    void* cur_arg = va_arg(args, void*);
    size_t cur_arg_size = va_arg(args, size_t);

    if (cur_arg_size > 0) {
      // TODO this allocation needs to use `chpl_mem_alloc` with a proper desc
      kernel_params[i] = (void*)chpl_malloc(1*sizeof(CUdeviceptr));

      void *offloaded_arg = chpl_gpu_mem_alloc(cur_arg_size,
                                               CHPL_RT_MD_GPU_KERNEL_ARG,
                                               ln, fn);

      kernel_params[i] = offloaded_arg;

      chpl_gpu_copy_host_to_device(offloaded_arg, cur_arg, cur_arg_size);

    }
    else {
      kernel_params[i] = *((void**)cur_arg);
    }
  }

  ptrdiff_t *offsets = (ptrdiff_t*)chpl_calloc(nargs, sizeof(ptrdiff_t));

  chpl_gpu_diags_verbose_launch(ln, fn, chpl_task_getRequestedSubloc());
  chpl_gpu_diags_incr(kernel_launch);

  int32_t ret = rtl->run_target_team_region(device_id, &kernel, kernel_params, offsets, nargs,
                              /*blocks_per_grid*/ -1, blk_dim, num_threads);

  assert(ret == OFFLOAD_SUCCESS);
  //chpl_internal_error("gpu launch kernel help not implemented yet");
}


void* chpl_gpu_mem_calloc(size_t number, size_t size,
                          chpl_mem_descInt_t description,
                          int32_t lineno, int32_t filename) {

  chpl_internal_error("gpu mem calloc not implemented yet");
  return NULL;
}

void* chpl_gpu_mem_memalign(size_t boundary, size_t size,
                            chpl_mem_descInt_t description,
                            int32_t lineno, int32_t filename) {
  chpl_internal_error("gpu memalign not implemented yet");
  return NULL;
}

void chpl_gpu_mem_free(void* memAlloc, int32_t lineno, int32_t filename) {
  int32_t device_id = chpl_task_getRequestedSubloc();
  if (rtl->data_delete(device_id, memAlloc) == OFFLOAD_FAIL) {
    chpl_mem_free(memAlloc, lineno, filename);
  }
}

}  // extern "C"
