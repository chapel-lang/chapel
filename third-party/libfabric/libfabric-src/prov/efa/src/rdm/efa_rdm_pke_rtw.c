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
#include "efa_rdm_rma.h"
#include "efa_rdm_ope.h"
#include "efa_rdm_pke.h"
#include "efa_rdm_pke_utils.h"
#include "efa_rdm_protocol.h"
#include "efa_rdm_pke_req.h"

/**
 * @brief initialize the payload and rma_iov of a RTW packet
 *
 * Used by EAGER and LONGCTS RTW.
 * @param[in,out]	pkt_entry	RTW packet entry
 * @param[in]		txe		TX entry that has RMA write information
 * @param[in]		rma_iov		the "rma_iov" field in RTW packet header
 *
 * @returns
 * 0 on success
 * negative libfabric error code on error.
 */
static inline
ssize_t efa_rdm_pke_init_rtw_common(struct efa_rdm_pke *pkt_entry,
				    struct efa_rdm_ope *txe,
				    struct efa_rma_iov *rma_iov)
{
	size_t hdr_size;
	size_t data_size;
	int i;

	for (i = 0; i < txe->rma_iov_count; ++i) {
		rma_iov[i].addr = txe->rma_iov[i].addr;
		rma_iov[i].len = txe->rma_iov[i].len;
		rma_iov[i].key = txe->rma_iov[i].key;
	}

	hdr_size = efa_rdm_pke_get_req_hdr_size(pkt_entry);
	data_size = MIN(txe->ep->mtu_size - hdr_size, txe->total_len);
	return efa_rdm_pke_init_payload_from_ope(pkt_entry, txe, hdr_size, 0, data_size);
}

/**
 * @brief allcoate an RX entry for a incoming RTW packet
 *
 * The RX entry will be allocated from endpoint's OP entry
 * pool
 * @param[in]	pkt_entry	received RTW packet
 *
 * @return
 * pointer to the newly allocated RX entry.
 * NULL when OP entry pool has been exhausted.
 */
static
struct efa_rdm_ope *efa_rdm_pke_alloc_rtw_rxe(struct efa_rdm_pke *pkt_entry)
{
	struct efa_rdm_ope *rxe;
	struct efa_rdm_base_hdr *base_hdr;

	rxe = efa_rdm_ep_alloc_rxe(pkt_entry->ep, pkt_entry->addr, ofi_op_write);
	if (OFI_UNLIKELY(!rxe))
		return NULL;

	base_hdr = efa_rdm_pke_get_base_hdr(pkt_entry);
	if (base_hdr->flags & EFA_RDM_REQ_OPT_CQ_DATA_HDR) {
		rxe->cq_entry.flags |= FI_REMOTE_CQ_DATA;
		rxe->cq_entry.data = efa_rdm_pke_get_req_cq_data(pkt_entry);
	}

	rxe->addr = pkt_entry->addr;
	rxe->bytes_received = 0;
	rxe->bytes_copied = 0;
	return rxe;
}

/**
 * @brief initialize a EFA_RDM_EAGER_RTW packet
 *
 * @param[in,out]	pkt_entry	packet entry to be initialized
 * @param[in]		txe		TX entry that has RMA write information
 *
 * @returns
 * 0 on success
 * negative libfabric error code on failure
 */
ssize_t efa_rdm_pke_init_eager_rtw(struct efa_rdm_pke *pkt_entry,
				   struct efa_rdm_ope *txe)
{
	struct efa_rdm_eager_rtw_hdr *rtw_hdr;

	assert(txe->op == ofi_op_write);

	rtw_hdr = (struct efa_rdm_eager_rtw_hdr *)pkt_entry->wiredata;
	rtw_hdr->rma_iov_count = txe->rma_iov_count;
	efa_rdm_pke_init_req_hdr_common(pkt_entry, EFA_RDM_EAGER_RTW_PKT, txe);
	return efa_rdm_pke_init_rtw_common(pkt_entry, txe, rtw_hdr->rma_iov);
}

/**
 * @brief handle the send completion event of an EAGER RTW packet
 *
 * This function apply to both EFA_RDM_EAGER_RTW_PKT and
 * EFA_RDM_DC_EAGER_RTW_PKT
 *
 * @param[in]		pkt_entry	packet entry
 */
