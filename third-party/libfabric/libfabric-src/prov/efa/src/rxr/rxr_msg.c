/*
 * Copyright (c) 2019-2020 Amazon.com, Inc. or its affiliates.
 * All rights reserved.
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

#include <inttypes.h>
#include <stdlib.h>
#include <string.h>
#include "ofi.h"
#include <ofi_util.h>
#include <ofi_iov.h>

#include "efa.h"
#include "rxr.h"
#include "rxr_msg.h"
#include "rxr_pkt_cmd.h"

/**
 * This file define the msg ops functions.
 * It is consisted of the following sections:
 *     send functions,
 *     receive functions and
 *     ops structure
 */

/**
 *  Send function
 */

/**
 *   Utility functions used by both non-tagged and tagged send.
 */
ssize_t rxr_msg_post_rtm(struct rxr_ep *rxr_ep, struct rxr_tx_entry *tx_entry)
{
	/*
	 * For performance consideration, this function assume the tagged rtm packet type id is
	 * always the correspondent message rtm packet type id + 1, thus the assertion here.
	 */
	assert(RXR_EAGER_MSGRTM_PKT + 1 == RXR_EAGER_TAGRTM_PKT);
	assert(RXR_READ_MSGRTM_PKT + 1 == RXR_READ_TAGRTM_PKT);
	assert(RXR_LONG_MSGRTM_PKT + 1 == RXR_LONG_TAGRTM_PKT);
	assert(RXR_MEDIUM_MSGRTM_PKT + 1 == RXR_MEDIUM_TAGRTM_PKT);

	int tagged;
	size_t max_rtm_data_size;
	ssize_t err;
	struct rxr_peer *peer;

	assert(tx_entry->op == ofi_op_msg || tx_entry->op == ofi_op_tagged);
	tagged = (tx_entry->op == ofi_op_tagged);
	assert(tagged == 0 || tagged == 1);

	max_rtm_data_size = rxr_pkt_req_max_data_size(rxr_ep,
						      tx_entry->addr,
						      RXR_EAGER_MSGRTM_PKT + tagged);

	peer = rxr_ep_get_peer(rxr_ep, tx_entry->addr);

	if (peer->is_local) {
		assert(rxr_ep->use_shm);
		/* intra instance message */
		int rtm_type = (tx_entry->total_len <= max_rtm_data_size) ? RXR_EAGER_MSGRTM_PKT
									  : RXR_READ_MSGRTM_PKT;

		return rxr_pkt_post_ctrl_or_queue(rxr_ep, RXR_TX_ENTRY, tx_entry, rtm_type + tagged, 0);
	}

	/* inter instance message */
	if (tx_entry->total_len <= max_rtm_data_size)
		return rxr_pkt_post_ctrl_or_queue(rxr_ep, RXR_TX_ENTRY, tx_entry,
						  RXR_EAGER_MSGRTM_PKT + tagged, 0);

	if (tx_entry->total_len <= rxr_env.efa_max_medium_msg_size) {
		/* we do not check the return value of rxr_ep_init_mr_desc()
		 * because medium message works even if MR registration failed
		 */
		if (efa_mr_cache_enable)
			rxr_ep_tx_init_mr_desc(rxr_ep_domain(rxr_ep),
					       tx_entry, 0, FI_SEND);
		return rxr_pkt_post_ctrl_or_queue(rxr_ep, RXR_TX_ENTRY, tx_entry,
						  RXR_MEDIUM_MSGRTM_PKT + tagged, 0);
	}

	if (efa_both_support_rdma_read(rxr_ep, peer) &&
	    (tx_entry->desc[0] || efa_mr_cache_enable)) {
		/* use read message protocol */
		err = rxr_pkt_post_ctrl_or_queue(rxr_ep, RXR_TX_ENTRY, tx_entry,
						 RXR_READ_MSGRTM_PKT + tagged, 0);

		if (err != -FI_ENOMEM)
			return err;

		/*
		 * If memory registration failed, we continue here
		 * and fall back to use long message protocol
		 */
	}

	err = rxr_ep_set_tx_credit_request(rxr_ep, tx_entry);
	if (OFI_UNLIKELY(err))
		return err;

	return rxr_pkt_post_ctrl_or_queue(rxr_ep, RXR_TX_ENTRY, tx_entry,
					  RXR_LONG_MSGRTM_PKT + tagged, 0);
}

