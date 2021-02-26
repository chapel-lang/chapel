/*
 * Copyright (c) 2014-2016, Cisco Systems, Inc. All rights reserved.
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

#include <rdma/fabric.h>
#include <rdma/fi_cm.h>
#include <rdma/fi_domain.h>
#include <rdma/fi_endpoint.h>
#include <rdma/fi_rma.h>
#include <rdma/fi_errno.h>
#include "ofi.h"

#include "usd.h"
#include "usd_post.h"

#include "usdf.h"
#include "usdf_dgram.h"
#include "usdf_av.h"

static inline size_t _usdf_iov_len(const struct iovec *iov, size_t count)
{
	size_t len;
	size_t i;

	for (i = 0, len = 0; i < count; i++)
		len += iov[i].iov_len;

	return len;
}

static inline struct usd_udp_hdr *_usdf_find_hdr(struct usd_wq *wq)
{
	uint8_t *copybuf;

	copybuf = wq->uwq_copybuf + (wq->uwq_post_index * USD_SEND_MAX_COPY);

	return (struct usd_udp_hdr *) copybuf;
}

static inline void _usdf_adjust_hdr(struct usd_udp_hdr *hdr,
		struct usd_qp_impl *qp, size_t len)
{
	hdr->uh_ip.tot_len = htons(len + sizeof(struct usd_udp_hdr) -
				sizeof(struct ether_header));
	hdr->uh_udp.len = htons(len + sizeof(struct usd_udp_hdr) -
				sizeof(struct ether_header) -
				sizeof(struct iphdr));
	hdr->uh_udp.source =
		qp->uq_attrs.uqa_local_addr.ul_addr.ul_udp.u_addr.sin_port;
}

static inline void _usdf_adjust_prefix_hdr(struct usd_udp_hdr *hdr,
		struct usd_qp_impl *qp, size_t len, size_t padding)
{

	hdr->uh_ip.tot_len = htons(len - padding - sizeof(struct ether_header));
	hdr->uh_udp.len = htons(len - padding - sizeof(struct ether_header) -
				sizeof(struct iphdr));
	hdr->uh_udp.source =
		qp->uq_attrs.uqa_local_addr.ul_addr.ul_udp.u_addr.sin_port;
}

static inline void _usdf_adjust_post_info(struct usd_wq *wq, uint32_t last_post,
		void *context, size_t len)
{
	struct usd_wq_post_info *info;

	info = &wq->uwq_post_info[last_post];
	info->wp_context = context;
	info->wp_len = len;
}

ssize_t
usdf_dgram_recv(struct fid_ep *fep, void *buf, size_t len,
		void *desc, fi_addr_t src_addr, void *context)
{
	struct usdf_ep *ep;
	struct usd_qp_impl *qp;
	struct usd_recv_desc rxd;
	uint32_t index;

	ep = ep_ftou(fep);
	qp = to_qpi(ep->e.dg.ep_qp);

	index = qp->uq_rq.urq_post_index;
	rxd.urd_context = context;
	rxd.urd_iov[0].iov_base = (uint8_t *)ep->e.dg.ep_hdr_buf +
		(index * USDF_HDR_BUF_ENTRY) +
		(USDF_HDR_BUF_ENTRY - sizeof(struct usd_udp_hdr));
	rxd.urd_iov[0].iov_len = sizeof(struct usd_udp_hdr);
	rxd.urd_iov[1].iov_base = buf;
	rxd.urd_iov[1].iov_len = len;
	rxd.urd_iov_cnt = 2;
	rxd.urd_next = NULL;

	ep->e.dg.ep_hdr_ptr[index] = rxd.urd_iov[0].iov_base;
	index = (index + 1) & qp->uq_rq.urq_post_index_mask;
	ep->e.dg.ep_hdr_ptr[index] = rxd.urd_iov[0].iov_base;

	return usd_post_recv(ep->e.dg.ep_qp, &rxd);
}

ssize_t
usdf_dgram_recvv(struct fid_ep *fep, const struct iovec *iov, void **desc,
		size_t count, fi_addr_t src_addr, void *context)
{
	struct usdf_ep *ep;
	struct usd_recv_desc rxd;
	struct usd_qp_impl *qp;
	uint32_t index;
	size_t i;

	ep = ep_ftou(fep);
	qp = to_qpi(ep->e.dg.ep_qp);

	rxd.urd_context = context;
	rxd.urd_iov[0].iov_base = ((uint8_t *)ep->e.dg.ep_hdr_buf) +
		qp->uq_rq.urq_post_index * USDF_HDR_BUF_ENTRY;
	rxd.urd_iov[0].iov_len = sizeof(struct usd_udp_hdr);
	memcpy(&rxd.urd_iov[1], iov, sizeof(*iov) * count);
	rxd.urd_iov_cnt = count + 1;
	rxd.urd_next = NULL;

	index = qp->uq_rq.urq_post_index;
	for (i = 0; i < count; ++i) {
		ep->e.dg.ep_hdr_ptr[index] = rxd.urd_iov[0].iov_base;
		index = (index + 1) & qp->uq_rq.urq_post_index_mask;
	}

	return usd_post_recv(ep->e.dg.ep_qp, &rxd);
}

ssize_t
usdf_dgram_recvmsg(struct fid_ep *fep, const struct fi_msg *msg, uint64_t flags)
{
	struct usdf_ep *ep;
	struct usd_qp_impl *qp;
	struct usd_rq *rq;
	struct vnic_rq *vrq;
	struct rq_enet_desc *desc;
	const struct iovec *iovp;
	uint8_t *hdr_ptr;
	uint32_t index;
	unsigned i;

	ep = ep_ftou(fep);
	qp = to_qpi(ep->e.dg.ep_qp);
	rq = &qp->uq_rq;
	vrq = &rq->urq_vnic_rq;
	desc = rq->urq_next_desc;
	index = rq->urq_post_index;

	iovp = msg->msg_iov;
	rq->urq_context[index] = msg->context;
	hdr_ptr = ((uint8_t *)ep->e.dg.ep_hdr_buf) +
			(index * USDF_HDR_BUF_ENTRY);
	rq_enet_desc_enc(desc, (dma_addr_t) hdr_ptr,
			RQ_ENET_TYPE_ONLY_SOP, sizeof(struct usd_udp_hdr));
	ep->e.dg.ep_hdr_ptr[index] = (struct usd_udp_hdr *) hdr_ptr;

	index = (index + 1) & rq->urq_post_index_mask;
	desc = (struct rq_enet_desc *)
		((uintptr_t)rq->urq_desc_ring + (index << 4));

	for (i = 0; i < msg->iov_count; ++i) {
		rq->urq_context[index] = msg->context;
		rq_enet_desc_enc(desc, (dma_addr_t) iovp[i].iov_base,
			     RQ_ENET_TYPE_NOT_SOP, iovp[i].iov_len);
		ep->e.dg.ep_hdr_ptr[index] = (struct usd_udp_hdr *) hdr_ptr;

		index = (index + 1) & rq->urq_post_index_mask;
		desc = (struct rq_enet_desc *)
			((uintptr_t)rq->urq_desc_ring + (index << 4));
	}

	if ((flags & FI_MORE) == 0) {
		wmb();
		iowrite32(index, &vrq->ctrl->posted_index);
	}

	rq->urq_next_desc = desc;
	rq->urq_post_index = index;
	rq->urq_recv_credits -= msg->iov_count + 1;

	return 0;
}

ssize_t
usdf_dgram_send(struct fid_ep *fep, const void *buf, size_t len, void *desc,
		fi_addr_t dest_addr, void *context)
{
	struct usdf_dest *dest;
	struct usdf_ep *ep;
	uint32_t flags;

	ep = ep_ftou(fep);
	dest = (struct usdf_dest *)(uintptr_t) dest_addr;
	flags = (ep->ep_tx_completion) ? USD_SF_SIGNAL : 0;

	assert(len <= ep->max_msg_size);

	if (len + sizeof(struct usd_udp_hdr) <= USD_SEND_MAX_COPY) {
		return usd_post_send_one_copy(ep->e.dg.ep_qp, &dest->ds_dest,
						buf, len, flags,
						context);
	} else if (ep->e.dg.tx_op_flags & FI_INJECT) {
		USDF_DBG_SYS(EP_DATA,
				"given inject length (%zu) exceeds max inject length (%d)\n",
				len + sizeof(struct usd_udp_hdr),
				USD_SEND_MAX_COPY);
		return -FI_ENOSPC;
	}

	return usd_post_send_one(ep->e.dg.ep_qp, &dest->ds_dest, buf, len,
				flags, context);
}

static ssize_t
_usdf_dgram_send_iov_copy(struct usdf_ep *ep, struct usd_dest *dest,
		const struct iovec *iov, size_t count, void *context,
		uint8_t cq_entry)
{
	struct usd_wq *wq;
 	struct usd_qp_impl *qp;
	struct usd_udp_hdr *hdr;
	uint32_t last_post;
	size_t len;
	unsigned i;

	qp = to_qpi(ep->e.dg.ep_qp);
	wq = &qp->uq_wq;

	hdr = _usdf_find_hdr(wq);
	memcpy(hdr, &dest->ds_dest.ds_udp.u_hdr, sizeof(*hdr));

	len = 0;
	for (i = 0; i < count; i++) {
		memcpy((char *) hdr + sizeof(*hdr) + len, iov[i].iov_base,
				iov[i].iov_len);
		len += iov[i].iov_len;
	}

	assert(len <= ep->max_msg_size);

	_usdf_adjust_hdr(hdr, qp, len);

	last_post = _usd_post_send_one(wq, hdr, len + sizeof(*hdr), cq_entry);

	_usdf_adjust_post_info(wq, last_post, context, len);

	return 0;
}

static ssize_t _usdf_dgram_send_iov(struct usdf_ep *ep, struct usd_dest *dest,
		const struct iovec *iov, size_t count, void *context, uint8_t
		cq_entry)
{
	struct iovec send_iov[USDF_DGRAM_MAX_SGE];
	struct usd_udp_hdr *hdr;
	struct usd_qp_impl *qp;
	struct usd_wq *wq;
	uint32_t last_post;
	size_t len;

	qp = to_qpi(ep->e.dg.ep_qp);
	wq = &qp->uq_wq;

	len = _usdf_iov_len(iov, count);
	hdr = _usdf_find_hdr(wq);
	memcpy(hdr, &dest->ds_dest.ds_udp.u_hdr, sizeof(*hdr));
	_usdf_adjust_hdr(hdr, qp, len);

	assert(len <= ep->max_msg_size);

	send_iov[0].iov_base = hdr;
	send_iov[0].iov_len = sizeof(*hdr);
	memcpy(&send_iov[1], iov, sizeof(struct iovec) * count);

	last_post = _usd_post_send_iov(wq, send_iov, count + 1,
					cq_entry);
	_usdf_adjust_post_info(wq, last_post, context, len);

	return FI_SUCCESS;
}

ssize_t
usdf_dgram_sendv(struct fid_ep *fep, const struct iovec *iov, void **desc,
		 size_t count, fi_addr_t dest_addr, void *context)
{
	struct usd_dest *dest;
	struct usdf_ep *ep;
	size_t len;

	ep = ep_ftou(fep);
	len = sizeof(struct usd_udp_hdr);
	dest = (struct usd_dest *)(uintptr_t) dest_addr;

	len += _usdf_iov_len(iov, count);
	assert(len <= ep->max_msg_size);

	if (len <= USD_SEND_MAX_COPY) {
		return _usdf_dgram_send_iov_copy(ep, dest, iov, count, context,
						ep->ep_tx_completion);
	} else if (ep->e.dg.tx_op_flags & FI_INJECT) {
		USDF_DBG_SYS(EP_DATA,
				"given inject length (%zu) exceeds max inject length (%d)\n",
				len, USD_SEND_MAX_COPY);
		return -FI_ENOSPC;
	}

	if (count > ep->e.dg.tx_iov_limit) {
		USDF_DBG_SYS(EP_DATA, "max iov count exceeded: %zu\n", count);
		return -FI_ENOSPC;
	}

	return _usdf_dgram_send_iov(ep, dest, iov, count, context,
					ep->ep_tx_completion);
}

ssize_t
usdf_dgram_sendmsg(struct fid_ep *fep, const struct fi_msg *msg, uint64_t flags)
{
	struct usd_dest *dest;
	struct usdf_ep *ep;
	uint8_t completion;
	size_t len;

	ep = ep_ftou(fep);
	len = sizeof(struct usd_udp_hdr);
	dest = (struct usd_dest *)(uintptr_t) msg->addr;
	completion = ep->ep_tx_dflt_signal_comp || (flags & FI_COMPLETION);

	len += _usdf_iov_len(msg->msg_iov, msg->iov_count);
	assert(len <= ep->max_msg_size);

	if (len <= USD_SEND_MAX_COPY) {
		return _usdf_dgram_send_iov_copy(ep, dest, msg->msg_iov,
							msg->iov_count,
							msg->context,
							completion);
	} else if (flags & FI_INJECT) {
		USDF_DBG_SYS(EP_DATA,
				"given inject length (%zu) exceeds max inject length (%d)\n",
				len, USD_SEND_MAX_COPY);
		return -FI_ENOSPC;
	}

	if (msg->iov_count > ep->e.dg.tx_iov_limit) {
		USDF_DBG_SYS(EP_DATA, "max iov count exceeded: %zu\n",
				msg->iov_count);
		return -FI_ENOSPC;
	}

	return _usdf_dgram_send_iov(ep, dest, msg->msg_iov, msg->iov_count,
					msg->context, completion);
}

ssize_t
usdf_dgram_inject(struct fid_ep *fep, const void *buf, size_t len,
		  fi_addr_t dest_addr)
{
	struct usdf_dest *dest;
	struct usdf_ep *ep;

	ep = ep_ftou(fep);
	dest = (struct usdf_dest *)(uintptr_t) dest_addr;

	if (len + sizeof(struct usd_udp_hdr) > USD_SEND_MAX_COPY) {
		USDF_DBG_SYS(EP_DATA,
				"given inject length (%zu) exceeds max inject length (%d)\n",
				len + sizeof(struct usd_udp_hdr),
				USD_SEND_MAX_COPY);
		return -FI_ENOSPC;
	}

	/*
	 * fi_inject never generates a completion
	 */
	return usd_post_send_one_copy(ep->e.dg.ep_qp, &dest->ds_dest, buf, len,
					0, NULL);
}

