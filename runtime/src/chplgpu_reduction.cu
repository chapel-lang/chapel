#ifdef __cplusplus
#include <typeinfo>
enum ReduceOp {
    MAX,
    MIN,
    SUM,
    PROD,
    BITAND,
    BITOR,
    BITXOR,
    LOGAND,
    LOGOR
  };

#include "sharedmem.cuh"
template <class T>
void 
cuda_reduce(int size, int threads, int blocks, 
    int whichKernel, const ReduceOp op, T *d_idata, T *d_odata);

template <class T, unsigned int blockSize>
__global__ void
reduce5(ReduceOp op, T *g_idata, T *g_odata, unsigned int n);

template <class T, unsigned int blockSize, bool nIsPow2>
__global__ void
reduce6(ReduceOp op, T *g_idata, T *g_odata, unsigned int n);

extern "C" {
#include <stdio.h>
#include "chplgpu.h"

#ifdef __DEVICE_EMULATION__
#define EMUSYNC __syncthreads()
#else
#define EMUSYNC
#endif

  bool isPow2(unsigned int x)
  {
    return ((x&(x-1))==0);
  }

  unsigned int nextPow2( unsigned int x ) {
    --x;
    x |= x >> 1;
    x |= x >> 2;
    x |= x >> 4;
    x |= x >> 8;
    x |= x >> 16;
    return ++x;
  }

  ////////////////////////////////////////////////////////////////////////////////
  // Compute the number of threads and blocks to use for the given reduction kernel
  // For the kernels >= 3, we set threads / block to the minimum of maxThreads and
  // n/2. For kernels < 3, we set to the minimum of maxThreads and n.  For kernel 
  // 6, we observe the maximum specified number of blocks, because each thread in 
  // that kernel can process a variable number of elements.
  ////////////////////////////////////////////////////////////////////////////////
#define MAXBLOCKS 64
#define MAXTHREADS 128
  void getNumBlocksAndThreads(int whichKernel, int n, int &blocks, int &threads)
  {
    threads = (n < MAXTHREADS*2) ? nextPow2((n + 1)/ 2) : MAXTHREADS;
    blocks = (n + (threads * 2 - 1)) / (threads * 2);
    if (whichKernel == 6)
      blocks = min(MAXBLOCKS, blocks);
  }
}

  template <class T>
T _internalReduce_real(int  n, 
    int  numThreads,
    int  numBlocks,
    int whichKernel,
    const ReduceOp op,
    T* h_odata,
    T* d_idata, 
    T* d_odata)
{
  T gpu_result = 0;
  bool needReadBack = true;
  gpu_result = 0;

  cudaThreadSynchronize();

  // execute the kernel
  cuda_reduce<T>(n, numThreads, numBlocks, whichKernel, op, d_idata, d_odata);

  // check if kernel execution generated an error
  //cutilCheckMsg("Kernel execution failed");

  // sum partial block sums on GPU
  int s=numBlocks;
  int kernel = (whichKernel == 6) ? 5 : whichKernel;
  while(s > 1) 
  {
    int threads = 0, blocks = 0;
    getNumBlocksAndThreads(kernel, s, blocks, threads);

    cuda_reduce<T>(s, threads, blocks, kernel, op, d_odata, d_odata);

    s = (s + (threads*2-1)) / (threads*2);
  }

  if (s > 1)
  {
    // copy result from device to host
    cudaMemcpy( h_odata, d_odata, s * sizeof(T), cudaMemcpyDeviceToHost);
    switch (op) {
      case MIN :
        for(int i=0; i < s; i++) gpu_result = min(gpu_result, h_odata[i]);
        break;
      case MAX :
        for(int i=0; i < s; i++) gpu_result = max(gpu_result, h_odata[i]);
        break;
      case SUM :
        for(int i=0; i < s; i++) gpu_result += h_odata[i];
        break;
      case PROD:
        for(int i=0; i < s; i++) gpu_result *= h_odata[i];
        break;
      case LOGAND:
        for(int i=0; i < s; i++) gpu_result = gpu_result && h_odata[i];
        break;
      case LOGOR:
        for(int i=0; i < s; i++) gpu_result = gpu_result || h_odata[i];
        break;
    }

    needReadBack = false;
  }

  cudaThreadSynchronize();

  if (needReadBack)
  {
    // copy final sum from device to host
    cudaMemcpy( &gpu_result, d_odata, sizeof(T), cudaMemcpyDeviceToHost);
  }

  return gpu_result;
}


  template <class T>
