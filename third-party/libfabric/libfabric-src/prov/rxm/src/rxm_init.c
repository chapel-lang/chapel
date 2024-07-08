/*
 * Copyright (c) 2016 Intel Corporation. All rights reserved.
 * (C) Copyright 2020 Hewlett Packard Enterprise Development LP.
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

#include <netdb.h>

#include <rdma/fi_errno.h>
#include <ofi_net.h>

#include <ofi_prov.h>
#include "rxm.h"

#define RXM_ATOMIC_UNSUPPORTED_MSG_ORDER (FI_ORDER_RAW | FI_ORDER_RAR |  \
					  FI_ORDER_WAW | FI_ORDER_WAR |  \
					  FI_ORDER_SAR | FI_ORDER_SAW)

#define RXM_PASSTHRU_CAPS (FI_MSG | FI_RMA | FI_SEND | FI_RECV |	\
			   FI_READ | FI_WRITE | FI_REMOTE_READ |	\
			   FI_REMOTE_WRITE | FI_HMEM)

size_t rxm_msg_tx_size;
size_t rxm_msg_rx_size;
size_t rxm_def_rx_size = 2048;
size_t rxm_def_tx_size = 2048;

size_t rxm_buffer_size = 16384;
size_t rxm_packet_size;

int rxm_passthru = 0; /* disable by default, need to analyze performance */
int force_auto_progress;
int rxm_use_write_rndv;
int rxm_detect_hmem_iface;
enum fi_wait_obj def_wait_obj = FI_WAIT_FD, def_tcp_wait_obj = FI_WAIT_UNSPEC;

char *rxm_proto_state_str[] = {
	RXM_PROTO_STATES(OFI_STR)
};

bool rxm_passthru_info(const struct fi_info *info)
{
	return info && info->ep_attr &&
	       info->ep_attr->protocol == FI_PROTO_RXM_TCP;
}

/*
 * - Support FI_MR_LOCAL/FI_LOCAL_MR as ofi_rxm can handle it.
 * - The RxM FI_RMA implementation is pass-through but the provider can handle
 *   FI_MR_PROV_KEY and FI_MR_VIRT_ADDR in its large message transfer rendezvous
 *   protocol.  We can set FI_MR_PROV_KEY and FI_MR_VIRT_ADDR only if the app
 *   is not using RMA.
 * - fi_alter_domain_attr should correctly set the mr_mode in return fi_info
 *   based on hints.
 */
void rxm_info_to_core_mr_modes(uint32_t version, const struct fi_info *hints,
			       struct fi_info *core_info)
{
	if (hints && hints->domain_attr &&
	    (hints->domain_attr->mr_mode & (FI_MR_SCALABLE | FI_MR_BASIC))) {
		core_info->mode |= FI_LOCAL_MR;
		core_info->domain_attr->mr_mode = hints->domain_attr->mr_mode;
	} else if (FI_VERSION_LT(version, FI_VERSION(1, 5))) {
		core_info->mode |= FI_LOCAL_MR;
		core_info->domain_attr->mr_mode = FI_MR_UNSPEC;
	} else {
		core_info->domain_attr->mr_mode |= FI_MR_LOCAL;
		if (!hints || !hints->domain_attr ||
		    !ofi_rma_target_allowed(hints->caps))
			core_info->domain_attr->mr_mode |= OFI_MR_BASIC_MAP;
		else
			core_info->domain_attr->mr_mode |=
				hints->domain_attr->mr_mode;

		/* RxM is setup to support FI_HMEM with the core provider requiring
		 * FI_MR_HMEM. Always set this MR mode bit.
		 */
		if (hints && hints->caps & FI_HMEM)
			core_info->domain_attr->mr_mode |= FI_MR_HMEM;
	}
}