ssize_t rxr_msg_generic_send(struct fid_ep *ep, const struct fi_msg *msg,
			     uint64_t tag, uint32_t op, uint64_t flags)
{
	struct rxr_ep *rxr_ep;
	ssize_t err;
	struct rxr_tx_entry *tx_entry;
	struct rxr_peer *peer;

	FI_DBG(&rxr_prov, FI_LOG_EP_DATA,
	       "iov_len: %lu tag: %lx op: %x flags: %lx\n",
	       ofi_total_iov_len(msg->msg_iov, msg->iov_count),
	       tag, op, flags);

	rxr_ep = container_of(ep, struct rxr_ep, util_ep.ep_fid.fid);
	assert(msg->iov_count <= rxr_ep->tx_iov_limit);

	rxr_perfset_start(rxr_ep, perf_rxr_tx);
	fastlock_acquire(&rxr_ep->util_ep.lock);

	if (OFI_UNLIKELY(is_tx_res_full(rxr_ep))) {
		err = -FI_EAGAIN;
		goto out;
	}

	tx_entry = rxr_ep_alloc_tx_entry(rxr_ep, msg, op, tag, flags);

	if (OFI_UNLIKELY(!tx_entry)) {
		err = -FI_EAGAIN;
		rxr_ep_progress_internal(rxr_ep);
		goto out;
	}

	assert(tx_entry->op == ofi_op_msg || tx_entry->op == ofi_op_tagged);

	peer = rxr_ep_get_peer(rxr_ep, tx_entry->addr);
	assert(peer);
	tx_entry->msg_id = peer->next_msg_id++;
	err = rxr_msg_post_rtm(rxr_ep, tx_entry);
	if (OFI_UNLIKELY(err)) {
		rxr_release_tx_entry(rxr_ep, tx_entry);
		peer->next_msg_id--;
	}

out:
	fastlock_release(&rxr_ep->util_ep.lock);
	rxr_perfset_end(rxr_ep, perf_rxr_tx);
	return err;
}

/**
 *   Non-tagged send ops function
 */
static
ssize_t rxr_msg_sendmsg(struct fid_ep *ep, const struct fi_msg *msg,
			uint64_t flags)
{
	return rxr_msg_generic_send(ep, msg, 0, ofi_op_msg, flags);
}

static
ssize_t rxr_msg_sendv(struct fid_ep *ep, const struct iovec *iov,
		      void **desc, size_t count, fi_addr_t dest_addr,
		      void *context)
{
	struct rxr_ep *rxr_ep;
	struct fi_msg msg;

	memset(&msg, 0, sizeof(msg));
	msg.msg_iov = iov;
	msg.desc = desc;
	msg.iov_count = count;
	msg.addr = dest_addr;
	msg.context = context;

	rxr_ep = container_of(ep, struct rxr_ep, util_ep.ep_fid.fid);
	return rxr_msg_sendmsg(ep, &msg, rxr_tx_flags(rxr_ep));
}

static
ssize_t rxr_msg_send(struct fid_ep *ep, const void *buf, size_t len,
		     void *desc, fi_addr_t dest_addr, void *context)
{
	struct iovec iov;

	iov.iov_base = (void *)buf;
	iov.iov_len = len;
	return rxr_msg_sendv(ep, &iov, desc, 1, dest_addr, context);
}

static
ssize_t rxr_msg_senddata(struct fid_ep *ep, const void *buf, size_t len,
			 void *desc, uint64_t data, fi_addr_t dest_addr,
			 void *context)
{
	struct fi_msg msg;
	struct iovec iov;
	struct rxr_ep *rxr_ep;

	iov.iov_base = (void *)buf;
	iov.iov_len = len;

	memset(&msg, 0, sizeof(msg));
	msg.msg_iov = &iov;
	msg.desc = desc;
	msg.iov_count = 1;
	msg.addr = dest_addr;
	msg.context = context;
	msg.data = data;

	rxr_ep = container_of(ep, struct rxr_ep, util_ep.ep_fid.fid);
	return rxr_msg_generic_send(ep, &msg, 0, ofi_op_msg,
				    rxr_tx_flags(rxr_ep) | FI_REMOTE_CQ_DATA);
}

static
ssize_t rxr_msg_inject(struct fid_ep *ep, const void *buf, size_t len,
		       fi_addr_t dest_addr)
{
	struct rxr_ep *rxr_ep;
	struct fi_msg msg;
	struct iovec iov;

	iov.iov_base = (void *)buf;
	iov.iov_len = len;

	memset(&msg, 0, sizeof(msg));
	msg.msg_iov = &iov;
	msg.iov_count = 1;
	msg.addr = dest_addr;

	rxr_ep = container_of(ep, struct rxr_ep, util_ep.ep_fid.fid);
	assert(len <= rxr_ep->core_inject_size - sizeof(struct rxr_eager_msgrtm_hdr));

	return rxr_msg_generic_send(ep, &msg, 0, ofi_op_msg,
				    rxr_tx_flags(rxr_ep) | RXR_NO_COMPLETION | FI_INJECT);
}

