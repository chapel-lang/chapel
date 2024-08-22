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

#include "psmx3.h"
#include "psmx3_trigger.h"

static ssize_t psmx3_tagged_peek_generic(struct fid_ep *ep,
					 void *buf, size_t len,
					 void *desc, fi_addr_t src_addr,
					 uint64_t tag, uint64_t ignore,
					 void *context, uint64_t flags)
{
	struct psmx3_fid_ep *ep_priv;
	struct psmx3_fid_av *av;
	struct psmx3_cq_event *event;
	psm2_epaddr_t psm2_epaddr;
	psm2_mq_req_t req = NULL;
	psm2_mq_status2_t psm2_status;
	psm2_mq_tag_t psm2_tag, psm2_tagsel;
	uint64_t data;
	int err;

	ep_priv = container_of(ep, struct psmx3_fid_ep, ep);

	if ((ep_priv->caps & FI_DIRECTED_RECV) && src_addr != FI_ADDR_UNSPEC) {
		av = ep_priv->av;
		assert(av);
		psm2_epaddr = psmx3_av_translate_addr(av, ep_priv->rx, src_addr, av->type);
	} else {
		psm2_epaddr = 0;
	}

	PSMX3_SET_TAG(psm2_tag, tag, 0, PSMX3_TYPE_TAGGED);
	PSMX3_SET_MASK(psm2_tagsel, ~ignore, PSMX3_TYPE_MASK);

	if (flags & (FI_CLAIM | FI_DISCARD))
		err = psm3_mq_improbe2(ep_priv->rx->psm2_mq,
				       psm2_epaddr, &psm2_tag,
				       &psm2_tagsel, &req, &psm2_status);
	else
		err = psm3_mq_iprobe2(ep_priv->rx->psm2_mq,
				      psm2_epaddr, &psm2_tag, &psm2_tagsel,
				      &psm2_status);
	switch (err) {
	case PSM2_OK:
		if (ep_priv->recv_cq) {
			if (flags & FI_CLAIM) {
				if (context)
					PSMX3_CTXT_REQ((struct fi_context *)context) = req;
			} else if (flags & FI_DISCARD) {
				if (!psm3_mq_imrecv(ep_priv->rx->psm2_mq, 0,
						    NULL, 0, req, &req))
					psm3_mq_wait2(&req, NULL);
			}

			tag = PSMX3_GET_TAG64(psm2_status.msg_tag);
			if (PSMX3_HAS_IMM(PSMX3_GET_FLAGS(psm2_status.msg_tag))) {
				data = PSMX3_GET_CQDATA(psm2_status.msg_tag);
				flags |= FI_REMOTE_CQ_DATA;
			} else {
				data = 0;
			}
			len = psm2_status.msg_length;
			event = psmx3_cq_create_event(
					ep_priv->recv_cq,
					context,		/* op_context */
					NULL,			/* buf */
					flags|FI_RECV|FI_TAGGED,/* flags */
					len,			/* len */
					data,			/* data */
					tag,			/* tag */
					len,			/* olen */
					0);			/* err */

			if (!event)
				return -FI_ENOMEM;

			event->source_is_valid = 1;
			event->source = psm2_status.msg_peer;
			event->source_av = ep_priv->av;
			psmx3_cq_enqueue_event(ep_priv->recv_cq, event);
		}
		return 0;

	case PSM2_MQ_NO_COMPLETIONS:
		if (ep_priv->recv_cq) {
			event = psmx3_cq_create_event(
					ep_priv->recv_cq,
					context,		/* op_context */
					NULL,			/* buf */
					flags|FI_RECV|FI_TAGGED,/* flags */
					len,			/* len */
					0,			/* data */
					tag,			/* tag */
					len,			/* olen */
					-FI_ENOMSG);		/* err */

			if (!event)
				return -FI_ENOMEM;

			event->source = 0;
			psmx3_cq_enqueue_event(ep_priv->recv_cq, event);
		}
		return 0;

	default:
		return psmx3_errno(err);
	}
}