static bool rxm_use_srx(const struct fi_info *hints,
			const struct fi_info *base_info)
{
	const struct fi_info *info;
	int ret, use_srx = 0;

	ret = fi_param_get_bool(&rxm_prov, "use_srx", &use_srx);
	if (ret != -FI_ENODATA)
		return use_srx;

	info = base_info ? base_info : hints;

	return info && info->fabric_attr && info->fabric_attr->prov_name &&
	       strcasestr(info->fabric_attr->prov_name, "tcp");
}

static int
rxm_info_thru_core(uint32_t version, const struct fi_info *hints,
		   const struct fi_info *base_info, struct fi_info *core_info)
{
	struct fi_domain_attr *core_dom, *hint_dom;
	struct fi_ep_attr *core_ep, *hint_ep;
	struct fi_tx_attr *core_tx, *hint_tx;
	struct fi_rx_attr *core_rx, *hint_rx;

	if (hints) {
		core_info->caps = hints->caps;
		core_info->mode = hints->mode;

		if (hints->domain_attr) {
			core_dom = core_info->domain_attr;
			hint_dom = hints->domain_attr;

			core_dom->threading = hint_dom->threading;
			core_dom->data_progress = hint_dom->data_progress;
			core_dom->resource_mgmt = hint_dom->resource_mgmt;
			core_dom->mr_mode = hint_dom->mr_mode;
			core_dom->cq_data_size = hint_dom->cq_data_size;
			core_dom->caps = hint_dom->caps;
			core_dom->mode = hint_dom->mode;
			core_dom->tclass = hint_dom->tclass;
		}

		if (hints->ep_attr) {
			core_ep = core_info->ep_attr;
			hint_ep = hints->ep_attr;

			core_ep->max_msg_size = hint_ep->max_msg_size;
			core_ep->msg_prefix_size = hint_ep->msg_prefix_size;
			core_ep->max_order_raw_size = hint_ep->max_order_raw_size;
			core_ep->max_order_war_size = hint_ep->max_order_war_size;
			core_ep->max_order_waw_size = hint_ep->max_order_waw_size;
			core_ep->mem_tag_format = hint_ep->mem_tag_format;
		}

		if (hints->tx_attr) {
			core_tx = core_info->tx_attr;
			hint_tx = hints->tx_attr;

			core_tx->caps = hint_tx->caps;
			core_tx->mode = hint_tx->mode;
			core_tx->op_flags = hint_tx->op_flags;
			core_tx->msg_order = hint_tx->msg_order;
			core_tx->inject_size = hint_tx->inject_size;
			core_tx->iov_limit = hint_tx->iov_limit;
			core_tx->rma_iov_limit = hint_tx->rma_iov_limit;
			core_tx->tclass = hint_tx->tclass;
		}

		if (hints->rx_attr) {
			core_rx = core_info->rx_attr;
			hint_rx = hints->rx_attr;

			core_rx->caps = hint_rx->caps;
			core_rx->mode = hint_rx->mode;
			core_rx->op_flags = hint_rx->op_flags;
			core_rx->msg_order = hint_rx->msg_order;
			core_rx->iov_limit = hint_rx->iov_limit;
		}
	}

	core_info->ep_attr->type = FI_EP_MSG;
	core_info->ep_attr->rx_ctx_cnt = FI_SHARED_CONTEXT;
	core_info->ep_attr->tx_ctx_cnt = 1;

	core_info->tx_attr->size = rxm_msg_tx_size ?
				   rxm_msg_tx_size : RXM_MSG_RXTX_SIZE;
	core_info->rx_attr->size = rxm_msg_rx_size ?
				   rxm_msg_rx_size : RXM_MSG_SRX_SIZE;

	return 0;
}

