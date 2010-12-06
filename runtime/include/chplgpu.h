#ifndef _GPU_H_
#define _GPU_H_

#define _GPU_GET_ARRAY(x) (x->_cudata)
#define _GPU_GET_VALUE(x, i) (&x->_cudata[i])
#define _GPU_GET_VAL(x, i) (&x[i])
#define _GPU_SHARED_ALLOC(name, numElem) __shared__ name[numElem]
#define _GPU_SHARED_GET_VAL(x, i) (x[i])
#define _GPU_THREADSYNC (__syncthreads())

//printf("allocating : %ld elements\n", numElem); 
#define _GPU_ALLOC(x, gpuclasstype, numElem, eltType) \
do { \
  gpuAllocate((void *)&(x->_cudata), numElem*sizeof(eltType)); \
} while(0);


  //printf("dst = %p and dst = %p\n", &((*dst)->_cudata), &((*dst)->_cudata[dst_offset]));
#define _GPU_COPY_HOST_GPU(dst, dst_offset, src, src_offset, type, size) \
do { \
  copyHostToGPU((void *)&((*dst)->_cudata[dst_offset]), (void *)&((*src)->_data[src_offset]), *size * sizeof(type));\
} while(0);
 
#define _GPU_COPY_GPU_GPU(dst, dst_offset, src, src_offset, type, size) \
do { \
  copyGPUtoGPU((void *)&((*dst)->_cudata[dst_offset]), (void *)&((*src)->_cudata[src_offset]), size * sizeof(type)); \
} while(0);
 
#define _GPU_COPY_GPU_HOST(dst, dst_offset, src, src_offset, type, size) \
do { \
  copyGPUtoHost((void *)&((*dst)->_data[dst_offset]), (void *)&((*src)->_cudata[src_offset]), *size * sizeof(type)); \
} while(0);

//  printf("dst = %p, src = %p size = %ld elements = %d\n", &((*dst)->_data), &((*src)->_data[src_offset]), *size * sizeof(type), *size); 
#define _GPU_COPY_HOST_HOST(dst, src, src_offset, type, size) \
do { \
  copyHosttoHost((void *)&((*dst)->_data[0]), (void *)&((*src)->_data[src_offset]), *size * sizeof(type)); \
} while(0);

  //copyToConstMem(dst, src, size*sizeof(type));
  //printf("Copying! : %p %p size = %ld totalsize = %ld\n", dst, src, base->array_size, base->array_size * sizeof(type));
#define _GPU_COPY_ARRAY_TO_CONST_MEM(dst, src, type, base) \
do { \
  copyToConstMem(dst, src, base->array_size*sizeof(type));\
} while(0);
  //printf("After copying!\n"); 

//  printf("Copying! : %p %p size = %d \n", dst, src, size); 
#define _GPU_COPY_SCALAR_TO_CONST_MEM(dst, src, size, type) \
do { \
  copyToConstMem(dst, src, size*sizeof(type)); \
} while(0);

#define _GPU_COPY_TO_CONST_MEM_EXPL(dst, src, size, type) \
do { \
  copyToConstMem(NULL, src, size*sizeof(type)); \
} while(0);

  //printf("HOST->GPU : %d elements %d sizeof %d total\n", (array)->array_size, sizeof(type), (array)->array_size * sizeof(type));
#define _IMPLICIT_HOST_TO_GPU_COPY(array, type) \
do { \
  printf("dst = %p src = %p\n", &((array)->_cudata[0]), &((array)->_data[0])); \
  copyHostToGPU((void *)&((array)->_cudata[0]), (void *)&((array)->_data[0]), (array)->array_size * sizeof(type)); \
} while(0);

  //printf("GPU->HOST : %d elements %d sizeof %d total\n", (array)->array_size, sizeof(type), (array)->array_size * sizeof(type));
#define _IMPLICIT_GPU_TO_HOST_COPY(array, type) \
do { \
  copyGPUtoHost((void *)&((array)->_data[0]), (void *)&((array)->_cudata[0]), (array)->array_size * sizeof(type)); \
} while(0);

#define _GPU_REDUCE(src, type, op) \
gpu_special_reduce##type(src->data->_cudata, src->size, op) 
//  gpu_special_reduce##type(NULL, src->size, op) 
  


#define _GPU_FREE(x) \
  gpuFree((void *)&(x->_cudata))

void gpuAllocate(void **data, size_t size);
void gpuFree(void **data);
void copyGPUtoHost(void *dst, void *src, size_t size);
void copyHosttoHost(void *dst, void *src, size_t size);
void copyGPUtoGPU(void *dst, void *src, size_t size);
void copyHostToGPU(void *dst, void *src, size_t size);
void copyToConstMem(char *dst, void *src, size_t size);
void chpl_init_accelerator(void);
int32_t gpu_special_reduceint32_t(int32_t *src, int size, const char *op);
float gpu_special_reduce_real32(float *src, int size, const char *op);
double gpu_special_reduce_real64(double *src, int size, const char *op);

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
  return blockIdx.z;
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
__device__ inline int getGridSize_x(void) {
  return gridDim.x;
}
__device__ inline int getGridSize_y(void) {
  return gridDim.y;
}
__device__ inline void thread_barrier(void) {
  __syncthreads();
}

#endif
#endif // _GPU_H
