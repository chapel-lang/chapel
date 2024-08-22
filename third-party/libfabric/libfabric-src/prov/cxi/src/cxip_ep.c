/*
 * SPDX-License-Identifier: BSD-2-Clause OR GPL-2.0-only
 *
 * Copyright (c) 2013-2014 Intel Corporation. All rights reserved.
 * Copyright (c) 2016 Cisco Systems, Inc. All rights reserved.
 * Copyright (c) 2017 DataDirect Networks, Inc. All rights reserved.
 * Copyright (c) 2018,2020-2023 Hewlett Packard Enterprise Development LP
 */

#include "config.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#include "ofi_util.h"
#include "cxip.h"

#define CXIP_DBG(...) _CXIP_DBG(FI_LOG_EP_CTRL, __VA_ARGS__)
#define CXIP_WARN(...) _CXIP_WARN(FI_LOG_EP_CTRL, __VA_ARGS__)

extern struct fi_ops_rma cxip_ep_rma_ops;
extern struct fi_ops_rma cxip_ep_rma_no_ops;

extern struct fi_ops_msg cxip_ep_msg_ops;
extern struct fi_ops_msg cxip_ep_msg_no_ops;
extern struct fi_ops_msg cxip_ep_msg_no_tx_ops;
extern struct fi_ops_msg cxip_ep_msg_no_rx_ops;

extern struct fi_ops_tagged cxip_ep_tagged_ops;
extern struct fi_ops_tagged cxip_ep_tagged_no_ops;
extern struct fi_ops_tagged cxip_ep_tagged_no_tx_ops;
extern struct fi_ops_tagged cxip_ep_tagged_no_rx_ops;

extern struct fi_ops_atomic cxip_ep_atomic_ops;
extern struct fi_ops_atomic cxip_ep_atomic_no_ops;

extern struct fi_ops_collective cxip_collective_ops;
extern struct fi_ops_collective cxip_collective_no_ops;

extern struct fi_ops_cm cxip_ep_cm_ops;
extern struct fi_ops_ep cxip_ep_ops;
extern struct fi_ops cxip_ep_fi_ops;
extern struct fi_ops_ep cxip_ctx_ep_ops;

/*
 * cxip_ep_cmdq() - Open a shareable TX or Target command queue.
 *
 * Caller must hold ep_obj->lock
 */
int cxip_ep_cmdq(struct cxip_ep_obj *ep_obj, bool transmit, uint32_t tclass,
		 struct cxi_eq *evtq, struct cxip_cmdq **cmdq)
{
	struct cxi_cq_alloc_opts cq_opts = {};
	struct cxip_cmdq **ep_obj_cmdq;
	ofi_atomic32_t *ep_obj_ref;
	int ret;
	size_t size;

	if (transmit) {
		ep_obj_cmdq = &ep_obj->txq;
		ep_obj_ref = &ep_obj->txq_ref;
		size = ep_obj->txq_size;
	} else {
		ep_obj_cmdq = &ep_obj->tgq;
		ep_obj_ref = &ep_obj->tgq_ref;
		size = ep_obj->tgq_size;
	}

	if (*ep_obj_cmdq) {
		ofi_atomic_inc32(ep_obj_ref);
		CXIP_DBG("Reusing %s base CMDQ: %p\n",
			 transmit ? "TX" : "RX", *ep_obj_cmdq);
		*cmdq = *ep_obj_cmdq;

		return FI_SUCCESS;
	}

	/* An IDC command can use up to 4 64 byte slots. */
	cq_opts.count = size * 4;
	cq_opts.flags = transmit ? CXI_CQ_IS_TX : 0;
	cq_opts.policy = cxip_env.cq_policy;

	ret = cxip_cmdq_alloc(ep_obj->domain->lni, evtq, &cq_opts,
			      ep_obj->auth_key.vni, cxip_ofi_to_cxi_tc(tclass),
			      CXI_TC_TYPE_DEFAULT, cmdq);
	if (ret != FI_SUCCESS) {
		CXIP_WARN("Unable to allocate CMDQ, ret: %d\n", ret);
		return -FI_ENOSPC;
	}
	*ep_obj_cmdq = *cmdq;
	ofi_atomic_inc32(ep_obj_ref);

	CXIP_DBG("Allocated %s CMDQ: %p CP: %u\n",
		 transmit ? "TX" : "RX", *cmdq, cq_opts.lcid);
	return ret;
}

/*
 * cxip_ep_cmdq_put() - Release reference to shareable TX or Target command
 * queue.
 *
 * Caller must hold ep_obj->lock.
 */
void cxip_ep_cmdq_put(struct cxip_ep_obj *ep_obj, bool transmit)
{
	struct cxip_cmdq **ep_obj_cmdq;
	ofi_atomic32_t *ep_obj_ref;

	if (transmit) {
		ep_obj_cmdq = &ep_obj->txq;
		ep_obj_ref = &ep_obj->txq_ref;
	} else {
		ep_obj_cmdq = &ep_obj->tgq;
		ep_obj_ref = &ep_obj->tgq_ref;
	}

	if (!ofi_atomic_dec32(ep_obj_ref)) {
		cxip_cmdq_free(*ep_obj_cmdq);

		CXIP_DBG("Freed %s CMDQ: %p\n",
			 transmit ? "TX" : "RX", *ep_obj_cmdq);
		*ep_obj_cmdq = NULL;
	} else {
		CXIP_DBG("Put %s CMDQ: %p\n",
			 transmit ? "TX" : "RX", *ep_obj_cmdq);
	}
}

static int cxip_ep_cm_getname(fid_t fid, void *addr, size_t *addrlen)
{
	struct cxip_ep *cxip_ep = container_of(fid, struct cxip_ep, ep.fid);
	size_t len;

	len = MIN(*addrlen, sizeof(struct cxip_addr));

	if (!cxip_ep->ep_obj->enabled)
		return -FI_EOPBADSTATE;

	CXIP_DBG("NIC: 0x%x PID: %u\n", cxip_ep->ep_obj->src_addr.nic,
		 cxip_ep->ep_obj->src_addr.pid);

	memcpy(addr, &cxip_ep->ep_obj->src_addr, len);
	*addrlen = sizeof(struct cxip_addr);

	return (len == sizeof(struct cxip_addr)) ? FI_SUCCESS : -FI_ETOOSMALL;
}

