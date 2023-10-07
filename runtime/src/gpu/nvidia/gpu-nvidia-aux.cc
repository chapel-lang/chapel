#include <cuda.h>
#include <cub/cub.cuh>

#include "chpl-gpu.h"
#include "../common/cuda-utils.h"

int64_t chpl_gpu_aux_sum_reduce(int64_t* data, int n) {

  CUdeviceptr result;

  CUDA_CALL(cuMemAlloc(&result, sizeof(int64_t)));

  void* temp = NULL;
  size_t temp_bytes = 0;
  cub::DeviceReduce::Sum(temp, temp_bytes, data, (int64_t*)result, n);

  //printf("Allocation for scratch %zu\n", temp_bytes);

  // Allocate temporary storage
  CUDA_CALL(cuMemAlloc(((CUdeviceptr*)&temp), temp_bytes));

  //printf("Temporary allocated %p\n", temp);

  // Run sum-reduction
  cub::DeviceReduce::Sum(temp, temp_bytes, data, (int64_t*)result, n, 0);

  int64_t result_host;
  CUDA_CALL(cuMemcpyDtoH(&result_host, result, sizeof(int64_t)));
  //printf("Result %ld\n", result_host);
  return result_host;
}
