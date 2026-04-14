/* Copyright Amazon.com, Inc. or its affiliates. All rights reserved. */
/* SPDX-License-Identifier: BSD-2-Clause OR GPL-2.0-only */

#include <inttypes.h>
#include <stdlib.h>
#include <string.h>
#include "ofi.h"
#include <ofi_util.h>
#include <ofi_iov.h>

#include "efa.h"

#include "efa_rdm_msg.h"
#include "efa_rdm_srx.h"
#include "efa_rdm_rxe_map.h"
#include "efa_rdm_pke_cmd.h"
#include "efa_rdm_pke_rtm.h"
#include "efa_rdm_pke_utils.h"
#include "efa_rdm_pke_req.h"

#include "efa_rdm_tracepoint.h"

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
 * @param [in]		efa_rdm_ep		endpoint
 * @param [in]		txe	contains information of the send operation
 * @param [in]		use_p2p		whether p2p can be used
 * @return		the RTM packet type of the two-sided protocol. Four
 *                      types of protocol can be used: eager, medium, longcts, longread.
 *                      Each protocol has tagged/non-tagged version. Some protocols has a DC version.
 * @related		efa_rdm_ep
 */
int efa_rdm_msg_select_rtm(struct efa_rdm_ep *efa_rdm_ep, struct efa_rdm_ope *txe, int use_p2p)
{
	/*
	 * For performance consideration, this function assume the tagged rtm packet type id is
	 * always the correspondent message rtm packet type id + 1, thus the assertion here.
	 */
	assert(EFA_RDM_EAGER_MSGRTM_PKT + 1 == EFA_RDM_EAGER_TAGRTM_PKT);
	assert(EFA_RDM_MEDIUM_MSGRTM_PKT + 1 == EFA_RDM_MEDIUM_TAGRTM_PKT);
	assert(EFA_RDM_LONGCTS_MSGRTM_PKT + 1 == EFA_RDM_LONGCTS_TAGRTM_PKT);
	assert(EFA_RDM_LONGREAD_MSGRTM_PKT + 1 == EFA_RDM_LONGREAD_TAGRTM_PKT);
	assert(EFA_RDM_DC_EAGER_MSGRTM_PKT + 1 == EFA_RDM_DC_EAGER_TAGRTM_PKT);
	assert(EFA_RDM_DC_MEDIUM_MSGRTM_PKT + 1 == EFA_RDM_DC_MEDIUM_TAGRTM_PKT);
	assert(EFA_RDM_DC_LONGCTS_MSGRTM_PKT + 1 == EFA_RDM_DC_LONGCTS_TAGRTM_PKT);

	int tagged;
	int eager_rtm, medium_rtm, longcts_rtm, readbase_rtm, iface;
	size_t eager_rtm_max_data_size;
	bool delivery_complete_requested;

	assert(txe->op == ofi_op_msg || txe->op == ofi_op_tagged);
	tagged = (txe->op == ofi_op_tagged);
	assert(tagged == 0 || tagged == 1);

	iface = txe->desc[0] ? ((struct efa_mr*) txe->desc[0])->peer.iface : FI_HMEM_SYSTEM;

	if (txe->fi_flags & FI_INJECT || efa_both_support_zero_hdr_data_transfer(efa_rdm_ep, txe->peer))
		delivery_complete_requested = false;
	else
		delivery_complete_requested = txe->fi_flags & FI_DELIVERY_COMPLETE;

	eager_rtm = (delivery_complete_requested) ? EFA_RDM_DC_EAGER_MSGRTM_PKT + tagged
						  : EFA_RDM_EAGER_MSGRTM_PKT + tagged;

	medium_rtm = (delivery_complete_requested) ? EFA_RDM_DC_MEDIUM_MSGRTM_PKT + tagged
						   :  EFA_RDM_MEDIUM_MSGRTM_PKT + tagged;

	longcts_rtm = (delivery_complete_requested) ? EFA_RDM_DC_LONGCTS_MSGRTM_PKT + tagged
						    : EFA_RDM_LONGCTS_MSGRTM_PKT + tagged;

	eager_rtm_max_data_size = efa_rdm_txe_max_req_data_capacity(efa_rdm_ep, txe, eager_rtm);

	readbase_rtm = efa_rdm_peer_select_readbase_rtm(txe->peer, efa_rdm_ep, txe);

	if (use_p2p &&
	    txe->total_len >= g_efa_hmem_info[iface].min_read_msg_size &&
	    efa_rdm_interop_rdma_read(efa_rdm_ep, txe->peer) &&
	    (txe->desc[0] || efa_is_cache_available(efa_rdm_ep_domain(efa_rdm_ep))))
		return readbase_rtm;

	if (txe->total_len <= eager_rtm_max_data_size)
		return eager_rtm;

	if (txe->total_len <= g_efa_hmem_info[iface].max_medium_msg_size)
		return medium_rtm;

	return longcts_rtm;
}

/**
 * @brief post RTM packet(s) for a send operation
 *
 * @param[in,out]	ep		endpoint
 * @param[in,out]	txe	information of the send operation.
 * @retval		0 if packet(s) was posted successfully.
 * @retval		-FI_ENOSUPP if the send operation requires an extra feature,
 * 			which peer does not support.
 * @retval		-FI_EAGAIN for temporary out of resources for send
 */
ssize_t efa_rdm_msg_post_rtm(struct efa_rdm_ep *ep, struct efa_rdm_ope *txe)
{
	ssize_t err;
	int rtm_type, use_p2p;

	assert(txe->peer);

	/*
	 * It is required to get receiver's user recv QP through handshake
	 * if sender supports this feature.
	 */
	if ((ep->extra_info[0] & EFA_RDM_EXTRA_FEATURE_REQUEST_USER_RECV_QP) &&
	    !(txe->peer->flags & EFA_RDM_PEER_HANDSHAKE_RECEIVED)) {
		return efa_rdm_ep_enforce_handshake_for_txe(ep, txe);
	}

	err = efa_rdm_ep_use_p2p(ep, txe->desc[0]);
	if (err < 0)
		return err;

	use_p2p = err;

	rtm_type = efa_rdm_msg_select_rtm(ep, txe, use_p2p);
	assert(rtm_type >= EFA_RDM_REQ_PKT_BEGIN);

	if (rtm_type < EFA_RDM_EXTRA_REQ_PKT_BEGIN) {
		/* rtm requires only baseline feature, which peer should always support. */
		return efa_rdm_ope_post_send(txe, rtm_type);
	}

	/*
	 * rtm_type requires an extra feature, which peer might not support.
	 *
	 * Check handshake packet from peer to verify support status.
	 */
	if (!ep->homogeneous_peers && !(txe->peer->flags & EFA_RDM_PEER_HANDSHAKE_RECEIVED))
		return efa_rdm_ep_enforce_handshake_for_txe(ep, txe);

	if (!ep->homogeneous_peers && !efa_rdm_pkt_type_is_supported_by_peer(rtm_type, txe->peer))
		return -FI_EOPNOTSUPP;

	return efa_rdm_ope_post_send(txe, rtm_type);
}

