/*
 * Copyright (C) 2016 by Argonne National Laboratory.
 * Copyright (C) 2021-2023 Cornelis Networks.
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
#ifndef _FI_PROV_OPX_INTERNAL_H_
#define _FI_PROV_OPX_INTERNAL_H_

#include <config.h>

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdio.h>

#define FI_OPX_CACHE_LINE_SIZE	(64)

#define FI_OPX_CQ_CONTEXT_EXT		(0x8000000000000000ull)
#define FI_OPX_CQ_CONTEXT_MULTIRECV	(0x4000000000000000ull)

union fi_opx_mp_egr_id {
	uint64_t		id;

	struct {
		uint32_t	uid;
		uint16_t	slid;
		uint8_t		origin_tx;
		uint8_t		unused;
	};
} __attribute__((__packed__));

union fi_opx_context {
	struct fi_context2			context;
	struct {
		//struct slist_entry		entry;		/* fi_cq_entry::op_context */
		union fi_opx_context *	next;		/* fi_cq_entry::op_context */
		uint64_t			flags;		/* fi_cq_msg_entry::flags */
		size_t				len;		/* fi_cq_msg_entry::len */
		void				*buf;		/* fi_cq_data_entry::buf (unused for tagged cq's and non-multi-receive message cq's) */

		union {
			uint64_t		data;		/* fi_cq_data_entry::data; only used _after_ a message is matched */
			fi_addr_t		src_addr;	/* only used _before_ a message is matched ('FI_DIRECTED_RECEIVE') */
		};

		union {
			uint64_t		tag;		/* fi_cq_tagged_entry::tag */
			union fi_opx_context	*multi_recv_next;	/* only for multi-receives; which is not tagged */
		};
		union {
			uint64_t		ignore;		/* only for tagged receive */
			void 			*claim;		/* only for peek/claim */
			void			*multi_recv_context;	/* only for individual FI_MULTI_RECV's */
			union fi_opx_mp_egr_id	mp_egr_id;
		};

		volatile uint64_t	byte_counter;
	};
};

struct fi_opx_extended_context {
	union fi_opx_context context;
	void *src_addr;
};

struct fi_opx_context_slist {
	union fi_opx_context *	head;
	union fi_opx_context *	tail;
};

static inline void fi_opx_context_slist_init (struct fi_opx_context_slist* list)
{
	list->head = list->tail = NULL;
}

static inline int fi_opx_context_slist_empty (struct fi_opx_context_slist* list)
{
	return !list->head;
}

static inline void fi_opx_context_slist_insert_head (union fi_opx_context *item,
		struct fi_opx_context_slist* list)
{
	assert(item->next == NULL);
	if (fi_opx_context_slist_empty(list))
		list->tail = item;
	else
		item->next = list->head;

	list->head = item;
}

static inline void fi_opx_context_slist_insert_tail (union fi_opx_context *item,
		struct fi_opx_context_slist* list)
{
	assert(item->next == NULL);
	if (fi_opx_context_slist_empty(list))
		list->head = item;
	else
		list->tail->next = item;

	list->tail = item;
}

static inline void fi_opx_context_slist_remove_item (union fi_opx_context *item,
		union fi_opx_context *prev, struct fi_opx_context_slist *list)
{
	if (prev) {
		prev->next = item->next;
	} else {
		list->head = item->next;
	}

	if (item->next == NULL) {
		list->tail = prev;
	}

	item->next = NULL;
}

struct fi_opx_context_ext {
	union fi_opx_context		opx_context;
	struct fi_cq_err_entry		err_entry;
	struct {
		struct fi_context	*op_context;
		size_t			iov_count;
		struct iovec		*iov;
	} msg;
};

#ifndef MAX
#define MAX(a,b) ((a)^(((a)^(b))&-((a)<(b))))
#endif
#ifndef MIN
#define MIN(a,b) ((b)^(((a)^(b))&-((a)<(b))))
#endif
#ifndef MIN3
#define MIN3(a,b,c) (MIN(MIN(a,b),c))
#endif
#ifndef MIN4
#define MIN4(a,b,c,d) (MIN(MIN(a,b),MIN(c,d)))
#endif

#define FI_OPX_LOCK_REQUIRED 1
#define FI_OPX_LOCK_NOT_REQUIRED 0

static inline int fi_opx_threading_unknown(const enum fi_threading threading)
{
	return threading != FI_THREAD_DOMAIN &&		// Most likely
		threading != FI_THREAD_ENDPOINT &&
		threading != FI_THREAD_SAFE &&
		threading != FI_THREAD_COMPLETION &&
		threading != FI_THREAD_FID &&
		threading != FI_THREAD_UNSPEC;		// Least likely
}

static inline int fi_opx_threading_lock_required(const enum fi_threading threading, enum fi_progress progress)
{
	return !(threading == FI_THREAD_DOMAIN ||
		 threading == FI_THREAD_ENDPOINT ||
		 threading == FI_THREAD_COMPLETION) ||
		 progress == FI_PROGRESS_AUTO;
}

static inline void fi_opx_lock_if_required (ofi_spin_t *lock, const int required)
{
	if (required) ofi_spin_lock(lock);
}

static inline void fi_opx_lock (ofi_spin_t *lock)
{
	ofi_spin_lock(lock);
}

static inline void fi_opx_unlock_if_required (ofi_spin_t *lock, const int required)
{
	if (required) ofi_spin_unlock(lock);
}

static inline void fi_opx_unlock (ofi_spin_t *lock)
{
	ofi_spin_unlock(lock);
}

#endif /* _FI_PROV_OPX_INTERNAL_H_ */
