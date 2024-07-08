/*
 * Copyright (c) Intel Corporation, Inc.  All rights reserved.
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

#include <stdint.h>
#include <ofi_mem.h>

#include "verbs_ofi.h"


enum ibv_wc_opcode vrb_wr2wc_opcode(enum ibv_wr_opcode wr)
{
	static enum ibv_wc_opcode wc[] = {
		[IBV_WR_RDMA_WRITE] = IBV_WC_RDMA_WRITE,
		[IBV_WR_RDMA_WRITE_WITH_IMM] = IBV_WC_RDMA_WRITE,
		[IBV_WR_SEND] = IBV_WC_SEND,
		[IBV_WR_SEND_WITH_IMM] = IBV_WC_SEND,
		[IBV_WR_RDMA_READ] = IBV_WC_RDMA_READ,
		[IBV_WR_ATOMIC_CMP_AND_SWP] = IBV_WC_COMP_SWAP,
		[IBV_WR_ATOMIC_FETCH_AND_ADD] = IBV_WC_FETCH_ADD,
	};

	return (wr < ARRAY_SIZE(wc)) ? wc[wr] : IBV_WC_SEND;
}

static void
vrb_get_cq_info(struct ibv_wc *wc, uint64_t *flags, uint64_t *data,
		size_t *len)
{
	switch (wc->opcode) {
	case IBV_WC_SEND:
		*len = 0;
		*flags = (FI_SEND | FI_MSG);
		*data = 0;
		break;
	case IBV_WC_RDMA_WRITE:
		*len = 0;
		*flags = (FI_RMA | FI_WRITE);
		*data = 0;
		break;
	case IBV_WC_RDMA_READ:
		*len = 0;
		*flags = (FI_RMA | FI_READ);
		*data = 0;
		break;
	case IBV_WC_COMP_SWAP:
		*len = 0;
		*flags = FI_ATOMIC;
		*data = 0;
		break;
	case IBV_WC_FETCH_ADD:
		*len = 0;
		*flags = FI_ATOMIC;
		*data = 0;
		break;
	case IBV_WC_RECV:
		*len = wc->byte_len;
		if (wc->wc_flags & IBV_WC_WITH_IMM) {
			*flags = (FI_RECV | FI_MSG | FI_REMOTE_CQ_DATA);
			*data = ntohl(wc->imm_data);
		} else {
			*flags = (FI_RECV | FI_MSG);
			*data = 0;
		}
		break;
	case IBV_WC_RECV_RDMA_WITH_IMM:
		*len = wc->byte_len;
		*flags = (FI_RMA | FI_REMOTE_WRITE | FI_REMOTE_CQ_DATA);
		*data = ntohl(wc->imm_data);
		break;
	default:
		*len = 0;
		*flags = 0;
		*data = 0;
		break;
	}
}

static ssize_t
vrb_cq_readfrom(struct fid_cq *cq_fid, void *buf, size_t count,
		fi_addr_t *src_addr)
{
	struct vrb_cq *cq;
	ssize_t ret;

	cq = container_of(cq_fid, struct vrb_cq, util_cq.cq_fid);
	ofi_genlock_lock(vrb_cq2_progress(cq)->active_lock);
	ret = ofi_cq_readfrom(cq_fid, buf, count, src_addr);
	ofi_genlock_unlock(vrb_cq2_progress(cq)->active_lock);
	return ret;
}

static ssize_t
vrb_cq_readerr(struct fid_cq *cq_fid, struct fi_cq_err_entry *entry,
	       uint64_t flags)
{
	struct vrb_cq *cq;
	ssize_t ret;

	cq = container_of(cq_fid, struct vrb_cq, util_cq.cq_fid);
	ofi_genlock_lock(vrb_cq2_progress(cq)->active_lock);
	ret = ofi_cq_readerr(cq_fid, entry, flags);
	ofi_genlock_unlock(vrb_cq2_progress(cq)->active_lock);
	return ret;
}

static inline int
vrb_poll_events(struct vrb_cq *_cq, int timeout)
{
	int ret, rc;
	void *context;
	struct pollfd fds[2];

	fds[0].fd = _cq->channel->fd;
	fds[1].fd = fd_signal_get(&_cq->signal);

	fds[0].events = fds[1].events = POLLIN;

	rc = poll(fds, 2, timeout);
	if (rc == 0)
		return -FI_EAGAIN;
	else if (rc < 0)
		return -errno;

	if (fds[0].revents & POLLIN) {
		ret = ibv_get_cq_event(_cq->channel, &_cq->cq, &context);
		if (ret)
			return ret;

		ofi_atomic_inc32(&_cq->nevents);
		rc--;
	}
	if (fds[1].revents & POLLIN) {
		fd_signal_reset(&_cq->signal);
		ret = -FI_EAGAIN;
		rc--;
	}
	if (rc) {
		VRB_WARN(FI_LOG_CQ, "Unknown poll error: check revents\n");
		return -FI_EOTHER;
	}

	return ret;
}

static ssize_t
vrb_cq_sread(struct fid_cq *cq, void *buf, size_t count, const void *cond,
		int timeout)
{
	ssize_t ret = 0, cur;
	ssize_t  threshold;
	struct vrb_cq *_cq;
	uint8_t *p;

	p = buf;
	_cq = container_of(cq, struct vrb_cq, util_cq.cq_fid);

	if (!_cq->channel)
		return -FI_ENOSYS;

	threshold = (_cq->wait_cond == FI_CQ_COND_THRESHOLD) ?
		MIN((ssize_t) cond, count) : 1;

	for (cur = 0; cur < threshold; ) {
		if (vrb_cq_trywait(_cq) == FI_SUCCESS) {
			ret = vrb_poll_events(_cq, timeout);
			if (ret)
				break;
		}

		ret = _cq->util_cq.cq_fid.ops->read(&_cq->util_cq.cq_fid, p, count - cur);
		if (ret > 0) {
			p += ret * _cq->entry_size;
			cur += ret;
			if (cur >= threshold)
				break;
		} else if (ret != -FI_EAGAIN) {
			break;
		}
	}

	return cur ? cur : ret;
}

int vrb_poll_cq(struct vrb_cq *cq, struct ibv_wc *wc)
{
	struct vrb_context *ctx;
	struct vrb_ep *ep;
	int ret;

	assert(ofi_genlock_held(vrb_cq2_progress(cq)->active_lock));
	do {
		ret = ibv_poll_cq(cq->cq, 1, wc);
		if (ret <= 0)
			break;

		ctx = (struct vrb_context *) (uintptr_t) wc->wr_id;
		wc->wr_id = (uintptr_t) ctx->user_ctx;
		if (wc->status != IBV_WC_SUCCESS && wc->status != IBV_WC_WR_FLUSH_ERR)
			vrb_shutdown_ep(ctx->ep);
		if (ctx->op_queue == VRB_OP_SQ) {
			ep = ctx->ep;
			assert(ep);
			assert(!slist_empty(&ep->sq_list));
			assert(ep->sq_list.head == &ctx->entry);
			(void) slist_remove_head(&ep->sq_list);
			ep->sq_credits++;

			/* workaround incorrect opcode reported by verbs */
			wc->opcode = vrb_wr2wc_opcode(ctx->sq_opcode);

		} else if (ctx->op_queue == VRB_OP_RQ) {
			ep = ctx->ep;
			assert(ep);
			assert(!slist_empty(&ep->rq_list));
			assert(ep->rq_list.head == &ctx->entry);
			(void) slist_remove_head(&ep->rq_list);
			if (wc->status)
				wc->opcode = IBV_WC_RECV;
		} else {
			assert(ctx->op_queue == VRB_OP_SRQ);
			wc->opcode = IBV_WC_RECV;
		}

		vrb_free_ctx(vrb_cq2_progress(cq), ctx);
	} while (wc->wr_id == VERBS_NO_COMP_FLAG);

	return ret;
}

