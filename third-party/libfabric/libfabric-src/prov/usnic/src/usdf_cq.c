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
#include <ofi_epoll.h>

#include <rdma/fabric.h>
#include <rdma/fi_cm.h>
#include <rdma/fi_domain.h>
#include <rdma/fi_endpoint.h>
#include <rdma/fi_rma.h>
#include <rdma/fi_errno.h>
#include <rdma/fi_eq.h>
#include <ofi_util.h>
#include "ofi.h"
#include "ofi_enosys.h"

#include "usnic_direct.h"
#include "usd.h"
#include "usdf.h"
#include "usdf_av.h"
#include "usdf_progress.h"
#include "usdf_cq.h"
#include "usd_ib_cmd.h"
#include "usdf_wait.h"

static inline int usdf_cqe_to_flags(struct usd_completion *comp)
{
	switch (comp->uc_type) {
	case USD_COMPTYPE_SEND:
		return (FI_MSG | FI_SEND);
	case USD_COMPTYPE_RECV:
		return (FI_MSG | FI_RECV);
	default:
		USDF_DBG_SYS(CQ, "WARNING: unknown completion type! (%d)\n",
				comp->uc_type);
		return 0;
	}

}

static ssize_t
usdf_cq_readerr(struct fid_cq *fcq, struct fi_cq_err_entry *entry,
	        uint64_t flags)
{
	struct usdf_cq *cq;
	uint32_t api_version;

	USDF_TRACE_SYS(CQ, "\n");

	cq = container_of(fcq, struct usdf_cq, cq_fid);
	api_version = cq->cq_domain->dom_fabric->fab_attr.fabric->api_version;

	// The return values are analogous to sockets cq_readerr
	if (cq->cq_comp.uc_status == 0) {
		return -FI_EAGAIN;
	}

	entry->op_context = cq->cq_comp.uc_context;
	entry->flags = 0;
	switch (cq->cq_comp.uc_status) {
	case USD_COMPSTAT_SUCCESS:
		entry->prov_errno = FI_SUCCESS;
		break;
	case USD_COMPSTAT_ERROR_CRC:
		entry->prov_errno = FI_ECRC;
		break;
	case USD_COMPSTAT_ERROR_TRUNC:
		entry->prov_errno = FI_ETRUNC;
		break;
	case USD_COMPSTAT_ERROR_TIMEOUT:
		entry->prov_errno = FI_ETIMEDOUT;
		break;
	case USD_COMPSTAT_ERROR_INTERNAL:
	default:
		entry->prov_errno = FI_EOTHER;
		break;
	}
	entry->err = entry->prov_errno;

	cq->cq_comp.uc_status = 0;

	/* We don't have err_data to give back to the user. */
	if (FI_VERSION_GE(api_version, FI_VERSION(1, 5)))
		entry->err_data_size = 0;

	return 1;
}

static ssize_t
usdf_cq_readerr_soft(struct fid_cq *fcq, struct fi_cq_err_entry *entry,
		uint64_t flags)
{
	struct usdf_cq *cq;
	struct usdf_cq_soft_entry *tail;

	USDF_TRACE_SYS(CQ, "\n");

	cq = container_of(fcq, struct usdf_cq, cq_fid);

	tail = cq->c.soft.cq_tail;

	entry->op_context = tail->cse_context;
	entry->flags = 0;
	entry->prov_errno = tail->cse_prov_errno;
	entry->err = entry->prov_errno;

	tail++;
	if (tail == cq->c.soft.cq_end) {
		tail = cq->c.soft.cq_comps;
	}
	cq->c.soft.cq_tail = tail;

	return 1;
}

/* Completion lengths should reflect the length given by the application to the
 * send/recv call. This means we need to update the lengths for both prefix and
 * non-prefix send paths.
 *
 * RECEIVE COMPLETIONS
 *
 * Non-prefix: the application isn't aware of the usd_udp_hdr struct. Default
 * completion semantics include this in the completion length since it is part
 * of the send.
 *
 * Prefix: the application has allocated a buffer that includes the advertised
 * prefix size. For performance reasons our advertised prefix size is not the
 * same size as hour headers. To reflect the correct size we need to add the
 * size of the padding.
 *
 * SEND COMPLETIONS
 * The send completions are dependent upon the wp_len value that is set by the
 * library when using the underscore prefixed variants of the usd functions or
 * by the usd library when using the non-underscore prefixed variants.
 * Currently all send functions have been unified to report wp_len as the
 * length of the payload. This means that adjustments need to be made when in
 * libfabric prefix mode.
 */
static inline void usdf_cq_adjust_len(struct usd_completion *src,
		size_t *len)
{
	struct usdf_ep *ep = src->uc_qp->uq_context;

	if (src->uc_type == USD_COMPTYPE_RECV) {
		if (ep->ep_mode & FI_MSG_PREFIX)
			*len += (USDF_HDR_BUF_ENTRY -
					sizeof(struct usd_udp_hdr));
		else
			*len -= sizeof(struct usd_udp_hdr);
	} else {
		if (ep->ep_mode & FI_MSG_PREFIX)
			*len += USDF_HDR_BUF_ENTRY;
	}
}

