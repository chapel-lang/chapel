/* SPDX-License-Identifier: BSD-2-Clause OR GPL-2.0-only */
/* SPDX-FileCopyrightText: Copyright Amazon.com, Inc. or its affiliates. All rights reserved. */

#include <assert.h>
#include "efa.h"
#include "efa_cntr.h"
#include "efa_rdm_msg.h"
#include "efa_rdm_rma.h"
#include "efa_rdm_atomic.h"
#include "efa_rdm_pke_cmd.h"
#include "efa_rdm_pke_nonreq.h"
#include "efa_rdm_tracepoint.h"
#include "efa_rdm_pke_req.h"
#include "efa_rdm_pkt_type.h"

void efa_rdm_txe_construct(struct efa_rdm_ope *txe,
			   struct efa_rdm_ep *ep,
			   struct efa_rdm_peer *peer,
			   const struct fi_msg *msg,
			   uint32_t op, uint64_t flags)
{
	uint64_t tx_op_flags;

	txe->ep = ep;
	txe->type = EFA_RDM_TXE;
	txe->op = op;
	txe->tx_id = ofi_buf_index(txe);
	txe->state = EFA_RDM_TXE_REQ;
	txe->peer = peer;
	/* peer would be NULL for local read operation */
	if (txe->peer) {
		dlist_insert_tail(&txe->peer_entry, &txe->peer->txe_list);
	}

	txe->internal_flags = 0;
	txe->bytes_received = 0;
	txe->bytes_copied = 0;
	txe->bytes_acked = 0;
	txe->bytes_sent = 0;
	txe->window = 0;
	txe->iov_count = msg->iov_count;
	txe->rma_iov_count = 0;
	txe->msg_id = 0;
	txe->efa_outstanding_tx_ops = 0;
	dlist_init(&txe->queued_pkts);

	memcpy(txe->iov, msg->msg_iov, sizeof(struct iovec) * msg->iov_count);
	memset(txe->mr, 0, sizeof(*txe->mr) * msg->iov_count);
	if (msg->desc)
		memcpy(txe->desc, msg->desc, sizeof(*msg->desc) * msg->iov_count);
	else
		memset(txe->desc, 0, sizeof(txe->desc));

	/* cq_entry on completion */
	txe->cq_entry.op_context = msg->context;
	txe->cq_entry.data = msg->data;
	txe->cq_entry.len = ofi_total_iov_len(txe->iov, txe->iov_count);
	txe->cq_entry.buf = OFI_LIKELY(txe->cq_entry.len > 0) ? txe->iov[0].iov_base : NULL;

	/*
	 * txe->iov_count is 0 only when posting handshake packets
	 *
	 * It's a bit silly to allocate extra header before the contents
	 * of the handshake packet and then consume that here. So instead
	 * just don't consume the prefix header if iov_count is 0.
	 *
	 * A send or RMA or atomic call from the application cannot have
	 * iov_count 0, so this is safe.
	 */
	if (txe->iov_count && ep->base_ep.info->mode & FI_MSG_PREFIX) {
		ofi_consume_iov_desc(txe->iov, txe->desc, &txe->iov_count, ep->msg_prefix_size);
	}
	txe->total_len = ofi_total_iov_len(txe->iov, txe->iov_count);

	/* set flags */
	assert(ep->base_ep.util_ep.tx_msg_flags == 0 ||
	       ep->base_ep.util_ep.tx_msg_flags == FI_COMPLETION);
	tx_op_flags = ep->base_ep.util_ep.tx_op_flags;
	if (ep->base_ep.util_ep.tx_msg_flags == 0)
		tx_op_flags &= ~FI_COMPLETION;
	txe->fi_flags = flags | tx_op_flags;
	txe->bytes_runt = 0;
	dlist_init(&txe->entry);

	switch (op) {
	case ofi_op_tagged:
		txe->cq_entry.flags = FI_TRANSMIT | FI_MSG | FI_TAGGED;
		break;
	case ofi_op_write:
		txe->cq_entry.flags = FI_RMA | FI_WRITE;
		break;
	case ofi_op_read_req:
		txe->cq_entry.flags = FI_RMA | FI_READ;
		break;
	case ofi_op_msg:
		txe->cq_entry.flags = FI_TRANSMIT | FI_MSG;
		break;
	case ofi_op_atomic:
		txe->cq_entry.flags = (FI_WRITE | FI_ATOMIC);
		break;
	case ofi_op_atomic_fetch:
	case ofi_op_atomic_compare:
		txe->cq_entry.flags = (FI_READ | FI_ATOMIC);
		break;
	default:
		EFA_WARN(FI_LOG_CQ, "invalid operation type\n");
		assert(0);
	}
}

void efa_rdm_txe_release(struct efa_rdm_ope *txe)
{
	int i, err = 0;
	struct dlist_entry *tmp;
	struct efa_rdm_pke *pkt_entry;

	/* txe->peer would be NULL for local read operation */
	if (txe->peer) {
		dlist_remove(&txe->peer_entry);
	}

	for (i = 0; i < txe->iov_count; i++) {
		if (txe->mr[i]) {
			err = fi_close((struct fid *)txe->mr[i]);
			if (OFI_UNLIKELY(err)) {
				EFA_WARN(FI_LOG_CQ, "mr dereg failed. err=%d\n", err);
				efa_base_ep_write_eq_error(&txe->ep->base_ep, err, FI_EFA_ERR_MR_DEREG);
			}

			txe->mr[i] = NULL;
		}
	}

	dlist_remove(&txe->ep_entry);

	/**
	 * Make sure the entry is removed
	 * from ope_longcts_list when the ope
	 * is released for whatever reasons.
	 */
	if (txe->state == EFA_RDM_OPE_SEND)
		dlist_remove(&txe->entry);

	dlist_foreach_container_safe(&txe->queued_pkts,
				     struct efa_rdm_pke,
				     pkt_entry, entry, tmp) {
		efa_rdm_pke_release_tx(pkt_entry);
	}

	if (txe->internal_flags & EFA_RDM_OPE_QUEUED_FLAGS) {
		dlist_remove(&txe->queued_entry);
		txe->internal_flags &= ~EFA_RDM_OPE_QUEUED_FLAGS;
	}

#ifdef ENABLE_EFA_POISONING
	efa_rdm_poison_mem_region(txe,
			      sizeof(struct efa_rdm_ope));
#endif
	ofi_buf_free(txe);
}

/**
 * @brief Release the efa specific resources in efa_rdm_rxe
 *
 * @param rxe efa_rdm_ope
 */
void efa_rdm_rxe_release_internal(struct efa_rdm_ope *rxe)
{
	struct efa_rdm_pke *pkt_entry;
	struct dlist_entry *tmp;
	int i, err;

	if (rxe->peer)
		dlist_remove(&rxe->peer_entry);

	dlist_remove(&rxe->ep_entry);

	/**
	 * Make sure the entry is removed
	 * from ope_longcts_list when the ope
	 * is released for whatever reasons.
	 */
	if (rxe->state == EFA_RDM_OPE_SEND)
		dlist_remove(&rxe->entry);

	if (rxe->rxe_map)
		efa_rdm_rxe_map_remove(rxe->rxe_map, rxe->msg_id, rxe);

	for (i = 0; i < rxe->iov_count; i++) {
		if (rxe->mr[i]) {
			err = fi_close((struct fid *)rxe->mr[i]);
			if (OFI_UNLIKELY(err)) {
				EFA_WARN(FI_LOG_CQ, "mr dereg failed. err=%d\n", err);
				efa_base_ep_write_eq_error(&rxe->ep->base_ep, err, FI_EFA_ERR_MR_DEREG);
			}

			rxe->mr[i] = NULL;
		}
	}

	dlist_foreach_container_safe(&rxe->queued_pkts,
				     struct efa_rdm_pke,
				     pkt_entry, entry, tmp)
		efa_rdm_pke_release_tx(pkt_entry);

	if (rxe->internal_flags & EFA_RDM_OPE_QUEUED_FLAGS) {
		dlist_remove(&rxe->queued_entry);
		rxe->internal_flags &= ~EFA_RDM_OPE_QUEUED_FLAGS;
	}

#ifdef ENABLE_EFA_POISONING
	efa_rdm_poison_mem_region(rxe,
			      sizeof(struct efa_rdm_ope));
#endif
	ofi_buf_free(rxe);
}

void efa_rdm_rxe_release(struct efa_rdm_ope *rxe)
{
	/* release the resource created by util srx */
	if (rxe->peer_rxe) {
		efa_rdm_ep_get_peer_srx(rxe->ep)->owner_ops->free_entry(rxe->peer_rxe);
		rxe->peer_rxe = NULL;
	}

	/* release efa specific resources in rxe */
	efa_rdm_rxe_release_internal(rxe);
}

/**
 * @brief try to fill the desc field of an ope by memory registration
 *
 * The desc field of ope contains the memory descriptors of
 * the user's data buffer.
 *
 * For EFA provider, a data buffer's memory descriptor is a pointer to an
 * efa_mr object, which contains the memory registration information
 * of the data buffer.
 *
 * EFA provider does not require user to provide a descriptor when
 * user's data buffer is on host memory (Though user can register
 * its buffer, and provide its descriptor as an optimization).
 *
 * However, there are a few occations that EFA device
 * require memory to be register with them:
 *
 * When EFA device is used to send data:
 *
 *   If a non-read based protocol (such as eager, meidum, longcts)
 *   is used, the send buffer must be registered with EFA device.
 *
 *   If a read based protocol is used, both send buffer
 *   and receive buffer must be registered with EFA device.
 *
 * Therefore, when user did not provide descriptors for the buffer(s),
 * EFA provider need to bridge the gap.
 *
 * On sender side:
 *
 * First, EFA provider can copy the user data to a pre-registered bounce
 * buffer, then send data from bounce buffer. (this happens when
 * EFA device is used, and eager protocol is used)
 *
 * Second, EFA provider can register the user's buffer and fill desc
 * (by calling this function). then send directly from send buffer.
 * Because of the high cost of memory registration, this happens
 * only when MR cache is available, which is checked by the caller
 * of this function on sender side. (this happens when
 * EFA device is used with non-eager protocols and
 *
 * This function is not guaranteed to fill all descriptors (which
 * is why the function name has try). When memory registration fail due
 * to limited resources, EFA provider may either fallback to
 * to use bounce buffer, or return -FI_EAGAIN to user and let
 * user run progress engine, which will release memory from
 * MR cache.
 *
 * On receiver side:
 *
 * The only occasion receiver buffer need to be registered is
 * when EFA device is used with a read base message protocol,
 * in which case this function is called for the registration.
 *
 * If the function did not fill desc due to temporarily out
 * of resource. EFA provider need to run progress engine to release
 * memory registration then retry.
 *
 * @param[in,out]	ope		contains the inforation of a TX/RX operation
 * @param[in]		mr_iov_start	the IOV index to start generating descriptors
 * @param[in]		access			the access flag for the memory registation.
 *
 */
