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
#include "rdma/bgq/fi_bgq.h"
#include <ofi_enosys.h>
#include <stdlib.h>

#include <ofi.h>

#include "rdma/bgq/fi_bgq_spi.h"

#define FI_BGQ_DEFAULT_CQ_DEPTH (8192)
#define FI_BGQ_MAXIMUM_CQ_DEPTH (8192)

#define FI_BGQ_L2ATOMIC_ERR_FIFO_DATA_SIZE (512)

struct fi_cq_bgq_l2atomic_data {
	struct l2atomic_boundedcounter_data	entry_counter;
	struct l2atomic_boundedcounter_data	bounded_counter;
	struct l2atomic_fifo_data		err_fifo_data;
	uint64_t				err_packet[FI_BGQ_L2ATOMIC_ERR_FIFO_DATA_SIZE];
	struct l2atomic_fifo_data		std_fifo_data;
	uint64_t				std_packet[0];
} __attribute((aligned(32)));

static int fi_bgq_close_cq(fid_t fid)
{
	int ret;
	struct fi_bgq_cq *bgq_cq =
		container_of(fid, struct fi_bgq_cq, cq_fid);

	ret = fi_bgq_fid_check(fid, FI_CLASS_CQ, "completion queue");
	if (ret)
		return ret;

	ret = fi_bgq_ref_dec(&bgq_cq->domain->ref_cnt, "domain");
	if (ret)
		return ret;

	ret = fi_bgq_ref_finalize(&bgq_cq->ref_cnt, "completion queue");
	if (ret)
		return ret;

	free(bgq_cq);

	return 0;
}

static int fi_bgq_bind_cq(struct fid *fid, struct fid *bfid,
		uint64_t flags)
{
	errno = FI_ENOSYS;
	return -errno;
}

static int fi_bgq_control_cq(fid_t fid, int command, void *arg)
{
	errno = FI_ENOSYS;
	return -errno;
}

static int fi_bgq_ops_open_cq(struct fid *fid, const char *name,
		uint64_t flags, void **ops, void *context)
{
	errno = FI_ENOSYS;
	return -errno;
}

static struct fi_ops fi_bgq_fi_ops = {
	.size		= sizeof(struct fi_ops),
	.close		= fi_bgq_close_cq,
	.bind		= fi_bgq_bind_cq,
	.control	= fi_bgq_control_cq,
	.ops_open	= fi_bgq_ops_open_cq
};

static ssize_t fi_bgq_cq_read(struct fid_cq *cq, void *buf, size_t count)
{
	int lock_required;
	int ret;
	struct fi_bgq_cq *bgq_cq = container_of(cq, struct fi_bgq_cq, cq_fid);

	switch (bgq_cq->domain->threading) {
	case FI_THREAD_ENDPOINT:
	case FI_THREAD_DOMAIN:
		lock_required = 0;
	default:
		lock_required = 1;
	}

	ret = fi_bgq_cq_read_generic(cq, buf, count, bgq_cq->format, lock_required);
	return ret;
}

static ssize_t
fi_bgq_cq_readfrom(struct fid_cq *cq, void *buf, size_t count, fi_addr_t *src_addr)
{
	int lock_required;
	int ret;
	struct fi_bgq_cq *bgq_cq = container_of(cq, struct fi_bgq_cq, cq_fid);

	switch (bgq_cq->domain->threading) {
	case FI_THREAD_ENDPOINT:
	case FI_THREAD_DOMAIN:
		lock_required = 0;
		break;
	default:
		lock_required = 1;
		break;
	}

	ret = fi_bgq_cq_readfrom_generic(cq, buf, count, src_addr, bgq_cq->format, lock_required);
	if (ret > 0) {
		unsigned n;
		for (n=0; n<ret; ++n) src_addr[n] = FI_ADDR_NOTAVAIL;
	}

	return ret;
}

