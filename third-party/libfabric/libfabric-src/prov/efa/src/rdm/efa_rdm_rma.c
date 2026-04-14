/* Copyright Amazon.com, Inc. or its affiliates. All rights reserved. */
/* SPDX-License-Identifier: BSD-2-Clause OR GPL-2.0-only */

#include <stdlib.h>
#include <string.h>
#include <ofi_mem.h>
#include <ofi_iov.h>
#include "efa.h"

#include "efa_rdm_msg.h"
#include "efa_rdm_rma.h"
#include "efa_rdm_pke_cmd.h"
#include "efa_cntr.h"
#include "efa_rdm_tracepoint.h"

int efa_rdm_rma_verified_copy_iov(struct efa_rdm_ep *ep, struct efa_rma_iov *rma,
			      size_t count, uint32_t flags,
			      struct iovec *iov, void **desc)
{
	void *context;
	struct efa_mr *efa_mr;
	int i, ret;

	for (i = 0; i < count; i++) {
		ofi_genlock_lock(&efa_rdm_ep_domain(ep)->util_domain.lock);
		ret = ofi_mr_map_verify(&efa_rdm_ep_domain(ep)->util_domain.mr_map,
					(uintptr_t *)(&rma[i].addr),
					rma[i].len, rma[i].key, flags,
					&context);
		efa_mr = context;
		desc[i] = fi_mr_desc(&efa_mr->mr_fid);
		ofi_genlock_unlock(&efa_rdm_ep_domain(ep)->util_domain.lock);
		if (ret) {
			EFA_WARN(FI_LOG_EP_CTRL,
				"MR verification failed (%s), addr: %lx key: %ld\n",
				fi_strerror(-ret), rma[i].addr, rma[i].key);
			return ret;
		}

		iov[i].iov_base = (void *)rma[i].addr;
		iov[i].iov_len = rma[i].len;
	}
	return 0;
}


struct efa_rdm_ope *
efa_rdm_rma_alloc_txe(struct efa_rdm_ep *efa_rdm_ep,
		      struct efa_rdm_peer *peer,
		      const struct fi_msg_rma *msg_rma,
		      uint32_t op,
		      uint64_t flags)
{
	struct efa_rdm_ope *txe;
	struct fi_msg msg;

	txe = ofi_buf_alloc(efa_rdm_ep->ope_pool);
	if (OFI_UNLIKELY(!txe)) {
		EFA_DBG(FI_LOG_EP_CTRL, "TX entries exhausted.\n");
		return NULL;
	}

	msg.addr = msg_rma->addr;
	msg.msg_iov = msg_rma->msg_iov;
	msg.context = msg_rma->context;
	msg.iov_count = msg_rma->iov_count;
	msg.data = msg_rma->data;
	msg.desc = msg_rma->desc;
	efa_rdm_txe_construct(txe, efa_rdm_ep, peer, &msg, op, flags);

	assert(msg_rma->rma_iov_count > 0);
	assert(msg_rma->rma_iov);
	txe->rma_iov_count = msg_rma->rma_iov_count;
	memcpy(txe->rma_iov, msg_rma->rma_iov,
	       sizeof(struct fi_rma_iov) * msg_rma->rma_iov_count);

	dlist_insert_tail(&txe->ep_entry, &efa_rdm_ep->txe_list);
	return txe;
}

/* rma_read functions */
ssize_t efa_rdm_rma_post_efa_emulated_read(struct efa_rdm_ep *ep, struct efa_rdm_ope *txe)
{
	int err;

#if ENABLE_DEBUG
	dlist_insert_tail(&txe->pending_recv_entry,
			  &ep->ope_recv_list);
	ep->pending_recv_counter++;
#endif

	if (txe->total_len < ep->mtu_size - sizeof(struct efa_rdm_readrsp_hdr)) {
		err = efa_rdm_ope_post_send(txe, EFA_RDM_SHORT_RTR_PKT);
	} else {
		assert(efa_env.tx_min_credits > 0);
		txe->window = MIN(txe->total_len,
				       efa_env.tx_min_credits * ep->max_data_payload_size);
		err = efa_rdm_ope_post_send(txe, EFA_RDM_LONGCTS_RTR_PKT);
	}

	if (OFI_UNLIKELY(err)) {
#if ENABLE_DEBUG
		dlist_remove(&txe->pending_recv_entry);
		ep->pending_recv_counter--;
#endif
	}

	return err;
}