ssize_t psmx3_tagged_recv_generic(struct fid_ep *ep, void *buf,
				  size_t len, void *desc,
				  fi_addr_t src_addr, 
				  uint64_t tag, uint64_t ignore,
				  void *context, uint64_t flags)
{
	struct psmx3_fid_ep *ep_priv;
	struct psmx3_fid_av *av;
	psm2_epaddr_t psm2_epaddr;
	psm2_mq_req_t psm2_req;
	psm2_mq_tag_t psm2_tag, psm2_tagsel;
	struct fi_context *fi_context;
	int err;
	int enable_completion;

	ep_priv = container_of(ep, struct psmx3_fid_ep, ep);

	if (flags & FI_PEEK)
		return psmx3_tagged_peek_generic(ep, buf, len, desc,
						 src_addr, tag, ignore,
						 context, flags);

	if (flags & FI_TRIGGER)
		return psmx3_trigger_queue_trecv(ep, buf, len, desc,
						 src_addr, tag, ignore,
						 context, flags);
 
	if (flags & FI_CLAIM) {
		assert(context);
		if (flags & FI_DISCARD) {
			psm2_mq_status2_t psm2_status;
			struct psmx3_cq_event *event;

			fi_context = context;
			psm2_req = PSMX3_CTXT_REQ(fi_context);
			err = psm3_mq_imrecv(ep_priv->rx->psm2_mq, 0,
					     NULL, 0, context, &psm2_req);
			if (err != PSM2_OK)
				return psmx3_errno(err);

			psm3_mq_wait2(&psm2_req, &psm2_status);

			if (ep_priv->recv_cq &&
			    (!ep_priv->recv_selective_completion || (flags & FI_COMPLETION))) {
				tag = PSMX3_GET_TAG64(psm2_status.msg_tag);
				event = psmx3_cq_create_event(
						ep_priv->recv_cq,
						context,		/* op_context */
						NULL,			/* buf */
						flags|FI_RECV|FI_TAGGED,/* flags */
						0,			/* len */
						0,			/* data */
						tag,			/* tag */
						0,			/* olen */
						0);			/* err */

				if (!event)
					return -FI_ENOMEM;

				event->source_is_valid = 1;
				event->source = psm2_status.msg_peer;
				event->source_av = ep_priv->av;
				psmx3_cq_enqueue_event(ep_priv->recv_cq, event);
			}

			if (ep_priv->recv_cntr)
				psmx3_cntr_inc(ep_priv->recv_cntr, 0);

			return 0;
		}

		fi_context = context;
		psm2_req = PSMX3_CTXT_REQ(fi_context);
		PSMX3_CTXT_TYPE(fi_context) = PSMX3_TRECV_CONTEXT;
		PSMX3_CTXT_USER(fi_context) = buf;
		PSMX3_CTXT_EP(fi_context) = ep_priv;

		err = psm3_mq_imrecv(ep_priv->rx->psm2_mq, 0,
				     buf, len, context, &psm2_req);
		if (err != PSM2_OK)
			return psmx3_errno(err);

		PSMX3_CTXT_REQ(fi_context) = psm2_req;
		return 0;
	}

	enable_completion = !ep_priv->recv_selective_completion || (flags & FI_COMPLETION);

	if (enable_completion) {
		assert(context);
		fi_context = context;
		PSMX3_CTXT_TYPE(fi_context) = PSMX3_TRECV_CONTEXT;
		PSMX3_CTXT_EP(fi_context) = ep_priv;
		PSMX3_CTXT_USER(fi_context) = buf;
		PSMX3_CTXT_SIZE(fi_context) = len;
	} else {
		PSMX3_EP_GET_OP_CONTEXT(ep_priv, fi_context);
		#if !PSMX3_USE_REQ_CONTEXT
		PSMX3_CTXT_TYPE(fi_context) = PSMX3_NOCOMP_TRECV_CONTEXT;
		PSMX3_CTXT_EP(fi_context) = ep_priv;
		PSMX3_CTXT_USER(fi_context) = buf;
		PSMX3_CTXT_SIZE(fi_context) = len;
		#endif
	}

	if ((ep_priv->caps & FI_DIRECTED_RECV) && src_addr != FI_ADDR_UNSPEC) {
		av = ep_priv->av;
		assert(av);
		psm2_epaddr = psmx3_av_translate_addr(av, ep_priv->rx, src_addr, av->type);
	} else {
		psm2_epaddr = 0;
	}

	PSMX3_SET_TAG(psm2_tag, tag, 0, PSMX3_TYPE_TAGGED);
	PSMX3_SET_MASK(psm2_tagsel, ~ignore, PSMX3_TYPE_MASK);

	err = psm3_mq_irecv2(ep_priv->rx->psm2_mq, psm2_epaddr,
			     &psm2_tag, &psm2_tagsel, 0, buf, len,
			     (void *)fi_context, &psm2_req);

	if (err != PSM2_OK)
		return psmx3_errno(err);

	if (enable_completion) {
		PSMX3_CTXT_REQ(fi_context) = psm2_req;
	} else {
		#if PSMX3_USE_REQ_CONTEXT
		PSMX3_REQ_GET_OP_CONTEXT(psm2_req, fi_context);
		PSMX3_CTXT_TYPE(fi_context) = PSMX3_NOCOMP_TRECV_CONTEXT;
		PSMX3_CTXT_EP(fi_context) = ep_priv;
		PSMX3_CTXT_USER(fi_context) = buf;
		PSMX3_CTXT_SIZE(fi_context) = len;
		#endif
	}

	return 0;
}

__attribute__((always_inline))
static inline ssize_t
psmx3_tagged_recv_specialized(struct fid_ep *ep, void *buf, size_t len,
			      void *desc, fi_addr_t src_addr,
			      uint64_t tag, uint64_t ignore,
			      void *context,
			      int enable_completion,
			      int directed_receive,
			      int av_map)
{
	struct psmx3_fid_ep *ep_priv;
	psm2_epaddr_t psm2_epaddr;
	psm2_mq_req_t psm2_req;
	psm2_mq_tag_t psm2_tag, psm2_tagsel;
	struct fi_context *fi_context;
	int err;

	ep_priv = container_of(ep, struct psmx3_fid_ep, ep);

	if (enable_completion) {
		fi_context = context;
		PSMX3_CTXT_TYPE(fi_context) = PSMX3_TRECV_CONTEXT;
		PSMX3_CTXT_EP(fi_context) = ep_priv;
		PSMX3_CTXT_USER(fi_context) = buf;
		PSMX3_CTXT_SIZE(fi_context) = len;
	} else {
		PSMX3_EP_GET_OP_CONTEXT(ep_priv, fi_context);
		#if !PSMX3_USE_REQ_CONTEXT
		PSMX3_CTXT_TYPE(fi_context) = PSMX3_NOCOMP_TRECV_CONTEXT;
		PSMX3_CTXT_EP(fi_context) = ep_priv;
		PSMX3_CTXT_USER(fi_context) = buf;
		PSMX3_CTXT_SIZE(fi_context) = len;
		#endif
	}

	if (directed_receive && src_addr != FI_ADDR_UNSPEC) {
		if (av_map) {
			psm2_epaddr = (psm2_epaddr_t)src_addr;
		} else {
			assert(ep_priv->av);
			psm2_epaddr = psmx3_av_translate_addr(ep_priv->av, ep_priv->rx, src_addr, FI_AV_TABLE);
		}
	} else {
		psm2_epaddr = 0;
	}

	PSMX3_SET_TAG(psm2_tag, tag, 0, PSMX3_TYPE_TAGGED);
	PSMX3_SET_MASK(psm2_tagsel, ~ignore, PSMX3_TYPE_MASK);

	err = psm3_mq_irecv2(ep_priv->rx->psm2_mq, psm2_epaddr,
			     &psm2_tag, &psm2_tagsel, 0, buf, len,
			     (void *)fi_context, &psm2_req);

	if (OFI_UNLIKELY((err != PSM2_OK)))
			return psmx3_errno(err);

	if (enable_completion) {
		PSMX3_CTXT_REQ(fi_context) = psm2_req;
	} else {
		#if PSMX3_USE_REQ_CONTEXT
		PSMX3_REQ_GET_OP_CONTEXT(psm2_req, fi_context);
		PSMX3_CTXT_TYPE(fi_context) = PSMX3_NOCOMP_TRECV_CONTEXT;
		PSMX3_CTXT_EP(fi_context) = ep_priv;
		PSMX3_CTXT_USER(fi_context) = buf;
		PSMX3_CTXT_SIZE(fi_context) = len;
		#endif
	}

	return 0;
}