static
ssize_t rxr_msg_injectdata(struct fid_ep *ep, const void *buf,
			   size_t len, uint64_t data,
			   fi_addr_t dest_addr)
{
	struct rxr_ep *rxr_ep;
	struct fi_msg msg;
	struct iovec iov;

	iov.iov_base = (void *)buf;
	iov.iov_len = len;

	memset(&msg, 0, sizeof(msg));
	msg.msg_iov = &iov;
	msg.iov_count = 1;
	msg.addr = dest_addr;
	msg.data = data;

	rxr_ep = container_of(ep, struct rxr_ep, util_ep.ep_fid.fid);
	/*
	 * We advertise the largest possible inject size with no cq data or
	 * source address. This means that we may end up not using the core
	 * providers inject for this send.
	 */
	assert(len <= rxr_ep->core_inject_size - sizeof(struct rxr_eager_msgrtm_hdr));
	return rxr_msg_generic_send(ep, &msg, 0, ofi_op_msg,
				    rxr_tx_flags(rxr_ep) | RXR_NO_COMPLETION |
				    FI_REMOTE_CQ_DATA | FI_INJECT);
}

/**
 *   Tagged send op functions
 */
static
ssize_t rxr_msg_tsendmsg(struct fid_ep *ep_fid, const struct fi_msg_tagged *tmsg,
			 uint64_t flags)
{
	struct fi_msg msg;

	msg.msg_iov = tmsg->msg_iov;
	msg.desc = tmsg->desc;
	msg.iov_count = tmsg->iov_count;
	msg.addr = tmsg->addr;
	msg.context = tmsg->context;
	msg.data = tmsg->data;
	return rxr_msg_generic_send(ep_fid, &msg, tmsg->tag, ofi_op_tagged, flags);
}

static
ssize_t rxr_msg_tsendv(struct fid_ep *ep_fid, const struct iovec *iov,
		       void **desc, size_t count, fi_addr_t dest_addr,
		       uint64_t tag, void *context)
{
	struct rxr_ep *rxr_ep;
	struct fi_msg_tagged msg;

	memset(&msg, 0, sizeof(msg));
	msg.msg_iov = iov;
	msg.desc = desc;
	msg.iov_count = count;
	msg.addr = dest_addr;
	msg.context = context;
	msg.tag = tag;

	rxr_ep = container_of(ep_fid, struct rxr_ep, util_ep.ep_fid.fid);
	return rxr_msg_tsendmsg(ep_fid, &msg, rxr_tx_flags(rxr_ep));
}

static
ssize_t rxr_msg_tsend(struct fid_ep *ep_fid, const void *buf, size_t len,
		      void *desc, fi_addr_t dest_addr, uint64_t tag,
		      void *context)
{
	struct iovec msg_iov;

	msg_iov.iov_base = (void *)buf;
	msg_iov.iov_len = len;
	return rxr_msg_tsendv(ep_fid, &msg_iov, &desc, 1, dest_addr, tag,
			      context);
}

static
ssize_t rxr_msg_tsenddata(struct fid_ep *ep_fid, const void *buf, size_t len,
			  void *desc, uint64_t data, fi_addr_t dest_addr,
			  uint64_t tag, void *context)
{
	struct fi_msg msg;
	struct iovec iov;
	struct rxr_ep *rxr_ep;

	iov.iov_base = (void *)buf;
	iov.iov_len = len;

	msg.msg_iov = &iov;
	msg.desc = desc;
	msg.iov_count = 1;
	msg.addr = dest_addr;
	msg.context = context;
	msg.data = data;

	rxr_ep = container_of(ep_fid, struct rxr_ep, util_ep.ep_fid.fid);
	return rxr_msg_generic_send(ep_fid, &msg, tag, ofi_op_tagged,
				    rxr_tx_flags(rxr_ep) | FI_REMOTE_CQ_DATA);
}

static
ssize_t rxr_msg_tinject(struct fid_ep *ep_fid, const void *buf, size_t len,
			fi_addr_t dest_addr, uint64_t tag)
{
	struct rxr_ep *rxr_ep;
	struct fi_msg msg;
	struct iovec iov;

	iov.iov_base = (void *)buf;
	iov.iov_len = len;

	memset(&msg, 0, sizeof(msg));
	msg.msg_iov = &iov;
	msg.iov_count = 1;
	msg.addr = dest_addr;

	rxr_ep = container_of(ep_fid, struct rxr_ep, util_ep.ep_fid.fid);
	assert(len <= rxr_ep->core_inject_size - sizeof(struct rxr_eager_tagrtm_hdr));

	return rxr_msg_generic_send(ep_fid, &msg, tag, ofi_op_tagged,
				    rxr_tx_flags(rxr_ep) | RXR_NO_COMPLETION | FI_INJECT);
}

static
ssize_t rxr_msg_tinjectdata(struct fid_ep *ep_fid, const void *buf, size_t len,
			    uint64_t data, fi_addr_t dest_addr, uint64_t tag)
{
	struct rxr_ep *rxr_ep;
	struct fi_msg msg;
	struct iovec iov;

	iov.iov_base = (void *)buf;
	iov.iov_len = len;

	memset(&msg, 0, sizeof(msg));
	msg.msg_iov = &iov;
	msg.iov_count = 1;
	msg.addr = dest_addr;
	msg.data = data;

	rxr_ep = container_of(ep_fid, struct rxr_ep, util_ep.ep_fid.fid);
	/*
	 * We advertise the largest possible inject size with no cq data or
	 * source address. This means that we may end up not using the core
	 * providers inject for this send.
	 */
	assert(len <= rxr_ep->core_inject_size - sizeof(struct rxr_eager_tagrtm_hdr));

	return rxr_msg_generic_send(ep_fid, &msg, tag, ofi_op_tagged,
				    rxr_tx_flags(rxr_ep) | RXR_NO_COMPLETION |
				    FI_REMOTE_CQ_DATA | FI_INJECT);
}