void vrb_report_wc(struct vrb_cq *cq, struct ibv_wc *wc)
{
	struct fi_cq_err_entry err_entry;
	uint64_t flags, data;
	size_t len;

	assert(ofi_genlock_held(vrb_cq2_progress(cq)->active_lock));

	if (wc->status) {
		vrb_get_cq_info(wc, &err_entry.flags, &err_entry.data, &len);

		err_entry.op_context = (void *) (uintptr_t) wc->wr_id;
		err_entry.len = 0;
		err_entry.buf = NULL;
		err_entry.tag = 0;
		err_entry.olen = 0;

		err_entry.err = (wc->status == IBV_WC_WR_FLUSH_ERR) ?
				FI_ECANCELED : FI_EIO;
		err_entry.prov_errno = (int) wc->status;
		err_entry.err_data = NULL;
		err_entry.err_data_size = 0;

		(void) ofi_cq_write_error(&cq->util_cq, &err_entry);
	} else {
		vrb_get_cq_info(wc, &flags, &data, &len);
		(void) ofi_cq_write(&cq->util_cq, (void *) (uintptr_t) wc->wr_id,
				    flags, len, NULL, data, 0);
	}
}

void vrb_flush_cq(struct vrb_cq *cq)
{
	struct ibv_wc wc;
	ssize_t ret;

	assert(ofi_genlock_held(vrb_cq2_progress(cq)->active_lock));
	while (1) {
		ret = vrb_poll_cq(cq, &wc);
		if (ret <= 0)
			break;

		vrb_report_wc(cq, &wc);
	};
}