T _internalReduce_int(int  n, 
    int  numThreads,
    int  numBlocks,
    int whichKernel,
    const ReduceOp op,
    T* h_odata,
    T* d_idata, 
    T* d_odata)
{
  T gpu_result = 0;
  bool needReadBack = true;
  gpu_result = 0;

  cudaThreadSynchronize();

  // execute the kernel
  cuda_reduce<T>(n, numThreads, numBlocks, whichKernel, op, d_idata, d_odata);

  // check if kernel execution generated an error
  //cutilCheckMsg("Kernel execution failed");

  // sum partial block sums on GPU
  int s=numBlocks;
  int kernel = (whichKernel == 6) ? 5 : whichKernel;
  while(s > 1) 
  {
    int threads = 0, blocks = 0;
    getNumBlocksAndThreads(kernel, s, blocks, threads);

    cuda_reduce<T>(s, threads, blocks, kernel, op, d_odata, d_odata);

    s = (s + (threads*2-1)) / (threads*2);
  }

  if (s > 1)
  {
    // copy result from device to host
    cudaMemcpy( h_odata, d_odata, s * sizeof(T), cudaMemcpyDeviceToHost);
    switch (op) {
      case MIN :
        for(int i=0; i < s; i++) gpu_result = min(gpu_result, h_odata[i]);
        break;
      case MAX :
        for(int i=0; i < s; i++) gpu_result = max(gpu_result, h_odata[i]);
        break;
      case SUM :
        for(int i=0; i < s; i++) gpu_result += h_odata[i];
        break;
      case PROD:
        for(int i=0; i < s; i++) gpu_result *= h_odata[i];
        break;
      case BITAND:
  //      if (!isFloat)
  //        for(int i=0; i < s; i++) gpu_result &= h_odata[i];
        break;
      case BITOR:
  //      if (!isFloat)
  //        for(int i=0; i < s; i++) gpu_result |= h_odata[i];
        break;
      case BITXOR:
  //      if (!isFloat)
  //        for(int i=0; i < s; i++) gpu_result ^= h_odata[i];
        break;
      case LOGAND:
        for(int i=0; i < s; i++) gpu_result = gpu_result && h_odata[i];
        break;
      case LOGOR:
        for(int i=0; i < s; i++) gpu_result = gpu_result || h_odata[i];
        break;
    }

    needReadBack = false;
  }

  cudaThreadSynchronize();

  if (needReadBack)
  {
    // copy final sum from device to host
    cudaMemcpy( &gpu_result, d_odata, sizeof(T), cudaMemcpyDeviceToHost);
  }

  return gpu_result;
}

template <class T>
  void 