void efa_rdm_ope_try_fill_desc(struct efa_rdm_ope *ope, int mr_iov_start, uint64_t access)
{
	struct efa_domain *domain;
	int i, err;

	for (i = mr_iov_start; i < ope->iov_count; ++i) {
		if (ope->desc[i])
			continue;

		if (OFI_UNLIKELY(ope->ep->base_ep.domain->mr_local))
			EFA_WARN(FI_LOG_EP_CTRL,
				 "No valid desc is provided, not compliant with FI_MR_LOCAL. "
				 "buf: %p len: %ld access: %#lx\n",
				 ope->iov[i].iov_base, ope->iov[i].iov_len, access);

		domain = efa_rdm_ep_domain(ope->ep);
		err = domain->internal_buf_mr_regv(
			&domain->util_domain.domain_fid, ope->iov + i, 1,
			access, 0, 0, 0, &ope->mr[i], NULL);

		if (err) {
			EFA_WARN(FI_LOG_EP_CTRL,
				 "Failed to register internal buffer! buf: %p "
				 "len: %ld access: %#lx\n",
				 ope->iov[i].iov_base, ope->iov[i].iov_len,
				 access);

			ope->mr[i] = NULL;
		} else {
			ope->desc[i] = fi_mr_desc(ope->mr[i]);
		}
	}
}

int efa_rdm_txe_prepare_to_be_read(struct efa_rdm_ope *txe, struct fi_rma_iov *read_iov)
{
	int i;

	efa_rdm_ope_try_fill_desc(txe, 0, FI_REMOTE_READ);

	for (i = 0; i < txe->iov_count; ++i) {
		read_iov[i].addr = (uint64_t)txe->iov[i].iov_base;
		read_iov[i].len = txe->iov[i].iov_len;

		if (!txe->desc[i]) {
			/* efa_rdm_ope_try_fill_desc() did not register the memory */
			return -FI_ENOMR;
		}

		read_iov[i].key = fi_mr_key(txe->desc[i]);
	}

	return 0;
}

/**
 * @brief calculate and set the bytes_runt field of a txe
 *
 * bytes_runt is number of bytes for a message to be sent by runting
 *
 * @param[in]		ep			endpoint
 * @param[in,out]	txe	txe to be set
 */
static inline
void efa_rdm_txe_set_runt_size(struct efa_rdm_ep *ep, struct efa_rdm_ope *txe)
{
	assert(txe->type == EFA_RDM_TXE);

	if (txe->bytes_runt > 0)
		return;

	assert(txe->peer);
	txe->bytes_runt = efa_rdm_peer_get_runt_size(txe->peer, ep, txe);

	assert(txe->bytes_runt);
}

/**
 * @brief total data size that will be sent/received via the multiple REQ packets
 *
 * Multi-req protocols send/receive data via multiple REQ packets. Different
 * protocols have different behavior:
 *
 *     Medium protocols send/receive all data via REQ packets
 *
 *     Runting read protocols send/receive part of the data via REQ packets.
 *     The reminder of the data is sent via other type of packets or via RDMA operations.
 *
 * which is why this function is needed.
 *
 * @param[in]          ope             contains operation information
 * @param[in]          pkt_type                REQ packet type
 * @return                     size of total data transfered by REQ packets
 */
size_t efa_rdm_ope_mulreq_total_data_size(struct efa_rdm_ope *ope, int pkt_type)
{
	assert(efa_rdm_pkt_type_is_mulreq(pkt_type));

	if (efa_rdm_pkt_type_is_medium(pkt_type)) {
		return ope->total_len;
	}

	assert(efa_rdm_pkt_type_is_runt(pkt_type));
	return ope->bytes_runt;
}

/**
 * @brief return the maximum data capacity of a REQ packet for an send operation
 *
 * The REQ packet header length is a variable that depends on a number of factors,
 * including:
 *
 *   packet_type, peer_type, cq_data and number of rma iov.
 *
 * As a result the maximum data capacity of a REQ packet for a send operation,(
 * which is the number of bytes of data can be saved in a REQ packet) is different.
 *
 * This function is used to caculate the maxium data capacity.
 *
 * @param[in]		ep			endpoint
 * @param[in]		txe	txe that has all information of
 * 					a send operation
 * @param[in]		pkt_type	type of REQ packet
 *
 * @return		maxiumum number of bytes of data can be save in a REQ packet
 * 			for given send operation and REQ packet type.
 */
size_t efa_rdm_txe_max_req_data_capacity(struct efa_rdm_ep *ep, struct efa_rdm_ope *txe, int pkt_type)
{
	uint16_t header_flags = 0;
	int max_data_offset;

	assert(pkt_type >= EFA_RDM_REQ_PKT_BEGIN);

	if (efa_rdm_peer_need_raw_addr_hdr(txe->peer))
		header_flags |= EFA_RDM_REQ_OPT_RAW_ADDR_HDR;
	else if (efa_rdm_peer_need_connid(txe->peer))
		header_flags |= EFA_RDM_PKT_CONNID_HDR;

	if (txe->fi_flags & FI_REMOTE_CQ_DATA)
		header_flags |= EFA_RDM_REQ_OPT_CQ_DATA_HDR;

	max_data_offset = efa_rdm_pkt_type_get_req_hdr_size(pkt_type, header_flags,
					       txe->rma_iov_count);

	if (efa_rdm_pkt_type_is_runtread(pkt_type)) {
		max_data_offset += txe->iov_count * sizeof(struct fi_rma_iov);
	}

	return ep->mtu_size - max_data_offset;
}

/**
 * @brief prepare ope to send the give pkt type
 *
 * For given packet type, calcuate how many packets are going to be
 * sent. If there are more than 1 packet, calculate how many data
 * each packet will carry.
 *
 * For runting read rtm, also set the "runt_size" field.
 *
 * @param[in]		ep		endpoint
 * @param[in,out]	ope		txe that has all information of
 * 					a send operation
 * @param[in]		pkt_type	type of REQ packet
 * @param[in]		pkt_entry_cnt	number of packets to be sent
 * @param[in]		pkt_entry_data_size_vec data size of each packet. This field
 *                                              is used when pkt_entry_cnt > 1.
 *                                              (for DATA, MEDIUM and RUNTREAD)
 * @return
 * On success, return 0
 * If there is not enough available packet entry in TX packet pool, return -FI_EAGAIN
 */
ssize_t efa_rdm_ope_prepare_to_post_send(struct efa_rdm_ope *ope,
					 int pkt_type,
					 int *pkt_entry_cnt,
					 int *pkt_entry_data_size_vec)
{
	struct efa_rdm_ep *ep;
	size_t total_pkt_entry_data_size; /* total number of bytes send via packet entry's payload */
	size_t single_pkt_entry_data_size;
	size_t single_pkt_entry_max_data_size;
	int i, memory_alignment, remainder, iface;
	int available_tx_pkts;

	ep = ope->ep;
	assert(ep->efa_max_outstanding_tx_ops >=
	       ep->efa_outstanding_tx_ops + ep->efa_rnr_queued_pkt_cnt);

	available_tx_pkts = ep->efa_max_outstanding_tx_ops
		- ep->efa_outstanding_tx_ops
		- ep->efa_rnr_queued_pkt_cnt;

	if (available_tx_pkts == 0)
		return -FI_EAGAIN;

	if (pkt_type == EFA_RDM_CTSDATA_PKT) {
		assert(ope->window);
		*pkt_entry_cnt = (ope->window - 1) / ope->ep->max_data_payload_size + 1;
		if (*pkt_entry_cnt > available_tx_pkts)
			*pkt_entry_cnt = available_tx_pkts;
		assert(*pkt_entry_cnt > 0 && *pkt_entry_cnt <= EFA_RDM_EP_MAX_WR_PER_IBV_POST_SEND);
		for (i = 0; i < *pkt_entry_cnt - 1; ++i)
			pkt_entry_data_size_vec[i] = ope->ep->max_data_payload_size;

		remainder = ope->window - (*pkt_entry_cnt - 1) * ep->max_data_payload_size;
		assert(remainder > 0);
		pkt_entry_data_size_vec[i] = MIN(remainder, ep->max_data_payload_size);
		return 0;
	}

	if (efa_rdm_pkt_type_is_medium(pkt_type) || efa_rdm_pkt_type_is_runt(pkt_type)) {
		if(efa_rdm_pkt_type_is_runt(pkt_type))
			efa_rdm_txe_set_runt_size(ep, ope);

		total_pkt_entry_data_size = efa_rdm_ope_mulreq_total_data_size(ope, pkt_type);
		assert(total_pkt_entry_data_size);

		single_pkt_entry_max_data_size = efa_rdm_txe_max_req_data_capacity(ep, ope, pkt_type);
		assert(single_pkt_entry_max_data_size);

		iface = ope->desc[0] ? ((struct efa_mr*) ope->desc[0])->peer.iface : FI_HMEM_SYSTEM;
		memory_alignment = efa_rdm_ep_get_memory_alignment(ep, iface);

		*pkt_entry_cnt = (total_pkt_entry_data_size - 1) / single_pkt_entry_max_data_size + 1;
		if (*pkt_entry_cnt > available_tx_pkts)
			return -FI_EAGAIN;

		/* when sending multiple packets, it is more performant that the data size of each packet are close
		 * to achieve that, we calculate the single packet size
		 */
		single_pkt_entry_data_size = (total_pkt_entry_data_size - 1) / *pkt_entry_cnt + 1;

		/* each packet must be aligned */
		single_pkt_entry_data_size = single_pkt_entry_data_size & ~(memory_alignment - 1);
		assert(single_pkt_entry_data_size);

		*pkt_entry_cnt = total_pkt_entry_data_size / single_pkt_entry_data_size;
		for (i = 0; i < *pkt_entry_cnt; ++i)
			pkt_entry_data_size_vec[i] = single_pkt_entry_data_size;

		remainder = total_pkt_entry_data_size - (*pkt_entry_cnt) * single_pkt_entry_data_size;
		if (single_pkt_entry_data_size + remainder <= single_pkt_entry_max_data_size) {
			pkt_entry_data_size_vec[*pkt_entry_cnt - 1] += remainder;
		} else {
			pkt_entry_data_size_vec[*pkt_entry_cnt] = remainder;
			*pkt_entry_cnt += 1;
		}

		return (*pkt_entry_cnt <= available_tx_pkts) ? 0 : -FI_EAGAIN;
	}

	/*
	 * pkt_entry_data_size_vec is only set when there are more than 1 packets
	 * to be sent.
	 */
	*pkt_entry_cnt = 1;
	pkt_entry_data_size_vec[0] = -1;
	return 0;
}

