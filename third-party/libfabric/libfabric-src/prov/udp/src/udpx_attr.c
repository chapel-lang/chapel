/*
 * Copyright (c) 2015-2016 Intel Corporation. All rights reserved.
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

#include "udpx.h"

#define UDPX_TX_CAPS (OFI_TX_MSG_CAPS | FI_MULTICAST)
#define UDPX_RX_CAPS (FI_SOURCE | OFI_RX_MSG_CAPS)
#define UDPX_DOMAIN_CAPS (FI_LOCAL_COMM | FI_REMOTE_COMM)

struct fi_tx_attr udpx_tx_attr = {
	.caps = UDPX_TX_CAPS,
	.comp_order = FI_ORDER_STRICT,
	.inject_size = 1472,
	.size = 1024,
	.iov_limit = UDPX_IOV_LIMIT
};

struct fi_rx_attr udpx_rx_attr = {
	.caps = UDPX_RX_CAPS,
	.comp_order = FI_ORDER_STRICT,
	.total_buffered_recv = (1 << 16),
	.size = 1024,
	.iov_limit = UDPX_IOV_LIMIT
};

struct fi_ep_attr udpx_ep_attr = {
	.type = FI_EP_DGRAM,
	.protocol = FI_PROTO_UDP,
	.protocol_version = 0,
	.max_msg_size = 1472,
	.tx_ctx_cnt = 1,
	.rx_ctx_cnt = 1
};

struct fi_domain_attr udpx_domain_attr = {
	.caps = UDPX_DOMAIN_CAPS,
	.name = "udp",
	.threading = FI_THREAD_SAFE,
	.control_progress = FI_PROGRESS_AUTO,
	.data_progress = FI_PROGRESS_AUTO,
	.resource_mgmt = FI_RM_ENABLED,
	.av_type = FI_AV_UNSPEC,
	.mr_mode = FI_MR_BASIC | FI_MR_SCALABLE,
	.mr_key_size = sizeof(uint64_t),
	.cq_cnt = 256,
	.ep_cnt = 256,
	.tx_ctx_cnt = 256,
	.rx_ctx_cnt = 256,
	.max_ep_tx_ctx = 1,
	.max_ep_rx_ctx = 1
};

struct fi_fabric_attr udpx_fabric_attr = {
	.name = "UDP-IP",
	.prov_version = OFI_VERSION_DEF_PROV
};

struct fi_info udpx_info = {
	.caps = UDPX_DOMAIN_CAPS | UDPX_TX_CAPS | UDPX_RX_CAPS,
	.addr_format = FI_SOCKADDR,
	.tx_attr = &udpx_tx_attr,
	.rx_attr = &udpx_rx_attr,
	.ep_attr = &udpx_ep_attr,
	.domain_attr = &udpx_domain_attr,
	.fabric_attr = &udpx_fabric_attr
};

struct util_prov udpx_util_prov = {
	.prov = &udpx_prov,
	.info = &udpx_info,
	.flags = 0,
};