ssize_t usdf_dgram_prefix_inject(struct fid_ep *fep, const void *buf,
		size_t len, fi_addr_t dest_addr)
{
	return usdf_dgram_inject(fep, ((uint8_t *)buf) + USDF_HDR_BUF_ENTRY,
			len - USDF_HDR_BUF_ENTRY, dest_addr);
}

ssize_t usdf_dgram_rx_size_left(struct fid_ep *fep)
{
	struct usdf_ep *ep;

	USDF_DBG_SYS(EP_DATA, "\n");

	if (fep == NULL)
		return -FI_EINVAL;

	ep = ep_ftou(fep);

	if (!(ep->flags & USDF_EP_ENABLED))
		return -FI_EOPBADSTATE;

	return usd_get_recv_credits(ep->e.dg.ep_qp) /
		(ep->e.dg.rx_iov_limit + 1);
}

ssize_t usdf_dgram_tx_size_left(struct fid_ep *fep)
{
	struct usdf_ep *ep;

	USDF_DBG_SYS(EP_DATA, "\n");

	if (fep == NULL)
		return -FI_EINVAL;

	ep = ep_ftou(fep);

	if (!(ep->flags & USDF_EP_ENABLED))
		return -FI_EOPBADSTATE;

	return usd_get_send_credits(ep->e.dg.ep_qp) /
		(ep->e.dg.tx_iov_limit + 1);
}

