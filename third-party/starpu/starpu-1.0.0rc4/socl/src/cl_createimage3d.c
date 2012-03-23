/* StarPU --- Runtime system for heterogeneous multicore architectures.
 *
 * Copyright (C) 2010,2011 University of Bordeaux
 *
 * StarPU is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 2.1 of the License, or (at
 * your option) any later version.
 *
 * StarPU is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *
 * See the GNU Lesser General Public License in COPYING.LGPL for more details.
 */

#include "socl.h"

CL_API_ENTRY cl_mem CL_API_CALL
soclCreateImage3D(cl_context              UNUSED(context),
                cl_mem_flags            UNUSED(flags),
                const cl_image_format * UNUSED(image_format),
                size_t                  UNUSED(image_width), 
                size_t                  UNUSED(image_height),
                size_t                  UNUSED(image_depth), 
                size_t                  UNUSED(image_row_pitch), 
                size_t                  UNUSED(image_slice_pitch), 
                void *                  UNUSED(host_ptr),
                cl_int *                errcode_ret) CL_API_SUFFIX__VERSION_1_0
{
   if (errcode_ret != NULL)
      *errcode_ret = CL_INVALID_OPERATION;
   return NULL;
}
