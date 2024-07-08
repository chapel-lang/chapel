/*
 * Copyright (C) 2016 by Argonne National Laboratory.
 * Copyright (C) 2022 Cornelis Networks.
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
#ifndef _FI_PROV_OPX_EQ_H_
#define _FI_PROV_OPX_EQ_H_

#include <assert.h>
#include <unistd.h>
#include <stdint.h>
#include <ofi_mem.h>
#include "rdma/opx/fi_opx_internal.h"
#include "rdma/opx/fi_opx_endpoint.h"
#include "rdma/opx/fi_opx_hfi1.h"
#include "rdma/opx/fi_opx_progress.h"

/* Macro indirection in order to support other macros as arguments
 * C requires another indirection for expanding macros since
 * operands of the token pasting operator are not expanded */

#define FI_OPX_CQ_SPECIALIZED_FUNC_NON_LOCKING(FORMAT, RELIABILITY, MASK, CAPS)	\
	FI_OPX_CQ_SPECIALIZED_FUNC_NON_LOCKING_(FORMAT, RELIABILITY, MASK, CAPS)

#define FI_OPX_CQ_SPECIALIZED_FUNC_NON_LOCKING_(FORMAT, RELIABILITY, MASK, CAPS)\
    __OPX_FORCE_INLINE__ ssize_t                      \
	fi_opx_cq_read_ ## FORMAT ## _0_ ## RELIABILITY ## _ ## MASK ## _ ## CAPS		\
		(struct fid_cq *cq, void *buf, size_t count)			\
	{									\
		return fi_opx_cq_read_generic_non_locking(cq, buf, count,	\
				FORMAT, RELIABILITY, MASK, CAPS);	\
	}									\
	__OPX_FORCE_INLINE__ ssize_t                          \
	fi_opx_cq_readfrom_ ## FORMAT ## _0_ ## RELIABILITY ## _ ## MASK ## _ ## CAPS		\
		(struct fid_cq *cq, void *buf, size_t count,			\
			fi_addr_t *src_addr)					\
	{									\
		return fi_opx_cq_readfrom_generic_non_locking(cq, buf, count,	\
				src_addr, FORMAT, RELIABILITY, MASK,		\
				CAPS);				\
	}									\

#define FI_OPX_CQ_SPECIALIZED_FUNC_LOCKING(FORMAT, RELIABILITY, MASK, CAPS)	\
	FI_OPX_CQ_SPECIALIZED_FUNC_LOCKING_(FORMAT, RELIABILITY, MASK, CAPS)

#define FI_OPX_CQ_SPECIALIZED_FUNC_LOCKING_(FORMAT, RELIABILITY, MASK, CAPS)\
    __OPX_FORCE_INLINE__ ssize_t                      \
	fi_opx_cq_read_ ## FORMAT ## _1_ ## RELIABILITY ## _ ## MASK ## _ ## CAPS		\
		(struct fid_cq *cq, void *buf, size_t count)			\
	{									\
		return fi_opx_cq_read_generic_locking(cq, buf, count,		\
				FORMAT, RELIABILITY, MASK, CAPS);	\
	}									\
	__OPX_FORCE_INLINE__ ssize_t                          \
	fi_opx_cq_readfrom_ ## FORMAT ## _1_ ## RELIABILITY ## _ ## MASK ## _ ## CAPS	\
		(struct fid_cq *cq, void *buf, size_t count,			\
			fi_addr_t *src_addr)					\
	{									\
		return fi_opx_cq_readfrom_generic_locking(cq, buf, count,	\
				src_addr, FORMAT, RELIABILITY, MASK,		\
				CAPS);				\
	}									\

#define FI_OPX_CQ_SPECIALIZED_FUNC_NAME(TYPE, FORMAT, LOCK, RELIABILITY, MASK, CAPS)			\
	FI_OPX_CQ_SPECIALIZED_FUNC_NAME_(TYPE, FORMAT, LOCK, RELIABILITY, MASK, CAPS)

