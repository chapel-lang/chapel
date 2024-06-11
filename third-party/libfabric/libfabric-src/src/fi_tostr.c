/*
 * Copyright (c) 2014-2017 Intel Corp., Inc.  All rights reserved.
 * Copyright (c) 2016 Cisco Systems, Inc.  All rights reserved.
 * Copyright (c) 2022 DataDirect Networks, Inc. All rights reserved.
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
 *	copyright notice, this list of conditions and the following
 *	disclaimer.
 *
 *      - Redistributions in binary form must reproduce the above
 *	copyright notice, this list of conditions and the following
 *	disclaimer in the documentation and/or other materials
 *	provided with the distribution.
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

#include "config.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdarg.h>
#include <inttypes.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "ofi.h"
#include "ofi_str.h"
#include "ofi_iov.h"
#include <rdma/fabric.h>
#include <rdma/fi_domain.h>
#include <rdma/fi_endpoint.h>
#include <rdma/fi_trigger.h>
#include <rdma/fi_collective.h>


/* Print fi_info and related structs, enums, OR_able flags, addresses.
 *
 * Each printable type should be well formatted YAML.
 *
 * A struct is a dictionary containing one key named after the struct tag
 * which contains a dictionary of member-value mappings. The struct member
 * keys are the field names (not the types).
 *
 * Enum values are currently just bare strings.
 * OR-able flags are a list of the values, ie: [ VAL1, VAL2 ]
 *
 * YAML does not contain tabs.
 * Indentation delineates lists and dictionaries (or they can be inline).
 *
 * Printing functions are generally named after this pattern:
 *
 * struct fi_info : ofi_tostr_info(..., struct fi_info, ...)
 * fi_info->caps  : ofi_tostr_caps(..., typeof(caps), ...)
 */


static void
ofi_tostr_fid(const char *label, char *buf, size_t len, const struct fid *fid)
{
	if (!fid || !FI_CHECK_OP(fid->ops, struct fi_ops, tostr))
		ofi_strncatf(buf, len, "%s%p\n", label, fid);
	else
		fid->ops->tostr(fid, buf, len - strnlen(buf, len));
}

static void ofi_tostr_opflags(char *buf, size_t len, uint64_t flags)
{
	IFFLAGSTRN(flags, FI_MULTICAST, len);

	IFFLAGSTRN(flags, FI_MULTI_RECV, len);
	IFFLAGSTRN(flags, FI_REMOTE_CQ_DATA, len);
	IFFLAGSTRN(flags, FI_MORE, len);
	IFFLAGSTRN(flags, FI_PEEK, len);
	IFFLAGSTRN(flags, FI_TRIGGER, len);
	IFFLAGSTRN(flags, FI_FENCE, len);

	IFFLAGSTRN(flags, FI_COMPLETION, len);
	IFFLAGSTRN(flags, FI_INJECT, len);
	IFFLAGSTRN(flags, FI_INJECT_COMPLETE, len);
	IFFLAGSTRN(flags, FI_TRANSMIT_COMPLETE, len);
	IFFLAGSTRN(flags, FI_DELIVERY_COMPLETE, len);
	IFFLAGSTRN(flags, FI_MATCH_COMPLETE, len);
	IFFLAGSTRN(flags, FI_AFFINITY, len);

	IFFLAGSTRN(flags, FI_CLAIM, len);
	IFFLAGSTRN(flags, FI_DISCARD, len);

	ofi_remove_comma(buf);
}

static void ofi_tostr_addr_format(char *buf, size_t len, uint32_t addr_format)
{
	switch (addr_format) {
	CASEENUMSTRN(FI_FORMAT_UNSPEC, len);
	CASEENUMSTRN(FI_SOCKADDR, len);
	CASEENUMSTRN(FI_SOCKADDR_IN, len);
	CASEENUMSTRN(FI_SOCKADDR_IN6, len);
	CASEENUMSTRN(FI_SOCKADDR_IB, len);
	CASEENUMSTRN(FI_ADDR_PSMX2, len);
	CASEENUMSTRN(FI_ADDR_MLX, len);
	CASEENUMSTRN(FI_ADDR_UCX, len);
	CASEENUMSTRN(FI_ADDR_STR, len);
	CASEENUMSTRN(FI_ADDR_IB_UD, len);
	CASEENUMSTRN(FI_ADDR_EFA, len);
	CASEENUMSTRN(FI_ADDR_PSMX3, len);
	CASEENUMSTRN(FI_ADDR_OPX, len);
	CASEENUMSTRN(FI_ADDR_CXI, len);
	default:
		ofi_strncatf(buf, len, "Unknown");
		break;
	}
}

static void ofi_tostr_progress(char *buf, size_t len, enum fi_progress progress)
{
	switch (progress) {
	CASEENUMSTRN(FI_PROGRESS_UNSPEC, len);
	CASEENUMSTRN(FI_PROGRESS_AUTO, len);
	CASEENUMSTRN(FI_PROGRESS_MANUAL, len);
	CASEENUMSTRN(FI_PROGRESS_CONTROL_UNIFIED, len);
	default:
		ofi_strncatf(buf, len, "Unknown");
		break;
	}
}

static void
ofi_tostr_threading(char *buf, size_t len, enum fi_threading threading)
{
	switch (threading) {
	CASEENUMSTRN(FI_THREAD_UNSPEC, len);
	CASEENUMSTRN(FI_THREAD_SAFE, len);
	CASEENUMSTRN(FI_THREAD_FID, len);
	CASEENUMSTRN(FI_THREAD_DOMAIN, len);
	CASEENUMSTRN(FI_THREAD_COMPLETION, len);
	CASEENUMSTRN(FI_THREAD_ENDPOINT, len);
	default:
		ofi_strncatf(buf, len, "Unknown");
		break;
	}
}