/**
 * @brief handle the situation that an error has happened to an RX (receive) operation
 *
 * This function does the following to handle error:
 *
 * 1. write an error cq entry to notify application the rx
 *    operation failed. If write failed, it will write an eq entry.
 *
 * 2. increase error counter.
 *
 * 3. print warning message about the error with self and peer's
 *    raw address.
 *
 * 4. release resources owned by the rxe, such as unexpected
 *    packet entry, because the RX operation is aborted.
 *
 * 5. remove the rxe from queued rxe list for the same reason.
 *
 * Note, It will NOT release the rxe because it is still possible to receive
 * packet for this rxe.
 *
 * @param[in]	rxe	rxe encountered error
 * @param[in]	err			positive libfabric error code
 * @param[in]	prov_errno	positive provider specific error code
 */
void efa_rdm_rxe_handle_error(struct efa_rdm_ope *rxe, int err, int prov_errno)
{
	struct efa_rdm_ep *ep;
	struct fi_cq_err_entry err_entry;
	struct util_cq *util_cq;
	struct dlist_entry *tmp;
	struct efa_rdm_pke *pkt_entry;
	int write_cq_err;
	char err_msg[EFA_ERROR_MSG_BUFFER_LENGTH] = {0};

	assert(rxe->type == EFA_RDM_RXE);

	memset(&err_entry, 0, sizeof(err_entry));

	ep = rxe->ep;
	util_cq = ep->base_ep.util_ep.rx_cq;

	err_entry.err = err;
	err_entry.prov_errno = prov_errno;

	switch (rxe->state) {
	case EFA_RDM_RXE_INIT:
	case EFA_RDM_RXE_UNEXP:
	case EFA_RDM_RXE_MATCHED:
		break;
	case EFA_RDM_OPE_SEND:
		dlist_remove(&rxe->entry);
		break;
	case EFA_RDM_RXE_RECV:
#if ENABLE_DEBUG
		dlist_remove(&rxe->pending_recv_entry);
#endif
		break;
	case EFA_RDM_OPE_ERR:
		/* Already progressed, no-op */
		return;
	default:
		EFA_WARN(FI_LOG_CQ, "rxe unknown state %d\n",
			rxe->state);
		assert(0 && "rxe unknown state");
	}

	rxe->state = EFA_RDM_OPE_ERR;

	dlist_foreach_container_safe(&rxe->queued_pkts,
				     struct efa_rdm_pke,
				     pkt_entry, entry, tmp)
		efa_rdm_pke_release_tx(pkt_entry);

	if (rxe->internal_flags & EFA_RDM_OPE_QUEUED_FLAGS) {
		dlist_remove(&rxe->queued_entry);
		rxe->internal_flags &= ~EFA_RDM_OPE_QUEUED_FLAGS;
	}

	if (rxe->unexp_pkt) {
		efa_rdm_pke_release_rx_list(rxe->unexp_pkt);
		rxe->unexp_pkt = NULL;
	}

	err_entry.flags = rxe->cq_entry.flags;
	if (rxe->state != EFA_RDM_RXE_UNEXP)
		err_entry.op_context = rxe->cq_entry.op_context;
	err_entry.buf = rxe->cq_entry.buf;
	err_entry.data = rxe->cq_entry.data;
	err_entry.tag = rxe->cq_entry.tag;
	if (OFI_UNLIKELY(efa_rdm_write_error_msg(ep, rxe->peer, prov_errno,
	                                         err_msg, &err_entry.err_data_size))) {
		err_entry.err_data_size = 0;
	} else {
		err_entry.err_data = err_msg;
	}

	EFA_WARN(FI_LOG_CQ, "err: %d, message: %s (%d)\n",
		err_entry.err,
		err_entry.err_data
			? (const char *) err_entry.err_data
			: efa_strerror(err_entry.prov_errno),
		 err_entry.prov_errno);
	efa_show_help(err_entry.prov_errno);
	/*
	 * TODO: We can't free the rxe as we may receive additional
	 * packets for this entry. Add ref counting so the rxe can safely
	 * be freed once all packets are accounted for.
	 */
	//efa_rdm_rxe_release(rxe);

	if (rxe->internal_flags & EFA_RDM_OPE_INTERNAL) {
		EFA_WARN(FI_LOG_CQ,
			"Writing eq error for rxe from internal operations\n");
		efa_base_ep_write_eq_error(&ep->base_ep, err, prov_errno);
		return;
	}

	efa_cntr_report_error(&ep->base_ep.util_ep, err_entry.flags);
	write_cq_err = ofi_cq_write_error(util_cq, &err_entry);
	if (write_cq_err) {
		EFA_WARN(FI_LOG_CQ,
			"Error writing error cq entry when handling RX error\n");
		efa_base_ep_write_eq_error(&ep->base_ep, err, prov_errno);
	}
}

/**
 * @brief handle the situation that a TX operation encountered error
 *
 * This function does the follow to handle error:
 *
 * 1. write an error cq entry for the TX operation, if writing
 *    CQ error entry failed, it will write eq entry.
 *
 * 2. increase error counter.
 *
 * 3. remove the txe from ep->tx_queued_list and ep->tx_pending_list
 *    if the txe is on it.
 *
 * 4. print warning message with self and peer's raw address
 *
 * Note It does NOT release txe because it is still possible to receive
 * send completion for this txe
 *
 * @param[in]	txe	txe that encountered error
 * @param[in]	err			positive libfabric error code
 * @param[in]	prov_errno	positive EFA provider specific error code
 */
void efa_rdm_txe_handle_error(struct efa_rdm_ope *txe, int err, int prov_errno)
{
	struct efa_rdm_ep *ep;
	struct fi_cq_err_entry err_entry;
	struct util_cq *util_cq;
	struct dlist_entry *tmp;
	struct efa_rdm_pke *pkt_entry;
	int write_cq_err;
	char err_msg[EFA_ERROR_MSG_BUFFER_LENGTH] = {0};

	ep = txe->ep;
	memset(&err_entry, 0, sizeof(err_entry));

	util_cq = ep->base_ep.util_ep.tx_cq;

	err_entry.err = err;
	err_entry.prov_errno = prov_errno;

	switch (txe->state) {
	case EFA_RDM_TXE_REQ:
		break;
	case EFA_RDM_OPE_SEND:
		dlist_remove(&txe->entry);
		break;
	case EFA_RDM_OPE_ERR:
		/* Already progressed, no-op */
		return;
	default:
		EFA_WARN(FI_LOG_CQ, "txe unknown state %d\n",
			txe->state);
		assert(0 && "txe unknown state");
	}

	txe->state = EFA_RDM_OPE_ERR;

	if (txe->internal_flags & EFA_RDM_OPE_QUEUED_FLAGS) {
		dlist_remove(&txe->queued_entry);
		txe->internal_flags &= ~EFA_RDM_OPE_QUEUED_FLAGS;
	}

	dlist_foreach_container_safe(&txe->queued_pkts,
				     struct efa_rdm_pke,
				     pkt_entry, entry, tmp)
		efa_rdm_pke_release_tx(pkt_entry);

	err_entry.flags = txe->cq_entry.flags;
	err_entry.op_context = txe->cq_entry.op_context;
	err_entry.buf = txe->cq_entry.buf;
	err_entry.data = txe->cq_entry.data;
	err_entry.tag = txe->cq_entry.tag;
	if (OFI_UNLIKELY(efa_rdm_write_error_msg(ep, txe->peer, prov_errno,
	                                         err_msg, &err_entry.err_data_size))) {
		err_entry.err_data_size = 0;
	} else {
		err_entry.err_data = err_msg;
	}

	EFA_WARN(FI_LOG_CQ, "err: %d, message: %s (%d)\n",
		err_entry.err,
		err_entry.err_data
			? (const char *) err_entry.err_data
			: efa_strerror(err_entry.prov_errno),
		err_entry.prov_errno);

	efa_show_help(err_entry.prov_errno);

	/*
	 * TODO: We can't free the txe as we may receive a control packet
	 * for this entry. Add ref counting so the txe can safely
	 * be freed once all packets are accounted for.
	 */
	//efa_rdm_txe_release(txe);

	if (txe->internal_flags & EFA_RDM_OPE_INTERNAL) {
		EFA_WARN(FI_LOG_CQ,
			"Writing eq error for txe from internal operations\n");
		efa_base_ep_write_eq_error(&ep->base_ep, err, prov_errno);
		return;
	}

	efa_cntr_report_error(&ep->base_ep.util_ep, txe->cq_entry.flags);
	write_cq_err = ofi_cq_write_error(util_cq, &err_entry);
	if (write_cq_err) {
		EFA_WARN(FI_LOG_CQ,
			"Error writing error cq entry when handling TX error\n");
		efa_base_ep_write_eq_error(&ep->base_ep, err, prov_errno);
	}
}

