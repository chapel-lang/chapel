/*
 * Copyright (c) 2018-2019 Intel Corporation, Inc.  All rights reserved.
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
 *	- Redistributions of source code must retain the above
 *	  copyright notice, this list of conditions and the following
 *	  disclaimer.
 *
 *	- Redistributions in binary form must reproduce the above
 *	  copyright notice, this list of conditions and the following
 *	  disclaimer in the documentation and/or other materials
 *	  provided with the distribution.
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

static int mrail_cq_write_send_comp(struct util_cq *cq,
				    struct mrail_tx_buf *tx_buf)
{
	int ret = 0;

	ofi_ep_cntr_inc(&tx_buf->ep->util_ep, CNTR_TX);

	if (tx_buf->flags & FI_COMPLETION) {
		ret = ofi_cq_write(cq, tx_buf->context,
				   (tx_buf->flags &
				    (FI_TAGGED | FI_MSG)) |
				   FI_SEND, 0, NULL, 0, 0);
		if (ret) {
			FI_WARN(&mrail_prov, FI_LOG_CQ,
				"Unable to write to util cq\n");
		}
	}

	if (tx_buf->hdr.protocol == MRAIL_PROTO_RNDV &&
	    tx_buf->hdr.protocol_cmd == MRAIL_RNDV_REQ) {
		free(tx_buf->rndv_req);
		fi_close(tx_buf->rndv_mr_fid);
	}

	ofi_genlock_lock(&tx_buf->ep->util_ep.lock);
	ofi_buf_free(tx_buf);
	ofi_genlock_unlock(&tx_buf->ep->util_ep.lock);

	return ret;
}

int mrail_cq_write_recv_comp(struct mrail_ep *mrail_ep, struct mrail_hdr *hdr,
			     struct fi_cq_tagged_entry *comp,
			     struct mrail_recv *recv)
{
	FI_DBG(&mrail_prov, FI_LOG_CQ, "finish recv: length: %zu "
	       "tag: 0x%" PRIx64 "\n", comp->len - sizeof(struct mrail_pkt),
	       hdr->tag);
	ofi_ep_cntr_inc(&mrail_ep->util_ep, CNTR_RX);
	if (!(recv->flags & FI_COMPLETION))
		return 0;
	return ofi_cq_write(mrail_ep->util_ep.rx_cq, recv->context,
			   recv->comp_flags |
			   (comp->flags & FI_REMOTE_CQ_DATA),
			   comp->len - sizeof(struct mrail_pkt),
			   NULL, comp->data, hdr->tag);
}

static int mrail_cq_write_rndv_recv_comp(struct mrail_ep *mrail_ep,
					 struct mrail_recv *recv)
{
	FI_DBG(&mrail_prov, FI_LOG_CQ, "finish rndv recv: length: %zu "
	       "tag: 0x%" PRIx64 "\n", recv->rndv.len, recv->rndv.tag);
	ofi_ep_cntr_inc(&mrail_ep->util_ep, CNTR_RX);
	if (!(recv->flags & FI_COMPLETION))
		return 0;
	return ofi_cq_write(mrail_ep->util_ep.rx_cq, recv->context,
			   recv->comp_flags | recv->rndv.flags,
			   recv->rndv.len, NULL, recv->rndv.data,
			   recv->rndv.tag);
}

static void mrail_finish_rndv_recv(struct util_cq *cq,
				   struct mrail_req *req,
				   struct fi_cq_tagged_entry *comp)
{
	struct mrail_cq *mrail_cq = container_of(cq, struct mrail_cq, util_cq);
	struct mrail_recv *recv = req->comp.op_context;
	int ret;

	ret = mrail_cq_write_rndv_recv_comp(req->mrail_ep, recv);
	if (ret) {
		FI_WARN(&mrail_prov, FI_LOG_CQ,
			"Cannot write to recv cq\n");
		assert(0);
	}

	ret = mrail_send_rndv_ack_blocking(req->mrail_ep, mrail_cq, recv->addr,
					   (void *)recv->rndv.context);
	if (ret) {
		FI_WARN(&mrail_prov, FI_LOG_CQ,
			"Cannot send rndv ack: %s\n", fi_strerror(-ret));
		assert(0);
	}

	mrail_free_req(req->mrail_ep, req);
	mrail_push_recv(recv);
}

static int mrail_cq_process_rndv_req(struct fi_cq_tagged_entry *comp,
				     struct mrail_recv *recv)
{
	struct fi_recv_context *recv_ctx = comp->op_context;
	struct fi_msg msg = {
		.context = recv_ctx,
	};
	struct mrail_ep *mrail_ep;
	struct mrail_pkt *mrail_pkt;
	struct mrail_rndv_hdr *rndv_hdr;
	struct mrail_rndv_req *rndv_req;
	struct fi_msg_rma rma_msg;
	uint64_t *base_addrs;
	size_t key_size;
	size_t offset;
	int ret, retv = 0;
	int i;

	mrail_ep = recv_ctx->ep->fid.context;
	mrail_pkt = (struct mrail_pkt *)comp->buf;
	rndv_hdr = (struct mrail_rndv_hdr *)&mrail_pkt[1];
	rndv_req = (struct mrail_rndv_req *)&rndv_hdr[1];
	recv->rndv.context = (void *)rndv_hdr->context;
	recv->rndv.flags = comp->flags & FI_REMOTE_CQ_DATA;
	recv->rndv.len = rndv_req->len;
	recv->rndv.tag = mrail_pkt->hdr.tag;
	recv->rndv.data = comp->data;

	base_addrs = (uint64_t *)(rndv_req->rawkey + rndv_req->rawkey_size);
	for (offset = 0, i = 0; i < rndv_req->count; i++) {
		if (i < rndv_req->mr_count) {
			key_size = rndv_req->rma_iov[i].key;
			ret = fi_mr_map_raw(&mrail_ep->util_ep.domain->domain_fid,
					    base_addrs[i],
					    rndv_req->rawkey + offset,
					    key_size,
					    &rndv_req->rma_iov[i].key,
					    0);
			assert(!ret);
			offset += key_size;
		} else {
			rndv_req->rma_iov[i].key = rndv_req->rma_iov[0].key;
		}
	}

	rma_msg.msg_iov		= recv->iov + 1;
	rma_msg.desc		= recv->desc + 1;
	rma_msg.iov_count	= recv->count - 1;
	rma_msg.addr		= recv->addr;
	rma_msg.rma_iov		= rndv_req->rma_iov;
	rma_msg.rma_iov_count	= rndv_req->count;
	rma_msg.context		= recv;

	ret = fi_readmsg(&mrail_ep->util_ep.ep_fid, &rma_msg,
			 MRAIL_RNDV_FLAG | FI_COMPLETION);
	if (ret)
		retv = ret;

	ret = fi_recvmsg(recv_ctx->ep, &msg, FI_DISCARD);
	if (ret) {
		FI_WARN(&mrail_prov, FI_LOG_CQ,
			"Unable to discard buffered recv\n");
		retv = retv ? retv : ret;
	}

	return retv;
}

static int mrail_cq_process_rndv_ack(struct fi_cq_tagged_entry *comp)
{
	struct fi_recv_context *recv_ctx = comp->op_context;
	struct fi_msg msg = {
		.context = recv_ctx,
	};
	struct mrail_pkt *mrail_pkt;
	struct mrail_rndv_hdr *rndv_hdr;
	struct mrail_tx_buf *tx_buf;
	int ret, retv = 0;

	mrail_pkt = (struct mrail_pkt *)comp->buf;
	rndv_hdr = (struct mrail_rndv_hdr *)&mrail_pkt[1];
	tx_buf = (struct mrail_tx_buf *)rndv_hdr->context;
	ret = mrail_cq_write_send_comp(tx_buf->ep->util_ep.tx_cq, tx_buf);
	if (ret)
		retv = ret;

	ret = fi_recvmsg(recv_ctx->ep, &msg, FI_DISCARD);
	if (ret) {
		FI_WARN(&mrail_prov, FI_LOG_CQ,
			"Unable to discard buffered recv\n");
		retv = retv ? retv : ret;
	}

	return retv;
}

int mrail_cq_process_buf_recv(struct fi_cq_tagged_entry *comp,
			      struct mrail_recv *recv)
{
	struct fi_recv_context *recv_ctx = comp->op_context;
	struct fi_msg msg = {
		.context = recv_ctx,
	};
	struct mrail_ep *mrail_ep;
	struct mrail_pkt *mrail_pkt;
	size_t size, len;
	int ret, retv = 0;

	if (comp->flags & FI_MORE) {
		msg.msg_iov	= recv->iov;
		msg.iov_count	= recv->count;
		msg.addr	= recv->addr;

		recv_ctx->context = recv;

		ret = fi_recvmsg(recv_ctx->ep, &msg, FI_CLAIM);
		if (ret) {
			FI_WARN(&mrail_prov, FI_LOG_CQ,
				"Unable to claim buffered recv\n");
			assert(0);
			// TODO write cq error entry
		}
		return ret;
	}

	mrail_ep = recv_ctx->ep->fid.context;
	mrail_pkt = (struct mrail_pkt *)comp->buf;

	if (mrail_pkt->hdr.protocol == MRAIL_PROTO_RNDV)
		return mrail_cq_process_rndv_req(comp, recv);

	len = comp->len - sizeof(*mrail_pkt);

	size = ofi_copy_to_iov(&recv->iov[1], recv->count - 1, 0,
			       mrail_pkt->data, len);

	if (size < len) {
		FI_WARN(&mrail_prov, FI_LOG_CQ, "Message truncated recv buf "
			"size: %zu message length: %zu\n", size, len);
		ret = ofi_cq_write_error_trunc(
			mrail_ep->util_ep.rx_cq, recv->context,
			recv->comp_flags | (comp->flags & FI_REMOTE_CQ_DATA),
			0, NULL, comp->data, mrail_pkt->hdr.tag, comp->len - size);
		if (ret) {
			FI_WARN(&mrail_prov, FI_LOG_CQ,
				"Unable to write truncation error to util cq\n");
			retv = ret;
		}
		mrail_cntr_incerr(mrail_ep->util_ep.cntrs[CNTR_RX]);
		goto out;
	}
	ret = mrail_cq_write_recv_comp(mrail_ep, &mrail_pkt->hdr, comp, recv);
	if (ret)
		retv = ret;
out:
	ret = fi_recvmsg(recv_ctx->ep, &msg, FI_DISCARD);
	if (ret) {
		FI_WARN(&mrail_prov, FI_LOG_CQ,
			"Unable to discard buffered recv\n");
		retv = retv ? retv : ret;
	}
	mrail_push_recv(recv);
	return retv;
}

/* Should only be called while holding the EP's lock */
static struct mrail_recv *mrail_match_recv(struct mrail_ep *mrail_ep,
					   struct fi_cq_tagged_entry *comp,
					   int src_addr)
{
	struct mrail_hdr *hdr = comp->buf;
	struct mrail_recv *recv;

	if (hdr->op == ofi_op_msg) {
		FI_DBG(&mrail_prov, FI_LOG_CQ, "Got MSG op\n");
		recv = mrail_match_recv_handle_unexp(&mrail_ep->recv_queue, 0,
						     src_addr, (char *)comp,
						     sizeof(*comp), NULL);
	} else {
		assert(hdr->op == ofi_op_tagged);
		FI_DBG(&mrail_prov, FI_LOG_CQ, "Got TAGGED op with tag: 0x%"
		       PRIx64 "\n", hdr->tag);
		recv = mrail_match_recv_handle_unexp(&mrail_ep->trecv_queue,
						     hdr->tag, src_addr,
						     (char *)comp,
						     sizeof(*comp), NULL);
	}

	return recv;
}

