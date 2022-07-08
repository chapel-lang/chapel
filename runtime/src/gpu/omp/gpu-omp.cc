#include "chplrt.h"
#include "gpu-cuda-target.h"
#include "dlfcn.h"
#include "chpl-mem.h"
#include "chpl-tasks.h"
#include "chpl-gpu-diags.h"
#include "chpl-linefile-support.h"
#include "cuda-support.h"
#include "chpl-gpu.h"
#include "chpl-gpu-impl.h"

#include "omptargetplugin.h"

typedef int32_t(init_device_fn)(int32_t);
typedef int32_t(num_devices_fn)(void);
typedef void *(mem_alloc_fn)(int32_t, int64_t, void*, int32_t);
typedef void (data_exchange_fn)(int32_t, void*, int32_t, void*, int64_t);
typedef int32_t (data_delete_fn)(int32_t, void*);
typedef int32_t (is_valid_binary_fn)(__tgt_device_image*);
typedef int32_t (run_target_team_region_fn)(int32_t, void*, void**, ptrdiff_t*,
                                            int32_t, int32_t, int32_t, uint64_t);
typedef int32_t (data_submit_fn)(int32_t, void *, void *, int64_t);
typedef int32_t (data_retrieve_fn)(int32_t, void *, void *, int64_t);


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
  data_retrieve_fn *data_retrieve = NULL;

} chpl_gpu_plugin_rtl_t;

static chpl_gpu_plugin_rtl_t *rtl = NULL;

extern "C" {

void chpl_gpu_impl_init() {
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

    rtl->data_retrieve = (data_retrieve_fn*)dlsym(rtl->handle, "__tgt_rtl_data_retrieve");
    assert(rtl->data_retrieve);

    // TODO
    (*(rtl->init_device))(0);


    // TODO can we stick device_id into something task-private? We keep asking
    // for that in every function
  }
}

void* chpl_gpu_impl_mem_alloc(size_t size) {
  int32_t device_id = chpl_task_getRequestedSubloc();
  void* res = (*(rtl->mem_alloc))(device_id, size, NULL, TARGET_ALLOC_SHARED);
  assert(res);

  return res;
}

void chpl_gpu_impl_mem_free(void* memAlloc) {
  int32_t device_id = chpl_task_getRequestedSubloc();
  rtl->data_delete(device_id, memAlloc);
}

void chpl_gpu_impl_copy_device_to_host(void* dst, void* src, size_t n) {
  int32_t device_id = chpl_task_getRequestedSubloc();
  rtl->data_retrieve(device_id, dst, src, n);
}

void chpl_gpu_impl_copy_host_to_device(void* dst, void* src, size_t n) {
  int32_t device_id = chpl_task_getRequestedSubloc();
  rtl->data_submit(device_id, dst, src, n);
}

void chpl_gpu_impl_copy_device_to_device(void* dst, void* src, size_t n) {
  int32_t device_id = chpl_task_getRequestedSubloc();

  // omptarget hhas the order swapped here, oops.
  rtl->data_exchange(device_id, src, device_id, dst, n);
}

bool chpl_gpu_is_device_ptr(void* ptr) {
  return chpl_gpu_common_is_device_ptr(ptr);
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
static void chpl_gpu_impl_launch_kernel_help_with_tripcount(int ln,
                                                            int32_t fn,
                                                            const char* fatbinData,
                                                            const char* name,
                                                            int nargs,
                                                            va_list args,
                                                            int num_threads,
                                                            int blk_dim) {

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


  int32_t ret = rtl->run_target_team_region(device_id, &kernel, kernel_params, offsets, nargs,
                              /*blocks_per_grid*/ -1, blk_dim, num_threads);

  assert(ret == OFFLOAD_SUCCESS);
  //chpl_internal_error("gpu launch kernel help not implemented yet");
}

void chpl_gpu_impl_launch_kernel(int ln, int32_t fn,
                                 const char* fatbinData, const char* name,
                                 int grd_dim_x, int grd_dim_y, int grd_dim_z,
                                 int blk_dim_x, int blk_dim_y, int blk_dim_z,
                                 int nargs, va_list args) {
  chpl_internal_error("GPU kernels with custom shapes can't be launched with the 'omp' GPU layer");
}

void chpl_gpu_impl_launch_kernel_flat(int ln, int32_t fn,
                                      const char* fatbinData, const char* name,
                                      int num_threads, int blk_dim,
                                      int nargs, va_list args) {

  // TODO get rid of this function
  chpl_gpu_impl_launch_kernel_help_with_tripcount(ln, fn,
                                                  fatbinData, name,
                                                  nargs, args, num_threads, blk_dim);
}

size_t chpl_gpu_impl_get_alloc_size(void* ptr) {
  return chpl_gpu_common_get_alloc_size(ptr);
}



}  // extern "C"
