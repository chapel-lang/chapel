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

#ifndef chpl_gpu_impl_h
#define chpl_gpu_impl_h

#include "chpl-topo.h"

#ifdef __cplusplus
extern "C" {
#endif

void chpl_gpu_impl_begin_init(int* num_all_devices);

void chpl_gpu_impl_collect_topo_addr_info(chpl_topo_pci_addr_t* into,
                                          int device_num);

void chpl_gpu_impl_setup_with_device_count(int num_my_devices);

void chpl_gpu_impl_setup_device(int my_index, int global_index);

void* chpl_gpu_impl_load_function(const char* kernel_name);
void chpl_gpu_impl_load_global(const char* global_name, void** ptr,
                               size_t* size);
void chpl_gpu_impl_launch_kernel(void* kernel,
                                 int grd_dim_x, int grd_dim_y, int grd_dim_z,
                                 int blk_dim_x, int blk_dim_y, int blk_dim_z,
                                 void* stream,
                                 void** kernel_params);

void* chpl_gpu_impl_mem_alloc(size_t size);
void* chpl_gpu_impl_mem_array_alloc(size_t size);
void chpl_gpu_impl_mem_free(void* memAlloc);
void* chpl_gpu_impl_memset(void* addr, const uint8_t val, size_t n,
                           void* stream);
void chpl_gpu_impl_hostmem_register(void *memAlloc, size_t size);

void chpl_gpu_impl_copy_device_to_host(void* dst, const void* src, size_t n,
                                       void* stream);
void chpl_gpu_impl_copy_host_to_device(void* dst, const void* src, size_t n,
                                       void* stream);
void chpl_gpu_impl_copy_device_to_device(void* dst, const void* src, size_t n,
                                         void* stream);

void* chpl_gpu_impl_comm_async(void *dst, void *src, size_t n);
void chpl_gpu_impl_comm_wait(void *stream);

// module code uses this to pick the right deallocator for a pointer
bool chpl_gpu_impl_is_device_ptr(const void* ptr);

bool chpl_gpu_impl_is_host_ptr(const void* ptr);

// TODO do we really need to expose this?
size_t chpl_gpu_impl_get_alloc_size(void* ptr);

bool chpl_gpu_impl_can_access_peer(int dev1, int dev2);
void chpl_gpu_impl_set_peer_access(int dev1, int dev2, bool enable);

void chpl_gpu_impl_use_device(c_sublocid_t dev_id);

void chpl_gpu_impl_synchronize(void);
bool chpl_gpu_impl_stream_supported(void);
void* chpl_gpu_impl_stream_create(void);
void chpl_gpu_impl_stream_destroy(void* stream);
bool chpl_gpu_impl_stream_ready(void* stream);
void chpl_gpu_impl_stream_synchronize(void* stream);

void* chpl_gpu_impl_host_register(void* var, size_t size);
void chpl_gpu_impl_host_unregister(void* var);

#define DECL_ONE_REDUCE_IMPL(chpl_kind, data_type) \
void chpl_gpu_impl_##chpl_kind##_reduce_##data_type(data_type* data, int n,\
                                                    data_type* val, int* idx,\
                                                    void* stream);
GPU_CUB_WRAP(DECL_ONE_REDUCE_IMPL, sum)
GPU_CUB_WRAP(DECL_ONE_REDUCE_IMPL, min)
GPU_CUB_WRAP(DECL_ONE_REDUCE_IMPL, max)
GPU_CUB_WRAP(DECL_ONE_REDUCE_IMPL, minloc)
GPU_CUB_WRAP(DECL_ONE_REDUCE_IMPL, maxloc)

#undef DECL_ONE_REDUCE_IMPL

#define DECL_ONE_SORT_IMPL(chpl_kind, data_type) \
void chpl_gpu_impl_sort_##chpl_kind##_##data_type(data_type* data_in, \
                                                  data_type* data_out, \
                                                  int n, void* stream);
GPU_CUB_WRAP(DECL_ONE_SORT_IMPL, keys)
// TODO: GPU_SORT(DECL_ONE_SORT_IMPL, keysDesc/ DoubleBuffer/Pairs etc)

#undef DECL_ONE_SORT_IMPL

void chpl_gpu_impl_name(int dev, char *resultBuffer, int bufferSize);

int chpl_gpu_impl_query_attribute(int dev, int attribute);

#ifdef __cplusplus
}
#endif

#endif