static void ofi_tostr_msgorder(char *buf, size_t len, uint64_t flags)
{
	IFFLAGSTRN(flags, FI_ORDER_RAR, len);
	IFFLAGSTRN(flags, FI_ORDER_RAW, len);
	IFFLAGSTRN(flags, FI_ORDER_RAS, len);
	IFFLAGSTRN(flags, FI_ORDER_WAR, len);
	IFFLAGSTRN(flags, FI_ORDER_WAW, len);
	IFFLAGSTRN(flags, FI_ORDER_WAS, len);
	IFFLAGSTRN(flags, FI_ORDER_SAR, len);
	IFFLAGSTRN(flags, FI_ORDER_SAW, len);
	IFFLAGSTRN(flags, FI_ORDER_SAS, len);
	IFFLAGSTRN(flags, FI_ORDER_RMA_RAR, len);
	IFFLAGSTRN(flags, FI_ORDER_RMA_RAW, len);
	IFFLAGSTRN(flags, FI_ORDER_RMA_WAR, len);
	IFFLAGSTRN(flags, FI_ORDER_RMA_WAW, len);
	IFFLAGSTRN(flags, FI_ORDER_ATOMIC_RAR, len);
	IFFLAGSTRN(flags, FI_ORDER_ATOMIC_RAW, len);
	IFFLAGSTRN(flags, FI_ORDER_ATOMIC_WAR, len);
	IFFLAGSTRN(flags, FI_ORDER_ATOMIC_WAW, len);

	ofi_remove_comma(buf);
}

static void ofi_tostr_comporder(char *buf, size_t len, uint64_t flags)
{
	if ((flags & FI_ORDER_STRICT) == FI_ORDER_NONE) {
		ofi_strncatf(buf, len, "FI_ORDER_NONE, ");
	} else if ((flags & FI_ORDER_STRICT) == FI_ORDER_STRICT) {
		ofi_strncatf(buf, len, "FI_ORDER_STRICT, ");
	}

	IFFLAGSTRN(flags, FI_ORDER_DATA, len);

	ofi_remove_comma(buf);
}

static void ofi_tostr_caps(char *buf, size_t len, uint64_t caps)
{
	IFFLAGSTRN(caps, FI_MSG, len);
	IFFLAGSTRN(caps, FI_RMA, len);
	IFFLAGSTRN(caps, FI_TAGGED, len);
	IFFLAGSTRN(caps, FI_ATOMIC, len);
	IFFLAGSTRN(caps, FI_MULTICAST, len);
	IFFLAGSTRN(caps, FI_COLLECTIVE, len);

	IFFLAGSTRN(caps, FI_READ, len);
	IFFLAGSTRN(caps, FI_WRITE, len);
	IFFLAGSTRN(caps, FI_RECV, len);
	IFFLAGSTRN(caps, FI_SEND, len);
	IFFLAGSTRN(caps, FI_REMOTE_READ, len);
	IFFLAGSTRN(caps, FI_REMOTE_WRITE, len);

	IFFLAGSTRN(caps, FI_MULTI_RECV, len);
	IFFLAGSTRN(caps, FI_REMOTE_CQ_DATA, len);
	IFFLAGSTRN(caps, FI_TRIGGER, len);
	IFFLAGSTRN(caps, FI_FENCE, len);

	IFFLAGSTRN(caps, FI_RMA_PMEM, len);
	IFFLAGSTRN(caps, FI_SOURCE_ERR, len);
	IFFLAGSTRN(caps, FI_LOCAL_COMM, len);
	IFFLAGSTRN(caps, FI_REMOTE_COMM, len);
	IFFLAGSTRN(caps, FI_SHARED_AV, len);
	IFFLAGSTRN(caps, FI_RMA_EVENT, len);
	IFFLAGSTRN(caps, FI_SOURCE, len);
	IFFLAGSTRN(caps, FI_NAMED_RX_CTX, len);
	IFFLAGSTRN(caps, FI_DIRECTED_RECV, len);
	IFFLAGSTRN(caps, FI_HMEM, len);

	ofi_remove_comma(buf);
}

static void ofi_tostr_ep_type(char *buf, size_t len, enum fi_ep_type ep_type)
{
	switch (ep_type) {
	CASEENUMSTRN(FI_EP_UNSPEC, len);
	CASEENUMSTRN(FI_EP_MSG, len);
	CASEENUMSTRN(FI_EP_DGRAM, len);
	CASEENUMSTRN(FI_EP_RDM, len);
	default:
		ofi_strncatf(buf, len, "Unknown");
		break;
	}
}

static void ofi_tostr_protocol(char *buf, size_t len, uint32_t protocol)
{
	switch (protocol) {
	CASEENUMSTRN(FI_PROTO_UNSPEC, len);
	CASEENUMSTRN(FI_PROTO_RDMA_CM_IB_RC, len);
	CASEENUMSTRN(FI_PROTO_IWARP, len);
	CASEENUMSTRN(FI_PROTO_IB_UD, len);
	CASEENUMSTRN(FI_PROTO_PSMX2, len);
	CASEENUMSTRN(FI_PROTO_UDP, len);
	CASEENUMSTRN(FI_PROTO_SOCK_TCP, len);
	CASEENUMSTRN(FI_PROTO_IB_RDM, len);
	CASEENUMSTRN(FI_PROTO_IWARP_RDM, len);
	CASEENUMSTRN(FI_PROTO_RXM, len);
	CASEENUMSTRN(FI_PROTO_RXD, len);
	CASEENUMSTRN(FI_PROTO_MLX, len);
	CASEENUMSTRN(FI_PROTO_UCX, len);
	CASEENUMSTRN(FI_PROTO_NETWORKDIRECT, len);
	CASEENUMSTRN(FI_PROTO_SHM, len);
	CASEENUMSTRN(FI_PROTO_RDMA_CM_IB_XRC, len);
	CASEENUMSTRN(FI_PROTO_EFA, len);
	CASEENUMSTRN(FI_PROTO_PSMX3, len);
	CASEENUMSTRN(FI_PROTO_RXM_TCP, len);
	CASEENUMSTRN(FI_PROTO_OPX, len);
	CASEENUMSTRN(FI_PROTO_CXI, len);
	CASEENUMSTRN(FI_PROTO_XNET, len);
	CASEENUMSTRN(FI_PROTO_SM2, len);
	CASEENUMSTRN(FI_PROTO_CXI_RNR, len);
	default:
		ofi_strncatf(buf, len, "Unknown");
		break;
	}
}

static void ofi_tostr_mode(char *buf, size_t len, uint64_t mode)
{
	IFFLAGSTRN(mode, FI_CONTEXT, len);
	IFFLAGSTRN(mode, FI_MSG_PREFIX, len);
	IFFLAGSTRN(mode, FI_ASYNC_IOV, len);
	IFFLAGSTRN(mode, FI_RX_CQ_DATA, len);
	IFFLAGSTRN(mode, FI_LOCAL_MR, len);
	IFFLAGSTRN(mode, FI_CONTEXT2, len);
	IFFLAGSTRN(mode, FI_BUFFERED_RECV, len);

	ofi_remove_comma(buf);
}

