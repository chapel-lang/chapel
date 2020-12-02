/*
 * Copyright (c) 2013-2018 Intel Corporation, Inc.  All rights reserved.
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

#include "config.h"

#include "fi_verbs.h"

static struct fi_ops_msg vrb_srq_msg_ops;


/* Receive CQ credits are pre-allocated */
ssize_t vrb_post_recv(struct vrb_ep *ep, struct ibv_recv_wr *wr)
{
	struct vrb_context *ctx;
	struct vrb_cq *cq;
	struct ibv_recv_wr *bad_wr;
	int ret;

	cq = container_of(ep->util_ep.rx_cq, struct vrb_cq, util_cq);
	cq->util_cq.cq_fastlock_acquire(&cq->util_cq.cq_lock);
	ctx = ofi_buf_alloc(cq->ctx_pool);
	if (!ctx)
		goto unlock;

	ctx->ep = ep;
	ctx->user_ctx = (void *) (uintptr_t) wr->wr_id;
	ctx->flags = FI_RECV;
	wr->wr_id = (uintptr_t) ctx;

	ret = ibv_post_recv(ep->ibv_qp, wr, &bad_wr);
	wr->wr_id = (uintptr_t) ctx->user_ctx;
	if (ret)
		goto freebuf;
	cq->util_cq.cq_fastlock_release(&cq->util_cq.cq_lock);
	return 0;

freebuf:
	ofi_buf_free(ctx);
unlock:
	cq->util_cq.cq_fastlock_release(&cq->util_cq.cq_lock);
	return -FI_EAGAIN;
}

ssize_t vrb_post_send(struct vrb_ep *ep, struct ibv_send_wr *wr)
{
	struct vrb_context *ctx;
	struct vrb_cq *cq;
	struct ibv_send_wr *bad_wr;
	struct ibv_wc wc;
	int ret;

	cq = container_of(ep->util_ep.tx_cq, struct vrb_cq, util_cq);
	cq->util_cq.cq_fastlock_acquire(&cq->util_cq.cq_lock);
	ctx = ofi_buf_alloc(cq->ctx_pool);
	if (!ctx)
		goto unlock;

	if (!cq->credits || !ep->tx_credits) {
		ret = vrb_poll_cq(cq, &wc);
		if (ret > 0)
			vrb_save_wc(cq, &wc);

		if (!cq->credits || !ep->tx_credits)
			goto freebuf;
	}

	cq->credits--;
	ep->tx_credits--;

	ctx->ep = ep;
	ctx->user_ctx = (void *) (uintptr_t) wr->wr_id;
	ctx->flags = FI_TRANSMIT;
	wr->wr_id = (uintptr_t) ctx;

	ret = ibv_post_send(ep->ibv_qp, wr, &bad_wr);
	wr->wr_id = (uintptr_t) ctx->user_ctx;
	if (ret) {
		VERBS_WARN(FI_LOG_EP_DATA,
			   "Post send failed - %zd\n", vrb_convert_ret(ret));
		goto credits;
	}
	cq->util_cq.cq_fastlock_release(&cq->util_cq.cq_lock);

	return 0;

credits:
	cq->credits++;
	ep->tx_credits++;
freebuf:
	ofi_buf_free(ctx);
unlock:
	cq->util_cq.cq_fastlock_release(&cq->util_cq.cq_lock);
	return -FI_EAGAIN;
}

static inline int vrb_msg_ep_cmdata_size(fid_t fid)
{
	struct vrb_pep *pep;
	struct vrb_ep *ep;
	struct fi_info *info;

	switch (fid->fclass) {
	case FI_CLASS_PEP:
		pep = container_of(fid, struct vrb_pep, pep_fid.fid);
		info = pep->info;
		break;
	case FI_CLASS_EP:
		ep = container_of(fid, struct vrb_ep, util_ep.ep_fid.fid);
		info = ep->info;
		break;
	default:
		info = NULL;
	};
	if (vrb_is_xrc(info))
		return VERBS_CM_DATA_SIZE - sizeof(struct vrb_xrc_cm_data);
	else
		return VERBS_CM_DATA_SIZE;
}

static int vrb_ep_getopt(fid_t fid, int level, int optname,
			    void *optval, size_t *optlen)
{
	switch (level) {
	case FI_OPT_ENDPOINT:
		switch (optname) {
		case FI_OPT_CM_DATA_SIZE:
			if (*optlen < sizeof(size_t))
				return -FI_ETOOSMALL;
			*((size_t *) optval) = vrb_msg_ep_cmdata_size(fid);
			*optlen = sizeof(size_t);
			return 0;
		default:
			return -FI_ENOPROTOOPT;
		}
	default:
		return -FI_ENOPROTOOPT;
	}
	return 0;
}

static int vrb_ep_setopt(fid_t fid, int level, int optname,
			    const void *optval, size_t optlen)
{
	switch (level) {
	case FI_OPT_ENDPOINT:
		return -FI_ENOPROTOOPT;
	default:
		return -FI_ENOPROTOOPT;
	}
	return 0;
}

static struct fi_ops_ep vrb_ep_base_ops = {
	.size = sizeof(struct fi_ops_ep),
	.cancel = fi_no_cancel,
	.getopt = vrb_ep_getopt,
	.setopt = vrb_ep_setopt,
	.tx_ctx = fi_no_tx_ctx,
	.rx_ctx = fi_no_rx_ctx,
	.rx_size_left = fi_no_rx_size_left,
	.tx_size_left = fi_no_tx_size_left,
};

static struct fi_ops_rma vrb_dgram_rma_ops = {
	.size = sizeof(struct fi_ops_rma),
	.read = fi_no_rma_read,
	.readv = fi_no_rma_readv,
	.readmsg = fi_no_rma_readmsg,
	.write = fi_no_rma_write,
	.writev = fi_no_rma_writev,
	.writemsg = fi_no_rma_writemsg,
	.inject = fi_no_rma_inject,
	.writedata = fi_no_rma_writedata,
	.injectdata = fi_no_rma_injectdata,
};

static int vrb_alloc_wrs(struct vrb_ep *ep)
{
	ep->wrs = calloc(1, sizeof(*ep->wrs));
	if (!ep->wrs)
		return -FI_ENOMEM;

	ep->wrs->msg_wr.wr_id = VERBS_NO_COMP_FLAG;
	ep->wrs->msg_wr.opcode = IBV_WR_SEND;
	ep->wrs->msg_wr.send_flags = IBV_SEND_INLINE;
	ep->wrs->msg_wr.sg_list = &ep->wrs->sge;
	ep->wrs->msg_wr.num_sge = 1;

	ep->wrs->rma_wr.wr_id = VERBS_NO_COMP_FLAG;
	ep->wrs->rma_wr.opcode = IBV_WR_RDMA_WRITE;
	ep->wrs->rma_wr.send_flags = IBV_SEND_INLINE;
	ep->wrs->rma_wr.sg_list = &ep->wrs->sge;
	ep->wrs->rma_wr.num_sge = 1;

	return FI_SUCCESS;
}

static void vrb_free_wrs(struct vrb_ep *ep)
{
	free(ep->wrs);
}

static void vrb_util_ep_progress_noop(struct util_ep *util_ep)
{
	/* This routine shouldn't be called */
	assert(0);
}

static struct vrb_ep *
vrb_alloc_init_ep(struct fi_info *info, struct vrb_domain *domain,
		     void *context)
{
	struct vrb_ep *ep;
	struct vrb_xrc_ep *xrc_ep;
	int ret;

	if (vrb_is_xrc(info)) {
		xrc_ep = calloc(1, sizeof(*xrc_ep));
		if (!xrc_ep)
			return NULL;
		xrc_ep->magic = VERBS_XRC_EP_MAGIC;
		ep = &xrc_ep->base_ep;
	} else {
		ep = calloc(1, sizeof(*ep));
		if (!ep)
			return NULL;
	}

	ep->info = fi_dupinfo(info);
	if (!ep->info)
		goto err1;

	if (domain->util_domain.threading != FI_THREAD_SAFE) {
		if (vrb_alloc_wrs(ep))
			goto err2;
	}

	ret = ofi_endpoint_init(&domain->util_domain.domain_fid, &vrb_util_prov, info,
				&ep->util_ep, context, vrb_util_ep_progress_noop);
	if (ret) {
		VERBS_WARN(FI_LOG_EP_CTRL,
			   "Unable to initialize EP, error - %d\n", ret);
		goto err3;
	}

	ep->util_ep.ep_fid.msg = calloc(1, sizeof(*ep->util_ep.ep_fid.msg));
	if (!ep->util_ep.ep_fid.msg)
		goto err4;

	return ep;
err4:
	(void) ofi_endpoint_close(&ep->util_ep);
err3:
	vrb_free_wrs(ep);
err2:
	fi_freeinfo(ep->info);
err1:
	free(ep);
	return NULL;
}

static int vrb_close_free_ep(struct vrb_ep *ep)
{
	struct vrb_cq *cq;
	int ret;

	free(ep->util_ep.ep_fid.msg);
	ep->util_ep.ep_fid.msg = NULL;
	free(ep->cm_hdr);

	if (ep->util_ep.rx_cq) {
		cq = container_of(ep->util_ep.rx_cq, struct vrb_cq, util_cq);
		cq->util_cq.cq_fastlock_acquire(&cq->util_cq.cq_lock);
		cq->credits += ep->rx_cq_size;
		cq->util_cq.cq_fastlock_release(&cq->util_cq.cq_lock);
	}
	ret = ofi_endpoint_close(&ep->util_ep);
	if (ret)
		return ret;

	vrb_free_wrs(ep);
	fi_freeinfo(ep->info);
	free(ep);

	return 0;
}

/* Caller must hold eq:lock */
static inline void vrb_ep_xrc_close(struct vrb_ep *ep)
{
	struct vrb_xrc_ep *xrc_ep = container_of(ep, struct vrb_xrc_ep,
						    base_ep);

	if (xrc_ep->conn_setup)
		vrb_free_xrc_conn_setup(xrc_ep, 0);

	if (xrc_ep->conn_map_node)
		vrb_eq_remove_sidr_conn(xrc_ep);
	vrb_ep_destroy_xrc_qp(xrc_ep);
	xrc_ep->magic = 0;
}

