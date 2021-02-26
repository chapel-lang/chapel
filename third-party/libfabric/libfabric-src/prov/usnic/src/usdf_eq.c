/*
 * Copyright (c) 2014-2017, Cisco Systems, Inc. All rights reserved.
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

#include "config.h"

#include <asm/types.h>
#include <assert.h>
#include <errno.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <poll.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/queue.h>
#include <sys/eventfd.h>
#include <inttypes.h>

#include <rdma/fabric.h>
#include <rdma/fi_cm.h>
#include <rdma/fi_domain.h>
#include <rdma/fi_endpoint.h>
#include <rdma/fi_rma.h>
#include <rdma/fi_errno.h>
#include "ofi.h"
#include "ofi_enosys.h"

#include "usnic_direct.h"
#include "usd.h"
#include "usdf.h"
#include "usdf_wait.h"
#include "ofi_util.h"

static inline int
usdf_eq_empty(struct usdf_eq *eq)
{
	return (ofi_atomic_get32(&eq->eq_num_events) == 0);
}

static inline int
usdf_eq_error(struct usdf_eq *eq)
{
	return ((eq->eq_ev_tail->ue_flags & USDF_EVENT_FLAG_ERROR) != 0);
}

/*
 * read an event from the ring.  Caller must hold eq lock, and caller
 * needs to have checked for empty and error
 */
static inline ssize_t usdf_eq_read_event(struct usdf_eq *eq, uint32_t *event,
		void *buf, size_t len, uint64_t flags)
{
	struct usdf_event *ev;
	size_t copylen;
	ssize_t nbytes;
	uint64_t val;

	ev = eq->eq_ev_tail;

	copylen = MIN(ev->ue_len, len);

	if (copylen < ev->ue_len) {
		USDF_WARN_SYS(EQ,
				"buffer too small, got: %zu needed %zu\n",
				copylen, ev->ue_len);
		return -FI_ETOOSMALL;
	}

	/* copy out the event */
	if (event)
		*event = ev->ue_event;

	memcpy(buf, ev->ue_buf, copylen);

	if (!(flags & FI_PEEK)) {
		/* update count */
		ofi_atomic_dec32(&eq->eq_num_events);

		/* Free the event buf if needed */
		if (ev->ue_flags & USDF_EVENT_FLAG_FREE_BUF)
			free(ev->ue_buf);

		/* new tail */
		eq->eq_ev_tail++;
		if (eq->eq_ev_tail >= eq->eq_ev_end)
			eq->eq_ev_tail = eq->eq_ev_ring;

		/* consume the event in eventfd */
		if (eq->eq_attr.wait_obj == FI_WAIT_FD) {
			nbytes = read(eq->eq_fd, &val, sizeof(val));
			if (nbytes != sizeof(val))
				return -errno;
		}
	}

	return copylen;
}

/*
 * unconditionally write an event to the EQ.  Caller is responsible for
 * ensuring there is room.  EQ must be locked.
 */
static inline ssize_t
usdf_eq_write_event(struct usdf_eq *eq, uint32_t event,
		const void *buf, size_t len, uint64_t flags)
{
	struct usdf_event *ev;
	void *ev_buf;

	ev = eq->eq_ev_head;
	ev->ue_event = event;
	ev->ue_len = len;
	ev->ue_flags = flags;

	/* save the event data if we can, else malloc() */
	if (len <= sizeof(struct fi_eq_entry)) {
		ev_buf = eq->eq_ev_buf + (ev - eq->eq_ev_ring);
	} else {
		ev_buf = malloc(len);
		if (ev_buf == NULL) {
			return -errno;
		}
		ev->ue_flags |= USDF_EVENT_FLAG_FREE_BUF;
	}
	memcpy(ev_buf, buf, len);
	ev->ue_buf = ev_buf;

	/* new head */
	eq->eq_ev_head++;
	if (eq->eq_ev_head >= eq->eq_ev_end) {
		eq->eq_ev_head = eq->eq_ev_ring;
	}

	/* increment queued event count */
	ofi_atomic_inc32(&eq->eq_num_events);

	return len;
}

static void usdf_eq_clean_err(struct usdf_eq *eq, uint8_t destroy)
{
	struct usdf_err_data_entry *err_data_entry;
	struct slist_entry *entry;

	while (!slist_empty(&eq->eq_err_data)) {
		entry = slist_remove_head(&eq->eq_err_data);
		err_data_entry = container_of(entry, struct usdf_err_data_entry,
				entry);
		if (err_data_entry->seen || destroy) {
			free(err_data_entry);
		} else {
			/* Oops, the rest hasn't been seen yet. Put this back
			 * and exit.
			 */
			slist_insert_head(entry, &eq->eq_err_data);
			break;
		}
	}
}

