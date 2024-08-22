/*
 * SPDX-License-Identifier: BSD-2-Clause OR GPL-2.0-only
 *
 * Copyright (c) 2014 Intel Corporation, Inc. All rights reserved.
 * Copyright (c) 2017 DataDirect Networks, Inc. All rights reserved.
 * Copyright (c) 2018,2020-2023 Hewlett Packard Enterprise Development LP
 */

#include "config.h"

#include <stdlib.h>
#include <string.h>

#include <ofi_util.h>

#include "cxip.h"

#define CXIP_DBG(...) _CXIP_DBG(FI_LOG_DOMAIN, __VA_ARGS__)
#define CXIP_WARN(...) _CXIP_WARN(FI_LOG_DOMAIN, __VA_ARGS__)

extern struct fi_ops_mr cxip_dom_mr_ops;

static void cxip_domain_cmdq_free(struct cxip_domain *dom)
{
	struct cxip_domain_cmdq *cmdq;

	while ((cmdq = dlist_first_entry_or_null(&dom->cmdq_list,
						 struct cxip_domain_cmdq,
						 entry))) {

		cxip_cmdq_free(cmdq->cmdq);
		dlist_remove(&cmdq->entry);
		dom->cmdq_cnt--;
		free(cmdq);
	}
}

static int cxip_domain_cmdq_alloc(struct cxip_domain *dom,
				  uint16_t vni,
				  enum cxi_traffic_class tc,
				  struct cxip_domain_cmdq **dom_cmdq)
{
	struct cxip_domain_cmdq *cmdq;
	struct cxi_cq_alloc_opts cq_opts = {
		.flags = CXI_CQ_IS_TX,
	};
	int ret;

	cmdq = calloc(1, sizeof(*cmdq));
	if (!cmdq) {
		CXIP_WARN("Failed to allocate cmdq memory\n");
		return -FI_ENOMEM;
	}

	/* Domain managed transmit command queues require being updated on
	 * empty to be able to safely change communication profile VNI.
	 */
	cq_opts.policy = CXI_CQ_UPDATE_HIGH_FREQ_EMPTY;

	/* An IDC command can use up to 4x 64 byte slots. */
	cq_opts.count = 4 * dom->tx_size;

	ret = cxip_cmdq_alloc(dom->lni, NULL, &cq_opts, vni, tc,
			      CXI_TC_TYPE_DEFAULT, &cmdq->cmdq);
	if (ret) {
		CXIP_WARN("Failed to allocate cmdq: %d\n", ret);
		goto err_free_mem;
	}

	dlist_insert_head(&cmdq->entry, &dom->cmdq_list);
	dom->cmdq_cnt++;

	*dom_cmdq = cmdq;

	return FI_SUCCESS;

err_free_mem:
	free(cmdq);

	return ret;
}

/* Hardware only allows for 16 different command profiles per RGID. Since each
 * domain maps to a single RGID, this means effectively limits the number of
 * TX command queue per domain to be 16. Since one TX command queue is
 * reserved for triggered commands, real number is 15.
 */
#define MAX_DOM_TX_CMDQ 15U

static int cxip_domain_find_cmdq(struct cxip_domain *dom,
				 uint16_t vni,
				 enum cxi_traffic_class tc,
				 struct cxip_domain_cmdq **dom_cmdq)
{
	struct cxip_domain_cmdq *cmdq;
	int ret;

	/* Prefer existing command queues. */
	dlist_foreach_container(&dom->cmdq_list, struct cxip_domain_cmdq, cmdq,
				entry) {
		if (cxip_cmdq_match(cmdq->cmdq, vni, tc,
				    CXI_TC_TYPE_DEFAULT)) {
			*dom_cmdq = cmdq;
			return FI_SUCCESS;
		}
	}

	/* Prefer reusing an empty command queue instead of allocating a new
	 * one.
	 */
	dlist_foreach_container(&dom->cmdq_list, struct cxip_domain_cmdq, cmdq,
				entry) {
		if (cxip_cmdq_empty(cmdq->cmdq)) {

			/* TODO: This needs to use new direct CP profile feature
			 * which disables sharing of communication profile
			 * across TX command queues.
			 */
			ret = cxip_cmdq_cp_set(cmdq->cmdq, vni, tc,
					       CXI_TC_TYPE_DEFAULT);
			if (ret) {
				CXIP_WARN("Failed to change communication profile: %d\n",
					  ret);
				return ret;
			}

			*dom_cmdq = cmdq;
			return FI_SUCCESS;
		}
	}

	/* Last resort is allocating a new transmit command queue. If limit has
	 * been reached, only option is to change communication profile for
	 * existing TX cmdq.
	 */
	if (dom->cmdq_cnt == MAX_DOM_TX_CMDQ) {
		CXIP_WARN("At domain command queue max\n");
		return -FI_EAGAIN;
	}

	ret = cxip_domain_cmdq_alloc(dom, vni, tc, &cmdq);
	if (ret) {
		CXIP_WARN("Failed to allocate domain command queue: %d\n", ret);
		return ret;
	}

	*dom_cmdq = cmdq;

	return FI_SUCCESS;
}

int cxip_domain_emit_idc_put(struct cxip_domain *dom, uint16_t vni,
			     enum cxi_traffic_class tc,
			     const struct c_cstate_cmd *c_state,
			     const struct c_idc_put_cmd *put, const void *buf,
			     size_t len, uint64_t flags)
{
	int ret;
	struct cxip_domain_cmdq *cmdq;

	ofi_genlock_lock(&dom->cmdq_lock);

	ret = cxip_domain_find_cmdq(dom, vni, tc, &cmdq);
	if (ret) {
		CXIP_WARN("Failed to find command queue: %d\n", ret);
		goto out_unlock;
	}

	ret = cxip_cmdq_emit_idc_put(cmdq->cmdq, c_state, put, buf, len, flags);
	if (ret) {
		CXIP_WARN("Failed to emit idc_put: %d\n", ret);
		goto out_unlock;
	}

	cxi_cq_ring(cmdq->cmdq->dev_cmdq);

	ofi_genlock_unlock(&dom->cmdq_lock);

	return FI_SUCCESS;

out_unlock:
	ofi_genlock_unlock(&dom->cmdq_lock);

	return ret;
}

int cxip_domain_emit_dma(struct cxip_domain *dom, uint16_t vni,
			 enum cxi_traffic_class tc, struct c_full_dma_cmd *dma,
			 uint64_t flags)
{
	int ret;
	struct cxip_domain_cmdq *cmdq;

	ofi_genlock_lock(&dom->cmdq_lock);

	ret = cxip_domain_find_cmdq(dom, vni, tc, &cmdq);
	if (ret) {
		CXIP_WARN("Failed to find command queue: %d\n", ret);
		goto out_unlock;
	}

	ret = cxip_cmdq_emit_dma(cmdq->cmdq, dma, flags);
	if (ret) {
		CXIP_WARN("Failed to emit dma: %d\n", ret);
		goto out_unlock;
	}

	cxi_cq_ring(cmdq->cmdq->dev_cmdq);

	ofi_genlock_unlock(&dom->cmdq_lock);

	return FI_SUCCESS;

out_unlock:
	ofi_genlock_unlock(&dom->cmdq_lock);

	return ret;
}

int cxip_domain_emit_idc_amo(struct cxip_domain *dom, uint16_t vni,
			     enum cxi_traffic_class tc,
			     const struct c_cstate_cmd *c_state,
			     const struct c_idc_amo_cmd *amo, uint64_t flags,
			     bool fetching, bool flush)
{
	int ret;
	struct cxip_domain_cmdq *cmdq;

	ofi_genlock_lock(&dom->cmdq_lock);

	ret = cxip_domain_find_cmdq(dom, vni, tc, &cmdq);
	if (ret) {
		CXIP_WARN("Failed to find command queue: %d\n", ret);
		goto out_unlock;
	}

	ret = cxip_cmdq_emic_idc_amo(cmdq->cmdq, c_state, amo, flags,
				     fetching, flush);
	if (ret) {
		CXIP_WARN("Failed to emit idc_amo: %d\n", ret);
		goto out_unlock;
	}

	cxi_cq_ring(cmdq->cmdq->dev_cmdq);

	ofi_genlock_unlock(&dom->cmdq_lock);

	return FI_SUCCESS;

out_unlock:
	ofi_genlock_unlock(&dom->cmdq_lock);

	return ret;
}