/**
 * @brief Post an rma read request for a given ep and tx entry
 *
 * @param ep efa rdm ep
 * @param txe tx entry
 * @return int 0 on success, negative integer on failure.
 */
ssize_t efa_rdm_rma_post_read(struct efa_rdm_ep *ep, struct efa_rdm_ope *txe)
{
	bool use_device_read = false;
	int use_p2p;
	ssize_t ret, err;

	/*
	 * A handshake is required to choose the correct protocol (whether to use device read).
	 * For local read (read from self ep), such handshake is not needed because we only
	 * need to check the local ep's capabilities.
	 */
	if (!ep->homogeneous_peers && !(txe->peer->is_self) && !(txe->peer->flags & EFA_RDM_PEER_HANDSHAKE_RECEIVED))
		return efa_rdm_ep_enforce_handshake_for_txe(ep, txe);

	/* Check p2p support. Cannot use device read when p2p is not available. */
	err = efa_rdm_ep_use_p2p(ep, txe->desc[0]);
	if (err < 0)
		return err;

	use_p2p = err;

	if (use_p2p && efa_rdm_interop_rdma_read(ep, txe->peer)) {
		/* RDMA read interoperability check also checks domain.use_device_rdma,
		 * so we do not check it here
		 */
		use_device_read = true;
	} else if (efa_mr_is_neuron(txe->desc[0])) {
		EFA_WARN(FI_LOG_EP_CTRL, "rdma read is required to post read for AWS trainium memory\n");
		return -FI_EOPNOTSUPP;
	}

	if (use_device_read) {
		ret = efa_rdm_ope_prepare_to_post_read(txe);
		if (ret)
			return ret;

		ret = efa_rdm_ope_post_read(txe);
		if (OFI_UNLIKELY(ret)) {
			if (ret == -FI_ENOBUFS)
				ret = -FI_EAGAIN;
		}
	} else {
		ret = efa_rdm_rma_post_efa_emulated_read(ep, txe);
	}

	return ret;
}

ssize_t efa_rdm_rma_generic_readmsg(struct efa_rdm_ep *efa_rdm_ep, struct efa_rdm_peer *peer, const struct fi_msg_rma *msg, uint64_t flags)
{
	ssize_t err;
	struct efa_rdm_ope *txe = NULL;
	struct util_srx_ctx *srx_ctx;

	efa_rdm_tracepoint(read_begin_msg_context,
			   (size_t) msg->context, (size_t) msg->addr);

	EFA_DBG(FI_LOG_EP_DATA,
	       "read iov_len: %lu flags: %lx\n",
	       ofi_total_iov_len(msg->msg_iov, msg->iov_count),
	       flags);

	srx_ctx = efa_rdm_ep_get_peer_srx_ctx(efa_rdm_ep);

	efa_perfset_start(efa_rdm_ep, perf_efa_tx);
	ofi_genlock_lock(srx_ctx->lock);

	assert(peer);

	if (peer->flags & EFA_RDM_PEER_IN_BACKOFF) {
		err = -FI_EAGAIN;
		goto out;
	}

	txe = efa_rdm_rma_alloc_txe(efa_rdm_ep, peer, msg, ofi_op_read_req, flags);
	if (OFI_UNLIKELY(!txe)) {
		err = -FI_EAGAIN;
		goto out;
	}

	err = efa_rdm_rma_post_read(efa_rdm_ep, txe);

out:
	if (OFI_UNLIKELY(err && txe))
		efa_rdm_txe_release(txe);

	ofi_genlock_unlock(srx_ctx->lock);
	efa_perfset_end(efa_rdm_ep, perf_efa_tx);
	return err;
}

