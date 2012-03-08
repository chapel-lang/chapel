/*
 * Copyright 1993-2009 NVIDIA Corporation.  All rights reserved.
 *
 * NVIDIA Corporation and its licensors retain all intellectual property and 
 * proprietary rights in and to this software and related documentation and 
 * any modifications thereto.  Any use, reproduction, disclosure, or distribution 
 * of this software and related documentation without an express license 
 * agreement from NVIDIA Corporation is strictly prohibited.
 * 
 */
 
 /*
 * Portions Copyright (c) 1993-2009 NVIDIA Corporation.  All rights reserved.
 * Portions Copyright (c) 2009 Mike Giles, Oxford University.  All rights reserved.
 * Portions Copyright (c) 2008 Frances Y. Kuo and Stephen Joe.  All rights reserved.
 *
 * Sobol Quasi-random Number Generator example
 *
 * Based on CUDA code submitted by Mike Giles, Oxford University, United Kingdom
 * http://people.maths.ox.ac.uk/~gilesm/
 *
 * and C code developed by Stephen Joe, University of Waikato, New Zealand
 * and Frances Kuo, University of New South Wales, Australia
 * http://web.maths.unsw.edu.au/~fkuo/sobol/
 *
 * For theoretical background see:
 *
 * P. Bratley and B.L. Fox.
 * Implementing Sobol's quasirandom sequence generator
 * http://portal.acm.org/citation.cfm?id=42288
 * ACM Trans. on Math. Software, 14(1):88-100, 1988
 *
 * S. Joe and F. Kuo.
 * Remark on algorithm 659: implementing Sobol's quasirandom sequence generator.
 * http://portal.acm.org/citation.cfm?id=641879
 * ACM Trans. on Math. Software, 29(1):49-57, 2003
 *
 */

#include "sobol.h"
#include "sobol_gpu.h"
#include <starpu.h>
#include <starpu_cuda.h>

#define k_2powneg32 2.3283064E-10F

__global__ void sobolGPU_kernel(unsigned n_vectors, unsigned n_dimensions, unsigned *d_directions, float *d_output)
{
    __shared__ unsigned int v[n_directions];

    // Offset into the correct dimension as specified by the
    // block y coordinate
    d_directions = d_directions + n_directions * blockIdx.y;
    d_output = d_output +  n_vectors * blockIdx.y;

    // Copy the direction numbers for this dimension into shared
    // memory - there are only 32 direction numbers so only the
    // first 32 (n_directions) threads need participate.
    if (threadIdx.x < n_directions)
    {
	    v[threadIdx.x] = d_directions[threadIdx.x];
    }
    __syncthreads();

    // Set initial index (i.e. which vector this thread is
    // computing first) and stride (i.e. step to the next vector
    // for this thread)
    int i0     = threadIdx.x + blockIdx.x * blockDim.x;
    int stride = gridDim.x * blockDim.x;

    // Get the gray code of the index
    // c.f. Numerical Recipes in C, chapter 20
    // http://www.nrbook.com/a/bookcpdf/c20-2.pdf
    unsigned int g = i0 ^ (i0 >> 1);

    // Initialisation for first point x[i0]
    // In the Bratley and Fox paper this is equation (*), where
    // we are computing the value for x[n] without knowing the
    // value of x[n-1].
    unsigned int X = 0;
    unsigned int mask;
    for (unsigned int k = 0 ; k < __ffs(stride) - 1 ; k++)
    {
        // We want X ^= g_k * v[k], where g_k is one or zero.
        // We do this by setting a mask with all bits equal to
        // g_k. In reality we keep shifting g so that g_k is the
        // LSB of g. This way we avoid multiplication.
        mask = - (g & 1);
        X ^= mask & v[k];
        g = g >> 1;
    }
    if (i0 < n_vectors)
    {
        d_output[i0] = (float)X * k_2powneg32;
    }

    // Now do rest of points, using the stride
    // Here we want to generate x[i] from x[i-stride] where we
    // don't have any of the x in between, therefore we have to
    // revisit the equation (**), this is easiest with an example
    // so assume stride is 16.
    // From x[n] to x[n+16] there will be:
    //   8 changes in the first bit
    //   4 changes in the second bit
    //   2 changes in the third bit
    //   1 change in the fourth
    //   1 change in one of the remaining bits
    //
    // What this means is that in the equation:
    //   x[n+1] = x[n] ^ v[p]
    //   x[n+2] = x[n+1] ^ v[q] = x[n] ^ v[p] ^ v[q]
    //   ...
    // We will apply xor with v[1] eight times, v[2] four times,
    // v[3] twice, v[4] once and one other direction number once.
    // Since two xors cancel out, we can skip even applications
    // and just apply xor with v[4] (i.e. log2(16)) and with
    // the current applicable direction number.
    // Note that all these indices count from 1, so we need to
    // subtract 1 from them all to account for C arrays counting
    // from zero.
    unsigned int v_log2stridem1 = v[__ffs(stride) - 2];
    unsigned int v_stridemask = stride - 1;
    for (unsigned int i = i0 + stride ; i < n_vectors ; i += stride)
    {
        // x[i] = x[i-stride] ^ v[b] ^ v[c]
        //  where b is log2(stride) minus 1 for C array indexing
        //  where c is the index of the rightmost zero bit in i,
        //  not including the bottom log2(stride) bits, minus 1
        //  for C array indexing
        // In the Bratley and Fox paper this is equation (**)
        X ^= v_log2stridem1 ^ v[__ffs(~((i - stride) | v_stridemask)) - 1];
        d_output[i] = (float)X * k_2powneg32;
    }
}

extern "C"
void sobolGPU(int n_vectors, int n_dimensions, unsigned int *d_directions, float *d_output)
{
    const int threadsperblock = 64;

    // Set up the execution configuration
    dim3 dimGrid;
    dim3 dimBlock;

    // This implementation of the generator outputs all the draws for
    // one dimension in a contiguous region of memory, followed by the
    // next dimension and so on.
    // Therefore all threads within a block will be processing different
    // vectors from the same dimension. As a result we want the total
    // number of blocks to be a multiple of the number of dimensions.
    dimGrid.y = n_dimensions;

    // If the number of dimensions is large then we will set the number
    // of blocks to equal the number of dimensions (i.e. dimGrid.x = 1)
    // but if the number of dimensions is small (e.g. less than 32) then
    // we'll partition the vectors across blocks (as well as threads).
    // We also need to cap the dimGrid.x where the number of vectors
    // is too small to be partitioned.
    dimGrid.x = 1 + 31 / n_dimensions;
    if (dimGrid.x > (unsigned int)(n_vectors / threadsperblock))
    {
        dimGrid.x = (n_vectors + threadsperblock - 1) / threadsperblock;
    }
    
    // Fix the number of threads
    dimBlock.x = threadsperblock;

    // Execute GPU kernel
    sobolGPU_kernel<<<dimGrid, dimBlock, 0, starpu_cuda_get_local_stream()>>>(n_vectors, n_dimensions, d_directions, d_output);
}
