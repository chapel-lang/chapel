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

struct arg_readbuffer {
   size_t offset;
   size_t cb;
   void * ptr;
   cl_mem buffer;
};

static void soclEnqueueReadBuffer_cpu_task(void *descr[], void *args) {
   struct arg_readbuffer *arg;
   arg = (struct arg_readbuffer*)args;
   void * ptr = (void*)STARPU_VARIABLE_GET_PTR(descr[0]);
   DEBUG_MSG("[Buffer %d] Reading %ld bytes from %p to %p\n", arg->buffer->id, arg->cb, ptr+arg->offset, arg->ptr);

   //This fix is for people who use USE_HOST_PTR and still use ReadBuffer to sync the buffer in host mem at host_ptr.
   //They should use buffer mapping facilities instead.
   if (ptr+arg->offset != arg->ptr)
      memcpy(arg->ptr, ptr+arg->offset, arg->cb);

   gc_entity_unstore(&arg->buffer);
   free(args);
}

static void soclEnqueueReadBuffer_opencl_task(void *descr[], void *args) {
   struct arg_readbuffer *arg;
   arg = (struct arg_readbuffer*)args;

   cl_mem mem = (cl_mem)STARPU_VARIABLE_GET_PTR(descr[0]);

   DEBUG_MSG("[Buffer %d] Reading %ld bytes from offset %ld into %p\n", arg->buffer->id, arg->cb, arg->offset, arg->ptr);

   int wid = starpu_worker_get_id();
   cl_command_queue cq;
   starpu_opencl_get_queue(wid, &cq);

   cl_int ret = clEnqueueReadBuffer(cq, mem, CL_TRUE, arg->offset, arg->cb, arg->ptr, 0, NULL, NULL);
   if (ret != CL_SUCCESS)
      DEBUG_CL("clEnqueueReadBuffer", ret);

   gc_entity_unstore(&arg->buffer);
   free(args);
}

static struct starpu_codelet codelet_readbuffer = {
   .where = STARPU_OPENCL,
   .model = NULL,
   .cpu_funcs = { &soclEnqueueReadBuffer_cpu_task, NULL },
   .opencl_funcs = { &soclEnqueueReadBuffer_opencl_task, NULL },
   .nbuffers = 1
};

cl_int command_read_buffer_submit(command_read_buffer cmd) {
	/* Aliases */
	cl_mem buffer = cmd->buffer;
	size_t offset = cmd->offset;
	size_t cb = cmd->cb;
	void * ptr = cmd->ptr;

	struct starpu_task *task;
	struct arg_readbuffer *arg;

	task = task_create(CL_COMMAND_READ_BUFFER);

	task->buffers[0].handle = buffer->handle;
	task->buffers[0].mode = STARPU_R;
	task->cl = &codelet_readbuffer;

	arg = (struct arg_readbuffer*)malloc(sizeof(struct arg_readbuffer));
	arg->offset = offset;
	arg->cb = cb;
	arg->ptr = ptr;
	task->cl_arg = arg;
	task->cl_arg_size = sizeof(struct arg_readbuffer);

	gc_entity_store(&arg->buffer, buffer);

	task_submit(task, cmd);

	return CL_SUCCESS;
}


CL_API_ENTRY cl_int CL_API_CALL
soclEnqueueReadBuffer(cl_command_queue  cq,
                    cl_mem              buffer,
                    cl_bool             blocking,
                    size_t              offset,
                    size_t              cb, 
                    void *              ptr,
                    cl_uint             num_events,
                    const cl_event *    events,
                    cl_event *          event) CL_API_SUFFIX__VERSION_1_0
{ 

	command_read_buffer cmd = command_read_buffer_create(buffer, offset, cb, ptr);

	command_queue_enqueue(cq, cmd, num_events, events);

	RETURN_EVENT(cmd, event);

	MAY_BLOCK(blocking);

	return CL_SUCCESS;
}
