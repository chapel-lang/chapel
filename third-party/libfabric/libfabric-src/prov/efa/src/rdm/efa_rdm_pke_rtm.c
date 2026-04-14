/* SPDX-License-Identifier: BSD-2-Clause OR GPL-2.0-only */
/* SPDX-FileCopyrightText: Copyright Amazon.com, Inc. or its affiliates. All rights reserved. */

#include "ofi_iov.h"
#include "ofi_proto.h"
#include "efa_errno.h"
#include "efa.h"
#include "efa_env.h"
#include "efa_hmem.h"
#include "efa_base_ep.h"
#include "efa_rdm_ep.h"
#include "efa_rdm_msg.h"
#include "efa_rdm_rma.h"
#include "efa_rdm_ope.h"
#include "efa_rdm_rxe_map.h"
#include "efa_rdm_pke.h"
#include "efa_rdm_pke_rtm.h"
#include "efa_rdm_pke_rta.h"
#include "efa_rdm_pke_utils.h"
#include "efa_rdm_protocol.h"
#include "efa_rdm_tracepoint.h"
#include "efa_rdm_pke_req.h"

/**
 * @brief the total length of the message corresponds to a RTM packet
 *
 * @details
 * A RTM packet is sent/received for an user's message, this function
 * return the total length of that message
 *
 * @param[in]	pkt_entry	RTM packet entry
 *
 * @returns
 * a 64-bits integer
 */
size_t efa_rdm_pke_get_rtm_msg_length(struct efa_rdm_pke *pkt_entry)
{
	struct efa_rdm_base_hdr *base_hdr;

	base_hdr = efa_rdm_pke_get_base_hdr(pkt_entry);
	switch (base_hdr->type) {
	case EFA_RDM_EAGER_MSGRTM_PKT:
	case EFA_RDM_EAGER_TAGRTM_PKT:
	case EFA_RDM_DC_EAGER_MSGRTM_PKT:
	case EFA_RDM_DC_EAGER_TAGRTM_PKT:
		return pkt_entry->payload_size;
	case EFA_RDM_MEDIUM_MSGRTM_PKT:
	case EFA_RDM_MEDIUM_TAGRTM_PKT:
		return efa_rdm_pke_get_medium_rtm_base_hdr(pkt_entry)->msg_length;
	case EFA_RDM_DC_MEDIUM_MSGRTM_PKT:
	case EFA_RDM_DC_MEDIUM_TAGRTM_PKT:
		return efa_rdm_pke_get_dc_medium_rtm_base_hdr(pkt_entry)->msg_length;
	case EFA_RDM_LONGCTS_MSGRTM_PKT:
	case EFA_RDM_LONGCTS_TAGRTM_PKT:
	case EFA_RDM_DC_LONGCTS_MSGRTM_PKT:
	case EFA_RDM_DC_LONGCTS_TAGRTM_PKT:
		return efa_rdm_pke_get_longcts_rtm_base_hdr(pkt_entry)->msg_length;
	case EFA_RDM_LONGREAD_MSGRTM_PKT:
	case EFA_RDM_LONGREAD_TAGRTM_PKT:
		return efa_rdm_pke_get_longread_rtm_base_hdr(pkt_entry)->msg_length;
	case EFA_RDM_RUNTREAD_MSGRTM_PKT:
	case EFA_RDM_RUNTREAD_TAGRTM_PKT:
		return efa_rdm_pke_get_runtread_rtm_base_hdr(pkt_entry)->msg_length;
	default:
		assert(0 && "Unknown REQ packet type\n");
	}

	return 0;
}

/**
 * @brief init a RTM packet entry that will carry payload (user data)
 *
 * @details
 * As the name indicate, this function is applied to RTM packets
 * that will carry payload (user data), which include EAGER, MEDIUM,
 * LONGCTS and RUNTREAD RTM (both DC and non-DC, tag and non-tag).
 * It is not applied to LONGREAD RTM.
 *
 * @param[in,out]	pkt_entry	RTM packet entry
 * @param[in]		pkt_type	RTM packet type
 * @param[in]		txe		TX entry that has user buffer information
 * @param[in]		segmment_offset data offset in respect of user buffer
 * @param[in]		data_size	user data size. If it is -1, the function
 * 					will select data size based on maximum
 * 					data capacity of packet entry.
 * @returns
 * 0 on success
 * negative libfabric error code for failure.
 */
static inline
ssize_t efa_rdm_pke_init_rtm_with_payload(struct efa_rdm_pke *pkt_entry,
					  int pkt_type,
					  struct efa_rdm_ope *txe,
					  size_t segment_offset,
					  int data_size)
{
	struct efa_rdm_rtm_base_hdr *rtm_hdr;

	efa_rdm_pke_init_req_hdr_common(pkt_entry, pkt_type, txe);

	rtm_hdr = (struct efa_rdm_rtm_base_hdr *)pkt_entry->wiredata;
	rtm_hdr->flags |= EFA_RDM_REQ_MSG;
	rtm_hdr->msg_id = txe->msg_id;

	if (txe->internal_flags & EFA_RDM_OPE_READ_NACK)
		rtm_hdr->flags |= EFA_RDM_REQ_READ_NACK;

	/* If this RTM packet is sent after the runting read protocol has failed
	because of a MR registration limit on the receiver, we don't want to
	send any data with the RTM packet. This is because the runting read RTM
	packets have already delivered some of the data and the long CTS RTM
	packet does not have a seg_offset field */
	if (txe->internal_flags & EFA_RDM_OPE_READ_NACK) {
		data_size = 0;
	} else if (data_size == -1) {
		data_size = MIN(txe->total_len - segment_offset,
				txe->ep->mtu_size - efa_rdm_pke_get_req_hdr_size(pkt_entry));

		if (data_size + segment_offset < txe->total_len) {
			if (efa_mr_is_cuda(txe->desc[0])) {
				if (txe->ep->sendrecv_in_order_aligned_128_bytes)
					data_size &= ~(EFA_RDM_IN_ORDER_ALIGNMENT - 1);
				else
					data_size &= ~(EFA_RDM_CUDA_MEMORY_ALIGNMENT -1);
			}
		}
	}

	return efa_rdm_pke_init_payload_from_ope(pkt_entry, txe,
						efa_rdm_pke_get_req_hdr_size(pkt_entry),
						segment_offset, data_size);
}

/**
 * @brief Update RX entry with the information in RTM packet entry.
 *
 * @details
 * The following field of RX entry is updated:
 *            address:       this is necessary because original address in
 *                           rxe can be FI_ADDR_UNSPEC
 *            cq_entry.data: for FI_REMOTE_CQ_DATA
 *            msg_id:        message id
 *            total_len:     application might provide a buffer that is larger
 *                           then incoming message size.
 *            tag:           sender's tag can be different from receiver's tag
 *                           becuase match only requires
 *                           (sender_tag | ignore) == (receiver_tag | ignore)
 *  This function is applied to both expected and unexpected RX entry
 *
 * @param[in]		pkt_entry	RTM packet entry
 * @param[in,out]	rxe		RX entry to be updated
 */
void efa_rdm_pke_rtm_update_rxe(struct efa_rdm_pke *pkt_entry,
				struct efa_rdm_ope *rxe)
{
	struct efa_rdm_base_hdr *base_hdr;

	base_hdr = efa_rdm_pke_get_base_hdr(pkt_entry);
	if (base_hdr->flags & EFA_RDM_REQ_OPT_CQ_DATA_HDR) {
		rxe->cq_entry.flags |= FI_REMOTE_CQ_DATA;
		rxe->cq_entry.data = efa_rdm_pke_get_req_cq_data(pkt_entry);
	}