/* op_flags=0, FI_SELECTIVE_COMPLETION not set, FI_DIRECTED_RECEIVE not set, av table */
static ssize_t
psmx3_tagged_recv_no_flag_undirected(struct fid_ep *ep, void *buf, size_t len,
				     void *desc, fi_addr_t src_addr,
				     uint64_t tag, uint64_t ignore,
				     void *context)
{
	return psmx3_tagged_recv_specialized(ep, buf, len, desc, src_addr,
					     tag, ignore, context, 1, 0, 0);
}

/* op_flags=0, FI_SELECTIVE_COMPLETION set, FI_DIRECTED_RECEIVE not set, av table */
static ssize_t
psmx3_tagged_recv_no_event_undirected(struct fid_ep *ep, void *buf, size_t len,
				      void *desc, fi_addr_t src_addr,
				      uint64_t tag, uint64_t ignore,
				      void *context)
{
	return psmx3_tagged_recv_specialized(ep, buf, len, desc, src_addr,
					     tag, ignore, context, 0, 0, 0);
}

/* op_flags=0, FI_SELECTIVE_COMPLETION not set, FI_DIRECTED_RECEIVE set, av_table */
static ssize_t
psmx3_tagged_recv_no_flag_directed(struct fid_ep *ep, void *buf, size_t len,
				   void *desc, fi_addr_t src_addr,
				   uint64_t tag, uint64_t ignore,
				   void *context)
{
	return psmx3_tagged_recv_specialized(ep, buf, len, desc, src_addr,
					     tag, ignore, context, 1, 1, 0);
}

/* op_flags=0, FI_SELECTIVE_COMPLETION set, FI_DIRECTED_RECEIVE set, av table */
static ssize_t
psmx3_tagged_recv_no_event_directed(struct fid_ep *ep, void *buf, size_t len,
				    void *desc, fi_addr_t src_addr,
				    uint64_t tag, uint64_t ignore,
				    void *context)
{
	return psmx3_tagged_recv_specialized(ep, buf, len, desc, src_addr,
					     tag, ignore, context, 0, 1, 0);
}

/* op_flags=0, FI_SELECTIVE_COMPLETION not set, FI_DIRECTED_RECEIVE not set, av map */
static ssize_t
psmx3_tagged_recv_no_flag_undirected_av_map(struct fid_ep *ep, void *buf, size_t len,
					    void *desc, fi_addr_t src_addr,
					    uint64_t tag, uint64_t ignore,
					    void *context)
{
	return psmx3_tagged_recv_specialized(ep, buf, len, desc, src_addr,
					     tag, ignore, context, 1, 0, 1);
}

/* op_flags=0, FI_SELECTIVE_COMPLETION set, FI_DIRECTED_RECEIVE not set, av map */
static ssize_t
psmx3_tagged_recv_no_event_undirected_av_map(struct fid_ep *ep, void *buf, size_t len,
					     void *desc, fi_addr_t src_addr,
					     uint64_t tag, uint64_t ignore,
					     void *context)
{
	return psmx3_tagged_recv_specialized(ep, buf, len, desc, src_addr,
					     tag, ignore, context, 0, 0, 1);
}

/* op_flags=0, FI_SELECTIVE_COMPLETION not set, FI_DIRECTED_RECEIVE set, av_map */
static ssize_t
psmx3_tagged_recv_no_flag_directed_av_map(struct fid_ep *ep, void *buf, size_t len,
					  void *desc, fi_addr_t src_addr,
					  uint64_t tag, uint64_t ignore,
					  void *context)
{
	return psmx3_tagged_recv_specialized(ep, buf, len, desc, src_addr,
					     tag, ignore, context, 1, 1, 1);
}

/* op_flags=0, FI_SELECTIVE_COMPLETION set, FI_DIRECTED_RECEIVE set, av map */
static ssize_t
psmx3_tagged_recv_no_event_directed_av_map(struct fid_ep *ep, void *buf, size_t len,
					   void *desc, fi_addr_t src_addr,
					   uint64_t tag, uint64_t ignore,
					   void *context)
{
	return psmx3_tagged_recv_specialized(ep, buf, len, desc, src_addr,
					     tag, ignore, context, 0, 1, 1);
}