ssize_t efa_rdm_rma_readmsg(struct fid_ep *ep, const struct fi_msg_rma *msg, uint64_t flags)
{
	ssize_t err;
	struct efa_rdm_ep *efa_rdm_ep;
	struct efa_rdm_peer *peer;
	fi_addr_t tmp_addr;
	struct fi_msg_rma *msg_clone;
	void *shm_desc[EFA_RDM_IOV_LIMIT];
	void **tmp_desc;

	efa_rdm_ep = container_of(ep, struct efa_rdm_ep, base_ep.util_ep.ep_fid.fid);
	err = efa_rdm_ep_cap_check_rma(efa_rdm_ep);
	if (err)
		return err;

	err = efa_rdm_attempt_to_sync_memops_iov(efa_rdm_ep, (struct iovec *)msg->msg_iov, msg->desc, msg->iov_count);
	if (err)
		return err;

	assert(msg->iov_count <= efa_rdm_ep->base_ep.info->tx_attr->iov_limit);

	peer = efa_rdm_ep_get_peer(efa_rdm_ep, msg->addr);
	assert(peer);

	if (peer->is_local && efa_rdm_ep->shm_ep) {
		tmp_addr = msg->addr;
		tmp_desc = msg->desc;
		msg_clone = (struct fi_msg_rma *)msg;
		msg_clone->addr = peer->conn->shm_fi_addr;
		if (msg->desc) {
			efa_rdm_get_desc_for_shm(msg->iov_count, msg->desc, shm_desc);
			msg_clone->desc = shm_desc;
		} else {
			msg_clone->desc = NULL;
		}
		err = fi_readmsg(efa_rdm_ep->shm_ep, msg_clone, flags);
		msg_clone->addr = tmp_addr;
		msg_clone->desc = tmp_desc;
		return err;
	}

	return efa_rdm_rma_generic_readmsg(efa_rdm_ep, peer, msg, flags);

}

ssize_t efa_rdm_rma_readv(struct fid_ep *ep, const struct iovec *iov, void **desc,
		      size_t iov_count, fi_addr_t src_addr, uint64_t addr,
		      uint64_t key, void *context)
{
	struct fi_rma_iov rma_iov;
	struct fi_msg_rma msg;
	struct efa_rdm_peer *peer;
	struct efa_rdm_ep *efa_rdm_ep;
	void *shm_desc[EFA_RDM_IOV_LIMIT] = {NULL};
	int err;

	efa_rdm_ep = container_of(ep, struct efa_rdm_ep, base_ep.util_ep.ep_fid.fid);
	err = efa_rdm_ep_cap_check_rma(efa_rdm_ep);
	if (err)
		return err;

	err = efa_rdm_attempt_to_sync_memops_iov(efa_rdm_ep, (struct iovec *)iov, desc, iov_count);
	if (err)
		return err;

	peer = efa_rdm_ep_get_peer(efa_rdm_ep, src_addr);
	assert(peer);
	if (peer->is_local && efa_rdm_ep->shm_ep) {
		if (desc)
			efa_rdm_get_desc_for_shm(iov_count, desc, shm_desc);

		return fi_readv(efa_rdm_ep->shm_ep, iov, desc? shm_desc : NULL, iov_count, peer->conn->shm_fi_addr, addr, key, context);
	}

	EFA_SETUP_RMA_IOV(rma_iov, addr, ofi_total_iov_len(iov, iov_count), key);

	memset(&msg, 0, sizeof(msg));
	EFA_SETUP_MSG_RMA(msg, iov, desc, iov_count, src_addr, &rma_iov, 1, context, 0);

	return efa_rdm_rma_generic_readmsg(efa_rdm_ep, peer, &msg, 0);
}

