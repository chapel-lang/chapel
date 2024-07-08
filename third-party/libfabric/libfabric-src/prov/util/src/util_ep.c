/*
 * Copyright (c) 2016 Intel Corporation, Inc.  All rights reserved.
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

#include <ofi_enosys.h>
#include <ofi_util.h>
#include <ofi_coll.h>

int ofi_ep_bind_cq(struct util_ep *ep, struct util_cq *cq, uint64_t flags)
{
	int ret;

	ret = ofi_check_bind_cq_flags(ep, cq, flags);
	if (ret)
		return ret;

	if (flags & FI_TRANSMIT) {
		ep->tx_cq = cq;
		if (!(flags & FI_SELECTIVE_COMPLETION)) {
			ep->tx_op_flags |= FI_COMPLETION;
			ep->tx_msg_flags = FI_COMPLETION;
		}
		ofi_atomic_inc32(&cq->ref);
	}

	if (flags & FI_RECV) {
		ep->rx_cq = cq;
		if (!(flags & FI_SELECTIVE_COMPLETION)) {
			ep->rx_op_flags |= FI_COMPLETION;
			ep->rx_msg_flags = FI_COMPLETION;
		}
		ofi_atomic_inc32(&cq->ref);
	}

	if (flags & (FI_TRANSMIT | FI_RECV)) {
		return fid_list_insert2(&cq->ep_list,
				       &cq->ep_list_lock,
				       &ep->ep_fid.fid);
	}

	return FI_SUCCESS;
}

int ofi_ep_bind_eq(struct util_ep *ep, struct util_eq *eq)
{
	if (ep->eq)
		ofi_atomic_dec32(&ep->eq->ref);

	ep->eq = eq;
	ofi_atomic_inc32(&eq->ref);
	return 0;
}

int ofi_ep_bind_av(struct util_ep *util_ep, struct util_av *av)
{
	if (util_ep->av) {
		FI_WARN(util_ep->av->prov, FI_LOG_EP_CTRL,
				"duplicate AV binding\n");
		return -FI_EINVAL;
	}
	util_ep->av = av;
	ofi_atomic_inc32(&av->ref);

	ofi_genlock_lock(&av->ep_list_lock);
	dlist_insert_tail(&util_ep->av_entry, &av->ep_list);
	ofi_genlock_unlock(&av->ep_list_lock);

	return 0;
}

int ofi_ep_bind_cntr(struct util_ep *ep, struct util_cntr *cntr, uint64_t flags)
{
	if (flags & ~(FI_TRANSMIT | FI_RECV | FI_READ  | FI_WRITE |
		      FI_REMOTE_READ | FI_REMOTE_WRITE)) {
		FI_WARN(ep->domain->fabric->prov, FI_LOG_EP_CTRL,
			"Unsupported bind flags\n");
		return -FI_EBADFLAGS;
	}

	if (((flags & FI_TRANSMIT) && ep->cntrs[CNTR_TX]) ||
	    ((flags & FI_RECV) && ep->cntrs[CNTR_RX]) ||
	    ((flags & FI_READ) && ep->cntrs[CNTR_RD]) ||
	    ((flags & FI_WRITE) && ep->cntrs[CNTR_WR]) ||
	    ((flags & FI_REMOTE_READ) && ep->cntrs[CNTR_REM_RD]) ||
	    ((flags & FI_REMOTE_WRITE) && ep->cntrs[CNTR_REM_WR])) {
		FI_WARN(ep->domain->fabric->prov, FI_LOG_EP_CTRL,
			"Duplicate counter binding\n");
		return -FI_EINVAL;
	}

	if (flags & FI_TRANSMIT) {
		ep->cntrs[CNTR_TX] = cntr;
		ep->cntr_inc_funcs[CNTR_TX] = ofi_cntr_inc;
		ofi_atomic_inc32(&cntr->ref);
	}

	if (flags & FI_RECV) {
		ep->cntrs[CNTR_RX]= cntr;
		ep->cntr_inc_funcs[CNTR_RX] = ofi_cntr_inc;
		ofi_atomic_inc32(&cntr->ref);
	}

	if (flags & FI_READ) {
		ep->cntrs[CNTR_RD] = cntr;
		ep->cntr_inc_funcs[CNTR_RD] = ofi_cntr_inc;
		ofi_atomic_inc32(&cntr->ref);
	}

	if (flags & FI_WRITE) {
		ep->cntrs[CNTR_WR] = cntr;
		ep->cntr_inc_funcs[CNTR_WR] = ofi_cntr_inc;
		ofi_atomic_inc32(&cntr->ref);
	}

	if (flags & FI_REMOTE_READ) {
		ep->cntrs[CNTR_REM_RD] = cntr;
		ep->cntr_inc_funcs[CNTR_REM_RD] = ofi_cntr_inc;
		ofi_atomic_inc32(&cntr->ref);
	}

	if (flags & FI_REMOTE_WRITE) {
		ep->cntrs[CNTR_REM_WR] = cntr;
		ep->cntr_inc_funcs[CNTR_REM_WR] = ofi_cntr_inc;
		ofi_atomic_inc32(&cntr->ref);
	}

	ep->flags |= OFI_CNTR_ENABLED;

	return fid_list_insert2(&cntr->ep_list, &cntr->ep_list_lock,
			       &ep->ep_fid.fid);
}

int ofi_ep_bind(struct util_ep *util_ep, struct fid *fid, uint64_t flags)
{
	int ret;
	struct util_av   *av;
	struct util_cq   *cq;
	struct util_eq   *eq;
	struct util_cntr *cntr;

	ret = ofi_ep_bind_valid(util_ep->domain->prov, fid, flags);
	if (ret)
		return ret;

	switch (fid->fclass) {
	case FI_CLASS_CQ:
		cq = container_of(fid, struct util_cq, cq_fid.fid);
		return ofi_ep_bind_cq(util_ep, cq, flags);
	case FI_CLASS_EQ:
		eq = container_of(fid, struct util_eq, eq_fid.fid);
		return ofi_ep_bind_eq(util_ep, eq);
	case FI_CLASS_AV:
		av = container_of(fid, struct util_av, av_fid.fid);
		return ofi_ep_bind_av(util_ep, av);
	case FI_CLASS_CNTR:
		cntr = container_of(fid, struct util_cntr, cntr_fid.fid);
		return ofi_ep_bind_cntr(util_ep, cntr, flags);
	}

	return -FI_EINVAL;
}

static inline int util_coll_init_cid_mask(struct ofi_bitmask *mask)
{
	int err = ofi_bitmask_create(mask, OFI_MAX_GROUP_ID);
	if (err)
		return err;

	ofi_bitmask_set_all(mask);

	/* reserving the first bit in context id to whole av set */
	ofi_bitmask_unset(mask, OFI_WORLD_GROUP_ID);

	return FI_SUCCESS;
}