cuda_reduce(int size, int threads, int blocks, 
    int whichKernel, const ReduceOp op, T *d_idata, T *d_odata)
{
  dim3 dimBlock(threads, 1, 1);
  dim3 dimGrid(blocks, 1, 1);
  int smemSize = threads * sizeof(T);

  // choose which of the optimized versions of reduction to launch
  switch (whichKernel)
  {
    case 5:
      switch (threads)
      {
        case 512:
          reduce5<T, 512><<< dimGrid, dimBlock, smemSize >>>(op, d_idata, d_odata, size); break;
        case 256:
          reduce5<T, 256><<< dimGrid, dimBlock, smemSize >>>(op, d_idata, d_odata, size); break;
        case 128:
          reduce5<T, 128><<< dimGrid, dimBlock, smemSize >>>(op, d_idata, d_odata, size); break;
        case 64:
          reduce5<T, 64><<< dimGrid, dimBlock, smemSize >>>(op, d_idata, d_odata, size); break;
        case 32:
          reduce5<T, 32><<< dimGrid, dimBlock, smemSize >>>(op, d_idata, d_odata, size); break;
        case 16:
          reduce5<T, 16><<< dimGrid, dimBlock, smemSize >>>(op, d_idata, d_odata, size); break;
        case  8:
          reduce5<T, 8><<< dimGrid, dimBlock, smemSize >>>(op, d_idata, d_odata, size); break;
        case  4:
          reduce5<T, 4><<< dimGrid, dimBlock, smemSize >>>(op, d_idata, d_odata, size); break;
        case  2:
          reduce5<T, 2><<< dimGrid, dimBlock, smemSize >>>(op, d_idata, d_odata, size); break;
        case  1:
          reduce5<T, 1><<< dimGrid, dimBlock, smemSize >>>(op, d_idata, d_odata, size); break;
      }
      break;       
    case 6:
    default:
      if (isPow2(size))
      {
        switch (threads)
        {
          case 512:
            reduce6<T, 512, true><<< dimGrid, dimBlock, smemSize >>>(op, d_idata, d_odata, size); break;
          case 256:
            reduce6<T, 256, true><<< dimGrid, dimBlock, smemSize >>>(op, d_idata, d_odata, size); break;
          case 128:
            reduce6<T, 128, true><<< dimGrid, dimBlock, smemSize >>>(op, d_idata, d_odata, size); break;
          case 64:
            reduce6<T,  64, true><<< dimGrid, dimBlock, smemSize >>>(op, d_idata, d_odata, size); break;
          case 32:
            reduce6<T,  32, true><<< dimGrid, dimBlock, smemSize >>>(op, d_idata, d_odata, size); break;
          case 16:
            reduce6<T,  16, true><<< dimGrid, dimBlock, smemSize >>>(op, d_idata, d_odata, size); break;
          case  8:
            reduce6<T,   8, true><<< dimGrid, dimBlock, smemSize >>>(op, d_idata, d_odata, size); break;
          case  4:
            reduce6<T,   4, true><<< dimGrid, dimBlock, smemSize >>>(op, d_idata, d_odata, size); break;
          case  2:
            reduce6<T,   2, true><<< dimGrid, dimBlock, smemSize >>>(op, d_idata, d_odata, size); break;
          case  1:
            reduce6<T,   1, true><<< dimGrid, dimBlock, smemSize >>>(op, d_idata, d_odata, size); break;
        }
      }
      else
      {
        switch (threads)
        {
          case 512:
            reduce6<T, 512, false><<< dimGrid, dimBlock, smemSize >>>(op, d_idata, d_odata, size); break;
          case 256:
            reduce6<T, 256, false><<< dimGrid, dimBlock, smemSize >>>(op, d_idata, d_odata, size); break;
          case 128:
            reduce6<T, 128, false><<< dimGrid, dimBlock, smemSize >>>(op, d_idata, d_odata, size); break;
          case 64:
            reduce6<T,  64, false><<< dimGrid, dimBlock, smemSize >>>(op, d_idata, d_odata, size); break;
          case 32:
            reduce6<T,  32, false><<< dimGrid, dimBlock, smemSize >>>(op, d_idata, d_odata, size); break;
          case 16:
            reduce6<T,  16, false><<< dimGrid, dimBlock, smemSize >>>(op, d_idata, d_odata, size); break;
          case  8:
            reduce6<T,   8, false><<< dimGrid, dimBlock, smemSize >>>(op, d_idata, d_odata, size); break;
          case  4:
            reduce6<T,   4, false><<< dimGrid, dimBlock, smemSize >>>(op, d_idata, d_odata, size); break;
          case  2:
            reduce6<T,   2, false><<< dimGrid, dimBlock, smemSize >>>(op, d_idata, d_odata, size); break;
          case  1:
            reduce6<T,   1, false><<< dimGrid, dimBlock, smemSize >>>(op, d_idata, d_odata, size); break;
        }
      }
      break;       
  }
}

  float gpu_special_reduce_real32(float *src, int size, const char *op) {
    int numBlocks = 0;
    int numThreads = 0;
    int whichKernel = 6;
    ReduceOp opType;

    if (!strcasecmp(op, "Max"))
      opType = MAX;
    else if (!strcasecmp(op, "Min"))
      opType = MIN;
    else if (!strcasecmp(op, "Sum"))
      opType = SUM;
    else if (!strcasecmp(op, "Product"))
      opType = PROD;
    else if (!strcasecmp(op, "LogicalAnd"))
      opType = LOGAND;
    else if (!strcasecmp(op, "LogicalOr"))
      opType = LOGOR;
    else if (!strcasecmp(op, "BitwiseAnd"))
      opType = BITAND;
    else if (!strcasecmp(op, "BitwiseOr"))
      opType = BITOR;
    else if (!strcasecmp(op, "BitwiseXor"))
      opType = BITXOR;

    getNumBlocksAndThreads(whichKernel, size, numBlocks, numThreads);

  // allocate mem for the result on host side
  float* h_odata = (float*) malloc(numBlocks*sizeof(float));
  // allocate device memory and data
  float* d_odata = NULL;

  cudaMalloc((void**) &d_odata, numBlocks*sizeof(float));

  // copy data directly to device memory
  cudaMemcpy(d_odata, src, numBlocks*sizeof(float), cudaMemcpyDeviceToDevice);

  float gpu_result = _internalReduce_real<float>(size, numThreads, numBlocks, whichKernel, opType,
      h_odata, src, d_odata);

  return gpu_result;

  }

  double gpu_special_reduce_real64(double *src, int size, const char *op) {
    int numBlocks = 0;
    int numThreads = 0;
    int whichKernel = 6;

    ReduceOp opType = MAX;
    printf("REDUCE : op type = %s\n", op);
    if (!strcasecmp(op, "Min"))
      opType = MIN;
    else if (!strcasecmp(op, "Sum"))
      opType = SUM;
    else if (!strcasecmp(op, "Product"))
      opType = PROD;
    else if (!strcasecmp(op, "LogicalAnd"))
      opType = LOGAND;
    else if (!strcasecmp(op, "LogicalOr"))
      opType = LOGOR;
    else if (!strcasecmp(op, "BitwiseAnd"))
      opType = BITAND;
    else if (!strcasecmp(op, "BitwiseOr"))
      opType = BITOR;
    else if (!strcasecmp(op, "BitwiseXor"))
      opType = BITXOR;

    getNumBlocksAndThreads(whichKernel, size, numBlocks, numThreads);

  // allocate mem for the result on host side
  double* h_odata = (double*) malloc(numBlocks*sizeof(double));
  // allocate device memory and data
  double* d_odata = NULL;

  cudaMalloc((void**) &d_odata, numBlocks*sizeof(double));

  // copy data directly to device memory
  cudaMemcpy(d_odata, src, numBlocks*sizeof(double), cudaMemcpyDeviceToDevice);

  double gpu_result = _internalReduce_real<double>(size, numThreads, numBlocks, whichKernel, opType,
      h_odata, src, d_odata);

  return gpu_result;

}

