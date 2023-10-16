#include <cuda.h>
#include <cub/cub.cuh>

#include "chpl-gpu.h"
#include "chpl-gpu-impl.h"
#include "../common/cuda-utils.h"
#include "gpu/chpl-gpu-reduce-util.h"

#define DEF_ONE_REDUCE_RET_VAL(cub_kind, chpl_kind, data_type) \
void chpl_gpu_impl_##chpl_kind##_reduce_##data_type(data_type* data, int n,\
                                                    data_type* val,\
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
}

GPU_IMPL_REDUCE(DEF_ONE_REDUCE_RET_VAL, Sum, sum)
GPU_IMPL_REDUCE(DEF_ONE_REDUCE_RET_VAL, Min, min)
GPU_IMPL_REDUCE(DEF_ONE_REDUCE_RET_VAL, Max, max)

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
}

GPU_IMPL_REDUCE(DEF_ONE_REDUCE_RET_VAL_IDX, ArgMin, minloc)
GPU_IMPL_REDUCE(DEF_ONE_REDUCE_RET_VAL_IDX, ArgMax, maxloc)

#undef DEF_ONE_REDUCE_RET_VAL_IDX

#undef DEF_REDUCE
