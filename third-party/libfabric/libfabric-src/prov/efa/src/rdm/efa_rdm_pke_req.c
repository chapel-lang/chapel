/* SPDX-License-Identifier: BSD-2-Clause OR GPL-2.0-only */
/* SPDX-FileCopyrightText: Copyright Amazon.com, Inc. or its affiliates. All rights reserved. */

#include <assert.h>
#include <ofi_atomic.h>
#include "efa.h"
#include "efa_rdm_protocol.h"
#include "efa_rdm_pke_req.h"
#include "efa_rdm_pke_rta.h"
#include "efa_rdm_pke_rtr.h"
#include "efa_rdm_pke_rtw.h"
#include "efa_rdm_pke_utils.h"
#include "efa_rdm_pkt_type.h"

/**
 * @brief initialize the packet header of a REQ packet entry
 *
 * This function init the common part of REQ packet entries,
 * specifically, it initialize the "efa_rdm_base_hdr"
 * and the 3 optional header of REQ packets:
 * raw address header, cq data header and connid header
 */
void efa_rdm_pke_init_req_hdr_common(struct efa_rdm_pke *pkt_entry,
				     int pkt_type,
				     struct efa_rdm_ope *txe)
{
	char *opt_hdr;
	struct efa_rdm_ep *ep;
	struct efa_rdm_base_hdr *base_hdr;

	/* init the base header */
	base_hdr = efa_rdm_pke_get_base_hdr(pkt_entry);
	base_hdr->type = pkt_type;
	base_hdr->version = EFA_RDM_PROTOCOL_VERSION;
	base_hdr->flags = 0;

	ep = txe->ep;

	if (efa_rdm_peer_need_raw_addr_hdr(txe->peer)) {
		/*
		 * This is the first communication with this peer on this
		 * endpoint, so send the core's address for this EP in the REQ
		 * so the remote side can insert it into its address vector.
		 */
		base_hdr->flags |= EFA_RDM_REQ_OPT_RAW_ADDR_HDR;
	} else if (efa_rdm_peer_need_connid(txe->peer)) {
		/*
		 * After receiving handshake packet, we will know the peer's capability.
		 *
		 * If the peer need connid, we will include the optional connid
		 * header in the req packet header.The peer will use it
		 * to verify my identity.
		 *
		 * This logic means that a req packet cannot have both
		 * the optional raw address header and the optional connid header.
		 */
		base_hdr->flags |= EFA_RDM_PKT_CONNID_HDR;
	}

	if (txe->fi_flags & FI_REMOTE_CQ_DATA) {
		base_hdr->flags |= EFA_RDM_REQ_OPT_CQ_DATA_HDR;
	}

	/* init the opt header */
	opt_hdr = (char *)base_hdr + efa_rdm_pke_get_req_base_hdr_size(pkt_entry);
	if (base_hdr->flags & EFA_RDM_REQ_OPT_RAW_ADDR_HDR) {
		struct efa_rdm_req_opt_raw_addr_hdr *raw_addr_hdr;

		raw_addr_hdr = (struct efa_rdm_req_opt_raw_addr_hdr *)opt_hdr;
		raw_addr_hdr->addr_len = EFA_RDM_REQ_OPT_RAW_ADDR_HDR_SIZE - sizeof(struct efa_rdm_req_opt_raw_addr_hdr);
		assert(raw_addr_hdr->addr_len >= sizeof(ep->base_ep.src_addr));
		memcpy(raw_addr_hdr->raw_addr, &ep->base_ep.src_addr, sizeof(ep->base_ep.src_addr));
		opt_hdr += EFA_RDM_REQ_OPT_RAW_ADDR_HDR_SIZE;
	}

	if (base_hdr->flags & EFA_RDM_REQ_OPT_CQ_DATA_HDR) {
		struct efa_rdm_req_opt_cq_data_hdr *cq_data_hdr;

		cq_data_hdr = (struct efa_rdm_req_opt_cq_data_hdr *)opt_hdr;
		cq_data_hdr->cq_data = txe->cq_entry.data;
		opt_hdr += sizeof(*cq_data_hdr);
	}

	if (base_hdr->flags & EFA_RDM_PKT_CONNID_HDR) {
		struct efa_rdm_req_opt_connid_hdr *connid_hdr;

		connid_hdr = (struct efa_rdm_req_opt_connid_hdr *)opt_hdr;
		connid_hdr->connid = efa_rdm_ep_raw_addr(ep)->qkey;
		opt_hdr += sizeof(*connid_hdr);
	}

	pkt_entry->peer = txe->peer;
	assert(opt_hdr - pkt_entry->wiredata == efa_rdm_pke_get_req_hdr_size(pkt_entry));
}