/*
 * Versions that rely on user to reserve space for header at start of buffer
 */
ssize_t
usdf_dgram_prefix_recv(struct fid_ep *fep, void *buf, size_t len,
		void *desc, fi_addr_t src_addr, void *context)
{
	struct usdf_ep *ep;
	struct usd_qp_impl *qp;
	struct usd_recv_desc rxd;
	uint32_t index;

	ep = ep_ftou(fep);
	qp = to_qpi(ep->e.dg.ep_qp);

	index = qp->uq_rq.urq_post_index;
	rxd.urd_context = context;
	rxd.urd_iov[0].iov_base = (uint8_t *)buf +
		USDF_HDR_BUF_ENTRY - sizeof(struct usd_udp_hdr);
	rxd.urd_iov[0].iov_len = len -
		(USDF_HDR_BUF_ENTRY - sizeof(struct usd_udp_hdr));
	rxd.urd_iov_cnt = 1;
	rxd.urd_next = NULL;

	ep->e.dg.ep_hdr_ptr[index] = rxd.urd_iov[0].iov_base;

	return usd_post_recv(ep->e.dg.ep_qp, &rxd);
}

ssize_t
usdf_dgram_prefix_recvv(struct fid_ep *fep, const struct iovec *iov,
		void **desc, size_t count, fi_addr_t src_addr, void *context)
{
	struct usdf_ep *ep;
	struct usd_recv_desc rxd;
	struct usd_qp_impl *qp;
	uint32_t index;
	size_t i;

	ep = ep_ftou(fep);
	qp = to_qpi(ep->e.dg.ep_qp);

	rxd.urd_context = context;
	memcpy(&rxd.urd_iov[0], iov, sizeof(*iov) * count);
	rxd.urd_iov[0].iov_base = (uint8_t *)rxd.urd_iov[0].iov_base +
		USDF_HDR_BUF_ENTRY - sizeof(struct usd_udp_hdr);
	rxd.urd_iov[0].iov_len -= (USDF_HDR_BUF_ENTRY -
					sizeof(struct usd_udp_hdr));

	rxd.urd_iov_cnt = count;
	rxd.urd_next = NULL;

	index = qp->uq_rq.urq_post_index;
	for (i = 0; i < count; ++i) {
		ep->e.dg.ep_hdr_ptr[index] = rxd.urd_iov[0].iov_base;
		index = (index + 1) & qp->uq_rq.urq_post_index_mask;
	}

	return usd_post_recv(ep->e.dg.ep_qp, &rxd);
}

