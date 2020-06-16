/*
 * Copyright (c) 2017-2018 Intel Corporation. All rights reserved.
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

#include "rstream.h"


struct fi_tx_attr rstream_tx_attr = {
	.caps = RSTREAM_CAPS,
	.msg_order = FI_ORDER_SAS,
	.size = RSTREAM_DEFAULT_QP_SIZE,
};

struct fi_rx_attr rstream_rx_attr = {
	.caps = RSTREAM_CAPS,
	.msg_order = FI_ORDER_SAS,
	.size = RSTREAM_DEFAULT_QP_SIZE,
};

struct fi_ep_attr rstream_ep_attr = {
	.type = FI_EP_SOCK_STREAM,
	.protocol = FI_PROTO_RSTREAM,
	.protocol_version = 1,
	.tx_ctx_cnt = 1,
	.rx_ctx_cnt = 1,
};

struct fi_domain_attr rstream_domain_attr = {
	.caps = FI_LOCAL_COMM | FI_REMOTE_COMM,
	.threading = FI_THREAD_SAFE,
	.control_progress = FI_PROGRESS_AUTO,
	.data_progress = FI_PROGRESS_MANUAL,
	.resource_mgmt = FI_RM_ENABLED,
	.av_type = FI_AV_UNSPEC,
	/* for the ofi mr_check  */
	.mr_mode = 0,
	.tx_ctx_cnt = 1,
	.rx_ctx_cnt = 1,
	.max_ep_tx_ctx = 1,
	.mr_iov_limit = 1,
};

struct fi_fabric_attr rstream_fabric_attr = {
	.prov_version = FI_VERSION(1, 8),
};

struct fi_info rstream_info = {
	.caps = RSTREAM_CAPS,
	.addr_format = FI_SOCKADDR,
	.tx_attr = &rstream_tx_attr,
	.rx_attr = &rstream_rx_attr,
	.ep_attr = &rstream_ep_attr,
	.domain_attr = &rstream_domain_attr,
	.fabric_attr = &rstream_fabric_attr
};

/* settings post CONNREQ for users */
void rstream_set_info(struct fi_info *info)
{
	info->caps = RSTREAM_CAPS;
	info->mode = 0;
	info->ep_attr->type = FI_EP_SOCK_STREAM;
	info->ep_attr->protocol = rstream_info.ep_attr->protocol;
	info->domain_attr->mr_mode = 0;
	info->domain_attr->mr_cnt = 0;
	*info->rx_attr = rstream_rx_attr;
	*info->tx_attr = rstream_tx_attr;
}
