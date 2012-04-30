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

#ifndef SOCL_EVENT_H
#define SOCL_EVENT_H

#include "socl.h"

/**
 * Create a new event
 *
 * Events have one-to-one relation with tag. Tag number is event ID
 */
cl_event event_create(void);

/**
 * Generate a unique tag id
 */
int event_unique_id();

#endif /* SOCL_EVENT_H */