#define FI_OPX_CQ_SPECIALIZED_FUNC_NAME_(TYPE, FORMAT, LOCK, RELIABILITY, MASK, CAPS)			\
		fi_opx_ ## TYPE ## _ ## FORMAT ## _ ## LOCK ## _ ## RELIABILITY ## _ ## MASK ## _ ## CAPS


#ifdef __cplusplus
extern "C" {
#endif

struct fi_opx_cntr {
	struct fid_cntr		cntr_fid;

	ofi_atomic64_t		std;
	ofi_atomic64_t		err;

	struct {
		uint64_t		ep_count;
		struct fi_opx_ep	*ep[64];	/* TODO - check this array size */
	} progress;

	uint64_t		ep_bind_count;
	struct fi_opx_ep	*ep[64];	/* TODO - check this array size */

	struct fi_cntr_attr	*attr;
	struct fi_opx_domain	*domain;
	enum fi_threading	threading;
	int			lock_required;
};

/* This structure is organized in a way that minimizes cacheline use for the
 * "FI_PROGRESS_MANUAL + inject" poll scenario.
 */
struct fi_opx_cq {
	struct fid_cq			cq_fid;		/* must be the first field in the structure; 24 + 64 bytes */
	uint64_t			pad_0[5];

	/* == CACHE LINE == */

	struct fi_opx_context_slist	pending;
	struct fi_opx_context_slist	completed;
	struct fi_opx_context_slist	err;		/* 'struct fi_opx_context_ext' element linked list */

	struct {
		uint64_t		ep_count;
		struct fi_opx_ep	*ep[64];	/* TODO - check this array size */
	} progress;

	struct fi_opx_progress_track	*progress_track;

//	struct fi_opx_context_ext	*err_tail;
	uint64_t			pad_1[9];

	struct fi_opx_domain		*domain;
	uint64_t			bflags;		/* fi_opx_bind_ep_cq() */
	size_t				size;
	enum fi_cq_format		format;

	uint64_t			ep_bind_count;
	struct fi_opx_ep		*ep[64];		/* TODO - check this array size */

	uint64_t			ep_comm_caps;

	int64_t				ref_cnt;
	ofi_spin_t			lock;
};


int fi_opx_eq_open(struct fid_fabric *fabric, struct fi_eq_attr *attr,
		struct fid_eq **eq, void *context);


#define DUMP_ENTRY_INPUT(entry)	\
({				\
	fprintf(stderr,"%s:%s():%d entry = %p\n", __FILE__, __func__, __LINE__, (entry));					\
	fprintf(stderr,"%s:%s():%d   op_context = %p\n", __FILE__, __func__, __LINE__, (entry)->tagged.op_context);		\
	fprintf(stderr,"%s:%s():%d   flags      = 0x%016lx\n", __FILE__, __func__, __LINE__, (entry)->tagged.flags);		\
	fprintf(stderr,"%s:%s():%d   len        = %zu\n", __FILE__, __func__, __LINE__, (entry)->tagged.len);			\
	fprintf(stderr,"%s:%s():%d   buf        = %p\n", __FILE__, __func__, __LINE__, (entry)->tagged.buf);			\
	fprintf(stderr,"%s:%s():%d   ignore     = 0x%016lx\n", __FILE__, __func__, __LINE__, (entry)->recv.ignore);		\
	fprintf(stderr,"%s:%s():%d   tag        = 0x%016lx\n", __FILE__, __func__, __LINE__, (entry)->tagged.tag);		\
	fprintf(stderr,"%s:%s():%d   entry_kind = %u\n", __FILE__, __func__, __LINE__, (entry)->recv.entry_kind);		\
	fprintf(stderr,"%s:%s():%d   entry_id   = %u\n", __FILE__, __func__, __LINE__, (entry)->recv.entry_id);		\
})

int fi_opx_cq_enqueue_err (struct fi_opx_cq * opx_cq,
		struct fi_opx_context_ext * ext,
		const int lock_required);

struct fi_ops_cq * fi_opx_cq_select_non_locking_2048_ops(const enum fi_cq_format format,
							const enum ofi_reliability_kind reliability,
							const uint64_t comm_caps);

struct fi_ops_cq * fi_opx_cq_select_non_locking_8192_ops(const enum fi_cq_format format,
							const enum ofi_reliability_kind reliability,
							const uint64_t comm_caps);

struct fi_ops_cq * fi_opx_cq_select_non_locking_runtime_ops(const enum fi_cq_format format,
							const enum ofi_reliability_kind reliability,
							const uint64_t comm_caps);

struct fi_ops_cq * fi_opx_cq_select_locking_2048_ops(const enum fi_cq_format format,
							const enum ofi_reliability_kind reliability,
							const uint64_t comm_caps);

struct fi_ops_cq * fi_opx_cq_select_locking_8192_ops(const enum fi_cq_format format,
                                                        const enum ofi_reliability_kind reliability,
                                                        const uint64_t comm_caps);

struct fi_ops_cq * fi_opx_cq_select_locking_runtime_ops(const enum fi_cq_format format,
                                                        const enum ofi_reliability_kind reliability,
                                                        const uint64_t comm_caps);

void fi_opx_cq_debug(struct fid_cq *cq, char *func, const int line);

static inline
int fi_opx_cq_enqueue_pending (struct fi_opx_cq * opx_cq,
		union fi_opx_context * context,
		const int lock_required)
{

	if (lock_required) { FI_WARN(fi_opx_global.prov, FI_LOG_CQ, "unimplemented\n"); abort(); }

	union fi_opx_context * tail = opx_cq->pending.tail;
	context->next = NULL;
	if (tail) {
		tail->next = context;
	} else {
		opx_cq->pending.head = context;
	}
	opx_cq->pending.tail = context;

	return 0;
}


static inline
int fi_opx_cq_enqueue_completed (struct fi_opx_cq * opx_cq,
		union fi_opx_context * context,
		const int lock_required)
{
	assert(0 == context->byte_counter);


	if (lock_required) { FI_WARN(fi_opx_global.prov, FI_LOG_CQ, "unimplemented\n"); abort(); }

	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA, "=================== MANUAL PROGRESS COMPLETION CQ ENQUEUED\n");

	union fi_opx_context * tail = opx_cq->completed.tail;
	context->next = NULL;
	if (tail) {

		assert(NULL != opx_cq->completed.head);
		tail->next = context;
		opx_cq->completed.tail = context;

	} else {
		assert(NULL == opx_cq->completed.head);
		opx_cq->completed.head = context;
		opx_cq->completed.tail = context;
	}

	return 0;
}