static ssize_t usdf_eq_readerr(struct fid_eq *feq,
		struct fi_eq_err_entry *given_buffer, uint64_t flags)
{
	struct usdf_err_data_entry *err_data_entry;
	struct fi_eq_err_entry entry;
	struct usdf_eq *eq;
	ssize_t ret, err_data_size;
	uint32_t api_version;
	void *err_data = NULL;

	USDF_TRACE_SYS(EQ, "\n");

	if (!feq) {
		USDF_DBG_SYS(EQ, "invalid input\n");
		return -FI_EINVAL;
	}

	eq = eq_ftou(feq);

	pthread_spin_lock(&eq->eq_lock);

	/* make sure there is an error on top */
	if (usdf_eq_empty(eq) || !usdf_eq_error(eq)) {
		pthread_spin_unlock(&eq->eq_lock);
		ret = -FI_EAGAIN;
		goto done;
	}

	ret = usdf_eq_read_event(eq, NULL, &entry, sizeof(entry), flags);

	pthread_spin_unlock(&eq->eq_lock);

	/* read the user's setting for err_data. */
	err_data = given_buffer->err_data;
	err_data_size = given_buffer->err_data_size;

	/* Copy the entry. */
	*given_buffer = entry;

	/* Mark as seen so it can be cleaned on the next iteration of read. */
	if (entry.err_data_size) {
		err_data_entry = container_of(entry.err_data,
				struct usdf_err_data_entry, err_data);
		err_data_entry->seen = 1;
	}


	/* For release > 1.5, we will copy the err_data directly
	 * to the user's buffer.
	 */
	api_version = eq->eq_fabric->fab_attr.fabric->api_version;
	if (FI_VERSION_GE(api_version, FI_VERSION(1, 5))) {
		given_buffer->err_data = err_data;
		given_buffer->err_data_size =
			MIN(err_data_size, entry.err_data_size);
		memcpy(given_buffer->err_data, entry.err_data,
				given_buffer->err_data_size);

		if (err_data_size < entry.err_data_size) {
			USDF_DBG_SYS(EQ, "err_data truncated by %zd bytes.\n",
				entry.err_data_size - err_data_size);
		}

		usdf_eq_clean_err(eq, 0);
	}

done:
	return ret;
}

static ssize_t _usdf_eq_read(struct usdf_eq *eq, uint32_t *event, void *buf,
		size_t len, uint64_t flags)
{
	ssize_t ret;

	pthread_spin_lock(&eq->eq_lock);

	if (usdf_eq_empty(eq)) {
		ret = -FI_EAGAIN;
		goto done;
	}

	if (usdf_eq_error(eq)) {
		ret = -FI_EAVAIL;
		goto done;
	}

	if (!slist_empty(&eq->eq_err_data))
		usdf_eq_clean_err(eq, 0);

	ret = usdf_eq_read_event(eq, event, buf, len, flags);

done:
	pthread_spin_unlock(&eq->eq_lock);
	return ret;
}

static ssize_t usdf_eq_read(struct fid_eq *feq, uint32_t *event, void *buf,
		size_t len, uint64_t flags)
{
	struct usdf_eq *eq;

	USDF_DBG_SYS(EQ, "\n");

	eq = eq_ftou(feq);

	/* Don't bother acquiring the lock if there is nothing to read. */
	if (usdf_eq_empty(eq))
		return -FI_EAGAIN;

	return _usdf_eq_read(eq, event, buf, len, flags);
}

/* TODO: The timeout handling seems off on this one. */
static ssize_t usdf_eq_sread_fd(struct fid_eq *feq, uint32_t *event, void *buf,
		size_t len, int timeout, uint64_t flags)
{
	struct usdf_eq *eq;
	struct pollfd pfd;
	int ret;

	USDF_DBG_SYS(EQ, "\n");

	eq = eq_ftou(feq);

	/* Setup poll context to block until the FD becomes readable. */
	pfd.fd = eq->eq_fd;
	pfd.events = POLLIN;

retry:
	ret = poll(&pfd, 1, timeout);
	if (ret < 0)
		return -errno;
	else if (ret == 0)
		return -FI_EAGAIN;

	ret = _usdf_eq_read(eq, event, buf, len, flags);
	if (ret == -FI_EAGAIN)
		goto retry;

	return ret;
}