void efa_rdm_pke_handle_eager_rtw_send_completion(struct efa_rdm_pke *pkt_entry)
{
	struct efa_rdm_ope *txe;

	txe = pkt_entry->ope;
	assert(txe->total_len == pkt_entry->payload_size);
	efa_rdm_ope_handle_send_completed(txe);
}

/**
 * @brief process an received EAGER RTW packet
 *
 * This function apply to both EFA_RDM_EAGER_RTW_PKT and
 * EFA_RDM_DC_EAGER_RTW_PKT
 *
 * @param[in]		pkt_entry	received EAGER RTW packet entry
 * @param[in,out]	rxe		RX entry
 * @param[in]		rma_iov		rma_iov array in RTW header
 * @param[in]		rma_iov_count	number of elements in rma_iov
 */
void efa_rdm_pke_proc_eager_rtw(struct efa_rdm_pke *pkt_entry,
				struct efa_rdm_ope *rxe,
				struct efa_rma_iov *rma_iov,
				size_t rma_iov_count)
{
	ssize_t err;
	struct efa_rdm_ep *ep;

	ep = pkt_entry->ep;

	err = efa_rdm_rma_verified_copy_iov(ep, rma_iov, rma_iov_count,
					FI_REMOTE_WRITE, rxe->iov, rxe->desc);

	if (OFI_UNLIKELY(err)) {
		EFA_WARN(FI_LOG_CQ, "RMA address verify failed!\n");
		efa_base_ep_write_eq_error(&ep->base_ep, err, FI_EFA_ERR_RMA_ADDR);
		efa_rdm_rxe_release(rxe);
		efa_rdm_pke_release_rx(pkt_entry);
		return;
	}

	rxe->cq_entry.len = ofi_total_iov_len(rxe->iov, rxe->iov_count);
	rxe->cq_entry.buf = rxe->iov[0].iov_base;
	rxe->total_len = rxe->cq_entry.len;

	rxe->bytes_received += pkt_entry->payload_size;
	if (pkt_entry->payload_size != rxe->total_len) {
		EFA_WARN(FI_LOG_CQ, "Eager RTM size mismatch! payload_size: %ld total_len: %ld.\n",
			 pkt_entry->payload_size, rxe->total_len);
		EFA_WARN(FI_LOG_CQ, "target buffer: %p length: %ld\n", rxe->iov[0].iov_base,
			rxe->iov[0].iov_len);
		efa_base_ep_write_eq_error(&ep->base_ep, FI_EINVAL, FI_EFA_ERR_RTM_MISMATCH);
		efa_rdm_pke_release_rx(pkt_entry);
		efa_rdm_rxe_release(rxe);
	} else {
		err = efa_rdm_pke_copy_payload_to_ope(pkt_entry, rxe);
		if (OFI_UNLIKELY(err)) {
			efa_base_ep_write_eq_error(&ep->base_ep, err, FI_EFA_ERR_RXE_COPY);
			efa_rdm_pke_release_rx(pkt_entry);
			efa_rdm_rxe_release(rxe);
		}
	}
}

/**
 * @brief handle the event that an EFA_RDM_EAGER_RTW packet has been received
 *
 * Calls #efa_rdm_pke_proc_eager_rtw()
 *
 * @param[in,out]	pkt_entry	received EFA_RDM_EAGER_RTW packet
 *
 */
void efa_rdm_pke_handle_eager_rtw_recv(struct efa_rdm_pke *pkt_entry)
{
	struct efa_rdm_ep *ep;
	struct efa_rdm_ope *rxe;
	struct efa_rdm_eager_rtw_hdr *rtw_hdr;

	ep = pkt_entry->ep;
	rxe = efa_rdm_pke_alloc_rtw_rxe(pkt_entry);

	if (!rxe) {
		EFA_WARN(FI_LOG_CQ,
			"RX entries exhausted.\n");
		efa_base_ep_write_eq_error(&ep->base_ep, FI_ENOBUFS, FI_EFA_ERR_RXE_POOL_EXHAUSTED);
		efa_rdm_pke_release_rx(pkt_entry);
		return;
	}

	rtw_hdr = (struct efa_rdm_eager_rtw_hdr *)pkt_entry->wiredata;
	rxe->iov_count = rtw_hdr->rma_iov_count;
	efa_rdm_pke_proc_eager_rtw(pkt_entry,
				   rxe,
				   rtw_hdr->rma_iov,
				   rtw_hdr->rma_iov_count);
}