static inline
ssize_t efa_rdm_msg_generic_send(struct efa_rdm_ep *ep, struct efa_rdm_peer *peer, const struct fi_msg *msg,
			     uint64_t tag, uint32_t op, uint64_t flags)
{
	ssize_t err;
	struct efa_rdm_ope *txe;
	struct util_srx_ctx *srx_ctx;

	efa_rdm_tracepoint(send_begin_msg_context,
		    (size_t) msg->context, (size_t) msg->addr);

	srx_ctx = efa_rdm_ep_get_peer_srx_ctx(ep);

	assert(msg->iov_count <= ep->base_ep.info->tx_attr->iov_limit);

	efa_perfset_start(ep, perf_efa_tx);
	ofi_genlock_lock(srx_ctx->lock);

	if (peer->flags & EFA_RDM_PEER_IN_BACKOFF) {
		err = -FI_EAGAIN;
		goto out;
	}

	txe = efa_rdm_ep_alloc_txe(ep, peer, msg, op, tag, flags);
	if (OFI_UNLIKELY(!txe)) {
		err = -FI_EAGAIN;
		goto out;
	}

	assert(txe->op == ofi_op_msg || txe->op == ofi_op_tagged);

	txe->msg_id = peer->next_msg_id++;

	EFA_DBG(FI_LOG_EP_DATA,
		"peer: %" PRIu64
		": size %lu tag: %lx op: %x flags: %lx msg_id: %" PRIu32 "\n",
		peer->conn->fi_addr, txe->total_len, tag, op, flags, txe->msg_id);

	efa_rdm_tracepoint(send_begin, txe->msg_id,
		    (size_t) txe->cq_entry.op_context, txe->total_len);

	err = efa_rdm_msg_post_rtm(ep, txe);
	if (OFI_UNLIKELY(err)) {
		efa_rdm_txe_release(txe);
		peer->next_msg_id--;
	}

out:
	ofi_genlock_unlock(srx_ctx->lock);
	efa_perfset_end(ep, perf_efa_tx);
	return err;
}

/**
 *   Non-tagged send ops function
 */
static
ssize_t efa_rdm_msg_sendmsg(struct fid_ep *ep, const struct fi_msg *msg,
			uint64_t flags)
{
	struct efa_rdm_peer *peer;
	struct efa_rdm_ep *efa_rdm_ep;
	void *shm_desc[EFA_RDM_IOV_LIMIT] = {NULL};
	struct fi_msg *shm_msg;
	void **efa_desc = NULL;
	fi_addr_t efa_addr;
	int ret;

	efa_rdm_ep = container_of(ep, struct efa_rdm_ep, base_ep.util_ep.ep_fid.fid);

	ret = efa_rdm_attempt_to_sync_memops_iov(efa_rdm_ep, (struct iovec *)msg->msg_iov, msg->desc, msg->iov_count);
	if (ret)
		return ret;

	peer = efa_rdm_ep_get_peer(efa_rdm_ep, msg->addr);
	assert(peer);
	if (peer->is_local && efa_rdm_ep->shm_ep) {
		shm_msg = (struct fi_msg *)msg;
		if (msg->desc) {
			efa_desc = msg->desc;
			efa_rdm_get_desc_for_shm(msg->iov_count, msg->desc, shm_desc);
			shm_msg->desc = shm_desc;
		}
		efa_addr = msg->addr;
		shm_msg->addr = peer->conn->shm_fi_addr;
		ret = fi_sendmsg(efa_rdm_ep->shm_ep, shm_msg, flags);
		/* Recover the application msg */
		if (efa_desc)
			shm_msg->desc = efa_desc;
		shm_msg->addr = efa_addr;
		return ret;
	}

	return efa_rdm_msg_generic_send(efa_rdm_ep, peer, msg, 0, ofi_op_msg, flags);
}

static
ssize_t efa_rdm_msg_sendv(struct fid_ep *ep, const struct iovec *iov,
		      void **desc, size_t count, fi_addr_t dest_addr,
		      void *context)
{
	struct efa_rdm_ep *efa_rdm_ep;
	struct fi_msg msg = {0};
	struct efa_rdm_peer *peer;
	void *shm_desc[EFA_RDM_IOV_LIMIT] = {NULL};
	int ret;

	efa_rdm_ep = container_of(ep, struct efa_rdm_ep, base_ep.util_ep.ep_fid.fid);

	ret = efa_rdm_attempt_to_sync_memops_iov(efa_rdm_ep, (struct iovec *)iov, desc, count);
	if (ret)
		return ret;

	peer = efa_rdm_ep_get_peer(efa_rdm_ep, dest_addr);
	assert(peer);
	if (peer->is_local && efa_rdm_ep->shm_ep) {
		if (desc)
			efa_rdm_get_desc_for_shm(count, desc, shm_desc);
		return fi_sendv(efa_rdm_ep->shm_ep, iov, shm_desc, count, peer->conn->shm_fi_addr, context);
	}

	efa_rdm_msg_construct(&msg, iov, desc, count, dest_addr, context, 0);
	return efa_rdm_msg_generic_send(efa_rdm_ep, peer, &msg, 0, ofi_op_msg, efa_rdm_tx_flags(efa_rdm_ep));
}

