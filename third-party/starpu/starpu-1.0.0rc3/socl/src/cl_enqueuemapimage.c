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

CL_API_ENTRY void * CL_API_CALL
soclEnqueueMapImage(cl_command_queue  UNUSED(command_queue),
                  cl_mem            UNUSED(image), 
                  cl_bool           UNUSED(blocking_map), 
                  cl_map_flags      UNUSED(map_flags), 
                  const size_t *    UNUSED(origin),
                  const size_t *    UNUSED(region),
                  size_t *          UNUSED(image_row_pitch),
                  size_t *          UNUSED(image_slice_pitch),
                  cl_uint           UNUSED(num_events_in_wait_list),
                  const cl_event *  UNUSED(event_wait_list),
                  cl_event *        UNUSED(event),
                  cl_int *          errcode_ret) CL_API_SUFFIX__VERSION_1_0
{
   if (errcode_ret != NULL)
      *errcode_ret = CL_INVALID_OPERATION;

   return NULL;
}