/**
 * @brief return the optional raw addr header pointer in a req packet
 *
 * @param[in]	pkt_entry	an REQ packet entry
 * @return	If the input has the optional raw addres header, return the pointer to it.
 *		Otherwise, return NULL
 */
void *efa_rdm_pke_get_req_raw_addr(struct efa_rdm_pke *pkt_entry)
{
	char *opt_hdr;
	struct efa_rdm_base_hdr *base_hdr;
	struct efa_rdm_req_opt_raw_addr_hdr *raw_addr_hdr;

	base_hdr = efa_rdm_pke_get_base_hdr(pkt_entry);
	opt_hdr = pkt_entry->wiredata + efa_rdm_pke_get_req_base_hdr_size(pkt_entry);
	if (base_hdr->flags & EFA_RDM_REQ_OPT_RAW_ADDR_HDR) {
		/* For req packet, the optional connid header and the optional
		 * raw address header are mutually exclusive.
		 */
		assert(!(base_hdr->flags & EFA_RDM_PKT_CONNID_HDR));
		raw_addr_hdr = (struct efa_rdm_req_opt_raw_addr_hdr *)opt_hdr;
		return raw_addr_hdr->raw_addr;
	}

	return NULL;
}

/**
 * @brief return the pointer to connid in a req packet
 *
 * @param[in]	pkt_entry	an REQ packet entry
 * @return	If the input has the optional connid header, return the pointer to connid
 * 		Otherwise, return NULL
 */
uint32_t *efa_rdm_pke_get_req_connid_ptr(struct efa_rdm_pke *pkt_entry)
{
	char *opt_hdr;
	struct efa_rdm_base_hdr *base_hdr;
	struct efa_rdm_req_opt_connid_hdr *connid_hdr;

	base_hdr = efa_rdm_pke_get_base_hdr(pkt_entry);
	opt_hdr = pkt_entry->wiredata + efa_rdm_pke_get_req_base_hdr_size(pkt_entry);

	if (base_hdr->flags & EFA_RDM_REQ_OPT_RAW_ADDR_HDR) {
		struct efa_rdm_req_opt_raw_addr_hdr *raw_addr_hdr;
		struct efa_ep_addr *raw_addr;

		raw_addr_hdr = (struct efa_rdm_req_opt_raw_addr_hdr *)opt_hdr;
		raw_addr = (struct efa_ep_addr *)raw_addr_hdr->raw_addr;
		return &raw_addr->qkey;
	}

	if (base_hdr->flags & EFA_RDM_REQ_OPT_CQ_DATA_HDR)
		opt_hdr += sizeof(struct efa_rdm_req_opt_cq_data_hdr);

	if (base_hdr->flags & EFA_RDM_PKT_CONNID_HDR) {
		connid_hdr = (struct efa_rdm_req_opt_connid_hdr *)opt_hdr;
		return &connid_hdr->connid;
	}

	return NULL;
}

/**
 * @brief get the value of the CQ data in packet header
 *
 * CQ data is present when application used fi_senddata,
 * fi_injectdata or fi_writedata. The CQ data was included
 * in the packet header of REQ packet.
 *
 * @param[in]	pkt_entry	REQ packet entry
 * @return
 * an integer
 */
uint64_t efa_rdm_pke_get_req_cq_data(struct efa_rdm_pke *pkt_entry)
{
	char *opt_hdr;
	struct efa_rdm_base_hdr *base_hdr;
	struct efa_rdm_req_opt_cq_data_hdr *cq_data_hdr;
	struct efa_rdm_req_opt_raw_addr_hdr *raw_addr_hdr;

	base_hdr = efa_rdm_pke_get_base_hdr(pkt_entry);
	opt_hdr = pkt_entry->wiredata + efa_rdm_pke_get_req_base_hdr_size(pkt_entry);
	if (base_hdr->flags & EFA_RDM_REQ_OPT_RAW_ADDR_HDR) {
		raw_addr_hdr = (struct efa_rdm_req_opt_raw_addr_hdr *)opt_hdr;
		opt_hdr += sizeof(struct efa_rdm_req_opt_raw_addr_hdr) + raw_addr_hdr->addr_len;
	}

	assert(base_hdr->flags & EFA_RDM_REQ_OPT_CQ_DATA_HDR);
	cq_data_hdr = (struct efa_rdm_req_opt_cq_data_hdr *)opt_hdr;
	return cq_data_hdr->cq_data;
}

