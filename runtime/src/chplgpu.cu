#ifdef __cplusplus
extern "C" {
#include <stdio.h>
#include "chplgpu.h"
	void gpuAllocate(void **data, size_t size) {
		cudaMalloc( data, size);
		printf("gpuAllocate: Allocated %u bytes at %p\n", (unsigned int)size, data, *data);
	}

	void gpuFree(void **data) {
		printf("Freed data : gpuFree at %p\n", data);
		cudaFree( *data);
	}

	void copyGPUtoHost(void **dst, void **src, size_t size) {
		printf("** Copying gpu to host : DST = %p SRC = %p SIZE = %u\n", *dst, *src, (unsigned int)size);
		cudaMemcpy( *dst, *src, size, cudaMemcpyDeviceToHost);
	}
	
	void copyHostToGPU(void **dst, void **src, size_t size) {
		printf("** Copying host to gpu : DST = %p SRC = %p SIZE = %u\n", *dst, *src, (unsigned int)size);
		cudaMemcpy( *dst, *src, size, cudaMemcpyHostToDevice);
		
	}
}
#endif
