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

CL_API_ENTRY cl_int CL_API_CALL
soclSetCommandQueueProperty(cl_command_queue            command_queue,
                          cl_command_queue_properties   properties, 
                          cl_bool                       enable,
                          cl_command_queue_properties * old_properties) CL_API_SUFFIX__VERSION_1_0
{
   if (command_queue == NULL)
      return CL_INVALID_COMMAND_QUEUE;

   if (old_properties != NULL)
      *old_properties = command_queue->properties;

   if (enable) {
      //Enable StarPU profiling if necessary
      if (properties & (~command_queue->properties) & CL_QUEUE_PROFILING_ENABLE) {
         if (profiling_queue_count == 0)
            starpu_profiling_status_set(STARPU_PROFILING_ENABLE);
         profiling_queue_count += 1;
      }  
      //Set new properties
      command_queue->properties |= properties;
   } else {
      //Disable StarPU profiling if necessary
      if ((~properties) & command_queue->properties & CL_QUEUE_PROFILING_ENABLE) {
         profiling_queue_count -= 1;
         if (profiling_queue_count == 0)
            starpu_profiling_status_set(STARPU_PROFILING_DISABLE);
      }  
      //Set new properties
      command_queue->properties &= ~properties;
   }

   return CL_SUCCESS;
}
