/*
 * Copyright (c) 2016 Intel Corporation, Inc.  All rights reserved.
 * (C) Copyright 2020 Hewlett Packard Enterprise Development LP
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

#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <ofi_util.h>
#include <ofi_coll.h>
#include "rxm.h"

int rxm_cntr_open(struct fid_domain *domain, struct fi_cntr_attr *attr,
		  struct fid_cntr **cntr_fid, void *context)
{
	int ret;
	struct util_cntr *cntr;

	cntr = calloc(1, sizeof(*cntr));
	if (!cntr)
		return -FI_ENOMEM;

	ret = ofi_cntr_init(&rxm_prov, domain, attr, cntr,
			    &ofi_cntr_progress, context);
	if (ret)
		goto free;

	*cntr_fid = &cntr->cntr_fid;
	return FI_SUCCESS;

free:
	free(cntr);
	return ret;
}

static struct fi_ops_domain rxm_domain_ops = {
	.size = sizeof(struct fi_ops_domain),
	.av_open = rxm_av_open,
	.cq_open = rxm_cq_open,
	.endpoint = rxm_endpoint,
	.scalable_ep = fi_no_scalable_ep,
	.cntr_open = rxm_cntr_open,
	.poll_open = fi_poll_create,
	.stx_ctx = fi_no_stx_context,
	.srx_ctx = fi_no_srx_context,
	.query_atomic = rxm_ep_query_atomic,
	.query_collective = ofi_query_collective,
};

static void rxm_mr_remove_map_entry(struct rxm_mr *mr)
{
	fastlock_acquire(&mr->domain->util_domain.lock);
	(void) ofi_mr_map_remove(&mr->domain->util_domain.mr_map,
				 mr->mr_fid.key);
	fastlock_release(&mr->domain->util_domain.lock);
}

static int rxm_mr_add_map_entry(struct util_domain *domain,
				struct fi_mr_attr *msg_attr,
				struct rxm_mr *rxm_mr)
{
	uint64_t temp_key;
	int ret;

	msg_attr->requested_key = rxm_mr->mr_fid.key;

	fastlock_acquire(&domain->lock);
	ret = ofi_mr_map_insert(&domain->mr_map, msg_attr, &temp_key, rxm_mr);
	if (OFI_UNLIKELY(ret)) {
		FI_WARN(&rxm_prov, FI_LOG_DOMAIN,
			"MR map insert for atomic verification failed %d\n",
			ret);
	} else {
		assert(rxm_mr->mr_fid.key == temp_key);
	}
	fastlock_release(&domain->lock);

	return ret;
}

struct rxm_mr *rxm_mr_get_map_entry(struct rxm_domain *domain, uint64_t key)
{
	struct rxm_mr *mr;

	fastlock_acquire(&domain->util_domain.lock);
	mr = ofi_mr_map_get(&domain->util_domain.mr_map, key);
	fastlock_release(&domain->util_domain.lock);

	return mr;
}

static int rxm_domain_close(fid_t fid)
{
	struct rxm_domain *rxm_domain;
	int ret;

	rxm_domain = container_of(fid, struct rxm_domain, util_domain.domain_fid.fid);

	fastlock_destroy(&rxm_domain->amo_bufpool_lock);
	ofi_bufpool_destroy(rxm_domain->amo_bufpool);

	ret = fi_close(&rxm_domain->msg_domain->fid);
	if (ret)
		return ret;

	ret = ofi_domain_close(&rxm_domain->util_domain);
	if (ret)
		return ret;

	free(rxm_domain);
	return 0;
}

static struct fi_ops rxm_domain_fi_ops = {
	.size = sizeof(struct fi_ops),
	.close = rxm_domain_close,
	.bind = fi_no_bind,
	.control = fi_no_control,
	.ops_open = fi_no_ops_open,
};

static int rxm_mr_close(fid_t fid)
{
	struct rxm_mr *rxm_mr;
	int ret;

	rxm_mr = container_of(fid, struct rxm_mr, mr_fid.fid);

	if (rxm_mr->domain->util_domain.info_domain_caps & FI_ATOMIC)
		rxm_mr_remove_map_entry(rxm_mr);

	ret = fi_close(&rxm_mr->msg_mr->fid);
	if (ret)
		FI_WARN(&rxm_prov, FI_LOG_DOMAIN, "Unable to close MSG MR\n");

	ofi_atomic_dec32(&rxm_mr->domain->util_domain.ref);
	free(rxm_mr);
	return ret;
}

static struct fi_ops rxm_mr_ops = {
	.size = sizeof(struct fi_ops),
	.close = rxm_mr_close,
	.bind = fi_no_bind,
	.control = fi_no_control,
	.ops_open = fi_no_ops_open,
};

int rxm_msg_mr_reg_internal(struct rxm_domain *rxm_domain, const void *buf,
	size_t len, uint64_t acs, uint64_t flags, struct fid_mr **mr)
{
	int ret, tries = 0;

	/* If we can't get a key within 1024 tries, give up */
	do {
		ret = fi_mr_reg(rxm_domain->msg_domain, buf, len, acs, 0,
				rxm_domain->mr_key++ | FI_PROV_SPECIFIC,
				flags, mr, NULL);
	} while (ret == -FI_ENOKEY && tries++ < 1024);

	return ret;
}