int rxm_info_to_core(uint32_t version, const struct fi_info *hints,
		     const struct fi_info *base_info, struct fi_info *core_info)
{
	if (rxm_passthru_info(base_info)) {
		if (!rxm_passthru)
			return -FI_ENODATA;
		return rxm_info_thru_core(version, hints, base_info, core_info);
	}

	rxm_info_to_core_mr_modes(version, hints, core_info);

	core_info->mode |= FI_RX_CQ_DATA | FI_CONTEXT;

	if (hints) {
		core_info->caps = hints->caps & RXM_PASSTHRU_CAPS;
		if (hints->caps & (FI_ATOMIC | FI_TAGGED))
			core_info->caps |= FI_MSG | FI_SEND | FI_RECV;

		/* FI_RMA cap is needed for large message transfer protocol */
		if (core_info->caps & FI_MSG) {
			core_info->caps |= FI_RMA | FI_READ |
					   FI_REMOTE_READ | FI_REMOTE_WRITE;
		}

		if (hints->domain_attr) {
			core_info->domain_attr->caps |= hints->domain_attr->caps;
			core_info->domain_attr->threading =
				hints->domain_attr->threading;
		}
		if (hints->tx_attr) {
			core_info->tx_attr->op_flags =
				hints->tx_attr->op_flags & RXM_PASSTHRU_TX_OP_FLAGS;
			core_info->tx_attr->msg_order = hints->tx_attr->msg_order;
			core_info->tx_attr->comp_order = hints->tx_attr->comp_order;
		}
		if (hints->rx_attr) {
			core_info->rx_attr->op_flags =
				hints->rx_attr->op_flags & RXM_PASSTHRU_RX_OP_FLAGS;
			core_info->rx_attr->msg_order = hints->rx_attr->msg_order;
			core_info->rx_attr->comp_order = hints->rx_attr->comp_order;
		}
		if ((hints->caps & FI_HMEM) && ofi_hmem_p2p_disabled())
			return -FI_ENODATA;
	}

	core_info->ep_attr->type = FI_EP_MSG;

	if (rxm_use_srx(hints, base_info)) {
		FI_DBG(&rxm_prov, FI_LOG_FABRIC,
		       "Requesting shared receive context from core provider\n");
		core_info->ep_attr->rx_ctx_cnt = FI_SHARED_CONTEXT;
		core_info->rx_attr->size = rxm_msg_rx_size ?
					   rxm_msg_rx_size : RXM_MSG_SRX_SIZE;
	} else {
		core_info->rx_attr->size = rxm_msg_rx_size ?
					   rxm_msg_rx_size : RXM_MSG_RXTX_SIZE;
	}

	core_info->tx_attr->op_flags &= ~RXM_TX_OP_FLAGS;
	core_info->tx_attr->size = rxm_msg_tx_size ?
				   rxm_msg_tx_size : RXM_MSG_RXTX_SIZE;

	core_info->rx_attr->op_flags &= ~FI_MULTI_RECV;

	return 0;
}

static int
rxm_info_thru_rxm(uint32_t version, const struct fi_info *core_info,
		  const struct fi_info *base_info, struct fi_info *info)
{
	info->caps = core_info->caps;
	info->mode = core_info->mode;

	*info->tx_attr = *core_info->tx_attr;
	info->tx_attr->comp_order = base_info->tx_attr->comp_order;
	info->tx_attr->size = MIN(base_info->tx_attr->size, rxm_def_tx_size);

	*info->rx_attr = *core_info->rx_attr;
	info->rx_attr->comp_order = base_info->rx_attr->comp_order;
	info->rx_attr->size = MIN(base_info->rx_attr->size, rxm_def_rx_size);

	*info->ep_attr = *base_info->ep_attr;
	info->ep_attr->max_msg_size = core_info->ep_attr->max_msg_size;
	info->ep_attr->max_order_raw_size = core_info->ep_attr->max_order_raw_size;
	info->ep_attr->max_order_war_size = core_info->ep_attr->max_order_war_size;
	info->ep_attr->max_order_waw_size = core_info->ep_attr->max_order_waw_size;

	*info->domain_attr = *base_info->domain_attr;
	info->domain_attr->mr_mode = core_info->domain_attr->mr_mode;
	info->domain_attr->mr_key_size = core_info->domain_attr->mr_key_size;
	info->domain_attr->cq_data_size = core_info->domain_attr->cq_data_size;
	info->domain_attr->mr_iov_limit = core_info->domain_attr->mr_iov_limit;
	info->domain_attr->caps = core_info->domain_attr->caps;
	info->domain_attr->mode = core_info->domain_attr->mode;
	info->domain_attr->max_err_data = core_info->domain_attr->max_err_data;
	info->domain_attr->mr_cnt = core_info->domain_attr->mr_cnt;
	info->domain_attr->tclass = core_info->domain_attr->tclass;

	if (core_info->nic) {
		info->nic = ofi_nic_dup(core_info->nic);
		if (!info->nic)
			return -FI_ENOMEM;
	}

	return 0;
}

