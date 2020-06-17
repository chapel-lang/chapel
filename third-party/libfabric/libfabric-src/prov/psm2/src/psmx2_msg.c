/*
 * Copyright (c) 2013-2019 Intel Corporation. All rights reserved.
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

#include "psmx2.h"
#include "psmx2_trigger.h"

ssize_t psmx2_recv_generic(struct fid_ep *ep, void *buf, size_t len,
			   void *desc, fi_addr_t src_addr, void *context,
			   uint64_t flags)
{
	struct psmx2_fid_ep *ep_priv;
	struct psmx2_fid_av *av;
	psm2_epaddr_t psm2_epaddr;
	psm2_mq_req_t psm2_req;
	psm2_mq_tag_t psm2_tag, psm2_tagsel;
	struct fi_context *fi_context;
	int recv_flag = 0;
	int err;
	int enable_completion;

	ep_priv = container_of(ep, struct psmx2_fid_ep, ep);

	if (flags & FI_TRIGGER)
		return psmx2_trigger_queue_recv(ep, buf, len, desc, src_addr,
						context, flags);

	if ((ep_priv->caps & FI_DIRECTED_RECV) && src_addr != FI_ADDR_UNSPEC) {
		av = ep_priv->av;
		assert(av);
		psm2_epaddr = psmx2_av_translate_addr(av, ep_priv->rx, src_addr, av->type);
	} else {
		psm2_epaddr = 0;
	}

	PSMX2_SET_TAG(psm2_tag, 0ULL, 0, PSMX2_TYPE_MSG);
	PSMX2_SET_MASK(psm2_tagsel, PSMX2_MATCH_NONE, PSMX2_TYPE_MASK);

	enable_completion = !ep_priv->recv_selective_completion ||
			    (flags & FI_COMPLETION);
	if (enable_completion) {
		assert(context);
		fi_context = context;
		if (flags & FI_MULTI_RECV) {
			struct psmx2_multi_recv *req;

			req = calloc(1, sizeof(*req));
			if (!req)
				return -FI_ENOMEM;

			req->src_addr = psm2_epaddr;
			req->tag = psm2_tag;
			req->tagsel = psm2_tagsel;
			req->flag = recv_flag;
			req->buf = buf;
			req->len = len;
			req->offset = 0;
			req->min_buf_size = ep_priv->min_multi_recv;
			req->context = fi_context; 
			PSMX2_CTXT_TYPE(fi_context) = PSMX2_MULTI_RECV_CONTEXT;
			PSMX2_CTXT_USER(fi_context) = req;
			if (len > PSMX2_MAX_MSG_SIZE)
				len = PSMX2_MAX_MSG_SIZE;
		} else {
			PSMX2_CTXT_TYPE(fi_context) = PSMX2_RECV_CONTEXT;
			PSMX2_CTXT_USER(fi_context) = buf;
		}
		PSMX2_CTXT_EP(fi_context) = ep_priv;
		PSMX2_CTXT_SIZE(fi_context) = len;
	} else {
		PSMX2_EP_GET_OP_CONTEXT(ep_priv, fi_context);
		#if !PSMX2_USE_REQ_CONTEXT
		PSMX2_CTXT_TYPE(fi_context) = PSMX2_NOCOMP_RECV_CONTEXT;
		PSMX2_CTXT_EP(fi_context) = ep_priv;
		PSMX2_CTXT_USER(fi_context) = buf;
		PSMX2_CTXT_SIZE(fi_context) = len;
		#endif
	}

	err = psm2_mq_irecv2(ep_priv->rx->psm2_mq, psm2_epaddr,
			     &psm2_tag, &psm2_tagsel, recv_flag, buf, len,
			     (void *)fi_context, &psm2_req);
	if (OFI_UNLIKELY(err != PSM2_OK))
		return psmx2_errno(err);

	if (enable_completion) {
		PSMX2_CTXT_REQ(fi_context) = psm2_req;
	} else {
		#if PSMX2_USE_REQ_CONTEXT
		PSMX2_REQ_GET_OP_CONTEXT(psm2_req, fi_context);
		PSMX2_CTXT_TYPE(fi_context) = PSMX2_NOCOMP_RECV_CONTEXT;
		PSMX2_CTXT_EP(fi_context) = ep_priv;
		PSMX2_CTXT_USER(fi_context) = buf;
		PSMX2_CTXT_SIZE(fi_context) = len;
		#endif
	}

	return 0;
}

DIRECT_FN
STATIC ssize_t psmx2_recv(struct fid_ep *ep, void *buf, size_t len,
			  void *desc, fi_addr_t src_addr, void *context)
{
	struct psmx2_fid_ep *ep_priv;

	ep_priv = container_of(ep, struct psmx2_fid_ep, ep);

	return psmx2_recv_generic(ep, buf, len, desc, src_addr, context,
				  ep_priv->rx_flags);
}

DIRECT_FN
STATIC ssize_t psmx2_recvmsg(struct fid_ep *ep, const struct fi_msg *msg,
			     uint64_t flags)
{
	void *buf;
	size_t len;

	assert(msg);
	assert(!msg->iov_count || msg->msg_iov);
	assert(msg->iov_count <= 1);

	if (msg->iov_count) {
		buf = msg->msg_iov[0].iov_base;
		len = msg->msg_iov[0].iov_len;
	} else {
		buf = NULL;
		len = 0;
	}

	return psmx2_recv_generic(ep, buf, len,
				  msg->desc ? msg->desc[0] : NULL,
				  msg->addr, msg->context, flags);
}

DIRECT_FN
STATIC ssize_t psmx2_recvv(struct fid_ep *ep, const struct iovec *iov,
			   void **desc, size_t count, fi_addr_t src_addr,
			   void *context)
{
	void *buf;
	size_t len;

	assert(!count || iov);
	assert(count <= 1);

	if (count) {
		buf = iov[0].iov_base;
		len = iov[0].iov_len;
	} else {
		buf = NULL;
		len = 0;
	}

	return psmx2_recv(ep, buf, len, desc ? desc[0] : NULL,
			  src_addr, context);
}

ssize_t psmx2_send_generic(struct fid_ep *ep, const void *buf, size_t len,
			   void *desc, fi_addr_t dest_addr, void *context,
			   uint64_t flags, uint64_t data)
{
	struct psmx2_fid_ep *ep_priv;
	struct psmx2_fid_av *av;
	psm2_epaddr_t psm2_epaddr;
	psm2_mq_req_t psm2_req;
	psm2_mq_tag_t psm2_tag;
	struct fi_context * fi_context;
	int send_flag = 0;
	int err;
	int no_completion = 0;
	struct psmx2_cq_event *event;
	int have_data = (flags & FI_REMOTE_CQ_DATA) > 0;

	ep_priv = container_of(ep, struct psmx2_fid_ep, ep);

	if (flags & FI_TRIGGER)
		return psmx2_trigger_queue_send(ep, buf, len, desc, dest_addr,
						context, flags, data);

	av = ep_priv->av;
	assert(av);
	psm2_epaddr = psmx2_av_translate_addr(av, ep_priv->tx, dest_addr, av->type);

	PSMX2_SET_TAG(psm2_tag, 0, data, PSMX2_TYPE_MSG | PSMX2_IMM_BIT_SET(have_data));

	if ((flags & PSMX2_NO_COMPLETION) ||
	    (ep_priv->send_selective_completion && !(flags & FI_COMPLETION)))
		no_completion = 1;

	if (flags & FI_INJECT) {
		if (len > psmx2_env.inject_size)
			return -FI_EMSGSIZE;

		err = psm2_mq_send2(ep_priv->tx->psm2_mq, psm2_epaddr,
				    send_flag, &psm2_tag, buf, len);

		if (err != PSM2_OK)
			return psmx2_errno(err);

		if (ep_priv->send_cntr)
			psmx2_cntr_inc(ep_priv->send_cntr, 0);

		if (ep_priv->send_cq && !no_completion) {
			event = psmx2_cq_create_event(
					ep_priv->send_cq,
					context, (void *)buf, flags, len,
					(uint64_t) data,
					0 /* tag */,
					0 /* olen */,
					0 /* err */);

			if (event)
				psmx2_cq_enqueue_event(ep_priv->send_cq, event);
			else
				return -FI_ENOMEM;
		}

		return 0;
	}

	if (no_completion) {
		fi_context = &ep_priv->nocomp_send_context;
	} else {
		assert(context);
		fi_context = context;
		PSMX2_CTXT_TYPE(fi_context) = PSMX2_SEND_CONTEXT;
		PSMX2_CTXT_USER(fi_context) = (void *)buf;
		PSMX2_CTXT_EP(fi_context) = ep_priv;
	}

	err = psm2_mq_isend2(ep_priv->tx->psm2_mq, psm2_epaddr,
			     send_flag, &psm2_tag, buf, len,
			     (void *)fi_context, &psm2_req);

	if (err != PSM2_OK)
		return psmx2_errno(err);

	if (fi_context == context)
		PSMX2_CTXT_REQ(fi_context) = psm2_req;

	return 0;
}