static inline ssize_t
usdf_cq_copy_cq_entry(void *dst, struct usd_completion *src,
			enum fi_cq_format format)
{
	struct fi_cq_entry *ctx_entry;
	struct fi_cq_msg_entry *msg_entry;
	struct fi_cq_data_entry *data_entry;

	switch (format) {
	case FI_CQ_FORMAT_CONTEXT:
		ctx_entry = (struct fi_cq_entry *)dst;
		ctx_entry->op_context = src->uc_context;
		break;
	case FI_CQ_FORMAT_MSG:
		msg_entry = (struct fi_cq_msg_entry *)dst;
		msg_entry->op_context = src->uc_context;
		msg_entry->flags = usdf_cqe_to_flags(src);
		msg_entry->len = src->uc_bytes;

		usdf_cq_adjust_len(src, &msg_entry->len);

		break;
	case FI_CQ_FORMAT_DATA:
		data_entry = (struct fi_cq_data_entry *)dst;
		data_entry->op_context = src->uc_context;
		data_entry->flags = usdf_cqe_to_flags(src);
		data_entry->len = src->uc_bytes;
		data_entry->buf = 0; /* XXX */
		data_entry->data = 0;

		usdf_cq_adjust_len(src, &data_entry->len);

		break;
	default:
		USDF_WARN("unexpected CQ format, internal error\n");
		return -FI_EOPNOTSUPP;
	}

	return FI_SUCCESS;
}

/*
 * poll a hard CQ
 * Since this routine is an inline and is always called with format as
 * a constant, I am counting on the compiler optimizing away all the switches
 * on format.
 */
static inline ssize_t
usdf_cq_read_common(struct fid_cq *fcq, void *buf, size_t count,
		enum fi_cq_format format)
{
	struct usdf_cq *cq;
	struct usdf_fabric *fab;
	size_t copylen;
	size_t copied;
	uint8_t *dest;
	ssize_t ret;

	cq = cq_ftou(fcq);
	fab = cq->cq_domain->dom_fabric;

	if (cq->cq_comp.uc_status != USD_COMPSTAT_SUCCESS)
		return -FI_EAVAIL;

	switch (format) {
	case FI_CQ_FORMAT_CONTEXT:
		copylen = sizeof(struct fi_cq_entry);
		break;
	case FI_CQ_FORMAT_MSG:
		copylen = sizeof(struct fi_cq_msg_entry);
		break;
	case FI_CQ_FORMAT_DATA:
		copylen = sizeof(struct fi_cq_data_entry);
		break;
	default:
		USDF_WARN_SYS(CQ, "unexpected CQ format, internal error\n");
		return -FI_EOPNOTSUPP;
	}

	dest = buf;

	for (copied = 0; copied < count; copied++) {
		ret = usd_poll_cq(cq->c.hard.cq_cq, &cq->cq_comp);
		if (ret == -EAGAIN)
			break;

		if (cq->cq_comp.uc_status != USD_COMPSTAT_SUCCESS) {
			if (copied == 0)
				return -FI_EAVAIL;

			break;
		}

		ret = usdf_cq_copy_cq_entry(dest, &cq->cq_comp, format);
		if (ret < 0)
			return ret;

		dest += copylen;
	}

	if (cq->cq_waiting) {
		cq->cq_waiting = false;
		ofi_atomic_dec32(&fab->num_blocked_waiting);
	}

	return copied > 0 ? copied : -FI_EAGAIN;
}

static ssize_t
usdf_cq_read_context(struct fid_cq *fcq, void *buf, size_t count)
{
	return usdf_cq_read_common(fcq, buf, count, FI_CQ_FORMAT_CONTEXT);
}

static ssize_t
usdf_cq_read_msg(struct fid_cq *fcq, void *buf, size_t count)
{
	return usdf_cq_read_common(fcq, buf, count, FI_CQ_FORMAT_MSG);
}

static ssize_t
usdf_cq_read_data(struct fid_cq *fcq, void *buf, size_t count)
{
	return usdf_cq_read_common(fcq, buf, count, FI_CQ_FORMAT_DATA);
}

static ssize_t
usdf_cq_readfrom_context(struct fid_cq *fcq, void *buf, size_t count,
			fi_addr_t *src_addr)
{
	struct usdf_cq *cq;
	struct usd_cq_impl *ucq;
	struct fi_cq_entry *entry;
	struct fi_cq_entry *last;
	ssize_t ret;
	struct cq_desc *cq_desc;
	struct usdf_ep *ep;
	struct sockaddr_in sin;
	struct usd_udp_hdr *hdr;
	uint16_t index;

	cq = cq_ftou(fcq);
	if (cq->cq_comp.uc_status != 0) {
		return -FI_EAVAIL;
	}
	ucq = to_cqi(cq->c.hard.cq_cq);

	ret = 0;
	entry = buf;
	last = entry + count;
	while (entry < last) {
		cq_desc = (struct cq_desc *)((uint8_t *)ucq->ucq_desc_ring +
				(ucq->ucq_next_desc << 4));

		ret = usd_poll_cq(cq->c.hard.cq_cq, &cq->cq_comp);
		if (ret == -EAGAIN) {
			ret = 0;
			break;
		}
		if (cq->cq_comp.uc_status != 0) {
			ret = -FI_EAVAIL;
			break;
		}

		if (cq->cq_comp.uc_type == USD_COMPTYPE_RECV) {
			index = le16_to_cpu(cq_desc->completed_index) &
				CQ_DESC_COMP_NDX_MASK;
			ep = cq->cq_comp.uc_qp->uq_context;
			hdr = ep->e.dg.ep_hdr_ptr[index];
			memset(&sin, 0, sizeof(sin));

			sin.sin_addr.s_addr = hdr->uh_ip.saddr;
			sin.sin_port = hdr->uh_udp.source;

			*src_addr = usdf_av_lookup_addr(ep->e.dg.ep_av, &sin);
			++src_addr;
		}


		entry->op_context = cq->cq_comp.uc_context;

		entry++;
	}

	if (entry > (struct fi_cq_entry *)buf) {
		return entry - (struct fi_cq_entry *)buf;
	} else {
		return ret;
	}
}

