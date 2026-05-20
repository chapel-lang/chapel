/*
 * SPDX-License-Identifier: BSD-2-Clause OR GPL-2.0-only
 *
 * Copyright (c) 2014 Intel Corporation, Inc. All rights reserved.
 * Copyright (c) 2016 Cisco Systems, Inc. All rights reserved.
 * Copyright (c) 2018-2023 Hewlett Packard Enterprise Development LP
 */

#include "config.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/types.h>

#include <ofi_list.h>
#include <ofi.h>

#include "cxip.h"

#define CXIP_DBG(...) _CXIP_DBG(FI_LOG_CQ, __VA_ARGS__)
#define CXIP_WARN(...) _CXIP_WARN(FI_LOG_CQ, __VA_ARGS__)

/*
 * cxip_cq_req_complete() - Generate a completion event for the request.
 */
int cxip_cq_req_complete(struct cxip_req *req)
{
	if (req->discard) {
		CXIP_DBG("Event discarded: %p\n", req);
		return FI_SUCCESS;
	}

	return ofi_peer_cq_write(&req->cq->util_cq, (void *)req->context,
				 req->flags, req->data_len, (void *)req->buf,
				 req->data, req->tag, FI_ADDR_NOTAVAIL);
}

/*
 * cxip_cq_req_complete() - Generate a completion event with source address for
 * the request.
 */
int cxip_cq_req_complete_addr(struct cxip_req *req, fi_addr_t src)
{
	if (req->discard) {
		CXIP_DBG("Event discarded: %p\n", req);
		return FI_SUCCESS;
	}

	return ofi_peer_cq_write(&req->cq->util_cq, (void *)req->context,
				 req->flags, req->data_len, (void *)req->buf,
				 req->data, req->tag, src);
}

/*
 * proverr2errno() - Match NIC errno to Linux errno.
 */
int proverr2errno(int err)
{
	if (err == C_RC_UNDELIVERABLE)
		return FI_EHOSTUNREACH;
	else if (err == C_RC_VNI_NOT_FOUND)
		return FI_ENOTCONN;
	return FI_EIO;
}

/*
 * cxip_cq_req_error() - Generate an error event for the request.
 */
int cxip_cq_req_error(struct cxip_req *req, size_t olen,
		      int err, int prov_errno, void *err_data,
		      size_t err_data_size, fi_addr_t src_addr)
{
	struct fi_cq_err_entry err_entry;

	if (req->discard) {
		CXIP_DBG("Event discarded: %p\n", req);
		return FI_SUCCESS;
	}

	err_entry.err = err;
	err_entry.olen = olen;
	err_entry.err_data = err_data;
	err_entry.err_data_size = err_data_size;
	err_entry.len = req->data_len;
	err_entry.prov_errno = prov_errno;
	err_entry.flags = req->flags;
	err_entry.data = req->data;
	err_entry.tag = req->tag;
	err_entry.op_context = (void *)(uintptr_t)req->context;
	err_entry.buf = (void *)(uintptr_t)req->buf;
	err_entry.src_addr = src_addr;

	return ofi_peer_cq_write_error(&req->cq->util_cq, &err_entry);
}

/*
 * cxip_cq_progress() - Progress the CXI Completion Queue.
 *
 * The CQ lock must not be held and this function can not be
 * called from within event queue callback processing.
 */
void cxip_cq_progress(struct cxip_cq *cq)
{
	cxip_util_cq_progress(&cq->util_cq);
}

/*
 * cxip_util_cq_progress() - Progress function wrapper for utility CQ.
 */
void cxip_util_cq_progress(struct util_cq *util_cq)
{
	struct cxip_cq *cq = container_of(util_cq, struct cxip_cq, util_cq);
	struct fid_list_entry *fid_entry;
	struct dlist_entry *item;

	ofi_genlock_lock(&cq->ep_list_lock);
	dlist_foreach(&util_cq->ep_list, item) {
		fid_entry = container_of(item, struct fid_list_entry, entry);
		cxip_ep_progress(fid_entry->fid);
	}
	ofi_genlock_unlock(&cq->ep_list_lock);
}

