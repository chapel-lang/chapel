/*
 * Copyright (c) 2015-2016 Intel Corporation. All rights reserved.
 * (C) Copyright 2020 Hewlett Packard Enterprise Development LP
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

#include "rxm.h"

#define RXM_TX_CAPS (OFI_TX_MSG_CAPS | FI_TAGGED | OFI_TX_RMA_CAPS | \
		     FI_ATOMICS)

#define RXM_RX_CAPS (FI_SOURCE | OFI_RX_MSG_CAPS | FI_TAGGED | \
		     OFI_RX_RMA_CAPS | FI_ATOMICS | FI_DIRECTED_RECV | \
		     FI_MULTI_RECV)

#define RXM_DOMAIN_CAPS (FI_LOCAL_COMM | FI_REMOTE_COMM)


/* Since we are a layering provider, the attributes for which we rely on the
 * core provider are set to full capability. This ensures that ofix_getinfo
 * check hints succeeds and the core provider can accept / reject any capability
 * requested by the app. */

struct fi_tx_attr rxm_tx_attr = {
	.caps = RXM_TX_CAPS | FI_HMEM,
	.op_flags = RXM_PASSTHRU_TX_OP_FLAGS | RXM_TX_OP_FLAGS,
	.msg_order = ~0x0ULL,
	.comp_order = FI_ORDER_NONE,
	.size = RXM_TX_SIZE,
	.iov_limit = RXM_IOV_LIMIT,
	.rma_iov_limit = RXM_IOV_LIMIT,
};

struct fi_rx_attr rxm_rx_attr = {
	.caps = RXM_RX_CAPS | FI_HMEM,
	.op_flags = RXM_PASSTHRU_RX_OP_FLAGS | RXM_RX_OP_FLAGS,
	.msg_order = ~0x0ULL,
	.comp_order = FI_ORDER_NONE,
	.size = RXM_RX_SIZE,
	.iov_limit= RXM_IOV_LIMIT,
};

static struct fi_tx_attr rxm_tx_attr_coll = {
	.caps = RXM_TX_CAPS | FI_COLLECTIVE,
	.op_flags = RXM_PASSTHRU_TX_OP_FLAGS | RXM_TX_OP_FLAGS,
	.msg_order = ~0x0ULL,
	.comp_order = FI_ORDER_NONE,
	.size = RXM_TX_SIZE,
	.iov_limit = RXM_IOV_LIMIT,
	.rma_iov_limit = RXM_IOV_LIMIT,
};

static struct fi_rx_attr rxm_rx_attr_coll = {
	.caps = RXM_RX_CAPS | FI_COLLECTIVE,
	.op_flags = RXM_PASSTHRU_RX_OP_FLAGS | RXM_RX_OP_FLAGS,
	.msg_order = ~0x0ULL,
	.comp_order = FI_ORDER_NONE,
	.size = RXM_RX_SIZE,
	.iov_limit= RXM_IOV_LIMIT,
};

static struct fi_ep_attr rxm_ep_attr = {
	.type = FI_EP_RDM,
	.protocol = FI_PROTO_RXM,
	.protocol_version = 1,
	.max_msg_size = SIZE_MAX,
	.tx_ctx_cnt = 1,
	.rx_ctx_cnt = 1,
	.max_order_raw_size = SIZE_MAX,
	.max_order_war_size = SIZE_MAX,
	.max_order_waw_size = SIZE_MAX,
	.mem_tag_format = FI_TAG_GENERIC,
};

static struct fi_ep_attr rxm_ep_attr_coll = {
	.type = FI_EP_RDM,
	.protocol = FI_PROTO_RXM,
	.protocol_version = 1,
	.max_msg_size = SIZE_MAX,
	.tx_ctx_cnt = 1,
	.rx_ctx_cnt = 1,
	.max_order_raw_size = SIZE_MAX,
	.max_order_war_size = SIZE_MAX,
	.max_order_waw_size = SIZE_MAX,
	.mem_tag_format = FI_TAG_GENERIC >> 1,
};

static struct fi_domain_attr rxm_domain_attr = {
	.caps = RXM_DOMAIN_CAPS,
	.threading = FI_THREAD_SAFE,
	.control_progress = FI_PROGRESS_AUTO,
	.data_progress = FI_PROGRESS_AUTO,
	.resource_mgmt = FI_RM_ENABLED,
	.av_type = FI_AV_UNSPEC,
	/* Advertise support for FI_MR_BASIC so that ofi_check_info call
	 * doesn't fail at RxM level. If an app requires FI_MR_BASIC, it
	 * would be passed down to core provider.
	 */
	.mr_mode = FI_MR_BASIC | FI_MR_SCALABLE,
	.cq_data_size = sizeof_field(struct ofi_op_hdr, data),
	.cq_cnt = (1 << 16),
	.ep_cnt = (1 << 15),
	.tx_ctx_cnt = 1,
	.rx_ctx_cnt = 1,
	.max_ep_tx_ctx = 1,
	.max_ep_rx_ctx = 1,
	.mr_iov_limit = 1,
};

struct fi_fabric_attr rxm_fabric_attr = {
	.prov_version = OFI_VERSION_DEF_PROV,
};

static struct fi_fabric_attr rxm_verbs_fabric_attr = {
	.prov_version = OFI_VERSION_DEF_PROV,
	.prov_name = "verbs",
};

static struct fi_fabric_attr rxm_tcp_fabric_attr = {
	.prov_version = OFI_VERSION_DEF_PROV,
	.prov_name = "tcp",
};