static int vrb_ep_close(fid_t fid)
{
	int ret;
	struct vrb_fabric *fab;
	struct vrb_ep *ep =
		container_of(fid, struct vrb_ep, util_ep.ep_fid.fid);

	switch (ep->util_ep.type) {
	case FI_EP_MSG:
		if (ep->eq) {
			fastlock_acquire(&ep->eq->lock);
			if (ep->eq->err.err && ep->eq->err.fid == fid) {
				if (ep->eq->err.err_data) {
					free(ep->eq->err.err_data);
					ep->eq->err.err_data = NULL;
					ep->eq->err.err_data_size = 0;
				}
				ep->eq->err.err = 0;
				ep->eq->err.prov_errno = 0;
			}
			vrb_eq_remove_events(ep->eq, fid);
		}

		if (vrb_is_xrc(ep->info))
			vrb_ep_xrc_close(ep);
		else
			rdma_destroy_ep(ep->id);

		if (ep->eq)
			fastlock_release(&ep->eq->lock);
		vrb_cleanup_cq(ep);
		break;
	case FI_EP_DGRAM:
		fab = container_of(&ep->util_ep.domain->fabric->fabric_fid,
				   struct vrb_fabric, util_fabric.fabric_fid.fid);
		ofi_ns_del_local_name(&fab->name_server,
				      &ep->service, &ep->ep_name);
		ret = ibv_destroy_qp(ep->ibv_qp);
		if (ret) {
			VERBS_WARN(FI_LOG_EP_CTRL,
				   "Unable to destroy QP (errno = %d)\n", errno);
			return -errno;
		}
		vrb_cleanup_cq(ep);
		break;
	default:
		VERBS_INFO(FI_LOG_DOMAIN, "Unknown EP type\n");
		assert(0);
		return -FI_EINVAL;
	}

	VERBS_INFO(FI_LOG_DOMAIN, "EP %p is being closed\n", ep);

	ret = vrb_close_free_ep(ep);
	if (ret) {
		VERBS_WARN(FI_LOG_DOMAIN,
			   "Unable to close EP (%p), error - %d\n", ep, ret);
		return ret;
	}

	return 0;
}

static inline int vrb_ep_xrc_set_tgt_chan(struct vrb_ep *ep)
{
	struct vrb_xrc_ep *xrc_ep = container_of(ep, struct vrb_xrc_ep,
						    base_ep);
	if (xrc_ep->tgt_id)
		return rdma_migrate_id(xrc_ep->tgt_id, ep->eq->channel);

	return FI_SUCCESS;
}

static int vrb_ep_bind(struct fid *fid, struct fid *bfid, uint64_t flags)
{
	struct vrb_ep *ep;
	struct vrb_cq *cq =
		container_of(bfid, struct vrb_cq, util_cq.cq_fid.fid);
	struct vrb_dgram_av *av;
	int ret;

	ep = container_of(fid, struct vrb_ep, util_ep.ep_fid.fid);
	ret = ofi_ep_bind_valid(&vrb_prov, bfid, flags);
	if (ret)
		return ret;

	switch (bfid->fclass) {
	case FI_CLASS_CQ:
		/* Reserve space for receives */
		if (flags & FI_RECV) {
			cq->util_cq.cq_fastlock_acquire(&cq->util_cq.cq_lock);
			if (cq->credits < ep->rx_cq_size) {
				VERBS_WARN(FI_LOG_DOMAIN,
					   "Rx CQ is fully reserved\n");
				ep->rx_cq_size = 0;
			} 
			cq->credits -= ep->rx_cq_size;
			cq->util_cq.cq_fastlock_release(&cq->util_cq.cq_lock);
		}

		ret = ofi_ep_bind_cq(&ep->util_ep, &cq->util_cq, flags);
		if (ret) {
			cq->util_cq.cq_fastlock_acquire(&cq->util_cq.cq_lock);
			cq->credits += ep->rx_cq_size;
			cq->util_cq.cq_fastlock_release(&cq->util_cq.cq_lock);
			return ret;
		}
		break;
	case FI_CLASS_EQ:
		if (ep->util_ep.type != FI_EP_MSG)
			return -FI_EINVAL;

		ep->eq = container_of(bfid, struct vrb_eq, eq_fid.fid);

		/* Make sure EQ channel is not polled during migrate */
		fastlock_acquire(&ep->eq->lock);
		if (vrb_is_xrc(ep->info))
			ret = vrb_ep_xrc_set_tgt_chan(ep);
		else
			ret = rdma_migrate_id(ep->id, ep->eq->channel);
		fastlock_release(&ep->eq->lock);
		if (ret)
			return -errno;

		break;
	case FI_CLASS_SRX_CTX:
		if (ep->util_ep.type != FI_EP_MSG)
			return -FI_EINVAL;

		ep->srq_ep = container_of(bfid, struct vrb_srq_ep, ep_fid.fid);
		break;
	case FI_CLASS_AV:
		if (ep->util_ep.type != FI_EP_DGRAM)
			return -FI_EINVAL;

		av = container_of(bfid, struct vrb_dgram_av,
				  util_av.av_fid.fid);
		return ofi_ep_bind_av(&ep->util_ep, &av->util_av);
	default:
		return -FI_EINVAL;
	}

	return 0;
}

static int vrb_create_dgram_ep(struct vrb_domain *domain, struct vrb_ep *ep,
				  struct ibv_qp_init_attr *init_attr)
{
	struct vrb_fabric *fab;
	struct ibv_qp_attr attr = {
		.qp_state = IBV_QPS_INIT,
		.pkey_index = 0,
		.port_num = 1,
		.qkey = 0x11111111,
	};
	int ret = 0;
	union ibv_gid gid;
	uint16_t p_key;
	struct ibv_port_attr port_attr;

	init_attr->qp_type = IBV_QPT_UD;

	ep->ibv_qp = ibv_create_qp(domain->pd, init_attr);
	if (!ep->ibv_qp) {
		VERBS_WARN(FI_LOG_EP_CTRL, "Unable to create IBV "
			   "Queue Pair\n");
		return -errno;
	}

	ret = ibv_modify_qp(ep->ibv_qp, &attr,
			    IBV_QP_STATE |
			    IBV_QP_PKEY_INDEX |
			    IBV_QP_PORT |
			    IBV_QP_QKEY);
	if (ret) {
		VERBS_WARN(FI_LOG_EP_CTRL, "Unable to modify QP state "
			   "to INIT\n");
		return -errno;
	}

	memset(&attr, 0, sizeof(attr));
	attr.qp_state = IBV_QPS_RTR;
	ret = ibv_modify_qp(ep->ibv_qp, &attr,
			    IBV_QP_STATE);
	if (ret) {
		VERBS_WARN(FI_LOG_EP_CTRL, "Unable to modify QP state "
			   "to RTR\n");
		return -errno;
	}

	if (ep->util_ep.tx_cq) {
		memset(&attr, 0, sizeof(attr));
		attr.qp_state = IBV_QPS_RTS;
		attr.sq_psn = 0xffffff;
		ret = ibv_modify_qp(ep->ibv_qp, &attr,
				    IBV_QP_STATE |
				    IBV_QP_SQ_PSN);
		if (ret) {
			VERBS_WARN(FI_LOG_EP_CTRL, "Unable to modify QP state "
				   "to RTS\n");
			return -errno;
		}
	}

	if (ibv_query_gid(domain->verbs, 1, vrb_gl_data.gid_idx, &gid)) {
		VERBS_WARN(FI_LOG_EP_CTRL,
			   "Unable to query GID, errno = %d",
			   errno);
		return -errno;
	}

	if (ibv_query_pkey(domain->verbs, 1, 0, &p_key)) {
		VERBS_WARN(FI_LOG_EP_CTRL,
			   "Unable to query P_Key, errno = %d",
			   errno);
		return -errno;
	}

	if (ibv_query_port(domain->verbs, 1, &port_attr)) {
		VERBS_WARN(FI_LOG_EP_CTRL,
			   "Unable to query port attributes, errno = %d",
			   errno);
		return -errno;
	}

	ep->ep_name.lid = port_attr.lid;
	ep->ep_name.sl = port_attr.sm_sl;
	ep->ep_name.gid = gid;
	ep->ep_name.qpn = ep->ibv_qp->qp_num;
	ep->ep_name.pkey = p_key;

	fab = container_of(ep->util_ep.domain->fabric,
			   struct vrb_fabric, util_fabric);

	ofi_ns_add_local_name(&fab->name_server,
			      &ep->service, &ep->ep_name);

	return 0;
}

/* vrb_srq_ep::xrc.prepost_lock must be held */
FI_VERBS_XRC_ONLY
static int vrb_process_xrc_preposted(struct vrb_srq_ep *srq_ep)
{
	struct vrb_xrc_srx_prepost *recv;
	struct slist_entry *entry;
	int ret;

	/* The pre-post SRQ function ops have been replaced so the
	 * posting here results in adding the RX entries to the SRQ */
	while (!slist_empty(&srq_ep->xrc.prepost_list)) {
		entry = slist_remove_head(&srq_ep->xrc.prepost_list);
		recv = container_of(entry, struct vrb_xrc_srx_prepost,
				    prepost_entry);
		ret = fi_recv(&srq_ep->ep_fid, recv->buf, recv->len,
			      recv->desc, recv->src_addr, recv->context);
		free(recv);
		if (ret) {
			VERBS_INFO_ERRNO(FI_LOG_DOMAIN, "fi_recv", errno);
			return -errno;
		}
	}
	return FI_SUCCESS;
}

