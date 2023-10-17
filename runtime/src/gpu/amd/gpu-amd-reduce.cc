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

#ifdef HAS_GPU_LOCALE

#include <hip/hip_common.h>
#include <rocprim/rocprim.hpp>

#include "chpl-gpu.h"
#include "chpl-gpu-impl.h"
#include "gpu/chpl-gpu-reduce-util.h"

#if ROCM_VERSION_MAJOR >= 5
#define DEF_ONE_REDUCE_RET_VAL(impl_kind, chpl_kind, data_type) \
void chpl_gpu_impl_##chpl_kind##_reduce_##data_type(data_type* data, int n,\
                                                    data_type* val,\
                                                    void* stream) {\
  hipDeviceptr_t result; \
  ROCM_CALL(hipMalloc(&result, sizeof(data_type))); \
  void* temp = NULL; \
  size_t temp_bytes = 0; \
  rocmprim::reduce(temp, temp_bytes, data, (data_type*)result, n,\
                   rocmprim::impl_kind<data_type>,\
                   (hipStream_t)stream); \
  ROCM_CALL(hipMalloc(((CUdeviceptr*)&temp), temp_bytes)); \
  rocmprim::reduce(temp, temp_bytes, data, (data_type*)result, n,\
                   rocmprim::impl_kind<data_type>,\
                   (hipStream_t)stream); \
  ROCM_CALL(hipMemcpyDtoHAsync(val, result, sizeof(data_type),\
                              (CUstream)stream)); \
}
#else
#define DEF_ONE_REDUCE_RET_VAL(impl_kind, chpl_kind, data_type) \
void chpl_gpu_impl_##chpl_kind##_reduce_##data_type(data_type* data, int n,\
                                                    data_type* val,\
                                                    void* stream) {\
  chpl_internal_error("Reduction is not supported with AMD GPUs using ROCm version <5\n");\
}
#endif

GPU_IMPL_REDUCE(DEF_ONE_REDUCE_RET_VAL, plus, sum)
GPU_IMPL_REDUCE(DEF_ONE_REDUCE_RET_VAL, minimum, min)
GPU_IMPL_REDUCE(DEF_ONE_REDUCE_RET_VAL, maximum, max)

#undef DEF_ONE_REDUCE_RET_VAL

#define DEF_ONE_REDUCE_RET_VAL_IDX(cub_kind, chpl_kind, data_type) \
void chpl_gpu_impl_##chpl_kind##_reduce_##data_type(data_type* data, int n,\
                                                    data_type* val, int* idx,\
                                                    void* stream) {\
  chpl_internal_error(chpl_kind # " reduction is not supported with AMD GPUs\n");\
}

GPU_IMPL_REDUCE(DEF_ONE_REDUCE_RET_VAL_IDX, unknown, minloc)
GPU_IMPL_REDUCE(DEF_ONE_REDUCE_RET_VAL_IDX, unknown, maxloc)

#undef DEF_ONE_REDUCE_RET_VAL_IDX

#undef DEF_REDUCE

#endif // HAS_GPU_LOCALE

