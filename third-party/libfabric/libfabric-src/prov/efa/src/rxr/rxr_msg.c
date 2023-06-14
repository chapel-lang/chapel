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

#include "rxr_tp.h"

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
 * @brief select a two-sided protocol for the send operation
 *
 * @param[in]		rxr_ep		endpoint
 * @param[in]		tx_entry	contains information of the send operation
 * @param[in]		use_p2p		whether p2p can be used
 * @return		the RTM packet type of the two-sided protocol. Four
 *                      types of protocol can be used: eager, medium, longcts, longread.
 *                      Each protocol has tagged/non-tagged version. Some protocols has a DC version.
 *
 */
int rxr_msg_select_rtm(struct rxr_ep *rxr_ep, struct rxr_op_entry *tx_entry, int use_p2p)
{
	/*
	 * For performance consideration, this function assume the tagged rtm packet type id is
	 * always the correspondent message rtm packet type id + 1, thus the assertion here.
	 */
	assert(RXR_EAGER_MSGRTM_PKT + 1 == RXR_EAGER_TAGRTM_PKT);
	assert(RXR_MEDIUM_MSGRTM_PKT + 1 == RXR_MEDIUM_TAGRTM_PKT);
	assert(RXR_LONGCTS_MSGRTM_PKT + 1 == RXR_LONGCTS_TAGRTM_PKT);
	assert(RXR_LONGREAD_MSGRTM_PKT + 1 == RXR_LONGREAD_TAGRTM_PKT);
	assert(RXR_DC_EAGER_MSGRTM_PKT + 1 == RXR_DC_EAGER_TAGRTM_PKT);
	assert(RXR_DC_MEDIUM_MSGRTM_PKT + 1 == RXR_DC_MEDIUM_TAGRTM_PKT);
	assert(RXR_DC_LONGCTS_MSGRTM_PKT + 1 == RXR_DC_LONGCTS_TAGRTM_PKT);

	int tagged;
	int eager_rtm, medium_rtm, longcts_rtm, readbase_rtm, iface;
	size_t eager_rtm_max_data_size;
	struct rdm_peer *peer;
	struct efa_hmem_info *hmem_info;
	bool delivery_complete_requested;

	assert(tx_entry->op == ofi_op_msg || tx_entry->op == ofi_op_tagged);
	tagged = (tx_entry->op == ofi_op_tagged);
	assert(tagged == 0 || tagged == 1);

	peer = rxr_ep_get_peer(rxr_ep, tx_entry->addr);
	assert(peer);

	if (peer->is_local && rxr_ep->use_shm_for_tx) {
		/* Use shm for intra instance message.
		 *
		 * Shm provider support delivery complete, so we do not need to
		 * use DC version of EAGER RTM.
		 *
		 * Currently the shm provider does not support mixed memory type
		 * iov (it will crash), which will happen if the eager message
		 * protocol is used for FI_HMEM enabled buffers. An GitHub
		 * issue has been opened regarding this:
		 *     https://github.com/ofiwg/libfabric/issues/6639
		 *
		 * Have the remote side issue a read to copy the data instead
		 * to work around this issue.
		 */
		if (tx_entry->total_len > rxr_env.shm_max_medium_size ||
		   (tx_entry->total_len > 0 && efa_mr_is_hmem(tx_entry->desc[0])))
			return RXR_LONGREAD_MSGRTM_PKT + tagged;

		return RXR_EAGER_MSGRTM_PKT + tagged;
	}

	if (tx_entry->fi_flags & FI_INJECT)
		delivery_complete_requested = false;
	else
		delivery_complete_requested = tx_entry->fi_flags & FI_DELIVERY_COMPLETE;

	eager_rtm = (delivery_complete_requested) ? RXR_DC_EAGER_MSGRTM_PKT + tagged
						  : RXR_EAGER_MSGRTM_PKT + tagged;

	medium_rtm = (delivery_complete_requested) ? RXR_DC_MEDIUM_MSGRTM_PKT + tagged
						   :  RXR_MEDIUM_MSGRTM_PKT + tagged;

	longcts_rtm = (delivery_complete_requested) ? RXR_DC_LONGCTS_MSGRTM_PKT + tagged
						    : RXR_LONGCTS_MSGRTM_PKT + tagged;

	eager_rtm_max_data_size = rxr_tx_entry_max_req_data_capacity(rxr_ep, tx_entry, eager_rtm);

	iface = tx_entry->desc[0] ? ((struct efa_mr*) tx_entry->desc[0])->peer.iface : FI_HMEM_SYSTEM;
	hmem_info = &rxr_ep_domain(rxr_ep)->hmem_info[iface];

	readbase_rtm = rxr_pkt_type_readbase_rtm(peer, tx_entry->op, tx_entry->fi_flags, hmem_info);

	if (tx_entry->total_len >= hmem_info->min_read_msg_size &&
		efa_ep_support_rdma_read(rxr_ep->rdm_ep) &&
		(tx_entry->desc[0] || efa_is_cache_available(rxr_ep_domain(rxr_ep))))
		return readbase_rtm;

	if (tx_entry->total_len <= eager_rtm_max_data_size)
		return eager_rtm;

	if (tx_entry->total_len <= hmem_info->max_medium_msg_size)
		return medium_rtm;

	return longcts_rtm;
}

