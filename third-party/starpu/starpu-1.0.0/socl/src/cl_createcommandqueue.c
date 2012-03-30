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

static void release_callback_command_queue(void * e) {
  cl_command_queue cq = (cl_command_queue)e;

  //Disable StarPU profiling if necessary
  if (cq->properties & CL_QUEUE_PROFILING_ENABLE) {
    profiling_queue_count -= 1;
    if (profiling_queue_count == 0)
      starpu_profiling_status_set(STARPU_PROFILING_DISABLE);
  }

  /* Release references */
  gc_entity_unstore(&cq->context);

  /* Destruct object */
  pthread_mutex_destroy(&cq->mutex);
}



CL_API_ENTRY cl_command_queue CL_API_CALL
soclCreateCommandQueue(cl_context                   context, 
                     cl_device_id                   device, 
                     cl_command_queue_properties    properties,
                     cl_int *                       errcode_ret) CL_API_SUFFIX__VERSION_1_0
{
   cl_command_queue cq;

   cq = (cl_command_queue)gc_entity_alloc(sizeof(struct _cl_command_queue),
                                          release_callback_command_queue);
   if (cq == NULL) {
      if (errcode_ret != NULL)
         *errcode_ret = CL_OUT_OF_HOST_MEMORY;
      return NULL;
   }

   cq->properties = properties;
   gc_entity_store(&cq->context, context);
   cq->device = device;

   #ifdef DEBUG
   static int id = 0;
   cq->id = id++;
   #endif

   //Enable StarPU profiling if necessary
   if (properties & CL_QUEUE_PROFILING_ENABLE) {
      if (profiling_queue_count == 0)
         starpu_profiling_status_set(STARPU_PROFILING_ENABLE);
      profiling_queue_count += 1;
   }

   cq->commands = NULL;
   cq->barrier = NULL;
   pthread_mutex_init(&cq->mutex, NULL);

   if (errcode_ret != NULL)
      *errcode_ret = CL_SUCCESS;

   return cq;
}
