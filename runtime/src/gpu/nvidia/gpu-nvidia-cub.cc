/*
 * Copyright 2020-2025 Hewlett Packard Enterprise Development LP
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

#include <cuda.h>
// Based on https://github.com/NVIDIA/cub/issues/246
// The aggressive loop unrolling doesn't offer many advantages
// So we disable warnings about being unable to unroll
// Without this we see clang errors about [-Wpass-failed=transform-warning]
#if defined(__clang__)
#  pragma clang diagnostic push
#  pragma clang diagnostic ignored "-Wpass-failed"
#endif
#include <cub/cub.cuh>
#if defined(__clang__)
#  pragma clang diagnostic pop
#endif
#include "chpl-gpu.h"
#include "chpl-gpu-impl.h"
#include "../common/cuda-utils.h"
#include "gpu/chpl-gpu-reduce-util.h"

// this version doesn't do anything with `idx`. Having a unified interface makes
// the implementation in the rest of the runtime and the modules more
// straightforward.
#define DEF_ONE_REDUCE_RET_VAL(cub_kind, chpl_kind, data_type) \
void chpl_gpu_impl_##chpl_kind##_reduce_##data_type(data_type* data, int n,\
                                                    data_type* val, int* idx,\
                                                    void* stream) {\
  CUdeviceptr result; \
  CUDA_CALL(cuMemAlloc(&result, sizeof(data_type))); \
  void* temp = NULL; \
  size_t temp_bytes = 0; \
  cub::DeviceReduce::cub_kind(temp, temp_bytes, data, (data_type*)result, n,\
                              (CUstream)stream); \
  CUDA_CALL(cuMemAlloc(((CUdeviceptr*)&temp), temp_bytes)); \
  cub::DeviceReduce::cub_kind(temp, temp_bytes, data, (data_type*)result, n,\
                              (CUstream)stream); \
  CUDA_CALL(cuMemcpyDtoHAsync(val, result, sizeof(data_type),\
                              (CUstream)stream)); \
  CUDA_CALL(cuMemFree(result)); \
  CUDA_CALL(cuMemFree((CUdeviceptr)temp)); \
}

GPU_DEV_CUB_WRAP(DEF_ONE_REDUCE_RET_VAL, Sum, sum)
GPU_DEV_CUB_WRAP(DEF_ONE_REDUCE_RET_VAL, Min, min)
GPU_DEV_CUB_WRAP(DEF_ONE_REDUCE_RET_VAL, Max, max)

#undef DEF_ONE_REDUCE_RET_VAL

#define DEF_ONE_REDUCE_RET_VAL_IDX(cub_kind, chpl_kind, data_type) \
void chpl_gpu_impl_##chpl_kind##_reduce_##data_type(data_type* data, int n,\
                                                    data_type* val, int* idx,\
                                                    void* stream) {\
  using kvp = cub::KeyValuePair<int,data_type>; \
  CUdeviceptr result; \
  CUDA_CALL(cuMemAlloc(&result, sizeof(kvp))); \
  void* temp = NULL; \
  size_t temp_bytes = 0; \
  cub::DeviceReduce::cub_kind(temp, temp_bytes, data, (kvp*)result, n,\
                              (CUstream)stream);\
  CUDA_CALL(cuMemAlloc(((CUdeviceptr*)&temp), temp_bytes)); \
  cub::DeviceReduce::cub_kind(temp, temp_bytes, data, (kvp*)result, n,\
                              (CUstream)stream);\
  kvp result_host; \
  CUDA_CALL(cuMemcpyDtoHAsync(&result_host, result, sizeof(kvp),\
                              (CUstream)stream)); \
  *val = result_host.value; \
  *idx = result_host.key; \
  CUDA_CALL(cuMemFree(result)); \
  CUDA_CALL(cuMemFree((CUdeviceptr)temp)); \
}

GPU_DEV_CUB_WRAP(DEF_ONE_REDUCE_RET_VAL_IDX, ArgMin, minloc)
GPU_DEV_CUB_WRAP(DEF_ONE_REDUCE_RET_VAL_IDX, ArgMax, maxloc)

#undef DEF_ONE_REDUCE_RET_VAL_IDX

#define DEF_ONE_SORT(cub_kind, chpl_kind, data_type) \
void chpl_gpu_impl_sort_##chpl_kind##_##data_type(data_type* data_in, \
                                                  data_type* data_out, \
                                                  int n, void* stream) {\
  void* temp = NULL; \
  size_t temp_bytes = 0; \
  cub::DeviceRadixSort::cub_kind(temp, temp_bytes, data_in, data_out,\
                                 n, /*beginBit*/0, \
                                 /*endBit*/ sizeof(data_type)*8,\
                                 (CUstream)stream); \
  CUDA_CALL(cuMemAlloc(((CUdeviceptr*)&temp), temp_bytes)); \
  cub::DeviceRadixSort::cub_kind(temp, temp_bytes, data_in, data_out,\
                                 n, /*beginBit*/0, \
                                 /*endBit*/ sizeof(data_type)*8,\
                                 (CUstream)stream); \
  CUDA_CALL(cuMemFree((CUdeviceptr)temp)); \
}

GPU_DEV_CUB_WRAP(DEF_ONE_SORT, SortKeys, keys)

#undef DEF_ONE_SORT

#endif // HAS_GPU_LOCALE

