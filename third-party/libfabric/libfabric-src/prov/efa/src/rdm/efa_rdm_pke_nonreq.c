/* SPDX-License-Identifier: BSD-2-Clause OR GPL-2.0-only */
/* SPDX-FileCopyrightText: Copyright Amazon.com, Inc. or its affiliates. All rights reserved. */

#include "efa.h"

#include "efa_rdm_msg.h"
#include "efa_cntr.h"
#include "efa_rdm_pke_cmd.h"
#include "efa_rdm_pke_utils.h"
#include "efa_rdm_pke_nonreq.h"

#include "efa_rdm_tracepoint.h"

/* This file define functons for the following packet type:
 *       HANDSHAKE
 *       CTS
 *       CTSDATA
 *       READRSP
 *       RMA_CONTEXT
 *       EOR
 */

/* HANDSHAKE packet related functions */

ssize_t efa_rdm_pke_init_handshake(struct efa_rdm_pke *pkt_entry,
				   struct efa_rdm_peer *peer)
{
	int nex;
	struct efa_rdm_handshake_hdr *handshake_hdr;
	struct efa_rdm_handshake_opt_connid_hdr *connid_hdr;
	struct efa_rdm_handshake_opt_host_id_hdr *host_id_hdr;
	struct efa_rdm_handshake_opt_device_version_hdr *device_version_hdr;
	struct efa_rdm_handshake_opt_user_recv_qp_hdr *user_recv_qp_hdr;

	handshake_hdr = (struct efa_rdm_handshake_hdr *)pkt_entry->wiredata;
	handshake_hdr->type = EFA_RDM_HANDSHAKE_PKT;
	handshake_hdr->version = EFA_RDM_PROTOCOL_VERSION;
	handshake_hdr->flags = 0;

	nex = (EFA_RDM_NUM_EXTRA_FEATURE_OR_REQUEST-1)/64 + 1;
	/*
	 * The action of plus 3 is for backward compatibility.
	 * See section 2.1 of protocol v4 document for detail.
	 */
	handshake_hdr->nextra_p3 = nex + 3;
	memcpy(handshake_hdr->extra_info, pkt_entry->ep->extra_info, nex * sizeof(uint64_t));
	pkt_entry->pkt_size = sizeof(struct efa_rdm_handshake_hdr) + nex * sizeof(uint64_t);

	/*
	 * Always include connid at the end of a handshake packet.
	 * If peer cannot make use of connid, the connid will be ignored.
	 */
	connid_hdr = (struct efa_rdm_handshake_opt_connid_hdr *)(pkt_entry->wiredata + pkt_entry->pkt_size);
	connid_hdr->connid = efa_rdm_ep_raw_addr(pkt_entry->ep)->qkey;
	handshake_hdr->flags |= EFA_RDM_PKT_CONNID_HDR;
	pkt_entry->pkt_size += sizeof(struct efa_rdm_handshake_opt_connid_hdr);

	/**
	 * Include the optional host id if it is non-zero
	 */
	if (pkt_entry->ep->host_id) {
		host_id_hdr = (struct efa_rdm_handshake_opt_host_id_hdr *)(pkt_entry->wiredata + pkt_entry->pkt_size);
		host_id_hdr->host_id = pkt_entry->ep->host_id;
		handshake_hdr->flags |= EFA_RDM_HANDSHAKE_HOST_ID_HDR;
		pkt_entry->pkt_size += sizeof(struct efa_rdm_handshake_opt_host_id_hdr);
	}

	/* Include the device version (0xEFA0, 0xEFA1, etc) */
	device_version_hdr = (struct efa_rdm_handshake_opt_device_version_hdr *) (pkt_entry->wiredata + pkt_entry->pkt_size);

	/* This assumes the global device list will never contain dissimilar EFA
	 * devices. I.e. the PCI bus will only contain EFA devices with the same
	 * vendor_part_id (0xEFA0, 0xEFA1, etc)
	 */
	device_version_hdr->device_version = g_efa_selected_device_list[0].ibv_attr.vendor_part_id;
	handshake_hdr->flags |= EFA_RDM_HANDSHAKE_DEVICE_VERSION_HDR;
	pkt_entry->pkt_size += sizeof (struct efa_rdm_handshake_opt_device_version_hdr);

	/* Include the user recv qp information (qpn and qkey) */
	if (pkt_entry->ep->extra_info[0] & EFA_RDM_EXTRA_FEATURE_REQUEST_USER_RECV_QP) {
		user_recv_qp_hdr = (struct efa_rdm_handshake_opt_user_recv_qp_hdr *) (pkt_entry->wiredata + pkt_entry->pkt_size);
		assert(pkt_entry->ep->base_ep.user_recv_qp);
		user_recv_qp_hdr->qpn = pkt_entry->ep->base_ep.user_recv_qp->qp_num;
		user_recv_qp_hdr->qkey = pkt_entry->ep->base_ep.user_recv_qp->qkey;
		handshake_hdr->flags |= EFA_RDM_HANDSHAKE_USER_RECV_QP_HDR;
		pkt_entry->pkt_size += sizeof (struct efa_rdm_handshake_opt_user_recv_qp_hdr);
	}

