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
#include "chpl-gpu.h"
#include "chpl-gpu-impl.h"
#include "chpl-tasks.h"
#include "error.h"
#include "chplcgfns.h"

#include <assert.h>

void chpl_gpu_impl_init() {}
void chpl_gpu_impl_on_std_modules_finished_initializing() {}

// TODO:
// HIP include files throw warnings when compiled under our set of
// settings. We compile with -Werror. For the time being, do not
// include HIP directly, and just forward declare the functions
// ourselves.
extern void hipGetDeviceCount(int* into);

void chpl_gpu_get_device_count(int* into) {
  hipGetDeviceCount(into);
}

void chpl_gpu_impl_launch_kernel(int ln, int32_t fn,
                                 const char* fatbinData, const char* name,
                                 int grd_dim_x, int grd_dim_y, int grd_dim_z,
                                 int blk_dim_x, int blk_dim_y, int blk_dim_z,
                                 int nargs, va_list args) {}

void chpl_gpu_impl_launch_kernel_flat(int ln, int32_t fn,
                                      const char* fatbinData, const char* name,
                                      int num_threads, int blk_dim, int nargs,
                                      va_list args) {}

void* chpl_gpu_mem_array_alloc(size_t size, chpl_mem_descInt_t description,
                               int32_t lineno, int32_t filename) {
  return chpl_malloc(size);
}

void* chpl_gpu_impl_mem_alloc(size_t size) {
  return chpl_malloc(size);
}
void chpl_gpu_impl_mem_free(void* memAlloc) {}
void* chpl_gpu_impl_memmove(void* dst, const void* src, size_t n) {
  return memmove(dst, src, n);
}

void chpl_gpu_impl_copy_device_to_host(void* dst, const void* src, size_t n) {}
void chpl_gpu_impl_copy_host_to_device(void* dst, const void* src, size_t n) {}

void chpl_gpu_impl_copy_device_to_device(void* dst, const void* src, size_t n) {}

bool chpl_gpu_impl_is_device_ptr(const void* ptr) {
  return false;
}

bool chpl_gpu_impl_is_host_ptr(const void* ptr) {
  return false;
}

// This can be used for proper reallocation
size_t chpl_gpu_impl_get_alloc_size(void* ptr) {
  return 0;
}


#endif // HAS_GPU_LOCALE