/*****************************************************************
 * "soft" CQ support
 *****************************************************************/

void
usdf_progress_hard_cq(struct usdf_cq_hard *hcq)
{
	int ret;
	struct usd_completion comp;
	struct usdf_cq_soft_entry *entry;
	struct usdf_cq *cq;

	cq = hcq->cqh_cq;

	do {
		ret = usd_poll_cq(hcq->cqh_ucq, &comp);
		if (ret == 0) {
			entry = cq->c.soft.cq_head;

			/* If the current entry is equal to the tail and the
			 * last operation was a write, then we have filled the
			 * queue and we just drop whatever there isn't space
			 * for.
			 */
			if ((entry == cq->c.soft.cq_tail) &&
					(cq->c.soft.cq_last_op ==
						USDF_SOFT_CQ_WRITE))
				return;

			entry->cse_context = cq->cq_comp.uc_context;
			entry->cse_flags = 0;
			entry->cse_len = cq->cq_comp.uc_bytes;
			entry->cse_buf = 0;		 /* XXX TODO */
			entry->cse_data = 0;

			/* update with wrap */
			entry++;
			if (entry != cq->c.soft.cq_end) {
				cq->c.soft.cq_head = entry;
			} else {
				cq->c.soft.cq_head = cq->c.soft.cq_comps;
			}

			cq->c.soft.cq_last_op = USDF_SOFT_CQ_WRITE;
		}
	} while (ret != -EAGAIN);
}

void
usdf_cq_post_soft(struct usdf_cq_hard *hcq, void *context, size_t len,
		int prov_errno, uint64_t flags)
{
	int ret;
	struct usdf_cq_soft_entry *entry;
	struct usdf_cq *cq;
	uint64_t val = 1;

	cq = hcq->cqh_cq;

	entry = cq->c.soft.cq_head;

	/* If the current entry is equal to the tail and the
	 * last operation was a write, then we have filled the
	 * queue and we just drop whatever there isn't space
	 * for.
	 */
	if ((entry == cq->c.soft.cq_tail) &&
			(cq->c.soft.cq_last_op == USDF_SOFT_CQ_WRITE))
		return;

	entry->cse_context = context;
	entry->cse_len = len;
	entry->cse_prov_errno = prov_errno;
	entry->cse_flags = flags;

	/* update with wrap */
	entry++;
	if (entry != cq->c.soft.cq_end) {
		cq->c.soft.cq_head = entry;
	} else {
		cq->c.soft.cq_head = cq->c.soft.cq_comps;
	}

	cq->c.soft.cq_last_op = USDF_SOFT_CQ_WRITE;

	if (cq->cq_attr.wait_obj == FI_WAIT_SET ||
			cq->cq_attr.wait_obj == FI_WAIT_FD)
		while (1) {
			ret = write(cq->object.fd, &val, sizeof(val));
			assert(ret == sizeof(val) ||
				(ret == -1 && errno == EINTR));
			if (ret == sizeof(val))
				return;
			else if (ret == -1 && errno == EINTR)
				continue;

			/* If the write() fails, there will be no user
			 * notification.  Best we can do is emit a
			 * debug notice...
			 */
			USDF_WARN_SYS(CQ, "error while writing to wake CQ\n");
			return;
		}
}

static inline ssize_t
usdf_cq_copy_soft_entry(void *dst, const struct usdf_cq_soft_entry *src,
			enum fi_cq_format dst_format)
{
	struct fi_cq_entry *ctx_entry;
	struct fi_cq_msg_entry *msg_entry;
	struct fi_cq_data_entry *data_entry;

	switch (dst_format) {
	case FI_CQ_FORMAT_CONTEXT:
		ctx_entry = (struct fi_cq_entry *)dst;
		ctx_entry->op_context = src->cse_context;
		break;
	case FI_CQ_FORMAT_MSG:
		msg_entry = (struct fi_cq_msg_entry *)dst;
		msg_entry->op_context = src->cse_context;
		msg_entry->flags = src->cse_flags;
		msg_entry->len = src->cse_len;
		break;
	case FI_CQ_FORMAT_DATA:
		data_entry = (struct fi_cq_data_entry *)dst;
		data_entry->op_context = src->cse_context;
		data_entry->flags = src->cse_flags;
		data_entry->len = src->cse_len;
		data_entry->buf = src->cse_buf;
		data_entry->data = src->cse_data;
		break;
	default:
		USDF_WARN("unexpected CQ format, internal error\n");
		return -FI_EOPNOTSUPP;
	}

	return FI_SUCCESS;
}

