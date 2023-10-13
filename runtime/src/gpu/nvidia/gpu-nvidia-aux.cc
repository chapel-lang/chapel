#include <cuda.h>
#include <cub/cub.cuh>

#include "chpl-gpu.h"
#include "../common/cuda-utils.h"

#define DEF_REDUCE(MACRO, cub_kind, chpl_kind) \
  MACRO(cub_kind, chpl_kind, int8_t)  \
  MACRO(cub_kind, chpl_kind, int16_t)  \
  MACRO(cub_kind, chpl_kind, int32_t)  \
  MACRO(cub_kind, chpl_kind, int64_t)  \
  MACRO(cub_kind, chpl_kind, uint8_t)  \
  MACRO(cub_kind, chpl_kind, uint16_t)  \
  MACRO(cub_kind, chpl_kind, uint32_t)  \
  MACRO(cub_kind, chpl_kind, uint64_t)  \
  MACRO(cub_kind, chpl_kind, float)   \
  MACRO(cub_kind, chpl_kind, double);

#define DEF_ONE_REDUCE_RET_VAL(cub_kind, chpl_kind, data_type) \
void chpl_gpu_##chpl_kind##_reduce_##data_type(data_type* data, int n,\
                                               data_type* val) {\
  CUdeviceptr result; \
  CUDA_CALL(cuMemAlloc(&result, sizeof(data_type))); \
  void* temp = NULL; \
  size_t temp_bytes = 0; \
  cub::DeviceReduce::cub_kind(temp, temp_bytes, data, (data_type*)result, n); \
  CUDA_CALL(cuMemAlloc(((CUdeviceptr*)&temp), temp_bytes)); \
  cub::DeviceReduce::cub_kind(temp, temp_bytes, data, (data_type*)result, n); \
  CUDA_CALL(cuMemcpyDtoH(val, result, sizeof(data_type))); \
}
DEF_REDUCE(DEF_ONE_REDUCE_RET_VAL, Sum, sum)
DEF_REDUCE(DEF_ONE_REDUCE_RET_VAL, Min, min)
DEF_REDUCE(DEF_ONE_REDUCE_RET_VAL, Max, max)
#undef DEF_ONE_REDUCE_RET_VAL

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
DEF_REDUCE(DEF_ONE_REDUCE_RET_VAL_IDX, ArgMin, minloc)
DEF_REDUCE(DEF_ONE_REDUCE_RET_VAL_IDX, ArgMax, maxloc)
#undef DEF_ONE_REDUCE_RET_VAL_IDX

#undef DEF_REDUCE