static
struct mrail_ooo_recv *mrail_get_next_recv(struct mrail_peer_info *peer_info)
{
	struct slist *queue = &peer_info->ooo_recv_queue;
	struct mrail_ooo_recv *ooo_recv;

	if (!slist_empty(queue)) {
		ooo_recv = container_of(queue->head, struct mrail_ooo_recv,
				entry);
		if (ooo_recv->seq_no == peer_info->expected_seq_no) {
			slist_remove_head(queue);
			peer_info->expected_seq_no++;
			return ooo_recv;
		}
	}
	return NULL;
}

static int mrail_process_ooo_recvs(struct mrail_ep *mrail_ep,
				   struct mrail_peer_info *peer_info)
{
	struct mrail_ooo_recv *ooo_recv;
	struct mrail_recv *recv;
	int ret;

	ofi_genlock_lock(&mrail_ep->util_ep.lock);
	ooo_recv = mrail_get_next_recv(peer_info);
	while (ooo_recv) {
		FI_DBG(&mrail_prov, FI_LOG_CQ, "found ooo_recv seq=%d\n",
				ooo_recv->seq_no);
		/* Requesting FI_AV_TABLE from the underlying provider allows
		 * us to use peer_info->addr as an int here. */
		recv = mrail_match_recv(mrail_ep, &ooo_recv->comp,
				(int) peer_info->addr);
		ofi_genlock_unlock(&mrail_ep->util_ep.lock);

		if (recv) {
			ret = mrail_cq_process_buf_recv(&ooo_recv->comp, recv);
			if (ret)
				return ret;
		}

		ofi_genlock_lock(&mrail_ep->util_ep.lock);
		ofi_buf_free(ooo_recv);
		ooo_recv = mrail_get_next_recv(peer_info);
	}
	ofi_genlock_unlock(&mrail_ep->util_ep.lock);
	return 0;
}

