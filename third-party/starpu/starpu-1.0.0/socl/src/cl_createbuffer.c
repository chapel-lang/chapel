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

static void release_callback_memobject(void * e) {
  cl_mem mem = (cl_mem)e;

  /* Release references */
  gc_entity_unstore(&mem->context);

  //Delete this mem_object from the mem_object list
  mem_object_release(mem);

  /* Destruct object */
  starpu_data_unregister_no_coherency(mem->handle);

  if (!(mem->flags & CL_MEM_USE_HOST_PTR))
    free(mem->ptr);
}


/**
 * \brief Create a buffer
 *
 * A buffer has always an allocated region in host memory. If CL_MEM_USE_HOST_PTR
 * is set, we use memory pointed by host_ptr, otherwise some host memory is
 * allocated.
 *
 * If CL_MEM_USE_HOST_PTR or CL_MEM_ALLOC_HOST_PTR are set, memory pointed by host_ptr
 * is not coherent. To enforce coherency, you have to map the buffer (clEnqueueMapBuffer).
 * 
 * If CL_MEM_COPY_HOST_PTR is set, the buffer will be duplicated in host memory. You
 * should avoid it.
 *
 */
CL_API_ENTRY cl_mem CL_API_CALL
soclCreateBuffer(cl_context   context,
               cl_mem_flags flags,
               size_t       size,
               void *       host_ptr,
               cl_int *     errcode_ret) CL_API_SUFFIX__VERSION_1_0
{
   if (errcode_ret != NULL)
      *errcode_ret = CL_SUCCESS;

   //Check flags
   if (((flags & CL_MEM_READ_ONLY) && (flags & CL_MEM_WRITE_ONLY))
      || ((flags & CL_MEM_READ_WRITE) && (flags & CL_MEM_READ_ONLY))
      || ((flags & CL_MEM_READ_WRITE) && (flags & CL_MEM_WRITE_ONLY))
      || ((flags & CL_MEM_USE_HOST_PTR) && (flags & CL_MEM_ALLOC_HOST_PTR))
      || ((flags & CL_MEM_USE_HOST_PTR) && (flags & CL_MEM_COPY_HOST_PTR))) {
      if (errcode_ret != NULL)
         *errcode_ret = CL_INVALID_VALUE;
      return NULL;
   }

   if (size == 0) {
      if (errcode_ret != NULL)
         *errcode_ret = CL_INVALID_BUFFER_SIZE;
      return NULL;
   }

   if ((host_ptr == NULL && (flags & (CL_MEM_USE_HOST_PTR | CL_MEM_COPY_HOST_PTR)))
      || (host_ptr != NULL && !(flags & (CL_MEM_USE_HOST_PTR | CL_MEM_COPY_HOST_PTR)))) {
      if (errcode_ret != NULL)
         *errcode_ret = CL_INVALID_HOST_PTR;
      return NULL;
   }

   {
      cl_mem mem;

      //Alloc cl_mem structure
      mem = (cl_mem)gc_entity_alloc(sizeof(struct _cl_mem), release_callback_memobject);
      if (mem == NULL) {
         if (errcode_ret != NULL)
            *errcode_ret = CL_OUT_OF_HOST_MEMORY;
         return NULL;
      }
      
      mem->ptr = NULL;
      mem->map_count = 0;
      gc_entity_store(&mem->context, context);
      mem->flags = flags;
      mem->size = size;
      mem->host_ptr = host_ptr;

      #ifdef DEBUG
      static int id = 0;
      mem->id = id++;
      #endif

      mem_object_store(mem);

      //TODO: we shouldn't allocate the buffer ourselves. StarPU allocates it if a NULL pointer is given

      // If not MEM_USE_HOST_PTR, we need to alloc the buffer ourselves
      if (!(flags & CL_MEM_USE_HOST_PTR)) {
         mem->ptr = valloc(size);
         if (mem->ptr == NULL) {
            if (errcode_ret != NULL)
               *errcode_ret = CL_MEM_OBJECT_ALLOCATION_FAILURE;
            free(mem);
            return NULL;
         }
         //The buffer doesn't contain meaningful data
         mem->scratch = 1;
      }
      else {
         //The buffer may contain meaningful data
         mem->scratch = 0;
         mem->ptr = host_ptr;
      }

      // Access mode
      if (flags & CL_MEM_READ_ONLY)
         mem->mode = CL_MEM_READ_ONLY;
      else if (flags & CL_MEM_WRITE_ONLY)
         mem->mode = CL_MEM_WRITE_ONLY;
      else
         mem->mode = CL_MEM_READ_WRITE;

      // Perform data copy if necessary
      if (flags & CL_MEM_COPY_HOST_PTR)
         memcpy(mem->ptr, host_ptr, size);
      
      // Create StarPU buffer (on home node? what's this?)
      starpu_variable_data_register(&mem->handle, 0, (uintptr_t)mem->ptr, size); 

      DEBUG_MSG("[Buffer %d] Initialized (cl_mem %p handle %p)\n", mem->id, mem, mem->handle);
      
      return mem;
   }
}
