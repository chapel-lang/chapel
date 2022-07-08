#include "chplrt.h"
#include "chpl-gpu.h"
#include "chpl-gpu-impl.h"
#include "chpl-gpu-diags.h"
#include "chpl-linefile-support.h"

void chpl_gpu_init(void) {
  chpl_gpu_impl_init();
}

void chpl_gpu_launch_kernel(int ln, int32_t fn,
                            const char* fatbinData, const char* name,
                            int grd_dim_x, int grd_dim_y, int grd_dim_z,
                            int blk_dim_x, int blk_dim_y, int blk_dim_z,
                            int nargs, ...) {
  va_list args;
  va_start(args, nargs);

  chpl_gpu_diags_verbose_launch(ln, fn, chpl_task_getRequestedSubloc());
  chpl_gpu_diags_incr(kernel_launch);

  chpl_gpu_impl_launch_kernel(ln, fn,
                              fatbinData, name,
                              grd_dim_x, grd_dim_y, grd_dim_z,
                              blk_dim_x, blk_dim_y, blk_dim_z,
                              nargs, args);
  va_end(args);
}

void chpl_gpu_launch_kernel_flat(int ln, int32_t fn,
                                 const char* fatbinData, const char* name,
                                 int num_threads, int blk_dim, int nargs, ...) {

  va_list args;
  va_start(args, nargs);

  chpl_gpu_diags_verbose_launch(ln, fn, chpl_task_getRequestedSubloc());
  chpl_gpu_diags_incr(kernel_launch);

  chpl_gpu_impl_launch_kernel_flat(ln, fn,
                                   fatbinData, name,
                                   num_threads, blk_dim,
                                   nargs, args);
  va_end(args);
}

void chpl_gpu_copy_device_to_host(void* dst, void* src, size_t n) {
  assert(chpl_gpu_is_device_ptr(src));

  CHPL_GPU_DEBUG("Copying %zu bytes from device to host\n", n);

  chpl_gpu_impl_copy_device_to_host(dst, src, n);

  CHPL_GPU_DEBUG("Copy successful\n");
}

void chpl_gpu_copy_host_to_device(void* dst, void* src, size_t n) {
  assert(chpl_gpu_is_device_ptr(dst));

  CHPL_GPU_DEBUG("Copying %zu bytes from host to device\n", n);

  chpl_gpu_impl_copy_host_to_device(dst, src, n);

  CHPL_GPU_DEBUG("Copy successful\n");
}

void* chpl_gpu_mem_alloc(size_t size, chpl_mem_descInt_t description,
                         int32_t lineno, int32_t filename) {

  CHPL_GPU_DEBUG("chpl_gpu_mem_alloc called. Size:%d file:%s line:%d\n", size,
               chpl_lookupFilename(filename), lineno);

  void *ptr = NULL;
  if (size > 0) {
    chpl_memhook_malloc_pre(1, size, description, lineno, filename);
    ptr = chpl_gpu_impl_mem_alloc(size);
    chpl_memhook_malloc_post((void*)ptr, 1, size, description, lineno, filename);

    CHPL_GPU_DEBUG("chpl_gpu_mem_alloc returning %p\n", (void*)ptr);
  }
  else {
    CHPL_GPU_DEBUG("chpl_gpu_mem_alloc returning NULL (size was 0)\n");
  }

  return ptr;
}

void chpl_gpu_mem_free(void* memAlloc, int32_t lineno, int32_t filename) {
  CHPL_GPU_DEBUG("chpl_gpu_mem_free is called. Ptr %p\n", memAlloc);

  chpl_memhook_free_pre(memAlloc, 0, lineno, filename);

  chpl_gpu_impl_mem_free(memAlloc);

  CHPL_GPU_DEBUG("chpl_gpu_mem_free is returning\n");
}


void* chpl_gpu_mem_calloc(size_t number, size_t size,
                          chpl_mem_descInt_t description,
                          int32_t lineno, int32_t filename) {
  chpl_internal_error("Zero-allocating GPU memory is not supported yet");
  return NULL;

  /*CHPL_GPU_DEBUG("chpl_gpu_mem_calloc called. Size:%d\n", size);*/

  /*void* ptr = chpl_gpu_impl_mem_calloc(number, size, description, lineno, filename);*/

  /*CHPL_GPU_DEBUG("chpl_gpu_mem_calloc returning %p\n", (void*)ptr);*/

  /*return ptr;*/
}

void* chpl_gpu_mem_realloc(void* memAlloc, size_t size,
                           chpl_mem_descInt_t description,
                           int32_t lineno, int32_t filename) {
  chpl_internal_error("Reallocating GPU memory is not supported yet");

  return NULL;
}

void* chpl_gpu_mem_memalign(size_t boundary, size_t size,
                            chpl_mem_descInt_t description,
                            int32_t lineno, int32_t filename) {
  chpl_internal_error("Allocating aligned GPU memory is not supported yet");

  return NULL;
}