	rxe->msg_id = efa_rdm_pke_get_rtm_msg_id(pkt_entry);
	rxe->total_len = efa_rdm_pke_get_rtm_msg_length(pkt_entry);
	if (rxe->op == ofi_op_tagged) {
		rxe->tag = efa_rdm_pke_get_rtm_tag(pkt_entry);
		rxe->cq_entry.tag = rxe->tag;
	}
}

/**
 * @brief process a RTM that has been matched to a RX entry
 *
 * @param[in,out]	pkt_entry	RTM packet entry
 * @param[in,out]	rxe			RX entry that matches the RTM
 *
 * @returns
 * 0 on success
 * negative libfabric error code on failure
 */
ssize_t efa_rdm_pke_proc_matched_rtm(struct efa_rdm_pke *pkt_entry)
{
	struct efa_rdm_ope *rxe;
	int pkt_type;
	ssize_t ret;

#if ENABLE_DEBUG
	struct efa_rdm_ep *ep;
	ep = pkt_entry->ep;
#endif

	rxe = pkt_entry->ope;
	assert(rxe && rxe->state == EFA_RDM_RXE_MATCHED);

	efa_rdm_tracepoint(rx_pke_proc_matched_msg_begin, (size_t) pkt_entry, pkt_entry->payload_size, rxe->msg_id, (size_t) rxe->cq_entry.op_context, rxe->total_len);
	if (!rxe->peer) {
		rxe->peer = pkt_entry->peer;
		assert(rxe->peer);
		dlist_insert_tail(&rxe->peer_entry, &rxe->peer->rxe_list);
	}

	/* Adjust rxe->cq_entry.len as needed.
	 * Initialy rxe->cq_entry.len is total recv buffer size.
	 * rxe->total_len is from REQ packet and is total send buffer size.
	 * if send buffer size < recv buffer size, we adjust value of rxe->cq_entry.len
	 * if send buffer size > recv buffer size, we have a truncated message and will
	 * write error CQ entry.
	 */
	if (rxe->cq_entry.len > rxe->total_len)
		rxe->cq_entry.len = rxe->total_len;

	pkt_type = efa_rdm_pke_get_base_hdr(pkt_entry)->type;

	if (pkt_type > EFA_RDM_DC_REQ_PKT_BEGIN &&
	    pkt_type < EFA_RDM_DC_REQ_PKT_END)
		rxe->internal_flags |= EFA_RDM_TXE_DELIVERY_COMPLETE_REQUESTED;

	if (pkt_type == EFA_RDM_LONGCTS_MSGRTM_PKT ||
	    pkt_type == EFA_RDM_LONGCTS_TAGRTM_PKT)
		rxe->tx_id = efa_rdm_pke_get_longcts_rtm_base_hdr(pkt_entry)->send_id;
	else if (pkt_type == EFA_RDM_DC_EAGER_MSGRTM_PKT ||
		 pkt_type == EFA_RDM_DC_EAGER_TAGRTM_PKT)
		rxe->tx_id = efa_rdm_pke_get_dc_eager_rtm_base_hdr(pkt_entry)->send_id;
	else if (pkt_type == EFA_RDM_DC_MEDIUM_MSGRTM_PKT ||
		 pkt_type == EFA_RDM_DC_MEDIUM_TAGRTM_PKT)
		rxe->tx_id = efa_rdm_pke_get_dc_medium_rtm_base_hdr(pkt_entry)->send_id;
	else if (pkt_type == EFA_RDM_DC_LONGCTS_MSGRTM_PKT ||
		 pkt_type == EFA_RDM_DC_LONGCTS_TAGRTM_PKT)
		rxe->tx_id = efa_rdm_pke_get_longcts_rtm_base_hdr(pkt_entry)->send_id;

	rxe->msg_id = efa_rdm_pke_get_rtm_base_hdr(pkt_entry)->msg_id;

	if (pkt_type == EFA_RDM_LONGREAD_MSGRTM_PKT || pkt_type == EFA_RDM_LONGREAD_TAGRTM_PKT)
		return efa_rdm_pke_proc_matched_longread_rtm(pkt_entry);

	if (efa_rdm_pkt_type_is_mulreq(pkt_type))
		return efa_rdm_pke_proc_matched_mulreq_rtm(pkt_entry);

	if (pkt_type == EFA_RDM_EAGER_MSGRTM_PKT ||
	    pkt_type == EFA_RDM_EAGER_TAGRTM_PKT ||
	    pkt_type == EFA_RDM_DC_EAGER_MSGRTM_PKT ||
	    pkt_type == EFA_RDM_DC_EAGER_TAGRTM_PKT) {
		return efa_rdm_pke_proc_matched_eager_rtm(pkt_entry);
	}

	assert(pkt_type == EFA_RDM_LONGCTS_MSGRTM_PKT ||
	       pkt_type == EFA_RDM_LONGCTS_TAGRTM_PKT ||
	       pkt_type == EFA_RDM_DC_LONGCTS_MSGRTM_PKT ||
	       pkt_type == EFA_RDM_DC_LONGCTS_TAGRTM_PKT);

	rxe->bytes_received += pkt_entry->payload_size;
	ret = efa_rdm_pke_copy_payload_to_ope(pkt_entry, rxe);
	if (ret) {
		return ret;
	}
#if ENABLE_DEBUG
	dlist_insert_tail(&rxe->pending_recv_entry, &ep->ope_recv_list);
	ep->pending_recv_counter++;
#endif
	rxe->state = EFA_RDM_RXE_RECV;
	ret = efa_rdm_ope_post_send_or_queue(rxe, EFA_RDM_CTS_PKT);

	return ret;
}

/**
 * @brief process a received non-tagged RTM packet
 *
 * @param[in,out]	pkt_entry	non-tagged RTM packet entry
 */
ssize_t efa_rdm_pke_proc_msgrtm(struct efa_rdm_pke *pkt_entry)
{
	ssize_t err;
	struct efa_rdm_ep *ep;
	struct efa_rdm_ope *rxe;
	struct fid_peer_srx *peer_srx;
	struct efa_rdm_rtm_base_hdr *rtm_hdr;

	ep = pkt_entry->ep;

	rtm_hdr = (struct efa_rdm_rtm_base_hdr *)pkt_entry->wiredata;
	if (rtm_hdr->flags & EFA_RDM_REQ_READ_NACK) {
		rxe = efa_rdm_rxe_map_lookup(&pkt_entry->peer->rxe_map, efa_rdm_pke_get_rtm_msg_id(pkt_entry));
		rxe->internal_flags |= EFA_RDM_OPE_READ_NACK;
	} else {
		rxe = efa_rdm_msg_alloc_rxe_for_msgrtm(ep, &pkt_entry);
		if (OFI_UNLIKELY(!rxe)) {
			efa_base_ep_write_eq_error(
				&ep->base_ep, FI_ENOBUFS,
				FI_EFA_ERR_RXE_POOL_EXHAUSTED);
			efa_rdm_pke_release_rx(pkt_entry);
			return -FI_ENOBUFS;
		}
	}

	pkt_entry->ope = rxe;

	if (rxe->state == EFA_RDM_RXE_MATCHED) {
		err = efa_rdm_pke_proc_matched_rtm(pkt_entry);
		if (OFI_UNLIKELY(err)) {
			efa_rdm_rxe_handle_error(rxe, -err, FI_EFA_ERR_PKT_PROC_MSGRTM);
			efa_rdm_pke_release_rx(pkt_entry);
			efa_rdm_rxe_release(rxe);
			return err;
		}
	} else if (rxe->state == EFA_RDM_RXE_UNEXP) {
		peer_srx = util_get_peer_srx(ep->peer_srx_ep);
		return peer_srx->owner_ops->queue_msg(rxe->peer_rxe);
	}

	return 0;
}

