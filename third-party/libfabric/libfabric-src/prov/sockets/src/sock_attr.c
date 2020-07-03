/*
 * Copyright (c) 2020 Intel Corporation. All rights reserved.
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

#include "sock.h"

#define SOCK_MSG_TX_CAPS (OFI_TX_MSG_CAPS | FI_TAGGED | OFI_TX_RMA_CAPS | \
			  FI_ATOMICS | FI_NAMED_RX_CTX | FI_FENCE | FI_TRIGGER)
#define SOCK_MSG_RX_CAPS (OFI_RX_MSG_CAPS | FI_TAGGED | OFI_RX_RMA_CAPS | \
			  FI_ATOMICS | FI_DIRECTED_RECV | FI_MULTI_RECV | \
			  FI_RMA_EVENT | FI_SOURCE | FI_TRIGGER)

#define SOCK_RDM_TX_CAPS (OFI_TX_MSG_CAPS | FI_TAGGED | OFI_TX_RMA_CAPS | \
			  FI_ATOMICS | FI_NAMED_RX_CTX | FI_FENCE | FI_TRIGGER | \
			  FI_RMA_PMEM)
#define SOCK_RDM_RX_CAPS (OFI_RX_MSG_CAPS | FI_TAGGED | OFI_RX_RMA_CAPS | \
			  FI_ATOMICS | FI_DIRECTED_RECV | FI_MULTI_RECV | \
			  FI_RMA_EVENT | FI_SOURCE | FI_TRIGGER | FI_RMA_PMEM)

#define SOCK_DGRAM_TX_CAPS (OFI_TX_MSG_CAPS | FI_TAGGED | FI_NAMED_RX_CTX | \
			    FI_FENCE | FI_TRIGGER)
#define SOCK_DGRAM_RX_CAPS (OFI_RX_MSG_CAPS | FI_TAGGED | FI_DIRECTED_RECV | \
			    FI_MULTI_RECV | FI_SOURCE | FI_TRIGGER)

#define SOCK_DOMAIN_CAPS (FI_LOCAL_COMM | FI_REMOTE_COMM | FI_SHARED_AV)

#define SOCK_TX_OP_FLAGS (FI_COMMIT_COMPLETE | FI_COMPLETION | \
			  FI_DELIVERY_COMPLETE | FI_INJECT | FI_INJECT_COMPLETE | \
			  FI_MULTICAST | FI_TRANSMIT_COMPLETE)
#define SOCK_RX_OP_FLAGS (FI_COMMIT_COMPLETE | FI_COMPLETION | \
			  FI_DELIVERY_COMPLETE | FI_INJECT | FI_INJECT_COMPLETE | \
			  FI_MULTI_RECV | FI_TRANSMIT_COMPLETE)

struct fi_ep_attr sock_msg_ep_attr = {
	.type = FI_EP_MSG,
	.protocol = FI_PROTO_SOCK_TCP,
	.protocol_version = SOCK_WIRE_PROTO_VERSION,
	.max_msg_size = SOCK_EP_MAX_MSG_SZ,
	.msg_prefix_size = SOCK_EP_MSG_PREFIX_SZ,
	.max_order_raw_size = SOCK_EP_MAX_ORDER_RAW_SZ,
	.max_order_war_size = SOCK_EP_MAX_ORDER_WAR_SZ,
	.max_order_waw_size = SOCK_EP_MAX_ORDER_WAW_SZ,
	.mem_tag_format = SOCK_EP_MEM_TAG_FMT,
	.tx_ctx_cnt = SOCK_EP_MAX_TX_CNT,
	.rx_ctx_cnt = SOCK_EP_MAX_RX_CNT,
};

struct fi_tx_attr sock_msg_tx_attr = {
	.caps = SOCK_MSG_TX_CAPS,
	.mode = SOCK_MODE,
	.op_flags = SOCK_TX_OP_FLAGS,
	.msg_order = SOCK_EP_MSG_ORDER,
	.inject_size = SOCK_EP_MAX_INJECT_SZ,
	.size = SOCK_EP_TX_SZ,
	.iov_limit = SOCK_EP_MAX_IOV_LIMIT,
	.rma_iov_limit = SOCK_EP_MAX_IOV_LIMIT,
};

struct fi_rx_attr sock_msg_rx_attr = {
	.caps = SOCK_MSG_RX_CAPS,
	.mode = SOCK_MODE,
	.op_flags = SOCK_RX_OP_FLAGS,
	.msg_order = SOCK_EP_MSG_ORDER,
	.comp_order = SOCK_EP_COMP_ORDER,
	.total_buffered_recv = SOCK_EP_MAX_BUFF_RECV,
	.size = SOCK_EP_RX_SZ,
	.iov_limit = SOCK_EP_MAX_IOV_LIMIT,
};

struct fi_ep_attr sock_dgram_ep_attr = {
	.type = FI_EP_DGRAM,
	.protocol = FI_PROTO_SOCK_TCP,
	.protocol_version = SOCK_WIRE_PROTO_VERSION,
	.max_msg_size = SOCK_EP_MAX_MSG_SZ,
	.msg_prefix_size = SOCK_EP_MSG_PREFIX_SZ,
	.max_order_raw_size = SOCK_EP_MAX_ORDER_RAW_SZ,
	.max_order_war_size = SOCK_EP_MAX_ORDER_WAR_SZ,
	.max_order_waw_size = SOCK_EP_MAX_ORDER_WAW_SZ,
	.mem_tag_format = SOCK_EP_MEM_TAG_FMT,
	.tx_ctx_cnt = SOCK_EP_MAX_TX_CNT,
	.rx_ctx_cnt = SOCK_EP_MAX_RX_CNT,
};

struct fi_ep_attr sock_rdm_ep_attr = {
	.type = FI_EP_RDM,
	.protocol = FI_PROTO_SOCK_TCP,
	.protocol_version = SOCK_WIRE_PROTO_VERSION,
	.max_msg_size = SOCK_EP_MAX_MSG_SZ,
	.msg_prefix_size = SOCK_EP_MSG_PREFIX_SZ,
	.max_order_raw_size = SOCK_EP_MAX_ORDER_RAW_SZ,
	.max_order_war_size = SOCK_EP_MAX_ORDER_WAR_SZ,
	.max_order_waw_size = SOCK_EP_MAX_ORDER_WAW_SZ,
	.mem_tag_format = SOCK_EP_MEM_TAG_FMT,
	.tx_ctx_cnt = SOCK_EP_MAX_TX_CNT,
	.rx_ctx_cnt = SOCK_EP_MAX_RX_CNT,
};

struct fi_tx_attr sock_rdm_tx_attr = {
	.caps = SOCK_RDM_TX_CAPS,
	.mode = SOCK_MODE,
	.op_flags = SOCK_TX_OP_FLAGS,
	.msg_order = SOCK_EP_MSG_ORDER,
	.inject_size = SOCK_EP_MAX_INJECT_SZ,
	.size = SOCK_EP_TX_SZ,
	.iov_limit = SOCK_EP_MAX_IOV_LIMIT,
	.rma_iov_limit = SOCK_EP_MAX_IOV_LIMIT,
};

struct fi_rx_attr sock_rdm_rx_attr = {
	.caps = SOCK_RDM_RX_CAPS,
	.mode = SOCK_MODE,
	.op_flags = SOCK_RX_OP_FLAGS,
	.msg_order = SOCK_EP_MSG_ORDER,
	.comp_order = SOCK_EP_COMP_ORDER,
	.total_buffered_recv = SOCK_EP_MAX_BUFF_RECV,
	.size = SOCK_EP_RX_SZ,
	.iov_limit = SOCK_EP_MAX_IOV_LIMIT,
};

struct fi_tx_attr sock_dgram_tx_attr = {
	.caps = SOCK_DGRAM_TX_CAPS,
	.mode = SOCK_MODE,
	.op_flags = SOCK_TX_OP_FLAGS,
	.msg_order = SOCK_EP_MSG_ORDER,
	.inject_size = SOCK_EP_MAX_INJECT_SZ,
	.size = SOCK_EP_TX_SZ,
	.iov_limit = SOCK_EP_MAX_IOV_LIMIT,
	.rma_iov_limit = 0,
};

struct fi_rx_attr sock_dgram_rx_attr = {
	.caps = SOCK_DGRAM_RX_CAPS,
	.mode = SOCK_MODE,
	.op_flags = SOCK_RX_OP_FLAGS,
	.msg_order = SOCK_EP_MSG_ORDER,
	.comp_order = SOCK_EP_COMP_ORDER,
	.total_buffered_recv = SOCK_EP_MAX_BUFF_RECV,
	.size = SOCK_EP_RX_SZ,
	.iov_limit = SOCK_EP_MAX_IOV_LIMIT,
};

struct fi_tx_attr sock_stx_attr = {
	.caps = SOCK_RDM_TX_CAPS | SOCK_RDM_RX_CAPS | SOCK_DOMAIN_CAPS,
	.mode = SOCK_MODE,
	.op_flags = FI_TRANSMIT_COMPLETE,
	.msg_order = SOCK_EP_MSG_ORDER,
	.inject_size = SOCK_EP_MAX_INJECT_SZ,
	.size = SOCK_EP_TX_SZ,
	.iov_limit = SOCK_EP_MAX_IOV_LIMIT,
	.rma_iov_limit = SOCK_EP_MAX_IOV_LIMIT,
};

struct fi_rx_attr sock_srx_attr = {
	.caps = SOCK_RDM_TX_CAPS | SOCK_RDM_RX_CAPS | SOCK_DOMAIN_CAPS,
	.mode = SOCK_MODE,
	.op_flags = 0,
	.msg_order = SOCK_EP_MSG_ORDER,
	.comp_order = SOCK_EP_COMP_ORDER,
	.total_buffered_recv = 0,
	.size = SOCK_EP_MAX_MSG_SZ,
	.iov_limit = SOCK_EP_MAX_IOV_LIMIT,
};

struct fi_domain_attr sock_domain_attr = {
	.name = "sockets",
	.threading = FI_THREAD_SAFE,
	.control_progress = FI_PROGRESS_AUTO,
	.data_progress = FI_PROGRESS_AUTO,
	.resource_mgmt = FI_RM_ENABLED,
	/* Provider supports basic memory registration mode */
	.mr_mode = FI_MR_BASIC | FI_MR_SCALABLE,
	.mr_key_size = sizeof(uint64_t),
	.cq_data_size = sizeof(uint64_t),
	.cq_cnt = SOCK_EP_MAX_CQ_CNT,
	.ep_cnt = SOCK_EP_MAX_EP_CNT,
	.tx_ctx_cnt = SOCK_EP_MAX_TX_CNT,
	.rx_ctx_cnt = SOCK_EP_MAX_RX_CNT,
	.max_ep_tx_ctx = SOCK_EP_MAX_TX_CNT,
	.max_ep_rx_ctx = SOCK_EP_MAX_RX_CNT,
	.max_ep_stx_ctx = SOCK_EP_MAX_EP_CNT,
	.max_ep_srx_ctx = SOCK_EP_MAX_EP_CNT,
	.cntr_cnt = SOCK_EP_MAX_CNTR_CNT,
	.mr_iov_limit = SOCK_EP_MAX_IOV_LIMIT,
	.max_err_data = SOCK_MAX_ERR_CQ_EQ_DATA_SZ,
	.mr_cnt = SOCK_DOMAIN_MR_CNT,
	.caps = SOCK_DOMAIN_CAPS,
};