/**
 * @brief Get the value of "rma_iov_count" in a REQ packet's header.
 *
 * @param[in] 	pkt_entry 	the REQ pkt entry
 * @return
 * The rma_iov_count in the pkt header, if it is part of the header.
 * Otherwise return 0.
 */
static inline
uint32_t efa_rdm_pke_get_req_rma_iov_count(struct efa_rdm_pke *pkt_entry)
{
	int pkt_type = efa_rdm_pke_get_base_hdr(pkt_entry)->type;

	if (pkt_type == EFA_RDM_EAGER_RTW_PKT ||
	    pkt_type == EFA_RDM_DC_EAGER_RTW_PKT ||
	    pkt_type == EFA_RDM_LONGCTS_RTW_PKT ||
	    pkt_type == EFA_RDM_DC_LONGCTS_RTW_PKT ||
	    pkt_type == EFA_RDM_LONGREAD_RTW_PKT)
		return efa_rdm_pke_get_rtw_base_hdr(pkt_entry)->rma_iov_count;

	if (pkt_type == EFA_RDM_SHORT_RTR_PKT ||
		 pkt_type == EFA_RDM_LONGCTS_RTR_PKT)
		return efa_rdm_pke_get_rtr_hdr(pkt_entry)->rma_iov_count;

	if (pkt_type == EFA_RDM_WRITE_RTA_PKT ||
		 pkt_type == EFA_RDM_DC_WRITE_RTA_PKT ||
		 pkt_type == EFA_RDM_FETCH_RTA_PKT ||
		 pkt_type == EFA_RDM_COMPARE_RTA_PKT)
		return efa_rdm_pke_get_rta_hdr(pkt_entry)->rma_iov_count;

	return 0;
}

/**
 * @brief get the base header size of a REQ packet
 *
 * @return
 * a integer that is > 0.
 */
size_t efa_rdm_pke_get_req_base_hdr_size(struct efa_rdm_pke *pkt_entry)
{
	struct efa_rdm_base_hdr *base_hdr;
	uint32_t rma_iov_count;

	base_hdr = efa_rdm_pke_get_base_hdr(pkt_entry);
	assert(base_hdr->type >= EFA_RDM_REQ_PKT_BEGIN);

	rma_iov_count = efa_rdm_pke_get_req_rma_iov_count(pkt_entry);
	return EFA_RDM_PKT_TYPE_REQ_INFO_VEC[base_hdr->type].base_hdr_size +
	       rma_iov_count * sizeof(struct fi_rma_iov);
}

/**
 * @brief calculate the exact header size for a given REQ pkt_entry
 *
 * The difference between this function and efa_rdm_pkt_type_req_hdr_size() is
 * the handling of the size of req opt raw address header.
 *
 * efa_rdm_pke_get_req_hdr_size() always use EFA_RDM_REQ_OPT_RAW_ADDR_HDR_SIZE, while
 * this function pull raw address from pkt_entry's size.
 *
 * The difference is because older version of libfabric EFA provider has
 * a different optional raw address header size.
 *
 * @param[in]	pkt_entry		packet entry
 * @return 	header size of the REQ packet entry
 */
size_t efa_rdm_pke_get_req_hdr_size(struct efa_rdm_pke *pkt_entry)
{
	char *opt_hdr;
	struct efa_rdm_base_hdr *base_hdr;
	struct efa_rdm_req_opt_raw_addr_hdr *raw_addr_hdr;

	base_hdr = efa_rdm_pke_get_base_hdr(pkt_entry);
	opt_hdr = pkt_entry->wiredata + efa_rdm_pke_get_req_base_hdr_size(pkt_entry);

	/*
	 * It is not possible to have both optional raw addr header and optional
	 * connid header in a packet header.
	 */
	if (base_hdr->flags & EFA_RDM_REQ_OPT_RAW_ADDR_HDR) {
		assert(!(base_hdr->flags & EFA_RDM_PKT_CONNID_HDR));
		raw_addr_hdr = (struct efa_rdm_req_opt_raw_addr_hdr *)opt_hdr;
		opt_hdr += sizeof(struct efa_rdm_req_opt_raw_addr_hdr) + raw_addr_hdr->addr_len;
	}

	if (base_hdr->flags & EFA_RDM_REQ_OPT_CQ_DATA_HDR)
		opt_hdr += sizeof(struct efa_rdm_req_opt_cq_data_hdr);

	if (base_hdr->flags & EFA_RDM_PKT_CONNID_HDR) {
		assert(!(base_hdr->flags & EFA_RDM_REQ_OPT_RAW_ADDR_HDR));
		opt_hdr += sizeof(struct efa_rdm_req_opt_connid_hdr);
	}

	return opt_hdr - pkt_entry->wiredata;
}
