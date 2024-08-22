/*
 * Copyright (c) 2018-2019 Intel Corporation, Inc.  All rights reserved.
 * Copyright (c) 2020 Cisco Systems, Inc.  All rights reserved.
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

#include <ofi_iov.h>

#include "mrail.h"

#define MRAIL_HDR_INITIALIZER(op_type, tag_val)		\
{							\
	.version 	= MRAIL_HDR_VERSION,		\
	.op		= op_type,			\
	.tag		= tag_val,			\
}

#define MRAIL_HDR_INITIALIZER_MSG MRAIL_HDR_INITIALIZER(ofi_op_msg, 0)

#define MRAIL_HDR_INITIALIZER_TAGGED(tag) \
	MRAIL_HDR_INITIALIZER(ofi_op_tagged, tag)

#define mrail_util_ep(ep_fid) \
	container_of(ep_fid, struct util_ep, ep_fid.fid)

#define mrail_comp_flag(ep_fid) \
	(mrail_util_ep(ep_fid)->tx_op_flags & FI_COMPLETION)

#define mrail_inject_flags(ep_fid) \
	((mrail_util_ep(ep_fid)->tx_op_flags & ~FI_COMPLETION) | FI_INJECT)

static int mrail_match_recv_any(struct dlist_entry *item, const void *arg)
{
	OFI_UNUSED(item);
	OFI_UNUSED(arg);
	return 1;
}

static int mrail_match_recv_addr(struct dlist_entry *item, const void *arg)
{
	struct mrail_match_attr *match_attr = (struct mrail_match_attr *)arg;
	struct mrail_recv *recv =
		container_of(item, struct mrail_recv, entry);

	return ofi_match_addr(recv->addr, match_attr->addr);
}

static int mrail_match_recv_tag(struct dlist_entry *item, const void *arg)
{
	struct mrail_match_attr *match_attr = (struct mrail_match_attr *)arg;
	struct mrail_recv *recv =
		container_of(item, struct mrail_recv, entry);

	return ofi_match_tag(recv->tag, recv->ignore, match_attr->tag);
}

static int mrail_match_recv_addr_tag(struct dlist_entry *item, const void *arg)
{
	struct mrail_match_attr *match_attr = (struct mrail_match_attr *)arg;
	struct mrail_recv *recv =
		container_of(item, struct mrail_recv, entry);

	return ofi_match_addr(recv->addr, match_attr->addr) &&
		ofi_match_tag(recv->tag, recv->ignore, match_attr->tag);
}

static int mrail_match_unexp_any(struct dlist_entry *item, const void *arg)
{
	OFI_UNUSED(item);
	OFI_UNUSED(arg);
	return 1;
}

static int mrail_match_unexp_addr(struct dlist_entry *item, const void *arg)
{
	struct mrail_recv *recv = (struct mrail_recv *)arg;
	struct mrail_unexp_msg_entry *unexp_msg_entry =
		container_of(item, struct mrail_unexp_msg_entry, entry);

	return ofi_match_addr(unexp_msg_entry->addr, recv->addr);
}

static int mrail_match_unexp_tag(struct dlist_entry *item, const void *arg)
{
	struct mrail_recv *recv = (struct mrail_recv *)arg;
	struct mrail_unexp_msg_entry *unexp_msg_entry =
		container_of(item, struct mrail_unexp_msg_entry, entry);

	return ofi_match_tag(recv->tag, recv->ignore, unexp_msg_entry->tag);
}

static int mrail_match_unexp_addr_tag(struct dlist_entry *item, const void *arg)
{
	struct mrail_recv *recv = (struct mrail_recv *)arg;
	struct mrail_unexp_msg_entry *unexp_msg_entry =
		container_of(item, struct mrail_unexp_msg_entry, entry);

	return ofi_match_addr(recv->addr, unexp_msg_entry->addr) &&
		ofi_match_tag(recv->tag, recv->ignore, unexp_msg_entry->tag);
}

int mrail_reprocess_directed_recvs(struct mrail_recv_queue *recv_queue)
{
	// TODO
	return -FI_ENOSYS;
}

struct mrail_recv *
mrail_match_recv_handle_unexp(struct mrail_recv_queue *recv_queue, uint64_t tag,
			      uint64_t addr, char *data, size_t len, void *context)
{
	struct dlist_entry *entry;
	struct mrail_unexp_msg_entry *unexp_msg_entry;
	struct mrail_match_attr match_attr = {
		.tag	= tag,
		.addr	= addr,
	};

	entry = dlist_remove_first_match(&recv_queue->recv_list,
					 recv_queue->match_recv, &match_attr);
	if (OFI_UNLIKELY(!entry)) {
		unexp_msg_entry = recv_queue->get_unexp_msg_entry(recv_queue,
								  context);
		if (!unexp_msg_entry) {
			FI_WARN(recv_queue->prov, FI_LOG_CQ,
				"Unable to get unexp_msg_entry!");
			assert(0);
			return NULL;
		}

		unexp_msg_entry->addr		= addr;
		unexp_msg_entry->tag		= tag;
		unexp_msg_entry->context	= context;
		memcpy(unexp_msg_entry->data, data, len);

		FI_DBG(recv_queue->prov, FI_LOG_CQ, "No matching recv found for"
		       " incoming msg with addr: 0x%" PRIx64 " tag: 0x%" PRIx64
		       "\n", unexp_msg_entry->addr, unexp_msg_entry->tag);

		FI_DBG(recv_queue->prov, FI_LOG_CQ, "Enqueueing unexp_msg_entry to "
		       "unexpected msg list\n");

		dlist_insert_tail(&unexp_msg_entry->entry,
				  &recv_queue->unexp_msg_list);
		return NULL;
	}
	return container_of(entry, struct mrail_recv, entry);
}

static void mrail_init_recv(struct mrail_recv *recv, void *arg)
{
	recv->ep		= arg;
	recv->iov[0].iov_base 	= &recv->hdr;
	recv->iov[0].iov_len 	= sizeof(recv->hdr);
	recv->comp_flags	= FI_RECV;
}

#define mrail_recv_assert(recv)						\
({									\
	assert(recv->ep == mrail_ep);					\
	assert(recv->iov[0].iov_base == &recv->hdr);			\
	assert(recv->iov[0].iov_len == sizeof(recv->hdr));		\
	assert(recv->comp_flags & FI_RECV);				\
	assert(recv->count <= mrail_ep->info->rx_attr->iov_limit + 1);	\
})

static void mrail_recv_queue_init(struct fi_provider *prov,
				  struct mrail_recv_queue *recv_queue,
				  dlist_func_t match_recv,
				  dlist_func_t match_unexp,
				  mrail_get_unexp_msg_entry_func get_unexp_msg_entry)
{
	recv_queue->prov = prov;
	dlist_init(&recv_queue->recv_list);
	dlist_init(&recv_queue->unexp_msg_list);
	recv_queue->match_recv = match_recv;
	recv_queue->match_unexp = match_unexp;
	recv_queue->get_unexp_msg_entry = get_unexp_msg_entry;
}

// TODO go for separate recv functions (recvmsg, recvv, etc) to be optimal
static ssize_t
mrail_recv_common(struct mrail_ep *mrail_ep, struct mrail_recv_queue *recv_queue,
		  struct iovec *iov, size_t count, void *context,
		  fi_addr_t src_addr, uint64_t tag, uint64_t ignore,
		  uint64_t flags, uint64_t comp_flags)
{
	struct mrail_recv *recv;
	struct mrail_unexp_msg_entry *unexp_msg_entry;

	recv = mrail_pop_recv(mrail_ep);
	if (!recv)
		return -FI_EAGAIN;

	recv->count 		= count + 1;
	recv->context 		= context;
	recv->flags 		= flags;
	recv->comp_flags 	|= comp_flags;
	recv->addr	 	= src_addr;
	recv->tag 		= tag;
	recv->ignore 		= ignore;

	memcpy(&recv->iov[1], iov, sizeof(*iov) * count);

	FI_DBG(&mrail_prov, FI_LOG_EP_DATA, "Posting recv of length: %zu "
	       "src_addr: 0x%" PRIx64 " tag: 0x%" PRIx64 " ignore: 0x%" PRIx64
	       "\n", ofi_total_iov_len(iov, count), recv->addr,
	       recv->tag, recv->ignore);

	ofi_genlock_lock(&mrail_ep->util_ep.lock);
	unexp_msg_entry = container_of(dlist_remove_first_match(
						&recv_queue->unexp_msg_list,
						recv_queue->match_unexp,
						recv),
				       struct mrail_unexp_msg_entry,
				       entry);
	if (!unexp_msg_entry) {
		dlist_insert_tail(&recv->entry, &recv_queue->recv_list);
		ofi_genlock_unlock(&mrail_ep->util_ep.lock);
		return 0;
	}
	ofi_genlock_unlock(&mrail_ep->util_ep.lock);

	FI_DBG(recv_queue->prov, FI_LOG_EP_DATA, "Match for posted recv"
	       " with addr: 0x%" PRIx64 ", tag: 0x%" PRIx64 " ignore: "
	       "0x%" PRIx64 " found in unexpected msg queue\n",
	       recv->addr, recv->tag, recv->ignore);

	return mrail_cq_process_buf_recv((struct fi_cq_tagged_entry *)
					 unexp_msg_entry->data, recv);
}

static ssize_t mrail_recv(struct fid_ep *ep_fid, void *buf, size_t len,
			  void *desc, fi_addr_t src_addr, void *context)
{
	struct mrail_ep *mrail_ep = container_of(ep_fid, struct mrail_ep,
					     util_ep.ep_fid.fid);
	struct iovec iov = {
		.iov_base	= buf,
		.iov_len	= len,
	};
	return mrail_recv_common(mrail_ep, &mrail_ep->recv_queue, &iov,
				 1, context, src_addr, 0, 0,
				 mrail_ep->util_ep.rx_op_flags, FI_MSG);
}

static ssize_t mrail_recvmsg(struct fid_ep *ep_fid, const struct fi_msg *msg,
		uint64_t flags)
{
	struct mrail_ep *mrail_ep = container_of(ep_fid, struct mrail_ep,
					     util_ep.ep_fid.fid);

	return mrail_recv_common(mrail_ep, &mrail_ep->recv_queue,
				 (struct iovec *)msg->msg_iov, msg->iov_count,
				 msg->context, msg->addr, 0, 0, flags, FI_MSG);
}

static ssize_t mrail_trecv(struct fid_ep *ep_fid, void *buf, size_t len,
			    void *desc, fi_addr_t src_addr, uint64_t tag,
			    uint64_t ignore, void *context)
{
	struct mrail_ep *mrail_ep = container_of(ep_fid, struct mrail_ep,
					     util_ep.ep_fid.fid);
	struct iovec iov = {
		.iov_base	= buf,
		.iov_len	= len,
	};
	return mrail_recv_common(mrail_ep, &mrail_ep->trecv_queue, &iov,
				 1, context, src_addr, tag, ignore,
				 mrail_ep->util_ep.rx_op_flags, FI_TAGGED);
}

static ssize_t mrail_trecvmsg(struct fid_ep *ep_fid,
				const struct fi_msg_tagged *msg,
				uint64_t flags)
{
	struct mrail_ep *mrail_ep = container_of(ep_fid, struct mrail_ep,
					     util_ep.ep_fid.fid);

	return mrail_recv_common(mrail_ep, &mrail_ep->trecv_queue,
				 (struct iovec *)msg->msg_iov, msg->iov_count,
				 msg->context, msg->addr, msg->tag,
				 msg->ignore,
				 (mrail_ep->util_ep.rx_op_flags | flags),
				 FI_TAGGED);
}

static void mrail_copy_iov_hdr(struct mrail_hdr *hdr, struct iovec *iov_dest,
			       const struct iovec *iov_src, size_t count)
{
	iov_dest[0].iov_base = hdr;
	iov_dest[0].iov_len = sizeof(*hdr);
	memcpy(&iov_dest[1], iov_src, sizeof(*iov_src) * count);
}

static struct mrail_tx_buf *mrail_get_tx_buf(struct mrail_ep *mrail_ep,
					     void *context, uint32_t seq,
					     uint8_t op, uint64_t flags)
{
	struct mrail_tx_buf *tx_buf = ofi_buf_alloc(mrail_ep->tx_buf_pool);
	if (OFI_UNLIKELY(!tx_buf))
		return NULL;

	assert(tx_buf->ep == mrail_ep);
	assert(tx_buf->hdr.version == MRAIL_HDR_VERSION);

	tx_buf->context		= context;
	tx_buf->flags		= flags;
	tx_buf->hdr.op		= op;
	tx_buf->hdr.seq		= htonl(seq);
	return tx_buf;
}

/*
 * This is an internal send that doesn't use seq_no and doesn't update
 * the counters. The call doesn't return -FI_EAGAIN.
 */