/**
 * @brief process a received tagged RTM packet
 *
 * @param[in,out]	pkt_entry	non-tagged RTM packet entry
 */
static ssize_t efa_rdm_pke_proc_tagrtm(struct efa_rdm_pke *pkt_entry)
{
	ssize_t err;
	struct efa_rdm_ep *ep;
	struct efa_rdm_ope *rxe;
	struct fid_peer_srx *peer_srx;
	struct efa_rdm_rtm_base_hdr *rtm_hdr;

	ep = pkt_entry->ep;

	rtm_hdr = (struct efa_rdm_rtm_base_hdr *) pkt_entry->wiredata;
	if (rtm_hdr->flags & EFA_RDM_REQ_READ_NACK) {
		rxe = efa_rdm_rxe_map_lookup(&pkt_entry->peer->rxe_map, efa_rdm_pke_get_rtm_msg_id(pkt_entry));
		rxe->internal_flags |= EFA_RDM_OPE_READ_NACK;
	} else {
		rxe = efa_rdm_msg_alloc_rxe_for_tagrtm(ep, &pkt_entry);
		if (OFI_UNLIKELY(!rxe)) {
			efa_base_ep_write_eq_error(
				&ep->base_ep, FI_ENOBUFS,
				FI_EFA_ERR_RXE_POOL_EXHAUSTED);
			efa_rdm_pke_release_rx(pkt_entry);
			return -FI_ENOBUFS;
		}
	}

	pkt_entry->ope = rxe;

	if (rxe->state == EFA_RDM_RXE_MATCHED) {
		err = efa_rdm_pke_proc_matched_rtm(pkt_entry);
		if (OFI_UNLIKELY(err)) {
			if (err == -FI_ENOMR)
				return err;
			efa_rdm_rxe_handle_error(rxe, -err, FI_EFA_ERR_PKT_PROC_TAGRTM);
			efa_rdm_pke_release_rx(pkt_entry);
			efa_rdm_rxe_release(rxe);
			return err;
		}
	} else if (rxe->state == EFA_RDM_RXE_UNEXP) {
		peer_srx = util_get_peer_srx(ep->peer_srx_ep);
		return peer_srx->owner_ops->queue_tag(rxe->peer_rxe);
	}

	return 0;
}

/**
 * @brief process a received RTM or RTA packet entry
 *
 * @details
 * The RTM or RTA passed to this function is ordered
 * by msg_id in the packet header
 *
 * @param[in,out]	pkt_entry	received RTM or RTA packet entry
 * @param[in]	peer	efa_rdm_peer struct of the sender
 */
ssize_t efa_rdm_pke_proc_rtm_rta(struct efa_rdm_pke *pkt_entry, struct efa_rdm_peer *peer)
{
	struct efa_rdm_ep *ep;
	struct efa_rdm_base_hdr *base_hdr;

	ep = pkt_entry->ep;
	base_hdr = efa_rdm_pke_get_base_hdr(pkt_entry);
	assert(base_hdr->type >= EFA_RDM_BASELINE_REQ_PKT_BEGIN);

	switch (base_hdr->type) {
	case EFA_RDM_EAGER_MSGRTM_PKT:
	case EFA_RDM_MEDIUM_MSGRTM_PKT:
	case EFA_RDM_LONGCTS_MSGRTM_PKT:
	case EFA_RDM_LONGREAD_MSGRTM_PKT:
	case EFA_RDM_RUNTREAD_MSGRTM_PKT:
	case EFA_RDM_DC_EAGER_MSGRTM_PKT:
	case EFA_RDM_DC_MEDIUM_MSGRTM_PKT:
	case EFA_RDM_DC_LONGCTS_MSGRTM_PKT:
		return efa_rdm_pke_proc_msgrtm(pkt_entry);
	case EFA_RDM_EAGER_TAGRTM_PKT:
	case EFA_RDM_MEDIUM_TAGRTM_PKT:
	case EFA_RDM_LONGCTS_TAGRTM_PKT:
	case EFA_RDM_LONGREAD_TAGRTM_PKT:
	case EFA_RDM_RUNTREAD_TAGRTM_PKT:
	case EFA_RDM_DC_EAGER_TAGRTM_PKT:
	case EFA_RDM_DC_MEDIUM_TAGRTM_PKT:
	case EFA_RDM_DC_LONGCTS_TAGRTM_PKT:
		return efa_rdm_pke_proc_tagrtm(pkt_entry);
	case EFA_RDM_WRITE_RTA_PKT:
		return efa_rdm_pke_proc_write_rta(pkt_entry);
	case EFA_RDM_DC_WRITE_RTA_PKT:
		return efa_rdm_pke_proc_dc_write_rta(pkt_entry);
	case EFA_RDM_FETCH_RTA_PKT:
		return efa_rdm_pke_proc_fetch_rta(pkt_entry);
	case EFA_RDM_COMPARE_RTA_PKT:
		return efa_rdm_pke_proc_compare_rta(pkt_entry);
	default:
		EFA_WARN(FI_LOG_EP_CTRL,
			"Unknown packet type ID: %d\n",
		       base_hdr->type);
		efa_base_ep_write_eq_error(&ep->base_ep, FI_EINVAL, FI_EFA_ERR_UNKNOWN_PKT_TYPE);
		efa_rdm_pke_release_rx(pkt_entry);
	}

	return -FI_EINVAL;
}

/**
 * @brief handle the event that a RTM or RTA packet entry has been received
 *
 * @details
 * If the RTM or RTA is out-of-order, save it in the re-order buffer.
 * Otherwise, process it then process packet in the re-order buffer.
 *
 * @param[in,out]	pkt_entry	received RTM or RTA packet entry
 */