/* common function for both eq and cq strerror function */
const char *cxip_strerror(int prov_errno)
{
	/* both CXI driver error and collective errors share this function */
	if (prov_errno < FI_CXI_ERRNO_RED_FIRST)
		return cxi_rc_to_str(prov_errno);

	switch (prov_errno) {
	/* EQ JOIN error codes */
	case FI_CXI_ERRNO_JOIN_MCAST_INUSE:
		return "coll join multicast address in-use";
	case FI_CXI_ERRNO_JOIN_HWROOT_INUSE:
		return "coll join hwroot in-use";
	case FI_CXI_ERRNO_JOIN_MCAST_INVALID:
		return "coll join multicast address invalid";
	case FI_CXI_ERRNO_JOIN_HWROOT_INVALID:
		return "coll join hwroot invalid";
	case FI_CXI_ERRNO_JOIN_CURL_FAILED:
		return "coll join FM REST CURL failed";
	case FI_CXI_ERRNO_JOIN_CURL_TIMEOUT:
		return "coll join FM REST CURL timed out";
	case FI_CXI_ERRNO_JOIN_FAIL_PTE:
		return "coll join PTE setup failed";
	case FI_CXI_ERRNO_JOIN_OTHER:
		return "coll join unknown error";
	case FI_CXI_ERRNO_JOIN_FAIL_RDMA:
		return "coll rdma setup error";

	/* CQ REDUCE error codes */
	case FI_CXI_ERRNO_RED_FLT_OVERFLOW:
		return "coll reduce FLT overflow";
	case FI_CXI_ERRNO_RED_FLT_INVALID:
		return "coll reduce FLT invalid";
	case FI_CXI_ERRNO_RED_INT_OVERFLOW:
		return "coll reduce INT overflow";
	case FI_CXI_ERRNO_RED_CONTR_OVERFLOW:
		return "coll reduce contribution overflow";
	case FI_CXI_ERRNO_RED_OP_MISMATCH:
		return "coll reduce opcode mismatch";
	case FI_CXI_ERRNO_RED_MC_FAILURE:
		return "coll reduce multicast timeout";
	case FI_CXI_COLL_RC_RDMA_FAILURE:
		return "coll leaf rdma read failure";
	case FI_CXI_COLL_RC_RDMA_DATA_FAILURE:
		return "coll leaf rdma read unexpected packet failure";

	/* Unknown error */
	default:
		return "coll unspecified error";
	}
}

/*
 * cxip_cq_strerror() - Converts provider specific error information into a
 * printable string.
 */
static const char *cxip_cq_strerror(struct fid_cq *cq, int prov_errno,
				    const void *err_data, char *buf, size_t len)
{
	const char *errmsg = cxip_strerror(prov_errno);
	if (buf && len > 0)
		strncpy(buf, errmsg, len);
	return errmsg;
}

int cxip_cq_trywait(struct cxip_cq *cq)
{
	struct fid_list_entry *fid_entry;
	struct dlist_entry *item;
	struct cxip_ep *ep;

	if (cq->ep_fd < 0) {
		CXIP_WARN("No CXI wait object\n");
		return -FI_EINVAL;
	}

	ofi_genlock_lock(&cq->util_cq.cq_lock);
	if (!ofi_cirque_isempty(cq->util_cq.cirq)) {
		ofi_genlock_unlock(&cq->util_cq.cq_lock);
		return -FI_EAGAIN;
	}
	ofi_genlock_unlock(&cq->util_cq.cq_lock);

	ofi_genlock_lock(&cq->ep_list_lock);
	dlist_foreach(&cq->util_cq.ep_list, item) {
		fid_entry = container_of(item, struct fid_list_entry, entry);
		ep = container_of(fid_entry->fid, struct cxip_ep, ep.fid);

		if (!ep->ep_obj->priv_wait)
			continue;

		if (cxip_ep_trywait(ep->ep_obj, cq)) {
			ofi_genlock_unlock(&cq->ep_list_lock);

			return -FI_EAGAIN;
		}
	}
	ofi_genlock_unlock(&cq->ep_list_lock);

	return FI_SUCCESS;
}

/*
 * cxip_cq_flush_trig_reqs() - Flush all triggered requests on the CQ.
 *
 * This function will free all triggered requests associated with the
 * CQ. This should only be called after canceling triggered operations
 * against all counters in use and verifying the cancellations have
 * completed successfully.
 */
void cxip_cq_flush_trig_reqs(struct cxip_cq *cq)
{
	struct fid_list_entry *fid_entry;
	struct dlist_entry *item;
	struct cxip_ep *ep;

	ofi_genlock_lock(&cq->ep_list_lock);
	dlist_foreach(&cq->util_cq.ep_list, item) {
		fid_entry = container_of(item, struct fid_list_entry, entry);
		ep = container_of(fid_entry->fid, struct cxip_ep, ep.fid);

		cxip_ep_flush_trig_reqs(ep->ep_obj);
	}
	ofi_genlock_unlock(&cq->ep_list_lock);
}

/*
 * cxip_cq_close() - Destroy the Completion Queue object.
 */
static int cxip_cq_close(struct fid *fid)
{
	struct cxip_cq *cq = container_of(fid, struct cxip_cq,
					  util_cq.cq_fid.fid);
	int count = ofi_atomic_get32(&cq->util_cq.ref);

	if (count) {
		CXIP_DBG("CQ refcount non-zero:%d returning FI_EBUSY\n", count);
		return -FI_EBUSY;
	}

	if (cq->ep_fd >= 0)
		close(cq->ep_fd);

	ofi_cq_cleanup(&cq->util_cq);
	ofi_genlock_destroy(&cq->ep_list_lock);
	cxip_domain_remove_cq(cq->domain, cq);

	free(cq);

	return 0;
}

