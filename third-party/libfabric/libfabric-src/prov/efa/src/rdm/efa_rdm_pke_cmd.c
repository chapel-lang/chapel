/* SPDX-License-Identifier: BSD-2-Clause OR GPL-2.0-only */
/* SPDX-FileCopyrightText: Copyright Amazon.com, Inc. or its affiliates. All rights reserved. */

#include "efa.h"
#include "efa_av.h"
#include "efa_cntr.h"
#include "efa_rdm_msg.h"
#include "efa_rdm_pke_cmd.h"
#include "efa_rdm_pke_rtm.h"
#include "efa_rdm_pke_rtw.h"
#include "efa_rdm_pke_rtr.h"
#include "efa_rdm_pke_rta.h"
#include "efa_rdm_pke_utils.h"
#include "efa_rdm_pke_nonreq.h"
#include "efa_rdm_pke_req.h"

/* Handshake wait timeout in microseconds */
#define EFA_RDM_HANDSHAKE_WAIT_TIMEOUT 1000000

/**
 * @brief fill the a pkt_entry with data
 *
 * This function call the init functions of each packet type to
 * fill a packet with data.
 * Generally, those init functions will:
 * 1. fill "wiredata" with packet header
 * 2. set the "payload", "payload_size" and "payload_mr" field of
 *    a packet entry
 * 3. set the "pkt_size" field of the packet entry
 *
 * @param[out]		pkt_entry	packet entry whose "wiredata" is to be filled
 * @param[in]		pkt_type	packet type
 * @param[in]		ope		operation entry
 * @param[in]		data_offset	the data's offset in respect of user's buffer.
 *					will be -1 for packet type that does not contain data.
 * @param[in]		data_size	data size carried in this packet. This argument
 * 					is only available for DATA, MEDIUM_RTM and RUNTREAD_RTM.
 * 					For all other data types, this argument will be -1.
 *
 * @return 	0 on success
 * 		negative libfabric error code on error
 *
 * @related efa_rdm_pke
 */