/**
 * @brief initialize a EFA_RDM_DC_EAGER_RTW_PKT packet
 *
 * DC means delivery complete
 *
 * @param[in,out]	pkt_entry	packet entry to be initialized
 * @param[in]		txe		TX entry that has RMA write information
 * @returns
 * 0 on success.
 * negative libfabric error code on failure
 */
ssize_t efa_rdm_pke_init_dc_eager_rtw(struct efa_rdm_pke *pkt_entry,
				      struct efa_rdm_ope *txe)
{
	struct efa_rdm_dc_eager_rtw_hdr *dc_eager_rtw_hdr;
	int ret;

	assert(txe->op == ofi_op_write);

	txe->internal_flags |= EFA_RDM_TXE_DELIVERY_COMPLETE_REQUESTED;
	dc_eager_rtw_hdr = (struct efa_rdm_dc_eager_rtw_hdr *)pkt_entry->wiredata;
	dc_eager_rtw_hdr->rma_iov_count = txe->rma_iov_count;
	efa_rdm_pke_init_req_hdr_common(pkt_entry, EFA_RDM_DC_EAGER_RTW_PKT, txe);
	ret = efa_rdm_pke_init_rtw_common(pkt_entry, txe,
					  dc_eager_rtw_hdr->rma_iov);
	dc_eager_rtw_hdr->send_id = txe->tx_id;
	return ret;
}

/**
 * @brief handle the event that an EFA_RDM_DC_EAGER_RTW packet has been received
 *
 * DC means delivery complete
 * Calls #efa_rdm_pke_proc_eager_rtw()
 *
 * @param[in,out]	pkt_entry	received EFA_RDM_DC_EAGER_RTW packet
 *
 */
void efa_rdm_pke_handle_dc_eager_rtw_recv(struct efa_rdm_pke *pkt_entry)
{
	struct efa_rdm_ope *rxe;
	struct efa_rdm_dc_eager_rtw_hdr *rtw_hdr;

	rxe = efa_rdm_pke_alloc_rtw_rxe(pkt_entry);
	if (!rxe) {
		EFA_WARN(FI_LOG_CQ,
			"RX entries exhausted.\n");
		efa_base_ep_write_eq_error(&pkt_entry->ep->base_ep,
					   FI_ENOBUFS, FI_EFA_ERR_RXE_POOL_EXHAUSTED);
		efa_rdm_pke_release_rx(pkt_entry);
		return;
	}

	rxe->internal_flags |= EFA_RDM_TXE_DELIVERY_COMPLETE_REQUESTED;
	rtw_hdr = (struct efa_rdm_dc_eager_rtw_hdr *)pkt_entry->wiredata;
	rxe->tx_id = rtw_hdr->send_id;
	rxe->iov_count = rtw_hdr->rma_iov_count;
	efa_rdm_pke_proc_eager_rtw(pkt_entry,
				   rxe,
				   rtw_hdr->rma_iov,
				   rtw_hdr->rma_iov_count);
}

/**
 * @brief initialize the the header of a LONGCTS RTW packet
 *
 * This function applies to both EFA_RDM_LONGCTS_RTW_PKT and
 * EFA_RDM_DC_LONGCTS_RTW_PKT
 */
static inline
void efa_rdm_pke_init_longcts_rtw_hdr(struct efa_rdm_pke *pkt_entry,
				      int pkt_type,
				      struct efa_rdm_ope *txe)
{
	struct efa_rdm_longcts_rtw_hdr *rtw_hdr;

	rtw_hdr = (struct efa_rdm_longcts_rtw_hdr *)pkt_entry->wiredata;
	rtw_hdr->rma_iov_count = txe->rma_iov_count;
	rtw_hdr->msg_length = txe->total_len;
	rtw_hdr->send_id = txe->tx_id;
	rtw_hdr->credit_request = efa_env.tx_min_credits;
	efa_rdm_pke_init_req_hdr_common(pkt_entry, pkt_type, txe);
}

/**
 * @brief initialize a EFA_RDM_LONGCTS_RTW packet
 *
 *
 * @param[in,out]	pkt_entry	packet entry to be initialized
 * @param[in]		txe		TX entry that has RMA write information
 * @returns
 * 0 on success.
 * negative libfabric error code on failure
 */