static int _join_collective(struct fid_ep *ep, const void *addr,
			    uint64_t flags, struct fid_mc **mc, void *context)
{
	struct fi_collective_addr *arg = (struct fi_collective_addr *)addr;

	return cxip_join_collective(ep, arg->coll_addr, arg->set,
				    flags, mc, context);
}

struct fi_ops_cm cxip_ep_cm_ops = {
	.size = sizeof(struct fi_ops_cm),
	.setname = fi_no_setname,
	.getname = cxip_ep_cm_getname,
	.getpeer = fi_no_getpeer,
	.connect = fi_no_connect,
	.listen = fi_no_listen,
	.accept = fi_no_accept,
	.reject = fi_no_reject,
	.shutdown = fi_no_shutdown,
	.join = _join_collective,
};

/*
 * cxip_ep_progress() - Progress an endpoint.
 */
void cxip_ep_progress(struct fid *fid)
{
	struct cxip_ep *ep = container_of(fid, struct cxip_ep, ep.fid);
	struct cxip_ep_obj *ep_obj = ep->ep_obj;

	if (ep_obj->enabled) {

		ofi_genlock_lock(&ep_obj->lock);
		ep_obj->rxc->ops.progress(ep_obj->rxc);
		ep_obj->txc->ops.progress(ep_obj->txc);
		cxip_ep_ctrl_progress_locked(ep_obj);
		ofi_genlock_unlock(&ep_obj->lock);
	}
}

/*
 * cxip_ep_peek() - Peek at EP event queues
 *
 * Return whether the associated EP event queues are empty.
 */
int cxip_ep_peek(struct fid *fid)
{
	struct cxip_ep *ep = container_of(fid, struct cxip_ep, ep.fid);
	struct cxip_ep_obj *ep_obj = ep->ep_obj;

	if (ep_obj->txc->tx_evtq.eq &&
	    cxi_eq_peek_event(ep_obj->txc->tx_evtq.eq))
		return -FI_EAGAIN;
	if (ep_obj->rxc->rx_evtq.eq &&
	    cxi_eq_peek_event(ep_obj->rxc->rx_evtq.eq))
		return -FI_EAGAIN;

	return FI_SUCCESS;
}

/*
 * fi_ep_get_unexpected_msgs() - Get unexpected message information, exposed
 * via domain open ops.
 */
size_t cxip_ep_get_unexp_msgs(struct fid_ep *fid_ep,
			      struct fi_cq_tagged_entry *entry, size_t count,
			      fi_addr_t *src_addr, size_t *ux_count)
{
	struct cxip_ep *ep = container_of(fid_ep, struct cxip_ep, ep);
	size_t ret_count = 0;

	/* Synchronous implementation to return a snapshot of the unexpected
	 * message queue for the endpoint.
	 */
	if (!ux_count)
		return -FI_EINVAL;

	if (ep->ep_obj->rxc->state == RXC_DISABLED)
		return -FI_EOPBADSTATE;

	if (!ofi_recv_allowed(ep->rx_attr.caps)) {
		CXIP_WARN("FI_RECV not enabled\n");
		return -FI_EINVAL;
	}

	/* If in flow control, let that complete since
	 * on-loading could be in progress.
	 */
	if (ep->ep_obj->rxc->state != RXC_ENABLED &&
	    ep->ep_obj->rxc->state != RXC_ENABLED_SOFTWARE) {
		cxip_cq_progress(ep->ep_obj->rxc->recv_cq);
		return -FI_EAGAIN;
	}

	ofi_genlock_lock(&ep->ep_obj->lock);
	if (cxip_evtq_saturated(&ep->ep_obj->rxc->rx_evtq)) {
		RXC_DBG(ep->ep_obj->rxc, "Target HW EQ saturated\n");
		ofi_genlock_unlock(&ep->ep_obj->lock);

		return -FI_EAGAIN;
	}

	/* Fill in supplied memory with what can fit */
	ret_count = cxip_build_ux_entry_info(ep, entry, count, src_addr,
					     ux_count);
	ofi_genlock_unlock(&ep->ep_obj->lock);

	return ret_count;
}

/*
 * cxip_ep_flush_trig_reqs() - Free triggered request for the EP.
 */
void cxip_ep_flush_trig_reqs(struct cxip_ep_obj *ep_obj)
{
	ofi_genlock_lock(&ep_obj->lock);
	cxip_evtq_flush_trig_reqs(&ep_obj->txc->tx_evtq);
	ofi_genlock_unlock(&ep_obj->lock);
}

/*
 * cxip_txc_close() - close the TX side of endpoint object.
 */
void cxip_txc_close(struct cxip_ep *ep)
{
	struct cxip_txc *txc = ep->ep_obj->txc;

	if (txc->send_cq) {
		ofi_genlock_lock(&txc->send_cq->ep_list_lock);
		fid_list_remove2(&txc->send_cq->util_cq.ep_list,
				&txc->send_cq->util_cq.ep_list_lock,
				&ep->ep.fid);
		ofi_genlock_unlock(&txc->send_cq->ep_list_lock);

		ofi_atomic_dec32(&txc->send_cq->util_cq.ref);
	}

	if (txc->send_cntr) {
		fid_list_remove(&txc->send_cntr->ctx_list,
				&txc->send_cntr->lock,
				&ep->ep.fid);
		ofi_atomic_dec32(&txc->send_cntr->ref);
	}

	if (txc->read_cntr) {
		fid_list_remove(&txc->read_cntr->ctx_list,
				&txc->read_cntr->lock,
				&ep->ep.fid);
		ofi_atomic_dec32(&txc->read_cntr->ref);
	}

	if (txc->write_cntr) {
		fid_list_remove(&txc->write_cntr->ctx_list,
				&txc->write_cntr->lock,
				&ep->ep.fid);
		ofi_atomic_dec32(&txc->write_cntr->ref);
	}

	cxip_domain_remove_txc(txc->domain, txc);

	cxip_txc_disable(txc);
}

/*
 * cxip_rxc_close() - close the RX side of the endpoint object.
 */
