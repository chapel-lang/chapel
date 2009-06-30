#ifndef _GPU_H_
#define _GPU_H_

#define _GPU_GET_ARRAY(x) (&((*x)->_data))
#define _GPU_GET_VALUE(x, i) (&((x)->_data[i]))

#define _GPU_ALLOC(x, type, size) \
  gpuAllocate((void **)&(x->_data), size*sizeof(type))

#define _GPU_COPY_HOST_GPU(dst, src, type, size) \
  copyHostToGPU((void **)_GPU_GET_ARRAY(dst), (void **)_GPU_GET_ARRAY(src), (*size) * sizeof(type))

#define _GPU_COPY_GPU_HOST(dst, src, type, size) \
  copyGPUtoHost((void **)_GPU_GET_ARRAY(dst), (void **)_GPU_GET_ARRAY(src), (*size) * sizeof(type))

#define _GPU_FREE(x) \
  gpuFree((void **)&(x->_data))

void gpuAllocate(void **data, size_t size);
void gpuFree(void **data);
void copyGPUtoHost(void **dst, void **src, size_t size);
void copyHostToGPU(void **dst, void **src, size_t size);

#endif // _GPU_H