ssize_t
usdf_dgram_prefix_recvmsg(struct fid_ep *fep, const struct fi_msg *msg, uint64_t flags)
{
	struct usdf_ep *ep;
	struct usd_qp_impl *qp;
	struct usd_rq *rq;
	struct vnic_rq *vrq;
	struct rq_enet_desc *desc;
	uint8_t *hdr_ptr;
	const struct iovec *iovp;
	uint32_t index;
	unsigned i;

	ep = ep_ftou(fep);
	qp = to_qpi(ep->e.dg.ep_qp);
	rq = &qp->uq_rq;
	vrq = &rq->urq_vnic_rq;
	desc = rq->urq_next_desc;
	index = rq->urq_post_index;

	iovp = msg->msg_iov;
	rq->urq_context[index] = msg->context;
	hdr_ptr = ((uint8_t *)iovp[0].iov_base) +
		(USDF_HDR_BUF_ENTRY - sizeof(struct usd_udp_hdr));
	rq_enet_desc_enc(desc, (dma_addr_t) hdr_ptr,
			 RQ_ENET_TYPE_ONLY_SOP,
			 iovp[0].iov_len -
			  (USDF_HDR_BUF_ENTRY - sizeof(struct usd_udp_hdr)));
	ep->e.dg.ep_hdr_ptr[index] = (struct usd_udp_hdr *) hdr_ptr;

	index = (index+1) & rq->urq_post_index_mask;
	desc = (struct rq_enet_desc *) ((uintptr_t)rq->urq_desc_ring
					    + (index<<4));

	for (i = 1; i < msg->iov_count; ++i) {
		rq->urq_context[index] = msg->context;
		rq_enet_desc_enc(desc, (dma_addr_t) iovp[i].iov_base,
			     RQ_ENET_TYPE_NOT_SOP, iovp[i].iov_len);
		ep->e.dg.ep_hdr_ptr[index] = (struct usd_udp_hdr *) hdr_ptr;

		index = (index+1) & rq->urq_post_index_mask;
		desc = (struct rq_enet_desc *) ((uintptr_t)rq->urq_desc_ring
					    + (index<<4));
	}

	if ((flags & FI_MORE) == 0) {
		wmb();
		iowrite32(index, &vrq->ctrl->posted_index);
	}

	rq->urq_next_desc = desc;
	rq->urq_post_index = index;
	rq->urq_recv_credits -= msg->iov_count;

	return 0;
}