static
ssize_t efa_rdm_msg_send(struct fid_ep *ep, const void *buf, size_t len,
		     void *desc, fi_addr_t dest_addr, void *context)
{
	struct iovec iov;
	struct efa_rdm_peer *peer;
	struct efa_rdm_ep *efa_rdm_ep;
	void *shm_desc[EFA_RDM_IOV_LIMIT] = {NULL};
	struct fi_msg msg = {0};
	int ret;

	iov.iov_base = (void *)buf;
	iov.iov_len = len;
	efa_rdm_ep = container_of(ep, struct efa_rdm_ep, base_ep.util_ep.ep_fid.fid);
	assert(len <= efa_rdm_ep->base_ep.max_msg_size);

	ret = efa_rdm_attempt_to_sync_memops_iov(efa_rdm_ep, &iov, &desc, 1);
	if (ret)
		return ret;

	peer = efa_rdm_ep_get_peer(efa_rdm_ep, dest_addr);
	assert(peer);
	if (peer->is_local && efa_rdm_ep->shm_ep) {
		if (desc)
			efa_rdm_get_desc_for_shm(1, &desc, shm_desc);
		return fi_send(efa_rdm_ep->shm_ep, buf, len, desc? shm_desc[0] : NULL, peer->conn->shm_fi_addr, context);
	}

	efa_rdm_msg_construct(&msg, &iov, &desc, 1, dest_addr, context, 0);
	return efa_rdm_msg_generic_send(efa_rdm_ep, peer, &msg, 0, ofi_op_msg, efa_rdm_tx_flags(efa_rdm_ep));
}

static
ssize_t efa_rdm_msg_senddata(struct fid_ep *ep, const void *buf, size_t len,
			 void *desc, uint64_t data, fi_addr_t dest_addr,
			 void *context)
{
	struct fi_msg msg = {0};
	struct iovec iov;
	struct efa_rdm_ep *efa_rdm_ep;
	struct efa_rdm_peer *peer;
	void *shm_desc[EFA_RDM_IOV_LIMIT] = {NULL};
	int ret;

	efa_rdm_ep = container_of(ep, struct efa_rdm_ep, base_ep.util_ep.ep_fid.fid);
	assert(len <= efa_rdm_ep->base_ep.max_msg_size);

	iov.iov_base = (void *)buf;
	iov.iov_len = len;

	ret = efa_rdm_attempt_to_sync_memops_iov(efa_rdm_ep, &iov, &desc, 1);
	if (ret)
		return ret;

	peer = efa_rdm_ep_get_peer(efa_rdm_ep, dest_addr);
	assert(peer);
	if (peer->is_local && efa_rdm_ep->shm_ep) {
		if (desc)
			efa_rdm_get_desc_for_shm(1, &desc, shm_desc);
		return fi_senddata(efa_rdm_ep->shm_ep, buf, len, desc? shm_desc[0] : NULL, data, peer->conn->shm_fi_addr, context);
	}

	efa_rdm_msg_construct(&msg, &iov, &desc, 1, dest_addr, context, data);
	return efa_rdm_msg_generic_send(efa_rdm_ep, peer, &msg, 0, ofi_op_msg,
				    efa_rdm_tx_flags(efa_rdm_ep) | FI_REMOTE_CQ_DATA);
}

static
ssize_t efa_rdm_msg_inject(struct fid_ep *ep, const void *buf, size_t len,
		       fi_addr_t dest_addr)
{
	struct efa_rdm_ep *efa_rdm_ep;
	struct fi_msg msg = {0};
	struct iovec iov;
	struct efa_rdm_peer *peer;

	efa_rdm_ep = container_of(ep, struct efa_rdm_ep, base_ep.util_ep.ep_fid.fid);
	assert(len <= efa_rdm_ep->base_ep.inject_msg_size);

	peer = efa_rdm_ep_get_peer(efa_rdm_ep, dest_addr);
	assert(peer);
	if (peer->is_local && efa_rdm_ep->shm_ep) {
		return fi_inject(efa_rdm_ep->shm_ep, buf, len, peer->conn->shm_fi_addr);
	}

	iov.iov_base = (void *)buf;
	iov.iov_len = len;

	efa_rdm_msg_construct(&msg, &iov, NULL, 1, dest_addr, NULL, 0);

	return efa_rdm_msg_generic_send(efa_rdm_ep, peer, &msg, 0, ofi_op_msg,
				    efa_rdm_tx_flags(efa_rdm_ep) | EFA_RDM_TXE_NO_COMPLETION | FI_INJECT);
}

static
ssize_t efa_rdm_msg_injectdata(struct fid_ep *ep, const void *buf,
			   size_t len, uint64_t data,
			   fi_addr_t dest_addr)
{
	struct efa_rdm_ep *efa_rdm_ep;
	struct fi_msg msg;
	struct iovec iov;
	struct efa_rdm_peer *peer;

	efa_rdm_ep = container_of(ep, struct efa_rdm_ep, base_ep.util_ep.ep_fid.fid);
	assert(len <= efa_rdm_ep->base_ep.inject_msg_size);

	peer = efa_rdm_ep_get_peer(efa_rdm_ep, dest_addr);
	assert(peer);
	if (peer->is_local && efa_rdm_ep->shm_ep) {
		return fi_injectdata(efa_rdm_ep->shm_ep, buf, len, data, peer->conn->shm_fi_addr);
	}

	iov.iov_base = (void *)buf;
	iov.iov_len = len;

	efa_rdm_msg_construct(&msg, &iov, NULL, 1, dest_addr, NULL, data);

	return efa_rdm_msg_generic_send(efa_rdm_ep, peer, &msg, 0, ofi_op_msg,
				    efa_rdm_tx_flags(efa_rdm_ep) | EFA_RDM_TXE_NO_COMPLETION |
				    FI_REMOTE_CQ_DATA | FI_INJECT);
}

/**
 *   Tagged send op functions
 */