int mrail_send_rndv_ack_blocking(struct mrail_ep *mrail_ep,
				 struct mrail_cq *mrail_cq,
				 fi_addr_t dest_addr, void *context)
{
	struct iovec iov_dest;
	struct mrail_tx_buf *tx_buf;
	size_t rndv_pkt_size = sizeof(tx_buf->hdr) + sizeof(tx_buf->rndv_hdr);
	int policy = mrail_get_policy(rndv_pkt_size);
	uint32_t i = mrail_get_tx_rail(mrail_ep, policy);
	struct fi_msg msg;
	ssize_t ret;
	uint64_t flags = FI_COMPLETION;

	ofi_genlock_lock(&mrail_ep->util_ep.lock);

	tx_buf = mrail_get_tx_buf(mrail_ep, context, 0, ofi_op_tagged, 0);
	if (OFI_UNLIKELY(!tx_buf))
		return -FI_ENOMEM;

	tx_buf->hdr.protocol = MRAIL_PROTO_RNDV;
	tx_buf->hdr.protocol_cmd = MRAIL_RNDV_ACK;
	tx_buf->rndv_hdr.context = (uint64_t)context;

	iov_dest.iov_base = &tx_buf->hdr;
	iov_dest.iov_len = rndv_pkt_size;

	msg.msg_iov 	= &iov_dest;
	msg.desc    	= NULL;
	msg.iov_count	= 1;
	msg.addr	= dest_addr;
	msg.context	= tx_buf;

	if (iov_dest.iov_len < mrail_ep->rails[i].info->tx_attr->inject_size)
		flags |= FI_INJECT;

	FI_DBG(&mrail_prov, FI_LOG_EP_DATA, "Posting rdnv ack "
	       " dest_addr: 0x%" PRIx64 " on rail: %d\n", dest_addr, i);

	do {
		ret = fi_sendmsg(mrail_ep->rails[i].ep, &msg, flags);
		if (ret == -FI_EAGAIN) {
			FI_DBG(&mrail_prov, FI_LOG_EP_DATA,
			        "Resource busy when trying to fi_sendmsg on rail: %"
				PRIu32 ", retrying.\n", i);
			fi_cq_read(mrail_cq->cqs[i], NULL, 0);
		}
	} while (ret == -FI_EAGAIN);

	if (ret) {
		FI_WARN(&mrail_prov, FI_LOG_EP_DATA,
			"Unable to fi_sendmsg on rail: %" PRIu32 "\n", i);
		ofi_buf_free(tx_buf);
	}

	ofi_genlock_unlock(&mrail_ep->util_ep.lock);
	return ret;
}

