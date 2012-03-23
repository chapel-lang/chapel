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

struct bp_data {
   cl_program program;
   char * options;
};

static void soclBuildProgram_task(void *data) {
   struct bp_data *d = (struct bp_data*)data;
   cl_device_id device;
   cl_int err;

   int wid = starpu_worker_get_id();
   int range = starpu_worker_get_range();
   starpu_opencl_get_device(wid, &device);

   DEBUG_MSG("[Worker %d] Building program...\n", wid);

   err = clBuildProgram(d->program->cl_programs[range], 1, &device, d->options, NULL, NULL);
   if (err != CL_SUCCESS) {
      size_t len;
      static char buffer[4096];
      clGetProgramBuildInfo(d->program->cl_programs[range], device, CL_PROGRAM_BUILD_LOG, sizeof(buffer), buffer, &len);
      DEBUG_CL("clBuildProgram", err);
      ERROR_MSG("clBuildProgram: %s\n Aborting.\n", buffer);
   }

   DEBUG_MSG("[Worker %d] Done building.\n", wid);
}


CL_API_ENTRY cl_int CL_API_CALL
soclBuildProgram(cl_program         program,
               cl_uint              UNUSED(num_devices),
               const cl_device_id * UNUSED(device_list),
               const char *         options, 
               void (*pfn_notify)(cl_program program, void * user_data),
               void *               user_data) CL_API_SUFFIX__VERSION_1_0
{
   struct bp_data *data;

   program->options = options != NULL ? strdup(options) : NULL;
   program->options_size = options != NULL ? strlen(options)+1 : 0;

   data = (struct bp_data*)malloc(sizeof(struct bp_data));
   gc_entity_store(&data->program, program);
   data->options = (char*)options;

   /*FIXME: starpu_execute_on_each_worker is synchronous.
    * However pfn_notify may be useful only because build is supposed to be asynchronous
    */
   starpu_execute_on_each_worker(soclBuildProgram_task, data, STARPU_OPENCL);


   if (pfn_notify != NULL)
      pfn_notify(program, user_data);

   gc_entity_unstore(&data->program);
   free(data);

   return CL_SUCCESS;
}