int rxm_info_to_rxm(uint32_t version, const struct fi_info *core_info,
		    const struct fi_info *base_info, struct fi_info *info)
{
	if (rxm_passthru_info(base_info))
		return rxm_info_thru_rxm(version, core_info, base_info, info);

	info->caps = ofi_pick_core_flags(base_info->caps, core_info->caps,
					 FI_LOCAL_COMM | FI_REMOTE_COMM);
	info->mode = (core_info->mode & ~FI_RX_CQ_DATA) | base_info->mode;

	info->tx_attr->caps		= base_info->tx_attr->caps;
	info->tx_attr->mode		= info->mode;
	info->tx_attr->msg_order 	= core_info->tx_attr->msg_order;
	info->tx_attr->comp_order 	= base_info->tx_attr->comp_order;

	/* If the core provider requires registering send buffers, it's
	 * usually faster to copy small transfer through bounce buffers
	 * than requiring the user to register the buffers.  Bump the
	 * inject size up to the rxm limit (eager buffer size) in this
	 * case.  If registration is not required, use the core provider's
	 * limit, which avoids potential extra data copies.
	 *
	 * If we report the size of the bounce buffer, apps may call inject
	 * rather than send, which hampers our ability to use the direct
	 * send feature that avoids data copies.
	 */
	if (ofi_mr_local(info) ||
	    (core_info->tx_attr->inject_size <= sizeof(struct rxm_pkt))) {
		info->tx_attr->inject_size = base_info->tx_attr->inject_size;
	} else {
		info->tx_attr->inject_size = core_info->tx_attr->inject_size -
					     sizeof(struct rxm_pkt);
	}

	/* User hints will override the modified info attributes through
	 * ofi_alter_info.  Set default sizes lower than supported maximums.
	 */
	info->tx_attr->size = MIN(base_info->tx_attr->size, rxm_def_tx_size);
	info->rx_attr->size = MIN(base_info->rx_attr->size, rxm_def_rx_size);

	info->tx_attr->iov_limit 	= MIN(base_info->tx_attr->iov_limit,
					      core_info->tx_attr->iov_limit);
	info->tx_attr->rma_iov_limit	= MIN(base_info->tx_attr->rma_iov_limit,
					      core_info->tx_attr->rma_iov_limit);

	info->rx_attr->caps		= base_info->rx_attr->caps;
	info->rx_attr->mode		= info->rx_attr->mode & ~FI_RX_CQ_DATA;
	info->rx_attr->msg_order 	= core_info->rx_attr->msg_order;
	info->rx_attr->comp_order 	= base_info->rx_attr->comp_order;
	info->rx_attr->iov_limit 	= MIN(base_info->rx_attr->iov_limit,
					      core_info->rx_attr->iov_limit);

	*info->ep_attr = *base_info->ep_attr;
	info->ep_attr->max_msg_size = core_info->ep_attr->max_msg_size;
	info->ep_attr->max_order_raw_size = core_info->ep_attr->max_order_raw_size;
	info->ep_attr->max_order_war_size = core_info->ep_attr->max_order_war_size;
	info->ep_attr->max_order_waw_size = core_info->ep_attr->max_order_waw_size;

	*info->domain_attr = *base_info->domain_attr;
	info->domain_attr->caps = ofi_pick_core_flags(base_info->domain_attr->caps,
						core_info->domain_attr->caps,
						FI_LOCAL_COMM | FI_REMOTE_COMM);
	info->domain_attr->mr_mode |= core_info->domain_attr->mr_mode;
	info->domain_attr->cq_data_size = MIN(core_info->domain_attr->cq_data_size,
					      base_info->domain_attr->cq_data_size);
	info->domain_attr->mr_key_size = core_info->domain_attr->mr_key_size;

	if (core_info->nic) {
		info->nic = ofi_nic_dup(core_info->nic);
		if (!info->nic)
			return -FI_ENOMEM;
	}

	/* FI_HMEM is only supported if core provider supports it. */
	if (!(core_info->caps & FI_HMEM)) {
		info->caps &= ~FI_HMEM;
		info->tx_attr->caps &= ~FI_HMEM;
		info->rx_attr->caps &= ~FI_HMEM;
	}

	return 0;
}

