/*
 * Copyright (c) 2013-2019 Intel Corporation. All rights reserved.
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

#include "psmx2.h"

/*
 * Default provider attributes are defined for:
 *
 * 	full set of capabilities
 * 	ep type = FI_EP_RDM
 * 	addr format = FI_ADDR_PSMX2
 * 	cq_data_size = 0
 *
 * This is used as a template to create actual provider info, which will
 * have some fields modified for different configurations and some fields
 * updated to environment settings.
 */

static struct fi_tx_attr psmx2_tx_attr = {
	.caps			= PSMX2_TX_CAPS, /* PSMX2_RMA_TX_CAPS */
	.mode			= FI_CONTEXT, /* 0 */
	.op_flags		= PSMX2_OP_FLAGS,
	.msg_order		= PSMX2_MSG_ORDER,
	.comp_order		= PSMX2_COMP_ORDER,
	.inject_size		= 64, /* psmx2_env.inject_size */
	.size			= UINT64_MAX,
	.iov_limit		= PSMX2_IOV_MAX_COUNT,
	.rma_iov_limit		= 1,
};

static struct fi_rx_attr psmx2_rx_attr = {
	.caps			= PSMX2_RX_CAPS, /* PSMX2_RMA_RX_CAPS */
	.mode			= FI_CONTEXT, /* 0 */
	.op_flags		= PSMX2_OP_FLAGS,
	.msg_order		= PSMX2_MSG_ORDER,
	.comp_order		= PSMX2_COMP_ORDER,
	.total_buffered_recv	= UINT64_MAX,
	.size			= UINT64_MAX,
	.iov_limit		= 1,
};

static struct fi_ep_attr psmx2_ep_attr = {
	.type			= FI_EP_RDM, /* FI_EP_DGRAM */
	.protocol		= FI_PROTO_PSMX2,
	.protocol_version	= PSM2_VERNO,
	.max_msg_size		= PSMX2_MAX_MSG_SIZE & ~0x0FFF,
	.msg_prefix_size	= 0,
	.max_order_raw_size	= PSMX2_RMA_ORDER_SIZE,
	.max_order_war_size	= PSMX2_RMA_ORDER_SIZE,
	.max_order_waw_size	= PSMX2_RMA_ORDER_SIZE,
	.mem_tag_format		= FI_TAG_GENERIC, /* >>= 4 */
	.tx_ctx_cnt		= 1,
	.rx_ctx_cnt		= 1,
	.auth_key_size		= sizeof(psm2_uuid_t),
	.auth_key		= NULL,
};

static struct fi_domain_attr psmx2_domain_attr = {
	.domain			= NULL,
	.name			= PSMX2_DOMAIN_NAME,
	.threading		= FI_THREAD_SAFE,
	.control_progress	= FI_PROGRESS_AUTO,
	.data_progress		= FI_PROGRESS_AUTO,
	.resource_mgmt		= FI_RM_ENABLED,
	.av_type		= FI_AV_UNSPEC,
	.mr_mode		= FI_MR_SCALABLE | FI_MR_BASIC,
	.mr_key_size		= sizeof(uint64_t),
	.cq_data_size		= 0, /* 4, 8 */
	.cq_cnt			= 65535,
	.ep_cnt			= 65535,
	.tx_ctx_cnt		= 1, /* psmx2_hfi_info.free_trx_ctxt */
	.rx_ctx_cnt		= 1, /* psmx2_hfi_info.free_trx_ctxt */
	.max_ep_tx_ctx		= 1, /* psmx2_hfi_info.max_trx_ctxt */
	.max_ep_rx_ctx		= 1, /* psmx2_hfi_info.max_trx_ctxt */
	.max_ep_stx_ctx		= 1, /* psmx2_hfi_info.max_trx_ctxt */
	.max_ep_srx_ctx		= 0,
	.cntr_cnt		= 65535,
	.mr_iov_limit		= 65535,
	.caps			= PSMX2_DOM_CAPS,
	.mode			= 0,
	.auth_key		= NULL,
	.auth_key_size		= sizeof(psm2_uuid_t),
	.max_err_data		= PSMX2_ERR_DATA_SIZE,
	.mr_cnt			= 65535,
};

static struct fi_fabric_attr psmx2_fabric_attr = {
	.name			= PSMX2_FABRIC_NAME,
	.prov_version		= OFI_VERSION_DEF_PROV,
};

