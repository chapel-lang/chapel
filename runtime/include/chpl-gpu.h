/*
 * Copyright 2020-2024 Hewlett Packard Enterprise Development LP
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

#include <stdbool.h>
#include "chpl-tasks.h"
#include "chpl-mem-desc.h"
#include "gpu/chpl-gpu-reduce-util.h"

#ifdef __cplusplus
extern "C" {
#endif

// We need to declare this variable outside of the commented out HAS_GPU_LOCALE
// section due to the fact that GpuDiagnostics module accesses it (and this
// module can be used despite what locale model you're using).
extern bool chpl_gpu_debug;
extern int chpl_gpu_num_devices;
extern bool chpl_gpu_no_cpu_mode_warning;
extern bool chpl_gpu_sync_with_host;
extern bool chpl_gpu_use_stream_per_task;


#ifdef HAS_GPU_LOCALE

__attribute__ ((format (printf, 1, 2)))
static inline void CHPL_GPU_DEBUG(const char *str, ...) {
  if (chpl_gpu_debug) {
    va_list args;
    va_start(args, str);
    vfprintf(stdout, str, args);
    va_end(args);
    fflush(stdout);
  }
}

#ifdef CHPL_GPU_ENABLE_PROFILE
// returns time from epoch in milliseconds. Used in macros below.
static inline long double get_time(void) {
  struct timeval tv;

  gettimeofday(&tv, NULL);

  return (tv.tv_sec*1000000.0+tv.tv_usec)/1000.0;
}

#define CHPL_GPU_START_TIMER(t) \
  long double t; \
  long double start_##t; \
  do { start_##t = get_time(); } while(0);

#define CHPL_GPU_STOP_TIMER(t) \
  do { t = get_time()-start_##t; } while(0);

#define CHPL_GPU_PRINT_TIMERS(...) \
  do { printf(__VA_ARGS__); } while(0);

#else
#define CHPL_GPU_START_TIMER(t) do {} while(0);
#define CHPL_GPU_STOP_TIMER(t)
#define CHPL_GPU_PRINT_TIMERS(...) do {} while(0);
#endif

static inline bool chpl_gpu_running_on_gpu_locale(void) {
  return chpl_task_getRequestedSubloc()>=0;
}

typedef void(*reduce_wrapper_fn_t)(void*, int, void*, int*);

void chpl_gpu_init(void);
void chpl_gpu_task_end(void);
void chpl_gpu_task_fence(void);
void chpl_gpu_support_module_finished_initializing(void);

void* chpl_gpu_init_kernel_cfg(const char* fn_name, int64_t num_threads,
                               int blk_dim, int n_params, int n_pids,
                               int n_redbufs, int n_hostreg_vars, int ln,
                               int32_t fn);
void* chpl_gpu_init_kernel_cfg_3d(const char* fn_name,
                                  int grd_dim_x, int grd_dim_y, int grd_dim_z,
                                  int blk_dim_x, int blk_dim_y, int blk_dim_z,
                                  int n_params, int n_pids, int n_redbufs,
                                  int n_hostreg_vars, int ln, int32_t fn);

void chpl_gpu_deinit_kernel_cfg(void* cfg);
void chpl_gpu_arg_offload(void* cfg, void* arg, size_t size);
void chpl_gpu_pid_offload(void* cfg, int64_t pid, size_t size);
void chpl_gpu_arg_pass(void* cfg, void* arg);
void chpl_gpu_arg_reduce(void* cfg, void* arg, size_t elem_size,
                         reduce_wrapper_fn_t wrapper);
void chpl_gpu_arg_host_register(void* _cfg, void* arg, size_t size);
void chpl_gpu_launch_kernel(void* cfg);

void* chpl_gpu_mem_array_alloc(size_t size, chpl_mem_descInt_t description,
                                   int32_t lineno, int32_t filename);
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
void chpl_gpu_hostmem_register(void *memAlloc, size_t size);

void chpl_gpu_memcpy(c_sublocid_t dst_subloc, void* dst,
                     c_sublocid_t src_subloc, const void* src,
                     size_t n, int32_t commID, int ln, int32_t fn);
void chpl_gpu_comm_put(c_nodeid_t dst_node, c_sublocid_t dst_subloc, void *dst,
                       c_sublocid_t src_subloc, void *src,
                       size_t size, int32_t commID, int ln, int32_t fn);

void chpl_gpu_comm_get(c_sublocid_t dst_subloc, void *dst,
                       c_nodeid_t src_node, c_sublocid_t src_subloc, void *src,
                       size_t size, int32_t commID, int ln, int32_t fn);

void chpl_gpu_comm_get_strd(c_sublocid_t dst_subloc,
                            void* dstaddr_arg, size_t* dststrides,
                            c_nodeid_t srclocale, c_sublocid_t src_subloc,
                            void* srcaddr_arg, size_t* srcstrides,
                            size_t* count, int32_t strlevels, size_t elemSize,
                            int32_t commID, int ln, int32_t fn);

void chpl_gpu_comm_put_strd(c_sublocid_t src_subloc,
                          void* dstaddr_arg, size_t* dststrides,
                          c_nodeid_t dstlocale, c_sublocid_t dst_subloc,
                          void* srcaddr_arg, size_t* srcstrides,
                          size_t* count, int32_t stridelevels, size_t elemSize,
                          int32_t commID, int ln, int32_t fn);


void* chpl_gpu_memset(void* addr, const uint8_t val, size_t n);
void chpl_gpu_copy_device_to_host(void* dst, c_sublocid_t src_dev,
                                  const void* src, size_t n, int32_t commID,
                                  int ln, int32_t fn);
void chpl_gpu_copy_host_to_device(c_sublocid_t dst_dev, void* dst,
                                  const void* src, size_t n, int32_t commID,
                                  int ln, int32_t fn);
void chpl_gpu_copy_device_to_device(c_sublocid_t dst_dev, void* dst,
                                    c_sublocid_t src_dev, const void* src,
                                    size_t n, int32_t commID, int ln,
                                    int32_t fn);
void* chpl_gpu_comm_async(void *dst, void *src, size_t n);
void chpl_gpu_comm_wait(void *stream);

bool chpl_gpu_is_device_ptr(const void* ptr);
bool chpl_gpu_is_host_ptr(const void* ptr);

unsigned int chpl_gpu_device_clock_rate(int32_t devNum);

// TODO do we really need to expose this?
size_t chpl_gpu_get_alloc_size(void* ptr);

bool chpl_gpu_can_access_peer(int dev1, int dev2);
void chpl_gpu_set_peer_access(int dev1, int dev2, bool enable);

bool chpl_gpu_can_reduce(void);
bool chpl_gpu_can_sort(void);

#define DECL_ONE_REDUCE(chpl_kind, data_type) \
void chpl_gpu_##chpl_kind##_reduce_##data_type(void* data, int n,\
                                               void* val, int* idx);

GPU_CUB_WRAP(DECL_ONE_REDUCE, sum);
GPU_CUB_WRAP(DECL_ONE_REDUCE, min);
GPU_CUB_WRAP(DECL_ONE_REDUCE, max);
GPU_CUB_WRAP(DECL_ONE_REDUCE, minloc);
GPU_CUB_WRAP(DECL_ONE_REDUCE, maxloc);

#undef DECL_ONE_REDUCE

#define DECL_ONE_SORT(chpl_kind, data_type) \
void chpl_gpu_sort_##chpl_kind##_##data_type(data_type* data_in, \
                                        data_type* data_out, \
                                        int n);

GPU_CUB_WRAP(DECL_ONE_SORT, keys);

#undef DECL_ONE_SORT

#else // HAS_GPU_LOCALE

// Provide a fallback for the chpl_assert_on_gpu function for non-GPU locales.
// This works exactly the same as the standard one.

static inline void chpl_assert_on_gpu(int32_t lineno, int32_t filenameIdx) {
  chpl_error("assertOnGpu() failed", lineno, filenameIdx);
}

#endif // HAS_GPU_LOCALE

#ifdef __cplusplus
}
#endif

#endif