static void rxm_init_infos(void)
{
	struct fi_info *cur;
	size_t buf_size, tx_size = 0, rx_size = 0;

	/* Historically, 'buffer_size' was the name given for the eager message
	 * size.  Maintain the name for backwards compatability.
	 */
	if (!fi_param_get_size_t(&rxm_prov, "buffer_size", &buf_size)) {
		/* We need enough space to carry extra headers */
		if (buf_size < sizeof(struct rxm_rndv_hdr) ||
		    buf_size < sizeof(struct rxm_atomic_hdr)) {
			FI_WARN(&rxm_prov, FI_LOG_CORE,
				"Requested buffer size too small\n");
			buf_size = MAX(sizeof(struct rxm_rndv_hdr),
				       sizeof(struct rxm_atomic_hdr));
		}

		if (buf_size > INT32_MAX)
			buf_size = INT32_MAX;

		rxm_buffer_size = buf_size;
	}

	rxm_packet_size = sizeof(struct rxm_pkt) + rxm_buffer_size;

	fi_param_get_size_t(&rxm_prov, "tx_size", &tx_size);
	fi_param_get_size_t(&rxm_prov, "rx_size", &rx_size);
	if (tx_size)
		rxm_def_tx_size = tx_size;
	if (rx_size)
		rxm_def_rx_size = rx_size;

	for (cur = (struct fi_info *) rxm_util_prov.info; cur; cur = cur->next) {
		if (!rxm_passthru_info(cur))
			cur->tx_attr->inject_size = rxm_buffer_size;
		if (tx_size)
			cur->tx_attr->size = tx_size;
		if (rx_size)
			cur->rx_attr->size = rx_size;
	}
}