static struct fi_info psmx2_prov_info = {
	.next			= NULL,
	.caps			= PSMX2_CAPS, /* PSMX2_RMA_CAPS */
	.mode			= FI_CONTEXT, /* 0 */
	.addr_format		= FI_ADDR_PSMX2, /* FI_ADDR_STR */
	.src_addrlen		= sizeof(struct psmx2_ep_name),
	.dest_addrlen		= sizeof(struct psmx2_ep_name),
	.src_addr		= NULL,
	.dest_addr		= NULL,
	.handle			= NULL,
	.tx_attr		= &psmx2_tx_attr,
	.rx_attr		= &psmx2_rx_attr,
	.ep_attr		= &psmx2_ep_attr,
	.domain_attr		= &psmx2_domain_attr,
	.fabric_attr		= &psmx2_fabric_attr,
};

/*
 * Possible provider variations:
 *
 *  (1)  FI_ADDR_PSMX2, FI_EP_RDM,   tag64 (cq_data_size 0, FI_CONTEXT)
 *  (2)  FI_ADDR_PSMX2, FI_EP_RDM,   tag60 (cq_data_size 4, FI_CONTEXT)
 *  (3)  FI_ADDR_PSMX2, FI_EP_RDM,   rma   (cq_data_size 8)
 *  (4)  FI_ADDR_PSMX2, FI_EP_DGRAM, tag64 (cq_data_size 0, FI_CONTEXT)
 *  (5)  FI_ADDR_PSMX2, FI_EP_DGRAM, tag60 (cq_data_size 4, FI_CONTEXT)
 *  (6)  FI_ADDR_PSMX2, FI_EP_DGRAM, rma   (cq_data_size 8)
 *  (7)  FI_ADDR_STR,   FI_EP_RDM,   tag64 (cq_data_size 0, FI_CONTEXT)
 *  (8)  FI_ADDR_STR,   FI_EP_RDM,   tag60 (cq_data_size 4, FI_CONTEXT)
 *  (9)  FI_ADDR_STR,   FI_EP_RDM,   rma   (cq_data_size 8)
 *  (10) FI_ADDR_STR,   FI_EP_DGRAM, tag64 (cq_data_size 0, FI_CONTEXT)
 *  (11) FI_ADDR_STR,   FI_EP_DGRAM, tag60 (cq_data_size 4, FI_CONTEXT)
 *  (12) FI_ADDR_STR,   FI_EP_DGRAM, rma   (cq_data_size 8)
 *
 * To avoid returning all 12 provider variations for an unrestricted query,
 * "addr_format" and "ep_type" are checked first and a single value is set
 * for each of them. As the result, at most three provider instances (tag64,
 * tag60, rma) are returned.
 *
 * This also bypasses queries obviously unsuitable for this provider and
 * avoid unnecessary initialization steps.
 */

