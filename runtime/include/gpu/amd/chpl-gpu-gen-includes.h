/*
 * Copyright 2020-2023 Hewlett Packard Enterprise Development LP
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

#ifndef _CHPL_GPU_GEN_INCLUDES_H
#define _CHPL_GPU_GEN_INCLUDES_H

#ifdef HAS_GPU_LOCALE

#include "chpltypes.h"
#include "chpl-comm.h"
#include <hip/hip_runtime.h>

// General TODO
// This file is included in the application executable only. It mirrors
// chpl-gen-include in providing these runtime interfaces to the Chapel module
// and the compiler-generated, communication-related code. We need these
// functions to be executing correctly on the GPU to have GPU-driven
// communication, with the assumption that GPU-driven communication will look
// like regular communication to the compiler. But until we have proper
// implementation for the GPU-driven communication, we just need __device__
// versions of these functions so that we can compile Chapel applications.

// TODO
// This file might need a `chpl_nodeID` analogue. We need the GPU kernels to be
// aware of which locale they're executing on.

__device__ static inline c_sublocid_t chpl_task_getRequestedSubloc(void)
{
  // TODO
  // We probably need an idea similar to `chpl_nodeID`. But, in our current
  // design, the subloc ID that this function is supposed to return is the same
  // as the device ID. So, maybe we call some device function to grab that and
  // return it?
  return 0;
}

// TODO Rest of the functions are relatively boilerplate once we have everything
__device__ static inline chpl_localeID_t chpl_gen_getLocaleID(void)
{
  chpl_localeID_t localeID;
  localeID = {0,chpl_task_getRequestedSubloc()};
  return localeID;
}

__device__ static inline void* c_pointer_return(void* x) { return x; }
__device__ static inline void* c_pointer_return_const(const void* x) {
  return (void*)x;
}

__device__ static inline chpl_localeID_t chpl_rt_buildLocaleID(c_nodeid_t node,  c_sublocid_t subloc) {
  chpl_localeID_t loc = { node, subloc };
  return loc;
}

__device__ static inline c_nodeid_t get_chpl_nodeID(void) {
  return 0;
}

__device__ static inline c_nodeid_t chpl_rt_nodeFromLocaleID(chpl_localeID_t loc) {
  return loc.node;
}

__device__ static inline void chpl_gen_comm_get(void *addr, c_nodeid_t node,
  void* raddr, size_t size, int32_t commID, int ln, int32_t fn)
{
  // TODO
}

__device__ static inline void chpl_gen_comm_put(void* addr, c_nodeid_t node,
  void* raddr, size_t size, int32_t commID, int ln, int32_t fn)
{
  // TODO
}

MAYBE_GPU static inline void chpl_gpu_write(const char *str) { printf("%s", str); }

__device__ static inline void chpl_assert_on_gpu(int32_t lineno, int32_t filenameIdx) { /* no op */ }
__host__ static inline void chpl_assert_on_gpu(int32_t lineno, int32_t filenameIdx) {
  chpl_error("assertOnGpu() failed", lineno, filenameIdx);
}

__device__ static inline unsigned int chpl_gpu_clock(void) {
  return (unsigned int)clock();
}
__host__ static inline unsigned int chpl_gpu_clock(void) {
  return 0;
}

MAYBE_GPU static inline void chpl_gpu_printTimeDelta(
  const char *msg, unsigned int start, unsigned int end)
{
  printf("%s%u\n", msg, end - start);
}

__device__ static inline void chpl_gpu_force_sync() {
  __builtin_amdgcn_s_barrier();
}

__host__ static inline void chpl_gpu_force_sync() {
  chpl_internal_error("chpl_gpu_force_sync called from host");
}

__device__ static inline uint32_t chpl_gpu_getThreadIdxX() { return __builtin_amdgcn_workitem_id_x(); }
__device__ static inline uint32_t chpl_gpu_getThreadIdxY() { return __builtin_amdgcn_workitem_id_y(); }
__device__ static inline uint32_t chpl_gpu_getThreadIdxZ() { return __builtin_amdgcn_workitem_id_z(); }

__device__ static inline uint32_t chpl_gpu_getBlockIdxX()  { return __builtin_amdgcn_workgroup_id_x(); }
__device__ static inline uint32_t chpl_gpu_getBlockIdxY()  { return __builtin_amdgcn_workgroup_id_y(); }
__device__ static inline uint32_t chpl_gpu_getBlockIdxZ()  { return __builtin_amdgcn_workgroup_id_z(); }

__device__ static inline uint32_t chpl_gpu_getBlockDimX()  { return __builtin_amdgcn_workgroup_size_x(); }
__device__ static inline uint32_t chpl_gpu_getBlockDimY()  { return __builtin_amdgcn_workgroup_size_y(); }
__device__ static inline uint32_t chpl_gpu_getBlockDimZ()  { return __builtin_amdgcn_workgroup_size_z(); }

__device__ static inline uint32_t chpl_gpu_getGridDimX()   {
  return __builtin_amdgcn_grid_size_x() / chpl_gpu_getBlockDimX();
}
__device__ static inline uint32_t chpl_gpu_getGridDimY()   {
  return __builtin_amdgcn_grid_size_y() / chpl_gpu_getBlockDimY();
}
__device__ static inline uint32_t chpl_gpu_getGridDimZ()   {
  return __builtin_amdgcn_grid_size_z() / chpl_gpu_getBlockDimZ();
}