int cxip_domain_emit_dma_amo(struct cxip_domain *dom, uint16_t vni,
			     enum cxi_traffic_class tc,
			     struct c_dma_amo_cmd *amo, uint64_t flags,
			     bool fetching, bool flush)
{
	int ret;
	struct cxip_domain_cmdq *cmdq;

	ofi_genlock_lock(&dom->cmdq_lock);

	ret = cxip_domain_find_cmdq(dom, vni, tc, &cmdq);
	if (ret) {
		CXIP_WARN("Failed to find command queue: %d\n", ret);
		goto out_unlock;
	}

	ret = cxip_cmdq_emit_dma_amo(cmdq->cmdq, amo, flags, fetching, flush);
	if (ret) {
		CXIP_WARN("Failed to emit amo: %d\n", ret);
		goto out_unlock;
	}

	cxi_cq_ring(cmdq->cmdq->dev_cmdq);

	ofi_genlock_unlock(&dom->cmdq_lock);

	return FI_SUCCESS;

out_unlock:
	ofi_genlock_unlock(&dom->cmdq_lock);

	return ret;
}

int cxip_domain_emit_idc_msg(struct cxip_domain *dom, uint16_t vni,
			     enum cxi_traffic_class tc,
			     const struct c_cstate_cmd *c_state,
			     const struct c_idc_msg_hdr *msg, const void *buf,
			     size_t len, uint64_t flags)
{
	int ret;
	struct cxip_domain_cmdq *cmdq;

	ofi_genlock_lock(&dom->cmdq_lock);

	ret = cxip_domain_find_cmdq(dom, vni, tc, &cmdq);
	if (ret) {
		CXIP_WARN("Failed to find command queue: %d\n", ret);
		goto out_unlock;
	}

	ret = cxip_cmdq_emit_idc_msg(cmdq->cmdq, c_state, msg, buf, len,
				     flags);
	if (ret) {
		CXIP_WARN("Failed to emit idc msg: %d\n", ret);
		goto out_unlock;
	}

	cxi_cq_ring(cmdq->cmdq->dev_cmdq);

	ofi_genlock_unlock(&dom->cmdq_lock);

	return FI_SUCCESS;

out_unlock:
	ofi_genlock_unlock(&dom->cmdq_lock);

	return ret;
}

/*
 * cxip_domain_req_alloc() - Allocate a domain control buffer ID
 */
int cxip_domain_ctrl_id_alloc(struct cxip_domain *dom,
			      struct cxip_ctrl_req *req)
{
	int buffer_id;

	ofi_spin_lock(&dom->ctrl_id_lock);
	buffer_id = ofi_idx_insert(&dom->req_ids, req);
	if (buffer_id < 0 || buffer_id >= CXIP_BUFFER_ID_MAX) {
		CXIP_WARN("Failed to allocate MR buffer ID: %d\n",
			  buffer_id);
		ofi_spin_unlock(&dom->ctrl_id_lock);
		return -FI_ENOSPC;
	}

	ofi_spin_unlock(&dom->ctrl_id_lock);
	req->req_id = buffer_id;

	return FI_SUCCESS;
}

/*
 * cxip_domain_ctrl_id_free() - Free a domain wide control buffer id.
 */
void cxip_domain_ctrl_id_free(struct cxip_domain *dom,
			      struct cxip_ctrl_req *req)
{
	/* Non-remote MR will not have a buffer ID assigned */
	if (req->req_id < 0)
		return;

	ofi_spin_lock(&dom->ctrl_id_lock);
	ofi_idx_remove(&dom->req_ids, req->req_id);
	ofi_spin_unlock(&dom->ctrl_id_lock);
}

/*
 * cxip_domain_prov_mr_key_alloc() - Allocate a domain unique
 * non-cached FI_MR_PROV_KEY key ID.
 */
int cxip_domain_prov_mr_id_alloc(struct cxip_domain *dom,
				 struct cxip_mr *mr)
{
	struct cxip_mr_key key = {};
	int buffer_id;

	/* Allocations favor optimized MR range (if enabled) */
	ofi_spin_lock(&dom->ctrl_id_lock);
	buffer_id = ofi_idx_insert(&dom->mr_ids, mr);
	if (buffer_id < 0 || buffer_id >= CXIP_BUFFER_ID_MAX) {
		CXIP_WARN("Failed to allocate FI_MR_PROV_KEY MR ID: %d\n",
			  buffer_id);
		ofi_spin_unlock(&dom->ctrl_id_lock);
		return -FI_ENOSPC;
	}

	/* IDX 0 is reserved and should never be returned */
	assert(buffer_id > 0);
	buffer_id = buffer_id - 1;

	mr->mr_id = buffer_id;
	key.is_prov = 1;
	key.id = buffer_id;
	key.seqnum = ++dom->prov_key_seqnum;

	/* Let the source know events are required and it should use
	 * unrestricted operations.
	 */
	key.events = mr->count_events || mr->rma_events || mr->cntr;

	key.opt = cxip_env.optimized_mrs &&
			key.id < CXIP_PTL_IDX_PROV_MR_OPT_CNT;
	mr->key = key.raw;
	ofi_spin_unlock(&dom->ctrl_id_lock);

	return FI_SUCCESS;
}

/*
 * cxip_domain_prov_mr_id_free() - Free a domain wide FI_MR_PROV_KEY MR id.
 */
void cxip_domain_prov_mr_id_free(struct cxip_domain *dom,
				 struct cxip_mr *mr)
{
	struct cxip_mr_key key = {
		.raw = mr->key,
	};

	/* Only non-cached FI_MR_PROV_KEY MR require MR ID */
	if (mr->mr_id < 0)
		return;

	ofi_spin_lock(&dom->ctrl_id_lock);
	ofi_idx_remove(&dom->mr_ids, key.id + 1);
	ofi_spin_unlock(&dom->ctrl_id_lock);
}

#define TLE_RESERVED 8U

/*
 * cxip_domain_enable() - Enable an FI Domain for use.
 *
 * Allocate hardware resources and initialize software to prepare the Domain
 * for use.
 */
static int cxip_domain_enable(struct cxip_domain *dom)
{
	int ret = FI_SUCCESS;
	struct cxi_svc_desc svc_desc;

	ofi_spin_lock(&dom->lock);

	if (dom->enabled)
		goto unlock;

	ret = cxip_get_if(dom->nic_addr, &dom->iface);
	if (ret != FI_SUCCESS) {
		CXIP_WARN("Unable to get IF\n");
		ret = -FI_ENODEV;
		goto unlock;
	}

	ret = cxil_get_svc(dom->iface->dev, dom->auth_key.svc_id, &svc_desc);
	if (ret) {
		CXIP_WARN("cxil_get_svc with %s and svc_id %d failed: %d:%s\n",
			  dom->iface->dev->info.device_name,
			  dom->auth_key.svc_id, ret, strerror(-ret));
		ret = -FI_EINVAL;
		goto put_if;
	}

	if (!svc_desc.restricted_members)
		CXIP_WARN("Security Issue: Using unrestricted service ID %d for %s. "
			  "Please provide a service ID via auth_key fields.\n",
			  dom->auth_key.svc_id,
			  dom->iface->dev->info.device_name);
	if (!svc_desc.restricted_vnis)
		CXIP_WARN("Security Issue: Using service ID %d with unrestricted VNI access %s. "
			  "Please provide a service ID via auth_key fields.\n",
			  dom->auth_key.svc_id,
			  dom->iface->dev->info.device_name);

	/* Need to reserved TLEs to prevent stalling. */
	dom->max_trig_op_in_use =
		svc_desc.limits.type[CXI_RSRC_TYPE_TLE].res - TLE_RESERVED;

	ret = cxip_alloc_lni(dom->iface, dom->auth_key.svc_id, &dom->lni);
	if (ret) {
		CXIP_WARN("cxip_alloc_lni returned: %d\n", ret);
		ret = -FI_ENODEV;
		goto put_if;
	}

	ret = cxip_iomm_init(dom);
	if (ret != FI_SUCCESS) {
		CXIP_WARN("Failed to initialize IOMM: %d\n", ret);
		assert(ret == -FI_ENOMEM);
		goto free_lni;
	}

	ret = cxil_get_amo_remap_to_pcie_fadd(dom->iface->dev,
					      &dom->amo_remap_to_pcie_fadd);
	if (ret) {
		CXIP_WARN("Failed to get amo_remap_to_pcie_fadd value: %d\n",
			  ret);
		goto iomm_fini;
	}

	cxip_mr_domain_init(&dom->mr_domain);

	dom->enabled = true;
	ofi_spin_unlock(&dom->lock);

	DOM_INFO(dom, "Domain enabled\n");

	/* Telemetry are considered optional and will not stop domain
	 * allocation.
	 */
	ret = cxip_telemetry_alloc(dom, &dom->telemetry);
	if (ret == FI_SUCCESS)
		DOM_INFO(dom, "Telemetry collection enabled\n");

	return FI_SUCCESS;

iomm_fini:
	cxip_iomm_fini(dom);
free_lni:
	cxip_free_lni(dom->lni);
	dom->lni = NULL;
put_if:
	cxip_put_if(dom->iface);
	dom->iface = NULL;
unlock:
	ofi_spin_unlock(&dom->lock);

	return ret;
}