static int vrb_ep_enable_xrc(struct vrb_ep *ep)
{
#if VERBS_HAVE_XRC
	struct vrb_xrc_ep *xrc_ep = container_of(ep, struct vrb_xrc_ep,
						    base_ep);
	struct vrb_srq_ep *srq_ep = ep->srq_ep;
	struct vrb_domain *domain = container_of(ep->util_ep.rx_cq->domain,
					    struct vrb_domain, util_domain);
	struct vrb_cq *cq = container_of(ep->util_ep.rx_cq,
					    struct vrb_cq, util_cq);
	struct ibv_srq_init_attr_ex attr;
	ssize_t ret;

	/* XRC EP additional initialization */
	dlist_init(&xrc_ep->ini_conn_entry);
	xrc_ep->conn_state = VRB_XRC_UNCONNECTED;

	fastlock_acquire(&srq_ep->xrc.prepost_lock);
	if (srq_ep->srq) {
		/*
		 * Multiple endpoints bound to the same XRC SRX context have
		 * the restriction that they must be bound to the same RX CQ
		 */
		if (!srq_ep->xrc.cq || srq_ep->xrc.cq != cq) {
			fastlock_release(&srq_ep->xrc.prepost_lock);
			VERBS_WARN(FI_LOG_EP_CTRL, "SRX_CTX/CQ mismatch\n");
			return -FI_EINVAL;
		}
		ibv_get_srq_num(srq_ep->srq, &xrc_ep->srqn);
		ret = FI_SUCCESS;
		goto done;
	}

	memset(&attr, 0, sizeof(attr));
	attr.attr.max_wr = srq_ep->xrc.max_recv_wr;
	attr.attr.max_sge = srq_ep->xrc.max_sge;
	attr.comp_mask = IBV_SRQ_INIT_ATTR_TYPE | IBV_SRQ_INIT_ATTR_XRCD |
			 IBV_SRQ_INIT_ATTR_CQ | IBV_SRQ_INIT_ATTR_PD;
	attr.srq_type = IBV_SRQT_XRC;
	attr.xrcd = domain->xrc.xrcd;
	attr.cq = cq->cq;
	attr.pd = domain->pd;

	srq_ep->srq = ibv_create_srq_ex(domain->verbs, &attr);
	if (!srq_ep->srq) {
		VERBS_INFO_ERRNO(FI_LOG_DOMAIN, "ibv_create_srq_ex", errno);
		ret = -errno;
		goto done;
	}
	/* The RX CQ maintains a list of all the XRC SRQs that were created
	 * using it as the CQ */
	cq->util_cq.cq_fastlock_acquire(&cq->xrc.srq_list_lock);
	dlist_insert_tail(&srq_ep->xrc.srq_entry, &cq->xrc.srq_list);
	srq_ep->xrc.cq = cq;
	cq->util_cq.cq_fastlock_release(&cq->xrc.srq_list_lock);

	ibv_get_srq_num(srq_ep->srq, &xrc_ep->srqn);

	/* Swap functions since locking is no longer required */
	srq_ep->ep_fid.msg = &vrb_srq_msg_ops;
	ret = vrb_process_xrc_preposted(srq_ep);
done:
	fastlock_release(&srq_ep->xrc.prepost_lock);

	return ret;
#else /* VERBS_HAVE_XRC */
	return -FI_ENOSYS;
#endif /* !VERBS_HAVE_XRC */
}

void vrb_msg_ep_get_qp_attr(struct vrb_ep *ep,
			       struct ibv_qp_init_attr *attr)
{
	attr->qp_context = ep;

	if (ep->util_ep.tx_cq) {
		struct vrb_cq *cq = container_of(ep->util_ep.tx_cq,
						    struct vrb_cq, util_cq);

		attr->cap.max_send_wr = ep->info->tx_attr->size;
		attr->cap.max_send_sge = ep->info->tx_attr->iov_limit;
		attr->send_cq = cq->cq;
	} else {
		struct vrb_cq *cq =
			container_of(ep->util_ep.rx_cq, struct vrb_cq, util_cq);

		attr->send_cq = cq->cq;
	}

	if (ep->util_ep.rx_cq) {
		struct vrb_cq *cq =
			container_of(ep->util_ep.rx_cq, struct vrb_cq, util_cq);

		attr->cap.max_recv_wr = ep->info->rx_attr->size;
		attr->cap.max_recv_sge = ep->info->rx_attr->iov_limit;
		attr->recv_cq = cq->cq;
	} else {
		struct vrb_cq *cq =
			container_of(ep->util_ep.tx_cq, struct vrb_cq, util_cq);

		attr->recv_cq = cq->cq;
	}
	attr->cap.max_inline_data = ep->info->tx_attr->inject_size;
	attr->qp_type = IBV_QPT_RC;
	attr->sq_sig_all = 1;

	if (ep->srq_ep) {
		attr->srq = ep->srq_ep->srq;
		/* Recieve posts are done to SRQ not QP RQ */
		attr->cap.max_recv_wr = 0;
	}
}


static int vrb_ep_enable(struct fid_ep *ep_fid)
{
	struct ibv_qp_init_attr attr = { 0 };
	struct vrb_ep *ep = container_of(ep_fid, struct vrb_ep,
					    util_ep.ep_fid);
	struct vrb_domain *domain = vrb_ep_to_domain(ep);
	int ret;

	if (!ep->eq && (ep->util_ep.type == FI_EP_MSG)) {
		VERBS_WARN(FI_LOG_EP_CTRL,
			   "Endpoint is not bound to an event queue\n");
		return -FI_ENOEQ;
	}

	if (!ep->util_ep.tx_cq && !ep->util_ep.rx_cq) {
		VERBS_WARN(FI_LOG_EP_CTRL, "Endpoint is not bound to "
			   "a send or receive completion queue\n");
		return -FI_ENOCQ;
	}

	if (!ep->util_ep.tx_cq && (ofi_send_allowed(ep->util_ep.caps) ||
				ofi_rma_initiate_allowed(ep->util_ep.caps))) {
		VERBS_WARN(FI_LOG_EP_CTRL, "Endpoint is not bound to "
			   "a send completion queue when it has transmit "
			   "capabilities enabled (FI_SEND | FI_RMA).\n");
		return -FI_ENOCQ;
	}

	if (!ep->util_ep.rx_cq && ofi_recv_allowed(ep->util_ep.caps)) {
		VERBS_WARN(FI_LOG_EP_CTRL, "Endpoint is not bound to "
			   "a receive completion queue when it has receive "
			   "capabilities enabled. (FI_RECV)\n");
		return -FI_ENOCQ;
	}
	vrb_msg_ep_get_qp_attr(ep, &attr);

	switch (ep->util_ep.type) {
	case FI_EP_MSG:
		if (ep->srq_ep) {
			/* Override receive function pointers to prevent the user from
			 * posting Receive WRs to a QP where a SRQ is attached to it */
			if (domain->flags & VRB_USE_XRC) {
				*ep->util_ep.ep_fid.msg = vrb_msg_srq_xrc_ep_msg_ops;
				return vrb_ep_enable_xrc(ep);
			} else {
				ep->util_ep.ep_fid.msg->recv = fi_no_msg_recv;
				ep->util_ep.ep_fid.msg->recvv = fi_no_msg_recvv;
				ep->util_ep.ep_fid.msg->recvmsg = fi_no_msg_recvmsg;
			}
		} else if (domain->flags & VRB_USE_XRC) {
			VERBS_WARN(FI_LOG_EP_CTRL, "XRC EP_MSG not bound "
				   "to srx_context\n");
			return -FI_EINVAL;
		}

		ret = rdma_create_qp(ep->id, domain->pd, &attr);
		if (ret) {
			ret = -errno;
			VERBS_WARN(FI_LOG_EP_CTRL,
				   "Unable to create rdma qp: %s (%d)\n",
				   fi_strerror(-ret), -ret);
			return ret;
		}

		/* Allow shared XRC INI QP not controlled by RDMA CM
		 * to share same post functions as RC QP. */
		ep->ibv_qp = ep->id->qp;
		break;
	case FI_EP_DGRAM:
		assert(domain);
		attr.sq_sig_all = 1;
		ret = vrb_create_dgram_ep(domain, ep, &attr);
		if (ret) {
			VERBS_WARN(FI_LOG_EP_CTRL, "Unable to create dgram EP: %s (%d)\n",
				   fi_strerror(-ret), -ret);
			return ret;
		}
		break;
	default:
		VERBS_INFO(FI_LOG_DOMAIN, "Unknown EP type\n");
		assert(0);
		return -FI_EINVAL;
	}
	return 0;
}

static int vrb_ep_control(struct fid *fid, int command, void *arg)
{
	struct fid_ep *ep;

	switch (fid->fclass) {
	case FI_CLASS_EP:
		ep = container_of(fid, struct fid_ep, fid);
		switch (command) {
		case FI_ENABLE:
			return vrb_ep_enable(ep);
			break;
		default:
			return -FI_ENOSYS;
		}
		break;
	default:
		return -FI_ENOSYS;
	}
}

static int vrb_dgram_ep_setname(fid_t ep_fid, void *addr, size_t addrlen)
{
	struct vrb_ep *ep;
	void *save_addr;
	int ret = FI_SUCCESS;

	ep = container_of(ep_fid, struct vrb_ep, util_ep.ep_fid.fid);
	if (addrlen < ep->info->src_addrlen) {
		VERBS_INFO(FI_LOG_EP_CTRL,
			   "addrlen expected: %zu, got: %zu\n",
			   ep->info->src_addrlen, addrlen);
		return -FI_ETOOSMALL;
	}
	/*
	 * save previous address to be able make
	 * a roll back on the previous one
	 */
	save_addr = ep->info->src_addr;

	ep->info->src_addr = calloc(1, ep->info->src_addrlen);
	if (!ep->info->src_addr) {
		ep->info->src_addr = save_addr;
		ret = -FI_ENOMEM;
		goto err;
	}

	memcpy(ep->info->src_addr, addr, ep->info->src_addrlen);
	memcpy(&ep->ep_name, addr, ep->info->src_addrlen);

err:
	ep->info->src_addr = save_addr;
	return ret;
}

static int vrb_dgram_ep_getname(fid_t ep_fid, void *addr, size_t *addrlen)
{
	struct vrb_ep *ep;

	ep = container_of(ep_fid, struct vrb_ep, util_ep.ep_fid.fid);
	if (*addrlen < sizeof(ep->ep_name)) {
		*addrlen = sizeof(ep->ep_name);
		VERBS_INFO(FI_LOG_EP_CTRL,
			   "addrlen expected: %zu, got: %zu\n",
			   sizeof(ep->ep_name), *addrlen);
		return -FI_ETOOSMALL;
	}

	memset(addr, 0, *addrlen);
	memcpy(addr, &ep->ep_name, sizeof(ep->ep_name));
	*addrlen = sizeof(ep->ep_name);

	return FI_SUCCESS;
}

static struct fi_ops vrb_ep_ops = {
	.size = sizeof(struct fi_ops),
	.close = vrb_ep_close,
	.bind = vrb_ep_bind,
	.control = vrb_ep_control,
	.ops_open = fi_no_ops_open,
};

static struct fi_ops_cm vrb_dgram_cm_ops = {
	.size = sizeof(vrb_dgram_cm_ops),
	.setname = vrb_dgram_ep_setname,
	.getname = vrb_dgram_ep_getname,
	.getpeer = fi_no_getpeer,
	.connect = fi_no_connect,
	.listen = fi_no_listen,
	.accept = fi_no_accept,
	.reject = fi_no_reject,
	.shutdown = fi_no_shutdown,
	.join = fi_no_join,
};

int vrb_open_ep(struct fid_domain *domain, struct fi_info *info,
		   struct fid_ep **ep_fid, void *context)
{
	struct vrb_domain *dom;
	struct vrb_ep *ep;
	struct vrb_connreq *connreq;
	struct vrb_pep *pep;
	struct fi_info *fi;
	int ret;

