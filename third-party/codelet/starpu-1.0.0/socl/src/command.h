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

#ifndef SOCL_COMMANDS_H
#define SOCL_COMMANDS_H

typedef struct cl_command_t * cl_command;

/**
 * Initialize a command structure
 *
 * Command constructors for each kind of command use this method
 * Implicit and explicit dependencies must be passed as parameters
 */
void command_init_ex(cl_command cmd, cl_command_type typ);
#define command_init(cmd,typ) \
	command_init_ex((cl_command)cmd,typ)

/** Submit a command for execution */
void command_submit_ex(cl_command cmd);
#define command_submit(cmd) \
	command_submit_ex(&(cmd)->_command)

/** Submit a command and its dependencies */
cl_int command_submit_deep_ex(cl_command cmd);
#define command_submit_deep(cmd) (command_submit_deep_ex((cl_command)cmd))

void command_graph_dump_ex(cl_command cmd);
#define command_graph_dump(cmd) (command_graph_dump_ex((cl_command)cmd))

/**************************
 * OpenCL Commands
 **************************/
struct cl_command_t {
	cl_command_type	typ;	 	/* Command type */
	cl_uint 	num_events;	/* Number of dependencies */
	cl_event * 	events;		/* Dependencies */
	cl_event  	event;		/* Event for this command */
	cl_command_queue cq;		/* Command queue the command is enqueued in */
	starpu_task	task;		/* Associated StarPU task, if any */
	char		submitted;	/* True if the command has been submitted to StarPU */
};

#define command_type_get(cmd) (((cl_command)cmd)->typ)
#define command_event_get(cmd) (((cl_command)cmd)->event)
#define command_num_events_get(cmd) (((cl_command)cmd)->num_events)
#define command_events_get(cmd) (((cl_command)cmd)->events)
#define command_task_get(cmd) (((cl_command)cmd)->task)
#define command_cq_get(cmd) (((cl_command)cmd)->cq)

#define CL_COMMAND struct cl_command_t _command;

typedef struct command_ndrange_kernel_t {
	CL_COMMAND

	cl_kernel        kernel;
	cl_uint          work_dim;
	const size_t *   global_work_offset;
	const size_t *   global_work_size;
	const size_t *   local_work_size;
	cl_uint 	 num_args;
	size_t *	 arg_sizes;
	enum kernel_arg_type * arg_types;
	void **		 args;
	struct starpu_codelet * codelet;
	cl_uint		 num_buffers;
	cl_mem *	 buffers;
} * command_ndrange_kernel;


typedef struct command_read_buffer_t {
	CL_COMMAND
	
	cl_mem buffer;
	size_t offset;
	size_t cb;
	void * ptr;
} * command_read_buffer;


typedef struct command_write_buffer_t {
	CL_COMMAND

	cl_mem buffer;
	size_t offset;
	size_t cb;
	const void * ptr;
} * command_write_buffer;


typedef struct command_copy_buffer_t {
	CL_COMMAND
	
	cl_mem src_buffer;
	cl_mem dst_buffer;
	size_t src_offset;
	size_t dst_offset;
	size_t cb;
} * command_copy_buffer;


typedef struct command_map_buffer_t {
	CL_COMMAND

	cl_mem buffer;
	cl_map_flags map_flags;
	size_t offset;
	size_t cb;
	cl_event event;
} * command_map_buffer;


typedef struct command_unmap_mem_object_t {
	CL_COMMAND

	cl_mem buffer;
	void * ptr;
} * command_unmap_mem_object;


typedef struct command_marker_t {
	CL_COMMAND
} * command_marker;

/*************************
 * Constructor functions
 *************************/

command_ndrange_kernel command_ndrange_kernel_create (
		cl_kernel        kernel,
		cl_uint          work_dim,
		const size_t *   global_work_offset,
		const size_t *   global_work_size,
		const size_t *   local_work_size);

command_ndrange_kernel command_task_create (cl_kernel kernel);

command_marker command_barrier_create ();

command_marker command_marker_create ();

command_map_buffer command_map_buffer_create(
		cl_mem buffer,
		cl_map_flags map_flags,
		size_t offset,
		size_t cb,
		cl_event event);

command_unmap_mem_object command_unmap_mem_object_create(
		cl_mem buffer,
		void * ptr);

command_read_buffer command_read_buffer_create(
		cl_mem buffer,
		size_t offset,
		size_t cb,
		void * ptr);

command_write_buffer command_write_buffer_create(
		cl_mem buffer,
		size_t offset,
		size_t cb,
		const void * ptr);

command_copy_buffer command_copy_buffer_create(
		cl_mem src_buffer,
		cl_mem dst_buffer,
		size_t src_offset,
		size_t dst_offset,
		size_t cb);

/*************************
 * Submit functions
 *************************/
cl_int command_ndrange_kernel_submit(command_ndrange_kernel cmd);
cl_int command_read_buffer_submit(command_read_buffer cmd);
cl_int command_write_buffer_submit(command_write_buffer cmd);
cl_int command_copy_buffer_submit(command_copy_buffer cmd);
cl_int command_map_buffer_submit(command_map_buffer cmd);
cl_int command_unmap_mem_object_submit(command_unmap_mem_object cmd);
cl_int command_marker_submit(command_marker cmd);


#endif /* SOCL_COMMANDS_H */