static ssize_t usdf_cq_sread(struct fid_cq *fcq, void *buf, size_t count,
		const void *cond, int timeout_ms)
{
	struct usdf_cq *cq;
	size_t sleep_time_us;
	size_t time_spent_us = 0;
	ssize_t ret;

	cq = cq_ftou(fcq);

	if (cq->cq_attr.wait_obj == FI_WAIT_NONE)
		return -FI_EOPNOTSUPP;

	sleep_time_us = SREAD_INIT_SLEEP_TIME_US;

	while (1) {
		ret = fi_cq_read(fcq, buf, count);
		if (ret != -FI_EAGAIN)
			return ret;

		if (timeout_ms >= 0) {
			if (time_spent_us >= (1000 * timeout_ms))
				break;
		}

		usleep(sleep_time_us);
		time_spent_us += sleep_time_us;

		/* exponentially back off up to a limit */
		if (sleep_time_us < SREAD_MAX_SLEEP_TIME_US)
			sleep_time_us *= SREAD_EXP_BASE;
		sleep_time_us = MIN(sleep_time_us, SREAD_MAX_SLEEP_TIME_US);
	}

	return -FI_EAGAIN;
}

static ssize_t usdf_cq_sread_fd(struct fid_cq *fcq, void *buf, size_t count,
		const void *cond, int timeout_ms)
{
	struct usdf_cq *cq;
	struct usdf_fabric *fabric;
	int ret;

	cq = cq_ftou(fcq);
	fabric = cq->cq_domain->dom_fabric;

	ret = usdf_cq_trywait(&fcq->fid);
	if (ret == FI_SUCCESS) {
		ofi_atomic_inc32(&fabric->num_blocked_waiting);

		ret = usdf_fabric_wake_thread(fabric);
		if (ret) {
			USDF_DBG_SYS(CQ,
					"error while waking progress thread\n");
			goto err;
		}

		ret = fi_poll_fd(cq->object.fd, timeout_ms);
		if (ret == 0) {
			ret = -FI_EAGAIN;
			goto err;
		} else if (ret < 0) {
			USDF_DBG_SYS(CQ, "poll failed: %s\n", strerror(-ret));
			goto err;
		}

		ofi_atomic_dec32(&fabric->num_blocked_waiting);
	} else if ((ret < 0) && (ret != -FI_EAGAIN)) {
		return ret;
	}

	return fi_cq_read(fcq, buf, count);

err:
	ofi_atomic_dec32(&fabric->num_blocked_waiting);
	return ret;
}

/*
 * poll a soft CQ
 * This will loop over all the hard CQs within, collecting results.
 * Since this routine is an inline and is always called with format as
 * a constant, I am counting on the compiler optimizing away all the switches
 * on format.
 */
static inline ssize_t
usdf_cq_read_common_soft(struct fid_cq *fcq, void *buf, size_t count,
		enum fi_cq_format format)
{
	struct usdf_cq *cq;
	uint8_t *dest;
	struct usdf_cq_soft_entry *tail;
	size_t copylen;
	size_t copied;
	ssize_t ret;

	cq = cq_ftou(fcq);

	if (cq->cq_comp.uc_status != USD_COMPSTAT_SUCCESS)
		return -FI_EAVAIL;

	/* progress... */
	usdf_domain_progress(cq->cq_domain);

	switch (format) {
	case FI_CQ_FORMAT_CONTEXT:
		copylen = sizeof(struct fi_cq_entry);
		break;
	case FI_CQ_FORMAT_MSG:
		copylen = sizeof(struct fi_cq_msg_entry);
		break;
	case FI_CQ_FORMAT_DATA:
		copylen = sizeof(struct fi_cq_data_entry);
		break;
	default:
		USDF_WARN_SYS(CQ, "unexpected CQ format, internal error\n");
		return -FI_EOPNOTSUPP;
	}

	dest = buf;
	tail = cq->c.soft.cq_tail;

	for (copied = 0; copied < count; copied++) {
		if (tail == cq->c.soft.cq_head) {
			/* If the tail and head match and the last operation was
			 * a read then we have an empty queue.
			 */
			if (cq->c.soft.cq_last_op == USDF_SOFT_CQ_READ)
				break;
		}

		if (tail->cse_prov_errno != FI_SUCCESS) {
			/* If this is the first read, then just return EAVAIL.
			 * Although we already checked above, this last read may
			 * have contained an error. If this isn't the first read
			 * then break and return the count read. The next read
			 * will yield an error.
			 */
			if (copied == 0)
				return -FI_EAVAIL;

			break;
		}

		ret = usdf_cq_copy_soft_entry(dest, tail, format);
		if (ret < 0)
			return ret;

		dest += copylen;

		tail++;
		if (tail == cq->c.soft.cq_end)
			tail = cq->c.soft.cq_comps;

		cq->c.soft.cq_last_op = USDF_SOFT_CQ_READ;
	}

	cq->c.soft.cq_tail = tail;

	return copied > 0 ? copied : -FI_EAGAIN;
}

static ssize_t
usdf_cq_read_context_soft(struct fid_cq *fcq, void *buf, size_t count)
{
	return usdf_cq_read_common_soft(fcq, buf, count, FI_CQ_FORMAT_CONTEXT);
}