/**
 *  Receive functions
 */

/**
 *   Utility functions and data structures
 */
struct rxr_match_info {
	fi_addr_t addr;
	uint64_t tag;
	uint64_t ignore;
};

static
int rxr_msg_match_unexp_anyaddr(struct dlist_entry *item, const void *arg)
{
	return 1;
}

static
int rxr_msg_match_unexp(struct dlist_entry *item, const void *arg)
{
	const struct rxr_match_info *match_info = arg;
	struct rxr_rx_entry *rx_entry;

	rx_entry = container_of(item, struct rxr_rx_entry, entry);

	return rxr_match_addr(match_info->addr, rx_entry->addr);
}

static
int rxr_msg_match_unexp_tagged_anyaddr(struct dlist_entry *item, const void *arg)
{
	const struct rxr_match_info *match_info = arg;
	struct rxr_rx_entry *rx_entry;

	rx_entry = container_of(item, struct rxr_rx_entry, entry);

	return rxr_match_tag(rx_entry->tag, match_info->ignore,
			     match_info->tag);
}

static
int rxr_msg_match_unexp_tagged(struct dlist_entry *item, const void *arg)
{
	const struct rxr_match_info *match_info = arg;
	struct rxr_rx_entry *rx_entry;

	rx_entry = container_of(item, struct rxr_rx_entry, entry);

	return rxr_match_addr(match_info->addr, rx_entry->addr) &&
	       rxr_match_tag(rx_entry->tag, match_info->ignore,
			     match_info->tag);
}

static
int rxr_msg_handle_unexp_match(struct rxr_ep *ep,
			       struct rxr_rx_entry *rx_entry,
			       uint64_t tag, uint64_t ignore,
			       void *context, fi_addr_t addr,
			       uint32_t op, uint64_t flags)
{
	struct rxr_pkt_entry *pkt_entry;
	uint64_t data_len;

	rx_entry->fi_flags = flags;
	rx_entry->ignore = ignore;
	rx_entry->state = RXR_RX_MATCHED;

	pkt_entry = rx_entry->unexp_pkt;
	rx_entry->unexp_pkt = NULL;
	data_len = rxr_pkt_rtm_total_len(pkt_entry);

	rx_entry->cq_entry.op_context = context;
	/*
	 * we don't expect recv buf from application for discard,
	 * hence setting to NULL
	 */
	if (OFI_UNLIKELY(flags & FI_DISCARD)) {
		rx_entry->cq_entry.buf = NULL;
		rx_entry->cq_entry.len = data_len;
	} else {
		rx_entry->cq_entry.buf = rx_entry->iov[0].iov_base;
		data_len = MIN(rx_entry->total_len,
			       ofi_total_iov_len(rx_entry->iov, rx_entry->iov_count));
		rx_entry->cq_entry.len = data_len;
	}

	rx_entry->cq_entry.flags = (FI_RECV | FI_MSG);

	if (op == ofi_op_tagged) {
		rx_entry->cq_entry.flags |= FI_TAGGED;
		rx_entry->cq_entry.tag = rx_entry->tag;
		rx_entry->ignore = ignore;
	} else {
		rx_entry->cq_entry.tag = 0;
		rx_entry->ignore = ~0;
	}

	return rxr_pkt_proc_matched_rtm(ep, rx_entry, pkt_entry);
}

/*
 *    Search unexpected list for matching message and process it if found.
 *    Returns 0 if the message is processed, -FI_ENOMSG if no match is found.
 */
static
int rxr_msg_proc_unexp_msg_list(struct rxr_ep *ep, const struct fi_msg *msg,
				uint64_t tag, uint64_t ignore, uint32_t op, uint64_t flags,
				struct rxr_rx_entry *posted_entry)
{
	struct rxr_match_info match_info;
	struct dlist_entry *match;
	struct rxr_rx_entry *rx_entry;
	dlist_func_t *match_func;
	int ret;

	if (op == ofi_op_tagged) {
		if (ep->util_ep.caps & FI_DIRECTED_RECV)
			match_func = &rxr_msg_match_unexp_tagged;
		else
			match_func = &rxr_msg_match_unexp_tagged_anyaddr;

		match_info.addr = msg->addr;
		match_info.tag = tag;
		match_info.ignore = ignore;
		match = dlist_remove_first_match(&ep->rx_unexp_tagged_list,
		                                 match_func,
						 (void *)&match_info);
	} else {
		if (ep->util_ep.caps & FI_DIRECTED_RECV)
			match_func = &rxr_msg_match_unexp;
		else
			match_func = &rxr_msg_match_unexp_anyaddr;

		match_info.addr = msg->addr;
		match = dlist_remove_first_match(&ep->rx_unexp_list,
		                                 match_func,
						 (void *)&match_info);
	}

