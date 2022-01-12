/*
* Copyright (c) 2015-2016 Intel Corporation, Inc.  All rights reserved.
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

#ifdef _WIN32

#include "netdir.h"
#include "netdir_buf.h"
#include "ofi_prov.h"
#include "ofi_util.h"
#include "ofi_mem.h"

#include "netdir_ov.h"
#include "netdir_iface.h"

#include "netdir_queue.h"

const char ofi_nd_prov_name[] = "netdir";

struct fi_provider ofi_nd_prov = {
	.name = ofi_nd_prov_name,
	.version = OFI_VERSION_DEF_PROV,
	.fi_version = OFI_VERSION_LATEST,
	.getinfo = ofi_nd_getinfo,
	.fabric = ofi_nd_fabric,
	.cleanup = ofi_nd_fini
};

struct util_prov ofi_nd_util_prov = {
	.prov = &ofi_nd_prov,
	.info = 0,
	.flags = UTIL_RX_SHARED_CTX,
};

struct gl_data gl_data = {
	/* 8 KByte */
	.inline_thr = 8192,
	.prepost_cnt = 8,
	.prepost_buf_cnt = 1,
	.flow_control_cnt = 1,
	.total_avail = 64
};

int ofi_nd_getinfo(uint32_t version, const char *node, const char *service,
		   uint64_t flags, const struct fi_info *hints,
		   struct fi_info **info)
{
	if (ofi_nd_util_prov.info) {
		return util_getinfo(&ofi_nd_util_prov, version, node, service, flags,
				    hints, info);
	} else {
		*info = NULL;
		return -FI_EINVAL;
	}
}

void ofi_nd_fini(void)
{
	if (ofi_nd_util_prov.info) {
		fi_freeinfo((void*)ofi_nd_util_prov.info);
		ofi_nd_util_prov.info = 0;
	}
	ofi_nd_shutdown();
	nd_buf_fini_apply();
}

extern struct fi_provider ofi_nd_prov;

static int ofi_nd_adapter_cb(const ND2_ADAPTER_INFO *adapter, const char *name)
{
	struct fi_info *info = fi_allocinfo();
	if (!info)
		return -FI_ENOMEM;

	info->tx_attr->caps = FI_MSG | FI_SEND;
	info->tx_attr->comp_order = FI_ORDER_STRICT;
	info->tx_attr->inject_size = (size_t)gl_data.inline_thr;
	info->tx_attr->size = (size_t)adapter->MaxTransferLength;
	/* TODO: if optimization will be needed, we can use adapter->MaxInitiatorSge,
	 * and use ND SGE to send/write iovecs */
	info->tx_attr->iov_limit = ND_MSG_IOV_LIMIT;
	info->tx_attr->rma_iov_limit = ND_MSG_IOV_LIMIT;
	info->tx_attr->op_flags = OFI_ND_TX_OP_FLAGS;
	info->tx_attr->msg_order = OFI_ND_MSG_ORDER;

	info->rx_attr->caps = FI_MSG | FI_RECV;
	info->rx_attr->comp_order = FI_ORDER_STRICT;
	info->rx_attr->total_buffered_recv = 0;
	info->rx_attr->size = (size_t)adapter->MaxTransferLength;
	/* TODO: if optimization will be needed, we can use adapter->MaxInitiatorSge,
	 * and use ND SGE to recv iovecs */
	info->rx_attr->iov_limit = ND_MSG_IOV_LIMIT;
	info->rx_attr->msg_order = OFI_ND_MSG_ORDER;

	info->ep_attr->type = FI_EP_MSG;
	info->ep_attr->protocol = FI_PROTO_NETWORKDIRECT;
	info->ep_attr->protocol_version = 0;
	info->ep_attr->max_msg_size = (size_t)adapter->MaxTransferLength;

	info->domain_attr->caps = OFI_ND_DOMAIN_CAPS;
	info->domain_attr->name = strdup(name);
	info->domain_attr->threading = FI_THREAD_SAFE;
	info->domain_attr->control_progress = FI_PROGRESS_AUTO;
	info->domain_attr->data_progress = FI_PROGRESS_AUTO;
	info->domain_attr->resource_mgmt = FI_RM_DISABLED;
	info->domain_attr->av_type = FI_AV_UNSPEC;
	info->domain_attr->mr_mode = FI_MR_BASIC | OFI_MR_BASIC_MAP | FI_MR_LOCAL;
	info->domain_attr->cq_cnt = (size_t)adapter->MaxCompletionQueueDepth;
	info->domain_attr->mr_iov_limit = ND_MSG_IOV_LIMIT;
	info->domain_attr->mr_cnt = OFI_ND_MAX_MR_CNT;

	info->fabric_attr->name = strdup(ofi_nd_prov_name);
	info->fabric_attr->prov_version = OFI_VERSION_DEF_PROV;

	info->caps = OFI_ND_EP_CAPS | OFI_ND_DOMAIN_CAPS;
	info->addr_format = FI_SOCKADDR;

	if (!ofi_nd_util_prov.info) {
		ofi_nd_util_prov.info = info;
	} else {
		struct fi_info *finfo = (struct fi_info *) ofi_nd_util_prov.info;

		while (finfo->next)
			finfo = finfo->next;
		finfo->next = info;
	}

	return FI_SUCCESS;
}

NETDIR_INI
{
	fi_param_define(&ofi_nd_prov, "inlinethr", FI_PARAM_INT,
		"Inline threshold: size of buffer to be send using pre-allocated buffer");
	fi_param_define(&ofi_nd_prov, "largemsgthr", FI_PARAM_INT,
		"Large msg threshold: size of user data that is considered as large message");
	fi_param_define(&ofi_nd_prov, "prepostcnt", FI_PARAM_INT,
		"Prepost Buffer Count: number of buffers to be preposted per EP and "
		"not required internal ACK");
	fi_param_define(&ofi_nd_prov, "prepostbufcnt", FI_PARAM_INT,
		"Count of Entries in Array of Preposted Buffers: number of set of buffer "
		"in each entry array of buffers to be preposted per EP");

	ofi_nd_startup(ofi_nd_adapter_cb);
	return &ofi_nd_prov;
}


#endif /* _WIN32 */