void rxm_msg_mr_closev(struct fid_mr **mr, size_t count)
{
	int ret;
	size_t i;

	for (i = 0; i < count; i++) {
		if (mr[i]) {
			ret = fi_close(&mr[i]->fid);
			if (ret)
				FI_WARN(&rxm_prov, FI_LOG_EP_DATA,
					"Unable to close msg mr: %zu\n", i);
			mr[i] = NULL;
		}
	}
}

int rxm_msg_mr_regv(struct rxm_ep *rxm_ep, const struct iovec *iov,
		    size_t count, size_t reg_limit, uint64_t access,
		    struct fid_mr **mr)
{
	struct rxm_domain *rxm_domain;
	size_t i;
	int ret;

	rxm_domain = container_of(rxm_ep->util_ep.domain, struct rxm_domain,
				  util_domain);

	for (i = 0; i < count && reg_limit; i++) {
		size_t len = MIN(iov[i].iov_len, reg_limit);
		ret = rxm_msg_mr_reg_internal(rxm_domain, iov[i].iov_base,
					      len, access, 0, &mr[i]);
		if (ret)
			goto err;
		reg_limit -= len;
	}
	return 0;
err:
	rxm_msg_mr_closev(mr, i);
	return ret;
}

/* Large send/recv transfers use RMA rendezvous protocol */
static uint64_t
rxm_mr_get_msg_access(struct rxm_domain *rxm_domain, uint64_t access)
{
	if (access & FI_SEND) {
		access |= rxm_use_write_rndv ? FI_WRITE : FI_REMOTE_READ;
	}

	if (access & FI_RECV) {
		access |= rxm_use_write_rndv ? FI_REMOTE_WRITE : FI_READ;
	}

	return access;
}

static void rxm_mr_init(struct rxm_mr *rxm_mr, struct rxm_domain *domain,
			void *context)
{
	rxm_mr->mr_fid.fid.fclass = FI_CLASS_MR;
	rxm_mr->mr_fid.fid.context = context;
	rxm_mr->mr_fid.fid.ops = &rxm_mr_ops;
	rxm_mr->mr_fid.mem_desc = rxm_mr;
	rxm_mr->mr_fid.key = fi_mr_key(rxm_mr->msg_mr);
	rxm_mr->domain = domain;
	ofi_atomic_inc32(&domain->util_domain.ref);
}