ssize_t psmx2_sendv_generic(struct fid_ep *ep, const struct iovec *iov,
			    void **desc, size_t count, fi_addr_t dest_addr,
			    void *context, uint64_t flags, uint64_t data)
{
	struct psmx2_fid_ep *ep_priv;
	struct psmx2_fid_av *av;
	psm2_epaddr_t psm2_epaddr;
	psm2_mq_req_t psm2_req;
	psm2_mq_tag_t psm2_tag;
	uint32_t msg_flags;
	struct fi_context * fi_context;
	int send_flag = 0;
	int err;
	int no_completion = 0;
	struct psmx2_cq_event *event;
	size_t real_count;
	size_t len, total_len;
	char *p;
	uint32_t *q;
	int i, j;
	struct psmx2_sendv_request *req;

	ep_priv = container_of(ep, struct psmx2_fid_ep, ep);

	if (flags & FI_TRIGGER)
		return psmx2_trigger_queue_sendv(ep, iov, desc, count,
						 dest_addr, context, flags,
						 data);

	total_len = 0;
	real_count = 0;
	for (i=0; i<count; i++) {
		if (iov[i].iov_len) {
			total_len += iov[i].iov_len;
			real_count++;
			j = i;
		}
	}

	if (real_count == 1)
		return psmx2_send_generic(ep, iov[j].iov_base, iov[j].iov_len,
					  desc ? desc[j] : NULL, dest_addr,
					  context, flags, data);

	req = malloc(sizeof(*req));
	if (!req)
		return -FI_ENOMEM;

	if (total_len <= PSMX2_IOV_BUF_SIZE) {
		req->iov_protocol = PSMX2_IOV_PROTO_PACK;
		p = req->buf;
		for (i=0; i<count; i++) {
			if (iov[i].iov_len) {
				memcpy(p, iov[i].iov_base, iov[i].iov_len);
				p += iov[i].iov_len;
			}
		}

		msg_flags = PSMX2_TYPE_MSG;
		len = total_len;
	} else {
		req->iov_protocol = PSMX2_IOV_PROTO_MULTI;
		req->iov_done = 0;
		req->iov_info.seq_num = (++ep_priv->iov_seq_num) %
					PSMX2_IOV_MAX_SEQ_NUM + 1;
		req->iov_info.count = (uint32_t)real_count;
		req->iov_info.total_len = (uint32_t)total_len;

		q = req->iov_info.len;
		for (i=0; i<count; i++) {
			if (iov[i].iov_len)
				*q++ = (uint32_t)iov[i].iov_len;
		}

		msg_flags = PSMX2_TYPE_MSG | PSMX2_IOV_BIT;
		len = (3 + real_count) * sizeof(uint32_t);
	}

	av = ep_priv->av;
	assert(av);
	psm2_epaddr = psmx2_av_translate_addr(av, ep_priv->tx, dest_addr, av->type);

	if (flags & FI_REMOTE_CQ_DATA)
		msg_flags |= PSMX2_IMM_BIT;

	PSMX2_SET_TAG(psm2_tag, 0ULL, data, msg_flags);

	if ((flags & PSMX2_NO_COMPLETION) ||
	    (ep_priv->send_selective_completion && !(flags & FI_COMPLETION)))
		no_completion = 1;

	if (flags & FI_INJECT) {
		if (len > psmx2_env.inject_size) {
			free(req);
			return -FI_EMSGSIZE;
		}

		err = psm2_mq_send2(ep_priv->tx->psm2_mq, psm2_epaddr,
				    send_flag, &psm2_tag, req->buf, len);

		free(req);

		if (err != PSM2_OK)
			return psmx2_errno(err);

		if (ep_priv->send_cntr)
			psmx2_cntr_inc(ep_priv->send_cntr, 0);

		if (ep_priv->send_cq && !no_completion) {
			event = psmx2_cq_create_event(
					ep_priv->send_cq,
					context, NULL, flags, len,
					(uint64_t) data,
					0 /* tag */,
					0 /* olen */,
					0 /* err */);

			if (event)
				psmx2_cq_enqueue_event(ep_priv->send_cq, event);
			else
				return -FI_ENOMEM;
		}

		return 0;
	}

	req->no_completion = no_completion;
	req->user_context = context;
	req->comp_flag = FI_MSG;

	fi_context = &req->fi_context;
	PSMX2_CTXT_TYPE(fi_context) = PSMX2_SENDV_CONTEXT;
	PSMX2_CTXT_USER(fi_context) = req;
	PSMX2_CTXT_EP(fi_context) = ep_priv;

	err = psm2_mq_isend2(ep_priv->tx->psm2_mq, psm2_epaddr,
			     send_flag, &psm2_tag, req->buf, len,
			     (void *)fi_context, &psm2_req);

	if (err != PSM2_OK) {
		free(req);
		return psmx2_errno(err);
	}

	PSMX2_CTXT_REQ(fi_context) = psm2_req;

	if (req->iov_protocol == PSMX2_IOV_PROTO_MULTI) {
		fi_context = &req->fi_context_iov;
		PSMX2_CTXT_TYPE(fi_context) = PSMX2_IOV_SEND_CONTEXT;
		PSMX2_CTXT_USER(fi_context) = req;
		PSMX2_CTXT_EP(fi_context) = ep_priv;
		PSMX2_SET_TAG(psm2_tag, req->iov_info.seq_num, 0, PSMX2_TYPE_IOV_PAYLOAD);
		for (i=0; i<count; i++) {
			if (iov[i].iov_len) {
				err = psm2_mq_isend2(ep_priv->tx->psm2_mq,
						     psm2_epaddr, send_flag, &psm2_tag,
						     iov[i].iov_base, iov[i].iov_len,
						     (void *)fi_context, &psm2_req);
				if (err != PSM2_OK)
					return psmx2_errno(err);
			}
		}
	}

	return 0;
}