static int mrail_ooo_recv_before(struct slist_entry *item, const void *arg)
{
	struct mrail_ooo_recv *ooo_recv;
	struct mrail_ooo_recv *new_recv;

	ooo_recv = container_of(item, struct mrail_ooo_recv, entry);
	new_recv = container_of(arg, struct mrail_ooo_recv, entry);
	return (new_recv->seq_no < ooo_recv->seq_no);
}

/* Should only be called while holding the EP's lock */
static void mrail_save_ooo_recv(struct mrail_ep *mrail_ep,
				struct mrail_peer_info *peer_info,
				uint32_t seq_no,
				struct fi_cq_tagged_entry *comp)
{
	struct slist *queue = &peer_info->ooo_recv_queue;
	struct mrail_ooo_recv *ooo_recv;

	ooo_recv = ofi_buf_alloc(mrail_ep->ooo_recv_pool);
	if (!ooo_recv) {
		FI_WARN(&mrail_prov, FI_LOG_CQ, "Cannot allocate ooo_recv\n");
		assert(0);
		return;
	}
	ooo_recv->entry.next = NULL;
	ooo_recv->seq_no = seq_no;
	memcpy(&ooo_recv->comp, comp, sizeof(*comp));

	slist_insert_before_first_match(queue, mrail_ooo_recv_before,
					&ooo_recv->entry);

