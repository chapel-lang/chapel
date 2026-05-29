/*
 * Copyright (c) 2014 Intel Corporation, Inc.  All rights reserved.
 * Copyright (c) 2016 Cisco Systems, Inc.  All rights reserved.
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <sys/types.h>

#include <ofi_list.h>
#include <ofi.h>

#include "sock.h"
#include "sock_util.h"

#define SOCK_LOG_DBG(...) _SOCK_LOG_DBG(FI_LOG_CQ, __VA_ARGS__)
#define SOCK_LOG_ERROR(...) _SOCK_LOG_ERROR(FI_LOG_CQ, __VA_ARGS__)

void sock_cq_add_tx_ctx(struct sock_cq *cq, struct sock_tx_ctx *tx_ctx)
{
	struct dlist_entry *entry;
	struct sock_tx_ctx *curr_ctx;
	pthread_mutex_lock(&cq->list_lock);
	for (entry = cq->tx_list.next; entry != &cq->tx_list;
	     entry = entry->next) {
		curr_ctx = container_of(entry, struct sock_tx_ctx, cq_entry);
		if (tx_ctx == curr_ctx)
			goto out;
	}
	dlist_insert_tail(&tx_ctx->cq_entry, &cq->tx_list);
	ofi_atomic_inc32(&cq->ref);
out:
	pthread_mutex_unlock(&cq->list_lock);
}

void sock_cq_remove_tx_ctx(struct sock_cq *cq, struct sock_tx_ctx *tx_ctx)
{
	pthread_mutex_lock(&cq->list_lock);
	dlist_remove(&tx_ctx->cq_entry);
	ofi_atomic_dec32(&cq->ref);
	pthread_mutex_unlock(&cq->list_lock);
}

void sock_cq_add_rx_ctx(struct sock_cq *cq, struct sock_rx_ctx *rx_ctx)
{
	struct dlist_entry *entry;
	struct sock_rx_ctx *curr_ctx;
	pthread_mutex_lock(&cq->list_lock);

	for (entry = cq->rx_list.next; entry != &cq->rx_list;
	     entry = entry->next) {
		curr_ctx = container_of(entry, struct sock_rx_ctx, cq_entry);
		if (rx_ctx == curr_ctx)
			goto out;
	}
	dlist_insert_tail(&rx_ctx->cq_entry, &cq->rx_list);
	ofi_atomic_inc32(&cq->ref);
out:
	pthread_mutex_unlock(&cq->list_lock);
}

void sock_cq_remove_rx_ctx(struct sock_cq *cq, struct sock_rx_ctx *rx_ctx)
{
	pthread_mutex_lock(&cq->list_lock);
	dlist_remove(&rx_ctx->cq_entry);
	ofi_atomic_dec32(&cq->ref);
	pthread_mutex_unlock(&cq->list_lock);
}

int sock_cq_progress(struct sock_cq *cq)
{
	struct sock_tx_ctx *tx_ctx;
	struct sock_rx_ctx *rx_ctx;
	struct dlist_entry *entry;

	if (cq->domain->progress_mode == FI_PROGRESS_AUTO)
		return 0;

	pthread_mutex_lock(&cq->list_lock);
	for (entry = cq->tx_list.next; entry != &cq->tx_list;
	     entry = entry->next) {
		tx_ctx = container_of(entry, struct sock_tx_ctx, cq_entry);
		if (!tx_ctx->enabled)
			continue;

		if (tx_ctx->use_shared)
			sock_pe_progress_tx_ctx(cq->domain->pe, tx_ctx->stx_ctx);
		else
			sock_pe_progress_ep_tx(cq->domain->pe, tx_ctx->ep_attr);
	}

	for (entry = cq->rx_list.next; entry != &cq->rx_list;
	     entry = entry->next) {
		rx_ctx = container_of(entry, struct sock_rx_ctx, cq_entry);
		if (!rx_ctx->enabled)
			continue;

		if (rx_ctx->use_shared)
			sock_pe_progress_rx_ctx(cq->domain->pe, rx_ctx->srx_ctx);
		else
			sock_pe_progress_ep_rx(cq->domain->pe, rx_ctx->ep_attr);
	}
	pthread_mutex_unlock(&cq->list_lock);

	return 0;
}

static ssize_t sock_cq_entry_size(struct sock_cq *sock_cq)
{
	ssize_t size;

	switch (sock_cq->attr.format) {
	case FI_CQ_FORMAT_CONTEXT:
		size = sizeof(struct fi_cq_entry);
		break;

	case FI_CQ_FORMAT_MSG:
		size = sizeof(struct fi_cq_msg_entry);
		break;

	case FI_CQ_FORMAT_DATA:
		size = sizeof(struct fi_cq_data_entry);
		break;

	case FI_CQ_FORMAT_TAGGED:
		size = sizeof(struct fi_cq_tagged_entry);
		break;

	case FI_CQ_FORMAT_UNSPEC:
	default:
		size = -1;
		SOCK_LOG_ERROR("Invalid CQ format\n");
		break;
	}
	return size;
}

static ssize_t _sock_cq_write(struct sock_cq *cq, fi_addr_t addr,
			      const void *buf, size_t len)
{
	ssize_t ret;
	struct sock_cq_overflow_entry_t *overflow_entry;

	pthread_mutex_lock(&cq->lock);
	if (ofi_rbfdavail(&cq->cq_rbfd) < len) {
		SOCK_LOG_ERROR("Not enough space in CQ\n");
		overflow_entry = calloc(1, sizeof(*overflow_entry) + len);
		if (!overflow_entry) {
			ret = -FI_ENOSPC;
			goto out;
		}

		memcpy(&overflow_entry->cq_entry[0], buf, len);
		overflow_entry->len = len;
		overflow_entry->addr = addr;
		dlist_insert_tail(&overflow_entry->entry, &cq->overflow_list);
		ret = len;
		goto out;
	}


	ofi_rbwrite(&cq->addr_rb, &addr, sizeof(addr));
	ofi_rbcommit(&cq->addr_rb);

	ofi_rbfdwrite(&cq->cq_rbfd, buf, len);
	if (cq->domain->progress_mode == FI_PROGRESS_MANUAL)
		ofi_rbcommit(&cq->cq_rbfd.rb);
	else
		ofi_rbfdcommit(&cq->cq_rbfd);

	ret = len;

	if (cq->signal)
		sock_wait_signal(cq->waitset);
out:
	pthread_mutex_unlock(&cq->lock);
	return ret;
}

static ssize_t sock_cq_report_context(struct sock_cq *cq, fi_addr_t addr,
				      struct sock_pe_entry *pe_entry)
{
	struct fi_cq_entry cq_entry;
	cq_entry.op_context = (void *) (uintptr_t) pe_entry->context;
	return _sock_cq_write(cq, addr, &cq_entry, sizeof(cq_entry));
}

static uint64_t sock_cq_sanitize_flags(uint64_t flags)
{
	return (flags & (FI_SEND | FI_RECV | FI_RMA | FI_ATOMIC |
				FI_MSG | FI_TAGGED |
				FI_READ | FI_WRITE |
				FI_REMOTE_READ | FI_REMOTE_WRITE |
				FI_REMOTE_CQ_DATA | FI_MULTI_RECV));
}

static ssize_t sock_cq_report_msg(struct sock_cq *cq, fi_addr_t addr,
			          struct sock_pe_entry *pe_entry)
{
	struct fi_cq_msg_entry cq_entry;
	cq_entry.op_context = (void *) (uintptr_t) pe_entry->context;
	cq_entry.flags = sock_cq_sanitize_flags(pe_entry->flags);
	cq_entry.len = pe_entry->data_len;
	return _sock_cq_write(cq, addr, &cq_entry, sizeof(cq_entry));
}

static ssize_t sock_cq_report_data(struct sock_cq *cq, fi_addr_t addr,
			           struct sock_pe_entry *pe_entry)
{
	struct fi_cq_data_entry cq_entry;
	cq_entry.op_context = (void *) (uintptr_t) pe_entry->context;
	cq_entry.flags = sock_cq_sanitize_flags(pe_entry->flags);
	cq_entry.len = pe_entry->data_len;
	cq_entry.buf = (void *) (uintptr_t) pe_entry->buf;
	cq_entry.data = pe_entry->data;
	return _sock_cq_write(cq, addr, &cq_entry, sizeof(cq_entry));
}

static ssize_t sock_cq_report_tagged(struct sock_cq *cq, fi_addr_t addr,
				     struct sock_pe_entry *pe_entry)
{
	struct fi_cq_tagged_entry cq_entry;
	cq_entry.op_context = (void *) (uintptr_t) pe_entry->context;
	cq_entry.flags = sock_cq_sanitize_flags(pe_entry->flags);
	cq_entry.len = pe_entry->data_len;
	cq_entry.buf = (void *) (uintptr_t) pe_entry->buf;
	cq_entry.data = pe_entry->data;
	cq_entry.tag = pe_entry->tag;
	return _sock_cq_write(cq, addr, &cq_entry, sizeof(cq_entry));
}

static void sock_cq_set_report_fn(struct sock_cq *sock_cq)
{
	switch (sock_cq->attr.format) {
	case FI_CQ_FORMAT_CONTEXT:
		sock_cq->report_completion = &sock_cq_report_context;
		break;

	case FI_CQ_FORMAT_MSG:
		sock_cq->report_completion = &sock_cq_report_msg;
		break;

	case FI_CQ_FORMAT_DATA:
		sock_cq->report_completion = &sock_cq_report_data;
		break;

	case FI_CQ_FORMAT_TAGGED:
		sock_cq->report_completion = &sock_cq_report_tagged;
		break;

	case FI_CQ_FORMAT_UNSPEC:
	default:
		SOCK_LOG_ERROR("Invalid CQ format\n");
		break;
	}
}

static inline void sock_cq_copy_overflow_list(struct sock_cq *cq, size_t count)
{
	size_t i;
	struct sock_cq_overflow_entry_t *overflow_entry;

	for (i = 0; i < count && !dlist_empty(&cq->overflow_list); i++) {
		overflow_entry = container_of(cq->overflow_list.next,
					      struct sock_cq_overflow_entry_t,
					      entry);
		ofi_rbwrite(&cq->addr_rb, &overflow_entry->addr, sizeof(fi_addr_t));
		ofi_rbcommit(&cq->addr_rb);

		ofi_rbfdwrite(&cq->cq_rbfd, &overflow_entry->cq_entry[0], overflow_entry->len);
		if (cq->domain->progress_mode == FI_PROGRESS_MANUAL)
			ofi_rbcommit(&cq->cq_rbfd.rb);
		else
			ofi_rbfdcommit(&cq->cq_rbfd);

		dlist_remove(&overflow_entry->entry);
		free(overflow_entry);
	}
}

static inline ssize_t sock_cq_rbuf_read(struct sock_cq *cq, void *buf,
					size_t count, fi_addr_t *src_addr,
					size_t cq_entry_len)
{
	size_t i;
	fi_addr_t addr;

	ofi_rbfdread(&cq->cq_rbfd, buf, cq_entry_len * count);
	for (i = 0; i < count; i++) {
		ofi_rbread(&cq->addr_rb, &addr, sizeof(addr));
		if (src_addr)
			src_addr[i] = addr;
	}
	sock_cq_copy_overflow_list(cq, count);
	return count;
}

static ssize_t sock_cq_sreadfrom(struct fid_cq *cq, void *buf, size_t count,
			fi_addr_t *src_addr, const void *cond, int timeout)
{
	ssize_t ret = 0;
	size_t threshold;
	struct sock_cq *sock_cq;
	uint64_t start_ms;
	ssize_t cq_entry_len, avail;

	sock_cq = container_of(cq, struct sock_cq, cq_fid);
	pthread_mutex_lock(&sock_cq->lock);
	if (ofi_rbused(&sock_cq->cqerr_rb))
		ret = -FI_EAVAIL;
	pthread_mutex_unlock(&sock_cq->lock);
	if (ret)
		return ret;

	cq_entry_len = sock_cq->cq_entry_size;
	if (sock_cq->attr.wait_cond == FI_CQ_COND_THRESHOLD)
		threshold = MIN((uintptr_t) cond, count);
	else
		threshold = count;

	start_ms = (timeout >= 0) ? ofi_gettime_ms() : 0;

	if (sock_cq->domain->progress_mode == FI_PROGRESS_MANUAL) {
		while (1) {
			sock_cq_progress(sock_cq);
			pthread_mutex_lock(&sock_cq->lock);
			avail = ofi_rbfdused(&sock_cq->cq_rbfd);
			if (avail) {
				ret = sock_cq_rbuf_read(sock_cq, buf,
					MIN(threshold, (size_t)(avail / cq_entry_len)),
					src_addr, cq_entry_len);
			}
			pthread_mutex_unlock(&sock_cq->lock);
			if (ret)
				return ret;

			if (timeout >= 0) {
				timeout -= (int) (ofi_gettime_ms() - start_ms);
				if (timeout <= 0)
					return -FI_EAGAIN;
			}

			if (ofi_atomic_get32(&sock_cq->signaled)) {
				ofi_atomic_set32(&sock_cq->signaled, 0);
				return -FI_ECANCELED;
			}
		};
	} else {
		do {
			pthread_mutex_lock(&sock_cq->lock);
			ret = 0;
			avail = ofi_rbfdused(&sock_cq->cq_rbfd);
			if (avail) {
				ret = sock_cq_rbuf_read(sock_cq, buf,
					MIN(threshold, (size_t)(avail / cq_entry_len)),
					src_addr, cq_entry_len);
			} else {
				ofi_rbfdreset(&sock_cq->cq_rbfd);
			}
			pthread_mutex_unlock(&sock_cq->lock);
			if (ret && ret != -FI_EAGAIN)
				return ret;

			if (timeout >= 0) {
				timeout -= (int) (ofi_gettime_ms() - start_ms);
				if (timeout <= 0)
					return -FI_EAGAIN;
			}

			if (ofi_atomic_get32(&sock_cq->signaled)) {
				ofi_atomic_set32(&sock_cq->signaled, 0);
				return -FI_ECANCELED;
			}
			ret = ofi_rbfdwait(&sock_cq->cq_rbfd, timeout);
		} while (ret > 0);
	}

	return (ret == 0 || ret == -FI_ETIMEDOUT || ret == -EINTR) ? -FI_EAGAIN : ret;
}

static ssize_t sock_cq_sread(struct fid_cq *cq, void *buf, size_t len,
			     const void *cond, int timeout)
{
	return sock_cq_sreadfrom(cq, buf, len, NULL, cond, timeout);
}

static ssize_t sock_cq_readfrom(struct fid_cq *cq, void *buf, size_t count,
			fi_addr_t *src_addr)
{
	return sock_cq_sreadfrom(cq, buf, count, src_addr, NULL, 0);
}

static ssize_t sock_cq_read(struct fid_cq *cq, void *buf, size_t count)
{
	return sock_cq_readfrom(cq, buf, count, NULL);
}

static ssize_t sock_cq_readerr(struct fid_cq *cq, struct fi_cq_err_entry *buf,
			uint64_t flags)
{
	struct sock_cq *sock_cq;
	ssize_t ret;
	struct fi_cq_err_entry entry;
	uint32_t api_version;
	size_t err_data_size = 0;
	void *err_data = NULL;

	sock_cq = container_of(cq, struct sock_cq, cq_fid);
	if (sock_cq->domain->progress_mode == FI_PROGRESS_MANUAL)
		sock_cq_progress(sock_cq);

	pthread_mutex_lock(&sock_cq->lock);
	if (ofi_rbused(&sock_cq->cqerr_rb) >= sizeof(struct fi_cq_err_entry)) {
		api_version = sock_cq->domain->fab->fab_fid.api_version;
		ofi_rbread(&sock_cq->cqerr_rb, &entry, sizeof(entry));

		if ((FI_VERSION_GE(api_version, FI_VERSION(1, 5)))
			&& buf->err_data && buf->err_data_size) {
			err_data = buf->err_data;
			err_data_size = buf->err_data_size;
			ofi_cq_err_memcpy(api_version, buf, &entry);

			buf->err_data = err_data;

			/* Fill provided user's buffer */
			buf->err_data_size = MIN(entry.err_data_size, err_data_size);
			memcpy(buf->err_data, entry.err_data, buf->err_data_size);
		} else {
			memcpy(buf, &entry, sizeof(struct fi_cq_err_entry_1_0));
		}

		ret = 1;
	} else {
		ret = -FI_EAGAIN;
	}
	pthread_mutex_unlock(&sock_cq->lock);
	return ret;
}

