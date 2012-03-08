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


void soclEnqueueNDRangeKernel_task(void *descr[], void *args) {
	command_ndrange_kernel cmd = (command_ndrange_kernel)args;

   cl_command_queue cq;
   int wid;
   cl_int err;

   wid = starpu_worker_get_id();
   starpu_opencl_get_queue(wid, &cq);

   DEBUG_MSG("[worker %d] [kernel %d] Executing kernel...\n", wid, cmd->kernel->id);

   int range = starpu_worker_get_range();

   /* Set arguments */
   {
	   unsigned int i;
	   int buf = 0;
	   for (i=0; i<cmd->num_args; i++) {
		   switch (cmd->arg_types[i]) {
			   case Null:
				   err = clSetKernelArg(cmd->kernel->cl_kernels[range], i, cmd->arg_sizes[i], NULL);
				   break;
			   case Buffer: {
						cl_mem mem;  
						mem = (cl_mem)STARPU_VARIABLE_GET_PTR(descr[buf]);
						err = clSetKernelArg(cmd->kernel->cl_kernels[range], i, cmd->arg_sizes[i], &mem);
						buf++;
					}
					break;
			   case Immediate:
					err = clSetKernelArg(cmd->kernel->cl_kernels[range], i, cmd->arg_sizes[i], cmd->args[i]);
					break;
		   }
		   if (err != CL_SUCCESS) {
			   DEBUG_CL("clSetKernelArg", err);
			   DEBUG_ERROR("Aborting\n");
		   }
	   }
   }

   /* Calling Kernel */
   cl_event event;
   err = clEnqueueNDRangeKernel(cq, cmd->kernel->cl_kernels[range], cmd->work_dim, cmd->global_work_offset, cmd->global_work_size, cmd->local_work_size, 0, NULL, &event);

   if (err != CL_SUCCESS) {
	   ERROR_MSG("Worker[%d] Unable to Enqueue kernel (error %d)\n", wid, err);
	   DEBUG_CL("clEnqueueNDRangeKernel", err);
	   DEBUG_MSG("Workdim %d, global_work_offset %p, global_work_size %p, local_work_size %p\n",
			   cmd->work_dim, cmd->global_work_offset, cmd->global_work_size, cmd->local_work_size);
	   DEBUG_MSG("Global work size: %ld %ld %ld\n", cmd->global_work_size[0],
			   (cmd->work_dim > 1 ? cmd->global_work_size[1] : 1), (cmd->work_dim > 2 ? cmd->global_work_size[2] : 1)); 
	   if (cmd->local_work_size != NULL)
		   DEBUG_MSG("Local work size: %ld %ld %ld\n", cmd->local_work_size[0],
				   (cmd->work_dim > 1 ? cmd->local_work_size[1] : 1), (cmd->work_dim > 2 ? cmd->local_work_size[2] : 1)); 
	   ERROR_MSG("Aborting.\n");
	   exit(1);
   }

   /* Waiting for kernel to terminate */
   clWaitForEvents(1, &event);
   clReleaseEvent(event);
}

static void cleaning_task_callback(void *args) {
	command_ndrange_kernel cmd = (command_ndrange_kernel)args;

	free(cmd->arg_sizes);
	free(cmd->arg_types);

	unsigned int i;
	for (i=0; i<cmd->num_args; i++) {
		free(cmd->args[i]);
	}
	free(cmd->args);

	for (i=0; i<cmd->num_buffers; i++)
		gc_entity_unstore(&cmd->buffers[i]);

	free(cmd->buffers);
	void * co = cmd->codelet;
	cmd->codelet = NULL;
	free(co);
}

static struct starpu_perfmodel perf_model = {
	.type = STARPU_HISTORY_BASED,
	.symbol = "perf_model"
};

/**
 * Real kernel enqueuing command
 */
cl_int command_ndrange_kernel_submit(command_ndrange_kernel cmd) {

	starpu_task task = task_create();
	task->cl = cmd->codelet;
	task->cl_arg = cmd;
	task->cl_arg_size = sizeof(cmd);

	struct starpu_codelet * codelet = cmd->codelet;

	/* We need to detect which parameters are OpenCL's memory objects and
	 * we retrieve their corresponding StarPU buffers */
	cmd->num_buffers = 0;
	cmd->buffers = malloc(sizeof(cl_mem) * cmd->num_args);

	unsigned int i;
	for (i=0; i<cmd->num_args; i++) {
		if (cmd->arg_types[i] == Buffer) {

			cl_mem buf = *(cl_mem*)cmd->args[i];

			gc_entity_store(&cmd->buffers[cmd->num_buffers], buf);
			task->buffers[cmd->num_buffers].handle = buf->handle;

			/* Determine best StarPU buffer access mode */
			int mode;
			if (buf->mode == CL_MEM_READ_ONLY)
				mode = STARPU_R;
			else if (buf->mode == CL_MEM_WRITE_ONLY) {
				mode = STARPU_W;
				buf->scratch = 0;
			}
			else if (buf->scratch) { //RW but never accessed in RW or W mode
				mode = STARPU_W;
				buf->scratch = 0;
			}
			else {
				mode = STARPU_RW;
				buf->scratch = 0;
			}
			task->buffers[cmd->num_buffers].mode = mode; 

			cmd->num_buffers += 1;
		}
	}
	codelet->nbuffers = cmd->num_buffers;

	task_submit(task, cmd);

	/* Enqueue a cleaning task */
	//FIXME: execute this in the callback?
	starpu_task cleaning_task = task_create_cpu(cleaning_task_callback, cmd,1);
	cl_event ev = command_event_get(cmd);
	task_depends_on(cleaning_task, 1, &ev);
	task_submit(cleaning_task, cmd);

	return CL_SUCCESS;
}


CL_API_ENTRY cl_int CL_API_CALL
soclEnqueueNDRangeKernel(cl_command_queue cq,
		cl_kernel        kernel,
		cl_uint          work_dim,
		const size_t *   global_work_offset,
		const size_t *   global_work_size,
		const size_t *   local_work_size,
		cl_uint          num_events,
		const cl_event * events,
		cl_event *       event) CL_API_SUFFIX__VERSION_1_1
{
	command_ndrange_kernel cmd = command_ndrange_kernel_create(kernel, work_dim,
			global_work_offset, global_work_size, local_work_size);

	command_queue_enqueue(cq, cmd, num_events, events);

	RETURN_EVENT(cmd, event);

	return CL_SUCCESS;
}