	if (info->src_addr)
		ofi_straddr_dbg(&vrb_prov, FI_LOG_FABRIC,
				"open_ep src addr", info->src_addr);
	if (info->dest_addr)
		ofi_straddr_dbg(&vrb_prov, FI_LOG_FABRIC,
				"open_ep dest addr", info->dest_addr);

	dom = container_of(domain, struct vrb_domain,
			   util_domain.domain_fid);
	/* strncmp is used here, because the function is used
	 * to allocate DGRAM (has prefix <dev_name>-dgram) and MSG EPs */
	if (strncmp(dom->verbs->device->name, info->domain_attr->name,
		    strlen(dom->verbs->device->name))) {
		VERBS_INFO(FI_LOG_DOMAIN,
			   "Invalid info->domain_attr->name: %s and %s\n",
			   dom->verbs->device->name, info->domain_attr->name);
		return -FI_EINVAL;
	}

	fi = dom->info;

	if (info->ep_attr) {
		ret = vrb_check_ep_attr(info, fi);
		if (ret)
			return ret;
	}

	if (info->tx_attr) {
		ret = ofi_check_tx_attr(&vrb_prov, fi->tx_attr,
					info->tx_attr, info->mode);
		if (ret)
			return ret;
	}

	if (info->rx_attr) {
		ret = vrb_check_rx_attr(info->rx_attr, info, fi);
		if (ret)
			return ret;
	}

	ep = vrb_alloc_init_ep(info, dom, context);
	if (!ep) {
		VERBS_WARN(FI_LOG_EP_CTRL,
			   "Unable to allocate/init EP memory\n");
		return -FI_ENOMEM;
	}

	ep->inject_limit = ep->info->tx_attr->inject_size;

	switch (info->ep_attr->type) {
	case FI_EP_MSG:
		if (dom->flags & VRB_USE_XRC) {
			if (dom->util_domain.threading == FI_THREAD_SAFE) {
				*ep->util_ep.ep_fid.msg = vrb_msg_xrc_ep_msg_ops_ts;
				ep->util_ep.ep_fid.rma = &vrb_msg_xrc_ep_rma_ops_ts;
			} else {
				*ep->util_ep.ep_fid.msg = vrb_msg_xrc_ep_msg_ops;
				ep->util_ep.ep_fid.rma = &vrb_msg_xrc_ep_rma_ops;
			}
			ep->util_ep.ep_fid.cm = &vrb_msg_xrc_ep_cm_ops;
			ep->util_ep.ep_fid.atomic = &vrb_msg_xrc_ep_atomic_ops;
		} else {
			if (dom->util_domain.threading == FI_THREAD_SAFE) {
				*ep->util_ep.ep_fid.msg = vrb_msg_ep_msg_ops_ts;
				ep->util_ep.ep_fid.rma = &vrb_msg_ep_rma_ops_ts;
			} else {
				*ep->util_ep.ep_fid.msg = vrb_msg_ep_msg_ops;
				ep->util_ep.ep_fid.rma = &vrb_msg_ep_rma_ops;
			}
			ep->util_ep.ep_fid.cm = &vrb_msg_ep_cm_ops;
			ep->util_ep.ep_fid.atomic = &vrb_msg_ep_atomic_ops;
		}

		if (!info->handle) {
			/* Only RC, XRC active RDMA CM ID is created at connect */
			if (!(dom->flags & VRB_USE_XRC)) {
				ret = vrb_create_ep(info, RDMA_PS_TCP,
						       &ep->id);
				if (ret)
					goto err1;
				ep->id->context = &ep->util_ep.ep_fid.fid;
			}
		} else if (info->handle->fclass == FI_CLASS_CONNREQ) {
			connreq = container_of(info->handle,
					       struct vrb_connreq, handle);
			if (dom->flags & VRB_USE_XRC) {
				assert(connreq->is_xrc);

				if (!connreq->xrc.is_reciprocal) {
					ret = vrb_process_xrc_connreq(ep,
								connreq);
					if (ret)
						goto err1;
				}
			} else {
				ep->id = connreq->id;
				ep->ibv_qp = ep->id->qp;
				ep->id->context = &ep->util_ep.ep_fid.fid;
			}
		} else if (info->handle->fclass == FI_CLASS_PEP) {
			pep = container_of(info->handle, struct vrb_pep, pep_fid.fid);
			ep->id = pep->id;
			ep->ibv_qp = ep->id->qp;
			pep->id = NULL;

			if (rdma_resolve_addr(ep->id, info->src_addr, info->dest_addr,
					      VERBS_RESOLVE_TIMEOUT)) {
				ret = -errno;
				VERBS_INFO(FI_LOG_DOMAIN, "Unable to rdma_resolve_addr\n");
				goto err2;
			}
			ep->id->context = &ep->util_ep.ep_fid.fid;
		} else {
			ret = -FI_ENOSYS;
			goto err1;
		}
		break;
	case FI_EP_DGRAM:
		ep->service = (info->src_addr) ?
			(((struct ofi_ib_ud_ep_name *)info->src_addr)->service) :
			(((getpid() & 0x7FFF) << 16) + ((uintptr_t)ep & 0xFFFF));

		if (dom->util_domain.threading == FI_THREAD_SAFE) {
			*ep->util_ep.ep_fid.msg = vrb_dgram_msg_ops_ts;
		} else {
			*ep->util_ep.ep_fid.msg = vrb_dgram_msg_ops;
		}
		ep->util_ep.ep_fid.rma = &vrb_dgram_rma_ops;
		ep->util_ep.ep_fid.cm = &vrb_dgram_cm_ops;
		break;
	default:
		VERBS_INFO(FI_LOG_DOMAIN, "Unknown EP type\n");
		ret = -FI_EINVAL;
		assert(0);
		goto err1;
	}

	if (info->ep_attr->rx_ctx_cnt == 0 || 
	    info->ep_attr->rx_ctx_cnt == 1)
		ep->rx_cq_size = info->rx_attr->size;
	
	if (info->ep_attr->tx_ctx_cnt == 0 || 
	    info->ep_attr->tx_ctx_cnt == 1)
		ep->tx_credits = info->tx_attr->size;

	*ep_fid = &ep->util_ep.ep_fid;
	ep->util_ep.ep_fid.fid.ops = &vrb_ep_ops;
	ep->util_ep.ep_fid.ops = &vrb_ep_base_ops;

	return FI_SUCCESS;
err2:
	ep->ibv_qp = NULL;
	if (ep->id)
		rdma_destroy_ep(ep->id);
err1:
	vrb_close_free_ep(ep);
	return ret;
}

static int vrb_pep_bind(fid_t fid, struct fid *bfid, uint64_t flags)
{
	struct vrb_pep *pep;
	int ret;

	pep = container_of(fid, struct vrb_pep, pep_fid.fid);
	if (bfid->fclass != FI_CLASS_EQ)
		return -FI_EINVAL;

	pep->eq = container_of(bfid, struct vrb_eq, eq_fid.fid);
	/*
	 * This is a restrictive solution that enables an XRC EP to
	 * inform it's peer the port that should be used in making the
	 * reciprocal connection request. While it meets RXM requirements
	 * it limits an EQ to a single passive endpoint. TODO: implement
	 * a more general solution.
	 */
	if (vrb_is_xrc(pep->info)) {
	       if (pep->eq->xrc.pep_port) {
			VERBS_WARN(FI_LOG_EP_CTRL,
				   "XRC limits EQ binding to a single PEP\n");
			return -FI_EINVAL;
	       }
	       pep->eq->xrc.pep_port = ntohs(rdma_get_src_port(pep->id));
	}

	ret = rdma_migrate_id(pep->id, pep->eq->channel);
	if (ret)
		return -errno;

	if (vrb_is_xrc(pep->info)) {
		ret = rdma_migrate_id(pep->xrc_ps_udp_id, pep->eq->channel);
		if (ret)
			return -errno;
	}
	return FI_SUCCESS;
}

static int vrb_pep_control(struct fid *fid, int command, void *arg)
{
	struct vrb_pep *pep;
	int ret = 0;

	switch (fid->fclass) {
	case FI_CLASS_PEP:
		pep = container_of(fid, struct vrb_pep, pep_fid.fid);
		switch (command) {
		case FI_BACKLOG:
			if (!arg)
				return -FI_EINVAL;
			pep->backlog = *(int *) arg;
			break;
		default:
			ret = -FI_ENOSYS;
			break;
		}
		break;
	default:
		ret = -FI_ENOSYS;
		break;
	}

	return ret;
}

static int vrb_pep_close(fid_t fid)
{
	struct vrb_pep *pep;

	pep = container_of(fid, struct vrb_pep, pep_fid.fid);
	if (pep->id)
		rdma_destroy_ep(pep->id);
	if (pep->xrc_ps_udp_id)
		rdma_destroy_ep(pep->xrc_ps_udp_id);

	fi_freeinfo(pep->info);
	free(pep);
	return 0;
}

static struct fi_ops vrb_pep_fi_ops = {
	.size = sizeof(struct fi_ops),
	.close = vrb_pep_close,
	.bind = vrb_pep_bind,
	.control = vrb_pep_control,
	.ops_open = fi_no_ops_open,
};

static struct fi_ops_ep vrb_pep_ops = {
	.size = sizeof(struct fi_ops_ep),
	.getopt = vrb_ep_getopt,
	.setopt = fi_no_setopt,
	.tx_ctx = fi_no_tx_ctx,
	.rx_ctx = fi_no_rx_ctx,
	.rx_size_left = fi_no_rx_size_left,
	.tx_size_left = fi_no_tx_size_left,
};

int vrb_passive_ep(struct fid_fabric *fabric, struct fi_info *info,
		      struct fid_pep **pep, void *context)
{
	struct vrb_pep *_pep;
	int ret;

	_pep = calloc(1, sizeof *_pep);
	if (!_pep)
		return -FI_ENOMEM;

	if (!(_pep->info = fi_dupinfo(info))) {
		ret = -FI_ENOMEM;
		goto err1;
	}

	if (_pep->info->dest_addr || _pep->info->dest_addrlen) {
		free(_pep->info->dest_addr);
		_pep->info->dest_addr = NULL;
		_pep->info->dest_addrlen = 0;
	}

	ret = rdma_create_id(NULL, &_pep->id, &_pep->pep_fid.fid, RDMA_PS_TCP);
	if (ret) {
		VERBS_INFO(FI_LOG_DOMAIN, "Unable to create PEP rdma_cm_id\n");
		goto err2;
	}

	if (info->src_addr) {
		ret = rdma_bind_addr(_pep->id, (struct sockaddr *)info->src_addr);
		if (ret) {
			VERBS_INFO(FI_LOG_DOMAIN, "Unable to bind address to rdma_cm_id\n");
			goto err3;
		}
		_pep->bound = 1;
	}