// =================
// Atomic Operations
// =================

#define GPU_2OP_ATOMIC(T, runtime_name, rocm_name)           \
  __device__ static inline void runtime_name(T *x, T val) {  \
    rocm_name(x, val);                                       \
  }                                                          \
  __host__ static inline void runtime_name(T *x, T val) {}

#define GPU_3OP_ATOMIC(T, runtime_name, rocm_name)                   \
  __device__ static inline void runtime_name(T *x, T val1, T val2) { \
    rocm_name(x, val1, val2);                                        \
  }                                                                  \
  __host__ static inline void runtime_name(T *x, T val1, T val2) {}

// Some atomic operations are only supported in CUDA while others are only
// supported in ROCM. We mark the operations that are unsupported for this
// header's SDK with [*]:

GPU_2OP_ATOMIC(int,                chpl_gpu_atomic_add_int,       atomicAdd);
GPU_2OP_ATOMIC(unsigned int,       chpl_gpu_atomic_add_uint,      atomicAdd);
GPU_2OP_ATOMIC(unsigned long long, chpl_gpu_atomic_add_ulonglong, atomicAdd);
GPU_2OP_ATOMIC(float,              chpl_gpu_atomic_add_float,     atomicAdd);
GPU_2OP_ATOMIC(double,             chpl_gpu_atomic_add_double,    atomicAdd);

GPU_2OP_ATOMIC(int,          chpl_gpu_atomic_sub_int,  atomicSub);
GPU_2OP_ATOMIC(unsigned int, chpl_gpu_atomic_sub_uint, atomicSub);

GPU_2OP_ATOMIC(int,                    chpl_gpu_atomic_exch_int,       atomicExch);
GPU_2OP_ATOMIC(unsigned int,           chpl_gpu_atomic_exch_uint,      atomicExch);
GPU_2OP_ATOMIC(unsigned long long int, chpl_gpu_atomic_exch_ulonglong, atomicExch);
GPU_2OP_ATOMIC(float,                  chpl_gpu_atomic_exch_float,     atomicExch);

GPU_2OP_ATOMIC(int,                    chpl_gpu_atomic_min_int,       atomicMin);
GPU_2OP_ATOMIC(unsigned int,           chpl_gpu_atomic_min_uint,      atomicMin);
GPU_2OP_ATOMIC(unsigned long long int, chpl_gpu_atomic_min_ulonglong, atomicMin);
// [*] GPU_2OP_ATOMIC(long long int,          chpl_gpu_atomic_min_longlong,  atomicMin);

GPU_2OP_ATOMIC(int,                    chpl_gpu_atomic_max_int,       atomicMax);
GPU_2OP_ATOMIC(unsigned int,           chpl_gpu_atomic_max_uint,      atomicMax);
GPU_2OP_ATOMIC(unsigned long long int, chpl_gpu_atomic_max_ulonglong, atomicMax);
// [*] GPU_2OP_ATOMIC(long long int,          chpl_gpu_atomic_max_longlong,  atomicMax);

GPU_2OP_ATOMIC(unsigned int, chpl_gpu_atomic_inc_uint, atomicInc);

GPU_2OP_ATOMIC(unsigned int, chpl_gpu_atomic_dec_uint, atomicDec);

GPU_2OP_ATOMIC(int,                    chpl_gpu_atomic_and_int,       atomicAnd);
GPU_2OP_ATOMIC(unsigned int,           chpl_gpu_atomic_and_uint,      atomicAnd);
GPU_2OP_ATOMIC(unsigned long long int, chpl_gpu_atomic_and_ulonglong, atomicAnd);

GPU_2OP_ATOMIC(int,                    chpl_gpu_atomic_or_int,       atomicOr);
GPU_2OP_ATOMIC(unsigned int,           chpl_gpu_atomic_or_uint,      atomicOr);
GPU_2OP_ATOMIC(unsigned long long int, chpl_gpu_atomic_or_ulonglong, atomicOr);

GPU_2OP_ATOMIC(int,                    chpl_gpu_atomic_xor_int,       atomicXor);
GPU_2OP_ATOMIC(unsigned int,           chpl_gpu_atomic_xor_uint,      atomicXor);
GPU_2OP_ATOMIC(unsigned long long int, chpl_gpu_atomic_xor_ulonglong, atomicXor);

GPU_3OP_ATOMIC(int,                    chpl_gpu_atomic_CAS_int,       atomicCAS);
GPU_3OP_ATOMIC(unsigned int,           chpl_gpu_atomic_CAS_uint,      atomicCAS);
GPU_3OP_ATOMIC(unsigned long long int, chpl_gpu_atomic_CAS_ulonglong, atomicCAS);
// [*] GPU_3OP_ATOMIC(unsigned short int,     chpl_gpu_atomic_CAS_ushort, atomicCAS);

#endif // HAS_GPU_LOCALE

#endif // _CHPL_GPU_GEN_INCLUDES_H