static ssize_t
mrail_prepare_rndv_req(struct mrail_ep *mrail_ep, struct mrail_tx_buf *tx_buf,
		       const struct iovec *iov, void **desc, size_t count,
		       size_t len, struct iovec *iov_dest)
{
	size_t mr_count;
	struct fid_mr *mr;
	uint64_t addr, *base_addrs;
	size_t key_size, offset;
	size_t total_key_size = 0;
	ssize_t ret;
	int i;

	tx_buf->hdr.protocol = MRAIL_PROTO_RNDV;
	tx_buf->hdr.protocol_cmd = MRAIL_RNDV_REQ;
	tx_buf->rndv_hdr.context = (uint64_t)tx_buf;
	tx_buf->rndv_req = NULL;

	if (!desc || !desc[0]) {
		ret = fi_mr_regv(&mrail_ep->util_ep.domain->domain_fid,
				 iov, count, FI_REMOTE_READ, 0, 0, 0, &mr, 0);
		if (ret)
			return ret;
		total_key_size = 0;
		ret = fi_mr_raw_attr(mr, &addr, NULL, &total_key_size, 0);
		assert(ret == -FI_ETOOSMALL);
		mr_count = 1;
		tx_buf->rndv_mr_fid = &mr->fid;
	} else {
		total_key_size = 0;
		for (i = 0; i < count; i++) {
			mr = &((struct mrail_mr *)desc[i])->mr_fid;
			key_size = 0;
			ret = fi_mr_raw_attr(mr, &addr, NULL, &key_size, 0);
			assert(ret == -FI_ETOOSMALL);
			total_key_size += key_size;
		}
		mr_count = count;
		tx_buf->rndv_mr_fid = NULL;
	}

	tx_buf->rndv_req = malloc(sizeof(*tx_buf->rndv_req) + total_key_size +
				  sizeof(*base_addrs) * mr_count);
	if (!tx_buf->rndv_req)
		return -FI_ENOMEM;

	tx_buf->rndv_req->len = len;
	tx_buf->rndv_req->count = count;
	tx_buf->rndv_req->mr_count = mr_count;
	tx_buf->rndv_req->rawkey_size = total_key_size;

	base_addrs = (uint64_t *)(tx_buf->rndv_req->rawkey + total_key_size);
	for (offset = 0, i = 0; i < count; i++) {
		if (i < mr_count) {
			if (mr_count > 1)
				mr = &((struct mrail_mr *)desc[i])->mr_fid;
			key_size = total_key_size - offset;
			ret = fi_mr_raw_attr(mr, &base_addrs[i],
					     tx_buf->rndv_req->rawkey + offset,
					     &key_size, 0);
			assert(!ret);
			offset += key_size;
		}
		tx_buf->rndv_req->rma_iov[i].addr = (uint64_t)iov[i].iov_base;
		tx_buf->rndv_req->rma_iov[i].len = iov[i].iov_len;
		tx_buf->rndv_req->rma_iov[i].key = key_size; /* otherwise unused */
	}

	iov_dest[0].iov_base = &tx_buf->hdr;
	iov_dest[0].iov_len = sizeof(tx_buf->hdr) + sizeof(tx_buf->rndv_hdr);
	iov_dest[1].iov_base = tx_buf->rndv_req;
	iov_dest[1].iov_len = sizeof(*tx_buf->rndv_req) + total_key_size +
			      sizeof(uint64_t) * mr_count;

	return 0;
}

