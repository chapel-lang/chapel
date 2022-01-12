/*
* Copyright (c) 2015-2016 Intel Corporation, Inc.  All rights reserved.
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

#ifndef _FI_NETDIR_QUEUE_H_
#define _FI_NETDIR_QUEUE_H_

#include <windows.h>

#include "rdma/fabric.h"

#include "ofi.h"
#include "ofi_osd.h"

#include "netdir.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

struct nd_queue_item {
	struct nd_queue_item	*next;
};

__declspec(align(16)) struct nd_queue_queue {
	union {
		struct {
			struct nd_queue_item	*head;
			struct nd_queue_item	*tail;
		};
		volatile LONG64 exchange[2];
	};
};

/* push front call is non-blocking thread safe */
static inline void ofi_nd_queue_push_front(struct nd_queue_queue *queue,
	struct nd_queue_item *item)
{
	assert(queue);

	item->next = 0;
	BOOLEAN success;

	struct {
		struct nd_queue_item *head;
		struct nd_queue_item *tail;
	} src;

	do {
		src.head = queue->head;
		src.tail = queue->tail;

		LONG64 head = (LONG64)(src.head ? src.head : item);
		LONG64 tail = (LONG64)item;
		__declspec(align(16)) LONG64 compare[2] = { (LONG64)src.head, (LONG64)src.tail };
		success = InterlockedCompareExchange128(
			queue->exchange, tail, head, compare);
	} while (!success);

	if (src.tail) {
		item->next = src.head;
		src.head = item;
		WakeByAddressAll(&src.head);
	}
}

/* push call is non-blocking thread safe */
static inline void ofi_nd_queue_push(struct nd_queue_queue *queue,
				     struct nd_queue_item *item)
{
	assert(queue);

	item->next = 0;
	BOOLEAN success;

	struct {
		struct nd_queue_item *head;
		struct nd_queue_item *tail;
	} src;

	do {
		src.head = queue->head;
		src.tail = queue->tail;

		LONG64 head = (LONG64)(src.head ? src.head : item);
		LONG64 tail = (LONG64)item;
		__declspec(align(16)) LONG64 compare[2] = {(LONG64)src.head, (LONG64)src.tail};
		success = InterlockedCompareExchange128(
			queue->exchange, tail, head, compare);
	} while (!success);

	if (src.tail) {
		src.tail->next = item;
		WakeByAddressAll(&src.tail->next);
	}
}

/* pop call is NOT thread safe, it allows only one consumer, but it is
   safe to be used with push operation without locks */
static inline int ofi_nd_queue_pop(struct nd_queue_queue *queue,
				   struct nd_queue_item **item)
{
	assert(queue);
	assert(item);

	BOOLEAN success;
	struct {
		struct nd_queue_item *head;
		struct nd_queue_item *tail;
	} src;

	do {
		src.head = queue->head;
		src.tail = queue->tail;

		if (!src.head)
			return 0;

		/* here is potential thread race: object located at src.head
		   may be destroyed while we're waiting. that is why pop
		   operation is not thread safe */
		if (src.head != src.tail) {
			/* in case if head and tail are not same - ensure that
			   head->next element is not NULL */
			void *zero = NULL;
			while (!src.head->next) {
				WaitOnAddress(&src.head->next, &zero, sizeof(zero), INFINITE);
			}
		}

		LONG64 head = (LONG64)src.head->next;
		LONG64 tail = (LONG64)(src.head != src.tail ? src.tail : NULL);
		__declspec(align(16)) LONG64 compare[2] = {(LONG64)src.head, (LONG64)src.tail};
		success = InterlockedCompareExchange128(
			queue->exchange, tail, head, compare);
	} while (!success);

	*item = src.head;

	return (*item) != NULL;
}

/* peek call is NOT thread safe, it allows only one consumer */
static inline int ofi_nd_queue_peek(struct nd_queue_queue *queue,
				    struct nd_queue_item **item)
{
	assert(queue);
	assert(item);

	*item = queue->head;
	return (*item) != 0;
}

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _FI_NETDIR_QUEUE_H_ */