int efa_rdm_pke_fill_data(struct efa_rdm_pke *pkt_entry,
			  int pkt_type,
			  struct efa_rdm_ope *ope,
			  int64_t data_offset,
			  int data_size)
{
	int ret = 0;

	/* Only 3 categories of packets has data_size and data_offset:
	 * data packet, medium req and runtread req.
	 *
	 * For other packet types:
	 *
	 * If the packet type does not contain data, the argument
	 * "data_offset" and "data_size" should both be -1.
	 *
	 * If the packet type contain data, the argument data_offset
	 * should 0, the argument "data_size" should be -1, and
	 * the actual data size will be decided by the packet
	 * type's init() function.
	 */
	switch (pkt_type) {
	case EFA_RDM_READRSP_PKT:
		assert(data_offset == 0 && data_size == -1);
		ret = efa_rdm_pke_init_readrsp(pkt_entry, ope);
		break;
	case EFA_RDM_CTS_PKT:
		assert(data_offset == -1 && data_size == -1);
		ret = efa_rdm_pke_init_cts(pkt_entry, ope);
		break;
	case EFA_RDM_EOR_PKT:
		assert(data_offset == -1 && data_size == -1);
		ret = efa_rdm_pke_init_eor(pkt_entry, ope);
		break;
	case EFA_RDM_ATOMRSP_PKT:
		assert(data_offset == 0 && data_size == -1);
		ret = efa_rdm_pke_init_atomrsp(pkt_entry, ope);
		break;
	case EFA_RDM_RECEIPT_PKT:
		assert(data_offset == -1 && data_size == -1);
		ret = efa_rdm_pke_init_receipt(pkt_entry, ope);
		break;
	case EFA_RDM_EAGER_MSGRTM_PKT:
		assert(data_offset == 0 && data_size == -1);
		ret = efa_rdm_pke_init_eager_msgrtm(pkt_entry, ope);
		break;
	case EFA_RDM_EAGER_TAGRTM_PKT:
		assert(data_offset == 0 && data_size == -1);
		ret = efa_rdm_pke_init_eager_tagrtm(pkt_entry, ope);
		break;
	case EFA_RDM_MEDIUM_MSGRTM_PKT:
		assert(data_offset >= 0 && data_size > 0);
		ret = efa_rdm_pke_init_medium_msgrtm(pkt_entry, ope, data_offset, data_size);
		break;
	case EFA_RDM_MEDIUM_TAGRTM_PKT:
		assert(data_offset >= 0 && data_size > 0);
		ret = efa_rdm_pke_init_medium_tagrtm(pkt_entry, ope, data_offset, data_size);
		break;
	case EFA_RDM_LONGCTS_MSGRTM_PKT:
		/* The data_offset will be non-zero when the long CTS RTM packet
		 * is sent to continue a runting read transfer after the
		 * receiver has run out of memory registrations */
		assert((data_offset == 0 || ope->internal_flags & EFA_RDM_OPE_READ_NACK) && data_size == -1);
		ret = efa_rdm_pke_init_longcts_msgrtm(pkt_entry, ope);
		break;
	case EFA_RDM_LONGCTS_TAGRTM_PKT:
		/* The data_offset will be non-zero when the long CTS RTM packet
		 * is sent to continue a runting read transfer after the
		 * receiver has run out of memory registrations */
		assert((data_offset == 0 || ope->internal_flags & EFA_RDM_OPE_READ_NACK) && data_size == -1);
		ret = efa_rdm_pke_init_longcts_tagrtm(pkt_entry, ope);
		break;
	case EFA_RDM_LONGREAD_MSGRTM_PKT:
		assert(data_offset == -1 && data_size == -1);
		ret = efa_rdm_pke_init_longread_msgrtm(pkt_entry, ope);
		break;
	case EFA_RDM_LONGREAD_TAGRTM_PKT:
		assert(data_offset == -1 && data_size == -1);
		ret = efa_rdm_pke_init_longread_tagrtm(pkt_entry, ope);
		break;
	case EFA_RDM_RUNTREAD_MSGRTM_PKT:
		assert(data_offset >= 0 && data_size > 0);
		ret = efa_rdm_pke_init_runtread_msgrtm(pkt_entry, ope, data_offset, data_size);
		break;
	case EFA_RDM_RUNTREAD_TAGRTM_PKT:
		assert(data_offset >= 0 && data_size > 0);
		ret = efa_rdm_pke_init_runtread_tagrtm(pkt_entry, ope, data_offset, data_size);
		break;
	case EFA_RDM_EAGER_RTW_PKT:
		assert(data_offset == 0 && data_size == -1);
		ret = efa_rdm_pke_init_eager_rtw(pkt_entry, ope);
		break;
	case EFA_RDM_LONGCTS_RTW_PKT:
		assert(data_offset == 0 && data_size == -1);
		ret = efa_rdm_pke_init_longcts_rtw(pkt_entry, ope);
		break;
	case EFA_RDM_LONGREAD_RTW_PKT:
		assert(data_offset == -1 && data_size == -1);
		ret = efa_rdm_pke_init_longread_rtw(pkt_entry, ope);
		break;
	case EFA_RDM_SHORT_RTR_PKT:
		assert(data_offset == -1 && data_size == -1);
		ret = efa_rdm_pke_init_short_rtr(pkt_entry, ope);
		break;
	case EFA_RDM_LONGCTS_RTR_PKT:
		assert(data_offset == -1 && data_size == -1);
		ret = efa_rdm_pke_init_longcts_rtr(pkt_entry, ope);
		break;
	case EFA_RDM_WRITE_RTA_PKT:
		assert(data_offset == 0 && data_size == -1);
		ret = efa_rdm_pke_init_write_rta(pkt_entry, ope);
		break;
	case EFA_RDM_FETCH_RTA_PKT:
		assert(data_offset == 0 && data_size == -1);
		ret = efa_rdm_pke_init_fetch_rta(pkt_entry, ope);
		break;
	case EFA_RDM_COMPARE_RTA_PKT:
		assert(data_offset == 0 && data_size == -1);
		ret = efa_rdm_pke_init_compare_rta(pkt_entry, ope);
		break;
	case EFA_RDM_DC_EAGER_MSGRTM_PKT:
		assert(data_offset == 0 && data_size == -1);
		ret = efa_rdm_pke_init_dc_eager_msgrtm(pkt_entry, ope);
		break;
	case EFA_RDM_DC_EAGER_TAGRTM_PKT:
		assert(data_offset == 0 && data_size == -1);
		ret = efa_rdm_pke_init_dc_eager_tagrtm(pkt_entry, ope);
		break;
	case EFA_RDM_DC_MEDIUM_MSGRTM_PKT:
		assert(data_offset >= 0 && data_size > 0);
		ret = efa_rdm_pke_init_dc_medium_msgrtm(pkt_entry, ope, data_offset, data_size);
		break;
	case EFA_RDM_DC_MEDIUM_TAGRTM_PKT:
		assert(data_offset >= 0 && data_size > 0);
		ret = efa_rdm_pke_init_dc_medium_tagrtm(pkt_entry, ope, data_offset, data_size);
		break;
	case EFA_RDM_DC_LONGCTS_MSGRTM_PKT:
		assert(data_offset == 0 && data_size == -1);
		ret = efa_rdm_pke_init_dc_longcts_msgrtm(pkt_entry, ope);
		break;
	case EFA_RDM_DC_LONGCTS_TAGRTM_PKT:
		assert(data_offset == 0 && data_size == -1);
		ret = efa_rdm_pke_init_dc_longcts_tagrtm(pkt_entry, ope);
		break;
	case EFA_RDM_DC_EAGER_RTW_PKT:
		assert(data_offset == 0 && data_size == -1);
		ret = efa_rdm_pke_init_dc_eager_rtw(pkt_entry, ope);
		break;
	case EFA_RDM_DC_LONGCTS_RTW_PKT:
		assert(data_offset == 0 && data_size == -1);
		ret = efa_rdm_pke_init_dc_longcts_rtw(pkt_entry, ope);
		break;
	case EFA_RDM_DC_WRITE_RTA_PKT:
		assert(data_offset == 0 && data_size == -1);
		ret = efa_rdm_pke_init_dc_write_rta(pkt_entry, ope);
		break;
	case EFA_RDM_CTSDATA_PKT:
		assert(data_offset >= 0 && data_size > 0);
		ret = efa_rdm_pke_init_ctsdata(pkt_entry, ope, data_offset, data_size);
		break;
	case EFA_RDM_READ_NACK_PKT:
		assert(data_offset == -1 && data_size == -1);
		ret = efa_rdm_pke_init_read_nack(pkt_entry, ope);
		break;
	default:
		assert(0 && "unknown pkt type to init");
		ret = -FI_EINVAL;
		break;
	}

	return ret;
}

/**
 * @brief handle the "sent" event of a packet
 *
 * "send" means that device has accepted the send request, but
 * has not returned a send completion.
 *
 * This function call the handle_sent() function of each
 * packet type to handle such event
 *
 * @param[in,out]	pkt_entry	packet entry
 */