void efa_rdm_pke_handle_rtm_rta_recv(struct efa_rdm_pke *pkt_entry)
{
	struct efa_rdm_ep *ep;
	struct efa_rdm_peer *peer;
	struct efa_rdm_base_hdr *base_hdr;
	struct efa_rdm_rtm_base_hdr *rtm_hdr;
	bool slide_recvwin;
	int ret;
	uint32_t msg_id, exp_msg_id;

	ep = pkt_entry->ep;

	peer = pkt_entry->peer;
	assert(peer);

	base_hdr = efa_rdm_pke_get_base_hdr(pkt_entry);
	assert(base_hdr->type >= EFA_RDM_BASELINE_REQ_PKT_BEGIN);

	if (efa_rdm_pkt_type_is_mulreq(base_hdr->type)) {
		struct efa_rdm_ope *rxe;
		struct efa_rdm_pke *unexp_pkt_entry;

		rxe = efa_rdm_rxe_map_lookup(&peer->rxe_map, efa_rdm_pke_get_rtm_msg_id(pkt_entry));
		if (rxe) {
			if (rxe->state == EFA_RDM_RXE_MATCHED) {
				pkt_entry->ope = rxe;
				efa_rdm_pke_proc_matched_mulreq_rtm(pkt_entry);
			} else {
				assert(rxe->unexp_pkt);
				unexp_pkt_entry = efa_rdm_pke_get_unexp(&pkt_entry);
				efa_rdm_pke_append(rxe->unexp_pkt, unexp_pkt_entry);
				unexp_pkt_entry->ope = rxe;
			}

			return;
		}
	}

	msg_id = efa_rdm_pke_get_rtm_msg_id(pkt_entry);
	ret = efa_rdm_peer_reorder_msg(peer, pkt_entry->ep, pkt_entry);
	if (ret == 1) {
		/* Packet was queued */
		return;
	} else if (OFI_UNLIKELY(ret < 0)) {
		if (OFI_UNLIKELY(ret == -FI_EALREADY)) {
			/* Packet with same msg_id has been processed before */
			EFA_WARN(FI_LOG_EP_CTRL,
				"Invalid msg_id: %" PRIu32
				" robuf->exp_msg_id: %" PRIu32 "\n",
			       msg_id, peer->robuf.exp_msg_id);
			efa_base_ep_write_eq_error(&ep->base_ep, ret, FI_EFA_ERR_PKT_ALREADY_PROCESSED);
			efa_rdm_pke_release_rx(pkt_entry);
			return;
		}

		if (OFI_UNLIKELY(ret == -FI_ENOMEM)) {
			/* running out of memory while copy packet */
			efa_base_ep_write_eq_error(&ep->base_ep, FI_ENOBUFS, FI_EFA_ERR_OOM);
			return;
		}

		EFA_WARN(FI_LOG_EP_CTRL,
			"Unknown error %d processing REQ packet msg_id: %"
			PRIu32 "\n", ret, msg_id);
		efa_base_ep_write_eq_error(&ep->base_ep, ret, FI_EFA_ERR_OTHER);
		return;
	}

	/* The condition below is false for long CTS RTM packet sent after the
	 * long read protocol failed. The message ID was marked as consumed when
	 * the long read RTM packet was processed. So we shouldn't slide the
	 * receive window again.
	 */
	rtm_hdr = (struct efa_rdm_rtm_base_hdr *)pkt_entry->wiredata;
	slide_recvwin = !(rtm_hdr->flags & EFA_RDM_REQ_READ_NACK);

	/*
	 * efa_rdm_pke_proc_rtm_rta() will write error cq entry if needed,
	 * thus we do not write error cq entry
	 */
	ret = efa_rdm_pke_proc_rtm_rta(pkt_entry, peer);
	if (OFI_UNLIKELY(ret))
		return;

	if (slide_recvwin) {
		ofi_recvwin_slide((&peer->robuf));
	}

	exp_msg_id = ofi_recvwin_next_exp_id((&peer->robuf));
	if (exp_msg_id % efa_env.recvwin_size == 0)
		efa_rdm_peer_move_overflow_pke_to_recvwin(peer);

	efa_rdm_peer_proc_pending_items_in_robuf(peer, ep);
}

/**
 * @brief construct a eager msgrtm pkt without hdr
 *
 * @param[in,out]	pkt_entry	pkt to be initialized
 * @param[in]		txe		TX entry
 */
static inline
ssize_t efa_rdm_pke_init_eager_msgrtm_zero_hdr(struct efa_rdm_pke *pkt_entry,
				      struct efa_rdm_ope *txe)
{
	pkt_entry->ope = txe;
	pkt_entry->peer = txe->peer;

	return efa_rdm_pke_init_payload_from_ope(pkt_entry, txe,
						0, 0, txe->total_len);
}

/**
 * @brief initialzie a EFA_RDM_EAGER_MSGRTM pacekt entry
 *
 * @param[in,out]	pkt_entry	EFA_RDM_EAGER_MSGRTM to be initialized
 * @param[in]		txe		TX entry
 */
ssize_t efa_rdm_pke_init_eager_msgrtm(struct efa_rdm_pke *pkt_entry,
				      struct efa_rdm_ope *txe)
{
	int ret;

	if (pkt_entry->flags & EFA_RDM_PKE_HAS_NO_BASE_HDR)
		ret = efa_rdm_pke_init_eager_msgrtm_zero_hdr(pkt_entry, txe);
	else
		ret = efa_rdm_pke_init_rtm_with_payload(pkt_entry,
						EFA_RDM_EAGER_MSGRTM_PKT,
						txe, 0, -1);
	if (ret)
		return ret;

	assert(txe->total_len == pkt_entry->payload_size);
	return 0;
}

/**
 * @brief initialize a EFA_RDM_EAGER_TAGRTM packet entry
 * @param[in,out]	pkt_entry	EFA_RDM_EAGER_TAGRTM to be initialized
 * @param[in]		txe		TX entry
 */
ssize_t efa_rdm_pke_init_eager_tagrtm(struct efa_rdm_pke *pkt_entry,
				  struct efa_rdm_ope *txe)
{
	struct efa_rdm_base_hdr *base_hdr;
	int ret;

	ret = efa_rdm_pke_init_rtm_with_payload(pkt_entry, EFA_RDM_EAGER_TAGRTM_PKT, txe, 0, -1);
	if (ret)
		return ret;
	assert(txe->total_len == pkt_entry->payload_size);
	base_hdr = efa_rdm_pke_get_base_hdr(pkt_entry);
	base_hdr->flags |= EFA_RDM_REQ_TAGGED;
	efa_rdm_pke_set_rtm_tag(pkt_entry, txe->tag);
	return 0;
}

/**
 * @brief initialzie a EFA_RDM_DC_EAGER_MSGRTM pacekt entry
 *
 * @param[in,out]	pkt_entry	EFA_RDM_DC_EAGER_MSGRTM to be initialized
 * @param[in]		txe		TX entry
 */
ssize_t efa_rdm_pke_init_dc_eager_msgrtm(struct efa_rdm_pke *pkt_entry,
					 struct efa_rdm_ope *txe)

{
	struct efa_rdm_dc_eager_msgrtm_hdr *dc_eager_msgrtm_hdr;
	int ret;

	txe->internal_flags |= EFA_RDM_TXE_DELIVERY_COMPLETE_REQUESTED;
	ret = efa_rdm_pke_init_rtm_with_payload(pkt_entry, EFA_RDM_DC_EAGER_MSGRTM_PKT, txe, 0, -1);
	if (ret)
		return ret;
	dc_eager_msgrtm_hdr = efa_rdm_pke_get_dc_eager_msgrtm_hdr(pkt_entry);
	dc_eager_msgrtm_hdr->hdr.send_id = txe->tx_id;
	return 0;
}

/**
 * @brief initialize a EFA_RDM_DC_EAGER_TAGRTM pacekt entry
 *
 * @param[in,out]	pkt_entry	EFA_RDM_DC_EAGER_TAGRTM to be initialized
 * @param[in]		txe		TX entry
 */
ssize_t efa_rdm_pke_init_dc_eager_tagrtm(struct efa_rdm_pke *pkt_entry,
					 struct efa_rdm_ope *txe)
{
	struct efa_rdm_base_hdr *base_hdr;
	struct efa_rdm_dc_eager_tagrtm_hdr *dc_eager_tagrtm_hdr;
	int ret;

	txe->internal_flags |= EFA_RDM_TXE_DELIVERY_COMPLETE_REQUESTED;
	ret = efa_rdm_pke_init_rtm_with_payload(pkt_entry, EFA_RDM_DC_EAGER_TAGRTM_PKT, txe, 0, -1);
	if (ret)
		return ret;
	base_hdr = efa_rdm_pke_get_base_hdr(pkt_entry);
	base_hdr->flags |= EFA_RDM_REQ_TAGGED;
	efa_rdm_pke_set_rtm_tag(pkt_entry, txe->tag);