static const char *sock_cq_strerror(struct fid_cq *cq, int prov_errno,
			      const void *err_data, char *buf, size_t len)
{
	if (buf && len)
		return strncpy(buf, fi_strerror(-prov_errno), len);
	return fi_strerror(-prov_errno);
}

static int sock_cq_close(struct fid *fid)
{
	struct sock_cq *cq;

	cq = container_of(fid, struct sock_cq, cq_fid.fid);
	if (ofi_atomic_get32(&cq->ref))
		return -FI_EBUSY;

	if (cq->signal && cq->attr.wait_obj == FI_WAIT_MUTEX_COND)
		sock_wait_close(&cq->waitset->fid);

	ofi_rbfree(&cq->addr_rb);
	ofi_rbfree(&cq->cqerr_rb);
	ofi_rbfdfree(&cq->cq_rbfd);

	pthread_mutex_destroy(&cq->lock);
	pthread_mutex_destroy(&cq->list_lock);
	ofi_atomic_dec32(&cq->domain->ref);

	free(cq);
	return 0;
}

static int sock_cq_signal(struct fid_cq *cq)
{
	struct sock_cq *sock_cq;
	sock_cq = container_of(cq, struct sock_cq, cq_fid);

	ofi_atomic_set32(&sock_cq->signaled, 1);
	pthread_mutex_lock(&sock_cq->lock);
	ofi_rbfdsignal(&sock_cq->cq_rbfd);
	pthread_mutex_unlock(&sock_cq->lock);
	return 0;
}