void cxip_rxc_close(struct cxip_ep *ep)
{
	struct cxip_rxc *rxc = ep->ep_obj->rxc;

	if (rxc->recv_cq) {
		/* EP FID may not be found in the list if recv_cq == send_cq,
		 * but we still need to decrement reference.
		 */
		ofi_genlock_lock(&rxc->recv_cq->ep_list_lock);
		fid_list_remove2(&rxc->recv_cq->util_cq.ep_list,
				&rxc->recv_cq->util_cq.ep_list_lock,
				&ep->ep.fid);
		ofi_genlock_unlock(&rxc->recv_cq->ep_list_lock);

		ofi_atomic_dec32(&rxc->recv_cq->util_cq.ref);
	}

	if (rxc->recv_cntr) {
		fid_list_remove(&rxc->recv_cntr->ctx_list,
				&rxc->recv_cntr->lock,
				&ep->ep.fid);
		ofi_atomic_dec32(&rxc->recv_cntr->ref);
	}

	cxip_rxc_disable(rxc);
}

/**
 * Get TX/RX option flags.
 *
 * Support TX/RX context control(FI_GETOPSFLAG).
 *
 * @param tx_attr : TX attributes, or NULL
 * @param rx_attr : RX attributes, or NULL
 * @param flags : storage for returned flags
 *
 * @return int : 0 on success, -errno on failure
 */
int cxip_getopflags(struct fi_tx_attr *tx_attr, struct fi_rx_attr *rx_attr,
		    uint64_t *flags)
{
	if ((*flags & FI_TRANSMIT) && (*flags & FI_RECV)) {
		CXIP_WARN("Both Tx/Rx flags cannot be specified\n");
		return -FI_EINVAL;
	} else if (tx_attr && (*flags & FI_TRANSMIT)) {
		*flags = tx_attr->op_flags;
	} else if (rx_attr && (*flags & FI_RECV)) {
		*flags = rx_attr->op_flags;
	} else {
		CXIP_WARN("Tx/Rx flags not specified\n");
		return -FI_EINVAL;
	}

	return 0;
}

/**
 * Set TX/RX option flags.
 *
 * Support TX/RX control(FI_SETOPSFLAG).
 *
 * @param tx_attr : TX attributes, or NULL
 * @param rx_attr : RX attributes, or NULL
 * @param flags : flags to set
 *
 * @return int : 0 on success, -errno on failure
 */
int cxip_setopflags(struct fi_tx_attr *tx_attr, struct fi_rx_attr *rx_attr,
		    uint64_t flags)
{
	if ((flags & FI_TRANSMIT) && (flags & FI_RECV)) {
		CXIP_WARN("Both Tx/Rx flags cannot be specified\n");
		return -FI_EINVAL;
	} else if (tx_attr && (flags & FI_TRANSMIT)) {
		tx_attr->op_flags = flags;
		tx_attr->op_flags &= ~FI_TRANSMIT;
		if (!(flags & (FI_INJECT_COMPLETE | FI_TRANSMIT_COMPLETE |
			       FI_DELIVERY_COMPLETE)))
			tx_attr->op_flags |= FI_TRANSMIT_COMPLETE;
	} else if (rx_attr && (flags & FI_RECV)) {
		rx_attr->op_flags = flags;
		rx_attr->op_flags &= ~FI_RECV;
	} else {
		CXIP_WARN("Tx/Rx flags not specified\n");
		return -FI_EINVAL;
	}

	return 0;
}

/**
 * Cancel RX operation
 *
 * Support TX/RX context cancel().
 *
 * Searches the RX queue for a pending async operation with the specified
 * 'context', and cancels it if still pending.
 *
 * @param rxc : RX context to search
 * @param context : user context pointer to search for
 *
 * @return ssize_t : 0 on success, -errno on failure
 */
ssize_t cxip_rxc_cancel(struct cxip_rxc *rxc, void *context)
{
	if (rxc->state == RXC_DISABLED)
		return -FI_EOPBADSTATE;

	return cxip_evtq_req_cancel(&rxc->rx_evtq, rxc, context, true);
}

/**
 * Cancel TX operation
 *
 * Support TX/RX context cancel().
 *
 * Searches the TX queue for a pending async operation with the specified
 * 'context', and request it be canceled.
 *
 * @param rxc : TX context to search
 * @param context : user context pointer to search for
 *
 * @return ssize_t : 0 on success, -errno on failure
 */
ssize_t cxip_txc_cancel(struct cxip_txc *txc, void *context)
{
	struct cxip_req *req;
	struct dlist_entry *tmp;

	if (!txc->enabled)
		return -FI_EOPBADSTATE;

	if (!context)
		return -FI_ENOENT;

	/* Only messaging may be canceled at this time */
	dlist_foreach_container_safe(&txc->msg_queue, struct cxip_req, req,
				     send.txc_entry, tmp) {
		if (req->type != CXIP_REQ_SEND ||
		    req->context != (uint64_t)context)
			continue;
		return txc->ops.cancel_msg_send(req);
	}

	return -FI_ENOENT;
}

/*
 * cxip_ep_cancel() - Cancel TX/RX operation for EP.
 */
ssize_t cxip_ep_cancel(fid_t fid, void *context)
{
	struct cxip_ep *ep = container_of(fid, struct cxip_ep, ep.fid);
	ssize_t ret;

	/* TODO: Remove this since it requires malicious programming to
	 * create this condition.
	 */
	if (fid->fclass != FI_CLASS_EP)
		return -FI_EINVAL;

	if (!ofi_recv_allowed(ep->ep_obj->caps))
		return -FI_ENOENT;

	ret = cxip_rxc_cancel(ep->ep_obj->rxc, context);
	if (ret != -FI_ENOENT)
		return ret;

	return cxip_txc_cancel(ep->ep_obj->txc, context);
}

/*
 * cxip_ep_enable() - Enable standard EP.
 */
