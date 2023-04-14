/*
 * Copyright 2020-2023 Hewlett Packard Enterprise Development LP
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

#ifdef HAS_GPU_LOCALE

#include "sys_basic.h"
#include "chplrt.h"
#include "chpl-mem.h"
#include "chpl-mem-array.h"
#include "chpl-gpu.h"
#include "chpl-gpu-impl.h"
#include "chpl-linefile-support.h"
#include "chpl-tasks.h"
#include "error.h"
#include "chplcgfns.h"

#include <assert.h>
#include <stdbool.h>


void chpl_gpu_impl_init(int* num_devices) {
  CHPL_GPU_DEBUG("Initializing none GPU layer.\n");
  *num_devices = 1;
}

static bool chpl_gpu_device_alloc = false;

void chpl_gpu_impl_on_std_modules_finished_initializing(void) {
  // The standard module has some memory that we allocate when we  are "on" a
  // GPU sublocale when in fact we want to allocate it on the device. (As of
  // the writing of this comment this is in `helpSetupLocaleGPU` in
  // `LocaleModelHelpSetup`).
  //
  // Basically during the setup of the locale model we need to be "on" a given
  // sublocale when we instantiate the object for it (the expectation is that
  // the wide pointer for a sublocale appears to be on that sublocale),
  // but in practice we don't actually want the data for the GPU sublocale
  // object to be on the GPU).
  //
  // It's a bit of a hack but to handle this we start off setting
  // `chpl_gpu_device_alloc` to false indicating that we shouldn't actually
  // do any allocations on the device. Once the standard modules have finished
  // loading this callback function
  // (`chpl_gpu_impl_on_std_modules_finished_initializing`) gets called and we
  // flip the flag.
  chpl_gpu_device_alloc = true;
}

bool chpl_gpu_impl_is_device_ptr(const void* ptr) {
  return false;  // this OK? maybe we want assertions to go through?
}

bool chpl_gpu_impl_is_host_ptr(const void* ptr) {
  return true;
}

inline void chpl_gpu_impl_launch_kernel(int ln, int32_t fn,
                                        const char* name,
                                        int grd_dim_x,
                                        int grd_dim_y,
                                        int grd_dim_z,
                                        int blk_dim_x,
                                        int blk_dim_y,
                                        int blk_dim_z,
                                        int nargs, va_list args) {
}

inline void chpl_gpu_impl_launch_kernel_flat(int ln, int32_t fn,
                                             const char* name,
                                             int num_threads,
                                             int blk_dim,
                                             int nargs,
                                             va_list args) {
}

void* chpl_gpu_impl_memmove(void* dst, const void* src, size_t n) {
  return chpl_memmove(dst, src, n);
}

void* chpl_gpu_impl_memset(void* addr, const uint8_t val, size_t n) {
  return memset(addr, val, n);
}

void chpl_gpu_impl_copy_device_to_host(void* dst, const void* src, size_t n) {
  chpl_memcpy(dst, src, n);
}

void chpl_gpu_impl_copy_host_to_device(void* dst, const void* src, size_t n) {
  chpl_memcpy(dst, src, n);
}

void chpl_gpu_impl_copy_device_to_device(void* dst, const void* src, size_t n) {
  chpl_memcpy(dst, src, n);
}

void* chpl_gpu_impl_comm_async(void *dst, void *src, size_t n) {
  chpl_memcpy(dst, src, n);
  return NULL;
}

void chpl_gpu_impl_comm_wait(void *stream) {
  assert(stream==NULL);
}

void* chpl_gpu_mem_array_alloc(size_t size, chpl_mem_descInt_t description,
                               int32_t lineno, int32_t filename) {
  // this function's upstream is blocked by GPU_RUNTIME_NONE check, This should
  // be unreachable
  chpl_internal_error("chpl_gpu_mem_array_alloc was called unexpectedly.");
  return chpl_malloc(size);
}


void* chpl_gpu_impl_mem_alloc(size_t size) {
  return chpl_mem_alloc(size, 0, 0, 0);
}

void chpl_gpu_impl_mem_free(void* memAlloc) {
  chpl_mem_free(0, 0, 0);
}

void chpl_gpu_impl_hostmem_register(void *memAlloc, size_t size) {

}

// This can be used for proper reallocation
size_t chpl_gpu_impl_get_alloc_size(void* ptr) {
  return -1;
}

unsigned int chpl_gpu_device_clock_rate(int32_t devNum) {
  return -1;
}

#endif // HAS_GPU_LOCALE