ssize_t efa_rdm_pke_init_longcts_rtw(struct efa_rdm_pke *pkt_entry,
				     struct efa_rdm_ope *txe)
{
	struct efa_rdm_longcts_rtw_hdr *rtw_hdr;

	assert(txe->op == ofi_op_write);

	rtw_hdr = (struct efa_rdm_longcts_rtw_hdr *)pkt_entry->wiredata;
	efa_rdm_pke_init_longcts_rtw_hdr(pkt_entry, EFA_RDM_LONGCTS_RTW_PKT, txe);
	return efa_rdm_pke_init_rtw_common(pkt_entry, txe, rtw_hdr->rma_iov);
}

/**
 * @brief handle the event that a LONGCTS RTW packet has been sent
 *
 * Apply to both EFA_RDM_LONGCTS_RTW and EFA_RDM_DC_LONGCTS_RTW
 *
 * @param[in]	pkt_entry	LONGCTS RTW packet entry
 */
void efa_rdm_pke_handle_longcts_rtw_sent(struct efa_rdm_pke *pkt_entry)
{
	struct efa_rdm_ep *ep;
	struct efa_rdm_ope *txe;
	struct efa_domain *efa_domain;

	ep = pkt_entry->ep;
	efa_domain = efa_rdm_ep_domain(ep);
	txe = pkt_entry->ope;
	txe->bytes_sent += pkt_entry->payload_size;
	assert(txe->bytes_sent < txe->total_len);
	if (efa_is_cache_available(efa_domain))
		efa_rdm_ope_try_fill_desc(txe, 0, FI_SEND);
}

/**
 * @brief handle the "send completion" event of a LONGCTS RTW packet
 *
 * Apply to both EFA_RDM_LONGCTS_RTW and EFA_RDM_DC_LONGCTS_RTW
 *
 * @param[in]	pkt_entry	LONGCTS RTW packet entry
 */
void efa_rdm_pke_handle_longcts_rtw_send_completion(struct efa_rdm_pke *pkt_entry)
{
	struct efa_rdm_ope *txe;

	txe = pkt_entry->ope;
	txe->bytes_acked += pkt_entry->payload_size;
	if (txe->total_len == txe->bytes_acked)
		efa_rdm_ope_handle_send_completed(txe);
}

/**
 * @brief handle the event that a LONGCTS RTW packet has been received
 *
 * applies to both EFA_RDM_LONGCTS_RTW_PKT and EFA_RDM_DC_LONGCTS_RTW_PKT
 *
 * @param[in]	pkt_entry	received LONGCTS RTW paket entry
 */