void vrb_cleanup_cq(struct vrb_ep *ep)
{
	assert(ofi_genlock_held(vrb_ep2_progress(ep)->active_lock));
	if (ep->util_ep.rx_cq) {
		vrb_flush_cq(container_of(ep->util_ep.rx_cq,
					  struct vrb_cq, util_cq));
	}
	if (ep->util_ep.tx_cq) {
		vrb_flush_cq(container_of(ep->util_ep.tx_cq,
					  struct vrb_cq, util_cq));
	}
}

static const char *
vrb_cq_strerror(struct fid_cq *eq, int prov_errno, const void *err_data,
		   char *buf, size_t len)
{
	if (buf && len) {
		strncpy(buf, ibv_wc_status_str(prov_errno), len);
		return buf;
	}
	return ibv_wc_status_str(prov_errno);
}

int vrb_cq_signal(struct fid_cq *cq)
{
	struct vrb_cq *_cq;

	_cq = container_of(cq, struct vrb_cq, util_cq.cq_fid);

	fd_signal_set(&_cq->signal);

	return 0;
}

int vrb_cq_trywait(struct vrb_cq *cq)
{
	void *context;
	int ret;

	if (!cq->channel) {
		VRB_WARN(FI_LOG_CQ, "No wait object object associated with CQ\n");
		return -FI_EINVAL;
	}

	ofi_genlock_lock(vrb_cq2_progress(cq)->active_lock);
	if (!ofi_cirque_isempty(cq->util_cq.cirq)) {
		ret = -FI_EAGAIN;
		goto out;
	}

	while (!ibv_get_cq_event(cq->channel, &cq->cq, &context))
		ofi_atomic_inc32(&cq->nevents);

	ret = ibv_req_notify_cq(cq->cq, 0);
	if (ret) {
		VRB_WARN(FI_LOG_CQ, "ibv_req_notify_cq error: %d\n", ret);
		ret = -errno;
		goto out;
	}

	/* Fetch any completions that we might have missed while rearming */
	vrb_flush_cq(cq);
	ret = ofi_cirque_isempty(cq->util_cq.cirq) ? FI_SUCCESS : -FI_EAGAIN;

out:
	ofi_genlock_unlock(vrb_cq2_progress(cq)->active_lock);
	return ret;
}

static struct fi_ops_cq vrb_cq_ops = {
	.size = sizeof(struct fi_ops_cq),
	.read = ofi_cq_read,
	.readfrom = vrb_cq_readfrom,
	.readerr = vrb_cq_readerr,
	.sread = vrb_cq_sread,
	.sreadfrom = ofi_cq_sreadfrom,
	.signal = vrb_cq_signal,
	.strerror = vrb_cq_strerror
};