static ssize_t
fi_bgq_cq_readerr(struct fid_cq *cq, struct fi_cq_err_entry *buf, uint64_t flags)
{
	struct fi_bgq_cq *bgq_cq = container_of(cq, struct fi_bgq_cq, cq_fid);

	if (FI_BGQ_FABRIC_DIRECT_PROGRESS == FI_PROGRESS_MANUAL) {

		struct fi_bgq_context_ext * ext = bgq_cq->err_head;
		if (NULL == ext) {
			errno = FI_EAGAIN;
			return -errno;
		}

		if (ext->bgq_context.byte_counter != 0) {
			/* perhaps an in-progress truncated rendezvous receive? */
			errno = FI_EAGAIN;
			return -errno;
		}

		assert(ext->bgq_context.flags & FI_BGQ_CQ_CONTEXT_EXT);	/* DEBUG */

		int lock_required = 0;
		switch (bgq_cq->domain->threading) {
		case FI_THREAD_ENDPOINT:
		case FI_THREAD_DOMAIN:
			lock_required = 0;
			break;
		default:
			lock_required = 1;
			break;
		}

		int ret;
		ret = fi_bgq_lock_if_required(&bgq_cq->lock, lock_required);
		if (ret) return ret;

		bgq_cq->err_head = (struct fi_bgq_context_ext *)ext->bgq_context.next;
		if (NULL == bgq_cq->err_head)
			bgq_cq->err_tail = NULL;

		*buf = ext->err_entry;
		free(ext);

		ret = fi_bgq_unlock_if_required(&bgq_cq->lock, lock_required);
		if (ret) return ret;

	} else {

		uint64_t value = 0;
		if (l2atomic_fifo_peek(&bgq_cq->err_consumer, &value) != 0) {
			errno = FI_EAGAIN;
			return -errno;
		}

		/* const uint64_t flags = value & 0xE000000000000000ull; -- currently not used */

		/* convert the fifo value into a context pointer */
		struct fi_bgq_context_ext * ext = (struct fi_bgq_context_ext *) (value << 3);

		if (ext->bgq_context.byte_counter != 0) {
			/* perhaps an in-progress truncated rendezvous receive? */
			errno = FI_EAGAIN;
			return -errno;
		}

		assert(ext->bgq_context.flags & FI_BGQ_CQ_CONTEXT_EXT);	/* DEBUG */

		*buf = ext->err_entry;
		free(ext);

		l2atomic_fifo_advance(&bgq_cq->err_consumer);
	}

	return 1;
}

static ssize_t
fi_bgq_cq_sread(struct fid_cq *cq, void *buf, size_t len, const void *cond, int timeout)
{
	int lock_required;
	struct fi_bgq_cq *bgq_cq = container_of(cq, struct fi_bgq_cq, cq_fid);

	switch (bgq_cq->domain->threading) {
	case FI_THREAD_ENDPOINT:
	case FI_THREAD_DOMAIN:
		lock_required = 0;
	default:
		lock_required = 1;
	}

	uint64_t timeout_cycles = (timeout < 0) ?
		ULLONG_MAX :
		GetTimeBase() + (1600UL * 1000 * timeout);
	do {
		ssize_t count = fi_bgq_cq_read_generic(cq, buf, len, bgq_cq->format, lock_required);
		if (count) return count;

	} while (GetTimeBase() < timeout_cycles);
	errno = FI_EAGAIN;
	return -errno;
}

static ssize_t
fi_bgq_cq_sreadfrom(struct fid_cq *cq, void *buf, size_t len,
		   fi_addr_t *src_addr, const void *cond, int timeout)
{
	int lock_required;
	struct fi_bgq_cq *bgq_cq = container_of(cq, struct fi_bgq_cq, cq_fid);

	switch (bgq_cq->domain->threading) {
	case FI_THREAD_ENDPOINT:
	case FI_THREAD_DOMAIN:
		lock_required = 0;
	default:
		lock_required = 1;
	}

	uint64_t timeout_cycles = (timeout < 0) ?
		ULLONG_MAX :
		GetTimeBase() + (1600UL * 1000 * timeout);
	do {
		ssize_t count = fi_bgq_cq_readfrom_generic(cq, buf, len, src_addr, bgq_cq->format, lock_required);
		if (count) return count;

	} while (GetTimeBase() < timeout_cycles);
	errno = FI_EAGAIN;
	return -errno;
}

static const char *
fi_bgq_cq_strerror(struct fid_cq *cq, int prov_errno, const void *err_data,
	       char *buf, size_t len)
{
	errno = FI_ENOSYS;
	return NULL;
}