/**
 * @brief report (to user) that an RX operation has completed
 *
 * An RX operation can be recevie, write response, read response
 * and atomic response.
 * This function will do the following:
 *
 * 1. write an CQ entry. If the message is truncated, (receiving
 *    buffer is too small to hold incming message), the CQ entry
 *    will be an error CQ entry, otherwise the CQ entry will be
 *    normal CQ entry. User will get the CQ entry will it call
 *    fi_cq_read()
 *
 * 2. increase counter. User will get the updated counter when
 *    it call fi_cntr_read()
 *
 * @param[in]		rxe	information of the completed RX operation
 */
void efa_rdm_rxe_report_completion(struct efa_rdm_ope *rxe)
{
	struct efa_rdm_ep *ep = rxe->ep;
	struct util_cq *rx_cq = ep->base_ep.util_ep.rx_cq;
	int ret = 0;
	uint64_t cq_flags;

	cq_flags = (ep->base_ep.util_ep.rx_msg_flags == FI_COMPLETION) ? 0 : FI_SELECTIVE_COMPLETION;
	if (OFI_UNLIKELY(rxe->cq_entry.len < rxe->total_len)) {
		EFA_WARN(FI_LOG_CQ,
			 "Message truncated! from peer %" PRIu64
			 " implicit fi_addr: %" PRIu64 " rx_id: %" PRIu32 " msg_id: %" PRIu32 " tag: %" PRIu64
			 " incoming message size: %" PRIu64
			 " receiving buffer size: %zu\n",
			 rxe->peer->conn->fi_addr, rxe->peer->conn->implicit_fi_addr, rxe->rx_id, rxe->msg_id, rxe->cq_entry.tag,
			 rxe->total_len, rxe->cq_entry.len);

		ret = ofi_cq_write_error_trunc(ep->base_ep.util_ep.rx_cq,
					       rxe->cq_entry.op_context,
					       rxe->cq_entry.flags,
					       rxe->total_len,
					       rxe->cq_entry.buf,
					       rxe->cq_entry.data,
					       rxe->cq_entry.tag,
					       rxe->total_len -
					       rxe->cq_entry.len);

		if (OFI_UNLIKELY(ret)) {
			EFA_WARN(FI_LOG_CQ,
				"Unable to write recv error cq: %s\n",
				fi_strerror(-ret));
			return;
		}

		rxe->fi_flags |= EFA_RDM_TXE_NO_COMPLETION;
		efa_cntr_report_error(&ep->base_ep.util_ep, rxe->cq_entry.flags);
		return;
	}

	if (!(rxe->internal_flags & EFA_RDM_RXE_RECV_CANCEL) &&
	    (ofi_need_completion(cq_flags, rxe->fi_flags) ||
	     (rxe->cq_entry.flags & FI_MULTI_RECV))) {
		EFA_DBG(FI_LOG_CQ,
			"Writing recv completion for rxe from peer: %" PRIu64
			" implicit fi_addr: %" PRIu64 " rx_id: %" PRIu32
			" msg_id: %" PRIu32 " tag: %lx total_len: %" PRIu64
			"\n",
			rxe->peer->conn->fi_addr,
			rxe->peer->conn->implicit_fi_addr, rxe->rx_id,
			rxe->msg_id, rxe->cq_entry.tag, rxe->total_len);

		efa_rdm_tracepoint(recv_end,
			    rxe->msg_id, (size_t) rxe->cq_entry.op_context,
			    rxe->total_len, rxe->cq_entry.tag, rxe->peer->conn->fi_addr);


		if (ep->base_ep.util_ep.caps & FI_SOURCE)
			ret = ofi_cq_write_src(rx_cq,
					       rxe->cq_entry.op_context,
					       rxe->cq_entry.flags,
					       rxe->cq_entry.len,
					       rxe->cq_entry.buf,
					       rxe->cq_entry.data,
					       rxe->cq_entry.tag,
					       rxe->peer->conn->fi_addr);
		else
			ret = ofi_cq_write(rx_cq,
					   rxe->cq_entry.op_context,
					   rxe->cq_entry.flags,
					   rxe->cq_entry.len,
					   rxe->cq_entry.buf,
					   rxe->cq_entry.data,
					   rxe->cq_entry.tag);

		if (OFI_UNLIKELY(ret)) {
			EFA_WARN(FI_LOG_CQ,
				"Unable to write recv completion: %s\n",
				fi_strerror(-ret));
			efa_rdm_rxe_handle_error(rxe, -ret, FI_EFA_ERR_WRITE_RECV_COMP);
			return;
		}

		rxe->fi_flags |= EFA_RDM_TXE_NO_COMPLETION;
	}

	efa_cntr_report_rx_completion(&ep->base_ep.util_ep, rxe->cq_entry.flags);
}

/**
 * @brief whether txe should write CQ entry when completed
 *
 * There are two situations that txe should not write CQ entry:
 *
 * 1. there are EFA_RDM_OPE_NO_COMPLETEION flag in txe->fi_flags, which
 *    is because this txe is for an emulated inject operation
 *
 * 2. user does not want CQ entry for this operation, this behavior
 *    is controled by the FI_SELECTIVE_COMPLETE flag when creating
 *    endpoint. However, this flag is override by the per message
 *    FI_COMPLETION flag.
 *
 * @param[in] 	txe 	information of the TX operation
 * @return 		a boolean
 */
static inline
bool efa_rdm_txe_should_update_cq(struct efa_rdm_ope *txe)

{
	if (txe->fi_flags & EFA_RDM_TXE_NO_COMPLETION)
		return false;

	/*
	 * ep->util_ep.tx_msg_flags is either 0 or FI_COMPLETION, depend on
	 * whether app specfied FI_SELECTIVE_COMPLETION when binding CQ.
	 * (ep->util_ep.tx_msg_flags was set in ofi_ep_bind_cq())
	 *
	 * If tx_msg_flags is 0, we only write completion when app specify
	 * FI_COMPLETION in flags.
	 */
	return txe->ep->base_ep.util_ep.tx_msg_flags == FI_COMPLETION ||
	       txe->fi_flags & FI_COMPLETION;
}

/**
 * @brief report (to user) that a TX operation has completed
 *
 * This function does the following to report completion:
 *
 * 1. write a cq entry for the TX operation when necessary
 *    Sometimes it is not necessary to to write CQ entry
 *    See #efa_rdm_ope_should_update_cq
 *
 * 2. update counter if necessary.
 *
 *
 * @param[in]	txe	information of the completed TX operation
 */
void efa_rdm_txe_report_completion(struct efa_rdm_ope *txe)
{
	struct util_cq *tx_cq = txe->ep->base_ep.util_ep.tx_cq;
	int ret;

	assert(txe->type == EFA_RDM_TXE);
	if (efa_rdm_txe_should_update_cq(txe)) {
		EFA_DBG(FI_LOG_CQ,
		       "Writing send completion for txe to peer: %" PRIu64
		       " tx_id: %" PRIu32 " msg_id: %" PRIu32 " tag: %lx len: %"
		       PRIu64 "\n",
		       txe->peer->conn->fi_addr, txe->tx_id, txe->msg_id,
		       txe->cq_entry.tag, txe->total_len);


	efa_rdm_tracepoint(send_end,
		    txe->msg_id, (size_t) txe->cq_entry.op_context,
		    txe->total_len, txe->cq_entry.tag, txe->peer->conn->fi_addr);

		/* TX completions should not send peer address to util_cq */
		if (txe->ep->base_ep.util_ep.caps & FI_SOURCE)
			ret = ofi_cq_write_src(tx_cq,
					       txe->cq_entry.op_context,
					       txe->cq_entry.flags,
					       txe->cq_entry.len,
					       txe->cq_entry.buf,
					       txe->cq_entry.data,
					       txe->cq_entry.tag,
					       FI_ADDR_NOTAVAIL);
		else
			ret = ofi_cq_write(tx_cq,
					   txe->cq_entry.op_context,
					   txe->cq_entry.flags,
					   txe->cq_entry.len,
					   txe->cq_entry.buf,
					   txe->cq_entry.data,
					   txe->cq_entry.tag);

		if (OFI_UNLIKELY(ret)) {
			EFA_WARN(FI_LOG_CQ,
				"Unable to write send completion: %s\n",
				fi_strerror(-ret));
			efa_rdm_txe_handle_error(txe, -ret, FI_EFA_ERR_WRITE_SEND_COMP);
			return;
		}
	}

	efa_cntr_report_tx_completion(&txe->ep->base_ep.util_ep, txe->cq_entry.flags);
	txe->fi_flags |= EFA_RDM_TXE_NO_COMPLETION;
	return;
}

/**
 * @brief handle the event that an ope has "sent all the data".
 *
 * Note that both txe and rxe can send data:
 *
 * txe will send data when operation type is send/write. In this case, the
 * exact timing of "all the data has been sent" event is:
 *
 * 	If the txe requested delivery complete, "all the data has been sent"
 *      happens when txe received a RECEIPT packet from receiver/write responder
 *
 * 	If the txe requested transmit complete, "all the data has been sent"
 *      happens when the send completion of all packets that contains data has been
 *      received.
 *
 * rxe will send data when operation type is read and emulated read is used.
 * In this case, "all data has been sent" happens when txe received the completion
 * of all the packets that contains data.
 *
 * In both cases, the "all data has been send" event mark the end of the operation,
 * therefore this function will call efa_rdm_txe/ope_report_completion(), and
 * release the ope
 *
 * @param[in]	ope	inforatminon of op entry that sends data
 */