static ssize_t
usdf_cq_read_msg_soft(struct fid_cq *fcq, void *buf, size_t count)
{
	return usdf_cq_read_common_soft(fcq, buf, count, FI_CQ_FORMAT_MSG);
}

static ssize_t
usdf_cq_read_data_soft(struct fid_cq *fcq, void *buf, size_t count)
{
	return usdf_cq_read_common_soft(fcq, buf, count, FI_CQ_FORMAT_DATA);
}

/*****************************************************************
 * common CQ support
 *****************************************************************/

static const char *
usdf_cq_strerror(struct fid_cq *eq, int prov_errno, const void *err_data,
		 char *buf, size_t len)
{
	if (buf && len) {
		strncpy(buf, fi_strerror(prov_errno), len);
		buf[len-1] = '\0';
		return buf;
	}
	return fi_strerror(prov_errno);
}

/* Handle the associated wait object when closing a CQ.
 * - Remove the FD from the wait set epoll context
 * - Decrement the ref count on the wait set
 * - Remove the CQ from the CQ list attached to the wait set
 */
static int usdf_cq_unbind_wait(struct usdf_cq *cq)
{
	int ret;
	struct usdf_wait *wait_priv;

	if (!cq->cq_attr.wait_set) {
		USDF_DBG_SYS(CQ, "can't unbind from non-existent wait set\n");
		return -FI_EINVAL;
	}

	wait_priv = wait_ftou(cq->cq_attr.wait_set);

	ret = ofi_epoll_del(wait_priv->object.epfd, cq->object.fd);
	if (ret) {
		USDF_WARN_SYS(CQ, "failed to remove FD from wait set\n");
		return ret;
	}

	fid_list_remove(&wait_priv->list, &wait_priv->lock, &cq->cq_fid.fid);

	ofi_atomic_dec32(&wait_priv->wait_refcnt);

	USDF_DBG_SYS(CQ,
			"dissasociated CQ FD %d from epoll FD %d using FID: %p\n",
			cq->object.fd, wait_priv->object.epfd, &cq->cq_fid.fid);

	return FI_SUCCESS;
}

static int
usdf_cq_close(fid_t fid)
{
	int ret;
	struct usdf_cq *cq;
	struct usdf_fabric *fab;
	struct usdf_cq_hard *hcq;

	USDF_TRACE_SYS(CQ, "\n");

	cq = container_of(fid, struct usdf_cq, cq_fid.fid);
	fab = cq->cq_domain->dom_fabric;

	if (ofi_atomic_get32(&cq->cq_refcnt) > 0) {
		return -FI_EBUSY;
	}

	if (cq->cq_attr.wait_obj == FI_WAIT_SET) {
		ret = usdf_cq_unbind_wait(cq);
		if (ret)
			return ret;
	}

	if (cq->cq_is_soft) {
		while (!TAILQ_EMPTY(&cq->c.soft.cq_list)) {
			hcq = TAILQ_FIRST(&cq->c.soft.cq_list);
			if (ofi_atomic_get32(&hcq->cqh_refcnt) > 0) {
				return -FI_EBUSY;
			}
			TAILQ_REMOVE(&cq->c.soft.cq_list, hcq, cqh_link);
			TAILQ_REMOVE(&cq->cq_domain->dom_hcq_list, hcq,
					cqh_dom_link);
			if (hcq->cqh_ucq != NULL) {
				ret = usd_destroy_cq(hcq->cqh_ucq);
				if (ret != 0) {
					return ret;
				}
			}
			free(hcq);
		}
	} else {
		if (cq->c.hard.cq_cq) {
			ret = usd_destroy_cq(cq->c.hard.cq_cq);
			if (ret != 0) {
				return ret;
			}
		}
	}

	if (cq->cq_waiting)
		ofi_atomic_dec32(&fab->num_blocked_waiting);

	free(cq);
	return 0;
}

static int usdf_cq_get_wait(struct usdf_cq *cq, void *arg)
{
	USDF_TRACE_SYS(CQ, "\n");

	switch (cq->cq_attr.wait_obj) {
	case FI_WAIT_FD:
		if (cq->object.fd == -1) {
			USDF_WARN_SYS(CQ,
					"CQ must be bound before FD can be retrieved\n");
			return -FI_EOPBADSTATE;
		}

		*(int *) arg = cq->object.fd;
		break;
	default:
		USDF_WARN_SYS(CQ, "unsupported wait type\n");
		return -FI_EINVAL;
	}

	return FI_SUCCESS;
}

static int usdf_wait_control(struct fid *fcq, int command, void *arg)
{
	struct usdf_cq *cq;

	USDF_TRACE_SYS(CQ, "\n");

	if (!fcq || !arg) {
		USDF_WARN_SYS(CQ, "CQ fid and arg can't be NULL\n");
		return -FI_EINVAL;
	}

	cq = cq_fidtou(fcq);

	switch (command) {
	case FI_GETWAIT:
		break;
	default:
		USDF_WARN_SYS(CQ, "unsupported control command\n");
		return -FI_EINVAL;
	}

	return usdf_cq_get_wait(cq, arg);
}

static struct fi_ops_cq usdf_cq_context_ops = {
	.size = sizeof(struct fi_ops_cq),
	.read = usdf_cq_read_context,
	.readfrom = usdf_cq_readfrom_context,
	.readerr = usdf_cq_readerr,
	.sread = usdf_cq_sread,
	.sreadfrom = fi_no_cq_sreadfrom,
	.signal = fi_no_cq_signal,
	.strerror = usdf_cq_strerror,
};