static
ssize_t efa_rdm_msg_tsendmsg(struct fid_ep *ep_fid, const struct fi_msg_tagged *tmsg,
			 uint64_t flags)
{
	struct fi_msg msg = {0};
	struct efa_rdm_peer *peer;
	struct efa_rdm_ep *efa_rdm_ep;
	void *shm_desc[EFA_RDM_IOV_LIMIT] = {NULL};
	struct fi_msg_tagged *shm_tmsg;
	void **efa_desc = NULL;
	fi_addr_t efa_addr;
	int ret;

	efa_rdm_ep = container_of(ep_fid, struct efa_rdm_ep, base_ep.util_ep.ep_fid.fid);

	ret = efa_rdm_attempt_to_sync_memops_iov(efa_rdm_ep, (struct iovec *)tmsg->msg_iov, tmsg->desc, tmsg->iov_count);
	if (ret)
		return ret;

	peer = efa_rdm_ep_get_peer(efa_rdm_ep, tmsg->addr);
	assert(peer);
	if (peer->is_local && efa_rdm_ep->shm_ep) {
		shm_tmsg = (struct fi_msg_tagged *)tmsg;
		if (tmsg->desc) {
			efa_desc = tmsg->desc;
			efa_rdm_get_desc_for_shm(tmsg->iov_count, tmsg->desc, shm_desc);
			shm_tmsg->desc = shm_desc;
		}
		efa_addr = tmsg->addr;
		shm_tmsg->addr = peer->conn->shm_fi_addr;
		ret = fi_tsendmsg(efa_rdm_ep->shm_ep, shm_tmsg, flags);
		/* Recover the application msg */
		if (efa_desc)
			shm_tmsg->desc = efa_desc;
		shm_tmsg->addr = efa_addr;
		return ret;
	}

	efa_rdm_msg_construct(&msg, tmsg->msg_iov, tmsg->desc, tmsg->iov_count, tmsg->addr, tmsg->context, tmsg->data);
	return efa_rdm_msg_generic_send(efa_rdm_ep, peer, &msg, tmsg->tag, ofi_op_tagged, flags);
}

static
ssize_t efa_rdm_msg_tsendv(struct fid_ep *ep_fid, const struct iovec *iov,
		       void **desc, size_t count, fi_addr_t dest_addr,
		       uint64_t tag, void *context)
{
	struct efa_rdm_ep *efa_rdm_ep;
	struct fi_msg msg = {0};
	struct efa_rdm_peer *peer;
	void *shm_desc[EFA_RDM_IOV_LIMIT] = {NULL};
	int ret;

	efa_rdm_ep = container_of(ep_fid, struct efa_rdm_ep, base_ep.util_ep.ep_fid.fid);
	peer = efa_rdm_ep_get_peer(efa_rdm_ep, dest_addr);

	ret = efa_rdm_attempt_to_sync_memops_iov(efa_rdm_ep, (struct iovec *)iov, desc, count);
	if (ret)
		return ret;

	assert(peer);
	if (peer->is_local && efa_rdm_ep->shm_ep) {
		if (desc)
			efa_rdm_get_desc_for_shm(count, desc, shm_desc);
		return fi_tsendv(efa_rdm_ep->shm_ep, iov, desc? shm_desc : NULL, count, peer->conn->shm_fi_addr, tag, context);
	}

	efa_rdm_msg_construct(&msg, iov, desc, count, dest_addr, context, 0);
	return efa_rdm_msg_generic_send(efa_rdm_ep, peer, &msg, tag, ofi_op_tagged, efa_rdm_tx_flags(efa_rdm_ep));
}

static
ssize_t efa_rdm_msg_tsend(struct fid_ep *ep_fid, const void *buf, size_t len,
		      void *desc, fi_addr_t dest_addr, uint64_t tag,
		      void *context)
{
	struct iovec msg_iov;
	struct fi_msg msg = {0};
	struct efa_rdm_peer *peer;
	struct efa_rdm_ep *efa_rdm_ep;
	void *shm_desc[EFA_RDM_IOV_LIMIT] = {NULL};
	int ret;

	efa_rdm_ep = container_of(ep_fid, struct efa_rdm_ep, base_ep.util_ep.ep_fid.fid);
	assert(len <= efa_rdm_ep->base_ep.max_msg_size);

	msg_iov.iov_base = (void *)buf;
	msg_iov.iov_len = len;
	ret = efa_rdm_attempt_to_sync_memops_iov(efa_rdm_ep, &msg_iov, &desc, 1);
	if (ret)
		return ret;

	peer = efa_rdm_ep_get_peer(efa_rdm_ep, dest_addr);
	assert(peer);
	if (peer->is_local && efa_rdm_ep->shm_ep) {
		if (desc)
			efa_rdm_get_desc_for_shm(1, &desc, shm_desc);
		return fi_tsend(efa_rdm_ep->shm_ep, buf, len, desc? shm_desc[0] : NULL, peer->conn->shm_fi_addr, tag, context);
	}

	efa_rdm_msg_construct(&msg, &msg_iov, &desc, 1, dest_addr, context, 0);
	return efa_rdm_msg_generic_send(efa_rdm_ep, peer, &msg, tag, ofi_op_tagged, efa_rdm_tx_flags(efa_rdm_ep));
}

static
ssize_t efa_rdm_msg_tsenddata(struct fid_ep *ep_fid, const void *buf, size_t len,
			  void *desc, uint64_t data, fi_addr_t dest_addr,
			  uint64_t tag, void *context)
{
	struct fi_msg msg = {0};
	struct iovec iov;
	struct efa_rdm_ep *efa_rdm_ep;
	struct efa_rdm_peer *peer;
	void *shm_desc[EFA_RDM_IOV_LIMIT] = {NULL};
	int ret;

	efa_rdm_ep = container_of(ep_fid, struct efa_rdm_ep, base_ep.util_ep.ep_fid.fid);
	assert(len <= efa_rdm_ep->base_ep.max_msg_size);

	iov.iov_base = (void *)buf;
	iov.iov_len = len;
	ret = efa_rdm_attempt_to_sync_memops_iov(efa_rdm_ep, &iov, &desc, 1);
	if (ret)
		return ret;

	peer = efa_rdm_ep_get_peer(efa_rdm_ep, dest_addr);
	assert(peer);
	if (peer->is_local && efa_rdm_ep->shm_ep) {
		if (desc)
			efa_rdm_get_desc_for_shm(1, &desc, shm_desc);
		return fi_tsenddata(efa_rdm_ep->shm_ep, buf, len, desc? shm_desc[0] : NULL, data, peer->conn->shm_fi_addr, tag, context);
	}

	efa_rdm_msg_construct(&msg, &iov, &desc, 1, dest_addr, context, data);
	return efa_rdm_msg_generic_send(efa_rdm_ep, peer, &msg, tag, ofi_op_tagged,
				    efa_rdm_tx_flags(efa_rdm_ep) | FI_REMOTE_CQ_DATA);
}