/*
 * cxip_domain_disable() - Disable an FI Domain.
 */
static void cxip_domain_disable(struct cxip_domain *dom)
{
	ofi_spin_lock(&dom->lock);

	if (!dom->enabled)
		goto unlock;

	DOM_INFO(dom, "Domain disabled\n");

	cxip_mr_domain_fini(&dom->mr_domain);
	cxip_dom_cntr_disable(dom);
	cxip_iomm_fini(dom);
	cxip_free_lni(dom->lni);
	cxip_put_if(dom->iface);

	dom->enabled = false;

unlock:
	ofi_spin_unlock(&dom->lock);
}

/*
 * cxip_dom_close() - Provider fi_close implementation for an FI Domain object.
 */
static int cxip_dom_close(struct fid *fid)
{
	struct cxip_domain *dom;

	dom = container_of(fid, struct cxip_domain,
			   util_domain.domain_fid.fid);
	if (ofi_atomic_get32(&dom->ref))
		return -FI_EBUSY;

	if (dom->telemetry) {
		cxip_telemetry_dump_delta(dom->telemetry);
		cxip_telemetry_free(dom->telemetry);
	}

	cxip_domain_cmdq_free(dom);
	cxip_domain_disable(dom);

	assert(dlist_empty(&dom->cmdq_list));
	assert(dom->cmdq_cnt == 0);

	ofi_spin_destroy(&dom->lock);
	ofi_spin_destroy(&dom->ctrl_id_lock);
	ofi_idx_reset(&dom->req_ids);
	ofi_idx_reset(&dom->mr_ids);
	ofi_domain_close(&dom->util_domain);
	ofi_genlock_destroy(&dom->cmdq_lock);
	free(dom);

	return 0;
}

/*
 * cxip_dom_bind() - Provider fi_domain_bind implementation.
 */
static int cxip_dom_bind(struct fid *fid, struct fid *bfid, uint64_t flags)
{
	struct cxip_domain *dom;
	struct cxip_eq *eq;

	dom = container_of(fid, struct cxip_domain, util_domain.domain_fid.fid);
	eq = container_of(bfid, struct cxip_eq, util_eq.eq_fid.fid);

	if (dom->eq)
		return -FI_EINVAL;

	dom->eq = eq;
	if (flags & FI_REG_MR)
		dom->mr_eq = eq;

	return 0;
}

static int cxip_dom_dwq_op_send(struct cxip_domain *dom, struct fi_op_msg *msg,
				struct cxip_cntr *trig_cntr,
				struct cxip_cntr *comp_cntr,
				uint64_t trig_thresh)
{
	struct cxip_ep *ep = container_of(msg->ep, struct cxip_ep, ep);
	struct cxip_txc *txc = ep->ep_obj->txc;
	const void *buf;
	size_t len;
	int ret;

	if (!msg || msg->msg.iov_count > 1)
		return -FI_EINVAL;

	/* FI_INJECT is not supported for triggered sends */
	if (msg->flags & FI_INJECT) {
		CXIP_WARN("FI_INJECT not supported for triggered op\n");
		return -FI_EINVAL;
	}

	/* To prevent triggered operation exhaustion, FI_MORE cannot be
	 * supported.
	 */
	msg->flags &= ~FI_MORE;

	buf = msg->msg.iov_count ? msg->msg.msg_iov[0].iov_base : NULL;
	len = msg->msg.iov_count ? msg->msg.msg_iov[0].iov_len : 0;

	ret = txc->ops.send_common(txc, ep->tx_attr.tclass, buf, len, NULL,
				   msg->msg.data, msg->msg.addr, 0,
				   msg->msg.context, msg->flags, false, true,
				   trig_thresh, trig_cntr, comp_cntr);
	if (ret)
		CXIP_DBG("Failed to emit message triggered op, ret=%d\n", ret);
	else
		CXIP_DBG("Queued triggered message op with threshold %lu\n",
			 trig_thresh);

	return ret;
}

static int cxip_dom_dwq_op_tsend(struct cxip_domain *dom,
				 struct fi_op_tagged *tagged,
				 struct cxip_cntr *trig_cntr,
				 struct cxip_cntr *comp_cntr,
				 uint64_t trig_thresh)
{
	struct cxip_ep *ep = container_of(tagged->ep, struct cxip_ep, ep);
	struct cxip_txc *txc = ep->ep_obj->txc;
	const void *buf;
	size_t len;
	int ret;

	if (!tagged || tagged->msg.iov_count > 1)
		return -FI_EINVAL;

	/* FI_INJECT is not supported for triggered tsends */
	if (tagged->flags & FI_INJECT) {
		CXIP_WARN("FI_INJECT not supported for triggered op\n");
		return -FI_EINVAL;
	}

	/* To prevent triggered operation exhaustion, FI_MORE cannot be
	 * supported.
	 */
	tagged->flags &= ~FI_MORE;

	buf = tagged->msg.iov_count ? tagged->msg.msg_iov[0].iov_base : NULL;
	len = tagged->msg.iov_count ? tagged->msg.msg_iov[0].iov_len : 0;

	ret = txc->ops.send_common(txc, ep->tx_attr.tclass, buf, len, NULL,
				   tagged->msg.data, tagged->msg.addr,
				   tagged->msg.tag, tagged->msg.context,
				   tagged->flags, true, true, trig_thresh,
				   trig_cntr, comp_cntr);
	if (ret)
		CXIP_DBG("Failed to emit tagged msg triggered op, ret=%d\n",
			 ret);
	else
		CXIP_DBG("Queued triggered tagged msg op with threshold %lu\n",
			 trig_thresh);

	return ret;
}

static int cxip_dom_dwq_op_rma(struct cxip_domain *dom, struct fi_op_rma *rma,
			       enum fi_op_type op, struct cxip_cntr *trig_cntr,
			       struct cxip_cntr *comp_cntr,
			       uint64_t trig_thresh)
{
	struct cxip_ep *ep = container_of(rma->ep, struct cxip_ep, ep);
	const void *buf;
	size_t len;
	int ret;

	if (!rma || !rma->msg.msg_iov || rma->msg.iov_count > 1 ||
	    !rma->msg.rma_iov || rma->msg.rma_iov_count != 1)
		return -FI_EINVAL;

	/* To prevent triggered operation exhaustion, FI_MORE cannot be
	 * supported.
	 */
	rma->flags &= ~FI_MORE;

	buf = rma->msg.iov_count ? rma->msg.msg_iov[0].iov_base : NULL;
	len = rma->msg.iov_count ? rma->msg.msg_iov[0].iov_len : 0;

	ret = cxip_rma_common(op, ep->ep_obj->txc, buf, len, NULL,
			      rma->msg.addr, rma->msg.rma_iov[0].addr,
			      rma->msg.rma_iov[0].key, rma->msg.data,
			      rma->flags, ep->tx_attr.tclass,
			      ep->tx_attr.msg_order, rma->msg.context, true,
			      trig_thresh, trig_cntr, comp_cntr);
	if (ret)
		CXIP_DBG("Failed to emit RMA triggered op, ret=%d\n", ret);
	else
		CXIP_DBG("Queued triggered RMA operation with threshold %lu\n",
			 trig_thresh);

	return ret;
}

static int cxip_dom_dwq_op_atomic(struct cxip_domain *dom,
				  struct fi_op_atomic *amo,
				  struct cxip_cntr *trig_cntr,
				  struct cxip_cntr *comp_cntr,
				  uint64_t trig_thresh)
{
	struct cxip_ep *ep = container_of(amo->ep, struct cxip_ep, ep);
	struct cxip_txc *txc = ep->ep_obj->txc;
	int ret;

	if (!amo)
		return -FI_EINVAL;

	/* To prevent triggered operation exhaustion, FI_MORE cannot be
	 * supported.
	 */
	amo->flags &= ~FI_MORE;

	ret = cxip_amo_common(CXIP_RQ_AMO, txc, txc->tclass, &amo->msg,
			      NULL, NULL, 0, NULL, NULL, 0, amo->flags,
			      true, trig_thresh, trig_cntr, comp_cntr);
	if (ret)
		CXIP_DBG("Failed to emit AMO triggered op, ret=%d\n", ret);
	else
		CXIP_DBG("Queued triggered AMO operation with threshold %lu\n",
			 trig_thresh);

	return ret;
}