static struct fi_ops_cq sock_cq_ops = {
	.size = sizeof(struct fi_ops_cq),
	.read = sock_cq_read,
	.readfrom = sock_cq_readfrom,
	.readerr = sock_cq_readerr,
	.sread = sock_cq_sread,
	.sreadfrom = sock_cq_sreadfrom,
	.signal = sock_cq_signal,
	.strerror = sock_cq_strerror,
};

static int sock_cq_control(struct fid *fid, int command, void *arg)
{
	struct sock_cq *cq;
	int ret = 0;

	cq = container_of(fid, struct sock_cq, cq_fid);
	switch (command) {
	case FI_GETWAIT:
		if (cq->domain->progress_mode == FI_PROGRESS_MANUAL)
			return -FI_ENOSYS;

		switch (cq->attr.wait_obj) {
		case FI_WAIT_NONE:
		case FI_WAIT_FD:
		case FI_WAIT_UNSPEC:
			memcpy(arg, &cq->cq_rbfd.fd[OFI_RB_READ_FD], sizeof(int));
			break;

		case FI_WAIT_SET:
		case FI_WAIT_MUTEX_COND:
			sock_wait_get_obj(cq->waitset, arg);
			break;

		default:
			ret = -FI_EINVAL;
			break;
		}
		break;

	default:
		ret =  -FI_EINVAL;
		break;
	}

	return ret;
}