void efa_rdm_pke_handle_sent(struct efa_rdm_pke *pkt_entry)
{
	int pkt_type = efa_rdm_pke_get_base_hdr(pkt_entry)->type;

	switch (pkt_type) {
	case EFA_RDM_READRSP_PKT:
		efa_rdm_pke_handle_readrsp_sent(pkt_entry);
		break;
	case EFA_RDM_CTS_PKT:
		efa_rdm_pke_handle_cts_sent(pkt_entry);
		break;
	case EFA_RDM_EOR_PKT:
		/* nothing to do */
		break;
	case EFA_RDM_ATOMRSP_PKT:
		/* nothing to do */
		break;
	case EFA_RDM_RECEIPT_PKT:
		/* nothing to do */
		break;
	case EFA_RDM_EAGER_MSGRTM_PKT:
	case EFA_RDM_EAGER_TAGRTM_PKT:
		/* nothing to do */
		break;
	case EFA_RDM_MEDIUM_MSGRTM_PKT:
	case EFA_RDM_MEDIUM_TAGRTM_PKT:
	case EFA_RDM_DC_MEDIUM_MSGRTM_PKT:
	case EFA_RDM_DC_MEDIUM_TAGRTM_PKT:
		efa_rdm_pke_handle_medium_rtm_sent(pkt_entry);
		break;
	case EFA_RDM_LONGCTS_MSGRTM_PKT:
	case EFA_RDM_DC_LONGCTS_MSGRTM_PKT:
	case EFA_RDM_LONGCTS_TAGRTM_PKT:
	case EFA_RDM_DC_LONGCTS_TAGRTM_PKT:
		efa_rdm_pke_handle_longcts_rtm_sent(pkt_entry);
		break;
	case EFA_RDM_LONGREAD_MSGRTM_PKT:
	case EFA_RDM_LONGREAD_TAGRTM_PKT:
		efa_rdm_pke_handle_longread_rtm_sent(pkt_entry);
		break;
	case EFA_RDM_RUNTREAD_MSGRTM_PKT:
	case EFA_RDM_RUNTREAD_TAGRTM_PKT:
		efa_rdm_pke_handle_runtread_rtm_sent(pkt_entry);
		break;
	case EFA_RDM_EAGER_RTW_PKT:
		/* nothing to do when EAGER RTW is sent */
		break;
	case EFA_RDM_LONGCTS_RTW_PKT:
	case EFA_RDM_DC_LONGCTS_RTW_PKT:
		efa_rdm_pke_handle_longcts_rtw_sent(pkt_entry);
		break;
	case EFA_RDM_LONGREAD_RTW_PKT:
		/* nothing to do when LONGREAD RTW is sent */
		break;
	case EFA_RDM_SHORT_RTR_PKT:
	case EFA_RDM_LONGCTS_RTR_PKT:
		/* nothing can be done when RTR packets are sent */
		break;
	case EFA_RDM_WRITE_RTA_PKT:
	case EFA_RDM_DC_WRITE_RTA_PKT:
	case EFA_RDM_FETCH_RTA_PKT:
	case EFA_RDM_COMPARE_RTA_PKT:
		/* nothing can be done when RTA packets are sent */
		break;
	case EFA_RDM_DC_EAGER_MSGRTM_PKT:
	case EFA_RDM_DC_EAGER_TAGRTM_PKT:
	case EFA_RDM_DC_EAGER_RTW_PKT:
		/* nothing to do for DC EAGER RTM/RTW */
		break;
	case EFA_RDM_CTSDATA_PKT:
		efa_rdm_pke_handle_ctsdata_sent(pkt_entry);
		break;
	case EFA_RDM_READ_NACK_PKT:
		/* Nothing to do */
		break;
	default:
		assert(0 && "Unknown packet type to handle sent");
		break;
	}
}

/**
 * @brief handle the event that data has been copied to the user buffer
 *
 * This function will increase the "bytes_copied" counter by "payload_size".
 * It will release the RX packet entry.
 * If all data has been copied, it will call ope's handle to handle
 * recv complete event.
 *
 * @param[in,out]	pkt_entry	packet entry
 */
void efa_rdm_pke_handle_data_copied(struct efa_rdm_pke *pkt_entry)
{
	struct efa_rdm_ope *ope;
	struct efa_rdm_ep *ep;

	ope = pkt_entry->ope;
	assert(ope);
	ep = pkt_entry->ep;
	assert(ep);

	ope->bytes_copied += pkt_entry->payload_size;
	efa_rdm_pke_release_rx(pkt_entry);

	if (ope->total_len == ope->bytes_copied) {
		if (ope->cuda_copy_method == EFA_RDM_CUDA_COPY_BLOCKING) {
			assert(ep->blocking_copy_rxe_num > 0);
			ope->cuda_copy_method = EFA_RDM_CUDA_COPY_UNSPEC;
			ep->blocking_copy_rxe_num -= 1;
		}

		efa_rdm_ope_handle_recv_completed(ope);
	}
}

/**
 * @brief handle the a packet that encountered error completion while sending
 *
 * Depend on the packet type and error type, the error are handled differently.
 *
 * If the packet is associated with an user initialized TX operation:
 * (TX means send,read or write; such packets include all REQ packets and DATA):
 *
 *    If the error is Receiver Not Ready (RNR). there are two cases:
 *
 *         If user wants to manager RNR by itself (FI_RM_DISABLED),
 *         an error CQ entry will be written.
 *
 *         Otherwise, the packet will be queued and resent by progress engine.
 *
 *    For other type of error, an error CQ entry is written.
 *
 * If the packet is associated with an user initialized recv operiaton,
 * (such packets include EOR, CTS):
 *
 *      If the error is RNR, the packet is queued and resent by progress
 *      engine. No CQ entry is written.
 *
 *      For other types of error, an error CQ entry is written.
 *
 * If the packet is not associated with a user operation (such packet include
 * HANDSHAKE):
 *
 *      If the error is RNR, the packet is queued and resent by progress engine.
 *
 *      For other types of error, an error EQ entry is written.
 *
 * @param[in]	pkt_entry	pkt entry
 * @param[in]	err		libfabric error code
 * @param[in]	prov_errno	provider specific error code
 */
