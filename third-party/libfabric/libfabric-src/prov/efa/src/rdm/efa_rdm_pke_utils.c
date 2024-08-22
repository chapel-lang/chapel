/* SPDX-License-Identifier: BSD-2-Clause OR GPL-2.0-only */
/* SPDX-FileCopyrightText: Copyright Amazon.com, Inc. or its affiliates. All rights reserved. */

#include <rdma/fi_rma.h>
#include "ofi_iov.h"
#include "efa.h"
#include "efa_mr.h"
#include "efa_hmem.h"
#include "efa_device.h"
#include "efa_rdm_ep.h"
#include "efa_rdm_pke.h"
#include "efa_rdm_pke_cmd.h"
#include "efa_rdm_pke_rtm.h"
#include "efa_rdm_pke_nonreq.h"
#include "efa_rdm_pke_utils.h"
#include "efa_rdm_pkt_type.h"
#include "efa_rdm_protocol.h"
#include "efa_rdm_pke_req.h"

/**
 * @brief initialize the payload, payload_size, payload_mr and pkt_size of an outgoing packet
 *
 * This function may or may not copy data from user's buffer to the packet's.
 * If copy, "payload" will be pointing to a location inside packet entry's wiredata.
 * Otherwise, "payload" will be pointing to a location insder user's buffer.
 *
 * @param[in,out]	pkt_entry	packet entry. Header must have been set when the function is called
 * @param[in]		ope		operation entry that has user buffer information.
 * @param[in]		payload_offset	the data offset in reference to pkt_entry->wiredata.
 * 					This argument is used when the function decide to copy data from
 * 					user's buffer to packet's wiredata.
 * @param[in]		segment_offset	the data offset in reference to user's buffer
 * @param[in]		data_size	length of the data to be set up.
 * @return		0 on success, negative libfabric error code on failure. Possible error codes include:
 * 	FI_EINVAL invalid segment offset.
 */
ssize_t efa_rdm_pke_init_payload_from_ope(struct efa_rdm_pke *pke,
					  struct efa_rdm_ope *ope,
					  size_t payload_offset,
					  size_t segment_offset,
					  size_t data_size)
{
	int tx_iov_index, ret;
	bool p2p_available;
	size_t tx_iov_offset, copied;
	struct efa_mr *iov_mr;

	assert(payload_offset > 0);

	pke->ope = ope;
	if (data_size == 0) {
		pke->pkt_size = payload_offset;
		return 0;
	}

	ret = ofi_iov_locate(ope->iov, ope->iov_count, segment_offset,
			     &tx_iov_index, &tx_iov_offset);
	if (OFI_UNLIKELY(ret)) {
		EFA_WARN(FI_LOG_CQ, "ofi_iov_locate failed! err: %d\n", ret);
		return ret;
	}

	assert(tx_iov_index < ope->iov_count);
	assert(tx_iov_offset < ope->iov[tx_iov_index].iov_len);
	iov_mr = ope->desc[tx_iov_index];

	/* When using EFA device, EFA device can access memory that
	 * that has been registered, and p2p is allowed to be used.
	 */
	if (iov_mr) {
		ret = efa_rdm_ep_use_p2p(pke->ep, iov_mr);
		if (ret < 0)
			return ret;
		p2p_available = ret;
	} else {
		p2p_available = false;
	}

	/*
	 * Copy can be avoid if the following 2 conditions are true:
	 * 1. EFA can directly access the memory
	 * 2. data to be send is in 1 iov, because device only support 2 iov, and we use
	 *    1st iov for header.
	 */
	if (p2p_available &&
	    (tx_iov_offset + data_size <= ope->iov[tx_iov_index].iov_len)) {
		pke->payload = (char *)ope->iov[tx_iov_index].iov_base + tx_iov_offset;
		pke->payload_size = data_size;
		pke->payload_mr = ope->desc[tx_iov_index];
		pke->pkt_size = payload_offset + data_size;
		return 0;
	}

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

	assert(copied == data_size);
	pke->payload = pke->wiredata + payload_offset;
	pke->payload_size = copied;
	pke->payload_mr = pke->mr;
	pke->pkt_size = payload_offset + copied;
	return 0;
}

/**
 * @brief flush queued blocking copy to hmem
 *
 * The copying of data from bounce buffer to hmem receiving buffer
 * is queued, and we copy them in batch.
 *
 * This functions is used to flush all the queued hmem copy.
 *
 * It can be called in two scenarios:
 *
 * 1. the number of queued hmem copy reached limit
 *
 * 2. all the data of one of the queued message has arrived.
 *
 * @param[in,out]	ep	endpoint, where queue_copy_num and queued_copy_vec reside.
 *
 */