static ssize_t psmx3_tagged_recv(struct fid_ep *ep, void *buf, size_t len,
				 void *desc, fi_addr_t src_addr, uint64_t tag,
				 uint64_t ignore, void *context)
{
	struct psmx3_fid_ep *ep_priv;

	ep_priv = container_of(ep, struct psmx3_fid_ep, ep);

	return psmx3_tagged_recv_generic(ep, buf, len, desc, src_addr, tag,
					 ignore, context, ep_priv->rx_flags);
}

static ssize_t psmx3_tagged_recvmsg(struct fid_ep *ep,
				    const struct fi_msg_tagged *msg,
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

	return psmx3_tagged_recv_generic(ep, buf, len,
					 msg->desc ? msg->desc[0] : NULL,
					 msg->addr, msg->tag, msg->ignore,
					 msg->context, flags);
}

#define PSMX3_TAGGED_RECVV_FUNC(suffix)					\
static ssize_t								\
psmx3_tagged_recvv##suffix(struct fid_ep *ep, const struct iovec *iov,	\
			   void **desc, size_t count,			\
			   fi_addr_t src_addr, uint64_t tag,		\
			   uint64_t ignore, void *context)		\
{									\
	void *buf;							\
	size_t len;							\
	assert(!count || iov);						\
	assert(count <= 1);						\
	if (count) {							\
		buf = iov[0].iov_base;					\
		len = iov[0].iov_len;					\
	} else {							\
		buf = NULL;						\
		len = 0;						\
	}								\
	return psmx3_tagged_recv##suffix(ep, buf, len,			\
					 desc ? desc[0] : NULL,		\
				 	 src_addr, tag, ignore, 	\
					 context); 			\
}

PSMX3_TAGGED_RECVV_FUNC()
PSMX3_TAGGED_RECVV_FUNC(_no_flag_directed)
PSMX3_TAGGED_RECVV_FUNC(_no_event_directed)
PSMX3_TAGGED_RECVV_FUNC(_no_flag_undirected)
PSMX3_TAGGED_RECVV_FUNC(_no_event_undirected)
PSMX3_TAGGED_RECVV_FUNC(_no_flag_directed_av_map)
PSMX3_TAGGED_RECVV_FUNC(_no_event_directed_av_map)
PSMX3_TAGGED_RECVV_FUNC(_no_flag_undirected_av_map)
PSMX3_TAGGED_RECVV_FUNC(_no_event_undirected_av_map)

ssize_t psmx3_tagged_send_generic(struct fid_ep *ep,
				  const void *buf, size_t len,
				  void *desc, fi_addr_t dest_addr,
				  uint64_t tag, void *context,
				  uint64_t flags, uint64_t data)
{
	struct psmx3_fid_ep *ep_priv;
	struct psmx3_fid_av *av;
	psm2_epaddr_t psm2_epaddr;
	psm2_mq_req_t psm2_req;
	psm2_mq_tag_t psm2_tag;
	struct fi_context *fi_context;
	int err;
	int no_completion = 0;
	struct psmx3_cq_event *event;
	int have_data = (flags & FI_REMOTE_CQ_DATA) > 0;

	assert((tag & ~PSMX3_TAG_MASK) == 0);

	ep_priv = container_of(ep, struct psmx3_fid_ep, ep);

	if (flags & FI_TRIGGER)
		return psmx3_trigger_queue_tsend(ep, buf, len, desc,
						 dest_addr, tag, context,
						 flags, data);

	av = ep_priv->av;
	assert(av);
	psm2_epaddr = psmx3_av_translate_addr(av, ep_priv->tx, dest_addr, av->type);

	if (have_data)
		PSMX3_SET_TAG(psm2_tag, tag, (uint32_t)data,
			      PSMX3_TYPE_TAGGED | PSMX3_IMM_BIT);
	else
		PSMX3_SET_TAG(psm2_tag, tag, (uint32_t)ep_priv->sep_id,
			      PSMX3_TYPE_TAGGED);

	if ((flags & PSMX3_NO_COMPLETION) ||
	    (ep_priv->send_selective_completion && !(flags & FI_COMPLETION)))
		no_completion = 1;

	if (flags & FI_INJECT) {
		if (len > psmx3_env.inject_size)
			return -FI_EMSGSIZE;

		err = psm3_mq_send2(ep_priv->tx->psm2_mq, psm2_epaddr,
				    PSM2_MQ_FLAG_INJECT, &psm2_tag, buf, len);

		if (err != PSM2_OK)
			return psmx3_errno(err);

		if (ep_priv->send_cntr)
			psmx3_cntr_inc(ep_priv->send_cntr, 0);

		if (ep_priv->send_cq && !no_completion) {
			event = psmx3_cq_create_event(
					ep_priv->send_cq,
					context, (void *)buf, flags, len,
					(uint64_t) data, tag,
					0 /* olen */,
					0 /* err */);

			if (event)
				psmx3_cq_enqueue_event(ep_priv->send_cq, event);
			else
				return -FI_ENOMEM;
		}

		return 0;
	}

	if (no_completion) {
		fi_context = &ep_priv->nocomp_tsend_context;
	} else {
		assert(context);
		fi_context = context;
		PSMX3_CTXT_TYPE(fi_context) = PSMX3_TSEND_CONTEXT;
		PSMX3_CTXT_USER(fi_context) = (void *)buf;
		PSMX3_CTXT_EP(fi_context) = ep_priv;
	}

	err = psm3_mq_isend2(ep_priv->tx->psm2_mq, psm2_epaddr, 0,
			     &psm2_tag, buf, len, (void*)fi_context,
			     &psm2_req);

	if (err != PSM2_OK)
		return psmx3_errno(err);

	if (fi_context == context)
		PSMX3_CTXT_REQ(fi_context) = psm2_req;

	return 0;
}

