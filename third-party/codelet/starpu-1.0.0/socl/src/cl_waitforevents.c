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

CL_API_ENTRY cl_int CL_API_CALL
soclWaitForEvents(cl_uint           num_events,
                const cl_event *    event_list) CL_API_SUFFIX__VERSION_1_0
{
   unsigned int i;
   DEBUG_MSG("Waiting for events: ");
   for (i=0; i<num_events; i++) {
   	command_graph_dump(event_list[i]->command);

   	/* We need to submit commands if it's not already done */
	command_submit_deep(event_list[i]->command);

      	DEBUG_MSG_NOHEAD("%d ", event_list[i]->id);
   }
   DEBUG_MSG_NOHEAD("\n");

   for (i=0; i<num_events; i++)
      starpu_tag_wait(event_list[i]->id);

   DEBUG_MSG("Stop waiting :)\n");
   return CL_SUCCESS;
}