	pkt_entry->peer = peer;
	return 0;
}

void efa_rdm_pke_handle_handshake_recv(struct efa_rdm_pke *pkt_entry)
{
	struct efa_rdm_handshake_hdr *handshake_pkt;
	struct efa_rdm_peer *peer;
	uint64_t *host_id_ptr;

	peer = pkt_entry->peer;
	assert(peer);

	EFA_INFO(FI_LOG_CQ,
		 "HANDSHAKE received from peer with explicit fi_addr %" PRIu64
		 " implicit fi_addr %" PRIu64 "\n",
		 pkt_entry->peer->conn->fi_addr,
		 pkt_entry->peer->conn->implicit_fi_addr);

	handshake_pkt = (struct efa_rdm_handshake_hdr *)pkt_entry->wiredata;

	/* nextra_p3 is number of members in extra_info plus 3.
	 * See section 2.1 of protocol v4 document for detail
	 */
	peer->nextra_p3 = handshake_pkt->nextra_p3;
	memcpy(peer->extra_info, handshake_pkt->extra_info,
		   (handshake_pkt->nextra_p3 - 3) * sizeof(uint64_t));
	peer->flags |= EFA_RDM_PEER_HANDSHAKE_RECEIVED;

	host_id_ptr = efa_rdm_pke_get_handshake_opt_host_id_ptr(pkt_entry);
	if (host_id_ptr) {
		peer->host_id = *host_id_ptr;
		EFA_INFO(FI_LOG_CQ, "Received peer host id: i-%017lx\n", peer->host_id);
	}

	peer->device_version = efa_rdm_pke_get_handshake_opt_device_version(pkt_entry);
	EFA_INFO(FI_LOG_CQ, "Received peer EFA device version: 0x%x\n", peer->device_version);

	if (peer->extra_info[0] & EFA_RDM_EXTRA_FEATURE_REQUEST_USER_RECV_QP) {
		struct efa_rdm_handshake_opt_user_recv_qp_hdr *user_recv_qp_hdr;
		user_recv_qp_hdr = efa_rdm_pke_get_handshake_opt_user_recv_qp_ptr(pkt_entry);
		peer->user_recv_qp.qpn = user_recv_qp_hdr->qpn;
		peer->user_recv_qp.qkey = user_recv_qp_hdr->qkey;
	}

	efa_rdm_pke_release_rx(pkt_entry);
}

/*  CTS packet related functions */
ssize_t efa_rdm_pke_init_cts(struct efa_rdm_pke *pkt_entry,
			     struct efa_rdm_ope *ope)
{
	struct efa_rdm_cts_hdr *cts_hdr;
	size_t bytes_left;

	cts_hdr = (struct efa_rdm_cts_hdr *)pkt_entry->wiredata;
	cts_hdr->type = EFA_RDM_CTS_PKT;
	cts_hdr->version = EFA_RDM_PROTOCOL_VERSION;
	cts_hdr->flags = 0;

	if (ope->cq_entry.flags & FI_READ)
		cts_hdr->flags |= EFA_RDM_CTS_READ_REQ;

	/* CTS is sent by different communication protocols.
	 * CTS is sent using txe in the emulated longcts read
	 * protocol. The emulated longcts write and the longcts
	 * message protocols sends CTS using rxe.
	 * This check ensures appropriate tx_id and rx_id are
	 * assigned for the respective protocols */
	if (ope->type == EFA_RDM_TXE){
		cts_hdr->send_id = ope->rx_id;
		cts_hdr->recv_id = ope->tx_id;
	} else {
		assert(ope->type == EFA_RDM_RXE);
		cts_hdr->send_id = ope->tx_id;
		cts_hdr->recv_id = ope->rx_id;
	}

	bytes_left = ope->total_len - ope->bytes_received;
	cts_hdr->recv_length = MIN(bytes_left, efa_env.tx_min_credits * ope->ep->max_data_payload_size);
	assert(cts_hdr->recv_length > 0);
	pkt_entry->pkt_size = sizeof(struct efa_rdm_cts_hdr);

	/*
	 * always set connid header. If the peer does not need it,
	 * it will be ignored.
	 */
	cts_hdr->flags |= EFA_RDM_PKT_CONNID_HDR;
	cts_hdr->connid = efa_rdm_ep_raw_addr(ope->ep)->qkey;

	pkt_entry->peer = ope->peer;
	pkt_entry->ope = (void *)ope;
	return 0;
}

void efa_rdm_pke_handle_cts_sent(struct efa_rdm_pke *pkt_entry)
{
	struct efa_rdm_ope *ope;

	ope = pkt_entry->ope;
	ope->window = efa_rdm_pke_get_cts_hdr(pkt_entry)->recv_length;
}

