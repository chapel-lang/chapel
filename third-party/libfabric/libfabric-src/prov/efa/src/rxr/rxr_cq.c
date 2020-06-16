/*
 * Copyright (c) 2019 Amazon.com, Inc. or its affiliates.
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

#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <ofi_iov.h>
#include <ofi_recvwin.h>
#include "rxr.h"
#include "rxr_rma.h"
#include "rxr_msg.h"
#include "rxr_cntr.h"
#include "rxr_atomic.h"
#include "efa.h"

static const char *rxr_cq_strerror(struct fid_cq *cq_fid, int prov_errno,
				   const void *err_data, char *buf, size_t len)
{
	struct fid_list_entry *fid_entry;
	struct util_ep *util_ep;
	struct util_cq *cq;
	struct rxr_ep *ep;
	const char *str;

	cq = container_of(cq_fid, struct util_cq, cq_fid);

	fastlock_acquire(&cq->ep_list_lock);
	assert(!dlist_empty(&cq->ep_list));
	fid_entry = container_of(cq->ep_list.next,
				 struct fid_list_entry, entry);
	util_ep = container_of(fid_entry->fid, struct util_ep, ep_fid.fid);
	ep = container_of(util_ep, struct rxr_ep, util_ep);

	str = fi_cq_strerror(ep->rdm_cq, prov_errno, err_data, buf, len);
	fastlock_release(&cq->ep_list_lock);
	return str;
}

/*
 * Teardown rx_entry and write an error cq entry. With our current protocol we
 * will only encounter an RX error when sending a queued REQ or CTS packet or
 * if we are sending a CTS message. Because of this, the sender will not send
 * any additional data packets if the receiver encounters an error. If there is
 * a scenario in the future where the sender will continue to send data packets
 * we need to prevent rx_id mismatch. Ideally, we should add a NACK message and
 * tear down both RX and TX entires although whatever caused the error may
 * prevent that.
 *
 * TODO: add a NACK message to tear down state on sender side
 */
int rxr_cq_handle_rx_error(struct rxr_ep *ep, struct rxr_rx_entry *rx_entry,
			   ssize_t prov_errno)
{
	struct fi_cq_err_entry err_entry;
	struct util_cq *util_cq;
	struct dlist_entry *tmp;
	struct rxr_pkt_entry *pkt_entry;

	memset(&err_entry, 0, sizeof(err_entry));

	util_cq = ep->util_ep.rx_cq;

	err_entry.err = FI_EIO;
	err_entry.prov_errno = (int)prov_errno;

	switch (rx_entry->state) {
	case RXR_RX_INIT:
	case RXR_RX_UNEXP:
		dlist_remove(&rx_entry->entry);
		break;
	case RXR_RX_MATCHED:
		break;
	case RXR_RX_RECV:
#if ENABLE_DEBUG
		dlist_remove(&rx_entry->rx_pending_entry);
#endif
		break;
	case RXR_RX_QUEUED_CTRL:
	case RXR_RX_QUEUED_CTS_RNR:
	case RXR_RX_QUEUED_EOR:
		dlist_remove(&rx_entry->queued_entry);
		break;
	default:
		FI_WARN(&rxr_prov, FI_LOG_CQ, "rx_entry unknown state %d\n",
			rx_entry->state);
		assert(0 && "rx_entry unknown state");
	}

	dlist_foreach_container_safe(&rx_entry->queued_pkts,
				     struct rxr_pkt_entry,
				     pkt_entry, entry, tmp)
		rxr_pkt_entry_release_tx(ep, pkt_entry);

	if (rx_entry->unexp_pkt) {
		rxr_pkt_entry_release_rx(ep, rx_entry->unexp_pkt);
		rx_entry->unexp_pkt = NULL;
	}

	if (rx_entry->fi_flags & FI_MULTI_RECV)
		rxr_msg_multi_recv_handle_completion(ep, rx_entry);

	err_entry.flags = rx_entry->cq_entry.flags;
	if (rx_entry->state != RXR_RX_UNEXP)
		err_entry.op_context = rx_entry->cq_entry.op_context;
	err_entry.buf = rx_entry->cq_entry.buf;
	err_entry.data = rx_entry->cq_entry.data;
	err_entry.tag = rx_entry->cq_entry.tag;

	rxr_msg_multi_recv_free_posted_entry(ep, rx_entry);

        FI_WARN(&rxr_prov, FI_LOG_CQ,
		"rxr_cq_handle_rx_error: err: %d, prov_err: %s (%d)\n",
		err_entry.err, fi_strerror(-err_entry.prov_errno),
		err_entry.prov_errno);

	/*
	 * TODO: We can't free the rx_entry as we may receive additional
	 * packets for this entry. Add ref counting so the rx_entry can safely
	 * be freed once all packets are accounted for.
	 */
	//rxr_release_rx_entry(ep, rx_entry);

	efa_cntr_report_error(&ep->util_ep, err_entry.flags);
	return ofi_cq_write_error(util_cq, &err_entry);
}

