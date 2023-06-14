/*
 * Copyright (c) 2016, Cisco Systems, Inc. All rights reserved.
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
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

/* This needs to be included for usdf.h */
#include "ofi.h"
#include "ofi_enosys.h"
#include "ofi_util.h"

#include "usdf.h"
#include "usdf_cq.h"
#include "usdf_wait.h"


/* Necessary to support top-of-file struct declarations. */
static int usdf_wait_wait(struct fid_wait *wait_fid, int timeout);
static int usdf_wait_close(struct fid *waitset);
static int usdf_wait_control(struct fid *fid, int command, void *arg);

static struct fi_ops_wait usdf_wait_ops = {
	.size = sizeof(struct fi_ops_wait),
	.wait = usdf_wait_wait,
};

static struct fi_ops usdf_wait_fi_ops = {
	.size = sizeof(struct fi_ops),
	.close = usdf_wait_close,
	.bind = fi_no_bind,
	.control = usdf_wait_control,
	.ops_open = fi_no_ops_open
};

static int usdf_wait_trywait(struct fid *fwait)
{
	struct usdf_wait *wait;
	struct dlist_entry *item;
	struct fid_list_entry *entry;
	int ret = FI_SUCCESS;

	wait = wait_fidtou(fwait);

	dlist_foreach(&wait->list, item) {
		entry = container_of(item, struct fid_list_entry, entry);

		switch (entry->fid->fclass) {
		case FI_CLASS_EQ:
			continue;
		case FI_CLASS_CQ:
			ret = usdf_cq_trywait(entry->fid);
			if (ret)
				return ret;
			break;
		default:
			USDF_DBG_SYS(FABRIC, "invalid fid %p\n", entry->fid);
			return -FI_EINVAL;
		}
	}

	return ret;
}

int usdf_trywait(struct fid_fabric *fabric, struct fid **fids, int count)
{
	size_t i;
	int ret;

	for (i = 0; i < count; i++) {
		assert(fids[i]);

		switch (fids[i]->fclass) {
		case FI_CLASS_EQ:
			continue;
		case FI_CLASS_CQ:
			ret = usdf_cq_trywait(fids[i]);
			break;
		case FI_CLASS_WAIT:
			ret = usdf_wait_trywait(fids[i]);
			break;
		default:
			USDF_DBG_SYS(FABRIC, "invalid fid\n");
			return -FI_EINVAL;
		}

		if (ret)
			return ret;
	}

	return FI_SUCCESS;
}

/* Since a domain hasn't been opened at the time of wait object creation, open a
 * device temporarily to check for the group interrupt capability.
 */
static int usdf_wait_check_support(struct usdf_fabric *fabric_priv)
{
	struct usd_open_params params = {
		.flags = UOPF_SKIP_PD_ALLOC,
		.cmd_fd = -1,
		.context = NULL
	};
	struct usd_device *dev;
	int ret;

	ret = usd_open_with_params(fabric_priv->fab_dev_attrs->uda_devname,
			&params, &dev);
	if (ret) {
		USDF_DBG_SYS(FABRIC,
				"opening device to check fd support failed.\n");
		return ret;
	}

	if (!usd_get_cap(dev, USD_CAP_GRP_INTR)) {
		USDF_WARN_SYS(FABRIC, "FD request invalid.\n");
		USDF_WARN_SYS(FABRIC, "group interrupts not supported.\n");
		ret = usd_close(dev);
		if (ret)
			USDF_WARN_SYS(FABRIC, "closing usd device failed: %s\n",
					strerror(ret));

		return -FI_EOPNOTSUPP;
	}

	return usd_close(dev);
}

/* Non-static because this is exported due to being returned as a callback for
 * fabric ops.
 *
 * Supporting wait objects in the usNIC provider is done using an epoll
 * context. When fi_wait_open is called an epoll context is created using
 * epoll_create1. This simplifies multi-CQ support and also allows us to get
 * around a limitation of the usNIC provider. IB completion channels are opened
 * on the domain because we have a context associated with the domain. At
 * fi_wait_open time, we only have access to the fabric. It isn't guaranteed
 * that a domain has been opened yet. The epoll context approach allows us to
 * defer creating the completion channel for the CQ until CQ open time.
 */
int usdf_wait_open(struct fid_fabric *fabric, struct fi_wait_attr *attr,
		struct fid_wait **waitset)
{
	struct usdf_wait *wait_priv;
	struct usdf_fabric *fabric_priv;
	ofi_epoll_t epfd;
	int ret;

	USDF_TRACE_SYS(FABRIC, "\n");

	switch (attr->wait_obj) {
	case FI_WAIT_UNSPEC:
	case FI_WAIT_FD:
		break;
	default:
		USDF_WARN_SYS(FABRIC, "unsupported wait object type\n");
		ret = -FI_EINVAL;
		goto error;
	}