static void rxm_alter_info(const struct fi_info *hints, struct fi_info *info)
{
	struct fi_info *cur;

	for (cur = info; cur; cur = cur->next) {
		/* auto progress requires starting a listener thread */
		if (cur->domain_attr->data_progress == FI_PROGRESS_AUTO ||
		    force_auto_progress)
			cur->domain_attr->threading = FI_THREAD_SAFE;

		if (rxm_passthru_info(cur))
			continue;

		/* Remove the following caps if they are not requested as they
		 * may affect performance in fast-path */
		if (!hints) {
			cur->caps &= ~(FI_DIRECTED_RECV | FI_SOURCE |
				       FI_ATOMIC);
			cur->tx_attr->caps &= ~(FI_ATOMIC);
			cur->rx_attr->caps &= ~(FI_DIRECTED_RECV | FI_ATOMIC |
						FI_SOURCE);
			cur->domain_attr->data_progress = FI_PROGRESS_MANUAL;
		} else {
			if (!(hints->caps & FI_DIRECTED_RECV)) {
				cur->caps &= ~FI_DIRECTED_RECV;
				cur->rx_attr->caps &= ~FI_DIRECTED_RECV;
			}

			if (hints->mode & FI_BUFFERED_RECV)
				cur->mode |= FI_BUFFERED_RECV;

			if (hints->caps & FI_ATOMIC) {
				cur->tx_attr->msg_order &=
					~(RXM_ATOMIC_UNSUPPORTED_MSG_ORDER);
				cur->rx_attr->msg_order &=
					~(RXM_ATOMIC_UNSUPPORTED_MSG_ORDER);
				cur->ep_attr->max_order_raw_size = 0;
				cur->ep_attr->max_order_war_size = 0;
				cur->ep_attr->max_order_waw_size = 0;
			} else {
				cur->caps &= ~FI_ATOMIC;
				cur->tx_attr->caps &= ~FI_ATOMIC;
				cur->rx_attr->caps &= ~FI_ATOMIC;
			}

			if (!ofi_mr_local(hints)) {
				cur->mode &= ~FI_LOCAL_MR;
				cur->tx_attr->mode &= ~FI_LOCAL_MR;
				cur->rx_attr->mode &= ~FI_LOCAL_MR;
				cur->domain_attr->mr_mode &= ~FI_MR_LOCAL;
			}

			if (!hints->domain_attr ||
			    hints->domain_attr->data_progress != FI_PROGRESS_AUTO)
				cur->domain_attr->data_progress = FI_PROGRESS_MANUAL;

			if (hints->ep_attr && hints->ep_attr->mem_tag_format &&
			    (info->caps & (FI_TAGGED | FI_COLLECTIVE))) {
				FI_INFO(&rxm_prov, FI_LOG_CORE,
					"mem_tag_format requested: 0x%" PRIx64
					" (note: provider doesn't optimize "
					"based on mem_tag_format)\n",
					hints->ep_attr->mem_tag_format);
				info->ep_attr->mem_tag_format =
					hints->ep_attr->mem_tag_format;
			}
		}
	}
}

static int rxm_validate_atomic_hints(const struct fi_info *hints)
{
	if (!hints || !(hints->caps & FI_ATOMIC))
		return 0;

	if (hints->tx_attr && (hints->tx_attr->msg_order &
			       RXM_ATOMIC_UNSUPPORTED_MSG_ORDER)) {
		FI_INFO(&rxm_prov, FI_LOG_CORE,
		        "Hints tx_attr msg_order not supported for atomics\n");
		return -FI_EINVAL;
	}
	if (hints->rx_attr && (hints->rx_attr->msg_order &
			       RXM_ATOMIC_UNSUPPORTED_MSG_ORDER)) {
		FI_INFO(&rxm_prov, FI_LOG_CORE,
		        "Hints rx_attr msg_order not supported for atomics\n");
		return -FI_EINVAL;
	}
	return 0;
}

static int rxm_getinfo(uint32_t version, const char *node, const char *service,
			uint64_t flags, const struct fi_info *hints,
			struct fi_info **info)
{
	struct fi_info *cur;
	struct addrinfo *ai;
	uint16_t port_save = 0;
	int ret;

	/* Avoid getting wild card address from MSG provider */
	if (ofi_is_wildcard_listen_addr(node, service, flags, hints)) {
		if (service) {
			ret = getaddrinfo(NULL, service, NULL, &ai);
			if (ret) {
				FI_WARN(&rxm_prov, FI_LOG_CORE,
					"Unable to getaddrinfo\n");
				return ret;
			}
			port_save = ofi_addr_get_port(ai->ai_addr);
			freeaddrinfo(ai);
			service = NULL;
			flags &= ~FI_SOURCE;
		} else {
			port_save = ofi_addr_get_port(hints->src_addr);
			ofi_addr_set_port(hints->src_addr, 0);
		}
	}
	ret = rxm_validate_atomic_hints(hints);
	if (ret)
		return ret;

	ret = ofix_getinfo(version, node, service, flags, &rxm_util_prov, hints,
			   rxm_info_to_core, rxm_info_to_rxm, info);
	if (ret)
		return ret;