static void
ofi_tostr_addr(char *buf, size_t len, uint32_t addr_format, void *addr)
{
	char *p;
	size_t addrlen;

	p = buf + strlen(buf);
	addrlen = len - strlen(buf);

	if (addr == NULL) {
		ofi_strncatf(p, addrlen, "(null)");
		return;
	}

	ofi_straddr(p, &addrlen, addr_format, addr);
}

static void
ofi_tostr_tx_attr(char *buf, size_t len, const struct fi_tx_attr *attr,
		  const char *prefix)
{
	if (!attr) {
		ofi_strncatf(buf, len, "%sfi_tx_attr: (null)\n", prefix);
		return;
	}

	ofi_strncatf(buf, len, "%sfi_tx_attr:\n", prefix);
	ofi_strncatf(buf, len, "%s%scaps: [ ", prefix, TAB);
	ofi_tostr_caps(buf, len, attr->caps);
	ofi_strncatf(buf, len, " ]\n");

	ofi_strncatf(buf, len, "%s%smode: [ ", prefix, TAB);
	ofi_tostr_mode(buf, len, attr->mode);
	ofi_strncatf(buf, len, " ]\n");

	ofi_strncatf(buf, len, "%s%sop_flags: [ ", prefix, TAB);
	ofi_tostr_opflags(buf, len, attr->op_flags);
	ofi_strncatf(buf, len, " ]\n");

	ofi_strncatf(buf, len, "%s%smsg_order: [ ", prefix, TAB);
	ofi_tostr_msgorder(buf, len, attr->msg_order);
	ofi_strncatf(buf, len, " ]\n");

	ofi_strncatf(buf, len, "%s%scomp_order: [ ", prefix, TAB);
	ofi_tostr_comporder(buf, len, attr->comp_order);
	ofi_strncatf(buf, len, " ]\n");

	ofi_strncatf(buf, len, "%s%sinject_size: %zu\n", prefix, TAB,
		     attr->inject_size);
	ofi_strncatf(buf, len, "%s%ssize: %zu\n", prefix, TAB, attr->size);
	ofi_strncatf(buf, len, "%s%siov_limit: %zu\n", prefix, TAB,
		     attr->iov_limit);
	ofi_strncatf(buf, len, "%s%srma_iov_limit: %zu\n", prefix, TAB,
		     attr->rma_iov_limit);
	ofi_strncatf(buf, len, "%s%stclass: 0x%x\n", prefix, TAB, attr->tclass);
}

static void
ofi_tostr_rx_attr(char *buf, size_t len, const struct fi_rx_attr *attr,
		  const char *prefix)
{
	if (!attr) {
		ofi_strncatf(buf, len, "%sfi_rx_attr: (null)\n", prefix);
		return;
	}

	ofi_strncatf(buf, len, "%sfi_rx_attr:\n", prefix);
	ofi_strncatf(buf, len, "%s%scaps: [ ", prefix, TAB);
	ofi_tostr_caps(buf, len, attr->caps);
	ofi_strncatf(buf, len, " ]\n");

	ofi_strncatf(buf, len, "%s%smode: [ ", prefix, TAB);
	ofi_tostr_mode(buf, len, attr->mode);
	ofi_strncatf(buf, len, " ]\n");

	ofi_strncatf(buf, len, "%s%sop_flags: [ ", prefix, TAB);
	ofi_tostr_opflags(buf, len, attr->op_flags);
	ofi_strncatf(buf, len, " ]\n");

	ofi_strncatf(buf, len, "%s%smsg_order: [ ", prefix, TAB);
	ofi_tostr_msgorder(buf, len, attr->msg_order);
	ofi_strncatf(buf, len, " ]\n");

	ofi_strncatf(buf, len, "%s%scomp_order: [ ", prefix, TAB);
	ofi_tostr_comporder(buf, len, attr->comp_order);
	ofi_strncatf(buf, len, " ]\n");

	ofi_strncatf(buf, len, "%s%stotal_buffered_recv: %zu\n", prefix, TAB,
		     attr->total_buffered_recv);
	ofi_strncatf(buf, len, "%s%ssize: %zu\n", prefix, TAB, attr->size);
	ofi_strncatf(buf, len, "%s%siov_limit: %zu\n", prefix, TAB,
		     attr->iov_limit);
}

static void
ofi_tostr_ep_attr(char *buf, size_t len, const struct fi_ep_attr *attr,
		  const char *prefix)
{
	if (!attr) {
		ofi_strncatf(buf, len, "%sfi_ep_attr: (null)\n", prefix);
		return;
	}

	ofi_strncatf(buf, len, "%sfi_ep_attr:\n", prefix);
	ofi_strncatf(buf, len, "%s%stype: ", prefix, TAB);
	ofi_tostr_ep_type(buf, len, attr->type);
	ofi_strncatf(buf, len, "\n");
	ofi_strncatf(buf, len, "%s%sprotocol: ", prefix, TAB);
	ofi_tostr_protocol(buf, len, attr->protocol);
	ofi_strncatf(buf, len, "\n");
	ofi_strncatf(buf, len, "%s%sprotocol_version: %d\n", prefix, TAB,
		     attr->protocol_version);
	ofi_strncatf(buf, len, "%s%smax_msg_size: %zu\n", prefix, TAB,
		     attr->max_msg_size);
	ofi_strncatf(buf, len, "%s%smsg_prefix_size: %zu\n", prefix, TAB,
		     attr->msg_prefix_size);
	ofi_strncatf(buf, len, "%s%smax_order_raw_size: %zu\n", prefix, TAB,
		     attr->max_order_raw_size);
	ofi_strncatf(buf, len, "%s%smax_order_war_size: %zu\n", prefix, TAB,
		     attr->max_order_war_size);
	ofi_strncatf(buf, len, "%s%smax_order_waw_size: %zu\n", prefix, TAB,
		     attr->max_order_waw_size);
	ofi_strncatf(buf, len, "%s%smem_tag_format: 0x%016llx\n", prefix, TAB,
		     attr->mem_tag_format);

	ofi_strncatf(buf, len, "%s%stx_ctx_cnt: ", prefix, TAB);
	if (attr->tx_ctx_cnt == FI_SHARED_CONTEXT)
		ofi_strncatf(buf, len, "FI_SHARED_CONTEXT\n");
	else
		ofi_strncatf(buf, len, "%zu\n", attr->tx_ctx_cnt);
	ofi_strncatf(buf, len, "%s%srx_ctx_cnt: ", prefix, TAB);
	if (attr->rx_ctx_cnt == FI_SHARED_CONTEXT)
		ofi_strncatf(buf, len, "FI_SHARED_CONTEXT\n");
	else
		ofi_strncatf(buf, len, "%zu\n", attr->rx_ctx_cnt);

	ofi_strncatf(buf, len, "%s%sauth_key_size: %zu\n", prefix, TAB,
		     attr->auth_key_size);
}