void efa_rdm_ope_handle_send_completed(struct efa_rdm_ope *ope)
{
	struct efa_rdm_ep *ep;
	struct efa_rdm_ope *rxe;

	ep = ope->ep;

	if (ope->cq_entry.flags & FI_READ) {
		/*
		 * This is on responder side of an emulated read operation.
		 * In this case, we do not write any completion.
		 * The entry is allocated for emulated read, so no need to write tx completion.
		 * EFA does not support FI_RMA_EVENT, so no need to write rx completion.
		 */
		assert(ope->type == EFA_RDM_RXE);
		rxe = ope;
		efa_rdm_rxe_release(rxe);
		return;
	} else if (ope->cq_entry.flags & FI_WRITE) {
		if (ope->fi_flags & FI_COMPLETION) {
			efa_rdm_txe_report_completion(ope);
		} else {
			if (!(ope->fi_flags & EFA_RDM_TXE_NO_COUNTER))
				efa_cntr_report_tx_completion(&ep->base_ep.util_ep, ope->cq_entry.flags);
		}

	} else {
		assert(ope->cq_entry.flags & FI_SEND);
		efa_rdm_txe_report_completion(ope);
	}

	assert(ope->type == EFA_RDM_TXE);
	efa_rdm_txe_release(ope);
}

/**
 * @brief handle the event "all data has been received" for an ope
 *
 * Both txe and rxe can receive data.
 *
 * txe can receive data when the operation is read and emulated
 * read is used.
 *
 * rxe can receiver data when the operation is receive or write response.
 *
 * To complete a RX operation, this function does 3 things:
 *
 * 1. If necessary, write completion to application. (Not all
 *    completed RX action will cause a completion to be written).
 *
 * 2. If delievery complete is requested send a receipt packet back to the peer.
 *
 * 3. Release the ope unless the ope is rxe, and an RECEIPT/EOR
 *    packet has been sent. This is because rxe is needed to handle the
 *    send completion event of RECEIPT/EOR
 *
 * @param[in,out]	ope	ope that contains information of a data receive operation
 */
void efa_rdm_ope_handle_recv_completed(struct efa_rdm_ope *ope)
{
	struct efa_rdm_ope *txe = NULL;
	struct efa_rdm_ope *rxe = NULL;
	int err;

	/* It is important to write completion before sending ctrl packet, because the
	 * action of sending ctrl packet may cause the release of rxe (when inject
	 * was used on lower device).
	 */
	if (ope->cq_entry.flags & FI_REMOTE_WRITE) {
		/*
		 * For write, only write RX completion when REMOTE_CQ_DATA is on
		 */
		if (ope->cq_entry.flags & FI_REMOTE_CQ_DATA)
			efa_rdm_rxe_report_completion(ope);
	} else if (ope->cq_entry.flags & FI_READ) {
		/* This ope is part of the for emulated read protocol,
		 * created on the read requester side.
		 * The following shows the sequence of events in an emulated
		 * read protocol.
		 *
		 * Requester                      Responder
		 * create txe
		 * send rtr
		 *                                receive rtr
		 *                                create rxe
		 *                                rxe sending data
		 * txe receiving data
		 * receive completed              send completed
		 * call this function             call #efa_rdm_ope_handle_send_completed()
		 *
		 * As can be seen, in the emulated read protocol, this function is called only
		 * on the requester side, so we need to find the corresponding txe and
		 * complete it.
		 */
		assert(ope->type == EFA_RDM_TXE);
		txe = ope; /* Intentionally assigned for easier understanding */

		assert(txe->state == EFA_RDM_TXE_REQ);
		if (txe->fi_flags & FI_COMPLETION) {
			efa_rdm_txe_report_completion(txe);
		} else {
			efa_cntr_report_tx_completion(&txe->ep->base_ep.util_ep, txe->cq_entry.flags);
		}
	} else {
		assert(ope->type == EFA_RDM_RXE);
		rxe = ope; /* Intentionally assigned for easier understanding */

		assert(rxe->op == ofi_op_msg || rxe->op == ofi_op_tagged);

		efa_rdm_rxe_report_completion(rxe);
	}

	/* As can be seen, this function does not release rxe when
	 * efa_rdm_ope_post_send_or_queue() was successful.
	 *
	 * This is because that efa_rdm_ope_post_send_or_queue() might have
	 * queued the ctrl packet (due to out of resource), and progress
	 * engine will resend the packet. In that case, progress engine
	 * needs the rxe to construct the ctrl packet.
	 *
	 * Hence, the rxe can be safely released only when we got
	 * the send completion of the ctrl packet.
	 */
	if (ope->internal_flags & EFA_RDM_TXE_DELIVERY_COMPLETE_REQUESTED) {
		assert(ope->type == EFA_RDM_RXE);
		rxe = ope; /* Intentionally assigned for easier understanding */
		err = efa_rdm_ope_post_send_or_queue(rxe, EFA_RDM_RECEIPT_PKT);
		if (OFI_UNLIKELY(err)) {
			EFA_WARN(FI_LOG_CQ,
				 "Posting of ctrl packet failed when complete rx! err=%s(%d)\n",
				 fi_strerror(-err), -err);
			efa_rdm_rxe_handle_error(rxe, -err, FI_EFA_ERR_PKT_POST);
			efa_rdm_rxe_release(rxe);
		}

		return;
	}

	/* An EOR (End Of Read) packet is sent when RDMA read operation has completed
	 * for read data. EOR was sent before "all data has been received". Therefore
	 * it is possible that when this function is called, EOR is still inflight
	 * (EOR has been sent, and the send completion has NOT been received).
	 *
	 * If EOR is inflight, the rxe cannot be released because the rxe
	 * is needed to handle the send completion of the EOR.
	 *
	 * see #efa_rdm_pke_handle_eor_send_completion
	 */
	if (ope->internal_flags & EFA_RDM_RXE_EOR_IN_FLIGHT) {
		return;
	}

	if (ope->type == EFA_RDM_TXE) {
		efa_rdm_txe_release(ope);
	} else {
		assert(ope->type == EFA_RDM_RXE);
		efa_rdm_rxe_release(ope);
	}
}

/**
 * @brief prepare an ope such that it is ready to post read request(s)
 *
 * An ope can post read request(s) for two reasons:
 *
 * First, it can be because user directly initiated a read requst
 * (by calling fi_readxxx() API). In this case the ope argument
 * will be a txe (ope->type == EFA_RDM_TXE)
 *
 * Second, it can be part of a read-base message protocol, such as
 * the longread message protocol. In this case, the ope argument
 * will be a rxe (ope->type == EFA_RDM_RXE)
 *
 * @param[in,out]		ep		endpoint
 * @param[in,out]		ope	information of the operation the needs to post a read
 * @return		0 if the read request is posted successfully.
 * 			negative libfabric error code on failure.
 */
int efa_rdm_ope_prepare_to_post_read(struct efa_rdm_ope *ope)
{
	int err;
	size_t total_iov_len, total_rma_iov_len;

	if (ope->type == EFA_RDM_RXE) {
		/* Often times, application will provide a receiving buffer that is larger
		 * then the incoming message size. For read based message transfer, the
		 * receiving buffer need to be registered. Thus truncating rxe->iov to
		 * extact message size to save memory registration pages.
		 */
		err = ofi_truncate_iov(ope->iov, &ope->iov_count,
				       ope->total_len + ope->ep->msg_prefix_size);
		if (err) {
			EFA_WARN(FI_LOG_CQ,
				 "ofi_truncated_iov failed. new_size: %ld\n",
				 ope->total_len + ope->ep->msg_prefix_size);
			return err;
		}
	}

	total_iov_len = ofi_total_iov_len(ope->iov, ope->iov_count);
	total_rma_iov_len = ofi_total_rma_iov_len(ope->rma_iov, ope->rma_iov_count);

	if (ope->type == EFA_RDM_TXE)
		ope->bytes_read_offset = 0;
	else
		ope->bytes_read_offset = ope->bytes_runt;

	ope->bytes_read_total_len = MIN(total_iov_len, total_rma_iov_len) - ope->bytes_read_offset;
	ope->bytes_read_submitted = 0;
	ope->bytes_read_completed = 0;
	return 0;
}

/**
 * @brief Prepare the local read pkt entry used by txe.
 *
 * This function is applied on txe->local_read_packet_entry.
 * In the following 2 situations, this function will clone
 * a new pkt entry from readcopy packet pool, release the
 * original packet and update the txe accordingly.
 *
 * 1. If the memory of the packet entry is not registered (this can happen when the
 * packet is from OOO or unexp packet pool), we need to clone this pkt entry to one
 * from read copy pool which is registered.
 * 2. If application requests sendrecv_in_order_aligned_128_bytes, which requires
 * the staging data in txe->rma_iov to be 128 byte aligned. To achive that, we need to
 * clone this pkt entry to one from read-copy pool, which is 128 byte aligned, and stores
 * application data at the beginning of pkt_entry->wiredata, which is also 128 byte aligned.
 *
 * Return value:
 *
 *     On success, return 0
 *     On pack entry allocation failure, return -FI_EAGAIN
 */