int32_t gpu_special_reduceint32_t(int32_t *src, int size, const char *op) {
  int numBlocks = 0;
  int numThreads = 0;
  int whichKernel = 6;
  ReduceOp opType;

  if (!strcasecmp(op, "Max"))
    opType = MAX;
  else if (!strcasecmp(op, "Min"))
    opType = MIN;
  else if (!strcasecmp(op, "Sum"))
    opType = SUM;
  else if (!strcasecmp(op, "Product"))
    opType = PROD;
  else if (!strcasecmp(op, "LogicalAnd"))
    opType = LOGAND;
  else if (!strcasecmp(op, "LogicalOr"))
    opType = LOGOR;
  else if (!strcasecmp(op, "BitwiseAnd"))
    opType = BITAND;
  else if (!strcasecmp(op, "BitwiseOr"))
    opType = BITOR;
  else if (!strcasecmp(op, "BitwiseXor"))
    opType = BITXOR;

  getNumBlocksAndThreads(whichKernel, size, numBlocks, numThreads);

  // allocate mem for the result on host side
  int32_t* h_odata = (int32_t*) malloc(numBlocks*sizeof(int32_t));
  // allocate device memory and data
  int32_t* d_odata = NULL;

  cudaMalloc((void**) &d_odata, numBlocks*sizeof(int32_t));

  // copy data directly to device memory
  cudaMemcpy(d_odata, src, numBlocks*sizeof(int32_t), cudaMemcpyDeviceToDevice);

  int32_t gpu_result = _internalReduce_int<int32_t>(size, numThreads, numBlocks, whichKernel, opType,
      h_odata, src, d_odata);

  return gpu_result;
}

