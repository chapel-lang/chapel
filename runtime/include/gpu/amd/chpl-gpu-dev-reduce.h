
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

#ifndef _CHPL_GPU_DEV_REDUCE_H
#define _CHPL_GPU_DEV_REDUCE_H

#include "chpltypes.h"
#include <hip/hip_common.h>
#include <hip/hip_runtime.h>
#include "gpu/amd/rocm-version.h"
#include "gpu/amd/rocm-utils.h"

#if ROCM_VERSION_MAJOR >= 5
// if we include this all the time, we get unused function errors
#include <hipcub/hipcub.hpp>
#endif

//
// TODO use chpl-gpu-reduce-util instead, or add these there
#define GPU_DEV_REDUCE_SPECS(MACRO, impl_kind, chpl_kind, data_type) \
  MACRO(impl_kind, chpl_kind, data_type, 32) \
  MACRO(impl_kind, chpl_kind, data_type, 64) \
  MACRO(impl_kind, chpl_kind, data_type, 128) \
  MACRO(impl_kind, chpl_kind, data_type, 256) \
  MACRO(impl_kind, chpl_kind, data_type, 512) \
  MACRO(impl_kind, chpl_kind, data_type, 1024) \

#define GPU_DEV_REDUCE(MACRO, impl_kind, chpl_kind) \
  GPU_DEV_REDUCE_SPECS(MACRO, impl_kind, chpl_kind, chpl_bool) \
  GPU_DEV_REDUCE_SPECS(MACRO, impl_kind, chpl_kind, int8_t) \
  GPU_DEV_REDUCE_SPECS(MACRO, impl_kind, chpl_kind, int16_t)  \
  GPU_DEV_REDUCE_SPECS(MACRO, impl_kind, chpl_kind, int32_t)  \
  GPU_DEV_REDUCE_SPECS(MACRO, impl_kind, chpl_kind, int64_t)  \
  GPU_DEV_REDUCE_SPECS(MACRO, impl_kind, chpl_kind, uint8_t)  \
  GPU_DEV_REDUCE_SPECS(MACRO, impl_kind, chpl_kind, uint16_t)  \
  GPU_DEV_REDUCE_SPECS(MACRO, impl_kind, chpl_kind, uint32_t)  \
  GPU_DEV_REDUCE_SPECS(MACRO, impl_kind, chpl_kind, uint64_t)  \
  GPU_DEV_REDUCE_SPECS(MACRO, impl_kind, chpl_kind, _real32)   \
  GPU_DEV_REDUCE_SPECS(MACRO, impl_kind, chpl_kind, _real64) \


#if ROCM_VERSION_MAJOR >= 5
#define DEF_ONE_DEV_SUM_REDUCE(impl_kind, chpl_kind, data_type, block_size) \
__device__ static inline void \
chpl_gpu_dev_##chpl_kind##_breduce_##data_type##_##block_size(data_type thread_val, \
                                                              data_type* interim_res) { \
\
  typedef hipcub::BlockReduce<data_type, block_size, hipcub::BLOCK_REDUCE_WARP_REDUCTIONS> BlockReduce; \
  __shared__ typename BlockReduce::TempStorage temp_storage; \
  data_type res = BlockReduce(temp_storage).impl_kind(thread_val); \
  if (threadIdx.x == 0) { \
    interim_res[blockIdx.x] = res; \
  } \
}
#else
// we don't produce an error message here, because the final reduction will
// trigger the error. On device, the best we could do is printf, which would
// result in a wall of text
#define DEF_ONE_DEV_SUM_REDUCE(impl_kind, chpl_kind, data_type, block_size) \
__device__ static inline void \
chpl_gpu_dev_##chpl_kind##_breduce_##data_type##_##block_size(data_type thread_val, \
                                                              data_type* interim_res) { \
}
#endif

GPU_DEV_REDUCE(DEF_ONE_DEV_SUM_REDUCE, Sum, sum);

#undef DEF_ONE_DEV_SUM_REDUCE

#if ROCM_VERSION_MAJOR >= 5
#define DEF_ONE_DEV_REDUCE_RET_VAL(impl_kind, chpl_kind, data_type, block_size) \
__device__ static inline void \
chpl_gpu_dev_##chpl_kind##_breduce_##data_type##_##block_size(data_type thread_val, \
                                                              data_type* interim_res) { \
\
  typedef hipcub::BlockReduce<data_type, block_size> BlockReduce; \
  __shared__ typename BlockReduce::TempStorage temp_storage; \
  data_type res = BlockReduce(temp_storage).Reduce(thread_val, hipcub::impl_kind()); \
  if (threadIdx.x == 0) { \
    interim_res[blockIdx.x] = res; \
  } \
}
#else
// we don't produce an error message here, because the final reduction will
// trigger the error. On device, the best we could do is printf, which would
// result in a wall of text
#define DEF_ONE_DEV_REDUCE_RET_VAL(impl_kind, chpl_kind, data_type, block_size) \
__device__ static inline void \
chpl_gpu_dev_##chpl_kind##_breduce_##data_type##_##block_size(data_type thread_val, \
                                                              data_type* interim_res) { \
\
}
#endif

GPU_DEV_REDUCE(DEF_ONE_DEV_REDUCE_RET_VAL, Min, min);
GPU_DEV_REDUCE(DEF_ONE_DEV_REDUCE_RET_VAL, Max, max);

#undef DEF_ONE_DEV_REDUCE_RET_VAL

#endif  // _CHPL_GPU_DEV_REDUCE_H