ssize_t
usdf_dgram_prefix_send(struct fid_ep *fep, const void *buf, size_t len,
		void *desc, fi_addr_t dest_addr, void *context)
{
	struct usd_udp_hdr *hdr;
	struct usd_qp_impl *qp;
	struct usdf_dest *dest;
	struct usdf_ep *ep;
	struct usd_wq *wq;
	uint32_t last_post;
	uint32_t flags;
	size_t padding;

	ep = ep_ftou(fep);
	dest = (struct usdf_dest *)(uintptr_t) dest_addr;
	padding = USDF_HDR_BUF_ENTRY - sizeof(struct usd_udp_hdr);
	flags = (ep->ep_tx_completion) ? USD_SF_SIGNAL : 0;

	assert(len <= ep->max_msg_size);

	if (ep->e.dg.tx_op_flags & FI_INJECT) {
		if ((len - padding) > USD_SEND_MAX_COPY) {
			USDF_DBG_SYS(EP_DATA,
					"given inject length (%zu) exceeds max inject length (%d)\n",
					len, USD_SEND_MAX_COPY);
			return -FI_ENOSPC;
		}

		return usd_post_send_one_copy(ep->e.dg.ep_qp, &dest->ds_dest,
				((uint8_t *)buf) + USDF_HDR_BUF_ENTRY, len -
				USDF_HDR_BUF_ENTRY, flags,
				context);
	}

	qp = to_qpi(ep->e.dg.ep_qp);
	wq = &qp->uq_wq;

	hdr = (struct usd_udp_hdr *) ((char *) buf + padding);
	memcpy(hdr, &dest->ds_dest.ds_dest.ds_udp.u_hdr, sizeof(*hdr));

	_usdf_adjust_prefix_hdr(hdr, qp, len, padding);

	last_post = _usd_post_send_one(wq, hdr, len - padding,
			ep->ep_tx_completion);

	_usdf_adjust_post_info(wq, last_post, context, len - USDF_HDR_BUF_ENTRY);

	return FI_SUCCESS;
}