int psmx2_handle_sendv_req(struct psmx2_fid_ep *ep,
			   PSMX2_STATUS_TYPE *status,
			   int multi_recv)
{
	psm2_mq_req_t psm2_req;
	psm2_mq_tag_t psm2_tag, psm2_tagsel;
	struct psmx2_sendv_reply *rep;
	struct psmx2_multi_recv *recv_req;
	struct fi_context *fi_context;
	struct fi_context *recv_context;
	int i, err;
	uint8_t *recv_buf;
	size_t recv_len, len;

	if (PSMX2_STATUS_ERROR(status) != PSM2_OK)
		return psmx2_errno(PSMX2_STATUS_ERROR(status));

	rep = malloc(sizeof(*rep));
	if (!rep) {
		PSMX2_STATUS_ERROR(status) = PSM2_NO_MEMORY;
		return -FI_ENOMEM;
	}

	recv_context = PSMX2_STATUS_CONTEXT(status);
	if (multi_recv) {
		recv_req = PSMX2_CTXT_USER(recv_context);
		recv_buf = recv_req->buf + recv_req->offset;
		recv_len = recv_req->len - recv_req->offset;
		rep->multi_recv = 1;
	} else {
		recv_buf = PSMX2_CTXT_USER(recv_context);
		recv_len = PSMX2_CTXT_SIZE(recv_context);
		rep->multi_recv = 0;
	}

	/* assert(PSMX2_STATUS_RCVLEN(status) <= PSMX2_IOV_BUF_SIZE); */

	memcpy(&rep->iov_info, recv_buf, PSMX2_STATUS_RCVLEN(status));

	rep->user_context = PSMX2_STATUS_CONTEXT(status);
	rep->tag = PSMX2_STATUS_TAG(status);
	rep->buf = recv_buf;
	rep->no_completion = 0;
	rep->iov_done = 0;
	rep->bytes_received = 0;
	rep->msg_length = 0;
	rep->error_code = PSM2_OK;

	fi_context = &rep->fi_context;
	PSMX2_CTXT_TYPE(fi_context) = PSMX2_IOV_RECV_CONTEXT;
	PSMX2_CTXT_USER(fi_context) = rep;
	PSMX2_CTXT_EP(fi_context) = ep;

	rep->comp_flag = PSMX2_IS_MSG(PSMX2_GET_FLAGS(rep->tag)) ? FI_MSG : FI_TAGGED;
	if (PSMX2_HAS_IMM(PSMX2_GET_FLAGS(rep->tag)))
		rep->comp_flag |= FI_REMOTE_CQ_DATA;

	/* IOV payload uses a sequence number in place of a tag. */
	PSMX2_SET_TAG(psm2_tag, rep->iov_info.seq_num, 0, PSMX2_TYPE_IOV_PAYLOAD);
	PSMX2_SET_MASK(psm2_tagsel, PSMX2_MATCH_ALL, PSMX2_TYPE_MASK);

	for (i=0; i<rep->iov_info.count; i++) {
		if (recv_len) {
			len = MIN(recv_len, rep->iov_info.len[i]);
			err = psm2_mq_irecv2(ep->rx->psm2_mq,
					     PSMX2_STATUS_PEER(status),
					     &psm2_tag, &psm2_tagsel,
					     0/*flag*/, recv_buf, len,
					     (void *)fi_context, &psm2_req);
			if (err) {
				PSMX2_STATUS_ERROR(status) = err;
				return psmx2_errno(err);
			}
			recv_buf += len;
			recv_len -= len;
		} else {
			/* recv buffer full, post empty recvs */
			err = psm2_mq_irecv2(ep->rx->psm2_mq,
					     PSMX2_STATUS_PEER(status),
					     &psm2_tag, &psm2_tagsel,
					     0/*flag*/, NULL, 0,
					     (void *)fi_context, &psm2_req);
			if (err) {
				PSMX2_STATUS_ERROR(status) = err;
				return psmx2_errno(err);
			}
		}
	}

	if (multi_recv && recv_len < recv_req->min_buf_size)
		rep->comp_flag |= FI_MULTI_RECV;

	return 0;
}

