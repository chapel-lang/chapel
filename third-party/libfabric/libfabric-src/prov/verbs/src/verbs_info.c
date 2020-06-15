/*
 * Copyright (c) 2013-2015 Intel Corporation, Inc.  All rights reserved.
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

#include <ofi_util.h>

#include <ifaddrs.h>
#include <net/if.h>
#include <stdint.h>

#include "fi_verbs.h"


#define VERBS_IB_PREFIX "IB-0x"
#define VERBS_IWARP_FABRIC "Ethernet-iWARP"

#define VERBS_DOMAIN_CAPS (FI_LOCAL_COMM | FI_REMOTE_COMM)

#define VERBS_MSG_TX_CAPS (OFI_TX_MSG_CAPS | OFI_TX_RMA_CAPS | FI_ATOMICS)
#define VERBS_MSG_RX_CAPS (OFI_RX_MSG_CAPS | OFI_RX_RMA_CAPS | FI_ATOMICS)
#define VERBS_MSG_CAPS (VERBS_MSG_TX_CAPS | VERBS_MSG_RX_CAPS | VERBS_DOMAIN_CAPS)
#define VERBS_DGRAM_TX_CAPS (OFI_TX_MSG_CAPS)
#define VERBS_DGRAM_RX_CAPS (OFI_RX_MSG_CAPS)
#define VERBS_DGRAM_CAPS (VERBS_DGRAM_TX_CAPS | VERBS_DGRAM_RX_CAPS | \
			  VERBS_DOMAIN_CAPS)

#define VERBS_DGRAM_RX_MODE (FI_MSG_PREFIX)

#define VERBS_TX_OP_FLAGS_IWARP (FI_INJECT | FI_INJECT_COMPLETE | FI_COMPLETION)
#define VERBS_TX_OP_FLAGS (VERBS_TX_OP_FLAGS_IWARP | FI_TRANSMIT_COMPLETE)

#define VERBS_RX_MODE (FI_RX_CQ_DATA)

#define VERBS_MSG_ORDER (OFI_ORDER_RAR_SET | OFI_ORDER_RAW_SET | FI_ORDER_RAS | \
		OFI_ORDER_WAW_SET | FI_ORDER_WAS | FI_ORDER_SAW | FI_ORDER_SAS )

#define VERBS_INFO_NODE_2_UD_ADDR(sybsys, node, svc, ib_ud_addr)			\
	VERBS_INFO(sybsys, "'%s:%u' resolved to <gid <interface_id=%"PRIu64		\
			   ", subnet_prefix=%"PRIu64">, lid=%d, service = %u>\n",	\
		   node, svc, be64toh((ib_ud_addr)->gid.global.interface_id),		\
		   be64toh((ib_ud_addr)->gid.global.subnet_prefix),			\
		   (ib_ud_addr)->lid, (ib_ud_addr)->service)

const struct fi_fabric_attr verbs_fabric_attr = {
	.prov_version		= OFI_VERSION_DEF_PROV,
};

const struct fi_domain_attr verbs_domain_attr = {
	.caps			= VERBS_DOMAIN_CAPS,
	.threading		= FI_THREAD_SAFE,
	.control_progress	= FI_PROGRESS_AUTO,
	.data_progress		= FI_PROGRESS_AUTO,
	.resource_mgmt		= FI_RM_ENABLED,
	.mr_mode		= OFI_MR_BASIC_MAP | FI_MR_LOCAL | FI_MR_BASIC,
	.mr_key_size		= sizeof_field(struct ibv_sge, lkey),
	.cq_data_size		= sizeof_field(struct ibv_send_wr, imm_data),
	.tx_ctx_cnt		= 1024,
	.rx_ctx_cnt		= 1024,
	.max_ep_tx_ctx		= 1,
	.max_ep_rx_ctx		= 1,
	.mr_iov_limit		= VERBS_MR_IOV_LIMIT,
	/* max_err_data is size of ibv_wc::vendor_err for CQ, UINT8_MAX - for EQ */
	.max_err_data		= MAX(sizeof_field(struct ibv_wc, vendor_err),
				      UINT8_MAX),
};

const struct fi_ep_attr verbs_ep_attr = {
	.protocol_version	= 1,
	.msg_prefix_size	= 0,
	.max_order_war_size	= 0,
	.mem_tag_format		= 0,
	.tx_ctx_cnt		= 1,
	.rx_ctx_cnt		= 1,
};

const struct fi_rx_attr verbs_rx_attr = {
	.caps			= VERBS_MSG_RX_CAPS,
	.mode			= VERBS_RX_MODE,
	.op_flags		= FI_COMPLETION,
	.msg_order		= VERBS_MSG_ORDER,
	.comp_order		= FI_ORDER_STRICT | FI_ORDER_DATA,
	.total_buffered_recv	= 0,
};

const struct fi_rx_attr verbs_dgram_rx_attr = {
	.caps			= VERBS_DGRAM_RX_CAPS,
	.mode			= VERBS_DGRAM_RX_MODE | VERBS_RX_MODE,
	.op_flags		= FI_COMPLETION,
	.msg_order		= VERBS_MSG_ORDER,
	.comp_order		= FI_ORDER_STRICT | FI_ORDER_DATA,
	.total_buffered_recv	= 0,
};

const struct fi_tx_attr verbs_tx_attr = {
	.caps			= VERBS_MSG_TX_CAPS,
	.mode			= 0,
	.op_flags		= VERBS_TX_OP_FLAGS,
	.msg_order		= VERBS_MSG_ORDER,
	.comp_order		= FI_ORDER_STRICT,
	.inject_size		= 0,
	.rma_iov_limit		= 1,
};

const struct fi_tx_attr verbs_dgram_tx_attr = {
	.caps			= VERBS_DGRAM_TX_CAPS,
	.mode			= 0,
	.op_flags		= VERBS_TX_OP_FLAGS,
	.msg_order		= VERBS_MSG_ORDER,
	.comp_order		= FI_ORDER_STRICT,
	.inject_size		= 0,
	.rma_iov_limit		= 1,
};

const struct verbs_ep_domain verbs_msg_domain = {
	.suffix			= "",
	.type			= FI_EP_MSG,
	.protocol		= FI_PROTO_UNSPEC,
};

const struct verbs_ep_domain verbs_msg_xrc_domain = {
	.suffix			= "-xrc",
	.type			= FI_EP_MSG,
	.protocol		= FI_PROTO_RDMA_CM_IB_XRC,
};

const struct verbs_ep_domain verbs_dgram_domain = {
	.suffix			= "-dgram",
	.type			= FI_EP_DGRAM,
	.protocol		= FI_PROTO_UNSPEC,
};

/* The list (not thread safe) is populated once when the provider is initialized */
DEFINE_LIST(verbs_devs);

int vrb_check_ep_attr(const struct fi_info *hints,
			 const struct fi_info *info)
{
	struct fi_info *user_hints;
	struct util_prov tmp_util_prov = {
		.prov = &vrb_prov,
		.info = NULL,
		.flags = (info->domain_attr->max_ep_srx_ctx &&
			  info->ep_attr->type == FI_EP_MSG) ?
			 UTIL_RX_SHARED_CTX : 0,
	};
	int ret;

	switch (hints->ep_attr->protocol) {
	case FI_PROTO_UNSPEC:
	case FI_PROTO_RDMA_CM_IB_RC:
	case FI_PROTO_RDMA_CM_IB_XRC:
	case FI_PROTO_IWARP:
	case FI_PROTO_IB_UD:
		break;
	default:
		VERBS_INFO(FI_LOG_CORE,
			   "Unsupported protocol\n");
		return -FI_ENODATA;
	}

	user_hints = fi_dupinfo(hints);
	if (!user_hints)
		return -FI_ENOMEM;

	/*
	 * verbs provider requires more complex verification of the
	 * protocol in compare to verification that is presented in
	 * the utility function. Change the protocol to FI_PROTO_UNSPEC
	 * to avoid verification of protocol in the ofi_check_ep_attr
	 */
	user_hints->ep_attr->protocol = FI_PROTO_UNSPEC;

	ret = ofi_check_ep_attr(&tmp_util_prov, info->fabric_attr->api_version,
				info, user_hints);
	fi_freeinfo(user_hints);
	return ret;
}

