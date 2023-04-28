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

#ifndef chpl_gpu_impl_h
#define chpl_gpu_impl_h


#ifdef __cplusplus
extern "C" {
#endif

void chpl_gpu_impl_init(void);
void chpl_gpu_impl_on_std_modules_finished_initializing(void);

void chpl_gpu_impl_launch_kernel(int ln, int32_t fn,
                                 const char* name,
                                 int grd_dim_x, int grd_dim_y, int grd_dim_z,
                                 int blk_dim_x, int blk_dim_y, int blk_dim_z,
                                 int nargs, va_list args);
void chpl_gpu_impl_launch_kernel_flat(int ln, int32_t fn,
                                 const char* name,
                                 int num_threads, int blk_dim,
                                 int nargs, va_list args);

void* chpl_gpu_impl_mem_alloc(size_t size);
void chpl_gpu_impl_mem_free(void* memAlloc);
void* chpl_gpu_impl_memmove(void* dst, const void* src, size_t n);
void* chpl_gpu_impl_memset(void* addr, const uint8_t val, size_t n);
void chpl_gpu_impl_hostmem_register(void *memAlloc, size_t size);

void chpl_gpu_impl_copy_device_to_host(void* dst, const void* src, size_t n);
void chpl_gpu_impl_copy_host_to_device(void* dst, const void* src, size_t n);

// this is all about copying within the same device that is on this subloc
void chpl_gpu_impl_copy_device_to_device(void* dst, const void* src, size_t n);

void* chpl_gpu_impl_comm_async(void *dst, void *src, size_t n);
void chpl_gpu_impl_comm_wait(void *stream);

// module code uses this to pick the right deallocator for a pointer
bool chpl_gpu_impl_is_device_ptr(const void* ptr);

bool chpl_gpu_impl_is_host_ptr(const void* ptr);

// TODO do we really need to expose this?
size_t chpl_gpu_impl_get_alloc_size(void* ptr);

bool chpl_gpu_impl_can_access_peer(int dev1, int dev2);
void chpl_gpu_impl_set_peer_access(int dev1, int dev2, bool enable);

#ifdef __cplusplus
}
#endif

#endif