ssize_t efa_rdm_txe_prepare_local_read_pkt_entry(struct efa_rdm_ope *txe)
{
	struct efa_rdm_pke *pkt_entry;
	struct efa_rdm_pke *pkt_entry_copy;

	assert(txe->type == EFA_RDM_TXE);
	assert(txe->rma_iov_count > 0 && txe->rma_iov_count <= efa_rdm_ep_domain(txe->ep)->info->tx_attr->rma_iov_limit);

	pkt_entry = txe->local_read_pkt_entry;
	if (pkt_entry->mr && !(txe->ep->sendrecv_in_order_aligned_128_bytes))
		return 0;

	assert(pkt_entry->alloc_type == EFA_RDM_PKE_FROM_OOO_POOL   ||
	       pkt_entry->alloc_type == EFA_RDM_PKE_FROM_UNEXP_POOL ||
	       pkt_entry->alloc_type == EFA_RDM_PKE_FROM_EFA_RX_POOL);

	pkt_entry_copy = efa_rdm_pke_clone(pkt_entry,
					   txe->ep->rx_readcopy_pkt_pool,
					   EFA_RDM_PKE_FROM_READ_COPY_POOL);
	if (!pkt_entry_copy) {
		EFA_WARN(FI_LOG_CQ,
			"readcopy pkt pool exhausted! Set FI_EFA_READCOPY_POOL_SIZE to a higher value!");
		return -FI_EAGAIN;
	}

#if ENABLE_DEBUG
	/* readcopy pkt is also rx pkt, insert it to rx pkt list so we can track it and clean up during ep close */
	dlist_insert_tail(&pkt_entry_copy->dbg_entry, &pkt_entry_copy->ep->rx_pkt_list);
#endif

	efa_rdm_pke_release_rx(pkt_entry);

	assert(pkt_entry_copy->mr);
	txe->local_read_pkt_entry = pkt_entry_copy;
	/* pkt from read-copy pool only stores actual application data in wiredata */
	assert(ofi_is_addr_aligned((void *)pkt_entry_copy->wiredata, EFA_RDM_IN_ORDER_ALIGNMENT));
	txe->rma_iov[0].addr = (uint64_t)pkt_entry_copy->wiredata;
	txe->rma_iov[0].key = fi_mr_key(pkt_entry_copy->mr);
	return 0;
}

/**
 * @brief prepare an ope such that it is ready to post write request(s)
 *
 * An ope will post write request(s) after a user directly initiated a
 * write request (by calling fi_writexxx() API).  The ope->type will be
 * EFA_RDM_TXE
 *
 * @param[in,out]	ep		endpoint
 * @param[in,out]	ope	information the operation needs to post a write
 */
void efa_rdm_ope_prepare_to_post_write(struct efa_rdm_ope *ope)
{
	size_t local_iov_len;

	assert(ope->type == EFA_RDM_TXE);

	local_iov_len = ofi_total_iov_len(ope->iov, ope->iov_count);
#ifndef NDEBUG
	{
		size_t remote_iov_len;
		remote_iov_len = ofi_total_rma_iov_len(ope->rma_iov, ope->rma_iov_count);
		assert( local_iov_len == remote_iov_len );
	}
#endif

	ope->bytes_write_total_len = local_iov_len;
	ope->bytes_write_submitted = 0;
	ope->bytes_write_completed = 0;
}

/**
 * @brief post read request(s)
 *
 * This function posts read request(s) according to information in ope.
 * Depend on ope->bytes_read_total_len and max read size of device. This function
 * might issue multiple read requdsts.
 *
 * @param[in,out]	ope	ope that has information of the read request.
 * 					If read request is successfully submitted,
 * 					ope->bytes_read_submitted will be updated.
 * @return	On success, return 0
 * 		On failure, return a negative error code.
 */
int efa_rdm_ope_post_read(struct efa_rdm_ope *ope)
{
	int err;
	int iov_idx = 0, rma_iov_idx = 0;
	size_t iov_offset = 0, rma_iov_offset = 0;
	size_t read_once_len, max_read_once_len;
	struct efa_rdm_ep *ep;
	struct efa_rdm_pke *pkt_entry;

	ep = ope->ep;

	assert(ope->iov_count > 0 && ope->iov_count <= efa_rdm_ep_domain(ep)->info->tx_attr->iov_limit);
	assert(ope->rma_iov_count > 0 && ope->rma_iov_count <= efa_rdm_ep_domain(ep)->info->tx_attr->rma_iov_limit);

	if (ope->bytes_read_total_len == 0) {

		/* According to libfabric document
		 *     https://ofiwg.github.io/libfabric/main/man/fi_rma.3.html
		 * read with 0 byte is allowed.
		 *
		 * Note that because send operation used a pkt_entry as wr_id,
		 * we had to use a pkt_entry as context for read too.
		 */
		pkt_entry = efa_rdm_pke_alloc(ep, ep->efa_tx_pkt_pool, EFA_RDM_PKE_FROM_EFA_TX_POOL);

		if (OFI_UNLIKELY(!pkt_entry))
			return -FI_EAGAIN;

		efa_rdm_pke_init_read_context(pkt_entry, ope, ofi_buf_index(ope), 0);
		err = efa_rdm_pke_read(pkt_entry,
					 ope->iov[0].iov_base,
					 0,
					 ope->desc[0],
					 ope->rma_iov[0].addr,
					 ope->rma_iov[0].key);
		if (err)
			efa_rdm_pke_release_tx(pkt_entry);
		return err;
	}

	assert(ope->bytes_read_submitted < ope->bytes_read_total_len);

	if (ope->type == EFA_RDM_TXE &&
	    ope->op == ofi_op_read_req &&
	    ope->peer == NULL) {
		err = efa_rdm_txe_prepare_local_read_pkt_entry(ope);
		if (err)
			return err;
	}

	efa_rdm_ope_try_fill_desc(ope, 0, FI_RECV);

	max_read_once_len = MIN(efa_env.efa_read_segment_size, efa_rdm_ep_domain(ep)->device->max_rdma_size);
	assert(max_read_once_len > 0);

	err = ofi_iov_locate(ope->iov, ope->iov_count,
			     ope->bytes_read_offset + ope->bytes_read_submitted + ep->msg_prefix_size,
			     &iov_idx, &iov_offset);
	if (OFI_UNLIKELY(err)) {
		EFA_WARN(FI_LOG_CQ, "ofi_iov_locate failed! err: %d\n", err);
		return err;
	}

	err = ofi_rma_iov_locate(ope->rma_iov, ope->rma_iov_count,
				 ope->bytes_read_offset + ope->bytes_read_submitted,
				 &rma_iov_idx, &rma_iov_offset);
	if (err) {
		EFA_WARN(FI_LOG_CQ, "ofi_rma_iov_locate failed! err: %d\n", err);
		return err;
	}

	while (ope->bytes_read_submitted < ope->bytes_read_total_len) {

		assert(iov_idx < ope->iov_count);
		assert(iov_offset < ope->iov[iov_idx].iov_len);
		assert(rma_iov_idx < ope->rma_iov_count);
		assert(rma_iov_offset < ope->rma_iov[rma_iov_idx].len);

			if (ep->efa_outstanding_tx_ops == ep->efa_max_outstanding_tx_ops)
				return -FI_EAGAIN;

			if (!ope->desc[iov_idx]) {
				/* efa_rdm_ope_try_fill_desc() did not fill the desc,
				 * which means memory registration failed.
				 * return -FI_ENOMR here so that we fallback to emulated read.
				 */
				return -FI_ENOMR;
			}

		pkt_entry = efa_rdm_pke_alloc(ep, ep->efa_tx_pkt_pool, EFA_RDM_PKE_FROM_EFA_TX_POOL);

		if (OFI_UNLIKELY(!pkt_entry))
			return -FI_EAGAIN;

		read_once_len = MIN(ope->iov[iov_idx].iov_len - iov_offset,
				    ope->rma_iov[rma_iov_idx].len - rma_iov_offset);
		read_once_len = MIN(read_once_len, max_read_once_len);

		efa_rdm_pke_init_read_context(pkt_entry, ope, ofi_buf_index(ope), read_once_len);
		err = efa_rdm_pke_read(pkt_entry,
					 (char *)ope->iov[iov_idx].iov_base + iov_offset,
					 read_once_len,
					 ope->desc[iov_idx],
					 ope->rma_iov[rma_iov_idx].addr + rma_iov_offset,
					 ope->rma_iov[rma_iov_idx].key);
		if (err) {
			EFA_WARN(FI_LOG_CQ, "efa_rdm_pke_read failed! err: %d\n", err);
			efa_rdm_pke_release_tx(pkt_entry);
			return err;
		}

		ope->bytes_read_submitted += read_once_len;

		iov_offset += read_once_len;
		assert(iov_offset <= ope->iov[iov_idx].iov_len);
		if (iov_offset == ope->iov[iov_idx].iov_len) {
			iov_idx += 1;
			iov_offset = 0;
		}

		rma_iov_offset += read_once_len;
		assert(rma_iov_offset <= ope->rma_iov[rma_iov_idx].len);
		if (rma_iov_offset == ope->rma_iov[rma_iov_idx].len) {
			rma_iov_idx += 1;
			rma_iov_offset = 0;
		}
	}

	return 0;
}

/**
 * @brief post RDMA write request(s)
 *
 * This function posts write request(s) according to information in ope.
 * Depending on ope->bytes_write_total_len and max write size of device,
 * this function might issue multiple write requests.
 *
 * @param[in,out]	ope	ope that has information of the read request.
 * 					If write request is successfully submitted,
 * 					ope->bytes_write_submitted will be updated.
 * @return	On success, return 0
 * 		On failure, return a negative error code.
 */