	if (!match)
		return -FI_ENOMSG;

	rx_entry = container_of(match, struct rxr_rx_entry, entry);

	/*
	 * Initialize the matched entry as a multi-recv consumer if the posted
	 * buffer is a multi-recv buffer.
	 */
	if (posted_entry) {
		/*
		 * rxr_ep_split_rx_entry will setup rx_entry iov and count
		 */
		rx_entry = rxr_ep_split_rx_entry(ep, posted_entry, rx_entry,
						 rx_entry->unexp_pkt);
		if (OFI_UNLIKELY(!rx_entry)) {
			FI_WARN(&rxr_prov, FI_LOG_CQ,
				"RX entries exhausted.\n");
			return -FI_ENOBUFS;
		}
	} else {
		memcpy(rx_entry->iov, msg->msg_iov, sizeof(*rx_entry->iov) * msg->iov_count);
		rx_entry->iov_count = msg->iov_count;
	}

	if (msg->desc)
		memcpy(rx_entry->desc, msg->desc, sizeof(void*) * msg->iov_count);

	FI_DBG(&rxr_prov, FI_LOG_EP_CTRL,
	       "Match found in unexp list for a posted recv msg_id: %" PRIu32
	       " total_len: %" PRIu64 " tag: %lx\n",
	       rx_entry->msg_id, rx_entry->total_len, rx_entry->tag);

	ret = rxr_msg_handle_unexp_match(ep, rx_entry, tag, ignore,
					 msg->context, msg->addr, op, flags);
	return ret;
}

bool rxr_msg_multi_recv_buffer_available(struct rxr_ep *ep,
					 struct rxr_rx_entry *rx_entry)
{
	assert(rx_entry->fi_flags & FI_MULTI_RECV);
	assert(rx_entry->rxr_flags & RXR_MULTI_RECV_POSTED);

	return (ofi_total_iov_len(rx_entry->iov, rx_entry->iov_count)
		>= ep->min_multi_recv_size);
}

static inline
bool rxr_msg_multi_recv_buffer_complete(struct rxr_ep *ep,
					struct rxr_rx_entry *rx_entry)
{
	assert(rx_entry->fi_flags & FI_MULTI_RECV);
	assert(rx_entry->rxr_flags & RXR_MULTI_RECV_POSTED);

	return (!rxr_msg_multi_recv_buffer_available(ep, rx_entry) &&
		dlist_empty(&rx_entry->multi_recv_consumers));
}

void rxr_msg_multi_recv_free_posted_entry(struct rxr_ep *ep,
					  struct rxr_rx_entry *rx_entry)
{
	assert(!(rx_entry->rxr_flags & RXR_MULTI_RECV_POSTED));

	if ((rx_entry->rxr_flags & RXR_MULTI_RECV_CONSUMER) &&
	    rxr_msg_multi_recv_buffer_complete(ep, rx_entry->master_entry))
		rxr_release_rx_entry(ep, rx_entry->master_entry);
}

static
ssize_t rxr_msg_multi_recv(struct rxr_ep *rxr_ep, const struct fi_msg *msg,
			   uint64_t tag, uint64_t ignore, uint32_t op, uint64_t flags)
{
	struct rxr_rx_entry *rx_entry;
	int ret = 0;

	if ((ofi_total_iov_len(msg->msg_iov, msg->iov_count)
	     < rxr_ep->min_multi_recv_size) || op != ofi_op_msg)
		return -FI_EINVAL;

	/*
	 * Always get new rx_entry of type RXR_MULTI_RECV_POSTED when in the
	 * multi recv path. The posted entry will not be used for receiving
	 * messages but will be used for tracking the application's buffer and
	 * when to write the completion to release the buffer.
	 */
	rx_entry = rxr_ep_get_rx_entry(rxr_ep, msg, tag, ignore, op, flags);
	if (OFI_UNLIKELY(!rx_entry)) {
		rxr_ep_progress_internal(rxr_ep);
		return -FI_EAGAIN;
	}

	rx_entry->rxr_flags |= RXR_MULTI_RECV_POSTED;
	dlist_init(&rx_entry->multi_recv_consumers);
	dlist_init(&rx_entry->multi_recv_entry);

	while (!dlist_empty(&rxr_ep->rx_unexp_list)) {
		ret = rxr_msg_proc_unexp_msg_list(rxr_ep, msg, tag,
						  ignore, op, flags, rx_entry);

		if (!rxr_msg_multi_recv_buffer_available(rxr_ep, rx_entry)) {
			/*
			 * Multi recv buffer consumed by short, unexp messages,
			 * free posted rx_entry.
			 */
			if (rxr_msg_multi_recv_buffer_complete(rxr_ep, rx_entry))
				rxr_release_rx_entry(rxr_ep, rx_entry);
			/*
			 * Multi recv buffer has been consumed, but waiting on
			 * long msg completion. Last msg completion will free
			 * posted rx_entry.
			 */
			if (ret == -FI_ENOMSG)
				return 0;
			return ret;
		}

		if (ret == -FI_ENOMSG) {
			ret = 0;
			break;
		}

		/*
		 * Error was encountered when processing unexpected messages,
		 * but there is buffer space available. Add the posted entry to
		 * the rx_list.
		 */
		if (ret)
			break;
	}

	dlist_insert_tail(&rx_entry->entry, &rxr_ep->rx_list);
	return ret;
}

