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

/* TODO uncomment these when the implementations are in
#include <hip/hip_common.h>
#include <hipcub/hipcub.hpp>
#include <rocm_version.h>
*/

#include "chpl-gpu.h"
#include "chpl-gpu-impl.h"
#include "gpu/chpl-gpu-reduce-util.h"

// Engin: I can't get neither hipCUB nor rocprim to work. (hipCUB is a light
// wrapper around rocprim anyways). I filed
// https://github.com/ROCmSoftwarePlatform/hipCUB/issues/304, but I don't know
// if/when I'll hear back something. For now, I am merging the code that's
// supposed to work but doesn't instead of removing them from my branch.
#if 1
#define DEF_ONE_REDUCE_RET_VAL(impl_kind, chpl_kind, data_type) \
void chpl_gpu_impl_##chpl_kind##_reduce_##data_type(data_type* data, int n,\
                                                    data_type* val, int* idx,\
                                                    void* stream) {\
  chpl_internal_error("This function shouldn't have been called. Reduction is not supported with AMD GPUs\n");\
}
#elif ROCM_VERSION_MAJOR >= 5
#define DEF_ONE_REDUCE_RET_VAL(impl_kind, chpl_kind, data_type) \
void chpl_gpu_impl_##chpl_kind##_reduce_##data_type(data_type* data, int n,\
                                                    data_type* val, int* idx,\
                                                    void* stream) {\
  data_type* result; \
  ROCM_CALL(hipMalloc(&result, sizeof(data_type)));\
  void* temp = NULL; \
  size_t temp_bytes = 0; \
  ROCM_CALL(hipcub::DeviceReduce::impl_kind(temp, temp_bytes, data, (data_type*)result, n,\
                                  0, true));\
  ROCM_CALL(hipMalloc(((hipDeviceptr_t*)&temp), temp_bytes)); \
  ROCM_CALL(hipcub::DeviceReduce::impl_kind(temp, temp_bytes, data, (data_type*)result, n,\
                                  0, true));\
  ROCM_CALL(hipMemcpyDtoHAsync(val, result, sizeof(data_type),\
                              (hipStream_t)stream)); \
}
#else
#define DEF_ONE_REDUCE_RET_VAL(impl_kind, chpl_kind, data_type) \
void chpl_gpu_impl_##chpl_kind##_reduce_##data_type(data_type* data, int n,\
                                                    data_type* val,\
                                                    void* stream) {\
  chpl_internal_error("Reduction is not supported with AMD GPUs using ROCm version <5\n");\
}
#endif // 1

GPU_IMPL_REDUCE(DEF_ONE_REDUCE_RET_VAL, Sum, sum)
GPU_IMPL_REDUCE(DEF_ONE_REDUCE_RET_VAL, Min, min)
GPU_IMPL_REDUCE(DEF_ONE_REDUCE_RET_VAL, Max, max)

#undef DEF_ONE_REDUCE_RET_VAL

#if 1
#define DEF_ONE_REDUCE_RET_VAL_IDX(cub_kind, chpl_kind, data_type) \
void chpl_gpu_impl_##chpl_kind##_reduce_##data_type(data_type* data, int n,\
                                                    data_type* val, int* idx,\
                                                    void* stream) {\
  chpl_internal_error("This function shouldn't have been called. Reduction is not supported with AMD GPUs\n");\
}
#else
#define DEF_ONE_REDUCE_RET_VAL_IDX(cub_kind, chpl_kind, data_type) \
void chpl_gpu_impl_##chpl_kind##_reduce_##data_type(data_type* data, int n,\
                                                    data_type* val, int* idx,\
                                                    void* stream) {\
  // TODO I don't know any other specific issues with these versions. Should be
  // able to whip up the implementation quickly once we figure out what's going
  // wrong here.
  chpl_internal_error("Unimplemented");
}
#endif // 1

GPU_IMPL_REDUCE(DEF_ONE_REDUCE_RET_VAL_IDX, ArgMin, minloc)
GPU_IMPL_REDUCE(DEF_ONE_REDUCE_RET_VAL_IDX, ArgMax, maxloc)

#undef DEF_ONE_REDUCE_RET_VAL_IDX

#undef DEF_REDUCE

#endif // HAS_GPU_LOCALE