int efa_rdm_ep_flush_queued_blocking_copy_to_hmem(struct efa_rdm_ep *ep)
{
	size_t i;
	size_t bytes_copied[EFA_RDM_MAX_QUEUED_COPY] = {0};
	struct efa_mr *desc;
	struct efa_rdm_ope *rxe;
	struct efa_rdm_pke *pkt_entry;
	char *data;
	size_t segment_offset;

	for (i = 0; i < ep->queued_copy_num; ++i) {
		pkt_entry = ep->queued_copy_vec[i].pkt_entry;
		data = ep->queued_copy_vec[i].data;
		segment_offset = ep->queued_copy_vec[i].data_offset;

		rxe = pkt_entry->ope;
		desc = rxe->desc[0];
		assert(desc && desc->peer.iface != FI_HMEM_SYSTEM);

		if (desc->peer.flags & OFI_HMEM_DATA_DEV_REG_HANDLE) {
			assert(desc->peer.hmem_data);
			bytes_copied[i] = ofi_dev_reg_copy_to_hmem_iov(
								desc->peer.iface,
								(uint64_t)desc->peer.hmem_data,
								rxe->iov, rxe->iov_count,
								segment_offset + ep->msg_prefix_size,
								data, pkt_entry->payload_size);
		} else {
			bytes_copied[i] = ofi_copy_to_hmem_iov(desc->peer.iface,
			                                       desc->peer.device.reserved,
			                                       rxe->iov, rxe->iov_count,
			                                       segment_offset + ep->msg_prefix_size,
			                                       data, pkt_entry->payload_size);
		}
	}

	for (i = 0; i < ep->queued_copy_num; ++i) {
		pkt_entry = ep->queued_copy_vec[i].pkt_entry;
		segment_offset = ep->queued_copy_vec[i].data_offset;
		rxe = pkt_entry->ope;
		if (pkt_entry->alloc_type == EFA_RDM_PKE_FROM_EFA_RX_POOL) {
			assert(ep->efa_rx_pkts_held > 0);
			ep->efa_rx_pkts_held--;
		}

		if (bytes_copied[i] != MIN(pkt_entry->payload_size,
					   rxe->cq_entry.len - segment_offset)) {
			EFA_WARN(FI_LOG_CQ, "wrong size! bytes_copied: %ld\n",
				bytes_copied[i]);
			return -FI_EIO;
		}

		rxe->bytes_queued_blocking_copy -= pkt_entry->payload_size;
		efa_rdm_pke_handle_data_copied(pkt_entry);
	}

	ep->queued_copy_num = 0;
	return 0;
}

/*
 * @brief copy data to hmem buffer by queueing
 *
 * This function queue multiple (up to EFA_RDM_MAX_QUEUED_COPY) copies to
 * device memory, and do them at the same time. This is to avoid any memory
 * barrier between copies, which will cause a flush.
 *
 * @param[in]		pke	the packet entry that contains data * @param[in]
 * @param[in]		rxe	the rx entry, who has the target buffer information
 * @return		On success, return 0
 * 			On failure, return libfabric error code
 */
static inline
int efa_rdm_pke_queued_copy_payload_to_hmem(struct efa_rdm_pke *pke,
					    struct efa_rdm_ope *rxe)
{
	struct efa_rdm_ep *ep;

	ep = pke->ep;
	assert(ep);
	assert(ep->queued_copy_num < EFA_RDM_MAX_QUEUED_COPY);
	ep->queued_copy_vec[ep->queued_copy_num].pkt_entry = pke;
	ep->queued_copy_vec[ep->queued_copy_num].data = pke->payload;
	ep->queued_copy_vec[ep->queued_copy_num].data_size = pke->payload_size;
	ep->queued_copy_vec[ep->queued_copy_num].data_offset = efa_rdm_pke_get_segment_offset(pke);
	ep->queued_copy_num += 1;

	rxe->bytes_queued_blocking_copy += pke->payload_size;

	if (pke->alloc_type == EFA_RDM_PKE_FROM_EFA_RX_POOL)
		ep->efa_rx_pkts_held++;

	if (ep->queued_copy_num < EFA_RDM_MAX_QUEUED_COPY &&
	    rxe->bytes_copied + rxe->bytes_queued_blocking_copy < rxe->total_len) {
		return 0;
	}

	return efa_rdm_ep_flush_queued_blocking_copy_to_hmem(ep);
}