/*
 * Teardown tx_entry and write an error cq entry. With our current protocol the
 * receiver will only send a CTS once the window is exhausted, meaning that all
 * data packets for that window will have been received successfully. This
 * means that the receiver will not send any CTS packets if the sender
 * encounters and error sending data packets. If that changes in the future we
 * will need to be careful to prevent tx_id mismatch.
 *
 * TODO: add NACK message to tear down receive side state
 */
int rxr_cq_handle_tx_error(struct rxr_ep *ep, struct rxr_tx_entry *tx_entry,
			   ssize_t prov_errno)
{
	struct fi_cq_err_entry err_entry;
	struct util_cq *util_cq;
	uint32_t api_version;
	struct dlist_entry *tmp;
	struct rxr_pkt_entry *pkt_entry;

	memset(&err_entry, 0, sizeof(err_entry));

	util_cq = ep->util_ep.tx_cq;
	api_version = util_cq->domain->fabric->fabric_fid.api_version;

	err_entry.err = FI_EIO;
	err_entry.prov_errno = (int)prov_errno;

	switch (tx_entry->state) {
	case RXR_TX_REQ:
		break;
	case RXR_TX_SEND:
		dlist_remove(&tx_entry->entry);
		break;
	case RXR_TX_QUEUED_CTRL:
	case RXR_TX_QUEUED_SHM_RMA:
	case RXR_TX_QUEUED_REQ_RNR:
	case RXR_TX_QUEUED_DATA_RNR:
		dlist_remove(&tx_entry->queued_entry);
		break;
	case RXR_TX_SENT_READRSP:
	case RXR_TX_WAIT_READ_FINISH:
		break;
	default:
		FI_WARN(&rxr_prov, FI_LOG_CQ, "tx_entry unknown state %d\n",
			tx_entry->state);
		assert(0 && "tx_entry unknown state");
	}

	dlist_foreach_container_safe(&tx_entry->queued_pkts,
				     struct rxr_pkt_entry,
				     pkt_entry, entry, tmp)
		rxr_pkt_entry_release_tx(ep, pkt_entry);

	err_entry.flags = tx_entry->cq_entry.flags;
	err_entry.op_context = tx_entry->cq_entry.op_context;
	err_entry.buf = tx_entry->cq_entry.buf;
	err_entry.data = tx_entry->cq_entry.data;
	err_entry.tag = tx_entry->cq_entry.tag;
	if (FI_VERSION_GE(api_version, FI_VERSION(1, 5)))
		err_entry.err_data_size = 0;

	FI_WARN(&rxr_prov, FI_LOG_CQ,
		"rxr_cq_handle_tx_error: err: %d, prov_err: %s (%d)\n",
		err_entry.err, fi_strerror(-err_entry.prov_errno),
		err_entry.prov_errno);

	/*
	 * TODO: We can't free the tx_entry as we may receive a control packet
	 * packet for this entry. Add ref counting so the tx_entry can safely
	 * be freed once all packets are accounted for.
	 */
	//rxr_release_tx_entry(ep, tx_entry);

	efa_cntr_report_error(&ep->util_ep, tx_entry->cq_entry.flags);
	return ofi_cq_write_error(util_cq, &err_entry);
}

/*
 * Queue a packet on the appropriate list when an RNR error is received.
 */
static inline void rxr_cq_queue_pkt(struct rxr_ep *ep,
				    struct dlist_entry *list,
				    struct rxr_pkt_entry *pkt_entry)
{
	struct rxr_peer *peer;

	peer = rxr_ep_get_peer(ep, pkt_entry->addr);

	/*
	 * Queue the packet if it has not been retransmitted yet.
	 */
	if (pkt_entry->state != RXR_PKT_ENTRY_RNR_RETRANSMIT) {
		pkt_entry->state = RXR_PKT_ENTRY_RNR_RETRANSMIT;
		peer->rnr_queued_pkt_cnt++;
		goto queue_pkt;
	}

	/*
	 * Otherwise, increase the backoff if the peer is already not in
	 * backoff. Reset the timer when starting backoff or if another RNR for
	 * a retransmitted packet is received while waiting for the timer to
	 * expire.
	 */
	peer->rnr_ts = ofi_gettime_us();
	if (peer->flags & RXR_PEER_IN_BACKOFF)
		goto queue_pkt;

