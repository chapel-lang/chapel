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

CL_API_ENTRY cl_program CL_API_CALL
soclCreateProgramWithBinary(cl_context                     UNUSED(context),
                          cl_uint                        UNUSED(num_devices),
                          const cl_device_id *           UNUSED(device_list),
                          const size_t *                 UNUSED(lengths),
                          const unsigned char **         UNUSED(binaries),
                          cl_int *                       UNUSED(binary_status),
                          cl_int *                       errcode_ret) CL_API_SUFFIX__VERSION_1_0
{
   //TODO
   if (errcode_ret != NULL)
      *errcode_ret = CL_INVALID_OPERATION;

   return NULL;
}