static void
ofi_tostr_resource_mgmt(char *buf, size_t len, enum fi_resource_mgmt rm)
{
	switch (rm) {
	CASEENUMSTRN(FI_RM_UNSPEC, len);
	CASEENUMSTRN(FI_RM_DISABLED, len);
	CASEENUMSTRN(FI_RM_ENABLED, len);
	default:
		ofi_strncatf(buf, len, "Unknown");
		break;
	}
}

static void ofi_tostr_av_type(char *buf, size_t len, enum fi_av_type type)
{
	switch (type) {
	CASEENUMSTRN(FI_AV_UNSPEC, len);
	CASEENUMSTRN(FI_AV_MAP, len);
	CASEENUMSTRN(FI_AV_TABLE, len);
	default:
		ofi_strncatf(buf, len, "Unknown");
		break;
	}
}

static void ofi_tostr_mr_mode(char *buf, size_t len, int mr_mode)
{
	IFFLAGSTRN(mr_mode, FI_MR_BASIC, len);
	IFFLAGSTRN(mr_mode, FI_MR_SCALABLE, len);
	IFFLAGSTRN(mr_mode, FI_MR_LOCAL, len);
	IFFLAGSTRN(mr_mode, FI_MR_RAW, len);
	IFFLAGSTRN(mr_mode, FI_MR_VIRT_ADDR, len);
	IFFLAGSTRN(mr_mode, FI_MR_ALLOCATED, len);
	IFFLAGSTRN(mr_mode, FI_MR_PROV_KEY, len);
	IFFLAGSTRN(mr_mode, FI_MR_MMU_NOTIFY, len);
	IFFLAGSTRN(mr_mode, FI_MR_RMA_EVENT, len);
	IFFLAGSTRN(mr_mode, FI_MR_ENDPOINT, len);
	IFFLAGSTRN(mr_mode, FI_MR_HMEM, len);
	IFFLAGSTRN(mr_mode, FI_MR_COLLECTIVE, len);

	ofi_remove_comma(buf);
}

static void ofi_tostr_op_type(char *buf, size_t len, int op_type)
{
	switch (op_type) {
	CASEENUMSTRN(FI_OP_RECV, len);
	CASEENUMSTRN(FI_OP_SEND, len);
	CASEENUMSTRN(FI_OP_TRECV, len);
	CASEENUMSTRN(FI_OP_TSEND, len);
	CASEENUMSTRN(FI_OP_READ, len);
	CASEENUMSTRN(FI_OP_WRITE, len);
	CASEENUMSTRN(FI_OP_ATOMIC, len);
	CASEENUMSTRN(FI_OP_FETCH_ATOMIC, len);
	CASEENUMSTRN(FI_OP_COMPARE_ATOMIC, len);
	CASEENUMSTRN(FI_OP_CNTR_SET, len);
	CASEENUMSTRN(FI_OP_CNTR_ADD, len);
	default:
		ofi_strncatf(buf, len, "Unknown");
		break;
	}
}

static void
ofi_tostr_domain_attr(char *buf, size_t len, const struct fi_domain_attr *attr,
		      const char *prefix)
{
	if (!attr) {
		ofi_strncatf(buf, len, "%sfi_domain_attr: (null)\n", prefix);
		return;
	}

	ofi_strncatf(buf, len, "%sfi_domain_attr:\n", prefix);

	ofi_strncatf(buf, len, "%s%sdomain: 0x%x\n", prefix, TAB, attr->domain);

	ofi_strncatf(buf, len, "%s%sname: %s\n", prefix, TAB, attr->name);
	ofi_strncatf(buf, len, "%s%sthreading: ", prefix, TAB);
	ofi_tostr_threading(buf, len, attr->threading);
	ofi_strncatf(buf, len, "\n");

	ofi_strncatf(buf, len, "%s%scontrol_progress: ", prefix,TAB);
	ofi_tostr_progress(buf, len, attr->control_progress);
	ofi_strncatf(buf, len, "\n");
	ofi_strncatf(buf, len, "%s%sdata_progress: ", prefix, TAB);
	ofi_tostr_progress(buf, len, attr->data_progress);
	ofi_strncatf(buf, len, "\n");
	ofi_strncatf(buf, len, "%s%sresource_mgmt: ", prefix, TAB);
	ofi_tostr_resource_mgmt(buf, len, attr->resource_mgmt);
	ofi_strncatf(buf, len, "\n");
	ofi_strncatf(buf, len, "%s%sav_type: ", prefix, TAB);
	ofi_tostr_av_type(buf, len, attr->av_type);
	ofi_strncatf(buf, len, "\n");
	ofi_strncatf(buf, len, "%s%smr_mode: [ ", prefix, TAB);
	ofi_tostr_mr_mode(buf, len, attr->mr_mode);
	ofi_strncatf(buf, len, " ]\n");

	ofi_strncatf(buf, len, "%s%smr_key_size: %zu\n", prefix, TAB,
		     attr->mr_key_size);
	ofi_strncatf(buf, len, "%s%scq_data_size: %zu\n", prefix, TAB,
		     attr->cq_data_size);
	ofi_strncatf(buf, len, "%s%scq_cnt: %zu\n", prefix, TAB,
		     attr->cq_cnt);
	ofi_strncatf(buf, len, "%s%sep_cnt: %zu\n", prefix, TAB, attr->ep_cnt);
	ofi_strncatf(buf, len, "%s%stx_ctx_cnt: %zu\n", prefix, TAB,
		     attr->tx_ctx_cnt);
	ofi_strncatf(buf, len, "%s%srx_ctx_cnt: %zu\n", prefix, TAB,
		     attr->rx_ctx_cnt);
	ofi_strncatf(buf, len, "%s%smax_ep_tx_ctx: %zu\n", prefix, TAB,
		     attr->max_ep_tx_ctx);
	ofi_strncatf(buf, len, "%s%smax_ep_rx_ctx: %zu\n", prefix, TAB,
		     attr->max_ep_rx_ctx);
	ofi_strncatf(buf, len, "%s%smax_ep_stx_ctx: %zu\n", prefix, TAB,
		     attr->max_ep_stx_ctx);
	ofi_strncatf(buf, len, "%s%smax_ep_srx_ctx: %zu\n", prefix, TAB,
		     attr->max_ep_srx_ctx);
	ofi_strncatf(buf, len, "%s%scntr_cnt: %zu\n", prefix, TAB,
		     attr->cntr_cnt);
	ofi_strncatf(buf, len, "%s%smr_iov_limit: %zu\n", prefix, TAB,
		     attr->mr_iov_limit);

	ofi_strncatf(buf, len, "%s%scaps: [ ", prefix, TAB);
	ofi_tostr_caps(buf, len, attr->caps);
	ofi_strncatf(buf, len, " ]\n");

	ofi_strncatf(buf, len, "%s%smode: [ ", prefix, TAB);
	ofi_tostr_mode(buf, len, attr->mode);
	ofi_strncatf(buf, len, " ]\n");

	ofi_strncatf(buf, len, "%s%sauth_key_size: %zu\n", prefix, TAB,
		     attr->auth_key_size);
	ofi_strncatf(buf, len, "%s%smax_err_data: %zu\n", prefix, TAB,
		     attr->max_err_data);
	ofi_strncatf(buf, len, "%s%smr_cnt: %zu\n", prefix, TAB, attr->mr_cnt);
	ofi_strncatf(buf, len, "%s%stclass: 0x%x\n", prefix, TAB, attr->tclass);
}

