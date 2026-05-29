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
#include "efa_rdm_protocol.h"
#include "efa_rdm_pke_req.h"

void efa_rdm_pke_init_rtr_common(struct efa_rdm_pke *pkt_entry,
				 int pkt_type,
				 struct efa_rdm_ope *txe,
				 int window)
{
	struct efa_rdm_rtr_hdr *rtr_hdr;
	int i;

	assert(txe->op == ofi_op_read_req);
	rtr_hdr = (struct efa_rdm_rtr_hdr *)pkt_entry->wiredata;
	rtr_hdr->rma_iov_count = txe->rma_iov_count;
	efa_rdm_pke_init_req_hdr_common(pkt_entry, pkt_type, txe);
	rtr_hdr->msg_length = txe->total_len;
	rtr_hdr->recv_id = txe->tx_id;
	rtr_hdr->recv_length = window;
	for (i = 0; i < txe->rma_iov_count; ++i) {
		rtr_hdr->rma_iov[i].addr = txe->rma_iov[i].addr;
		rtr_hdr->rma_iov[i].len = txe->rma_iov[i].len;
		rtr_hdr->rma_iov[i].key = txe->rma_iov[i].key;
	}

	pkt_entry->pkt_size = efa_rdm_pke_get_req_hdr_size(pkt_entry);
	pkt_entry->ope = txe;
	pkt_entry->peer = txe->peer;
}

/**
 * @brief initialize a EFA_RDM_SHORT_RTR_PKT
 *
 * @param[in]		pkt_entry	packet entry to be initialized
 *
*/
ssize_t efa_rdm_pke_init_short_rtr(struct efa_rdm_pke *pkt_entry,
				   struct efa_rdm_ope *txe)
{
	efa_rdm_pke_init_rtr_common(pkt_entry,
				    EFA_RDM_SHORT_RTR_PKT,
				    txe,
				    txe->total_len);
	return 0;
}

ssize_t efa_rdm_pke_init_longcts_rtr(struct efa_rdm_pke *pkt_entry,
				     struct efa_rdm_ope *txe)
{
	efa_rdm_pke_init_rtr_common(pkt_entry,
				    EFA_RDM_LONGCTS_RTR_PKT,
				    txe,
				    txe->window);
	return 0;
}

/**
 * @brief allcoate an RX entry for a incoming RTR packet
 *
 * The RX entry will be allocated from endpoint's OP entry
 * pool
 * @param[in]	pkt_entry	received RTR packet
 *
 * @return
 * pointer to the newly allocated RX entry.
 * NULL when OP entry pool has been exhausted.
 */
struct efa_rdm_ope *efa_rdm_pke_alloc_rtr_rxe(struct efa_rdm_pke *pkt_entry)
{
	struct efa_rdm_ep *ep = pkt_entry->ep;
	struct efa_rdm_ope *rxe;
	struct efa_rdm_rtr_hdr *rtr_hdr;

	rxe = efa_rdm_ep_alloc_rxe(ep, pkt_entry->peer, ofi_op_read_rsp);
	if (OFI_UNLIKELY(!rxe))
		return NULL;

	rtr_hdr = (struct efa_rdm_rtr_hdr *)pkt_entry->wiredata;
	rxe->tx_id = rtr_hdr->recv_id;
	rxe->window = rtr_hdr->recv_length;
	rxe->iov_count = rtr_hdr->rma_iov_count;
	rxe->internal_flags |= EFA_RDM_OPE_INTERNAL;

	return rxe;
}

/**
 * @brief process an incoming RTR packet
 *
 * This functions works for both EFA_RDM_SHORT_RTR_PKT and EFA_RDM_LONGCTS_RTR_PKT
 * @param[in]		pkt_entry	packet entry
 */
void efa_rdm_pke_handle_rtr_recv(struct efa_rdm_pke *pkt_entry)
{
	struct efa_rdm_ep *ep;
	struct efa_rdm_rtr_hdr *rtr_hdr;
	struct efa_rdm_ope *rxe;
	ssize_t err;

	ep = pkt_entry->ep;

	rxe = efa_rdm_pke_alloc_rtr_rxe(pkt_entry);
	if (OFI_UNLIKELY(!rxe)) {
		EFA_WARN(FI_LOG_CQ,
			"RX entries exhausted.\n");
		efa_base_ep_write_eq_error(&ep->base_ep, FI_ENOBUFS, FI_EFA_ERR_RXE_POOL_EXHAUSTED);
		efa_rdm_pke_release_rx(pkt_entry);
		return;
	}

	rtr_hdr = (struct efa_rdm_rtr_hdr *)pkt_entry->wiredata;
	err = efa_rdm_rma_verified_copy_iov(ep, rtr_hdr->rma_iov, rtr_hdr->rma_iov_count,
					FI_REMOTE_READ, rxe->iov, rxe->desc);
	if (OFI_UNLIKELY(err)) {
		EFA_WARN(FI_LOG_CQ, "RMA address verification failed!\n");
		efa_base_ep_write_eq_error(&ep->base_ep, err, FI_EFA_ERR_RMA_ADDR);
		efa_rdm_rxe_release(rxe);
		efa_rdm_pke_release_rx(pkt_entry);
		return;
	}

	rxe->cq_entry.flags |= (FI_RMA | FI_READ);
	rxe->cq_entry.len = ofi_total_iov_len(rxe->iov, rxe->iov_count);
	rxe->cq_entry.buf = rxe->iov[0].iov_base;
	rxe->total_len = rxe->cq_entry.len;

	err = efa_rdm_ope_post_send_or_queue(rxe, EFA_RDM_READRSP_PKT);
	if (OFI_UNLIKELY(err)) {
		EFA_WARN(FI_LOG_CQ, "Posting of readrsp packet failed! err=%ld\n", err);
		efa_base_ep_write_eq_error(&ep->base_ep, err, FI_EFA_ERR_PKT_POST);
		efa_rdm_rxe_release(rxe);
		efa_rdm_pke_release_rx(pkt_entry);
		return;
	}

	efa_rdm_pke_release_rx(pkt_entry);
}