static int vrb_cq_control(fid_t fid, int command, void *arg)
{
	struct fi_wait_pollfd *pollfd;
	struct vrb_cq *cq;
	int ret;

	cq = container_of(fid, struct vrb_cq, util_cq.cq_fid);
	switch(command) {
	case FI_GETWAIT:
		if (!cq->channel) {
			ret = -FI_ENODATA;
			break;
		}

		if (cq->wait_obj == FI_WAIT_FD) {
			*(int *) arg = cq->channel->fd;
			return 0;
		}

		pollfd = arg;
		if (pollfd->nfds >= 1) {
			pollfd->fd[0].fd = cq->channel->fd;
			pollfd->fd[0].events = POLLIN;
			ret = 0;
		} else {
			ret = -FI_ETOOSMALL;
		}
		pollfd->nfds = 1;
		break;
	case FI_GETWAITOBJ:
		*(enum fi_wait_obj *) arg = cq->wait_obj;
		ret = 0;
		break;
	default:
		ret = -FI_ENOSYS;
		break;
	}

	return ret;
}

static int vrb_cq_close(fid_t fid)
{
	struct vrb_cq *cq = container_of(fid, struct vrb_cq, util_cq.cq_fid);
	struct vrb_srx *srx;
	struct dlist_entry *srx_temp;
	int ret;

	if (ofi_atomic_get32(&cq->nevents))
		ibv_ack_cq_events(cq->cq, ofi_atomic_get32(&cq->nevents));

	/* Since an RX CQ and SRX context can be destroyed in any order,
	 * and the XRC SRQ references the RX CQ, we must destroy any
	 * XRC SRQ using this CQ before destroying the CQ. */
	ofi_genlock_lock(vrb_cq2_progress(cq)->active_lock);
	dlist_foreach_container_safe(&cq->xrc.srq_list, struct vrb_srx,
				     srx, xrc.srq_entry, srx_temp) {
		ret = vrb_xrc_close_srq(srx);
		if (ret) {
			ofi_genlock_unlock(vrb_cq2_progress(cq)->active_lock);
			return -ret;
		}
	}
	ofi_genlock_unlock(vrb_cq2_progress(cq)->active_lock);

	if (cq->cq) {
		ret = ibv_destroy_cq(cq->cq);
		if (ret)
			return -ret;
	}

	if (cq->wait_obj != FI_WAIT_NONE)
		fd_signal_free(&cq->signal);

	ofi_cq_cleanup(&cq->util_cq);

	if (cq->channel)
		ibv_destroy_comp_channel(cq->channel);

	free(cq);
	return 0;
}

static struct fi_ops vrb_cq_fi_ops = {
	.size = sizeof(struct fi_ops),
	.close = vrb_cq_close,
	.bind = fi_no_bind,
	.control = vrb_cq_control,
	.ops_open = fi_no_ops_open,
};

static void vrb_cq_progress(struct util_cq *util_cq)
{
	struct vrb_cq *cq;

	cq = container_of(util_cq, struct vrb_cq, util_cq);
	vrb_flush_cq(cq);
}

int vrb_cq_open(struct fid_domain *domain_fid, struct fi_cq_attr *attr,
		   struct fid_cq **cq_fid, void *context)
{
	struct vrb_cq *cq;
	struct vrb_domain *domain =
		container_of(domain_fid, struct vrb_domain,
			     util_domain.domain_fid);
	size_t size;
	int ret;
	struct fi_cq_attr tmp_attr = *attr;
	int comp_vector = 0;

	cq = calloc(1, sizeof(*cq));
	if (!cq)
		return -FI_ENOMEM;

	/* verbs uses its own implementation of wait objects for CQ */
	tmp_attr.wait_obj = FI_WAIT_NONE;
	ret = ofi_cq_init(&vrb_prov, domain_fid, &tmp_attr, &cq->util_cq,
			  vrb_cq_progress, context);
	if (ret)
		goto err1;

	switch (attr->wait_obj) {
	case FI_WAIT_UNSPEC:
		cq->wait_obj = FI_WAIT_FD;
		break;
	case FI_WAIT_FD:
	case FI_WAIT_POLLFD:
	case FI_WAIT_NONE:
		cq->wait_obj = attr->wait_obj;
		break;
	default:
		ret = -FI_ENOSYS;
		goto err2;
	}