ssize_t usdf_eq_write_internal(struct usdf_eq *eq, uint32_t event,
		const void *buf, size_t len, uint64_t flags)
{
	uint64_t val = 1;
	int ret;
	int n;

	USDF_DBG_SYS(EQ, "event=%#" PRIx32 " flags=%#" PRIx64 "\n", event,
			flags);

	pthread_spin_lock(&eq->eq_lock);

	/* Return -FI_EAGAIN if the EQ is full.
	 * TODO: Disable the EQ.
	 */
	if (ofi_atomic_get32(&eq->eq_num_events) == eq->eq_ev_ring_size) {
		ret = -FI_EAGAIN;
		goto done;
	}

	ret = usdf_eq_write_event(eq, event, buf, len, flags);

	/* If successful, post to eventfd */
	if (ret >= 0 && eq->eq_attr.wait_obj == FI_WAIT_FD) {
		n = write(eq->eq_fd, &val, sizeof(val));

		/* TODO: If the write call fails, then roll back the EQ entry.
		 */
		if (n != sizeof(val))
			ret = -FI_EIO;
	}

done:
	pthread_spin_unlock(&eq->eq_lock);
	return ret;
}

static ssize_t usdf_eq_write(struct fid_eq *feq, uint32_t event,
		const void *buf, size_t len, uint64_t flags)
{
	struct usdf_eq *eq;

	USDF_DBG_SYS(EQ, "\n");

	if (!feq) {
		USDF_DBG_SYS(EQ, "invalid input\n");
		return -FI_EINVAL;
	}

	eq = eq_ftou(feq);

	return usdf_eq_write_internal(eq, event, buf, len, flags);
}

static const char *
usdf_eq_strerror(struct fid_eq *feq, int prov_errno, const void *err_data,
		 char *buf, size_t len)
{
	return NULL;
}

static int usdf_eq_get_wait(struct usdf_eq *eq, void *arg)
{
	USDF_TRACE_SYS(EQ, "\n");

	switch (eq->eq_attr.wait_obj) {
	case FI_WAIT_FD:
		*(int *) arg = eq->eq_fd;
		break;
	default:
		USDF_WARN_SYS(EQ, "unsupported wait type\n");
		return -FI_EINVAL;
	}

	return FI_SUCCESS;
}

static int
usdf_eq_control(fid_t fid, int command, void *arg)
{
	struct usdf_eq *eq;

	USDF_TRACE_SYS(EQ, "\n");

	eq = eq_fidtou(fid);

	switch (command) {
	case FI_GETWAIT:
		break;
	default:
		return -FI_EINVAL;
	}

	return usdf_eq_get_wait(eq, arg);
}

static int usdf_eq_bind_wait(struct usdf_eq *eq)
{
	int ret;
	struct usdf_wait *wait_priv;

	if (!eq->eq_attr.wait_set) {
		USDF_DBG_SYS(EQ, "can't bind to non-existent wait set\n");
		return -FI_EINVAL;
	}

	wait_priv = wait_ftou(eq->eq_attr.wait_set);

	ret = fid_list_insert(&wait_priv->list, &wait_priv->lock,
			&eq->eq_fid.fid);
	if (ret) {
		USDF_WARN_SYS(EQ,
				"failed to associate eq with wait fid list\n");
		return ret;
	}

	ret = ofi_epoll_add(wait_priv->object.epfd, eq->eq_fd, OFI_EPOLL_IN, eq);
	if (ret) {
		USDF_WARN_SYS(EQ, "failed to associate FD with wait set\n");
		goto err;
	}

	USDF_DBG_SYS(EQ, "associated EQ FD %d with epoll FD %d using fid %p\n",
			eq->eq_fd, wait_priv->object.epfd, &eq->eq_fid.fid);

	return ret;

err:
	fid_list_remove(&wait_priv->list, &wait_priv->lock, &eq->eq_fid.fid);
	return ret;
}

static int usdf_eq_unbind_wait(struct usdf_eq *eq)
{
	int ret;
	struct usdf_wait *wait_priv;

	if (!eq->eq_attr.wait_set) {
		USDF_DBG_SYS(EQ, "can't unbind from non-existent wait set\n");
		return -FI_EINVAL;
	}

	wait_priv = wait_ftou(eq->eq_attr.wait_set);

	ret = ofi_epoll_del(wait_priv->object.epfd, eq->eq_fd);
	if (ret) {
		USDF_WARN_SYS(EQ,
				"failed to remove FD from wait set\n");
		return ret;
	}

	fid_list_remove(&wait_priv->list, &wait_priv->lock, &eq->eq_fid.fid);

	ofi_atomic_dec32(&wait_priv->wait_refcnt);

	USDF_DBG_SYS(EQ,
			"dissasociated EQ FD %d from epoll FD %d using FID: %p\n",
			eq->eq_fd, wait_priv->object.epfd, &eq->eq_fid.fid);

	return FI_SUCCESS;
}

