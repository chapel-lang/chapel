/*
 * Copyright (C) 2016 by Argonne National Laboratory.
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
#ifndef _FI_BGQ_DIRECT_EQ_H_
#define _FI_BGQ_DIRECT_EQ_H_

#define FABRIC_DIRECT_EQ 1

#include <unistd.h>
#include <stdint.h>

#include "rdma/bgq/fi_bgq_hwi.h"

#include "rdma/bgq/fi_bgq_l2atomic.h"
#include "rdma/bgq/fi_bgq_mu.h"

#ifdef __cplusplus
extern "C" {
#endif

struct fi_bgq_cntr {
	struct fid_cntr		cntr_fid;

	struct {
		volatile uint64_t	*l2_vaddr;
		uint64_t		paddr;
		uint64_t		batid;
	} std;
	struct {
		volatile uint64_t	*l2_vaddr;
		uint64_t		paddr;
		uint64_t		batid;
	} err;

	volatile uint64_t	data[2];

	struct {
		uint64_t		ep_count;
		struct fi_bgq_ep	*ep[64];	/* TODO - check this array size */
	} progress;

	uint64_t		ep_bind_count;
	struct fi_bgq_ep	*ep[64];	/* TODO - check this array size */

	struct fi_cntr_attr	*attr;
	struct fi_bgq_domain	*domain;
};

#define FI_BGQ_CQ_CONTEXT_EXT		(0x8000000000000000ull)
#define FI_BGQ_CQ_CONTEXT_MULTIRECV	(0x4000000000000000ull)


union fi_bgq_context {
	struct fi_context		context;
	struct {
		union fi_bgq_context	*next;		// fi_cq_entry::op_context
		uint64_t		flags;		// fi_cq_msg_entry::flags
		size_t			len;		// fi_cq_msg_entry::len (only need 37 bits)
		void			*buf;		// fi_cq_data_entry::buf (unused for tagged cq's and non-multi-receive message cq's)

		union {
			uint64_t	data;		// fi_cq_data_entry::data; only used after a message is matched
			fi_addr_t	src_addr;	/* only used before a message is matched ('FI_DIRECTED_RECEIVE') */
		};

		union {
			uint64_t	tag;		// fi_cq_tagged_entry::tag
			union fi_bgq_context	*multi_recv_next;	// only for multi-receives
		};
		union {
			uint64_t		ignore;	// only for tagged receive
			struct fi_bgq_mu_packet	*claim;	// only for peek/claim
			void			*multi_recv_context;	// only for individual FI_MULTI_RECV's
		};

		volatile uint64_t	byte_counter;
	};
};

struct fi_bgq_context_ext {
	union fi_bgq_context		bgq_context;
	struct fi_cq_err_entry		err_entry;
	struct {
		struct fi_context	*op_context;
		size_t			iov_count;
		struct iovec		*iov;
	} msg;
};

/* This structure is organized in a way that minimizes cacheline use for the
 * "FI_PROGRESS_MANUAL + inject" poll scenario.
 */
struct fi_bgq_cq {
	struct fid_cq			cq_fid;		/* must be the first field in the structure */
	uint64_t			pad_0[5];

	/* == L2 CACHE LINE == */

	struct fi_bgq_context_ext	*err_head;

	union fi_bgq_context		*pending_head;
	union fi_bgq_context		*pending_tail;
	union fi_bgq_context		*completed_head;
	union fi_bgq_context		*completed_tail;

	struct {
		uint64_t		ep_count;
		struct fi_bgq_ep	*ep[64];	/* TODO - check this array size */
	} progress;

	struct fi_bgq_context_ext	*err_tail;
	uint64_t			pad_1[9];

	/* == L2 CACHE LINE == */

	struct l2atomic_lock		lock;

	struct l2atomic_fifo_consumer	err_consumer;
	struct l2atomic_fifo_consumer	std_consumer;
	struct l2atomic_fifo_producer	err_producer;
	struct l2atomic_fifo_producer	std_producer;


	struct fi_bgq_domain	*domain;
	uint64_t		bflags;		/* fi_bgq_bind_ep_cq() */
	size_t			size;
	enum fi_cq_format	format;

	MUHWI_Descriptor_t	local_completion_model;

	uint64_t		ep_bind_count;
	struct fi_bgq_ep	*ep[64];		/* TODO - check this array size */