void efa_rdm_pke_handle_cts_recv(struct efa_rdm_pke *pkt_entry)
{
	struct efa_rdm_ep *ep;
	struct efa_rdm_ope *ope;
	struct efa_rdm_cts_hdr *cts_pkt;

	ep = pkt_entry->ep;
	cts_pkt = (struct efa_rdm_cts_hdr *)pkt_entry->wiredata;
	ope = ofi_bufpool_get_ibuf(pkt_entry->ep->ope_pool, cts_pkt->send_id);

	ope->rx_id = cts_pkt->recv_id;
	ope->window = cts_pkt->recv_length;
	assert(ope->window > 0);

	efa_rdm_pke_release_rx(pkt_entry);

	if (ope->state != EFA_RDM_OPE_SEND) {
		ope->state = EFA_RDM_OPE_SEND;
		dlist_insert_tail(&ope->entry, &efa_rdm_ep_domain(ep)->ope_longcts_send_list);
	}
}

/* CTSDATA pakcet related functions */
int efa_rdm_pke_init_ctsdata(struct efa_rdm_pke *pkt_entry,
			     struct efa_rdm_ope *ope,
			     size_t data_offset,
			     int data_size)
{
	struct efa_rdm_ctsdata_hdr *data_hdr;
	struct efa_rdm_ep *ep;
	size_t hdr_size;
	int ret;

	ep = ope->ep;
	data_hdr = efa_rdm_pke_get_ctsdata_hdr(pkt_entry);
	data_hdr->type = EFA_RDM_CTSDATA_PKT;
	data_hdr->version = EFA_RDM_PROTOCOL_VERSION;
	data_hdr->flags = 0;

	/* Data is sent using rxe in the emulated longcts read 
	 * protocol. The emulated longcts write and the longcts 
	 * message protocols sends data using txe.
	 * This check ensures appropriate recv_id is 
	 * assigned for the respective protocols */
	if (ope->type == EFA_RDM_RXE) {
		data_hdr->recv_id = ope->tx_id;
	} else {
		assert(ope->type == EFA_RDM_TXE);
		data_hdr->recv_id = ope->rx_id;
		if (ope->internal_flags & EFA_RDM_TXE_DELIVERY_COMPLETE_REQUESTED)
			pkt_entry->flags |= EFA_RDM_PKE_DC_LONGCTS_DATA;
	}

	hdr_size = sizeof(struct efa_rdm_ctsdata_hdr);
	assert(ope->peer);
	if (efa_rdm_peer_need_connid(ope->peer)) {
		data_hdr->flags |= EFA_RDM_PKT_CONNID_HDR;
		data_hdr->connid_hdr->connid = efa_rdm_ep_raw_addr(ep)->qkey;
		hdr_size += sizeof(struct efa_rdm_ctsdata_opt_connid_hdr);
	}

	/*
	 * Data packets are sent in order so using bytes_sent is okay here.
	 */
	data_hdr->seg_offset = data_offset;
	data_hdr->seg_length = data_size;
	ret = efa_rdm_pke_init_payload_from_ope(pkt_entry,
						ope,
						hdr_size,
						data_offset,
						data_size);
	if (ret)
		return ret;

	pkt_entry->ope = (void *)ope;
	pkt_entry->peer = ope->peer;

	return 0;
}

void efa_rdm_pke_handle_ctsdata_sent(struct efa_rdm_pke *pkt_entry)
{
	struct efa_rdm_ope *ope;
	struct efa_rdm_ctsdata_hdr *data_hdr;

	data_hdr = efa_rdm_pke_get_ctsdata_hdr(pkt_entry);
	assert(data_hdr->seg_length > 0);

	ope = pkt_entry->ope;
	ope->bytes_sent += data_hdr->seg_length;
	ope->window -= data_hdr->seg_length;
	assert(ope->window >= 0);
}

void efa_rdm_pke_handle_ctsdata_send_completion(struct efa_rdm_pke *pkt_entry)
{
	struct efa_rdm_ope *ope;

	/* if this DATA packet is used by a DC protocol, the completion
	 * was (or will be) written when the receipt packet was received.
	 * The txe may have already been released. So nothing
	 * to do (or can be done) here.
	 */
	if (pkt_entry->flags & EFA_RDM_PKE_DC_LONGCTS_DATA)
		return;

	ope = pkt_entry->ope;
	ope->bytes_acked += efa_rdm_pke_get_ctsdata_hdr(pkt_entry)->seg_length;

	if (ope->total_len == ope->bytes_acked)
		efa_rdm_ope_handle_send_completed(ope);
}

void efa_rdm_pke_proc_ctsdata(struct efa_rdm_pke *pkt_entry,
			      struct efa_rdm_ope *ope,
			      char *data, size_t seg_offset,
			      size_t seg_size)
{
	bool all_received = 0;
	ssize_t err;

#if ENABLE_DEBUG
	int pkt_type = efa_rdm_pke_get_base_hdr(pkt_entry)->type;

	assert(pkt_type == EFA_RDM_CTSDATA_PKT || pkt_type == EFA_RDM_READRSP_PKT);
#endif
	ope->bytes_received += seg_size;
	assert(ope->bytes_received <= ope->total_len);
	all_received = (ope->bytes_received == ope->total_len);

	ope->window -= seg_size;
#if ENABLE_DEBUG
	/* ope can be released by #efa_rdm_pke_copy_payload_to_ope
	 * so the call to dlist_remove must happen before
	 * call to #efa_rdm_pke_copy_payload_to_ope
	 */
	if (all_received) {
		dlist_remove(&ope->pending_recv_entry);
		ope->ep->pending_recv_counter--;
	}
#endif
	err = efa_rdm_pke_copy_payload_to_ope(pkt_entry, ope);
	if (err) {
		efa_rdm_pke_release_rx(pkt_entry);
		efa_rdm_rxe_handle_error(ope, -err, FI_EFA_ERR_RXE_COPY);
	}

	if (all_received)
		return;

	if (!ope->window) {
		err = efa_rdm_ope_post_send_or_queue(ope, EFA_RDM_CTS_PKT);
		if (err) {
			EFA_WARN(FI_LOG_CQ, "post CTS packet failed!\n");
			efa_rdm_rxe_handle_error(ope, -err, FI_EFA_ERR_PKT_POST);
		}
	}
}