static void
ofi_tostr_fabric_attr(char *buf, size_t len, const struct fi_fabric_attr *attr,
		      const char *prefix)
{
	if (!attr) {
		ofi_strncatf(buf, len, "%sfi_fabric_attr: (null)\n", prefix);
		return;
	}

	ofi_strncatf(buf, len, "%sfi_fabric_attr:\n", prefix);
	ofi_strncatf(buf, len, "%s%sname: %s\n", prefix, TAB, attr->name);
	ofi_strncatf(buf, len, "%s%sprov_name: %s\n", prefix, TAB,
		     attr->prov_name);
	ofi_strncatf(buf, len, "%s%sprov_version: %d.%d\n", prefix, TAB,
		FI_MAJOR(attr->prov_version), FI_MINOR(attr->prov_version));
	ofi_strncatf(buf, len, "%s%sapi_version: %d.%d\n", prefix, TAB,
		FI_MAJOR(attr->api_version), FI_MINOR(attr->api_version));
}

static void ofi_tostr_info(char *buf, size_t len, const struct fi_info *info)
{
	ofi_strncatf(buf, len, "fi_info:\n");
	ofi_strncatf(buf, len, "%scaps: [ ", TAB);
	ofi_tostr_caps(buf, len, info->caps);
	ofi_strncatf(buf, len, " ]\n");

	ofi_strncatf(buf, len, "%smode: [ ", TAB);
	ofi_tostr_mode(buf, len, info->mode);
	ofi_strncatf(buf, len, " ]\n");

	ofi_strncatf(buf, len, "%saddr_format: ", TAB);
	ofi_tostr_addr_format(buf, len, info->addr_format);
	ofi_strncatf(buf, len, "\n");

	ofi_strncatf(buf, len, "%ssrc_addrlen: %zu\n", TAB, info->src_addrlen);
	ofi_strncatf(buf, len, "%sdest_addrlen: %zu\n", TAB,
		     info->dest_addrlen);
	ofi_strncatf(buf, len, "%ssrc_addr: ", TAB);
	ofi_tostr_addr(buf, len, info->addr_format, info->src_addr);
	ofi_strncatf(buf, len, "\n");
	ofi_strncatf(buf, len, "%sdest_addr: ", TAB);
	ofi_tostr_addr(buf, len, info->addr_format, info->dest_addr);
	ofi_strncatf(buf, len, "\n");
	ofi_tostr_fid(TAB "handle: ", buf, len, info->handle);

	ofi_tostr_tx_attr(buf, len, info->tx_attr, TAB);
	ofi_tostr_rx_attr(buf, len, info->rx_attr, TAB);
	ofi_tostr_ep_attr(buf, len, info->ep_attr, TAB);
	ofi_tostr_domain_attr(buf, len, info->domain_attr, TAB);
	ofi_tostr_fabric_attr(buf, len, info->fabric_attr, TAB);
	ofi_tostr_fid(TAB "nic: ", buf, len, &info->nic->fid);
}

static void ofi_tostr_atomic_type(char *buf, size_t len, enum fi_datatype type)
{
	switch (type) {
	CASEENUMSTRN(FI_INT8, len);
	CASEENUMSTRN(FI_UINT8, len);
	CASEENUMSTRN(FI_INT16, len);
	CASEENUMSTRN(FI_UINT16, len);
	CASEENUMSTRN(FI_INT32, len);
	CASEENUMSTRN(FI_UINT32, len);
	CASEENUMSTRN(FI_INT64, len);
	CASEENUMSTRN(FI_UINT64, len);
	CASEENUMSTRN(FI_INT128, len);
	CASEENUMSTRN(FI_UINT128, len);
	CASEENUMSTRN(FI_FLOAT, len);
	CASEENUMSTRN(FI_DOUBLE, len);
	CASEENUMSTRN(FI_FLOAT_COMPLEX, len);
	CASEENUMSTRN(FI_DOUBLE_COMPLEX, len);
	CASEENUMSTRN(FI_LONG_DOUBLE, len);
	CASEENUMSTRN(FI_LONG_DOUBLE_COMPLEX, len);
	default:
		ofi_strncatf(buf, len, "Unknown");
		break;
	}
}