	struct fi_cq_bgq_l2atomic_data	*fifo_memptr;
	struct l2atomic_counter	ref_cnt;
};

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

int fi_bgq_cq_enqueue_err (struct fi_bgq_cq * bgq_cq,
		struct fi_bgq_context_ext * ext,
		const int lock_required);

static inline
int fi_bgq_cq_enqueue_pending (struct fi_bgq_cq * bgq_cq,
		union fi_bgq_context * context,
		const int lock_required)
{
	if (FI_BGQ_FABRIC_DIRECT_PROGRESS == FI_PROGRESS_MANUAL) {

		int ret;
		ret = fi_bgq_lock_if_required(&bgq_cq->lock, lock_required);
		if (ret) return ret;

		union fi_bgq_context * tail = bgq_cq->pending_tail;
		context->next = NULL;
		if (tail) {
			tail->next = context;
		} else {
			bgq_cq->pending_head = context;
		}
		bgq_cq->pending_tail = context;

		ret = fi_bgq_unlock_if_required(&bgq_cq->lock, lock_required);
		if (ret) return ret;

	} else {

		struct l2atomic_fifo_producer * std_producer = &bgq_cq->std_producer;
		uint64_t context_rsh3b = (uint64_t)context >> 3;
		while(0 != l2atomic_fifo_produce(std_producer, context_rsh3b));	/* spin loop! */
	}

	return 0;
}


static inline
int fi_bgq_cq_enqueue_completed (struct fi_bgq_cq * bgq_cq,
		union fi_bgq_context * context,
		const int lock_required)
{
	assert(0 == context->byte_counter);

	if (FI_BGQ_FABRIC_DIRECT_PROGRESS == FI_PROGRESS_MANUAL) {

		int ret;
		ret = fi_bgq_lock_if_required(&bgq_cq->lock, lock_required);
		if (ret) return ret;

		union fi_bgq_context * tail = bgq_cq->completed_tail;
		context->next = NULL;
		if (tail) {

			assert(NULL != bgq_cq->completed_head);
			tail->next = context;
			bgq_cq->completed_tail = context;

		} else {

			assert(NULL == bgq_cq->completed_head);
			bgq_cq->completed_head = context;
			bgq_cq->completed_tail = context;
		}

		ret = fi_bgq_unlock_if_required(&bgq_cq->lock, lock_required);
		if (ret) return ret;

	} else {

		struct l2atomic_fifo_producer * std_producer = &bgq_cq->std_producer;
		uint64_t context_rsh3b = (uint64_t)context >> 3;
		while(0 != l2atomic_fifo_produce(std_producer, context_rsh3b));	/* spin loop! */
	}

	return 0;
}



static size_t fi_bgq_cq_fill(uintptr_t output,
		union fi_bgq_context * context,
		const enum fi_cq_format format)
{
	assert((context->flags & FI_BGQ_CQ_CONTEXT_EXT)==0);
#ifndef FABRIC_DIRECT
	fprintf(stderr,"BGQ provider must be run in fabric-direct mode only\n");
	assert(0);
#endif
	assert(sizeof(struct fi_context) == sizeof(union fi_bgq_context));

	struct fi_cq_tagged_entry * entry = (struct fi_cq_tagged_entry *) output;
	switch (format) {
	case FI_CQ_FORMAT_CONTEXT:
		if ((context->flags & FI_BGQ_CQ_CONTEXT_MULTIRECV) == 0) {	/* likely */
			entry->op_context = (void *)context;
		} else {
			entry->op_context = (void *)context->multi_recv_context;
		}
		return sizeof(struct fi_cq_entry);
		break;
	case FI_CQ_FORMAT_MSG:
		*((struct fi_cq_msg_entry *)output) = *((struct fi_cq_msg_entry *)context);
		if ((context->flags & FI_BGQ_CQ_CONTEXT_MULTIRECV) == 0) {	/* likely */
			entry->op_context = (void *)context;
		} else {
			entry->op_context = (void *)context->multi_recv_context;
		}
		return sizeof(struct fi_cq_msg_entry);
		break;
	case FI_CQ_FORMAT_DATA:
		*((struct fi_cq_data_entry *)output) = *((struct fi_cq_data_entry *)context);
		if ((context->flags & FI_BGQ_CQ_CONTEXT_MULTIRECV) == 0) {	/* likely */
			entry->op_context = (void *)context;
		} else {
			entry->op_context = (void *)context->multi_recv_context;
		}
		return sizeof(struct fi_cq_data_entry);
		break;
	case FI_CQ_FORMAT_TAGGED:
		*((struct fi_cq_tagged_entry *)output) = *((struct fi_cq_tagged_entry *)context);
		if ((context->flags & FI_BGQ_CQ_CONTEXT_MULTIRECV) == 0) {	/* likely */
			entry->op_context = (void *)context;
		} else {
			entry->op_context = (void *)context->multi_recv_context;
		}
		return sizeof(struct fi_cq_tagged_entry);
		break;
	default:
		assert(0);
	}

	return 0;
}