void efa_rdm_pke_handle_ctsdata_recv(struct efa_rdm_pke *pkt_entry)
{
	struct efa_rdm_ctsdata_hdr *data_hdr;
	struct efa_rdm_ope *ope;
	size_t hdr_size;

	data_hdr = efa_rdm_pke_get_ctsdata_hdr(pkt_entry);

	ope = ofi_bufpool_get_ibuf(pkt_entry->ep->ope_pool,
				   data_hdr->recv_id);

	hdr_size = sizeof(struct efa_rdm_ctsdata_hdr);
	if (data_hdr->flags & EFA_RDM_PKT_CONNID_HDR)
		hdr_size += sizeof(struct efa_rdm_ctsdata_opt_connid_hdr);

	efa_rdm_pke_proc_ctsdata(pkt_entry, ope,
				 pkt_entry->wiredata + hdr_size,
				 data_hdr->seg_offset,
				 data_hdr->seg_length);
}

/*  READRSP packet functions */
int efa_rdm_pke_init_readrsp(struct efa_rdm_pke *pkt_entry,
			     struct efa_rdm_ope *rxe)
{
	struct efa_rdm_readrsp_hdr *readrsp_hdr;
	int ret;

	readrsp_hdr = efa_rdm_pke_get_readrsp_hdr(pkt_entry);
	readrsp_hdr->type = EFA_RDM_READRSP_PKT;
	readrsp_hdr->version = EFA_RDM_PROTOCOL_VERSION;
	readrsp_hdr->flags = 0;
	readrsp_hdr->send_id = rxe->rx_id;
	readrsp_hdr->recv_id = rxe->tx_id;
	readrsp_hdr->flags |= EFA_RDM_PKT_CONNID_HDR;
	readrsp_hdr->connid = efa_rdm_ep_raw_addr(rxe->ep)->qkey;
	readrsp_hdr->seg_length = MIN(rxe->ep->mtu_size - sizeof(struct efa_rdm_readrsp_hdr),
				      rxe->total_len);
	pkt_entry->peer = rxe->peer;
	ret = efa_rdm_pke_init_payload_from_ope(pkt_entry, rxe,
						sizeof(struct efa_rdm_readrsp_hdr),	/* payload offset */
						0,					/* segment offset */
						readrsp_hdr->seg_length);
	return ret;
}

void efa_rdm_pke_handle_readrsp_sent(struct efa_rdm_pke *pkt_entry)
{
	struct efa_rdm_ope *rxe;
	size_t data_len;

	rxe = pkt_entry->ope;
	data_len = efa_rdm_pke_get_readrsp_hdr(pkt_entry)->seg_length;
	rxe->bytes_sent += data_len;
	rxe->window -= data_len;
	assert(rxe->window >= 0);
	if (rxe->bytes_sent < rxe->total_len) {
		if (efa_is_cache_available(efa_rdm_ep_domain(pkt_entry->ep)))
			efa_rdm_ope_try_fill_desc(rxe, 0, FI_SEND);

		rxe->state = EFA_RDM_OPE_SEND;
		dlist_insert_tail(&rxe->entry, &efa_rdm_ep_domain(pkt_entry->ep)->ope_longcts_send_list);
	}
}

void efa_rdm_pke_handle_readrsp_send_completion(struct efa_rdm_pke *pkt_entry)
{
	struct efa_rdm_ope *rxe;
	struct efa_rdm_readrsp_hdr *readrsp_hdr;

	readrsp_hdr = (struct efa_rdm_readrsp_hdr *)pkt_entry->wiredata;

	rxe = pkt_entry->ope;
	assert(rxe->cq_entry.flags & FI_READ);
	rxe->bytes_acked += readrsp_hdr->seg_length;
	if (rxe->total_len == rxe->bytes_acked)
		efa_rdm_ope_handle_send_completed(rxe);
}

void efa_rdm_pke_handle_readrsp_recv(struct efa_rdm_pke *pkt_entry)
{
	struct efa_rdm_readrsp_pkt *readrsp_pkt = NULL;
	struct efa_rdm_readrsp_hdr *readrsp_hdr = NULL;
	struct efa_rdm_ope *txe = NULL;

	readrsp_pkt = (struct efa_rdm_readrsp_pkt *)pkt_entry->wiredata;
	readrsp_hdr = &readrsp_pkt->hdr;
	txe = ofi_bufpool_get_ibuf(pkt_entry->ep->ope_pool, readrsp_hdr->recv_id);
	assert(txe->cq_entry.flags & FI_READ);
	txe->rx_id = readrsp_hdr->send_id;
	efa_rdm_pke_proc_ctsdata(pkt_entry, txe,
				 readrsp_pkt->data,
				 0, readrsp_hdr->seg_length);
}

