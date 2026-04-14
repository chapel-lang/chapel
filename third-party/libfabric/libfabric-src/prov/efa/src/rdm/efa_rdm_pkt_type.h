/* SPDX-License-Identifier: BSD-2-Clause OR GPL-2.0-only */
/* SPDX-FileCopyrightText: Copyright Amazon.com, Inc. or its affiliates. All rights reserved. */

#ifndef _EFA_RDM_PKT_TYPE_H
#define _EFA_RDM_PKT_TYPE_H

#include <assert.h>
#include <stdint.h>
#include "efa_rdm_peer.h"

/**
 * @brief information of a REQ packet type
 */
struct efa_rdm_pkt_type_req_info {
	uint64_t extra_info_id;
	uint64_t base_hdr_size;
	uint64_t ex_feature_flag;
};

extern struct efa_rdm_pkt_type_req_info EFA_RDM_PKT_TYPE_REQ_INFO_VEC[];

/**
 * @brief determine whether a packet types header cotains the "rma_iov" field
 * @returns a boolean
 */
static inline
int efa_rdm_pkt_type_contains_rma_iov(int pkt_type)
{
	switch (pkt_type) {
		case EFA_RDM_EAGER_RTW_PKT:
		case EFA_RDM_DC_EAGER_RTW_PKT:
		case EFA_RDM_LONGCTS_RTW_PKT:
		case EFA_RDM_DC_LONGCTS_RTW_PKT:
		case EFA_RDM_LONGREAD_RTW_PKT:
		case EFA_RDM_SHORT_RTR_PKT:
		case EFA_RDM_LONGCTS_RTR_PKT:
		case EFA_RDM_WRITE_RTA_PKT:
		case EFA_RDM_DC_WRITE_RTA_PKT:
		case EFA_RDM_FETCH_RTA_PKT:
		case EFA_RDM_COMPARE_RTA_PKT:
			return 1;
			break;
		default:
			return 0;
			break;
	}
}

/**
 * @brief determine whether a req pkt type is part of a runt protocol
 *
 * A runt protocol send user data into two parts. The first part
 * was sent by multiple eagerly sent packages. The rest of the
 * data is sent regularly.
 *
 * @param[in]		pkt_type		REQ packet type
 * @return		a boolean
 */
static inline
bool efa_rdm_pkt_type_is_runt(int pkt_type)
{
	return (pkt_type >= EFA_RDM_RUNT_PKT_BEGIN && pkt_type < EFA_RDM_RUNT_PKT_END);
}

/**
 * @brief determine whether a req pkt type is eager RTM or RTW
 *
 * @param[in]		pkt_type		REQ packet type
 * @return		a boolean
 */
static inline
bool efa_rdm_pkt_type_is_eager(int pkt_type)
{
	switch(pkt_type) {
	case EFA_RDM_EAGER_MSGRTM_PKT:
	case EFA_RDM_EAGER_TAGRTM_PKT:
	case EFA_RDM_EAGER_RTW_PKT:
	case EFA_RDM_DC_EAGER_MSGRTM_PKT:
	case EFA_RDM_DC_EAGER_TAGRTM_PKT:
	case EFA_RDM_DC_EAGER_RTW_PKT:
		return 1;
	default:
		return 0;
	}
}

/**
 * @brief determine whether a req pkt type is part of a medium protocol
 *
 * medium protocol send user data eagerly without CTS based flow control.
 *
 * @param[in]		pkt_type		REQ packet type
 * @return		a boolean
 */
static inline
bool efa_rdm_pkt_type_is_medium(int pkt_type)
{
	return pkt_type == EFA_RDM_MEDIUM_TAGRTM_PKT || pkt_type == EFA_RDM_MEDIUM_MSGRTM_PKT ||
	       pkt_type == EFA_RDM_DC_MEDIUM_MSGRTM_PKT ||pkt_type == EFA_RDM_DC_MEDIUM_TAGRTM_PKT;
}

/**
 * @brief determine whether a req pkt type is longcts RTM or RTW.
 *
 * @param[in]		pkt_type		REQ packet type
 * @return		a boolean
 */