	dc_eager_tagrtm_hdr = efa_rdm_pke_get_dc_eager_tagrtm_hdr(pkt_entry);
	dc_eager_tagrtm_hdr->hdr.send_id = txe->tx_id;
	return 0;
}

/**
 * @brief handle the event that an EAGER RTM has send completed
 *
 * @details
 * This function applies to EAGER_MSGRTM and EAGER_TAGRTM, it
 * does not apply to DC_EAGER_MSGRTM and DC_EAGER_TAGRTM
 *
 * @param[in,out]	pkt_entry	EAGER_MSGRTM or EAGER_TAGRTM packet entry
 */
void efa_rdm_pke_handle_eager_rtm_send_completion(struct efa_rdm_pke *pkt_entry)
{
	struct efa_rdm_ope *txe;

	txe = pkt_entry->ope;
	assert(txe->total_len == pkt_entry->payload_size);
	efa_rdm_ope_handle_send_completed(txe);
}

/**
 * @brief process a matched eager rtm packet entry
 *
 * @details
 * This function applies to all 4 types of EAGER RTM.
 *
 * @param[in]	pkt_entry	packet entry
 * @return	On success, return 0
 * 		On failure, return libfabric error code
 */
ssize_t efa_rdm_pke_proc_matched_eager_rtm(struct efa_rdm_pke *pkt_entry)
{
	int err;
	struct efa_rdm_ope *rxe;

	rxe = pkt_entry->ope;

	/*
	 * On success, efa_rdm_pke_copy_data_to_ope will write rx completion,
	 * release pkt_entry and rxe
	 */
	err = efa_rdm_pke_copy_payload_to_ope(pkt_entry, rxe);
	if (err)
		efa_rdm_pke_release_rx(pkt_entry);

	return err;
}


/**
 * @brief initialize a EFA_RDM_MEDIUM_MSGRTM packet
 *
 * @param[in,out]	pkt_entry	EFA_RDM_MEDIUM_MSGRTM packet entry
 * @param[in]		txe		TX entry
 * @param[in]		segment_offset	data offset in repect of user buffer
 * @param[in]		data_size	data size in the unit of bytes
 */
ssize_t efa_rdm_pke_init_medium_msgrtm(struct efa_rdm_pke *pkt_entry,
				       struct efa_rdm_ope *txe,
				       size_t segment_offset,
				       int data_size)

{
	struct efa_rdm_medium_rtm_base_hdr *rtm_hdr;
	int ret;

	efa_rdm_ope_try_fill_desc(txe, 0, FI_SEND);

	ret = efa_rdm_pke_init_rtm_with_payload(pkt_entry, EFA_RDM_MEDIUM_MSGRTM_PKT,
						txe, segment_offset, data_size);
	if (ret)
		return ret;

	rtm_hdr = efa_rdm_pke_get_medium_rtm_base_hdr(pkt_entry);
	rtm_hdr->msg_length = txe->total_len;
	rtm_hdr->seg_offset = segment_offset;
	return 0;
}

/**
 * @brief initialize a EFA_RDM_MEDIUM_TAGRTM packet
 *
 * @param[in,out]	pkt_entry	EFA_RDM_MEDIUM_TAGRTM packet entry
 * @param[in]		txe		TX entry
 * @param[in]		segment_offset	data offset in repect of user buffer
 * @param[in]		data_size	data size in the unit of bytes
 */
ssize_t efa_rdm_pke_init_medium_tagrtm(struct efa_rdm_pke *pkt_entry,
				       struct efa_rdm_ope *txe,
				       size_t segment_offset,
				       int data_size)
{
	struct efa_rdm_medium_rtm_base_hdr *rtm_hdr;
	int ret;

	efa_rdm_ope_try_fill_desc(txe, 0, FI_SEND);

	ret = efa_rdm_pke_init_rtm_with_payload(pkt_entry, EFA_RDM_MEDIUM_TAGRTM_PKT,
						txe, segment_offset, data_size);
	if (ret)
		return ret;

	rtm_hdr = efa_rdm_pke_get_medium_rtm_base_hdr(pkt_entry);
	rtm_hdr->msg_length = txe->total_len;
	rtm_hdr->seg_offset = segment_offset;
	rtm_hdr->hdr.flags |= EFA_RDM_REQ_TAGGED;
	efa_rdm_pke_set_rtm_tag(pkt_entry, txe->tag);
	return 0;
}

/**
 * @brief initialize a EFA_RDM_DC_MEDIUM_MSGRTM packet
 *
 * @details
 * DC means delivery complete
 *
 * @param[in,out]	pkt_entry	EFA_RDM_DC_MEDIUM_MSGRTM packet entry
 * @param[in]		txe		TX entry
 * @param[in]		segment_offset	data offset in repect of user buffer
 * @param[in]		data_size	data size in the unit of bytes
 */
ssize_t efa_rdm_pke_init_dc_medium_msgrtm(struct efa_rdm_pke *pkt_entry,
					  struct efa_rdm_ope *txe,
					  size_t segment_offset,
					  int data_size)
{
	struct efa_rdm_dc_medium_msgrtm_hdr *dc_medium_msgrtm_hdr;
	int ret;

	txe->internal_flags |= EFA_RDM_TXE_DELIVERY_COMPLETE_REQUESTED;

	efa_rdm_ope_try_fill_desc(txe, 0, FI_SEND);

	ret = efa_rdm_pke_init_rtm_with_payload(pkt_entry, EFA_RDM_DC_MEDIUM_MSGRTM_PKT,
						txe, segment_offset, data_size);
	if (ret)
		return ret;

	dc_medium_msgrtm_hdr = efa_rdm_pke_get_dc_medium_msgrtm_hdr(pkt_entry);
	dc_medium_msgrtm_hdr->hdr.msg_length = txe->total_len;
	dc_medium_msgrtm_hdr->hdr.seg_offset = segment_offset;
	dc_medium_msgrtm_hdr->hdr.send_id = txe->tx_id;
	return 0;
}

/**
 * @brief initialize a EFA_RDM_DC_MEDIUM_TAGRTM packet
 *
 * @param[in,out]	pkt_entry	EFA_RDM_DC_MEDIUM_TAGRTM packet entry
 * @param[in]		txe		TX entry
 * @param[in]		segment_offset	data offset in repect of user buffer
 * @param[in]		data_size	data size in the unit of bytes
 */
ssize_t efa_rdm_pke_init_dc_medium_tagrtm(struct efa_rdm_pke *pkt_entry,
					  struct efa_rdm_ope *txe,
					  size_t segment_offset,
					  int data_size)
{
	struct efa_rdm_dc_medium_tagrtm_hdr *dc_medium_tagrtm_hdr;
	int ret;

	txe->internal_flags |= EFA_RDM_TXE_DELIVERY_COMPLETE_REQUESTED;

	efa_rdm_ope_try_fill_desc(txe, 0, FI_SEND);

	ret = efa_rdm_pke_init_rtm_with_payload(pkt_entry, EFA_RDM_DC_MEDIUM_TAGRTM_PKT,
						txe, segment_offset, data_size);
	if (ret)
		return ret;

	dc_medium_tagrtm_hdr = efa_rdm_pke_get_dc_medium_tagrtm_hdr(pkt_entry);
	dc_medium_tagrtm_hdr->hdr.msg_length = txe->total_len;
	dc_medium_tagrtm_hdr->hdr.seg_offset = segment_offset;
	dc_medium_tagrtm_hdr->hdr.hdr.flags |= EFA_RDM_REQ_TAGGED;
	dc_medium_tagrtm_hdr->hdr.send_id = txe->tx_id;
	efa_rdm_pke_set_rtm_tag(pkt_entry, txe->tag);
	return 0;
}