int efa_rdm_ope_post_remote_write(struct efa_rdm_ope *ope)
{
	int err;
	int iov_idx = 0, rma_iov_idx = 0;
	ssize_t copied;
	size_t iov_offset = 0, rma_iov_offset = 0;
	size_t write_once_len, max_write_once_len;
	struct efa_rdm_ep *ep;
	struct efa_rdm_pke *pkt_entry;

	ep = ope->ep;

	assert(ope->iov_count > 0 && ope->iov_count <= efa_rdm_ep_domain(ep)->info->tx_attr->iov_limit);
	assert(ope->rma_iov_count > 0 && ope->rma_iov_count <= efa_rdm_ep_domain(ep)->info->tx_attr->rma_iov_limit);

	if (ope->bytes_write_total_len == 0) {
		/* According to libfabric document
		 *     https://ofiwg.github.io/libfabric/main/man/fi_rma.3.html
		 * write with 0 byte is allowed.
		 *
		 * Note that because send operation used a pkt_entry as wr_id,
		 * we had to use a pkt_entry as context for write too.
		 */
		pkt_entry = efa_rdm_pke_alloc(ep, ep->efa_tx_pkt_pool, EFA_RDM_PKE_FROM_EFA_TX_POOL);

		if (OFI_UNLIKELY(!pkt_entry))
			return -FI_EAGAIN;

		/* Provide the registered bounce buffer and its desc to rdma-core.
		 * The user provided buffer/desc will not be used for 0 byte writes.
		 * This allows the user to pass NULL for buff/desc.
		 */
		efa_rdm_pke_init_write_context(
			pkt_entry, ope, pkt_entry->wiredata, 0, fi_mr_desc(pkt_entry->mr),
			ope->rma_iov[0].addr, ope->rma_iov[0].key);
		err = efa_rdm_pke_write(pkt_entry);
		if (err)
			efa_rdm_pke_release_tx(pkt_entry);
		return err;
	}

	if (!(ope->fi_flags & FI_INJECT))
		efa_rdm_ope_try_fill_desc(ope, 0, FI_WRITE);

	assert(ope->bytes_write_submitted < ope->bytes_write_total_len);
	max_write_once_len = MIN(efa_env.efa_write_segment_size, efa_rdm_ep_domain(ep)->device->max_rdma_size);

	assert(max_write_once_len > 0);

	err = ofi_iov_locate(ope->iov, ope->iov_count,
				 ope->bytes_write_submitted,
				 &iov_idx, &iov_offset);
	if (OFI_UNLIKELY(err)) {
		EFA_WARN(FI_LOG_CQ, "ofi_iov_locate failed! err: %d\n", err);
		return err;
	}

	err = ofi_rma_iov_locate(ope->rma_iov, ope->rma_iov_count,
				 ope->bytes_write_submitted,
				 &rma_iov_idx, &rma_iov_offset);
	if (OFI_UNLIKELY(err)) {
		EFA_WARN(FI_LOG_CQ, "ofi_rma_iov_locate failed! err: %d\n", err);
		return err;
	}

	while (ope->bytes_write_submitted < ope->bytes_write_total_len) {

		assert(iov_idx < ope->iov_count);
		assert(iov_offset < ope->iov[iov_idx].iov_len);
		assert(rma_iov_idx < ope->rma_iov_count);
		assert(rma_iov_offset < ope->rma_iov[rma_iov_idx].len);

		if (ep->efa_outstanding_tx_ops == ep->efa_max_outstanding_tx_ops)
			return -FI_EAGAIN;

		if (!ope->desc[iov_idx] && !(ope->fi_flags & FI_INJECT)) {
			/* efa_rdm_ope_try_fill_desc() did not fill the desc,
			 * which means memory registration failed.
			 * return -FI_EAGAIN here will cause user to run progress
			 * engine, which will cause some memory registration
			 * in MR cache to be released.
			 */
			return -FI_EAGAIN;
		}
		pkt_entry = efa_rdm_pke_alloc(ep, ep->efa_tx_pkt_pool, EFA_RDM_PKE_FROM_EFA_TX_POOL);

		if (OFI_UNLIKELY(!pkt_entry))
			return -FI_EAGAIN;

		if (ope->fi_flags & FI_INJECT) {
			assert(ope->iov_count == 1);
			assert(ope->total_len <= ep->base_ep.inject_rma_size);
			copied = efa_rdm_pke_copy_from_hmem_iov(
				ope->desc[iov_idx], pkt_entry, ope,
				sizeof(struct efa_rdm_rma_context_pkt), 0,
				ope->total_len);
			assert(copied == ope->total_len);
			(void) copied; /* suppress compiler warning for non-debug build */
			ope->desc[0] = fi_mr_desc(pkt_entry->mr);
			ope->iov[0].iov_base = pkt_entry->wiredata + sizeof(struct efa_rdm_rma_context_pkt);
		}

		write_once_len = MIN(ope->iov[iov_idx].iov_len - iov_offset,
				    ope->rma_iov[rma_iov_idx].len - rma_iov_offset);
		write_once_len = MIN(write_once_len, max_write_once_len);

		efa_rdm_pke_init_write_context(
			pkt_entry, ope,
			(char *) ope->iov[iov_idx].iov_base + iov_offset,
			write_once_len, ope->desc[iov_idx],
			ope->rma_iov[rma_iov_idx].addr + rma_iov_offset,
			ope->rma_iov[rma_iov_idx].key);
		err = efa_rdm_pke_write(pkt_entry);
		if (err) {
			EFA_WARN(FI_LOG_CQ, "efa_rdm_pke_write failed! err: %d\n", err);
			efa_rdm_pke_release_tx(pkt_entry);
			return err;
		}

		ope->bytes_write_submitted += write_once_len;

		iov_offset += write_once_len;
		assert(iov_offset <= ope->iov[iov_idx].iov_len);
		if (iov_offset == ope->iov[iov_idx].iov_len) {
			iov_idx += 1;
			iov_offset = 0;
		}

		rma_iov_offset += write_once_len;
		assert(rma_iov_offset <= ope->rma_iov[rma_iov_idx].len);
		if (rma_iov_offset == ope->rma_iov[rma_iov_idx].len) {
			rma_iov_idx += 1;
			rma_iov_offset = 0;
		}
	}

	return 0;
}

int efa_rdm_ope_post_remote_read_or_queue(struct efa_rdm_ope *ope)
{
	int err;

	err = efa_rdm_ope_prepare_to_post_read(ope);
	if (err) {
		EFA_WARN(FI_LOG_CQ, "Prepare to post read failed. err=%d\n", err);
		return err;
	}

	err = efa_rdm_ope_post_read(ope);
	switch (err) {
	case -FI_EAGAIN:
		dlist_insert_tail(&ope->queued_entry,
				  &efa_rdm_ep_domain(ope->ep)->ope_queued_list);
		ope->internal_flags |= EFA_RDM_OPE_QUEUED_READ;
		err = 0;
		break;
	case -FI_ENOMR:
		/* We want to fallback to long CTS, so just return FI_ENOMR
		 * without printing warning
		 */
	case 0:
		break;
	default:
		EFA_WARN(FI_LOG_CQ, "RDMA post read failed. errno=%d.\n", err);
		break;
	}
	return err;
}

/**
 * @brief post a local read request, queue it if necessary
 *
 * a local read request is posted to copy data from a packet
 * entry to user posted receive buffer on device.
 *
 * @param[in]		rxe	which has the receive buffer information
 * @param[in]		rx_data_offset	offset of data in the receive buffer
 * @param[in]		pkt_entry	which has the data
 * @param[in]		pkt_data	pointer to the data in the packet entry
 * @param[in]		data_size	size of the data
 * @return		0 on success, negative error code on failure
 */
int efa_rdm_rxe_post_local_read_or_queue(struct efa_rdm_ope *rxe,
					  size_t rx_data_offset,
					  struct efa_rdm_pke *pkt_entry,
					  char *pkt_data, size_t data_size)
{
	int err;
	struct iovec iov[EFA_RDM_IOV_LIMIT];
	void *desc[EFA_RDM_IOV_LIMIT];
	size_t iov_count;
	struct fi_rma_iov rma_iov;
	struct fi_msg_rma msg_rma;
	struct efa_rdm_ope *txe;

	efa_rdm_tracepoint(rx_pke_local_read_copy_payload_begin, (size_t) pkt_entry, pkt_entry->payload_size, rxe->msg_id, (size_t) rxe->cq_entry.op_context, rxe->total_len);
	/* setup rma_iov, which is pointing to buffer in the packet entry */
	rma_iov.addr = (uint64_t)pkt_data;
	rma_iov.len = data_size;
	/* if pkt_entry memory is not registered, efa_rdm_ope_prepare_local_read_pkt_entry_mr()
	 * will clone a packet entry from read_copy pkt pool, whose memory is registered.
	 * The clone can fail due to temporary out of resource, therefore cannot be called
	 * from inside efa_rdm_ope_post_read().
	 */
	rma_iov.key = (pkt_entry->mr) ? fi_mr_key(pkt_entry->mr) : 0;

	/* setup iov */
	assert(pkt_entry->ope == rxe);
	assert(rxe->desc && efa_mr_is_hmem(rxe->desc[0]));
	iov_count = rxe->iov_count;
	memcpy(iov, rxe->iov, rxe->iov_count * sizeof(struct iovec));
	memcpy(desc, rxe->desc, rxe->iov_count * sizeof(void *));
	ofi_consume_iov_desc(iov, desc, &iov_count, rx_data_offset);
	if (iov_count == 0) {
		EFA_WARN(FI_LOG_CQ,
			"data_offset %ld out of range\n",
			rx_data_offset);
		return -FI_ETRUNC;
	}

	assert(efa_mr_is_hmem(rxe->desc[0]));
	err = ofi_truncate_iov(iov, &iov_count, data_size);
	if (err) {
		EFA_WARN(FI_LOG_CQ,
			"data_offset %ld data_size %ld out of range\n",
			rx_data_offset, data_size);
		return -FI_ETRUNC;
	}

	msg_rma.msg_iov = iov;
	msg_rma.desc = desc;
	msg_rma.iov_count = iov_count;
	msg_rma.addr = FI_ADDR_NOTAVAIL;
	msg_rma.rma_iov = &rma_iov;
	msg_rma.rma_iov_count = 1;

	txe = efa_rdm_rma_alloc_txe(rxe->ep, NULL, &msg_rma, ofi_op_read_req,
				    0 /* flags*/);
	if (!txe) {
		return -FI_ENOBUFS;
	}

	txe->local_read_pkt_entry = pkt_entry;
	txe->internal_flags |= EFA_RDM_OPE_INTERNAL;
	err = efa_rdm_ope_post_remote_read_or_queue(txe);
	/* The rx pkts are held until the local read completes */
	if (err)
		efa_rdm_txe_release(txe);
	else if (txe->local_read_pkt_entry->alloc_type == EFA_RDM_PKE_FROM_EFA_RX_POOL)
		txe->ep->efa_rx_pkts_held++;

	return err;
}

/**
 * @brief post packet(s) according to packet type.
 *
 * Depend on packet type, this function may post one packet or multiple packets.
 *
 * @param[in]   ope            pointer to efa_rdm_ope. (either a txe or an rxe)
 * @param[in]   pkt_type        packet type.
 * @return      On success return 0, otherwise return a negative libfabric error code. Possible error codes include:
 *             -FI_EAGAIN      temporarily  out of resource
 */
