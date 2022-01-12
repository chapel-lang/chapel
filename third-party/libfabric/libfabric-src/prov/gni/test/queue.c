/*
 * Copyright (c) 2015 Los Alamos National Security, LLC. All rights reserved.
 * Copyright (c) 2015-2016 Cray Inc.  All rights reserved.
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

#include "gnix_queue.h"

#include <criterion/criterion.h>
#include "gnix_rdma_headers.h"

struct gnix_queue *queue;

struct int_entry {
	int x;
	struct slist_entry item;
};

static struct slist_entry *alloc_int_entry(size_t size)
{
	(void) size;
	struct int_entry *entry = malloc(sizeof(*entry));

	if (!entry)
		return NULL;

	return &entry->item;
}

static void free_int_entry(struct slist_entry *list_entry)
{
	struct int_entry *entry;

	entry = container_of(list_entry, struct int_entry, item);

	free(entry);
}

void setup_count_eight(void)
{
	int ret;

	ret = _gnix_queue_create(&queue, alloc_int_entry, free_int_entry,
				 0, 8);
	cr_assert_eq(ret, FI_SUCCESS, "failed to create queue.");
}

void setup_count_zero(void)
{
	int ret;

	ret = _gnix_queue_create(&queue, alloc_int_entry, free_int_entry,
				 0, 0);
	cr_assert_eq(ret, FI_SUCCESS, "failed to create queue.");
}

void teardown_queue(void)
{
	_gnix_queue_destroy(queue);
}

TestSuite(empty, .init = setup_count_zero, .fini = teardown_queue);

Test(empty, null_read)
{
	struct slist_entry *list_entry;

	list_entry = _gnix_queue_dequeue(queue);
	cr_expect(!list_entry, "non null read on empty queue.");

	list_entry = _gnix_queue_dequeue_free(queue);
	cr_expect(!list_entry, "non null read on free list of empty queue.");

	list_entry = _gnix_queue_peek(queue);
	cr_expect(!list_entry, "non null peek on empty queue.");
}

Test(empty, single_write)
{
	struct slist_entry *list_entry;
	struct int_entry *entry;

	/*
	 * Write single entry with value 4 to queue.
	 */
	list_entry = _gnix_queue_get_free(queue);
	cr_expect(list_entry, "null entry from queue get free function.");

	entry = container_of(list_entry, struct int_entry, item);

	entry->x = 4;

	_gnix_queue_enqueue(queue, &entry->item);

	/*
	 * Read back entry with value 4.
	 */
	list_entry = _gnix_queue_dequeue(queue);
	cr_expect(list_entry, "null entry from queue after enqueue.");

	entry = container_of(list_entry, struct int_entry, item);

	cr_expect_eq(4, entry->x, "entry does not contain assigned value.");

	/*
	 * Add to free list.
	 */
	_gnix_queue_enqueue_free(queue, &entry->item);

	/*
	 * Read from now empty queue.
	 */
	list_entry = _gnix_queue_dequeue(queue);
	cr_expect(!list_entry, "entry read from empty queue is non-null.");

	/*
	 * Read from free and make sure it's the same.
	 */
	list_entry = _gnix_queue_get_free(queue);
	cr_expect(list_entry,
		  "null entry from free queue after adding to free.");

	entry = container_of(list_entry, struct int_entry, item);

	cr_expect_eq(4, entry->x, "entry does not contain assigned value.");

	/*
	 * Completely empty list. Shouldn't seg fault on teardown.
	 */
	queue->free_item(&entry->item);
}

TestSuite(eight, .init = setup_count_eight, .fini = teardown_queue);

Test(eight, read_nine)
{
	struct slist_entry *list_entry;
	struct int_entry *entry;

	/*
	 * Fill the queue. The value of each will be the counter position.
	 */
	for (size_t i = 0; i < 8; i++) {
		list_entry = _gnix_queue_get_free(queue);
		cr_expect(list_entry,
			  "null entry from queue get free function.");

		entry = container_of(list_entry, struct int_entry, item);

		entry->x = i;

		_gnix_queue_enqueue(queue, &entry->item);
	}

	/*
	 * Peek and make sure the top of queue is 0.
	 */
	list_entry = _gnix_queue_peek(queue);
	cr_expect(list_entry, "null entry from peek.");

	entry = container_of(list_entry, struct int_entry, item);

	cr_expect_eq(0, entry->x, "value of peek isn't first added to queue.");

	/*
	 * Peek again and make sure it's still 0.
	 */
	list_entry = _gnix_queue_peek(queue);
	cr_expect(list_entry, "null entry from peek.");

	entry = container_of(list_entry, struct int_entry, item);

	cr_expect_eq(0, entry->x, "value of peek isn't first added to queue.");

	/*
	 * Read it back.
	 */
	for (size_t i = 0; i < 8; i++) {
		list_entry = _gnix_queue_dequeue(queue);
		cr_expect(list_entry, "null entry from queue dequeue.");

		entry = container_of(list_entry, struct int_entry, item);

		cr_expect_eq(i, entry->x, "value not same as assigned.");

		_gnix_queue_enqueue_free(queue, &entry->item);
	}

	/*
	 * Read an extra item. Should return null.
	 */
	list_entry = _gnix_queue_dequeue(queue);
	cr_expect(!list_entry, "entry from empty queue not null.");
}
