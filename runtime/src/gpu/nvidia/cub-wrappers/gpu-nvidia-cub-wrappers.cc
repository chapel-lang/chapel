#include <cuda.h>
#include <cub/cub.cuh>

#include "chpl-gpu.h"

int chpl_gpu_aux_sum_reduce(int* data, int n) {

  int result = 0;
  void* temp = NULL;
  size_t temp_bytes = 0;
  cub::DeviceReduce::Sum(temp, temp_bytes, data, &result, n);

  // Allocate temporary storage
  cuMemAlloc(((CUdeviceptr*)&temp), temp_bytes);

  // Run sum-reduction
  cub::DeviceReduce::Sum(temp, temp_bytes, data, &result, n);

  printf("Result %d\n", result);
  return result;
}
