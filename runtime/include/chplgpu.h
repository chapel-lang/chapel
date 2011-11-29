#ifndef _GPU_H_
#define _GPU_H_

#define _GPU_GET_ARRAY(x) (x->_data)
#define _GPU_GET_VALUE(x, i) (&x->_data[i])
#define _GPU_GET_VAL(x, i) (&x[i])

#define _GPU_ALLOC(x, gpuclasstype, numElem, eltType) \
do { \
  gpuAllocate((void *)&(x->_data), numElem*sizeof(eltType)); \
} while(0);

#define _GPU_COPY_HOST_GPU(dst, src, type, size) \
do { \
  copyHostToGPU((void *)&((*dst)->_data), (void *)&((*src)->_data), *size * sizeof(type)); \
} while(0);
  
#define _GPU_COPY_GPU_HOST(dst, src, type, size) \
do { \
  copyGPUtoHost((void *)&((*dst)->_data), (void *)&((*src)->_data), *size * sizeof(type)); \
} while(0);

#define _GPU_FREE(x) \
  gpuFree((void *)&(x->_data))

void gpuAllocate(void **data, size_t size);
void gpuFree(void **data);
void copyGPUtoHost(void **dst, void **src, size_t size);
void copyHostToGPU(void **dst, void **src, size_t size);
void chpl_init_accelerator(void);
#ifdef ENABLE_GPU
__device__ inline int getThreadID_x(void) {
  return threadIdx.x;
}
__device__ inline int getThreadID_y(void) {
  return threadIdx.y;
}
__device__ inline int getThreadID_z(void) {
  return threadIdx.z;
}
__device__ inline int getBlockID_x(void) {
  return blockIdx.x;
}
__device__ inline int getBlockID_y(void) {
  return blockIdx.y;
}
__device__ inline int getBlockID_z(void) {
  return blockIdx.y;
}
__device__ inline int getBlockSize_x(void) {
  return blockDim.x;
}
__device__ inline int getBlockSize_y(void) {
  return blockDim.y;
}
__device__ inline int getBlockSize_z(void) {
  return blockDim.z;
}
#endif
#endif // _GPU_H
