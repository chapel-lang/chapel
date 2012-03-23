/*
 * Copyright 1993-2009 NVIDIA Corporation.  All rights reserved.
 *
 * NVIDIA Corporation and its licensors retain all intellectual property and
 * proprietary rights in and to this software and related documentation.
 * Any use, reproduction, disclosure, or distribution of this software
 * and related documentation without an express license agreement from
 * NVIDIA Corporation is strictly prohibited.
 *
 * Please refer to the applicable NVIDIA end user license agreement (EULA)
 * associated with this source code for terms and conditions that govern
 * your use of this NVIDIA software.
 *
 */

/* Matrix-vector multiplication: W = M * V.
 * Device code.
 *
 * This sample implements matrix-vector multiplication.
 * It has been written for clarity of exposition to illustrate various OpenCL
 * programming principles and optimizatoins, not with the goal of providing
 * the most performant generic kernel for matrix-vector multiplication.
 *
 * CUBLAS provides high-performance matrix-vector multiplication on GPU.
 */

__kernel void matVecMult(
                         __global float* M,
                         __global float* V,
                         int width, int height,
                         __global float* W
                         )
{
        // Row index
        uint y = get_global_id(0);
        if (y < height)
	{
                // Row pointer
                const __global float* row = M + y * width;

                // Compute dot product
                float dotProduct = 0;
                for (int x = 0; x < width; ++x)
                        dotProduct += row[x] * V[x];

                // Write result to global memory
                W[y] = dotProduct;
        }
}
