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

/**
 * \brief Get StarPU platform ID
 */
CL_API_ENTRY cl_int CL_API_CALL
soclGetPlatformIDs(cl_uint          num_entries,
                 cl_platform_id * platforms,
                 cl_uint *        num_platforms) CL_API_SUFFIX__VERSION_1_0
{
   if ((num_entries == 0 && platforms != NULL)
       || (num_platforms == NULL && platforms == NULL))
	   return CL_INVALID_VALUE;

   if (starpu_opencl_worker_get_count() == 0) {
      DEBUG_MSG("StarPU didn't find any OpenCL device. Try disabling CUDA support in StarPU (export STARPU_NCUDA=0).\n")

      if (num_platforms != NULL)
         *num_platforms = 0;
   }
   else {

      if (platforms != NULL)
         platforms[0] = &socl_platform;

      if (num_platforms != NULL)
         *num_platforms = 1;
   }

   return CL_SUCCESS;
}
