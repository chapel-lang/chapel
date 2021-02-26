/*
 * Copyright (c) 2016 Intel Corporation. All rights reserved.
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

#define RXM_ATOMIC_UNSUPPORTED_MSG_ORDER (FI_ORDER_RAW | FI_ORDER_ATOMIC_RAW | \
					  FI_ORDER_RAR | FI_ORDER_ATOMIC_RAR | \
					  FI_ORDER_WAW | FI_ORDER_ATOMIC_WAW | \
					  FI_ORDER_WAR | FI_ORDER_ATOMIC_WAR | \
					  FI_ORDER_SAR | FI_ORDER_SAW)

#define RXM_PASSTHRU_CAPS (FI_MSG | FI_RMA | FI_SEND | FI_RECV |	\
			   FI_READ | FI_WRITE | FI_REMOTE_READ |	\
			   FI_REMOTE_WRITE)

size_t rxm_msg_tx_size		= 128;
size_t rxm_msg_rx_size		= 128;
size_t rxm_def_univ_size	= 256;
size_t rxm_eager_limit		= RXM_BUF_SIZE - sizeof(struct rxm_pkt);
int force_auto_progress		= 0;
enum fi_wait_obj def_wait_obj	= FI_WAIT_FD;

char *rxm_proto_state_str[] = {
	RXM_PROTO_STATES(OFI_STR)
};

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
	}
}

int rxm_info_to_core(uint32_t version, const struct fi_info *hints,
		     struct fi_info *core_info)
{
	int use_srx = 0;

	rxm_info_to_core_mr_modes(version, hints, core_info);

	core_info->mode |= FI_RX_CQ_DATA | FI_CONTEXT;

	if (hints) {
		core_info->caps = hints->caps & RXM_PASSTHRU_CAPS;
		if (hints->caps & (FI_ATOMIC | FI_TAGGED))
			core_info->caps |= FI_MSG | FI_SEND | FI_RECV;

		/* FI_RMA cap is needed for large message transfer protocol */
		if (core_info->caps & FI_MSG)
			core_info->caps |= FI_RMA | FI_READ | FI_REMOTE_READ;

		if (hints->domain_attr) {
			core_info->domain_attr->caps |= hints->domain_attr->caps;
			core_info->domain_attr->threading = hints->domain_attr->threading;
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
	}
	core_info->ep_attr->type = FI_EP_MSG;
	if (!fi_param_get_bool(&rxm_prov, "use_srx", &use_srx) && use_srx) {
		FI_DBG(&rxm_prov, FI_LOG_FABRIC,
		       "Requesting shared receive context from core provider\n");
		core_info->ep_attr->rx_ctx_cnt = FI_SHARED_CONTEXT;
	}

	core_info->tx_attr->op_flags &= ~RXM_TX_OP_FLAGS;
	core_info->tx_attr->size = rxm_msg_tx_size;

	core_info->rx_attr->op_flags &= ~FI_MULTI_RECV;
	core_info->rx_attr->size = rxm_msg_rx_size;

	return 0;
}

int rxm_info_to_rxm(uint32_t version, const struct fi_info *core_info,
		    struct fi_info *info)
{
	info->caps = rxm_info.caps;
	// TODO find which other modes should be filtered
	info->mode = (core_info->mode & ~FI_RX_CQ_DATA) | rxm_info.mode;

	info->tx_attr->caps		= rxm_info.tx_attr->caps;
	info->tx_attr->mode		= info->mode;
	info->tx_attr->msg_order 	= core_info->tx_attr->msg_order;
	info->tx_attr->comp_order 	= rxm_info.tx_attr->comp_order;
	info->tx_attr->inject_size	= rxm_info.tx_attr->inject_size;
	info->tx_attr->size 		= rxm_info.tx_attr->size;
	info->tx_attr->iov_limit 	= MIN(rxm_info.tx_attr->iov_limit,
					      core_info->tx_attr->iov_limit);
	info->tx_attr->rma_iov_limit	= MIN(rxm_info.tx_attr->rma_iov_limit,
					      core_info->tx_attr->rma_iov_limit);

	info->rx_attr->caps		= rxm_info.rx_attr->caps;
	info->rx_attr->mode		= info->rx_attr->mode & ~FI_RX_CQ_DATA;
	info->rx_attr->msg_order 	= core_info->rx_attr->msg_order;
	info->rx_attr->comp_order 	= rxm_info.rx_attr->comp_order;
	info->rx_attr->size 		= rxm_info.rx_attr->size;
	info->rx_attr->iov_limit 	= MIN(rxm_info.rx_attr->iov_limit,
					      core_info->rx_attr->iov_limit);