static ssize_t
_usdf_dgram_send_iov_prefix(struct usdf_ep *ep,
		struct usd_dest *dest, const struct iovec *iov,
		size_t count, void *context, uint8_t cq_entry)
{
	struct iovec send_iov[USDF_DGRAM_MAX_SGE];
	struct usd_udp_hdr *hdr;
	struct usd_qp_impl *qp;
	uint32_t last_post;
	struct usd_wq *wq;
	size_t padding;
	size_t len;

	qp = to_qpi(ep->e.dg.ep_qp);
	wq = &qp->uq_wq;

	len = _usdf_iov_len(iov, count);
	padding = USDF_HDR_BUF_ENTRY - sizeof(struct usd_udp_hdr);

	assert(len <= ep->max_msg_size);

	hdr = (struct usd_udp_hdr *) ((char *) iov[0].iov_base +
			padding);
	memcpy(hdr, &dest->ds_dest.ds_udp.u_hdr, sizeof(*hdr));

	_usdf_adjust_prefix_hdr(hdr, qp, len, padding);

	memcpy(send_iov, iov, sizeof(struct iovec) * count);
	send_iov[0].iov_base = hdr;
	send_iov[0].iov_len -= padding;

	last_post = _usd_post_send_iov(wq, send_iov, count, cq_entry);
	_usdf_adjust_post_info(wq, last_post, context, len - USDF_HDR_BUF_ENTRY);

	return FI_SUCCESS;
}

ssize_t
usdf_dgram_prefix_sendv(struct fid_ep *fep, const struct iovec *iov,
		void **desc, size_t count, fi_addr_t dest_addr, void *context)
{
	struct iovec send_iov[USDF_DGRAM_MAX_SGE];
	struct usd_dest *dest;
	struct usdf_ep *ep;
	size_t len;
	size_t padding;

	ep = ep_ftou(fep);
	dest = (struct usd_dest *)(uintptr_t) dest_addr;
	len = _usdf_iov_len(iov, count);
	padding = USDF_HDR_BUF_ENTRY - sizeof(struct usd_udp_hdr);

	assert(len <= ep->max_msg_size);

	if (count > ep->e.dg.tx_iov_limit) {
		USDF_DBG_SYS(EP_DATA, "max iov count exceeded: %zu\n", count);
		return -FI_ENOSPC;
	}

	if ((len - padding) <= USD_SEND_MAX_COPY) {
		/* _usdf_dgram_send_iov_copy isn't prefix aware and allocates
		 * its own prefix. reorganize iov[0] base to point to data and
		 * len to reflect data length.
		 */
		memcpy(send_iov, iov, sizeof(struct iovec) * count);
		send_iov[0].iov_base = ((char *) send_iov[0].iov_base +
				USDF_HDR_BUF_ENTRY);
		send_iov[0].iov_len -= USDF_HDR_BUF_ENTRY;

		return _usdf_dgram_send_iov_copy(ep, dest, send_iov, count,
				context, ep->ep_tx_completion);
	} else if (ep->e.dg.tx_op_flags & FI_INJECT) {
		USDF_DBG_SYS(EP_DATA,
				"given inject length (%zu) exceeds max inject length (%d)\n",
				len, USD_SEND_MAX_COPY);
		return -FI_ENOSPC;
	}

	return _usdf_dgram_send_iov_prefix(ep, dest, iov, count, context,
					   ep->ep_tx_completion);
}

