#include "chplrt.h"
#include "dlfcn.h"
#include "chpl-mem.h"
#include "chpl-tasks.h"
#include "chpl-gpu-diags.h"
#include "chpl-linefile-support.h"
#include "cuda-support.h"
#include "chpl-gpu.h"
#include "chpl-gpu-impl.h"

#include "omptargetplugin.h"

// I stole this macro trick from chpl-comm-diags.h
// For any `__tgt_rtl_X` add one `MACRO` call here.
#define OMP_FN_ALL(MACRO) \
  MACRO(init_device) \
  MACRO(number_of_devices) \
  MACRO(data_alloc) \
  MACRO(data_exchange) \
  MACRO(data_delete) \
  MACRO(is_valid_binary)\
  MACRO(run_target_team_region) \
  MACRO(data_submit) \
  MACRO(data_retrieve)


// For any `__tgt_rtl_X` add one function typedef here
typedef int32_t(init_device_fn)(int32_t);
typedef int32_t(number_of_devices_fn)(void);
typedef void *(data_alloc_fn)(int32_t, int64_t, void*, int32_t);
typedef void  (data_exchange_fn)(int32_t, void*, int32_t, void*, int64_t);
typedef int32_t (data_delete_fn)(int32_t, void*);
typedef int32_t (is_valid_binary_fn)(__tgt_device_image*);
typedef int32_t (run_target_team_region_fn)(int32_t, void*, void**, ptrdiff_t*,
                                            int32_t, int32_t, int32_t, uint64_t);
typedef int32_t (data_submit_fn)(int32_t, void *, void *, int64_t);
typedef int32_t (data_retrieve_fn)(int32_t, void *, void *, int64_t);


typedef struct chpl_gpu_plugin_rtl_s {
  void *handle = NULL;
#define _OMP_FN_DECL(arg) arg##_fn *arg;
  OMP_FN_ALL(_OMP_FN_DECL);
#undef _OMP_FN_DECL
} chpl_gpu_plugin_rtl_t;

static chpl_gpu_plugin_rtl_t *rtl = NULL;

extern "C" {

void chpl_gpu_impl_init() {
  if (rtl == NULL) {
    rtl = (chpl_gpu_plugin_rtl_t*)chpl_malloc(sizeof(chpl_gpu_plugin_rtl_t));

    rtl->handle = dlopen("/home/engin/code/chapel/versions/1/chapel/third-party/llvm/install/linux64-x86_64/lib/libomptarget.rtl.cuda.so", RTLD_NOW);
    assert(rtl->handle);
#define _OMP_FN_LOAD(arg) \
    do { \
      (*rtl).arg = (arg##_fn*)dlsym(rtl->handle, "__tgt_rtl_" #arg);\
      assert((*rtl).arg);\
    } while(0);

    OMP_FN_ALL(_OMP_FN_LOAD);
#undef _OMP_FN_LOAD

    const int32_t num_devices = rtl->number_of_devices();
    for (int i=0; i<num_devices ; i++) {
      rtl->init_device(i);
    }

    // TODO can we stick device_id into something task-private? We keep asking
    // for that in every function
  }
}

void* chpl_gpu_impl_mem_alloc(size_t size) {
  int32_t device_id = chpl_task_getRequestedSubloc();
  void* res = (*(rtl->data_alloc))(device_id, size, NULL, TARGET_ALLOC_SHARED);
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

bool chpl_gpu_impl_is_device_ptr(void* ptr) {
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

  kernel_t kernel;
  kernel.function = chpl_gpu_getKernel(fatbinData, name);
  kernel.execution_mode = 1 << 1;
  kernel.max_threads_per_block = blk_dim;

  int32_t device_id = chpl_task_getRequestedSubloc();

  CHPL_GPU_DEBUG("Creating kernel parameters\n");

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

      CHPL_GPU_DEBUG("\tKernel parameter %d: %p (device ptr)\n",
                   i, kernel_params[i]);
    }
    else {
      kernel_params[i] = *((void**)cur_arg);
      CHPL_GPU_DEBUG("\tKernel parameter %d: %p\n",
                   i, kernel_params[i]);
    }
  }

  ptrdiff_t *offsets = (ptrdiff_t*)chpl_calloc(nargs, sizeof(ptrdiff_t));


  int32_t ret = rtl->run_target_team_region(device_id, &kernel,
                                            kernel_params, offsets, nargs,
                                            /*blocks_per_grid*/ -1,
                                            blk_dim, num_threads);

  CHPL_GPU_DEBUG("run_target_team_region returned %s\n", name);
  assert(ret == OFFLOAD_SUCCESS);

  chpl_free(offsets);
  chpl_free(kernel_params);
}

size_t chpl_gpu_impl_get_alloc_size(void* ptr) {
  return chpl_gpu_common_get_alloc_size(ptr);
}
}  // extern "C"