static struct fi_ops sock_cq_fi_ops = {
	.size = sizeof(struct fi_ops),
	.close = sock_cq_close,
	.bind = fi_no_bind,
	.control = sock_cq_control,
	.ops_open = fi_no_ops_open,
};

static int sock_cq_verify_attr(struct fi_cq_attr *attr)
{
	if (!attr)
		return 0;

	switch (attr->format) {
	case FI_CQ_FORMAT_CONTEXT:
	case FI_CQ_FORMAT_MSG:
	case FI_CQ_FORMAT_DATA:
	case FI_CQ_FORMAT_TAGGED:
		break;
	case FI_CQ_FORMAT_UNSPEC:
		attr->format = FI_CQ_FORMAT_CONTEXT;
		break;
	default:
		return -FI_ENOSYS;
	}

	switch (attr->wait_obj) {
	case FI_WAIT_NONE:
	case FI_WAIT_FD:
	case FI_WAIT_SET:
	case FI_WAIT_MUTEX_COND:
		break;
	case FI_WAIT_UNSPEC:
		attr->wait_obj = FI_WAIT_FD;
		break;
	default:
		return -FI_ENOSYS;
	}

	return 0;
}

static struct fi_cq_attr _sock_cq_def_attr = {
	.size = SOCK_CQ_DEF_SZ,
	.flags = 0,
	.format = FI_CQ_FORMAT_CONTEXT,
	.wait_obj = FI_WAIT_FD,
	.signaling_vector = 0,
	.wait_cond = FI_CQ_COND_NONE,
	.wait_set = NULL,
};

