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
#include "task.h"
#include "gc.h"

/**
 * WARNING: command queues do NOT hold references on events. Only events hold references
 * on command queues. This way, event release will automatically remove the event from
 * its command queue.
 */


/**
 * Returned implicit dependencies for a task
 * Command queue must be locked!
 */
void command_queue_dependencies_implicit(
	cl_command_queue cq, 	/* Command queue */
	char is_barrier,	/* Is the task a barrier */
	cl_int * ret_num_events,	/* Returned number of dependencies */
	cl_event ** ret_events	/* Returned dependencies */
) {

	/*********************
	 * Count dependencies
	 *********************/
	int ndeps = 0;

	/* Add dependency to last barrier if applicable */
	if (cq->barrier != NULL)
		ndeps++;

	/* Add dependencies to out-of-order events (if any) */
	if (is_barrier) {
		command_list cl = cq->commands;
		while (cl != NULL) {
			ndeps++;
			cl = cl->next;
		}
	}

	/*********************
	 * Return dependencies
	 *********************/

	cl_event * evs = malloc(ndeps * sizeof(cl_event));
	int n = 0;

	/* Add dependency to last barrier if applicable */
	if (cq->barrier != NULL)
		evs[n++] = cq->barrier->event;

	/* Add dependencies to out-of-order events (if any) */
	if (is_barrier) {
		command_list cl = cq->commands;
		while (cl != NULL) {
			evs[n++] = cl->cmd->event;
			cl = cl->next;
		}
	}

	*ret_num_events = ndeps;
	*ret_events = evs;
}
	
/**
 * Insert a command in the command queue
 * The command queue must be locked!
 */
void command_queue_insert(
	cl_command_queue 	cq, 	/* Command queue */
	cl_command 		cmd,	/* Command */
	int 			is_barrier		/* Is the task a barrier */
) {

	int in_order = !(cq->properties & CL_QUEUE_OUT_OF_ORDER_EXEC_MODE_ENABLE);

	if (is_barrier)
		cq->commands = NULL;

	/* Add command to the list of out-of-order commands */
	if (!in_order)
		cq->commands = command_list_cons(cmd, cq->commands);

	/* Register this event as last barrier */
	if (is_barrier || in_order)
		cq->barrier = cmd;

	/* Add reference to the command queue */
	gc_entity_store(&cmd->event->cq, cq);
}

/**
 * Return implicit and explicit dependencies for a task
 * The command queue must be locked!
 */
void command_queue_dependencies(
	cl_command_queue 	cq,		/* Command queue */
	int 			is_barrier,	/* Is the task a barrier */
	cl_int 			num_events,	/* Number of explicit dependencies */
	const cl_event *	events,		/* Explicit dependencies */
	cl_int * 		ret_num_events,	/* Returned number of dependencies */
	cl_event ** 		ret_events	/* Returned dependencies */
) {
	cl_int implicit_num_events;
	cl_event * implicit_events;

	/* Implicit dependencies */
	command_queue_dependencies_implicit(cq, is_barrier, &implicit_num_events, &implicit_events);

	/* Explicit dependencies */
	cl_int ndeps = implicit_num_events + num_events;
	cl_event * evs = malloc(sizeof(cl_event) * ndeps);
	memcpy(evs, implicit_events, sizeof(cl_event) * implicit_num_events);
	memcpy(&evs[implicit_num_events], events, sizeof(cl_event) * num_events);

	*ret_num_events = ndeps;
	*ret_events = evs;
}

void command_queue_enqueue_ex(cl_command_queue cq, cl_command cmd, cl_uint num_events, const cl_event * events) {

	/* Check if the command is a barrier */
	int is_barrier = 0;
	if (cmd->typ == CL_COMMAND_BARRIER) {
		is_barrier = 1;
		/* OpenCL has no CL_COMMAND_BARRIER type, so we fall back on CL_COMMAND_MARKER */
		cmd->typ = CL_COMMAND_MARKER;
	}

	/* Set command queue field */
	cmd->cq = cq;

	/* Lock command queue */
	pthread_mutex_lock(&cq->mutex);

	//FIXME: crappy separation (command_queue_dependencies + command_queue_insert)

	/* Get all (explicit + implicit) dependencies */
	cl_int all_num_events;
	cl_event * all_events;
	command_queue_dependencies(cq, is_barrier, num_events, events, &all_num_events, &all_events);

	/* Make all dependencies explicit for the command */
	cmd->num_events = all_num_events;
	cmd->events = all_events;

	/* Insert command in the queue */
	command_queue_insert(cq, cmd, is_barrier);

	/* Unlock command queue */
	pthread_mutex_unlock(&cq->mutex);
}