	/* XRC listens on both RDMA_PS_TCP and RDMA_PS_UDP */
	if (vrb_is_xrc(info)) {
		ret = rdma_create_id(NULL, &_pep->xrc_ps_udp_id,
				     &_pep->pep_fid.fid, RDMA_PS_UDP);
		if (ret) {
			VERBS_INFO(FI_LOG_DOMAIN,
				   "Unable to create PEP PS_UDP rdma_cm_id\n");
			goto err3;
		}
		/* Currently both listens must be bound to same port number */
		ofi_addr_set_port(_pep->info->src_addr,
				  ntohs(rdma_get_src_port(_pep->id)));
		ret = rdma_bind_addr(_pep->xrc_ps_udp_id,
				     (struct sockaddr *)_pep->info->src_addr);
		if (ret) {
			VERBS_INFO(FI_LOG_DOMAIN,
				   "Unable to bind address to PS_UDP rdma_cm_id\n");
			goto err4;
		}
	}

	_pep->pep_fid.fid.fclass = FI_CLASS_PEP;
	_pep->pep_fid.fid.context = context;
	_pep->pep_fid.fid.ops = &vrb_pep_fi_ops;
	_pep->pep_fid.ops = &vrb_pep_ops;
	_pep->pep_fid.cm = vrb_pep_ops_cm(_pep);

	_pep->src_addrlen = info->src_addrlen;

	*pep = &_pep->pep_fid;
	return 0;

err4:
	/* Only possible for XRC code path */
	rdma_destroy_id(_pep->xrc_ps_udp_id);
err3:
	rdma_destroy_id(_pep->id);
err2:
	fi_freeinfo(_pep->info);
err1:
	free(_pep);
	return ret;
}

static struct fi_ops_ep vrb_srq_ep_base_ops = {
	.size = sizeof(struct fi_ops_ep),
	.cancel = fi_no_cancel,
	.getopt = fi_no_getopt,
	.setopt = fi_no_setopt,
	.tx_ctx = fi_no_tx_ctx,
	.rx_ctx = fi_no_rx_ctx,
	.rx_size_left = fi_no_rx_size_left,
	.tx_size_left = fi_no_tx_size_left,
};

static struct fi_ops_cm vrb_srq_cm_ops = {
	.size = sizeof(struct fi_ops_cm),
	.setname = fi_no_setname,
	.getname = fi_no_getname,
	.getpeer = fi_no_getpeer,
	.connect = fi_no_connect,
	.listen = fi_no_listen,
	.accept = fi_no_accept,
	.reject = fi_no_reject,
	.shutdown = fi_no_shutdown,
	.join = fi_no_join,
};

static struct fi_ops_rma vrb_srq_rma_ops = {
	.size = sizeof(struct fi_ops_rma),
	.read = fi_no_rma_read,
	.readv = fi_no_rma_readv,
	.readmsg = fi_no_rma_readmsg,
	.write = fi_no_rma_write,
	.writev = fi_no_rma_writev,
	.writemsg = fi_no_rma_writemsg,
	.inject = fi_no_rma_inject,
	.writedata = fi_no_rma_writedata,
	.injectdata = fi_no_rma_injectdata,
};

static struct fi_ops_atomic vrb_srq_atomic_ops = {
	.size = sizeof(struct fi_ops_atomic),
	.write = fi_no_atomic_write,
	.writev = fi_no_atomic_writev,
	.writemsg = fi_no_atomic_writemsg,
	.inject = fi_no_atomic_inject,
	.readwrite = fi_no_atomic_readwrite,
	.readwritev = fi_no_atomic_readwritev,
	.readwritemsg = fi_no_atomic_readwritemsg,
	.compwrite = fi_no_atomic_compwrite,
	.compwritev = fi_no_atomic_compwritev,
	.compwritemsg = fi_no_atomic_compwritemsg,
	.writevalid = fi_no_atomic_writevalid,
	.readwritevalid = fi_no_atomic_readwritevalid,
	.compwritevalid = fi_no_atomic_compwritevalid,
};

/* Receive CQ credits are pre-allocated */
ssize_t vrb_post_srq(struct vrb_srq_ep *ep, struct ibv_recv_wr *wr)
{
	struct vrb_context *ctx;
	struct ibv_recv_wr *bad_wr;
	int ret;

	fastlock_acquire(&ep->ctx_lock);
	ctx = ofi_buf_alloc(ep->ctx_pool);
	if (!ctx)
		goto unlock;

	ctx->srx = ep;
	ctx->user_ctx = (void *) (uintptr_t) wr->wr_id;
	ctx->flags = FI_RECV;
	wr->wr_id = (uintptr_t) ctx;

	ret = ibv_post_srq_recv(ep->srq, wr, &bad_wr);
	wr->wr_id = (uintptr_t) ctx->user_ctx;
	if (ret)
		goto freebuf;
	fastlock_release(&ep->ctx_lock);
	return 0;

freebuf:
	ofi_buf_free(ctx);
unlock:
	fastlock_release(&ep->ctx_lock);
	return -FI_EAGAIN;
}

static inline ssize_t
vrb_srq_ep_recvmsg(struct fid_ep *ep_fid, const struct fi_msg *msg, uint64_t flags)
{
	struct vrb_srq_ep *ep = container_of(ep_fid, struct vrb_srq_ep, ep_fid);
	struct ibv_recv_wr wr = {
		.wr_id = (uintptr_t )msg->context,
		.num_sge = msg->iov_count,
		.next = NULL,
	};

	vrb_set_sge_iov(wr.sg_list, msg->msg_iov, msg->iov_count, msg->desc);
	return vrb_post_srq(ep, &wr);
}

static ssize_t
vrb_srq_ep_recv(struct fid_ep *ep_fid, void *buf, size_t len,
		void *desc, fi_addr_t src_addr, void *context)
{
	struct vrb_srq_ep *ep = container_of(ep_fid, struct vrb_srq_ep, ep_fid);
	struct ibv_sge sge = vrb_init_sge(buf, len, desc);
	struct ibv_recv_wr wr = {
		.wr_id = (uintptr_t) context,
		.num_sge = 1,
		.sg_list = &sge,
		.next = NULL,
	};

	return vrb_post_srq(ep, &wr);
}

static ssize_t
vrb_srq_ep_recvv(struct fid_ep *ep_fid, const struct iovec *iov, void **desc,
		    size_t count, fi_addr_t src_addr, void *context)
{
	struct fi_msg msg = {
		.msg_iov = iov,
		.desc = desc,
		.iov_count = count,
		.addr = src_addr,
		.context = context,
	};

	return vrb_srq_ep_recvmsg(ep_fid, &msg, 0);
}

static struct fi_ops_msg vrb_srq_msg_ops = {
	.size = sizeof(struct fi_ops_msg),
	.recv = vrb_srq_ep_recv,
	.recvv = vrb_srq_ep_recvv,
	.recvmsg = vrb_srq_ep_recvmsg,
	.send = fi_no_msg_send,
	.sendv = fi_no_msg_sendv,
	.sendmsg = fi_no_msg_sendmsg,
	.inject = fi_no_msg_inject,
	.senddata = fi_no_msg_senddata,
	.injectdata = fi_no_msg_injectdata,
};

/*
 * XRC SRQ semantics differ from basic SRQ semantics in that the SRQ not the
 * QP selects which CQ will be used for receive completions. An artifact of
 * this is that the XRC SRQ can not be created until a CQ is bound to the
 * endpoint. This routine will be swapped out when the first endpoint bound
 * to the shared receive context is enabled.
 */
static ssize_t
vrb_xrc_srq_ep_prepost_recv(struct fid_ep *ep_fid, void *buf, size_t len,
			void *desc, fi_addr_t src_addr, void *context)
{
	struct vrb_srq_ep *ep =
		container_of(ep_fid, struct vrb_srq_ep, ep_fid);
	struct vrb_xrc_srx_prepost *recv;
	ssize_t ret;

	fastlock_acquire(&ep->xrc.prepost_lock);

	/* Handle race that can occur when SRQ is created and pre-post
	 * receive message function is swapped out. */
	if (ep->srq) {
		fastlock_release(&ep->xrc.prepost_lock);
		return vrb_convert_ret(fi_recv(ep_fid, buf, len, desc,
						 src_addr, context));
	}

	/* The only software error that can occur is overflow */
	if (OFI_UNLIKELY(ep->xrc.prepost_count >= ep->xrc.max_recv_wr)) {
		ret = -FI_EAVAIL;
		goto done;
	}

	recv = calloc(1, sizeof(*recv));
	if (OFI_UNLIKELY(!recv)) {
		ret = -FI_EAGAIN;
		goto done;
	}

	recv->buf = buf;
	recv->desc = desc;
	recv->src_addr = src_addr;
	recv->len = len;
	recv->context = context;
	ep->xrc.prepost_count++;
	slist_insert_tail(&recv->prepost_entry, &ep->xrc.prepost_list);
	ret = FI_SUCCESS;
done:
	fastlock_release(&ep->xrc.prepost_lock);
	return ret;
}

static struct fi_ops_msg vrb_xrc_srq_msg_ops = {
	.size = sizeof(struct fi_ops_msg),
	.recv = vrb_xrc_srq_ep_prepost_recv,
	.recvv = fi_no_msg_recvv,		/* Not used by RXM */
	.recvmsg = fi_no_msg_recvmsg,		/* Not used by RXM */
	.send = fi_no_msg_send,
	.sendv = fi_no_msg_sendv,
	.sendmsg = fi_no_msg_sendmsg,
	.inject = fi_no_msg_inject,
	.senddata = fi_no_msg_senddata,
	.injectdata = fi_no_msg_injectdata,
};

static void vrb_cleanup_prepost_bufs(struct vrb_srq_ep *srq_ep)
{
	struct vrb_xrc_srx_prepost *recv;
	struct slist_entry *entry;

	while (!slist_empty(&srq_ep->xrc.prepost_list)) {
		entry = slist_remove_head(&srq_ep->xrc.prepost_list);
		recv = container_of(entry, struct vrb_xrc_srx_prepost,
				    prepost_entry);
		free(recv);
	}
}