ssize_t efa_rdm_rma_read(struct fid_ep *ep, void *buf, size_t len, void *desc,
		     fi_addr_t src_addr, uint64_t addr, uint64_t key,
		     void *context)
{
	struct iovec iov;
	struct efa_rdm_peer *peer;
	struct efa_rdm_ep *efa_rdm_ep;
	struct fi_msg_rma msg;
	struct fi_rma_iov rma_iov;
	void *shm_desc[EFA_RDM_IOV_LIMIT] = {NULL};
	int err;

	efa_rdm_ep = container_of(ep, struct efa_rdm_ep, base_ep.util_ep.ep_fid.fid);
	assert(len <= efa_rdm_ep->base_ep.max_rma_size);
	err = efa_rdm_ep_cap_check_rma(efa_rdm_ep);
	if (err)
		return err;

	iov.iov_base = (void *)buf;
	iov.iov_len = len;
	err = efa_rdm_attempt_to_sync_memops_iov(efa_rdm_ep, &iov, &desc, 1);
	if (err)
		return err;

	peer = efa_rdm_ep_get_peer(efa_rdm_ep, src_addr);
	assert(peer);
	if (peer->is_local && efa_rdm_ep->shm_ep) {
		if (desc)
			efa_rdm_get_desc_for_shm(1, &desc, shm_desc);

		return fi_read(efa_rdm_ep->shm_ep, buf, len, desc? shm_desc[0] : NULL, peer->conn->shm_fi_addr, addr, key, context);
	}

	EFA_SETUP_RMA_IOV(rma_iov, addr, ofi_total_iov_len(&iov, 1), key);

	memset(&msg, 0, sizeof(msg));
	EFA_SETUP_MSG_RMA(msg, &iov, &desc, 1, src_addr, &rma_iov, 1, context, 0);

	return efa_rdm_rma_generic_readmsg(efa_rdm_ep, peer, &msg, 0);
}

/**
 * @brief Decide if we should issue this WRITE using rdma-core, or via emulation.
 *
 * This function could force a handshake with peer, otherwise ep and peer will be
 * read-only.
 *
 * @param ep[in,out]		The endpoint.
 * @param txe[in]		The ope context for this write.
 * @param peer[in,out]		The peer we will be writing to.
 * @return true			When WRITE can be done using RDMA_WRITE
 * @return false		When WRITE should be emulated with SEND's
 */
static inline
bool efa_rdm_rma_should_write_using_rdma(struct efa_rdm_ep *ep, struct efa_rdm_ope *txe, struct efa_rdm_peer *peer)
{
	/*
	 * Because EFA is unordered and EFA iov descriptions can be more
	 * expressive than the IBV sge's, we only implement
	 * FI_REMOTE_CQ_DATA using RDMA_WRITE_WITH_IMM when a single iov
	 * is given, otherwise we use sends to emulate it.
	 */
	if ((txe->fi_flags & FI_REMOTE_CQ_DATA) &&
	    (txe->iov_count > 1 || txe->rma_iov_count > 1))
		return false;

	return efa_both_support_rdma_write(ep, peer);
}

/**
 * @brief Post a WRITE described the txe
 *
 * @param ep		The endpoint.
 * @param txe	The ope context for this write.
 * @return On success return 0, otherwise return a negative libfabric error code.
 */