DIRECT_FN
STATIC ssize_t psmx2_send(struct fid_ep *ep, const void *buf, size_t len,
			  void *desc, fi_addr_t dest_addr, void *context)
{
	struct psmx2_fid_ep *ep_priv;

	ep_priv = container_of(ep, struct psmx2_fid_ep, ep);

	return psmx2_send_generic(ep, buf, len, desc, dest_addr, context,
				  ep_priv->tx_flags, 0);
}

DIRECT_FN
STATIC ssize_t psmx2_sendmsg(struct fid_ep *ep, const struct fi_msg *msg,
			     uint64_t flags)
{
	void *buf;
	size_t len;

	assert(msg);
	assert(!msg->iov_count || msg->msg_iov);
	assert(msg->iov_count <= PSMX2_IOV_MAX_COUNT);

	if (msg->iov_count > 1) {
		return psmx2_sendv_generic(ep, msg->msg_iov, msg->desc,
					   msg->iov_count, msg->addr,
					   msg->context, flags,
					   msg->data);
	} else if (msg->iov_count) {
		buf = msg->msg_iov[0].iov_base;
		len = msg->msg_iov[0].iov_len;
	} else {
		buf = NULL;
		len = 0;
	}

	return psmx2_send_generic(ep, buf, len,
				  msg->desc ? msg->desc[0] : NULL,
				  msg->addr, msg->context, flags,
				  msg->data);
}