int fi_bgq_ep_progress_manual (struct fi_bgq_ep *bgq_ep);

static ssize_t fi_bgq_cq_poll (struct fi_bgq_cq *bgq_cq,
		void *buf,
		size_t count,
		const enum fi_cq_format format)
{
	if (FI_BGQ_FABRIC_DIRECT_PROGRESS == FI_PROGRESS_MANUAL) {	/* branch will compile out */	/* TODO - FI_PROGRESS_AUTO + 64 ppn */

		/* check if the err list has anything in it and return */
		if (NULL != bgq_cq->err_head) {		/* unlikely */
			assert(NULL != bgq_cq->err_tail);

			errno = FI_EAVAIL;
			return -errno;
		}

	} else if (FI_BGQ_FABRIC_DIRECT_PROGRESS == FI_PROGRESS_AUTO) {	/* branch will compile out */

		/* check if the err fifo has anything in it and return */
		if (!l2atomic_fifo_isempty(&bgq_cq->err_consumer)) {

			errno = FI_EAVAIL;
			return -errno;
		}

	} else assert(0);	/* huh? */

	ssize_t num_entries = 0;
	uintptr_t output = (uintptr_t)buf;

	/* examine each context in the pending completion queue and, if the
	 * operation is complete, initialize the cq entry in the application
	 * buffer and remove the context from the queue. */
	union fi_bgq_context * pending_head = bgq_cq->pending_head;
	union fi_bgq_context * pending_tail = bgq_cq->pending_tail;
	if (NULL != pending_head) {
		union fi_bgq_context * context = pending_head;
		union fi_bgq_context * prev = NULL;
		while ((count - num_entries) > 0 && context != NULL) {

			const uint64_t byte_counter = context->byte_counter;

			if (byte_counter == 0) {
				output += fi_bgq_cq_fill(output, context, format);
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
		bgq_cq->pending_head = pending_head;
		bgq_cq->pending_tail = pending_tail;
	}

	if (FI_BGQ_FABRIC_DIRECT_PROGRESS == FI_PROGRESS_MANUAL) {	/* branch will compile out */

		union fi_bgq_context * head = bgq_cq->completed_head;
		if (head) {
			union fi_bgq_context * context = head;
			while ((count - num_entries) > 0 && context != NULL) {
				output += fi_bgq_cq_fill(output, context, format);
				++ num_entries;
				context = context->next;
			}
			bgq_cq->completed_head = context;
			if (!context) bgq_cq->completed_tail = NULL;

		}

	} else if (FI_BGQ_FABRIC_DIRECT_PROGRESS == FI_PROGRESS_AUTO) {	/* branch will compile out */

		/* drain the std fifo and initialize the cq entries in the application
		 * buffer if the operation is complete; otherwise append to the
		 * pending completion queue */
		uint64_t value = 0;
		struct l2atomic_fifo_consumer * consumer = &bgq_cq->std_consumer;
		while ((count - num_entries) > 0 &&
				l2atomic_fifo_consume(consumer, &value) == 0) {

			/* const uint64_t flags = value & 0xE000000000000000ull; -- currently not used */

			/* convert the fifo value into a context pointer */
			union fi_bgq_context *context = (union fi_bgq_context *) (value << 3);

			if (context->byte_counter == 0) {
				output += fi_bgq_cq_fill(output, context, format);
				++ num_entries;
			} else {
				context->next = NULL;
				if (pending_tail)
					pending_tail->next = context;
				else
					pending_head = context;
				pending_tail = context;
			}
		}

		/* save the updated pending head and pending tail pointers */
		bgq_cq->pending_head = pending_head;
		bgq_cq->pending_tail = pending_tail;
	}

	return num_entries;
}

static ssize_t fi_bgq_cq_poll_inline(struct fid_cq *cq, void *buf, size_t count,
		fi_addr_t *src_addr, const enum fi_cq_format format,
		const int lock_required)
{
	ssize_t num_entries = 0;

	struct fi_bgq_cq *bgq_cq = (struct fi_bgq_cq *)cq;

	int ret;
	ret = fi_bgq_lock_if_required(&bgq_cq->lock, lock_required);
	if (ret) return ret;


	if (FI_BGQ_FABRIC_DIRECT_PROGRESS == FI_PROGRESS_MANUAL) {	/* branch will compile out */	/* TODO - FI_PROGRESS_AUTO + 64 ppn */

		const uint64_t count = bgq_cq->progress.ep_count;
		uint64_t i;
		for (i=0; i<count; ++i) {
			fi_bgq_ep_progress_manual(bgq_cq->progress.ep[i]);
		}

		const uintptr_t tmp_eh = (const uintptr_t)bgq_cq->err_head;
		const uintptr_t tmp_ph = (const uintptr_t)bgq_cq->pending_head;
		const uintptr_t tmp_ch = (const uintptr_t)bgq_cq->completed_head;

		/* check for "all empty" and return */
		if (0 == (tmp_eh | tmp_ph | tmp_ch)) {

			ret = fi_bgq_unlock_if_required(&bgq_cq->lock, lock_required);
			if (ret) return ret;

			errno = FI_EAGAIN;
			return -errno;
		}

		/* check for "fast path" and return */
		if (tmp_ch == (tmp_eh | tmp_ph | tmp_ch)) {

			uintptr_t output = (uintptr_t)buf;

			union fi_bgq_context * context = (union fi_bgq_context *)tmp_ch;
			while ((count - num_entries) > 0 && context != NULL) {
				output += fi_bgq_cq_fill(output, context, format);
				++ num_entries;
				context = context->next;
			}
			bgq_cq->completed_head = context;
			if (!context) bgq_cq->completed_tail = NULL;

			ret = fi_bgq_unlock_if_required(&bgq_cq->lock, lock_required);
			if (ret) return ret;

			return num_entries;
		}
	}

	num_entries = fi_bgq_cq_poll(bgq_cq, buf, count, format);

	ret = fi_bgq_unlock_if_required(&bgq_cq->lock, lock_required);
	if (ret) return ret;

	if (num_entries == 0) {
		errno = FI_EAGAIN;
		return -errno;
	}

	return num_entries;
}


static inline
ssize_t fi_bgq_cq_read_generic (struct fid_cq *cq, void *buf, size_t count,
		const enum fi_cq_format format, const int lock_required)
{
	int ret;
	ret = fi_bgq_cq_poll_inline(cq, buf, count, NULL, format, lock_required);
	return ret;
}

static inline
ssize_t fi_bgq_cq_readfrom_generic (struct fid_cq *cq, void *buf, size_t count, fi_addr_t *src_addr,
		const enum fi_cq_format format, const int lock_required)
{
	int ret;
	ret = fi_bgq_cq_poll_inline(cq, buf, count, src_addr, format, lock_required);
	if (ret > 0) {
		unsigned n;
		for (n=0; n<ret; ++n) src_addr[n] = FI_ADDR_NOTAVAIL;
	}

	return ret;
}

/*
 * Declare specialized functions that qualify for FABRIC_DIRECT.
 * - No locks
 * - FI_CQ_FORMAT_TAGGED
 */
#define FI_BGQ_CQ_FABRIC_DIRECT_LOCK		0
#define FI_BGQ_CQ_FABRIC_DIRECT_FORMAT		FI_CQ_FORMAT_TAGGED

FI_BGQ_CQ_SPECIALIZED_FUNC(FI_BGQ_CQ_FABRIC_DIRECT_FORMAT,
		FI_BGQ_CQ_FABRIC_DIRECT_LOCK)

#ifdef FABRIC_DIRECT
#define fi_cq_read(cq, buf, count)					\
	(FI_BGQ_CQ_SPECIALIZED_FUNC_NAME(cq_read,			\
			FI_BGQ_CQ_FABRIC_DIRECT_FORMAT,			\
			FI_BGQ_CQ_FABRIC_DIRECT_LOCK)			\
	(cq, buf, count))

#define fi_cq_readfrom(cq, buf, count, src_addr)			\
	(FI_BGQ_CQ_SPECIALIZED_FUNC_NAME(cq_readfrom,			\
			FI_BGQ_CQ_FABRIC_DIRECT_FORMAT,			\
			FI_BGQ_CQ_FABRIC_DIRECT_LOCK)			\
	(cq, buf, count, src_addr))


static inline
ssize_t fi_cq_readerr(struct fid_cq *cq, struct fi_cq_err_entry *buf,
		uint64_t flags)
{
	return cq->ops->readerr(cq, buf, flags);
}

static inline
uint64_t fi_cntr_read(struct fid_cntr *cntr)
{
	return cntr->ops->read(cntr);
}

static inline
int fi_cntr_wait(struct fid_cntr *cntr, uint64_t threshold, int timeout)
{
	return cntr->ops->wait(cntr, threshold, timeout);
}

static inline
int fi_trywait(struct fid_fabric *fabric, struct fid **fids, int count)
{
	return -FI_ENOSYS;	/* TODO - implement this */
}

static inline
int fi_wait(struct fid_wait *waitset, int timeout)
{
	return -FI_ENOSYS;	/* TODO - implement this */
}

static inline
int fi_poll(struct fid_poll *pollset, void **context, int count)
{
	return -FI_ENOSYS;	/* TODO - implement this */
}

static inline
int fi_poll_add(struct fid_poll *pollset, struct fid *event_fid,
			      uint64_t flags)
{
	return -FI_ENOSYS;	/* TODO - implement this */
}

static inline
int fi_poll_del(struct fid_poll *pollset, struct fid *event_fid,
			      uint64_t flags)
{
	return -FI_ENOSYS;	/* TODO - implement this */
}

static inline
int fi_eq_open(struct fid_fabric *fabric, struct fi_eq_attr *attr,
			     struct fid_eq **eq, void *context)
{
	return -FI_ENOSYS;	/* TODO - implement this */
}

static inline
ssize_t fi_eq_read(struct fid_eq *eq, uint32_t *event, void *buf,
				 size_t len, uint64_t flags)
{
	return -FI_ENOSYS;	/* TODO - implement this */
}

static inline
ssize_t fi_eq_readerr(struct fid_eq *eq,
				    struct fi_eq_err_entry *buf, uint64_t flags)
{
	return -FI_ENOSYS;	/* TODO - implement this */
}

static inline
ssize_t fi_eq_write(struct fid_eq *eq, uint32_t event,
				  const void *buf, size_t len, uint64_t flags)
{
	return -FI_ENOSYS;	/* TODO - implement this */
}

static inline
ssize_t fi_eq_sread(struct fid_eq *eq, uint32_t *event, void *buf,
				  size_t len, int timeout, uint64_t flags)
{
	return -FI_ENOSYS;	/* TODO - implement this */
}

static inline
const char *fi_eq_strerror(struct fid_eq *eq, int prov_errno,
					 const void *err_data, char *buf,
					 size_t len)
{
	return NULL;		/* TODO - implement this */
}

static inline
ssize_t fi_cq_sread(struct fid_cq *cq, void *buf, size_t count,
				  const void *cond, int timeout)
{
	return -FI_ENOSYS;	/* TODO - implement this */
}

static inline
ssize_t fi_cq_sreadfrom(struct fid_cq *cq, void *buf,
				      size_t count, fi_addr_t *src_addr,
				      const void *cond, int timeout)
{
	return -FI_ENOSYS;	/* TODO - implement this */
}

static inline
int fi_cq_signal(struct fid_cq *cq)
{
	return -FI_ENOSYS;	/* TODO - implement this */
}

static inline
const char *fi_cq_strerror(struct fid_cq *cq, int prov_errno,
					 const void *err_data, char *buf,
					 size_t len)
{
	return NULL;		/* TODO - implement this */
}

static inline
uint64_t fi_cntr_readerr(struct fid_cntr *cntr)
{
	return 0;		/* TODO - implement this */
}

static inline
int fi_cntr_add(struct fid_cntr *cntr, uint64_t value)
{
	return -FI_ENOSYS;	/* TODO - implement this */
}

static inline
int fi_cntr_set(struct fid_cntr *cntr, uint64_t value)
{
	return -FI_ENOSYS;	/* TODO - implement this */
}

#endif

#ifdef __cplusplus
}
#endif

#endif /* _FI_BGQ_DIRECT_EQ_H_ */
