/*
 * Copyright (c) 2019 Amazon.com, Inc. or its affiliates.
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

#include "rxr.h"

#ifdef ENABLE_EFA_POISONING
const uint32_t rxr_poison_value = 0xdeadbeef;
#endif

#ifdef HAVE_LIBCUDA
#define EFA_HMEM_CAP FI_HMEM
#else
#define EFA_HMEM_CAP 0
#endif
#define RXR_TX_CAPS (OFI_TX_MSG_CAPS | FI_TAGGED | OFI_TX_RMA_CAPS | \
		     FI_ATOMIC | EFA_HMEM_CAP)
#define RXR_RX_CAPS (OFI_RX_MSG_CAPS | FI_TAGGED | OFI_RX_RMA_CAPS | \
		     FI_SOURCE | FI_MULTI_RECV | FI_DIRECTED_RECV | \
		     FI_ATOMIC | EFA_HMEM_CAP)
#define RXR_DOM_CAPS (FI_LOCAL_COMM | FI_REMOTE_COMM)

/* TODO: Add support for true FI_DELIVERY_COMPLETE */
#define RXR_TX_OP_FLAGS (FI_INJECT | FI_COMPLETION | FI_TRANSMIT_COMPLETE | \
			 FI_DELIVERY_COMPLETE)
#define RXR_RX_OP_FLAGS (FI_COMPLETION)

struct fi_tx_attr rxr_tx_attr = {
	.caps = RXR_TX_CAPS,
	.msg_order = FI_ORDER_SAS |
		     FI_ORDER_ATOMIC_RAR | FI_ORDER_ATOMIC_RAW |
		     FI_ORDER_ATOMIC_WAR | FI_ORDER_ATOMIC_WAW,
	.op_flags = RXR_TX_OP_FLAGS,
	.comp_order = FI_ORDER_NONE,
	.inject_size = 0,
	.size = 0,
	.iov_limit = RXR_IOV_LIMIT,
};

struct fi_rx_attr rxr_rx_attr = {
	.caps = RXR_RX_CAPS,
	.msg_order = FI_ORDER_SAS |
		     FI_ORDER_ATOMIC_RAR | FI_ORDER_ATOMIC_RAW |
		     FI_ORDER_ATOMIC_WAR | FI_ORDER_ATOMIC_WAW,
	.op_flags = RXR_RX_OP_FLAGS,
	.comp_order = FI_ORDER_NONE,
	.total_buffered_recv = 0,
	.size = 0,
	.iov_limit = RXR_IOV_LIMIT
};

struct fi_ep_attr rxr_ep_attr = {
	.type = FI_EP_RDM,
	.protocol = FI_PROTO_EFA,
	.mem_tag_format = FI_TAG_GENERIC,
	.protocol_version = RXR_CUR_PROTOCOL_VERSION,
	.max_msg_size = UINT64_MAX,
	.tx_ctx_cnt = 1,
	.rx_ctx_cnt = 1
};

struct fi_domain_attr rxr_domain_attr = {
	.threading = FI_THREAD_SAFE,
	.control_progress = FI_PROGRESS_AUTO,
	.data_progress = FI_PROGRESS_AUTO,
	.resource_mgmt = FI_RM_ENABLED,
	.av_type = FI_AV_UNSPEC,
	.mr_mode = OFI_MR_BASIC_MAP | FI_MR_BASIC,
	.mr_iov_limit = 1,
	.mr_cnt = 65535,
	/* Will be overwritten with core provider value in rxr_info_to_rxr() */
	.cq_cnt = 8,
	/* Will be overwritten with core provider value in rxr_info_to_rxr() */
	.ep_cnt = 8,
	.tx_ctx_cnt = 1,
	.rx_ctx_cnt = 1,
	.max_ep_tx_ctx = 1,
	.max_ep_rx_ctx = 1,
	.cq_data_size = RXR_CQ_DATA_SIZE,
	.caps = RXR_DOM_CAPS
};

struct fi_fabric_attr rxr_fabric_attr = {
	.prov_version = FI_VERSION(RXR_MAJOR_VERSION, RXR_MINOR_VERSION),
};

struct fi_info rxr_info = {
	.caps = RXR_TX_CAPS | RXR_RX_CAPS | RXR_DOM_CAPS,
	.addr_format = FI_FORMAT_UNSPEC,
	.tx_attr = &rxr_tx_attr,
	.rx_attr = &rxr_rx_attr,
	.ep_attr = &rxr_ep_attr,
	.domain_attr = &rxr_domain_attr,
	.fabric_attr = &rxr_fabric_attr
};

struct util_prov rxr_util_prov = {
	.prov = &rxr_prov,
	.info = &rxr_info,
	.flags = 0,
};
