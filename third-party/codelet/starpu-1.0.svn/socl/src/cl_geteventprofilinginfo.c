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
soclGetEventProfilingInfo(cl_event          event,
                        cl_profiling_info   param_name,
                        size_t              param_value_size,
                        void *              param_value,
                        size_t *            param_value_size_ret) CL_API_SUFFIX__VERSION_1_0
{
   struct starpu_task_profiling_info * prof = event->profiling_info;

   if (prof == NULL)
      return CL_PROFILING_INFO_NOT_AVAILABLE;

   #define TONANO(t) (t.tv_nsec + t.tv_sec*1e9)

   switch (param_name) {
      case CL_PROFILING_COMMAND_QUEUED:
      INFO_CASE_VALUE(CL_PROFILING_COMMAND_SUBMIT, cl_ulong, TONANO(prof->submit_time));
      INFO_CASE_VALUE(CL_PROFILING_COMMAND_START, cl_ulong, TONANO(prof->start_time));
      INFO_CASE_VALUE(CL_PROFILING_COMMAND_END, cl_ulong, TONANO(prof->end_time));
      default:
         return CL_INVALID_VALUE;
   }

   #undef TONANO

   return CL_SUCCESS;
}