	if (attr->flags & FI_AFFINITY) {
		if (attr->signaling_vector < 0 ||
		    attr->signaling_vector > domain->verbs->num_comp_vectors)  {

			VRB_WARN(FI_LOG_CQ,
				   "Invalid value for the CQ attribute signaling_vector: %d\n",
				   attr->signaling_vector);
			ret = -FI_EINVAL;
			goto err2;
		}
		comp_vector = attr->signaling_vector;
	}

	if (cq->wait_obj != FI_WAIT_NONE) {
		cq->channel = ibv_create_comp_channel(domain->verbs);
		if (!cq->channel) {
			ret = -errno;
			VRB_WARN(FI_LOG_CQ,
				   "Unable to create completion channel\n");
			goto err2;
		}

		ret = fi_fd_nonblock(cq->channel->fd);
		if (ret)
			goto err3;

		if (fd_signal_init(&cq->signal)) {
			ret = -errno;
			goto err3;
		}
	}

	size = attr->size ? attr->size : VERBS_DEF_CQ_SIZE;

	/*
	 * Verbs may throw an error if CQ size exceeds ibv_device_attr->max_cqe.
	 * OFI doesn't expose CQ size to the apps because it's better to fix the
	 * issue in the provider than the app dealing with it. The fix is to
	 * open multiple verbs CQs and load balance "MSG EP to CQ binding"* among
	 * them to avoid any CQ overflow.
	 * Something like:
	 * num_qp_per_cq = ibv_device_attr->max_cqe / (qp_send_wr + qp_recv_wr)
	 */
	cq->cq = ibv_create_cq(domain->verbs, size, cq, cq->channel,
			       comp_vector);
	if (!cq->cq) {
		ret = -errno;
		VRB_WARN(FI_LOG_CQ, "Unable to create verbs CQ\n");
		goto err3;
	}

	if (cq->channel) {
		ret = ibv_req_notify_cq(cq->cq, 0);
		if (ret) {
			VRB_WARN(FI_LOG_CQ,
				   "ibv_req_notify_cq failed\n");
			goto err4;
		}
	}

	cq->flags |= attr->flags;
	cq->wait_cond = attr->wait_cond;
	/* verbs uses its own ops for CQ */
	cq->util_cq.cq_fid.fid.ops = &vrb_cq_fi_ops;
	cq->util_cq.cq_fid.ops = &vrb_cq_ops;

	// slist_init(&cq->saved_wc_list);
	dlist_init(&cq->xrc.srq_list);

	ofi_atomic_initialize32(&cq->nevents, 0);

	*cq_fid = &cq->util_cq.cq_fid;
	return 0;

err4:
	ibv_destroy_cq(cq->cq);
err3:
	if (cq->channel)
		ibv_destroy_comp_channel(cq->channel);
err2:
	ofi_cq_cleanup(&cq->util_cq);
err1:
	free(cq);
	return ret;
}


int vrb_init_progress(struct vrb_progress *progress, struct fi_info *info)
{
	int ret;

	ret = ofi_genlock_init(&progress->ep_lock, OFI_LOCK_MUTEX);
	if (ret)
		return ret;

	/* Active lock will be needed when adding rdm ep support */
	progress->active_lock = &progress->ep_lock;

	ret = ofi_bufpool_create(&progress->ctx_pool, sizeof(struct fi_context),
				 16, 0, 1024, OFI_BUFPOOL_NO_TRACK);
	if (ret)
		goto err1;

	ret = ofi_bufpool_create(&progress->recv_wr_pool,
				 sizeof(struct vrb_recv_wr) +
				 info->rx_attr->iov_limit * sizeof(struct ibv_sge),
				 16, 0, 1024, OFI_BUFPOOL_NO_TRACK);
	if (ret)
		goto err2;

	return 0;

err2:
	ofi_bufpool_destroy(progress->ctx_pool);
err1:
	ofi_genlock_destroy(&progress->ep_lock);
	return ret;
}

void vrb_close_progress(struct vrb_progress *progress)
{
	ofi_bufpool_destroy(progress->recv_wr_pool);
	ofi_bufpool_destroy(progress->ctx_pool);
	ofi_genlock_destroy(&progress->ep_lock);
}
