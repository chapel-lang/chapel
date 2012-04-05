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

struct _cl_platform_id socl_platform = {};

const char * SOCL_PROFILE = "FULL_PROFILE";
const char * SOCL_VERSION = "OpenCL 1.0 StarPU Edition (0.0.1)";
const char * SOCL_PLATFORM_NAME    = "StarPU Platform";
const char * SOCL_VENDOR  = "INRIA";
const char * SOCL_PLATFORM_EXTENSIONS = "";


/* Command queues with profiling enabled
 * This allows us to disable StarPU profiling it
 * is equal to 0
 */
int profiling_queue_count = 0;