static inline size_t fi_opx_cq_fill(uintptr_t output,
		union fi_opx_context * context,
		const enum fi_cq_format format)
{
	assert(!(context->flags & FI_OPX_CQ_CONTEXT_HMEM));
	assert(!(context->flags & FI_OPX_CQ_CONTEXT_EXT));
	const uint64_t is_multi_recv = context->flags & FI_OPX_CQ_CONTEXT_MULTIRECV;

	size_t return_size;

	struct fi_cq_tagged_entry * entry = (struct fi_cq_tagged_entry *) output;
	switch (format) {
		case FI_CQ_FORMAT_CONTEXT:
			return_size = sizeof(struct fi_cq_entry);
			break;
		case FI_CQ_FORMAT_MSG:
			*((struct fi_cq_msg_entry *)output) = *((struct fi_cq_msg_entry *)context);
			return_size = sizeof(struct fi_cq_msg_entry);
			break;
		case FI_CQ_FORMAT_DATA:
			*((struct fi_cq_data_entry *)output) = *((struct fi_cq_data_entry *)context);
			return_size = sizeof(struct fi_cq_data_entry);
			break;
		case FI_CQ_FORMAT_TAGGED:
			*((struct fi_cq_tagged_entry *)output) = *((struct fi_cq_tagged_entry *)context);
			return_size = sizeof(struct fi_cq_tagged_entry);
			break;
		default:
			assert(0);
			return_size = 0;
	}

	if (OFI_LIKELY(!is_multi_recv)) {
		entry->op_context = (void *)context;
	} else {
		entry->op_context = (void *)context->multi_recv_context;
	}

	return return_size;
}

