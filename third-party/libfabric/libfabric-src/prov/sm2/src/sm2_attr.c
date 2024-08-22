/*
 * Copyright (c) Intel Corporation. All rights reserved.
 * Copyright (c) Amazon.com, Inc. or its affiliates. All rights reserved.
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

#include "sm2.h"

#define SM2_TX_CAPS \
	(OFI_TX_MSG_CAPS | FI_TAGGED | FI_ATOMICS | FI_READ | FI_WRITE)
#define SM2_RX_CAPS                                                            \
	(FI_SOURCE | OFI_RX_MSG_CAPS | FI_TAGGED | FI_ATOMICS |                \
	 FI_REMOTE_READ | FI_REMOTE_WRITE | FI_DIRECTED_RECV | FI_MULTI_RECV | \
	 FI_RMA_EVENT)
#define SM2_HMEM_TX_CAPS ((SM2_TX_CAPS | FI_HMEM) & ~FI_ATOMICS)
#define SM2_HMEM_RX_CAPS ((SM2_RX_CAPS | FI_HMEM) & ~FI_ATOMICS)
#define SM2_TX_OP_FLAGS                                              \
	(FI_COMPLETION | FI_INJECT_COMPLETE | FI_TRANSMIT_COMPLETE | \
	 FI_DELIVERY_COMPLETE)
#define SM2_RX_OP_FLAGS (FI_COMPLETION | FI_MULTI_RECV)

struct fi_tx_attr sm2_tx_attr = {
	.caps = SM2_TX_CAPS,
	.op_flags = SM2_TX_OP_FLAGS,
	.comp_order = FI_ORDER_NONE,
	.msg_order = FI_ORDER_SAS,
	.inject_size = SM2_ATOMIC_INJECT_SIZE,
	.size = 1024,
	.iov_limit = SM2_IOV_LIMIT,
	.rma_iov_limit = SM2_IOV_LIMIT,
};

struct fi_rx_attr sm2_rx_attr = {
	.caps = SM2_RX_CAPS,
	.op_flags = SM2_RX_OP_FLAGS,
	.comp_order = FI_ORDER_STRICT,
	.msg_order = FI_ORDER_SAS,
	.size = 1024,
	.iov_limit = SM2_IOV_LIMIT,
};

struct fi_tx_attr sm2_hmem_tx_attr = {
	.caps = SM2_HMEM_TX_CAPS,
	.op_flags = SM2_TX_OP_FLAGS,
	.comp_order = FI_ORDER_NONE,
	.msg_order = FI_ORDER_SAS,
	.inject_size = 0,
	.size = 1024,
	.iov_limit = SM2_IOV_LIMIT,
	.rma_iov_limit = SM2_IOV_LIMIT,
};

struct fi_rx_attr sm2_hmem_rx_attr = {
	.caps = SM2_HMEM_RX_CAPS,
	.op_flags = SM2_RX_OP_FLAGS,
	.comp_order = FI_ORDER_STRICT,
	.msg_order = FI_ORDER_SAS,
	.size = 1024,
	.iov_limit = SM2_IOV_LIMIT,
};

struct fi_ep_attr sm2_ep_attr = {
	.type = FI_EP_RDM,
	.protocol = FI_PROTO_SM2,
	.protocol_version = 1,
	.max_msg_size = SIZE_MAX,
	.max_order_raw_size = SIZE_MAX,
	.max_order_waw_size = SIZE_MAX,
	.max_order_war_size = SIZE_MAX,
	.mem_tag_format = FI_TAG_GENERIC,
	.tx_ctx_cnt = 1,
	.rx_ctx_cnt = 1,
};

struct fi_domain_attr sm2_domain_attr = {
	.name = "sm2",
	.threading = FI_THREAD_SAFE,
	.control_progress = FI_PROGRESS_AUTO,
	.data_progress = FI_PROGRESS_MANUAL,
	.resource_mgmt = FI_RM_ENABLED,
	.av_type = FI_AV_UNSPEC,
	.mr_mode = FI_MR_BASIC | FI_MR_SCALABLE,
	.mr_key_size = sizeof_field(struct fi_rma_iov, key),
	.cq_data_size = sizeof_field(struct sm2_xfer_hdr, cq_data),
	.cq_cnt = (1 << 10),
	.ep_cnt = SM2_MAX_UNIVERSE_SIZE,
	.tx_ctx_cnt = (1 << 10),
	.rx_ctx_cnt = (1 << 10),
	.max_ep_tx_ctx = 1,
	.max_ep_rx_ctx = 1,
	.mr_iov_limit = SM2_IOV_LIMIT,
	.caps = FI_LOCAL_COMM,
};

struct fi_domain_attr sm2_hmem_domain_attr = {
	.name = "sm2",
	.threading = FI_THREAD_SAFE,
	.control_progress = FI_PROGRESS_AUTO,
	.data_progress = FI_PROGRESS_MANUAL,
	.resource_mgmt = FI_RM_ENABLED,
	.av_type = FI_AV_UNSPEC,
	.mr_mode = FI_MR_HMEM,
	.mr_key_size = sizeof_field(struct fi_rma_iov, key),
	.cq_data_size = sizeof_field(struct sm2_xfer_hdr, cq_data),
	.cq_cnt = (1 << 10),
	.ep_cnt = SM2_MAX_UNIVERSE_SIZE,
	.tx_ctx_cnt = (1 << 10),
	.rx_ctx_cnt = (1 << 10),
	.max_ep_tx_ctx = 1,
	.max_ep_rx_ctx = 1,
	.mr_iov_limit = SM2_IOV_LIMIT,
	.caps = FI_LOCAL_COMM,
};

struct fi_fabric_attr sm2_fabric_attr = {
	.name = "sm2",
	.prov_version = OFI_VERSION_DEF_PROV,
};

struct fi_info sm2_hmem_info = {
	.caps = SM2_HMEM_TX_CAPS | SM2_HMEM_RX_CAPS | FI_MULTI_RECV |
		FI_LOCAL_COMM,
	.addr_format = FI_ADDR_STR,
	.tx_attr = &sm2_hmem_tx_attr,
	.rx_attr = &sm2_hmem_rx_attr,
	.ep_attr = &sm2_ep_attr,
	.domain_attr = &sm2_hmem_domain_attr,
	.fabric_attr = &sm2_fabric_attr,
};

struct fi_info sm2_info = {
	.caps = SM2_TX_CAPS | SM2_RX_CAPS | FI_MULTI_RECV | FI_LOCAL_COMM,
	.addr_format = FI_ADDR_STR,
	.tx_attr = &sm2_tx_attr,
	.rx_attr = &sm2_rx_attr,
	.ep_attr = &sm2_ep_attr,
	.domain_attr = &sm2_domain_attr,
	.fabric_attr = &sm2_fabric_attr,
	.next = &sm2_hmem_info,
};