ssize_t efa_rdm_rma_post_write(struct efa_rdm_ep *ep, struct efa_rdm_ope *txe)
{
	ssize_t err;
	bool delivery_complete_requested;
	int ctrl_type, iface, use_p2p;
	size_t max_eager_rtw_data_size;
	char err_msg[EFA_ERROR_MSG_BUFFER_LENGTH] = {0};

	/*
	 * A handshake is required to choose the correct protocol (whether to use device write/read).
	 * For local write (writing it self), this handshake is not required because we only need to
	 * check one-side capability
	 */
	if (!ep->homogeneous_peers && !(txe->peer->is_self) && !(txe->peer->flags & EFA_RDM_PEER_HANDSHAKE_RECEIVED))
		return efa_rdm_ep_enforce_handshake_for_txe(ep, txe);

	if (efa_rdm_rma_should_write_using_rdma(ep, txe, txe->peer)) {
		/**
		 * Unsolicited write recv is a feature that makes rdma-write with
		 * imm not consume an rx buffer on the responder side, and this
		 * feature requires consistent support status on both sides.
		 */
		if (!ep->homogeneous_peers && (txe->fi_flags & FI_REMOTE_CQ_DATA) && 
			(efa_rdm_ep_support_unsolicited_write_recv(ep) != efa_rdm_peer_support_unsolicited_write_recv(txe->peer))) {
			(void) efa_rdm_construct_msg_with_local_and_peer_information(ep, txe->peer, err_msg, "", EFA_ERROR_MSG_BUFFER_LENGTH);
			EFA_WARN(FI_LOG_EP_DATA,
				"Inconsistent support status detected on unsolicited write recv.\n"
				"My support status: %d, peer support status: %d. %s.\n"
				"This is usually caused by inconsistent efa driver, libfabric, or rdma-core versions.\n"
				"Please use consistent software versions on both hosts, or disable the unsolicited write "
				"recv feature by setting environment variable FI_EFA_USE_UNSOLICITED_WRITE_RECV=0\n",
				ep->base_ep.qp->unsolicited_write_recv_enabled, efa_rdm_peer_support_unsolicited_write_recv(txe->peer),
				err_msg);
			return -FI_EOPNOTSUPP;
		}
		efa_rdm_ope_prepare_to_post_write(txe);
		return efa_rdm_ope_post_remote_write(txe);
	}

	delivery_complete_requested = txe->fi_flags & FI_DELIVERY_COMPLETE;
	if (delivery_complete_requested) {
		/*
		 * Because delivery complete is defined as an extra
		 * feature, the receiver might not support it.
		 *
		 * The sender cannot send with FI_DELIVERY_COMPLETE
		 * if the peer is not able to handle it.
		 *
		 * handshake is already made now since we enforce
		 * handshake for write earlier.
		 */

		if (!ep->homogeneous_peers && !(txe->peer->is_self) && !efa_rdm_peer_support_delivery_complete(txe->peer))
			return -FI_EOPNOTSUPP;

		max_eager_rtw_data_size = efa_rdm_txe_max_req_data_capacity(ep, txe, EFA_RDM_DC_EAGER_RTW_PKT);
	} else {
		max_eager_rtw_data_size = efa_rdm_txe_max_req_data_capacity(ep, txe, EFA_RDM_EAGER_RTW_PKT);
	}

	err = efa_rdm_ep_use_p2p(ep, txe->desc[0]);
	if (err < 0)
		return err;

	use_p2p = err;

	iface = txe->desc[0] ? ((struct efa_mr*) txe->desc[0])->peer.iface : FI_HMEM_SYSTEM;

	if (use_p2p &&
	    txe->total_len >= g_efa_hmem_info[iface].min_read_write_size &&
	    efa_rdm_interop_rdma_read(ep, txe->peer) &&
	    (txe->desc[0] || efa_is_cache_available(efa_rdm_ep_domain(ep)))) {
		err = efa_rdm_ope_post_send(txe, EFA_RDM_LONGREAD_RTW_PKT);
		if (err != -FI_ENOMEM)
			return err;
		/*
		 * If read write protocol failed due to memory registration, fall back to use long
		 * message protocol
		 */
	}

	if (txe->total_len <= max_eager_rtw_data_size) {
		ctrl_type = delivery_complete_requested ? EFA_RDM_DC_EAGER_RTW_PKT : EFA_RDM_EAGER_RTW_PKT;
		return efa_rdm_ope_post_send(txe, ctrl_type);
	}

	ctrl_type = delivery_complete_requested ? EFA_RDM_DC_LONGCTS_RTW_PKT : EFA_RDM_LONGCTS_RTW_PKT;
	return efa_rdm_ope_post_send(txe, ctrl_type);
}

