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
soclSetKernelArg(cl_kernel  kernel,
               cl_uint      arg_index,
               size_t       arg_size,
               const void * arg_value) CL_API_SUFFIX__VERSION_1_0
{
   if (kernel == NULL)
      return CL_INVALID_KERNEL;

   if (arg_index >= kernel->num_args)
      return CL_INVALID_ARG_INDEX;

   //FIXME: we don't return CL_INVALID_ARG_VALUE if "arg_value is NULL for an argument that is not declared with __local qualifier or vice-versa"
   //FIXME: we don't return CL_INVALID_MEM_OBJECT
   //FIXME: we don't return CL_INVALID_ARG_SIZE

   /* Free previous argument (set to NULL) */
   switch (kernel->arg_type[arg_index]) {
      case Null:
         break;
      case Buffer:
         kernel->arg_type[arg_index] = Null;
         gc_entity_unstore((cl_mem*)kernel->arg_value[arg_index]);
	 free(kernel->arg_value[arg_index]);
         kernel->arg_value[arg_index] = NULL;
         break;
      case Immediate:
         free(kernel->arg_value[arg_index]);
         kernel->arg_type[arg_index] = Null;
         kernel->arg_value[arg_index] = NULL;
         break;
   }

   kernel->arg_type[arg_index] = Null;
   kernel->arg_size[arg_index] = arg_size;

   DEBUG_MSG("[Kernel %d] Set argument %d: argsize %ld argvalue %p\n", kernel->id, arg_index, arg_size, arg_value);

   /* Argument is not Null */
   if (arg_value != NULL) {
      cl_mem buf = NULL;
      /* Check if argument is a memory object */
      if ((arg_size == sizeof(cl_mem)) && ((buf = mem_object_fetch(arg_value)) != NULL)) {
         DEBUG_MSG("Found buffer %d \n", buf->id);
         kernel->arg_type[arg_index] = Buffer;
         kernel->arg_value[arg_index] = malloc(sizeof(void*));
	 gc_entity_store((cl_mem*)kernel->arg_value[arg_index], buf);
      }
      else {
         /* Argument must be an immediate buffer  */
         DEBUG_MSG("Immediate data\n");
         kernel->arg_type[arg_index] = Immediate;
         kernel->arg_value[arg_index] = malloc(arg_size);
         memcpy(kernel->arg_value[arg_index], arg_value, arg_size);
      }
   }

   return CL_SUCCESS;
}