static
ssize_t efa_rdm_msg_tinject(struct fid_ep *ep_fid, const void *buf, size_t len,
			fi_addr_t dest_addr, uint64_t tag)
{
	struct efa_rdm_ep *efa_rdm_ep;
	struct fi_msg msg = {0};
	struct iovec iov;
	struct efa_rdm_peer *peer;

	efa_rdm_ep = container_of(ep_fid, struct efa_rdm_ep, base_ep.util_ep.ep_fid.fid);
	assert(len <= efa_rdm_ep->inject_tagged_size);

	peer = efa_rdm_ep_get_peer(efa_rdm_ep, dest_addr);
	assert(peer);
	if (peer->is_local && efa_rdm_ep->shm_ep) {
		return fi_tinject(efa_rdm_ep->shm_ep, buf, len, peer->conn->shm_fi_addr, tag);
	}

	iov.iov_base = (void *)buf;
	iov.iov_len = len;

	efa_rdm_msg_construct(&msg, &iov, NULL, 1, dest_addr, NULL, 0);

	return efa_rdm_msg_generic_send(efa_rdm_ep, peer, &msg, tag, ofi_op_tagged,
				    efa_rdm_tx_flags(efa_rdm_ep) | EFA_RDM_TXE_NO_COMPLETION | FI_INJECT);
}

static
ssize_t efa_rdm_msg_tinjectdata(struct fid_ep *ep_fid, const void *buf, size_t len,
			    uint64_t data, fi_addr_t dest_addr, uint64_t tag)
{
	struct efa_rdm_ep *efa_rdm_ep;
	struct fi_msg msg = {0};
	struct iovec iov;
	struct efa_rdm_peer *peer;

	efa_rdm_ep = container_of(ep_fid, struct efa_rdm_ep, base_ep.util_ep.ep_fid.fid);
	assert(len <= efa_rdm_ep->inject_tagged_size);

	peer = efa_rdm_ep_get_peer(efa_rdm_ep, dest_addr);
	assert(peer);
	if (peer->is_local && efa_rdm_ep->shm_ep) {
		return fi_tinjectdata(efa_rdm_ep->shm_ep, buf, len, data, peer->conn->shm_fi_addr, tag);
	}

	iov.iov_base = (void *)buf;
	iov.iov_len = len;

	efa_rdm_msg_construct(&msg, &iov, NULL, 1, dest_addr, NULL, data);

	return efa_rdm_msg_generic_send(efa_rdm_ep, peer, &msg, tag, ofi_op_tagged,
				    efa_rdm_tx_flags(efa_rdm_ep) | EFA_RDM_TXE_NO_COMPLETION |
				    FI_REMOTE_CQ_DATA | FI_INJECT);
}

/**
 *  Receive functions
 */

/**
 * @brief allocate an rxe for a fi_msg in the zero-copy path.
 *        This function is used by two sided operation only.
 *
 * @param[in] ep	end point
 * @param[in] msg	fi_msg contains iov,iov_count,context for ths operation
 * @param[in] op	operation type (ofi_op_msg or ofi_op_tagged)
 * @param[in] flags	flags application used to call fi_recv/fi_trecv functions
 * @param[in] tag	tag (used only if op is ofi_op_tagged)
 * @param[in] ignore	ignore mask (used only if op is ofi_op_tagged)
 * @return		if allocation succeeded, return pointer to rxe
 * 			if allocation failed, return NULL
 */
struct efa_rdm_ope *efa_rdm_msg_alloc_rxe_zcpy(struct efa_rdm_ep *ep,
					    const struct fi_msg *msg,
					    uint32_t op, uint64_t flags,
					    uint64_t tag, uint64_t ignore)
{
	struct efa_rdm_ope *rxe;
	struct efa_rdm_peer *peer;

	if (ep->base_ep.util_ep.caps & FI_DIRECTED_RECV)
		peer = efa_rdm_ep_get_peer(ep, msg->addr);
	else
		peer = NULL;

	rxe = efa_rdm_ep_alloc_rxe(ep, peer, op);
	if (!rxe)
		return NULL;

	rxe->fi_flags = flags;
	if (op == ofi_op_tagged) {
		rxe->tag = tag;
		rxe->cq_entry.tag = tag;
		rxe->ignore = ignore;
	}

	/* Handle case where we're allocating an unexpected rxe */
	rxe->iov_count = msg->iov_count;
	if (rxe->iov_count) {
		assert(msg->msg_iov);
		memcpy(rxe->iov, msg->msg_iov, sizeof(*rxe->iov) * msg->iov_count);
		rxe->cq_entry.len = ofi_total_iov_len(rxe->iov, rxe->iov_count);
		rxe->cq_entry.buf = msg->msg_iov[0].iov_base;
	}

	if (msg->desc)
		memcpy(&rxe->desc[0], msg->desc, sizeof(*msg->desc) * msg->iov_count);
	else
		memset(&rxe->desc[0], 0, sizeof(rxe->desc));

	rxe->cq_entry.op_context = msg->context;

	return rxe;
}

/**
 * @brief allocate a RX entry for an unexpected RTM
 *
 * unexpected RTM is an RTM that was received before
 * the application (user) has called corresponding `fi_recv`
 * @param[in,out]	ep		endpoint
 * @param[in,out]	pkt_entry_ptr	pointer to the RTM pacekt
 * @param[in]		op		libfabric op type, can be either
 * 					ofi_op_msg or ofi_op_tagged
 * @returns
 * pointer to the allocated RX entry
 * IF endpoint's operation entry (ope) pool has been exhausted,
 * return NULL
 */