/*  RMA_CONTEXT packet functions
 *
 *  RMA context packet is used a context of RMA operations and is not
 *  sent over wire. It is named packet because currently all EFA operation
 *  use a packet as context.
 */
void efa_rdm_pke_init_write_context(struct efa_rdm_pke *pkt_entry,
				    struct efa_rdm_ope *txe, void *local_buf,
				    size_t seg_size, void *desc,
				    uint64_t remote_buf, size_t remote_key)
{
	struct efa_rdm_rma_context_pkt *rma_context_pkt;

	pkt_entry->ope = (void *)txe;
	pkt_entry->peer = txe->peer;
	rma_context_pkt = (struct efa_rdm_rma_context_pkt *)pkt_entry->wiredata;
	rma_context_pkt->type = EFA_RDM_RMA_CONTEXT_PKT;
	rma_context_pkt->version = EFA_RDM_PROTOCOL_VERSION;
	rma_context_pkt->context_type = EFA_RDM_RDMA_WRITE_CONTEXT;
	rma_context_pkt->tx_id = txe->tx_id;

	rma_context_pkt->local_buf = local_buf;
	rma_context_pkt->seg_size = seg_size;
	rma_context_pkt->desc = desc;
	rma_context_pkt->remote_buf = remote_buf;
	rma_context_pkt->remote_key = remote_key;
}

void efa_rdm_pke_init_read_context(struct efa_rdm_pke *pkt_entry,
				   struct efa_rdm_ope *ope,
				   int read_id,
				   size_t seg_size)
{
	struct efa_rdm_rma_context_pkt *ctx_pkt;

	pkt_entry->ope = ope;
	pkt_entry->peer = ope->peer;
	pkt_entry->pkt_size = sizeof(struct efa_rdm_rma_context_pkt);

	ctx_pkt = (struct efa_rdm_rma_context_pkt *)pkt_entry->wiredata;
	ctx_pkt->type = EFA_RDM_RMA_CONTEXT_PKT;
	ctx_pkt->flags = 0;
	ctx_pkt->version = EFA_RDM_PROTOCOL_VERSION;
	ctx_pkt->context_type = EFA_RDM_RDMA_READ_CONTEXT;
	ctx_pkt->read_id = read_id;
	ctx_pkt->seg_size = seg_size;
}

static
void efa_rdm_pke_handle_rma_read_completion(struct efa_rdm_pke *context_pkt_entry)
{
	enum efa_rdm_ope_type x_entry_type;
	struct efa_rdm_ope *txe;
	struct efa_rdm_ope *rxe;
	struct efa_rdm_pke *data_pkt_entry;
	struct efa_rdm_rma_context_pkt *rma_context_pkt;
	int err;

	rma_context_pkt = (struct efa_rdm_rma_context_pkt *)context_pkt_entry->wiredata;
	assert(rma_context_pkt->type == EFA_RDM_RMA_CONTEXT_PKT);
	assert(rma_context_pkt->context_type == EFA_RDM_RDMA_READ_CONTEXT);

	x_entry_type = context_pkt_entry->ope->type;
	if (x_entry_type == EFA_RDM_TXE) {
		txe = context_pkt_entry->ope;
		assert(txe->op == ofi_op_read_req);
		txe->bytes_read_completed += rma_context_pkt->seg_size;
		if (txe->bytes_read_total_len == txe->bytes_read_completed) {
			if (txe->peer == NULL) {
				data_pkt_entry = txe->local_read_pkt_entry;
				assert(data_pkt_entry->payload_size > 0);
				/* We were using a held rx pkt to post local read */
				if (data_pkt_entry->alloc_type == EFA_RDM_PKE_FROM_EFA_RX_POOL) {
					assert(txe->ep->efa_rx_pkts_held > 0);
					txe->ep->efa_rx_pkts_held--;
				}
				efa_rdm_tracepoint(rx_pke_local_read_copy_payload_end, (size_t) data_pkt_entry, data_pkt_entry->payload_size, data_pkt_entry->ope->msg_id, (size_t) data_pkt_entry->ope->cq_entry.op_context, data_pkt_entry->ope->total_len);
				efa_rdm_pke_handle_data_copied(data_pkt_entry);
			} else {
				assert(txe && txe->cq_entry.flags & FI_READ);
				efa_rdm_txe_report_completion(txe);
			}

			efa_rdm_txe_release(txe);
		}
	} else {
		assert(x_entry_type == EFA_RDM_RXE);
		rxe = context_pkt_entry->ope;
		rxe->bytes_read_completed += rma_context_pkt->seg_size;
		assert(rxe->bytes_read_completed <= rxe->bytes_read_total_len);
		if (rxe->bytes_read_completed == rxe->bytes_read_total_len) {
			efa_rdm_tracepoint(read_completed,
				    rxe->msg_id, (size_t) rxe->cq_entry.op_context,
				    rxe->total_len, (size_t) rxe);
			err = efa_rdm_ope_post_send_or_queue(rxe, EFA_RDM_EOR_PKT);
			if (OFI_UNLIKELY(err)) {
				EFA_WARN(FI_LOG_CQ,
					"Posting of EOR failed! err=%s(%d)\n",
					fi_strerror(-err), -err);
				efa_rdm_rxe_handle_error(rxe, -err, FI_EFA_ERR_PKT_POST);
				efa_rdm_rxe_release(rxe);
			}

			rxe->internal_flags |= EFA_RDM_RXE_EOR_IN_FLIGHT;
			rxe->bytes_received += rxe->bytes_read_completed;
			rxe->bytes_copied += rxe->bytes_read_completed;
			if (rxe->bytes_copied == rxe->total_len) {
				efa_rdm_ope_handle_recv_completed(rxe);
			} else if(rxe->bytes_copied + rxe->bytes_queued_blocking_copy == rxe->total_len) {
				efa_rdm_ep_flush_queued_blocking_copy_to_hmem(context_pkt_entry->ep);
			}
		}
	}
}