/* @brief copy data in pkt_entry to CUDA memory
 *
 * There are 3 ways to copy data to CUDA memory. None of them is guaranteed to
 * be available:
 *
 * gdrcopy, which is avaibale only when cuda_is_gdrcopy_enabled() is true
 *
 * cudaMemcpy, which is available only when endpoint is permitted to call CUDA api
 *
 * localread copy, which is available only when p2p is supported by device, and device support read.
 *
 * gdrcopy and cudaMemcpy is mutally exclusive, when they are both available, cudaMemcpy is used.
 * so we consider them as blocking copy.
 *
 * When neither blocking copy and localread copy is available, this function return error.
 *
 * When only one method is available, the availble one will be used.
 *
 * When both methods are available, we used a mixed approach, e.g.
 *
 * we use blocking copy up to certain number.
 *
 * For the rest of the receive buffers, we use local read copy.
 *
 * @param[in]		pke	the packet entry that contains payload
 * @parma[in,out]	rxe	the rx entry that contains receive buffer information
 * @return		On success, return 0
 * 			On failure, return libfabric error code
 */
static inline
int efa_rdm_pke_copy_payload_to_cuda(struct efa_rdm_pke *pke,
				     struct efa_rdm_ope *rxe)
{
	static const int max_blocking_copy_rxe_num = 4;
	struct efa_mr *desc;
	struct efa_rdm_ep *ep;
	size_t segment_offset;
	bool p2p_available, local_read_available, gdrcopy_available, cuda_memcpy_available;
	int ret, err;

	desc = rxe->desc[0];
	assert(efa_mr_is_cuda(desc));

	ep = pke->ep;
	assert(ep);

	ret = efa_rdm_ep_use_p2p(ep, desc);
	if (ret < 0)
		return ret;

	segment_offset = efa_rdm_pke_get_segment_offset(pke),

	p2p_available = ret;
	local_read_available = p2p_available && efa_rdm_ep_support_rdma_read(ep);
	cuda_memcpy_available = ep->cuda_api_permitted;
	gdrcopy_available = desc->peer.flags & OFI_HMEM_DATA_DEV_REG_HANDLE;

	/* For in-order aligned send/recv, only allow local read to be used to copy data */
	if (ep->sendrecv_in_order_aligned_128_bytes) {
		cuda_memcpy_available = false;
		gdrcopy_available = false;
	}

	if (!local_read_available && !gdrcopy_available && !cuda_memcpy_available) {
		EFA_WARN(FI_LOG_CQ, "None of the copy methods: localread, gdrcopy or cudaMemcpy is available,"
			"thus libfabric is not able to copy received data to Nvidia GPU\n");
		return -FI_EINVAL;
	}

	if (!local_read_available) {
		assert(cuda_memcpy_available || gdrcopy_available);
		return efa_rdm_pke_queued_copy_payload_to_hmem(pke, rxe);
	}

	assert(local_read_available);

	if (!gdrcopy_available) {
		/* prefer local read over cudaMemcpy (when it is available)
		 * because local read copy is faster
		 */
		err = efa_rdm_rxe_post_local_read_or_queue(rxe,
							   segment_offset,
							   pke, pke->payload,
							   pke->payload_size);
		if (err)
			EFA_WARN(FI_LOG_CQ, "cannot post read to copy data\n");
		return err;
	}

	assert(gdrcopy_available && local_read_available);

	/* when both local read and gdrcopy are available, we use a mixed approach */

	if (rxe->cuda_copy_method != EFA_RDM_CUDA_COPY_LOCALREAD) {
		assert(rxe->bytes_copied + pke->payload_size <= rxe->total_len);

		/* If this packet is the last uncopied piece (or the only piece), copy it right away
		 * to achieve best latency.
		 */
		if (rxe->bytes_copied + pke->payload_size == rxe->total_len) {
			assert(desc->peer.hmem_data);
			ofi_dev_reg_copy_to_hmem_iov(FI_HMEM_CUDA, (uint64_t)desc->peer.hmem_data,
			                             rxe->iov, rxe->iov_count,
			                             segment_offset + ep->msg_prefix_size,
			                             pke->payload, pke->payload_size);
			efa_rdm_pke_handle_data_copied(pke);
			return 0;
		}

		/* If this rxe is already been chosen to use gdrcopy/cudaMemcpy, keep using on it */
		if (rxe->cuda_copy_method == EFA_RDM_CUDA_COPY_BLOCKING)
			return efa_rdm_pke_queued_copy_payload_to_hmem(pke, rxe);

		/* If there are still empty slot for using gdrcopy, use gdrcopy on this rxe */
		if (rxe->cuda_copy_method == EFA_RDM_CUDA_COPY_UNSPEC && ep->blocking_copy_rxe_num < max_blocking_copy_rxe_num) {
			rxe->cuda_copy_method = EFA_RDM_CUDA_COPY_BLOCKING;
			ep->blocking_copy_rxe_num += 1;
			return efa_rdm_pke_queued_copy_payload_to_hmem(pke, rxe);
		}
	}