static int cxip_dom_dwq_op_fetch_atomic(struct cxip_domain *dom,
					struct fi_op_fetch_atomic *fetch_amo,
					struct cxip_cntr *trig_cntr,
					struct cxip_cntr *comp_cntr,
					uint64_t trig_thresh)
{
	struct cxip_ep *ep = container_of(fetch_amo->ep, struct cxip_ep, ep);
	struct cxip_txc *txc = ep->ep_obj->txc;
	int ret;

	if (!fetch_amo)
		return -FI_EINVAL;

	/* To prevent triggered operation exhaustion, FI_MORE cannot be
	 * supported.
	 */
	fetch_amo->flags &= ~FI_MORE;

	ret = cxip_amo_common(CXIP_RQ_AMO_FETCH, txc, txc->tclass,
			      &fetch_amo->msg, NULL, NULL, 0,
			      fetch_amo->fetch.msg_iov, fetch_amo->fetch.desc,
			      fetch_amo->fetch.iov_count, fetch_amo->flags,
			      true, trig_thresh, trig_cntr, comp_cntr);
	if (ret)
		CXIP_DBG("Failed to emit fetching AMO triggered op, ret=%d\n",
			 ret);
	else
		CXIP_DBG("Queued triggered fetching AMO op with threshold %lu\n",
			 trig_thresh);

	return ret;
}

static int cxip_dom_dwq_op_comp_atomic(struct cxip_domain *dom,
				       struct fi_op_compare_atomic *comp_amo,
				       struct cxip_cntr *trig_cntr,
				       struct cxip_cntr *comp_cntr,
				       uint64_t trig_thresh)
{
	struct cxip_ep *ep = container_of(comp_amo->ep, struct cxip_ep, ep);
	struct cxip_txc *txc = ep->ep_obj->txc;
	int ret;

	if (!comp_amo)
		return -FI_EINVAL;

	/* To prevent triggered operation exhaustion, FI_MORE cannot be
	 * supported.
	 */
	comp_amo->flags &= ~FI_MORE;

	ret = cxip_amo_common(CXIP_RQ_AMO_SWAP, txc, txc->tclass,
			      &comp_amo->msg, comp_amo->compare.msg_iov,
			      comp_amo->compare.desc,
			      comp_amo->compare.iov_count,
			      comp_amo->fetch.msg_iov, comp_amo->fetch.desc,
			      comp_amo->fetch.iov_count, comp_amo->flags, true,
			      trig_thresh, trig_cntr, comp_cntr);
	if (ret)
		CXIP_DBG("Failed to emit compare AMO triggered op, ret=%d\n",
			 ret);
	else
		CXIP_DBG("Queued triggered compare AMO op with threshold %lu\n",
			 trig_thresh);

	return ret;
}

static int cxip_dom_dwq_op_cntr(struct cxip_domain *dom,
				struct fi_op_cntr *cntr, enum fi_op_type op,
				struct cxip_cntr *trig_cntr,
				struct cxip_cntr *comp_cntr,
				uint64_t trig_thresh,
				bool cntr_wb)
{
	struct cxip_cntr *op_cntr;
	int ret;
	unsigned opcode;
	struct c_ct_cmd cmd = {};

	/* Completion counter must be NULL. */
	if (!cntr || !cntr->cntr || comp_cntr)
		return -FI_EINVAL;

	if (cntr_wb) {
		opcode = C_CMD_CT_TRIG_EVENT;
		cmd.eq = C_EQ_NONE;
	} else {
		opcode = op == FI_OP_CNTR_SET ?
			C_CMD_CT_TRIG_SET : C_CMD_CT_TRIG_INC;
	}

	op_cntr = container_of(cntr->cntr, struct cxip_cntr, cntr_fid);

	cmd.trig_ct = trig_cntr->ct->ctn;
	cmd.threshold = trig_thresh;
	cmd.ct = op_cntr->ct->ctn;
	cmd.set_ct_success = 1;
	cmd.ct_success = cntr->value;

	ofi_genlock_lock(&dom->trig_cmdq_lock);
	ret = cxi_cq_emit_ct(dom->trig_cmdq->dev_cmdq, opcode, &cmd);
	if (ret) {
		/* TODO: Handle this assert. */
		assert(!ret);
	}
	cxi_cq_ring(dom->trig_cmdq->dev_cmdq);
	ofi_genlock_unlock(&dom->trig_cmdq_lock);

	return FI_SUCCESS;
}

static int cxip_dom_dwq_op_recv(struct cxip_domain *dom, struct fi_op_msg *msg,
				struct cxip_cntr *trig_cntr,
				struct cxip_cntr *comp_cntr,
				uint64_t trig_thresh)
{
	struct cxip_ep *ep = container_of(msg->ep, struct cxip_ep, ep);
	struct cxip_rxc *rxc = ep->ep_obj->rxc;
	void *buf;
	size_t len;

	/* Non-zero thresholds for triggered receives are not supported. */
	if (!msg || msg->msg.iov_count > 1 || trig_thresh)
		return -FI_EINVAL;

	buf = msg->msg.iov_count ? msg->msg.msg_iov[0].iov_base : NULL;
	len = msg->msg.iov_count ? msg->msg.msg_iov[0].iov_len : 0;

	return rxc->ops.recv_common(rxc, buf, len, NULL, msg->msg.addr, 0, 0,
				    msg->msg.context, msg->flags, false,
				    comp_cntr);
}

static int cxip_dom_dwq_op_trecv(struct cxip_domain *dom,
				 struct fi_op_tagged *tagged,
				 struct cxip_cntr *trig_cntr,
				 struct cxip_cntr *comp_cntr,
				 uint64_t trig_thresh)
{
	struct cxip_ep *ep = container_of(tagged->ep, struct cxip_ep, ep);
	struct cxip_rxc *rxc = ep->ep_obj->rxc;
	void *buf;
	size_t len;

	/* Non-zero thresholds for triggered receives are not supported. */
	if (!tagged || tagged->msg.iov_count > 1 || trig_thresh)
		return -FI_EINVAL;

	buf = tagged->msg.iov_count ? tagged->msg.msg_iov[0].iov_base : NULL;
	len = tagged->msg.iov_count ? tagged->msg.msg_iov[0].iov_len : 0;

	return rxc->ops.recv_common(rxc, buf, len, tagged->msg.desc,
				    tagged->msg.addr, tagged->msg.tag,
				    tagged->msg.ignore, tagged->msg.context,
				    tagged->flags, true, comp_cntr);
}

/* Must hold domain lock. */
static void cxip_dom_progress_all_cqs(struct cxip_domain *dom)
{
	struct cxip_cq *cq;

	dlist_foreach_container(&dom->cq_list, struct cxip_cq, cq,
				dom_entry)
		cxip_util_cq_progress(&cq->util_cq);
}

static int cxip_dom_trig_op_get_in_use(struct cxip_domain *dom)
{
	struct cxi_rsrc_use in_use;
	int ret;

	ret = cxil_get_svc_rsrc_use(dom->iface->dev, dom->auth_key.svc_id,
				    &in_use);
	if (ret)
		return ret;

	return in_use.in_use[CXI_RSRC_TYPE_TLE];
}

#define DWQ_SEMAPHORE_TIMEOUT 10U

static int cxip_dom_dwq_queue_work_validate(struct cxip_domain *dom,
					    struct fi_deferred_work *work)
{
	struct cxip_ep *ep;

	if (!work->triggering_cntr)
		return -FI_EINVAL;

	switch (work->op_type) {
	case FI_OP_SEND:
	case FI_OP_RECV:
		ep = container_of(work->op.msg->ep, struct cxip_ep, ep);
		break;

	case FI_OP_TSEND:
	case FI_OP_TRECV:
		ep = container_of(work->op.tagged->ep, struct cxip_ep, ep);
		break;

	case FI_OP_READ:
	case FI_OP_WRITE:
		ep = container_of(work->op.rma->ep, struct cxip_ep, ep);
		break;

	case FI_OP_ATOMIC:
		ep = container_of(work->op.atomic->ep, struct cxip_ep, ep);
		break;

	case FI_OP_FETCH_ATOMIC:
		ep = container_of(work->op.fetch_atomic->ep, struct cxip_ep,
				  ep);
		break;

	case FI_OP_COMPARE_ATOMIC:
		ep = container_of(work->op.compare_atomic->ep, struct cxip_ep,
				  ep);
		break;

	case FI_OP_CNTR_SET:
	case FI_OP_CNTR_ADD:
		return FI_SUCCESS;

	default:
		return -FI_EINVAL;
	}

	/* All EPs that share a Domain must use the same VNI. This is a
	 * simplification due to Cassini requiring triggered op TXQs to
	 * use CP 0.
	 */
	if (ep->ep_obj->auth_key.vni != dom->auth_key.vni) {
		CXIP_WARN("Invalid VNI: %u\n", ep->ep_obj->auth_key.vni);
		return -FI_EINVAL;
	}

	return FI_SUCCESS;
}