static int
usdf_eq_close(fid_t fid)
{
	struct usdf_eq *eq;
	int ret = FI_SUCCESS;

	USDF_TRACE_SYS(EQ, "\n");

	eq = eq_fidtou(fid);

	if (ofi_atomic_get32(&eq->eq_refcnt) > 0) {
		return -FI_EBUSY;
	}
	ofi_atomic_dec32(&eq->eq_fabric->fab_refcnt);

	/* release wait obj */
	switch (eq->eq_attr.wait_obj) {
	case FI_WAIT_SET:
		ret = usdf_eq_unbind_wait(eq);
		/* FALLTHROUGH */
		/* Need to close the FD used for wait set. */
	case FI_WAIT_FD:
		close(eq->eq_fd);
		break;
	default:
		break;
	}

	/* Set destroy flag to clear everything out */
	usdf_eq_clean_err(eq, 1);

	free(eq->eq_ev_ring);
	free(eq->eq_ev_buf);
	free(eq);

	return ret;
}

static struct fi_ops_eq usdf_eq_ops = {
	.size = sizeof(struct fi_ops_eq),
	.read = usdf_eq_read,
	.readerr = usdf_eq_readerr,
	.write = usdf_eq_write,
	.sread = fi_no_eq_sread,
	.strerror = usdf_eq_strerror,
};

static struct fi_ops usdf_eq_fi_ops = {
	.size = sizeof(struct fi_ops),
	.close = usdf_eq_close,
	.bind = fi_no_bind,
	.control = usdf_eq_control,
	.ops_open = fi_no_ops_open,
};

int
usdf_eq_open(struct fid_fabric *fabric, struct fi_eq_attr *attr,
	struct fid_eq **feq, void *context)
{
	struct usdf_eq *eq;
	struct usdf_fabric *fab;
	int ret;

	USDF_TRACE_SYS(EQ, "\n");

	fab = fab_ftou(fabric);

	eq = calloc(1, sizeof(*eq));
	if (eq == NULL) {
		ret = -errno;
		goto fail;
	}

	/* fill in the EQ struct */
	eq->eq_fid.fid.fclass = FI_CLASS_EQ;
	eq->eq_fid.fid.context = context;
	eq->eq_fid.fid.ops = &usdf_eq_fi_ops;
	eq->eq_fid.ops = &eq->eq_ops_data;

	eq->eq_fabric = fab;
	ofi_atomic_initialize32(&eq->eq_refcnt, 0);
	ret = pthread_spin_init(&eq->eq_lock, PTHREAD_PROCESS_PRIVATE);
	if (ret != 0) {
		ret = -ret;
		goto fail;
	}

	slist_init(&eq->eq_err_data);

	/* get baseline routines */
	eq->eq_ops_data = usdf_eq_ops;

	/* fill in sread based on wait type */
	switch (attr->wait_obj) {
	case FI_WAIT_NONE:
		break;
	case FI_WAIT_UNSPEC:
		/* default to FD */
		attr->wait_obj = FI_WAIT_FD;
		/* FALLTHROUGH */
	case FI_WAIT_FD:
		eq->eq_ops_data.sread = usdf_eq_sread_fd;
		/* FALLTHROUGH  */
		/* Don't set sread for wait set. */
	case FI_WAIT_SET:
		eq->eq_fd = eventfd(0, EFD_NONBLOCK | EFD_SEMAPHORE);
		if (eq->eq_fd == -1) {
			ret = -errno;
			goto fail;
		}

		if (attr->wait_obj == FI_WAIT_SET) {
			ret = usdf_eq_bind_wait(eq);
			if (ret)
				goto fail;
		}
		break;
	default:
		ret = -FI_ENOSYS;
		goto fail;
	}

	/*
	 * Dis-allow write if requested
	 */
	if ((attr->flags & FI_WRITE) == 0) {
		eq->eq_ops_data.write = fi_no_eq_write;
	}

	/*
	 * Allocate and initialize event ring
	 */
	if (attr->size == 0) {
		attr->size = 1024;	// XXX
	}
	eq->eq_ev_ring = calloc(attr->size, sizeof(*eq->eq_ev_ring));
	eq->eq_ev_buf = calloc(attr->size, sizeof(*eq->eq_ev_buf));
	if (eq->eq_ev_ring == NULL || eq->eq_ev_buf == NULL) {
		ret = -errno;
		goto fail;
	}
	eq->eq_ev_head = eq->eq_ev_ring;
	eq->eq_ev_tail = eq->eq_ev_ring;
	eq->eq_ev_ring_size = attr->size;
	eq->eq_ev_end = eq->eq_ev_ring + eq->eq_ev_ring_size;
	ofi_atomic_initialize32(&eq->eq_num_events, 0);

	ofi_atomic_inc32(&eq->eq_fabric->fab_refcnt);

	eq->eq_attr = *attr;
	*feq = eq_utof(eq);

	return 0;

fail:
	if (eq != NULL) {
		free(eq->eq_ev_ring);
		free(eq->eq_ev_buf);
		free(eq);
	}
	return ret;
}