static void ofi_tostr_atomic_op(char *buf, size_t len, enum fi_op op)
{
	switch (op) {
	CASEENUMSTRN(FI_MIN, len);
	CASEENUMSTRN(FI_MAX, len);
	CASEENUMSTRN(FI_SUM, len);
	CASEENUMSTRN(FI_PROD, len);
	CASEENUMSTRN(FI_LOR, len);
	CASEENUMSTRN(FI_LAND, len);
	CASEENUMSTRN(FI_BOR, len);
	CASEENUMSTRN(FI_BAND, len);
	CASEENUMSTRN(FI_LXOR, len);
	CASEENUMSTRN(FI_BXOR, len);
	CASEENUMSTRN(FI_ATOMIC_READ, len);
	CASEENUMSTRN(FI_ATOMIC_WRITE, len);
	CASEENUMSTRN(FI_CSWAP, len);
	CASEENUMSTRN(FI_CSWAP_NE, len);
	CASEENUMSTRN(FI_CSWAP_LE, len);
	CASEENUMSTRN(FI_CSWAP_LT, len);
	CASEENUMSTRN(FI_CSWAP_GE, len);
	CASEENUMSTRN(FI_CSWAP_GT, len);
	CASEENUMSTRN(FI_MSWAP, len);
	default:
		ofi_strncatf(buf, len, "Unknown");
		break;
	}
}

static void
ofi_tostr_collective_op(char *buf, size_t len, enum fi_collective_op op)
{
	switch (op) {
	CASEENUMSTRN(FI_BARRIER, len);
	CASEENUMSTRN(FI_BROADCAST, len);
	CASEENUMSTRN(FI_ALLTOALL, len);
	CASEENUMSTRN(FI_ALLREDUCE, len);
	CASEENUMSTRN(FI_ALLGATHER, len);
	CASEENUMSTRN(FI_REDUCE_SCATTER, len);
	CASEENUMSTRN(FI_REDUCE, len);
	CASEENUMSTRN(FI_SCATTER, len);
	CASEENUMSTRN(FI_GATHER, len);
	default:
		ofi_strncatf(buf, len, "Unknown");
		break;
	}
}

static void ofi_tostr_version(char *buf, size_t len)
{
	ofi_strncatf(buf, len, VERSION);
	ofi_strncatf(buf, len, BUILD_ID);
}

static void ofi_tostr_eq_event(char *buf, size_t len, int type)
{
	switch (type) {
	CASEENUMSTRN(FI_NOTIFY, len);
	CASEENUMSTRN(FI_CONNREQ, len);
	CASEENUMSTRN(FI_CONNECTED, len);
	CASEENUMSTRN(FI_SHUTDOWN, len);
	CASEENUMSTRN(FI_MR_COMPLETE, len);
	CASEENUMSTRN(FI_AV_COMPLETE, len);
	CASEENUMSTRN(FI_JOIN_COMPLETE, len);
	default:
		ofi_strncatf(buf, len, "Unknown");
		break;
	}
}

static void ofi_tostr_cq_event_flags(char *buf, size_t len, uint64_t flags)
{
	IFFLAGSTRN(flags, FI_SEND, len);
	IFFLAGSTRN(flags, FI_RECV, len);
	IFFLAGSTRN(flags, FI_RMA, len);
	IFFLAGSTRN(flags, FI_ATOMIC, len);
	IFFLAGSTRN(flags, FI_MSG, len);
	IFFLAGSTRN(flags, FI_TAGGED, len);
	IFFLAGSTRN(flags, FI_READ, len);
	IFFLAGSTRN(flags, FI_WRITE, len);
	IFFLAGSTRN(flags, FI_REMOTE_READ, len);
	IFFLAGSTRN(flags, FI_REMOTE_WRITE, len);
	IFFLAGSTRN(flags, FI_REMOTE_CQ_DATA, len);
	IFFLAGSTRN(flags, FI_MULTI_RECV, len);
	IFFLAGSTRN(flags, FI_MORE, len);
	IFFLAGSTRN(flags, FI_CLAIM, len);
	ofi_remove_comma(buf);
}

static void
ofi_tostr_hmem_iface(char *buf, size_t len, enum fi_hmem_iface iface)
{
	switch (iface) {
	CASEENUMSTRN(FI_HMEM_SYSTEM, len);
	CASEENUMSTRN(FI_HMEM_CUDA, len);
	CASEENUMSTRN(FI_HMEM_ROCR, len);
	CASEENUMSTRN(FI_HMEM_ZE, len);
	CASEENUMSTRN(FI_HMEM_NEURON, len);
	CASEENUMSTRN(FI_HMEM_SYNAPSEAI, len);
	default:
		ofi_strncatf(buf, len, "Unknown");
		break;
	}
}

static void
ofi_tostr_cq_format(char *buf, size_t len, enum fi_cq_format cq_format)
{
	switch (cq_format) {
	CASEENUMSTRN(FI_CQ_FORMAT_UNSPEC, len);
	CASEENUMSTRN(FI_CQ_FORMAT_CONTEXT, len);
	CASEENUMSTRN(FI_CQ_FORMAT_MSG, len);
	CASEENUMSTRN(FI_CQ_FORMAT_DATA, len);
	CASEENUMSTRN(FI_CQ_FORMAT_TAGGED, len);
	default:
		ofi_strncatf(buf, len, "Unknown");
		break;
	}
}

static void
ofi_tostr_av_flags(char *buf, size_t len, uint64_t flags)
{
	IFFLAGSTRN(flags, FI_EVENT, len);
	IFFLAGSTRN(flags, FI_READ, len);
	IFFLAGSTRN(flags, FI_SYMMETRIC, len);
	ofi_remove_comma(buf);
}

static void
ofi_tostr_av_attr(char *buf, size_t len, const struct fi_av_attr *attr)
{
	if (!attr) {
		ofi_strncatf(buf, len, "fi_av_attr: (null)\n");
		return;
	}

	ofi_strncatf(buf, len, "fi_av_attr:\n");
	ofi_strncatf(buf, len, "%stype: ", TAB);
	ofi_tostr_av_type(buf, len, attr->type);
	ofi_strncatf(buf, len, "\n");
	ofi_strncatf(buf, len, "%scount: %zu\n", TAB, attr->count);
	ofi_strncatf(buf, len, "%sep_per_node: %zu\n", TAB,
	             attr->ep_per_node);
	ofi_strncatf(buf, len, "%sname: %s\n", TAB, attr->name);
	ofi_strncatf(buf, len, "%sflags: [ ", TAB);
	ofi_tostr_av_flags(buf, len, attr->flags);
	ofi_strncatf(buf, len, " ]\n");
}