	peer->flags |= RXR_PEER_IN_BACKOFF;

	if (!peer->timeout_interval) {
		if (rxr_env.timeout_interval)
			peer->timeout_interval = rxr_env.timeout_interval;
		else
			peer->timeout_interval = MAX(RXR_RAND_MIN_TIMEOUT,
						     rand() %
						     RXR_RAND_MAX_TIMEOUT);

		peer->rnr_timeout_exp = 1;
		FI_DBG(&rxr_prov, FI_LOG_EP_DATA,
		       "initializing backoff timeout for peer: %" PRIu64
		       " timeout: %d rnr_queued_pkts: %d\n",
		       pkt_entry->addr, peer->timeout_interval,
		       peer->rnr_queued_pkt_cnt);
	} else {
		/* Only backoff once per peer per progress thread loop. */
		if (!(peer->flags & RXR_PEER_BACKED_OFF)) {
			peer->flags |= RXR_PEER_BACKED_OFF;
			peer->rnr_timeout_exp++;
			FI_DBG(&rxr_prov, FI_LOG_EP_DATA,
			       "increasing backoff for peer: %" PRIu64
			       " rnr_timeout_exp: %d rnr_queued_pkts: %d\n",
			       pkt_entry->addr, peer->rnr_timeout_exp,
			       peer->rnr_queued_pkt_cnt);
		}
	}
	dlist_insert_tail(&peer->rnr_entry,
			  &ep->peer_backoff_list);

queue_pkt:
#if ENABLE_DEBUG
	dlist_remove(&pkt_entry->dbg_entry);
#endif
	dlist_insert_tail(&pkt_entry->entry, list);
}