static int cxip_ep_enable(struct fid_ep *fid_ep)
{
	struct cxip_ep *ep = container_of(fid_ep, struct cxip_ep, ep);
	struct cxip_ep_obj *ep_obj = ep->ep_obj;
	int ret = FI_SUCCESS;

	ofi_genlock_lock(&ep_obj->lock);
	if (ep_obj->enabled)
		goto unlock;

	if (!ep_obj->av) {
		CXIP_WARN("Endpoint must be bound to an AV\n");
		ret = -FI_ENOAV;
		goto unlock;
	}

	assert(ep_obj->domain->enabled);

	/* src_addr.pid may be C_PID_ANY at this point. */
	if (ep_obj->av_auth_key) {
		ret = cxip_av_auth_key_get_vnis(ep_obj->av, &ep_obj->vnis,
						&ep_obj->vni_count);
		if (ret)
			goto unlock;

		ret = cxip_portals_table_alloc(ep_obj->domain->lni,
					       ep_obj->vnis, ep_obj->vni_count,
					       ep_obj->src_addr.pid,
					       &ep_obj->ptable);
		if (ret != FI_SUCCESS) {
			CXIP_WARN("Failed to allocate auth key ring portals table: %d\n",
				  ret);
			goto free_vnis;
		}

		/* This is unfortunately needed to allocate a command queue.
		 * But, this can be changed later.
		 */
		ep_obj->auth_key.vni = ep_obj->vnis[0];
	} else {
		ret = cxip_portals_table_alloc(ep_obj->domain->lni,
					       &ep_obj->auth_key.vni, 1,
					       ep_obj->src_addr.pid,
					       &ep_obj->ptable);
		if (ret != FI_SUCCESS) {
			CXIP_WARN("Failed to allocate portals table: %d\n",
				  ret);
			goto unlock;
		}
	}

	ep_obj->src_addr.pid = ep_obj->ptable->pid;

	ret = cxip_ep_ctrl_init(ep_obj);
	if (ret != FI_SUCCESS) {
		CXIP_WARN("cxip_ep_ctrl_init returned: %d\n", ret);
		goto free_portals_table;
	}

	ret = cxip_zbcoll_init(ep_obj);
	if (ret != FI_SUCCESS) {
		CXIP_WARN("cxip_zbcoll_init returned: %d\n", ret);
		goto free_ep_ctrl;
	}

	CXIP_DBG("EP assigned NIC: %#x VNI: %u PID: %u\n",
		 ep_obj->src_addr.nic,
		 ep_obj->auth_key.vni,
		 ep_obj->src_addr.pid);

	ret = cxip_txc_enable(ep_obj->txc);
	if (ret != FI_SUCCESS) {
		CXIP_WARN("cxip_txc_enable returned: %d\n", ret);
		goto unlock;
	}

	ret = cxip_rxc_enable(ep_obj->rxc);
	if (ret != FI_SUCCESS) {
		CXIP_WARN("cxip_rxc_enable returned: %d\n", ret);
		goto unlock;
	}

	ret = cxip_coll_enable(ep);
	if (ret != FI_SUCCESS) {
		CXIP_WARN("cxip_coll_enable returned: %d\n", ret);
		/* collectives will not function, but EP will */
	}

	/* Enable only appropriate API functions based on primary/secondary
	 * capabilities. Send/Receive requires FI_MSG or FI_TAGGED.
	 */
	if (ofi_send_allowed(ep->tx_attr.caps & ~FI_MSG) &&
	    ofi_recv_allowed(ep->rx_attr.caps & ~FI_MSG))
		ep->ep.tagged = &cxip_ep_tagged_ops;
	else if (ofi_send_allowed(ep->tx_attr.caps & ~FI_MSG))
		ep->ep.tagged = &cxip_ep_tagged_no_rx_ops;
	else if (ofi_recv_allowed(ep->rx_attr.caps & ~FI_MSG))
		ep->ep.tagged = &cxip_ep_tagged_no_tx_ops;

	if (ofi_send_allowed(ep->tx_attr.caps & ~FI_TAGGED) &&
	    ofi_recv_allowed(ep->rx_attr.caps & ~FI_TAGGED))
		ep->ep.msg = &cxip_ep_msg_ops;
	else if (ofi_send_allowed(ep->tx_attr.caps & ~FI_TAGGED))
		ep->ep.msg = &cxip_ep_msg_no_rx_ops;
	else if (ofi_recv_allowed(ep->rx_attr.caps & ~FI_TAGGED))
		ep->ep.msg = &cxip_ep_msg_no_tx_ops;

	/* Initiate requires FI_RMA or FI_ATOMIC */
	if (ofi_rma_initiate_allowed(ep->tx_attr.caps & ~FI_RMA))
		ep->ep.atomic = &cxip_ep_atomic_ops;

	if (ofi_rma_initiate_allowed(ep->tx_attr.caps & ~FI_ATOMIC))
		ep->ep.rma = &cxip_ep_rma_ops;

	ep_obj->enabled = true;
	ofi_genlock_unlock(&ep_obj->lock);

	return FI_SUCCESS;

free_ep_ctrl:
	cxip_ep_ctrl_fini(ep_obj);

free_portals_table:
	cxip_portals_table_free(ep_obj->ptable);
	ep_obj->ptable = NULL;
free_vnis:
	if (ep_obj->vnis) {
		cxip_av_auth_key_put_vnis(ep_obj->av, ep_obj->vnis,
					  ep_obj->vni_count);
		ep_obj->vnis = NULL;
	}
unlock:
	ofi_genlock_unlock(&ep_obj->lock);

	return ret;
}

/*
 * cxip_ep_disable() - Disable the base EP if enabled.
 */
static void cxip_ep_disable(struct cxip_ep_obj *ep_obj)
{
	if (ep_obj->enabled) {
		cxip_coll_disable(ep_obj);
		cxip_zbcoll_fini(ep_obj);
		cxip_ep_ctrl_fini(ep_obj);
		cxip_portals_table_free(ep_obj->ptable);
		if (ep_obj->vnis) {
			cxip_av_auth_key_put_vnis(ep_obj->av, ep_obj->vnis,
						  ep_obj->vni_count);
			ep_obj->vnis = NULL;
		}
		ep_obj->ptable = NULL;
		ep_obj->enabled = false;
	}
}

/*
 * cxip_free_endpoint() - Release base EP object resources and free object.
 */
