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

#ifndef _CHPL_GPU_GEN_INCLUDES_H
#define _CHPL_GPU_GEN_INCLUDES_H

#ifdef HAS_GPU_LOCALE

#include "../chpl-gpu-gen-common.h"

#include "chpltypes.h"
#include "chpl-comm.h"

// this variable is added by the compiler. See codegenGpuGlobals.
extern __device__ c_nodeid_t chpl_nodeID;

// General TODO
// This file is included in the application executable only. It mirrors
// chpl-gen-include in providing these runtime interfaces to the Chapel module
// and the compiler-generated, communication-related code. We need these
// functions to be executing correctly on the GPU to have GPU-driven
// communication, with the assumption that GPU-driven communication will look
// like regular communication to the compiler. But until we have proper
// implementation for the GPU-driven communication, we just need __device__
// versions of these functions so that we can compile Chapel applications.

__device__ static inline c_nodeid_t get_chpl_nodeID(void) {
  return chpl_nodeID;
}

// TODO Rest of the functions are relatively boilerplate once we have everything
__device__ static inline chpl_localeID_t chpl_gen_getLocaleID(void)
{
  chpl_localeID_t localeID;
  localeID = {get_chpl_nodeID() ,chpl_task_getRequestedSubloc()};
  return localeID;
}

__device__ static inline void chpl_gpu_force_sync() {
  // Using __syncThreads() directly causes issues when compiling programs with
  // --fast.  It's likely due to an issue with clang discussed here:
  // https://github.com/llvm/llvm-project/issues/58626
  asm volatile("bar.sync 0;" : : : "memory");
}

__device__ static inline void chpl_gpu_force_warp_sync(unsigned mask) {
  // Call nvidia's syncwarp
  __syncwarp(mask);
}

__device__ static inline uint32_t chpl_gpu_getThreadIdxX() { return __nvvm_read_ptx_sreg_tid_x(); }
__device__ static inline uint32_t chpl_gpu_getThreadIdxY() { return __nvvm_read_ptx_sreg_tid_y(); }
__device__ static inline uint32_t chpl_gpu_getThreadIdxZ() { return __nvvm_read_ptx_sreg_tid_z(); }

__device__ static inline uint32_t chpl_gpu_getBlockIdxX()  { return __nvvm_read_ptx_sreg_ctaid_x(); }
__device__ static inline uint32_t chpl_gpu_getBlockIdxY()  { return __nvvm_read_ptx_sreg_ctaid_y(); }
__device__ static inline uint32_t chpl_gpu_getBlockIdxZ()  { return __nvvm_read_ptx_sreg_ctaid_z(); }

__device__ static inline uint32_t chpl_gpu_getBlockDimX()  { return __nvvm_read_ptx_sreg_ntid_x(); }
__device__ static inline uint32_t chpl_gpu_getBlockDimY()  { return __nvvm_read_ptx_sreg_ntid_y(); }
__device__ static inline uint32_t chpl_gpu_getBlockDimZ()  { return __nvvm_read_ptx_sreg_ntid_z(); }

__device__ static inline uint32_t chpl_gpu_getGridDimX()   { return __nvvm_read_ptx_sreg_nctaid_x(); }
__device__ static inline uint32_t chpl_gpu_getGridDimY()   { return __nvvm_read_ptx_sreg_nctaid_y(); }
__device__ static inline uint32_t chpl_gpu_getGridDimZ()   { return __nvvm_read_ptx_sreg_nctaid_z(); }

// =================
// Atomic Operations
// =================

#define GPU_2OP_ATOMIC(T, runtime_name, cuda_name)            \
  __device__ static inline T runtime_name(T *x, T val) {      \
    return cuda_name(x, val);                                 \
  }                                                           \
  __host__ static inline T runtime_name(T *x, T val) {return 0;}

#define GPU_3OP_ATOMIC(T, runtime_name, cuda_name)                   \
  __device__ static inline T runtime_name(T *x, T val1, T val2) {    \
    return cuda_name(x, val1, val2);                                 \
  }                                                                  \
  __host__ static inline T runtime_name(T *x, T val1, T val2) {return 0;}

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
GPU_2OP_ATOMIC(long long int,          chpl_gpu_atomic_min_longlong,  atomicMin);

GPU_2OP_ATOMIC(int,                    chpl_gpu_atomic_max_int,       atomicMax);
GPU_2OP_ATOMIC(unsigned int,           chpl_gpu_atomic_max_uint,      atomicMax);
GPU_2OP_ATOMIC(unsigned long long int, chpl_gpu_atomic_max_ulonglong, atomicMax);
GPU_2OP_ATOMIC(long long int,          chpl_gpu_atomic_max_longlong,  atomicMax);

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

#if defined(__CUDA_ARCH__) && (__CUDA_ARCH__ >= 700)
GPU_3OP_ATOMIC(unsigned short int,     chpl_gpu_atomic_CAS_ushort, atomicCAS);
#endif

#endif // HAS_GPU_LOCALE

#endif // _CHPL_GPU_GEN_INCLUDES_H
