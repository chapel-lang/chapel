#include "chplrt.h"

#include "chpl-mem.h"
#include "chplgpu.h"

#include <stdio.h>
#include <string.h>

void gpuAllocate(void **data, size_t size) {
  *data = chpl_mem_alloc ( size, CHPL_RT_MD_CHAPEL_CODE, 0, 0 );
  printf("gpuAllocate: Allocated %u bytes at %p\n", (unsigned int)size, *data);
}

void gpuFree(void **data) {
  chpl_mem_free( *data, 0, 0);
  //printf("Freed data : gpuFree (chpl_mem_free) at %p\n", data);
  printf("Freed data : gpuFree\n");
}

void copyGPUtoHost(void **dst, void **src, size_t size) {
  memcpy( *dst, *src, size);
  //printf("** Copying gpu to host (memcpy): DST = %p SRC = %p SIZE = %u\n", *dst, *src, (unsigned int)size);
  printf("** Copying gpu to host\n");
}

void copyHostToGPU(void **dst, void **src, size_t size) {
  memcpy( *dst, *src, size);
  printf("** Copying host to gpu\n");
  //printf("** Copying host to gpu (memcpy): DST = %p SRC = %p SIZE = %u\n", *dst, *src, (unsigned int)size);
}

void chpl_init_accelerator(void) {
  /* Do nothing here since this is a stub */
  printf("Initializing device 0\n");
}
