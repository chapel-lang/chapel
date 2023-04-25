/*
 * Copyright (C) 2016 by Argonne National Laboratory.
 * Copyright (C) 2021 Cornelis Networks.
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

#include <ofi.h>
#include <ofi_mem.h>
#include "rdma/opx/fi_opx.h"
#include "rdma/opx/fi_opx_internal.h"
#include "rdma/opx/fi_opx_hfi1.h"

#include "rdma/opx/fi_opx_addr.h"

void fi_opx_set_info(struct fi_info *fi, enum fi_progress progress)
{
	*fi->tx_attr = (struct fi_tx_attr) {
		.caps		= FI_OPX_DEFAULT_TX_CAPS,
		.mode		= FI_OPX_DEFAULT_MODE,
		.op_flags	= FI_TRANSMIT_COMPLETE,
		.msg_order	= FI_OPX_DEFAULT_MSG_ORDER,
		.comp_order	= FI_ORDER_NONE,
		.inject_size	= FI_OPX_HFI1_PACKET_IMM,
		.size		= SIZE_MAX,
		.iov_limit	= SIZE_MAX,
		.rma_iov_limit  = 0
	};

	*fi->rx_attr = (struct fi_rx_attr) {
		.caps		= FI_OPX_DEFAULT_RX_CAPS,
		.mode		= FI_OPX_DEFAULT_MODE,
		.op_flags	= FI_MULTI_RECV,
		.msg_order	= FI_OPX_DEFAULT_MSG_ORDER,
		.comp_order	= FI_ORDER_NONE,
		.total_buffered_recv = FI_OPX_HFI1_PACKET_MTU + 64 /* header */,
		.size		= SIZE_MAX,
		.iov_limit	= SIZE_MAX
	};

	*fi->ep_attr = (struct fi_ep_attr) {
		.type			= FI_EP_RDM,
		.protocol		= FI_PROTO_OPX,
		.protocol_version	= FI_OPX_PROTOCOL_VERSION,
		.max_msg_size		= FI_OPX_MAX_MSG_SIZE,
		.msg_prefix_size	= 0,
		.max_order_raw_size	= 0,
		.max_order_war_size	= 0,
		.max_order_waw_size	= 0,
		.mem_tag_format		= FI_OPX_MEM_TAG_FORMAT,
		.tx_ctx_cnt		= 1,
		.rx_ctx_cnt		= 1,
		.auth_key_size		= 0,
		.auth_key		= NULL
	};


	*fi->domain_attr = (struct fi_domain_attr) {
		.domain		= NULL,
		.name		= strdup(FI_OPX_DOMAIN_NAME), /* TODO: runtime query for name? */
		.threading	= OPX_THREAD,
		.control_progress = progress,
		.data_progress	= progress,
		.resource_mgmt	= FI_RM_ENABLED,
		.av_type	= OPX_AV,
		.mr_mode	= OPX_MR,
		.mr_key_size	= 2,
		.cq_data_size	= FI_OPX_REMOTE_CQ_DATA_SIZE,
		.cq_cnt		= SIZE_MAX,
		.ep_cnt		= 160,
		.tx_ctx_cnt	= 160,	/* TODO ppn */
		.rx_ctx_cnt	= 160,	/* TODO ppn */

		.max_ep_tx_ctx	= 160,
		.max_ep_rx_ctx	= 160,
		.max_ep_stx_ctx	= 0,
		.max_ep_srx_ctx	= SIZE_MAX,
		.cntr_cnt	= 0,
		.mr_iov_limit	= 1,
		.caps		= FI_LOCAL_COMM | FI_REMOTE_COMM,	/* TODO: FI_SHARED_AV */
		.mode		= 0,
		.auth_key	= NULL,
		.auth_key_size	= 0,
		.max_err_data	= 0,
		.mr_cnt		= 0					/* TODO: FI_MR */
	};

	*fi->fabric_attr = (struct fi_fabric_attr) {
		.fabric		= NULL,
		.name		= strdup(FI_OPX_FABRIC_NAME),
		.prov_name	= NULL,
		.prov_version	= FI_OPX_PROVIDER_VERSION
	};

	fi->caps		= FI_OPX_DEFAULT_TX_CAPS | FI_OPX_DEFAULT_RX_CAPS;
	fi->mode		= FI_OPX_DEFAULT_MODE;

	fi->addr_format		= FI_ADDR_OPX;
	fi->src_addrlen		= sizeof(union fi_opx_addr);
	fi->dest_addrlen	= sizeof(union fi_opx_addr);
	fi->dest_addr = NULL;
	fi->src_addr = mem_dup(&opx_default_addr, sizeof(opx_default_addr));
	fi->next = NULL;
}

int fi_opx_set_default_info()
{
	struct fi_info *fi;
	struct fi_info *fi_auto;
	fi = fi_dupinfo(NULL);
	fi_auto = fi_dupinfo(NULL);
	if (!fi || !fi_auto) {
		errno = FI_ENOMEM;
		goto err;
	}

	fi_opx_global.info = fi;
	fi_opx_set_info(fi, FI_PROGRESS_MANUAL);
	fi_opx_set_info(fi_auto, FI_PROGRESS_AUTO);
	fi->next = fi_auto;
	
	return 0;

err:
	if (fi) {
		fi_freeinfo(fi);
	}
	if (fi_auto) {
		fi_freeinfo(fi_auto);
	}
	return -errno;
}
