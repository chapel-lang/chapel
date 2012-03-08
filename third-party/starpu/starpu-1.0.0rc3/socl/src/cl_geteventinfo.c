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
soclGetEventInfo(cl_event       event,
               cl_event_info    param_name,
               size_t           param_value_size,
               void *           param_value,
               size_t *         param_value_size_ret) CL_API_SUFFIX__VERSION_1_0
{
   if (event == NULL)
      return CL_INVALID_EVENT;

   #define STAT_CASE(starpu,opencl) case starpu: \
      status = opencl; \
      break;

   switch (param_name) {
      INFO_CASE(CL_EVENT_COMMAND_QUEUE, event->cq);
      INFO_CASE(CL_EVENT_COMMAND_TYPE, event->command->typ);
      INFO_CASE(CL_EVENT_COMMAND_EXECUTION_STATUS, event->status);
      INFO_CASE(CL_EVENT_REFERENCE_COUNT, event->_entity.refs);
      default:
         return CL_INVALID_VALUE;
   }

   return CL_SUCCESS; 
}
