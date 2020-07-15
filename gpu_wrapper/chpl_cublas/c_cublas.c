#include <cuda_runtime.h>
#include <stdio.h>
#include <stdlib.h>
#include  "cublas_v2.h"

float* cublas_array(size_t size){

    float *ptr;
    cudaError_t err = cudaMallocManaged(&ptr, size*sizeof(float), cudaMemAttachGlobal);
    //printf("CUDA Error: %s\n", cudaGetErrorString(err));
    cudaDeviceSynchronize();
    return ptr;

}

void* cublas_create(){

    cublasHandle_t *handle = (cublasHandle_t*)malloc(sizeof(cublasHandle_t));
    cublasStatus_t err = cublasCreate(handle);
    //printf("cublasCreate status: %s\n", cudaGetErrorString(err));
    return handle;

}

void cublas_destroy(cublasHandle_t *handle){

    cublasStatus_t err = cublasDestroy(*handle);
    //printf("cublasDestroy status: %s\n", cudaGetErrorString(err));

}


int cublas_saxpy(cublasHandle_t *handle, int N, float alpha, float *x, int incX, float *y, int incY){

    cublasSaxpy(*handle, N, &alpha, x, 1, y, 1);
    cudaDeviceSynchronize();
    return 0;

}