void efa_rdm_pke_handle_longcts_rtw_recv(struct efa_rdm_pke *pkt_entry)
{
	struct efa_rdm_ep *ep;
	struct efa_rdm_ope *rxe;
	struct efa_rdm_longcts_rtw_hdr *rtw_hdr;
	ssize_t err;
	uint32_t tx_id;

	ep = pkt_entry->ep;
	rxe = efa_rdm_pke_alloc_rtw_rxe(pkt_entry);
	if (!rxe) {
		EFA_WARN(FI_LOG_CQ,
			"RX entries exhausted.\n");
		efa_base_ep_write_eq_error(&pkt_entry->ep->base_ep,
					   FI_ENOBUFS,
					   FI_EFA_ERR_RXE_POOL_EXHAUSTED);
		efa_rdm_pke_release_rx(pkt_entry);
		return;
	}

	rtw_hdr = (struct efa_rdm_longcts_rtw_hdr *)pkt_entry->wiredata;
	tx_id = rtw_hdr->send_id;
	if (rtw_hdr->type == EFA_RDM_DC_LONGCTS_RTW_PKT)
		rxe->internal_flags |= EFA_RDM_TXE_DELIVERY_COMPLETE_REQUESTED;

	rxe->iov_count = rtw_hdr->rma_iov_count;
	err = efa_rdm_rma_verified_copy_iov(ep, rtw_hdr->rma_iov, rtw_hdr->rma_iov_count,
					FI_REMOTE_WRITE, rxe->iov, rxe->desc);
	if (OFI_UNLIKELY(err)) {
		EFA_WARN(FI_LOG_CQ, "RMA address verify failed!\n");
		efa_base_ep_write_eq_error(&ep->base_ep, err, FI_EFA_ERR_RMA_ADDR);
		efa_rdm_rxe_release(rxe);
		efa_rdm_pke_release_rx(pkt_entry);
		return;
	}

	rxe->cq_entry.len = ofi_total_iov_len(rxe->iov, rxe->iov_count);
	rxe->cq_entry.buf = rxe->iov[0].iov_base;
	rxe->total_len = rxe->cq_entry.len;

	rxe->bytes_received += pkt_entry->payload_size;
	if (pkt_entry->payload_size >= rxe->total_len) {
		EFA_WARN(FI_LOG_CQ, "Long RTM size mismatch! payload_size: %ld total_len: %ld\n",
			 pkt_entry->payload_size, rxe->total_len);
		EFA_WARN(FI_LOG_CQ, "target buffer: %p length: %ld\n", rxe->iov[0].iov_base,
			rxe->iov[0].iov_len);
		efa_base_ep_write_eq_error(&ep->base_ep, FI_EINVAL, FI_EFA_ERR_RTM_MISMATCH);
		efa_rdm_rxe_release(rxe);
		efa_rdm_pke_release_rx(pkt_entry);
		return;
	} else {
		err = efa_rdm_pke_copy_payload_to_ope(pkt_entry, rxe);
		if (OFI_UNLIKELY(err)) {
			efa_base_ep_write_eq_error(&ep->base_ep, err, FI_EFA_ERR_RXE_COPY);
			efa_rdm_rxe_release(rxe);
			efa_rdm_pke_release_rx(pkt_entry);
			return;
		}
	}


#if ENABLE_DEBUG
	dlist_insert_tail(&rxe->pending_recv_entry, &ep->ope_recv_list);
	ep->pending_recv_counter++;
#endif
	rxe->state = EFA_RDM_RXE_RECV;
	rxe->tx_id = tx_id;
	err = efa_rdm_ope_post_send_or_queue(rxe, EFA_RDM_CTS_PKT);
	if (OFI_UNLIKELY(err)) {
		EFA_WARN(FI_LOG_CQ, "Cannot post CTS packet\n");
		efa_rdm_rxe_handle_error(rxe, -err, FI_EFA_ERR_PKT_POST);
		efa_rdm_rxe_release(rxe);
	}
}

/**
 * @brief initialize a EFA_RDM_DC_LONGCTS_RTW packet
 *
 * DC means delivery complete
 * @param[in,out]	pkt_entry	packet entry to be initialized
 * @param[in]		txe		TX entry that has RMA write information
 * @returns
 * 0 on success.
 * negative libfabric error code on failure
 */
ssize_t efa_rdm_pke_init_dc_longcts_rtw(struct efa_rdm_pke *pkt_entry,
					struct efa_rdm_ope *txe)
{
	struct efa_rdm_longcts_rtw_hdr *rtw_hdr;

	assert(txe->op == ofi_op_write);

	txe->internal_flags |= EFA_RDM_TXE_DELIVERY_COMPLETE_REQUESTED;
	rtw_hdr = (struct efa_rdm_longcts_rtw_hdr *)pkt_entry->wiredata;
	efa_rdm_pke_init_longcts_rtw_hdr(pkt_entry, EFA_RDM_DC_LONGCTS_RTW_PKT, txe);
	return efa_rdm_pke_init_rtw_common(pkt_entry, txe, rtw_hdr->rma_iov);
}

/**
 * @brief initialize a EFA_RDM_LONGREAD_RTA_RTW packet
 *
 * DC means delivery complete
 * @param[in,out]	pkt_entry	packet entry to be initialized
 * @param[in]		txe		TX entry that has RMA write information
 * @returns
 * 0 on success.
 * negative libfabric error code on failure
 */
ssize_t efa_rdm_pke_init_longread_rtw(struct efa_rdm_pke *pkt_entry,
				      struct efa_rdm_ope *txe)
{
	struct efa_rdm_longread_rtw_hdr *rtw_hdr;
	struct efa_rma_iov *rma_iov;
	struct fi_rma_iov *read_iov;
	size_t hdr_size;
	int i, err;

	assert(txe->op == ofi_op_write);