void rxr_msg_multi_recv_handle_completion(struct rxr_ep *ep,
					  struct rxr_rx_entry *rx_entry)
{
	assert(!(rx_entry->rxr_flags & RXR_MULTI_RECV_POSTED) &&
	       (rx_entry->rxr_flags & RXR_MULTI_RECV_CONSUMER));

	dlist_remove(&rx_entry->multi_recv_entry);
	rx_entry->rxr_flags &= ~RXR_MULTI_RECV_CONSUMER;

	if (!rxr_msg_multi_recv_buffer_complete(ep, rx_entry->master_entry))
		return;

	/*
	 * Buffer is consumed and all messages have been received. Update the
	 * last message to release the application buffer.
	 */
	rx_entry->cq_entry.flags |= FI_MULTI_RECV;
}

/*
 *     create a rx entry and verify in unexpected message list
 *     else add to posted recv list
 */
static
ssize_t rxr_msg_generic_recv(struct fid_ep *ep, const struct fi_msg *msg,
			     uint64_t tag, uint64_t ignore, uint32_t op,
			     uint64_t flags)
{
	ssize_t ret = 0;
	struct rxr_ep *rxr_ep;
	struct dlist_entry *unexp_list;
	struct rxr_rx_entry *rx_entry;
	uint64_t rx_op_flags;

	FI_DBG(&rxr_prov, FI_LOG_EP_DATA,
	       "%s: iov_len: %lu tag: %lx ignore: %lx op: %x flags: %lx\n",
	       __func__, ofi_total_iov_len(msg->msg_iov, msg->iov_count), tag, ignore,
	       op, flags);

	rxr_ep = container_of(ep, struct rxr_ep, util_ep.ep_fid.fid);

	assert(msg->iov_count <= rxr_ep->rx_iov_limit);

	rxr_perfset_start(rxr_ep, perf_rxr_recv);

	assert(rxr_ep->util_ep.rx_msg_flags == 0 || rxr_ep->util_ep.rx_msg_flags == FI_COMPLETION);
	rx_op_flags = rxr_ep->util_ep.rx_op_flags;
	if (rxr_ep->util_ep.rx_msg_flags == 0)
		rx_op_flags &= ~FI_COMPLETION;
	flags = flags | rx_op_flags;

	fastlock_acquire(&rxr_ep->util_ep.lock);
	if (OFI_UNLIKELY(is_rx_res_full(rxr_ep))) {
		ret = -FI_EAGAIN;
		goto out;
	}

	if (flags & FI_MULTI_RECV) {
		ret = rxr_msg_multi_recv(rxr_ep, msg, tag, ignore, op, flags);
		goto out;
	}

	unexp_list = (op == ofi_op_tagged) ? &rxr_ep->rx_unexp_tagged_list :
		     &rxr_ep->rx_unexp_list;

	if (!dlist_empty(unexp_list)) {
		ret = rxr_msg_proc_unexp_msg_list(rxr_ep, msg, tag,
						  ignore, op, flags, NULL);

		if (ret != -FI_ENOMSG)
			goto out;
		ret = 0;
	}

	rx_entry = rxr_ep_get_rx_entry(rxr_ep, msg, tag,
				       ignore, op, flags);

	if (OFI_UNLIKELY(!rx_entry)) {
		ret = -FI_EAGAIN;
		rxr_ep_progress_internal(rxr_ep);
		goto out;
	}

	if (op == ofi_op_tagged)
		dlist_insert_tail(&rx_entry->entry, &rxr_ep->rx_tagged_list);
	else
		dlist_insert_tail(&rx_entry->entry, &rxr_ep->rx_list);

out:
	fastlock_release(&rxr_ep->util_ep.lock);

	rxr_perfset_end(rxr_ep, perf_rxr_recv);
	return ret;
}