struct fi_fabric_attr sock_fabric_attr = {
	.name = "sockets",
	.prov_version = OFI_VERSION_DEF_PROV,
};

struct fi_info sock_msg_info = {
	.caps = SOCK_MSG_TX_CAPS | SOCK_MSG_RX_CAPS | SOCK_DOMAIN_CAPS,
	.addr_format = FI_SOCKADDR,
	.tx_attr = &sock_msg_tx_attr,
	.rx_attr = &sock_msg_rx_attr,
	.ep_attr = &sock_msg_ep_attr,
	.domain_attr = &sock_domain_attr,
	.fabric_attr = &sock_fabric_attr
};

struct fi_info sock_rdm_info = {
	.next = &sock_msg_info,
	.caps = SOCK_RDM_TX_CAPS | SOCK_RDM_RX_CAPS | SOCK_DOMAIN_CAPS,
	.addr_format = FI_SOCKADDR,
	.tx_attr = &sock_rdm_tx_attr,
	.rx_attr = &sock_rdm_rx_attr,
	.ep_attr = &sock_rdm_ep_attr,
	.domain_attr = &sock_domain_attr,
	.fabric_attr = &sock_fabric_attr
};

struct fi_info sock_dgram_info = {
	.next = &sock_rdm_info,
	.caps = SOCK_DGRAM_TX_CAPS | SOCK_DGRAM_RX_CAPS | SOCK_DOMAIN_CAPS,
	.addr_format = FI_SOCKADDR,
	.tx_attr = &sock_dgram_tx_attr,
	.rx_attr = &sock_dgram_rx_attr,
	.ep_attr = &sock_dgram_ep_attr,
	.domain_attr = &sock_domain_attr,
	.fabric_attr = &sock_fabric_attr
};