	FI_DBG(&mrail_prov, FI_LOG_CQ, "saved ooo_recv seq=%d\n", seq_no);
}

static int mrail_handle_recv_completion(struct fi_cq_tagged_entry *comp,
					fi_addr_t src_addr)
{
	struct fi_recv_context *recv_ctx;
	struct mrail_peer_info *peer_info;
	struct mrail_ep *mrail_ep;
	struct mrail_recv *recv;
	struct mrail_hdr *hdr;
	uint32_t seq_no;
	int ret;

	if (comp->flags & FI_CLAIM) {
		/* This message has already been processed and claimed.
		 * See mrail_cq_process_buf_recv().
		 */
		recv = comp->op_context;
		assert(recv->hdr.version == MRAIL_HDR_VERSION);
		ret =  mrail_cq_write_recv_comp(recv->ep, &recv->hdr, comp,
						recv);
		mrail_push_recv(recv);
		goto exit;
	}

	recv_ctx = comp->op_context;
	mrail_ep = recv_ctx->ep->fid.context;
	hdr = comp->buf;

	// TODO make rxm send buffered recv amount of data for large message
	assert(hdr->version == MRAIL_HDR_VERSION);

	if (hdr->protocol == MRAIL_PROTO_RNDV &&
	    hdr->protocol_cmd == MRAIL_RNDV_ACK)
		return mrail_cq_process_rndv_ack(comp);

	seq_no = ntohl(hdr->seq);
	peer_info = ofi_av_get_addr(mrail_ep->util_ep.av, (int) src_addr);
	FI_DBG(&mrail_prov, FI_LOG_CQ,
			"ep=%p peer=%d received seq=%d, expected=%d\n",
			mrail_ep, (int)peer_info->addr, seq_no,
			peer_info->expected_seq_no);
	ofi_genlock_lock(&mrail_ep->util_ep.lock);
	if (seq_no == peer_info->expected_seq_no) {
		/* This message was received in order */
		peer_info->expected_seq_no++;
		/* Requesting FI_AV_TABLE from the underlying provider allows
		 * us to use src_addr as an int here. */
		recv = mrail_match_recv(mrail_ep, comp, (int) src_addr);
		ofi_genlock_unlock(&mrail_ep->util_ep.lock);

		if (recv) {
			ret = mrail_cq_process_buf_recv(comp, recv);
			if (ret)
				goto exit;
		}

		/* Process any next-in-order message that had already arrived */
		ret = mrail_process_ooo_recvs(mrail_ep, peer_info);
	} else {
		/* This message was received early.
		 * Save it into the out-of-order recv queue.
		 */
		mrail_save_ooo_recv(mrail_ep, peer_info, seq_no, comp);
		ofi_genlock_unlock(&mrail_ep->util_ep.lock);
		ret = 0;
	}
exit:
	return ret;
}

