/* SPDX-License-Identifier: BSD-2-Clause OR GPL-2.0-only */
/* SPDX-FileCopyrightText: Copyright Amazon.com, Inc. or its affiliates. All rights reserved. */

#include "efa.h"
#include "efa_av.h"
#include "efa_rdm_pkt_type.h"
#include "efa_rdm_pke_rtm.h"
#include "efa_rdm_pke_utils.h"
#include "efa_rdm_pke_req.h"
#include "efa_rdm_pke_rtm.h"

/**
 * @brief initialize a rdm peer
 *
 * @param[in,out]	peer	rdm peer
 * @param[in]		ep	rdm endpoint
 * @param[in]		conn	efa conn object
 * @relates efa_rdm_peer
 */
void efa_rdm_peer_construct(struct efa_rdm_peer *peer, struct efa_rdm_ep *ep, struct efa_conn *conn)
{
	int ret;
	memset(peer, 0, sizeof(struct efa_rdm_peer));

	peer->ep = ep;
	peer->conn = conn;
	peer->is_self = efa_is_same_addr(&ep->base_ep.src_addr, conn->ep_addr);
	peer->host_id = peer->is_self ? ep->host_id : 0;	/* Peer host id is exchanged via handshake */
	peer->num_runt_bytes_in_flight = 0;
	/* allocate the robuf circular queue from the pre-allocated buffer pool */
	ret = efa_recvwin_buf_alloc(&peer->robuf, efa_env.recvwin_size, true, ep->peer_robuf_pool);
	if (OFI_UNLIKELY(ret == -FI_ENOMEM)) {
		/* ran out of memory while creating the peer reorder buffer */
		EFA_WARN(FI_LOG_EP_CTRL, "Unable to allocate peer->robuf\n");
		return;
	}
	dlist_init(&peer->outstanding_tx_pkts);
	dlist_init(&peer->txe_list);
	dlist_init(&peer->rxe_list);
	dlist_init(&peer->overflow_pke_list);

	if (conn->shm_fi_addr != FI_ADDR_NOTAVAIL) {
		peer->is_local = 1;
	}

	efa_rdm_rxe_map_construct(&peer->rxe_map);
}

/**
 * @brief clear resources accociated with a peer
 *
 * release reorder buffer, txe list and rxe list of a peer
 *
 * @param[in,out]	peer 	rdm peer
 * @relates efa_rdm_peer
 */
void efa_rdm_peer_destruct(struct efa_rdm_peer *peer, struct efa_rdm_ep *ep)
{
	struct dlist_entry *tmp;
	struct efa_rdm_ope *txe;
	struct efa_rdm_ope *rxe;
	struct efa_rdm_pke *pkt_entry;
	struct efa_rdm_peer_overflow_pke_list_entry *overflow_pke_list_entry;
	/*
	 * TODO: Add support for wait/signal until all pending messages have
	 * been sent/received so we do not attempt to complete a data transfer
	 * or internal transfer after the EP is shutdown.
	 */
	if ((peer->flags & EFA_RDM_PEER_REQ_SENT) &&
	    !(peer->flags & EFA_RDM_PEER_HANDSHAKE_RECEIVED))
		EFA_WARN_ONCE(FI_LOG_EP_CTRL, "Closing EP with unacked CONNREQs in flight\n");

	if (peer->robuf.pending)
		efa_recvwin_free(&peer->robuf, true);

	if (!ep) {
		/* ep is NULL means the endpoint has been closed.
		 * In this case there is no need to proceed because
		 * all the txe, rxe, pkt_entry has been released.
		 */
		return;
	}

	/* we cannot release outstanding TX packets because device
	 * will report completion of these packets later. Setting
	 * pkt_entry->peer to NULL so the completion will be ignored.
	 */
	dlist_foreach_container(&peer->outstanding_tx_pkts,
				struct efa_rdm_pke,
				pkt_entry, entry) {
		pkt_entry->peer = NULL;
	}

	dlist_foreach_container_safe(&peer->overflow_pke_list,
				     struct efa_rdm_peer_overflow_pke_list_entry,
				     overflow_pke_list_entry, entry, tmp) {
		dlist_remove(&overflow_pke_list_entry->entry);
		efa_rdm_pke_release_rx(overflow_pke_list_entry->pkt_entry);
		ofi_buf_free(overflow_pke_list_entry);
	}

	dlist_foreach_container_safe(&peer->txe_list,
				     struct efa_rdm_ope,
				     txe, peer_entry, tmp) {
		efa_rdm_txe_release(txe);
	}

	dlist_foreach_container_safe(&peer->rxe_list,
				     struct efa_rdm_ope,
				     rxe, peer_entry, tmp) {
		efa_rdm_rxe_release(rxe);
	}

	if (peer->flags & EFA_RDM_PEER_HANDSHAKE_QUEUED)
		dlist_remove(&peer->handshake_queued_entry);

	if (peer->flags & EFA_RDM_PEER_IN_BACKOFF)
		dlist_remove(&peer->rnr_backoff_entry);

#ifdef ENABLE_EFA_POISONING
	efa_rdm_poison_mem_region(peer, sizeof(struct efa_rdm_peer));
#endif
}