static struct fi_ops_cq usdf_cq_context_soft_ops = {
	.size = sizeof(struct fi_ops_cq),
	.read = usdf_cq_read_context_soft,
	.readfrom = fi_no_cq_readfrom,
	.readerr = usdf_cq_readerr_soft,
	.sread = usdf_cq_sread,
	.sreadfrom = fi_no_cq_sreadfrom,
	.signal = fi_no_cq_signal,
	.strerror = usdf_cq_strerror,
};

static struct fi_ops_cq usdf_cq_msg_ops = {
	.size = sizeof(struct fi_ops_cq),
	.read = usdf_cq_read_msg,
	.readfrom = fi_no_cq_readfrom,  /* XXX */
	.readerr = usdf_cq_readerr,
	.sread = usdf_cq_sread,
	.sreadfrom = fi_no_cq_sreadfrom,
	.signal = fi_no_cq_signal,
	.strerror = usdf_cq_strerror,
};

static struct fi_ops_cq usdf_cq_msg_soft_ops = {
	.size = sizeof(struct fi_ops_cq),
	.read = usdf_cq_read_msg_soft,
	.readfrom = fi_no_cq_readfrom,  /* XXX */
	.readerr = usdf_cq_readerr_soft,
	.sread = usdf_cq_sread,
	.sreadfrom = fi_no_cq_sreadfrom,
	.signal = fi_no_cq_signal,
	.strerror = usdf_cq_strerror,
};

static struct fi_ops_cq usdf_cq_data_ops = {
	.size = sizeof(struct fi_ops_cq),
	.read = usdf_cq_read_data,
	.readfrom = fi_no_cq_readfrom,  /* XXX */
	.readerr = usdf_cq_readerr,
	.sread = usdf_cq_sread,
	.sreadfrom = fi_no_cq_sreadfrom,
	.signal = fi_no_cq_signal,
	.strerror = usdf_cq_strerror,
};

static struct fi_ops_cq usdf_cq_data_soft_ops = {
	.size = sizeof(struct fi_ops_cq),
	.read = usdf_cq_read_data_soft,
	.readfrom = fi_no_cq_readfrom,  /* XXX */
	.readerr = usdf_cq_readerr_soft,
	.sread = usdf_cq_sread,
	.sreadfrom = fi_no_cq_sreadfrom,
	.signal = fi_no_cq_signal,
	.strerror = usdf_cq_strerror,
};

static struct fi_ops usdf_cq_fi_ops = {
	.size = sizeof(struct fi_ops),
	.close = usdf_cq_close,
	.bind = fi_no_bind,
	.control = usdf_wait_control,
	.ops_open = fi_no_ops_open,
};

int
usdf_cq_make_soft(struct usdf_cq *cq)
{
        struct fi_ops_cq *soft_ops;
	struct usdf_cq_hard *hcq;
	struct usd_cq *ucq;

        switch (cq->cq_attr.format) {
        case FI_CQ_FORMAT_CONTEXT:
                soft_ops = &usdf_cq_context_soft_ops;
                break;
        case FI_CQ_FORMAT_MSG:
                soft_ops = &usdf_cq_msg_soft_ops;
                break;
        case FI_CQ_FORMAT_DATA:
                soft_ops = &usdf_cq_data_soft_ops;
                break;
	default:
		return 0;
        }

	if (!cq->cq_is_soft) {

		/* save the CQ before we trash the union */
		ucq = cq->c.hard.cq_cq;

		/* fill in the soft part of union */
		TAILQ_INIT(&cq->c.soft.cq_list);
		cq->c.soft.cq_comps = calloc(cq->cq_attr.size,
					sizeof(struct usdf_cq_soft_entry));
		if (cq->c.soft.cq_comps == NULL) {
			return -FI_ENOMEM;
		}
		cq->c.soft.cq_end = cq->c.soft.cq_comps + cq->cq_attr.size;
		cq->c.soft.cq_head = cq->c.soft.cq_comps;
		cq->c.soft.cq_tail = cq->c.soft.cq_comps;

		/* need to add hard queue to list? */
		if (ucq != NULL) {
			hcq = malloc(sizeof(*hcq));
			if (hcq == NULL) {
				free(cq->c.soft.cq_comps);
				cq->c.hard.cq_cq = ucq;	/* restore */
				return -FI_ENOMEM;
			}

			hcq->cqh_cq = cq;
			hcq->cqh_ucq = ucq;
			hcq->cqh_progress = usdf_progress_hard_cq;

			ofi_atomic_initialize32(&hcq->cqh_refcnt,
					ofi_atomic_get32(&cq->cq_refcnt));
			TAILQ_INSERT_HEAD(&cq->c.soft.cq_list, hcq, cqh_link);
		}

		cq->cq_is_soft = 1;
		cq->cq_ops = *soft_ops;
        }
	return 0;
}

int usdf_check_empty_soft_cq(struct usdf_cq *cq)
{
	if (cq->c.soft.cq_tail == cq->c.soft.cq_head)
		return cq->c.soft.cq_last_op == USDF_SOFT_CQ_READ;

	return 0;
}