/**
 * @brief post RTM packet(s) for a send operation
 *
 * @param[in,out]	ep		endpoint
 * @param[in,out]	tx_entry	information of the send operation.
 * @param[in]		use_p2p		whether p2p can be used for this send operation.
 * @return		0 if packet(s) was posted successfully.
 *
 * 			negative libfabric error code on faliure, including but not limited to:
 *
 * 			-FI_ENOSUPP if the send operation requires an extra feature,
 * 			which peer does not support.
 *
 * 			-FI_EAGAIN for temporary out of resources for send
 */
ssize_t rxr_msg_post_rtm(struct rxr_ep *ep, struct rxr_op_entry *tx_entry, int use_p2p)
{
	ssize_t err;
	int rtm_type;
	struct rdm_peer *peer;

	peer = rxr_ep_get_peer(ep, tx_entry->addr);
	assert(peer);

	rtm_type = rxr_msg_select_rtm(ep, tx_entry, use_p2p);
	assert(rtm_type >= RXR_REQ_PKT_BEGIN);

	if (peer->is_local && ep->use_shm_for_tx) {
		/*
		 * We know shm's capablity, so no need to check handshake.
		 * AWS Neuron and SynapseAI are currently not supported by the SHM provider.
		 */

		if (efa_mr_is_neuron(tx_entry->desc[0]) || efa_mr_is_synapseai(tx_entry->desc[0])) {
			FI_WARN(&rxr_prov, FI_LOG_CQ,
			"Hmem iface: %s is currently not supported by the SHM provider\n",
			fi_tostr(&((struct efa_mr *)tx_entry->desc[0])->peer.iface, FI_TYPE_HMEM_IFACE));
			return -FI_EINVAL;
		}
		return rxr_pkt_post_req(ep, tx_entry, rtm_type, 0, 0);
	}

	if (rtm_type < RXR_EXTRA_REQ_PKT_BEGIN) {
		/* rtm requires only baseline feature, which peer should always support. */
		return rxr_pkt_post_req(ep, tx_entry, rtm_type, 0, 0);
	}

	/*
	 * rtm_type requrirs an extra feature, which peer might not support.
	 *
	 * Check handshake packet from peer to verify support status.
	 */
	if (!(peer->flags & RXR_PEER_HANDSHAKE_RECEIVED)) {
		err = rxr_pkt_trigger_handshake(ep, tx_entry->addr, peer);
		if (err)
			return err;

		rxr_ep_progress_internal(ep);
		return -FI_EAGAIN;
	}

	if (!rxr_pkt_req_supported_by_peer(rtm_type, peer))
		return -FI_EOPNOTSUPP;

	return rxr_pkt_post_req(ep, tx_entry, rtm_type, 0, 0);
}

ssize_t rxr_msg_generic_send(struct fid_ep *ep, const struct fi_msg *msg,
			     uint64_t tag, uint32_t op, uint64_t flags)
{
	struct rxr_ep *rxr_ep;
	struct efa_ep *efa_ep;
	ssize_t err, ret, use_p2p;
	struct rxr_op_entry *tx_entry;
	struct rdm_peer *peer;

	rxr_ep = container_of(ep, struct rxr_ep, util_ep.ep_fid.fid);
	assert(msg->iov_count <= rxr_ep->tx_iov_limit);

	efa_perfset_start(rxr_ep, perf_efa_tx);
	ofi_mutex_lock(&rxr_ep->util_ep.lock);

	if (OFI_UNLIKELY(is_tx_res_full(rxr_ep))) {
		err = -FI_EAGAIN;
		goto out;
	}

	peer = rxr_ep_get_peer(rxr_ep, msg->addr);
	assert(peer);

	if (peer->flags & RXR_PEER_IN_BACKOFF) {
		err = -FI_EAGAIN;
		goto out;
	}

	tx_entry = rxr_ep_alloc_tx_entry(rxr_ep, msg, op, tag, flags);
	if (OFI_UNLIKELY(!tx_entry)) {
		err = -FI_EAGAIN;
		rxr_ep_progress_internal(rxr_ep);
		goto out;
	}

	efa_ep = container_of(rxr_ep->rdm_ep, struct efa_ep, util_ep.ep_fid);
	ret = efa_ep_use_p2p(efa_ep, tx_entry->desc[0]);
	if (ret < 0) {
		err = ret;
		goto out;
	}

	use_p2p = ret;

	FI_DBG(&rxr_prov, FI_LOG_EP_DATA,
	       "iov_len: %lu tag: %lx op: %x flags: %lx\n",
	       tx_entry->total_len,
	       tag, op, flags);

	assert(tx_entry->op == ofi_op_msg || tx_entry->op == ofi_op_tagged);

	tx_entry->msg_id = peer->next_msg_id++;

	rxr_tracing(send_begin, tx_entry->msg_id, 
		    (size_t) tx_entry->cq_entry.op_context, tx_entry->total_len);
	rxr_tracing(send_begin_msg_context, 
		    (size_t) msg->context, (size_t) msg->addr);


	err = rxr_msg_post_rtm(rxr_ep, tx_entry, use_p2p);
	if (OFI_UNLIKELY(err)) {
		rxr_ep_progress_internal(rxr_ep);
		rxr_release_tx_entry(rxr_ep, tx_entry);
		peer->next_msg_id--;
	}

out:
	ofi_mutex_unlock(&rxr_ep->util_ep.lock);
	efa_perfset_end(rxr_ep, perf_efa_tx);
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
	struct fi_msg msg = {0};

	rxr_msg_construct(&msg, iov, desc, count, dest_addr, context, 0);
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
	return rxr_msg_sendv(ep, &iov, &desc, 1, dest_addr, context);
}