DIRECT_FN
STATIC ssize_t psmx2_sendv(struct fid_ep *ep, const struct iovec *iov,
			   void **desc, size_t count, fi_addr_t dest_addr,
			   void *context)
{
	void *buf;
	size_t len;

	assert(!count || iov);
	assert(count <= PSMX2_IOV_MAX_COUNT);

	if (count > 1) {
		struct psmx2_fid_ep *ep_priv;
		ep_priv = container_of(ep, struct psmx2_fid_ep, ep);

		return psmx2_sendv_generic(ep, iov, desc, count, dest_addr,
					   context, ep_priv->tx_flags, 0);
	} else if (count) {
		buf = iov[0].iov_base;
		len = iov[0].iov_len;
	} else {
		buf = NULL;
		len = 0;
	}

	return psmx2_send(ep, buf, len, desc ? desc[0] : NULL,
			  dest_addr, context);
}

DIRECT_FN
STATIC ssize_t psmx2_inject(struct fid_ep *ep, const void *buf, size_t len,
			    fi_addr_t dest_addr)
{
	struct psmx2_fid_ep *ep_priv;

	ep_priv = container_of(ep, struct psmx2_fid_ep, ep);

	return psmx2_send_generic(ep, buf, len, NULL, dest_addr, NULL,
				  ep_priv->tx_flags | FI_INJECT | PSMX2_NO_COMPLETION, 
				  0);
}