static int cxip_dom_dwq_queue_work(struct cxip_domain *dom,
				   struct fi_deferred_work *work)
{
	struct cxip_cntr *trig_cntr;
	struct cxip_cntr *comp_cntr;
	bool queue_wb_work;
	int ret;
	int trig_op_count;
	int trig_op_in_use;
	struct timespec ts;
	bool again;

	ret = cxip_dom_dwq_queue_work_validate(dom, work);
	if (ret != FI_SUCCESS)
		return ret;

	comp_cntr = work->completion_cntr ?
		container_of(work->completion_cntr, struct cxip_cntr,
			     cntr_fid) : NULL;
	trig_cntr = container_of(work->triggering_cntr, struct cxip_cntr,
				 cntr_fid);

	switch (work->op_type) {
	case FI_OP_SEND:
	case FI_OP_RECV:
		queue_wb_work = !!(work->op.msg->flags & FI_CXI_CNTR_WB);
		break;

	case FI_OP_TSEND:
	case FI_OP_TRECV:
		queue_wb_work = !!(work->op.tagged->flags & FI_CXI_CNTR_WB);
		break;

	case FI_OP_READ:
	case FI_OP_WRITE:
		queue_wb_work = !!(work->op.rma->flags & FI_CXI_CNTR_WB);
		break;

	case FI_OP_ATOMIC:
		queue_wb_work = !!(work->op.atomic->flags & FI_CXI_CNTR_WB);
		break;

	case FI_OP_FETCH_ATOMIC:
		queue_wb_work = !!(work->op.fetch_atomic->flags & FI_CXI_CNTR_WB);
		break;

	case FI_OP_COMPARE_ATOMIC:
		queue_wb_work = !!(work->op.compare_atomic->flags & FI_CXI_CNTR_WB);
		break;

	default:
		queue_wb_work = false;
	}

	if (cxip_env.enable_trig_op_limit) {
		if (queue_wb_work)
			trig_op_count = 2;
		else
			trig_op_count = 1;

		if (clock_gettime(CLOCK_REALTIME, &ts) == -1) {
			CXIP_WARN("clock_gettime failed: %d\n", -errno);
			return -errno;
		}

		ts.tv_sec += DWQ_SEMAPHORE_TIMEOUT;

		again = true;
		do {
			if (sem_timedwait(dom->trig_op_lock, &ts) == -1) {
				if (errno == EINTR) {
					CXIP_WARN("sem_timedwait failed: %d\n",
						  -errno);
					return -errno;
				}
			} else {
				again = false;
			}
		} while (again);

		ret = cxip_dom_trig_op_get_in_use(dom);
		if (ret < 0) {
			CXIP_WARN("cxip_dom_trig_op_get_in_use: %d\n", ret);
			goto unlock;
		}

		trig_op_in_use = ret;

		if ((trig_op_in_use + trig_op_count) > dom->max_trig_op_in_use) {
			CXIP_WARN("Trig ops exhausted: in-use=%d\n", trig_op_in_use);
			ret = -FI_ENOSPC;
			goto unlock;
		}
	}

	switch (work->op_type) {
	case FI_OP_SEND:
		ret = cxip_dom_dwq_op_send(dom, work->op.msg, trig_cntr,
					   comp_cntr, work->threshold);
		break;

	case FI_OP_TSEND:
		ret = cxip_dom_dwq_op_tsend(dom, work->op.tagged, trig_cntr,
					    comp_cntr, work->threshold);
		break;

	case FI_OP_RECV:
		ret = cxip_dom_dwq_op_recv(dom, work->op.msg, trig_cntr,
					   comp_cntr, work->threshold);
		break;

	case FI_OP_TRECV:
		ret = cxip_dom_dwq_op_trecv(dom, work->op.tagged, trig_cntr,
					    comp_cntr, work->threshold);
		break;

	case FI_OP_READ:
	case FI_OP_WRITE:
		ret = cxip_dom_dwq_op_rma(dom, work->op.rma, work->op_type,
					  trig_cntr, comp_cntr,
					  work->threshold);
		break;

	case FI_OP_ATOMIC:
		ret = cxip_dom_dwq_op_atomic(dom, work->op.atomic, trig_cntr,
					     comp_cntr, work->threshold);
		break;

	case FI_OP_FETCH_ATOMIC:
		ret = cxip_dom_dwq_op_fetch_atomic(dom, work->op.fetch_atomic,
						   trig_cntr, comp_cntr,
						   work->threshold);
		break;

	case FI_OP_COMPARE_ATOMIC:
		ret = cxip_dom_dwq_op_comp_atomic(dom, work->op.compare_atomic,
						  trig_cntr, comp_cntr,
						  work->threshold);
		break;

	case FI_OP_CNTR_SET:
	case FI_OP_CNTR_ADD:
		ret = cxip_dom_dwq_op_cntr(dom, work->op.cntr, work->op_type,
					   trig_cntr, comp_cntr,
					   work->threshold, false);
		break;

	default:
		ret = -FI_EINVAL;
		CXIP_WARN("Invalid FI_QUEUE_WORK op %s\n",
				fi_tostr(&work->op_type, FI_TYPE_OP_TYPE));
	}

	if (ret)
		goto unlock;

	if (queue_wb_work) {
		struct fi_op_cntr op_cntr = {
			.cntr = &trig_cntr->cntr_fid,
		};

		/* no op_type needed for counter writeback */
		ret = cxip_dom_dwq_op_cntr(dom, &op_cntr, 0, trig_cntr, NULL,
					   work->threshold + 1, true);
		/* TODO: If cxip_dom_dwq_op_cntr fails we need to cancel the
		 * above work queue.
		 */
	}

	/* Wait until the command queue is empty. This is a sign that hardware
	 * has processed triggered operation commands. At this point, it is
	 * safe to release the trigger op pool lock.
	 */
	if (cxip_env.enable_trig_op_limit) {
		ofi_genlock_lock(&dom->trig_cmdq_lock);
		while (dom->trig_cmdq->dev_cmdq->status->rd_ptr !=
		       (dom->trig_cmdq->dev_cmdq->hw_wp32 / 2)) {};
		ofi_genlock_unlock(&dom->trig_cmdq_lock);
	}

unlock:
	if (cxip_env.enable_trig_op_limit)
		sem_post(dom->trig_op_lock);

	return ret;
}

static int cxip_dom_dwq_flush_work(struct cxip_domain *dom)
{
	struct cxip_cntr *trig_cntr;
	struct cxip_txc *txc;
	struct cxip_cq *cq;
	int ret __attribute__ ((unused));

	ofi_spin_lock(&dom->lock);
	if (!dom->cntr_init) {
		ofi_spin_unlock(&dom->lock);
		return FI_SUCCESS;
	}

	ofi_genlock_lock(&dom->trig_cmdq_lock);

	/* Issue cancels to all allocated counters. */
	dlist_foreach_container(&dom->cntr_list, struct cxip_cntr,
				trig_cntr, dom_entry) {
		struct c_ct_cmd ct_cmd = {};

		if (!trig_cntr->ct)
			continue;

		ct_cmd.ct = trig_cntr->ct->ctn;
		ret = cxi_cq_emit_ct(dom->trig_cmdq->dev_cmdq, C_CMD_CT_CANCEL,
				     &ct_cmd);

		// TODO: Handle this assert. Multiple triggered CQs may
		// be required.
		assert(!ret);
		cxi_cq_ring(dom->trig_cmdq->dev_cmdq);
	};

	/* Rely on the triggered CQ ack counter to know when there are no more
	 * pending triggered operations. In-between, progress CQs to cleanup
	 * internal transaction state.
	 */
	while (true) {
		unsigned int ack_counter;

		ret = cxil_cmdq_ack_counter(dom->trig_cmdq->dev_cmdq,
					    &ack_counter);
		assert(!ret);

		if (!ack_counter)
			break;

		cxip_dom_progress_all_cqs(dom);
	}

	/* It is possible that the ack counter is zero and there are completion
	 * events in-flight meaning that the above progression may have missed
	 * events. Perform a sleep to help ensure events have arrived and
	 * progress all CQs one more time.
	 *
	 * TODO: Investigate better way to resolve this race condition.
	 */
	sleep(1);
	cxip_dom_progress_all_cqs(dom);

	/* At this point, all triggered operations should be cancelled or have
	 * completed. Due to special handling of message operations, flush any
	 * remaining message triggered requests from the TX context first.
	 */
	dlist_foreach_container(&dom->txc_list, struct cxip_txc, txc,
				dom_entry)
		cxip_txc_flush_msg_trig_reqs(txc);

	/* Flush all the CQs of any remaining non-message triggered operation
	 * requests.
	 */
	dlist_foreach_container(&dom->cq_list, struct cxip_cq, cq, dom_entry)
		cxip_cq_flush_trig_reqs(cq);

	ofi_genlock_unlock(&dom->trig_cmdq_lock);
	ofi_spin_unlock(&dom->lock);

	return FI_SUCCESS;
}

