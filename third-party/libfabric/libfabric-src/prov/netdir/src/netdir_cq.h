/*
* Copyright (c) 2015-2016 Intel Corporation, Inc.  All rights reserved.
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

#ifndef _FI_NETDIR_CQ_H_
#define _FI_NETDIR_CQ_H_

#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>

#include "ndspi.h"

#include "rdma/fabric.h"
#include "ofi_mem.h"

#include "netdir.h"
#include "netdir_buf.h"
#include "netdir_log.h"
#include "netdir_util.h"
#include "netdir_iface.h"
#include "netdir_queue.h"

#include "rdma/fi_eq.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

static inline void ofi_nd_free_cq_entry(struct nd_cq_entry *entry)
{
	assert(entry);

	if (entry->prefix)
		__ofi_nd_buf_free_nd_msgprefix(entry->prefix,
					       &entry->domain->msgfooter);

	if (entry->inline_buf)
		__ofi_nd_buf_free_nd_inlinebuf(entry->inline_buf,
					       &entry->domain->inlinebuf);

	while (entry->mr_count) {
		entry->mr_count--;
		entry->mr[entry->mr_count]->lpVtbl->Release(entry->mr[entry->mr_count]);
	}

	/* Means that waiting of completion are used. The completion
	 * critical section must be released */
	if (entry->wait_completion.total_count != 0)
		DeleteCriticalSection(&entry->wait_completion.comp_lock);

	/* Release nested entry */
	if (entry->aux_entry)
		ofi_nd_free_cq_entry(entry->aux_entry);

	ND_BUF_FREE(nd_cq_entry, entry);
}

static inline ssize_t ofi_nd_cq_cancel(fid_t fid, void *context)
{
	assert(context);

	ssize_t ret = -ENOENT;
	struct nd_cq_entry *entry = (struct nd_cq_entry *)ND_FI_CONTEXT(context);
	CRITICAL_SECTION *prepost_lock;
	struct nd_queue_queue *prepost;
	struct nd_srx *srx;
	struct nd_ep *ep;

	switch (fid->fclass) {
	case FI_CLASS_SRX_CTX:
		srx = container_of(fid, struct nd_srx, fid.fid);
		prepost_lock = &srx->prepost_lock;
		prepost = &srx->prepost;
		break;
	case FI_CLASS_EP:
		ep = container_of(fid, struct nd_ep, fid.fid);
		prepost_lock = &ep->prepost_lock;
		prepost = &ep->prepost;
		break;
	default:
		ND_LOG_WARN(FI_LOG_EP_DATA, "Invalid endpoint type \n");
		return -FI_EINVAL;
	}

	if (entry) {
		struct nd_queue_item *item = &entry->queue_item;

		EnterCriticalSection(prepost_lock);

		ofi_nd_queue_pop(prepost, &item);
		ofi_nd_free_cq_entry(entry);

		if (context)
			ND_FI_CONTEXT(context) = 0;

		LeaveCriticalSection(prepost_lock);

		ret = 0;
	}

	return ret;
}

/* do NOT forget to add progress of Send Queue in all places
 * where entry is enqueued for specific EP. Just to don't
 * rely on ND's asynchronous invocation of providers callback
 * about completion of an operation */
static inline void ofi_nd_ep_progress(struct nd_ep *ep)
{
	HRESULT hr;
	struct nd_queue_item *qentry = NULL;
	nd_send_entry *send_entry = NULL;

	EnterCriticalSection(&ep->send_op.send_lock);
	while (ofi_nd_queue_peek(&ep->send_queue, &qentry) &&
		!(ep->send_op.flags.is_send_blocked)) {
		ep->send_op.used_counter++;
		send_entry = container_of(qentry, nd_send_entry, queue_item);
		ofi_nd_queue_pop(&ep->send_queue, &qentry);

		if (!(ep->send_op.used_counter % gl_data.prepost_cnt)) {
			ep->send_op.flags.is_send_blocked = 1;
			ep->send_op.used_counter = 0;
			struct nd_msgheader *header = (struct nd_msgheader *)
				send_entry->sge->entries[0].Buffer;
			header->flags.req_ack = 1;
		}

		/* If there is prepost entry (it means that this SEND event
		 * expects an answer). In this case, push CQ entry to prepost
		 * queue to receive event(answer) */
		if (send_entry->prepost_entry) {
			ND_LOG_DEBUG(FI_LOG_EP_DATA, "Posted entry(state = %d) that "
				     "expects an answer from peer to which the send "
				     "event is belong\n", send_entry->prepost_entry->state);
			ofi_nd_queue_push(&ep->internal_prepost,
				&send_entry->prepost_entry->queue_item);
		}

		hr = send_entry->ep->qp->lpVtbl->Send(send_entry->ep->qp,
			send_entry->cq_entry,
			send_entry->sge->entries,
			send_entry->sge->count, 0);
		if (FAILED(hr))
			ND_LOG_WARN(FI_LOG_CQ, "Send failed from Send Queue\n");
	}
	LeaveCriticalSection(&ep->send_op.send_lock);
}

#if 0
static inline void ofi_nd_progress(void *arg)
{
	struct nd_domain *domain = arg;
	struct dlist_entry *item;
	struct nd_ep *ep;

	while (domain->do_progress) {
		dlist_foreach(&domain->ep_list, item) {
			ep = container_of(item, struct nd_ep, entry);
			ofi_nd_ep_progress(ep);
		}
	}
}
#endif

void ofi_nd_repack_iovecs(const struct iovec *from_iovecs, const size_t from_count,
			  const struct iovec *to_iovecs, const size_t to_count,
			  struct iovec new_iovecs[ND_MSG_INTERNAL_IOV_LIMIT],
			  size_t *new_count,
			  size_t from_split_map[ND_MSG_INTERNAL_IOV_LIMIT],
			  size_t to_split_map[ND_MSG_INTERNAL_IOV_LIMIT],
			  uint64_t remote_addr[ND_MSG_INTERNAL_IOV_LIMIT]);


void ofi_nd_dispatch_cq_event(ofi_nd_cq_event event, nd_cq_entry *entry,
			      void *misc);
void ofi_nd_send_ack(nd_cq_entry *entry, struct nd_ep *ep);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _FI_NETDIR_CQ_H_ */

