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

#ifdef STARPU_VERBOSE
void DEBUG_CL(char *s, cl_int err) {
   #define ERR_CASE(a) case a: DEBUG_MSG("[OpenCL] %s CL error: %s\n", s, #a); break;
   switch(err) {
      case CL_SUCCESS:
         DEBUG_MSG("[OpenCL] %s SUCCESS.\n", s);
         break;
      ERR_CASE(CL_DEVICE_NOT_FOUND)
      ERR_CASE(CL_DEVICE_NOT_AVAILABLE)
      ERR_CASE(CL_COMPILER_NOT_AVAILABLE)
      ERR_CASE(CL_MEM_OBJECT_ALLOCATION_FAILURE)
      ERR_CASE(CL_OUT_OF_RESOURCES)
      ERR_CASE(CL_OUT_OF_HOST_MEMORY)
      ERR_CASE(CL_PROFILING_INFO_NOT_AVAILABLE)
      ERR_CASE(CL_MEM_COPY_OVERLAP)
      ERR_CASE(CL_IMAGE_FORMAT_MISMATCH)
      ERR_CASE(CL_IMAGE_FORMAT_NOT_SUPPORTED)
      ERR_CASE(CL_BUILD_PROGRAM_FAILURE)
      ERR_CASE(CL_MAP_FAILURE)
      ERR_CASE(CL_INVALID_VALUE)
      ERR_CASE(CL_INVALID_DEVICE_TYPE)
      ERR_CASE(CL_INVALID_PLATFORM)
      ERR_CASE(CL_INVALID_DEVICE)
      ERR_CASE(CL_INVALID_CONTEXT)
      ERR_CASE(CL_INVALID_QUEUE_PROPERTIES)
      ERR_CASE(CL_INVALID_COMMAND_QUEUE)
      ERR_CASE(CL_INVALID_HOST_PTR)
      ERR_CASE(CL_INVALID_MEM_OBJECT)
      ERR_CASE(CL_INVALID_IMAGE_FORMAT_DESCRIPTOR)
      ERR_CASE(CL_INVALID_IMAGE_SIZE)
      ERR_CASE(CL_INVALID_SAMPLER)
      ERR_CASE(CL_INVALID_BINARY)
      ERR_CASE(CL_INVALID_BUILD_OPTIONS)
      ERR_CASE(CL_INVALID_PROGRAM)
      ERR_CASE(CL_INVALID_PROGRAM_EXECUTABLE)
      ERR_CASE(CL_INVALID_KERNEL_NAME)
      ERR_CASE(CL_INVALID_KERNEL_DEFINITION)
      ERR_CASE(CL_INVALID_KERNEL)
      ERR_CASE(CL_INVALID_ARG_INDEX)
      ERR_CASE(CL_INVALID_ARG_VALUE)
      ERR_CASE(CL_INVALID_ARG_SIZE)
      ERR_CASE(CL_INVALID_KERNEL_ARGS)
      ERR_CASE(CL_INVALID_WORK_DIMENSION)
      ERR_CASE(CL_INVALID_WORK_GROUP_SIZE)
      ERR_CASE(CL_INVALID_WORK_ITEM_SIZE)
      ERR_CASE(CL_INVALID_GLOBAL_OFFSET)
      ERR_CASE(CL_INVALID_EVENT_WAIT_LIST)
      ERR_CASE(CL_INVALID_EVENT)
      ERR_CASE(CL_INVALID_OPERATION)
      ERR_CASE(CL_INVALID_GL_OBJECT)
      ERR_CASE(CL_INVALID_BUFFER_SIZE)
      ERR_CASE(CL_INVALID_MIP_LEVEL)
      ERR_CASE(CL_INVALID_GLOBAL_WORK_SIZE)
      default:
         DEBUG_MSG("%s CL error: Error message not supported by DEBUG_CL macro (%d).\n", s, err);
   }
}
#endif