static int cxip_domain_enable_mr_match_events(struct fid *fid, bool enable)
{
	struct cxip_domain *dom;

	if (fid->fclass != FI_CLASS_DOMAIN) {
		CXIP_WARN("Invalid FID: %p\n", fid);
		return -FI_EINVAL;
	}

	dom = container_of(fid, struct cxip_domain,
			   util_domain.domain_fid.fid);
	dom->mr_match_events = enable;

	return FI_SUCCESS;
}

static int cxip_domain_enable_optimized_mrs(struct fid *fid, bool enable)
{
	struct cxip_domain *dom;

	if (fid->fclass != FI_CLASS_DOMAIN) {
		CXIP_WARN("Invalid FID: %p\n", fid);
		return -FI_EINVAL;
	}

	dom = container_of(fid, struct cxip_domain,
			   util_domain.domain_fid.fid);
	if (!dom->is_prov_key) {
		CXIP_WARN("Requires FI_MR_PROV_KEY\n");
		return -FI_EINVAL;
	}

	dom->optimized_mrs = enable;

	return FI_SUCCESS;
}

static int cxip_domain_enable_prov_key_cache(struct fid *fid, bool enable)
{
	struct cxip_domain *dom;

	if (fid->fclass != FI_CLASS_DOMAIN) {
		CXIP_WARN("Invalid FID: %p\n", fid);
		return -FI_EINVAL;
	}

	dom = container_of(fid, struct cxip_domain,
			   util_domain.domain_fid.fid);
	if (!dom->is_prov_key) {
		CXIP_WARN("Requires FI_MR_PROV_KEY\n");
		return -FI_EINVAL;
	}

	dom->prov_key_cache = enable;

	return FI_SUCCESS;
}


static int cxip_dom_control(struct fid *fid, int command, void *arg)
{
	struct cxip_domain *dom;

	dom = container_of(fid, struct cxip_domain, util_domain.domain_fid.fid);

	if (command != FI_FLUSH_WORK && !arg) {
		CXIP_WARN("Required argument missing\n");
		return -FI_EINVAL;
	}

	switch (command) {
	case FI_QUEUE_WORK:
		return cxip_dom_dwq_queue_work(dom, arg);

	case FI_FLUSH_WORK:
		return cxip_dom_dwq_flush_work(dom);

	case FI_OPT_CXI_SET_OPTIMIZED_MRS:
		return cxip_domain_enable_optimized_mrs(fid, *(bool *)arg);

	case FI_OPT_CXI_GET_OPTIMIZED_MRS:
		*(bool *)arg = dom->optimized_mrs;
		break;

	case FI_OPT_CXI_SET_MR_MATCH_EVENTS:
		return cxip_domain_enable_mr_match_events(fid, *(bool *)arg);

	case FI_OPT_CXI_GET_MR_MATCH_EVENTS:
		*(bool *)arg = dom->mr_match_events;
		break;

	case FI_OPT_CXI_SET_PROV_KEY_CACHE:
		return cxip_domain_enable_prov_key_cache(fid, *(bool *)arg);

	case FI_OPT_CXI_GET_PROV_KEY_CACHE:
		*(bool *)arg = dom->prov_key_cache;
		break;

	default:
		return -FI_EINVAL;
	}

	return FI_SUCCESS;
}

static int cxip_domain_cntr_read(struct fid *fid, unsigned int cntr,
				 uint64_t *value, struct timespec *ts)
{
	struct cxip_domain *dom;
	int ret;

	if (fid->fclass != FI_CLASS_DOMAIN) {
		CXIP_WARN("Invalid FID: %p\n", fid);
		return -FI_EINVAL;
	}

	dom = container_of(fid, struct cxip_domain,
			   util_domain.domain_fid.fid);

	if (!dom->enabled)
		return -FI_EOPBADSTATE;

	ret = cxil_read_cntr(dom->iface->dev, cntr, value, ts);

	return ret ? -FI_EINVAL : FI_SUCCESS;
}

static int cxip_domain_topology(struct fid *fid, unsigned int *group_id,
				unsigned int *switch_id, unsigned int *port_id)
{
	struct cxip_domain *dom;
	struct cxip_topo_addr topo;

	if (fid->fclass != FI_CLASS_DOMAIN) {
		CXIP_WARN("Invalid FID: %p\n", fid);
		return -FI_EINVAL;
	}

	dom = container_of(fid, struct cxip_domain,
			   util_domain.domain_fid.fid);
	topo.addr = dom->nic_addr;

	/* Only a dragonfly topology is supported at this time */
	if (group_id)
		*group_id = topo.dragonfly.group_num;
	if (switch_id)
		*switch_id = topo.dragonfly.switch_num;
	if (port_id)
		*port_id = topo.dragonfly.port_num;

	return FI_SUCCESS;
}

static int cxip_domain_enable_hybrid_mr_desc(struct fid *fid, bool enable)
{
	struct cxip_domain *dom;

	if (fid->fclass != FI_CLASS_DOMAIN) {
		CXIP_WARN("Invalid FID: %p\n", fid);
		return -FI_EINVAL;
	}

	dom = container_of(fid, struct cxip_domain,
			   util_domain.domain_fid.fid);

	dom->hybrid_mr_desc = enable;

	return FI_SUCCESS;
}

static int cxip_domain_get_dwq_depth(struct fid *fid, size_t *depth)
{
	struct cxip_domain *dom;

	if (fid->fclass != FI_CLASS_DOMAIN) {
		CXIP_WARN("Invalid FID: %p\n", fid);
		return -FI_EINVAL;
	}

	dom = container_of(fid, struct cxip_domain,
			   util_domain.domain_fid.fid);

	*depth = dom->max_trig_op_in_use;

	return FI_SUCCESS;
}

static struct fi_cxi_dom_ops cxip_dom_ops_ext = {
	.cntr_read = cxip_domain_cntr_read,
	.topology = cxip_domain_topology,
	.enable_hybrid_mr_desc = cxip_domain_enable_hybrid_mr_desc,
	.ep_get_unexp_msgs = cxip_ep_get_unexp_msgs,
	.get_dwq_depth = cxip_domain_get_dwq_depth,
	.enable_mr_match_events = cxip_domain_enable_mr_match_events,
	.enable_optimized_mrs = cxip_domain_enable_optimized_mrs,
};

static int cxip_dom_ops_open(struct fid *fid, const char *ops_name,
			     uint64_t flags, void **ops, void *context)
{
	/* v5 only appended a new function */
	if (!strcmp(ops_name, FI_CXI_DOM_OPS_1) ||
	    !strcmp(ops_name, FI_CXI_DOM_OPS_2) ||
	    !strcmp(ops_name, FI_CXI_DOM_OPS_3) ||
	    !strcmp(ops_name, FI_CXI_DOM_OPS_4) ||
	    !strcmp(ops_name, FI_CXI_DOM_OPS_5) ||
	    !strcmp(ops_name, FI_CXI_DOM_OPS_6)) {
		*ops = &cxip_dom_ops_ext;
		return FI_SUCCESS;
	}

	return -FI_EINVAL;
}

static int cxip_domain_ops_set(struct fid *fid, const char *name,
			       uint64_t flags, void *ops, void *context)
{
	struct cxip_domain *domain =
		container_of(fid, struct cxip_domain,
			     util_domain.domain_fid.fid);
	struct fi_hmem_override_ops *hmem_ops;

	if (strcmp(FI_SET_OPS_HMEM_OVERRIDE, name) == 0) {
		hmem_ops = ops;

		if (!hmem_ops->copy_from_hmem_iov ||
		    !hmem_ops->copy_to_hmem_iov)
			return -FI_EINVAL;

		domain->hmem_ops = *hmem_ops;

		return FI_SUCCESS;
	}

	return -FI_ENOSYS;
}

static int cxip_query_atomic_flags_valid(uint64_t flags)
{
	/* FI_COMPARE_ATOMIC and FI_FETCH_ATOMIC are mutually exclusive. */
	if ((flags & FI_COMPARE_ATOMIC) && (flags & FI_FETCH_ATOMIC))
		return -FI_EINVAL;

	if (flags & FI_CXI_PCIE_AMO) {
		/* Only FI_FETCH_ATOMIC is support with FI_CXI_PCIE_AMO. */
		if (!(flags & FI_FETCH_ATOMIC))
			return -FI_EOPNOTSUPP;
	}

	return FI_SUCCESS;
}

static int cxip_query_atomic(struct fid_domain *domain,
			     enum fi_datatype datatype, enum fi_op op,
			     struct fi_atomic_attr *attr, uint64_t flags)
{
	enum cxip_amo_req_type req_type;
	int ret;
	unsigned int datatype_len;
	struct cxip_domain *dom;