static ssize_t
mrail_send_common(struct fid_ep *ep_fid, const struct iovec *iov, void **desc,
		  size_t count, size_t len, fi_addr_t dest_addr, uint64_t tag,
		  uint64_t data, void *context, uint64_t flags, uint64_t op)
{
	struct mrail_ep *mrail_ep = container_of(ep_fid, struct mrail_ep,
						 util_ep.ep_fid.fid);
	struct mrail_peer_info *peer_info;
	struct iovec *iov_dest = alloca(sizeof(*iov_dest) * (count + 1));
	struct mrail_tx_buf *tx_buf;
	int policy = mrail_get_policy(len);
	uint32_t rail = mrail_get_tx_rail(mrail_ep, policy);
	struct fi_msg msg;
	ssize_t ret;
	size_t total_len;

	peer_info = ofi_av_get_addr(mrail_ep->util_ep.av, (int) dest_addr);

	ofi_genlock_lock(&mrail_ep->util_ep.lock);

	tx_buf = mrail_get_tx_buf(mrail_ep, context, peer_info->seq_no++,
				  ofi_op_tagged, flags | op);
	if (OFI_UNLIKELY(!tx_buf)) {
		ret = -FI_ENOMEM;
		goto err1;
	}
	tx_buf->hdr.tag = tag;

	if (policy == MRAIL_POLICY_STRIPING) {
		ret = mrail_prepare_rndv_req(mrail_ep, tx_buf, iov, desc,
					     count, len, iov_dest);
		if (ret)
			goto err2;

		msg.msg_iov 	= iov_dest;
		msg.desc    	= NULL;	/* it's fine since FI_MR_LOCAL is unsupported */
		msg.iov_count	= 2;
		msg.addr	= dest_addr;
		msg.context	= tx_buf;
		msg.data	= data;
		total_len = iov_dest[0].iov_len + iov_dest[1].iov_len;
	} else {
		tx_buf->hdr.protocol = MRAIL_PROTO_EAGER;
		mrail_copy_iov_hdr(&tx_buf->hdr, iov_dest, iov, count);

		msg.msg_iov 	= iov_dest;
		msg.desc    	= desc;	/* doesn't matter since FI_MR_LOCAL is unsupported */
		msg.iov_count	= count + 1;
		msg.addr	= dest_addr;
		msg.context	= tx_buf;
		msg.data	= data;
		total_len = len + iov_dest[0].iov_len;
	}

	if (total_len < mrail_ep->rails[rail].info->tx_attr->inject_size)
		flags |= FI_INJECT;

	FI_DBG(&mrail_prov, FI_LOG_EP_DATA, "Posting send of length: %zu"
	       " dest_addr: 0x%" PRIx64 " tag: 0x%" PRIx64 " seq: %d"
	       " on rail: %d\n", len, dest_addr, tag, peer_info->seq_no - 1, rail);

	ret = fi_sendmsg(mrail_ep->rails[rail].ep, &msg, flags | FI_COMPLETION);
	if (ret) {
		FI_WARN(&mrail_prov, FI_LOG_EP_DATA,
			"Unable to fi_sendmsg on rail: %" PRIu32 "\n", rail);
		goto err2;
	} else if (!(flags & FI_COMPLETION)) {
		ofi_ep_cntr_inc(&mrail_ep->util_ep, CNTR_TX);
	}
	ofi_genlock_unlock(&mrail_ep->util_ep.lock);
	return ret;
err2:
	if (tx_buf->hdr.protocol == MRAIL_PROTO_RNDV) {
		free(tx_buf->rndv_req);
		fi_close(tx_buf->rndv_mr_fid);
	}
	ofi_buf_free(tx_buf);
err1:
	peer_info->seq_no--;
	ofi_genlock_unlock(&mrail_ep->util_ep.lock);
	return ret;
}