int fi_bgq_bind_ep_cq(struct fi_bgq_ep *bgq_ep,
		struct fi_bgq_cq *bgq_cq, uint64_t flags)
{
	if (!(flags & (FI_SEND | FI_RECV)))
		goto err;

	if (flags & FI_SEND) {
		fi_bgq_ref_inc(&bgq_cq->ref_cnt, "completion queue");
		bgq_ep->send_cq = bgq_cq;
		bgq_ep->tx.send.local_completion_model = bgq_cq->local_completion_model;
	}
	if (flags & FI_RECV) {
		fi_bgq_ref_inc(&bgq_cq->ref_cnt, "completion queue");
		bgq_ep->recv_cq = bgq_cq;
	}
	bgq_cq->bflags = flags;

	if (FI_CLASS_RX_CTX == bgq_ep->ep_fid.fid.fclass ||
			FI_CLASS_EP == bgq_ep->ep_fid.fid.fclass) {
		bgq_cq->ep[(bgq_cq->ep_bind_count)++] = bgq_ep;
	}

	if (ofi_recv_allowed(bgq_ep->rx.caps) || ofi_rma_target_allowed(bgq_ep->rx.caps)) {
		bgq_cq->progress.ep[(bgq_cq->progress.ep_count)++] = bgq_ep;
	}

	return 0;
err:
	errno = FI_EINVAL;
	return -errno;
}

int fi_bgq_cq_enqueue_err (struct fi_bgq_cq * bgq_cq,
		struct fi_bgq_context_ext * ext,
		const int lock_required)
{
	if (FI_BGQ_FABRIC_DIRECT_PROGRESS == FI_PROGRESS_MANUAL) {

		int lock_required = 0;
		switch (bgq_cq->domain->threading) {
		case FI_THREAD_ENDPOINT:
		case FI_THREAD_DOMAIN:
			lock_required = 0;
		default:
			lock_required = 0;
		}

		int ret;
		ret = fi_bgq_lock_if_required(&bgq_cq->lock, lock_required);
		if (ret) return ret;

		struct fi_bgq_context_ext * tail = bgq_cq->err_tail;
		if (tail) {
			assert(NULL != bgq_cq->err_head);

			tail->bgq_context.next = (union fi_bgq_context *)ext;
			bgq_cq->err_tail = ext;

		} else {
			assert(NULL == bgq_cq->err_head);

			bgq_cq->err_tail = ext;
			bgq_cq->err_head = ext;
		}
		ext->bgq_context.next = NULL;

		ret = fi_bgq_unlock_if_required(&bgq_cq->lock, lock_required);
		if (ret) return ret;

	} else {

		struct l2atomic_fifo_producer * err_producer = &bgq_cq->err_producer;
		uint64_t ext_rsh3b = (uint64_t)ext >> 3;
		while(0 != l2atomic_fifo_produce(err_producer, ext_rsh3b));
	}

	return 0;
}

FI_BGQ_CQ_SPECIALIZED_FUNC(FI_CQ_FORMAT_UNSPEC, 0)
FI_BGQ_CQ_SPECIALIZED_FUNC(FI_CQ_FORMAT_UNSPEC, 1)
FI_BGQ_CQ_SPECIALIZED_FUNC(FI_CQ_FORMAT_CONTEXT, 0)
FI_BGQ_CQ_SPECIALIZED_FUNC(FI_CQ_FORMAT_CONTEXT, 1)
FI_BGQ_CQ_SPECIALIZED_FUNC(FI_CQ_FORMAT_MSG, 0)
FI_BGQ_CQ_SPECIALIZED_FUNC(FI_CQ_FORMAT_MSG, 1)
FI_BGQ_CQ_SPECIALIZED_FUNC(FI_CQ_FORMAT_DATA, 0)
FI_BGQ_CQ_SPECIALIZED_FUNC(FI_CQ_FORMAT_DATA, 1)
/* "FI_BGQ_CQ_SPECIALIZED_FUNC(FI_CQ_FORMAT_TAGGED, 0)" is already declared via FABRIC_DIRECT */
FI_BGQ_CQ_SPECIALIZED_FUNC(FI_CQ_FORMAT_TAGGED, 1)