	if (port_save) {
		for (cur = *info; cur; cur = cur->next) {
			assert(cur->src_addr);
			ofi_addr_set_port(cur->src_addr, port_save);
		}
	}

	rxm_alter_info(hints, *info);
	return 0;
}


static void rxm_fini(void)
{
#if HAVE_RXM_DL
	ofi_hmem_cleanup();
	ofi_mem_fini();
#endif
}

struct fi_provider rxm_prov = {
	.name = OFI_UTIL_PREFIX "rxm",
	.version = OFI_VERSION_DEF_PROV,
	.fi_version = OFI_VERSION_LATEST,
	.getinfo = rxm_getinfo,
	.fabric = rxm_fabric,
	.cleanup = rxm_fini
};

static void rxm_get_def_wait(void)
{
	char *wait_str = NULL;

	fi_param_define(&rxm_prov, "def_wait_obj", FI_PARAM_STRING,
			"Specifies the default wait object used for blocking "
			"operations (e.g. fi_cq_sread).  Supported values "
			"are: fd and pollfd (default: fd).");

	fi_param_define(&rxm_prov, "def_tcp_wait_obj", FI_PARAM_STRING,
			"See def_wait_obj for description.  If set, this "
			"overrides the def_wait_obj when running over the "
			"tcp provider.  See def_wait_obj for valid values. "
			"(default: UNSPEC, tcp provider will select).");

	fi_param_get_str(&rxm_prov, "def_wait_obj", &wait_str);
	if (wait_str && !strcasecmp(wait_str, "pollfd"))
		def_wait_obj = FI_WAIT_POLLFD;

	wait_str = NULL;
	fi_param_get_str(&rxm_prov, "def_tcp_wait_obj", &wait_str);
	if (wait_str) {
		def_tcp_wait_obj = (!strcasecmp(wait_str, "pollfd")) ?
				   FI_WAIT_POLLFD : FI_WAIT_FD;
	}
}