void efa_rdm_pke_handle_tx_error(struct efa_rdm_pke *pkt_entry, int err, int prov_errno)
{
	struct efa_rdm_peer *peer;
	struct efa_rdm_ope *txe;
	struct efa_rdm_ope *rxe;
	struct efa_rdm_ep *ep;

	assert(pkt_entry->alloc_type == EFA_RDM_PKE_FROM_EFA_TX_POOL);

	EFA_DBG(FI_LOG_CQ, "Packet send error: %s (%d)\n",
	        efa_strerror(prov_errno), prov_errno);

	ep = pkt_entry->ep;
	efa_rdm_ep_record_tx_op_completed(ep, pkt_entry);

	peer = efa_rdm_ep_get_peer(ep, pkt_entry->addr);
	if (!peer) {
		/*
		 * If peer is NULL, it means the peer has been removed from AV.
		 * In this case, ignore this error completion.
		 */
		EFA_WARN(FI_LOG_CQ, "ignoring send error completion of a packet to a removed peer.\n");
		efa_rdm_pke_release_tx(pkt_entry);
		return;
	}

	switch (pkt_entry->ope->type) {
	case EFA_RDM_TXE:
		txe = pkt_entry->ope;
		if (efa_rdm_pke_get_base_hdr(pkt_entry)->type == EFA_RDM_HANDSHAKE_PKT) {
			if (prov_errno == EFA_IO_COMP_STATUS_REMOTE_ERROR_RNR) {
				/*
				 * handshake should always be queued for RNR
				 */
				assert(!(peer->flags & EFA_RDM_PEER_HANDSHAKE_QUEUED));
				peer->flags |= EFA_RDM_PEER_HANDSHAKE_QUEUED;
				dlist_insert_tail(&peer->handshake_queued_entry,
						  &ep->handshake_queued_peer_list);
			} else if (prov_errno != EFA_IO_COMP_STATUS_REMOTE_ERROR_BAD_DEST_QPN) {
				/*
				 * If prov_errno is EFA_IO_COMP_STATUS_REMOTE_ERROR_BAD_DEST_QPN
				 * the peer has been destroyed. Which is normal, as peer does not
				 * always need a handshake packet to perform its duty. (For example,
				 * if a peer just want to sent 1 message to the ep, it does not need
				 * handshake.) In this case, it is safe to ignore this error
				 * completion. In all other cases, we write an eq entry because
				 * there is no application operation associated with handshake.
				 */
				char ep_addr_str[OFI_ADDRSTRLEN], peer_addr_str[OFI_ADDRSTRLEN];
				size_t buflen=0;

				memset(&ep_addr_str, 0, sizeof(ep_addr_str));
				memset(&peer_addr_str, 0, sizeof(peer_addr_str));
				buflen = sizeof(ep_addr_str);
				efa_rdm_ep_raw_addr_str(ep, ep_addr_str, &buflen);
				buflen = sizeof(peer_addr_str);
				efa_rdm_ep_get_peer_raw_addr_str(ep, pkt_entry->addr, peer_addr_str, &buflen);
				EFA_WARN(FI_LOG_CQ,
					"While sending a handshake packet, an error occurred."
					"  Our address: %s, peer address: %s\n",
					ep_addr_str, peer_addr_str);
				efa_base_ep_write_eq_error(&ep->base_ep, err, prov_errno);
			}

			efa_rdm_pke_release_tx(pkt_entry);
			efa_rdm_txe_release(txe);

			break;
		}

		if (prov_errno == EFA_IO_COMP_STATUS_REMOTE_ERROR_RNR) {
			if (ep->handle_resource_management == FI_RM_DISABLED) {
				/*
				 * Write an error to the application for RNR when resource
				 * management is disabled.
				 *
				 * Note that a txe might send multiple packets, therefore
				 * might encounter RNR from device multiple times, but it
				 * should only write cq err entry once
				 */
				if (!(txe->internal_flags & EFA_RDM_TXE_WRITTEN_RNR_CQ_ERR_ENTRY)) {
					txe->internal_flags |= EFA_RDM_TXE_WRITTEN_RNR_CQ_ERR_ENTRY;
					efa_rdm_txe_handle_error(pkt_entry->ope, FI_ENORX, prov_errno);
				}

				efa_rdm_pke_release_tx(pkt_entry);
				if (!txe->efa_outstanding_tx_ops)
					efa_rdm_txe_release(txe);
			} else {
				/*
				 * This packet is associated with a send operation, (such
				 * packets include all REQ, DATA) thus shoud be queued for RNR
				 * only if application wants EFA to manager resource.
				 */
				efa_rdm_ep_queue_rnr_pkt(ep, &txe->queued_pkts, pkt_entry);
				if (!(txe->internal_flags & EFA_RDM_OPE_QUEUED_RNR)) {
					txe->internal_flags |= EFA_RDM_OPE_QUEUED_RNR;
					dlist_insert_tail(&txe->queued_rnr_entry,
							  &ep->ope_queued_rnr_list);
				}
			}
		} else {
			efa_rdm_txe_handle_error(pkt_entry->ope, err, prov_errno);
			efa_rdm_pke_release_tx(pkt_entry);
		}
		break;
	case EFA_RDM_RXE:
		rxe = pkt_entry->ope;
		if (prov_errno == EFA_IO_COMP_STATUS_REMOTE_ERROR_RNR) {
			/*
			 * This packet is associated with a recv operation, (such packets
			 * include CTS and EOR) thus should always be queued for RNR. This
			 * is regardless value of ep->handle_resource_management, because
			 * resource management is only applied to send operation.
			 */
			efa_rdm_ep_queue_rnr_pkt(ep, &rxe->queued_pkts, pkt_entry);
			if (!(rxe->internal_flags & EFA_RDM_OPE_QUEUED_RNR)) {
				rxe->internal_flags |= EFA_RDM_OPE_QUEUED_RNR;
				dlist_insert_tail(&rxe->queued_rnr_entry,
						  &ep->ope_queued_rnr_list);
			}
		} else {
			efa_rdm_rxe_handle_error(pkt_entry->ope, err, prov_errno);
			efa_rdm_pke_release_tx(pkt_entry);
		}
		break;
	default:
		EFA_WARN(FI_LOG_CQ, "Unknown x_entry type: %d\n", pkt_entry->ope->type);
		assert(0 && "unknown x_entry state");
		efa_base_ep_write_eq_error(&ep->base_ep, err, prov_errno);
		efa_rdm_pke_release_tx(pkt_entry);
		break;
	}
}

/**
 * @brief handle the event that a send request has been completed
 *
 * send completion means that EFA device has acknowledge that data has been sent,
 * therefore buffer can be safely reused.
 *
 * This function will call each packet type's send completion handler,
 * then release the packet entry.
 *
 * @param[in,out]	pkt_entry	packet entry
 */