#define FI_BGQ_CQ_OPS_STRUCT_NAME(FORMAT, LOCK)					\
  fi_bgq_ops_cq_ ## FORMAT ## _ ## LOCK						\

#define FI_BGQ_CQ_OPS_STRUCT(FORMAT, LOCK)					\
static struct fi_ops_cq								\
	FI_BGQ_CQ_OPS_STRUCT_NAME(FORMAT, LOCK) = {				\
    .size    = sizeof(struct fi_ops_cq),					\
    .read      = FI_BGQ_CQ_SPECIALIZED_FUNC_NAME(cq_read, FORMAT, LOCK),	\
    .readfrom  = FI_BGQ_CQ_SPECIALIZED_FUNC_NAME(cq_readfrom, FORMAT, LOCK),	\
    .readerr   = fi_bgq_cq_readerr,						\
    .sread     = fi_bgq_cq_sread,						\
    .sreadfrom = fi_bgq_cq_sreadfrom,						\
    .signal    = fi_no_cq_signal,						\
    .strerror  = fi_bgq_cq_strerror,						\
}

FI_BGQ_CQ_OPS_STRUCT(FI_CQ_FORMAT_UNSPEC, 0);
FI_BGQ_CQ_OPS_STRUCT(FI_CQ_FORMAT_UNSPEC, 1);
FI_BGQ_CQ_OPS_STRUCT(FI_CQ_FORMAT_CONTEXT, 0);
FI_BGQ_CQ_OPS_STRUCT(FI_CQ_FORMAT_CONTEXT, 1);
FI_BGQ_CQ_OPS_STRUCT(FI_CQ_FORMAT_MSG, 0);
FI_BGQ_CQ_OPS_STRUCT(FI_CQ_FORMAT_MSG, 1);
FI_BGQ_CQ_OPS_STRUCT(FI_CQ_FORMAT_DATA, 0);
FI_BGQ_CQ_OPS_STRUCT(FI_CQ_FORMAT_DATA, 1);
FI_BGQ_CQ_OPS_STRUCT(FI_CQ_FORMAT_TAGGED, 0);
FI_BGQ_CQ_OPS_STRUCT(FI_CQ_FORMAT_TAGGED, 1);


static struct fi_ops_cq fi_bgq_ops_cq_default = {
	.size		= sizeof(struct fi_ops_cq),
	.read		= fi_bgq_cq_read,
	.readfrom	= fi_bgq_cq_readfrom,
	.readerr	= fi_bgq_cq_readerr,
	.signal		= fi_no_cq_signal,
	.sread		= fi_bgq_cq_sread,
	.sreadfrom	= fi_bgq_cq_sreadfrom,
	.strerror	= fi_bgq_cq_strerror
};


int fi_bgq_cq_open(struct fid_domain *dom,
		struct fi_cq_attr *attr,
		struct fid_cq **cq, void *context)
{
	int ret;
	struct fi_bgq_cq *bgq_cq;
	int lock_required;

	if (!attr) {
		FI_LOG(fi_bgq_global.prov, FI_LOG_DEBUG, FI_LOG_CQ,
				"no attr supplied\n");
		errno = FI_EINVAL;
		return -errno;
	}
	ret = fi_bgq_fid_check(&dom->fid, FI_CLASS_DOMAIN, "domain");
	if (ret)
		return ret;

	bgq_cq = calloc(1, sizeof(*bgq_cq));
	if (!bgq_cq) {
		errno = FI_ENOMEM;
		goto err;
	}

	bgq_cq->cq_fid.fid.fclass = FI_CLASS_CQ;
	bgq_cq->cq_fid.fid.context= context;
	bgq_cq->cq_fid.fid.ops    = &fi_bgq_fi_ops;

	bgq_cq->size = attr->size ? attr->size : FI_BGQ_DEFAULT_CQ_DEPTH;

	bgq_cq->domain = (struct fi_bgq_domain *) dom;

	bgq_cq->format = attr->format ? attr->format : FI_CQ_FORMAT_CONTEXT;

	bgq_cq->pending_head = NULL;
	bgq_cq->pending_tail = NULL;
	bgq_cq->completed_head = NULL;
	bgq_cq->completed_tail = NULL;
	bgq_cq->err_head = NULL;
	bgq_cq->err_tail = NULL;

