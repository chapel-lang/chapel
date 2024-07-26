/*
 * Copyright 2020-2024 Hewlett Packard Enterprise Development LP
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

void chpl_gpu_impl_load_global(const char* global_name, void** ptr,
                               size_t* size) {
  *ptr = (void*)1; // we don't want to return NULL here to avoid an assertion
}

bool chpl_gpu_impl_is_device_ptr(const void* ptr) {
  return false;  // this OK? maybe we want assertions to go through?
}

bool chpl_gpu_impl_is_host_ptr(const void* ptr) {
  return true;
}

void* chpl_gpu_impl_load_function(const char* kernel_name) {
  return (void*)1; // we don't want to return NULL here to avoid an assertion
}

void chpl_gpu_impl_launch_kernel(void* kernel,
                                 int grd_dim_x, int grd_dim_y, int grd_dim_z,
                                 int blk_dim_x, int blk_dim_y, int blk_dim_z,
                                 void* stream,
                                 void** kernel_params) {
}

void* chpl_gpu_impl_memset(void* addr, const uint8_t val, size_t n,
                           void* stream) {
  return memset(addr, val, n);
}

void chpl_gpu_impl_copy_device_to_host(void* dst, const void* src, size_t n,
                                       void* stream) {
  chpl_memcpy(dst, src, n);
}

void chpl_gpu_impl_copy_host_to_device(void* dst, const void* src, size_t n,
                                       void* stream) {
  chpl_memcpy(dst, src, n);
}

void chpl_gpu_impl_copy_device_to_device(void* dst, const void* src, size_t n,
                                         void* stream) {
  chpl_memcpy(dst, src, n);
}

void* chpl_gpu_impl_comm_async(void *dst, void *src, size_t n) {
  chpl_memcpy(dst, src, n);
  return NULL;
}

void chpl_gpu_impl_comm_wait(void *stream) {
  assert(stream==NULL);
}

void* chpl_gpu_impl_mem_array_alloc(size_t size) {
  return chpl_malloc(size);
}


void* chpl_gpu_impl_mem_alloc(size_t size) {
  return chpl_malloc(size);
}

void chpl_gpu_impl_mem_free(void* memAlloc) {
  chpl_free(memAlloc);
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

bool chpl_gpu_impl_can_access_peer(int dev1, int dev2) {
  return false;
}

void chpl_gpu_impl_set_peer_access(int dev1, int dev2, bool enable) {
}

void chpl_gpu_impl_use_device(c_sublocid_t dev_id) {
}

void chpl_gpu_impl_synchronize(void) {
}

bool chpl_gpu_impl_stream_supported(void) {
  return false;
}

void* chpl_gpu_impl_stream_create(void) {
  return NULL;
}

void chpl_gpu_impl_stream_destroy(void* stream) {
}

bool chpl_gpu_impl_stream_ready(void* stream) {
  return true;
}

void chpl_gpu_impl_stream_synchronize(void* stream) {
}

bool chpl_gpu_impl_can_reduce(void) {
  return false;
}

bool chpl_gpu_impl_can_sort(void){
  return false;
}

#define DEF_ONE_REDUCE_RET_VAL(impl_kind, chpl_kind, data_type) \
void chpl_gpu_impl_##chpl_kind##_reduce_##data_type(data_type* data, int n,\
                                                    data_type* val, int* idx,\
                                                    void* stream) {\
  chpl_internal_error("This function shouldn't have been called. "\
                      "cpu-as-device mode should handle reductions in "\
                      "the module code\n");\
}

GPU_DEV_CUB_WRAP(DEF_ONE_REDUCE_RET_VAL, Sum, sum)
GPU_DEV_CUB_WRAP(DEF_ONE_REDUCE_RET_VAL, Min, min)
GPU_DEV_CUB_WRAP(DEF_ONE_REDUCE_RET_VAL, Max, max)

#undef DEF_ONE_REDUCE_RET_VAL

#define DEF_ONE_REDUCE_RET_VAL_IDX(cub_kind, chpl_kind, data_type) \
void chpl_gpu_impl_##chpl_kind##_reduce_##data_type(data_type* data, int n,\
                                                    data_type* val, int* idx,\
                                                    void* stream) {\
  chpl_internal_error("This function shouldn't have been called. "\
                      "cpu-as-device mode should handle reductions in "\
                      "the module code\n");\
}

GPU_DEV_CUB_WRAP(DEF_ONE_REDUCE_RET_VAL_IDX, ArgMin, minloc)
GPU_DEV_CUB_WRAP(DEF_ONE_REDUCE_RET_VAL_IDX, ArgMax, maxloc)

#undef DEF_ONE_REDUCE_RET_VAL_IDX

#define DEF_ONE_SORT(cub_kind, chpl_kind, data_type) \
void chpl_gpu_impl_sort_##chpl_kind##_##data_type(data_type* data_in, \
                                                  data_type* data_out, \
                                                  int n, void* stream) {\
  chpl_internal_error("This function shouldn't have been called. "\
                      "cpu-as-device mode should handle sorting in "\
                      "the module code\n");\
}

GPU_DEV_CUB_WRAP(DEF_ONE_SORT, SortKeys, keys)

void* chpl_gpu_impl_host_register(void* var, size_t size) { return var; }
void chpl_gpu_impl_host_unregister(void* var) { }

#undef DEF_ONE_SORT
#endif // HAS_GPU_LOCALE