/**
 * @brief Handle a single RMA completion (read or write)
 *
 * RMA Completion can either be caused by the completion of an RDMA Read,
 * an emulated RDMA Write, or a true RDMA Write.
 *
 * Note that if true RDMA Read/Write was used, the packet here was not sent over
 * but only used as the context for the request.
 *
 * @param ep[in,out]			Endpoint
 * @param context_pkt_entry[in,out]	The "Packet" which serves as context
 */
void efa_rdm_pke_handle_rma_completion(struct efa_rdm_pke *context_pkt_entry)
{
	struct efa_rdm_ope *txe = NULL;
	struct efa_rdm_rma_context_pkt *rma_context_pkt;

	assert(efa_rdm_pke_get_base_hdr(context_pkt_entry)->version == EFA_RDM_PROTOCOL_VERSION);

	rma_context_pkt = (struct efa_rdm_rma_context_pkt *)context_pkt_entry->wiredata;

	switch (rma_context_pkt->context_type) {
	case EFA_RDM_RDMA_WRITE_CONTEXT:
		txe = context_pkt_entry->ope;
		txe->bytes_write_completed += rma_context_pkt->seg_size;
		if (txe->bytes_write_completed == txe->bytes_write_total_len) {
			if (txe->fi_flags & FI_COMPLETION)
				efa_rdm_txe_report_completion(txe);
			else
				efa_cntr_report_tx_completion(&context_pkt_entry->ep->base_ep.util_ep, txe->cq_entry.flags);
			efa_rdm_txe_release(txe);
		}
		break;
	case EFA_RDM_RDMA_READ_CONTEXT:
		efa_rdm_pke_handle_rma_read_completion(context_pkt_entry);
		break;
	default:
		EFA_WARN(FI_LOG_CQ, "invalid rma_context_type in EFA_RDM_RMA_CONTEXT_PKT %d\n",
			rma_context_pkt->context_type);
		assert(0 && "invalid EFA_RDM_RMA_CONTEXT_PKT rma_context_type\n");
	}

	efa_rdm_ep_record_tx_op_completed(context_pkt_entry->ep, context_pkt_entry);
	efa_rdm_pke_release_tx(context_pkt_entry);
}

/*  EOR packet related functions */
int efa_rdm_pke_init_eor(struct efa_rdm_pke *pkt_entry, struct efa_rdm_ope *rxe)
{
	struct efa_rdm_eor_hdr *eor_hdr;

	eor_hdr = (struct efa_rdm_eor_hdr *)pkt_entry->wiredata;
	eor_hdr->type = EFA_RDM_EOR_PKT;
	eor_hdr->version = EFA_RDM_PROTOCOL_VERSION;
	eor_hdr->flags = 0;
	eor_hdr->send_id = rxe->tx_id;
	eor_hdr->recv_id = rxe->rx_id;
	eor_hdr->flags |= EFA_RDM_PKT_CONNID_HDR;
	eor_hdr->connid = efa_rdm_ep_raw_addr(rxe->ep)->qkey;
	pkt_entry->pkt_size = sizeof(struct efa_rdm_eor_hdr);
	pkt_entry->peer = rxe->peer;
	pkt_entry->ope = rxe;
	return 0;
}

void efa_rdm_pke_handle_eor_send_completion(struct efa_rdm_pke *pkt_entry)
{
	struct efa_rdm_ope *rxe;

	rxe = pkt_entry->ope;
	assert(rxe && rxe->rx_id == efa_rdm_pke_get_eor_hdr(pkt_entry)->recv_id);

	if (rxe->bytes_copied == rxe->total_len) {
		efa_rdm_rxe_release(rxe);
	} else {
		rxe->internal_flags &= ~EFA_RDM_RXE_EOR_IN_FLIGHT;
	}
}