static ssize_t fi_opx_cq_poll_noinline (struct fi_opx_cq *opx_cq,
		void *buf,
		size_t count,
		const enum fi_cq_format format)
{
	/* check if the err list has anything in it and return */
	if (opx_cq->err.head != NULL) {
		errno = FI_EAVAIL;
		return -errno;
	}

	ssize_t num_entries = 0;
	uintptr_t output = (uintptr_t)buf;

	/* examine each context in the pending completion queue and, if the
	 * operation is complete, initialize the cq entry in the application
	 * buffer and remove the context from the queue. */
	union fi_opx_context * pending_head = opx_cq->pending.head;
	union fi_opx_context * pending_tail = opx_cq->pending.tail;

	if (NULL != pending_head) {
		union fi_opx_context * context = pending_head;
		union fi_opx_context * prev = NULL;
		while ((count - num_entries) > 0 && context != NULL) {

			const uint64_t byte_counter = context->byte_counter;

			if (byte_counter == 0) {
				if (context->flags & FI_OPX_CQ_CONTEXT_MULTIRECV) {
					assert(!(context->flags & FI_OPX_CQ_CONTEXT_HMEM));
					assert(!(context->flags & FI_OPX_CQ_CONTEXT_EXT));

					union fi_opx_context *multi_recv_context = context->multi_recv_context;
					assert(multi_recv_context != NULL);
					multi_recv_context->byte_counter-=1;
					assert(((int64_t)multi_recv_context->byte_counter) >= 0);
					// Reusing byte counter as pending flag
					// re-using tag to store the min multi_receive
					struct fi_opx_ep * opx_ep = (struct fi_opx_ep *)multi_recv_context->tag;
					if(multi_recv_context->len < opx_ep->rx->min_multi_recv &&
					   multi_recv_context->byte_counter == 0) {
						/* Signal the user to repost their buffers */
						assert(multi_recv_context->next == NULL);
						fi_opx_context_slist_insert_tail(multi_recv_context, opx_ep->rx->cq_completed_ptr);
					}
				} else if (context->flags & FI_OPX_CQ_CONTEXT_EXT) {
					struct fi_opx_context_ext *ext = (struct fi_opx_context_ext *) context;
					context = (union fi_opx_context *) ext->msg.op_context;
					*context = ext->opx_context;
					context->flags &= ~(FI_OPX_CQ_CONTEXT_EXT | FI_OPX_CQ_CONTEXT_HMEM);
					OPX_BUF_FREE(ext);
				}
				output += fi_opx_cq_fill(output, context, format);
				++ num_entries;

				if (prev)
					prev->next = context->next;
				else
					/* remove the head */
					pending_head = context->next;

				if (!(context->next))
					/* remove the tail */
					pending_tail = prev;
			}
			else
				prev = context;
			context = context->next;
		}

		/* save the updated pending head and pending tail pointers */
		opx_cq->pending.head = pending_head;
		opx_cq->pending.tail = pending_tail;
	}


	union fi_opx_context * head = opx_cq->completed.head;
	if (head) {
		union fi_opx_context * context = head;
		while ((count - num_entries) > 0 && context != NULL) {
			output += fi_opx_cq_fill(output, context, format);
			++ num_entries;
			context = context->next;
		}
		opx_cq->completed.head = context;
		if (!context) opx_cq->completed.tail = NULL;

	}

	return num_entries;
}