int usdf_check_empty_hard_cq(struct usdf_cq *cq)
{
	struct usd_cq_impl *cqi;
	struct cq_desc *cq_desc;
	struct cq_desc *base;
	uint8_t last_color;
	uint8_t current_color;

	cqi = to_cqi(cq->c.hard.cq_cq);

	base = cqi->ucq_desc_ring;
	cq_desc = &base[cqi->ucq_next_desc];

	last_color = cqi->ucq_last_color;
	current_color = cq_desc->type_color >> CQ_DESC_COLOR_SHIFT;

	return current_color == last_color;
}

static int
usdf_cq_process_attr(struct fi_cq_attr *attr, struct usdf_domain *udp)
{
	if (!attr || !udp)
		return -FI_EINVAL;

	switch (attr->wait_obj) {
	case FI_WAIT_NONE:
	case FI_WAIT_UNSPEC:
		break;
	case FI_WAIT_FD:
	case FI_WAIT_SET:
		if (!usd_get_cap(udp->dom_dev, USD_CAP_GRP_INTR)) {
			USDF_WARN_SYS(CQ, "FD request invalid.\n");
			USDF_WARN_SYS(CQ, "group interrupts not supported.\n");
			return -FI_EINVAL;
		}
		break;
	default:
		return -FI_ENOSYS;
	}

	/* bound and default size */
	if (attr->size > udp->dom_fabric->fab_dev_attrs->uda_max_cqe) {
		return -FI_EINVAL;
	}
	if (attr->size == 0) {
		attr->size = udp->dom_fabric->fab_dev_attrs->uda_max_cqe;
	}

	/* default format is FI_CQ_FORMAT_CONTEXT */
	if (attr->format == FI_CQ_FORMAT_UNSPEC) {

		attr->format = FI_CQ_FORMAT_CONTEXT;
	}
	return 0;
}

static int usdf_cq_fd_set_nonblock(int fd)
{
	int flags;

	flags = fcntl(fd, F_GETFL, 0);
	if (flags == -1) {
		USDF_WARN_SYS(CQ, "fcntl getfl failed[%d]\n", errno);
		return -errno;
	}

	if (fcntl(fd, F_SETFL, flags | O_NONBLOCK) < 0) {
		USDF_WARN_SYS(CQ, "fcntl setfl failed[%d]\n", errno);
		return -errno;
	}

	return FI_SUCCESS;
}

static int usdf_cq_create_fd(struct usdf_cq *cq)
{
	int ret;

	ret = usd_ib_cmd_create_comp_channel(cq->cq_domain->dom_dev,
			&cq->object.fd);
	if (ret) {
		USDF_WARN_SYS(CQ, "failed to create comp channel\n");
		return -FI_EINVAL;
	}

	USDF_DBG_SYS(CQ, "successfully created comp channel with fd %d\n",
			cq->object.fd);

	/* Going to need this assuming edge-triggered semantics.
	 */
	return usdf_cq_fd_set_nonblock(cq->object.fd);
}

int usdf_cq_trywait(struct fid *fcq)
{
	struct usdf_cq *cq;
	struct usdf_fabric *fab;
	uint64_t ev;
	int empty;
	int ret;

	cq = cq_fidtou(fcq);
	fab = cq->cq_domain->dom_fabric;

	switch (cq->cq_attr.wait_obj) {
	case FI_WAIT_UNSPEC:
		return FI_SUCCESS;
	case FI_WAIT_FD:
	case FI_WAIT_SET:
		break;
	default:
		USDF_WARN_SYS(CQ, "unsupported wait object type\n");
		return -FI_EINVAL;
	}

	while (1) {
		ret = read(cq->object.fd, &ev, sizeof(ev));
		if (ret == 0) {
			USDF_WARN_SYS(CQ,
					"FD read returned 0, is it closed?\n");
			return -FI_EINVAL;
		}

		if (ret < 0) {
			if (errno == EAGAIN)
				break;
			else
				return -errno;
		}
	}

	cq->cq_waiting = true;
	ofi_atomic_inc32(&fab->num_blocked_waiting);
	ret = usdf_fabric_wake_thread(fab);
	if (ret) {
		USDF_DBG_SYS(FABRIC, "error while waking progress thread\n");
		ofi_atomic_dec32(&fab->num_blocked_waiting);
	}

	if (cq->cq_is_soft) {
		empty = usdf_check_empty_soft_cq(cq);
	} else {
		usd_poll_req_notify(cq->c.hard.cq_cq);
		empty = usdf_check_empty_hard_cq(cq);
	}

	if (empty)
		return FI_SUCCESS;

	return -FI_EAGAIN;
}