DIRECT_FN
STATIC ssize_t psmx2_senddata(struct fid_ep *ep, const void *buf, size_t len,
			      void *desc, uint64_t data, fi_addr_t dest_addr,
			      void *context)
{
	struct psmx2_fid_ep *ep_priv;

	ep_priv = container_of(ep, struct psmx2_fid_ep, ep);

	return psmx2_send_generic(ep, buf, len, desc, dest_addr, context,
				  ep_priv->tx_flags | FI_REMOTE_CQ_DATA, data);
}

DIRECT_FN
STATIC ssize_t psmx2_injectdata(struct fid_ep *ep, const void *buf, size_t len,
				uint64_t data, fi_addr_t dest_addr)
{
	struct psmx2_fid_ep *ep_priv;

	ep_priv = container_of(ep, struct psmx2_fid_ep, ep);

	return psmx2_send_generic(ep, buf, len, NULL, dest_addr, NULL,
				  ep_priv->tx_flags | FI_INJECT | PSMX2_NO_COMPLETION |
					FI_REMOTE_CQ_DATA,
				  data);
}

struct fi_ops_msg psmx2_msg_ops = {
	.size = sizeof(struct fi_ops_msg),
	.recv = psmx2_recv,
	.recvv = psmx2_recvv,
	.recvmsg = psmx2_recvmsg,
	.send = psmx2_send,
	.sendv = psmx2_sendv,
	.sendmsg = psmx2_sendmsg,
	.inject = psmx2_inject,
	.senddata = psmx2_senddata,
	.injectdata = psmx2_injectdata,
};