static ssize_t mrail_sendmsg(struct fid_ep *ep_fid, const struct fi_msg *msg,
			     uint64_t flags)
{
	return mrail_send_common(ep_fid, msg->msg_iov, msg->desc, msg->iov_count,
				 ofi_total_iov_len(msg->msg_iov, msg->iov_count),
				 msg->addr, 0, msg->data, msg->context,
				 flags | mrail_comp_flag(ep_fid), FI_MSG);
}

static ssize_t mrail_send(struct fid_ep *ep_fid, const void *buf, size_t len,
			  void *desc, fi_addr_t dest_addr, void *context)
{
	struct iovec iov = {
		.iov_base 	= (void *)buf,
		.iov_len 	= len,
	};
	return mrail_send_common(ep_fid, &iov, &desc, 1, len, dest_addr, 0, 0,
				 context, mrail_comp_flag(ep_fid), FI_MSG);
}

static ssize_t mrail_inject(struct fid_ep *ep_fid, const void *buf, size_t len,
			    fi_addr_t dest_addr)
{
	struct iovec iov = {
		.iov_base 	= (void *)buf,
		.iov_len 	= len,
	};
	return mrail_send_common(ep_fid, &iov, NULL, 1, len, dest_addr, 0, 0,
				 NULL, mrail_inject_flags(ep_fid), FI_MSG);
}

static ssize_t mrail_injectdata(struct fid_ep *ep_fid, const void *buf,
				size_t len, uint64_t data, fi_addr_t dest_addr)
{
	struct iovec iov = {
		.iov_base 	= (void *)buf,
		.iov_len 	= len,
	};
	return mrail_send_common(ep_fid, &iov, NULL, 1, len, dest_addr, 0, data,
				 NULL, (mrail_inject_flags(ep_fid) |
					FI_REMOTE_CQ_DATA), FI_MSG);
}

static ssize_t
mrail_tsendmsg(struct fid_ep *ep_fid, const struct fi_msg_tagged *msg,
	       uint64_t flags)
{
	return mrail_send_common(ep_fid, msg->msg_iov, msg->desc, msg->iov_count,
				 ofi_total_iov_len(msg->msg_iov, msg->iov_count),
				 msg->addr, msg->tag, msg->data, msg->context,
				 flags | mrail_comp_flag(ep_fid), FI_TAGGED);
}

static ssize_t mrail_tsend(struct fid_ep *ep_fid, const void *buf, size_t len,
			   void *desc, fi_addr_t dest_addr, uint64_t tag,
			   void *context)
{
	struct iovec iov = {
		.iov_base 	= (void *)buf,
		.iov_len 	= len,
	};
	return mrail_send_common(ep_fid, &iov, &desc, 1, len, dest_addr, tag,
				 0, context, mrail_comp_flag(ep_fid), FI_TAGGED);
}

static ssize_t mrail_tsenddata(struct fid_ep *ep_fid, const void *buf, size_t len,
			       void *desc, uint64_t data, fi_addr_t dest_addr,
			       uint64_t tag, void *context)
{
	struct iovec iov = {
		.iov_base 	= (void *)buf,
		.iov_len 	= len,
	};
	return mrail_send_common(ep_fid, &iov, &desc, 1, len, dest_addr, tag,
				 data, context, (mrail_comp_flag(ep_fid) |
						 FI_REMOTE_CQ_DATA), FI_TAGGED);
}