static
ssize_t rxr_msg_senddata(struct fid_ep *ep, const void *buf, size_t len,
			 void *desc, uint64_t data, fi_addr_t dest_addr,
			 void *context)
{
	struct fi_msg msg = {0};
	struct iovec iov;
	struct rxr_ep *rxr_ep;

	iov.iov_base = (void *)buf;
	iov.iov_len = len;

	rxr_msg_construct(&msg, &iov, &desc, 1, dest_addr, context, data);
	rxr_ep = container_of(ep, struct rxr_ep, util_ep.ep_fid.fid);
	return rxr_msg_generic_send(ep, &msg, 0, ofi_op_msg,
				    rxr_tx_flags(rxr_ep) | FI_REMOTE_CQ_DATA);
}

static
ssize_t rxr_msg_inject(struct fid_ep *ep, const void *buf, size_t len,
		       fi_addr_t dest_addr)
{
	struct rxr_ep *rxr_ep;
	struct fi_msg msg = {0};
	struct iovec iov;

	iov.iov_base = (void *)buf;
	iov.iov_len = len;

	rxr_msg_construct(&msg, &iov, NULL, 1, dest_addr, NULL, 0);
	rxr_ep = container_of(ep, struct rxr_ep, util_ep.ep_fid.fid);
	if (len > rxr_ep->inject_size) {
		FI_WARN(&rxr_prov, FI_LOG_CQ, "invalid message size %ld for inject.\n", len);
		return -FI_EINVAL;
	}

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

	rxr_msg_construct(&msg, &iov, NULL, 1, dest_addr, NULL, data);
	rxr_ep = container_of(ep, struct rxr_ep, util_ep.ep_fid.fid);
	if (len > rxr_ep->inject_size) {
		FI_WARN(&rxr_prov, FI_LOG_CQ, "invalid message size %ld for inject.\n", len);
		return -FI_EINVAL;
	}

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
	struct fi_msg msg = {0};

	rxr_msg_construct(&msg, tmsg->msg_iov, tmsg->desc, tmsg->iov_count, tmsg->addr, tmsg->context, tmsg->data);
	return rxr_msg_generic_send(ep_fid, &msg, tmsg->tag, ofi_op_tagged, flags);
}

static
ssize_t rxr_msg_tsendv(struct fid_ep *ep_fid, const struct iovec *iov,
		       void **desc, size_t count, fi_addr_t dest_addr,
		       uint64_t tag, void *context)
{
	struct rxr_ep *rxr_ep;
	struct fi_msg_tagged msg = {0};

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
	struct fi_msg msg = {0};
	struct iovec iov;
	struct rxr_ep *rxr_ep;

	iov.iov_base = (void *)buf;
	iov.iov_len = len;

	rxr_msg_construct(&msg, &iov, &desc, 1, dest_addr, context, data);
	rxr_ep = container_of(ep_fid, struct rxr_ep, util_ep.ep_fid.fid);
	return rxr_msg_generic_send(ep_fid, &msg, tag, ofi_op_tagged,
				    rxr_tx_flags(rxr_ep) | FI_REMOTE_CQ_DATA);
}