void efa_rdm_pke_handle_send_completion(struct efa_rdm_pke *pkt_entry)
{
	struct efa_rdm_ep *ep;

	ep = pkt_entry->ep;
	/*
	 * For a send completion, pkt_entry->addr can be FI_ADDR_NOTAVAIL in 3 situations:
	 * 1. the pkt_entry is used for a local read operation
	 * 2. a new peer with same gid+qpn was inserted to av, thus the peer was removed from AV.
	 * 3. application removed the peer's address from av.
	 * In 1, we should proceed. For 2 and 3, the send completion should be ignored.
	 */
	if (pkt_entry->addr == FI_ADDR_NOTAVAIL &&
	    !(pkt_entry->flags & EFA_RDM_PKE_LOCAL_READ)) {
		EFA_WARN(FI_LOG_CQ, "ignoring send completion of a packet to a removed peer.\n");
		efa_rdm_ep_record_tx_op_completed(ep, pkt_entry);
		efa_rdm_pke_release_tx(pkt_entry);
		return;
	}

	switch (efa_rdm_pke_get_base_hdr(pkt_entry)->type) {
	case EFA_RDM_HANDSHAKE_PKT:
		efa_rdm_txe_release(pkt_entry->ope);
		break;
	case EFA_RDM_CTS_PKT:
		break;
	case EFA_RDM_CTSDATA_PKT:
		efa_rdm_pke_handle_ctsdata_send_completion(pkt_entry);
		break;
	case EFA_RDM_READRSP_PKT:
		efa_rdm_pke_handle_readrsp_send_completion(pkt_entry);
		break;
	case EFA_RDM_EOR_PKT:
		efa_rdm_pke_handle_eor_send_completion(pkt_entry);
		break;
	case EFA_RDM_RMA_CONTEXT_PKT:
		efa_rdm_pke_handle_rma_completion(pkt_entry);
		return;
	case EFA_RDM_ATOMRSP_PKT:
		efa_rdm_pke_handle_atomrsp_send_completion(pkt_entry);
		break;
	case EFA_RDM_RECEIPT_PKT:
		efa_rdm_pke_handle_receipt_send_completion(pkt_entry);
		break;
	case EFA_RDM_EAGER_MSGRTM_PKT:
	case EFA_RDM_EAGER_TAGRTM_PKT:
		efa_rdm_pke_handle_eager_rtm_send_completion(pkt_entry);
		break;
	case EFA_RDM_MEDIUM_MSGRTM_PKT:
	case EFA_RDM_MEDIUM_TAGRTM_PKT:
		efa_rdm_pke_handle_medium_rtm_send_completion(pkt_entry);
		break;
	case EFA_RDM_LONGCTS_MSGRTM_PKT:
	case EFA_RDM_LONGCTS_TAGRTM_PKT:
		efa_rdm_pke_handle_longcts_rtm_send_completion(pkt_entry);
		break;
	case EFA_RDM_LONGREAD_MSGRTM_PKT:
	case EFA_RDM_LONGREAD_TAGRTM_PKT:
		/* nothing to do */
		break;
	case EFA_RDM_RUNTREAD_MSGRTM_PKT:
	case EFA_RDM_RUNTREAD_TAGRTM_PKT:
		efa_rdm_pke_handle_runtread_rtm_send_completion(pkt_entry);
		break;
	case EFA_RDM_EAGER_RTW_PKT:
		efa_rdm_pke_handle_eager_rtw_send_completion(pkt_entry);
		break;
	case EFA_RDM_LONGCTS_RTW_PKT:
		efa_rdm_pke_handle_longcts_rtw_send_completion(pkt_entry);
		break;
	case EFA_RDM_LONGREAD_RTW_PKT:
		/* nothing to do when long rtw send completes*/
		break;
	case EFA_RDM_SHORT_RTR_PKT:
	case EFA_RDM_LONGCTS_RTR_PKT:
		/* Unlike other protocol, for emulated read, txe
		 * is released in efa_rdm_ope_handle_recv_completed().
	         * Therefore there is nothing to be done here.
		 */
		break;
	case EFA_RDM_WRITE_RTA_PKT:
		efa_rdm_pke_handle_write_rta_send_completion(pkt_entry);
		break;
	case EFA_RDM_FETCH_RTA_PKT:
		/* no action to be taken here */
		break;
	case EFA_RDM_COMPARE_RTA_PKT:
		/* no action to be taken here */
		break;
	case EFA_RDM_DC_EAGER_MSGRTM_PKT:
	case EFA_RDM_DC_EAGER_TAGRTM_PKT:
	case EFA_RDM_DC_MEDIUM_MSGRTM_PKT:
	case EFA_RDM_DC_MEDIUM_TAGRTM_PKT:
	case EFA_RDM_DC_EAGER_RTW_PKT:
	case EFA_RDM_DC_WRITE_RTA_PKT:
	case EFA_RDM_DC_LONGCTS_MSGRTM_PKT:
	case EFA_RDM_DC_LONGCTS_TAGRTM_PKT:
	case EFA_RDM_DC_LONGCTS_RTW_PKT:
		/* no action to be taken here
		 * For non-dc version of these packet types,
		 * this is the place to increase bytes_acked or
		 * write tx completion.
		 * For dc, tx completion will always be
		 * written upon receving the receipt packet
		 * Moreoever, because receipt can arrive
		 * before send completion, we cannot take
		 * any action on txe here.
		 */
	case EFA_RDM_READ_NACK_PKT:
		/* no action needed for NACK packet */
		break;
	default:
		EFA_WARN(FI_LOG_CQ,
			"invalid control pkt type %d\n",
			efa_rdm_pke_get_base_hdr(pkt_entry)->type);
		assert(0 && "invalid control pkt type");
		efa_base_ep_write_eq_error(&ep->base_ep, FI_EIO, FI_EFA_ERR_INVALID_PKT_TYPE);
		return;
	}

	efa_rdm_ep_record_tx_op_completed(ep, pkt_entry);
	efa_rdm_pke_release_tx(pkt_entry);
}

/**
 * @brief handle the a packet that encountered error completion while receiving
 *
 * This function will write error cq or eq entry, then release the packet entry.
 *
 * @param[in]	pkt_entry	pkt entry
 * @param[in]	err		libfabric error code
 * @param[in]	prov_errno	provider specific error code
 */