__attribute__((always_inline))
static inline ssize_t
psmx3_tagged_send_specialized(struct fid_ep *ep, const void *buf,
			      size_t len, void *desc,
			      fi_addr_t dest_addr, uint64_t tag,
			      void *context,
			      int enable_completion, int av_map,
			      int have_data, uint64_t data)
{
	struct psmx3_fid_ep *ep_priv;
	psm2_epaddr_t psm2_epaddr;
	psm2_mq_req_t psm2_req;
	psm2_mq_tag_t psm2_tag;
	struct fi_context *fi_context;
	int err;

	assert((tag & ~PSMX3_TAG_MASK) == 0);

	ep_priv = container_of(ep, struct psmx3_fid_ep, ep);

	if (av_map) {
		psm2_epaddr = (psm2_epaddr_t)dest_addr;
	} else {
		assert(ep_priv->av);
		psm2_epaddr = psmx3_av_translate_addr(ep_priv->av, ep_priv->tx, dest_addr, FI_AV_TABLE);
	}

	if (have_data)
		PSMX3_SET_TAG(psm2_tag, tag, data, PSMX3_TYPE_TAGGED | PSMX3_IMM_BIT);
	else
		PSMX3_SET_TAG(psm2_tag, tag, ep_priv->sep_id, PSMX3_TYPE_TAGGED);

	if (enable_completion) {
		fi_context = context;
		PSMX3_CTXT_TYPE(fi_context) = PSMX3_TSEND_CONTEXT;
		PSMX3_CTXT_USER(fi_context) = (void *)buf;
		PSMX3_CTXT_EP(fi_context) = ep_priv;
	} else {
		fi_context = &ep_priv->nocomp_tsend_context;
	}

	err = psm3_mq_isend2(ep_priv->tx->psm2_mq, psm2_epaddr, 0,
			     &psm2_tag, buf, len, (void*)fi_context,
			     &psm2_req);

	if (err != PSM2_OK)
		return psmx3_errno(err);

	if (enable_completion)
		PSMX3_CTXT_REQ(fi_context) = psm2_req;

	return 0;
}

/* op_flags=0, FI_SELECTIVE_COMPLETION not set, av_table */
static ssize_t
psmx3_tagged_send_no_flag(struct fid_ep *ep, const void *buf, size_t len,
			  void *desc, fi_addr_t dest_addr, uint64_t tag,
			  void *context)
{
	return psmx3_tagged_send_specialized(ep, buf, len, desc, dest_addr, tag,
					     context, 1, 0, 0, 0);
}

/* op_flags=0, FI_SELECTIVE_COMPLETION set, av_table */
static ssize_t
psmx3_tagged_send_no_event(struct fid_ep *ep, const void *buf, size_t len,
			   void *desc, fi_addr_t dest_addr, uint64_t tag,
			   void *context)
{
	return psmx3_tagged_send_specialized(ep, buf, len, desc, dest_addr, tag,
					     context, 0, 0, 0, 0);
}

/* op_flags=0, FI_SELECTIVE_COMPLETION not set, av_map */
static ssize_t
psmx3_tagged_send_no_flag_av_map(struct fid_ep *ep, const void *buf, size_t len,
				 void *desc, fi_addr_t dest_addr, uint64_t tag,
				 void *context)
{
	return psmx3_tagged_send_specialized(ep, buf, len, desc, dest_addr, tag,
					     context, 1, 1, 0, 0);
}

/* op_flags=0, FI_SELECTIVE_COMPLETION set, av_map */
static ssize_t
psmx3_tagged_send_no_event_av_map(struct fid_ep *ep, const void *buf, size_t len,
				  void *desc, fi_addr_t dest_addr, uint64_t tag,
				  void *context)
{
	return psmx3_tagged_send_specialized(ep, buf, len, desc, dest_addr, tag,
					     context, 0, 1, 0, 0);
}

/* op_flags=0, FI_SELECTIVE_COMPLETION not set, av_table */
static ssize_t
psmx3_tagged_senddata_no_flag(struct fid_ep *ep, const void *buf, size_t len,
			      void *desc, uint64_t data, fi_addr_t dest_addr,
			      uint64_t tag, void *context)
{
	return psmx3_tagged_send_specialized(ep, buf, len, desc, dest_addr, tag,
					     context, 1, 0, 1, data);
}

/* op_flags=0, FI_SELECTIVE_COMPLETION set, av_table */
static ssize_t
psmx3_tagged_senddata_no_event(struct fid_ep *ep, const void *buf, size_t len,
			       void *desc, uint64_t data, fi_addr_t dest_addr,
			       uint64_t tag, void *context)
{
	return psmx3_tagged_send_specialized(ep, buf, len, desc, dest_addr, tag,
					     context, 0, 0, 1, data);
}

/* op_flags=0, FI_SELECTIVE_COMPLETION not set, av_map */
static ssize_t
psmx3_tagged_senddata_no_flag_av_map(struct fid_ep *ep, const void *buf, size_t len,
				     void *desc, uint64_t data, fi_addr_t dest_addr,
				     uint64_t tag, void *context)
{
	return psmx3_tagged_send_specialized(ep, buf, len, desc, dest_addr, tag,
					     context, 1, 1, 1, data);
}

/* op_flags=0, FI_SELECTIVE_COMPLETION set, av_map */
static ssize_t
psmx3_tagged_senddata_no_event_av_map(struct fid_ep *ep, const void *buf, size_t len,
				      void *desc, uint64_t data, fi_addr_t dest_addr,
				      uint64_t tag, void *context)
{
	return psmx3_tagged_send_specialized(ep, buf, len, desc, dest_addr, tag,
					     context, 0, 1, 1, data);
}