int ofi_endpoint_init(struct fid_domain *domain, const struct util_prov *util_prov,
		      struct fi_info *info, struct util_ep *ep, void *context,
		      ofi_ep_progress_func progress)
{
	struct util_domain *util_domain;
	int ret;
	int i;

	util_domain = container_of(domain, struct util_domain, domain_fid);

	if (!info || !info->ep_attr || !info->rx_attr || !info->tx_attr)
		return -FI_EINVAL;

	ret = ofi_prov_check_info(util_prov,
				  util_domain->fabric->fabric_fid.api_version,
				  info);
	if (ret)
		return ret;

	ep->ep_fid.fid.fclass = FI_CLASS_EP;
	ep->ep_fid.fid.context = context;
	ep->domain = util_domain;
	ep->caps = info->caps;
	ep->flags = 0;
	ep->progress = progress;
	ep->tx_op_flags = info->tx_attr->op_flags;
	ep->rx_op_flags = info->rx_attr->op_flags;
	ep->tx_msg_flags = 0;
	ep->rx_msg_flags = 0;
	ep->inject_op_flags =
		((info->tx_attr->op_flags &
		  ~(FI_COMPLETION | FI_INJECT_COMPLETE |
		    FI_TRANSMIT_COMPLETE | FI_DELIVERY_COMPLETE)) | FI_INJECT);

	for (i = 0; i < CNTR_CNT; i++)
		ep->cntr_inc_funcs[i] = ofi_cntr_inc_noop;

	ep->type = info->ep_attr->type;
	ofi_atomic_inc32(&util_domain->ref);
	if (util_domain->eq)
		ofi_ep_bind_eq(ep, util_domain->eq);

	ret = ofi_genlock_init(&ep->lock,
			       ep->domain->threading != FI_THREAD_SAFE ?
			       OFI_LOCK_NOOP : OFI_LOCK_MUTEX);
	if (ret)
		return ret;

	if (ep->caps & FI_COLLECTIVE) {
		ep->coll_cid_mask = calloc(1, sizeof(*ep->coll_cid_mask));
		if (!ep->coll_cid_mask) {
			ofi_genlock_destroy(&ep->lock);
			return -FI_ENOMEM;
		}
		util_coll_init_cid_mask(ep->coll_cid_mask);
	} else {
		ep->coll_cid_mask = NULL;
	}
	slist_init(&ep->coll_ready_queue);
	return 0;
}

int ofi_endpoint_close(struct util_ep *util_ep)
{
	int i;

	if (util_ep->tx_cq) {
		fid_list_remove2(&util_ep->tx_cq->ep_list,
				&util_ep->tx_cq->ep_list_lock,
				&util_ep->ep_fid.fid);
		ofi_atomic_dec32(&util_ep->tx_cq->ref);
	}

	if (util_ep->rx_cq) {
		fid_list_remove2(&util_ep->rx_cq->ep_list,
				&util_ep->rx_cq->ep_list_lock,
				&util_ep->ep_fid.fid);
		ofi_atomic_dec32(&util_ep->rx_cq->ref);
	}

	for (i = 0; i < CNTR_CNT; i++) {
		if (util_ep->cntrs[i]) {
			fid_list_remove2(&util_ep->cntrs[i]->ep_list,
					&util_ep->cntrs[i]->ep_list_lock,
					&util_ep->ep_fid.fid);
			ofi_atomic_dec32(&util_ep->cntrs[i]->ref);
		}
	}

	if (util_ep->av) {
		ofi_genlock_lock(&util_ep->av->ep_list_lock);
		dlist_remove(&util_ep->av_entry);
		ofi_genlock_unlock(&util_ep->av->ep_list_lock);

		ofi_atomic_dec32(&util_ep->av->ref);
	}

	if (util_ep->coll_cid_mask) {
		ofi_bitmask_free(util_ep->coll_cid_mask);
		free(util_ep->coll_cid_mask);
	}

	if (util_ep->eq) {
		ofi_eq_remove_fid_events(util_ep->eq,
					 &util_ep->ep_fid.fid);
		ofi_atomic_dec32(&util_ep->eq->ref);
	}
	ofi_atomic_dec32(&util_ep->domain->ref);
	ofi_genlock_destroy(&util_ep->lock);
	return 0;
}