	*info->ep_attr = *rxm_info.ep_attr;
	info->ep_attr->max_msg_size = core_info->ep_attr->max_msg_size;
	info->ep_attr->max_order_raw_size = core_info->ep_attr->max_order_raw_size;
	info->ep_attr->max_order_war_size = core_info->ep_attr->max_order_war_size;
	info->ep_attr->max_order_waw_size = core_info->ep_attr->max_order_waw_size;

	*info->domain_attr = *rxm_info.domain_attr;
	info->domain_attr->mr_mode |= core_info->domain_attr->mr_mode;
	info->domain_attr->cq_data_size = MIN(core_info->domain_attr->cq_data_size,
					      rxm_info.domain_attr->cq_data_size);
	info->domain_attr->mr_key_size = core_info->domain_attr->mr_key_size;

	if (core_info->nic) {
		info->nic = ofi_nic_dup(core_info->nic);
		if (!info->nic)
			return -FI_ENOMEM;
	}

	return 0;
}

static int rxm_init_info(void)
{
	size_t param;

	if (!fi_param_get_size_t(&rxm_prov, "buffer_size", &param)) {
		if (param < sizeof(struct rxm_pkt) + sizeof(struct rxm_rndv_hdr)) {
			FI_WARN(&rxm_prov, FI_LOG_CORE,
				"Requested buffer size too small\n");
			return -FI_EINVAL;
		}

		rxm_eager_limit = param - sizeof(struct rxm_pkt);
	}
	rxm_info.tx_attr->inject_size = rxm_eager_limit;
	rxm_util_prov.info = &rxm_info;
	return 0;
}

static void rxm_alter_info(const struct fi_info *hints, struct fi_info *info)
{
	struct fi_info *cur;

	for (cur = info; cur; cur = cur->next) {
		/* RxM can support higher inject size without any big
		 * performance penalty even if app had requested lower value
		 * in hints. App is still free to reduce this when opening an
		 * endpoint. This overrides setting by ofi_alter_info */
		cur->tx_attr->inject_size = rxm_eager_limit;

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
			if (!(hints->caps & FI_SOURCE)) {
				cur->caps &= ~FI_SOURCE;
				cur->rx_attr->caps &= ~FI_SOURCE;
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
			    (info->caps & FI_TAGGED)) {
				FI_INFO(&rxm_prov, FI_LOG_CORE,
					"mem_tag_format requested: 0x%" PRIx64
					" (note: provider doesn't optimize "
					"based on mem_tag_format)\n",
					hints->ep_attr->mem_tag_format);
				info->ep_attr->mem_tag_format =
					hints->ep_attr->mem_tag_format;
			}
		}
		if (cur->domain_attr->data_progress == FI_PROGRESS_AUTO ||
		    force_auto_progress)
			cur->domain_attr->threading = FI_THREAD_SAFE;
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
	/* yawn */
}

struct fi_provider rxm_prov = {
	.name = OFI_UTIL_PREFIX "rxm",
	.version = OFI_VERSION_DEF_PROV,
	.fi_version = OFI_VERSION_LATEST,
	.getinfo = rxm_getinfo,
	.fabric = rxm_fabric,
	.cleanup = rxm_fini
};

static void rxm_param_get_def_wait(void)
{
	char *wait_str = NULL;

	fi_param_get_str(&rxm_prov, "def_wait_obj", &wait_str);
	if (wait_str && !strcasecmp(wait_str, "pollfd"))
		def_wait_obj = FI_WAIT_POLLFD;
}

