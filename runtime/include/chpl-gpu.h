/*
 * Copyright 2020-2021 Hewlett Packard Enterprise Development LP
 * Copyright 2004-2019 Cray Inc.
 * Other additional copyright holders may be indicated within.
 * 
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

#ifndef _CHPL_GPU_H_
#define _CHPL_GPU_H_

#ifdef __cplusplus
extern "C" {
#endif

#ifdef HAS_GPU_LOCALE

void  chpl_gpu_init(void);
void chpl_gpu_launch_kernel(const char* name,
                            int grd_dim_x, int grd_dim_y, int grd_dim_z,
                            int blk_dim_x, int blk_dim_y, int blk_dim_z,
                            int nargs, ...);
void chpl_gpu_launch_kernel_flat(const char* name, int grid_dim_x, int block_dim_x,
                                 int nargs, ...);

void* chpl_gpu_mem_alloc(size_t size, chpl_mem_descInt_t description,
                         int32_t lineno, int32_t filename);
void* chpl_gpu_mem_calloc(size_t number, size_t size,
                          chpl_mem_descInt_t description,
                          int32_t lineno, int32_t filename);
void* chpl_gpu_mem_realloc(void* memAlloc, size_t size,
                           chpl_mem_descInt_t description,
                           int32_t lineno, int32_t filename);
void* chpl_gpu_mem_memalign(size_t boundary, size_t size,
                            chpl_mem_descInt_t description,
                            int32_t lineno, int32_t filename);
void chpl_gpu_mem_free(void* memAlloc, int32_t lineno, int32_t filename);

bool chpl_gpu_has_context(void);
size_t chpl_gpu_get_alloc_size(void* ptr);

void chpl_gpu_copy_device_to_host(void* dst, void* src, size_t n);
void chpl_gpu_copy_host_to_device(void* dst, void* src, size_t n);

bool chpl_gpu_is_device_ptr(void* ptr);
                           
#endif // HAS_GPU_LOCALE

#ifdef __cplusplus
}
#endif

#endif