	rtw_hdr = (struct efa_rdm_longread_rtw_hdr *)pkt_entry->wiredata;
	rtw_hdr->rma_iov_count = txe->rma_iov_count;
	rtw_hdr->msg_length = txe->total_len;
	rtw_hdr->send_id = txe->tx_id;
	rtw_hdr->read_iov_count = txe->iov_count;
	efa_rdm_pke_init_req_hdr_common(pkt_entry, EFA_RDM_LONGREAD_RTW_PKT, txe);

	rma_iov = rtw_hdr->rma_iov;
	for (i = 0; i < txe->rma_iov_count; ++i) {
		rma_iov[i].addr = txe->rma_iov[i].addr;
		rma_iov[i].len = txe->rma_iov[i].len;
		rma_iov[i].key = txe->rma_iov[i].key;
	}

	hdr_size = efa_rdm_pke_get_req_hdr_size(pkt_entry);
	read_iov = (struct fi_rma_iov *)(pkt_entry->wiredata + hdr_size);
	err = efa_rdm_txe_prepare_to_be_read(txe, read_iov);
	if (OFI_UNLIKELY(err))
		return err;

	pkt_entry->pkt_size = hdr_size + txe->iov_count * sizeof(struct efa_rma_iov);
	pkt_entry->ope = txe;
	return 0;
}

/**
 * @brief handle the event that a EFA_RDM_LONGREAD_RTA_PKE has been received
 *
 * @param[in]		pkt_entry	received EFA_RDM_LONGREAD_RTA_PKT packet entry
 */
void efa_rdm_pke_handle_longread_rtw_recv(struct efa_rdm_pke *pkt_entry)
{
	struct efa_rdm_ep *ep;
	struct efa_rdm_ope *rxe;
	struct efa_rdm_longread_rtw_hdr *rtw_hdr;
	struct fi_rma_iov *read_iov;
	size_t hdr_size;
	ssize_t err;

	ep = pkt_entry->ep;
	rxe = efa_rdm_pke_alloc_rtw_rxe(pkt_entry);
	if (!rxe) {
		EFA_WARN(FI_LOG_CQ,
			"RX entries exhausted.\n");
		efa_base_ep_write_eq_error(&pkt_entry->ep->base_ep,
					   FI_ENOBUFS,
					   FI_EFA_ERR_RXE_POOL_EXHAUSTED);
		efa_rdm_pke_release_rx(pkt_entry);
		return;
	}

	rtw_hdr = (struct efa_rdm_longread_rtw_hdr *)pkt_entry->wiredata;
	rxe->iov_count = rtw_hdr->rma_iov_count;
	err = efa_rdm_rma_verified_copy_iov(pkt_entry->ep,
					    rtw_hdr->rma_iov,
					    rtw_hdr->rma_iov_count,
					    FI_REMOTE_WRITE, rxe->iov, rxe->desc);
	if (OFI_UNLIKELY(err)) {
		EFA_WARN(FI_LOG_CQ, "RMA address verify failed!\n");
		efa_base_ep_write_eq_error(&ep->base_ep, err, FI_EFA_ERR_RMA_ADDR);
		efa_rdm_rxe_release(rxe);
		efa_rdm_pke_release_rx(pkt_entry);
		return;
	}

	rxe->cq_entry.len = ofi_total_iov_len(rxe->iov, rxe->iov_count);
	rxe->cq_entry.buf = rxe->iov[0].iov_base;
	rxe->total_len = rxe->cq_entry.len;

	hdr_size = efa_rdm_pke_get_req_hdr_size(pkt_entry);
	read_iov = (struct fi_rma_iov *)(pkt_entry->wiredata + hdr_size);
	rxe->addr = pkt_entry->addr;
	rxe->tx_id = rtw_hdr->send_id;
	rxe->rma_iov_count = rtw_hdr->read_iov_count;
	memcpy(rxe->rma_iov, read_iov,
	       rxe->rma_iov_count * sizeof(struct fi_rma_iov));

	efa_rdm_pke_release_rx(pkt_entry);

	err = efa_rdm_ope_post_remote_read_or_queue(rxe);
	if (OFI_UNLIKELY(err)) {
		EFA_WARN(FI_LOG_CQ,
			"RDMA post read or queue failed.\n");
		efa_base_ep_write_eq_error(&ep->base_ep, err, FI_EFA_ERR_RDMA_READ_POST);
		efa_rdm_rxe_release(rxe);
		efa_rdm_pke_release_rx(pkt_entry);
	}
}