static int cxip_cq_signal(struct fid_cq *cq_fid)
{
	return -FI_ENOSYS;
}

static int cxip_cq_control(fid_t fid, int command, void *arg)
{
	struct cxip_cq *cq = container_of(fid, struct cxip_cq, util_cq.cq_fid);
	struct fi_wait_pollfd *pollfd;
	int ret;

	switch (command) {
	case FI_GETWAIT:
		if (cq->ep_fd < 0) {
			ret = -FI_ENODATA;
			break;
		}
		if (cq->attr.wait_obj == FI_WAIT_FD) {
			*(int *) arg = cq->ep_fd;
			return FI_SUCCESS;
		}

		pollfd = arg;
		if (pollfd->nfds >= 1) {
			pollfd->fd[0].fd = cq->ep_fd;
			pollfd->fd[0].events = POLLIN;
			pollfd->nfds = 1;

			ret = FI_SUCCESS;
		} else {
			ret = -FI_ETOOSMALL;
		}
		break;
	case FI_GETWAITOBJ:
		*(enum fi_wait_obj *) arg = cq->attr.wait_obj;
		ret = FI_SUCCESS;
		break;
	default:
		ret = -FI_ENOSYS;
		break;
	}

	return ret;
}

static ssize_t cxip_cq_sreadfrom(struct fid_cq *cq_fid, void *buf,
				 size_t count, fi_addr_t *src_addr,
				 const void *cond, int timeout)
{
	struct cxip_cq *cq = container_of(cq_fid, struct cxip_cq,
					  util_cq.cq_fid);
	struct epoll_event ev;
	uint64_t endtime;
	ssize_t ret;

	if (!cq->attr.wait_obj)
		return -FI_EINVAL;

	endtime = ofi_timeout_time(timeout);

	do {
		ret = fi_cq_readfrom(cq_fid, buf, count, src_addr);
		if (ret != -FI_EAGAIN)
			break;

		if (ofi_adjust_timeout(endtime, &timeout))
			return -FI_EAGAIN;

		ret = cxip_cq_trywait(cq);
		if (ret == -FI_EAGAIN) {
			ret = 0;
			continue;
		}
		assert(ret == FI_SUCCESS);

		memset(&ev, 0, sizeof(ev));
		ret = epoll_wait(cq->ep_fd, &ev, 1, timeout);
		if (ret > 0)
			ret = 0;

	} while (!ret);

	return ret == -FI_ETIMEDOUT ? -FI_EAGAIN : ret;
}

static ssize_t cxip_cq_sread(struct fid_cq *cq_fid, void *buf, size_t count,
			     const void *cond, int timeout)
{
	return cxip_cq_sreadfrom(cq_fid, buf, count, NULL, cond, timeout);
}

static struct fi_ops cxip_cq_fi_ops = {
	.size = sizeof(struct fi_ops),
	.close = cxip_cq_close,
	.bind = fi_no_bind,
	.control = cxip_cq_control,
	.ops_open = fi_no_ops_open,
};

static struct fi_ops_cq cxip_cq_ops = {
	.size = sizeof(struct fi_ops_cq),
	.read = ofi_cq_read,
	.readfrom = ofi_cq_readfrom,
	.readerr = ofi_cq_readerr,
	.sread = cxip_cq_sread,
	.sreadfrom = cxip_cq_sreadfrom,
	.signal = cxip_cq_signal,
	.strerror = cxip_cq_strerror,
};

static struct fi_cq_attr cxip_cq_def_attr = {
	.flags = 0,
	.format = FI_CQ_FORMAT_CONTEXT,
	.wait_obj = FI_WAIT_NONE,
	.signaling_vector = 0,
	.wait_cond = FI_CQ_COND_NONE,
	.wait_set = NULL,
};

/*
 * cxip_cq_verify_attr() - Verify input Completion Queue attributes.
 */