template <class T, unsigned int blockSize>
  __global__ void
reduce5(ReduceOp op, T *g_idata, T *g_odata, unsigned int n)
{
  SharedMemory<T> smem;
  T *sdata = smem.getPointer();

  // perform first level of reduction,
  // reading from global memory, writing to shared memory
  unsigned int tid = threadIdx.x;
  unsigned int i = blockIdx.x*(blockSize*2) + threadIdx.x;

  sdata[tid] = (i < n) ? g_idata[i] : 0;
  switch (op) {
    case SUM :
      if (i + blockSize < n) 
        sdata[tid] += g_idata[i+blockSize];  

      __syncthreads();

      // do reduction in shared mem
      if (blockSize >= 512) { if (tid < 256) { sdata[tid] += sdata[tid + 256]; } __syncthreads(); }
      if (blockSize >= 256) { if (tid < 128) { sdata[tid] += sdata[tid + 128]; } __syncthreads(); }
      if (blockSize >= 128) { if (tid <  64) { sdata[tid] += sdata[tid +  64]; } __syncthreads(); }

#ifndef __DEVICE_EMULATION__
      if (tid < 32)
#endif
      {
        if (blockSize >=  64) { sdata[tid] += sdata[tid + 32]; EMUSYNC; }
        if (blockSize >=  32) { sdata[tid] += sdata[tid + 16]; EMUSYNC; }
        if (blockSize >=  16) { sdata[tid] += sdata[tid +  8]; EMUSYNC; }
        if (blockSize >=   8) { sdata[tid] += sdata[tid +  4]; EMUSYNC; }
        if (blockSize >=   4) { sdata[tid] += sdata[tid +  2]; EMUSYNC; }
        if (blockSize >=   2) { sdata[tid] += sdata[tid +  1]; EMUSYNC; }
      }
      break;
    case PROD :
      if (i + blockSize < n) 
        sdata[tid] *= g_idata[i+blockSize];  

      __syncthreads();

      // do reduction in shared mem
      if (blockSize >= 512) { if (tid < 256) { sdata[tid] *= sdata[tid + 256]; } __syncthreads(); }
      if (blockSize >= 256) { if (tid < 128) { sdata[tid] *= sdata[tid + 128]; } __syncthreads(); }
      if (blockSize >= 128) { if (tid <  64) { sdata[tid] *= sdata[tid +  64]; } __syncthreads(); }

#ifndef __DEVICE_EMULATION__
      if (tid < 32)
#endif
      {
        if (blockSize >=  64) { sdata[tid] *= sdata[tid + 32]; EMUSYNC; }
        if (blockSize >=  32) { sdata[tid] *= sdata[tid + 16]; EMUSYNC; }
        if (blockSize >=  16) { sdata[tid] *= sdata[tid +  8]; EMUSYNC; }
        if (blockSize >=   8) { sdata[tid] *= sdata[tid +  4]; EMUSYNC; }
        if (blockSize >=   4) { sdata[tid] *= sdata[tid +  2]; EMUSYNC; }
        if (blockSize >=   2) { sdata[tid] *= sdata[tid +  1]; EMUSYNC; }
      }
      break;
    case MAX :
      if (i + blockSize < n) 
        sdata[tid] = max(sdata[tid], g_idata[i+blockSize]);  

      __syncthreads();

      // do reduction in shared mem
      if (blockSize >= 512) { if (tid < 256) { sdata[tid] = max(sdata[tid], sdata[tid + 256]); } __syncthreads(); }
      if (blockSize >= 256) { if (tid < 128) { sdata[tid] = max(sdata[tid], sdata[tid + 128]); } __syncthreads(); }
      if (blockSize >= 128) { if (tid < 64) { sdata[tid] = max(sdata[tid], sdata[tid + 64]); } __syncthreads(); }

#ifndef __DEVICE_EMULATION__
      if (tid < 32)
#endif
      {
        if (blockSize >=  64) { sdata[tid] = max(sdata[tid], sdata[tid + 32]); EMUSYNC; }
        if (blockSize >=  32) { sdata[tid] = max(sdata[tid], sdata[tid + 16]); EMUSYNC; }
        if (blockSize >=  16) { sdata[tid] = max(sdata[tid], sdata[tid + 8]); EMUSYNC; }
        if (blockSize >=  8) { sdata[tid] = max(sdata[tid], sdata[tid + 4]); EMUSYNC; }
        if (blockSize >=  4) { sdata[tid] = max(sdata[tid], sdata[tid + 2]); EMUSYNC; }
        if (blockSize >=  2) { sdata[tid] = max(sdata[tid], sdata[tid + 1]); EMUSYNC; }
      }
      break;
    case MIN:
      if (i + blockSize < n) 
        sdata[tid] = min(sdata[tid], g_idata[i+blockSize]);  

      __syncthreads();

      // do reduction in shared mem
      if (blockSize >= 512) { if (tid < 256) { sdata[tid] = min(sdata[tid], sdata[tid + 256]); } __syncthreads(); }
      if (blockSize >= 256) { if (tid < 128) { sdata[tid] = min(sdata[tid], sdata[tid + 128]); } __syncthreads(); }
      if (blockSize >= 128) { if (tid < 64) { sdata[tid] = min(sdata[tid], sdata[tid + 64]); } __syncthreads(); }

#ifndef __DEVICE_EMULATION__
      if (tid < 32)
#endif
      {
        if (blockSize >=  64) { sdata[tid] = min(sdata[tid], sdata[tid + 32]); EMUSYNC; }
        if (blockSize >=  32) { sdata[tid] = min(sdata[tid], sdata[tid + 16]); EMUSYNC; }
        if (blockSize >=  16) { sdata[tid] = min(sdata[tid], sdata[tid + 8]); EMUSYNC; }
        if (blockSize >=  8) { sdata[tid] = min(sdata[tid], sdata[tid + 4]); EMUSYNC; }
        if (blockSize >=  4) { sdata[tid] = min(sdata[tid], sdata[tid + 2]); EMUSYNC; }
        if (blockSize >=  2) { sdata[tid] = min(sdata[tid], sdata[tid + 1]); EMUSYNC; }
      }
      break;
  }

  // write result for this block to global mem 
  if (tid == 0) g_odata[blockIdx.x] = sdata[0];
}