static ssize_t mrail_tinject(struct fid_ep *ep_fid, const void *buf, size_t len,
			     fi_addr_t dest_addr, uint64_t tag)
{
	struct iovec iov = {
		.iov_base 	= (void *)buf,
		.iov_len 	= len,
	};
	return mrail_send_common(ep_fid, &iov, NULL, 1, len, dest_addr, tag,
				 0, NULL, mrail_inject_flags(ep_fid), FI_TAGGED);
}

static ssize_t mrail_tinjectdata(struct fid_ep *ep_fid, const void *buf,
				 size_t len, uint64_t data, fi_addr_t dest_addr,
				 uint64_t tag)
{
	struct iovec iov = {
		.iov_base 	= (void *)buf,
		.iov_len 	= len,
	};
	return mrail_send_common(ep_fid, &iov, NULL, 1, len, dest_addr, tag,
				 data, NULL, (mrail_inject_flags(ep_fid) |
					      FI_REMOTE_CQ_DATA), FI_TAGGED);
}

static struct mrail_unexp_msg_entry *
mrail_get_unexp_msg_entry(struct mrail_recv_queue *recv_queue, void *context)
{
	// TODO use buf pool
	// context would be mrail_ep from which u can get the buf pool
	struct mrail_unexp_msg_entry *unexp_msg_entry =
		malloc(sizeof(*unexp_msg_entry) + sizeof(struct fi_cq_tagged_entry));
	return unexp_msg_entry;
}

static int mrail_getname(fid_t fid, void *addr, size_t *addrlen)
{
	struct mrail_ep *mrail_ep =
		container_of(fid, struct mrail_ep, util_ep.ep_fid.fid);
	struct mrail_domain *mrail_domain =
		container_of(mrail_ep->util_ep.domain, struct mrail_domain,
			     util_domain);
	size_t i, offset = 0, rail_addrlen;
	int ret;

	if (*addrlen < mrail_domain->addrlen) {
		*addrlen = mrail_domain->addrlen;
		return -FI_ETOOSMALL;
	}

	for (i = 0; i < mrail_ep->num_eps; i++) {
		rail_addrlen = *addrlen - offset;
		ret = fi_getname(&mrail_ep->rails[i].ep->fid,
				 (char *)addr + offset, &rail_addrlen);
		if (ret) {
			FI_WARN(&mrail_prov, FI_LOG_EP_CTRL,
				"Unable to get name for rail: %zd\n", i);
			return ret;
		}
		offset += rail_addrlen;
	}

	assert(offset == mrail_domain->addrlen);
	*addrlen = mrail_domain->addrlen;
	return 0;
}


static void mrail_tx_buf_init(struct ofi_bufpool_region *region, void *buf)
{
	struct mrail_tx_buf *tx_buf = buf;

	tx_buf->ep		= region->pool->attr.context;
	tx_buf->hdr.version	= MRAIL_HDR_VERSION;
}

static void mrail_ep_free_bufs(struct mrail_ep *mrail_ep)
{
	if (mrail_ep->req_pool)
		ofi_bufpool_destroy(mrail_ep->req_pool);

	if (mrail_ep->ooo_recv_pool)
		ofi_bufpool_destroy(mrail_ep->ooo_recv_pool);

	if (mrail_ep->tx_buf_pool)
		ofi_bufpool_destroy(mrail_ep->tx_buf_pool);

	if (mrail_ep->recv_fs)
		mrail_recv_fs_free(mrail_ep->recv_fs);
}

static int mrail_ep_alloc_bufs(struct mrail_ep *mrail_ep)
{
	struct ofi_bufpool_attr attr = {
		.size		= sizeof(struct mrail_tx_buf),
		.alignment	= sizeof(void *),
		.chunk_cnt	= 64,
		.init_fn	= mrail_tx_buf_init,
		.context	= mrail_ep,
	};
	size_t buf_size, rxq_total_size = 0;
	struct fi_info *fi;
	int ret;

	for (fi = mrail_ep->info->next; fi; fi = fi->next)
		rxq_total_size += fi->rx_attr->size;

	mrail_ep->recv_fs = mrail_recv_fs_create(rxq_total_size, mrail_init_recv,
						 mrail_ep);
	if (!mrail_ep->recv_fs)
		return -FI_ENOMEM;

	ret = ofi_bufpool_create(&mrail_ep->ooo_recv_pool,
				 sizeof(struct mrail_ooo_recv),
				 sizeof(void *), 0, 64, 0);
	if (!mrail_ep->ooo_recv_pool)
		goto err;

	ret = ofi_bufpool_create_attr(&attr, &mrail_ep->tx_buf_pool);
	if (!mrail_ep->tx_buf_pool)
		goto err;

	buf_size = (sizeof(struct mrail_req) +
		    (mrail_ep->num_eps * sizeof(struct mrail_subreq)));

	ret = ofi_bufpool_create(&mrail_ep->req_pool, buf_size,
				 sizeof(void *), 0, 64, OFI_BUFPOOL_HUGEPAGES);
	if (ret)
		goto err;
	return 0;
err:
	mrail_ep_free_bufs(mrail_ep);
	return ret;
}

