/*
 * Copyright 2020-2025 Hewlett Packard Enterprise Development LP
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

#include <hip/hip_runtime.h>

#include "../chpl-gpu-gen-common.h"

#include "chpltypes.h"
#include "chpl-comm.h"

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

__device__ static inline chpl_localeID_t chpl_gen_getLocaleID(void)
{
  chpl_localeID_t localeID;
  localeID = {0,chpl_task_getRequestedSubloc()};
  return localeID;
}


__device__ static inline c_nodeid_t get_chpl_nodeID(void) {
  return 0;
}

__device__ static inline void chpl_gpu_force_sync() {
  __builtin_amdgcn_s_barrier();
}

__device__ static inline void chpl_gpu_force_warp_sync(unsigned mask) {
  // no-op
  // AMD gpu's have no architectures which need a syncWarp operation
  // This is because as of the time of writing, HIP guarantees that all
  // threads within a warp will be executed in lockstep
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
  __device__ static inline T runtime_name(T *x, T val) {     \
    return rocm_name(x, val);                                \
  }                                                          \
  __host__ static inline T runtime_name(T *x, T val) {return 0;}

#define GPU_3OP_ATOMIC(T, runtime_name, rocm_name)                   \
  __device__ static inline T runtime_name(T *x, T val1, T val2) {    \
    return rocm_name(x, val1, val2);                                 \
  }                                                                  \
  __host__ static inline T runtime_name(T *x, T val1, T val2) {return 0;}

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