int vrb_check_rx_attr(const struct fi_rx_attr *attr,
			 const struct fi_info *hints,
			 const struct fi_info *info)
{
	struct fi_info *dup_info;
	int ret;

	if ((hints->domain_attr && hints->domain_attr->cq_data_size) ||
	    (hints->rx_attr && hints->rx_attr->mode & FI_RX_CQ_DATA) ||
	    hints->mode & FI_RX_CQ_DATA) {
		ret = ofi_check_rx_attr(&vrb_prov, info, attr, hints->mode);
	} else {
		dup_info = fi_dupinfo(info);
		if (!dup_info)
			return -FI_ENOMEM;

		dup_info->rx_attr->mode &= ~FI_RX_CQ_DATA;
		ret = ofi_check_rx_attr(&vrb_prov, dup_info, attr,
					hints->mode);
		fi_freeinfo(dup_info);
	}
	return ret;
}

static int vrb_check_hints(uint32_t version, const struct fi_info *hints,
			      const struct fi_info *info)
{
	int ret;
	uint64_t prov_mode;

	if (hints->caps & ~(info->caps)) {
		VERBS_INFO(FI_LOG_CORE, "Unsupported capabilities\n");
		FI_INFO_CHECK(&vrb_prov, info, hints, caps, FI_TYPE_CAPS);
		return -FI_ENODATA;
	}

	prov_mode = ofi_mr_get_prov_mode(version, hints, info);

	if ((hints->mode & prov_mode) != prov_mode) {
		VERBS_INFO(FI_LOG_CORE, "needed mode not set\n");
		FI_INFO_MODE(&vrb_prov, prov_mode, hints->mode);
		return -FI_ENODATA;
	}

	if (hints->fabric_attr) {
		ret = ofi_check_fabric_attr(&vrb_prov, info->fabric_attr,
					    hints->fabric_attr);
		if (ret)
			return ret;
	}

	if (hints->domain_attr) {
		if (hints->domain_attr->name &&
		    strcasecmp(hints->domain_attr->name, info->domain_attr->name)) {
			VERBS_INFO(FI_LOG_CORE, "skipping device %s (want %s)\n",
				   info->domain_attr->name, hints->domain_attr->name);
			return -FI_ENODATA;
		}

		ret = ofi_check_domain_attr(&vrb_prov, version,
					    info->domain_attr,
					    hints);
		if (ret)
			return ret;
	}

	if (hints->ep_attr) {
		ret = vrb_check_ep_attr(hints, info);
		if (ret)
			return ret;
	}

	if (hints->rx_attr) {
		ret = vrb_check_rx_attr(hints->rx_attr, hints, info);
		if (ret)
			return ret;
	}

	if (hints->tx_attr) {
		ret = ofi_check_tx_attr(&vrb_prov, info->tx_attr,
					hints->tx_attr, hints->mode);
		if (ret)
			return ret;
	}

	return FI_SUCCESS;
}

int vrb_fi_to_rai(const struct fi_info *fi, uint64_t flags,
		     struct rdma_addrinfo *rai)
{
	memset(rai, 0, sizeof *rai);
	if (flags & FI_SOURCE)
		rai->ai_flags = RAI_PASSIVE;
	if (flags & FI_NUMERICHOST)
		rai->ai_flags |= RAI_NUMERICHOST;

	rai->ai_qp_type = IBV_QPT_RC;
	rai->ai_port_space = RDMA_PS_TCP;

	if (!fi)
		return 0;

	switch(fi->addr_format) {
	case FI_SOCKADDR_IN:
	case FI_FORMAT_UNSPEC:
		rai->ai_family = AF_INET;
		rai->ai_flags |= RAI_FAMILY;
		break;
	case FI_SOCKADDR_IN6:
		rai->ai_family = AF_INET6;
		rai->ai_flags |= RAI_FAMILY;
		break;
	case FI_SOCKADDR_IB:
		rai->ai_family = AF_IB;
		rai->ai_flags |= RAI_FAMILY;
		break;
	case FI_SOCKADDR:
		if (fi->src_addrlen) {
			rai->ai_family = ((struct sockaddr *)fi->src_addr)->sa_family;
			rai->ai_flags |= RAI_FAMILY;
		} else if (fi->dest_addrlen) {
			rai->ai_family = ((struct sockaddr *)fi->dest_addr)->sa_family;
			rai->ai_flags |= RAI_FAMILY;
		}
		break;
	default:
		VERBS_INFO(FI_LOG_FABRIC, "Unknown fi->addr_format\n");
	}

	if (fi->src_addrlen) {
		if (!(rai->ai_src_addr = malloc(fi->src_addrlen)))
			return -FI_ENOMEM;
		memcpy(rai->ai_src_addr, fi->src_addr, fi->src_addrlen);
		rai->ai_src_len = fi->src_addrlen;
	}
	if (fi->dest_addrlen) {
		if (!(rai->ai_dst_addr = malloc(fi->dest_addrlen)))
			return -FI_ENOMEM;
		memcpy(rai->ai_dst_addr, fi->dest_addr, fi->dest_addrlen);
		rai->ai_dst_len = fi->dest_addrlen;
	}

	return 0;
}

static inline
void *vrb_dgram_ep_name_to_string(const struct ofi_ib_ud_ep_name *name,
				     size_t *len)
{
	char *str;
	if (!name)
		return NULL;

	*len = sizeof(struct ofi_ib_ud_ep_name);

	str = calloc(*len, 1);
	if (!str)
		return NULL;

	if (!ofi_straddr((void *)str, len, FI_ADDR_IB_UD, name)) {
		free(str);
		return NULL;
	}

	return str;
}

static int vrb_fill_addr_by_ep_name(struct ofi_ib_ud_ep_name *ep_name,
				       uint32_t fmt, void **addr, size_t *addrlen)
{
	if (fmt == FI_ADDR_STR) {
		*addr = vrb_dgram_ep_name_to_string(ep_name, addrlen);
		if (!*addr)
			return -FI_ENOMEM;
	} else {
		*addr = calloc(1, sizeof(*ep_name));
		if (!*addr)
			return -FI_ENOMEM;
		memcpy(*addr, ep_name, sizeof(*ep_name));
		*addrlen = sizeof(*ep_name);
	}

	return FI_SUCCESS;
}

static int vrb_rai_to_fi(struct rdma_addrinfo *rai, struct fi_info *fi)
{
	if (!rai)
		return FI_SUCCESS;

	fi->addr_format = ofi_translate_addr_format(rai->ai_family);
	if (fi->addr_format == FI_FORMAT_UNSPEC) {
		VERBS_WARN(FI_LOG_FABRIC, "Unknown address format\n");
		return -FI_EINVAL;
	}

	if (rai->ai_src_len) {
		free(fi->src_addr);
 		if (!(fi->src_addr = malloc(rai->ai_src_len)))
 			return -FI_ENOMEM;
 		memcpy(fi->src_addr, rai->ai_src_addr, rai->ai_src_len);
 		fi->src_addrlen = rai->ai_src_len;
 	}
 	if (rai->ai_dst_len) {
		free(fi->dest_addr);
		if (!(fi->dest_addr = malloc(rai->ai_dst_len)))
			return -FI_ENOMEM;
 		memcpy(fi->dest_addr, rai->ai_dst_addr, rai->ai_dst_len);
 		fi->dest_addrlen = rai->ai_dst_len;
 	}

 	return FI_SUCCESS;
}

