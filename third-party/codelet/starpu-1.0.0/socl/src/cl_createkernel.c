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


static void soclCreateKernel_task(void *data) {
   struct _cl_kernel *k = (struct _cl_kernel *)data;

   int range = starpu_worker_get_range();
   cl_int err;

   DEBUG_MSG("[Device %d] Creating kernel...\n", starpu_worker_get_id());
   k->cl_kernels[range] = clCreateKernel(k->program->cl_programs[range], k->kernel_name, &err);
   if (err != CL_SUCCESS) {
      k->errcodes[range] = err;
      ERROR_STOP("[Device %d] Unable to create kernel. Aborting.\n", starpu_worker_get_id());
      return;
   }

   /* One worker creates argument structures */
   if (__sync_bool_compare_and_swap(&k->num_args, 0, 666)) {
      unsigned int i;
      cl_uint num_args;

      err = clGetKernelInfo(k->cl_kernels[range], CL_KERNEL_NUM_ARGS, sizeof(num_args), &num_args, NULL);
      if (err != CL_SUCCESS) {
         DEBUG_CL("clGetKernelInfo", err);
         ERROR_STOP("Unable to get kernel argument count. Aborting.\n");
      }
      k->num_args = num_args;
      DEBUG_MSG("Kernel has %d arguments\n", num_args);

      k->arg_size = (size_t*)malloc(sizeof(size_t) * num_args);
      k->arg_value = (void**)malloc(sizeof(void*) * num_args);
      k->arg_type = (enum kernel_arg_type*)malloc(sizeof(enum kernel_arg_type) * num_args);
      /* Settings default type to NULL */
      for (i=0; i<num_args; i++) {
         k->arg_value[i] = NULL;
         k->arg_type[i] = Null;
      }
   }
}

static void rk_task(void *data) {
   cl_kernel k = (cl_kernel)data;

   int range = starpu_worker_get_range();

   cl_int err = clReleaseKernel(k->cl_kernels[range]);
   if (err != CL_SUCCESS)
      DEBUG_CL("clReleaseKernel", err);
}

static void release_callback_kernel(void * e) {
  cl_kernel kernel = (cl_kernel)e;

  //Free args
  unsigned int j;
  for (j=0; j<kernel->num_args; j++) {
    switch (kernel->arg_type[j]) {
      case Null:
        break;
      case Buffer:
        gc_entity_unstore((cl_mem*)&kernel->arg_value[j]);
        break;
      case Immediate:
        free(kernel->arg_value[j]);
        break;
    }
  }
  if (kernel->arg_size != NULL)
    free(kernel->arg_size);
  if (kernel->arg_value != NULL)
    free(kernel->arg_value);
  if (kernel->arg_type != NULL)
    free(kernel->arg_type);

  //Release real kernels...
  starpu_execute_on_each_worker(rk_task, kernel, STARPU_OPENCL);

  gc_entity_unstore(&kernel->program);

  free(kernel->kernel_name);
  free(kernel->cl_kernels);
  free(kernel->errcodes);
}

CL_API_ENTRY cl_kernel CL_API_CALL
soclCreateKernel(cl_program    program,
               const char *    kernel_name,
               cl_int *        errcode_ret) CL_API_SUFFIX__VERSION_1_0
{
   cl_kernel k;
   int device_count;

   if (program == NULL) {
      if (errcode_ret != NULL)
         *errcode_ret = CL_INVALID_PROGRAM;
      return NULL;
   }

   //TODO: check programs (see opencl specs)

   /* Create Kernel structure */
   k = (cl_kernel)gc_entity_alloc(sizeof(struct _cl_kernel), release_callback_kernel);
   if (k == NULL) {
      if (errcode_ret != NULL)
         *errcode_ret = CL_OUT_OF_HOST_MEMORY;
      return NULL;
   }
   
   gc_entity_store(&k->program, program);
   k->kernel_name = strdup(kernel_name);
   k->num_args = 0;
   k->arg_value = NULL;
   k->arg_size = NULL;

   #ifdef DEBUG
   static int id = 0;
   k->id = id++;
   #endif
   
   device_count = starpu_opencl_worker_get_count();
   k->cl_kernels = (cl_kernel*)malloc(device_count * sizeof(cl_kernel));
   k->errcodes = (cl_int*)malloc(device_count * sizeof(cl_int));
   {
      int i;
      for (i=0; i<device_count; i++) {
         k->cl_kernels[i] = NULL;
         k->errcodes[i] = -9999;
      }
   }

   /* Create kernel on each device */
   DEBUG_MSG("[Kernel %d] Create %d kernels (name \"%s\")\n", k->id, starpu_opencl_worker_get_count(), kernel_name);
   starpu_execute_on_each_worker(soclCreateKernel_task, k, STARPU_OPENCL);

   if (errcode_ret != NULL) {
      int i;
      *errcode_ret = CL_SUCCESS;
      for (i=0; i<device_count; i++) {
         switch (k->errcodes[i]) {
            #define CASE_RET(e) case e: *errcode_ret = e; return k;
            CASE_RET(CL_INVALID_PROGRAM)
            CASE_RET(CL_INVALID_PROGRAM_EXECUTABLE)
            CASE_RET(CL_INVALID_KERNEL_NAME)
            CASE_RET(CL_INVALID_KERNEL_DEFINITION)
            CASE_RET(CL_INVALID_VALUE)
            CASE_RET(CL_OUT_OF_RESOURCES)
            CASE_RET(CL_OUT_OF_HOST_MEMORY)
            #undef CASE_RET
         }
      }
   }

   return k;
}
