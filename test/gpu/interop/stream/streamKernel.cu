#include <stdio.h>
#include <stdlib.h>
#include "cuda.h"
#include "cuda_runtime.h"

__global__ void vector_add_kernel(float *a, float *b, float *c, float alpha, int n) {
    int i = blockIdx.x * blockDim.x + threadIdx.x;
    a[i] = b[i] + alpha * c[i];
}

void checkForCudaDevices() {
    int nDevices;
    cudaError_t errCode = cudaGetDeviceCount(&nDevices);
    if(errCode != cudaSuccess) {
        printf("Error determining number of devices\n");
        exit(1);
    }
    printf("Number of Cuda devices: %i\n", nDevices);
}

void checkCudaError() {
     cudaError_t err = cudaGetLastError();
     if ( err != cudaSuccess ) {
        printf("CUDA Error: %s\n", cudaGetErrorString(err));       
     }
}

extern "C" void vector_add_cuda(float *a, float *b, float *c, float alpha, int n) {
    float *d_a, *d_b, *d_c;

    // Allocate device memory and transfer data from host to device
    cudaMalloc((void**)&d_a, sizeof(float) * n);
    cudaMalloc((void**)&d_b, sizeof(float) * n);
    cudaMalloc((void**)&d_c, sizeof(float) * n);

    cudaMemcpy(d_a, a, sizeof(float) * n, cudaMemcpyHostToDevice);
    cudaMemcpy(d_b, b, sizeof(float) * n, cudaMemcpyHostToDevice);
    cudaMemcpy(d_c, c, sizeof(float) * n, cudaMemcpyHostToDevice);

    // Execute the kernel and copy memory from device back to host
    checkCudaError();
    vector_add_kernel<<<1,n>>>(d_a, d_b, d_c, alpha, n);
    checkCudaError();
    cudaMemcpy(a, d_a, sizeof(float) * n, cudaMemcpyDeviceToHost);

    // Deallocate device memory
    cudaFree(d_a);
    cudaFree(d_b);
    cudaFree(d_c);
}