	dom = container_of(domain, struct cxip_domain,
			   util_domain.domain_fid.fid);

	if (!attr)
		return -FI_EINVAL;

	ret = cxip_query_atomic_flags_valid(flags);
	if (ret)
		return ret;

	if (flags & FI_COMPARE_ATOMIC) {
		req_type = CXIP_RQ_AMO_SWAP;
	} else if (flags & FI_FETCH_ATOMIC) {
		if (flags & FI_CXI_PCIE_AMO)
			req_type = CXIP_RQ_AMO_PCIE_FETCH;
		else
			req_type = CXIP_RQ_AMO_FETCH;
	} else {
		req_type = CXIP_RQ_AMO;
	}

	ret = _cxip_atomic_opcode(req_type, datatype, op,
				  dom->amo_remap_to_pcie_fadd, NULL, NULL, NULL,
				  &datatype_len);
	if (ret)
		return ret;

	attr->count = 1;
	attr->size = datatype_len;

	return FI_SUCCESS;
}

static int cxip_query_collective(struct fid_domain *domain,
				 enum fi_collective_op coll,
			         struct fi_collective_attr *attr,
				 uint64_t flags)
{
	int ext_op;

	/* BARRIER does not require attr */
	if (coll == FI_BARRIER && !attr)
		return FI_SUCCESS;

	/* Anything else requires attr */
	if (!attr)
		return -FI_EINVAL;

	/* Flags are not supported */
	if (flags)
		return -FI_EOPNOTSUPP;

	/* The limit to collective membership is the size of the multicast tree,
	 * which is limited by the maximum address space of addressable ports on
	 * the fabric.
	 */
	attr->max_members = (1L << C_DFA_NIC_BITS) - 1;

	/* supported collective operations */
	ext_op = (int)attr->op;
	switch (coll) {
	case FI_BARRIER:
		/* ignore attr->op: barrier takes no operator */
		/* ignore attr->datatype: barrier takes no data */
		attr->datatype_attr.count = 0;
		attr->datatype_attr.size = 0;
		break;
	case FI_BROADCAST:
		/* ignore attr->op: barrier takes no operator */
		switch (attr->datatype) {
		case FI_INT8:
		case FI_UINT8:
			attr->datatype_attr.count = 32;
			attr->datatype_attr.size = 1;
			break;
		case FI_INT16:
		case FI_UINT16:
			attr->datatype_attr.count = 16;
			attr->datatype_attr.size = 2;
			break;
		case FI_INT32:
		case FI_UINT32:
		case FI_FLOAT:
			attr->datatype_attr.count = 8;
			attr->datatype_attr.size = 4;
			break;
		case FI_INT64:
		case FI_UINT64:
		case FI_DOUBLE:
			attr->datatype_attr.count = 4;
			attr->datatype_attr.size = 8;
			break;
		default:
			return -FI_EOPNOTSUPP;
		}
		break;
	case FI_REDUCE:
	case FI_ALLREDUCE:
		switch (ext_op) {
		case FI_BOR:
		case FI_BAND:
		case FI_BXOR:
			switch (attr->datatype) {
			case FI_INT8:
			case FI_UINT8:
				attr->datatype_attr.count = 32;
				attr->datatype_attr.size = 1;
				break;
			case FI_INT16:
			case FI_UINT16:
				attr->datatype_attr.count = 16;
				attr->datatype_attr.size = 2;
				break;
			case FI_INT32:
			case FI_UINT32:
				attr->datatype_attr.count = 8;
				attr->datatype_attr.size = 4;
				break;
			case FI_INT64:
			case FI_UINT64:
				attr->datatype_attr.count = 4;
				attr->datatype_attr.size = 8;
				break;
			default:
				return -FI_EOPNOTSUPP;
			}
			break;
		case FI_MIN:
		case FI_MAX:
		case FI_SUM:
			if (attr->datatype != FI_INT64 &&
			    attr->datatype != FI_DOUBLE)
				return -FI_EOPNOTSUPP;
			attr->datatype_attr.count = 4;
			attr->datatype_attr.size = 8;
			break;
		case FI_CXI_MINMAXLOC:
			attr->datatype_attr.count = 1;
			attr->datatype_attr.size = 32;
			break;
		case FI_CXI_REPSUM:
			attr->datatype_attr.count = 1;
			attr->datatype_attr.size = 8;
			break;
		default:
			return -FI_EOPNOTSUPP;
		}
		break;
	default:
		return -FI_EOPNOTSUPP;
	}
	return FI_SUCCESS;
}

static struct fi_ops cxip_dom_fi_ops = {
	.size = sizeof(struct fi_ops),
	.close = cxip_dom_close,
	.bind = cxip_dom_bind,
	.control = cxip_dom_control,
	.ops_open = cxip_dom_ops_open,
	.ops_set = cxip_domain_ops_set,
};

static struct fi_ops_domain cxip_dom_ops = {
	.size = sizeof(struct fi_ops_domain),
	.av_open = cxip_av_open,
	.cq_open = cxip_cq_open,
	.endpoint = cxip_endpoint,
	.scalable_ep = fi_no_scalable_ep,
	.cntr_open = cxip_cntr_open,
	.poll_open = fi_no_poll_open,
	.stx_ctx = fi_no_stx_context,
	.srx_ctx = fi_no_srx_context,
	.query_atomic = cxip_query_atomic,
	.query_collective = cxip_query_collective
};

/*
 * cxip_domain() - Provider fi_domain() implementation.
 */
int cxip_domain(struct fid_fabric *fabric, struct fi_info *info,
		struct fid_domain **dom, void *context)
{
	struct cxip_domain *cxi_domain;
	struct cxip_fabric *fab;
	struct cxip_addr *src_addr;
	uint32_t seed;
	int ret;

	/* The OFI check_info function does not verify that rx/tx attribute
	 * capabilities are a subset of the info capabilities. Currently
	 * MPI removes the FI_HMEM cap from info->caps but not the rx/tx
	 * caps. To avoided breaking MPI, the capabilities are removed
	 * here as a temporary work around.
	 * TODO: Remove this code when no longer required.
	 */
	if (info->caps && !(info->caps & FI_HMEM)) {
		if (info->tx_attr)
			info->tx_attr->caps &= ~FI_HMEM;
		if (info->rx_attr)
			info->rx_attr->caps &= ~FI_HMEM;
	}

	ret = ofi_prov_check_info(&cxip_util_prov, CXIP_FI_VERSION, info);
	if (ret != FI_SUCCESS)
		return -FI_EINVAL;

	ret = cxip_check_auth_key_info(info);
	if (ret)
		return ret;

	fab = container_of(fabric, struct cxip_fabric, util_fabric.fabric_fid);

	cxi_domain = calloc(1, sizeof(*cxi_domain));
	if (!cxi_domain)
		return -FI_ENOMEM;

	ret = ofi_domain_init(&fab->util_fabric.fabric_fid, info,
			      &cxi_domain->util_domain, context,
			      OFI_LOCK_SPINLOCK);
	if (ret)
		goto free_dom;

	if (!info || !info->src_addr) {
		CXIP_WARN("Invalid fi_info\n");
		goto close_util_dom;
	}
	src_addr = (struct cxip_addr *)info->src_addr;
	cxi_domain->nic_addr = src_addr->nic;

	if (info->domain_attr->auth_key) {
		/* Auth key size is verified in ofi_prov_check_info(). */
		assert(info->domain_attr->auth_key_size ==
		       sizeof(struct cxi_auth_key));

		memcpy(&cxi_domain->auth_key, info->domain_attr->auth_key,
		       sizeof(struct cxi_auth_key));
	} else {
		ret = cxip_gen_auth_key(info, &cxi_domain->auth_key);
		if (ret) {
			CXIP_WARN("cxip_gen_auth_key failed: %d:%s", ret,
				  fi_strerror(-ret));
			return ret;
		}

		/* If FI_AV_AUTH_KEY is used, the auth_key.vni value will never
		 * be used. Thus, set it to zero which is invalid.
		 */
		cxi_domain->av_auth_key =
			info->domain_attr->auth_key_size == FI_AV_AUTH_KEY;
		if (cxi_domain->av_auth_key)
			cxi_domain->auth_key.vni = 0;
	}

	if (info->domain_attr->tclass != FI_TC_UNSPEC) {
		if (info->domain_attr->tclass >= FI_TC_LABEL &&
		    info->domain_attr->tclass <= FI_TC_SCAVENGER) {
			cxi_domain->tclass = info->domain_attr->tclass;
		} else {
			CXIP_WARN("Invalid tclass\n");
			goto close_util_dom;
		}
	} else {
		/* Use default tclass */
		cxi_domain->tclass = FI_TC_BEST_EFFORT;
	}