static inline int vrb_get_qp_cap(struct ibv_context *ctx,
				    struct fi_info *info, uint32_t protocol)
{
	struct ibv_pd *pd;
	struct ibv_cq *cq;
	struct ibv_qp *qp;
	struct ibv_qp_init_attr init_attr;
	enum ibv_qp_type qp_type;
	int ret = 0;

	pd = ibv_alloc_pd(ctx);
	if (!pd) {
		VERBS_INFO_ERRNO(FI_LOG_FABRIC, "ibv_alloc_pd", errno);
		return -errno;
	}

	cq = ibv_create_cq(ctx, 1, NULL, NULL, 0);
	if (!cq) {
		VERBS_INFO_ERRNO(FI_LOG_FABRIC, "ibv_create_cq", errno);
		ret = -errno;
		goto err1;
	}

	if (protocol == FI_PROTO_RDMA_CM_IB_XRC)
		qp_type = IBV_QPT_XRC_SEND;
	else
		qp_type = (info->ep_attr->type != FI_EP_DGRAM) ?
				    IBV_QPT_RC : IBV_QPT_UD;

	memset(&init_attr, 0, sizeof init_attr);
	init_attr.send_cq = cq;

	assert(info->tx_attr->size &&
	       info->tx_attr->iov_limit &&
	       info->rx_attr->size &&
	       info->rx_attr->iov_limit);

	init_attr.cap.max_send_wr = MIN(vrb_gl_data.def_tx_size,
					info->tx_attr->size);
	init_attr.cap.max_send_sge = MIN(vrb_gl_data.def_tx_iov_limit,
					 info->tx_attr->iov_limit);

	if (qp_type != IBV_QPT_XRC_SEND) {
		init_attr.recv_cq = cq;
		init_attr.cap.max_recv_wr = MIN(vrb_gl_data.def_rx_size,
						info->rx_attr->size);
		init_attr.cap.max_recv_sge = MIN(vrb_gl_data.def_rx_iov_limit,
						 info->rx_attr->iov_limit);
	}
	init_attr.cap.max_inline_data = vrb_find_max_inline(pd, ctx, qp_type);
	init_attr.qp_type = qp_type;

	qp = ibv_create_qp(pd, &init_attr);
	if (!qp) {
		VERBS_INFO_ERRNO(FI_LOG_FABRIC, "ibv_create_qp", errno);
		ret = -errno;
		goto err2;
	}

	info->tx_attr->inject_size = init_attr.cap.max_inline_data;

	ibv_destroy_qp(qp);
err2:
	ibv_destroy_cq(cq);
err1:
	ibv_dealloc_pd(pd);

	return ret;
}

static int vrb_mtu_type_to_len(enum ibv_mtu mtu_type)
{
	switch (mtu_type) {
	case IBV_MTU_256:
		return 256;
	case IBV_MTU_512:
		return 512;
	case IBV_MTU_1024:
		return 1024;
	case IBV_MTU_2048:
		return 2048;
	case IBV_MTU_4096:
		return 4096;
	default:
		return -FI_EINVAL;
	}
}

static enum fi_link_state vrb_pstate_2_lstate(enum ibv_port_state pstate)
{
	switch (pstate) {
	case IBV_PORT_DOWN:
	case IBV_PORT_INIT:
	case IBV_PORT_ARMED:
		return FI_LINK_DOWN;
	case IBV_PORT_ACTIVE:
		return FI_LINK_UP;
	default:
		return FI_LINK_UNKNOWN;
	}
}

static const char *vrb_link_layer_str(uint8_t link_layer)
{
	switch (link_layer) {
	case IBV_LINK_LAYER_UNSPECIFIED:
	case IBV_LINK_LAYER_INFINIBAND:
		return "InfiniBand";
	case IBV_LINK_LAYER_ETHERNET:
		return "Ethernet";
	default:
		return "Unknown";
	}
}

static size_t vrb_speed(uint8_t speed, uint8_t width)
{
	const size_t gbit_2_bit_coef = 1024 * 1024;
	size_t width_val, speed_val;

	switch (speed) {
	case 1:
		speed_val = (size_t) (2.5 * (float) gbit_2_bit_coef);
		break;
	case 2:
		speed_val = 5 * gbit_2_bit_coef;
		break;
	case 4:
	case 8:
		speed_val = 8 * gbit_2_bit_coef;
		break;
	case 16:
		speed_val = 14 * gbit_2_bit_coef;
		break;
	case 32:
		speed_val = 25 * gbit_2_bit_coef;
		break;
	default:
		speed_val = 0;
		break;
	}

	switch (width) {
	case 1:
		width_val = 1;
		break;
	case 2:
		width_val = 4;
		break;
	case 4:
		width_val = 8;
		break;
	case 8:
		width_val = 12;
		break;
	default:
		width_val = 0;
		break;
	}

	return width_val * speed_val;
}


static int vrb_get_device_attrs(struct ibv_context *ctx,
				   struct fi_info *info, uint32_t protocol)
{
	struct ibv_device_attr device_attr;
	struct ibv_port_attr port_attr;
	size_t max_sup_size;
	int ret = 0, mtu_size;
	uint8_t port_num;
	enum fi_log_level level =
		vrb_gl_data.msg.prefer_xrc ? FI_LOG_WARN : FI_LOG_INFO;
	const char *dev_name = ibv_get_device_name(ctx->device);

	ret = ibv_query_device(ctx, &device_attr);
	if (ret) {
		VERBS_INFO_ERRNO(FI_LOG_FABRIC,
				 "ibv_query_device", errno);
		return -errno;
	}

	if (protocol == FI_PROTO_RDMA_CM_IB_XRC) {
		if (!(device_attr.device_cap_flags & IBV_DEVICE_XRC)) {
			FI_LOG(&vrb_prov, level, FI_LOG_FABRIC,
			       "XRC support unavailable in device: %s\n",
			       dev_name);
			return -FI_EINVAL;
		}
	}

	info->domain_attr->cq_cnt 		= device_attr.max_cq;
	info->domain_attr->ep_cnt 		= device_attr.max_qp;
	info->domain_attr->tx_ctx_cnt 		= MIN(info->domain_attr->tx_ctx_cnt,
						      device_attr.max_qp);
	info->domain_attr->rx_ctx_cnt 		= MIN(info->domain_attr->rx_ctx_cnt,
						      device_attr.max_qp);
	info->domain_attr->max_ep_tx_ctx 	= MIN(info->domain_attr->tx_ctx_cnt,
						      device_attr.max_qp);
	info->domain_attr->max_ep_rx_ctx 	= MIN(info->domain_attr->rx_ctx_cnt,
						      device_attr.max_qp);
	info->domain_attr->max_ep_srx_ctx	= device_attr.max_srq;
	info->domain_attr->mr_cnt		= device_attr.max_mr;
	info->tx_attr->size 			= device_attr.max_qp_wr;
	info->tx_attr->iov_limit 		= device_attr.max_sge;

	info->rx_attr->size 			= device_attr.max_srq_wr ?
						  MIN(device_attr.max_qp_wr,
						      device_attr.max_srq_wr) :
						  device_attr.max_qp_wr;
	// TODO set one of srq sge or regular sge based on hints?
	info->rx_attr->iov_limit 		= device_attr.max_srq_sge ?
						  MIN(device_attr.max_sge,
						      device_attr.max_srq_sge) :
						  device_attr.max_sge;
	if (protocol == FI_PROTO_RDMA_CM_IB_XRC) {
		info->rx_attr->iov_limit = MIN(info->rx_attr->iov_limit, 1);
		info->ep_attr->rx_ctx_cnt = FI_SHARED_CONTEXT;
	}