int psmx2_init_prov_info(const struct fi_info *hints, struct fi_info **info)
{
	struct fi_fabric_attr *fabric_attr = &psmx2_fabric_attr;
	struct fi_domain_attr *domain_attr = &psmx2_domain_attr;
	struct fi_info *prov_info = &psmx2_prov_info;
	struct fi_info *info_out, *info_new;
	int addr_format = FI_ADDR_PSMX2;
	int addr_format2 = FI_ADDR_STR;
	int ep_type = FI_EP_RDM;
	int ep_type2 = FI_EP_DGRAM;

	if (!hints)
		goto alloc_info;

	if (hints->fabric_attr && hints->fabric_attr->name &&
	    strcasecmp(hints->fabric_attr->name, fabric_attr->name)) {
		FI_INFO(&psmx2_prov, FI_LOG_CORE, "Unknown fabric name\n");
		FI_INFO_NAME(&psmx2_prov, fabric_attr, hints->fabric_attr);
		return -FI_ENODATA;
	}

	if (hints->domain_attr && hints->domain_attr->name &&
	    strncasecmp(hints->domain_attr->name, domain_attr->name, strlen(PSMX2_DOMAIN_NAME))) {
		FI_INFO(&psmx2_prov, FI_LOG_CORE, "Unknown domain name\n");
		FI_INFO_NAME(&psmx2_prov, domain_attr, hints->domain_attr);
		return -FI_ENODATA;
	}

	if (hints->ep_attr) {
		switch (hints->ep_attr->type) {
		case FI_EP_UNSPEC:
		case FI_EP_RDM:
			break;
		case FI_EP_DGRAM:
			ep_type = FI_EP_DGRAM;
			break;
		default:
			FI_INFO(&psmx2_prov, FI_LOG_CORE,
				"Unsupported endpoint type\n");
			FI_INFO(&psmx2_prov, FI_LOG_CORE, "Supported: %s\n",
				fi_tostr(&ep_type, FI_TYPE_EP_TYPE));
			FI_INFO(&psmx2_prov, FI_LOG_CORE, "Supported: %s\n",
				fi_tostr(&ep_type2, FI_TYPE_EP_TYPE));
			FI_INFO(&psmx2_prov, FI_LOG_CORE, "Requested: %s\n",
				fi_tostr(&hints->ep_attr->type, FI_TYPE_EP_TYPE));
			return -FI_ENODATA;
		}
	}

	switch (hints->addr_format) {
		case FI_FORMAT_UNSPEC:
		case FI_ADDR_PSMX2:
			break;
		case FI_ADDR_STR:
			addr_format = FI_ADDR_STR;
			break;
		default:
			FI_INFO(&psmx2_prov, FI_LOG_CORE,
				"Unsupported address format\n");
			FI_INFO(&psmx2_prov, FI_LOG_CORE, "Supported: %s\n",
				fi_tostr(&addr_format, FI_TYPE_ADDR_FORMAT));
			FI_INFO(&psmx2_prov, FI_LOG_CORE, "Supported: %s\n",
				fi_tostr(&addr_format2, FI_TYPE_ADDR_FORMAT));
			FI_INFO(&psmx2_prov, FI_LOG_CORE, "Requested: %s\n",
				fi_tostr(&hints->addr_format, FI_TYPE_ADDR_FORMAT));
			return -FI_ENODATA;
	}

	if ((hints->caps & PSMX2_CAPS) != hints->caps) {
		FI_INFO(&psmx2_prov, FI_LOG_CORE, "caps not supported\n");
		FI_INFO_CHECK(&psmx2_prov, prov_info, hints, caps, FI_TYPE_CAPS);
		return -FI_ENODATA;
	}

alloc_info:
	info_out = NULL;
	if (!hints || !(hints->caps & (FI_TAGGED | FI_MSG))) {
		info_new = fi_dupinfo(&psmx2_prov_info);
		if (info_new) {
			/* rma only, 64 bit CQ data */
			info_new->addr_format = addr_format;
			info_new->ep_attr->type = ep_type;
			info_new->caps = PSMX2_RMA_CAPS;
			info_new->mode = 0;
			info_new->tx_attr->caps = PSMX2_RMA_TX_CAPS;
			info_new->tx_attr->mode = 0;
			info_new->rx_attr->caps = PSMX2_RMA_RX_CAPS;
			info_new->rx_attr->mode = 0;
			info_new->domain_attr->cq_data_size = 8;
			info_out = info_new;
			FI_INFO(&psmx2_prov, FI_LOG_CORE,
				"RMA only instance included\n");
		}
	}

	info_new = fi_dupinfo(&psmx2_prov_info);
	if (info_new) {
		/* 60 bit tag, 32 bit CQ data */
		info_new->addr_format = addr_format;
		info_new->ep_attr->type = ep_type;
		info_new->ep_attr->mem_tag_format >>= 4;
		info_new->domain_attr->cq_data_size = 4;
		info_new->next = info_out;
		info_out = info_new;
		FI_INFO(&psmx2_prov, FI_LOG_CORE,
			"TAG60 instance included\n");
	}

	if (!hints || !hints->domain_attr ||
	    !hints->domain_attr->cq_data_size) {
		info_new = fi_dupinfo(&psmx2_prov_info);
		if (info_new) {
			/* 64 bit tag, no CQ data */
			info_new->addr_format = addr_format;
			info_new->ep_attr->type = ep_type;
			info_new->next = info_out;
			info_out = info_new;
			FI_INFO(&psmx2_prov, FI_LOG_CORE,
				"TAG64 instance included\n");
		}
	}

	*info = info_out;
	return info_out ? 0 : -FI_ENODATA;
}

static void psmx2_dup_addr(int format, struct psmx2_ep_name *addr,
			   void **addr_out, size_t *len)
{
	if (!addr)
		return;

	if (format == FI_ADDR_STR) {
		*addr_out = psmx2_ep_name_to_string(addr, len);
	} else {
		*addr_out = mem_dup(addr, sizeof(*addr));
		*len = sizeof(*addr);
	}
}

static void psmx2_expand_default_unit(struct fi_info *info)
{
	struct fi_info *p, *next;
	struct psmx2_ep_name *src_addr;
	int i;

	p = info;
	while (p) {
		next = p->next;
		src_addr = p->src_addr;
		if (src_addr->unit == PSMX2_DEFAULT_UNIT) {
			if (psmx2_hfi_info.num_active_units == 1) {
				src_addr->unit = psmx2_hfi_info.active_units[0];
			} else {
				for (i = 0; i < psmx2_hfi_info.num_active_units; i++) {
					p->next = fi_dupinfo(p);
					if (!p->next) {
						FI_WARN(&psmx2_prov, FI_LOG_CORE,
							"Failed to duplicate info for HFI unit %d\n",
							psmx2_hfi_info.active_units[i]);
						break;
					}
					p = p->next;
					src_addr = p->src_addr;
					src_addr->unit = psmx2_hfi_info.active_units[i];
				}
			}
		}
		p->next = next;
		p = next;
	}
}