/*
   This version adds multiple elements per thread sequentially.  This reduces the overall
   cost of the algorithm while keeping the work complexity O(n) and the step complexity O(log n).
   (Brent's Theorem optimization)
 */
template <class T, unsigned int blockSize, bool nIsPow2>
  __global__ void
reduce6(ReduceOp op, T *g_idata, T *g_odata, unsigned int n)
{
  SharedMemory<T> smem;
  T *sdata = smem.getPointer();

  // perform first level of reduction,
  // reading from global memory, writing to shared memory
  unsigned int tid = threadIdx.x;
  unsigned int i = blockIdx.x*(blockSize*2) + threadIdx.x;
  unsigned int gridSize = blockSize*2*gridDim.x;

  // we reduce multiple elements per thread.  The number is determined by the 
  // number of active thread blocks (via gridDim).  More blocks will result
  // in a larger gridSize and therefore fewer elements per thread
  switch (op)
  {
    case SUM :
      sdata[tid] = 0;
      while (i < n)
      {         
        sdata[tid] += g_idata[i];
        // ensure we don't read out of bounds -- this is optimized away for powerOf2 sized arrays
        if (nIsPow2 || i + blockSize < n) 
          sdata[tid] += g_idata[i+blockSize];  
        i += gridSize;
      } 
      __syncthreads();

      // do reduction in shared mem
      if (blockSize >= 512) { if (tid < 256) { sdata[tid] += sdata[tid + 256]; } __syncthreads(); }
      if (blockSize >= 256) { if (tid < 128) { sdata[tid] += sdata[tid + 128]; } __syncthreads(); }
      if (blockSize >= 128) { if (tid <  64) { sdata[tid] += sdata[tid +  64]; } __syncthreads(); }

#ifndef __DEVICE_EMULATION__
      if (tid < 32)
#endif
      {
        if (blockSize >=  64) { sdata[tid] += sdata[tid + 32]; EMUSYNC; }
        if (blockSize >=  32) { sdata[tid] += sdata[tid + 16]; EMUSYNC; }
        if (blockSize >=  16) { sdata[tid] += sdata[tid +  8]; EMUSYNC; }
        if (blockSize >=   8) { sdata[tid] += sdata[tid +  4]; EMUSYNC; }
        if (blockSize >=   4) { sdata[tid] += sdata[tid +  2]; EMUSYNC; }
        if (blockSize >=   2) { sdata[tid] += sdata[tid +  1]; EMUSYNC; }
      }
      break;
    case MAX :
      sdata[tid] = INT_MIN;
      while (i < n)
      {         
        sdata[tid] = max(sdata[tid], g_idata[i]);
        // ensure we don't read out of bounds -- this is optimized away for powerOf2 sized arrays
        if (nIsPow2 || i + blockSize < n) 
          sdata[tid] = max(sdata[tid], g_idata[i+blockSize]);
        i += gridSize;
      } 
      __syncthreads();

      // do reduction in shared mem
      if (blockSize >= 512) { if (tid < 256) { sdata[tid] = max(sdata[tid], sdata[tid + 256]); } __syncthreads(); }
      if (blockSize >= 256) { if (tid < 128) { sdata[tid] = max(sdata[tid], sdata[tid + 128]); } __syncthreads(); }
      if (blockSize >= 128) { if (tid < 64) { sdata[tid] = max(sdata[tid], sdata[tid + 64]); } __syncthreads(); }

#ifndef __DEVICE_EMULATION__
      if (tid < 32)
#endif
      {
        if (blockSize >=  64) { sdata[tid] = max(sdata[tid], sdata[tid + 32]); EMUSYNC; }
        if (blockSize >=  32) { sdata[tid] = max(sdata[tid], sdata[tid + 16]); EMUSYNC; }
        if (blockSize >=  16) { sdata[tid] = max(sdata[tid], sdata[tid + 8]); EMUSYNC; }
        if (blockSize >=  8) { sdata[tid] = max(sdata[tid], sdata[tid + 4]); EMUSYNC; }
        if (blockSize >=  4) { sdata[tid] = max(sdata[tid], sdata[tid + 2]); EMUSYNC; }
        if (blockSize >=  2) { sdata[tid] = max(sdata[tid], sdata[tid + 1]); EMUSYNC; }
      }
      break;
    case MIN:
      sdata[tid] = INT_MAX;
      while (i < n)
      {         
        sdata[tid] = min(sdata[tid], g_idata[i]);
        // ensure we don't read out of bounds -- this is optimized away for powerOf2 sized arrays
        if (nIsPow2 || i + blockSize < n) 
          sdata[tid] = min(sdata[tid], g_idata[i+blockSize]);
        i += gridSize;
      } 
      __syncthreads();

      // do reduction in shared mem
      if (blockSize >= 512) { if (tid < 256) { sdata[tid] = min(sdata[tid], sdata[tid + 256]); } __syncthreads(); }
      if (blockSize >= 256) { if (tid < 128) { sdata[tid] = min(sdata[tid], sdata[tid + 128]); } __syncthreads(); }
      if (blockSize >= 128) { if (tid < 64) { sdata[tid] = min(sdata[tid], sdata[tid + 64]); } __syncthreads(); }

#ifndef __DEVICE_EMULATION__
      if (tid < 32)
#endif
      {
        if (blockSize >=  64) { sdata[tid] = min(sdata[tid], sdata[tid + 32]); EMUSYNC; }
        if (blockSize >=  32) { sdata[tid] = min(sdata[tid], sdata[tid + 16]); EMUSYNC; }
        if (blockSize >=  16) { sdata[tid] = min(sdata[tid], sdata[tid + 8]); EMUSYNC; }
        if (blockSize >=  8) { sdata[tid] = min(sdata[tid], sdata[tid + 4]); EMUSYNC; }
        if (blockSize >=  4) { sdata[tid] = min(sdata[tid], sdata[tid + 2]); EMUSYNC; }
        if (blockSize >=  2) { sdata[tid] = min(sdata[tid], sdata[tid + 1]); EMUSYNC; }
      }
      break;
  }

  // write result for this block to global mem 
  if (tid == 0) g_odata[blockIdx.x] = sdata[0];
}

#endif