static int mrail_ep_close(fid_t fid)
{
	struct mrail_ep *mrail_ep =
		container_of(fid, struct mrail_ep, util_ep.ep_fid.fid);
	int ret, retv = 0;
	size_t i;

	mrail_ep_free_bufs(mrail_ep);

	for (i = 0; i < mrail_ep->num_eps; i++) {
		ret = fi_close(&mrail_ep->rails[i].ep->fid);
		if (ret)
			retv = ret;
	}
	free(mrail_ep->rails);

	ret = ofi_endpoint_close(&mrail_ep->util_ep);
	if (ret)
		retv = ret;
	free(mrail_ep);
	return retv;
}

static int mrail_ep_bind(struct fid *ep_fid, struct fid *bfid, uint64_t flags)
{
	struct mrail_ep *mrail_ep =
		container_of(ep_fid, struct mrail_ep, util_ep.ep_fid.fid);
	struct mrail_cq *mrail_cq;
	struct mrail_av *mrail_av;
	struct util_cntr *cntr;
	int ret = 0;
	size_t i;

	switch (bfid->fclass) {
	case FI_CLASS_AV:
		mrail_av = container_of(bfid, struct mrail_av,
					util_av.av_fid.fid);
		ret = ofi_ep_bind_av(&mrail_ep->util_ep, &mrail_av->util_av);
		if (ret)
			return ret;
		for (i = 0; i < mrail_ep->num_eps; i++) {
			ret = fi_ep_bind(mrail_ep->rails[i].ep,
					 &mrail_av->avs[i]->fid, flags);
			if (ret)
				return ret;
		}
		break;
	case FI_CLASS_CQ:
		mrail_cq = container_of(bfid, struct mrail_cq,
					util_cq.cq_fid.fid);

		ret = ofi_ep_bind_cq(&mrail_ep->util_ep, &mrail_cq->util_cq,
				     flags);
		if (ret)
			return ret;
		for (i = 0; i < mrail_ep->num_eps; i++) {
			ret = fi_ep_bind(mrail_ep->rails[i].ep,
					 &mrail_cq->cqs[i]->fid, flags);
			if (ret)
				return ret;
		}
		break;
	case FI_CLASS_CNTR:
		cntr = container_of(bfid, struct util_cntr, cntr_fid.fid);

		ret = ofi_ep_bind_cntr(&mrail_ep->util_ep, cntr, flags);
		if (ret)
			return ret;
		break;
	case FI_CLASS_EQ:
		ret = -FI_ENOSYS;
		break;
	default:
		FI_WARN(&mrail_prov, FI_LOG_EP_CTRL, "invalid fid class\n");
		ret = -FI_EINVAL;
		break;
	}
	return ret;
}

static int mrail_ep_ctrl(struct fid *fid, int command, void *arg)
{
	struct mrail_ep *mrail_ep;
	size_t i, buf_recv_min = sizeof(struct mrail_hdr);
	int ret;

	mrail_ep = container_of(fid, struct mrail_ep, util_ep.ep_fid.fid);

	switch (command) {
	case FI_ENABLE:
		if (!mrail_ep->util_ep.rx_cq || !mrail_ep->util_ep.tx_cq)
			return -FI_ENOCQ;
		if (!mrail_ep->util_ep.av)
			return -FI_ENOAV;
		for (i = 0; i < mrail_ep->num_eps; i++) {
			ret = fi_setopt(&mrail_ep->rails[i].ep->fid,
					FI_OPT_ENDPOINT, FI_OPT_BUFFERED_MIN,
					&buf_recv_min, sizeof(buf_recv_min));
			if (ret)
				return ret;

			ret = fi_enable(mrail_ep->rails[i].ep);
			if (ret)
				return ret;
		}
		break;
	default:
		return -FI_ENOSYS;
	}
	return 0;
}

static struct fi_ops mrail_ep_fi_ops = {
	.size = sizeof(struct fi_ops),
	.close = mrail_ep_close,
	.bind = mrail_ep_bind,
	.control = mrail_ep_ctrl,
	.ops_open = fi_no_ops_open,
};

static int mrail_ep_setopt(fid_t fid, int level, int optname,
		const void *optval, size_t optlen)
{
	struct mrail_ep *mrail_ep;
	size_t i;
	int ret = 0;

	mrail_ep = container_of(fid, struct mrail_ep, util_ep.ep_fid.fid);

	for (i = 0; i < mrail_ep->num_eps; i++) {
		ret = fi_setopt(&mrail_ep->rails[i].ep->fid, level, optname,
				optval, optlen);
		if (ret)
			return ret;
	}

	return ret;
}

static struct fi_ops_ep mrail_ops_ep = {
	.size = sizeof(struct fi_ops_ep),
	.cancel = fi_no_cancel,
	.getopt = fi_no_getopt,
	.setopt = mrail_ep_setopt,
	.tx_ctx = fi_no_tx_ctx,
	.rx_ctx = fi_no_rx_ctx,
	.rx_size_left = fi_no_rx_size_left,
	.tx_size_left = fi_no_tx_size_left,
};

static struct fi_ops_cm mrail_ops_cm = {
	.size = sizeof(struct fi_ops_cm),
	.setname = fi_no_setname,
	.getname = mrail_getname,
	.getpeer = fi_no_getpeer,
	.connect = fi_no_connect,
	.listen = fi_no_listen,
	.accept = fi_no_accept,
	.reject = fi_no_reject,
	.shutdown = fi_no_shutdown,
	.join = fi_no_join,
};