	ret = vrb_get_qp_cap(ctx, info, protocol);
	if (ret)
		return ret;

	for (port_num = 1; port_num < device_attr.phys_port_cnt + 1; port_num++) {
		ret = ibv_query_port(ctx, port_num, &port_attr);
		if (ret) {
			VERBS_INFO_ERRNO(FI_LOG_FABRIC,
					 "ibv_query_port", errno);
			return -errno;
		}
		if (port_attr.state == IBV_PORT_ACTIVE)
			break;
	}

	if (port_num == device_attr.phys_port_cnt + 1) {
		FI_WARN(&vrb_prov, FI_LOG_FABRIC, "device %s: there are no "
			"active ports\n", dev_name);
		return -FI_ENODATA;
	} else {
		VERBS_INFO(FI_LOG_FABRIC, "device %s: first found active port "
			   "is %"PRIu8"\n", dev_name, port_num);
	}

	if (info->ep_attr->type == FI_EP_DGRAM) {
		ret = vrb_mtu_type_to_len(port_attr.active_mtu);
		if (ret < 0) {
			VERBS_WARN(FI_LOG_FABRIC, "device %s (port: %d) reports"
				   " an unrecognized MTU (%d) \n",
				   dev_name, port_num, port_attr.active_mtu);
			return ret;
		}
		max_sup_size = MIN(ret, port_attr.max_msg_sz);
	} else {
		max_sup_size = port_attr.max_msg_sz;
	}

	info->ep_attr->max_msg_size 		= max_sup_size;
	info->ep_attr->max_order_raw_size 	= max_sup_size;
	info->ep_attr->max_order_waw_size	= max_sup_size;

	ret = asprintf(&info->nic->device_attr->device_id, "0x%04x",
		       device_attr.vendor_part_id);
	if (ret < 0) {
		info->nic->device_attr->device_id = NULL;
		VERBS_WARN(FI_LOG_FABRIC,
			   "Unable to allocate memory for device_attr::device_id\n");
		return -FI_ENOMEM;
	}

	ret = asprintf(&info->nic->device_attr->vendor_id, "0x%04x",
		       device_attr.vendor_id);
	if (ret < 0) {
		info->nic->device_attr->vendor_id = NULL;
		VERBS_WARN(FI_LOG_FABRIC,
			   "Unable to allocate memory for device_attr::vendor_id\n");
		return -FI_ENOMEM;
	}

	ret = asprintf(&info->nic->device_attr->device_version, "%"PRIu32,
		       device_attr.hw_ver);
	if (ret < 0) {
		info->nic->device_attr->device_version = NULL;
		VERBS_WARN(FI_LOG_FABRIC,
			   "Unable to allocate memory for device_attr::device_version\n");
		return -FI_ENOMEM;
	}

        info->nic->device_attr->firmware = strdup(device_attr.fw_ver);
	if (!info->nic->device_attr->firmware) {
		VERBS_WARN(FI_LOG_FABRIC,
			   "Unable to allocate memory for device_attr::firmware\n");
		return -FI_ENOMEM;
	}

	mtu_size = vrb_mtu_type_to_len(port_attr.active_mtu);
	info->nic->link_attr->mtu = (size_t) (mtu_size > 0 ? mtu_size : 0);
	info->nic->link_attr->speed = vrb_speed(port_attr.active_speed,
						   port_attr.active_width);
	info->nic->link_attr->state =
		vrb_pstate_2_lstate(port_attr.state);
	info->nic->link_attr->network_type =
		strdup(vrb_link_layer_str(port_attr.link_layer));
	if (!info->nic->link_attr->network_type) {
		VERBS_WARN(FI_LOG_FABRIC,
			   "Unable to allocate memory for link_attr::network_type\n");
		return -FI_ENOMEM;
	}

	return 0;
}

/*
 * USNIC plugs into the verbs framework, but is not a usable device.
 * Manually check for devices and fail gracefully if none are present.
 * This avoids the lower libraries (libibverbs and librdmacm) from
 * reporting error messages to stderr.
 */
static int vrb_have_device(void)
{
	struct ibv_device **devs;
	struct ibv_context *verbs;
	struct ibv_device_attr attr;
	const int AWS_VENDOR_ID = 0x1d0f;
	int i, ret = 0;

	devs = ibv_get_device_list(NULL);
	if (!devs)
		return 0;

	for (i = 0; devs[i]; i++) {
		verbs = ibv_open_device(devs[i]);
		if (verbs) {
			ret = ibv_query_device(verbs, &attr);
			ibv_close_device(verbs);
			/*
			 * According to the librdmacm library interface,
			 * rdma_get_devices() in vrb_init_info leaves devices
			 * open even after rdma_free_devices() is called,
			 * causing failure in efa provider.
			 * Also, efa and verb devices are not expected to
			 * co-exist on a system. If its an efa device, then it
			 * should be handled by the efa provider.
			 */
			if (!ret && (attr.vendor_id != AWS_VENDOR_ID)) {
				ret = 1;
				break;
			}
		}
	}

	ibv_free_device_list(devs);
	return ret;
}

static int vrb_alloc_info(struct ibv_context *ctx, struct fi_info **info,
			     const struct verbs_ep_domain *ep_dom)
{
	struct fi_info *fi;
	union ibv_gid gid;
	size_t name_len;
	const char *dev_name = ibv_get_device_name(ctx->device);
	int ret;

	if ((ctx->device->transport_type != IBV_TRANSPORT_IB) &&
	    ((ep_dom->type == FI_EP_DGRAM) ||
	    (ep_dom->protocol == FI_PROTO_RDMA_CM_IB_XRC)))
		return -FI_EINVAL;

	fi = fi_allocinfo();
	if (!fi)
		return -FI_ENOMEM;

	fi->handle = NULL;
	*(fi->ep_attr) = verbs_ep_attr;
	*(fi->domain_attr) = verbs_domain_attr;

	switch (ep_dom->type) {
	case FI_EP_MSG:
		fi->caps = VERBS_MSG_CAPS;
		*(fi->tx_attr) = verbs_tx_attr;
		*(fi->rx_attr) = verbs_rx_attr;
		break;
	case FI_EP_DGRAM:
		fi->caps = VERBS_DGRAM_CAPS;
		fi->mode = VERBS_DGRAM_RX_MODE;
		*(fi->tx_attr) = verbs_dgram_tx_attr;
		*(fi->rx_attr) = verbs_dgram_rx_attr;
		fi->ep_attr->msg_prefix_size = VERBS_DGRAM_MSG_PREFIX_SIZE;
		break;
	default:
		assert(0);
		return -FI_EINVAL;
	}
		

	*(fi->fabric_attr) = verbs_fabric_attr;

	fi->ep_attr->type = ep_dom->type;

	fi->nic = ofi_nic_dup(NULL);
	if (!fi->nic) {
		ret = -FI_ENOMEM;
		goto err;
	}

	fi->nic->device_attr->name = strdup(dev_name);
	if (!fi->nic->device_attr->name) {
		ret = -FI_ENOMEM;
		goto err;
	}

	ret = vrb_get_device_attrs(ctx, fi, ep_dom->protocol);
	if (ret)
		goto err;

	switch (ctx->device->transport_type) {
	case IBV_TRANSPORT_IB:
		if (ibv_query_gid(ctx, 1, 0, &gid)) {
			VERBS_INFO_ERRNO(FI_LOG_FABRIC,
					 "ibv_query_gid", errno);
			ret = -errno;
			goto err;
		}

		name_len = strlen(VERBS_IB_PREFIX) + INET6_ADDRSTRLEN;
		if (!(fi->fabric_attr->name = calloc(1, name_len + 1))) {
			ret = -FI_ENOMEM;
			goto err;
		}

		snprintf(fi->fabric_attr->name, name_len, VERBS_IB_PREFIX "%" PRIx64,
			 be64toh(gid.global.subnet_prefix));

		switch (ep_dom->type) {
		case FI_EP_MSG:
			fi->ep_attr->protocol =
				ep_dom->protocol == FI_PROTO_UNSPEC ?
				FI_PROTO_RDMA_CM_IB_RC : ep_dom->protocol;
			break;
		case FI_EP_DGRAM:
			fi->ep_attr->protocol = FI_PROTO_IB_UD;
			break;
		default:
			assert(0); /* Shouldn't go here */
			ret = -FI_EINVAL;
			goto err;
		}
		break;
	case IBV_TRANSPORT_IWARP:
		fi->fabric_attr->name = strdup(VERBS_IWARP_FABRIC);
		if (!fi->fabric_attr->name) {
			ret = -FI_ENOMEM;
			goto err;
		}
		fi->ep_attr->protocol = FI_PROTO_IWARP;
		fi->tx_attr->op_flags = VERBS_TX_OP_FLAGS_IWARP;

		/* TODO Some iWarp HW may support immediate data as per RFC 7306
		 * (RDMA Protocol Extensions). Update this to figure out if the
		 * hw supports immediate data dynamically */
		fi->domain_attr->cq_data_size = 0;
		break;
	default:
		VERBS_INFO(FI_LOG_CORE, "Unknown transport type\n");
		ret = -FI_ENODATA;
		goto err;
	}

	name_len = strlen(dev_name) + strlen(ep_dom->suffix);
	fi->domain_attr->name = calloc(1, name_len + 2);
	if (!fi->domain_attr->name) {
		ret = -FI_ENOMEM;
		goto err;
	}

	snprintf(fi->domain_attr->name, name_len + 1, "%s%s",
		 ctx->device->name, ep_dom->suffix);

	*info = fi;

	return 0;
err:
	fi_freeinfo(fi);
	return ret;
}