struct efa_rdm_ope *efa_rdm_msg_alloc_unexp_rxe_for_rtm(struct efa_rdm_ep *ep,
							struct efa_rdm_pke **pkt_entry_ptr,
							uint32_t op)
{
	struct efa_rdm_ope *rxe;
	struct efa_rdm_pke *unexp_pkt_entry;

	assert(op == ofi_op_msg || ofi_op_tagged);

	unexp_pkt_entry = efa_rdm_pke_get_unexp(pkt_entry_ptr);
	if (OFI_UNLIKELY(!unexp_pkt_entry)) {
		EFA_WARN(FI_LOG_CQ, "packet entries exhausted.\n");
		return NULL;
	}

	rxe = efa_rdm_ep_alloc_rxe(ep, unexp_pkt_entry->peer, op);
	if (OFI_UNLIKELY(!rxe))
		return NULL;

	rxe->internal_flags = 0;
	rxe->state = EFA_RDM_RXE_UNEXP;
	rxe->unexp_pkt = unexp_pkt_entry;
	efa_rdm_pke_rtm_update_rxe(unexp_pkt_entry, rxe);
	return rxe;
}

/**
 * @brief allocate a RX entry for an RTM matched with a peer_rxe
 *
 * param[in,out]	ep		endpoint
 * param[in]		pkt_entry	RTM that has been matched with a peer rx entry
 * param[in]		peer_rxe	peer RX entry that match the RTM
 * param[in]		op		libfabric op type. Possible values are: ofi_op_msg, ofi_op_tagged
 *
 * @returns
 * pointer to the RX entry.
 * If endpoint's operation entry pool has been exhausted, return NULL
 */
struct efa_rdm_ope *efa_rdm_msg_alloc_matched_rxe_for_rtm(struct efa_rdm_ep *ep,
							  struct efa_rdm_pke *pkt_entry,
							  struct fi_peer_rx_entry *peer_rxe,
							  uint32_t op)
{
	struct efa_rdm_ope *rxe;

	rxe = efa_rdm_ep_alloc_rxe(ep, pkt_entry->peer, op);
	if (OFI_UNLIKELY(!rxe))
		return NULL;

	rxe->state = EFA_RDM_RXE_MATCHED;

	efa_rdm_srx_update_rxe(peer_rxe, rxe);
	efa_rdm_pke_rtm_update_rxe(pkt_entry, rxe);
	return rxe;
}

/**
 * @brief allocate a RX entry for a non-tagged RTM
 *
 * Depend on the timing of the RTM (expected or unexpected),
 * this function will call either #efa_rdm_msg_alloc_matched_rxe_for_rtm()
 * or #efa_rdm_msg_unexp_rxe_for_rtm().
 *
 * @param[in]		ep		endpoint
 * @param[in]		pkt_entry	RTM packet entry
 *
 * @returns
 * Pointer to the allocated RX entry.
 * If endpoint's operation entry pool (ope_pool) has been exhausted,
 * return NULL
 */
struct efa_rdm_ope *
efa_rdm_msg_alloc_rxe_for_msgrtm(struct efa_rdm_ep *ep,
				 struct efa_rdm_pke **pkt_entry_ptr)
{
	struct fid_peer_srx *peer_srx;
	struct fi_peer_match_attr attr;
	struct fi_peer_rx_entry *peer_rxe;
	struct efa_rdm_peer *peer;
	struct efa_rdm_ope *rxe;
	int ret;
	int pkt_type;

	peer_srx = util_get_peer_srx(ep->peer_srx_ep);

	peer = (*pkt_entry_ptr)->peer;
	attr.addr = peer->conn->fi_addr;
	attr.msg_size = efa_rdm_pke_get_rtm_msg_length(*pkt_entry_ptr);
	attr.tag = 0;
	ret = peer_srx->owner_ops->get_msg(peer_srx, &attr, &peer_rxe);

	if (ret == FI_SUCCESS) { /* A matched rxe is found */
		rxe = efa_rdm_msg_alloc_matched_rxe_for_rtm(ep, *pkt_entry_ptr, peer_rxe, ofi_op_msg);
		if (OFI_UNLIKELY(!rxe)) {
			efa_base_ep_write_eq_error(&ep->base_ep, FI_ENOBUFS, FI_EFA_ERR_RXE_POOL_EXHAUSTED);
			return NULL;
		}
		efa_rdm_tracepoint(msg_match_expected_nontagged, rxe->msg_id,
			    (size_t) rxe->cq_entry.op_context, rxe->total_len);
	} else if (ret == -FI_ENOENT) { /* No matched rxe is found */
		/*
		 * efa_rdm_msg_alloc_unexp_rxe_for_rtm() might release pkt_entry,
		 * thus we have to use pkt_entry_ptr here
		 */
		rxe = efa_rdm_msg_alloc_unexp_rxe_for_rtm(ep, pkt_entry_ptr, ofi_op_msg);
		if (OFI_UNLIKELY(!rxe)) {
			efa_base_ep_write_eq_error(&ep->base_ep, FI_ENOBUFS, FI_EFA_ERR_RXE_POOL_EXHAUSTED);
			return NULL;
		}
		(*pkt_entry_ptr)->ope = rxe;
		peer_rxe->peer_context = (*pkt_entry_ptr);
		rxe->peer_rxe = peer_rxe;
		efa_rdm_tracepoint(msg_recv_unexpected_nontagged, rxe->msg_id,
			    (size_t) rxe->cq_entry.op_context, rxe->total_len);
	} else { /* Unexpected errors */
		EFA_WARN(FI_LOG_EP_CTRL,
			"get_msg failed, error: %d\n",
			ret);
		return NULL;
	}

	pkt_type = efa_rdm_pke_get_base_hdr(*pkt_entry_ptr)->type;
	if (efa_rdm_pkt_type_is_mulreq(pkt_type))
		efa_rdm_rxe_map_insert(&peer->rxe_map, efa_rdm_pke_get_rtm_msg_id(*pkt_entry_ptr), rxe);

	return rxe;
}

/**
 * @brief allocate a RX entry for a tagged RTM
 *
 * Depend on the timing of the RTM (expected or unexpected),
 * this function will call either #efa_rdm_msg_alloc_matched_rxe_for_rtm()
 * or #efa_rdm_msg_unexp_rxe_for_rtm().
 *
 * @param[in]		ep		endpoint
 * @param[in]		pkt_entry	RTM packet entry
 *
 * @returns
 * Pointer to the allocated RX entry.
 * If endpoint's operation entry pool (ope_pool) has been exhausted,
 * return NULL
 */