void efa_rdm_pke_handle_rx_error(struct efa_rdm_pke *pkt_entry, int err, int prov_errno)
{
	struct efa_rdm_ep *ep;

	ep = pkt_entry->ep;
	/*
	 * we should still decrement the efa_rx_pkts_posted
	 * when getting a failed rx completion.
	 */
	assert(ep->efa_rx_pkts_posted > 0);
	ep->efa_rx_pkts_posted--;

	EFA_DBG(FI_LOG_CQ, "Packet receive error: %s (%d)\n",
	        efa_strerror(prov_errno), prov_errno);

	/*
	 * pkes posted by efa_rdm_ep_bulk_post_internal_rx_pkts
	 * are not associated with ope before being progressed
	 */
	if (!pkt_entry->ope) {
		char ep_addr_str[OFI_ADDRSTRLEN];
		size_t buflen=0;

		memset(&ep_addr_str, 0, sizeof(ep_addr_str));
		buflen = sizeof(ep_addr_str);
		efa_rdm_ep_raw_addr_str(ep, ep_addr_str, &buflen);
		EFA_WARN(FI_LOG_CQ,
			"Packet receive error from non TX/RX packet.  Our address: %s\n",
			ep_addr_str);

		efa_base_ep_write_eq_error(&ep->base_ep, err, prov_errno);
		efa_rdm_pke_release_rx(pkt_entry);
		return;
	}

	if (pkt_entry->ope->type == EFA_RDM_TXE) {
		efa_rdm_txe_handle_error(pkt_entry->ope, err, prov_errno);
	} else if (pkt_entry->ope->type == EFA_RDM_RXE) {
		efa_rdm_rxe_handle_error(pkt_entry->ope, err, prov_errno);
	} else {
		EFA_WARN(FI_LOG_CQ,
		"%s unknown x_entry type %d\n",
			__func__, pkt_entry->ope->type);
		assert(0 && "unknown x_entry state");
		efa_base_ep_write_eq_error(&ep->base_ep, err, prov_errno);
	}

	efa_rdm_pke_release_rx(pkt_entry);
}

static
fi_addr_t efa_rdm_pke_insert_addr(struct efa_rdm_pke *pkt_entry, void *raw_addr)
{
	int ret;
	fi_addr_t rdm_addr;
	struct efa_rdm_ep *ep;
	struct efa_rdm_base_hdr *base_hdr;

	ep = pkt_entry->ep;

	base_hdr = efa_rdm_pke_get_base_hdr(pkt_entry);
	if (base_hdr->version < EFA_RDM_PROTOCOL_VERSION) {
		char self_raw_addr_str[OFI_ADDRSTRLEN];
		size_t buflen = OFI_ADDRSTRLEN;

		efa_rdm_ep_raw_addr_str(ep, self_raw_addr_str, &buflen);
		EFA_WARN(FI_LOG_CQ,
			"Host %s received a packet with invalid protocol version %d.\n"
			"This host can only support protocol version %d and above.\n",
			self_raw_addr_str, base_hdr->version, EFA_RDM_PROTOCOL_VERSION);
		efa_base_ep_write_eq_error(&ep->base_ep, FI_EIO, FI_EFA_ERR_INVALID_PKT_TYPE);
		fprintf(stderr, "Host %s received a packet with invalid protocol version %d.\n"
			"This host can only support protocol version %d and above. %s:%d\n",
			self_raw_addr_str, base_hdr->version, EFA_RDM_PROTOCOL_VERSION, __FILE__, __LINE__);
		abort();
	}

	assert(base_hdr->type >= EFA_RDM_REQ_PKT_BEGIN);

	/*
	 * The message is from a peer through efa device, which means peer is not local
	 * or shm is disabled for transmission.
	 * We shouldn't insert shm av anyway in this case.
	 * Also, calling fi_av_insert internally inside progress engine is violating
	 * Libfabric standard for FI_AV_TABLE.
	 */
	ret = efa_av_insert_one(ep->base_ep.av, (struct efa_ep_addr *)raw_addr,
	                        &rdm_addr, 0, NULL, false);
	if (OFI_UNLIKELY(ret != 0)) {
		efa_base_ep_write_eq_error(&ep->base_ep, ret, FI_EFA_ERR_AV_INSERT);
		return -1;
	}

	return rdm_addr;
}

/**
 * @brief process a received packet
 *
 * @param[in]	ep		endpoint
 * @param[in]	pkt_entry	received packet entry
 */
