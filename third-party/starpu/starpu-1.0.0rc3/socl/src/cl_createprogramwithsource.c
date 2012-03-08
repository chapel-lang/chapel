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

struct cpws_data {
   struct _cl_program *program;
   cl_int *errcodes;
   cl_uint count;
   char **strings;
   size_t *lengths;
};

static void soclCreateProgramWithSource_task(void *data) {
   struct cpws_data *d = (struct cpws_data*)data;

   cl_context context;
   int wid = starpu_worker_get_id();
   DEBUG_MSG("Worker id: %d\n", wid);

   int range = starpu_worker_get_range();

   starpu_opencl_get_context(wid, &context);

   d->program->cl_programs[range] = clCreateProgramWithSource(context, d->count, (const char**)d->strings, d->lengths, &d->errcodes[range]);

}

static void rp_task(void *data) {
   cl_program program = (cl_program)data;

   int range = starpu_worker_get_range();

   cl_int err = clReleaseProgram(program->cl_programs[range]);
   if (err != CL_SUCCESS)
      DEBUG_CL("clReleaseProgram", err);
}

static void release_callback_program(void * e) {
  cl_program program = (cl_program)e;

  /* Destruct object */
  starpu_execute_on_each_worker(rp_task, program, STARPU_OPENCL);

  /* Release references */
  gc_entity_unstore(&program->context);

  free(program->cl_programs);

  if (program->options != NULL)
    free(program->options);
}


CL_API_ENTRY cl_program CL_API_CALL
soclCreateProgramWithSource(cl_context      context,
                          cl_uint           count,
                          const char **     strings,
                          const size_t *    lengths,
                          cl_int *          errcode_ret) CL_API_SUFFIX__VERSION_1_0
{
   cl_program p;
   struct cpws_data *data;
   int device_count;

   if (errcode_ret != NULL)
      *errcode_ret = CL_SUCCESS;

   device_count = starpu_opencl_worker_get_count();
   DEBUG_MSG("Worker count: %d\n", device_count);

   /* Check arguments */
   if (count == 0 || strings == NULL) {
      if (errcode_ret != NULL)
         *errcode_ret = CL_INVALID_VALUE;
      return NULL;
   }

   /* Alloc cl_program structure */
   p = (cl_program)gc_entity_alloc(sizeof(struct _cl_program), release_callback_program);
   if (p == NULL) {
      if (errcode_ret != NULL)
         *errcode_ret = CL_OUT_OF_HOST_MEMORY;
      return NULL;
   }

   gc_entity_store(&p->context, context);
   p->options = NULL;

   #ifdef DEBUG
   static int id = 0;
   p->id = id++;
   #endif


   p->cl_programs = (cl_program*)malloc(sizeof(cl_program) * device_count);
   if (p->cl_programs == NULL) {
      if (errcode_ret != NULL)
         *errcode_ret = CL_OUT_OF_HOST_MEMORY;
      return NULL;
   }

   {
      int i;
      for (i=0; i<device_count; i++)
         p->cl_programs[i] = NULL;
   }

   /* Construct structure to pass arguments to workers */
   data = (struct cpws_data*)malloc(sizeof(struct cpws_data));
   if (data == NULL) {
      if (errcode_ret != NULL)
         *errcode_ret = CL_OUT_OF_HOST_MEMORY;
      return NULL;
   }
   data->count = count;
   data->program = p;
   data->strings = (char**)strings;
   data->lengths = (size_t*)lengths;

   data->errcodes = (cl_int*)malloc(sizeof(cl_int) * device_count);

   /* Init real cl_program for each OpenCL device */
   starpu_execute_on_each_worker(soclCreateProgramWithSource_task, data, STARPU_OPENCL);

   if (errcode_ret != NULL) {
      int i;
      *errcode_ret = CL_SUCCESS;
      for (i=0; i<device_count; i++) {
         if (data->errcodes[i] != CL_SUCCESS) {
            DEBUG_MSG("Worker [%d] failed\n", i);
            DEBUG_CL("clCreateProgramWithSource", data->errcodes[i]);
            *errcode_ret = data->errcodes[i];
            break;
         }
      }
   }

   free(data->errcodes);
   free(data);
   
   return p;
}
