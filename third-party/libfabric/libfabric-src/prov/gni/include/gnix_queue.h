/*
 * Copyright (c) 2015 Los Alamos National Security, LLC. All rights reserved.
 * Copyright (c) 2016 Cray Inc.  All rights reserved.
 *
 * This software is available to you under a choice of one of two
 * licenses.  You may choose to be licensed under the terms of the GNU
 * General Public License (GPL) Version 2, available from the file
 * COPYING in the main directory of this source tree, or the
 * BSD license below:
 *
 *     Redistribution and use in source and binary forms, with or
 *     without modification, are permitted provided that the following
 *     conditions are met:
 *
 *      - Redistributions of source code must retain the above
 *        copyright notice, this list of conditions and the following
 *        disclaimer.
 *
 *      - Redistributions in binary form must reproduce the above
 *        copyright notice, this list of conditions and the following
 *        disclaimer in the documentation and/or other materials
 *        provided with the distribution.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef _GNIX_QUEUE_H
#define _GNIX_QUEUE_H

#include <ofi_list.h>

typedef struct slist_entry *(*alloc_func)(size_t entry_size);
typedef void (*free_func)(struct slist_entry *item);

struct gnix_queue {
	struct slist item_list;
	struct slist free_list;

	alloc_func alloc_item;
	free_func free_item;

	size_t entry_size;
};

int _gnix_queue_create(struct gnix_queue **queue, alloc_func alloc_item,
		       free_func free_item, size_t entry_size,
		       size_t entry_count);
void _gnix_queue_destroy(struct gnix_queue *queue);

struct slist_entry *_gnix_queue_peek(struct gnix_queue *queue);

struct slist_entry *_gnix_queue_get_free(struct gnix_queue *queue);
struct slist_entry *_gnix_queue_dequeue(struct gnix_queue *queue);
struct slist_entry *_gnix_queue_dequeue_free(struct gnix_queue *queue);

void _gnix_queue_enqueue(struct gnix_queue *queue, struct slist_entry *item);
void _gnix_queue_enqueue_free(struct gnix_queue *queue,
			      struct slist_entry *item);

#endif /* #define _GNIX_QUEUE_H */