static void verbs_devs_print(void)
{
	struct verbs_dev_info *dev;
	struct verbs_addr *addr;
	char addr_str[INET6_ADDRSTRLEN];
	int i = 0;

	FI_INFO(&vrb_prov, FI_LOG_FABRIC,
		"list of verbs devices found for FI_EP_MSG:\n");
	dlist_foreach_container(&verbs_devs, struct verbs_dev_info,
				dev, entry) {
		FI_INFO(&vrb_prov, FI_LOG_FABRIC,
			"#%d %s - IPoIB addresses:\n", ++i, dev->name);
		dlist_foreach_container(&dev->addrs, struct verbs_addr,
					addr, entry) {
			if (!inet_ntop(addr->rai->ai_family,
				       ofi_get_ipaddr(addr->rai->ai_src_addr),
				       addr_str, INET6_ADDRSTRLEN))
				FI_INFO(&vrb_prov, FI_LOG_FABRIC,
					"unable to convert address to string\n");
			else
				FI_INFO(&vrb_prov, FI_LOG_FABRIC,
					"\t%s\n", addr_str);
		}
	}
}

static int verbs_devs_add(struct dlist_entry *verbs_devs, char *dev_name,
			  struct rdma_addrinfo *rai)
{
	struct verbs_dev_info *dev;
	struct verbs_addr *addr;

	if (!(addr = malloc(sizeof(*addr))))
		return -FI_ENOMEM;

	addr->rai = rai;

	dlist_foreach_container(verbs_devs, struct verbs_dev_info, dev, entry)
		if (!strcmp(dev_name, dev->name))
			goto add_rai;

	if (!(dev = malloc(sizeof(*dev))))
		goto err1;

	dev->name = dev_name;
	dlist_init(&dev->addrs);
	dlist_insert_tail(&dev->entry, verbs_devs);
add_rai:
	dlist_insert_tail(&addr->entry, &dev->addrs);
	return 0;
err1:
	free(addr);
	return -FI_ENOMEM;
}

#define IPV6_LINK_LOCAL_ADDR_PREFIX_STR "fe80"

static int vrb_ifa_rdma_info(const struct ifaddrs *ifa, char **dev_name,
				struct rdma_addrinfo **rai)
{
	char name[INET6_ADDRSTRLEN];
	struct rdma_addrinfo rai_hints = {
		.ai_flags = RAI_PASSIVE | RAI_NUMERICHOST,
	}, *rai_;
	struct rdma_cm_id *id;
	int ret;

	if (!inet_ntop(ifa->ifa_addr->sa_family, ofi_get_ipaddr(ifa->ifa_addr),
		       name, INET6_ADDRSTRLEN))
		return -errno;

	ret = rdma_create_id(NULL, &id, NULL, RDMA_PS_TCP);
	if (ret)
		return ret;

	/* Detect if the IPv6 address is link local.
	 * TODO should we do something similar for IPv4? */
	if (!strncmp(name, IPV6_LINK_LOCAL_ADDR_PREFIX_STR,
		     strlen(IPV6_LINK_LOCAL_ADDR_PREFIX_STR))) {
		assert(strlen(name) + strlen(ifa->ifa_name) < INET6_ADDRSTRLEN);
		strcat(name, "%");
		strcat(name, ifa->ifa_name);
	}

	ret = rdma_getaddrinfo((char *) name, NULL, &rai_hints, &rai_);
	if (ret) {
		ret = -errno;
		FI_DBG(&vrb_prov, FI_LOG_FABRIC, "rdma_getaddrinfo failed "
		       "with error code: %d (%s) for interface %s with address:"
		       " %s\n", -ret, strerror(-ret), ifa->ifa_name, name);
		goto err1;
	}

	ret = rdma_bind_addr(id, rai_->ai_src_addr);
	if (ret) {
		ret = -errno;
		FI_DBG(&vrb_prov, FI_LOG_FABRIC, "rdma_bind_addr failed "
		       "with error code: %d (%s) for interface %s with address:"
		       " %s\n", -ret, strerror(-ret), ifa->ifa_name, name);
		goto err2;
	}

	if (!id->verbs) {
		ret = -FI_EINVAL;
		goto err2;
	}

	*dev_name = strdup(ibv_get_device_name(id->verbs->device));
	if (!(*dev_name)) {
		ret = -FI_ENOMEM;
		goto err2;
	}

	rdma_destroy_id(id);
	*rai = rai_;
	return 0;
err2:
	rdma_freeaddrinfo(rai_);
err1:
	rdma_destroy_id(id);
	return ret;
}

/* Builds a list of interfaces that correspond to active verbs devices */
static int vrb_getifaddrs(struct dlist_entry *verbs_devs)
{
	struct ifaddrs *ifaddr, *ifa;
	struct rdma_addrinfo *rai = NULL;
	char *dev_name = NULL;
	char *iface = vrb_gl_data.iface;
	int ret, num_verbs_ifs = 0;
	size_t iface_len = 0;
	int exact_match = 0;

	ret = ofi_getifaddrs(&ifaddr);
	if (ret) {
		VERBS_WARN(FI_LOG_FABRIC,
			   "unable to get interface addresses\n");
		return ret;
	}

	/* select best iface name based on user's input */
	if (iface) {
		iface_len = strlen(iface);
		if (iface_len > IFNAMSIZ) {
			VERBS_INFO(FI_LOG_FABRIC, "iface name: %s, too long "
				   "max: %d\n", iface, IFNAMSIZ);

		}
		for (ifa = ifaddr; ifa && !exact_match; ifa = ifa->ifa_next)
			exact_match = !strcmp(ifa->ifa_name, iface);
	}

	for (ifa = ifaddr; ifa; ifa = ifa->ifa_next) {
		if (!ifa->ifa_addr || !(ifa->ifa_flags & IFF_UP) ||
				!strcmp(ifa->ifa_name, "lo"))
			continue;

		if (iface) {
			if (exact_match) {
				if (strcmp(ifa->ifa_name, iface)) {
					FI_INFO(&vrb_prov, FI_LOG_FABRIC,
						"skipping interface: %s for FI_EP_MSG"
						" as it doesn't match filter: %s\n",
						ifa->ifa_name, iface);
					continue;
				}
			} else {
				if (strncmp(ifa->ifa_name, iface, iface_len)) {
					FI_INFO(&vrb_prov, FI_LOG_FABRIC,
						"skipping interface: %s for FI_EP_MSG"
						" as it doesn't match filter: %s\n",
						ifa->ifa_name, iface);
					continue;
				}
			}
		}

		ret = vrb_ifa_rdma_info(ifa, &dev_name, &rai);
		if (ret)
			continue;

		ret = verbs_devs_add(verbs_devs, dev_name, rai);
		if (ret) {
			free(dev_name);
			rdma_freeaddrinfo(rai);
			continue;
		}
		num_verbs_ifs++;
	}

	verbs_devs_print();

	freeifaddrs(ifaddr);
	return num_verbs_ifs ? 0 : -FI_ENODATA;
}