int cxip_free_endpoint(struct cxip_ep *ep)
{
	struct cxip_ep_obj *ep_obj = ep->ep_obj;
	int count;

	/* Each bound MR increments ref, so MRs must be removed.
	 */
	count = ofi_atomic_get32(&ep_obj->ref);
	if (count) {
		CXIP_WARN("EP refcount non-zero: %d\n", count);
		return -FI_EBUSY;
	}

	count = ofi_atomic_get32(&ep_obj->coll.num_mc);
	if (count) {
		CXIP_WARN("EP num_mc non-zero: %d\n", count);
		return -FI_EBUSY;
	}

	if (ep_obj->av)
		cxip_av_unbind_ep(ep_obj->av, ep);

	if (ep->ep_obj->eq) {
		ofi_mutex_lock(&ep_obj->eq->list_lock);
		dlist_remove(&ep_obj->eq_link);
		ofi_mutex_unlock(&ep_obj->eq->list_lock);
		ofi_atomic_dec32(&ep_obj->eq->util_eq.ref);
	}

	ofi_genlock_lock(&ep_obj->lock);
	cxip_coll_close(ep_obj);
	cxip_txc_close(ep);
	cxip_rxc_close(ep);
	cxip_ep_disable(ep_obj);
	ofi_genlock_unlock(&ep_obj->lock);

	ofi_atomic_dec32(&ep_obj->domain->ref);
	ofi_genlock_destroy(&ep_obj->lock);

	cxip_txc_free(ep_obj->txc);
	cxip_rxc_free(ep_obj->rxc);
	free(ep_obj);
	ep->ep_obj = NULL;

	return FI_SUCCESS;
}

/*
 * cxip_ep_close() - Close (destroy) the base EP.
 */
static int cxip_ep_close(struct fid *fid)
{
	struct cxip_ep *ep = container_of(fid, struct cxip_ep, ep.fid);
	int ret;
	int count;

	if (ep->is_alias) {
		count = ofi_atomic_get32(&ep->ep_obj->ref);

		if (count > 0) {
			ofi_atomic_dec32(&ep->ep_obj->ref);
			free(ep);
			return FI_SUCCESS;
		}

		CXIP_WARN("EP alias %p, invalid EP object refcnt %d\n",
			  ep, count);
		return -FI_EINVAL;
	}

	ret = cxip_free_endpoint(ep);
	if (ret) {
		CXIP_WARN("Unable to free EP object %d : %s\n",
			  ret, fi_strerror(-ret));
		return ret;
	}
	free(ep);

	return FI_SUCCESS;
}

/*
 * cxip_ep_bind_cq() - Bind the EP to the CQ resource.
 */
static int cxip_ep_bind_cq(struct cxip_ep *ep, struct cxip_cq *cq,
			   uint64_t flags)
{
	struct cxip_txc *txc;
	struct cxip_rxc *rxc;
	int ret;

	if (ep->ep_obj->domain != cq->domain) {
		CXIP_WARN("Invalid CQ domain for EP\n");
		return -FI_EINVAL;
	}

	if ((flags | CXIP_EP_CQ_FLAGS) != CXIP_EP_CQ_FLAGS) {
		CXIP_WARN("Invalid CQ flags\n");
		return -FI_EINVAL;
	}

	if (flags & FI_TRANSMIT) {
		txc = ep->ep_obj->txc;
		if (txc->send_cq) {
			CXIP_WARN("SEND CQ previously bound\n");
			return -FI_EINVAL;
		}

		ofi_atomic_inc32(&cq->util_cq.ref);
		txc->send_cq = cq;

		if (flags & FI_SELECTIVE_COMPLETION)
			txc->selective_completion = 1;
		if (!txc->selective_completion)
			txc->attr.op_flags |= FI_COMPLETION;

		ep->tx_attr.op_flags = txc->attr.op_flags;

		/* Use CXI ep_list_lock that can be selectively optimized */
		ofi_genlock_lock(&cq->ep_list_lock);
		ret = fid_list_insert2(&cq->util_cq.ep_list,
				      &cq->util_cq.ep_list_lock,
				      &ep->ep.fid);
		ofi_genlock_unlock(&cq->ep_list_lock);

		if (ret) {
			CXIP_WARN("EP CQ fid insert failed %d\n", ret);
			ofi_atomic_dec32(&cq->util_cq.ref);
			txc->send_cq = NULL;
		}
	}

	if (flags & FI_RECV) {
		rxc = ep->ep_obj->rxc;
		if (rxc->recv_cq) {
			CXIP_WARN("RECV CQ previously bound\n");
			return -FI_EINVAL;
		}

		ofi_atomic_inc32(&cq->util_cq.ref);
		rxc->recv_cq = cq;

		if (flags & FI_SELECTIVE_COMPLETION)
			rxc->selective_completion = 1;
		if (!rxc->selective_completion)
			rxc->attr.op_flags |= FI_COMPLETION;

		ep->rx_attr.op_flags = rxc->attr.op_flags;

		/* Use CXI ep_list_lock that can be selectively optimized */
		ofi_genlock_lock(&cq->ep_list_lock);
		ret = fid_list_insert2(&cq->util_cq.ep_list,
				      &cq->util_cq.ep_list_lock,
				      &ep->ep.fid);
		ofi_genlock_unlock(&cq->ep_list_lock);

		if (ret) {
			CXIP_WARN("EP CQ fid insert failed %d\n", ret);
			ofi_atomic_dec32(&cq->util_cq.ref);
			rxc->recv_cq = NULL;
		}
	}
	return FI_SUCCESS;
}

/*
 * cxip_ep_bind_cntr() - Bind EP to counter resource
 */
