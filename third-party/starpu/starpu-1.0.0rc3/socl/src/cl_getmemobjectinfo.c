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
soclGetMemObjectInfo(cl_mem           mem,
                   cl_mem_info      param_name, 
                   size_t           param_value_size,
                   void *           param_value,
                   size_t *         param_value_size_ret) CL_API_SUFFIX__VERSION_1_0
{
   static cl_mem_object_type mot = CL_MEM_OBJECT_BUFFER;

   switch (param_name) {
      INFO_CASE(CL_MEM_TYPE, mot)
      INFO_CASE(CL_MEM_FLAGS, mem->flags)
      INFO_CASE(CL_MEM_SIZE, mem->size)
      INFO_CASE(CL_MEM_HOST_PTR, mem->host_ptr)
      INFO_CASE(CL_MEM_MAP_COUNT, mem->map_count)
      INFO_CASE(CL_MEM_REFERENCE_COUNT, mem->_entity.refs)
      INFO_CASE(CL_MEM_CONTEXT, mem->context)
      default:
         return CL_INVALID_VALUE;
   }

   return CL_SUCCESS;
}
