#include <stdio.h>
#include <stdlib.h>
#include <cuda_runtime.h>
#include <time.h>
#include "cublas_v2.h"

int main(int argc, char** argv){

  int N = atoi(argv[1]);
  float *x, *y;
  cublasHandle_t handle;
  clock_t start;
  clock_t end;
  const float alpha = 2.0f;

  //allocate memory `accessible` on CPU and GPU
  cudaMallocManaged(&x, N*sizeof(float), cudaMemAttachGlobal);
  cudaMallocManaged(&y, N*sizeof(float), cudaMemAttachGlobal);

  printf("N: %i\n", N);
  for (int i = 0; i < N; i++) {
    x[i] = 3.0f;
    y[i] = 5.0f;
  }

  //initialize cuBLAS context
  cublasCreate(&handle);

  //prefetch data to gpu
  //int device = -1;
  //cudaGetDevice(&device);
  //cudaMemPrefetchAsync(x, N*sizeof(float), device, NULL);
  //cudaMemPrefetchAsync(y, N*sizeof(float), device, NULL);

  start = clock();
  cublasSaxpy(handle, N, &alpha, x, 1, y, 1);
  cudaDeviceSynchronize();
  end = clock();

  // Check for error
  float maxError = 0.0f;
  for (int i = 0; i < N; i++){
      maxError = fmax(maxError, abs(y[i]-11.0f));}
  printf("Max error: %f\n", maxError);
  printf("Time for kernel=%f\n", (double) (end-start)*1000/ CLOCKS_PER_SEC);

  FILE *fp;
  fp = fopen("c_cublas_times.log","a+");
  fprintf(fp, "%f\n", (double) (end-start)*1000/ CLOCKS_PER_SEC);
  fclose(fp);

  cudaFree(x);
  cudaFree(y);
  
  cublasDestroy(handle);

  return 0;
}