	if (rxe->cuda_copy_method == EFA_RDM_CUDA_COPY_UNSPEC)
		rxe->cuda_copy_method = EFA_RDM_CUDA_COPY_LOCALREAD;

	err = efa_rdm_rxe_post_local_read_or_queue(rxe, segment_offset,
						   pke, pke->payload, pke->payload_size);
	if (err)
		EFA_WARN(FI_LOG_CQ, "cannot post read to copy data\n");

	/* At this point data has NOT been copied yet, thus we cannot call
	 * efa_rdm_pke_handle_data_copied(). The function will be called
	 * when the completion of the local read request is received
	 * (by progress engine).
	 */
	return err;
}


/**
 * @brief copy data to application's receive buffer and update counter in rxe.
 *
 * Depend on when application's receive buffer is located (host or device) and
 * the software stack, this function will select different, strategies to copy data.
 *
 * When application's receive buffer is on device, there are two scenarios:
 *
 *    If memory is on cuda GPU, and gdrcopy is not available, this function
 *    will post a local read request to copy data. (This is because NCCL forbids its
 *    plugin to make cuda calls). In this case, the data is not copied upon return of
 *    this function, and the function efa_rdm_pke_handle_data_copied() is not called. It will
 *    be called upon the completion of local read operation by the progress engine.
 *
 *    Otherwise, this function calls efa_rdm_pke_copy_payload_to_hmem(), which will batch
 *    multiple copies, and perform the copy (then call efa_rdm_pke_handle_data_copied()) together
 *    to improve performance.
 *
 * When application's receive buffer is on host, data is copied immediately, and
 * efa_rdm_pke_handle_data_copied() is called.
 *
 *
 * @param[in]		pke	the packet entry that contains data
 * @param[in,out]	ope	ope contains information of the receive
 *                      	buffer.
 * @return		On success, return 0
 * 			On failure, return libfabric error code
 */
ssize_t efa_rdm_pke_copy_payload_to_ope(struct efa_rdm_pke *pke,
					struct efa_rdm_ope *ope)
{
	struct efa_mr *desc;
	struct efa_rdm_ep *ep;
	size_t segment_offset;
	ssize_t bytes_copied;

	ep = pke->ep;
	assert(ep);

	pke->ope = ope;
	segment_offset = efa_rdm_pke_get_segment_offset(pke);
	/*
	 * Under 3 rare situations, this function does not perform the copy
	 * action, but still consider data is copied:
	 *
	 * 1. application cancelled the receive, thus the receive buffer is not
	 *    available for copying to. In the case, this function is still
	 *    called because sender will keep sending data as receiver did not
	 *    notify the sender about the cancelation,
	 *
	 * 2. application's receiving buffer is smaller than incoming message size,
	 *    and data in the packet is outside of receiving buffer (truncated message).
	 *    In this case, this function is still called because sender will
	 *    keep sending data as receiver did not notify the sender about the
	 *    truncation.
	 *
	 * 3. message size is 0, thus no data to copy.
	 */
	if (OFI_UNLIKELY((ope->internal_flags & EFA_RDM_RXE_RECV_CANCEL) ||
	    (segment_offset >= ope->cq_entry.len) ||
	    (pke->payload_size == 0))) {
		efa_rdm_pke_handle_data_copied(pke);
		return 0;
	}

	desc = ope->desc[0];

	if (efa_mr_is_cuda(desc))
		return efa_rdm_pke_copy_payload_to_cuda(pke, ope);

	if (efa_mr_is_hmem(desc))
		return efa_rdm_pke_queued_copy_payload_to_hmem(pke, ope);

	assert( !desc || desc->peer.iface == FI_HMEM_SYSTEM);
	bytes_copied = ofi_copy_to_iov(ope->iov, ope->iov_count,
				       segment_offset + ep->msg_prefix_size,
				       pke->payload, pke->payload_size);

	if (bytes_copied != MIN(pke->payload_size, ope->cq_entry.len - segment_offset)) {
		EFA_WARN(FI_LOG_CQ, "wrong size! bytes_copied: %ld\n",
			bytes_copied);
		return -FI_EIO;
	}

	efa_rdm_pke_handle_data_copied(pke);
	return 0;
}