/**
 * @brief run incoming packet_entry through reorder buffer
 * queue the packet entry if msg_id is larger than expected.
 * If queue failed, abort the application and print error message.
 *
 * @param[in]		peer		peer
 * @param[in]		ep		endpoint
 * @param[in,out]	pkt_entry	packet entry, will be released if successfully queued
 * @returns
 * 0 if `msg_id` of `pkt_entry` matches expected msg_id.
 * 1 if `msg_id` of `pkt_entry` is larger than expected, and the packet entry is queued successfully
 * -FI_EALREADY if `msg_id` of `pkt_entry` is smaller than expected.
 */
int efa_rdm_peer_reorder_msg(struct efa_rdm_peer *peer, struct efa_rdm_ep *ep,
			     struct efa_rdm_pke *pkt_entry)
{
	struct efa_rdm_robuf *robuf;
	struct efa_rdm_rtm_base_hdr *rtm_hdr;
	uint32_t msg_id;

	assert(efa_rdm_pke_get_base_hdr(pkt_entry)->type >= EFA_RDM_REQ_PKT_BEGIN);

	rtm_hdr = (struct efa_rdm_rtm_base_hdr *)pkt_entry->wiredata;
	if (rtm_hdr->flags & EFA_RDM_REQ_READ_NACK) {
		/* This is the long CTS RTM packet sent after the long read
		 * protocol failed. The message ID was reordered and processed
		 * when the long read RTM packet was processed. So we don't need
		 * to reorder here again
		 */
		return 0;
	}

	msg_id = efa_rdm_pke_get_rtm_msg_id(pkt_entry);
	robuf = &peer->robuf;
#if ENABLE_DEBUG
	if (msg_id != ofi_recvwin_next_exp_id(robuf))
		EFA_DBG(FI_LOG_EP_CTRL,
		       "msg OOO msg_id: %" PRIu32 " expected: %"
		       PRIu32 "\n", msg_id,
		       ofi_recvwin_next_exp_id(robuf));
#endif
	if (ofi_recvwin_is_exp(robuf, msg_id))
		return 0;
	else if (!ofi_recvwin_id_valid(robuf, msg_id)) {
		if (ofi_recvwin_id_processed(robuf, msg_id)) {
			EFA_WARN(FI_LOG_EP_CTRL,
			       "Error: message id has already been processed. received: %" PRIu32 " expected: %"
			       PRIu32 "\n", msg_id, ofi_recvwin_next_exp_id(robuf));
			return -FI_EALREADY;
		} else {
			/* Current receive window size is too small to hold incoming messages.
			 * Store the overflow messages in a double linked list, and move it 
			 * back to receive window later.
			 */
			struct efa_rdm_peer_overflow_pke_list_entry *overflow_pke_list_entry;

			overflow_pke_list_entry = ofi_buf_alloc(ep->overflow_pke_pool);
			if (OFI_UNLIKELY(!overflow_pke_list_entry)) {
				EFA_WARN(FI_LOG_EP_CTRL, "Unable to allocate an overflow_pke_list_entry.\n");
				return -FI_ENOMEM;
			}

			overflow_pke_list_entry->pkt_entry = pkt_entry;
			dlist_insert_head(&overflow_pke_list_entry->entry, &peer->overflow_pke_list);

			EFA_DBG(FI_LOG_EP_CTRL,
				"Current receive window size is too small to hold incoming messages."
				"Storing overflow msg_id %d in overflow_pke_list.\n",
				msg_id);

			return 1;
		}
	}

	return efa_rdm_peer_recvwin_queue_or_append_pke(pkt_entry, msg_id, robuf);
}

