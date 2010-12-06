#ifdef __cplusplus
extern "C" {
#include <stdio.h>
#include "chplgpu.h"

void gpuAllocate(void **data, size_t size) {
  printf("data = %p and size = %d\n", data, (int)size);
  cudaError_t err = cudaMalloc( data, size);
  if ( cudaSuccess != err) {
    fprintf(stderr, "gpuAllocate() Runtime API error in file <%s>, line %i : %s.\n",
                     __FILE__, __LINE__, cudaGetErrorString( err) );
    exit(-1);
  }
  //printf("gpuAllocate: Allocated %u bytes at %p\n", (unsigned int)size, *data);
}

void gpuFree(void **data) {
  cudaError_t err = cudaFree( *data);
  if ( cudaSuccess != err) {
    fprintf(stderr, "gpuFree() Runtime API error in file <%s>, line %i : %s.\n",
                     __FILE__, __LINE__, cudaGetErrorString( err) );
    exit(-1);
  }
}

void copyGPUtoHost(void *dst, void *src, size_t size) {
  //printf("** Copying gpu to host : DST = %p SRC = %p SIZE = %u\n", dst, src, (unsigned int)size);
  cudaError_t err = cudaMemcpy( dst, src, size, cudaMemcpyDeviceToHost);
  if ( cudaSuccess != err) {
    fprintf(stderr, "copyGPUtoHost() Runtime API error in file <%s>, line %i : %s.\n",
                     __FILE__, __LINE__, cudaGetErrorString( err) );
    exit(-1);
  }
}

void copyHosttoHost(void *dst, void *src, size_t size) {
  //printf("** Copying gpu to host : DST = %p SRC = %p SIZE = %u\n", dst, src, (unsigned int)size);
  cudaError_t err = cudaMemcpy( dst, src, size, cudaMemcpyHostToHost);
  if ( cudaSuccess != err) {
    fprintf(stderr, "copyGPUtoHost() Runtime API error in file <%s>, line %i : %s.\n",
                     __FILE__, __LINE__, cudaGetErrorString( err) );
    exit(-1);
  }
}

void copyGPUtoGPU(void *dst, void *src, size_t size) {
  cudaError_t err = cudaMemcpy( dst, src, size, cudaMemcpyDeviceToDevice);
  if ( cudaSuccess != err) {
    fprintf(stderr, "copyGPUtoGPU() Runtime API error in file <%s>, line %i : %s.\n",
                     __FILE__, __LINE__, cudaGetErrorString( err) );
    exit(-1);
  }
}
void copyHostToGPU(void *dst, void *src, size_t size) {
  printf("** Copying host to gpu : DST = %p SRC = %p SIZE = %u\n", dst, src, (unsigned int)size);
  cudaError_t err = cudaMemcpy( dst, src, size, cudaMemcpyHostToDevice);
  if ( cudaSuccess != err) {
    fprintf(stderr, "copyHostToGPU() Runtime API error in file <%s>, line %i : %s.\n",
                     __FILE__, __LINE__, cudaGetErrorString( err) );
    exit(-1);
  }
}

void copyToConstMem(char *symbol, void *src, size_t size) {
  cudaError_t err = cudaMemcpyToSymbol(symbol, src, size, 0, cudaMemcpyHostToDevice);
  if ( cudaSuccess != err) {
    fprintf(stderr, "copyToConstMem() Runtime API error in file <%s>, line %i : %s.\n",
                     __FILE__, __LINE__, cudaGetErrorString( err) );
    exit(-1);
  }
}

void chpl_init_accelerator(void) {
  cudaSetDevice( 0 ); // init device 0 (this is not clean)
}

}
#endif