/**
 * @brief handle the event that a MEDIUM RTM has been sent
 *
 * @details
 * this function applies to all 4 types of MEDIUM RTM
 *
 * @param[in,out]		pkt_entry	MEDIUM RTM packet entry
 */
void efa_rdm_pke_handle_medium_rtm_sent(struct efa_rdm_pke *pkt_entry)
{
	struct efa_rdm_ope *txe;

	txe = pkt_entry->ope;
	txe->bytes_sent += pkt_entry->payload_size;
}

/**
 * @brief handle the event that a MEDIUM RTM has sent completed
 *
 * @details
 * this function applies to non DC version of MEDIUM RTM
 *
 * @param[in,out]		pkt_entry	MEDIUM RTM packet entry
 */
void efa_rdm_pke_handle_medium_rtm_send_completion(struct efa_rdm_pke *pkt_entry)
{
	struct efa_rdm_ope *txe;

	txe = pkt_entry->ope;
	txe->bytes_acked += pkt_entry->payload_size;
	if (txe->total_len == txe->bytes_acked)
		efa_rdm_ope_handle_send_completed(txe);
}

/**
 * @brief process a matched MEDIUM or RUNTREAD RTM
 *
 * @details
 * This function applies to all 4 types of MEDIUM
 * RTM and 2 types of RUNTREAD RTM.
 *
 * @param[in,out]	pkt_entry	packet entry
 * @param[in]	peer	efa_rdm_peer struct of the sender
 */
ssize_t efa_rdm_pke_proc_matched_mulreq_rtm(struct efa_rdm_pke *pkt_entry)
{
	struct efa_rdm_ep *ep;
	struct efa_rdm_ope *rxe;
	struct efa_rdm_pke *cur, *nxt;
	int pkt_type;
	ssize_t ret, err;
	uint64_t msg_id;

	ep = pkt_entry->ep;
	rxe = pkt_entry->ope;
	pkt_type = efa_rdm_pke_get_base_hdr(pkt_entry)->type;

	ret = 0;
	if (efa_rdm_pkt_type_is_runtread(pkt_type)) {
		struct efa_rdm_runtread_rtm_base_hdr *runtread_rtm_hdr;

		runtread_rtm_hdr = efa_rdm_pke_get_runtread_rtm_base_hdr(pkt_entry);
		rxe->bytes_runt = runtread_rtm_hdr->runt_length;
		if (rxe->total_len > rxe->bytes_runt && rxe->bytes_read_total_len == 0) {
			struct fi_rma_iov *read_iov;

			rxe->tx_id = runtread_rtm_hdr->send_id;
			read_iov = (struct fi_rma_iov *)(pkt_entry->wiredata + efa_rdm_pke_get_req_hdr_size(pkt_entry));
			rxe->rma_iov_count = runtread_rtm_hdr->read_iov_count;
			memcpy(rxe->rma_iov, read_iov, rxe->rma_iov_count * sizeof(struct fi_rma_iov));
			efa_rdm_tracepoint(runtread_read_posted, rxe->msg_id,
				    (size_t) rxe->cq_entry.op_context, rxe->total_len);

			err = efa_rdm_pke_post_remote_read_or_nack(ep, pkt_entry, rxe);
			if (err)
				return err;
		}
	}

	cur = pkt_entry;
	while (cur) {
		assert(cur->payload);
		assert(cur->payload_size);
		/* efa_rdm_pke_copy_payload_to_ope() can release rxe, so
		 * bytes_received must be calculated before it.
		 */
		rxe->bytes_received += cur->payload_size;
		rxe->bytes_received_via_mulreq += cur->payload_size;
		if (efa_rdm_ope_mulreq_total_data_size(rxe, pkt_type) ==
		    rxe->bytes_received_via_mulreq) {
			if (rxe->internal_flags & EFA_RDM_OPE_READ_NACK) {
				EFA_INFO(FI_LOG_EP_CTRL,
					 "Receiver sending long read NACK "
					 "packet because memory registration "
					 "limit was reached on the receiver\n");
				err = efa_rdm_ope_post_send_or_queue(
					rxe, EFA_RDM_READ_NACK_PKT);
				if (err)
					return err;
			} else {
				msg_id = efa_rdm_pke_get_rtm_msg_id(cur);
				efa_rdm_rxe_map_remove(&cur->peer->rxe_map, msg_id,
						       rxe);
			}
		}

		/* efa_rdm_pke_copy_data_to_ope() will release cur, so
		 * cur->next must be copied out before it.
		 */
		nxt = cur->next;
		cur->next = NULL;

		err = efa_rdm_pke_copy_payload_to_ope(cur, rxe);
		if (err) {
			efa_rdm_pke_release_rx(cur);
			ret = err;
		}

		cur = nxt;
	}

	return ret;
}

/**
 * @brief initialize a LONGCTS RTM packet
 *
 * @details
 * This function is used by all 4 types of LONGCTS RTM
 *
 * @param[in,out]	pkt_entry	LONGCTS RTM packet entry
 * @param[in]		pkt_type	packe type, must be one of:
 *					EFA_RDM_LONGCTS_MSGRTM_PKT,
 *					EFA_RDM_LONGCTS_TAGGRTM_PKT,
 *					EFA_RDM_DC_LONGCTS_MSGRTM_PKT,
 *					EFA_RDM_DC_LONGCTS_TAGRTM_PKT,
 * @param[in]		txe		TX entry
 */
int efa_rdm_pke_init_longcts_rtm_common(struct efa_rdm_pke *pkt_entry,
					int pkt_type,
					struct efa_rdm_ope *txe)
{
	struct efa_rdm_longcts_rtm_base_hdr *rtm_hdr;
	int ret;

	ret = efa_rdm_pke_init_rtm_with_payload(pkt_entry, pkt_type, txe, 0, -1);
	if (ret)
		return ret;

	rtm_hdr = efa_rdm_pke_get_longcts_rtm_base_hdr(pkt_entry);
	rtm_hdr->msg_length = txe->total_len;
	rtm_hdr->send_id = txe->tx_id;
	rtm_hdr->credit_request = efa_env.tx_min_credits;
	return 0;
}

/**
 * @brief initialize a EFA_RDM_LONGCTS_MSGRTM packet
 *
 * @param[in,out]	pkt_entry	EFA_RDM_LONGCTS_MSGRTM packet entry
 * @param[in]		txe		TX entry
 */
ssize_t efa_rdm_pke_init_longcts_msgrtm(struct efa_rdm_pke *pkt_entry,
					struct efa_rdm_ope *txe)
{
	return efa_rdm_pke_init_longcts_rtm_common(pkt_entry,
						   EFA_RDM_LONGCTS_MSGRTM_PKT,
						   txe);
}

/**
 * @brief initialize a EFA_RDM_LONGCTS_TAGRTM packet
 *
 * @param[in,out]	pkt_entry	EFA_RDM_LONGCTS_TAGRTM packet entry
 * @param[in]		txe		TX entry
 */
ssize_t efa_rdm_pke_init_longcts_tagrtm(struct efa_rdm_pke *pkt_entry,
				    struct efa_rdm_ope *txe)
{
	struct efa_rdm_base_hdr *base_hdr;
	int ret;