int sock_cq_open(struct fid_domain *domain, struct fi_cq_attr *attr,
		 struct fid_cq **cq, void *context)
{
	struct sock_domain *sock_dom;
	struct sock_cq *sock_cq;
	struct fi_wait_attr wait_attr;
	struct sock_fid_list *list_entry;
	struct sock_wait *wait;
	int ret;
	ssize_t retsize;

	sock_dom = container_of(domain, struct sock_domain, dom_fid);
	ret = sock_cq_verify_attr(attr);
	if (ret)
		return ret;

	sock_cq = calloc(1, sizeof(*sock_cq));
	if (!sock_cq)
		return -FI_ENOMEM;

	ofi_atomic_initialize32(&sock_cq->ref, 0);
	ofi_atomic_initialize32(&sock_cq->signaled, 0);
	sock_cq->cq_fid.fid.fclass = FI_CLASS_CQ;
	sock_cq->cq_fid.fid.context = context;
	sock_cq->cq_fid.fid.ops = &sock_cq_fi_ops;
	sock_cq->cq_fid.ops = &sock_cq_ops;

	if (attr == NULL) {
		sock_cq->attr = _sock_cq_def_attr;
	} else {
		sock_cq->attr = *attr;
		if (attr->size == 0)
			sock_cq->attr.size = _sock_cq_def_attr.size;
	}

