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

void command_init_ex(cl_command cmd, cl_command_type typ) {
	cmd->typ = typ;
	cmd->num_events = 0;
	cmd->events = NULL;
	cmd->event = event_create();
	cmd->event->command = cmd;
	cmd->cq = NULL;
	cmd->task = NULL;
	cmd->submitted = 0;
}


void command_submit_ex(cl_command cmd) {
#define SUBMIT(typ,name) case typ:\
	name##_submit((name)cmd);\
	break;

	assert(cmd->submitted == 0);

	switch(cmd->typ) {
		SUBMIT(CL_COMMAND_NDRANGE_KERNEL, command_ndrange_kernel)
		SUBMIT(CL_COMMAND_TASK, command_ndrange_kernel)
		SUBMIT(CL_COMMAND_READ_BUFFER, command_read_buffer)
		SUBMIT(CL_COMMAND_WRITE_BUFFER, command_write_buffer)
		SUBMIT(CL_COMMAND_COPY_BUFFER, command_copy_buffer)
		SUBMIT(CL_COMMAND_MAP_BUFFER, command_map_buffer)
		SUBMIT(CL_COMMAND_UNMAP_MEM_OBJECT, command_unmap_mem_object)
		SUBMIT(CL_COMMAND_MARKER, command_marker)
		default:
			ERROR_STOP("Trying to submit unknown command (type %x)", cmd->typ);
	}

	cmd->submitted = 1;
#undef SUBMIT
}

cl_int command_submit_deep_ex(cl_command cmd) {
	if (cmd->submitted == 1)
		return CL_SUCCESS;
	
	/* We set this in order to avoid cyclic dependencies */
	cmd->submitted = 1;

	unsigned int i;
	for (i=0; i<cmd->num_events; i++)
		command_submit_deep(cmd->events[i]->command);
	
	cmd->submitted = 0;

	command_submit_ex(cmd);

	return CL_SUCCESS;
}

void command_graph_dump_ex(cl_command cmd) {

	unsigned int i;
	for (i=0; i<cmd->num_events; i++)
		command_graph_dump_ex(cmd->events[i]->command);

	printf("CMD %p TYPE %d DEPS", cmd, cmd->typ);
	for (i=0; i<cmd->num_events; i++)
		printf(" %p", cmd->events[i]->command);
	printf("\n");

}

#define nullOrDup(name,size) cmd->name = memdup_safe(name,size)
#define dup(name) cmd->name = name
#define dupEntity(name) do { cmd->name = name; gc_entity_retain(name); } while (0);

void soclEnqueueNDRangeKernel_task(void *descr[], void *args);

command_ndrange_kernel command_ndrange_kernel_create (
		cl_kernel        kernel,
		cl_uint          work_dim,
		const size_t *   global_work_offset,
		const size_t *   global_work_size,
		const size_t *   local_work_size)
{
	command_ndrange_kernel cmd = calloc(1, sizeof(struct command_ndrange_kernel_t));
	command_init(cmd, CL_COMMAND_NDRANGE_KERNEL);

	dupEntity(kernel);
	dup(work_dim);
	nullOrDup(global_work_offset, work_dim*sizeof(size_t));
	nullOrDup(global_work_size, work_dim*sizeof(size_t));
	nullOrDup(local_work_size, work_dim*sizeof(size_t));

   	/* Codelet */
   	cmd->codelet = (struct starpu_codelet*)calloc(1, sizeof(struct starpu_codelet));
	struct starpu_codelet * codelet = cmd->codelet;
	codelet->where = STARPU_OPENCL;
	codelet->power_model = NULL;
	codelet->opencl_func = &soclEnqueueNDRangeKernel_task;
	codelet->model = NULL;

   	/* Kernel is mutable, so we duplicate its parameters... */
	cmd->num_args = kernel->num_args;
	cmd->arg_sizes = memdup(kernel->arg_size, sizeof(size_t) * kernel->num_args);
	cmd->arg_types = memdup(kernel->arg_type, sizeof(enum kernel_arg_type) * kernel->num_args);
	cmd->args = memdup_deep_varsize_safe(kernel->arg_value, kernel->num_args, kernel->arg_size);

	return cmd;
}

command_ndrange_kernel command_task_create (cl_kernel kernel) {

	static cl_uint task_work_dim = 3;
	static const size_t task_global_work_offset[3] = {0,0,0};
	static const size_t task_global_work_size[3] = {1,1,1};
	static const size_t * task_local_work_size = NULL;

	command_ndrange_kernel cmd = command_ndrange_kernel_create(
			kernel, task_work_dim, task_global_work_offset,
			task_global_work_size, task_local_work_size);

	/* This is the only difference with command_ndrange_kernel_create */
	cmd->_command.typ = CL_COMMAND_TASK;

	return cmd;
}

command_marker command_barrier_create () {

	command_marker cmd = malloc(sizeof(struct command_marker_t));
	command_init(cmd, CL_COMMAND_BARRIER);

	return cmd;
}

command_marker command_marker_create () {

	command_marker cmd = malloc(sizeof(struct command_marker_t));
	command_init(cmd, CL_COMMAND_MARKER);

	return cmd;
}

command_map_buffer command_map_buffer_create(
		cl_mem buffer,
		cl_map_flags map_flags,
		size_t offset,
		size_t cb,
		cl_event event
		) {

	command_map_buffer cmd = malloc(sizeof(struct command_map_buffer_t));
	command_init(cmd, CL_COMMAND_MAP_BUFFER);

	dupEntity(buffer);
	dup(map_flags);
	dup(offset);
	dup(cb);
	dupEntity(event);

	return cmd;
}

command_unmap_mem_object command_unmap_mem_object_create(cl_mem buffer, void * ptr) {
	command_unmap_mem_object cmd = malloc(sizeof(struct command_unmap_mem_object_t));
	command_init(cmd, CL_COMMAND_UNMAP_MEM_OBJECT);

	dupEntity(buffer);
	dup(ptr);

	return cmd;
}

command_read_buffer command_read_buffer_create(cl_mem buffer, size_t offset, size_t cb, void * ptr) {

	command_read_buffer cmd = malloc(sizeof(struct command_read_buffer_t));
	command_init(cmd, CL_COMMAND_READ_BUFFER);

	dupEntity(buffer);
	dup(offset);
	dup(cb);
	dup(ptr);

	return cmd;
}

command_write_buffer command_write_buffer_create(cl_mem buffer, size_t offset, size_t cb, const void * ptr) {

	command_write_buffer cmd = malloc(sizeof(struct command_write_buffer_t));
	command_init(cmd, CL_COMMAND_WRITE_BUFFER);

	dupEntity(buffer);
	dup(offset);
	dup(cb);
	dup(ptr);

	return cmd;
}

command_copy_buffer command_copy_buffer_create( cl_mem src_buffer, cl_mem dst_buffer,
		size_t src_offset, size_t dst_offset, size_t cb)
{
	command_copy_buffer cmd = malloc(sizeof(struct command_copy_buffer_t));
	command_init(cmd, CL_COMMAND_COPY_BUFFER);

	dupEntity(src_buffer);
	dupEntity(dst_buffer);
	dup(src_offset);
	dup(dst_offset);
	dup(cb);

	return cmd;
}

#undef nullOrDup
#undef nodeNullOrDup
#undef dup
#undef dupEntity
#undef nodeDup
#undef memdup