int rxr_cq_handle_cq_error(struct rxr_ep *ep, ssize_t err)
{
	struct fi_cq_err_entry err_entry;
	struct rxr_pkt_entry *pkt_entry;
	struct rxr_rx_entry *rx_entry;
	struct rxr_tx_entry *tx_entry;
	struct rxr_peer *peer;
	ssize_t ret;

	memset(&err_entry, 0, sizeof(err_entry));

	/*
	 * If the cq_read failed with another error besides -FI_EAVAIL or
	 * the cq_readerr fails we don't know if this is an rx or tx error.
	 * We'll write an error eq entry to the event queue instead.
	 */

	err_entry.err = FI_EIO;
	err_entry.prov_errno = (int)err;

	if (err != -FI_EAVAIL) {
		FI_WARN(&rxr_prov, FI_LOG_CQ, "fi_cq_read: %s\n",
			fi_strerror(-err));
		goto write_err;
	}

	ret = fi_cq_readerr(ep->rdm_cq, &err_entry, 0);
	if (ret != 1) {
		if (ret < 0) {
			FI_WARN(&rxr_prov, FI_LOG_CQ, "fi_cq_readerr: %s\n",
				fi_strerror(-ret));
			err_entry.prov_errno = ret;
		} else {
			FI_WARN(&rxr_prov, FI_LOG_CQ,
				"fi_cq_readerr unexpected size %zu expected %zu\n",
				ret, sizeof(err_entry));
			err_entry.prov_errno = -FI_EIO;
		}
		goto write_err;
	}

	if (err_entry.err != -FI_EAGAIN)
		OFI_CQ_STRERROR(&rxr_prov, FI_LOG_WARN, FI_LOG_CQ, ep->rdm_cq,
				&err_entry);

	pkt_entry = (struct rxr_pkt_entry *)err_entry.op_context;
	peer = rxr_ep_get_peer(ep, pkt_entry->addr);

	/*
	 * A handshake send could fail at the core provider if the peer endpoint
	 * is shutdown soon after it receives a send completion for the REQ
	 * packet that included src_address. The handshake itself is irrelevant if
	 * that happens, so just squelch this error entry and move on without
	 * writing an error completion or event to the application.
	 */
	if (rxr_get_base_hdr(pkt_entry->pkt)->type == RXR_HANDSHAKE_PKT) {
		FI_WARN(&rxr_prov, FI_LOG_CQ,
			"Squelching error CQE for RXR_HANDSHAKE_PKT\n");
		/*
		 * HANDSHAKE packets do not have an associated rx/tx entry. Use
		 * the flags instead to determine if this is a send or recv.
		 */
		if (err_entry.flags & FI_SEND) {
			rxr_ep_dec_tx_pending(ep, peer, 1);
			rxr_pkt_entry_release_tx(ep, pkt_entry);
		} else if (err_entry.flags & FI_RECV) {
			rxr_pkt_entry_release_rx(ep, pkt_entry);
		} else {
			assert(0 && "unknown err_entry flags in HANDSHAKE packet");
		}
		return 0;
	}

	if (!pkt_entry->x_entry) {
		/*
		 * A NULL x_entry means this is a recv posted buf pkt_entry.
		 * Since we don't have any context besides the error code,
		 * we will write to the eq instead.
		 */
		rxr_pkt_entry_release_rx(ep, pkt_entry);
		goto write_err;
	}

	/*
	 * If x_entry is set this rx or tx entry error is for a sent
	 * packet. Decrement the tx_pending counter and fall through to
	 * the rx or tx entry handlers.
	 */
	if (!peer->is_local)
		rxr_ep_dec_tx_pending(ep, peer, 1);
	if (RXR_GET_X_ENTRY_TYPE(pkt_entry) == RXR_TX_ENTRY) {
		tx_entry = (struct rxr_tx_entry *)pkt_entry->x_entry;
		if (err_entry.err != -FI_EAGAIN ||
		    rxr_ep_domain(ep)->resource_mgmt == FI_RM_ENABLED) {
			ret = rxr_cq_handle_tx_error(ep, tx_entry,
						     err_entry.prov_errno);
			rxr_pkt_entry_release_tx(ep, pkt_entry);
			return ret;
		}

		rxr_cq_queue_pkt(ep, &tx_entry->queued_pkts, pkt_entry);
		if (tx_entry->state == RXR_TX_SEND) {
			dlist_remove(&tx_entry->entry);
			tx_entry->state = RXR_TX_QUEUED_DATA_RNR;
			dlist_insert_tail(&tx_entry->queued_entry,
					  &ep->tx_entry_queued_list);
		} else if (tx_entry->state == RXR_TX_REQ) {
			tx_entry->state = RXR_TX_QUEUED_REQ_RNR;
			dlist_insert_tail(&tx_entry->queued_entry,
					  &ep->tx_entry_queued_list);
		}
		return 0;
	} else if (RXR_GET_X_ENTRY_TYPE(pkt_entry) == RXR_RX_ENTRY) {
		rx_entry = (struct rxr_rx_entry *)pkt_entry->x_entry;
		if (err_entry.err != -FI_EAGAIN ||
		    rxr_ep_domain(ep)->resource_mgmt == FI_RM_ENABLED) {
			ret = rxr_cq_handle_rx_error(ep, rx_entry,
						     err_entry.prov_errno);
			rxr_pkt_entry_release_tx(ep, pkt_entry);
			return ret;
		}
		rxr_cq_queue_pkt(ep, &rx_entry->queued_pkts, pkt_entry);
		if (rx_entry->state == RXR_RX_RECV) {
			rx_entry->state = RXR_RX_QUEUED_CTS_RNR;
			dlist_insert_tail(&rx_entry->queued_entry,
					  &ep->rx_entry_queued_list);
		}
		return 0;
	}

	FI_WARN(&rxr_prov, FI_LOG_CQ,
		"%s unknown x_entry state %d\n",
		__func__, RXR_GET_X_ENTRY_TYPE(pkt_entry));
	assert(0 && "unknown x_entry state");
write_err:
	efa_eq_write_error(&ep->util_ep, err_entry.err, err_entry.prov_errno);
	return 0;
}

void rxr_cq_write_rx_completion(struct rxr_ep *ep,
				struct rxr_rx_entry *rx_entry)
{
	struct util_cq *rx_cq = ep->util_ep.rx_cq;
	int ret = 0;
	if (OFI_UNLIKELY(rx_entry->cq_entry.len < rx_entry->total_len)) {
		FI_WARN(&rxr_prov, FI_LOG_CQ,
			"Message truncated: tag: %"PRIu64" len: %"PRIu64" total_len: %zu\n",
			rx_entry->cq_entry.tag,	rx_entry->total_len,
			rx_entry->cq_entry.len);

		ret = ofi_cq_write_error_trunc(ep->util_ep.rx_cq,
					       rx_entry->cq_entry.op_context,
					       rx_entry->cq_entry.flags,
					       rx_entry->total_len,
					       rx_entry->cq_entry.buf,
					       rx_entry->cq_entry.data,
					       rx_entry->cq_entry.tag,
					       rx_entry->total_len -
					       rx_entry->cq_entry.len);

		rxr_rm_rx_cq_check(ep, rx_cq);

		if (OFI_UNLIKELY(ret))
			FI_WARN(&rxr_prov, FI_LOG_CQ,
				"Unable to write recv error cq: %s\n",
				fi_strerror(-ret));

		efa_cntr_report_error(&ep->util_ep, rx_entry->cq_entry.flags);
		return;
	}

