/*
 * Copyright (c) 2015 Los Alamos National Security, LLC. All rights reserved.
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

#include <stdlib.h>
#include "gnix.h"
#include "gnix_queue.h"

int _gnix_queue_create(struct gnix_queue **queue, alloc_func alloc_item,
		       free_func free_item, size_t entry_size,
		       size_t entry_count)
{
	struct gnix_queue *q;
	struct slist_entry *temp;
	int ret = FI_SUCCESS;

	if (!alloc_item || !free_item) {
		ret = -FI_EINVAL;
		goto err;
	}

	q = calloc(1, sizeof(*q));
	if (!q) {
		ret = -FI_ENOMEM;
		goto err;
	}

	q->alloc_item = alloc_item;
	q->free_item = free_item;

	q->entry_size = entry_size;

	slist_init(&q->item_list);
	slist_init(&q->free_list);

	for (size_t count = 0; count < entry_count; count++) {
		temp = q->alloc_item(entry_size);
		if (!temp) {
			ret = -FI_ENOMEM;
			goto err1;
		}

		_gnix_queue_enqueue_free(q, temp);
	}

	*queue = q;

	return ret;

err1:
	_gnix_queue_destroy(q);
	*queue = NULL;
err:
	return ret;
}

void _gnix_queue_destroy(struct gnix_queue *queue)
{
	struct slist_entry *temp;

	while ((temp = _gnix_queue_dequeue(queue)))
		queue->free_item(temp);

	while ((temp = _gnix_queue_dequeue_free(queue)))
		queue->free_item(temp);

	free(queue);
}

struct slist_entry *_gnix_queue_peek(struct gnix_queue *queue)
{
	return queue->item_list.head;
}

struct slist_entry *_gnix_queue_get_free(struct gnix_queue *queue)
{
	struct slist_entry *ret;

	ret = _gnix_queue_dequeue_free(queue);
	if (!ret)
		ret = queue->alloc_item(queue->entry_size);

	return ret;
}

struct slist_entry *_gnix_queue_dequeue(struct gnix_queue *queue)
{
	return slist_remove_head(&queue->item_list);
}

struct slist_entry *_gnix_queue_dequeue_free(struct gnix_queue *queue)
{
	return slist_remove_head(&queue->free_list);
}

void _gnix_queue_enqueue(struct gnix_queue *queue, struct slist_entry *item)
{
	gnix_slist_insert_tail(item, &queue->item_list);
}

void _gnix_queue_enqueue_free(struct gnix_queue *queue,
			      struct slist_entry *item)
{
	slist_insert_head(item, &queue->free_list);
}