static int cxip_ep_bind_cntr(struct cxip_ep *ep, struct cxip_cntr *cntr,
			     uint64_t flags)
{
	int ret;

	if (ep->ep_obj->domain != cntr->domain) {
		CXIP_WARN("Counter domain invalid for EP\n");
		return -FI_EINVAL;
	}

	if (!(flags & CXIP_EP_CNTR_FLAGS))
		return FI_SUCCESS;

	if ((flags & FI_SEND && ep->ep_obj->txc->send_cntr) ||
	    (flags & FI_READ && ep->ep_obj->txc->read_cntr) ||
	    (flags & FI_WRITE && ep->ep_obj->txc->write_cntr) ||
	    (flags & FI_RECV && ep->ep_obj->rxc->recv_cntr)) {
		CXIP_WARN("EP previously bound to counter\n");
		return -FI_EINVAL;
	}

	ret = fid_list_insert(&cntr->ctx_list, &cntr->lock, &ep->ep.fid);
	if (ret) {
		CXIP_WARN("Add of EP to cntr EP list failed: %d:%s\n",
			  ret, fi_strerror(-ret));
		return ret;
	}

	if (flags & FI_SEND) {
		ep->ep_obj->txc->send_cntr = cntr;
		ofi_atomic_inc32(&cntr->ref);
	}
	if (flags & FI_READ) {
		ep->ep_obj->txc->read_cntr = cntr;
		ofi_atomic_inc32(&cntr->ref);
	}
	if (flags & FI_WRITE) {
		ep->ep_obj->txc->write_cntr = cntr;
		ofi_atomic_inc32(&cntr->ref);
	}
	if (flags & FI_RECV) {
		ep->ep_obj->rxc->recv_cntr = cntr;
		ofi_atomic_inc32(&cntr->ref);
	}

	return FI_SUCCESS;
}

/*
 * cxip_ep_bind() - Bind EP resources.
 */
int cxip_ep_bind(struct fid *fid, struct fid *bfid, uint64_t flags)
{
	int ret;
	struct cxip_ep *ep = container_of(fid, struct cxip_ep, ep.fid);
	struct cxip_eq *eq;
	struct cxip_cq *cq;
	struct cxip_av *av;
	struct cxip_cntr *cntr;

	/* TODO: Remove this since it requires malicious programming to
	 * create this condition.
	 */
	if (fid->fclass != FI_CLASS_EP)
		return -FI_EINVAL;

	ret = ofi_ep_bind_valid(&cxip_prov, bfid, flags);
	if (ret)
		return ret;

	switch (bfid->fclass) {
	case FI_CLASS_EQ:
		eq = container_of(bfid, struct cxip_eq, util_eq.eq_fid.fid);
		ofi_atomic_inc32(&eq->util_eq.ref);
		ofi_mutex_lock(&eq->list_lock);
		dlist_insert_tail(&ep->ep_obj->eq_link, &eq->ep_list);
		ofi_mutex_unlock(&eq->list_lock);
		ep->ep_obj->eq = eq;
		break;

	case FI_CLASS_CQ:
		cq = container_of(bfid, struct cxip_cq, util_cq.cq_fid.fid);
		ret = cxip_ep_bind_cq(ep, cq, flags);
		if (ret)
			return ret;
		break;

	case FI_CLASS_CNTR:
		cntr = container_of(bfid, struct cxip_cntr, cntr_fid.fid);
		ret = cxip_ep_bind_cntr(ep, cntr, flags);
		if (ret)
			return ret;
		break;

	case FI_CLASS_AV:
		av = container_of(bfid, struct cxip_av, av_fid.fid);
		ret = cxip_av_bind_ep(av, ep);
		if (ret)
			return ret;
		ep->ep_obj->av = av;

		break;

	default:
		return -FI_EINVAL;
	}

	return FI_SUCCESS;
}

/*
 * cxip_set_tclass()
 */
int cxip_set_tclass(uint32_t desired_tc, uint32_t default_tc, uint32_t *new_tc)
{
	assert(new_tc != NULL);

	if (desired_tc != FI_TC_UNSPEC) {
		if (desired_tc >= FI_TC_LABEL &&
		    desired_tc <= FI_TC_SCAVENGER) {
			*new_tc = desired_tc;
		} else {
			CXIP_WARN("Invalid tclass\n");
			return -FI_EINVAL;
		}
	} else {
		*new_tc = default_tc;
	}

	CXIP_DBG("Set tclass to %d\n", *new_tc);
	return FI_SUCCESS;
}

/**
 * provider fi_set_val()/FI_SET_VAL implementation for EP
 *
 * @param fid : EP fid
 * @param val : parameter structure for set value operations.
 *
 * @return int : 0 on success, -errno on failure
 */
static inline int cxip_ep_set_val(struct cxip_ep *cxi_ep,
				  struct fi_fid_var *val)
{
	struct cxip_txc_rnr *txc_rnr;
	uint64_t *req_order;
	uint64_t *req_rnr_max_time;
	uint32_t *req_tclass;
	uint32_t new_tclass;

	if (!val->val)
		return -FI_EINVAL;

	switch (val->name) {
	case FI_OPT_CXI_SET_TCLASS:
		req_tclass = (uint32_t *) val->val;

		if (cxip_set_tclass(*req_tclass, cxi_ep->tx_attr.tclass,
				    &new_tclass))
			return -FI_EINVAL;

		cxi_ep->tx_attr.tclass = new_tclass;
		break;
	case FI_OPT_CXI_SET_MSG_ORDER:
		req_order = (uint64_t *) val->val;

		if (*req_order & ~CXIP_MSG_ORDER) {
			CXIP_WARN("Invalid message order 0x%" PRIx64 "\n",
				  *req_order);
			return -FI_EINVAL;
		}

		cxi_ep->tx_attr.msg_order = *req_order;
		break;
	case FI_OPT_CXI_SET_RNR_MAX_RETRY_TIME:
		req_rnr_max_time = (uint64_t *) val->val;

		if (cxi_ep->ep_obj->protocol != FI_PROTO_CXI_RNR) {
			CXIP_WARN("Not FI_PROTO_CXI_RNR EP\n");
			return -FI_EINVAL;
		}

		txc_rnr = container_of(cxi_ep->ep_obj->txc, struct cxip_txc_rnr,
				       base);
		txc_rnr->max_retry_wait_us = *req_rnr_max_time;
		CXIP_DBG("RNR maximum timeout set to %ld usec\n",
			 txc_rnr->max_retry_wait_us);
		break;
	default:
		return -FI_EINVAL;
	}

	return FI_SUCCESS;
}

/*
 * cxip_ep_control() - Provider EP control implementation.
 */