static inline
bool efa_rdm_pkt_type_is_longcts_req(int pkt_type)
{
	switch(pkt_type) {
	case EFA_RDM_LONGCTS_MSGRTM_PKT:
	case EFA_RDM_LONGCTS_TAGRTM_PKT:
	case EFA_RDM_LONGCTS_RTW_PKT:
	case EFA_RDM_DC_LONGCTS_MSGRTM_PKT:
	case EFA_RDM_DC_LONGCTS_TAGRTM_PKT:
	case EFA_RDM_DC_LONGCTS_RTW_PKT:
		return 1;
	default:
		return 0;
	}
}

/**
 * @brief determine whether a req pkt type is RTM
 *
 * @param[in]		pkt_type		REQ packet type
 * @return		a boolean
 */
static inline
bool efa_rdm_pkt_type_is_rtm(int pkt_type)
{
	switch(pkt_type) {
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
		return 1;
	default:
		return 0;
	}
}

/**
 * @brief determine whether a req pkt type is RTA
 *
 * @param[in]		pkt_type		REQ packet type
 * @return		a boolean
 */
static inline
bool efa_rdm_pkt_type_is_rta(int pkt_type)
{
	switch(pkt_type) {
	case EFA_RDM_WRITE_RTA_PKT:
	case EFA_RDM_FETCH_RTA_PKT:
	case EFA_RDM_COMPARE_RTA_PKT:
	case EFA_RDM_DC_WRITE_RTA_PKT:
		return 1;
	default:
		return 0;
	}
}

/**
 * @brief determine whether a pkt type is runtread rtm
 *
 * @param[in]		pkt_type		REQ packet type
 * @return		a boolean
 */
static inline
bool efa_rdm_pkt_type_is_runtread(int pkt_type)
{
	return pkt_type == EFA_RDM_RUNTREAD_TAGRTM_PKT || pkt_type == EFA_RDM_RUNTREAD_MSGRTM_PKT;
}

/**
 * @brief determine whether a req pkt type is part of a multi-req protocol
 *
 * A multi-req protocol sends multiple (>=2) data containing REQ packets.
 * This function determine whether a req pkt type is part of a multi-req
 * protocol
 *
 * @param[in]		pkt_type		REQ packet type
 * @return		a boolean
 */
static inline
bool efa_rdm_pkt_type_is_mulreq(int pkt_type)
{
	return efa_rdm_pkt_type_is_medium(pkt_type) || efa_rdm_pkt_type_is_runt(pkt_type);
}

/**
 * @brief determine whether a packet for a given type has user data in it.
 *
 * @param[in]		pkt_type		packet type
 * @return		a boolean
 */
static inline
bool efa_rdm_pkt_type_contains_data(int pkt_type)
{
	return pkt_type == EFA_RDM_READRSP_PKT ||
	       pkt_type == EFA_RDM_ATOMRSP_PKT ||
	       pkt_type == EFA_RDM_CTSDATA_PKT ||
	       efa_rdm_pkt_type_is_runt(pkt_type) ||
	       efa_rdm_pkt_type_is_eager(pkt_type) ||
	       efa_rdm_pkt_type_is_medium(pkt_type) ||
	       efa_rdm_pkt_type_is_longcts_req(pkt_type) ||
	       efa_rdm_pkt_type_is_rta(pkt_type);
}

/**
 * @brief determine whether a pack type is req packet
 *
 * @returns a boolean
 */
static inline
bool efa_rdm_pkt_type_is_req(int pkt_type)
{
	if (pkt_type >= EFA_RDM_REQ_PKT_BEGIN) {
		assert(pkt_type < EFA_RDM_BASELINE_REQ_PKT_END ||
		       (pkt_type >= EFA_RDM_EXTRA_REQ_PKT_BEGIN &&
		        pkt_type < EFA_RDM_EXTRA_REQ_PKT_END));
		return true;
	}

	return false;
}

/**
 * @brief determine whether a packet type has "seg_offset" field in it.
 *
 */
static inline
bool efa_rdm_pkt_type_contains_seg_offset(int pkt_type)
{
	return efa_rdm_pkt_type_is_mulreq(pkt_type) || pkt_type == EFA_RDM_CTSDATA_PKT;
}

bool efa_rdm_pkt_type_is_supported_by_peer(int pkt_type, struct efa_rdm_peer *peer);

size_t efa_rdm_pkt_type_get_req_hdr_size(int pkt_type, uint16_t flags, size_t rma_iov_count);

size_t efa_rdm_pkt_type_get_max_hdr_size(void);

#endif