struct efa_rdm_ope *
efa_rdm_msg_alloc_rxe_for_tagrtm(struct efa_rdm_ep *ep,
				 struct efa_rdm_pke **pkt_entry_ptr)
{
	struct fid_peer_srx *peer_srx;
	struct fi_peer_match_attr attr;
	struct fi_peer_rx_entry *peer_rxe;
	struct efa_rdm_peer *peer;
	struct efa_rdm_ope *rxe;
	int ret;
	int pkt_type;

	peer = (*pkt_entry_ptr)->peer;

	peer_srx = util_get_peer_srx(ep->peer_srx_ep);
	attr.addr = peer->conn->fi_addr;
	attr.msg_size = efa_rdm_pke_get_rtm_msg_length(*pkt_entry_ptr);
	attr.tag = efa_rdm_pke_get_rtm_tag(*pkt_entry_ptr);

	ret = peer_srx->owner_ops->get_tag(peer_srx, &attr, &peer_rxe);

	if (ret == FI_SUCCESS) { /* A matched rxe is found */
		rxe = efa_rdm_msg_alloc_matched_rxe_for_rtm(ep, *pkt_entry_ptr, peer_rxe, ofi_op_tagged);
		if (OFI_UNLIKELY(!rxe)) {
			efa_base_ep_write_eq_error(&ep->base_ep, FI_ENOBUFS, FI_EFA_ERR_RXE_POOL_EXHAUSTED);
			return NULL;
		}
		efa_rdm_tracepoint(msg_match_expected_tagged, rxe->msg_id,
			    (size_t) rxe->cq_entry.op_context, rxe->total_len);
	} else if (ret == -FI_ENOENT) { /* No matched rxe is found */
		/*
		 * efa_rdm_msg_alloc_unexp_rxe_for_rtm() might release pkt_entry,
		 * thus we have to use pkt_entry_ptr here
		 */
		rxe = efa_rdm_msg_alloc_unexp_rxe_for_rtm(ep, pkt_entry_ptr, ofi_op_tagged);
		if (OFI_UNLIKELY(!rxe)) {
			efa_base_ep_write_eq_error(&ep->base_ep, FI_ENOBUFS, FI_EFA_ERR_RXE_POOL_EXHAUSTED);
			return NULL;
		}
		(*pkt_entry_ptr)->ope = rxe;

		if (efa_rdm_pke_get_base_hdr(*pkt_entry_ptr)->flags &
		    EFA_RDM_REQ_OPT_CQ_DATA_HDR) {
			peer_rxe->flags |= FI_REMOTE_CQ_DATA;
			peer_rxe->cq_data = efa_rdm_pke_get_req_cq_data(*pkt_entry_ptr);
		}

		peer_rxe->peer_context = *pkt_entry_ptr;
		rxe->peer_rxe = peer_rxe;
		efa_rdm_tracepoint(msg_recv_unexpected_tagged, rxe->msg_id,
			    (size_t) rxe->cq_entry.op_context, rxe->total_len,
			    rxe->tag, rxe->peer->conn->fi_addr);
	} else { /* Unexpected errors */
		EFA_WARN(FI_LOG_EP_CTRL,
			"get_tag failed, error: %d\n",
			ret);
		return NULL;
	}

	pkt_type = efa_rdm_pke_get_base_hdr(*pkt_entry_ptr)->type;
	if (efa_rdm_pkt_type_is_mulreq(pkt_type))
		efa_rdm_rxe_map_insert(&peer->rxe_map, efa_rdm_pke_get_rtm_msg_id(*pkt_entry_ptr), rxe);

	return rxe;
}

/*
 *     create a rxe and verify in unexpected message list
 *     else add to posted recv list
 */
static inline
ssize_t efa_rdm_msg_generic_recv(struct efa_rdm_ep *ep, const struct fi_msg *msg,
			     uint64_t tag, uint64_t ignore, uint32_t op,
			     uint64_t flags)
{
	ssize_t ret = 0;
	struct efa_rdm_ope *rxe;
	struct util_srx_ctx *srx_ctx;

	assert(msg->iov_count <= ep->base_ep.info->rx_attr->iov_limit);

	efa_perfset_start(ep, perf_efa_recv);

	EFA_DBG(FI_LOG_EP_DATA,
		"peer: %lu iov_len: %lu tag: %lx ignore: %lx op: %x flags: "
		"%lx\n",
		msg->addr, ofi_total_iov_len(msg->msg_iov, msg->iov_count), tag,
		ignore, op, flags);

	efa_rdm_tracepoint(recv_begin_msg_context, (size_t) msg->context, (size_t) msg->addr);

	ret = efa_rdm_attempt_to_sync_memops_iov(ep, (struct iovec *)msg->msg_iov, msg->desc, msg->iov_count);
	if (ret)
		return ret;

	if (ep->use_zcpy_rx) {
		srx_ctx = efa_rdm_ep_get_peer_srx_ctx(ep);
		ofi_genlock_lock(srx_ctx->lock);
		rxe = efa_rdm_msg_alloc_rxe_zcpy(ep, msg, op, flags, tag, ignore);
		if (OFI_UNLIKELY(!rxe)) {
			ret = -FI_EAGAIN;
			ofi_genlock_unlock(srx_ctx->lock);
			goto out;
		}

		ret = efa_rdm_ep_post_user_recv_buf(ep, rxe, flags);
		if (OFI_UNLIKELY(ret))
			efa_rdm_rxe_release(rxe);

		ofi_genlock_unlock(srx_ctx->lock);
	} else if (op == ofi_op_tagged) {
		ret = util_srx_generic_trecv(ep->peer_srx_ep, msg->msg_iov, msg->desc,
					     msg->iov_count, msg->addr, msg->context,
					     tag, ignore, flags);
	} else {
		ret = util_srx_generic_recv(ep->peer_srx_ep, msg->msg_iov, msg->desc,
				            msg->iov_count, msg->addr, msg->context, flags);
	}

out:
	efa_perfset_end(ep, perf_efa_recv);
	return ret;
}

/**
 *   Non-tagged receive ops
 */
