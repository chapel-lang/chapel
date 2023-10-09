#include <cuda.h>
#include <cub/cub.cuh>

#include "chpl-gpu.h"
#include "../common/cuda-utils.h"

#define DEF_ONE_BASIC_REDUCE(cub_kind, chpl_kind, data_type) \
data_type chpl_gpu_##chpl_kind##_reduce_##data_type(data_type* data, int n) {\
  CUdeviceptr result; \
  CUDA_CALL(cuMemAlloc(&result, sizeof(data_type))); \
  void* temp = NULL; \
  size_t temp_bytes = 0; \
  cub::DeviceReduce::cub_kind(temp, temp_bytes, data, (data_type*)result, n); \
  CUDA_CALL(cuMemAlloc(((CUdeviceptr*)&temp), temp_bytes)); \
  cub::DeviceReduce::cub_kind(temp, temp_bytes, data, (data_type*)result, n); \
  data_type result_host; \
  CUDA_CALL(cuMemcpyDtoH(&result_host, result, sizeof(data_type))); \
  return result_host; \
}

#define DEF_BASIC_REDUCE(cub_kind, chpl_kind) \
  DEF_ONE_BASIC_REDUCE(cub_kind, chpl_kind, int8_t)  \
  DEF_ONE_BASIC_REDUCE(cub_kind, chpl_kind, int16_t)  \
  DEF_ONE_BASIC_REDUCE(cub_kind, chpl_kind, int32_t)  \
  DEF_ONE_BASIC_REDUCE(cub_kind, chpl_kind, int64_t)  \
  DEF_ONE_BASIC_REDUCE(cub_kind, chpl_kind, uint8_t)  \
  DEF_ONE_BASIC_REDUCE(cub_kind, chpl_kind, uint16_t)  \
  DEF_ONE_BASIC_REDUCE(cub_kind, chpl_kind, uint32_t)  \
  DEF_ONE_BASIC_REDUCE(cub_kind, chpl_kind, uint64_t)  \
  DEF_ONE_BASIC_REDUCE(cub_kind, chpl_kind, float)   \
  DEF_ONE_BASIC_REDUCE(cub_kind, chpl_kind, double);

DEF_BASIC_REDUCE(Sum, sum)
DEF_BASIC_REDUCE(Min, min)
DEF_BASIC_REDUCE(Max, max)

#define DEF_ONE_REDUCE_RET_VAL_IDX(cub_kind, chpl_kind, data_type) \
void chpl_gpu_##chpl_kind##_reduce_##data_type(data_type* data, int n,\
                                               data_type* val, int* idx) {\
  using kvp = cub::KeyValuePair<int,data_type>; \
  CUdeviceptr result; \
  CUDA_CALL(cuMemAlloc(&result, sizeof(kvp))); \
  void* temp = NULL; \
  size_t temp_bytes = 0; \
  cub::DeviceReduce::cub_kind(temp, temp_bytes, data, (kvp*)result, n); \
  CUDA_CALL(cuMemAlloc(((CUdeviceptr*)&temp), temp_bytes)); \
  cub::DeviceReduce::cub_kind(temp, temp_bytes, data, (kvp*)result, n); \
  kvp result_host; \
  CUDA_CALL(cuMemcpyDtoH(&result_host, result, sizeof(kvp))); \
  *val = result_host.value; \
  *idx = result_host.key; \
}

#define DEF_REDUCE_RET_VAL_IDX(cub_kind, chpl_kind) \
  DEF_ONE_REDUCE_RET_VAL_IDX(cub_kind, chpl_kind, int8_t)  \
  DEF_ONE_REDUCE_RET_VAL_IDX(cub_kind, chpl_kind, int16_t)  \
  DEF_ONE_REDUCE_RET_VAL_IDX(cub_kind, chpl_kind, int32_t)  \
  DEF_ONE_REDUCE_RET_VAL_IDX(cub_kind, chpl_kind, int64_t)  \
  DEF_ONE_REDUCE_RET_VAL_IDX(cub_kind, chpl_kind, uint8_t)  \
  DEF_ONE_REDUCE_RET_VAL_IDX(cub_kind, chpl_kind, uint16_t)  \
  DEF_ONE_REDUCE_RET_VAL_IDX(cub_kind, chpl_kind, uint32_t)  \
  DEF_ONE_REDUCE_RET_VAL_IDX(cub_kind, chpl_kind, uint64_t)  \
  DEF_ONE_REDUCE_RET_VAL_IDX(cub_kind, chpl_kind, float)   \
  DEF_ONE_REDUCE_RET_VAL_IDX(cub_kind, chpl_kind, double);

DEF_REDUCE_RET_VAL_IDX(ArgMin, minloc)
DEF_REDUCE_RET_VAL_IDX(ArgMax, maxloc)