	if (!(rx_entry->rxr_flags & RXR_RECV_CANCEL) &&
	    (ofi_need_completion(rxr_rx_flags(ep), rx_entry->fi_flags) ||
	     (rx_entry->cq_entry.flags & FI_MULTI_RECV))) {
		FI_DBG(&rxr_prov, FI_LOG_CQ,
		       "Writing recv completion for rx_entry from peer: %"
		       PRIu64 " rx_id: %" PRIu32 " msg_id: %" PRIu32
		       " tag: %lx total_len: %" PRIu64 "\n",
		       rx_entry->addr, rx_entry->rx_id, rx_entry->msg_id,
		       rx_entry->cq_entry.tag, rx_entry->total_len);

		if (ep->util_ep.caps & FI_SOURCE)
			ret = ofi_cq_write_src(rx_cq,
					       rx_entry->cq_entry.op_context,
					       rx_entry->cq_entry.flags,
					       rx_entry->cq_entry.len,
					       rx_entry->cq_entry.buf,
					       rx_entry->cq_entry.data,
					       rx_entry->cq_entry.tag,
					       rx_entry->addr);
		else
			ret = ofi_cq_write(rx_cq,
					   rx_entry->cq_entry.op_context,
					   rx_entry->cq_entry.flags,
					   rx_entry->cq_entry.len,
					   rx_entry->cq_entry.buf,
					   rx_entry->cq_entry.data,
					   rx_entry->cq_entry.tag);

		rxr_rm_rx_cq_check(ep, rx_cq);

		if (OFI_UNLIKELY(ret)) {
			FI_WARN(&rxr_prov, FI_LOG_CQ,
				"Unable to write recv completion: %s\n",
				fi_strerror(-ret));
			if (rxr_cq_handle_rx_error(ep, rx_entry, ret))
				assert(0 && "failed to write err cq entry");
			return;
		}
	}

	efa_cntr_report_rx_completion(&ep->util_ep, rx_entry->cq_entry.flags);
}

void rxr_cq_handle_rx_completion(struct rxr_ep *ep,
				 struct rxr_pkt_entry *pkt_entry,
				 struct rxr_rx_entry *rx_entry)
{
	struct rxr_tx_entry *tx_entry = NULL;

	if (rx_entry->cq_entry.flags & FI_WRITE) {
		/*
		 * must be on the remote side, notify cq/counter
		 * if FI_RMA_EVENT is requested or REMOTE_CQ_DATA is on
		 */
		if (rx_entry->cq_entry.flags & FI_REMOTE_CQ_DATA)
			rxr_cq_write_rx_completion(ep, rx_entry);
		else if (ep->util_ep.caps & FI_RMA_EVENT)
			efa_cntr_report_rx_completion(&ep->util_ep, rx_entry->cq_entry.flags);

		rxr_pkt_entry_release_rx(ep, pkt_entry);
		return;
	}

	if (rx_entry->cq_entry.flags & FI_READ) {
		/* Note for emulated FI_READ, there is an rx_entry on
		 * both initiator side and on remote side.
		 * However, only on the initiator side,
		 * rxr_cq_handle_rx_completion() will be called.
		 * The following shows the sequence of events that
		 * is happening
		 *
		 * Initiator side                 Remote side
		 * create tx_entry
		 * create rx_entry
		 * send rtr(with rx_id)
		 *                                receive rtr
		 *                                create rx_entry
		 *                                create tx_entry
		 *                                tx_entry sending data
		 * rx_entry receiving data
		 * receive completed              send completed
		 * handle_rx_completion()         handle_pkt_send_completion()
		 * |->write_tx_completion()       |-> if (FI_RMA_EVENT)
		 *                                         write_rx_completion()
		 *
		 * As can be seen, although there is a rx_entry on remote side,
		 * the entry will not enter into rxr_cq_handle_rx_completion
		 * So at this point we must be on the initiator side, we
		 *     1. find the corresponding tx_entry
		 *     2. call rxr_cq_write_tx_completion()
		 */
		tx_entry = ofi_bufpool_get_ibuf(ep->tx_entry_pool, rx_entry->rma_loc_tx_id);
		assert(tx_entry->state == RXR_TX_WAIT_READ_FINISH);
		if (tx_entry->fi_flags & FI_COMPLETION) {
			/* Note write_tx_completion() will release tx_entry */
			rxr_cq_write_tx_completion(ep, tx_entry);
		} else {
			efa_cntr_report_tx_completion(&ep->util_ep, tx_entry->cq_entry.flags);
			rxr_release_tx_entry(ep, tx_entry);
		}

		/*
		 * do not call rxr_release_rx_entry here because
		 * caller will release
		 */
		rxr_pkt_entry_release_rx(ep, pkt_entry);
		return;
	}