void efa_rdm_pke_proc_received(struct efa_rdm_pke *pkt_entry)
{
	struct efa_rdm_ep *ep;
	struct efa_rdm_base_hdr *base_hdr;
	size_t payload_offset;

	ep = pkt_entry->ep;
	assert(ep);
	base_hdr = efa_rdm_pke_get_base_hdr(pkt_entry);
	if (efa_rdm_pkt_type_contains_data(base_hdr->type)) {
		payload_offset = efa_rdm_pke_get_payload_offset(pkt_entry);
		pkt_entry->payload = pkt_entry->wiredata + payload_offset;
		pkt_entry->payload_size = pkt_entry->pkt_size - payload_offset;
	}

	switch (base_hdr->type) {
	case EFA_RDM_RETIRED_RTS_PKT:
		EFA_WARN(FI_LOG_CQ,
			"Received a RTS packet, which has been retired since protocol version 4\n");
		assert(0 && "deprecated RTS pakcet received");
		efa_base_ep_write_eq_error(&ep->base_ep, FI_EIO, FI_EFA_ERR_DEPRECATED_PKT_TYPE);
		efa_rdm_pke_release_rx(pkt_entry);
		return;
	case EFA_RDM_RETIRED_CONNACK_PKT:
		EFA_WARN(FI_LOG_CQ,
			"Received a CONNACK packet, which has been retired since protocol version 4\n");
		assert(0 && "deprecated CONNACK pakcet received");
		efa_base_ep_write_eq_error(&ep->base_ep, FI_EIO, FI_EFA_ERR_DEPRECATED_PKT_TYPE);
		efa_rdm_pke_release_rx(pkt_entry);
		return;
	case EFA_RDM_EOR_PKT:
		efa_rdm_pke_handle_eor_recv(pkt_entry);
		return;
	case EFA_RDM_HANDSHAKE_PKT:
		efa_rdm_pke_handle_handshake_recv(pkt_entry);
		return;
	case EFA_RDM_CTS_PKT:
		efa_rdm_pke_handle_cts_recv(pkt_entry);
		return;
	case EFA_RDM_CTSDATA_PKT:
		efa_rdm_pke_handle_ctsdata_recv(pkt_entry);
		return;
	case EFA_RDM_READRSP_PKT:
		efa_rdm_pke_handle_readrsp_recv(pkt_entry);
		return;
	case EFA_RDM_ATOMRSP_PKT:
		efa_rdm_pke_handle_atomrsp_recv(pkt_entry);
		return;
	case EFA_RDM_RECEIPT_PKT:
		efa_rdm_pke_handle_receipt_recv(pkt_entry);
		return;
	case EFA_RDM_EAGER_MSGRTM_PKT:
	case EFA_RDM_EAGER_TAGRTM_PKT:
	case EFA_RDM_DC_EAGER_MSGRTM_PKT:
	case EFA_RDM_DC_EAGER_TAGRTM_PKT:
	case EFA_RDM_MEDIUM_MSGRTM_PKT:
	case EFA_RDM_MEDIUM_TAGRTM_PKT:
	case EFA_RDM_DC_MEDIUM_MSGRTM_PKT:
	case EFA_RDM_DC_MEDIUM_TAGRTM_PKT:
	case EFA_RDM_LONGCTS_MSGRTM_PKT:
	case EFA_RDM_LONGCTS_TAGRTM_PKT:
	case EFA_RDM_DC_LONGCTS_MSGRTM_PKT:
	case EFA_RDM_DC_LONGCTS_TAGRTM_PKT:
	case EFA_RDM_LONGREAD_MSGRTM_PKT:
	case EFA_RDM_LONGREAD_TAGRTM_PKT:
	case EFA_RDM_RUNTREAD_MSGRTM_PKT:
	case EFA_RDM_RUNTREAD_TAGRTM_PKT:
	case EFA_RDM_WRITE_RTA_PKT:
	case EFA_RDM_DC_WRITE_RTA_PKT:
	case EFA_RDM_FETCH_RTA_PKT:
	case EFA_RDM_COMPARE_RTA_PKT:
		efa_rdm_pke_handle_rtm_rta_recv(pkt_entry);
		return;
	case EFA_RDM_EAGER_RTW_PKT:
		efa_rdm_pke_handle_eager_rtw_recv(pkt_entry);
		return;
	case EFA_RDM_LONGCTS_RTW_PKT:
	case EFA_RDM_DC_LONGCTS_RTW_PKT:
		efa_rdm_pke_handle_longcts_rtw_recv(pkt_entry);
		return;
	case EFA_RDM_LONGREAD_RTW_PKT:
		efa_rdm_pke_handle_longread_rtw_recv(pkt_entry);
		return;
	case EFA_RDM_SHORT_RTR_PKT:
	case EFA_RDM_LONGCTS_RTR_PKT:
		efa_rdm_pke_handle_rtr_recv(pkt_entry);
		return;
	case EFA_RDM_DC_EAGER_RTW_PKT:
		efa_rdm_pke_handle_dc_eager_rtw_recv(pkt_entry);
		return;
	case EFA_RDM_READ_NACK_PKT:
		efa_rdm_pke_handle_read_nack_recv(pkt_entry);
		return;
	default:
		EFA_WARN(FI_LOG_CQ,
			"invalid control pkt type %d\n",
			efa_rdm_pke_get_base_hdr(pkt_entry)->type);
		assert(0 && "invalid control pkt type");
		efa_base_ep_write_eq_error(&ep->base_ep, FI_EIO, FI_EFA_ERR_INVALID_PKT_TYPE);
		efa_rdm_pke_release_rx(pkt_entry);
		return;
	}
}

/**
 * @brief Read peer raw address from packet header and insert the peer in AV.
 * @param ep Pointer to RDM endpoint
 * @param pkt_entry Pointer to packet entry
 * @returns Peer address, or FI_ADDR_NOTAVIL if the packet header does not include raw address
 */
fi_addr_t efa_rdm_pke_determine_addr(struct efa_rdm_pke *pkt_entry)
{
	struct efa_rdm_base_hdr *base_hdr;

	base_hdr = efa_rdm_pke_get_base_hdr(pkt_entry);
	if (base_hdr->type >= EFA_RDM_REQ_PKT_BEGIN && efa_rdm_pke_get_req_raw_addr(pkt_entry)) {
		void *raw_addr;
		raw_addr = efa_rdm_pke_get_req_raw_addr(pkt_entry);
		assert(raw_addr);
		return efa_rdm_pke_insert_addr(pkt_entry, raw_addr);
	}

	return FI_ADDR_NOTAVAIL;
}

/**
 * @brief handle a received packet
 *
 * @param	ep[in,out]		endpoint
 * @param	pkt_entry[in,out]	received packet, will be released by this function
 */