static int
vrb_info_add_dev_addr(struct fi_info **info, struct verbs_dev_info *dev)
{
	struct fi_info *add_info;
	struct verbs_addr *addr;
	int ret;

	dlist_foreach_container(&dev->addrs, struct verbs_addr, addr, entry) {
		/* When a device has multiple interfaces/addresses configured
		 * duplicate fi_info and add the address info. fi->src_addr
		 * would have been set in the previous iteration */
		if ((*info)->src_addr) {
			if (!(add_info = fi_dupinfo(*info)))
				return -FI_ENOMEM;

			add_info->next = (*info)->next;
			(*info)->next = add_info;
			*info = add_info;
		}

		ret = vrb_rai_to_fi(addr->rai, *info);
		if (ret)
			return ret;
	}
	return 0;
}

static int vrb_get_srcaddr_devs(struct fi_info **info)
{
	struct verbs_dev_info *dev;
	struct fi_info *fi;
	int ret;

	for (fi = *info; fi; fi = fi->next) {
		if (fi->ep_attr->type == FI_EP_DGRAM)
			continue;
		dlist_foreach_container(&verbs_devs, struct verbs_dev_info,
					dev, entry) {
			/* strncmp because we want to process XRC fi_info as
			 * well which have a "-xrc" suffix in domain name */
			if (!strncmp(fi->domain_attr->name, dev->name,
				     strlen(dev->name))) {
				ret = vrb_info_add_dev_addr(&fi, dev);
				if (ret)
					return ret;
				break;
			}
		}
	}
	return 0;
}

static void vrb_sockaddr_set_port(struct sockaddr *sa, uint16_t port)
{
	switch(sa->sa_family) {
	case AF_INET:
		((struct sockaddr_in *)sa)->sin_port = port;
		break;
	case AF_INET6:
		((struct sockaddr_in6 *)sa)->sin6_port = port;
		break;
	}
}

/* the `rai` parameter is used for the MSG EP type */
/* the `fmt`, `[src | dest]_addr` parameters are used for the DGRAM EP type */
/* if the `fmt` parameter isn't used, pass FI_FORMAT_UNSPEC */
static int vrb_set_info_addrs(struct fi_info *info,
				 struct rdma_addrinfo *rai,
				 uint32_t fmt,
				 struct ofi_ib_ud_ep_name *src_addr,
				 struct ofi_ib_ud_ep_name *dest_addr)
{
	struct fi_info *iter_info = info;
	int ret;

	for (; iter_info; iter_info = iter_info->next) {
		if (iter_info->ep_attr->type != FI_EP_DGRAM) {
			ret = vrb_rai_to_fi(rai, iter_info);
			if (ret)
				return ret;
		} else {
			if (src_addr) {
				ret = vrb_fill_addr_by_ep_name(src_addr, fmt,
								  &iter_info->src_addr,
								  &iter_info->src_addrlen);
				if (ret)
					return ret;
			}
			if (dest_addr) {
				ret = vrb_fill_addr_by_ep_name(dest_addr, fmt,
								  &iter_info->dest_addr,
								  &iter_info->dest_addrlen);
				if (ret)
					return ret;
			}
			iter_info->addr_format = FI_ADDR_IB_UD;
		}
	}

	return FI_SUCCESS;
}

static int vrb_fill_addr(struct rdma_addrinfo *rai, struct fi_info **info,
			    struct rdma_cm_id *id)
{
	struct sockaddr *local_addr;

	/*
	 * TODO MPICH CH3 doesn't work with verbs provider without skipping the
	 * loopback address. An alternative approach if there is one is needed
	 * to allow both.
	 */
	if (rai->ai_src_addr && !ofi_is_loopback_addr(rai->ai_src_addr))
		goto rai_to_fi;

	if (!id->verbs)
		return vrb_get_srcaddr_devs(info);

	/* Handle the case when rdma_cm doesn't fill src address even
	 * though it fills the destination address (presence of id->verbs
	 * corresponds to a valid dest addr) */
	local_addr = rdma_get_local_addr(id);
	if (!local_addr) {
		VERBS_WARN(FI_LOG_CORE,
			   "Unable to get local address\n");
		return -FI_ENODATA;
	}

	rai->ai_src_len = vrb_sockaddr_len(local_addr);
	if (!(rai->ai_src_addr = malloc(rai->ai_src_len)))
		return -FI_ENOMEM;

	memcpy(rai->ai_src_addr, local_addr, rai->ai_src_len);
	/* User didn't specify a port. Zero out the random port
	 * assigned by rdmamcm so that this rai/fi_info can be
	 * used multiple times to create rdma endpoints.*/
	vrb_sockaddr_set_port(rai->ai_src_addr, 0);

rai_to_fi:
	return vrb_set_info_addrs(*info, rai, FI_FORMAT_UNSPEC,
				     NULL, NULL);
}

static int vrb_device_has_ipoib_addr(const char *dev_name)
{
	struct verbs_dev_info *dev;

	dlist_foreach_container(&verbs_devs, struct verbs_dev_info, dev, entry) {
		if (!strcmp(dev_name, dev->name))
			return 1;
	}
	return 0;
}

#define VERBS_NUM_DOMAIN_TYPES		3

int vrb_init_info(const struct fi_info **all_infos)
{
	struct ibv_context **ctx_list;
	struct fi_info *fi = NULL, *tail = NULL;
	const struct verbs_ep_domain *ep_type[VERBS_NUM_DOMAIN_TYPES];
	int ret = 0, i, j, num_devices, dom_count = 0;

	*all_infos = NULL;

	if (!vrb_have_device()) {
		VERBS_INFO(FI_LOG_FABRIC, "no RDMA devices found\n");
		ret = -FI_ENODATA;
		goto done;
	}

	/* List XRC MSG_EP domain before default RC MSG_EP if requested */
	if (vrb_gl_data.msg.prefer_xrc) {
		if (VERBS_HAVE_XRC)
			ep_type[dom_count++] = &verbs_msg_xrc_domain;
		else
			FI_WARN(&vrb_prov, FI_LOG_FABRIC,
				"XRC not built into provider, skip allocating "
				"fi_info for XRC FI_EP_MSG endpoints\n");
	}

	vrb_getifaddrs(&verbs_devs);

	if (dlist_empty(&verbs_devs))
		FI_WARN(&vrb_prov, FI_LOG_FABRIC,
			"no valid IPoIB interfaces found, FI_EP_MSG endpoint "
			"type would not be available\n");
	else
		ep_type[dom_count++] = &verbs_msg_domain;

	if (!vrb_gl_data.msg.prefer_xrc && VERBS_HAVE_XRC)
		ep_type[dom_count++] = &verbs_msg_xrc_domain;

	ep_type[dom_count++] = &verbs_dgram_domain;

	ctx_list = rdma_get_devices(&num_devices);
	if (!num_devices) {
		VERBS_INFO_ERRNO(FI_LOG_FABRIC, "rdma_get_devices", errno);
		ret = -errno;
		goto done;
	}

	for (i = 0; i < num_devices; i++) {
		for (j = 0; j < dom_count; j++) {
			if (ep_type[j]->type == FI_EP_MSG &&
			    !vrb_device_has_ipoib_addr(ctx_list[i]->device->name)) {
				FI_INFO(&vrb_prov, FI_LOG_FABRIC,
					"skipping device: %s for FI_EP_MSG, "
					"it may have a filtered IPoIB interface"
					" (FI_VERBS_IFACE) or it may not have a"
					" valid IP address configured\n",
					ctx_list[i]->device->name);
				continue;
			}
			if (vrb_gl_data.device_name &&
			    strncasecmp(ctx_list[i]->device->name,
					vrb_gl_data.device_name,
					strlen(vrb_gl_data.device_name)))
				continue;

			ret = vrb_alloc_info(ctx_list[i], &fi, ep_type[j]);
			if (!ret) {
				if (!*all_infos)
					*all_infos = fi;
				else
					tail->next = fi;
				tail = fi;
			}
		}
	}

	/* note we are possibly discarding ENOMEM */
	ret = *all_infos ? 0 : ret;

	rdma_free_devices(ctx_list);
done:
	return ret;
}