static
ssize_t rxr_msg_tinject(struct fid_ep *ep_fid, const void *buf, size_t len,
			fi_addr_t dest_addr, uint64_t tag)
{
	struct rxr_ep *rxr_ep;
	struct fi_msg msg = {0};
	struct iovec iov;

	iov.iov_base = (void *)buf;
	iov.iov_len = len;

	rxr_msg_construct(&msg, &iov, NULL, 1, dest_addr, NULL, 0);
	rxr_ep = container_of(ep_fid, struct rxr_ep, util_ep.ep_fid.fid);
	if (len > rxr_ep->inject_size) {
		FI_WARN(&rxr_prov, FI_LOG_CQ, "invalid message size %ld for inject.\n", len);
		return -FI_EINVAL;
	}

	return rxr_msg_generic_send(ep_fid, &msg, tag, ofi_op_tagged,
				    rxr_tx_flags(rxr_ep) | RXR_NO_COMPLETION | FI_INJECT);
}

static
ssize_t rxr_msg_tinjectdata(struct fid_ep *ep_fid, const void *buf, size_t len,
			    uint64_t data, fi_addr_t dest_addr, uint64_t tag)
{
	struct rxr_ep *rxr_ep;
	struct fi_msg msg = {0};
	struct iovec iov;

	iov.iov_base = (void *)buf;
	iov.iov_len = len;

	rxr_msg_construct(&msg, &iov, NULL, 1, dest_addr, NULL, data);
	rxr_ep = container_of(ep_fid, struct rxr_ep, util_ep.ep_fid.fid);
	if (len > rxr_ep->inject_size) {
		FI_WARN(&rxr_prov, FI_LOG_CQ, "invalid message size %ld for inject.\n", len);
		return -FI_EINVAL;
	}

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
	uint64_t tag;
	uint64_t ignore;
};

/**
 * @brief match function for rx_entry in ep->unexp_tagged_list
 *
 * @param[in]	item	pointer to rx_entry->entry.
 * @param[in]	arg	pointer to rxr_match_info
 * @return   0 or 1 indicating wether this entry is a match
 */
static
int rxr_msg_match_ep_unexp_by_tag(struct dlist_entry *item, const void *arg)
{
	const struct rxr_match_info *match_info = arg;
	struct rxr_op_entry *rx_entry;

	rx_entry = container_of(item, struct rxr_op_entry, entry);

	return ofi_match_tag(rx_entry->tag, match_info->ignore,
			     match_info->tag);
}

/**
 * @brief match function for rx_entry in peer->unexp_tagged_list
 *
 * @param[in]	item	pointer to rx_entry->peer_unexp_entry.
 * @param[in]	arg	pointer to rxr_match_info
 * @return   0 or 1 indicating wether this entry is a match
 */
static
int rxr_msg_match_peer_unexp_by_tag(struct dlist_entry *item, const void *arg)
{
	const struct rxr_match_info *match_info = arg;
	struct rxr_op_entry *rx_entry;

	rx_entry = container_of(item, struct rxr_op_entry, peer_unexp_entry);

	return ofi_match_tag(rx_entry->tag, match_info->ignore,
			     match_info->tag);
}

