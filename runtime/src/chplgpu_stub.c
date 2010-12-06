#include <stdio.h>
#include <string.h>
#include "chplrt.h"
#include "chpl_mem.h"
#include "chplgpu.h"
void gpuAllocate(void **data, size_t size) {
  *data = chpl_alloc ( size, CHPL_RT_MD_CHAPEL_CODE, 0, 0 );
  printf("gpuAllocate: Allocated %u bytes at %p\n", (unsigned int)size, *data);
}

void gpuFree(void **data) {
  chpl_free( *data, 0, 0);
  //printf("Freed data : gpuFree (chpl_free) at %p\n", data);
}

void copyGPUtoGPU(void *dst, void *src, size_t size) {
  printf("*** STUB ***** : copyGPUtoGPU\n");
  memcpy( dst, src, size);
  //printf("** Copying gpu to host (memcpy): DST = %p SRC = %p SIZE = %u\n", *dst, *src, (unsigned int)size);
}

void copyGPUtoHost(void *dst, void *src, size_t size) {
  printf("*** STUB ***** : copyGPUtoHost\n");
  memcpy( dst, src, size);
  //printf("** Copying gpu to host (memcpy): DST = %p SRC = %p SIZE = %u\n", *dst, *src, (unsigned int)size);
}

void copyHostToGPU(void *dst, void *src, size_t size) {
  printf("*** STUB ***** : copyHostToGPU\n");
  memcpy( dst, src, size);
  printf("** Copying host to gpu\n");
}

void copyToConstMem(char *dst, void *src, size_t size) {
}

void chpl_init_accelerator(void) {
  /* Do nothing here since this is a stub */
  printf("Initializing device 0\n");
}