static inline ssize_t efa_rdm_rma_generic_writemsg(struct efa_rdm_ep *efa_rdm_ep,
			 struct efa_rdm_peer *peer,
			 const struct fi_msg_rma *msg,
			 uint64_t flags)
{
	ssize_t err;
	struct efa_rdm_ope *txe;
	struct util_srx_ctx *srx_ctx;

	efa_rdm_tracepoint(write_begin_msg_context,
			   (size_t) msg->context, (size_t) msg->addr);

	efa_perfset_start(efa_rdm_ep, perf_efa_tx);

	EFA_DBG(FI_LOG_EP_DATA,
	       "write iov_len %lu flags: %lx\n",
	       ofi_total_iov_len(msg->msg_iov, msg->iov_count),
	       flags);

	if (peer->flags & EFA_RDM_PEER_IN_BACKOFF)
		return -FI_EAGAIN;

	srx_ctx = efa_rdm_ep_get_peer_srx_ctx(efa_rdm_ep);
	ofi_genlock_lock(srx_ctx->lock);

	txe = efa_rdm_rma_alloc_txe(efa_rdm_ep, peer, msg, ofi_op_write, flags);
	if (OFI_UNLIKELY(!txe)) {
		err = -FI_EAGAIN;
		goto out;
	}

	err = efa_rdm_rma_post_write(efa_rdm_ep, txe);
	if (OFI_UNLIKELY(err)) {
		efa_rdm_txe_release(txe);
	}
out:
	ofi_genlock_unlock(srx_ctx->lock);
	efa_perfset_end(efa_rdm_ep, perf_efa_tx);
	return err;
}

ssize_t efa_rdm_rma_writemsg(struct fid_ep *ep,
			 const struct fi_msg_rma *msg,
			 uint64_t flags)
{
	struct efa_rdm_peer *peer;
	struct efa_rdm_ep *efa_rdm_ep;
	fi_addr_t tmp_addr;
	struct fi_msg_rma *msg_clone;
	void *shm_desc[EFA_RDM_IOV_LIMIT];
	void **tmp_desc;
	int err;

	efa_rdm_ep = container_of(ep, struct efa_rdm_ep, base_ep.util_ep.ep_fid.fid);
	err = efa_rdm_ep_cap_check_rma(efa_rdm_ep);
	if (err)
		return err;

	err = efa_rdm_attempt_to_sync_memops_iov(efa_rdm_ep, (struct iovec *)msg->msg_iov, msg->desc, msg->iov_count);
	if (err)
		return err;

	assert(msg->iov_count <= efa_rdm_ep->base_ep.info->tx_attr->iov_limit);

	peer = efa_rdm_ep_get_peer(efa_rdm_ep, msg->addr);
	assert(peer);

	if (peer->is_local && efa_rdm_ep->shm_ep) {
		tmp_addr = msg->addr;
		tmp_desc = msg->desc;
		msg_clone = (struct fi_msg_rma *)msg;
		msg_clone->addr = peer->conn->shm_fi_addr;
		if (msg->desc) {
			efa_rdm_get_desc_for_shm(msg->iov_count, msg->desc, shm_desc);
			msg_clone->desc = shm_desc;
		} else {
			msg_clone->desc = NULL;
		}
		err = fi_writemsg(efa_rdm_ep->shm_ep, msg, flags);
		msg_clone->addr = tmp_addr;
		msg_clone->desc = tmp_desc;
		return err;
	}

	return efa_rdm_rma_generic_writemsg(efa_rdm_ep, peer, msg, flags);
}

ssize_t efa_rdm_rma_writev(struct fid_ep *ep, const struct iovec *iov, void **desc,
		       size_t iov_count, fi_addr_t dest_addr, uint64_t addr,
		       uint64_t key, void *context)
{
	struct fi_rma_iov rma_iov;
	struct fi_msg_rma msg;
	struct efa_rdm_peer *peer;
	struct efa_rdm_ep *efa_rdm_ep;
	void *shm_desc[EFA_RDM_IOV_LIMIT] = {NULL};
	int err;

	efa_rdm_ep = container_of(ep, struct efa_rdm_ep, base_ep.util_ep.ep_fid.fid);
	err = efa_rdm_ep_cap_check_rma(efa_rdm_ep);
	if (err)
		return err;

	err = efa_rdm_attempt_to_sync_memops_iov(efa_rdm_ep, (struct iovec *)iov, desc, iov_count);
	if (err)
		return err;

	peer = efa_rdm_ep_get_peer(efa_rdm_ep, dest_addr);
	assert(peer);
	if (peer->is_local && efa_rdm_ep->shm_ep) {
		if (desc)
			efa_rdm_get_desc_for_shm(iov_count, desc, shm_desc);
		return fi_writev(efa_rdm_ep->shm_ep, iov, desc? shm_desc : NULL, iov_count, peer->conn->shm_fi_addr, addr, key, context);
	}

	EFA_SETUP_RMA_IOV(rma_iov, addr, ofi_total_iov_len(iov, iov_count), key);

	memset(&msg, 0, sizeof(msg));
	EFA_SETUP_MSG_RMA(msg, iov, desc, iov_count, dest_addr, &rma_iov, 1, context, 0);

	return efa_rdm_rma_generic_writemsg(efa_rdm_ep, peer, &msg, 0);
}