void psmx2_update_prov_info(struct fi_info *info,
			    struct psmx2_ep_name *src_addr,
			    struct psmx2_ep_name *dest_addr)
{
	struct fi_info *p;

	for (p = info; p; p = p->next) {
		psmx2_dup_addr(p->addr_format, src_addr,
			       &p->src_addr, &p->src_addrlen);
		psmx2_dup_addr(p->addr_format, dest_addr,
			       &p->dest_addr, &p->dest_addrlen);
	}

	psmx2_expand_default_unit(info);

	for (p = info; p; p = p->next) {
		int unit = ((struct psmx2_ep_name *)p->src_addr)->unit;

		if (unit == PSMX2_DEFAULT_UNIT || !psmx2_env.multi_ep) {
			p->domain_attr->tx_ctx_cnt = psmx2_hfi_info.free_trx_ctxt;
			p->domain_attr->rx_ctx_cnt = psmx2_hfi_info.free_trx_ctxt;
			p->domain_attr->max_ep_tx_ctx = psmx2_hfi_info.max_trx_ctxt;
			p->domain_attr->max_ep_rx_ctx = psmx2_hfi_info.max_trx_ctxt;
			p->domain_attr->max_ep_stx_ctx = psmx2_hfi_info.max_trx_ctxt;
		} else {
			p->domain_attr->tx_ctx_cnt = psmx2_hfi_info.unit_nfreectxts[unit];
			p->domain_attr->rx_ctx_cnt = psmx2_hfi_info.unit_nfreectxts[unit];
			p->domain_attr->max_ep_tx_ctx = psmx2_hfi_info.unit_nctxts[unit];
			p->domain_attr->max_ep_rx_ctx = psmx2_hfi_info.unit_nctxts[unit];
			p->domain_attr->max_ep_stx_ctx = psmx2_hfi_info.unit_nctxts[unit];
		}

		free(p->domain_attr->name);
		if (unit == PSMX2_DEFAULT_UNIT)
			p->domain_attr->name = strdup(psmx2_hfi_info.default_domain_name);
		else
			asprintf(&p->domain_attr->name, "hfi1_%d", unit);

		p->tx_attr->inject_size = psmx2_env.inject_size;
	}
}

int psmx2_check_prov_info(uint32_t api_version,
			  const struct fi_info *hints,
			  struct fi_info **info)
{
	struct util_prov util_prov = { .prov = &psmx2_prov };
	struct fi_info *next;
	struct fi_info *prev = NULL;
	struct fi_info *curr = *info;
	struct fi_info *new_info = *info;

	while (curr) {
		next = curr->next;
		if (ofi_check_info(&util_prov, curr, api_version, hints)) {
			if (prev)
				prev->next = next;
			else
				new_info = next;
			curr->next = NULL;
			fi_freeinfo(curr);
		} else {
			prev = curr;
		}
		curr = next;
	}

	*info = new_info;
	return new_info ? 0 : -FI_ENODATA;
}

void psmx2_alter_prov_info(uint32_t api_version,
			   const struct fi_info *hints,
			   struct fi_info *info)
{
	int cnt = 0;
	int cq_data_cnt = 0;

	ofi_alter_info(info, hints, api_version);

	/*
	 * Some of the default values are set to simplify info
	 * checking. Now change them back to the preferred values.
	 */
	for (; info; info = info->next) {
		if (!hints || !hints->domain_attr ||
		    !hints->domain_attr->control_progress)
			info->domain_attr->control_progress =
				FI_PROGRESS_MANUAL;

		if (!hints || !hints->domain_attr ||
		    !hints->domain_attr->data_progress)
			info->domain_attr->data_progress =
				FI_PROGRESS_MANUAL;

		if (info->domain_attr->mr_mode == (FI_MR_BASIC | FI_MR_SCALABLE))
			info->domain_attr->mr_mode = FI_MR_SCALABLE;

		/*
		 * Avoid automatically adding secondary caps that may negatively
		 * impact performance.
		 */
		if (hints && hints->caps && !(hints->caps & FI_TRIGGER))
			info->caps &= ~FI_TRIGGER;

		if (info->domain_attr->cq_data_size)
			cq_data_cnt++;

		cnt++;
	}

	FI_INFO(&psmx2_prov, FI_LOG_CORE,
		"%d instances available, %d with CQ data flag set\n",
		cnt, cq_data_cnt);
}

