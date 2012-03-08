/* StarPU --- Runtime system for heterogeneous multicore architectures.
 *
 * Copyright (C) 2011 William Braik, Yann Courtois, Jean-Marie Couteyen, Anthony
 * Roy
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

#ifndef __STARPU_TOP_CONNECTION_H__
#define __STARPU_TOP_CONNECTION_H__

#include <stdlib.h>
#include <top/starpu_top_message_queue.h>
#include <starpu_top.h>

#ifdef __cplusplus
extern "C"
{
#endif

extern struct _starpu_top_message_queue* _starpu_top_mt;

/*
 * This function initialize the two communications threads.
 * It initializes the connection and then launches the threads.
 * The function wait the UI connection before launching the threads.
 * About mt : mt MUST be allocated before call.
 * All messages in the queue are freed after used.
 */
void _starpu_top_communications_threads_launcher();

#ifdef __cplusplus
}
#endif

#endif // __STARPU_TOP_CONNECTION_H__