/**
 * @brief determine the payload offset of a received packet.
 *
 * For a packet receive overwire, the payload is in the wiredata,
 * immediately after header and optional user buffer information.
 * This function find the offset of payoff in respect of wiredata.
 *
 * @param[in]	pkt_entry	received paket entry
 * @return	an integer offset
 */
size_t efa_rdm_pke_get_payload_offset(struct efa_rdm_pke *pkt_entry)
{
	struct efa_rdm_base_hdr *base_hdr;
	int pkt_type, read_iov_count;
	size_t payload_offset;

	assert(pkt_entry->alloc_type != EFA_RDM_PKE_FROM_EFA_TX_POOL);

	/* packet entry from read copy pool contains only payload
	 * in its wire data (no header, no optional user buffer
	 * information)
	 */
	if (pkt_entry->alloc_type == EFA_RDM_PKE_FROM_READ_COPY_POOL)
		return 0;

	base_hdr = efa_rdm_pke_get_base_hdr(pkt_entry);
	pkt_type = base_hdr->type;
	assert(efa_rdm_pkt_type_contains_data(pkt_type));
	if (efa_rdm_pkt_type_is_req(pkt_type)) {
		payload_offset = efa_rdm_pke_get_req_hdr_size(pkt_entry);
		assert(payload_offset > 0);

		if (pkt_type == EFA_RDM_RUNTREAD_MSGRTM_PKT ||
		    pkt_type == EFA_RDM_RUNTREAD_TAGRTM_PKT) {
			read_iov_count = efa_rdm_pke_get_runtread_rtm_base_hdr(pkt_entry)->read_iov_count;
			payload_offset +=  read_iov_count * sizeof(struct fi_rma_iov);
		}

		return payload_offset;
	}

	if (pkt_type == EFA_RDM_CTSDATA_PKT) {
		payload_offset = sizeof(struct efa_rdm_ctsdata_hdr);
		if (base_hdr->flags & EFA_RDM_PKT_CONNID_HDR)
			payload_offset += sizeof(struct efa_rdm_ctsdata_opt_connid_hdr);
		return payload_offset;
	}

	if (pkt_type == EFA_RDM_READRSP_PKT)
		return sizeof(struct efa_rdm_readrsp_hdr);

	if (pkt_type == EFA_RDM_ATOMRSP_PKT)
		return sizeof(struct efa_rdm_atomrsp_hdr);

	/* all packet types that can contain data has been processed.
	 * we should never reach here;
	 */
	assert(0);
	return -1;
}

/**
 * @brief return the optional connid header pointer in a packet
 *
 * @param[in]	pkt_entry	an packet entry
 * @return	If the input has the optional connid header, return the pointer to connid header
 * 		Otherwise, return NULL
 */
uint32_t *efa_rdm_pke_connid_ptr(struct efa_rdm_pke *pkt_entry)
{
	struct efa_rdm_base_hdr *base_hdr;

	base_hdr = efa_rdm_pke_get_base_hdr(pkt_entry);

	if (base_hdr->type >= EFA_RDM_REQ_PKT_BEGIN)
		return efa_rdm_pke_get_req_connid_ptr(pkt_entry);

	if (!(base_hdr->flags & EFA_RDM_PKT_CONNID_HDR))
		return NULL;

	switch (base_hdr->type) {
	case EFA_RDM_CTS_PKT:
		return &(efa_rdm_pke_get_cts_hdr(pkt_entry)->connid);

	case EFA_RDM_RECEIPT_PKT:
		return &(efa_rdm_pke_get_receipt_hdr(pkt_entry)->connid);

	case EFA_RDM_CTSDATA_PKT:
		return &(efa_rdm_pke_get_ctsdata_hdr(pkt_entry)->connid_hdr->connid);

	case EFA_RDM_READRSP_PKT:
		return &(efa_rdm_pke_get_readrsp_hdr(pkt_entry)->connid);

	case EFA_RDM_ATOMRSP_PKT:
		return &(efa_rdm_pke_get_atomrsp_hdr(pkt_entry)->connid);

	case EFA_RDM_EOR_PKT:
		return &efa_rdm_pke_get_eor_hdr(pkt_entry)->connid;

	case EFA_RDM_READ_NACK_PKT:
		return &efa_rdm_pke_get_read_nack_hdr(pkt_entry)->connid;

	case EFA_RDM_HANDSHAKE_PKT:
		return &(efa_rdm_pke_get_handshake_opt_connid_hdr(pkt_entry)->connid);

	default:
		EFA_WARN(FI_LOG_CQ, "unknown packet type: %d\n", base_hdr->type);
		assert(0 && "Unknown packet type");
	}

	return NULL;
}