static void
ofi_tostr_wait_obj(char *buf, size_t len, enum fi_wait_obj obj)
{
	switch (obj) {
	CASEENUMSTRN(FI_WAIT_NONE, len);
	CASEENUMSTRN(FI_WAIT_UNSPEC, len);
	CASEENUMSTRN(FI_WAIT_SET, len);
	CASEENUMSTRN(FI_WAIT_FD, len);
	CASEENUMSTRN(FI_WAIT_MUTEX_COND, len);
	CASEENUMSTRN(FI_WAIT_YIELD, len);
	CASEENUMSTRN(FI_WAIT_POLLFD, len);
	default:
		ofi_strncatf(buf, len, "Unknown");
		break;
	}
}

static void
ofi_tostr_cq_wait_cond(char *buf, size_t len, enum fi_cq_wait_cond cond)
{
	switch (cond) {
	CASEENUMSTRN(FI_CQ_COND_NONE, len);
	CASEENUMSTRN(FI_CQ_COND_THRESHOLD, len);
	default:
		ofi_strncatf(buf, len, "Unknown");
		break;
	}
}

static void
ofi_tostr_cq_flags(char *buf, size_t len, uint64_t flags)
{
	IFFLAGSTRN(flags, FI_AFFINITY, len);
	ofi_remove_comma(buf);
}

static void
ofi_tostr_cq_attr(char *buf, size_t len, const struct fi_cq_attr *attr)
{
	if (!attr) {
		ofi_strncatf(buf, len, "fi_cq_attr: (null)\n");
		return;
	}
	ofi_strncatf(buf, len, "fi_cq_attr:\n");
	ofi_strncatf(buf, len, "%ssize: %zu\n", TAB, attr->size);
	ofi_strncatf(buf, len, "%sflags: [ ", TAB);
	ofi_tostr_cq_flags(buf, len, attr->flags);
	ofi_strncatf(buf, len, " ]\n");
	ofi_strncatf(buf, len, "%sformat: ", TAB);
	ofi_tostr_cq_format(buf, len, attr->format);
	ofi_strncatf(buf, len, "\n");
	ofi_strncatf(buf, len, "%swait_obj: ", TAB);
	ofi_tostr_wait_obj(buf, len, attr->wait_obj);
	ofi_strncatf(buf, len, "\n");
	ofi_strncatf(buf, len, "%ssignaling_vector: %d\n", TAB,
	             attr->signaling_vector);
	ofi_strncatf(buf, len, "%swait_cond: ", TAB);
	ofi_tostr_cq_wait_cond(buf, len, attr->wait_cond);
	ofi_strncatf(buf, len, "\n");
}

static void ofi_tostr_mr_access(char *buf, size_t len, uint64_t access)
{
	IFFLAGSTRN(access, FI_RECV, len);
	IFFLAGSTRN(access, FI_SEND, len);
	IFFLAGSTRN(access, FI_READ, len);
	IFFLAGSTRN(access, FI_WRITE, len);
	IFFLAGSTRN(access, FI_REMOTE_READ, len);
	IFFLAGSTRN(access, FI_REMOTE_WRITE, len);
	IFFLAGSTRN(access, FI_COLLECTIVE, len);
	ofi_remove_comma(buf);
}

static void
ofi_tostr_mr_attr(char *buf, size_t len, const struct fi_mr_attr *attr)
{
	if (!attr) {
		ofi_strncatf(buf, len, "fi_mr_attr: (null)\n");
		return;
	}
	ofi_strncatf(buf, len, "fi_mr_attr:\n");
	ofi_strncatf(buf, len, "%smr_iov: %p\n", TAB, attr->mr_iov);
	ofi_strncatf(buf, len, "%siov_count: %zu\n", TAB, attr->iov_count);
	ofi_strncatf(buf, len, "%saccess: [ ", TAB);
	ofi_tostr_mr_access(buf, len, attr->access);
	ofi_strncatf(buf, len, " ]\n");
	ofi_strncatf(buf, len, "%soffset: 0x%lx\n", TAB, attr->offset);
	ofi_strncatf(buf, len, "%srequested_key: 0x%lx\n",
	             TAB, attr->requested_key);
	ofi_strncatf(buf, len, "%scontext: %p\n", TAB, attr->context);
	ofi_strncatf(buf, len, "%sauth_key_size: %zu\n", TAB,
	             attr->auth_key_size);
	ofi_strncatf(buf, len, "%siface: ", TAB);
	ofi_tostr_hmem_iface(buf, len, attr->iface);
	ofi_strncatf(buf, len, "\n");
}

static void
ofi_tostr_cntr_events(char *buf, size_t len, enum fi_cntr_events events)
{
	switch (events) {
	CASEENUMSTRN(FI_CNTR_EVENTS_COMP, len);
	CASEENUMSTRN(FI_CNTR_EVENTS_BYTES, len);
	default:
		ofi_strncatf(buf, len, "Unknown");
		break;
	}
}

static void
ofi_tostr_cntr_attr(char *buf, size_t len, const struct fi_cntr_attr *attr)
{
	if (!attr) {
		ofi_strncatf(buf, len, "fi_cntr_attr: (null)\n");
		return;
	}
	ofi_strncatf(buf, len, "fi_cntr_attr:\n");
	ofi_strncatf(buf, len, "%sevents: ", TAB);
	ofi_tostr_cntr_events(buf, len, attr->events);
	ofi_strncatf(buf, len, "\n");
	ofi_strncatf(buf, len, "%swait_obj: ", TAB);
	ofi_tostr_wait_obj(buf, len, attr->wait_obj);
	ofi_strncatf(buf, len, "\n");
	ofi_strncatf(buf, len, "%sflags: 0x%lx\n", TAB, attr->flags);
}