static struct fi_ops_msg mrail_ops_msg = {
	.size = sizeof(struct fi_ops_msg),
	.recv = mrail_recv,
	.recvv = fi_no_msg_recvv,
	.recvmsg = mrail_recvmsg,
	.send = mrail_send,
	.sendv = fi_no_msg_sendv,
	.sendmsg = mrail_sendmsg,
	.inject = mrail_inject,
	.senddata = fi_no_msg_senddata,
	.injectdata = mrail_injectdata,
};

struct fi_ops_tagged mrail_ops_tagged = {
	.size = sizeof(struct fi_ops_tagged),
	.recv = mrail_trecv,
	.recvv = fi_no_tagged_recvv,
	.recvmsg = mrail_trecvmsg,
	.send = mrail_tsend,
	.sendv = fi_no_tagged_sendv,
	.sendmsg = mrail_tsendmsg,
	.inject = mrail_tinject,
	.senddata = mrail_tsenddata,
	.injectdata = mrail_tinjectdata,
};

void mrail_ep_progress(struct util_ep *ep)
{
	struct mrail_ep *mrail_ep;
	mrail_ep = container_of(ep, struct mrail_ep, util_ep);
	mrail_progress_deferred_reqs(mrail_ep);
}

int mrail_ep_open(struct fid_domain *domain_fid, struct fi_info *info,
		  struct fid_ep **ep_fid, void *context)
{
	struct mrail_domain *mrail_domain =
		container_of(domain_fid, struct mrail_domain,
			     util_domain.domain_fid);
	struct mrail_ep *mrail_ep;
	struct fi_info *fi;
	size_t i;
	int ret;

	if (strcmp(mrail_domain->info->domain_attr->name,
		    info->domain_attr->name)) {
		FI_WARN(&mrail_prov, FI_LOG_EP_CTRL, "info domain name: %s "
			"doesn't match fid_domain name: %s!\n",
			info->domain_attr->name,
			mrail_domain->info->domain_attr->name);
		return -FI_EINVAL;
	}

	mrail_ep = calloc(1, sizeof(*mrail_ep));
	if (!mrail_ep)
		return -FI_ENOMEM;

	// TODO detect changes b/w mrail_domain->info and info arg
	// this may be difficult and we may not support such changes
	mrail_ep->info = mrail_domain->info;
	mrail_ep->num_eps = mrail_domain->num_domains;

	ret = ofi_endpoint_init(domain_fid, &mrail_util_prov, info, &mrail_ep->util_ep,
				context, &mrail_ep_progress);
	if (ret) {
		goto free_ep;
	}

	mrail_ep->rails = calloc(mrail_ep->num_eps, sizeof(*mrail_ep->rails));
	if (!mrail_ep->rails) {
		ret = -FI_ENOMEM;
		goto err;
	}

	for (i = 0, fi = mrail_ep->info->next; fi; fi = fi->next, i++) {
		fi->tx_attr->op_flags &= ~FI_COMPLETION;
		ret = fi_endpoint(mrail_domain->domains[i], fi,
				  &mrail_ep->rails[i].ep, mrail_ep);
		if (ret) {
			FI_WARN(&mrail_prov, FI_LOG_EP_CTRL,
				"Unable to open EP\n");
			goto err;
		}
		mrail_ep->rails[i].info = fi;
	}

	ret = mrail_ep_alloc_bufs(mrail_ep);
	if (ret)
		goto err;

	slist_init(&mrail_ep->deferred_reqs);

	if (mrail_ep->info->caps & FI_DIRECTED_RECV) {
		mrail_recv_queue_init(&mrail_prov, &mrail_ep->recv_queue,
				      mrail_match_recv_addr,
				      mrail_match_unexp_addr,
				      mrail_get_unexp_msg_entry);
		mrail_recv_queue_init(&mrail_prov, &mrail_ep->trecv_queue,
				      mrail_match_recv_addr_tag,
				      mrail_match_unexp_addr_tag,
				      mrail_get_unexp_msg_entry);
	} else {
		mrail_recv_queue_init(&mrail_prov, &mrail_ep->recv_queue,
				      mrail_match_recv_any,
				      mrail_match_unexp_any,
				      mrail_get_unexp_msg_entry);
		mrail_recv_queue_init(&mrail_prov, &mrail_ep->trecv_queue,
				      mrail_match_recv_tag,
				      mrail_match_unexp_tag,
				      mrail_get_unexp_msg_entry);
	}

	ofi_atomic_initialize32(&mrail_ep->tx_rail, 0);
	ofi_atomic_initialize32(&mrail_ep->rx_rail, 0);
	mrail_ep->default_tx_rail = mrail_local_rank % mrail_ep->num_eps;

	*ep_fid = &mrail_ep->util_ep.ep_fid;
	(*ep_fid)->fid.ops = &mrail_ep_fi_ops;
	(*ep_fid)->ops = &mrail_ops_ep;
	(*ep_fid)->cm = &mrail_ops_cm;
	(*ep_fid)->msg = &mrail_ops_msg;
	(*ep_fid)->tagged = &mrail_ops_tagged;
	(*ep_fid)->rma = &mrail_ops_rma;

	return 0;
err:
	mrail_ep_close(&mrail_ep->util_ep.ep_fid.fid);
free_ep:
	free(mrail_ep);
	return ret;
}