__attribute__((always_inline))
static inline ssize_t
psmx3_tagged_inject_specialized(struct fid_ep *ep, const void *buf,
				size_t len, fi_addr_t dest_addr,
				uint64_t tag, int av_map,
				int have_data, uint64_t data)
{
	struct psmx3_fid_ep *ep_priv;
	psm2_epaddr_t psm2_epaddr;
	psm2_mq_tag_t psm2_tag;
	int err;

	assert((tag & ~PSMX3_TAG_MASK) == 0);

	if (len > psmx3_env.inject_size)
		return -FI_EMSGSIZE;

	ep_priv = container_of(ep, struct psmx3_fid_ep, ep);

	if (av_map) {
		psm2_epaddr = (psm2_epaddr_t)dest_addr;
	} else {
		assert(ep_priv->av);
		psm2_epaddr = psmx3_av_translate_addr(ep_priv->av, ep_priv->tx, dest_addr, FI_AV_TABLE);
	}

	if (have_data)
		PSMX3_SET_TAG(psm2_tag, tag, data, PSMX3_TYPE_TAGGED | PSMX3_IMM_BIT);
	else
		PSMX3_SET_TAG(psm2_tag, tag, ep_priv->sep_id, PSMX3_TYPE_TAGGED);

	err = psm3_mq_send2(ep_priv->tx->psm2_mq, psm2_epaddr,
				PSM2_MQ_FLAG_INJECT, &psm2_tag, buf, len);

	if (err != PSM2_OK)
		return psmx3_errno(err);

	if (ep_priv->send_cntr)
		psmx3_cntr_inc(ep_priv->send_cntr, 0);

	return 0;
}

/* op_flags=0, av_table */
static ssize_t
psmx3_tagged_inject_no_flag(struct fid_ep *ep, const void *buf, size_t len,
			    fi_addr_t dest_addr, uint64_t tag)
{
	return psmx3_tagged_inject_specialized(ep, buf, len, dest_addr, tag,
					       0, 0, 0);
}

/* op_flags=0, av_map */
static ssize_t
psmx3_tagged_inject_no_flag_av_map(struct fid_ep *ep, const void *buf, size_t len,
				   fi_addr_t dest_addr, uint64_t tag)
{
	return psmx3_tagged_inject_specialized(ep, buf, len, dest_addr, tag,
					       1, 0, 0);
}

/* op_flags=0, av_table */
static ssize_t
psmx3_tagged_injectdata_no_flag(struct fid_ep *ep, const void *buf, size_t len,
				uint64_t data, fi_addr_t dest_addr, uint64_t tag)
{
	return psmx3_tagged_inject_specialized(ep, buf, len, dest_addr, tag,
					       0, 1, data);
}

/* op_flags=0, av_map */
static ssize_t
psmx3_tagged_injectdata_no_flag_av_map(struct fid_ep *ep, const void *buf, size_t len,
				       uint64_t data, fi_addr_t dest_addr, uint64_t tag)
{
	return psmx3_tagged_inject_specialized(ep, buf, len, dest_addr, tag,
					       1, 1, data);
}