	ret = efa_rdm_pke_init_longcts_rtm_common(pkt_entry,
						  EFA_RDM_LONGCTS_TAGRTM_PKT,
						  txe);
	if (ret)
		return ret;

	base_hdr = efa_rdm_pke_get_base_hdr(pkt_entry);
	base_hdr->flags |= EFA_RDM_REQ_TAGGED;
	efa_rdm_pke_set_rtm_tag(pkt_entry, txe->tag);
	return 0;
}

/**
 * @brief initialize a EFA_RDM_DC_LONGCTS_MSGRTM packet
 *
 * @param[in,out]	pkt_entry	EFA_RDM_DC_LONGCTS_TAGRTM packet entry
 * @param[in]		txe		TX entry
 */
ssize_t efa_rdm_pke_init_dc_longcts_msgrtm(struct efa_rdm_pke *pkt_entry,
					   struct efa_rdm_ope *txe)
{
	txe->internal_flags |= EFA_RDM_TXE_DELIVERY_COMPLETE_REQUESTED;
	return efa_rdm_pke_init_longcts_rtm_common(pkt_entry,
						   EFA_RDM_DC_LONGCTS_MSGRTM_PKT,
						   txe);
}

/**
 * @brief initialize a EFA_RDM_DC_LONGCTS_TAGRTM packet
 *
 * @param[in,out]	pkt_entry	EFA_RDM_DC_MEDIUM_TAGRTM packet entry
 * @param[in]		txe		TX entry
 */
ssize_t efa_rdm_pke_init_dc_longcts_tagrtm(struct efa_rdm_pke *pkt_entry,
					   struct efa_rdm_ope *txe)
{
	struct efa_rdm_base_hdr *base_hdr;
	int ret;

	txe->internal_flags |= EFA_RDM_TXE_DELIVERY_COMPLETE_REQUESTED;
	ret = efa_rdm_pke_init_longcts_rtm_common(pkt_entry,
						  EFA_RDM_DC_LONGCTS_TAGRTM_PKT,
						  txe);
	if (ret)
		return ret;
	base_hdr = efa_rdm_pke_get_base_hdr(pkt_entry);
	base_hdr->flags |= EFA_RDM_REQ_TAGGED;
	efa_rdm_pke_set_rtm_tag(pkt_entry, txe->tag);
	return 0;
}

/**
 * @brief handle the event that a LONGCTS RTM has been sent
 *
 * this function applies to all 4 types of LONGCTS RTM
 *
 * @param[in,out]		pkt_entry	LONGCTS RTM packet entry
 */
void efa_rdm_pke_handle_longcts_rtm_sent(struct efa_rdm_pke *pkt_entry)
{
	struct efa_rdm_ope *txe;

	txe = pkt_entry->ope;
	txe->bytes_sent += pkt_entry->payload_size;
	assert(txe->bytes_sent < txe->total_len);

	if (efa_is_cache_available(efa_rdm_ep_domain(pkt_entry->ep)))
		efa_rdm_ope_try_fill_desc(txe, 0, FI_SEND);
}

/**
 * @brief handle the event that a LONGCTS RTM has been sent
 *
 * this function only applies to non DC version of LONGCTS RTM
 *
 * @param[in,out]		pkt_entry	LONGCTS RTM packet entry
 */
void efa_rdm_pke_handle_longcts_rtm_send_completion(struct efa_rdm_pke *pkt_entry)
{
	struct efa_rdm_ope *txe;

	/**
	 * A zero-payload longcts rtm pkt currently should only happen when it's
	 * used for the READ NACK protocol. In this case, this pkt doesn't
	 * contribute to the send completion, and the associated tx entry
	 * may be released earlier as the CTSDATA pkts have already kicked off
	 * and finished the send.
	 */
	if (pkt_entry->payload_size == 0) {
		assert(efa_rdm_pke_get_rtm_base_hdr(pkt_entry)->flags & EFA_RDM_REQ_READ_NACK);
		return;
	}

	txe = pkt_entry->ope;
	txe->bytes_acked += pkt_entry->payload_size;
	if (txe->total_len == txe->bytes_acked)
		efa_rdm_ope_handle_send_completed(txe);
}

/**
 * @brief initialize a longread RTM packet
 *
 * @details
 * This function applies to both tagged and non-tagged version
 * of LONGREAD RTM. Note that there is no DC longread RTM, because
 * LONGREAD protocol ensures DC by nature
 */
ssize_t efa_rdm_pke_init_longread_rtm(struct efa_rdm_pke *pkt_entry,
				      int pkt_type,
				      struct efa_rdm_ope *txe)
{
	struct efa_rdm_longread_rtm_base_hdr *rtm_hdr;
	struct fi_rma_iov *read_iov;
	size_t hdr_size;
	int err;

	efa_rdm_pke_init_req_hdr_common(pkt_entry, pkt_type, txe);

	rtm_hdr = efa_rdm_pke_get_longread_rtm_base_hdr(pkt_entry);
	rtm_hdr->hdr.flags |= EFA_RDM_REQ_MSG;
	rtm_hdr->hdr.msg_id = txe->msg_id;
	rtm_hdr->msg_length = txe->total_len;
	rtm_hdr->send_id = txe->tx_id;
	rtm_hdr->read_iov_count = txe->iov_count;

	hdr_size = efa_rdm_pke_get_req_hdr_size(pkt_entry);
	read_iov = (struct fi_rma_iov *)(pkt_entry->wiredata + hdr_size);
	err = efa_rdm_txe_prepare_to_be_read(txe, read_iov);
	if (OFI_UNLIKELY(err))
		return err;

	pkt_entry->pkt_size = hdr_size + txe->iov_count * sizeof(struct fi_rma_iov);
	pkt_entry->ope = txe;
	pkt_entry->peer = txe->peer;
	return 0;
}

/**
 * @brief initialize a EFA_RDM_LONGREAD_RTA_MSGRTM
 *
 */
ssize_t efa_rdm_pke_init_longread_msgrtm(struct efa_rdm_pke *pkt_entry,
					 struct efa_rdm_ope *txe)
{
	return efa_rdm_pke_init_longread_rtm(pkt_entry, EFA_RDM_LONGREAD_MSGRTM_PKT, txe);
}

/**
 * @brief initialize a EFA_RDM_LONGREAD_RTA_TAGRTM
 *
 */
ssize_t efa_rdm_pke_init_longread_tagrtm(struct efa_rdm_pke *pkt_entry,
					 struct efa_rdm_ope *txe)
{
	ssize_t err;
	struct efa_rdm_base_hdr *base_hdr;

	err = efa_rdm_pke_init_longread_rtm(pkt_entry, EFA_RDM_LONGREAD_TAGRTM_PKT, txe);
	if (err)
		return err;

	base_hdr = efa_rdm_pke_get_base_hdr(pkt_entry);
	base_hdr->flags |= EFA_RDM_REQ_TAGGED;
	efa_rdm_pke_set_rtm_tag(pkt_entry, txe->tag);
	return 0;
}

/**
 * @brief handle the event that a longread RTM has been sent
 *
 * @details
 * this function applies to both tagged and non-tagged
 * longread RTM
 *
 * @param[in,out]	pkt_entry	packet entry
 */
void efa_rdm_pke_handle_longread_rtm_sent(struct efa_rdm_pke *pkt_entry)
{
	efa_rdm_ep_domain(pkt_entry->ep)->num_read_msg_in_flight += 1;
}