/**
 * @brief Queue the packet entry in the receive window when its msg_id is larger
 * than expected. If using the multi-req protocol and a packet entry with the
 * same msg_id already exists in the receive window, append this pkt_entry to
 * the existing packet entry.
 *
 * @param[in]		pkt_entry	packet entry, will be released if successfully queued
 * @param[in]		msg_id		msg id of the pkt_entry
 * @param[in, out]	robuf		receive window of the peer
 *
 * @returns
 * 1 when the packet entry is queued successfully.
 * -FI_ENOMEM if running out of memory while allocating rx_pkt_entry for OOO msg.
 */
int efa_rdm_peer_recvwin_queue_or_append_pke(struct efa_rdm_pke *pkt_entry,
					     uint32_t msg_id,
					     struct efa_rdm_robuf *robuf)
{
	struct efa_rdm_pke *ooo_entry;
	struct efa_rdm_pke *cur_ooo_entry;
	if (OFI_LIKELY(efa_env.rx_copy_ooo)) {
		assert(pkt_entry->alloc_type == EFA_RDM_PKE_FROM_EFA_RX_POOL);
		ooo_entry = efa_rdm_pke_clone(pkt_entry, pkt_entry->ep->rx_ooo_pkt_pool, EFA_RDM_PKE_FROM_OOO_POOL);
#if ENABLE_DEBUG
		/* ooo pkt is also rx pkt, insert it to rx pkt list so we can track it and clean up during ep close */
		dlist_insert_tail(&ooo_entry->dbg_entry, &ooo_entry->ep->rx_pkt_list);
#endif

		if (OFI_UNLIKELY(!ooo_entry)) {
			EFA_WARN(FI_LOG_EP_CTRL,
				"Unable to allocate rx_pkt_entry for OOO msg\n");
			return -FI_ENOMEM;
		}
		efa_rdm_pke_release_rx(pkt_entry);
	} else {
		ooo_entry = pkt_entry;
	}

	cur_ooo_entry = *ofi_recvwin_get_msg(robuf, msg_id);
	if (cur_ooo_entry) {
		assert(efa_rdm_pkt_type_is_mulreq(efa_rdm_pke_get_base_hdr(cur_ooo_entry)->type));
		assert(efa_rdm_pke_get_rtm_msg_id(cur_ooo_entry) == msg_id);
		assert(efa_rdm_pke_get_rtm_msg_length(cur_ooo_entry) == efa_rdm_pke_get_rtm_msg_length(ooo_entry));
		efa_rdm_pke_append(cur_ooo_entry, ooo_entry);
	} else {
		ofi_recvwin_queue_msg(robuf, &ooo_entry, msg_id);
	}

	return 1;
}

/**
 * @brief After processing recvwin_size of pkt entries, iterate over the
 * overflow_pke_list and move the pkt entry to receive window if it fits.
 *
 * @param[in,out]	peer 		peer
 * 
 */
void efa_rdm_peer_move_overflow_pke_to_recvwin(struct efa_rdm_peer *peer)
{
	struct efa_rdm_peer_overflow_pke_list_entry *overflow_pke_list_entry;
	struct efa_rdm_pke *overflow_pkt_entry;
	struct dlist_entry *tmp;
	uint32_t msg_id;
	int ret;

	if (dlist_empty(&peer->overflow_pke_list)) {
		return;
	}

	dlist_foreach_container_safe (
		&peer->overflow_pke_list,
		struct efa_rdm_peer_overflow_pke_list_entry,
		overflow_pke_list_entry, entry, tmp) {
		overflow_pkt_entry = overflow_pke_list_entry->pkt_entry;
		msg_id = efa_rdm_pke_get_rtm_msg_id(overflow_pkt_entry);
		if (ofi_recvwin_id_valid((&peer->robuf), msg_id)) {
			ret = efa_rdm_peer_recvwin_queue_or_append_pke(
				overflow_pkt_entry, msg_id, (&peer->robuf));
			if (OFI_UNLIKELY(ret == -FI_ENOMEM)) {
				/* running out of memory while copy packet */
				efa_base_ep_write_eq_error(
					&(overflow_pkt_entry->ep->base_ep),
					FI_ENOBUFS, FI_EFA_ERR_OOM);
				return;
			}
			dlist_remove(&overflow_pke_list_entry->entry);
			ofi_buf_free(overflow_pke_list_entry);
			EFA_DBG(FI_LOG_EP_CTRL,
				"Moving pkt entry with msg_id %d from "
				"overflow_pke_list to receive window.\n",
				msg_id);
		}
	}
	return;
}

