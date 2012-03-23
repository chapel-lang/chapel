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
soclGetProgramBuildInfo(cl_program          program,
                      cl_device_id          UNUSED(device),
                      cl_program_build_info param_name,
                      size_t                param_value_size,
                      void *                param_value,
                      size_t *              param_value_size_ret) CL_API_SUFFIX__VERSION_1_0
{
   if (program == NULL)
      return CL_INVALID_PROGRAM;


   switch (param_name) {
      //TODO
      //INFO_CASE(CL_PROGRAM_BUILD_STATUS, program->build_status);
      INFO_CASE_EX(CL_PROGRAM_BUILD_OPTIONS, program->options, program->options_size);
      //TODO
      //INFO_CASE(CL_PROGRAM_BUILD_LOG, program->build_log);
      default:
         return CL_INVALID_VALUE;
   }

   return CL_SUCCESS;
}