/*  Read NACK packet related functions */
int efa_rdm_pke_init_read_nack(struct efa_rdm_pke *pkt_entry, struct efa_rdm_ope *rxe)
{
	struct efa_rdm_read_nack_hdr *nack_hdr;

	nack_hdr = (struct efa_rdm_read_nack_hdr *)pkt_entry->wiredata;
	nack_hdr->type = EFA_RDM_READ_NACK_PKT;
	nack_hdr->version = EFA_RDM_PROTOCOL_VERSION;
	nack_hdr->flags = 0;
	nack_hdr->send_id = rxe->tx_id;
	nack_hdr->recv_id = rxe->rx_id;
	nack_hdr->flags |= EFA_RDM_PKT_CONNID_HDR;
	nack_hdr->connid = efa_rdm_ep_raw_addr(rxe->ep)->qkey;
	pkt_entry->pkt_size = sizeof(struct efa_rdm_read_nack_hdr);
	pkt_entry->peer = rxe->peer;
	pkt_entry->ope = rxe;
	return 0;
}

/*
 *   Sender handles the acknowledgment (EFA_RDM_EOR_PKT) from receiver on the completion
 *   of the large message copy via fi_readmsg operation
 */
void efa_rdm_pke_handle_eor_recv(struct efa_rdm_pke *pkt_entry)
{
	struct efa_rdm_eor_hdr *eor_hdr;
	struct efa_rdm_ope *txe;

	efa_rdm_ep_domain(pkt_entry->ep)->num_read_msg_in_flight -= 1;

	eor_hdr = (struct efa_rdm_eor_hdr *)pkt_entry->wiredata;

	/* pre-post buf used here, so can NOT track back to txe with x_entry */
	txe = ofi_bufpool_get_ibuf(pkt_entry->ep->ope_pool, eor_hdr->send_id);

	txe->bytes_acked += txe->total_len - txe->bytes_runt;
	if (txe->bytes_acked == txe->total_len) {
		efa_rdm_txe_report_completion(txe);
		efa_rdm_txe_release(txe);
	}

	efa_rdm_pke_release_rx(pkt_entry);

}

/*
 *   Sender handles the read NACK from receiver when receiver has failed to register the receive buffer
 */
void efa_rdm_pke_handle_read_nack_recv(struct efa_rdm_pke *pkt_entry)
{
	struct efa_rdm_read_nack_hdr *nack_hdr;
	struct efa_rdm_ope *txe;
	bool delivery_complete_requested;

	efa_rdm_ep_domain(pkt_entry->ep)->num_read_msg_in_flight -= 1;

	nack_hdr = (struct efa_rdm_read_nack_hdr *) pkt_entry->wiredata;

	txe = ofi_bufpool_get_ibuf(pkt_entry->ep->ope_pool, nack_hdr->send_id);

	efa_rdm_pke_release_rx(pkt_entry);
	txe->internal_flags |= EFA_RDM_OPE_READ_NACK;

	delivery_complete_requested = txe->fi_flags & FI_DELIVERY_COMPLETE;

	if (txe->op == ofi_op_write) {
		EFA_INFO(FI_LOG_EP_CTRL,
			 "Sender fallback to emulated long CTS write "
			 "protocol because p2p is not available\n");
		efa_rdm_ope_post_send_or_queue(
			txe, delivery_complete_requested ?
			     EFA_RDM_DC_LONGCTS_RTW_PKT :
			     EFA_RDM_LONGCTS_RTW_PKT);
	} else if (txe->op == ofi_op_tagged) {
		EFA_INFO(FI_LOG_EP_CTRL,
			 "Sender fallback to long CTS tagged "
			 "protocol because memory registration limit "
			 "was reached on the receiver\n");
		efa_rdm_ope_post_send_or_queue(
			txe, delivery_complete_requested ?
			     EFA_RDM_DC_LONGCTS_TAGRTM_PKT :
			     EFA_RDM_LONGCTS_TAGRTM_PKT);
	} else {
		EFA_INFO(FI_LOG_EP_CTRL,
			 "Sender fallback to long CTS untagged "
			 "protocol because memory registration limit "
			 "was reached on the receiver\n");
		efa_rdm_ope_post_send_or_queue(
			txe, delivery_complete_requested ?
			     EFA_RDM_DC_LONGCTS_MSGRTM_PKT :
			     EFA_RDM_LONGCTS_MSGRTM_PKT);
	}
}

/* receipt packet related functions */
int efa_rdm_pke_init_receipt(struct efa_rdm_pke *pkt_entry, struct efa_rdm_ope *rxe)
{
	struct efa_rdm_receipt_hdr *receipt_hdr;

	receipt_hdr = efa_rdm_pke_get_receipt_hdr(pkt_entry);
	receipt_hdr->type = EFA_RDM_RECEIPT_PKT;
	receipt_hdr->version = EFA_RDM_PROTOCOL_VERSION;
	receipt_hdr->flags = 0;
	receipt_hdr->tx_id = rxe->tx_id;
	receipt_hdr->msg_id = rxe->msg_id;
	receipt_hdr->flags |= EFA_RDM_PKT_CONNID_HDR;
	receipt_hdr->connid = efa_rdm_ep_raw_addr(rxe->ep)->qkey;

	pkt_entry->pkt_size = sizeof(struct efa_rdm_receipt_hdr);
	pkt_entry->peer = rxe->peer;
	pkt_entry->ope = rxe;

	return 0;
}