ssize_t psmx3_tagged_sendv_generic(struct fid_ep *ep,
				   const struct iovec *iov, void **desc,
				   size_t count, fi_addr_t dest_addr,
				   uint64_t tag, void *context,
				   uint64_t flags, uint64_t data)
{
	struct psmx3_fid_ep *ep_priv;
	struct psmx3_fid_av *av;
	psm2_epaddr_t psm2_epaddr;
	psm2_mq_req_t psm2_req;
	psm2_mq_tag_t psm2_tag;
	struct fi_context * fi_context;
	int send_flag = 0;
	int err;
	int no_completion = 0;
	struct psmx3_cq_event *event;
	size_t real_count;
	size_t len, total_len;
	char *p;
	uint32_t *q;
	int i, j=0;
	struct psmx3_sendv_request *req;
	int have_data = (flags & FI_REMOTE_CQ_DATA) > 0;
	uint32_t msg_flags;

	assert((tag & ~PSMX3_TAG_MASK) == 0);

	ep_priv = container_of(ep, struct psmx3_fid_ep, ep);

	if (flags & FI_TRIGGER)
		return psmx3_trigger_queue_tsendv(ep, iov, desc, count,
						  dest_addr, tag, context,
						  flags, data);

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
		return psmx3_tagged_send_generic(ep, iov[j].iov_base, iov[j].iov_len,
						 desc ? desc[j] : NULL, dest_addr,
						 tag, context, flags, data);

	req = malloc(sizeof(*req));
	if (!req)
		return -FI_ENOMEM;

	if (total_len <= PSMX3_IOV_BUF_SIZE) {
		req->iov_protocol = PSMX3_IOV_PROTO_PACK;
		p = req->buf;
		for (i=0; i<count; i++) {
			if (iov[i].iov_len) {
				memcpy(p, iov[i].iov_base, iov[i].iov_len);
				p += iov[i].iov_len;
			}
		}

		msg_flags = PSMX3_TYPE_TAGGED;
		len = total_len;
	} else {
		req->iov_protocol = PSMX3_IOV_PROTO_MULTI;
		req->iov_done = 0;
		req->iov_info.seq_num = (++ep_priv->iov_seq_num) %
					PSMX3_IOV_MAX_SEQ_NUM + 1;
		req->iov_info.count = (uint32_t)real_count;
		req->iov_info.total_len = (uint32_t)total_len;

		q = req->iov_info.len;
		for (i=0; i<count; i++) {
			if (iov[i].iov_len)
				*q++ = (uint32_t)iov[i].iov_len;
		}

		msg_flags = PSMX3_TYPE_TAGGED | PSMX3_IOV_BIT;
		len = (3 + real_count) * sizeof(uint32_t);
	}

	av = ep_priv->av;
	assert(av);
	psm2_epaddr = psmx3_av_translate_addr(av, ep_priv->tx, dest_addr, av->type);

	if (have_data)
		PSMX3_SET_TAG(psm2_tag, tag, (uint32_t)data, msg_flags | PSMX3_IMM_BIT);
	else
		PSMX3_SET_TAG(psm2_tag, tag, (uint32_t)ep_priv->sep_id, msg_flags);

	if ((flags & PSMX3_NO_COMPLETION) ||
	    (ep_priv->send_selective_completion && !(flags & FI_COMPLETION)))
		no_completion = 1;

	if (flags & FI_INJECT) {
		if (len > psmx3_env.inject_size) {
			free(req);
			return -FI_EMSGSIZE;
		}

		err = psm3_mq_send2(ep_priv->tx->psm2_mq, psm2_epaddr,
				    send_flag|PSM2_MQ_FLAG_INJECT, &psm2_tag, req->buf, len);

		free(req);

		if (err != PSM2_OK)
			return psmx3_errno(err);

		if (ep_priv->send_cntr)
			psmx3_cntr_inc(ep_priv->send_cntr, 0);

		if (ep_priv->send_cq && !no_completion) {
			event = psmx3_cq_create_event(
					ep_priv->send_cq,
					context, NULL, flags, len,
					(uint64_t) data,
					0 /* tag */,
					0 /* olen */,
					0 /* err */);

			if (event)
				psmx3_cq_enqueue_event(ep_priv->send_cq, event);
			else
				return -FI_ENOMEM;
		}

		return 0;
	}

	req->no_completion = no_completion;
	req->user_context = context;
	req->comp_flag = FI_TAGGED;

	fi_context = &req->fi_context;
	PSMX3_CTXT_TYPE(fi_context) = PSMX3_SENDV_CONTEXT;
	PSMX3_CTXT_USER(fi_context) = req;
	PSMX3_CTXT_EP(fi_context) = ep_priv;

	err = psm3_mq_isend2(ep_priv->tx->psm2_mq, psm2_epaddr,
			     send_flag, &psm2_tag, req->buf, len,
			     (void *)fi_context, &psm2_req);

	if (err != PSM2_OK) {
		free(req);
		return psmx3_errno(err);
	}

	PSMX3_CTXT_REQ(fi_context) = psm2_req;

	if (req->iov_protocol == PSMX3_IOV_PROTO_MULTI) {
		fi_context = &req->fi_context_iov;
		PSMX3_CTXT_TYPE(fi_context) = PSMX3_IOV_SEND_CONTEXT;
		PSMX3_CTXT_USER(fi_context) = req;
		PSMX3_CTXT_EP(fi_context) = ep_priv;
		PSMX3_SET_TAG(psm2_tag, req->iov_info.seq_num, 0,
			      PSMX3_TYPE_IOV_PAYLOAD);
		for (i=0; i<count; i++) {
			if (iov[i].iov_len) {
				err = psm3_mq_isend2(ep_priv->tx->psm2_mq,
						     psm2_epaddr, send_flag, &psm2_tag,
						     iov[i].iov_base, iov[i].iov_len,
						     (void *)fi_context, &psm2_req);
				if (err != PSM2_OK)
					return psmx3_errno(err);
			}
		}
	}

	return 0;
}

static ssize_t psmx3_tagged_send(struct fid_ep *ep,
				 const void *buf, size_t len,
				 void *desc, fi_addr_t dest_addr,
				 uint64_t tag, void *context)
{
	struct psmx3_fid_ep *ep_priv;

	ep_priv = container_of(ep, struct psmx3_fid_ep, ep);

	return psmx3_tagged_send_generic(ep, buf, len, desc, dest_addr,
					 tag, context, ep_priv->tx_flags, 0);
}

static ssize_t psmx3_tagged_sendmsg(struct fid_ep *ep,
				    const struct fi_msg_tagged *msg,
				    uint64_t flags)
{
	void *buf;
	size_t len;

	assert(msg);
	assert(!msg->iov_count || msg->msg_iov);
	assert(msg->iov_count <= PSMX3_IOV_MAX_COUNT);

	if (msg->iov_count > 1) {
		return psmx3_tagged_sendv_generic(ep, msg->msg_iov,
						  msg->desc, msg->iov_count,
						  msg->addr, msg->tag,
						  msg->context, flags,
						  msg->data);
	} else if (msg->iov_count) {
		buf = msg->msg_iov[0].iov_base;
		len = msg->msg_iov[0].iov_len;
	} else {
		buf = NULL;
		len = 0;
	}

	return psmx3_tagged_send_generic(ep, buf, len,
					 msg->desc ? msg->desc[0] : NULL,
					 msg->addr, msg->tag, msg->context,
					 flags, msg->data);
}

ssize_t psmx3_tagged_senddata(struct fid_ep *ep, const void *buf, size_t len,
			      void *desc, uint64_t data, fi_addr_t dest_addr,
			      uint64_t tag, void *context)
{
	struct psmx3_fid_ep *ep_priv;

	ep_priv = container_of(ep, struct psmx3_fid_ep, ep);

	return psmx3_tagged_send_generic(ep, buf, len, desc, dest_addr,
					 tag, context,
					 ep_priv->tx_flags | FI_REMOTE_CQ_DATA,
					 data);
}

