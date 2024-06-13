/* SPDX-License-Identifier: BSD-2-Clause OR GPL-2.0-only */
/* SPDX-FileCopyrightText: Copyright Amazon.com, Inc. or its affiliates. All rights reserved. */

#include "efa.h"
#include "efa_rdm_srx.h"
#include "efa_rdm_msg.h"
#include "efa_rdm_pke_rtm.h"
#include "efa_rdm_pke_req.h"
#include "efa_rdm_ope.h"

/**
 * @brief update an rxe for a peer rx entry.
 *        This function is used by two sided operation only.
 *
 * @param[in] ep	endpoint
 * @param[in] peer_rxe	fi_peer_rx_entry_msg contains iov,iov_count,context for ths operation
 * @param[in] rxe	efa_rdm_ope to be updated
 */
void efa_rdm_srx_update_rxe(struct fi_peer_rx_entry *peer_rxe,
			    struct efa_rdm_ope *rxe)
{
	rxe->fi_flags = peer_rxe->flags;

	/* Handle case where we're allocating an unexpected rxe */
	rxe->iov_count = peer_rxe->count;
	if (rxe->iov_count) {
		assert(peer_rxe->iov);
		memcpy(rxe->iov, peer_rxe->iov, sizeof(*rxe->iov) * peer_rxe->count);
		rxe->cq_entry.len = ofi_total_iov_len(rxe->iov, rxe->iov_count);
		rxe->cq_entry.buf = peer_rxe->iov[0].iov_base;
	}

	if (peer_rxe->desc)
		memcpy(&rxe->desc[0], peer_rxe->desc,
			sizeof(*peer_rxe->desc) * peer_rxe->count);
	else
		memset(&rxe->desc[0], 0, sizeof(rxe->desc));

	rxe->cq_entry.op_context = peer_rxe->context;
	rxe->peer_rxe = peer_rxe;
}

/**
 * @brief This call is invoked by the owner provider to start progressing
 * the peer_rxe that matches a received message.
 *
 * @param[in] peer_rxe the rxe to be progressed.
 * @return int 0 on success, a negative integer on failure.
 */
static int efa_rdm_srx_start(struct fi_peer_rx_entry *peer_rxe)
{
	int ret;
	struct efa_rdm_pke *pkt_entry;
	struct efa_rdm_ope *rxe;

	assert(ofi_genlock_held(efa_rdm_srx_get_srx_ctx(peer_rxe)->lock));

	pkt_entry = peer_rxe->peer_context;
	assert(pkt_entry);
	rxe = pkt_entry->ope;
	efa_rdm_srx_update_rxe(peer_rxe, rxe);

	rxe->state = EFA_RDM_RXE_MATCHED;

	ret = efa_rdm_pke_proc_matched_rtm(pkt_entry);
	if (OFI_UNLIKELY(ret)) {
		/* If we run out of memory registrations, we fall back to
		 * emulated protocols */
		if (ret == -FI_ENOMR)
			return 0;
		efa_rdm_rxe_handle_error(rxe, -ret,
			rxe->op == ofi_op_msg ? FI_EFA_ERR_PKT_PROC_MSGRTM : FI_EFA_ERR_PKT_PROC_TAGRTM);
		efa_rdm_pke_release_rx(pkt_entry);
		efa_rdm_rxe_release(rxe);
	}

	return ret;
}

/**
 * @brief This call is invoked by the owner provider to discard
 * the message and data associated with the specified fi_peer_rx_entry.
 * This often indicates that the application has canceled or discarded
 * the receive operation.
 * Currently, this function will be called by
 * util_srx_close() during EP close.
 *
 * @param[in] peer_rxe the fi_peer_rx_entry to be discarded.
 * @return int 0 on success, a negative integer on failure.
 */
static int efa_rdm_srx_discard(struct fi_peer_rx_entry *peer_rxe)
{
	struct efa_rdm_pke *pkt_entry;
	struct efa_rdm_ope *rxe;

	assert(ofi_genlock_held(efa_rdm_srx_get_srx_ctx(peer_rxe)->lock));

	pkt_entry = peer_rxe->peer_context;
	assert(pkt_entry);
	rxe = pkt_entry->ope;
	EFA_WARN(FI_LOG_EP_CTRL,
		"Discarding unmatched unexpected rxe: %p pkt_entry %p\n",
		rxe, rxe->unexp_pkt);
	efa_rdm_pke_release_rx(rxe->unexp_pkt);
	efa_rdm_rxe_release_internal(rxe);
	return FI_SUCCESS;
}

static struct fi_ops_srx_peer efa_rdm_srx_peer_ops = {
	.size = sizeof(struct fi_ops_srx_peer),
	.start_msg = efa_rdm_srx_start,
	.start_tag = efa_rdm_srx_start,
	.discard_msg = efa_rdm_srx_discard,
	.discard_tag = efa_rdm_srx_discard,
};

/**
 * @brief update the mr desc in peer_rx_entry
 * efa has different format of memory registration descriptor from other providers like shm.
 * efa returns application a desc as the ptr to efa_mr struct, which has shm_mr as part of
 * the struct. So the desc passed in by application cannot be handled by peer (shm).
 * This function is invoked inside get_msg/tag, start_msg/tag to update the mr desc before
 * handing off to peer provider.
 * @param srx util_srx_ctx (context)
 * @param rx_entry the util_rx_entry to be updated
 */
static void efa_rdm_srx_update_mr(struct util_srx_ctx *srx, struct util_rx_entry *rx_entry)
{
	struct fid_peer_srx *owner_srx;
	struct fid_peer_srx *peer_srx;

	owner_srx = &srx->peer_srx;
	peer_srx = rx_entry->peer_entry.srx;

	assert(owner_srx);
	assert(peer_srx);
	/* This means the rx_entry is handed off to peer (shm) provider */
	if (rx_entry->peer_entry.desc && (owner_srx != peer_srx)) /* Do inline update */
		efa_rdm_get_desc_for_shm(rx_entry->peer_entry.count,
					 rx_entry->peer_entry.desc,
					 rx_entry->peer_entry.desc);
}

/**
 * @brief Construct peer srx
 *
 * @param[in] efa_rdm_ep efa_rdm_ep
 * @param[out] peer_srx the constructed peer srx
 */
int efa_rdm_peer_srx_construct(struct efa_rdm_ep *ep)
{
	int ret;
	ret = util_ep_srx_context(&efa_rdm_ep_domain(ep)->util_domain,
				ep->rx_size, EFA_RDM_IOV_LIMIT,
				ep->min_multi_recv_size,
				&efa_rdm_srx_update_mr,
				&efa_rdm_ep_domain(ep)->srx_lock,
				&ep->peer_srx_ep);
	if (ret) {
		EFA_WARN(FI_LOG_EP_CTRL, "util_ep_srx_context failed, err: %d\n", ret);
		return ret;
	}
	util_get_peer_srx(ep->peer_srx_ep)->peer_ops = &efa_rdm_srx_peer_ops;
	return util_srx_bind(&ep->peer_srx_ep->fid,
			     &ep->base_ep.util_ep.rx_cq->cq_fid.fid,
			     FI_RECV);
}
