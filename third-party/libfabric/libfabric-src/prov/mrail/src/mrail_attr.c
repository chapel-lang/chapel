/*
 * Copyright (c) 2018 Intel Corporation, Inc.  All rights reserved.
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

#include "mrail.h"

struct fi_tx_attr mrail_tx_attr = {
	.caps 		= ~0x0ULL,
	.op_flags	= MRAIL_PASSTHRU_TX_OP_FLAGS | MRAIL_TX_OP_FLAGS,
	.msg_order 	= ~0x0ULL,
	.comp_order 	= ~0x0ULL,
	.inject_size 	= SIZE_MAX,
	.size 		= SIZE_MAX,
	.iov_limit 	= MRAIL_IOV_LIMIT,
	.rma_iov_limit 	= SIZE_MAX,
};

struct fi_rx_attr mrail_rx_attr = {
	.caps 			= ~0x0ULL,
	.op_flags		= MRAIL_PASSTHRU_RX_OP_FLAGS | MRAIL_RX_OP_FLAGS,
	.msg_order 		= ~0x0ULL,
	.comp_order 		= ~0x0ULL,
	.total_buffered_recv 	= SIZE_MAX,
	.size 			= SIZE_MAX,
	.iov_limit		= SIZE_MAX,
};

struct fi_ep_attr mrail_ep_attr = {
	.type 			= FI_EP_UNSPEC,
	.protocol 		= FI_PROTO_MRAIL,
	.protocol_version 	= 1,
	.max_msg_size 		= SIZE_MAX,
	.msg_prefix_size	= SIZE_MAX,
	.max_order_raw_size 	= SIZE_MAX,
	.max_order_war_size 	= SIZE_MAX,
	.max_order_waw_size 	= SIZE_MAX,
	.tx_ctx_cnt 		= SIZE_MAX,
	.rx_ctx_cnt 		= SIZE_MAX,
	.auth_key_size		= SIZE_MAX,
};

struct fi_domain_attr mrail_domain_attr = {
	.name			= "ofi_mrail_domain",
	.threading 		= FI_THREAD_SAFE,
	.control_progress 	= FI_PROGRESS_AUTO,
	.data_progress 		= FI_PROGRESS_AUTO,
	.resource_mgmt 		= FI_RM_ENABLED,
	.av_type 		= FI_AV_UNSPEC,
	.mr_mode 		= FI_MR_BASIC | FI_MR_SCALABLE | FI_MR_RAW,
	.mr_key_size		= SIZE_MAX,
	.cq_data_size 		= SIZE_MAX,
	.cq_cnt 		= SIZE_MAX,
	.ep_cnt 		= SIZE_MAX,
	.tx_ctx_cnt 		= SIZE_MAX,
	.rx_ctx_cnt 		= SIZE_MAX,
	.max_ep_tx_ctx 		= SIZE_MAX,
	.max_ep_rx_ctx 		= SIZE_MAX,
	.max_ep_stx_ctx 	= SIZE_MAX,
	.max_ep_srx_ctx 	= SIZE_MAX,
	.cntr_cnt 		= SIZE_MAX,
	.mr_iov_limit 		= SIZE_MAX,
	.caps			= ~0x0ULL,
	.auth_key_size		= SIZE_MAX,
	.max_err_data		= SIZE_MAX,
	.mr_cnt			= SIZE_MAX,
};

struct fi_fabric_attr mrail_fabric_attr = {
	.prov_version = OFI_VERSION_DEF_PROV,
	.name = "ofi_mrail_fabric",
};

struct fi_info mrail_info = {
	.caps = ~0x0ULL,
	.tx_attr = &mrail_tx_attr,
	.rx_attr = &mrail_rx_attr,
	.ep_attr = &mrail_ep_attr,
	.domain_attr = &mrail_domain_attr,
	.fabric_attr = &mrail_fabric_attr
};