	fabric_priv = fab_fidtou(fabric);
	ret = usdf_wait_check_support(fabric_priv);
	if (ret)
		goto error;

	ret = ofi_epoll_create(&epfd);
	if (ret) {
		USDF_WARN_SYS(FABRIC, "failed to create epoll fd[%d]\n", errno);
		goto error;
	}

	USDF_DBG_SYS(FABRIC, "successfully created epoll fd: %d\n", epfd);

	wait_priv = calloc(1, sizeof(*wait_priv));
	if (!wait_priv) {
		USDF_WARN_SYS(FABRIC,
				"unable to allocate memory for usdf_wait obj");
		ret = -FI_ENOMEM;
		goto calloc_fail;
	}

	wait_priv->wait_fid.fid.fclass = FI_CLASS_WAIT;
	wait_priv->wait_fid.fid.ops = &usdf_wait_fi_ops;
	wait_priv->wait_fid.ops = &usdf_wait_ops;
	wait_priv->wait_fid.fid.context = 0;
	wait_priv->wait_fabric = fabric_priv;
	wait_priv->wait_obj = attr->wait_obj;
	wait_priv->object.epfd = epfd;

	ofi_atomic_initialize32(&wait_priv->wait_refcnt, 0);
	ofi_mutex_init(&wait_priv->lock);
	dlist_init(&wait_priv->list);

	ofi_atomic_inc32(&wait_priv->wait_fabric->fab_refcnt);

	*waitset = &wait_priv->wait_fid;

	return FI_SUCCESS;

calloc_fail:
	ofi_epoll_close(epfd);
error:
	*waitset = NULL;
	return ret;
}

/* Close a wait object. Make sure all resources associated with the wait object
 * have been closed.
 */
static int usdf_wait_close(struct fid *waitset)
{
	struct usdf_wait *wait_priv;

	USDF_TRACE_SYS(FABRIC, "\n");
	if (!waitset) {
		USDF_WARN_SYS(FABRIC, "invalid input.\n");
		return -FI_EINVAL;
	}

	wait_priv = wait_ftou(waitset);

	if (ofi_atomic_get32(&wait_priv->wait_refcnt) > 0) {
		USDF_DBG_SYS(FABRIC,
				"failed to close waitset with non-zero refcnt");
		return -FI_EBUSY;
	}

	switch (wait_priv->wait_obj) {
	case FI_WAIT_UNSPEC:
	case FI_WAIT_FD:
		ofi_epoll_close(wait_priv->object.epfd);
		break;
	default:
		USDF_WARN_SYS(FABRIC,
				"unsupported wait object type\n");
		return -FI_EINVAL;
	}

	ofi_atomic_dec32(&wait_priv->wait_fabric->fab_refcnt);
	free(wait_priv);

	return FI_SUCCESS;
}

static int usdf_wait_wait(struct fid_wait *fwait, int timeout)
{
	struct usdf_wait *wait;
	struct ofi_epollfds_event event;
	int ret = FI_SUCCESS;
	int nevents;

	USDF_TRACE_SYS(FABRIC, "\n");
	wait = wait_ftou(fwait);

	ret = usdf_wait_trywait(&fwait->fid);
	if (ret) {
		if (ret == -FI_EAGAIN)
			return FI_SUCCESS;

		return ret;
	}

	nevents = ofi_epoll_wait(wait->object.epfd, &event, 1, timeout);
	if (nevents == 0) {
		ret = -FI_ETIMEDOUT;
	} else if (nevents < 0) {
		USDF_DBG_SYS(FABRIC, "epoll wait failed\n");
		ret = nevents;
	}

	return ret;
}

static int usdf_wait_get_wait(struct usdf_wait *wait_priv, void *arg)
{
	USDF_TRACE_SYS(FABRIC, "\n");

	if (!arg || !wait_priv) {
		USDF_WARN_SYS(FABRIC, "invalid input\n");
		return -FI_EINVAL;
	}

	switch (wait_priv->wait_obj) {
	case FI_WAIT_UNSPEC:
	case FI_WAIT_FD:
#ifdef HAVE_EPOLL
		*(int *) arg = wait_priv->object.epfd;
#else
		return -FI_ENOSYS;
#endif
		break;
	default:
		USDF_DBG_SYS(FABRIC, "unsupported wait type\n");
		return -FI_EINVAL;
	}

	return FI_SUCCESS;
}

static int usdf_wait_control(struct fid *fid, int command, void *arg)
{
	struct usdf_wait *wait_priv;

	USDF_TRACE_SYS(FABRIC, "\n");

	wait_priv = container_of(fid, struct usdf_wait, wait_fid.fid);

	switch (command) {
	case FI_GETWAIT:
		break;
	default:
		USDF_DBG_SYS(FABRIC, "unsupported control command\n");
		return -FI_EINVAL;
	}

	return usdf_wait_get_wait(wait_priv, arg);
}