static
ssize_t efa_rdm_msg_recvmsg(struct fid_ep *ep_fid, const struct fi_msg *msg,
			uint64_t flags)
{
	struct efa_rdm_ep *ep;

	ep = container_of(ep_fid, struct efa_rdm_ep, base_ep.util_ep.ep_fid.fid);

	/*
	 * For efa_rdm_msg_recvmsg (trecvmsg), it should pass application
	 * flags |= util_ep.rx_msg_flags, which will have NO FI_COMPLETION
	 * when application binds rx cq with FI_SELECTIVE_COMPLETION,
	 * and does not have FI_COMPLETION in the flags of fi_recvmsg.
	 */
	return efa_rdm_msg_generic_recv(ep, msg, 0, 0, ofi_op_msg, flags | ep->base_ep.util_ep.rx_msg_flags);
}

static
ssize_t efa_rdm_msg_recv(struct fid_ep *ep_fid, void *buf, size_t len,
		     void *desc, fi_addr_t src_addr, void *context)
{
	struct fi_msg msg;
	struct iovec iov;
	struct efa_rdm_ep *ep;

	ep = container_of(ep_fid, struct efa_rdm_ep, base_ep.util_ep.ep_fid.fid);

	iov.iov_base = buf;
	iov.iov_len = len;

	efa_rdm_msg_construct(&msg, &iov, &desc, 1, src_addr, context, 0);
	return efa_rdm_msg_generic_recv(ep, &msg, 0, 0, ofi_op_msg,
					efa_rdm_rx_flags(ep) | ep->base_ep.util_ep.rx_msg_flags);
}

static
ssize_t efa_rdm_msg_recvv(struct fid_ep *ep_fid, const struct iovec *iov,
		      void **desc, size_t count, fi_addr_t src_addr,
		      void *context)
{
	struct fi_msg msg;
	struct efa_rdm_ep *ep;

	ep = container_of(ep_fid, struct efa_rdm_ep, base_ep.util_ep.ep_fid.fid);

	efa_rdm_msg_construct(&msg, iov, desc, count, src_addr, context, 0);
	return efa_rdm_msg_generic_recv(ep, &msg, 0, 0, ofi_op_msg,
					efa_rdm_rx_flags(ep) | ep->base_ep.util_ep.rx_msg_flags);
}

/**
 *   Tagged receive ops functions
 */
static
ssize_t efa_rdm_msg_trecv(struct fid_ep *ep_fid, void *buf, size_t len, void *desc,
		      fi_addr_t src_addr, uint64_t tag, uint64_t ignore,
		      void *context)
{
	struct fi_msg msg;
	struct iovec iov;
	struct efa_rdm_ep *ep;

	ep = container_of(ep_fid, struct efa_rdm_ep, base_ep.util_ep.ep_fid.fid);

	iov.iov_base = (void *)buf;
	iov.iov_len = len;

	efa_rdm_msg_construct(&msg, &iov, &desc, 1, src_addr, context, 0);
	return efa_rdm_msg_generic_recv(ep, &msg, tag, ignore, ofi_op_tagged, efa_rdm_rx_flags(ep));
}

static
ssize_t efa_rdm_msg_trecvv(struct fid_ep *ep_fid, const struct iovec *iov,
		       void **desc, size_t count, fi_addr_t src_addr,
		       uint64_t tag, uint64_t ignore, void *context)
{
	struct fi_msg msg;
	struct efa_rdm_ep *ep;

	ep = container_of(ep_fid, struct efa_rdm_ep, base_ep.util_ep.ep_fid.fid);

	efa_rdm_msg_construct(&msg, iov, desc, count, src_addr, context, 0);
	return efa_rdm_msg_generic_recv(ep, &msg, tag, ignore, ofi_op_tagged, efa_rdm_rx_flags(ep));
}

static
ssize_t efa_rdm_msg_trecvmsg(struct fid_ep *ep_fid, const struct fi_msg_tagged *tmsg,
			 uint64_t flags)
{
	struct fi_msg msg;
	struct efa_rdm_ep *ep;

	ep = container_of(ep_fid, struct efa_rdm_ep, base_ep.util_ep.ep_fid.fid);

	/*
	 * For efa_rdm_msg_recvmsg (trecvmsg), it should pass application
	 * flags |= util_ep.rx_msg_flags, which will have NO FI_COMPLETION
	 * when application binds rx cq with FI_SELECTIVE_COMPLETION,
	 * and does not have FI_COMPLETION in the flags of fi_recvmsg.
	 */

	efa_rdm_msg_construct(&msg, tmsg->msg_iov, tmsg->desc, tmsg->iov_count, tmsg->addr, tmsg->context, tmsg->data);
	return efa_rdm_msg_generic_recv(ep, &msg, tmsg->tag, tmsg->ignore,
				   ofi_op_tagged, flags | ep->base_ep.util_ep.rx_msg_flags);
}

/**
 * Ops structures used by #efa_rdm_ep_open()
 */
struct fi_ops_msg efa_rdm_msg_ops = {
	.size = sizeof(struct fi_ops_msg),
	.send = efa_rdm_msg_send,
	.sendv = efa_rdm_msg_sendv,
	.sendmsg = efa_rdm_msg_sendmsg,
	.senddata = efa_rdm_msg_senddata,
	.inject = efa_rdm_msg_inject,
	.injectdata = efa_rdm_msg_injectdata,
	.recv = efa_rdm_msg_recv,
	.recvv = efa_rdm_msg_recvv,
	.recvmsg = efa_rdm_msg_recvmsg,
};

struct fi_ops_tagged efa_rdm_msg_tagged_ops = {
	.size = sizeof(struct fi_ops_tagged),
	.send = efa_rdm_msg_tsend,
	.sendv = efa_rdm_msg_tsendv,
	.sendmsg = efa_rdm_msg_tsendmsg,
	.senddata = efa_rdm_msg_tsenddata,
	.inject = efa_rdm_msg_tinject,
	.injectdata = efa_rdm_msg_tinjectdata,
	.recv = efa_rdm_msg_trecv,
	.recvv = efa_rdm_msg_trecvv,
	.recvmsg = efa_rdm_msg_trecvmsg,
};