static int mrail_cq_close(fid_t fid)
{
	struct mrail_cq *mrail_cq = container_of(fid, struct mrail_cq, util_cq.cq_fid.fid);
	int ret, retv = 0;

	ret = mrail_close_fids((struct fid **)mrail_cq->cqs,
			       mrail_cq->num_cqs);
	if (ret)
		retv = ret;
	free(mrail_cq->cqs);

	ret = ofi_cq_cleanup(&mrail_cq->util_cq);
	if (ret)
		retv = retv ? retv : ret;

	free(mrail_cq);
	return retv;
}

static struct fi_ops mrail_cq_fi_ops = {
	.size = sizeof(struct fi_ops),
	.close = mrail_cq_close,
	.bind = fi_no_bind,
	.control = fi_no_control,
	.ops_open = fi_no_ops_open,
};

static struct fi_ops_cq mrail_cq_ops = {
	.size = sizeof(struct fi_ops_cq),
	.read = ofi_cq_read,
	.readfrom = ofi_cq_readfrom,
	.readerr = ofi_cq_readerr,
	.sread = ofi_cq_sread,
	.sreadfrom = ofi_cq_sreadfrom,
	.signal = ofi_cq_signal,
	// TODO define cq strerror, may need to pass rail index
	// in err_data
	.strerror = fi_no_cq_strerror,
};

static void mrail_handle_rma_completion(struct util_cq *cq,
		struct fi_cq_tagged_entry *comp)
{
	int ret;
	struct mrail_req *req;
	struct mrail_subreq *subreq;

	subreq = comp->op_context;
	req = subreq->parent;

	if (ofi_atomic_dec32(&req->expected_subcomps) == 0) {
		if (req->comp.flags & MRAIL_RNDV_FLAG) {
			mrail_finish_rndv_recv(cq, req, comp);
			return;
		}

		ret = ofi_cq_write(cq, req->comp.op_context, req->comp.flags,
				req->comp.len, req->comp.buf, req->comp.data,
				req->comp.tag);
		if (ret) {
			FI_WARN(&mrail_prov, FI_LOG_CQ,
				"Cannot write to util cq\n");
			/* This should not happen unless totally out of memory,
			 * in which case there is nothing we can do.  */
			assert(0);
		}

		if (comp->flags & FI_WRITE)
			ofi_ep_cntr_inc(&req->mrail_ep->util_ep, CNTR_WR);
		else
			ofi_ep_cntr_inc(&req->mrail_ep->util_ep, CNTR_RD);

		mrail_free_req(req->mrail_ep, req);
	}
}