	if (rx_entry->fi_flags & FI_MULTI_RECV)
		rxr_msg_multi_recv_handle_completion(ep, rx_entry);

	rxr_cq_write_rx_completion(ep, rx_entry);
	rxr_pkt_entry_release_rx(ep, pkt_entry);
	return;
}

int rxr_cq_reorder_msg(struct rxr_ep *ep,
		       struct rxr_peer *peer,
		       struct rxr_pkt_entry *pkt_entry)
{
	struct rxr_pkt_entry *ooo_entry;
	struct rxr_pkt_entry *cur_ooo_entry;
	uint32_t msg_id;

	assert(rxr_get_base_hdr(pkt_entry->pkt)->type >= RXR_REQ_PKT_BEGIN);

	msg_id = rxr_pkt_msg_id(pkt_entry);
	/*
	 * TODO: Initialize peer state  at the time of AV insertion
	 * where duplicate detection is available.
	 */
	if (!peer->rx_init)
		rxr_ep_peer_init_rx(ep, peer);

#if ENABLE_DEBUG
	if (msg_id != ofi_recvwin_next_exp_id(peer->robuf))
		FI_DBG(&rxr_prov, FI_LOG_EP_CTRL,
		       "msg OOO msg_id: %" PRIu32 " expected: %"
		       PRIu32 "\n", msg_id,
		       ofi_recvwin_next_exp_id(peer->robuf));
#endif
	if (ofi_recvwin_is_exp(peer->robuf, msg_id))
		return 0;
	else if (!ofi_recvwin_id_valid(peer->robuf, msg_id))
		return -FI_EALREADY;

	if (OFI_LIKELY(rxr_env.rx_copy_ooo)) {
		assert(pkt_entry->type == RXR_PKT_ENTRY_POSTED);
		ooo_entry = rxr_pkt_entry_clone(ep, ep->rx_ooo_pkt_pool, pkt_entry, RXR_PKT_ENTRY_OOO);
		if (OFI_UNLIKELY(!ooo_entry)) {
			FI_WARN(&rxr_prov, FI_LOG_EP_CTRL,
				"Unable to allocate rx_pkt_entry for OOO msg\n");
			return -FI_ENOMEM;
		}
		rxr_pkt_entry_release_rx(ep, pkt_entry);
	} else {
		ooo_entry = pkt_entry;
	}

	cur_ooo_entry = *ofi_recvwin_get_msg(peer->robuf, msg_id);
	if (cur_ooo_entry) {
		assert(rxr_get_base_hdr(cur_ooo_entry->pkt)->type == RXR_MEDIUM_MSGRTM_PKT ||
		       rxr_get_base_hdr(cur_ooo_entry->pkt)->type == RXR_MEDIUM_TAGRTM_PKT);
		assert(rxr_pkt_msg_id(cur_ooo_entry) == msg_id);
		assert(rxr_pkt_rtm_total_len(cur_ooo_entry) == rxr_pkt_rtm_total_len(ooo_entry));
		rxr_pkt_entry_append(cur_ooo_entry, ooo_entry);
	} else {
		ofi_recvwin_queue_msg(peer->robuf, &ooo_entry, msg_id);
	}

	return 1;
}

void rxr_cq_proc_pending_items_in_recvwin(struct rxr_ep *ep,
					  struct rxr_peer *peer)
{
	struct rxr_pkt_entry *pending_pkt;
	int ret = 0;
	uint32_t msg_id;

	while (1) {
		pending_pkt = *ofi_recvwin_peek(peer->robuf);
		if (!pending_pkt || !pending_pkt->pkt)
			return;

		msg_id = rxr_pkt_msg_id(pending_pkt);
		FI_DBG(&rxr_prov, FI_LOG_EP_CTRL,
		       "Processing msg_id %d from robuf\n", msg_id);
		/* rxr_pkt_proc_rtm_rta will write error cq entry if needed */
		ret = rxr_pkt_proc_rtm_rta(ep, pending_pkt);
		*ofi_recvwin_get_next_msg(peer->robuf) = NULL;
		if (OFI_UNLIKELY(ret)) {
			FI_WARN(&rxr_prov, FI_LOG_CQ,
				"Error processing msg_id %d from robuf: %s\n",
				msg_id, fi_strerror(-ret));
			return;
		}
	}
	return;
}

/* Handle two scenarios:
 *  1. RMA writes with immediate data at remote endpoint,
 *  2. atomic completion on the requester
 * write completion for both
 */
