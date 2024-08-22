/* SPDX-License-Identifier: BSD-2-Clause OR GPL-2.0-only */
/* SPDX-FileCopyrightText: Copyright Amazon.com, Inc. or its affiliates. All rights reserved. */

#include <stdint.h>
#include "efa_mr.h"
#include "efa_rdm_ope.h"
#include "efa_rdm_peer.h"
#include "efa_rdm_protocol.h"
#include "efa_rdm_pkt_type.h"
#include "efa_rdm_pke_nonreq.h"

struct efa_rdm_pkt_type_req_info EFA_RDM_PKT_TYPE_REQ_INFO_VEC[] = {
	/* rtm header */
	[EFA_RDM_EAGER_MSGRTM_PKT] = {0, sizeof(struct efa_rdm_eager_msgrtm_hdr), 0},
	[EFA_RDM_EAGER_TAGRTM_PKT] = {0, sizeof(struct efa_rdm_eager_tagrtm_hdr), 0},
	[EFA_RDM_MEDIUM_MSGRTM_PKT] = {0, sizeof(struct efa_rdm_medium_msgrtm_hdr), 0},
	[EFA_RDM_MEDIUM_TAGRTM_PKT] = {0, sizeof(struct efa_rdm_medium_tagrtm_hdr), 0},
	[EFA_RDM_LONGCTS_MSGRTM_PKT] = {0, sizeof(struct efa_rdm_longcts_msgrtm_hdr), 0},
	[EFA_RDM_LONGCTS_TAGRTM_PKT] = {0, sizeof(struct efa_rdm_longcts_tagrtm_hdr), 0},
	[EFA_RDM_LONGREAD_MSGRTM_PKT] = {0, sizeof(struct efa_rdm_longread_msgrtm_hdr), EFA_RDM_EXTRA_FEATURE_RDMA_READ},
	[EFA_RDM_LONGREAD_TAGRTM_PKT] = {0, sizeof(struct efa_rdm_longread_tagrtm_hdr), EFA_RDM_EXTRA_FEATURE_RDMA_READ},
	[EFA_RDM_DC_EAGER_MSGRTM_PKT] = {0, sizeof(struct efa_rdm_dc_eager_msgrtm_hdr), EFA_RDM_EXTRA_FEATURE_DELIVERY_COMPLETE},
	[EFA_RDM_DC_EAGER_TAGRTM_PKT] = {0, sizeof(struct efa_rdm_dc_eager_tagrtm_hdr), EFA_RDM_EXTRA_FEATURE_DELIVERY_COMPLETE},
	[EFA_RDM_DC_MEDIUM_MSGRTM_PKT] = {0, sizeof(struct efa_rdm_dc_medium_msgrtm_hdr), EFA_RDM_EXTRA_FEATURE_DELIVERY_COMPLETE},
	[EFA_RDM_DC_MEDIUM_TAGRTM_PKT] = {0, sizeof(struct efa_rdm_dc_medium_tagrtm_hdr), EFA_RDM_EXTRA_FEATURE_DELIVERY_COMPLETE},
	[EFA_RDM_DC_LONGCTS_MSGRTM_PKT] = {0, sizeof(struct efa_rdm_longcts_msgrtm_hdr), EFA_RDM_EXTRA_FEATURE_DELIVERY_COMPLETE},
	[EFA_RDM_DC_LONGCTS_TAGRTM_PKT] = {0, sizeof(struct efa_rdm_longcts_tagrtm_hdr), EFA_RDM_EXTRA_FEATURE_DELIVERY_COMPLETE},
	[EFA_RDM_RUNTCTS_MSGRTM_PKT] = {0, sizeof(struct efa_rdm_runtcts_msgrtm_hdr), EFA_RDM_EXTRA_FEATURE_RUNT},
	[EFA_RDM_RUNTCTS_TAGRTM_PKT] = {0, sizeof(struct efa_rdm_runtcts_tagrtm_hdr), EFA_RDM_EXTRA_FEATURE_RUNT},
	[EFA_RDM_RUNTREAD_MSGRTM_PKT] = {0, sizeof(struct efa_rdm_runtread_msgrtm_hdr), EFA_RDM_EXTRA_FEATURE_RUNT | EFA_RDM_EXTRA_FEATURE_RDMA_READ},
	[EFA_RDM_RUNTREAD_TAGRTM_PKT] = {0, sizeof(struct efa_rdm_runtread_tagrtm_hdr), EFA_RDM_EXTRA_FEATURE_RUNT | EFA_RDM_EXTRA_FEATURE_RDMA_READ},
	/* rtw header */
	[EFA_RDM_EAGER_RTW_PKT] = {0, sizeof(struct efa_rdm_eager_rtw_hdr), 0},
	[EFA_RDM_DC_EAGER_RTW_PKT] = {0, sizeof(struct efa_rdm_dc_eager_rtw_hdr), EFA_RDM_EXTRA_FEATURE_DELIVERY_COMPLETE},
	[EFA_RDM_LONGCTS_RTW_PKT] = {0, sizeof(struct efa_rdm_longcts_rtw_hdr), 0},
	[EFA_RDM_DC_LONGCTS_RTW_PKT] = {0, sizeof(struct efa_rdm_longcts_rtw_hdr), EFA_RDM_EXTRA_FEATURE_DELIVERY_COMPLETE},
	[EFA_RDM_LONGREAD_RTW_PKT] = {0, sizeof(struct efa_rdm_longread_rtw_hdr), EFA_RDM_EXTRA_FEATURE_RDMA_READ},
	[EFA_RDM_RUNTCTS_RTW_PKT] = {0, sizeof(struct efa_rdm_runtcts_rtw_hdr), EFA_RDM_EXTRA_FEATURE_RUNT},
	[EFA_RDM_RUNTREAD_RTW_PKT] = {0, sizeof(struct efa_rdm_runtread_rtw_hdr), EFA_RDM_EXTRA_FEATURE_RUNT},
	/* rtr header */
	[EFA_RDM_SHORT_RTR_PKT] = {0, sizeof(struct efa_rdm_rtr_hdr), 0},
	[EFA_RDM_LONGCTS_RTR_PKT] = {0, sizeof(struct efa_rdm_rtr_hdr), 0},
	[EFA_RDM_READ_RTR_PKT] = {0, sizeof(struct efa_rdm_base_hdr), EFA_RDM_EXTRA_FEATURE_RDMA_READ},
	/* rta header */
	[EFA_RDM_WRITE_RTA_PKT] = {0, sizeof(struct efa_rdm_rta_hdr), 0},
	[EFA_RDM_DC_WRITE_RTA_PKT] = {0, sizeof(struct efa_rdm_rta_hdr), EFA_RDM_EXTRA_FEATURE_DELIVERY_COMPLETE},
	[EFA_RDM_FETCH_RTA_PKT] = {0, sizeof(struct efa_rdm_rta_hdr), 0},
	[EFA_RDM_COMPARE_RTA_PKT] = {0, sizeof(struct efa_rdm_rta_hdr), 0},
};