void mrail_poll_cq(struct util_cq *cq)
{
	struct mrail_cq *mrail_cq;
	struct mrail_tx_buf *tx_buf;
	struct fi_cq_tagged_entry comp;
	fi_addr_t src_addr;
	size_t i, idx;
	int ret;
	static int last_succ_rail = 0;

	mrail_cq = container_of(cq, struct mrail_cq, util_cq);

	for (i = 0; i < mrail_cq->num_cqs;) {
		idx = (last_succ_rail + i) % mrail_cq->num_cqs;
		ret = fi_cq_readfrom(mrail_cq->cqs[idx], &comp, 1, &src_addr);
		if (ret == -FI_EAGAIN || !ret) {
			i++;
			continue;
		}
		if (ret < 0) {
			FI_WARN(&mrail_prov, FI_LOG_CQ,
				"Unable to read rail completion: %s\n",
				fi_strerror(-ret));
			goto err;
		}
		// TODO handle variable length message
		if (comp.flags & FI_RECV) {
			ret = mrail_cq->process_comp(&comp, src_addr);
			if (ret)
				goto err;
		} else if (comp.flags & (FI_READ | FI_WRITE)) {
			mrail_handle_rma_completion(cq, &comp);
		} else if (comp.flags & FI_SEND) {
			tx_buf = comp.op_context;
			if (tx_buf->hdr.protocol == MRAIL_PROTO_RNDV) {
				if (tx_buf->hdr.protocol_cmd == MRAIL_RNDV_REQ) {
					/* buf will be freed when ACK comes */
				} else if (tx_buf->hdr.protocol_cmd == MRAIL_RNDV_ACK) {
					ofi_genlock_lock(&tx_buf->ep->util_ep.lock);
					ofi_buf_free(tx_buf);
					ofi_genlock_unlock(&tx_buf->ep->util_ep.lock);
				}
			} else {
				ret = mrail_cq_write_send_comp(cq, tx_buf);
				if (ret)
					goto err;
			}
		} else {
			/* We currently cannot support FI_REMOTE_READ and
			 * FI_REMOTE_WRITE because RMA operations are split
			 * across all rails. We would need to introduce some
			 * sort of protocol to keep track of remotely-initiated
			 * RMA operations. */
			assert(comp.flags & (FI_REMOTE_READ | FI_REMOTE_WRITE));
			FI_WARN(&mrail_prov, FI_LOG_CQ,
				"Unsupported completion flag\n");
		}

		last_succ_rail = idx;
		if (mrail_config[0].policy == MRAIL_POLICY_FIXED)
			break;
	}

	return;

err:
	// TODO write error to cq
	assert(0);
}

static void mrail_cq_progress(struct util_cq *cq)
{
	mrail_poll_cq(cq);

	/* Progress the bound EPs */
	ofi_cq_progress(cq);
}

int mrail_cq_open(struct fid_domain *domain, struct fi_cq_attr *attr,
		   struct fid_cq **cq_fid, void *context)
{
	struct mrail_domain *mrail_domain;
	struct mrail_cq *mrail_cq;
	struct fi_cq_attr rail_cq_attr = {
		.wait_obj = FI_WAIT_NONE,
		.format = MRAIL_RAIL_CQ_FORMAT,
		.size = attr->size,
	};
	size_t i;
	int ret;

	mrail_cq = calloc(1, sizeof(*mrail_cq));
	if (!mrail_cq)
		return -FI_ENOMEM;

	ret = ofi_cq_init(&mrail_prov, domain, attr, &mrail_cq->util_cq,
			  &mrail_cq_progress, context);
	if (ret) {
		free(mrail_cq);
		return ret;
	}

	mrail_domain = container_of(domain, struct mrail_domain,
				    util_domain.domain_fid);

	mrail_cq->cqs = calloc(mrail_domain->num_domains,
			       sizeof(*mrail_cq->cqs));
	if (!mrail_cq->cqs)
		goto err;

	mrail_cq->num_cqs = mrail_domain->num_domains;

	for (i = 0; i < mrail_cq->num_cqs; i++) {
		ret = fi_cq_open(mrail_domain->domains[i], &rail_cq_attr,
				 &mrail_cq->cqs[i], NULL);
		if (ret) {
			FI_WARN(&mrail_prov, FI_LOG_EP_CTRL,
				"Unable to open rail CQ\n");
			goto err;
		}
	}

	// TODO add regular process comp when FI_BUFFERED_RECV not set
	mrail_cq->process_comp = mrail_handle_recv_completion;

	*cq_fid = &mrail_cq->util_cq.cq_fid;
	(*cq_fid)->fid.ops = &mrail_cq_fi_ops;
	(*cq_fid)->ops = &mrail_cq_ops;

	return 0;
err:
	mrail_cq_close(&mrail_cq->util_cq.cq_fid.fid);
	return ret;
}