	switch (bgq_cq->domain->threading) {
	case FI_THREAD_ENDPOINT:
	case FI_THREAD_DOMAIN:
	case FI_THREAD_COMPLETION:
		lock_required = 0;
		break;
	case FI_THREAD_FID:
	case FI_THREAD_UNSPEC:
	case FI_THREAD_SAFE:
		lock_required = 1;
		break;
	default:
		errno = FI_EINVAL;
	goto err;
	}

	if (lock_required == 0 &&
			bgq_cq->format == FI_CQ_FORMAT_UNSPEC) {
		bgq_cq->cq_fid.ops =
			&FI_BGQ_CQ_OPS_STRUCT_NAME(FI_CQ_FORMAT_UNSPEC, 0);
	} else if (lock_required == 0 &&
			bgq_cq->format == FI_CQ_FORMAT_CONTEXT) {
		bgq_cq->cq_fid.ops =
			&FI_BGQ_CQ_OPS_STRUCT_NAME(FI_CQ_FORMAT_CONTEXT, 0);
	} else if (lock_required == 0 &&
			bgq_cq->format == FI_CQ_FORMAT_MSG) {
		bgq_cq->cq_fid.ops =
			&FI_BGQ_CQ_OPS_STRUCT_NAME(FI_CQ_FORMAT_MSG, 0);
	} else if (lock_required == 0 &&
			bgq_cq->format == FI_CQ_FORMAT_DATA) {
		bgq_cq->cq_fid.ops =
			&FI_BGQ_CQ_OPS_STRUCT_NAME(FI_CQ_FORMAT_DATA, 0);
	} else if (lock_required == 0 &&
			bgq_cq->format == FI_CQ_FORMAT_TAGGED) {
		bgq_cq->cq_fid.ops =
			&FI_BGQ_CQ_OPS_STRUCT_NAME(FI_CQ_FORMAT_TAGGED, 0);
	} else if (lock_required == 1 &&
			bgq_cq->format == FI_CQ_FORMAT_UNSPEC) {
		bgq_cq->cq_fid.ops =
			&FI_BGQ_CQ_OPS_STRUCT_NAME(FI_CQ_FORMAT_UNSPEC, 1);
	} else if (lock_required == 1 &&
			bgq_cq->format == FI_CQ_FORMAT_CONTEXT) {
		bgq_cq->cq_fid.ops =
			&FI_BGQ_CQ_OPS_STRUCT_NAME(FI_CQ_FORMAT_CONTEXT, 1);
	} else if (lock_required == 1 &&
			bgq_cq->format == FI_CQ_FORMAT_MSG) {
		bgq_cq->cq_fid.ops =
			&FI_BGQ_CQ_OPS_STRUCT_NAME(FI_CQ_FORMAT_MSG, 1);
	} else if (lock_required == 1 &&
			bgq_cq->format == FI_CQ_FORMAT_DATA) {
		bgq_cq->cq_fid.ops =
			&FI_BGQ_CQ_OPS_STRUCT_NAME(FI_CQ_FORMAT_DATA, 1);
	} else if (lock_required == 1 &&
			bgq_cq->format == FI_CQ_FORMAT_TAGGED) {
		bgq_cq->cq_fid.ops =
			&FI_BGQ_CQ_OPS_STRUCT_NAME(FI_CQ_FORMAT_TAGGED, 1);

	} else {
		bgq_cq->cq_fid.ops =
			&fi_bgq_ops_cq_default;
	}

