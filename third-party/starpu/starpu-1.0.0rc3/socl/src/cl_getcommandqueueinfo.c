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
soclGetCommandQueueInfo(cl_command_queue    cq,
                      cl_command_queue_info param_name,
                      size_t                param_value_size,
                      void *                param_value,
                      size_t *              param_value_size_ret) CL_API_SUFFIX__VERSION_1_0
{
   if (cq == NULL)
      return CL_INVALID_COMMAND_QUEUE;

   switch (param_name) {
      INFO_CASE(CL_QUEUE_CONTEXT, cq->context);
      INFO_CASE(CL_QUEUE_DEVICE, cq->device);
      INFO_CASE(CL_QUEUE_REFERENCE_COUNT, cq->_entity.refs);
      INFO_CASE(CL_QUEUE_PROPERTIES, cq->properties);
      default:
         return CL_INVALID_VALUE;
   }

   return CL_SUCCESS; 
}