/**
 * @brief process a matched longread RTM
 *
 * @details
 * this function applies to both tagged and non-tagged
 * longread RTM
 *
 * @param[in,out]	pkt_entry	packet entry
 */
ssize_t efa_rdm_pke_proc_matched_longread_rtm(struct efa_rdm_pke *pkt_entry)
{
	struct efa_rdm_ope *rxe;
	struct efa_rdm_longread_rtm_base_hdr *rtm_hdr;
	struct fi_rma_iov *read_iov;
	struct efa_rdm_ep *ep;
	int err;

	rxe = pkt_entry->ope;
	ep = rxe->ep;

	rtm_hdr = efa_rdm_pke_get_longread_rtm_base_hdr(pkt_entry);
	read_iov = (struct fi_rma_iov *)(pkt_entry->wiredata + efa_rdm_pke_get_req_hdr_size(pkt_entry));

	rxe->tx_id = rtm_hdr->send_id;
	rxe->rma_iov_count = rtm_hdr->read_iov_count;
	memcpy(rxe->rma_iov, read_iov,
	       rxe->rma_iov_count * sizeof(struct fi_rma_iov));

	efa_rdm_tracepoint(longread_read_posted, rxe->msg_id,
		    (size_t) rxe->cq_entry.op_context, rxe->total_len);

	err = efa_rdm_pke_post_remote_read_or_nack(ep, pkt_entry, rxe);

	efa_rdm_pke_release_rx(pkt_entry);
	return err;
}

/**
 * @brief fill in the efa_rdm_runtread_rtm_base_hdr and data of a RUNTREAD packet
 *
 * only thing left that need to be set is tag
 *
 * @param[out]		pkt_entry	pkt_entry to be initialzied
 * @param[in]		pkt_type	EFA_RDM_RUNREAD_MSGRTM or EFA_RDM_RUNTREAD_TAGRTM
 * @param[in]		txe		contains information of the send operation
 * @param[in]		segment_offset	data offset in repect of user buffer
 * @param[in]		data_size	data size in the unit of bytes
 */
static
ssize_t efa_rdm_pke_init_runtread_rtm(struct efa_rdm_pke *pkt_entry,
				      int pkt_type,
				      struct efa_rdm_ope *txe,
				      int64_t segment_offset,
				      int64_t data_size)
{
	struct efa_rdm_runtread_rtm_base_hdr *rtm_hdr;
	struct fi_rma_iov *read_iov;
	size_t hdr_size, payload_offset;
	int err;

	assert(txe->bytes_runt);

	efa_rdm_pke_init_req_hdr_common(pkt_entry, pkt_type, txe);

	rtm_hdr = efa_rdm_pke_get_runtread_rtm_base_hdr(pkt_entry);
	rtm_hdr->hdr.flags |= EFA_RDM_REQ_MSG;
	rtm_hdr->hdr.msg_id = txe->msg_id;
	rtm_hdr->msg_length = txe->total_len;
	rtm_hdr->send_id = txe->tx_id;
	rtm_hdr->seg_offset = segment_offset;
	rtm_hdr->runt_length = txe->bytes_runt;
	rtm_hdr->read_iov_count = txe->iov_count;

	hdr_size = efa_rdm_pke_get_req_hdr_size(pkt_entry);
	read_iov = (struct fi_rma_iov *)(pkt_entry->wiredata + hdr_size);
	err = efa_rdm_txe_prepare_to_be_read(txe, read_iov);
	if (OFI_UNLIKELY(err))
		return err;

	payload_offset  = hdr_size + txe->iov_count * sizeof(struct fi_rma_iov);
	return efa_rdm_pke_init_payload_from_ope(pkt_entry, txe,
						 payload_offset,
						 segment_offset,
						 data_size);
}

/**
 * @brief initialize a EFA_RDM_RUNTREAD_MSGRTM packet
 *
 * @param[out]		pkt_entry	pkt_entry to be initialzied
 * @param[in]		txe		contains information of the send operation
 * @param[in]		segment_offset	data offset in repect of user buffer
 * @param[in]		data_size	data size in the unit of bytes
 */
ssize_t efa_rdm_pke_init_runtread_msgrtm(struct efa_rdm_pke *pkt_entry,
					 struct efa_rdm_ope *txe,
					 size_t segment_offset,
					 int data_size)
{
	return efa_rdm_pke_init_runtread_rtm(pkt_entry,
					     EFA_RDM_RUNTREAD_MSGRTM_PKT,
					     txe,
					     segment_offset,
					     data_size);
}

/**
 * @brief initialize a EFA_RDM_RUNTREAD_TAGRTM packet
 *
 * @param[out]		pkt_entry	pkt_entry to be initialzied
 * @param[in]		txe		contains information of the send operation
 * @param[in]		segment_offset	data offset in repect of user buffer
 * @param[in]		data_size	data size in the unit of bytes
 */
ssize_t efa_rdm_pke_init_runtread_tagrtm(struct efa_rdm_pke *pkt_entry,
					 struct efa_rdm_ope *txe,
					 size_t segment_offset,
					 int data_size)
{
	ssize_t err;
	struct efa_rdm_base_hdr *base_hdr;

	err = efa_rdm_pke_init_runtread_rtm(pkt_entry,
					    EFA_RDM_RUNTREAD_TAGRTM_PKT,
					    txe,
					    segment_offset,
					    data_size);
	if (err)
		return err;

	base_hdr = efa_rdm_pke_get_base_hdr(pkt_entry);
	base_hdr->flags |= EFA_RDM_REQ_TAGGED;
	efa_rdm_pke_set_rtm_tag(pkt_entry, txe->tag);
	return 0;
}

/**
 * @brief handle the event that a runtread RTM has been sent
 *
 * This function applies to both RUNTREAD_MSGRTM and RUNTREAD_TAGRTM.
 *
 * @param[in,out]	pkt_entry	packet entry
 */
void efa_rdm_pke_handle_runtread_rtm_sent(struct efa_rdm_pke *pkt_entry, struct efa_rdm_peer *peer)
{
	struct efa_rdm_ope *txe;
	size_t pkt_data_size = pkt_entry->payload_size;

	assert(peer);

	txe = pkt_entry->ope;
	txe->bytes_sent += pkt_data_size;
	peer->num_runt_bytes_in_flight += pkt_data_size;

	if (efa_rdm_pke_get_runtread_rtm_base_hdr(pkt_entry)->seg_offset == 0 &&
	    txe->total_len > txe->bytes_runt)
		efa_rdm_ep_domain(pkt_entry->ep)->num_read_msg_in_flight += 1;
}

/**
 * @brief handle the event that the send of a runtread RTM has been completed
 *
 * This function applies to both RUNTREAD_MSGRTM and RUNTREAD_TAGRTM
 * There is no DC version of RUNTREAD. If user requested DC,
 * LONGREAD RTM will be used.
 * @param[in,out]	pkt_entry	packet entry
 */
void efa_rdm_pke_handle_runtread_rtm_send_completion(struct efa_rdm_pke *pkt_entry)
{
	struct efa_rdm_ope *txe;
	struct efa_rdm_peer *peer;
	size_t pkt_data_size;

	txe = pkt_entry->ope;
	pkt_data_size = pkt_entry->payload_size;
	txe->bytes_acked += pkt_data_size;

	peer = txe->peer;
	assert(peer);
	assert(peer->num_runt_bytes_in_flight >= pkt_data_size);
	peer->num_runt_bytes_in_flight -= pkt_data_size;
	if (txe->total_len == txe->bytes_acked)
		efa_rdm_ope_handle_send_completed(txe);
}