static
ssize_t rxr_msg_discard_trecv(struct rxr_ep *ep,
			      struct rxr_rx_entry *rx_entry,
			      const struct fi_msg_tagged *msg,
			      int64_t flags)
{
	int ret;

	if ((flags & FI_DISCARD) && !(flags & (FI_PEEK | FI_CLAIM)))
		return -FI_EINVAL;

	rx_entry->fi_flags |= FI_DISCARD;
	rx_entry->rxr_flags |= RXR_RECV_CANCEL;
	ret = ofi_cq_write(ep->util_ep.rx_cq, msg->context,
			   FI_TAGGED | FI_RECV | FI_MSG,
			   0, NULL, rx_entry->cq_entry.data,
			   rx_entry->cq_entry.tag);
	rxr_rm_rx_cq_check(ep, ep->util_ep.rx_cq);
	return ret;
}

static
ssize_t rxr_msg_claim_trecv(struct fid_ep *ep_fid,
			    const struct fi_msg_tagged *msg,
			    int64_t flags)
{
	ssize_t ret = 0;
	struct rxr_ep *ep;
	struct rxr_rx_entry *rx_entry;
	struct fi_context *context;

	ep = container_of(ep_fid, struct rxr_ep, util_ep.ep_fid.fid);
	fastlock_acquire(&ep->util_ep.lock);

	context = (struct fi_context *)msg->context;
	rx_entry = (struct rxr_rx_entry *)context->internal[0];

	if (flags & FI_DISCARD) {
		ret = rxr_msg_discard_trecv(ep, rx_entry, msg, flags);
		if (OFI_UNLIKELY(ret))
			goto out;
	}

	/*
	 * Handle unexp match entry even for discard entry as we are sinking
	 * messages for that case
	 */
	memcpy(rx_entry->iov, msg->msg_iov,
	       sizeof(*msg->msg_iov) * msg->iov_count);
	rx_entry->iov_count = msg->iov_count;

	ret = rxr_msg_handle_unexp_match(ep, rx_entry, msg->tag,
					 msg->ignore, msg->context,
					 msg->addr, ofi_op_tagged, flags);

out:
	fastlock_release(&ep->util_ep.lock);
	return ret;
}

static
ssize_t rxr_msg_peek_trecv(struct fid_ep *ep_fid,
			   const struct fi_msg_tagged *msg,
			   uint64_t flags)
{
	ssize_t ret = 0;
	struct rxr_ep *ep;
	struct dlist_entry *match;
	dlist_func_t *match_func;
	struct rxr_match_info match_info;
	struct rxr_rx_entry *rx_entry;
	struct fi_context *context;
	struct rxr_pkt_entry *pkt_entry;
	size_t data_len;
	int64_t tag;

	ep = container_of(ep_fid, struct rxr_ep, util_ep.ep_fid.fid);

	fastlock_acquire(&ep->util_ep.lock);

	rxr_ep_progress_internal(ep);
	match_info.addr = msg->addr;
	match_info.tag = msg->tag;
	match_info.ignore = msg->ignore;

	if (ep->util_ep.caps & FI_DIRECTED_RECV)
		match_func = &rxr_msg_match_unexp_tagged;
	else
		match_func = &rxr_msg_match_unexp_tagged_anyaddr;

	match = dlist_find_first_match(&ep->rx_unexp_tagged_list,
	                               match_func,
				       (void *)&match_info);
	if (!match) {
		FI_DBG(&rxr_prov, FI_LOG_EP_CTRL,
		       "Message not found addr: %" PRIu64
		       " tag: %lx ignore %lx\n", msg->addr, msg->tag,
		       msg->ignore);
		ret = ofi_cq_write_error_peek(ep->util_ep.rx_cq, msg->tag,
					      msg->context);
		goto out;
	}

	rx_entry = container_of(match, struct rxr_rx_entry, entry);
	context = (struct fi_context *)msg->context;
	if (flags & FI_CLAIM) {
		context->internal[0] = rx_entry;
		dlist_remove(match);
	} else if (flags & FI_DISCARD) {
		dlist_remove(match);

		ret = rxr_msg_discard_trecv(ep, rx_entry, msg, flags);
		if (ret)
			goto out;

		memcpy(rx_entry->iov, msg->msg_iov,
		       sizeof(*msg->msg_iov) * msg->iov_count);
		rx_entry->iov_count = msg->iov_count;

		ret = rxr_msg_handle_unexp_match(ep, rx_entry,
						 msg->tag, msg->ignore,
						 msg->context, msg->addr,
						 ofi_op_tagged, flags);

		goto out;
	}

	pkt_entry = rx_entry->unexp_pkt;
	data_len = rxr_pkt_rtm_total_len(pkt_entry);
	tag = rxr_pkt_rtm_tag(pkt_entry);

	if (ep->util_ep.caps & FI_SOURCE)
		ret = ofi_cq_write_src(ep->util_ep.rx_cq, context,
				       FI_TAGGED | FI_RECV,
				       data_len, NULL,
				       rx_entry->cq_entry.data, tag,
				       rx_entry->addr);
	else
		ret = ofi_cq_write(ep->util_ep.rx_cq, context,
				   FI_TAGGED | FI_RECV,
				   data_len, NULL,
				   rx_entry->cq_entry.data, tag);
	rxr_rm_rx_cq_check(ep, ep->util_ep.rx_cq);
out:
	fastlock_release(&ep->util_ep.lock);
	return ret;
}