/* Must hold the associated CQ lock cq::xrc.srq_list_lock */
int vrb_xrc_close_srq(struct vrb_srq_ep *srq_ep)
{
	int ret;

	assert(srq_ep->domain->flags & VRB_USE_XRC);
	if (!srq_ep->xrc.cq || !srq_ep->srq)
		return FI_SUCCESS;

	ret = ibv_destroy_srq(srq_ep->srq);
	if (ret) {
		VERBS_WARN(FI_LOG_EP_CTRL, "Cannot destroy SRQ rc=%d\n", ret);
		return -ret;
	}
	srq_ep->srq = NULL;
	srq_ep->xrc.cq = NULL;
	dlist_remove(&srq_ep->xrc.srq_entry);
	vrb_cleanup_prepost_bufs(srq_ep);

	return FI_SUCCESS;
}

static int vrb_srq_close(fid_t fid)
{
	struct vrb_srq_ep *srq_ep = container_of(fid, struct vrb_srq_ep,
						 ep_fid.fid);
	int ret;

	if (srq_ep->domain->flags & VRB_USE_XRC) {
		if (srq_ep->xrc.cq) {
			fastlock_acquire(&srq_ep->xrc.cq->xrc.srq_list_lock);
			ret = vrb_xrc_close_srq(srq_ep);
			fastlock_release(&srq_ep->xrc.cq->xrc.srq_list_lock);
			if (ret)
				goto err;
		}
		fastlock_destroy(&srq_ep->xrc.prepost_lock);
	} else {
		ret = ibv_destroy_srq(srq_ep->srq);
		if (ret)
			goto err;
	}

	ofi_bufpool_destroy(srq_ep->ctx_pool);
	fastlock_destroy(&srq_ep->ctx_lock);
	free(srq_ep);
	return FI_SUCCESS;

err:
	VERBS_WARN(FI_LOG_EP_CTRL, "Cannot destroy SRQ rc=%d\n", ret);
	return ret;
}

static struct fi_ops vrb_srq_ep_ops = {
	.size = sizeof(struct fi_ops),
	.close = vrb_srq_close,
	.bind = fi_no_bind,
	.control = fi_no_control,
	.ops_open = fi_no_ops_open,
};

int vrb_srq_context(struct fid_domain *domain, struct fi_rx_attr *attr,
		       struct fid_ep **srq_ep_fid, void *context)
{
	struct ibv_srq_init_attr srq_init_attr = { 0 };
	struct vrb_domain *dom;
	struct vrb_srq_ep *srq_ep;
	int ret;

	if (!domain)
		return -FI_EINVAL;

	srq_ep = calloc(1, sizeof(*srq_ep));
	if (!srq_ep)
		return -FI_ENOMEM;

	fastlock_init(&srq_ep->ctx_lock);
	ret = ofi_bufpool_create(&srq_ep->ctx_pool, sizeof(struct fi_context),
				 16, attr->size, 1024, OFI_BUFPOOL_NO_TRACK);
	if (ret)
		goto free_ep;

	dom = container_of(domain, struct vrb_domain,
			   util_domain.domain_fid);

	srq_ep->ep_fid.fid.fclass = FI_CLASS_SRX_CTX;
	srq_ep->ep_fid.fid.context = context;
	srq_ep->ep_fid.fid.ops = &vrb_srq_ep_ops;
	srq_ep->ep_fid.ops = &vrb_srq_ep_base_ops;
	srq_ep->ep_fid.cm = &vrb_srq_cm_ops;
	srq_ep->ep_fid.rma = &vrb_srq_rma_ops;
	srq_ep->ep_fid.atomic = &vrb_srq_atomic_ops;
	srq_ep->domain = dom;

	/* XRC SRQ creation is delayed until the first endpoint it is bound
	 * to is enabled.*/
	if (dom->flags & VRB_USE_XRC) {
		fastlock_init(&srq_ep->xrc.prepost_lock);
		slist_init(&srq_ep->xrc.prepost_list);
		dlist_init(&srq_ep->xrc.srq_entry);
		srq_ep->xrc.max_recv_wr = attr->size;
		srq_ep->xrc.max_sge = attr->iov_limit;
		srq_ep->ep_fid.msg = &vrb_xrc_srq_msg_ops;
		goto done;
	}

	srq_ep->ep_fid.msg = &vrb_srq_msg_ops;
	srq_init_attr.attr.max_wr = attr->size;
	srq_init_attr.attr.max_sge = attr->iov_limit;

	srq_ep->srq = ibv_create_srq(dom->pd, &srq_init_attr);
	if (!srq_ep->srq) {
		VERBS_INFO_ERRNO(FI_LOG_DOMAIN, "ibv_create_srq", errno);
		ret = -errno;
		goto free_bufs;
	}

done:
	*srq_ep_fid = &srq_ep->ep_fid;
	return FI_SUCCESS;

free_bufs:
	ofi_bufpool_destroy(srq_ep->ctx_pool);
free_ep:
	fastlock_destroy(&srq_ep->ctx_lock);
	free(srq_ep);
	return ret;
}


#define vrb_atomicvalid(name, flags)					\
static int vrb_msg_ep_atomic_ ## name(struct fid_ep *ep_fid,		\
					 enum fi_datatype datatype,	\
					 enum fi_op op, size_t *count)	\
{									\
	struct vrb_ep *ep = container_of(ep_fid, struct vrb_ep,	\
					    util_ep.ep_fid);		\
	struct fi_atomic_attr attr;					\
	int ret;							\
									\
	ret = vrb_query_atomic(&ep->util_ep.domain->domain_fid,	\
				  datatype, op, &attr, flags);		\
	if (!ret)							\
		*count = attr.count;					\
	return ret;							\
}

vrb_atomicvalid(writevalid, 0);
vrb_atomicvalid(readwritevalid, FI_FETCH_ATOMIC);
vrb_atomicvalid(compwritevalid, FI_COMPARE_ATOMIC);

int vrb_query_atomic(struct fid_domain *domain_fid, enum fi_datatype datatype,
			enum fi_op op, struct fi_atomic_attr *attr,
			uint64_t flags)
{
	struct vrb_domain *domain = container_of(domain_fid,
						    struct vrb_domain,
						    util_domain.domain_fid);
	char *log_str_fetch = "fi_fetch_atomic with FI_SUM op";
	char *log_str_comp = "fi_compare_atomic";
	char *log_str;

	if (flags & FI_TAGGED)
		return -FI_ENOSYS;

	if ((flags & FI_FETCH_ATOMIC) && (flags & FI_COMPARE_ATOMIC))
		return -FI_EBADFLAGS;

	if (!flags) {
		switch (op) {
		case FI_ATOMIC_WRITE:
			break;
		default:
			return -FI_ENOSYS;
		}
	} else {
		if (flags & FI_FETCH_ATOMIC) {
			switch (op) {
			case FI_ATOMIC_READ:
				goto check_datatype;
			case FI_SUM:
				log_str = log_str_fetch;
				break;
			default:
				return -FI_ENOSYS;
			}
		} else if (flags & FI_COMPARE_ATOMIC) {
			if (op != FI_CSWAP)
				return -FI_ENOSYS;
			log_str = log_str_comp;
		} else {
			return  -FI_EBADFLAGS;
		}
		if (domain->info->tx_attr->op_flags & FI_INJECT) {
			VERBS_INFO(FI_LOG_EP_DATA,
				   "FI_INJECT not supported for %s\n", log_str);
			return -FI_EINVAL;
		}
	}
check_datatype:
	switch (datatype) {
	case FI_INT64:
	case FI_UINT64:
#if __BITS_PER_LONG == 64
	case FI_DOUBLE:
	case FI_FLOAT:
#endif
		break;
	default:
		return -FI_EINVAL;
	}

	attr->size = ofi_datatype_size(datatype);
	if (attr->size == 0)
		return -FI_EINVAL;

	attr->count = 1;
	return 0;
}

static ssize_t
vrb_msg_ep_atomic_write(struct fid_ep *ep_fid, const void *buf, size_t count,
			void *desc, fi_addr_t dest_addr, uint64_t addr, uint64_t key,
			enum fi_datatype datatype, enum fi_op op, void *context)
{
	struct vrb_ep *ep =
		container_of(ep_fid, struct vrb_ep, util_ep.ep_fid);
	struct ibv_send_wr wr = {
		.wr_id = VERBS_COMP(ep, (uintptr_t)context),
		.opcode = IBV_WR_RDMA_WRITE,
		.wr.rdma.remote_addr = addr,
		.wr.rdma.rkey = (uint32_t)(uintptr_t)key,
		.send_flags = VERBS_INJECT(ep, sizeof(uint64_t)) |
			      IBV_SEND_FENCE,
	};
	size_t count_copy;
	int ret;

	if (OFI_UNLIKELY(count != 1))
		return -FI_E2BIG;

	if (OFI_UNLIKELY(op != FI_ATOMIC_WRITE))
		return -FI_ENOSYS;

	count_copy = count;

	ret = vrb_msg_ep_atomic_writevalid(ep_fid, datatype, op, &count_copy);
	if (ret)
		return ret;

	return vrb_send_buf(ep, &wr, buf, sizeof(uint64_t), desc);
}

static ssize_t
vrb_msg_ep_atomic_writev(struct fid_ep *ep,
			const struct fi_ioc *iov, void **desc, size_t count,
			fi_addr_t dest_addr, uint64_t addr, uint64_t key,
			enum fi_datatype datatype, enum fi_op op, void *context)
{
	if (OFI_UNLIKELY(iov->count != 1))
		return -FI_E2BIG;

	return vrb_msg_ep_atomic_write(ep, iov->addr, count, desc[0],
			dest_addr, addr, key, datatype, op, context);
}

static ssize_t
vrb_msg_ep_atomic_writemsg(struct fid_ep *ep_fid,
			const struct fi_msg_atomic *msg, uint64_t flags)
{
	struct vrb_ep *ep =
		container_of(ep_fid, struct vrb_ep, util_ep.ep_fid);
	struct ibv_send_wr wr = {
		.wr_id = VERBS_COMP_FLAGS(ep, flags, (uintptr_t)msg->context),
		.wr.rdma.remote_addr = msg->rma_iov->addr,
		.wr.rdma.rkey = (uint32_t)(uintptr_t)msg->rma_iov->key,
		.send_flags = VERBS_INJECT_FLAGS(ep, sizeof(uint64_t), flags) |
			      IBV_SEND_FENCE,
	};
	size_t count_copy;
	int ret;

	if (OFI_UNLIKELY(msg->iov_count != 1 || msg->msg_iov->count != 1))
		return -FI_E2BIG;

	if (OFI_UNLIKELY(msg->op != FI_ATOMIC_WRITE))
		return -FI_ENOSYS;

	count_copy = msg->iov_count;

	ret = vrb_msg_ep_atomic_writevalid(ep_fid, msg->datatype, msg->op,
			&count_copy);
	if (ret)
		return ret;

	if (flags & FI_REMOTE_CQ_DATA) {
		wr.opcode = IBV_WR_RDMA_WRITE_WITH_IMM;
		wr.imm_data = htonl((uint32_t)msg->data);
	} else {
		wr.opcode = IBV_WR_RDMA_WRITE;
	}

	return vrb_send_buf(ep, &wr, msg->msg_iov->addr, sizeof(uint64_t),
			       msg->desc[0]);
}