	sock_cq->domain = sock_dom;
	retsize = sock_cq_entry_size(sock_cq);
	if (retsize < 0) {
		ret = (int) retsize;
		goto err1;
	}

	sock_cq->cq_entry_size = retsize;
	sock_cq_set_report_fn(sock_cq);

	dlist_init(&sock_cq->tx_list);
	dlist_init(&sock_cq->rx_list);
	dlist_init(&sock_cq->ep_list);
	dlist_init(&sock_cq->overflow_list);

	ret = ofi_rbfdinit(&sock_cq->cq_rbfd, sock_cq->attr.size *
			sock_cq->cq_entry_size);
	if (ret)
		goto err1;

	ret = ofi_rbinit(&sock_cq->addr_rb,
			sock_cq->attr.size * sizeof(fi_addr_t));
	if (ret)
		goto err2;

	ret = ofi_rbinit(&sock_cq->cqerr_rb, sock_cq->attr.size *
			sizeof(struct fi_cq_err_entry));
	if (ret)
		goto err3;

	pthread_mutex_init(&sock_cq->lock, NULL);

	switch (sock_cq->attr.wait_obj) {
	case FI_WAIT_NONE:
	case FI_WAIT_UNSPEC:
	case FI_WAIT_FD:
		break;

	case FI_WAIT_MUTEX_COND:
		wait_attr.flags = 0;
		wait_attr.wait_obj = FI_WAIT_MUTEX_COND;
		ret = sock_wait_open(&sock_dom->fab->fab_fid, &wait_attr,
				     &sock_cq->waitset);
		if (ret) {
			ret = -FI_EINVAL;
			goto err4;
		}
		sock_cq->signal = 1;
		break;

	case FI_WAIT_SET:
		if (!attr) {
			ret = -FI_EINVAL;
			goto err4;
		}

		sock_cq->waitset = attr->wait_set;
		sock_cq->signal = 1;
		wait = container_of(attr->wait_set, struct sock_wait, wait_fid);
		list_entry = calloc(1, sizeof(*list_entry));
		if (!list_entry) {
                        ret = -FI_ENOMEM;
                        goto err4;
                }
		dlist_init(&list_entry->entry);
		list_entry->fid = &sock_cq->cq_fid.fid;
		dlist_insert_after(&list_entry->entry, &wait->fid_list);
		break;

	default:
		break;
	}