/* We will use the core provider's attributes when using pass through
 * transfers.  This option only supports the tcp provider.
 */
static struct fi_tx_attr rxm_tx_thru_attr = {
	.caps = OFI_PRIMARY_TX_CAPS | OFI_SECONDARY_TX_CAPS,
	.op_flags = OFI_TX_OP_FLAGS,
	.msg_order = ~0x0ULL,
	.comp_order = FI_ORDER_NONE,
	.inject_size = SIZE_MAX,
	.size = RXM_TX_SIZE,
	.iov_limit = SIZE_MAX,
	.rma_iov_limit = SIZE_MAX,
};

static struct fi_rx_attr rxm_rx_thru_attr = {
	.caps = OFI_PRIMARY_RX_CAPS | OFI_SECONDARY_RX_CAPS,
	.op_flags = OFI_RX_OP_FLAGS,
	.msg_order = ~0x0ULL,
	.comp_order = FI_ORDER_NONE,
	.size = SIZE_MAX,
	.iov_limit= SIZE_MAX,
};

static struct fi_ep_attr rxm_ep_thru_attr = {
	.type = FI_EP_RDM,
	.protocol = FI_PROTO_RXM_TCP,
	.protocol_version = 1,
	.max_msg_size = SIZE_MAX,
	.tx_ctx_cnt = 1,
	.rx_ctx_cnt = 1,
	.max_order_raw_size = SIZE_MAX,
	.max_order_war_size = SIZE_MAX,
	.max_order_waw_size = SIZE_MAX,
	.mem_tag_format = FI_TAG_GENERIC,
};

static struct fi_domain_attr rxm_domain_thru_attr = {
	.caps = RXM_DOMAIN_CAPS,
	.threading = FI_THREAD_SAFE,
	.control_progress = FI_PROGRESS_AUTO,
	.data_progress = FI_PROGRESS_AUTO,
	.resource_mgmt = FI_RM_ENABLED,
	.av_type = FI_AV_UNSPEC,
	/* Advertise support for FI_MR_BASIC so that ofi_check_info call
	 * doesn't fail at RxM level. If an app requires FI_MR_BASIC, it
	 * would be passed down to core provider.
	 */
	.mr_mode = FI_MR_BASIC | FI_MR_SCALABLE,
	.cq_data_size = sizeof(uint64_t),
	.cq_cnt = (1 << 16),
	.ep_cnt = (1 << 15),
	.tx_ctx_cnt = 1,
	.rx_ctx_cnt = 1,
	.max_ep_tx_ctx = 1,
	.max_ep_rx_ctx = 1,
	.mr_iov_limit = 1,
};

static struct fi_info rxm_coll_info = {
	.caps = RXM_TX_CAPS | RXM_RX_CAPS | RXM_DOMAIN_CAPS | FI_COLLECTIVE,
	.addr_format = FI_SOCKADDR,
	.tx_attr = &rxm_tx_attr_coll,
	.rx_attr = &rxm_rx_attr_coll,
	.ep_attr = &rxm_ep_attr_coll,
	.domain_attr = &rxm_domain_attr,
	.fabric_attr = &rxm_fabric_attr
};

static struct fi_info rxm_base_info = {
	.caps = RXM_TX_CAPS | RXM_RX_CAPS | RXM_DOMAIN_CAPS | FI_HMEM,
	.addr_format = FI_SOCKADDR,
	.tx_attr = &rxm_tx_attr,
	.rx_attr = &rxm_rx_attr,
	.ep_attr = &rxm_ep_attr,
	.domain_attr = &rxm_domain_attr,
	.fabric_attr = &rxm_fabric_attr,
	.next = &rxm_coll_info,
};

static struct fi_info rxm_tcp_info = {
	.caps = RXM_TX_CAPS | RXM_RX_CAPS | RXM_DOMAIN_CAPS,
	.addr_format = FI_SOCKADDR,
	.tx_attr = &rxm_tx_attr,
	.rx_attr = &rxm_rx_attr,
	.ep_attr = &rxm_ep_attr,
	.domain_attr = &rxm_domain_attr,
	.fabric_attr = &rxm_tcp_fabric_attr,
	.next = &rxm_base_info,
};

struct fi_info rxm_thru_info = {
	.caps = OFI_PRIMARY_CAPS | OFI_SECONDARY_CAPS,
	.addr_format = FI_SOCKADDR,
	.tx_attr = &rxm_tx_thru_attr,
	.rx_attr = &rxm_rx_thru_attr,
	.ep_attr = &rxm_ep_thru_attr,
	.domain_attr = &rxm_domain_thru_attr,
	.fabric_attr = &rxm_tcp_fabric_attr,
	.next = &rxm_tcp_info,
};

static struct fi_info rxm_verbs_info = {
	.caps = RXM_TX_CAPS | RXM_RX_CAPS | RXM_DOMAIN_CAPS | FI_HMEM,
	.addr_format = FI_SOCKADDR,
	.tx_attr = &rxm_tx_attr,
	.rx_attr = &rxm_rx_attr,
	.ep_attr = &rxm_ep_attr,
	.domain_attr = &rxm_domain_attr,
	.fabric_attr = &rxm_verbs_fabric_attr,
	.next = &rxm_thru_info,
};

struct util_prov rxm_util_prov = {
	.prov = &rxm_prov,
	.info = &rxm_verbs_info,
	.flags = 0,
};