static ssize_t
vrb_msg_ep_atomic_readwrite(struct fid_ep *ep_fid, const void *buf, size_t count,
			void *desc, void *result, void *result_desc,
			fi_addr_t dest_addr, uint64_t addr, uint64_t key,
			enum fi_datatype datatype,
			enum fi_op op, void *context)
{
	struct vrb_ep *ep =
		container_of(ep_fid, struct vrb_ep, util_ep.ep_fid);
	struct ibv_send_wr wr = {
		.wr_id = VERBS_COMP(ep, (uintptr_t)context),
		.send_flags = IBV_SEND_FENCE,
	};
	size_t count_copy;
	int ret;

	if (OFI_UNLIKELY(count != 1))
		return -FI_E2BIG;

	count_copy = count;

	ret = vrb_msg_ep_atomic_readwritevalid(ep_fid, datatype, op,
			&count_copy);
	if (ret)
		return ret;

	switch (op) {
	case FI_ATOMIC_READ:
		wr.opcode = IBV_WR_RDMA_READ;
		wr.wr.rdma.remote_addr = addr;
		wr.wr.rdma.rkey = (uint32_t)(uintptr_t)key;
		break;
	case FI_SUM:
		wr.opcode = IBV_WR_ATOMIC_FETCH_AND_ADD;
		wr.wr.atomic.remote_addr = addr;
		wr.wr.atomic.compare_add = (uintptr_t)buf;
		wr.wr.atomic.swap = 0;
		wr.wr.atomic.rkey = (uint32_t)(uintptr_t)key;
		break;
	default:
		return -FI_ENOSYS;
	}

	return vrb_send_buf(ep, &wr, result, sizeof(uint64_t), result_desc);
}

static ssize_t
vrb_msg_ep_atomic_readwritev(struct fid_ep *ep, const struct fi_ioc *iov,
			void **desc, size_t count,
			struct fi_ioc *resultv, void **result_desc,
			size_t result_count, fi_addr_t dest_addr, uint64_t addr,
			uint64_t key, enum fi_datatype datatype,
			enum fi_op op, void *context)
{
	if (OFI_UNLIKELY(iov->count != 1))
		return -FI_E2BIG;

	return vrb_msg_ep_atomic_readwrite(ep, iov->addr, count,
			desc[0], resultv->addr, result_desc[0],
			dest_addr, addr, key, datatype, op, context);
}

static ssize_t
vrb_msg_ep_atomic_readwritemsg(struct fid_ep *ep_fid,
				const struct fi_msg_atomic *msg,
				struct fi_ioc *resultv, void **result_desc,
				size_t result_count, uint64_t flags)
{
	struct vrb_ep *ep =
		container_of(ep_fid, struct vrb_ep, util_ep.ep_fid);
	struct ibv_send_wr wr = {
		.wr_id = VERBS_COMP_FLAGS(ep, flags, (uintptr_t)msg->context),
		.send_flags = IBV_SEND_FENCE,
	};
	size_t count_copy;
	int ret;

	if (OFI_UNLIKELY(msg->iov_count != 1 || msg->msg_iov->count != 1))
		return -FI_E2BIG;

	count_copy = msg->iov_count;

	ret = vrb_msg_ep_atomic_readwritevalid(ep_fid, msg->datatype, msg->op,
		       &count_copy);
	if (ret)
		return ret;

	switch (msg->op) {
	case FI_ATOMIC_READ:
		wr.opcode = IBV_WR_RDMA_READ;
		wr.wr.rdma.remote_addr = msg->rma_iov->addr;
		wr.wr.rdma.rkey = (uint32_t) (uintptr_t) msg->rma_iov->key;
		break;
	case FI_SUM:
		wr.opcode = IBV_WR_ATOMIC_FETCH_AND_ADD;
		wr.wr.atomic.remote_addr = msg->rma_iov->addr;
		wr.wr.atomic.compare_add = (uintptr_t) msg->addr;
		wr.wr.atomic.swap = 0;
		wr.wr.atomic.rkey = (uint32_t) (uintptr_t) msg->rma_iov->key;
		break;
	default:
		return -FI_ENOSYS;
	}

	if (flags & FI_REMOTE_CQ_DATA)
		wr.imm_data = htonl((uint32_t) msg->data);

	return vrb_send_buf(ep, &wr, resultv->addr,
			       sizeof(uint64_t), result_desc[0]);
}

static ssize_t
vrb_msg_ep_atomic_compwrite(struct fid_ep *ep_fid, const void *buf, size_t count,
			void *desc, const void *compare,
			void *compare_desc, void *result,
			void *result_desc,
			fi_addr_t dest_addr, uint64_t addr, uint64_t key,
			enum fi_datatype datatype,
			enum fi_op op, void *context)
{
	struct vrb_ep *ep =
		container_of(ep_fid, struct vrb_ep, util_ep.ep_fid);
	struct ibv_send_wr wr = {
		.wr_id = VERBS_COMP(ep, (uintptr_t)context),
		.opcode = IBV_WR_ATOMIC_CMP_AND_SWP,
		.wr.atomic.remote_addr = addr,
		.wr.atomic.compare_add = (uintptr_t)compare,
		.wr.atomic.swap = (uintptr_t)buf,
		.wr.atomic.rkey = (uint32_t)(uintptr_t)key,
		.send_flags = IBV_SEND_FENCE,
	};
	size_t count_copy;
	int ret;

	if (OFI_UNLIKELY(count != 1))
		return -FI_E2BIG;

	count_copy = count;

	ret = vrb_msg_ep_atomic_compwritevalid(ep_fid, datatype, op, &count_copy);
	if (ret)
		return ret;

	return vrb_send_buf(ep, &wr, result, sizeof(uint64_t), result_desc);
}

static ssize_t
vrb_msg_ep_atomic_compwritev(struct fid_ep *ep, const struct fi_ioc *iov,
				void **desc, size_t count,
				const struct fi_ioc *comparev,
				void **compare_desc, size_t compare_count,
				struct fi_ioc *resultv, void **result_desc,
				size_t result_count,
				fi_addr_t dest_addr, uint64_t addr, uint64_t key,
				enum fi_datatype datatype,
				enum fi_op op, void *context)
{
	if (OFI_UNLIKELY(iov->count != 1))
		return -FI_E2BIG;

	return vrb_msg_ep_atomic_compwrite(ep, iov->addr, count, desc[0],
				comparev->addr, compare_desc[0], resultv->addr,
				result_desc[0], dest_addr, addr, key,
				datatype, op, context);
}

static ssize_t
vrb_msg_ep_atomic_compwritemsg(struct fid_ep *ep_fid,
				const struct fi_msg_atomic *msg,
				const struct fi_ioc *comparev,
				void **compare_desc, size_t compare_count,
				struct fi_ioc *resultv,
				void **result_desc, size_t result_count,
				uint64_t flags)
{
	struct vrb_ep *ep =
		container_of(ep_fid, struct vrb_ep, util_ep.ep_fid);
	struct ibv_send_wr wr = {
		.wr_id = VERBS_COMP_FLAGS(ep, flags, (uintptr_t)msg->context),
		.opcode = IBV_WR_ATOMIC_CMP_AND_SWP,
		.wr.atomic.remote_addr = msg->rma_iov->addr,
		.wr.atomic.compare_add = (uintptr_t)comparev->addr,
		.wr.atomic.swap = (uintptr_t)msg->addr,
		.wr.atomic.rkey = (uint32_t)(uintptr_t)msg->rma_iov->key,
		.send_flags = IBV_SEND_FENCE,
	};
	size_t count_copy;
	int ret;

	if (OFI_UNLIKELY(msg->iov_count != 1 || msg->msg_iov->count != 1))
		return -FI_E2BIG;

	count_copy = msg->iov_count;

	ret = vrb_msg_ep_atomic_compwritevalid(ep_fid, msg->datatype, msg->op,
		       &count_copy);
	if (ret)
		return ret;

	if (flags & FI_REMOTE_CQ_DATA)
		wr.imm_data = htonl((uint32_t) msg->data);

	return vrb_send_buf(ep, &wr, resultv->addr, sizeof(uint64_t),
			result_desc[0]);
}

struct fi_ops_atomic vrb_msg_ep_atomic_ops = {
	.size		= sizeof(struct fi_ops_atomic),
	.write		= vrb_msg_ep_atomic_write,
	.writev		= vrb_msg_ep_atomic_writev,
	.writemsg	= vrb_msg_ep_atomic_writemsg,
	.inject		= fi_no_atomic_inject,
	.readwrite	= vrb_msg_ep_atomic_readwrite,
	.readwritev	= vrb_msg_ep_atomic_readwritev,
	.readwritemsg	= vrb_msg_ep_atomic_readwritemsg,
	.compwrite	= vrb_msg_ep_atomic_compwrite,
	.compwritev	= vrb_msg_ep_atomic_compwritev,
	.compwritemsg	= vrb_msg_ep_atomic_compwritemsg,
	.writevalid	= vrb_msg_ep_atomic_writevalid,
	.readwritevalid	= vrb_msg_ep_atomic_readwritevalid,
	.compwritevalid = vrb_msg_ep_atomic_compwritevalid
};

static ssize_t
vrb_msg_xrc_ep_atomic_write(struct fid_ep *ep_fid, const void *buf,
		size_t count, void *desc, fi_addr_t dest_addr, uint64_t addr,
		uint64_t key, enum fi_datatype datatype, enum fi_op op,
		void *context)
{
	struct vrb_xrc_ep *ep = container_of(ep_fid, struct vrb_xrc_ep,
						base_ep.util_ep.ep_fid);
	struct ibv_send_wr wr = {
		.wr_id = VERBS_COMP(&ep->base_ep, (uintptr_t)context),
		.opcode = IBV_WR_RDMA_WRITE,
		.wr.rdma.remote_addr = addr,
		.wr.rdma.rkey = (uint32_t)(uintptr_t)key,
		.send_flags = VERBS_INJECT(&ep->base_ep, sizeof(uint64_t)) |
			      IBV_SEND_FENCE,
	};
	size_t count_copy;
	int ret;

	if (OFI_UNLIKELY(count != 1))
		return -FI_E2BIG;

	if (OFI_UNLIKELY(op != FI_ATOMIC_WRITE))
		return -FI_ENOSYS;

	VRB_SET_REMOTE_SRQN(wr, ep->peer_srqn);

	count_copy = count;

	ret = vrb_msg_ep_atomic_writevalid(ep_fid, datatype, op, &count_copy);
	if (ret)
		return ret;

	return vrb_send_buf(&ep->base_ep, &wr, buf, sizeof(uint64_t), desc);
}