/**
 * @brief determine whether a REQ packet type is supported by the peer
 *
 * Some REQ packet type rely on an extra feature, which may not be
 * supported by peer.
 *
 * It is caller's responsibility to ensure this function is
 * called after the handshake packet has been received from the peer.
 *
 * @param[in]		req_pkt_type	REQ packet type
 * @param[in]		peer		peer, from whose the handshake has been received.
 * @returns
 * a boolean
 */
bool efa_rdm_pkt_type_is_supported_by_peer(int req_pkt_type, struct efa_rdm_peer *peer)
{
	assert(peer->flags & EFA_RDM_PEER_HANDSHAKE_RECEIVED);

	int extra_info_id = EFA_RDM_PKT_TYPE_REQ_INFO_VEC[req_pkt_type].extra_info_id;

	return peer->extra_info[extra_info_id] & EFA_RDM_PKT_TYPE_REQ_INFO_VEC[req_pkt_type].ex_feature_flag;
}

/**
 * @brief calculates the exact header size given a REQ packet type, flags, and IOV count.
 *
 * @param[in]	pkt_type	packet type
 * @param[in]	flags	flags from packet
 * @param[in]	rma_iov_count	number of RMA IOV structures present
 * @return	The exact size of the packet header
 */
size_t efa_rdm_pkt_type_get_req_hdr_size(int pkt_type, uint16_t flags, size_t rma_iov_count)
{
	int hdr_size = EFA_RDM_PKT_TYPE_REQ_INFO_VEC[pkt_type].base_hdr_size;

	if (flags & EFA_RDM_REQ_OPT_RAW_ADDR_HDR) {
		/* It is impossible to have both optional connid hdr and opt_raw_addr_hdr
		 * in the header, and length of opt raw addr hdr is larger than
		 * connid hdr (which is confirmed by the following assertion).
		 */
		assert(EFA_RDM_REQ_OPT_RAW_ADDR_HDR_SIZE >= sizeof(struct efa_rdm_req_opt_connid_hdr));
		hdr_size += EFA_RDM_REQ_OPT_RAW_ADDR_HDR_SIZE;
	} else if (flags & EFA_RDM_PKT_CONNID_HDR) {
		hdr_size += sizeof(struct efa_rdm_req_opt_connid_hdr);
	}

	if (flags & EFA_RDM_REQ_OPT_CQ_DATA_HDR) {
		hdr_size += sizeof(struct efa_rdm_req_opt_cq_data_hdr);
	}

	if (efa_rdm_pkt_type_contains_rma_iov(pkt_type)) {
		hdr_size += rma_iov_count * sizeof(struct fi_rma_iov);
	}

	return hdr_size;
}

/**
 * @brief calculates the max header size given a REQ packet type
 *
 * @param[in]	pkt_type	packet type
 * @return	The max possible size of the packet header
 */
static inline
size_t efa_rdm_pkt_type_get_req_max_hdr_size(int pkt_type)
{
	/* To calculate max REQ reader size, we should include all possible REQ opt header flags.
	 * However, because the optional connid header and optional raw address header cannot
	 * exist at the same time, and the raw address header is longer than connid header,
	 * we did not include the flag for CONNID header
	 */
	uint16_t header_flags = EFA_RDM_REQ_OPT_RAW_ADDR_HDR | EFA_RDM_REQ_OPT_CQ_DATA_HDR;

	return efa_rdm_pkt_type_get_req_hdr_size(pkt_type, header_flags, EFA_RDM_IOV_LIMIT);
}

/**
 * @brief maximum header size of all possible packet types
 */
size_t efa_rdm_pkt_type_get_max_hdr_size(void)
{
	size_t max_hdr_size = 0;
	size_t pkt_type = EFA_RDM_REQ_PKT_BEGIN;

	while (pkt_type < EFA_RDM_EXTRA_REQ_PKT_END) {
		max_hdr_size = MAX(max_hdr_size,
				efa_rdm_pkt_type_get_req_max_hdr_size(pkt_type));
		if (pkt_type == EFA_RDM_BASELINE_REQ_PKT_END)
			pkt_type = EFA_RDM_EXTRA_REQ_PKT_BEGIN;
		else
			pkt_type += 1;
	}

	/* Non-emulated (real) rdma inject write requires a header */
	max_hdr_size = MAX(max_hdr_size, sizeof(struct efa_rdm_rma_context_pkt));

	return max_hdr_size;
}