static int rxm_mr_regattr(struct fid *fid, const struct fi_mr_attr *attr,
			  uint64_t flags, struct fid_mr **mr)
{
	struct rxm_domain *rxm_domain;
	struct fi_mr_attr msg_attr = *attr;
	struct rxm_mr *rxm_mr;
	int ret;

	rxm_domain = container_of(fid, struct rxm_domain,
				  util_domain.domain_fid.fid);

	rxm_mr = calloc(1, sizeof(*rxm_mr));
	if (!rxm_mr)
		return -FI_ENOMEM;

	ofi_mr_update_attr(rxm_domain->util_domain.fabric->fabric_fid.api_version,
			   rxm_domain->util_domain.info_domain_caps, attr,
			   &msg_attr);

	msg_attr.access = rxm_mr_get_msg_access(rxm_domain, attr->access);

	ret = fi_mr_regattr(rxm_domain->msg_domain, &msg_attr,
			    flags, &rxm_mr->msg_mr);
	if (ret) {
		FI_WARN(&rxm_prov, FI_LOG_DOMAIN, "Unable to register MSG MR\n");
		goto err;
	}
	rxm_mr_init(rxm_mr, rxm_domain, attr->context);
	fastlock_init(&rxm_mr->amo_lock);
	rxm_mr->iface = msg_attr.iface;
	rxm_mr->device = msg_attr.device.reserved;
	*mr = &rxm_mr->mr_fid;

	if (rxm_domain->util_domain.info_domain_caps & FI_ATOMIC) {
		ret = rxm_mr_add_map_entry(&rxm_domain->util_domain,
					   &msg_attr, rxm_mr);
		if (ret)
			goto map_err;
	}

	return 0;

map_err:
	fi_close(&rxm_mr->mr_fid.fid);
	return ret;
err:
	free(rxm_mr);
	return ret;

}

static int rxm_mr_regv(struct fid *fid, const struct iovec *iov, size_t count,
		       uint64_t access, uint64_t offset, uint64_t requested_key,
		       uint64_t flags, struct fid_mr **mr, void *context)
{
	struct rxm_domain *rxm_domain;
	struct rxm_mr *rxm_mr;
	int ret;
	struct fi_mr_attr msg_attr = {
		.mr_iov = iov,
		.iov_count = count,
		.access = access,
		.offset = offset,
		.requested_key = requested_key,
		.context = context,
	};

	rxm_domain = container_of(fid, struct rxm_domain,
				  util_domain.domain_fid.fid);

	rxm_mr = calloc(1, sizeof(*rxm_mr));
	if (!rxm_mr)
		return -FI_ENOMEM;

	access = rxm_mr_get_msg_access(rxm_domain, access);

	ret = fi_mr_regv(rxm_domain->msg_domain, iov, count, access, offset,
			 requested_key, flags, &rxm_mr->msg_mr, context);
	if (ret) {
		FI_WARN(&rxm_prov, FI_LOG_DOMAIN, "Unable to register MSG MR\n");
		goto err;
	}
	rxm_mr_init(rxm_mr, rxm_domain, context);
	*mr = &rxm_mr->mr_fid;

	if (rxm_domain->util_domain.info_domain_caps & FI_ATOMIC) {
		ret = rxm_mr_add_map_entry(&rxm_domain->util_domain,
					   &msg_attr, rxm_mr);
		if (ret)
			goto map_err;
	}

	return 0;
map_err:
	fi_close(&rxm_mr->mr_fid.fid);
	return ret;
err:
	free(rxm_mr);
	return ret;
}

static int rxm_mr_reg(struct fid *fid, const void *buf, size_t len,
		      uint64_t access, uint64_t offset, uint64_t requested_key,
		      uint64_t flags, struct fid_mr **mr, void *context)
{
	struct iovec iov;

	iov.iov_base = (void *) buf;
	iov.iov_len = len;
	return rxm_mr_regv(fid, &iov, 1, access, offset, requested_key,
			   flags, mr, context);
}

static struct fi_ops_mr rxm_domain_mr_ops = {
	.size = sizeof(struct fi_ops_mr),
	.reg = rxm_mr_reg,
	.regv = rxm_mr_regv,
	.regattr = rxm_mr_regattr,
};