static int cxip_ep_control(struct fid *fid, int command, void *arg)
{
	struct cxip_ep *ep = container_of(fid, struct cxip_ep, ep.fid);
	struct cxip_ep *new_ep;
	struct fi_alias *alias;
	int ret;

	/* TODO: Remove this since it requires malicious programming to
	 * create this condition.
	 */
	if (fid->fclass != FI_CLASS_EP)
		return -FI_EINVAL;

	switch (command) {
	case FI_ALIAS:
		if (!arg)
			return -FI_EINVAL;
		alias = (struct fi_alias *)arg;
		if (!alias->fid)
			return -FI_EINVAL;
		new_ep = calloc(1, sizeof(*new_ep));
		if (!new_ep)
			return -FI_ENOMEM;

		memcpy(&new_ep->tx_attr, &ep->tx_attr,
		       sizeof(struct fi_tx_attr));
		memcpy(&new_ep->rx_attr, &ep->rx_attr,
		       sizeof(struct fi_rx_attr));
		ret = cxip_setopflags(&new_ep->tx_attr, &new_ep->rx_attr,
				      alias->flags);
		if (ret) {
			free(new_ep);
			return -FI_EINVAL;
		}
		new_ep->ep_obj = ep->ep_obj;
		new_ep->is_alias = 1;
		memcpy(&new_ep->ep, &ep->ep, sizeof(struct fid_ep));
		*alias->fid = &new_ep->ep.fid;
		ofi_atomic_inc32(&new_ep->ep_obj->ref);
		break;
	case FI_GETOPSFLAG:
		if (!arg)
			return -FI_EINVAL;
		ret = cxip_getopflags(&ep->tx_attr, &ep->rx_attr,
				      (uint64_t *)arg);
		if (ret)
			return -FI_EINVAL;
		break;
	case FI_SETOPSFLAG:
		if (!arg)
			return -FI_EINVAL;
		ret = cxip_setopflags(&ep->tx_attr, &ep->rx_attr,
				      *(uint64_t *)arg);
		if (ret)
			return -FI_EINVAL;
		break;
	case FI_ENABLE:
		return cxip_ep_enable(&ep->ep);
	case FI_SET_VAL:
		if (!arg)
			return -FI_EINVAL;
		return cxip_ep_set_val(ep, (struct fi_fid_var *) arg);
	default:
		return -FI_EINVAL;
	}

	return FI_SUCCESS;
}

struct fi_ops cxip_ep_fi_ops = {
	.size = sizeof(struct fi_ops),
	.close = cxip_ep_close,
	.bind = cxip_ep_bind,
	.control = cxip_ep_control,
	.ops_open = fi_no_ops_open,
};

int cxip_ep_getopt_priv(struct cxip_ep *ep, int level, int optname,
			void *optval, size_t *optlen)
{
	if (level != FI_OPT_ENDPOINT)
		return -FI_ENOPROTOOPT;

	switch (optname) {
	case FI_OPT_MIN_MULTI_RECV:
		if (!optval || !optlen)
			return -FI_EINVAL;
		if (*optlen < sizeof(size_t))
			return -FI_ETOOSMALL;

		*(size_t *)optval = ep->ep_obj->rxc->min_multi_recv;
		*optlen = sizeof(size_t);
		break;

	default:
		return -FI_ENOPROTOOPT;
	}

	return FI_SUCCESS;
}

/*
 * cxip_ep_getopt() - Return endpoint option value if supported.
 */
static int cxip_ep_getopt(fid_t fid, int level, int optname, void *optval,
			  size_t *optlen)
{
	struct cxip_ep *ep = container_of(fid, struct cxip_ep, ep.fid);

	return cxip_ep_getopt_priv(ep, level, optname, optval, optlen);
}

int cxip_ep_setopt_priv(struct cxip_ep *ep, int level, int optname,
			const void *optval, size_t optlen)
{
	size_t min_multi_recv;

	if (level != FI_OPT_ENDPOINT)
		return -FI_ENOPROTOOPT;

	switch (optname) {
	case FI_OPT_MIN_MULTI_RECV:
		if (!optval)
			return -FI_EINVAL;

		min_multi_recv = *(size_t *)optval;

		if (min_multi_recv > CXIP_EP_MAX_MULTI_RECV) {
			CXIP_WARN("Maximum min_multi_recv value is: %u\n",
				  CXIP_EP_MAX_MULTI_RECV);
			return -FI_EINVAL;
		}
		ep->ep_obj->rxc->min_multi_recv = min_multi_recv;
		break;

	default:
		return -FI_ENOPROTOOPT;
	}

	return FI_SUCCESS;
}

/*
 * cxip_ep_setopt() - Set endpoint option value if supported.
 */
static int cxip_ep_setopt(fid_t fid, int level, int optname, const void *optval,
			  size_t optlen)
{
	struct cxip_ep *ep = container_of(fid, struct cxip_ep, ep.fid);

	return cxip_ep_setopt_priv(ep, level, optname, optval, optlen);
}

struct fi_ops_ep cxip_ep_ops = {
	.size = sizeof(struct fi_ops_ep),
	.cancel = cxip_ep_cancel,
	.getopt = cxip_ep_getopt,
	.setopt = cxip_ep_setopt,
	.tx_ctx = fi_no_tx_ctx,
	.rx_ctx = fi_no_rx_ctx,
	.rx_size_left = fi_no_rx_size_left,
	.tx_size_left = fi_no_tx_size_left,
};

/*
 * cxip_alloc_endpoint() - Allocate and initialize base EP object.
 */
int cxip_alloc_endpoint(struct cxip_domain *cxip_dom, struct fi_info *hints,
			struct cxip_ep_obj **ep_base_obj, void *context)
{
	int ret;
	struct cxip_ep_obj *ep_obj;
	uint32_t txc_tclass;
	uint32_t nic;
	uint32_t pid;
	int i;

	if (!hints || !hints->ep_attr || !hints->tx_attr || !hints->rx_attr)
		return -FI_EINVAL;

	ret = ofi_prov_check_info(&cxip_util_prov, CXIP_FI_VERSION, hints);
	if (ret != FI_SUCCESS)
		return -FI_EINVAL;

	if (cxip_dom->auth_key_entry_max > 1 &&
	    ((hints->caps & FI_DIRECTED_RECV) ||
	     (hints->rx_attr->caps & FI_DIRECTED_RECV))) {
		CXIP_WARN("FI_DIRECTED_RECV not supported with multiple auth key per EP\n");
		return -FI_EINVAL;
	}

	ret = cxip_check_auth_key_info(hints);
	if (ret)
		return ret;