	cxi_domain->av_user_id =
		!!(cxi_domain->util_domain.info_domain_caps & FI_AV_USER_ID);
	cxi_domain->auth_key_entry_max = info->domain_attr->max_ep_auth_key;
	cxi_domain->util_domain.domain_fid.fid.ops = &cxip_dom_fi_ops;
	cxi_domain->util_domain.domain_fid.ops = &cxip_dom_ops;
	cxi_domain->util_domain.domain_fid.mr = &cxip_dom_mr_ops;

	dlist_init(&cxi_domain->cmdq_list);
	cxi_domain->cmdq_cnt = 0;

	/* Align domain TX command size based on EP TX size attribute. In
	 * addition, support ENV vars to override size.
	 */
	cxi_domain->tx_size = 0;
	if (info->tx_attr)
		cxi_domain->tx_size = info->tx_attr->size;

	if (!info->tx_attr) {
		cxi_domain->tx_size = cxip_env.default_tx_size;
		cxi_domain->tx_size =
			MAX(cxip_env.default_cq_size, cxi_domain->tx_size);
	}

	if (cxi_domain->util_domain.threading == FI_THREAD_DOMAIN)
		ofi_genlock_init(&cxi_domain->cmdq_lock, OFI_LOCK_NONE);
	else
		ofi_genlock_init(&cxi_domain->cmdq_lock, OFI_LOCK_MUTEX);

	dlist_init(&cxi_domain->txc_list);
	dlist_init(&cxi_domain->cntr_list);
	dlist_init(&cxi_domain->cq_list);
	ofi_spin_init(&cxi_domain->lock);
	ofi_spin_init(&cxi_domain->ctrl_id_lock);
	memset(&cxi_domain->req_ids, 0, sizeof(cxi_domain->req_ids));
	memset(&cxi_domain->mr_ids, 0, sizeof(cxi_domain->mr_ids));

	ofi_atomic_initialize32(&cxi_domain->ref, 0);
	cxi_domain->fab = fab;

	cxi_domain->hmem_ops.copy_from_hmem_iov = ofi_copy_from_hmem_iov;
	cxi_domain->hmem_ops.copy_to_hmem_iov = ofi_copy_to_hmem_iov;

	/* Allocate/initialize domain hardware resources */
	ret = cxip_domain_enable(cxi_domain);
	if (ret) {
		CXIP_WARN("Resource allocation failed: %d: %s\n",
			  ret, fi_strerror(-ret));
		goto cleanup_dom;
	}

	/* Handle client vs provider MR RKEY differences */
	if (cxi_domain->util_domain.mr_mode & FI_MR_PROV_KEY) {
		cxi_domain->is_prov_key = true;

		seed = (uint32_t)ofi_gettime_ns();
		cxi_domain->prov_key_seqnum = ofi_xorshift_random(seed);
	}

	cxi_domain->mr_match_events = cxip_env.mr_match_events;
	cxi_domain->optimized_mrs = cxip_env.optimized_mrs;
	cxi_domain->prov_key_cache = cxip_env.prov_key_cache;
	*dom = &cxi_domain->util_domain.domain_fid;

	return 0;

cleanup_dom:
	ofi_spin_destroy(&cxi_domain->lock);
close_util_dom:
	ofi_domain_close(&cxi_domain->util_domain);
free_dom:
	free(cxi_domain);
	return -FI_EINVAL;
}

int cxip_domain_valid_vni(struct cxip_domain *dom, unsigned int vni)
{
	/* Currently the auth_key.svc_id field contains the resource group ID.
	*/
	return cxip_if_valid_rgroup_vni(dom->iface, dom->auth_key.svc_id, vni);
}

#define SUPPORTED_DWQ_FLAGS (FI_MORE | FI_COMPLETION | FI_DELIVERY_COMPLETE | \
	FI_MATCH_COMPLETE | FI_TRANSMIT_COMPLETE | FI_CXI_CNTR_WB)

static int cxip_domain_dwq_emit_validate(struct cxip_domain *dom, uint16_t vni,
					 enum cxi_traffic_class tc,
					 enum cxi_traffic_class_type tc_type,
					 uint64_t flags)
{
	uint64_t unsupported_flags = flags & ~SUPPORTED_DWQ_FLAGS;

	if (unsupported_flags) {
		CXIP_WARN("Unsupported flags: %lx\n", unsupported_flags);
		return -FI_EINVAL;
	}

	if (tc != dom->trig_cmdq->cur_cp->tc) {
		CXIP_WARN("Invalid tc: %d\n", tc);
		return -FI_EINVAL;
	}

	if (tc_type != dom->trig_cmdq->cur_cp->tc_type) {
		CXIP_WARN("Invalid tc_type: %d\n", tc_type);
		return -FI_EINVAL;
	}

	if (vni != dom->trig_cmdq->cur_cp->vni) {
		CXIP_WARN("Invalid vni: %d\n", vni);
		return -FI_EINVAL;
	}

	return FI_SUCCESS;
}

int cxip_domain_dwq_emit_dma(struct cxip_domain *dom, uint16_t vni,
			     enum cxi_traffic_class tc,
			     enum cxi_traffic_class_type tc_type,
			     struct cxip_cntr *trig_cntr, size_t trig_thresh,
			     struct c_full_dma_cmd *dma, uint64_t flags)
{
	struct c_ct_cmd ct_cmd = {
		.trig_ct = trig_cntr->ct->ctn,
		.threshold = trig_thresh,
	};
	int ret;

	ret = cxip_domain_dwq_emit_validate(dom, vni, tc, tc_type, flags);
	if (ret)
		return ret;

	ofi_genlock_lock(&dom->trig_cmdq_lock);

	ret = cxi_cq_emit_trig_full_dma(dom->trig_cmdq->dev_cmdq, &ct_cmd, dma);
	if (ret) {
		CXIP_WARN("Failed to emit trigger dma command: %d:%s\n", ret,
			  fi_strerror(-ret));
		ret = -FI_EAGAIN;
	} else {
		cxip_txq_ring(dom->trig_cmdq, false, 1);
	}

	ofi_genlock_unlock(&dom->trig_cmdq_lock);

	return ret;
}

int cxip_domain_dwq_emit_amo(struct cxip_domain *dom, uint16_t vni,
			     enum cxi_traffic_class tc,
			     enum cxi_traffic_class_type tc_type,
			     struct cxip_cntr *trig_cntr, size_t trig_thresh,
			     struct c_dma_amo_cmd *amo, uint64_t flags,
			     bool fetching, bool flush)
{
	struct c_ct_cmd ct_cmd = {
		.trig_ct = trig_cntr->ct->ctn,
		.threshold = trig_thresh,
	};
	struct c_full_dma_cmd flush_cmd;
	bool fetching_flush = fetching && flush;
	int ret;

	/* TODO: Need to ensure there are at least 2 TLEs free for the following
	 * triggered commands.
	 */

	/* TODO: Support triggered operations with different VNIs. */

	if (fetching_flush) {
		memset(&flush_cmd, 0, sizeof(flush_cmd));
		flush_cmd.command.opcode = C_CMD_PUT;
		flush_cmd.index_ext = amo->index_ext;
		flush_cmd.event_send_disable = 1;
		flush_cmd.dfa = amo->dfa;
		flush_cmd.remote_offset = amo->remote_offset;
		flush_cmd.eq = amo->eq;
		flush_cmd.user_ptr = amo->user_ptr;
		flush_cmd.flush = 1;
	}

	ret = cxip_domain_dwq_emit_validate(dom, vni, tc, tc_type, flags);
	if (ret)
		return ret;

	ofi_genlock_lock(&dom->trig_cmdq_lock);

	if (fetching_flush &&
	    __cxi_cq_free_slots(dom->trig_cmdq->dev_cmdq) >= 16) {
		CXIP_WARN("No space for FAMO with FI_DELIVERY_COMPLETE\n");
		ret = -FI_EAGAIN;
		goto out_unlock;
	}

	ret = cxi_cq_emit_trig_dma_amo(dom->trig_cmdq->dev_cmdq, &ct_cmd,
				       amo, fetching);
	if (ret) {
		CXIP_WARN("Failed to emit trigger amo command: %d:%s\n", ret,
			  fi_strerror(-ret));
		ret = -FI_EAGAIN;
		goto out_unlock;
	}

	if (fetching_flush) {
		/* CQ space check already occurred. Thus, return code can be
		 * ignored.
		 */
		ret = cxi_cq_emit_trig_full_dma(dom->trig_cmdq->dev_cmdq,
						&ct_cmd, &flush_cmd);
		assert(ret == 0);
	}

	cxip_txq_ring(dom->trig_cmdq, false, 1);

out_unlock:
	ofi_genlock_unlock(&dom->trig_cmdq_lock);

	return ret;
}
