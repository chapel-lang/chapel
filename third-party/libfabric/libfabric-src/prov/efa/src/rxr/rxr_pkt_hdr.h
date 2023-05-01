/*
 * Copyright (c) 2022 Amazon.com, Inc. or its affiliates.
 * All rights reserved.
 *
 * This software is available to you under a choice of one of two
 * licenses.  You may choose to be licensed under the terms of the GNU
 * General Public License (GPL) Version 2, available from the file
 * COPYING in the main directory of this source tree, or the
 * BSD license below:
 *
 *     Redistribution and use in source and binary forms, with or
 *     without modification, are permitted provided that the following
 *     conditions are met:
 *
 *      - Redistributions of source code must retain the above
 *        copyright notice, this list of conditions and the following
 *        disclaimer.
 *
 *      - Redistributions in binary form must reproduce the above
 *        copyright notice, this list of conditions and the following
 *        disclaimer in the documentation and/or other materials
 *        provided with the distribution.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef _RXR_PKT_HDR_H
#define _RXR_PKT_HDR_H

#include "rdm_proto_v4.h"

/**
 * @brief return the seg_offset field in a packet entry
 *
 * @param[in]	pkt_entry	packet entry
 * @return	the value of seg_offset in the packet
 */
static inline
size_t rxr_pkt_hdr_seg_offset(char *pkt)
{
	static const int offset_of_seg_offset_in_header[] = {
		[RXR_DATA_PKT] = offsetof(struct rxr_data_hdr, seg_offset),
		[RXR_MEDIUM_MSGRTM_PKT] = offsetof(struct rxr_medium_rtm_base_hdr, seg_offset),
		[RXR_MEDIUM_TAGRTM_PKT] = offsetof(struct rxr_medium_rtm_base_hdr, seg_offset),
		[RXR_DC_MEDIUM_MSGRTM_PKT] = offsetof(struct rxr_dc_medium_rtm_base_hdr, seg_offset),
		[RXR_DC_MEDIUM_TAGRTM_PKT] = offsetof(struct rxr_dc_medium_rtm_base_hdr, seg_offset),
		[RXR_RUNTREAD_MSGRTM_PKT] = offsetof(struct rxr_runtread_rtm_base_hdr, seg_offset),
		[RXR_RUNTREAD_TAGRTM_PKT] = offsetof(struct rxr_runtread_rtm_base_hdr, seg_offset),
	};

	int pkt_type = rxr_get_base_hdr(pkt)->type;
	int hdr_offset = offset_of_seg_offset_in_header[pkt_type];

	assert(hdr_offset);
	return *(uint64_t *)(pkt + hdr_offset);
}

#endif