/**
 * @brief process packet entries in reorder buffer
 * This function is called after processing the expected packet entry
 *
 * @param[in,out]	peer 		peer
 * @param[in,out]	ep		end point
 */
void efa_rdm_peer_proc_pending_items_in_robuf(struct efa_rdm_peer *peer, struct efa_rdm_ep *ep)
{
	struct efa_rdm_pke *pending_pkt;
	int ret = 0;
	uint32_t msg_id, exp_msg_id;

	while (1) {
		pending_pkt = *ofi_recvwin_peek((&peer->robuf));
		if (!pending_pkt)
			return;

		msg_id = efa_rdm_pke_get_rtm_msg_id(pending_pkt);
		EFA_DBG(FI_LOG_EP_CTRL,
		       "Processing msg_id %d from robuf\n", msg_id);
		/* efa_rdm_pke_proc_rtm_rta will write error cq entry if needed */
		ret = efa_rdm_pke_proc_rtm_rta(pending_pkt, peer);
		*ofi_recvwin_get_next_msg((&peer->robuf)) = NULL;

		exp_msg_id = ofi_recvwin_next_exp_id((&peer->robuf));
		if (exp_msg_id % efa_env.recvwin_size == 0)
			efa_rdm_peer_move_overflow_pke_to_recvwin(peer);

		if (OFI_UNLIKELY(ret)) {
			EFA_WARN(FI_LOG_CQ,
				"Error processing msg_id %d from robuf: %s\n",
				msg_id, fi_strerror(-ret));
			return;
		}
	}
	return;
}

/**
 * @brief Get the runt size for a given peer and ope
 *
 * @param peer rdm peer
 * @param ep efa rdm ep
 * @param ope efa rdm ope
 * @return size_t the number of bytes that can be runt
 */
size_t efa_rdm_peer_get_runt_size(struct efa_rdm_peer *peer,
				  struct efa_rdm_ep *ep, struct efa_rdm_ope *ope)
{
	size_t runt_size;
	size_t memory_alignment;
	int iface;

	iface = ope->desc[0] ? ((struct efa_mr*) ope->desc[0])->peer.iface : FI_HMEM_SYSTEM;

	if (g_efa_hmem_info[iface].runt_size < peer->num_runt_bytes_in_flight)
		return 0;

	runt_size = MIN(g_efa_hmem_info[iface].runt_size - peer->num_runt_bytes_in_flight, ope->total_len);
	memory_alignment = efa_rdm_ep_get_memory_alignment(ep, iface);
	/*
	 * runt size must be aligned because:
	 * 1. For LL128 protocol, the size to be copied on the receiver side must be 128-multiple,
	 * 128 is the alignment in this case.
	 * 2. For non-LL128 protocol, using aligned runt size has optimal performance for data copy.
	 * Note the returned value can be 0. In that case we will not use runting read protocol.
	 */
	return (runt_size & ~(memory_alignment - 1));
}

/**
 * @brief Determine which Read based protocol to use for a given peer
 *
 * @param[in] peer		rdm peer
 * @param[in] ep		efa rdm ep
 * @param[in] efa_rdm_ope	efa rdm ope
 * @return The read-based protocol to use based on inputs.
 */
int efa_rdm_peer_select_readbase_rtm(struct efa_rdm_peer *peer,
				     struct efa_rdm_ep *ep, struct efa_rdm_ope *ope)
{
	int op = ope->op;

	assert(op == ofi_op_tagged || op == ofi_op_msg);

	if (efa_rdm_ep_domain(ep)->num_read_msg_in_flight == 0 &&
	    efa_rdm_peer_get_runt_size(peer, ep, ope) > 0 &&
	    !(ope->fi_flags & FI_DELIVERY_COMPLETE)) {
		return (op == ofi_op_tagged) ? EFA_RDM_RUNTREAD_TAGRTM_PKT
					     : EFA_RDM_RUNTREAD_MSGRTM_PKT;
	} else {
		return (op == ofi_op_tagged) ? EFA_RDM_LONGREAD_TAGRTM_PKT
					     : EFA_RDM_LONGREAD_MSGRTM_PKT;
	}
}