RXM_INI
{
	fi_param_define(&rxm_prov, "buffer_size", FI_PARAM_SIZE_T,
			"Defines the transmit buffer size / inject size "
			"(default: 16 KB). Eager protocol would be used to "
			"transmit messages of size less than eager limit "
			"(FI_OFI_RXM_BUFFER_SIZE - RxM header size (%zu B)). "
			"Any message whose size is greater than eager limit would"
			" be transmitted via rendezvous or SAR "
			"(Segmentation And Reassembly) protocol depending on "
			"the value of FI_OFI_RXM_SAR_LIMIT). Also, transmit data "
			" would be copied up to eager limit.",
			sizeof(struct rxm_pkt));

	fi_param_define(&rxm_prov, "comp_per_progress", FI_PARAM_INT,
			"Defines the maximum number of MSG provider CQ entries "
			"(default: 1) that would be read per progress "
			"(RxM CQ read).");

	fi_param_define(&rxm_prov, "sar_limit", FI_PARAM_SIZE_T,
			"Set this environment variable to enable and control "
			"RxM SAR (Segmentation And Reassembly) protocol "
			"(default: 128 KB). This value should be set greater than "
			" eager limit (FI_OFI_RXM_BUFFER_SIZE - RxM protocol "
			"header size (%zu B)) for SAR to take effect. Messages "
			"of size greater than this would be transmitted via "
			"rendezvous protocol.", sizeof(struct rxm_pkt));

	fi_param_define(&rxm_prov, "use_srx", FI_PARAM_BOOL,
			"Set this environment variable to control the RxM "
			"receive path. If this variable set to 1 (default: 0), "
			"the RxM uses Shared Receive Context. This mode improves "
			"memory consumption, but it may increase small message "
			"latency as a side-effect.");

	fi_param_define(&rxm_prov, "tx_size", FI_PARAM_SIZE_T,
			"Defines default tx context size (default: 1024).");

	fi_param_define(&rxm_prov, "rx_size", FI_PARAM_SIZE_T,
			"Defines default rx context size (default: 1024).");

	fi_param_define(&rxm_prov, "msg_tx_size", FI_PARAM_SIZE_T,
			"Defines FI_EP_MSG tx size that would be requested "
			"(default: 128). Setting this to 0 would get default "
			"value defined by the MSG provider.");

	fi_param_define(&rxm_prov, "msg_rx_size", FI_PARAM_SIZE_T,
			"Defines FI_EP_MSG rx size that would be requested "
			"(default: 128). Setting this to 0 would get default "
			"value defined by the MSG provider.");

	fi_param_define(&rxm_prov, "cm_progress_interval", FI_PARAM_INT,
			"Defines the number of microseconds to wait between "
			"function calls to the connection management progression "
			"functions during fi_cq_read calls. Higher values may "
			"decrease noise during cq polling, but may result in "
			"longer connection establishment times. (default: 10000).");

	fi_param_define(&rxm_prov, "cq_eq_fairness", FI_PARAM_INT,
			"Defines the maximum number of message provider CQ entries"
			" that can be consecutively read across progress calls "
			"without checking to see if the CM progress interval has "
			"been reached. (default: 128).");

	fi_param_define(&rxm_prov, "data_auto_progress", FI_PARAM_BOOL,
			"Force auto-progress for data transfers even if app "
			"requested manual progress (default: false/no).");

	fi_param_define(&rxm_prov, "def_wait_obj", FI_PARAM_STRING,
			"Specifies the default wait object used for blocking "
			"operations (e.g. fi_cq_sread).  Supported values "
			"are: fd and pollfd (default: fd).");

	fi_param_get_size_t(&rxm_prov, "tx_size", &rxm_info.tx_attr->size);
	fi_param_get_size_t(&rxm_prov, "rx_size", &rxm_info.rx_attr->size);
	fi_param_get_size_t(&rxm_prov, "msg_tx_size", &rxm_msg_tx_size);
	fi_param_get_size_t(&rxm_prov, "msg_rx_size", &rxm_msg_rx_size);
	fi_param_get_size_t(NULL, "universe_size", &rxm_def_univ_size);
	if (fi_param_get_int(&rxm_prov, "cm_progress_interval",
				(int *) &rxm_cm_progress_interval))
		rxm_cm_progress_interval = 10000;
	if (fi_param_get_int(&rxm_prov, "cq_eq_fairness",
				(int *) &rxm_cq_eq_fairness))
		rxm_cq_eq_fairness = 128;
	fi_param_get_bool(&rxm_prov, "data_auto_progress", &force_auto_progress);
	rxm_param_get_def_wait();

	if (force_auto_progress)
		FI_INFO(&rxm_prov, FI_LOG_CORE, "auto-progress for data requested "
			"(FI_OFI_RXM_DATA_AUTO_PROGRESS = 1), domain threading "
			"level would be set to FI_THREAD_SAFE\n");

	if (rxm_init_info()) {
		FI_WARN(&rxm_prov, FI_LOG_CORE, "Unable to initialize rxm_info\n");
		return NULL;
	}

	return &rxm_prov;
}