	*cq = &sock_cq->cq_fid;
	ofi_atomic_inc32(&sock_dom->ref);
	pthread_mutex_init(&sock_cq->list_lock, NULL);

	return 0;

err4:
	ofi_rbfree(&sock_cq->cqerr_rb);
err3:
	ofi_rbfree(&sock_cq->addr_rb);
err2:
	ofi_rbfdfree(&sock_cq->cq_rbfd);
err1:
	free(sock_cq);
	return ret;
}

int sock_cq_report_error(struct sock_cq *cq, struct sock_pe_entry *entry,
			 size_t olen, int err, int prov_errno, void *err_data,
			 size_t err_data_size)
{
	int ret;
	struct fi_cq_err_entry err_entry = {0};

	pthread_mutex_lock(&cq->lock);
	if (ofi_rbavail(&cq->cqerr_rb) < sizeof(err_entry)) {
		ret = -FI_ENOSPC;
		goto out;
	}

	err_entry.err = err;
	err_entry.olen = olen;
	err_entry.err_data = err_data;
	err_entry.err_data_size = err_data_size;
	err_entry.len = entry->data_len;
	err_entry.prov_errno = prov_errno;
	err_entry.flags = entry->flags;
	err_entry.data = entry->data;
	err_entry.tag = entry->tag;
	err_entry.op_context = (void *) (uintptr_t) entry->context;

	if (entry->type == SOCK_PE_RX)
		err_entry.buf = (void *) (uintptr_t) entry->pe.rx.rx_iov[0].iov.addr;
	else
		err_entry.buf = (void *) (uintptr_t) entry->pe.tx.tx_iov[0].src.iov.addr;

	ofi_rbwrite(&cq->cqerr_rb, &err_entry, sizeof(err_entry));
	ofi_rbcommit(&cq->cqerr_rb);
	ret = 0;

	ofi_rbfdsignal(&cq->cq_rbfd);

out:
	pthread_mutex_unlock(&cq->lock);
	return ret;
}
