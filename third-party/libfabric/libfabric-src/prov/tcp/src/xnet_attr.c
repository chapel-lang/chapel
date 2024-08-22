/*
 * Copyright (c) 2017-2022 Intel Corporation. All rights reserved.
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

#include "xnet.h"


#define XNET_DOMAIN_CAPS (FI_LOCAL_COMM | FI_REMOTE_COMM)
#define XNET_EP_CAPS	 (FI_MSG | FI_RMA | FI_RMA_PMEM)
#define XNET_SRX_EP_CAPS (XNET_EP_CAPS | FI_TAGGED)
#define XNET_RDM_EP_CAPS (XNET_EP_CAPS | FI_TAGGED)
#define XNET_TX_CAPS	 (FI_SEND | FI_WRITE | FI_READ)
#define XNET_RX_CAPS	 (FI_RECV | FI_REMOTE_READ | \
			  FI_REMOTE_WRITE | FI_RMA_EVENT)
#define XNET_SRX_CAPS	 (XNET_RX_CAPS | FI_DIRECTED_RECV | FI_SOURCE | \
			  FI_MULTI_RECV)

#define XNET_MSG_ORDER (OFI_ORDER_RAR_SET | OFI_ORDER_RAW_SET | \
			OFI_ORDER_WAW_SET | \
			FI_ORDER_RAS | FI_ORDER_WAS | \
			FI_ORDER_SAW | FI_ORDER_SAS)

#define XNET_TX_OP_FLAGS \
	(FI_INJECT | FI_INJECT_COMPLETE | FI_TRANSMIT_COMPLETE | \
	 FI_DELIVERY_COMPLETE | FI_COMMIT_COMPLETE | FI_COMPLETION)

#define XNET_RX_OP_FLAGS (FI_COMPLETION)
#define XNET_SRX_OP_FLAGS (FI_COMPLETION | FI_MULTI_RECV)

static struct fi_tx_attr xnet_tx_attr = {
	.caps = XNET_EP_CAPS | XNET_TX_CAPS,
	.op_flags = XNET_TX_OP_FLAGS,
	.comp_order = FI_ORDER_STRICT,
	.msg_order = XNET_MSG_ORDER,
	.inject_size = XNET_DEF_INJECT,
	.size = 1024,
	.iov_limit = XNET_IOV_LIMIT,
	.rma_iov_limit = XNET_IOV_LIMIT,
};

static struct fi_rx_attr xnet_rx_attr = {
	.caps = XNET_EP_CAPS | XNET_RX_CAPS,
	.op_flags = XNET_RX_OP_FLAGS,
	.comp_order = FI_ORDER_STRICT,
	.msg_order = XNET_MSG_ORDER,
	.total_buffered_recv = 0,
	.size = 1024,
	.iov_limit = XNET_IOV_LIMIT,
};

static struct fi_ep_attr xnet_ep_attr = {
	.type = FI_EP_MSG,
	.protocol = FI_PROTO_SOCK_TCP,
	.protocol_version = 0,
	.max_msg_size = SIZE_MAX,
	.tx_ctx_cnt = 1,
	.rx_ctx_cnt = 1,
	.max_order_raw_size = SIZE_MAX,
	.max_order_waw_size = SIZE_MAX,
};

static struct fi_tx_attr xnet_srx_tx_attr = {
	.caps = XNET_SRX_EP_CAPS | XNET_TX_CAPS,
	.op_flags = XNET_TX_OP_FLAGS,
	.comp_order = FI_ORDER_NONE,
	.msg_order = XNET_MSG_ORDER,
	.inject_size = XNET_DEF_INJECT,
	.size = 1024,
	.iov_limit = XNET_IOV_LIMIT,
	.rma_iov_limit = XNET_IOV_LIMIT,
};

static struct fi_rx_attr xnet_srx_rx_attr = {
	.caps = XNET_SRX_EP_CAPS | XNET_SRX_CAPS,
	.op_flags = XNET_SRX_OP_FLAGS,
	.comp_order = FI_ORDER_NONE,
	.msg_order = XNET_MSG_ORDER,
	.total_buffered_recv = 0,
	.size = 65536,
	.iov_limit = XNET_IOV_LIMIT,
};

static struct fi_ep_attr xnet_srx_ep_attr = {
	.type = FI_EP_MSG,
	.protocol = FI_PROTO_SOCK_TCP,
	.protocol_version = 0,
	.max_msg_size = SIZE_MAX,
	.tx_ctx_cnt = 1,
	.rx_ctx_cnt = FI_SHARED_CONTEXT,
	.max_order_raw_size = SIZE_MAX,
	.max_order_waw_size = SIZE_MAX,
	.mem_tag_format = FI_TAG_GENERIC,
};

static struct fi_tx_attr xnet_rdm_tx_attr = {
	.caps = XNET_RDM_EP_CAPS | XNET_TX_CAPS,
	.op_flags = XNET_TX_OP_FLAGS,
	.comp_order = FI_ORDER_STRICT,
	.msg_order = XNET_MSG_ORDER,
	.inject_size = XNET_DEF_INJECT,
	.size = 65536,
	.iov_limit = XNET_IOV_LIMIT,
	.rma_iov_limit = XNET_IOV_LIMIT,
};

static struct fi_rx_attr xnet_rdm_rx_attr = {
	.caps = XNET_RDM_EP_CAPS | XNET_SRX_CAPS,
	.op_flags = XNET_SRX_OP_FLAGS,
	.comp_order = FI_ORDER_STRICT,
	.msg_order = XNET_MSG_ORDER,
	.total_buffered_recv = 0,
	.size = 65536,
	.iov_limit = XNET_IOV_LIMIT,
};

static struct fi_ep_attr xnet_rdm_ep_attr = {
	.type = FI_EP_RDM,
	.protocol = FI_PROTO_XNET,
	.protocol_version = XNET_RDM_VERSION,
	.max_msg_size = SIZE_MAX,
	.tx_ctx_cnt = 1,
	.rx_ctx_cnt = 1,
	.max_order_raw_size = SIZE_MAX,
	.max_order_waw_size = SIZE_MAX,
	.mem_tag_format = FI_TAG_GENERIC,
};

static struct fi_domain_attr xnet_domain_attr = {
	.name = "tcp",
	.caps = XNET_DOMAIN_CAPS,
	.threading = FI_THREAD_SAFE,
	.control_progress = FI_PROGRESS_AUTO,
	.data_progress = FI_PROGRESS_AUTO,
	.resource_mgmt = FI_RM_ENABLED,
	.mr_mode = FI_MR_SCALABLE | FI_MR_BASIC,
	.mr_key_size = sizeof(uint64_t),
	.av_type = FI_AV_UNSPEC,
	.cq_data_size = sizeof(uint64_t),
	.cq_cnt = 256,
	.ep_cnt = 8192,
	.tx_ctx_cnt = 8192,
	.rx_ctx_cnt = 8192,
	.max_ep_srx_ctx = 8192,
	.max_ep_tx_ctx = 1,
	.max_ep_rx_ctx = 1,
	.mr_iov_limit = 1,
};

static struct fi_domain_attr xnet_rdm_domain_attr = {
	.name = "tcp",
	.caps = XNET_DOMAIN_CAPS,
	.threading = FI_THREAD_SAFE,
	.control_progress = FI_PROGRESS_AUTO,
	.data_progress = FI_PROGRESS_AUTO,
	.resource_mgmt = FI_RM_ENABLED,
	.mr_mode = FI_MR_SCALABLE | FI_MR_BASIC,
	.mr_key_size = sizeof(uint64_t),
	.av_type = FI_AV_UNSPEC,
	.cq_data_size = sizeof(uint64_t),
	.cq_cnt = 256,
	.ep_cnt = 256,
	.tx_ctx_cnt = 256,
	.rx_ctx_cnt = 256,
	.max_ep_srx_ctx = 0,
	.max_ep_tx_ctx = 1,
	.max_ep_rx_ctx = 1,
	.mr_iov_limit = 1,
};

struct fi_fabric_attr xnet_fabric_attr = {
	.name = "tcp",
	.prov_version = OFI_VERSION_DEF_PROV,
};

static struct fi_info xnet_rdm_info = {
	.caps = XNET_DOMAIN_CAPS | XNET_RDM_EP_CAPS | XNET_TX_CAPS | XNET_SRX_CAPS,
	.addr_format = FI_SOCKADDR,
	.tx_attr = &xnet_rdm_tx_attr,
	.rx_attr = &xnet_rdm_rx_attr,
	.ep_attr = &xnet_rdm_ep_attr,
	.domain_attr = &xnet_rdm_domain_attr,
	.fabric_attr = &xnet_fabric_attr,
};

struct fi_info xnet_srx_info = {
	.next = &xnet_rdm_info,
	.caps = XNET_DOMAIN_CAPS | XNET_SRX_EP_CAPS | XNET_TX_CAPS | XNET_SRX_CAPS,
	.addr_format = FI_SOCKADDR,
	.tx_attr = &xnet_srx_tx_attr,
	.rx_attr = &xnet_srx_rx_attr,
	.ep_attr = &xnet_srx_ep_attr,
	.domain_attr = &xnet_domain_attr,
	.fabric_attr = &xnet_fabric_attr,
};

static struct fi_info xnet_info = {
	.next = &xnet_srx_info,
	.caps = XNET_DOMAIN_CAPS | XNET_EP_CAPS | XNET_TX_CAPS | XNET_RX_CAPS,
	.addr_format = FI_SOCKADDR,
	.tx_attr = &xnet_tx_attr,
	.rx_attr = &xnet_rx_attr,
	.ep_attr = &xnet_ep_attr,
	.domain_attr = &xnet_domain_attr,
	.fabric_attr = &xnet_fabric_attr,
};

void xnet_init_infos(void)
{
	struct fi_info *info;

	for (info = &xnet_info; info; info = info->next) {
		info->tx_attr->inject_size = xnet_max_inject;
	}
}

/* User hints will still override the modified dest_info attributes
 * through ofi_alter_info
 */
static void
xnet_alter_defaults(uint32_t version, const struct fi_info *hints,
		    const struct fi_info *base_info,
		    struct fi_info *dest_info)
{
	dest_info->tx_attr->size = xnet_default_tx_size;
	if ((base_info->ep_attr->rx_ctx_cnt != FI_SHARED_CONTEXT) &&
	    hints && hints->ep_attr &&
	    (hints->ep_attr->rx_ctx_cnt != FI_SHARED_CONTEXT))
		dest_info->rx_attr->size = xnet_default_rx_size;
}


struct util_prov xnet_util_prov = {
	.prov = &xnet_prov,
	.info = &xnet_info,
	.alter_defaults = &xnet_alter_defaults,
	.flags = 0,
};
