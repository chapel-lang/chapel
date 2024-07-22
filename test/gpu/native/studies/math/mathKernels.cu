#include <stdio.h>
#include <stdlib.h>
#include <float.h>
#include <cuda_runtime.h>
#include <curand.h>
#include <curand_kernel.h>
#include <time.h>
#include <cub/cub.cuh>

#ifndef PREFIX
#define PREFIX
#endif

#ifndef LABEL
#define LABEL ""
#endif

#ifndef MATH_FUNC
#define MATH_FUNC tanh
#endif

#ifndef SIZE
#define SIZE 100'000'000
#endif

#ifndef ITERATIONS
#define ITERATIONS 1
#endif


#define FUNC_NAME(x) PREFIX##x

template<class T>
__host__ __device__ T FUNC_NAME(ceil_div)(T dividend, T divisor) {
    return (dividend + divisor-1) / divisor;
}
__global__ void FUNC_NAME(kernel)(float* arr, int N) {
  int i = blockIdx.x * blockDim.x + threadIdx.x;
  if (i < N){
    arr[i] = MATH_FUNC(i);
  }
}

__global__ void FUNC_NAME(init_seed)(curandState *state, int seed) {
  int idx = blockIdx.x * blockDim.x + threadIdx.x;
  curand_init(seed, idx, 0, &state[idx]);
}
__global__ void FUNC_NAME(fillRand)(curandState *state, float *arr, int N) {
 int i = blockIdx.x * blockDim.x + threadIdx.x;
  if (i < N) {
    arr[i] = curand_uniform(&state[i]);
  }
}

extern int reduce_gpu_array()

void FUNC_NAME(main)(int printTime, int correctness) {
  int deviceIdx = 0;
  cudaSetDevice(deviceIdx);
  int N = SIZE; // 100M elements
  int block_size = 256;
  float* arr;
  cudaMalloc(&arr, N * sizeof(float));
  float grid_size = ceil_div(N, block_size);

  curandState *d_state;
  cudaMalloc(&d_state, (N* sizeof(curandState)));
  FUNC_NAME(init_seed)<<<grid_size, block_size>>>(d_state, time(NULL));
  FUNC_NAME(fillRand)<<<grid_size, block_size>>>(d_state, arr, N);


  cudaEvent_t start, stop;
  cudaEventCreate(&start);
  cudaEventCreate(&stop);

  cudaEventRecord(start, nullptr);
  FUNC_NAME(kernel)<<<grid_size, block_size>>>(arr, N);
  cudaDeviceSynchronize(); // Wait for the GPU to finish
  cudaEventRecord(stop, nullptr);
  cudaEventSynchronize(start);
  cudaEventSynchronize(stop);

  float elapsed;
  cudaEventElapsedTime(&elapsed, start, stop);
  if (printTime) printf(LABEL"Time: %f ms\n", elapsed);

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
    printf(LABEL"Sum: %f\n", sum_host);
  }
}