void rxr_cq_handle_shm_completion(struct rxr_ep *ep, struct fi_cq_data_entry *cq_entry, fi_addr_t src_addr)
{
	struct util_cq *target_cq;
	int ret;

	if (cq_entry->flags & FI_ATOMIC) {
		target_cq = ep->util_ep.tx_cq;
	} else {
		assert(cq_entry->flags & FI_REMOTE_CQ_DATA);
		target_cq = ep->util_ep.rx_cq;
	}

	if (ep->util_ep.caps & FI_SOURCE)
		ret = ofi_cq_write_src(target_cq,
				       cq_entry->op_context,
				       cq_entry->flags,
				       cq_entry->len,
				       cq_entry->buf,
				       cq_entry->data,
				       0,
				       src_addr);
	else
		ret = ofi_cq_write(target_cq,
				   cq_entry->op_context,
				   cq_entry->flags,
				   cq_entry->len,
				   cq_entry->buf,
				   cq_entry->data,
				   0);

	rxr_rm_rx_cq_check(ep, target_cq);

	if (OFI_UNLIKELY(ret)) {
		FI_WARN(&rxr_prov, FI_LOG_CQ,
			"Unable to write a cq entry for shm operation: %s\n",
			fi_strerror(-ret));
		efa_eq_write_error(&ep->util_ep, FI_EIO, ret);
	}

	if (cq_entry->flags & FI_ATOMIC) {
		efa_cntr_report_tx_completion(&ep->util_ep, cq_entry->flags);
	} else {
		assert(cq_entry->flags & FI_REMOTE_CQ_DATA);
		efa_cntr_report_rx_completion(&ep->util_ep, cq_entry->flags);
	}
}

static inline
bool rxr_cq_need_tx_completion(struct rxr_ep *ep,
			       struct rxr_tx_entry *tx_entry)

{
	if (tx_entry->fi_flags & RXR_NO_COMPLETION)
		return false;

	/*
	 * ep->util_ep.tx_msg_flags is either 0 or FI_COMPLETION, depend on
	 * whether app specfied FI_SELECTIVE_COMPLETION when binding CQ.
	 * (ep->util_ep.tx_msg_flags was set in ofi_ep_bind_cq())
	 *
	 * If tx_msg_flags is 0, we only write completion when app specify
	 * FI_COMPLETION in flags.
	 */
	return ep->util_ep.tx_msg_flags == FI_COMPLETION ||
	       tx_entry->fi_flags & FI_COMPLETION;
}


void rxr_cq_write_tx_completion(struct rxr_ep *ep,
				struct rxr_tx_entry *tx_entry)
{
	struct util_cq *tx_cq = ep->util_ep.tx_cq;
	int ret;

	if (rxr_cq_need_tx_completion(ep, tx_entry)) {
		FI_DBG(&rxr_prov, FI_LOG_CQ,
		       "Writing send completion for tx_entry to peer: %" PRIu64
		       " tx_id: %" PRIu32 " msg_id: %" PRIu32 " tag: %lx len: %"
		       PRIu64 "\n",
		       tx_entry->addr, tx_entry->tx_id, tx_entry->msg_id,
		       tx_entry->cq_entry.tag, tx_entry->total_len);

		/* TX completions should not send peer address to util_cq */
		if (ep->util_ep.caps & FI_SOURCE)
			ret = ofi_cq_write_src(tx_cq,
					       tx_entry->cq_entry.op_context,
					       tx_entry->cq_entry.flags,
					       tx_entry->cq_entry.len,
					       tx_entry->cq_entry.buf,
					       tx_entry->cq_entry.data,
					       tx_entry->cq_entry.tag,
					       FI_ADDR_NOTAVAIL);
		else
			ret = ofi_cq_write(tx_cq,
					   tx_entry->cq_entry.op_context,
					   tx_entry->cq_entry.flags,
					   tx_entry->cq_entry.len,
					   tx_entry->cq_entry.buf,
					   tx_entry->cq_entry.data,
					   tx_entry->cq_entry.tag);

		rxr_rm_tx_cq_check(ep, tx_cq);

		if (OFI_UNLIKELY(ret)) {
			FI_WARN(&rxr_prov, FI_LOG_CQ,
				"Unable to write send completion: %s\n",
				fi_strerror(-ret));
			if (rxr_cq_handle_tx_error(ep, tx_entry, ret))
				assert(0 && "failed to write err cq entry");
			return;
		}
	}

	efa_cntr_report_tx_completion(&ep->util_ep, tx_entry->cq_entry.flags);
	rxr_release_tx_entry(ep, tx_entry);
	return;
}