static void vrb_set_default_attr(size_t *attr, size_t default_attr)
{
	if (default_attr <= *attr)
		*attr = default_attr;
}

/* Set default values for attributes. ofi_alter_info would change them if the
 * user has asked for a different value in hints */
static void vrb_set_default_info(struct fi_info *info)
{
	vrb_set_default_attr(&info->tx_attr->size,
				vrb_gl_data.def_tx_size);

	vrb_set_default_attr(&info->rx_attr->size,
				vrb_gl_data.def_rx_size);

	vrb_set_default_attr(&info->tx_attr->iov_limit,
				vrb_gl_data.def_tx_iov_limit);
	vrb_set_default_attr(&info->rx_attr->iov_limit,
				vrb_gl_data.def_rx_iov_limit);

	if (info->ep_attr->type == FI_EP_MSG) {
		/* For verbs iov limit is same for
		 * both regular messages and RMA */
		vrb_set_default_attr(&info->tx_attr->rma_iov_limit,
					vrb_gl_data.def_tx_iov_limit);
	}
}

static struct fi_info *vrb_get_passive_info(const struct fi_info *prov_info,
					       const struct fi_info *hints)
{
	struct fi_info *info;

	if (!(info = fi_dupinfo(hints)))
		return NULL;

	info->mode = prov_info->mode;
	info->tx_attr->mode = prov_info->tx_attr->mode;
	info->rx_attr->mode = prov_info->rx_attr->mode;
	info->ep_attr->type = prov_info->ep_attr->type;

	info->domain_attr->domain 	= prov_info->domain_attr->domain;
	if (!info->domain_attr->name)
		info->domain_attr->name = strdup(VERBS_ANY_DOMAIN);
	info->domain_attr->mr_mode 	= prov_info->domain_attr->mr_mode;
	info->domain_attr->mode 	= prov_info->domain_attr->mode;

	info->fabric_attr->fabric = prov_info->fabric_attr->fabric;
	if (!info->fabric_attr->name)
		info->fabric_attr->name = strdup(VERBS_ANY_FABRIC);

	/* prov_name is set by libfabric core */
	free(info->fabric_attr->prov_name);
	info->fabric_attr->prov_name = NULL;
	return info;
}

int vrb_get_matching_info(uint32_t version, const struct fi_info *hints,
			     struct fi_info **info, const struct fi_info *verbs_info,
			     uint8_t passive)
{
	const struct fi_info *check_info = verbs_info;
	struct fi_info *fi, *tail;
	int ret, i;
	uint8_t got_passive_info = 0;
	enum fi_log_level level =
		vrb_gl_data.msg.prefer_xrc ? FI_LOG_WARN : FI_LOG_INFO;

	*info = tail = NULL;

	for (i = 1; check_info; check_info = check_info->next, i++) {
		if (hints) {
			FI_INFO(&vrb_prov, FI_LOG_FABRIC,
				"checking domain: #%d %s\n",
				i, check_info->domain_attr->name);

			if (hints->ep_attr) {
				/* check EP type first to avoid other unnecessary checks */
				ret = ofi_check_ep_type(
					&vrb_prov, check_info->ep_attr, hints->ep_attr);
				if (ret)
					continue;
			}

			ret = vrb_check_hints(version, hints,
						 check_info);
			if (ret)
				continue;

			if ((check_info->ep_attr->protocol ==
			     FI_PROTO_RDMA_CM_IB_XRC) &&
			    (!hints->ep_attr ||
			     (hints->ep_attr->rx_ctx_cnt != FI_SHARED_CONTEXT))) {
				FI_LOG(&vrb_prov, level, FI_LOG_FABRIC,
				       "hints->ep_attr->rx_ctx_cnt != "
				       "FI_SHARED_CONTEXT. Skipping "
				       "XRC FI_EP_MSG endpoints\n");
				continue;
			}
		}

		if ((check_info->ep_attr->type == FI_EP_MSG) && passive) {
			if (got_passive_info)
				continue;

			if (!(fi = vrb_get_passive_info(check_info, hints))) {
				ret = -FI_ENOMEM;
				goto err;
			}
			got_passive_info = 1;
		} else {
			if (!(fi = fi_dupinfo(check_info))) {
				ret = -FI_ENOMEM;
				goto err;
			}
			vrb_set_default_info(fi);
		}

		FI_INFO(&vrb_prov, FI_LOG_FABRIC,
			"adding fi_info for domain: %s\n", fi->domain_attr->name);
		if (!*info)
			*info = fi;
		else
			tail->next = fi;
		tail = fi;
	}

	if (!*info)
		return -FI_ENODATA;

	return FI_SUCCESS;
err:
	fi_freeinfo(*info);
	return ret;
}

static int vrb_del_info_not_belong_to_dev(const char *dev_name, struct fi_info **info)
{
	struct fi_info *check_info = *info;
	struct fi_info *cur, *prev = NULL;

	*info = NULL;

	while (check_info) {
		/* Use strncmp since verbs domain names would have "-<ep_type>" suffix */
		if (dev_name && strncmp(dev_name, check_info->domain_attr->name,
					strlen(dev_name))) {
			/* This branch removing `check_info` entry from the list */
			cur = check_info;
			if (prev)
				prev->next = check_info->next;
			check_info = check_info->next;

			cur->next = NULL;
			fi_freeinfo(cur);
		} else {
			prev = check_info;
			if (!*info)
				/* if find the first matched `fi_info` entry,
				 * then save this to original list */
				*info = check_info;
			check_info = check_info->next;
		}
	}

	if (!*info)
		return -FI_ENODATA;

	return FI_SUCCESS;
}

static int vrb_resolve_ib_ud_dest_addr(const char *node, const char *service,
					  struct ofi_ib_ud_ep_name **dest_addr)
{
	int svc = VERBS_IB_UD_NS_ANY_SERVICE;
	struct util_ns ns = {
		.port = vrb_gl_data.dgram.name_server_port,
		.name_len = sizeof(**dest_addr),
		.service_len = sizeof(svc),
		.service_cmp = vrb_dgram_ns_service_cmp,
		.is_service_wildcard = vrb_dgram_ns_is_service_wildcard,
	};

	ofi_ns_init(&ns);