ssize_t efa_rdm_rma_write(struct fid_ep *ep, const void *buf, size_t len, void *desc,
		      fi_addr_t dest_addr, uint64_t addr, uint64_t key,
		      void *context)
{
	struct iovec iov;
	struct fi_rma_iov rma_iov;
	struct fi_msg_rma msg;
	struct efa_rdm_peer *peer;
	struct efa_rdm_ep *efa_rdm_ep;
	void *shm_desc[EFA_RDM_IOV_LIMIT] = {NULL};
	int err;

	efa_rdm_ep = container_of(ep, struct efa_rdm_ep, base_ep.util_ep.ep_fid.fid);
	assert(len <= efa_rdm_ep->base_ep.max_rma_size);
	err = efa_rdm_ep_cap_check_rma(efa_rdm_ep);
	if (err)
		return err;

	iov.iov_base = (void *)buf;
	iov.iov_len = len;
	err = efa_rdm_attempt_to_sync_memops_iov(efa_rdm_ep, &iov, &desc, 1);
	if (err)
		return err;

	peer = efa_rdm_ep_get_peer(efa_rdm_ep, dest_addr);
	assert(peer);
	if (peer->is_local && efa_rdm_ep->shm_ep) {
		if (desc)
			efa_rdm_get_desc_for_shm(1, &desc, shm_desc);
		return fi_write(efa_rdm_ep->shm_ep, buf, len, desc? shm_desc[0] : NULL, peer->conn->shm_fi_addr, addr, key, context);
	}

	EFA_SETUP_RMA_IOV(rma_iov, addr, ofi_total_iov_len(&iov, 1), key);

	memset(&msg, 0, sizeof(msg));
	EFA_SETUP_MSG_RMA(msg, &iov, &desc, 1, dest_addr, &rma_iov, 1, context, 0);

	return efa_rdm_rma_generic_writemsg(efa_rdm_ep, peer, &msg, 0);
}

ssize_t efa_rdm_rma_writedata(struct fid_ep *ep, const void *buf, size_t len,
			  void *desc, uint64_t data, fi_addr_t dest_addr,
			  uint64_t addr, uint64_t key, void *context)
{
	struct iovec iov;
	struct fi_rma_iov rma_iov;
	struct fi_msg_rma msg;
	struct efa_rdm_peer *peer;
	struct efa_rdm_ep *efa_rdm_ep;
	void *shm_desc[EFA_RDM_IOV_LIMIT] = {NULL};
	int err;

	efa_rdm_ep = container_of(ep, struct efa_rdm_ep, base_ep.util_ep.ep_fid.fid);
	assert(len <= efa_rdm_ep->base_ep.max_rma_size);
	err = efa_rdm_ep_cap_check_rma(efa_rdm_ep);
	if (err)
		return err;

	iov.iov_base = (void *)buf;
	iov.iov_len = len;
	err = efa_rdm_attempt_to_sync_memops_iov(efa_rdm_ep, &iov, &desc, 1);
	if (err)
		return err;

	peer = efa_rdm_ep_get_peer(efa_rdm_ep, dest_addr);
	assert(peer);
	if (peer->is_local && efa_rdm_ep->shm_ep) {
		if (desc)
			efa_rdm_get_desc_for_shm(1, &desc, shm_desc);
		return fi_writedata(efa_rdm_ep->shm_ep, buf, len, desc? shm_desc[0] : NULL, data, peer->conn->shm_fi_addr, addr, key, context);
	}

	EFA_SETUP_RMA_IOV(rma_iov, addr, len, key);

	memset(&msg, 0, sizeof(msg));
	EFA_SETUP_MSG_RMA(msg, &iov, &desc, 1, dest_addr, &rma_iov, 1, context, data);

	return efa_rdm_rma_generic_writemsg(efa_rdm_ep, peer, &msg, FI_REMOTE_CQ_DATA);
}