static int cxip_cq_verify_attr(struct fi_cq_attr *attr)
{
	if (!attr)
		return FI_SUCCESS;

	if (attr->flags & FI_PEER &&
	    attr->wait_obj != FI_WAIT_NONE)
		return -FI_ENOSYS;

	switch (attr->format) {
	case FI_CQ_FORMAT_CONTEXT:
	case FI_CQ_FORMAT_MSG:
	case FI_CQ_FORMAT_DATA:
	case FI_CQ_FORMAT_TAGGED:
		break;
	case FI_CQ_FORMAT_UNSPEC:
		attr->format = cxip_cq_def_attr.format;
		break;
	default:
		CXIP_WARN("Unsupported CQ attribute format: %d\n",
			  attr->format);
		return -FI_ENOSYS;
	}

	/* Applications should set wait_obj == FI_WAIT_NONE for best
	 * performance. However, if a wait_obj is required and not
	 * specified, default to FI_WAIT_FD.
	 */
	switch (attr->wait_obj) {
	case FI_WAIT_UNSPEC:
		attr->wait_obj = FI_WAIT_FD;
		break;
	case FI_WAIT_NONE:
	case FI_WAIT_FD:
	case FI_WAIT_POLLFD:
		break;
	default:
		CXIP_WARN("Unsupported CQ wait object: %d\n",
			  attr->wait_obj);
		return -FI_ENOSYS;
	}

	/* Use environment variable to allow for dynamic setting of default CQ
	 * size.
	 */
	if (!attr->size)
		attr->size = cxip_env.default_cq_size;

	return FI_SUCCESS;
}

/* EP adds wait FD to the CQ epoll FD */
int cxip_cq_add_wait_fd(struct cxip_cq *cq, int wait_fd, int events)
{
	struct epoll_event ev = {
		.events = events,
	};
	int ret;

	ret = epoll_ctl(cq->ep_fd, EPOLL_CTL_ADD, wait_fd, &ev);
	if (ret < 0) {
		ret = errno;
		CXIP_WARN("EP wait FD add to CQ failed %d\n", ret);

		return -FI_EINVAL;
	}

	return FI_SUCCESS;
}

/* EP deletes wait FD from the CQ epoll FD */
void cxip_cq_del_wait_fd(struct cxip_cq *cq, int wait_fd)
{
	int ret;

	ret = epoll_ctl(cq->ep_fd, EPOLL_CTL_DEL, wait_fd, NULL);
	if (ret < 0) {
		ret = errno;
		CXIP_WARN("EP wait FD delete from CQ failed %d\n", ret);
	}
}

/*
 * cxip_cq_open() - Allocate a new Completion Queue object.
 */
int cxip_cq_open(struct fid_domain *domain, struct fi_cq_attr *attr,
		 struct fid_cq **cq, void *context)
{
	struct cxip_domain *cxi_dom;
	struct cxip_cq *cxi_cq;
	struct fi_cq_attr temp_attr;
	int ret;

	if (!domain || !cq)
		return -FI_EINVAL;

	cxi_dom = container_of(domain, struct cxip_domain,
			       util_domain.domain_fid);

	ret = cxip_cq_verify_attr(attr);
	if (ret != FI_SUCCESS)
		return ret;

	cxi_cq = calloc(1, sizeof(*cxi_cq));
	if (!cxi_cq)
		return -FI_ENOMEM;

	if (!attr) {
		cxi_cq->attr = cxip_cq_def_attr;
		cxi_cq->attr.size = cxip_env.default_cq_size;
	} else {
		cxi_cq->attr = *attr;
	}

	/* CXI does not use common code internal wait object */
	temp_attr = cxi_cq->attr;
	temp_attr.wait_obj = FI_WAIT_NONE;
	ret = ofi_cq_init(&cxip_prov, domain, &temp_attr, &cxi_cq->util_cq,
			  cxip_util_cq_progress, context);
	if (ret != FI_SUCCESS) {
		CXIP_WARN("ofi_cq_init() failed: %d\n", ret);
		goto err_util_cq;
	}

	cxi_cq->util_cq.cq_fid.fid.ops = &cxip_cq_fi_ops;
	if (!(cxi_cq->attr.flags & FI_PEER))
		cxi_cq->util_cq.cq_fid.ops = &cxip_cq_ops;
	cxi_cq->domain = cxi_dom;
	cxi_cq->ack_batch_size = cxip_env.eq_ack_batch_size;
	cxi_cq->ep_fd = -1;

	/* Optimize locking when possible */
	if (cxi_dom->util_domain.threading == FI_THREAD_DOMAIN ||
	    cxi_dom->util_domain.threading == FI_THREAD_COMPLETION)
		ofi_genlock_init(&cxi_cq->ep_list_lock, OFI_LOCK_NONE);
	else
		ofi_genlock_init(&cxi_cq->ep_list_lock, OFI_LOCK_SPINLOCK);

	if (cxi_cq->attr.wait_obj) {
		cxi_cq->ep_fd = epoll_create1(0);
		if (cxi_cq->ep_fd < 0) {
			CXIP_WARN("Unable to open epoll FD: %s\n",
				  strerror(errno));
			goto err_wait_alloc;
		}
	}

	cxip_domain_add_cq(cxi_dom, cxi_cq);
	*cq = &cxi_cq->util_cq.cq_fid;

	return FI_SUCCESS;

err_wait_alloc:
	ofi_cq_cleanup(&cxi_cq->util_cq);
err_util_cq:
	free(cxi_cq);

	return ret;
}
