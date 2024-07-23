#include <stdio.h>
#include <stdlib.h>
#include <float.h>
#include <cuda_runtime.h>
#include <curand.h>
#include <curand_kernel.h>
#include <time.h>
#include <cub/cub.cuh>
#include <math.h>

#ifndef mk_PREFIX
#define mk_PREFIX
#endif

#ifndef mk_LABEL
#define mk_LABEL ""
#endif

#ifndef mk_MATH_FUNC
#define mk_MATH_FUNC tanhf
#endif

#ifndef mk_SIZE
#define mk_SIZE 100'000'000
#endif

#define TO_STR_inner(x) #x
#define TO_STR(x) TO_STR_inner(x)

#define mk_FUNC_NAME_inner2(a, b) a ## b
#define mk_FUNC_NAME_inner1(a, b) mk_FUNC_NAME_inner2(a, b)
#define mk_FUNC_NAME(a) mk_FUNC_NAME_inner1(mk_PREFIX, a)

int mk_FUNC_NAME(ceil_div)(int dividend, int divisor) {
    return (dividend + divisor - 1) / divisor;
}
__global__ void mk_FUNC_NAME(kernel)(float* arr, int N) {
  int i = blockIdx.x * blockDim.x + threadIdx.x;
  if (i < N) {
    arr[i] = mk_MATH_FUNC(i);
  }
}

__global__ void mk_FUNC_NAME(init_seed)(curandState *state, int seed) {
  int idx = blockIdx.x * blockDim.x + threadIdx.x;
  curand_init(seed, idx, 0, &state[idx]);
}
__global__ void mk_FUNC_NAME(fillRand)(curandState *state, float *arr, int N) {
 int i = blockIdx.x * blockDim.x + threadIdx.x;
  if (i < N) {
    arr[i] = curand_uniform(&state[i]);
  }
}

void mk_FUNC_NAME(main)(int printTime, int correctness) {
  int deviceIdx = 0;
  cudaSetDevice(deviceIdx);
  int N = mk_SIZE;
  int block_size = 256;
  float* arr;
  cudaMalloc(&arr, N * sizeof(float));
  float grid_size = mk_FUNC_NAME(ceil_div)(N, block_size);

  curandState *d_state;
  cudaMalloc(&d_state, (N * sizeof(curandState)));
  mk_FUNC_NAME(init_seed)<<<grid_size, block_size>>>(d_state, time(NULL));
  mk_FUNC_NAME(fillRand)<<<grid_size, block_size>>>(d_state, arr, N);


  cudaEvent_t start, stop;
  cudaEventCreate(&start);
  cudaEventCreate(&stop);

  cudaEventRecord(start, nullptr);
  mk_FUNC_NAME(kernel)<<<grid_size, block_size>>>(arr, N);
  cudaDeviceSynchronize(); // Wait for the GPU to finish
  cudaEventRecord(stop, nullptr);
  cudaEventSynchronize(start);
  cudaEventSynchronize(stop);

  float elapsed;
  cudaEventElapsedTime(&elapsed, start, stop);
  if (printTime)
    printf(mk_LABEL" Time: %f ms\n", elapsed);

  if (correctness) {
    float* sum;
    cudaMalloc(&sum, sizeof(float));
    void *temp_storage = nullptr;
    size_t n_temp_storage = 0;
    cub::DeviceReduce::Sum(temp_storage, n_temp_storage, arr, sum, N);
    cudaMalloc(&temp_storage, n_temp_storage);
    cub::DeviceReduce::Sum(temp_storage, n_temp_storage, arr, sum, N);
    float sum_host;
    cudaMemcpy(&sum_host, sum, sizeof(float), cudaMemcpyDeviceToHost);
    printf(mk_LABEL" Sum: %f\n", sum_host);
  }
}