	nic = cxip_dom->nic_addr;
	if (hints->src_addr) {
		struct cxip_addr *src = hints->src_addr;
		if (src->nic != nic) {
			CXIP_WARN("bad src_addr NIC value\n");
			return -FI_EINVAL;
		}
		pid = src->pid;
	} else {
		pid = C_PID_ANY;
	}

	ep_obj = calloc(1, sizeof(struct cxip_ep_obj));
	if (!ep_obj)
		return -FI_ENOMEM;

	/* Save EP attributes from hints */
	ep_obj->protocol = hints->ep_attr->protocol;
	ep_obj->caps = hints->caps;
	ep_obj->ep_attr = *hints->ep_attr;
	ep_obj->txq_size = hints->tx_attr->size;
	ep_obj->tgq_size = hints->rx_attr->size;
	ep_obj->tx_attr = *hints->tx_attr;
	ep_obj->rx_attr = *hints->rx_attr;

	ep_obj->asic_ver = cxip_dom->iface->info->cassini_version;

	ofi_atomic_initialize32(&ep_obj->ref, 0);

	/* Allow FI_THREAD_DOMAIN optimizaiton */
	if (cxip_dom->util_domain.threading == FI_THREAD_DOMAIN ||
	    cxip_dom->util_domain.threading == FI_THREAD_COMPLETION)
		ofi_genlock_init(&ep_obj->lock, OFI_LOCK_NONE);
	else
		ofi_genlock_init(&ep_obj->lock, OFI_LOCK_SPINLOCK);

	ep_obj->domain = cxip_dom;
	ep_obj->src_addr.nic = nic;
	ep_obj->src_addr.pid = pid;
	ep_obj->fi_addr = FI_ADDR_NOTAVAIL;

	ofi_atomic_initialize32(&ep_obj->txq_ref, 0);
	ofi_atomic_initialize32(&ep_obj->tgq_ref, 0);

	for (i = 0; i < CXIP_NUM_CACHED_KEY_LE; i++) {
		ofi_atomic_initialize32(&ep_obj->ctrl.std_mr_cache[i].ref, 0);
		ofi_atomic_initialize32(&ep_obj->ctrl.opt_mr_cache[i].ref, 0);
	}

	dlist_init(&ep_obj->ctrl.mr_list);
	ep_obj->ep_attr.tx_ctx_cnt = 1;
	ep_obj->ep_attr.rx_ctx_cnt = 1;

	if (hints->ep_attr->auth_key) {
		/* Auth key size is verified in ofi_prov_check_info(). */
		assert(hints->ep_attr->auth_key_size ==
		       sizeof(struct cxi_auth_key));

		memcpy(&ep_obj->auth_key, hints->ep_attr->auth_key,
		       sizeof(struct cxi_auth_key));

		/* All EPs that share a Domain must use the same Service ID. */
		if (ep_obj->auth_key.svc_id != cxip_dom->auth_key.svc_id) {
			CXIP_WARN("Invalid svc_id: %u\n",
				  ep_obj->auth_key.svc_id);
			ret = -FI_EINVAL;
			goto err;
		}
	} else {
		if (cxip_dom->av_auth_key) {
			ep_obj->av_auth_key = true;
		} else {
			/* Inherit auth_key from Domain. */
			ep_obj->auth_key = cxip_dom->auth_key;
			CXIP_DBG("Inherited domain auth_key\n");
		}
	}

	if (cxip_set_tclass(ep_obj->tx_attr.tclass,
			    cxip_dom->tclass, &txc_tclass)) {
		CXIP_WARN("Invalid tclass\n");
		ret = -FI_EINVAL;
		goto err;
	}

	ep_obj->tx_attr.tclass = txc_tclass;

	ep_obj->txc = cxip_txc_calloc(ep_obj, context);
	if (!ep_obj->txc) {
		ret = -FI_ENOMEM;
		goto err;
	}

	ep_obj->rxc = cxip_rxc_calloc(ep_obj, context);
	if (!ep_obj->rxc) {
		ret = -FI_ENOMEM;
		goto err;
	}

	ofi_atomic_inc32(&cxip_dom->ref);
	*ep_base_obj = ep_obj;

	return FI_SUCCESS;

err:
	/* handles null check */
	cxip_txc_free(ep_obj->txc);
	cxip_rxc_free(ep_obj->rxc);
	free(ep_obj);

	return ret;
}

/*
 * cxip_endpoint() - Provider fi_endpoint() implementation.
 */
int cxip_endpoint(struct fid_domain *domain, struct fi_info *info,
		  struct fid_ep **fid_ep, void *context)
{
	int ret;
	struct cxip_ep *ep;
	struct cxip_domain *cxip_dom = container_of(domain, struct cxip_domain,
						    util_domain.domain_fid);
	if (!fid_ep)
		return -FI_EINVAL;

	ep = calloc(1, sizeof(*ep));
	if (!ep)
		return -FI_ENOMEM;

	/* Allocate and initialize the base endpoint */
	ret = cxip_alloc_endpoint(cxip_dom, info, &ep->ep_obj, context);
	if (ret) {
		free(ep);
		return ret;
	}

	/* Store EP attribures with the wrapper since values can be
	 * overridden by alias EP that share the same EP object.
	 */
	ep->tx_attr = ep->ep_obj->tx_attr;
	ep->rx_attr = ep->ep_obj->rx_attr;

	ep->ep.fid.fclass = FI_CLASS_EP;
	ep->ep.fid.context = context;
	ep->ep.fid.ops = &cxip_ep_fi_ops;
	ep->ep.ops = &cxip_ep_ops;
	ep->ep.cm = &cxip_ep_cm_ops;

	/* Initialize API to not supported until EP is enabled */
	ep->ep.msg = &cxip_ep_msg_no_ops;
	ep->ep.tagged = &cxip_ep_tagged_no_ops;
	ep->ep.rma = &cxip_ep_rma_no_ops;
	ep->ep.atomic = &cxip_ep_atomic_no_ops;
	ep->ep.collective = &cxip_collective_no_ops;

	*fid_ep = &ep->ep;

	cxip_coll_init(ep->ep_obj);
	cxip_domain_add_txc(ep->ep_obj->domain, ep->ep_obj->txc);

	return FI_SUCCESS;
}
