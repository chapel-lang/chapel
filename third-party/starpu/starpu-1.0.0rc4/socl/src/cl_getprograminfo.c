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
#include "getinfo.h"

CL_API_ENTRY cl_int CL_API_CALL
soclGetProgramInfo(cl_program       program,
                 cl_program_info    param_name,
                 size_t             param_value_size,
                 void *             param_value,
                 size_t *           param_value_size_ret) CL_API_SUFFIX__VERSION_1_0
{
   if (program == NULL)
      return CL_INVALID_PROGRAM;

   switch (param_name) {
      INFO_CASE(CL_PROGRAM_REFERENCE_COUNT, program->_entity.refs);
      INFO_CASE(CL_PROGRAM_CONTEXT, program->context);
      INFO_CASE(CL_PROGRAM_NUM_DEVICES, program->context->num_devices);
      INFO_CASE_EX(CL_PROGRAM_DEVICES, program->context->devices, sizeof(cl_device_id)*program->context->num_devices);
      //TODO
      /*INFO_CASE(CL_PROGRAM_SOURCE, program->source);
      INFO_CASE(CL_PROGRAM_BINARY_SIZE, program->binary_sizes);
      INFO_CASE(CL_PROGRAM_BINARIES, program->binaries);*/
      default:
         return CL_INVALID_VALUE;
   }

   return CL_SUCCESS;
}