static ssize_t
vrb_msg_xrc_ep_atomic_writemsg(struct fid_ep *ep_fid,
			const struct fi_msg_atomic *msg, uint64_t flags)
{
	struct vrb_xrc_ep *ep = container_of(ep_fid, struct vrb_xrc_ep,
						base_ep.util_ep.ep_fid);
	struct ibv_send_wr wr = {
		.wr_id = VERBS_COMP_FLAGS(&ep->base_ep, flags,
					  (uintptr_t)msg->context),
		.wr.rdma.remote_addr = msg->rma_iov->addr,
		.wr.rdma.rkey = (uint32_t)(uintptr_t)msg->rma_iov->key,
		.send_flags = VERBS_INJECT_FLAGS(&ep->base_ep,
				sizeof(uint64_t), flags) | IBV_SEND_FENCE,
	};
	size_t count_copy;
	int ret;

	if (OFI_UNLIKELY(msg->iov_count != 1 || msg->msg_iov->count != 1))
		return -FI_E2BIG;

	if (OFI_UNLIKELY(msg->op != FI_ATOMIC_WRITE))
		return -FI_ENOSYS;

	VRB_SET_REMOTE_SRQN(wr, ep->peer_srqn);
	count_copy = msg->iov_count;

	ret = vrb_msg_ep_atomic_writevalid(ep_fid, msg->datatype, msg->op,
			&count_copy);
	if (ret)
		return ret;

	if (flags & FI_REMOTE_CQ_DATA) {
		wr.opcode = IBV_WR_RDMA_WRITE_WITH_IMM;
		wr.imm_data = htonl((uint32_t)msg->data);
	} else {
		wr.opcode = IBV_WR_RDMA_WRITE;
	}

	return vrb_send_buf(&ep->base_ep, &wr, msg->msg_iov->addr,
			       sizeof(uint64_t), msg->desc[0]);
}

static ssize_t
vrb_msg_xrc_ep_atomic_readwrite(struct fid_ep *ep_fid, const void *buf,
		size_t count, void *desc, void *result, void *result_desc,
		fi_addr_t dest_addr, uint64_t addr, uint64_t key,
		enum fi_datatype datatype, enum fi_op op, void *context)
{
	struct vrb_xrc_ep *ep = container_of(ep_fid, struct vrb_xrc_ep,
						base_ep.util_ep.ep_fid);
	struct ibv_send_wr wr = {
		.wr_id = VERBS_COMP(&ep->base_ep, (uintptr_t)context),
		.send_flags = IBV_SEND_FENCE,
	};
	size_t count_copy;
	int ret;

	if (OFI_UNLIKELY(count != 1))
		return -FI_E2BIG;

	VRB_SET_REMOTE_SRQN(wr, ep->peer_srqn);
	count_copy = count;

	ret = vrb_msg_ep_atomic_readwritevalid(ep_fid, datatype, op,
			&count_copy);
	if (ret)
		return ret;

	switch (op) {
	case FI_ATOMIC_READ:
		wr.opcode = IBV_WR_RDMA_READ;
		wr.wr.rdma.remote_addr = addr;
		wr.wr.rdma.rkey = (uint32_t)(uintptr_t)key;
		break;
	case FI_SUM:
		wr.opcode = IBV_WR_ATOMIC_FETCH_AND_ADD;
		wr.wr.atomic.remote_addr = addr;
		wr.wr.atomic.compare_add = (uintptr_t)buf;
		wr.wr.atomic.swap = 0;
		wr.wr.atomic.rkey = (uint32_t)(uintptr_t)key;
		break;
	default:
		return -FI_ENOSYS;
	}

	return vrb_send_buf(&ep->base_ep, &wr, result,
			       sizeof(uint64_t), result_desc);
}

static ssize_t
vrb_msg_xrc_ep_atomic_readwritemsg(struct fid_ep *ep_fid,
			const struct fi_msg_atomic *msg,
			struct fi_ioc *resultv, void **result_desc,
			size_t result_count, uint64_t flags)
{
	struct vrb_xrc_ep *ep = container_of(ep_fid, struct vrb_xrc_ep,
						base_ep.util_ep.ep_fid);
	struct ibv_send_wr wr = {
		.wr_id = VERBS_COMP_FLAGS(&ep->base_ep, flags,
					  (uintptr_t)msg->context),
		.send_flags = IBV_SEND_FENCE,
	};
	size_t count_copy;
	int ret;

	if (OFI_UNLIKELY(msg->iov_count != 1 || msg->msg_iov->count != 1))
		return -FI_E2BIG;

	VRB_SET_REMOTE_SRQN(wr, ep->peer_srqn);
	count_copy = msg->iov_count;

	ret = vrb_msg_ep_atomic_readwritevalid(ep_fid, msg->datatype, msg->op,
		       &count_copy);
	if (ret)
		return ret;

	switch (msg->op) {
	case FI_ATOMIC_READ:
		wr.opcode = IBV_WR_RDMA_READ;
		wr.wr.rdma.remote_addr = msg->rma_iov->addr;
		wr.wr.rdma.rkey = (uint32_t) (uintptr_t) msg->rma_iov->key;
		break;
	case FI_SUM:
		wr.opcode = IBV_WR_ATOMIC_FETCH_AND_ADD;
		wr.wr.atomic.remote_addr = msg->rma_iov->addr;
		wr.wr.atomic.compare_add = (uintptr_t) msg->addr;
		wr.wr.atomic.swap = 0;
		wr.wr.atomic.rkey = (uint32_t) (uintptr_t) msg->rma_iov->key;
		break;
	default:
		return -FI_ENOSYS;
	}

	if (flags & FI_REMOTE_CQ_DATA)
		wr.imm_data = htonl((uint32_t) msg->data);

	return vrb_send_buf(&ep->base_ep, &wr, resultv->addr,
			       sizeof(uint64_t), result_desc[0]);
}

static ssize_t
vrb_msg_xrc_ep_atomic_compwrite(struct fid_ep *ep_fid, const void *buf, size_t count,
			void *desc, const void *compare,
			void *compare_desc, void *result,
			void *result_desc,
			fi_addr_t dest_addr, uint64_t addr, uint64_t key,
			enum fi_datatype datatype,
			enum fi_op op, void *context)
{
	struct vrb_xrc_ep *ep = container_of(ep_fid, struct vrb_xrc_ep,
						base_ep.util_ep.ep_fid);
	struct ibv_send_wr wr = {
		.wr_id = VERBS_COMP(&ep->base_ep, (uintptr_t)context),
		.opcode = IBV_WR_ATOMIC_CMP_AND_SWP,
		.wr.atomic.remote_addr = addr,
		.wr.atomic.compare_add = (uintptr_t)compare,
		.wr.atomic.swap = (uintptr_t)buf,
		.wr.atomic.rkey = (uint32_t)(uintptr_t)key,
		.send_flags = IBV_SEND_FENCE,
	};
	size_t count_copy;
	int ret;

	if (OFI_UNLIKELY(count != 1))
		return -FI_E2BIG;

	VRB_SET_REMOTE_SRQN(wr, ep->peer_srqn);
	count_copy = count;

	ret = vrb_msg_ep_atomic_compwritevalid(ep_fid, datatype, op, &count_copy);
	if (ret)
		return ret;

	return vrb_send_buf(&ep->base_ep, &wr, result,
			       sizeof(uint64_t), result_desc);
}

static ssize_t
vrb_msg_xrc_ep_atomic_compwritemsg(struct fid_ep *ep_fid,
				const struct fi_msg_atomic *msg,
				const struct fi_ioc *comparev,
				void **compare_desc, size_t compare_count,
				struct fi_ioc *resultv,
				void **result_desc, size_t result_count,
				uint64_t flags)
{
	struct vrb_xrc_ep *ep = container_of(ep_fid, struct vrb_xrc_ep,
						base_ep.util_ep.ep_fid);
	struct ibv_send_wr wr = {
		.wr_id = VERBS_COMP_FLAGS(&ep->base_ep, flags,
					  (uintptr_t)msg->context),
		.opcode = IBV_WR_ATOMIC_CMP_AND_SWP,
		.wr.atomic.remote_addr = msg->rma_iov->addr,
		.wr.atomic.compare_add = (uintptr_t)comparev->addr,
		.wr.atomic.swap = (uintptr_t)msg->addr,
		.wr.atomic.rkey = (uint32_t)(uintptr_t)msg->rma_iov->key,
		.send_flags = IBV_SEND_FENCE,
	};
	size_t count_copy;
	int ret;

	if (OFI_UNLIKELY(msg->iov_count != 1 || msg->msg_iov->count != 1))
		return -FI_E2BIG;

	VRB_SET_REMOTE_SRQN(wr, ep->peer_srqn);
	count_copy = msg->iov_count;

	ret = vrb_msg_ep_atomic_compwritevalid(ep_fid, msg->datatype, msg->op,
		       &count_copy);
	if (ret)
		return ret;

	if (flags & FI_REMOTE_CQ_DATA)
		wr.imm_data = htonl((uint32_t) msg->data);

	return vrb_send_buf(&ep->base_ep, &wr, resultv->addr,
			       sizeof(uint64_t), result_desc[0]);
}

struct fi_ops_atomic vrb_msg_xrc_ep_atomic_ops = {
	.size		= sizeof(struct fi_ops_atomic),
	.write		= vrb_msg_xrc_ep_atomic_write,
	.writev		= vrb_msg_ep_atomic_writev,
	.writemsg	= vrb_msg_xrc_ep_atomic_writemsg,
	.inject		= fi_no_atomic_inject,
	.readwrite	= vrb_msg_xrc_ep_atomic_readwrite,
	.readwritev	= vrb_msg_ep_atomic_readwritev,
	.readwritemsg	= vrb_msg_xrc_ep_atomic_readwritemsg,
	.compwrite	= vrb_msg_xrc_ep_atomic_compwrite,
	.compwritev	= vrb_msg_ep_atomic_compwritev,
	.compwritemsg	= vrb_msg_xrc_ep_atomic_compwritemsg,
	.writevalid	= vrb_msg_ep_atomic_writevalid,
	.readwritevalid	= vrb_msg_ep_atomic_readwritevalid,
	.compwritevalid = vrb_msg_ep_atomic_compwritevalid
};
