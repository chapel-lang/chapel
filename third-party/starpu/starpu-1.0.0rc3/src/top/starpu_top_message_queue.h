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

#include <sys/types.h>
#include <semaphore.h>
#include <pthread.h>

#ifndef __STARPU_TOP_MESSAGE_QUEUE_H__
#define __STARPU_TOP_MESSAGE_QUEUE_H__

struct _starpu_top_message_queue_item
{
	char *message;
	struct _starpu_top_message_queue_item* next;
};

struct _starpu_top_message_queue
{
	struct _starpu_top_message_queue_item* head;
	struct _starpu_top_message_queue_item* tail;
	sem_t semaphore;
	pthread_mutex_t mutex;
};


struct _starpu_top_message_queue *_starpu_top_message_add(struct _starpu_top_message_queue*,
							char*);

char* _starpu_top_message_remove(struct _starpu_top_message_queue*);

struct _starpu_top_message_queue* _starpu_top_message_queue_new();
struct _starpu_top_message_queue* _starpu_top_message_queue_free(struct _starpu_top_message_queue*);

#endif
