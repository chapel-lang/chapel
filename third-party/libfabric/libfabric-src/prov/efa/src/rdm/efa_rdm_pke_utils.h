/* SPDX-License-Identifier: BSD-2-Clause OR GPL-2.0-only */
/* SPDX-FileCopyrightText: Copyright Amazon.com, Inc. or its affiliates. All rights reserved. */

#ifndef _EFA_RDM_PKE_UTILS_H
#define _EFA_RDM_PKE_UTILS_H

#include "efa_rdm_pke.h"
#include "efa_rdm_protocol.h"
#include "efa_rdm_pkt_type.h"

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

size_t efa_rdm_pke_get_payload_offset(struct efa_rdm_pke *pkt_entry);

ssize_t efa_rdm_pke_init_payload_from_ope(struct efa_rdm_pke *pke,
					  struct efa_rdm_ope *ope,
					  size_t segment_offset,
					  size_t payload_offset,
					  size_t data_size);

ssize_t efa_rdm_pke_copy_payload_to_ope(struct efa_rdm_pke *pke,
					struct efa_rdm_ope *ope);

uint32_t *efa_rdm_pke_connid_ptr(struct efa_rdm_pke *pkt_entry);

#endif