ssize_t efa_rdm_ope_post_send(struct efa_rdm_ope *ope, int pkt_type)
{
	struct efa_rdm_ep *ep;
	struct efa_rdm_pke *pkt_entry_vec[EFA_RDM_EP_MAX_WR_PER_IBV_POST_SEND];
	ssize_t err;
	size_t segment_offset;
	int pkt_entry_cnt, pkt_entry_cnt_allocated = 0, pkt_entry_data_size_vec[EFA_RDM_EP_MAX_WR_PER_IBV_POST_SEND];
	int i;
	uint64_t flags = 0;

	err = efa_rdm_ope_prepare_to_post_send(ope, pkt_type, &pkt_entry_cnt, pkt_entry_data_size_vec);
	if (err)
		return err;
	assert(pkt_entry_cnt <= EFA_RDM_EP_MAX_WR_PER_IBV_POST_SEND);

	ep = ope->ep;
	assert(ep);
	segment_offset = efa_rdm_pkt_type_contains_data(pkt_type) ? ope->bytes_sent : -1;
	for (i = 0; i < pkt_entry_cnt; ++i) {
		pkt_entry_vec[i] = efa_rdm_pke_alloc(ep, ep->efa_tx_pkt_pool, EFA_RDM_PKE_FROM_EFA_TX_POOL);

		if (OFI_UNLIKELY(!pkt_entry_vec[i])) {
			err = -FI_EAGAIN;
			goto handle_err;
		}

		pkt_entry_cnt_allocated++;

		err = efa_rdm_pke_fill_data(pkt_entry_vec[i],
					    pkt_type,
					    ope,
					    segment_offset,
					    pkt_entry_data_size_vec[i]);
		if (err)
			goto handle_err;

		if (segment_offset != -1 && pkt_entry_cnt > 1) {
			assert(pkt_entry_data_size_vec[i] > 0);
			segment_offset += pkt_entry_data_size_vec[i];
		}
	}

	assert(pkt_entry_cnt == pkt_entry_cnt_allocated);

	/**
	 * We currently respect FI_MORE only for eager pkt type because
	 * 1. For some non-REQ pkts like CTSDATA, its current implementation
	 * relies on the logic that efa_rdm_ope_post_send always rings the doorbell,
	 * because the ep progress call will keep calling this function until
	 * ope->window is 0, but ope->window will only be decremented after
	 * the CTSDATA pkts are actually posted to rdma-core.
	 * 2. For non-eager REQ packets, we already send multiple pkts that contain
	 * data and make the firmware saturated, there is no meaning to queue
	 * pkts in this case.
	 */
	if (ope->fi_flags & FI_MORE && efa_rdm_pkt_type_is_eager(pkt_type))
		flags |= FI_MORE;

	err = efa_rdm_pke_sendv(pkt_entry_vec, pkt_entry_cnt, flags);
	if (err)
		goto handle_err;

	ope->peer->flags |= EFA_RDM_PEER_REQ_SENT;
	for (i = 0; i < pkt_entry_cnt; ++i)
		efa_rdm_pke_handle_sent(pkt_entry_vec[i], pkt_type, ope->peer);

	return FI_SUCCESS;

handle_err:
	for (i = 0; i < pkt_entry_cnt_allocated; ++i)
		efa_rdm_pke_release_tx(pkt_entry_vec[i]);

	return efa_rdm_ope_post_send_fallback(ope, pkt_type, err);
}

/**
 * @brief Fallback to a different message type if a packet send fails.
 *
 * Currently, this function is only used in the read nack protocol. If a long read or
 * runting read RTM packet fails to send because of a memory registration failure, it
 * will send a long CTS RTM packet.
 *
 * @param[in]   ope            pointer to efa_rdm_ope. (either a txe or an rxe)
 * @param[in]   pkt_type       packet type that failed to send
 * @param[in]   err		       error code of the original failure
 * @return      On success return 0, otherwise return a negative libfabric error code. Possible error codes include:
 *             -FI_EAGAIN      temporarily  out of resource
 */
ssize_t efa_rdm_ope_post_send_fallback(struct efa_rdm_ope *ope,
					   int pkt_type, ssize_t err)
{
	bool delivery_complete_requested = ope->fi_flags & FI_DELIVERY_COMPLETE;

	if (err == -FI_ENOMR) {
		/* Long read and runting read protocols could fail because of a
		 * lack of memory registrations. In that case, we retry with
		 * long CTS protocol
		 */
		switch (pkt_type) {
		case EFA_RDM_LONGREAD_MSGRTM_PKT:
		case EFA_RDM_RUNTREAD_MSGRTM_PKT:
			EFA_INFO(FI_LOG_EP_CTRL,
				 "Sender fallback to long CTS untagged "
				 "protocol because memory registration limit "
				 "was reached on the sender\n");
			return efa_rdm_ope_post_send_or_queue(
				ope, delivery_complete_requested ?  EFA_RDM_DC_LONGCTS_MSGRTM_PKT : EFA_RDM_LONGCTS_MSGRTM_PKT);
		case EFA_RDM_LONGREAD_TAGRTM_PKT:
		case EFA_RDM_RUNTREAD_TAGRTM_PKT:
			EFA_INFO(FI_LOG_EP_CTRL,
				 "Sender fallback to long CTS tagged protocol "
				 "because memory registration limit was "
				 "reached on the sender\n");
			return efa_rdm_ope_post_send_or_queue(
				ope, delivery_complete_requested ?  EFA_RDM_DC_LONGCTS_TAGRTM_PKT : EFA_RDM_LONGCTS_TAGRTM_PKT);
		default:
			return err;
		}
	}
	return err;
}

/**
 * @brief post packet(s) according to packet type. Queue the post if -FI_EAGAIN is encountered.
 *
 * This function will call efa_rdm_ope_post_send() to post packet(s) according to packet type.
 * If efa_rdm_ope_post_send() returned -FI_EAGAIN, this function will put the txe in efa_domain's
 * queued_list. The progress engine will try to post the packet later.
 *
 * This function is mainly used by packet handler to post responsive ctrl packet (such as EOR and CTS).
 *
 * @param[in]   ope             pointer to efa_rdm_ope. (either a txe or an rxe)
 * @param[in]   pkt_type        packet type.
 * @return      On success return 0, otherwise return a negative libfabric error code.
 */
ssize_t efa_rdm_ope_post_send_or_queue(struct efa_rdm_ope *ope, int pkt_type)
{
	ssize_t err;

	err = efa_rdm_ope_post_send(ope, pkt_type);
	if (err == -FI_EAGAIN) {
		assert(!(ope->internal_flags & EFA_RDM_OPE_QUEUED_RNR));
		ope->internal_flags |= EFA_RDM_OPE_QUEUED_CTRL;
		ope->queued_ctrl_type = pkt_type;
		dlist_insert_tail(&ope->queued_entry,
				  &efa_rdm_ep_domain(ope->ep)->ope_queued_list);
		err = 0;
	}

	return err;
}

/**
 * @brief Repost the ope that was queued before a handshake is made with peer
 *
 * @param ope efa rdm ope
 * @return ssize_t 0 on success, negative integer on failure.
 */
ssize_t efa_rdm_ope_repost_ope_queued_before_handshake(struct efa_rdm_ope *ope)
{
	assert(ope->internal_flags & EFA_RDM_OPE_QUEUED_BEFORE_HANDSHAKE);

	if (!(ope->peer->flags & EFA_RDM_PEER_HANDSHAKE_RECEIVED))
		return -FI_EAGAIN;

	switch (ope->op) {
	case ofi_op_msg: /* fall through */
	case ofi_op_tagged:
		return efa_rdm_msg_post_rtm(ope->ep, ope);
	case ofi_op_write:
		return efa_rdm_rma_post_write(ope->ep, ope);
	case ofi_op_read_req:
		return efa_rdm_rma_post_read(ope->ep, ope);
	case ofi_op_atomic: /* fall through */
	case ofi_op_atomic_fetch: /* fall through */
	case ofi_op_atomic_compare:
		return efa_rdm_atomic_post_atomic(ope->ep, ope);
	default:
		EFA_WARN(FI_LOG_EP_DATA, "Unknown operation type: %d\n", ope->op);
		return -FI_EINVAL;
	}
}

int efa_rdm_ope_process_queued_ope(struct efa_rdm_ope *ope, uint16_t flag)
{
	int ret = 0;

	assert(flag & EFA_RDM_OPE_QUEUED_FLAGS);

	if (!(ope->internal_flags & flag))
		return 0;

	switch (flag) {
	case EFA_RDM_OPE_QUEUED_BEFORE_HANDSHAKE:
		ret = efa_rdm_ope_repost_ope_queued_before_handshake(ope);
		--ope->ep->ope_queued_before_handshake_cnt;
		break;
	case EFA_RDM_OPE_QUEUED_RNR:
		assert(!dlist_empty(&ope->queued_pkts));
		ret = efa_rdm_ep_post_queued_pkts(ope->ep, &ope->queued_pkts);
		break;
	case EFA_RDM_OPE_QUEUED_CTRL:
		ret = efa_rdm_ope_post_send(ope, ope->queued_ctrl_type);
		break;
	case EFA_RDM_OPE_QUEUED_READ:
		ret = efa_rdm_ope_post_read(ope);
		break;
	default:
		break;
	}

	if (OFI_UNLIKELY(ret)) {
		if (ret == -FI_EAGAIN)
			return ret;

		assert(ope->type == EFA_RDM_TXE || ope->type == EFA_RDM_RXE);
		if (ope->type == EFA_RDM_TXE)
			efa_rdm_txe_handle_error(ope, -ret, FI_EFA_ERR_PKT_POST);
		else
			efa_rdm_rxe_handle_error(ope, -ret, FI_EFA_ERR_PKT_POST);
		return ret;
	}

	ope->internal_flags &= ~flag;
	dlist_remove(&ope->queued_entry);
	return ret;
}