static
int rxr_msg_handle_unexp_match(struct rxr_ep *ep,
			       struct rxr_op_entry *rx_entry,
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

/**
 * @brief allocate an rx entry for a fi_msg.
 *        This function is used by two sided operation only.
 *
 * @param ep[in]	end point
 * @param msg[in]	fi_msg contains iov,iov_count,context for ths operation
 * @param op[in]	operation type (ofi_op_msg or ofi_op_tagged)
 * @param flags[in]	flags application used to call fi_recv/fi_trecv functions
 * @param tag[in]	tag (used only if op is ofi_op_tagged)
 * @param ignore[in]	ignore mask (used only if op is ofi_op_tagged)
 * @return		if allocation succeeded, return pointer to rx_entry
 * 			if allocation failed, return NULL
 */
struct rxr_op_entry *rxr_msg_alloc_rx_entry(struct rxr_ep *ep,
					    const struct fi_msg *msg,
					    uint32_t op, uint64_t flags,
					    uint64_t tag, uint64_t ignore)
{
	struct rxr_op_entry *rx_entry;
	fi_addr_t addr;

	if (ep->util_ep.caps & FI_DIRECTED_RECV)
		addr = msg->addr;
	else
		addr = FI_ADDR_UNSPEC;

	rx_entry = rxr_ep_alloc_rx_entry(ep, addr, op);
	if (!rx_entry)
		return NULL;

	rx_entry->fi_flags = flags;
	if (op == ofi_op_tagged) {
		rx_entry->tag = tag;
		rx_entry->cq_entry.tag = tag;
		rx_entry->ignore = ignore;
	}

	/* Handle case where we're allocating an unexpected rx_entry */
	rx_entry->iov_count = msg->iov_count;
	if (rx_entry->iov_count) {
		assert(msg->msg_iov);
		memcpy(rx_entry->iov, msg->msg_iov, sizeof(*rx_entry->iov) * msg->iov_count);
		rx_entry->cq_entry.len = ofi_total_iov_len(rx_entry->iov, rx_entry->iov_count);
		rx_entry->cq_entry.buf = msg->msg_iov[0].iov_base;
	}

	if (msg->desc)
		memcpy(&rx_entry->desc[0], msg->desc, sizeof(*msg->desc) * msg->iov_count);
	else
		memset(&rx_entry->desc[0], 0, sizeof(rx_entry->desc));

	rx_entry->cq_entry.op_context = msg->context;
	return rx_entry;
}

struct rxr_op_entry *rxr_msg_alloc_unexp_rx_entry_for_msgrtm(struct rxr_ep *ep,
							     struct rxr_pkt_entry **pkt_entry_ptr)
{
	struct rdm_peer *peer;
	struct rxr_op_entry *rx_entry;
	struct rxr_pkt_entry *unexp_pkt_entry;

	unexp_pkt_entry = rxr_pkt_get_unexp(ep, pkt_entry_ptr);
	if (OFI_UNLIKELY(!unexp_pkt_entry)) {
		FI_WARN(&rxr_prov, FI_LOG_CQ, "packet entries exhausted.\n");
		return NULL;
	}

	rx_entry = rxr_ep_alloc_rx_entry(ep, unexp_pkt_entry->addr, ofi_op_msg);
	if (OFI_UNLIKELY(!rx_entry))
		return NULL;

	rx_entry->rxr_flags = 0;
	rx_entry->state = RXR_RX_UNEXP;
	rx_entry->unexp_pkt = unexp_pkt_entry;
	rxr_pkt_rtm_update_rx_entry(unexp_pkt_entry, rx_entry);
	dlist_insert_tail(&rx_entry->entry, &ep->rx_unexp_list);
	peer = rxr_ep_get_peer(ep, unexp_pkt_entry->addr);
	dlist_insert_tail(&rx_entry->peer_unexp_entry, &peer->rx_unexp_list);
	return rx_entry;
}

struct rxr_op_entry *rxr_msg_alloc_unexp_rx_entry_for_tagrtm(struct rxr_ep *ep,
							     struct rxr_pkt_entry **pkt_entry_ptr)
{
	struct rdm_peer *peer;
	struct rxr_op_entry *rx_entry;
	struct rxr_pkt_entry *unexp_pkt_entry;

	unexp_pkt_entry = rxr_pkt_get_unexp(ep, pkt_entry_ptr);
	if (OFI_UNLIKELY(!unexp_pkt_entry)) {
		FI_WARN(&rxr_prov, FI_LOG_CQ, "packet entries exhausted.\n");
		return NULL;
	}

	rx_entry = rxr_ep_alloc_rx_entry(ep, unexp_pkt_entry->addr, ofi_op_tagged);
	if (OFI_UNLIKELY(!rx_entry))
		return NULL;

	rx_entry->tag = rxr_pkt_rtm_tag(unexp_pkt_entry);
	rx_entry->rxr_flags = 0;
	rx_entry->state = RXR_RX_UNEXP;
	rx_entry->unexp_pkt = unexp_pkt_entry;
	rxr_pkt_rtm_update_rx_entry(unexp_pkt_entry, rx_entry);
	dlist_insert_tail(&rx_entry->entry, &ep->rx_unexp_tagged_list);
	peer = rxr_ep_get_peer(ep, unexp_pkt_entry->addr);
	dlist_insert_tail(&rx_entry->peer_unexp_entry, &peer->rx_unexp_tagged_list);
	return rx_entry;
}

struct rxr_op_entry *rxr_msg_split_rx_entry(struct rxr_ep *ep,
					    struct rxr_op_entry *posted_entry,
					    struct rxr_op_entry *consumer_entry,
					    struct rxr_pkt_entry *pkt_entry)
{
	struct rxr_op_entry *rx_entry;
	size_t buf_len, consumed_len, data_len;
	uint64_t tag, ignore;
	struct fi_msg msg = {0};

	assert(rxr_get_base_hdr(pkt_entry->pkt)->type >= RXR_REQ_PKT_BEGIN);

	if (!consumer_entry) {
		tag = 0;
		ignore = ~0;
		msg.msg_iov = posted_entry->iov;
		msg.iov_count = posted_entry->iov_count;
		msg.addr = pkt_entry->addr;
		rx_entry = rxr_msg_alloc_rx_entry(ep, &msg,
						  ofi_op_msg,
						  posted_entry->fi_flags,
						  tag, ignore);
		if (OFI_UNLIKELY(!rx_entry))
			return NULL;

		FI_DBG(&rxr_prov, FI_LOG_EP_CTRL,
		       "Splitting into new multi_recv consumer rx_entry %d from rx_entry %d\n",
		       rx_entry->rx_id,
		       posted_entry->rx_id);
	} else {
		rx_entry = consumer_entry;
		memcpy(rx_entry->iov, posted_entry->iov,
		       sizeof(*posted_entry->iov) * posted_entry->iov_count);
		rx_entry->iov_count = posted_entry->iov_count;
	}

	rxr_pkt_rtm_update_rx_entry(pkt_entry, rx_entry);
	data_len = rx_entry->total_len;
	buf_len = ofi_total_iov_len(rx_entry->iov,
				    rx_entry->iov_count);
	consumed_len = MIN(buf_len, data_len);

	rx_entry->rxr_flags |= RXR_MULTI_RECV_CONSUMER;
	rx_entry->total_len = data_len;
	rx_entry->fi_flags |= FI_MULTI_RECV;
	rx_entry->master_entry = posted_entry;
	rx_entry->cq_entry.len = consumed_len;
	rx_entry->cq_entry.buf = rx_entry->iov[0].iov_base;
	rx_entry->cq_entry.op_context = posted_entry->cq_entry.op_context;
	rx_entry->cq_entry.flags = (FI_RECV | FI_MSG);

	ofi_consume_iov(posted_entry->iov, &posted_entry->iov_count,
			consumed_len);

	dlist_init(&rx_entry->multi_recv_entry);
	dlist_insert_tail(&rx_entry->multi_recv_entry,
			  &posted_entry->multi_recv_consumers);
	return rx_entry;
}

/**
 * @brief find an unexpected rx entry for a receive operation.
 *
 * @param[in]	ep	endpoint
 * @param[in]	addr	fi_addr of the peer want to receive from, can be FI_ADDR_UNSPEC
 * @param[in]	tag	tag of the unexpected message, used only if op is ofi_op_tagged.
 * @param[in]	ignore	mask of the tag, used only if op is ofi_op_tagged.
 * @param[in]	op	either ofi_op_tagged or ofi_op_msg.
 * @param[in]	claim   whether to claim the rx_entry, e.g. remove it from unexpected queue.
 * @return	If an unexpected rx_entry was found, return the pointer.
 * 		Otherwise, return NULL.
 */
static inline
struct rxr_op_entry *rxr_msg_find_unexp_rx_entry(struct rxr_ep *ep, fi_addr_t addr,
						 int64_t tag, uint64_t ignore, uint32_t op,
						 bool claim)
{
	struct rxr_match_info match_info;
	struct rxr_op_entry *rx_entry;
	struct dlist_entry *match;
	struct rdm_peer *peer;

	peer = (ep->util_ep.caps & FI_DIRECTED_RECV) ? rxr_ep_get_peer(ep, addr) : NULL;

	switch(op) {
	case ofi_op_msg:
		if (peer) {
			match = dlist_empty(&peer->rx_unexp_list) ? NULL : peer->rx_unexp_list.next;
			rx_entry = match ? container_of(match, struct rxr_op_entry, peer_unexp_entry) : NULL;
		} else {
			match = dlist_empty(&ep->rx_unexp_list) ? NULL : ep->rx_unexp_list.next;
			rx_entry = match ? container_of(match, struct rxr_op_entry, entry) : NULL;
		}
		break;
	case ofi_op_tagged:
		match_info.tag = tag;
		match_info.ignore = ignore;

		if (peer) {
			match = dlist_find_first_match(&peer->rx_unexp_tagged_list,
			                               rxr_msg_match_peer_unexp_by_tag,
						       (void *)&match_info);
			rx_entry = match ? container_of(match, struct rxr_op_entry, peer_unexp_entry) : NULL;
		} else {
			match = dlist_find_first_match(&ep->rx_unexp_tagged_list,
						       rxr_msg_match_ep_unexp_by_tag,
						       (void *)&match_info);
			rx_entry = match ? container_of(match, struct rxr_op_entry, entry) : NULL;
		}
		break;
	default:
		FI_WARN(&rxr_prov, FI_LOG_CQ, "Error: wrong op in rxr_msg_find_unexp_rx_entry()");
		abort();
	}

	if (rx_entry && claim) {
		dlist_remove(&rx_entry->entry);
		dlist_remove(&rx_entry->peer_unexp_entry);
	}

	return rx_entry;
}

/*
 *    Search unexpected list for matching message and process it if found.
 *    Returns 0 if the message is processed, -FI_ENOMSG if no match is found.
 */
static
int rxr_msg_proc_unexp_msg_list(struct rxr_ep *ep, const struct fi_msg *msg,
				uint64_t tag, uint64_t ignore, uint32_t op, uint64_t flags,
				struct rxr_op_entry *posted_entry)
{
	struct rxr_op_entry *rx_entry;
	int ret;
	bool claim;

	claim = true;
	rx_entry = rxr_msg_find_unexp_rx_entry(ep, msg->addr, tag, ignore, op, claim);
	if (!rx_entry)
		return -FI_ENOMSG;

	/* 
	 * TODO: Use a realiable way to trigger this function. Can we swap packet order with fake-rdma-core?
	 * NOTE: Cannot trigger this routine, didn't debug.
	 */
	rxr_tracing(msg_match_unexpected, rx_entry->msg_id, 
		    (size_t) rx_entry->cq_entry.op_context, rx_entry->total_len, 
		    (int) tag, msg->addr);
	/*
	 * Initialize the matched entry as a multi-recv consumer if the posted
	 * buffer is a multi-recv buffer.
	 */
	if (posted_entry) {
		/*
		 * rxr_msg_split_rx_entry will setup rx_entry iov and count
		 */
		rx_entry = rxr_msg_split_rx_entry(ep, posted_entry, rx_entry,
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
					 struct rxr_op_entry *rx_entry)
{
	assert(rx_entry->fi_flags & FI_MULTI_RECV);
	assert(rx_entry->rxr_flags & RXR_MULTI_RECV_POSTED);

	return (ofi_total_iov_len(rx_entry->iov, rx_entry->iov_count)
		>= ep->min_multi_recv_size);
}

static inline
bool rxr_msg_multi_recv_buffer_complete(struct rxr_ep *ep,
					struct rxr_op_entry *rx_entry)
{
	assert(rx_entry->fi_flags & FI_MULTI_RECV);
	assert(rx_entry->rxr_flags & RXR_MULTI_RECV_POSTED);

	return (!rxr_msg_multi_recv_buffer_available(ep, rx_entry) &&
		dlist_empty(&rx_entry->multi_recv_consumers));
}

void rxr_msg_multi_recv_free_posted_entry(struct rxr_ep *ep,
					  struct rxr_op_entry *rx_entry)
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
	struct rxr_op_entry *rx_entry;
	int ret = 0;

	/*
	 * Always get new rx_entry of type RXR_MULTI_RECV_POSTED when in the
	 * multi recv path. The posted entry will not be used for receiving
	 * messages but will be used for tracking the application's buffer and
	 * when to write the completion to release the buffer.
	 */
	rx_entry = rxr_msg_alloc_rx_entry(rxr_ep, msg, op, flags, tag, ignore);
	if (OFI_UNLIKELY(!rx_entry)) {
		rxr_ep_progress_internal(rxr_ep);
		return -FI_EAGAIN;
	}

	if (rx_entry->cq_entry.len < rxr_ep->min_multi_recv_size) {
		FI_WARN(&rxr_prov, FI_LOG_EP_CTRL, "invalid size (%ld) for multi_recv! expected to be >= %ld\n",
			rx_entry->cq_entry.len, rxr_ep->min_multi_recv_size);
		rxr_release_rx_entry(rxr_ep, rx_entry);
		return -FI_EINVAL;
	}

	if (op == ofi_op_tagged) {
		FI_WARN(&rxr_prov, FI_LOG_EP_CTRL, "tagged recv cannot be applied to multi_recv!\n");
		rxr_release_rx_entry(rxr_ep, rx_entry);
		return -FI_EINVAL;
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
					  struct rxr_op_entry *rx_entry)
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
	struct rxr_op_entry *rx_entry;
	uint64_t rx_op_flags;

	rxr_ep = container_of(ep, struct rxr_ep, util_ep.ep_fid.fid);

	assert(msg->iov_count <= rxr_ep->rx_iov_limit);

	efa_perfset_start(rxr_ep, perf_efa_recv);

	assert(rxr_ep->util_ep.rx_msg_flags == 0 || rxr_ep->util_ep.rx_msg_flags == FI_COMPLETION);
	rx_op_flags = rxr_ep->util_ep.rx_op_flags;
	if (rxr_ep->util_ep.rx_msg_flags == 0)
		rx_op_flags &= ~FI_COMPLETION;
	flags = flags | rx_op_flags;

	ofi_mutex_lock(&rxr_ep->util_ep.lock);
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

	rx_entry = rxr_msg_alloc_rx_entry(rxr_ep, msg, op, flags, tag, ignore);

	if (OFI_UNLIKELY(!rx_entry)) {
		ret = -FI_EAGAIN;
		rxr_ep_progress_internal(rxr_ep);
		goto out;
	}

	FI_DBG(&rxr_prov, FI_LOG_EP_DATA,
	       "%s: iov_len: %lu tag: %lx ignore: %lx op: %x flags: %lx\n",
	       __func__, rx_entry->total_len, tag, ignore,
	       op, flags);

	rxr_tracing(recv_begin, rx_entry->msg_id, 
		    (size_t) rx_entry->cq_entry.op_context, rx_entry->total_len);
	rxr_tracing(recv_begin_msg_context, (size_t) msg->context, (size_t) msg->addr);

	if (rxr_ep->use_zcpy_rx) {
		ret = rxr_ep_post_user_recv_buf(rxr_ep, rx_entry, flags);
		if (ret == -FI_EAGAIN)
			rxr_ep_progress_internal(rxr_ep);
	} else if (op == ofi_op_tagged) {
		dlist_insert_tail(&rx_entry->entry, &rxr_ep->rx_tagged_list);
	} else {
		dlist_insert_tail(&rx_entry->entry, &rxr_ep->rx_list);
	}

out:
	ofi_mutex_unlock(&rxr_ep->util_ep.lock);

	efa_perfset_end(rxr_ep, perf_efa_recv);
	return ret;
}

static
ssize_t rxr_msg_discard_trecv(struct rxr_ep *ep,
			      struct rxr_op_entry *rx_entry,
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
	struct rxr_op_entry *rx_entry;
	struct fi_context *context;

	ep = container_of(ep_fid, struct rxr_ep, util_ep.ep_fid.fid);
	ofi_mutex_lock(&ep->util_ep.lock);

	context = (struct fi_context *)msg->context;
	rx_entry = (struct rxr_op_entry *)context->internal[0];

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
	ofi_mutex_unlock(&ep->util_ep.lock);
	return ret;
}

static
ssize_t rxr_msg_peek_trecv(struct fid_ep *ep_fid,
			   const struct fi_msg_tagged *msg,
			   uint64_t flags)
{
	ssize_t ret = 0;
	struct rxr_ep *ep;
	struct rxr_op_entry *rx_entry;
	struct fi_context *context;
	struct rxr_pkt_entry *pkt_entry;
	size_t data_len;
	int64_t tag;
	bool claim;

	ep = container_of(ep_fid, struct rxr_ep, util_ep.ep_fid.fid);

	ofi_mutex_lock(&ep->util_ep.lock);

	rxr_ep_progress_internal(ep);

	claim = (flags & (FI_CLAIM | FI_DISCARD));
	rx_entry = rxr_msg_find_unexp_rx_entry(ep, msg->addr, msg->tag, msg->ignore, ofi_op_tagged,
					       claim);
	if (!rx_entry) {
		FI_DBG(&rxr_prov, FI_LOG_EP_CTRL,
		       "Message not found addr: %" PRIu64
		       " tag: %lx ignore %lx\n", msg->addr, msg->tag,
		       msg->ignore);
		ret = ofi_cq_write_error_peek(ep->util_ep.rx_cq, msg->tag,
					      msg->context);
		goto out;
	}

	context = (struct fi_context *)msg->context;
	if (flags & FI_CLAIM) {
		context->internal[0] = rx_entry;
	} else if (flags & FI_DISCARD) {
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
	ofi_mutex_unlock(&ep->util_ep.lock);
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
	struct fi_msg msg = {0};
	struct iovec iov;

	iov.iov_base = buf;
	iov.iov_len = len;

	rxr_msg_construct(&msg, &iov, &desc, 1, src_addr, context, 0);
	return rxr_msg_recvmsg(ep, &msg, 0);
}

static
ssize_t rxr_msg_recvv(struct fid_ep *ep, const struct iovec *iov,
		      void **desc, size_t count, fi_addr_t src_addr,
		      void *context)
{
	struct fi_msg msg = {0};

	rxr_msg_construct(&msg, iov, desc, count, src_addr, context, 0);
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
	struct fi_msg msg = {0};
	struct iovec iov;

	iov.iov_base = (void *)buf;
	iov.iov_len = len;

	rxr_msg_construct(&msg, &iov, &desc, 1, src_addr, context, 0);
	return rxr_msg_generic_recv(ep_fid, &msg, tag, ignore, ofi_op_tagged, 0);
}

static
ssize_t rxr_msg_trecvv(struct fid_ep *ep_fid, const struct iovec *iov,
		       void **desc, size_t count, fi_addr_t src_addr,
		       uint64_t tag, uint64_t ignore, void *context)
{
	struct fi_msg msg = {0};

	rxr_msg_construct(&msg, iov, desc, count, src_addr, context, 0);
	return rxr_msg_generic_recv(ep_fid, &msg, tag, ignore, ofi_op_tagged, 0);
}

static
ssize_t rxr_msg_trecvmsg(struct fid_ep *ep_fid, const struct fi_msg_tagged *tmsg,
			 uint64_t flags)
{
	ssize_t ret;
	struct fi_msg msg = {0};

	if (flags & FI_PEEK) {
		ret = rxr_msg_peek_trecv(ep_fid, tmsg, flags);
		goto out;
	} else if (flags & FI_CLAIM) {
		ret = rxr_msg_claim_trecv(ep_fid, tmsg, flags);
		goto out;
	}

	rxr_msg_construct(&msg, tmsg->msg_iov, tmsg->desc, tmsg->iov_count, tmsg->addr, tmsg->context, tmsg->data);
	ret = rxr_msg_generic_recv(ep_fid, &msg, tmsg->tag, tmsg->ignore,
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