void efa_rdm_pke_handle_receipt_send_completion(struct efa_rdm_pke *pkt_entry)
{
	struct efa_rdm_ope *rxe;

	rxe = pkt_entry->ope;
	efa_rdm_rxe_release(rxe);
}

void efa_rdm_pke_handle_receipt_recv(struct efa_rdm_pke *pkt_entry)
{
	struct efa_rdm_ope *txe = NULL;
	struct efa_rdm_receipt_hdr *receipt_hdr;

	receipt_hdr = efa_rdm_pke_get_receipt_hdr(pkt_entry);
	/* Retrieve the txe that will be written into TX CQ*/
	txe = ofi_bufpool_get_ibuf(pkt_entry->ep->ope_pool,
				   receipt_hdr->tx_id);
	if (!txe) {
		EFA_WARN(FI_LOG_CQ,
			"Failed to retrive the txe when hadling receipt packet.\n");
		return;
	}

	efa_rdm_ope_handle_send_completed(txe);
	efa_rdm_pke_release_rx(pkt_entry);
}

/* atomrsp packet related functions: init, handle_sent, handle_send_completion and recv
 *
 * initialize atomic response packet by creating a packet that hold original data
 * in rxe->iov. rxe->iov will then be changed by atomic operation.
 * release that packet entry until it is sent.
 */
int efa_rdm_pke_init_atomrsp(struct efa_rdm_pke *pkt_entry, struct efa_rdm_ope *rxe)
{
	struct efa_rdm_atomrsp_pkt *atomrsp_pkt;
	struct efa_rdm_atomrsp_hdr *atomrsp_hdr;

	assert(rxe->atomrsp_data);
	pkt_entry->peer = rxe->peer;
	pkt_entry->ope = rxe;

	atomrsp_pkt = (struct efa_rdm_atomrsp_pkt *)pkt_entry->wiredata;
	atomrsp_hdr = &atomrsp_pkt->hdr;
	atomrsp_hdr->type = EFA_RDM_ATOMRSP_PKT;
	atomrsp_hdr->version = EFA_RDM_PROTOCOL_VERSION;
	atomrsp_hdr->flags = 0;
	atomrsp_hdr->recv_id = rxe->tx_id;
	atomrsp_hdr->seg_length = ofi_total_iov_len(rxe->iov, rxe->iov_count);
	atomrsp_hdr->flags |= EFA_RDM_PKT_CONNID_HDR;
	atomrsp_hdr->connid = efa_rdm_ep_raw_addr(rxe->ep)->qkey;
	assert(sizeof(struct efa_rdm_atomrsp_hdr) + atomrsp_hdr->seg_length < rxe->ep->mtu_size);
	/* rxe->atomrsp_data was filled in efa_rdm_pke_handle_req_recv() */
	memcpy(atomrsp_pkt->data, rxe->atomrsp_data, atomrsp_hdr->seg_length);
	pkt_entry->pkt_size = sizeof(struct efa_rdm_atomrsp_hdr) + atomrsp_hdr->seg_length;
	return 0;
}

void efa_rdm_pke_handle_atomrsp_send_completion(struct efa_rdm_pke *pkt_entry)
{
	struct efa_rdm_ope *rxe;

	rxe = pkt_entry->ope;
	ofi_buf_free(rxe->atomrsp_data);
	rxe->atomrsp_data = NULL;
	efa_rdm_rxe_release(rxe);
}

void efa_rdm_pke_handle_atomrsp_recv(struct efa_rdm_pke *pkt_entry)
{
	struct efa_rdm_atomrsp_pkt *atomrsp_pkt = NULL;
	struct efa_rdm_atomrsp_hdr *atomrsp_hdr = NULL;
	struct efa_rdm_ope *txe = NULL;
	ssize_t ret;

	atomrsp_pkt = (struct efa_rdm_atomrsp_pkt *)pkt_entry->wiredata;
	atomrsp_hdr = &atomrsp_pkt->hdr;
	txe = ofi_bufpool_get_ibuf(pkt_entry->ep->ope_pool, atomrsp_hdr->recv_id);

	ret = efa_copy_to_hmem_iov(txe->atomic_ex.result_desc, txe->atomic_ex.resp_iov,
	                           txe->atomic_ex.resp_iov_count, atomrsp_pkt->data,
	                           atomrsp_hdr->seg_length);
	if (OFI_UNLIKELY(ret < 0)) {
		efa_base_ep_write_eq_error(&pkt_entry->ep->base_ep, -ret, EFA_IO_COMP_STATUS_LOCAL_ERROR_BAD_LENGTH);
		return;
	}

	if (txe->fi_flags & FI_COMPLETION)
		efa_rdm_txe_report_completion(txe);
	else
		efa_cntr_report_tx_completion(&pkt_entry->ep->base_ep.util_ep, txe->cq_entry.flags);

	efa_rdm_txe_release(txe);
	efa_rdm_pke_release_rx(pkt_entry);
}