int rxr_tx_entry_mr_dereg(struct rxr_tx_entry *tx_entry)
{
	int i, err = 0;

	for (i = 0; i < tx_entry->iov_count; i++) {
		if (tx_entry->mr[i]) {
			err = fi_close((struct fid *)tx_entry->mr[i]);
			if (OFI_UNLIKELY(err)) {
				FI_WARN(&rxr_prov, FI_LOG_CQ, "mr dereg failed. err=%d\n", err);
				return err;
			}

			tx_entry->mr[i] = NULL;
		}
	}

	return 0;
}

void rxr_cq_handle_tx_completion(struct rxr_ep *ep, struct rxr_tx_entry *tx_entry)
{
	int ret;
	struct rxr_peer *peer;

	if (tx_entry->state == RXR_TX_SEND)
		dlist_remove(&tx_entry->entry);

	if (efa_mr_cache_enable && rxr_ep_mr_local(ep)) {
		ret = rxr_tx_entry_mr_dereg(tx_entry);
		if (OFI_UNLIKELY(ret)) {
			FI_WARN(&rxr_prov, FI_LOG_MR,
				"In-line memory deregistration failed with error: %s.\n",
				fi_strerror(-ret));
		}
	}

	peer = rxr_ep_get_peer(ep, tx_entry->addr);
	peer->tx_credits += tx_entry->credit_allocated;

	if (tx_entry->cq_entry.flags & FI_READ) {
		/*
		 * this must be on remote side
		 * see explaination on rxr_cq_handle_rx_completion
		 */
		struct rxr_rx_entry *rx_entry = NULL;

		rx_entry = ofi_bufpool_get_ibuf(ep->rx_entry_pool, tx_entry->rma_loc_rx_id);
		assert(rx_entry);
		assert(rx_entry->state == RXR_RX_WAIT_READ_FINISH);

		if (ep->util_ep.caps & FI_RMA_EVENT) {
			rx_entry->cq_entry.len = rx_entry->total_len;
			rx_entry->bytes_done = rx_entry->total_len;
			efa_cntr_report_rx_completion(&ep->util_ep, rx_entry->cq_entry.flags);
		}

		rxr_release_rx_entry(ep, rx_entry);
		/* just release tx, do not write completion */
		rxr_release_tx_entry(ep, tx_entry);
	} else if (tx_entry->cq_entry.flags & FI_WRITE) {
		if (tx_entry->fi_flags & FI_COMPLETION) {
			rxr_cq_write_tx_completion(ep, tx_entry);
		} else {
			efa_cntr_report_tx_completion(&ep->util_ep, tx_entry->cq_entry.flags);
			rxr_release_tx_entry(ep, tx_entry);
		}
	} else {
		assert(tx_entry->cq_entry.flags & FI_SEND);
		rxr_cq_write_tx_completion(ep, tx_entry);
	}
}

static int rxr_cq_close(struct fid *fid)
{
	int ret;
	struct util_cq *cq;

	cq = container_of(fid, struct util_cq, cq_fid.fid);
	ret = ofi_cq_cleanup(cq);
	if (ret)
		return ret;
	free(cq);
	return 0;
}

static struct fi_ops rxr_cq_fi_ops = {
	.size = sizeof(struct fi_ops),
	.close = rxr_cq_close,
	.bind = fi_no_bind,
	.control = fi_no_control,
	.ops_open = fi_no_ops_open,
};

static struct fi_ops_cq rxr_cq_ops = {
	.size = sizeof(struct fi_ops_cq),
	.read = ofi_cq_read,
	.readfrom = ofi_cq_readfrom,
	.readerr = ofi_cq_readerr,
	.sread = fi_no_cq_sread,
	.sreadfrom = fi_no_cq_sreadfrom,
	.signal = fi_no_cq_signal,
	.strerror = rxr_cq_strerror,
};

int rxr_cq_open(struct fid_domain *domain, struct fi_cq_attr *attr,
		struct fid_cq **cq_fid, void *context)
{
	int ret;
	struct util_cq *cq;
	struct rxr_domain *rxr_domain;

	if (attr->wait_obj != FI_WAIT_NONE)
		return -FI_ENOSYS;

	cq = calloc(1, sizeof(*cq));
	if (!cq)
		return -FI_ENOMEM;

	rxr_domain = container_of(domain, struct rxr_domain,
				  util_domain.domain_fid);
	/* Override user cq size if it's less than recommended cq size */
	attr->size = MAX(rxr_domain->cq_size, attr->size);

	ret = ofi_cq_init(&rxr_prov, domain, attr, cq,
			  &ofi_cq_progress, context);

	if (ret)
		goto free;

	*cq_fid = &cq->cq_fid;
	(*cq_fid)->fid.ops = &rxr_cq_fi_ops;
	(*cq_fid)->ops = &rxr_cq_ops;
	return 0;
free:
	free(cq);
	return ret;
}