	/* initialize the 'local completion' direct-put descriptor model */
	{
		MUHWI_Descriptor_t * desc = &bgq_cq->local_completion_model;
		MUSPI_DescriptorZeroOut(desc);

		desc->Half_Word0.Prefetch_Only = MUHWI_DESCRIPTOR_PRE_FETCH_ONLY_NO;
		desc->Half_Word1.Interrupt = MUHWI_DESCRIPTOR_DO_NOT_INTERRUPT_ON_PACKET_ARRIVAL;
		desc->Pa_Payload = 0;				/* specified at injection time */
		desc->Message_Length = sizeof(uint64_t);
		desc->Torus_FIFO_Map =
			MUHWI_DESCRIPTOR_TORUS_FIFO_MAP_LOCAL0 |
			MUHWI_DESCRIPTOR_TORUS_FIFO_MAP_LOCAL1;
		desc->PacketHeader.NetworkHeader.pt2pt.Data_Packet_Type = MUHWI_PT2PT_DATA_PACKET_TYPE;
		desc->PacketHeader.NetworkHeader.pt2pt.Hints = 
			MUHWI_PACKET_HINT_A_NONE |
			MUHWI_PACKET_HINT_B_NONE |
			MUHWI_PACKET_HINT_C_NONE |
			MUHWI_PACKET_HINT_D_NONE;
		desc->PacketHeader.NetworkHeader.pt2pt.Byte2.Byte2 = 0;

		desc->PacketHeader.NetworkHeader.pt2pt.Byte3.Byte3 =
			MUHWI_PACKET_VIRTUAL_CHANNEL_DETERMINISTIC;

		desc->PacketHeader.NetworkHeader.pt2pt.Destination.Destination.Destination = 0;	/* not used for local transfers */

		desc->PacketHeader.NetworkHeader.pt2pt.Byte8.Byte8 = MUHWI_PACKET_TYPE_PUT;
		desc->PacketHeader.NetworkHeader.pt2pt.Byte8.Size = 16;
		desc->PacketHeader.messageUnitHeader.Packet_Types.Direct_Put.Rec_Payload_Base_Address_Id = FI_BGQ_MU_BAT_ID_COUNTER;
		desc->PacketHeader.messageUnitHeader.Packet_Types.Direct_Put.Pacing = MUHWI_PACKET_DIRECT_PUT_IS_NOT_PACED;
		desc->PacketHeader.messageUnitHeader.Packet_Types.Direct_Put.Put_Offset_MSB = 0;
		desc->PacketHeader.messageUnitHeader.Packet_Types.Direct_Put.Put_Offset_LSB = 0;
		desc->PacketHeader.messageUnitHeader.Packet_Types.Direct_Put.Unused1 = 0;
		desc->PacketHeader.messageUnitHeader.Packet_Types.Direct_Put.Rec_Counter_Base_Address_Id = FI_BGQ_MU_BAT_ID_COUNTER;
		desc->PacketHeader.messageUnitHeader.Packet_Types.Direct_Put.Valid_Bytes_In_Payload = 0;
		desc->PacketHeader.messageUnitHeader.Packet_Types.Direct_Put.Unused2 = 0;
		desc->PacketHeader.messageUnitHeader.Packet_Types.Direct_Put.Counter_Offset = 0;
	}

	/* allocate the 'std' and 'err' l2atomic fifos */
	{
		struct fi_cq_bgq_l2atomic_data * memptr = NULL;
		size_t bytes = sizeof(struct fi_cq_bgq_l2atomic_data) +
			sizeof(uint64_t) * bgq_cq->size;
		if (posix_memalign((void **)&memptr, 32, bytes)) {
			errno = FI_ENOMEM;
			goto err;
		}
		memset((void*)memptr, 0, bytes);
		bgq_cq->fifo_memptr = (void*)memptr;

		l2atomic_fifo_initialize(&bgq_cq->err_consumer,
			&bgq_cq->err_producer,
			&memptr->err_fifo_data, FI_BGQ_L2ATOMIC_ERR_FIFO_DATA_SIZE);
		l2atomic_fifo_initialize(&bgq_cq->std_consumer,
			&bgq_cq->std_producer,
			&memptr->std_fifo_data, bgq_cq->size);
	};

	bgq_cq->ep_bind_count = 0;
	bgq_cq->progress.ep_count = 0;
	unsigned i;
	for (i=0; i<64; ++i) {		/* TODO - check this array size */
		bgq_cq->ep[i] = NULL;
		bgq_cq->progress.ep[i] = NULL;
	}


	fi_bgq_ref_init(&bgq_cq->domain->fabric->node, &bgq_cq->ref_cnt, "completion queue");
	fi_bgq_ref_inc(&bgq_cq->domain->ref_cnt, "domain");

	*cq = &bgq_cq->cq_fid;

	return 0;
err:
	if(bgq_cq)
		free(bgq_cq);
	return -errno;
}
