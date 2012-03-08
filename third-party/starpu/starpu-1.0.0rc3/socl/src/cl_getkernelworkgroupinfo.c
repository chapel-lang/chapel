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

struct gkwgi_data {
   cl_kernel_work_group_info param_name;
   cl_kernel kernel;
   union {
      size_t work_group_size;
      size_t compile_work_group_size[3];
      cl_ulong local_mem_size;
   };
};

static void gkwgi_task(void *data) {
   cl_int err;
   struct gkwgi_data *d = (struct gkwgi_data*)data;

   int wid = starpu_worker_get_id();
   int range = starpu_worker_get_range();

   cl_device_id device;
   starpu_opencl_get_device(wid, &device);

   size_t value;
   size_t oldval;
   err = clGetKernelWorkGroupInfo(d->kernel->cl_kernels[range], device, d->param_name, sizeof(value), &value, NULL);
   if (err != CL_SUCCESS) {
      DEBUG_MSG("Worker [%d] failed\n", wid);
      DEBUG_CL("clGetKernelWorkGroupInfo", err);
   }

   switch (d->param_name) {
      case CL_KERNEL_WORK_GROUP_SIZE: {
         //Get the smallest work group size
         do {
            oldval = d->work_group_size;
         } while (value < oldval && !(__sync_bool_compare_and_swap(&d->work_group_size, oldval, value)));
      }
      break;
      case CL_KERNEL_LOCAL_MEM_SIZE: {
         //Get the biggest local mem size
         do {
            oldval = d->local_mem_size;
         } while (value > oldval && !(__sync_bool_compare_and_swap(&d->local_mem_size, oldval, value)));
      }
      break;
   }

}

static void gkwgi_task2(void **UNUSED(desc), void *data) {
   cl_int err;
   struct gkwgi_data *d = (struct gkwgi_data*)data;

   int wid = starpu_worker_get_id();
   int range = starpu_worker_get_range();

   cl_device_id device;
   starpu_opencl_get_device(wid, &device);

   err = clGetKernelWorkGroupInfo(d->kernel->cl_kernels[range], device, d->param_name, sizeof(d->compile_work_group_size), &d->compile_work_group_size, NULL);
   if (err != CL_SUCCESS) {
      DEBUG_MSG("Worker [%d] failed\n", wid);
      DEBUG_CL("clGetKernelWorkGroupInfo", err);
   }
}

static struct starpu_codelet gkwgi_codelet = {
   .where = STARPU_OPENCL,
   .opencl_funcs = { gkwgi_task2, NULL },
   .nbuffers = 0,
   .model = NULL
};

CL_API_ENTRY cl_int CL_API_CALL
soclGetKernelWorkGroupInfo(cl_kernel                kernel,
                         cl_device_id               UNUSED(device),
                         cl_kernel_work_group_info  param_name,
                         size_t                     param_value_size,
                         void *                     param_value,
                         size_t *                   param_value_size_ret) CL_API_SUFFIX__VERSION_1_0
{
   if (kernel == NULL)
      return CL_INVALID_KERNEL;


   struct gkwgi_data data;
   data.param_name = param_name;
   data.kernel = kernel;

   switch (param_name) {
      case CL_KERNEL_WORK_GROUP_SIZE:
         /* We take the smallest value to be sure the kernel can be executed on any available device */
         data.work_group_size = SIZE_MAX;
         starpu_execute_on_each_worker(gkwgi_task, &data, STARPU_OPENCL);
         INFO_CASE_EX2(data.work_group_size);
      case CL_KERNEL_COMPILE_WORK_GROUP_SIZE: {
         struct starpu_task *task;
         task = starpu_task_create();
         task->cl = &gkwgi_codelet;
         task->cl_arg = &data;
         task->cl_arg_size = sizeof(data);
         task->synchronous = 1;
         starpu_task_submit(task);
         INFO_CASE_EX2(data.compile_work_group_size);
         }
      case CL_KERNEL_LOCAL_MEM_SIZE:
         /* We take the biggest value to be sure the kernel can be executed on any available device */
         data.local_mem_size = 0;
         starpu_execute_on_each_worker(gkwgi_task, &data, STARPU_OPENCL);
         INFO_CASE_EX2(data.local_mem_size);
      default:
         return CL_INVALID_OPERATION;
   }

   return CL_SUCCESS;
}