static ssize_t rxm_send_credits(struct fid_ep *ep, size_t credits)
{
	struct rxm_conn *rxm_conn = ep->fid.context;
	struct rxm_ep *rxm_ep = rxm_conn->ep;
	struct rxm_deferred_tx_entry *def_tx_entry;
	struct rxm_tx_buf *tx_buf;
	struct iovec iov;
	struct fi_msg msg;
	ssize_t ret;

	tx_buf = ofi_buf_alloc(rxm_ep->tx_pool);
	if (!tx_buf) {
		FI_WARN(&rxm_prov, FI_LOG_EP_DATA,
			"Ran out of buffers from TX credit buffer pool.\n");
		return -FI_ENOMEM;
	}

	tx_buf->hdr.state = RXM_CREDIT_TX;
	rxm_ep_format_tx_buf_pkt(rxm_conn, 0, rxm_ctrl_credit, 0, 0, FI_SEND,
				 &tx_buf->pkt);
	tx_buf->pkt.ctrl_hdr.type = rxm_ctrl_credit;
	tx_buf->pkt.ctrl_hdr.msg_id = ofi_buf_index(tx_buf);
	tx_buf->pkt.ctrl_hdr.ctrl_data = credits;

	if (rxm_conn->state != RXM_CM_CONNECTED)
		goto defer;

	iov.iov_base = &tx_buf->pkt;
	iov.iov_len = sizeof(struct rxm_pkt);
	msg.msg_iov = &iov;
	msg.iov_count = 1;
	msg.context = tx_buf;
	msg.desc = &tx_buf->hdr.desc;

	ret = fi_sendmsg(ep, &msg, FI_PRIORITY);
	if (!ret)
		return FI_SUCCESS;

defer:
	def_tx_entry = rxm_ep_alloc_deferred_tx_entry(
		rxm_ep, rxm_conn, RXM_DEFERRED_TX_CREDIT_SEND);
	if (!def_tx_entry) {
		FI_WARN(&rxm_prov, FI_LOG_CQ,
			"unable to allocate TX entry for deferred CREDIT mxg\n");
		ofi_buf_free(tx_buf);
		return -FI_ENOMEM;
	}

	def_tx_entry->credit_msg.tx_buf = tx_buf;
	rxm_queue_deferred_tx(def_tx_entry, OFI_LIST_HEAD);
	return FI_SUCCESS;
}

static void rxm_no_set_threshold(struct fid_ep *ep_fid, size_t threshold)
{ }

static void rxm_no_add_credits(struct fid_ep *ep_fid, size_t credits)
{ }

static void rxm_no_credit_handler(struct fid_domain *domain_fid,
		ssize_t (*credit_handler)(struct fid_ep *ep, size_t credits))
{ }

static int rxm_no_enable_flow_ctrl(struct fid_ep *ep_fid)
{
	return -FI_ENOSYS;
}

struct ofi_ops_flow_ctrl rxm_no_ops_flow_ctrl = {
	.size = sizeof(struct ofi_ops_flow_ctrl),
	.set_threshold = rxm_no_set_threshold,
	.add_credits = rxm_no_add_credits,
	.enable = rxm_no_enable_flow_ctrl,
	.set_send_handler = rxm_no_credit_handler,
};

static int rxm_config_flow_ctrl(struct rxm_domain *domain)
{
	struct ofi_ops_flow_ctrl *flow_ctrl_ops;
	int ret;

	ret = fi_open_ops(&domain->msg_domain->fid, OFI_OPS_FLOW_CTRL, 0,
			  (void **) &flow_ctrl_ops, NULL);
	if (ret) {
		if (ret == -FI_ENOSYS) {
			domain->flow_ctrl_ops = &rxm_no_ops_flow_ctrl;
			return 0;
		}
		return ret;
	}

	assert(flow_ctrl_ops);
	domain->flow_ctrl_ops = flow_ctrl_ops;
	domain->flow_ctrl_ops->set_send_handler(domain->msg_domain,
						rxm_send_credits);
	return 0;
}