#define PSMX3_TAGGED_SENDV_FUNC(suffix)					\
static ssize_t								\
psmx3_tagged_sendv##suffix(struct fid_ep *ep, const struct iovec *iov,	\
			   void **desc,size_t count,			\
			   fi_addr_t dest_addr,	uint64_t tag,		\
			   void *context)				\
{									\
	void *buf;							\
	size_t len;							\
	assert(!count || iov);						\
	assert(count <= PSMX3_IOV_MAX_COUNT); 				\
	if (count > 1) {						\
		struct psmx3_fid_ep *ep_priv;				\
		ep_priv = container_of(ep, struct psmx3_fid_ep, ep);	\
		return psmx3_tagged_sendv_generic(ep, iov, desc, count, \
						  dest_addr, tag,	\
						  context,		\
						  ep_priv->tx_flags, 0);\
	} else if (count) {						\
		buf = iov[0].iov_base;					\
		len = iov[0].iov_len;					\
	} else {							\
		buf = NULL;						\
		len = 0;						\
	}								\
	return psmx3_tagged_send##suffix(ep, buf, len,			\
					 desc ? desc[0] : NULL,		\
				 	 dest_addr, tag, context);	\
}

PSMX3_TAGGED_SENDV_FUNC()
PSMX3_TAGGED_SENDV_FUNC(_no_flag)
PSMX3_TAGGED_SENDV_FUNC(_no_event)
PSMX3_TAGGED_SENDV_FUNC(_no_flag_av_map)
PSMX3_TAGGED_SENDV_FUNC(_no_event_av_map)

static ssize_t psmx3_tagged_inject(struct fid_ep *ep,
				   const void *buf, size_t len,
				   fi_addr_t dest_addr, uint64_t tag)
{
	struct psmx3_fid_ep *ep_priv;

	ep_priv = container_of(ep, struct psmx3_fid_ep, ep);

	return psmx3_tagged_send_generic(ep, buf, len, NULL, dest_addr,
					 tag, NULL,
				  	 ep_priv->tx_flags | FI_INJECT | PSMX3_NO_COMPLETION,
					 0);
}

static ssize_t psmx3_tagged_injectdata(struct fid_ep *ep,
				       const void *buf, size_t len, uint64_t data,
				       fi_addr_t dest_addr, uint64_t tag)
{
	struct psmx3_fid_ep *ep_priv;

	ep_priv = container_of(ep, struct psmx3_fid_ep, ep);

	return psmx3_tagged_send_generic(ep, buf, len, NULL, dest_addr,
					 tag, NULL,
					 ep_priv->tx_flags | FI_INJECT | FI_REMOTE_CQ_DATA |
						  PSMX3_NO_COMPLETION,
					 data);
}

#define PSMX3_TAGGED_OPS(suffix,sendopt,recvopt,injopt)	\
struct fi_ops_tagged psmx3_tagged_ops##suffix = {	\
	.size = sizeof(struct fi_ops_tagged),		\
	.recv = psmx3_tagged_recv##recvopt,		\
	.recvv = psmx3_tagged_recvv##recvopt,		\
	.recvmsg = psmx3_tagged_recvmsg,		\
	.send = psmx3_tagged_send##sendopt,		\
	.sendv = psmx3_tagged_sendv##sendopt,		\
	.sendmsg = psmx3_tagged_sendmsg,		\
	.inject = psmx3_tagged_inject##injopt,		\
	.senddata = psmx3_tagged_senddata##sendopt,	\
	.injectdata = psmx3_tagged_injectdata##injopt,	\
};

PSMX3_TAGGED_OPS(,,,)
PSMX3_TAGGED_OPS(_no_flag_directed, _no_flag, _no_flag_directed, _no_flag)
PSMX3_TAGGED_OPS(_no_event_directed, _no_event, _no_event_directed, _no_flag)
PSMX3_TAGGED_OPS(_no_send_event_directed, _no_event, _no_flag_directed, _no_flag)
PSMX3_TAGGED_OPS(_no_recv_event_directed, _no_flag, _no_event_directed, _no_flag)
PSMX3_TAGGED_OPS(_no_flag_undirected, _no_flag, _no_flag_undirected, _no_flag)
PSMX3_TAGGED_OPS(_no_event_undirected, _no_event, _no_event_undirected, _no_flag)
PSMX3_TAGGED_OPS(_no_send_event_undirected, _no_event, _no_flag_undirected, _no_flag)
PSMX3_TAGGED_OPS(_no_recv_event_undirected, _no_flag, _no_event_undirected, _no_flag)
PSMX3_TAGGED_OPS(_no_flag_directed_av_map, _no_flag_av_map, _no_flag_directed_av_map, _no_flag_av_map)
PSMX3_TAGGED_OPS(_no_event_directed_av_map, _no_event_av_map, _no_event_directed_av_map, _no_flag_av_map)
PSMX3_TAGGED_OPS(_no_send_event_directed_av_map, _no_event_av_map, _no_flag_directed_av_map, _no_flag_av_map)
PSMX3_TAGGED_OPS(_no_recv_event_directed_av_map, _no_flag_av_map, _no_event_directed_av_map, _no_flag_av_map)
PSMX3_TAGGED_OPS(_no_flag_undirected_av_map, _no_flag_av_map, _no_flag_undirected_av_map, _no_flag_av_map)
PSMX3_TAGGED_OPS(_no_event_undirected_av_map, _no_event_av_map, _no_event_undirected_av_map, _no_flag_av_map)
PSMX3_TAGGED_OPS(_no_send_event_undirected_av_map, _no_event_av_map, _no_flag_undirected_av_map, _no_flag_av_map)
PSMX3_TAGGED_OPS(_no_recv_event_undirected_av_map, _no_flag_av_map, _no_event_undirected_av_map, _no_flag_av_map)