__OPX_FORCE_INLINE__
__attribute__ ((flatten))
ssize_t fi_opx_cq_poll_inline(struct fid_cq *cq, void *buf, size_t count,
		fi_addr_t *src_addr, const enum fi_cq_format format,
		const int lock_required,
		const enum ofi_reliability_kind reliability,
		const uint64_t hdrq_mask,
		const uint64_t caps)
{
	ssize_t num_entries = 0;

	struct fi_opx_cq *opx_cq = (struct fi_opx_cq *)cq;

	// If locking is required, we should already hold the cq lock coming
	// into this function, but we need to know whether or not to lock each
	// endpoint as we poll them below. Verify that if lock_required is on,
	// we hold the cq lock, or if lock_required is off, we are not holding
	// the cq lock.
	assert((lock_required && ofi_spin_held(&opx_cq->lock)) ||
		(!lock_required && !ofi_spin_held(&opx_cq->lock)));

	if (OFI_UNLIKELY(buf == NULL && count > 0)) {
		errno = FI_EINVAL;
		FI_WARN(fi_opx_global.prov, FI_LOG_CQ, "Invalid buffer and count combination\n");
		return -errno;
	}

	const uint64_t ep_count = opx_cq->progress.ep_count;
	uint64_t i;

	if (OFI_UNLIKELY(lock_required)) {
		if (hdrq_mask == FI_OPX_HDRQ_MASK_2048) {  /* constant compile-time expression */
			for (i=0; i<ep_count; ++i) {
				fi_opx_lock(&opx_cq->progress.ep[i]->lock);
				fi_opx_ep_rx_poll(&opx_cq->progress.ep[i]->ep_fid, caps, reliability, FI_OPX_HDRQ_MASK_2048);
				fi_opx_unlock(&opx_cq->progress.ep[i]->lock);
			}
		} else if (hdrq_mask == FI_OPX_HDRQ_MASK_8192) {
			for (i=0; i<ep_count; ++i) {
				fi_opx_lock(&opx_cq->progress.ep[i]->lock);
				fi_opx_ep_rx_poll(&opx_cq->progress.ep[i]->ep_fid, caps, reliability, FI_OPX_HDRQ_MASK_8192);
				fi_opx_unlock(&opx_cq->progress.ep[i]->lock);
			}				
			
		} else {
			for (i=0; i<ep_count; ++i) {
				fi_opx_lock(&opx_cq->progress.ep[i]->lock);
				fi_opx_ep_rx_poll(&opx_cq->progress.ep[i]->ep_fid, caps, reliability, FI_OPX_HDRQ_MASK_RUNTIME);
				fi_opx_unlock(&opx_cq->progress.ep[i]->lock);
			}
		}
	} else {
		if (hdrq_mask == FI_OPX_HDRQ_MASK_2048) {  /* constant compile-time expression */
			for (i=0; i<ep_count; ++i) {
				fi_opx_ep_rx_poll(&opx_cq->progress.ep[i]->ep_fid, caps, reliability, FI_OPX_HDRQ_MASK_2048);
			}
		} else if (hdrq_mask == FI_OPX_HDRQ_MASK_8192) {
			for (i=0; i<ep_count; ++i) {
				fi_opx_ep_rx_poll(&opx_cq->progress.ep[i]->ep_fid, caps, reliability, FI_OPX_HDRQ_MASK_8192);
			}
		} else {
			for (i=0; i<ep_count; ++i) {
				fi_opx_ep_rx_poll(&opx_cq->progress.ep[i]->ep_fid, caps, reliability, FI_OPX_HDRQ_MASK_RUNTIME);
			}
		}
	}

	//This is meant for auto progress to just access the rx_polls and exit
	if (count == 0 && buf == NULL) {
		return 0;
	}

	const uintptr_t tmp_eh = (const uintptr_t)opx_cq->err.head;
	const uintptr_t tmp_ph = (const uintptr_t)opx_cq->pending.head;
	const uintptr_t tmp_ch = (const uintptr_t)opx_cq->completed.head;

		/* check for "all empty" and return */
	if (0 == (tmp_eh | tmp_ph | tmp_ch)) {
		errno = FI_EAGAIN;
		return -errno;
	}

	/* check for "fast path" and return (something has completed, but nothing is pending and there are no errors) */
	if (0 == (tmp_eh | tmp_ph)) {

		uintptr_t output = (uintptr_t) buf;
		union fi_opx_context * context = (union fi_opx_context *)tmp_ch;
		while ((count - num_entries) > 0 && context != NULL) {
			output += fi_opx_cq_fill(output, context, format);
			++ num_entries;
			context = context->next;
		}
		opx_cq->completed.head = context;
		if (!context) opx_cq->completed.tail = NULL;

		return num_entries;
	}

	num_entries = fi_opx_cq_poll_noinline(opx_cq, buf, count, format);


	if (num_entries == 0) {
		errno = FI_EAGAIN;
		return -errno;
	}

	return num_entries;
}