struct ofi_ops_dynamic_rbuf rxm_dynamic_rbuf = {
	.size = sizeof(struct ofi_ops_dynamic_rbuf),
	.get_rbuf = rxm_get_dyn_rbuf,
};

static void rxm_config_dyn_rbuf(struct rxm_domain *domain, struct fi_info *info,
				struct fi_info *msg_info)
{
	int ret = 1;

	/* Collective support requires rxm generated and consumed messages.
	 * Although we could update the code to handle receiving collective
	 * messages, collective support is mostly for development purposes.
	 * So, fallback to bounce buffers when enabled.
	 * We also can't pass through HMEM buffers, unless the lower layer
	 * can handle them.
	 */
	if ((info->caps & FI_COLLECTIVE) ||
	    ((info->caps & FI_HMEM) && !(msg_info->caps & FI_HMEM)))
		return;

	fi_param_get_bool(&rxm_prov, "enable_dyn_rbuf", &ret);
	domain->dyn_rbuf = (ret != 0);
	if (!domain->dyn_rbuf)
		return;

	ret = fi_set_ops(&domain->msg_domain->fid, OFI_OPS_DYNAMIC_RBUF, 0,
			 (void *) &rxm_dynamic_rbuf, NULL);
	domain->dyn_rbuf = (ret == FI_SUCCESS);

	if (domain->dyn_rbuf) {
		domain->rx_post_size = sizeof(struct rxm_pkt);
	}
}

int rxm_domain_open(struct fid_fabric *fabric, struct fi_info *info,
		struct fid_domain **domain, void *context)
{
	struct rxm_domain *rxm_domain;
	struct rxm_fabric *rxm_fabric;
	struct fi_info *msg_info;
	int ret;

	rxm_domain = calloc(1, sizeof(*rxm_domain));
	if (!rxm_domain)
		return -FI_ENOMEM;

	rxm_fabric = container_of(fabric, struct rxm_fabric, util_fabric.fabric_fid);

	ret = ofi_get_core_info(fabric->api_version, NULL, NULL, 0, &rxm_util_prov,
				info, NULL, rxm_info_to_core, &msg_info);
	if (ret)
		goto err1;

	ret = fi_domain(rxm_fabric->msg_fabric, msg_info,
			&rxm_domain->msg_domain, context);
	if (ret)
		goto err2;

	ret = ofi_domain_init(fabric, info, &rxm_domain->util_domain, context);
	if (ret) {
		goto err3;
	}

	/* We turn off the mr map mode bit FI_MR_PROV_KEY.  We always use the
	 * key returned by the MSG provider.  That key may be generated by the
	 * MSG provider, or will be provided as input by the rxm provider.
	 */
	rxm_domain->util_domain.mr_map.mode &= ~FI_MR_PROV_KEY;

	rxm_domain->max_atomic_size = rxm_ep_max_atomic_size(info);
	rxm_domain->rx_post_size = rxm_packet_size;

	*domain = &rxm_domain->util_domain.domain_fid;
	(*domain)->fid.ops = &rxm_domain_fi_ops;
	/* Replace MR ops set by ofi_domain_init() */
	(*domain)->mr = &rxm_domain_mr_ops;
	(*domain)->ops = &rxm_domain_ops;

	ret = ofi_bufpool_create(&rxm_domain->amo_bufpool,
				 rxm_domain->max_atomic_size, 64, 0, 0, 0);
	if (ret)
		goto err3;

	fastlock_init(&rxm_domain->amo_bufpool_lock);

	ret = rxm_config_flow_ctrl(rxm_domain);
	if (ret)
		goto err4;

	rxm_config_dyn_rbuf(rxm_domain, info, msg_info);

	fi_freeinfo(msg_info);
	return 0;
err4:
	fastlock_destroy(&rxm_domain->amo_bufpool_lock);
	ofi_bufpool_destroy(rxm_domain->amo_bufpool);
err3:
	fi_close(&rxm_domain->msg_domain->fid);
err2:
	fi_freeinfo(msg_info);
err1:
	free(rxm_domain);
	return ret;
}
