/* SPDX-License-Identifier: BSD-2-Clause OR GPL-2.0-only */
/* SPDX-FileCopyrightText: Copyright Amazon.com, Inc. or its affiliates. All rights reserved. */

#ifndef _EFA_RDM_PKE_UTILS_H
#define _EFA_RDM_PKE_UTILS_H

#include "efa_rdm_ep.h"
#include "efa_rdm_pke.h"
#include "efa_rdm_protocol.h"
#include "efa_rdm_pkt_type.h"
#include "efa_rdm_pke_rtm.h"
#include "efa_mr.h"

static inline
bool efa_rdm_pke_has_base_hdr(struct efa_rdm_pke *pke)
{
	return !(pke->flags & EFA_RDM_PKE_HAS_NO_BASE_HDR);
}

/**
 * @brief get the base header of an pke
 *
 * @param[in]	pke	packet entry
 * @returns	base header
 */
static inline
struct efa_rdm_base_hdr *efa_rdm_pke_get_base_hdr(struct efa_rdm_pke *pke)
{
	return (struct efa_rdm_base_hdr *)pke->wiredata;
}

#define efa_rdm_pkt_type_of(obj) _Generic((obj), \
		struct efa_rdm_pke *: efa_rdm_pkt_type_of_pke, \
		default: efa_rdm_pkt_type_of_base_hdr)(obj)

static inline
int efa_rdm_pkt_type_of_base_hdr(struct efa_rdm_base_hdr *base_hdr)
{
	return base_hdr->type;
}

static inline
int efa_rdm_pkt_type_of_pke(struct efa_rdm_pke *pke)
{
	if (efa_rdm_pke_has_base_hdr(pke)) {
		return efa_rdm_pkt_type_of_base_hdr(efa_rdm_pke_get_base_hdr(pke));
	}
	return EFA_RDM_HEADERLESS_PKT;
}

/**
 * @brief Get the packet type from a queued operation entry
 *
 * This function determines the packet type for control packets (like RECEIPT, EOR)
 * that are queued in an operation entry. For RNR-queued operations, it examines
 * the first packet in the queued_pkts list. For CTRL-queued operations, it
 * returns the stored queued_ctrl_type.
 *
 * @param[in] ope Operation entry with queued packets
 * @return Packet type constant (e.g., EFA_RDM_RECEIPT_PKT, EFA_RDM_EOR_PKT),
 *         or 0 if no valid packet type is found
 */
static inline
int efa_rdm_pke_get_ctrl_pkt_type_from_queued_ope(struct efa_rdm_ope *ope)
{
	struct efa_rdm_pke *pke;

	if (ope->internal_flags & EFA_RDM_OPE_QUEUED_RNR) {
		assert(!dlist_empty(&ope->queued_pkts));
		/**
		 * It should be safe to check the first
		 * pke in the queued_pkts, as for the
		 * ctrl pkt we care about,
		 * they are the only tx pkt posted
		 * from the ope
		 */
		pke = container_of(ope->queued_pkts.next, struct efa_rdm_pke,
				   entry);
		return efa_rdm_pkt_type_of_pke(pke);
	} else if (ope->internal_flags & EFA_RDM_OPE_QUEUED_CTRL) {
		return ope->queued_ctrl_type;
	} else { /* No valid ctrl pkt type */
		return 0;
	}
}

/**
 * @brief return the segment offset of user data in packet entry
 *
 * segment_offset is the user data's offset in repect of user's
 * buffer.
 *
 * @param[in]	pkt_entry	packet entry
 * @return	the value of seg_offset in the packet
 */
static inline
size_t efa_rdm_pke_get_segment_offset(struct efa_rdm_pke *pke)
{
	int pkt_type, hdr_offset;
	static const int offset_of_seg_offset_in_header[] = {
		[EFA_RDM_CTSDATA_PKT] = offsetof(struct efa_rdm_ctsdata_hdr, seg_offset),
		[EFA_RDM_MEDIUM_MSGRTM_PKT] = offsetof(struct efa_rdm_medium_rtm_base_hdr, seg_offset),
		[EFA_RDM_MEDIUM_TAGRTM_PKT] = offsetof(struct efa_rdm_medium_rtm_base_hdr, seg_offset),
		[EFA_RDM_DC_MEDIUM_MSGRTM_PKT] = offsetof(struct efa_rdm_dc_medium_rtm_base_hdr, seg_offset),
		[EFA_RDM_DC_MEDIUM_TAGRTM_PKT] = offsetof(struct efa_rdm_dc_medium_rtm_base_hdr, seg_offset),
		[EFA_RDM_RUNTREAD_MSGRTM_PKT] = offsetof(struct efa_rdm_runtread_rtm_base_hdr, seg_offset),
		[EFA_RDM_RUNTREAD_TAGRTM_PKT] = offsetof(struct efa_rdm_runtread_rtm_base_hdr, seg_offset),
	};

	pkt_type = efa_rdm_pke_get_base_hdr(pke)->type;
	assert(efa_rdm_pkt_type_contains_data(pkt_type));

	if (efa_rdm_pkt_type_contains_seg_offset(pkt_type)) {
		/* all such packet types has been listed in the array */
		hdr_offset = offset_of_seg_offset_in_header[pkt_type];

		assert(hdr_offset);
		return *(uint64_t *)(pke->wiredata + hdr_offset);
	}

	return 0;
}