__OPX_FORCE_INLINE__
ssize_t fi_opx_cq_read_generic_non_locking (struct fid_cq *cq, void *buf, size_t count,
		const enum fi_cq_format format,
		const enum ofi_reliability_kind reliability,
		const uint64_t hdrq_mask,
		const uint64_t caps)
{
	return fi_opx_cq_poll_inline(cq, buf, count, NULL, format, FI_OPX_LOCK_NOT_REQUIRED, reliability, hdrq_mask, caps);	
}

__OPX_FORCE_INLINE__
ssize_t fi_opx_cq_read_generic_locking (struct fid_cq *cq, void *buf, size_t count,
		const enum fi_cq_format format,
		const enum ofi_reliability_kind reliability,
		const uint64_t hdrq_mask,
		const uint64_t caps)
{
	int ret;
	fi_opx_lock(&((struct fi_opx_cq *) cq)->lock);
	ret = fi_opx_cq_poll_inline(cq, buf, count, NULL, format, FI_OPX_LOCK_REQUIRED, reliability, hdrq_mask, caps);
	fi_opx_unlock(&((struct fi_opx_cq *) cq)->lock);

	return ret;
}

__OPX_FORCE_INLINE__
ssize_t fi_opx_cq_readfrom_generic_non_locking (struct fid_cq *cq, void *buf, size_t count, fi_addr_t *src_addr,
		const enum fi_cq_format format,
		const enum ofi_reliability_kind reliability,
		const uint64_t hdrq_mask,
		const uint64_t caps)
{
	int ret;
	ret = fi_opx_cq_poll_inline(cq, buf, count, src_addr, format, FI_OPX_LOCK_NOT_REQUIRED, reliability, hdrq_mask, caps);
	if (ret > 0) {
		unsigned n;
		for (n=0; n<ret; ++n) src_addr[n] = FI_ADDR_NOTAVAIL;
	}

	return ret;
}

__OPX_FORCE_INLINE__
ssize_t fi_opx_cq_readfrom_generic_locking (struct fid_cq *cq, void *buf, size_t count, fi_addr_t *src_addr,
		const enum fi_cq_format format,
		const enum ofi_reliability_kind reliability,
		const uint64_t hdrq_mask,
		const uint64_t caps)
{
	int ret;
	fi_opx_lock(&((struct fi_opx_cq *) cq)->lock);
	ret = fi_opx_cq_poll_inline(cq, buf, count, src_addr, format, FI_OPX_LOCK_REQUIRED, reliability, hdrq_mask, caps);
	fi_opx_unlock(&((struct fi_opx_cq *) cq)->lock);
	if (ret > 0) {
		unsigned n;
		for (n=0; n<ret; ++n) src_addr[n] = FI_ADDR_NOTAVAIL;
	}

	return ret;
}

#ifdef __cplusplus
}
#endif

#endif /* _FI_PROV_OPX_EQ_H_ */