ssize_t
usdf_dgram_prefix_sendmsg(struct fid_ep *fep, const struct fi_msg *msg,
	uint64_t flags)
{
	struct iovec send_iov[USDF_DGRAM_MAX_SGE];
	struct usd_dest *dest;
	struct usdf_ep *ep;
	uint8_t completion;
	size_t len;
	size_t padding;

	ep = ep_ftou(fep);
	dest = (struct usd_dest *)(uintptr_t) msg->addr;
	len = _usdf_iov_len(msg->msg_iov, msg->iov_count);
	completion = ep->ep_tx_dflt_signal_comp || (flags & FI_COMPLETION);
	padding = USDF_HDR_BUF_ENTRY - sizeof(struct usd_udp_hdr);

	assert(len <= ep->max_msg_size);

	if (msg->iov_count > ep->e.dg.tx_iov_limit) {
		USDF_DBG_SYS(EP_DATA, "max iov count exceeded: %zu\n",
				msg->iov_count);
		return -FI_ENOSPC;
	}

	if ((len - padding) <= USD_SEND_MAX_COPY) {
		/* _usdf_dgram_send_iov_copy isn't prefix aware and allocates
		 * its own prefix. reorganize iov[0] base to point to data and
		 * len to reflect data length.
		 */
		memcpy(send_iov, msg->msg_iov,
				sizeof(struct iovec) * msg->iov_count);
		send_iov[0].iov_base = ((char *) send_iov[0].iov_base +
				USDF_HDR_BUF_ENTRY);
		send_iov[0].iov_len -= USDF_HDR_BUF_ENTRY;

		return _usdf_dgram_send_iov_copy(ep, dest, send_iov,
				msg->iov_count, msg->context, completion);
	} else if (flags & FI_INJECT) {
		USDF_DBG_SYS(EP_DATA,
				"given inject length (%zu) exceeds max inject length (%d)\n",
				len, USD_SEND_MAX_COPY);
		return -FI_ENOSPC;
	}

	return _usdf_dgram_send_iov_prefix(ep, dest, msg->msg_iov,
			msg->iov_count, msg->context, completion);
}

ssize_t usdf_dgram_prefix_rx_size_left(struct fid_ep *fep)
{
	struct usdf_ep *ep;

	USDF_DBG_SYS(EP_DATA, "\n");

	if (fep == NULL)
		return -FI_EINVAL;

	ep = ep_ftou(fep);

	if (!(ep->flags & USDF_EP_ENABLED))
		return -FI_EOPBADSTATE;

	/* prefix_recvv can post up to iov_limit descriptors
	 */
	return (usd_get_recv_credits(ep->e.dg.ep_qp) / ep->e.dg.rx_iov_limit);
}

ssize_t usdf_dgram_prefix_tx_size_left(struct fid_ep *fep)
{
	struct usdf_ep *ep;

	USDF_DBG_SYS(EP_DATA, "\n");

	if (fep == NULL)
		return -FI_EINVAL;

	ep = ep_ftou(fep);

	if (!(ep->flags & USDF_EP_ENABLED))
		return -FI_EOPBADSTATE;

	/* prefix_sendvcan post up to iov_limit descriptors
	 */
	return (usd_get_send_credits(ep->e.dg.ep_qp) / ep->e.dg.tx_iov_limit);
}
