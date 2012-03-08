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

struct arg_writebuffer {
   size_t offset;
   size_t cb;
   const void * ptr;
   cl_mem buffer;
};

static void soclEnqueueWriteBuffer_cpu_task(void *descr[], void *args) {
   struct arg_writebuffer *arg;
   arg = (struct arg_writebuffer*)args;
   void * ptr = (void*)STARPU_VARIABLE_GET_PTR(descr[0]);
   DEBUG_MSG("[Buffer %d] Writing %ld bytes from %p to %p\n", arg->buffer->id, arg->cb, arg->ptr, ptr+arg->offset);

   //FIXME: Fix for people who use USE_HOST_PTR, modify data at host_ptr and use WriteBuffer to commit the change.
   // StarPU may have erased host mem at host_ptr (for instance by retrieving current buffer data at host_ptr)
   // Buffer mapping facilities should be used instead
   // Maybe we should report the bug here... for now, we just avoid memcpy crash due to overlapping regions...
   if (ptr+arg->offset != arg->ptr)
      memcpy(ptr+arg->offset, arg->ptr, arg->cb);

   gc_entity_unstore(&arg->buffer);
   free(args);
}

static void soclEnqueueWriteBuffer_opencl_task(void *descr[], void *args) {
   struct arg_writebuffer *arg;
   arg = (struct arg_writebuffer*)args;

   cl_mem mem = (cl_mem)STARPU_VARIABLE_GET_PTR(descr[0]);

   DEBUG_MSG("[Buffer %d] Writing %ld bytes to offset %ld from %p\n", arg->buffer->id, arg->cb, arg->offset, arg->ptr);

   int wid = starpu_worker_get_id();
   cl_command_queue cq;
   starpu_opencl_get_queue(wid, &cq);

   cl_int err = clEnqueueWriteBuffer(cq, mem, CL_TRUE, arg->offset, arg->cb, arg->ptr, 0, NULL, NULL);
   if (err != CL_SUCCESS)
      DEBUG_CL("clEnqueueWriteBuffer", err);

   gc_entity_unstore(&arg->buffer);
   free(args);
}

static struct starpu_codelet codelet_writebuffer = {
   .where = STARPU_OPENCL,
   .model = NULL,
   .cpu_funcs = { &soclEnqueueWriteBuffer_cpu_task, NULL },
   .opencl_funcs = { &soclEnqueueWriteBuffer_opencl_task, NULL },
   .nbuffers = 1
};

cl_int command_write_buffer_submit(command_write_buffer cmd) {
	/* Aliases */
	cl_mem buffer = cmd->buffer;
	size_t offset = cmd->offset;
	size_t cb = cmd->cb;
	const void * ptr = cmd->ptr;

	struct starpu_task *task;
	struct arg_writebuffer *arg;

	task = task_create(CL_COMMAND_WRITE_BUFFER);

	task->buffers[0].handle = buffer->handle;
	//If only a subpart of the buffer is written, RW access mode is required
	if (cb != buffer->size)
		task->buffers[0].mode = STARPU_RW;
	else 
		task->buffers[0].mode = STARPU_W;
	task->cl = &codelet_writebuffer;

	arg = (struct arg_writebuffer*)malloc(sizeof(struct arg_writebuffer));
	arg->offset = offset;
	arg->cb = cb;
	arg->ptr = ptr;
	task->cl_arg = arg;
	task->cl_arg_size = sizeof(struct arg_writebuffer);

	gc_entity_store(&arg->buffer, buffer);

	//The buffer now contains meaningful data
	arg->buffer->scratch = 0;

	task_submit(task, cmd);

	return CL_SUCCESS;
}

CL_API_ENTRY cl_int CL_API_CALL
soclEnqueueWriteBuffer(cl_command_queue cq, 
                     cl_mem             buffer, 
                     cl_bool            blocking, 
                     size_t             offset, 
                     size_t             cb, 
                     const void *       ptr, 
                     cl_uint            num_events, 
                     const cl_event *   events, 
                     cl_event *         event) CL_API_SUFFIX__VERSION_1_0
{ 
	command_write_buffer cmd = command_write_buffer_create(buffer, offset, cb, ptr);

	command_queue_enqueue(cq, cmd, num_events, events);

	RETURN_EVENT(cmd, event);

	MAY_BLOCK(blocking);

	return CL_SUCCESS;
}