	if (service)
		svc = atoi(service);
	*dest_addr = (struct ofi_ib_ud_ep_name *)
		ofi_ns_resolve_name(&ns, node, &svc);
	if (*dest_addr) {
		VERBS_INFO_NODE_2_UD_ADDR(FI_LOG_CORE, node, svc, *dest_addr);
	} else {
		VERBS_INFO(FI_LOG_CORE,
			   "failed to resolve '%s:%u'.\n", node, svc);
		return -FI_ENODATA;
	}

	return 0;
}

static void vrb_delete_dgram_infos(struct fi_info **info)
{
	struct fi_info *check_info = *info;
	struct fi_info *cur, *prev = NULL;

	*info = NULL;

	while (check_info) {
		if (check_info->ep_attr->type == FI_EP_DGRAM) {
			cur = check_info;
			if (prev)
				prev->next = check_info->next;
			check_info = check_info->next;

			cur->next = NULL;
			fi_freeinfo(cur);
		} else {
			prev = check_info;
			if (!*info)
				*info = check_info;
			check_info = check_info->next;
		}
	}
}

static int vrb_handle_ib_ud_addr(const char *node, const char *service,
				    uint64_t flags, struct fi_info **info)
{
	struct ofi_ib_ud_ep_name *dest_addr = NULL;
	struct ofi_ib_ud_ep_name *src_addr = NULL;
	void *addr = NULL;
	size_t len = 0;
	uint32_t fmt = FI_FORMAT_UNSPEC;
	int svc = VERBS_IB_UD_NS_ANY_SERVICE, ret = FI_SUCCESS;

	if (node && !ofi_str_toaddr(node, &fmt, &addr, &len) &&
	    fmt == FI_ADDR_IB_UD) {
		if (flags & FI_SOURCE) {
			src_addr = addr;
			VERBS_INFO_NODE_2_UD_ADDR(FI_LOG_CORE, node,
						  svc, src_addr);
		} else {
			dest_addr = addr;
			VERBS_INFO_NODE_2_UD_ADDR(FI_LOG_CORE, node,
						  svc, dest_addr);
		}
		node = NULL;
	}

	if (!src_addr) {
		src_addr = calloc(1, sizeof(*src_addr));
		if (!src_addr) {
			VERBS_INFO(FI_LOG_CORE,
			           "failed to allocate src addr.\n");
			ret = -FI_ENODATA;
			goto err;
		}

		if (flags & FI_SOURCE) {
			if (service) {
				ret = sscanf(service, "%" SCNu16,
					     &src_addr->service);
				if (ret != 1) {
					ret = -errno;
					goto err;
				}
			}

			VERBS_INFO(FI_LOG_CORE, "node '%s' service '%s' "
				                "converted to <service=%d>\n",
				   node, service, src_addr->service);
		}
	}

	if (!dest_addr && node && !(flags & FI_SOURCE)) {
		ret = vrb_resolve_ib_ud_dest_addr(node, service, &dest_addr);
		if (ret)
			goto err; /* Here possible that `src_addr` isn't a NULL */
	}

	ret = vrb_set_info_addrs(*info, NULL, fmt, src_addr, dest_addr);
	if  (!ret)
		goto out;
err:
	vrb_delete_dgram_infos(info);
	/* `fi_info::src_addr` and `fi_info::dest_addr` is freed
	 * in the `fi_freeinfo` function in case of failure */
out:
	if (src_addr)
		free(src_addr);
	if (dest_addr)
		free(dest_addr);
	return ret;
}

static int vrb_handle_sock_addr(const char *node, const char *service,
				   uint64_t flags, const struct fi_info *hints,
				   struct fi_info **info)
{
	struct rdma_cm_id *id = NULL;
	struct rdma_addrinfo *rai;
	const char *dev_name = NULL;
	int ret;

	ret = vrb_get_rai_id(node, service, flags, hints, &rai, &id);
	if (ret)
		return ret;
	if (id->verbs) {
		dev_name = ibv_get_device_name(id->verbs->device);
		ret = vrb_del_info_not_belong_to_dev(dev_name, info);
		if (ret)
			goto out;
	}

	ret = vrb_fill_addr(rai, info, id);
out:
	rdma_freeaddrinfo(rai);
	if (rdma_destroy_id(id))
		VERBS_INFO_ERRNO(FI_LOG_FABRIC, "rdma_destroy_id", errno);
	return ret;
}

static int vrb_get_match_infos(uint32_t version, const char *node,
				  const char *service, uint64_t flags,
				  const struct fi_info *hints,
				  const struct fi_info **raw_info,
				  struct fi_info **info)
{
	int ret, ret_sock_addr = -FI_ENODATA, ret_ib_ud_addr = -FI_ENODATA;

	// TODO check for AF_IB addr
	ret = vrb_get_matching_info(version, hints, info, *raw_info,
				       ofi_is_wildcard_listen_addr(node, service,
								   flags, hints));
	if (ret)
		return ret;

	if (!hints || !hints->ep_attr || hints->ep_attr->type == FI_EP_MSG ||
	    hints->ep_attr->type == FI_EP_UNSPEC) {
		ret_sock_addr = vrb_handle_sock_addr(node, service, flags, hints, info);
		if (ret_sock_addr) {
			VERBS_INFO(FI_LOG_FABRIC,
				   "handling of the socket address fails - %d\n",
				   ret_sock_addr);
		} else {
			if (!*info)
				return -FI_ENODATA;
		}
	}

	if (!hints || !hints->ep_attr || hints->ep_attr->type == FI_EP_DGRAM ||
	    hints->ep_attr->type == FI_EP_UNSPEC) {
		ret_ib_ud_addr = vrb_handle_ib_ud_addr(node, service, flags, info);
		if (ret_ib_ud_addr)
			VERBS_INFO(FI_LOG_FABRIC,
				   "handling of the IB ID address fails - %d\n",
				   ret_ib_ud_addr);
	}

	if (ret_sock_addr && ret_ib_ud_addr) {
		/* neither the sockaddr nor the ib_ud address wasn't
		 * handled to satisfy the selection procedure */
		VERBS_INFO(FI_LOG_CORE, "Handling of the addresses fails, "
			   "the getting infos is unsuccessful\n");
		fi_freeinfo(*info);
		return -FI_ENODATA;
	}

	return FI_SUCCESS;
}

void vrb_alter_info(const struct fi_info *hints, struct fi_info *info)
{
	struct fi_info *cur;

	if (!ofi_check_rx_mode(hints, FI_RX_CQ_DATA)) {
		for (cur = info; cur; cur = cur->next)
			cur->domain_attr->cq_data_size = 0;
	} else {
		for (cur = info; cur; cur = cur->next) {
			/* App may just set rx_attr.mode */
			if (!hints || (hints->mode & FI_RX_CQ_DATA))
				cur->mode |= FI_RX_CQ_DATA;
			assert(cur->rx_attr->mode & FI_RX_CQ_DATA);
		}
	}

	if (!hints || !hints->tx_attr || !hints->tx_attr->inject_size) {
		for (cur = info; cur; cur = cur->next) {
			if (cur->ep_attr->type != FI_EP_MSG)
				continue;
			/* The default inline size is usually smaller.
			 * This is to avoid drop in throughput */
			cur->tx_attr->inject_size =
				MIN(cur->tx_attr->inject_size,
				    vrb_gl_data.def_inline_size);
		}
	}
}

int vrb_getinfo(uint32_t version, const char *node, const char *service,
		   uint64_t flags, const struct fi_info *hints,
		   struct fi_info **info)
{
	int ret;

	ret = vrb_get_match_infos(version, node, service,
				     flags, hints,
				     &vrb_util_prov.info, info);
	if (ret)
		goto out;

	ofi_alter_info(*info, hints, version);

	vrb_alter_info(hints, *info);
out:
	if (!ret || ret == -FI_ENOMEM || ret == -FI_ENODEV)
		return ret;
	else
		return -FI_ENODATA;
}