/**
 *   Non-tagged receive ops
 */
static
ssize_t rxr_msg_recvmsg(struct fid_ep *ep_fid, const struct fi_msg *msg,
			uint64_t flags)
{
	return rxr_msg_generic_recv(ep_fid, msg, 0, 0, ofi_op_msg, flags);
}

static
ssize_t rxr_msg_recv(struct fid_ep *ep, void *buf, size_t len,
		     void *desc, fi_addr_t src_addr, void *context)
{
	struct fi_msg msg;
	struct iovec msg_iov;

	memset(&msg, 0, sizeof(msg));
	msg_iov.iov_base = buf;
	msg_iov.iov_len = len;

	msg.msg_iov = &msg_iov;
	msg.desc = &desc;
	msg.iov_count = 1;
	msg.addr = src_addr;
	msg.context = context;
	msg.data = 0;

	return rxr_msg_recvmsg(ep, &msg, 0);
}

static
ssize_t rxr_msg_recvv(struct fid_ep *ep, const struct iovec *iov,
		      void **desc, size_t count, fi_addr_t src_addr,
		      void *context)
{
	struct fi_msg msg;

	memset(&msg, 0, sizeof(msg));
	msg.msg_iov = iov;
	msg.desc = desc;
	msg.iov_count = count;
	msg.addr = src_addr;
	msg.context = context;
	msg.data = 0;

	return rxr_msg_recvmsg(ep, &msg, 0);
}

/**
 *   Tagged receive ops functions
 */
static
ssize_t rxr_msg_trecv(struct fid_ep *ep_fid, void *buf, size_t len, void *desc,
		      fi_addr_t src_addr, uint64_t tag, uint64_t ignore,
		      void *context)
{
	struct fi_msg msg;
	struct iovec msg_iov;

	msg_iov.iov_base = (void *)buf;
	msg_iov.iov_len = len;

	msg.msg_iov = &msg_iov;
	msg.iov_count = 1;
	msg.addr = src_addr;
	msg.context = context;
	msg.desc = &desc;

	return rxr_msg_generic_recv(ep_fid, &msg, tag, ignore, ofi_op_tagged, 0);
}

static
ssize_t rxr_msg_trecvv(struct fid_ep *ep_fid, const struct iovec *iov,
		       void **desc, size_t count, fi_addr_t src_addr,
		       uint64_t tag, uint64_t ignore, void *context)
{
	struct fi_msg msg;

	msg.msg_iov = iov;
	msg.iov_count = count;
	msg.addr = src_addr;
	msg.desc = desc;
	msg.context = context;

	return rxr_msg_generic_recv(ep_fid, &msg, tag, ignore, ofi_op_tagged, 0);
}

static
ssize_t rxr_msg_trecvmsg(struct fid_ep *ep_fid, const struct fi_msg_tagged *tagmsg,
			 uint64_t flags)
{
	ssize_t ret;
	struct fi_msg msg;

	if (flags & FI_PEEK) {
		ret = rxr_msg_peek_trecv(ep_fid, tagmsg, flags);
		goto out;
	} else if (flags & FI_CLAIM) {
		ret = rxr_msg_claim_trecv(ep_fid, tagmsg, flags);
		goto out;
	}

	msg.msg_iov = tagmsg->msg_iov;
	msg.iov_count = tagmsg->iov_count;
	msg.addr = tagmsg->addr;
	msg.desc = tagmsg->desc;
	msg.context = tagmsg->context;

	ret = rxr_msg_generic_recv(ep_fid, &msg, tagmsg->tag, tagmsg->ignore,
				   ofi_op_tagged, flags);

out:
	return ret;
}

/**
 * Ops structures used by rxr_endpoint()
 */
struct fi_ops_msg rxr_ops_msg = {
	.size = sizeof(struct fi_ops_msg),
	.send = rxr_msg_send,
	.sendv = rxr_msg_sendv,
	.sendmsg = rxr_msg_sendmsg,
	.senddata = rxr_msg_senddata,
	.inject = rxr_msg_inject,
	.injectdata = rxr_msg_injectdata,
	.recv = rxr_msg_recv,
	.recvv = rxr_msg_recvv,
	.recvmsg = rxr_msg_recvmsg,
};

struct fi_ops_tagged rxr_ops_tagged = {
	.size = sizeof(struct fi_ops_tagged),
	.send = rxr_msg_tsend,
	.sendv = rxr_msg_tsendv,
	.sendmsg = rxr_msg_tsendmsg,
	.senddata = rxr_msg_tsenddata,
	.inject = rxr_msg_tinject,
	.injectdata = rxr_msg_tinjectdata,
	.recv = rxr_msg_trecv,
	.recvv = rxr_msg_trecvv,
	.recvmsg = rxr_msg_trecvmsg,
};