static int usdf_cq_bind_wait(struct usdf_cq *cq)
{
	int ret;
	struct usdf_wait *wait_priv;

	if (!cq->cq_attr.wait_set) {
		USDF_DBG_SYS(CQ, "can't bind to non-existent wait set\n");
		return -FI_EINVAL;
	}

	/* Wait set ref count doesn't need to be incremented here since it was
	 * already incremented during CQ open. It's incremented in CQ open
	 * because the CQ isn't actually created until bind time, and we want
	 * to make sure that the wait object is not closed in between open and
	 * bind.
	 */
	wait_priv = wait_ftou(cq->cq_attr.wait_set);

	ret = fid_list_insert(&wait_priv->list, &wait_priv->lock,
			&cq->cq_fid.fid);
	if (ret) {
		USDF_WARN_SYS(CQ,
				"failed to associate cq with wait fid list\n");
		return ret;
	}

	ret = ofi_epoll_add(wait_priv->object.epfd, cq->object.fd,
			    OFI_EPOLL_IN, cq);
	if (ret) {
		USDF_WARN_SYS(CQ, "failed to associate FD with wait set\n");
		goto err;
	}

	USDF_DBG_SYS(CQ, "associated CQ FD %d with epoll FD %d using fid %p\n",
			cq->object.fd, wait_priv->object.epfd, &cq->cq_fid.fid);

	return ret;

err:
	fid_list_remove(&wait_priv->list, &wait_priv->lock, &cq->cq_fid.fid);
	return ret;
}

/* If cq->cq_attr.wait_obj == (FI_WAIT_FD | FI_WAIT_SET), then use an FD with
 * the CQ. If create_fd evaluates to true, then it will create a hardware
 * completion channel.
 *
 * If create_fd does not evaluate to true, then it is assumed that a valid file
 * descriptor is available in cq->object.fd.
 */
int usdf_cq_create_cq(struct usdf_cq *cq, struct usd_cq **ucq, int create_fd)
{
	int ret;
	struct usd_cq_init_attr attr = {0};

	if (!cq || !cq->cq_domain || !cq->cq_domain->dom_dev) {
		USDF_DBG_SYS(CQ, "Invalid input.\n");
		return -FI_EINVAL;
	}

	attr.num_entries = cq->cq_attr.size;
	attr.comp_fd = -1;

	/* For hard queues we will need to create an FD for CQs configured to
	 * use both wait sets and FDs. For a wait set this FD will get added to
	 * the epoll structure used by the waitset.
	 *
	 * For soft queues (emulated endpoints) we will not be creating an FD,
	 * but will need to set the appropriate functions and bind to the wait
	 * object, if any.
	 */
	if ((cq->cq_attr.wait_obj == FI_WAIT_FD) ||
			(cq->cq_attr.wait_obj == FI_WAIT_SET)) {
		cq->cq_ops.sread = usdf_cq_sread_fd;

		if (create_fd) {
			ret = usdf_cq_create_fd(cq);
			if (ret)
				return ret;

			attr.comp_fd = cq->object.fd;

			/* usd_create_cq will only set
			 * USNIC_CQ_COMP_SIGNAL_VERBS if an ibv_cq is present,
			 * but we don't have one. Just shove the cq in.
			 */
			attr.ibv_cq = &ucq;
		}

		if (cq->cq_attr.wait_obj == FI_WAIT_SET) {
			cq->cq_ops.sread = fi_no_cq_sread;
			ret = usdf_cq_bind_wait(cq);
			if (ret)
				return ret;
		}
	}

	ret = usd_create_cq(cq->cq_domain->dom_dev, &attr, ucq);
	if (ret && cq->cq_attr.wait_obj == FI_WAIT_SET)
		usdf_cq_unbind_wait(cq);
	return ret;
}

int
usdf_cq_open(struct fid_domain *domain, struct fi_cq_attr *attr,
	    struct fid_cq **cq_o, void *context)
{
	struct usdf_cq *cq;
	struct usdf_domain *udp;
	struct usdf_wait *wait_priv;
	int ret;

	USDF_TRACE_SYS(CQ, "\n");

	udp = dom_ftou(domain);
	ret = usdf_cq_process_attr(attr, udp);
	if (ret != 0) {
		return ret;
	}

	cq = calloc(1, sizeof(*cq));
	if (cq == NULL) {
		return -FI_ENOMEM;
	}

	/* Do this here because we don't actually create the CQ until bind
	 * time. At open time the CQ should be associated with the wait set
	 * using the ref count so the app can't delete the wait set out from
	 * under the CQ.
	 */
	if (attr->wait_obj == FI_WAIT_SET) {
		wait_priv = wait_ftou(attr->wait_set);
		ofi_atomic_inc32(&wait_priv->wait_refcnt);
	}

	cq->object.fd = -1;
	cq->cq_domain = udp;
	cq->cq_fid.fid.fclass = FI_CLASS_CQ;
	cq->cq_fid.fid.context = context;
	cq->cq_fid.fid.ops = &usdf_cq_fi_ops;
	ofi_atomic_initialize32(&cq->cq_refcnt, 0);

	switch (attr->format) {
	case FI_CQ_FORMAT_CONTEXT:
		cq->cq_ops = usdf_cq_context_ops;
		break;
	case FI_CQ_FORMAT_MSG:
		cq->cq_ops = usdf_cq_msg_ops;
		break;
	case FI_CQ_FORMAT_DATA:
		cq->cq_ops = usdf_cq_data_ops;
		break;
	default:
		ret = -FI_ENOSYS;
		goto fail;
	}

	cq->cq_fid.ops = &cq->cq_ops;

	cq->cq_attr = *attr;
	*cq_o = &cq->cq_fid;
	return 0;

fail:
	if (cq != NULL) {
		if (cq->c.hard.cq_cq != NULL) {
			usd_destroy_cq(cq->c.hard.cq_cq);
		}
		free(cq);
	}
	return ret;
}