RXM_INI
{
	fi_param_define(&rxm_prov, "buffer_size", FI_PARAM_SIZE_T,
			"Defines the allocated buffer size used for bounce "
			"buffers, including buffers posted at the receive side "
			"to handle unexpected messages.  This value "
			"corresponds to the rxm inject limit, and is also "
			"typically used as the eager message size. "
			"(default %zu)", rxm_buffer_size);

	fi_param_define(&rxm_prov, "comp_per_progress", FI_PARAM_INT,
			"Defines the maximum number of MSG provider CQ entries "
			"(default: 1) that would be read per progress "
			"(RxM CQ read).");

	fi_param_define(&rxm_prov, "sar_limit", FI_PARAM_SIZE_T,
			"Specifies the maximum size transfer that the SAR "
			"Segmentation And Reassembly) protocol "
			"For transfers smaller than SAR, data may be copied "
			"into multiple bounce buffers on the transmit side "
			"and received into bounce buffers at the receiver. "
			"The sar_limit value must be greater than the "
			"eager_limit to take effect.  (default %zu).",
			rxm_buffer_size * 8);

	fi_param_define(&rxm_prov, "use_srx", FI_PARAM_BOOL,
			"Set this environment variable to control the RxM "
			"receive path. If this variable set to 1 (default: 0), "
			"the RxM uses Shared Receive Context. This mode improves "
			"memory consumption, but it may increase small message "
			"latency as a side-effect.");

	fi_param_define(&rxm_prov, "tx_size", FI_PARAM_SIZE_T,
			"Defines default tx context size (default: 2048).");

	fi_param_define(&rxm_prov, "rx_size", FI_PARAM_SIZE_T,
			"Defines default rx context size (default: 2048).");

	fi_param_define(&rxm_prov, "msg_tx_size", FI_PARAM_SIZE_T,
			"Defines FI_EP_MSG tx size that would be requested "
			"(default: 128).");

	fi_param_define(&rxm_prov, "msg_rx_size", FI_PARAM_SIZE_T,
			"Defines FI_EP_MSG rx or srx size that would be requested. "
			"(default: 128, 4096 with srx");

	fi_param_define(&rxm_prov, "cm_progress_interval", FI_PARAM_INT,
			"Defines the number of microseconds to wait between "
			"function calls to the connection management progression "
			"functions during fi_cq_read calls. Higher values may "
			"decrease noise during cq polling, but may result in "
			"longer connection establishment times. (default: 10000).");

	fi_param_define(&rxm_prov, "cq_eq_fairness", FI_PARAM_INT,
			"Defines the maximum number of message provider CQ entries "
			"that can be consecutively read across progress calls "
			"without checking to see if the CM progress interval has "
			"been reached. (default: 128).");

	fi_param_define(&rxm_prov, "data_auto_progress", FI_PARAM_BOOL,
			"Force auto-progress for data transfers even if app "
			"requested manual progress (default: false/no).");

	fi_param_define(&rxm_prov, "use_rndv_write", FI_PARAM_BOOL,
			"Set this environment variable to control the  "
			"RxM Rendezvous protocol.  If set (1), RxM will use "
			"RMA writes rather than RMA reads during Rendezvous "
			"transactions. (default: false/no).");

	fi_param_define(&rxm_prov, "enable_direct_send", FI_PARAM_BOOL,
			"Enable support to pass application buffers directly "
			"to the core provider when possible.  This avoids "
			"copying application buffers through bounce buffers "
			"before passing them to the core provider.  This "
			"feature targets small to medium size message "
			"transfers over the tcp provider.  (default: true)");

	fi_param_define(&rxm_prov, "enable_passthru", FI_PARAM_BOOL,
			"Enable passthru optimization.  Pass thru allows "
			"rxm to pass all data transfer calls directly to the "
			"core provider, which eliminates the rxm protocol and "
			"related overhead.  Pass thru is an optimized path "
			"to the tcp provider, depending on the capabilities "
			"requested by the application.");

	fi_param_define(&rxm_prov, "detect_hmem_iface", FI_PARAM_BOOL,
			"Detect iface for user buffers with NULL desc passed "
			"in. This allows such buffers be copied or registered "
			"internally by RxM. (default: false).");

	/* passthru supported disabled - to re-enable would need to fix call to
	 * fi_cq_read to pass in the correct data structure.  However, passthru
	 * will not be needed at all with in-work tcp changes.
	 */
	fi_param_get_bool(&rxm_prov, "enable_passthru", &rxm_passthru);

	rxm_init_infos();
	fi_param_get_size_t(&rxm_prov, "msg_tx_size", &rxm_msg_tx_size);
	fi_param_get_size_t(&rxm_prov, "msg_rx_size", &rxm_msg_rx_size);
	if (fi_param_get_int(&rxm_prov, "cm_progress_interval",
				(int *) &rxm_cm_progress_interval))
		rxm_cm_progress_interval = 10000;
	if (fi_param_get_int(&rxm_prov, "cq_eq_fairness",
				(int *) &rxm_cq_eq_fairness))
		rxm_cq_eq_fairness = 128;
	fi_param_get_bool(&rxm_prov, "data_auto_progress", &force_auto_progress);
	fi_param_get_bool(&rxm_prov, "use_rndv_write", &rxm_use_write_rndv);

	rxm_get_def_wait();

	if (force_auto_progress)
		FI_INFO(&rxm_prov, FI_LOG_CORE, "auto-progress for data requested "
			"(FI_OFI_RXM_DATA_AUTO_PROGRESS = 1), domain threading "
			"level would be set to FI_THREAD_SAFE\n");

	fi_param_get_bool(&rxm_prov, "detect_hmem_iface", &rxm_detect_hmem_iface);

#if HAVE_RXM_DL
	ofi_mem_init();
	ofi_hmem_init();
#endif

	return &rxm_prov;
}