void efa_rdm_pke_handle_recv_completion(struct efa_rdm_pke *pkt_entry)
{
	int pkt_type;
	struct efa_rdm_ep *ep;
	struct efa_rdm_peer *peer;
	struct efa_rdm_base_hdr *base_hdr;
	struct efa_rdm_ope *zcpy_rxe = NULL;

	ep = pkt_entry->ep;
	assert(ep);

	assert(ep->efa_rx_pkts_posted > 0);
	ep->efa_rx_pkts_posted--;

	base_hdr = efa_rdm_pke_get_base_hdr(pkt_entry);
	pkt_type = base_hdr->type;
	if (pkt_type >= EFA_RDM_EXTRA_REQ_PKT_END) {
		EFA_WARN(FI_LOG_CQ,
			"Peer %d is requesting feature %d, which this EP does not support.\n",
			(int)pkt_entry->addr, base_hdr->type);

		assert(0 && "invalid REQ packet type");
		efa_base_ep_write_eq_error(&ep->base_ep, FI_EIO, FI_EFA_ERR_INVALID_PKT_TYPE);
		efa_rdm_pke_release_rx(pkt_entry);
		return;
	}

	/*
	 * Ignore packet if peer address cannot be determined. This ususally happens if
	 * we had prior communication with the peer, but
	 * application called fi_av_remove() to remove the address
	 * from address vector.
	 */
	if (pkt_entry->addr == FI_ADDR_NOTAVAIL) {
		EFA_WARN(FI_LOG_CQ,
			"Warning: ignoring a received packet from a removed address. packet type: %" PRIu8
			", packet flags: %x\n",
			efa_rdm_pke_get_base_hdr(pkt_entry)->type,
			efa_rdm_pke_get_base_hdr(pkt_entry)->flags);
		efa_rdm_pke_release_rx(pkt_entry);
		return;
	}

#if ENABLE_DEBUG
	if (!ep->use_zcpy_rx) {
		dlist_remove(&pkt_entry->dbg_entry);
		dlist_insert_tail(&pkt_entry->dbg_entry, &ep->rx_pkt_list);
	}
#ifdef ENABLE_EFA_RDM_PKE_DUMP
	efa_rdm_pke_print(pkt_entry, "Received");
#endif
#endif
	peer = efa_rdm_ep_get_peer(ep, pkt_entry->addr);
	assert(peer);
	if (peer->is_local) {
		/*
		 * This happens when the peer is on same instance, but chose to
		 * use EFA device to communicate with me. In this case, we respect
		 * that and will not use shm with the peer.
		 * TODO: decide whether to use shm through handshake packet.
		 */
		peer->is_local = 0;
	}

	efa_rdm_ep_post_handshake_or_queue(ep, peer);


	if (pkt_entry->alloc_type == EFA_RDM_PKE_FROM_USER_BUFFER) {
		assert(pkt_entry->ope);
		zcpy_rxe = pkt_entry->ope;
	}

	efa_rdm_pke_proc_received(pkt_entry);

	if (zcpy_rxe && pkt_type != EFA_RDM_EAGER_MSGRTM_PKT) {
		/* user buffer was not matched with a message,
		 * therefore reposting the buffer */
		efa_rdm_ep_post_user_recv_buf(ep, zcpy_rxe, 0);
	}
}

#if ENABLE_DEBUG

/*
 *  Functions used to dump packets
 */

#define EFA_RDM_PKE_DUMP_DATA_LEN 64

static
void efa_rdm_pke_print_handshake(char *prefix,
			         struct efa_rdm_handshake_hdr *handshake_hdr)
{
	EFA_DBG(FI_LOG_EP_DATA,
	       "%s EFA RDM HANDSHAKE packet - version: %" PRIu8
	       " flags: %x\n", prefix, handshake_hdr->version,
	       handshake_hdr->flags);

	EFA_DBG(FI_LOG_EP_DATA,
	       "%s EFA RDM HANDSHAKE packet, nextra_p3: %d\n",
	       prefix, handshake_hdr->nextra_p3);
}

static
void efa_rdm_pke_print_cts(char *prefix, struct efa_rdm_cts_hdr *cts_hdr)
{
	EFA_DBG(FI_LOG_EP_DATA,
	       "%s EFA RDM CTS packet - version: %"	PRIu8
	       " flags: %x tx_id: %" PRIu32
	       " rx_id: %"	   PRIu32
	       " window: %"	   PRIu64
	       "\n", prefix, cts_hdr->version, cts_hdr->flags,
	       cts_hdr->send_id, cts_hdr->recv_id, cts_hdr->recv_length);
}

static
void efa_rdm_pke_print_data(char *prefix, struct efa_rdm_pke *pkt_entry)
{
	struct efa_rdm_ctsdata_hdr *data_hdr;
	char str[EFA_RDM_PKE_DUMP_DATA_LEN * 4];
	size_t str_len = EFA_RDM_PKE_DUMP_DATA_LEN * 4, l, hdr_size;
	uint8_t *data;
	int i;

	str[str_len - 1] = '\0';

	data_hdr = efa_rdm_pke_get_ctsdata_hdr(pkt_entry);

	EFA_DBG(FI_LOG_EP_DATA,
	       "%s EFA RDM CTSDATA packet -  version: %" PRIu8
	       " flags: %x rx_id: %" PRIu32
	       " seg_size: %"	     PRIu64
	       " seg_offset: %"	     PRIu64
	       "\n", prefix, data_hdr->version, data_hdr->flags,
	       data_hdr->recv_id, data_hdr->seg_length,
	       data_hdr->seg_offset);

	hdr_size = sizeof(struct efa_rdm_ctsdata_hdr);
	if (data_hdr->flags & EFA_RDM_PKT_CONNID_HDR) {
		hdr_size += sizeof(struct efa_rdm_ctsdata_opt_connid_hdr);
		EFA_DBG(FI_LOG_EP_DATA,
		       "sender_connid: %d\n",
		       data_hdr->connid_hdr->connid);
	}

	data = (uint8_t *)pkt_entry->wiredata + hdr_size;

	l = snprintf(str, str_len, ("\tdata:    "));
	for (i = 0; i < MIN(data_hdr->seg_length, EFA_RDM_PKE_DUMP_DATA_LEN);
	     i++)
		l += snprintf(str + l, str_len - l, "%02x ",
			      data[i]);
	EFA_DBG(FI_LOG_EP_DATA, "%s\n", str);
}

void efa_rdm_pke_print(struct efa_rdm_pke *pkt_entry, char *prefix)
{
	struct efa_rdm_base_hdr *hdr;

	hdr = efa_rdm_pke_get_base_hdr(pkt_entry);

	switch (hdr->type) {
	case EFA_RDM_HANDSHAKE_PKT:
		efa_rdm_pke_print_handshake(prefix, efa_rdm_pke_get_handshake_hdr(pkt_entry));
		break;
	case EFA_RDM_CTS_PKT:
		efa_rdm_pke_print_cts(prefix, efa_rdm_pke_get_cts_hdr(pkt_entry));
		break;
	case EFA_RDM_CTSDATA_PKT:
		efa_rdm_pke_print_data(prefix, pkt_entry);
		break;
	default:
		EFA_WARN(FI_LOG_CQ, "invalid ctl pkt type %d\n",
			 hdr->type);
		assert(0);
		return;
	}
}
#endif