ssize_t efa_rdm_rma_inject_write(struct fid_ep *ep, const void *buf, size_t len,
			     fi_addr_t dest_addr, uint64_t addr, uint64_t key)
{
	struct fi_msg_rma msg;
	struct iovec iov;
	struct fi_rma_iov rma_iov;
	struct efa_rdm_ep *efa_rdm_ep;
	struct efa_rdm_peer *peer;
	int err;

	efa_rdm_ep = container_of(ep, struct efa_rdm_ep, base_ep.util_ep.ep_fid.fid);
	assert(len <= efa_rdm_ep->base_ep.inject_rma_size);
	err = efa_rdm_ep_cap_check_rma(efa_rdm_ep);
	if (err)
		return err;

	peer = efa_rdm_ep_get_peer(efa_rdm_ep, dest_addr);
	if (peer->is_local)
		return fi_inject_write(efa_rdm_ep->shm_ep, buf, len, peer->conn->shm_fi_addr, addr, key);

	iov.iov_base = (void *)buf;
	iov.iov_len = len;

	EFA_SETUP_RMA_IOV(rma_iov, addr, len, key);

	memset(&msg, 0, sizeof(msg));
	EFA_SETUP_MSG_RMA(msg, &iov, NULL, 1, dest_addr, &rma_iov, 1, NULL, 0);

	return efa_rdm_rma_generic_writemsg(efa_rdm_ep, peer, &msg, FI_INJECT | EFA_RDM_TXE_NO_COMPLETION);
}

ssize_t efa_rdm_rma_inject_writedata(struct fid_ep *ep, const void *buf, size_t len,
				 uint64_t data, fi_addr_t dest_addr, uint64_t addr,
				 uint64_t key)
{
	struct fi_msg_rma msg;
	struct iovec iov;
	struct fi_rma_iov rma_iov;
	struct efa_rdm_ep *efa_rdm_ep;
	struct efa_rdm_peer *peer;
	int err;

	efa_rdm_ep = container_of(ep, struct efa_rdm_ep, base_ep.util_ep.ep_fid.fid);
	assert(len <= efa_rdm_ep->base_ep.inject_rma_size);
	err = efa_rdm_ep_cap_check_rma(efa_rdm_ep);
	if (err)
		return err;

	peer = efa_rdm_ep_get_peer(efa_rdm_ep, dest_addr);
	if (peer->is_local)
		return fi_inject_writedata(efa_rdm_ep->shm_ep, buf, len, data, peer->conn->shm_fi_addr, addr, key);

	iov.iov_base = (void *)buf;
	iov.iov_len = len;

	EFA_SETUP_RMA_IOV(rma_iov, addr, len, key);

	memset(&msg, 0, sizeof(msg));
	EFA_SETUP_MSG_RMA(msg, &iov, NULL, 1, dest_addr, &rma_iov, 1, NULL, data);

	return efa_rdm_rma_generic_writemsg(efa_rdm_ep, peer, &msg, FI_INJECT | EFA_RDM_TXE_NO_COMPLETION |
				FI_REMOTE_CQ_DATA);
}

struct fi_ops_rma efa_rdm_rma_ops = {
	.size = sizeof(struct fi_ops_rma),
	.read = efa_rdm_rma_read,
	.readv = efa_rdm_rma_readv,
	.readmsg = efa_rdm_rma_readmsg,
	.write = efa_rdm_rma_write,
	.writev = efa_rdm_rma_writev,
	.writemsg = efa_rdm_rma_writemsg,
	.inject = efa_rdm_rma_inject_write,
	.writedata = efa_rdm_rma_writedata,
	.injectdata = efa_rdm_rma_inject_writedata,
};