/**
 * @brief copy data from the user's buffer to the packet's wiredata.
 *
 * @param[in]		iov_mr  memory descriptors of the user's data buffer.
 * @param[in,out]	pke	    packet entry. Header must have been set when the function is called.
 * @param[in]		ope		operation entry that has user buffer information.
 * @param[in]		payload_offset	the data offset in reference to pkt_entry->wiredata.
 * @param[in]		segment_offset	the data offset in reference to user's buffer
 * @param[in]		data_size	length of the data to be set up.
 * @return   		length of data copied.
 */
static inline size_t
efa_rdm_pke_copy_from_hmem_iov(struct efa_mr *iov_mr, struct efa_rdm_pke *pke,
			       struct efa_rdm_ope *ope, size_t payload_offset,
			       size_t segment_offset, size_t data_size)
{
	size_t copied;

	if (iov_mr && (iov_mr->peer.flags & OFI_HMEM_DATA_DEV_REG_HANDLE)) {
		assert(iov_mr->peer.hmem_data);
		copied = ofi_dev_reg_copy_from_hmem_iov(pke->wiredata + payload_offset,
							data_size, iov_mr->peer.iface,
							(uint64_t)iov_mr->peer.hmem_data,
							ope->iov, ope->iov_count,
							segment_offset);
	} else {
		copied = ofi_copy_from_hmem_iov(pke->wiredata + payload_offset,
		                                data_size,
		                                iov_mr ? iov_mr->peer.iface : FI_HMEM_SYSTEM,
		                                iov_mr ? iov_mr->peer.device.reserved : 0,
		                                ope->iov, ope->iov_count, segment_offset);
	}

	return copied;
}

/** 
 * @brief This function either posts RDMA read, or sends a NACK packet when p2p
 * is not available or memory registration limit was reached on the receiver.
 *
 * @param[in]	ep		endpoint
 * @param[in]	pkt_entry	packet entry
 * @param[in]	rxe		RX entry
 *
 * @return 0 on success, or a negative error code.
 */
static inline int
efa_rdm_pke_post_remote_read_or_nack(struct efa_rdm_ep *ep,
				     struct efa_rdm_pke *pkt_entry,
				     struct efa_rdm_ope *rxe)
{
	int err = 0;
	int pkt_type;
	int p2p_avail;

	pkt_type = efa_rdm_pke_get_base_hdr(pkt_entry)->type;
	err = efa_rdm_ep_use_p2p(ep, rxe->desc[0]);
	if (err < 0)
		return err;

	p2p_avail = err;
	if (p2p_avail) {
		err = efa_rdm_ope_post_remote_read_or_queue(rxe);
	} else if (ep->homogeneous_peers || efa_rdm_peer_support_read_nack(rxe->peer)) {
		EFA_INFO(FI_LOG_EP_CTRL,
			 "Receiver sending long read "
			 "NACK packet because P2P is not available, "
			 "unable to post RDMA read.\n");
		goto send_nack;
	} else {
		EFA_INFO(FI_LOG_EP_CTRL, "P2P is not available, "
					 "unable to post RDMA read.\n");
		return -FI_EOPNOTSUPP;
	}

	if (err == -FI_ENOMR) {
		if (ep->homogeneous_peers || efa_rdm_peer_support_read_nack(rxe->peer)) {
			EFA_INFO(FI_LOG_EP_CTRL, "Receiver sending long read "
						 "NACK packet because memory "
						 "registration limit was "
						 "reached on the receiver.\n");
			goto send_nack;
		} else {
			/* Peer does not support the READ_NACK packet. So we
			 * return EAGAIN and hope that the app runs progress
			 * again which will free some MR registrations */
			return -FI_EAGAIN;
		}
	}

	return err;

send_nack:
	rxe->internal_flags |= EFA_RDM_OPE_READ_NACK;
	/* Only set the flag for runting read. The NACK
	 * packet is sent after all runting read
	 * RTM packets have been received */
	if (efa_rdm_pkt_type_is_runtread(pkt_type)) {
		return 0;
	}

	if (efa_rdm_pkt_type_is_rtm(pkt_type)) {
		efa_rdm_rxe_map_insert(&pkt_entry->peer->rxe_map, efa_rdm_pke_get_rtm_msg_id(pkt_entry), rxe);
	}

	return efa_rdm_ope_post_send_or_queue(rxe, EFA_RDM_READ_NACK_PKT);
}

size_t efa_rdm_pke_get_payload_offset(struct efa_rdm_pke *pkt_entry);

ssize_t efa_rdm_pke_init_payload_from_ope(struct efa_rdm_pke *pke,
					  struct efa_rdm_ope *ope,
					  size_t payload_offset,
					  size_t segment_offset,
					  size_t data_size);

ssize_t efa_rdm_pke_copy_payload_to_ope(struct efa_rdm_pke *pke,
					struct efa_rdm_ope *ope);

uint32_t *efa_rdm_pke_connid_ptr(struct efa_rdm_pke *pkt_entry);

int efa_rdm_pke_get_available_copy_methods(struct efa_rdm_ep *ep,
					   struct efa_mr *efa_mr,
					   bool *restrict local_read_available,
					   bool *restrict cuda_memcpy_available,
					   bool *restrict gdrcopy_available);

#endif
