/*
 * Copyright (c) 2022 Intel Corporation. All rights reserved.
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

#include "coll.h"

#define COLL_TX_CAPS (0)
#define COLL_RX_CAPS (0)
#define COLL_DOMAIN_CAPS (FI_LOCAL_COMM | FI_REMOTE_COMM)

static struct fi_tx_attr coll_tx_attr = {
	.caps = COLL_TX_CAPS | FI_COLLECTIVE,
	.op_flags = COLL_TX_OP_FLAGS,
	.msg_order = ~0x0ULL,
	.comp_order = FI_ORDER_NONE,
	.size = COLL_TX_SIZE,
	.iov_limit = COLL_IOV_LIMIT,
	.rma_iov_limit = COLL_IOV_LIMIT,
};

static struct fi_rx_attr coll_rx_attr = {
	.caps = COLL_RX_CAPS | FI_COLLECTIVE,
	.op_flags = COLL_RX_OP_FLAGS,
	.msg_order = ~0x0ULL,
	.comp_order = FI_ORDER_NONE,
	.size = COLL_RX_SIZE,
	.iov_limit= COLL_IOV_LIMIT,
};

static struct fi_ep_attr coll_ep_attr = {
	.type = FI_EP_RDM,
	.protocol = FI_PROTO_COLL,
	.protocol_version = 1,
	.max_msg_size = SIZE_MAX,
	.tx_ctx_cnt = 1,
	.rx_ctx_cnt = 1,
	.max_order_raw_size = SIZE_MAX,
	.max_order_war_size = SIZE_MAX,
	.max_order_waw_size = SIZE_MAX,
	.mem_tag_format = FI_TAG_GENERIC >> 1,
};

static struct fi_domain_attr coll_domain_attr = {
	.name = "util-coll",
	.caps = COLL_DOMAIN_CAPS,
	.threading = FI_THREAD_SAFE,
	.control_progress = FI_PROGRESS_AUTO,
	.data_progress = FI_PROGRESS_AUTO,
	.resource_mgmt = FI_RM_ENABLED,
	.av_type = FI_AV_UNSPEC,
	.mr_mode = 0,
	.cq_data_size = sizeof_field(struct ofi_op_hdr, data),
	.cq_cnt = (1 << 16),
	.ep_cnt = (1 << 15),
	.tx_ctx_cnt = 1,
	.rx_ctx_cnt = 1,
	.max_ep_tx_ctx = 1,
	.max_ep_rx_ctx = 1,
	.mr_iov_limit = 1,
};

struct fi_fabric_attr coll_fabric_attr = {
	.name = "UTIL-COLL",
	.prov_version = OFI_VERSION_DEF_PROV,
};

struct fi_info coll_info = {
	.caps = COLL_TX_CAPS | COLL_RX_CAPS | COLL_DOMAIN_CAPS | FI_COLLECTIVE,
	.mode = FI_PEER_TRANSFER,
	.addr_format = FI_SOCKADDR,
	.tx_attr = &coll_tx_attr,
	.rx_attr = &coll_rx_attr,
	.ep_attr = &coll_ep_attr,
	.domain_attr = &coll_domain_attr,
	.fabric_attr = &coll_fabric_attr
};

struct util_prov coll_util_prov = {
	.prov = &coll_prov,
	.info = &coll_info,
	.flags = 0,
};