static void
ofi_tostr_cq_err_entry(char *buf, size_t len,
                       const struct fi_cq_err_entry *entry)
{
	if (!entry) {
		ofi_strncatf(buf, len, "fi_cq_err_entry: (null)\n");
		return;
	}
	ofi_strncatf(buf, len, "fi_cq_err_entry:\n");
	ofi_strncatf(buf, len, "%sop_context: %p\n", TAB, entry->op_context);
	ofi_strncatf(buf, len, "%sflags: 0x%lx\n", TAB, entry->flags);
	ofi_strncatf(buf, len, "%slen: %zu\n", TAB, entry->len);
	ofi_strncatf(buf, len, "%sbuf: %p\n", TAB, entry->buf);
	ofi_strncatf(buf, len, "%sdata: %lu\n", TAB, entry->data);
	ofi_strncatf(buf, len, "%stag: 0x%lx\n", TAB, entry->tag);
	ofi_strncatf(buf, len, "%solen: %zu\n", TAB, entry->olen);
	ofi_strncatf(buf, len, "%serr: %d\n", TAB, entry->err);
	ofi_strncatf(buf, len, "%sprov_errno: %d\n", TAB, entry->err);
	ofi_strncatf(buf, len, "%serr_data: %p\n", TAB, entry->err_data);
	ofi_strncatf(buf, len, "%serr_data_size: %zu\n", TAB, entry->err_data_size);
}

__attribute__((visibility ("default"),EXTERNALLY_VISIBLE))
char *DEFAULT_SYMVER_PRE(fi_tostr_r)(char *buf, size_t len,
				     const void *data, enum fi_type datatype)
{
	const uint64_t *val64;
	const uint32_t *val32;
	const int *enumval;

	if (!data || !buf || !len)
		return NULL;

	val64 = (const uint64_t *) data;
	val32 = (const uint32_t *) data;
	enumval = (const int *) data;

	buf[0] = '\0';

	switch (datatype) {
	case FI_TYPE_INFO:
		ofi_tostr_info(buf, len, data);
		break;
	case FI_TYPE_EP_TYPE:
		ofi_tostr_ep_type(buf, len, *enumval);
		break;
	case FI_TYPE_CAPS:
		ofi_tostr_caps(buf, len, *val64);
		break;
	case FI_TYPE_OP_FLAGS:
		ofi_tostr_opflags(buf, len, *val64);
		break;
	case FI_TYPE_ADDR_FORMAT:
		ofi_tostr_addr_format(buf, len, *val32);
		break;
	case FI_TYPE_TX_ATTR:
		ofi_tostr_tx_attr(buf, len, data, "");
		break;
	case FI_TYPE_RX_ATTR:
		ofi_tostr_rx_attr(buf, len, data, "");
		break;
	case FI_TYPE_EP_ATTR:
		ofi_tostr_ep_attr(buf, len, data, "");
		break;
	case FI_TYPE_DOMAIN_ATTR:
		ofi_tostr_domain_attr(buf, len, data, "");
		break;
	case FI_TYPE_FABRIC_ATTR:
		ofi_tostr_fabric_attr(buf, len, data, "");
		break;
	case FI_TYPE_THREADING:
		ofi_tostr_threading(buf, len, *enumval);
		break;
	case FI_TYPE_PROGRESS:
		ofi_tostr_progress(buf, len, *enumval);
		break;
	case FI_TYPE_PROTOCOL:
		ofi_tostr_protocol(buf, len, *val32);
		break;
	case FI_TYPE_MSG_ORDER:
		ofi_tostr_msgorder(buf, len, *val64);
		break;
	case FI_TYPE_MODE:
		ofi_tostr_mode(buf, len, *val64);
		break;
	case FI_TYPE_AV_TYPE:
		ofi_tostr_av_type(buf, len, *enumval);
		break;
	case FI_TYPE_ATOMIC_TYPE:
		ofi_tostr_atomic_type(buf, len, *enumval);
		break;
	case FI_TYPE_ATOMIC_OP:
		ofi_tostr_atomic_op(buf, len, *enumval);
		break;
	case FI_TYPE_VERSION:
		ofi_tostr_version(buf, len);
		break;
	case FI_TYPE_EQ_EVENT:
		ofi_tostr_eq_event(buf, len, *enumval);
		break;
	case FI_TYPE_CQ_EVENT_FLAGS:
		ofi_tostr_cq_event_flags(buf, len, *val64);
		break;
	case FI_TYPE_MR_MODE:
		/* mr_mode was an enum converted to int flags */
		ofi_tostr_mr_mode(buf, len, *enumval);
		break;
	case FI_TYPE_OP_TYPE:
		ofi_tostr_op_type(buf, len, *enumval);
		break;
	case FI_TYPE_FID:
		ofi_tostr_fid("fid: ", buf, len, data);
		break;
	case FI_TYPE_COLLECTIVE_OP:
		ofi_tostr_collective_op(buf, len, *enumval);
		break;
	case FI_TYPE_HMEM_IFACE:
		ofi_tostr_hmem_iface(buf, len, *enumval);
		break;
	case FI_TYPE_CQ_FORMAT:
		ofi_tostr_cq_format(buf, len, *enumval);
		break;
	case FI_TYPE_LOG_LEVEL:
		ofi_tostr_log_level(buf, len, *enumval);
		break;
	case FI_TYPE_LOG_SUBSYS:
		ofi_tostr_log_subsys(buf, len, *enumval);
		break;
	case FI_TYPE_AV_ATTR:
		ofi_tostr_av_attr(buf, len, data);
		break;
	case FI_TYPE_CQ_ATTR:
		ofi_tostr_cq_attr(buf, len, data);
		break;
	case FI_TYPE_MR_ATTR:
		ofi_tostr_mr_attr(buf, len, data);
		break;
	case FI_TYPE_CNTR_ATTR:
		ofi_tostr_cntr_attr(buf, len, data);
		break;
	case FI_TYPE_CQ_ERR_ENTRY:
		ofi_tostr_cq_err_entry(buf, len, data);
		break;
	default:
		ofi_strncatf(buf, len, "Unknown type");
		break;
	}
	return buf;
}
DEFAULT_SYMVER(fi_tostr_r_, fi_tostr_r, FABRIC_1.4);

__attribute__((visibility ("default"),EXTERNALLY_VISIBLE))
char *DEFAULT_SYMVER_PRE(fi_tostr)(const void *data, enum fi_type datatype)
{
	static char *buf = NULL;
	size_t len = 8192;

	if (!buf) {
		buf = calloc(len, 1);
		if (!buf)
			return NULL;
	}

	return fi_tostr_r(buf, len, data, datatype);
}
DEFAULT_SYMVER(fi_tostr_, fi_tostr, FABRIC_1.0);
