/*
 * Copyright (c) 2019 Intel Corporation. All rights reserved.
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

#include <inttypes.h>

#include "ofi.h"
#include "ofi_mem.h"
#include "ofi_prov.h"
#include "ofi_hook.h"
#include "hook_prov.h"
#include "ofi_enosys.h"

#include "hook_debug.h"

struct hook_prov_ctx hook_debug_prov_ctx;

struct hook_debug_config config = {
	.trace_exit = 1,
	.trace_cq_entry = 1,
	.track_sends = 1,
	/* Disable for now: debug hang */
	.track_recvs = 0,
};

static struct hook_debug_txrx_entry *
hook_debug_get_tx_entry(struct hook_debug_ep *myep, void *context,
			uint64_t flags)
{
	struct hook_debug_txrx_entry *tx_entry;

	tx_entry = ofi_buf_alloc(myep->tx_pool);
	assert(tx_entry);
	assert(tx_entry->magic == OFI_MAGIC_64);

	tx_entry->op_flags = myep->tx_op_flags | flags;
	tx_entry->context = context;
	return tx_entry;
}

static struct hook_debug_txrx_entry *
hook_debug_get_rx_entry(struct hook_debug_ep *myep, void *context,
			uint64_t flags)
{
	struct hook_debug_txrx_entry *rx_entry;

	rx_entry = ofi_buf_alloc(myep->rx_pool);
	assert(rx_entry);
	assert(rx_entry->magic == OFI_MAGIC_64);

	rx_entry->op_flags = myep->rx_op_flags | flags;
	rx_entry->context = context;
	return rx_entry;
}

static void hook_debug_trace_exit(struct fid *fid, struct fid *hfid,
				  enum fi_log_subsys subsys, const char *fn,
				  ssize_t ret, size_t *eagain_count)
{
	if (!config.trace_exit)
		return;

	if (ret > 0) {
		FI_TRACE(hook_to_hprov(fid), subsys, "%s (fid: %p) returned: "
			 "%zd\n", fn, (void *) hfid, ret);
		goto out;
	}

	if (ret != -FI_EAGAIN || !eagain_count ||
	    !((*eagain_count)++ % HOOK_DEBUG_EAGAIN_LOG))
		FI_TRACE(hook_to_hprov(fid), subsys, "%s (fid: %p) returned: "
			 "%zd (%s)\n", fn, (void *) hfid, ret, fi_strerror(-ret));
out:
	if (eagain_count && ret != -FI_EAGAIN)
		*eagain_count = 0;
}

static void
hook_debug_trace_exit_eq(struct hook_debug_eq *eq, const char *fn, ssize_t ret)
{
	hook_debug_trace_exit(&eq->hook_eq.eq.fid, &eq->hook_eq.heq->fid,
			      FI_LOG_EQ, fn, ret, &eq->eagain_count);
}

static void
hook_debug_trace_exit_cq(struct hook_debug_cq *cq, const char *fn, ssize_t ret)
{
	hook_debug_trace_exit(&cq->hook_cq.cq.fid, &cq->hook_cq.hcq->fid,
			      FI_LOG_CQ, fn, ret, &cq->eagain_count);
}

static void
hook_debug_trace_exit_cntr(struct hook_cntr *cntr, const char *fn, ssize_t ret)
{
	hook_debug_trace_exit(&cntr->cntr.fid, &cntr->hcntr->fid,
			      FI_LOG_CNTR, fn, ret, NULL);
}

static void
hook_debug_trace_exit_ep(struct hook_debug_ep *ep, const char *fn, ssize_t ret,
			 size_t *eagain_count)
{
	hook_debug_trace_exit(&ep->hook_ep.ep.fid, &ep->hook_ep.hep->fid,
			      FI_LOG_EP_DATA, fn, ret, eagain_count);
}

static void hook_debug_rx_end(struct hook_debug_ep *ep, char *fn,
			      ssize_t ret, void *mycontext)
{
	struct hook_debug_txrx_entry *rx_entry;

	hook_debug_trace_exit_ep(ep, fn, ret, &ep->rx_eagain_count);

	if (config.track_recvs) {
		if (!ret) {
			ep->rx_outs++;
			FI_TRACE(hook_to_hprov(&ep->hook_ep.ep.fid),
				 FI_LOG_EP_DATA, "ep: %p rx_outs: %zu\n",
				 (void *) ep->hook_ep.hep, ep->rx_outs);
		} else {
			rx_entry = mycontext;
			ofi_buf_free(rx_entry);
		}
	}
}

static int hook_debug_rx_start(struct hook_debug_ep *ep, void *context,
			       uint64_t flags, void **mycontext)
{
	struct hook_debug_txrx_entry *rx_entry;

	if (config.track_recvs) {
		if (flags & ~(FI_MULTI_RECV | FI_COMPLETION)) {
			FI_TRACE(&hook_debug_prov_ctx.prov, FI_LOG_EP_DATA,
				 "unsupported flags: %s\n",
				 fi_tostr(&flags, FI_TYPE_OP_FLAGS));
			return -FI_EINVAL;
		}

		rx_entry = hook_debug_get_rx_entry(ep, context, flags);
		*mycontext = rx_entry;
	} else {
		*mycontext = context;
	}
	return 0;
}

static ssize_t
hook_debug_recv(struct fid_ep *ep, void *buf, size_t len, void *desc,
		fi_addr_t src_addr, void *context)
{
	struct hook_debug_ep *myep = container_of(ep, struct hook_debug_ep,
						  hook_ep.ep);
	void *mycontext;
	ssize_t ret;

	ret = hook_debug_rx_start(myep, context, 0, &mycontext);
	if (ret)
		return ret;

	ret = fi_recv(myep->hook_ep.hep, buf, len, desc, src_addr, mycontext);
	hook_debug_rx_end(myep, "fi_recv", ret, mycontext);
	return ret;
}

static ssize_t
hook_debug_recvmsg(struct fid_ep *ep, const struct fi_msg *msg, uint64_t flags)
{
	struct hook_debug_ep *myep = container_of(ep, struct hook_debug_ep, hook_ep.ep);
	struct fi_msg mymsg = *msg;
	ssize_t ret;

	ret = hook_debug_rx_start(myep, msg->context, flags, &mymsg.context);
	if (ret)
		return ret;

	ret = fi_recvmsg(myep->hook_ep.hep, &mymsg, flags);
	hook_debug_rx_end(myep, "fi_recvmsg", ret, mymsg.context);
	return ret;
}

static ssize_t
hook_debug_trecv(struct fid_ep *ep, void *buf, size_t len, void *desc,
		 fi_addr_t src_addr, uint64_t tag, uint64_t ignore,
		 void *context)
{
	struct hook_debug_ep *myep = container_of(ep, struct hook_debug_ep, hook_ep.ep);
	void *mycontext;
	ssize_t ret;

	ret = hook_debug_rx_start(myep, context, 0, &mycontext);
	if (ret)
		return ret;

	ret = fi_trecv(myep->hook_ep.hep, buf, len, desc, src_addr,
		       tag, ignore, mycontext);

	hook_debug_rx_end(myep, "fi_trecv", ret, mycontext);
	return ret;
}

static void hook_debug_tx_end(struct hook_debug_ep *ep, char *fn,
			      ssize_t ret, void *mycontext)
{
	struct hook_debug_txrx_entry *tx_entry;

	hook_debug_trace_exit_ep(ep, fn, ret, &ep->tx_eagain_count);

	if (mycontext && config.track_sends) {
		if (!ret) {
			ep->tx_outs++;
			FI_TRACE(hook_to_hprov(&ep->hook_ep.ep.fid),
				 FI_LOG_EP_DATA, "ep: %p tx_outs: %zu\n",
				 (void *) ep->hook_ep.hep, ep->tx_outs);
		} else {
			tx_entry = mycontext;
			ofi_buf_free(tx_entry);
		}
	}
}

static int hook_debug_tx_start(struct hook_debug_ep *ep, void *context,
			       uint64_t flags, void **mycontext)
{
	struct hook_debug_txrx_entry *tx_entry;

	if (mycontext) {
		if (config.track_sends) {
			tx_entry = hook_debug_get_tx_entry(ep, context, flags);
			*mycontext = tx_entry;
		} else {
			*mycontext = context;
		}
	}
	return 0;
}

static ssize_t
hook_debug_send(struct fid_ep *ep, const void *buf, size_t len, void *desc,
	      fi_addr_t dest_addr, void *context)
{
	struct hook_debug_ep *myep = container_of(ep, struct hook_debug_ep, hook_ep.ep);
	void *mycontext;
	ssize_t ret;

	ret = hook_debug_tx_start(myep, context, 0, &mycontext);
	if (ret)
		return ret;

	ret = fi_send(myep->hook_ep.hep, buf, len, desc, dest_addr, mycontext);

	hook_debug_tx_end(myep, "fi_send", ret, mycontext);
	return ret;
}

static ssize_t
hook_debug_sendv(struct fid_ep *ep, const struct iovec *iov, void **desc,
		 size_t count, fi_addr_t dest_addr, void *context)
{
	struct hook_debug_ep *myep = container_of(ep, struct hook_debug_ep, hook_ep.ep);
	void *mycontext;
	ssize_t ret;

	ret = hook_debug_tx_start(myep, context, 0, &mycontext);
	if (ret)
		return ret;

	ret = fi_sendv(myep->hook_ep.hep, iov, desc, count, dest_addr, mycontext);

	hook_debug_tx_end(myep, "fi_sendv", ret, mycontext);
	return ret;
}

static ssize_t
hook_debug_sendmsg(struct fid_ep *ep, const struct fi_msg *msg, uint64_t flags)
{
	struct hook_debug_ep *myep = container_of(ep, struct hook_debug_ep, hook_ep.ep);
	struct fi_msg mymsg = *msg;
	ssize_t ret;

	ret = hook_debug_tx_start(myep, msg->context, flags, &mymsg.context);
	if (ret)
		return ret;

	ret = fi_sendmsg(myep->hook_ep.hep, &mymsg, flags);
	hook_debug_tx_end(myep, "fi_sendmsg", ret, mymsg.context);
	return ret;
}

static ssize_t
hook_debug_inject(struct fid_ep *ep, const void *buf, size_t len,
		fi_addr_t dest_addr)
{
	struct hook_debug_ep *myep = container_of(ep, struct hook_debug_ep, hook_ep.ep);
	ssize_t ret;

	ret = hook_debug_tx_start(myep, NULL, 0, NULL);
	if (ret)
		return ret;

	ret = fi_inject(myep->hook_ep.hep, buf, len, dest_addr);

	hook_debug_tx_end(myep, "fi_inject", ret, NULL);
	return ret;
}

static ssize_t
hook_debug_senddata(struct fid_ep *ep, const void *buf, size_t len, void *desc,
		  uint64_t data, fi_addr_t dest_addr, void *context)
{
	struct hook_debug_ep *myep = container_of(ep, struct hook_debug_ep, hook_ep.ep);
	void *mycontext;
	ssize_t ret;

	ret = hook_debug_tx_start(myep, context, 0, &mycontext);
	if (ret)
		return ret;

	ret = fi_senddata(myep->hook_ep.hep, buf, len, desc, data, dest_addr, mycontext);
	hook_debug_tx_end(myep, "fi_senddata", ret, mycontext);
	return ret;
}

static ssize_t
hook_debug_injectdata(struct fid_ep *ep, const void *buf, size_t len,
		      uint64_t data, fi_addr_t dest_addr)
{
	struct hook_debug_ep *myep = container_of(ep, struct hook_debug_ep, hook_ep.ep);
	ssize_t ret;

	ret = hook_debug_tx_start(myep, NULL, 0, NULL);
	if (ret)
		return ret;

	ret = fi_injectdata(myep->hook_ep.hep, buf, len, data, dest_addr);

	hook_debug_tx_end(myep, "fi_injectdata", ret, NULL);
	return ret;
}

static ssize_t
hook_debug_tsend(struct fid_ep *ep, const void *buf, size_t len, void *desc,
		 fi_addr_t dest_addr, uint64_t tag, void *context)
{
	struct hook_debug_ep *myep = container_of(ep, struct hook_debug_ep, hook_ep.ep);
	void *mycontext;
	ssize_t ret;

	ret = hook_debug_tx_start(myep, context, 0, &mycontext);
	if (ret)
		return ret;

	ret = fi_tsend(myep->hook_ep.hep, buf, len, desc, dest_addr, tag, mycontext);
	hook_debug_tx_end(myep, "fi_tsend", ret, mycontext);
	return ret;
}

static ssize_t
hook_debug_tsendv(struct fid_ep *ep, const struct iovec *iov, void **desc,
		  size_t count, fi_addr_t dest_addr, uint64_t tag, void *context)
{
	struct hook_debug_ep *myep = container_of(ep, struct hook_debug_ep, hook_ep.ep);
	void *mycontext;
	ssize_t ret;

	ret = hook_debug_tx_start(myep, context, 0, &mycontext);
	if (ret)
		return ret;

	ret = fi_tsendv(myep->hook_ep.hep, iov, desc, count,
			dest_addr, tag, mycontext);

	hook_debug_tx_end(myep, "fi_tsendv", ret, mycontext);
	return ret;
}

static ssize_t
hook_debug_tsendmsg(struct fid_ep *ep, const struct fi_msg_tagged *msg,
		    uint64_t flags)
{
	struct hook_debug_ep *myep = container_of(ep, struct hook_debug_ep, hook_ep.ep);
	struct fi_msg_tagged mymsg = *msg;
	ssize_t ret;

	ret = hook_debug_tx_start(myep, msg->context, flags, &mymsg.context);
	if (ret)
		return ret;

	ret = fi_tsendmsg(myep->hook_ep.hep, &mymsg, flags);
	hook_debug_tx_end(myep, "fi_tsendmsg", ret, mymsg.context);
	return ret;
}

static ssize_t
hook_debug_tinject(struct fid_ep *ep, const void *buf, size_t len,
		   fi_addr_t dest_addr, uint64_t tag)
{
	struct hook_debug_ep *myep = container_of(ep, struct hook_debug_ep, hook_ep.ep);
	ssize_t ret;

	ret = hook_debug_tx_start(myep, NULL, 0, NULL);
	if (ret)
		return ret;

	ret = fi_tinject(myep->hook_ep.hep, buf, len, dest_addr, tag);

	hook_debug_tx_end(myep, "fi_tinject", ret, NULL);
	return ret;
}

static ssize_t
hook_debug_tsenddata(struct fid_ep *ep, const void *buf, size_t len, void *desc,
		     uint64_t data, fi_addr_t dest_addr, uint64_t tag,
		     void *context)
{
	struct hook_debug_ep *myep = container_of(ep, struct hook_debug_ep, hook_ep.ep);
	void *mycontext;
	ssize_t ret;

	ret = hook_debug_tx_start(myep, context, 0, &mycontext);
	if (ret)
		return ret;

	ret = fi_tsenddata(myep->hook_ep.hep, buf, len, desc, data,
			    dest_addr, tag, mycontext);
	hook_debug_tx_end(myep, "fi_tsenddata", ret, mycontext);
	return ret;
}

static ssize_t
hook_debug_tinjectdata(struct fid_ep *ep, const void *buf, size_t len,
		       uint64_t data, fi_addr_t dest_addr, uint64_t tag)
{
	struct hook_debug_ep *myep = container_of(ep, struct hook_debug_ep, hook_ep.ep);
	ssize_t ret;

	ret = hook_debug_tx_start(myep, NULL, 0, NULL);
	if (ret)
		return ret;

	ret = fi_tinjectdata(myep->hook_ep.hep, buf, len, data, dest_addr, tag);

	hook_debug_tx_end(myep, "fi_tinjectdata", ret, NULL);
	return ret;
}

#define HOOK_DEBUG_TRACE(fabric, subsys, ...) \
	FI_TRACE(hook_fabric_to_hprov(fabric), subsys, __VA_ARGS__)

#define HOOK_DEBUG_CQ_TRACE(cq, ...) \
	HOOK_DEBUG_TRACE(cq->hook_cq.domain->fabric, FI_LOG_CQ, __VA_ARGS__)

static void hook_debug_cq_entry_log(struct hook_debug_cq *cq,
				    struct fi_cq_tagged_entry *entry)
{
	if (!config.trace_cq_entry)
		return;

	HOOK_DEBUG_CQ_TRACE(cq, "cq_entry:\n");
	HOOK_DEBUG_CQ_TRACE(cq, "\top_context: %p\n", entry->op_context);

	if (cq->format > FI_CQ_FORMAT_CONTEXT) {
		HOOK_DEBUG_CQ_TRACE(cq, "\tflags: %s\n",
				    fi_tostr(&entry->flags, FI_TYPE_CAPS));

		if (entry->flags & FI_RECV)
			HOOK_DEBUG_CQ_TRACE(cq, "\tlen: %zu\n", entry->len);

		if (cq->format == FI_CQ_FORMAT_TAGGED)
			HOOK_DEBUG_CQ_TRACE(cq, "\ttag: %" PRIx64 "\n", entry->tag);
	}
}

static void hook_debug_cq_process_entry(struct hook_debug_cq *mycq,
					const char *fn, ssize_t ret, char *buf)
{
	struct hook_debug_txrx_entry *rx_entry, *tx_entry;
	struct fi_cq_tagged_entry *cq_entry;
	int i;

	hook_debug_trace_exit_cq(mycq, fn, ret);

	for (i = 0; i < ret; i++, buf += mycq->entry_size) {
		cq_entry = (struct fi_cq_tagged_entry *)buf;
		hook_debug_cq_entry_log(mycq, cq_entry);

		if (config.track_recvs && (cq_entry->flags & FI_RECV)) {
			rx_entry = cq_entry->op_context;
			assert(rx_entry->magic == OFI_MAGIC_64);

			cq_entry->op_context = rx_entry->context;

			if (!(rx_entry->op_flags & FI_MULTI_RECV) ||
			    cq_entry->flags & FI_MULTI_RECV) {
				rx_entry->ep->rx_outs--;
				FI_TRACE(hook_to_hprov(&mycq->hook_cq.cq.fid),
					 FI_LOG_CQ, "ep: %p rx_outs: %zu\n",
					 (void *) rx_entry->ep->hook_ep.hep,
					 rx_entry->ep->rx_outs);
				ofi_buf_free(rx_entry);
			}
		} else if (config.track_sends && (cq_entry->flags & FI_SEND)) {
			tx_entry = cq_entry->op_context;
			assert(tx_entry->magic == OFI_MAGIC_64);

			cq_entry->op_context = tx_entry->context;

			tx_entry->ep->tx_outs--;
			FI_TRACE(hook_to_hprov(&mycq->hook_cq.cq.fid),
				 FI_LOG_CQ, "ep: %p tx_outs: %zu\n",
				 (void *) tx_entry->ep->hook_ep.hep,
				 tx_entry->ep->tx_outs);
			ofi_buf_free(tx_entry);
		}
	}
}

static ssize_t hook_debug_cq_read(struct fid_cq *cq, void *buf, size_t count)
{
	struct hook_debug_cq *mycq = container_of(cq, struct hook_debug_cq,
						  hook_cq.cq);
	ssize_t ret;

	ret = fi_cq_read(mycq->hook_cq.hcq, buf, count);
	hook_debug_cq_process_entry(mycq, "fi_cq_read", ret, buf);
	return ret;
}

static ssize_t hook_debug_cq_readfrom(struct fid_cq *cq, void *buf, size_t count,
				      fi_addr_t *src_addr)
{
	struct hook_debug_cq *mycq = container_of(cq, struct hook_debug_cq,
						  hook_cq.cq);
	ssize_t ret;

	ret = fi_cq_readfrom(mycq->hook_cq.hcq, buf, count, src_addr);
	hook_debug_cq_process_entry(mycq, "fi_cq_readfrom", ret, buf);
	return ret;
}

static int hook_debug_cq_close(struct fid *fid)
{
	struct hook_debug_cq *mycq =
		container_of(fid, struct hook_debug_cq, hook_cq.cq.fid);
	int ret = 0;

	if (mycq->hook_cq.hcq)
		ret = fi_close(&mycq->hook_cq.hcq->fid);
	if (!ret)
		free(mycq);
	return ret;
}

// TODO move to common code
static size_t cq_entry_size[] = {
	[FI_CQ_FORMAT_UNSPEC] = 0,
	[FI_CQ_FORMAT_CONTEXT] = sizeof(struct fi_cq_entry),
	[FI_CQ_FORMAT_MSG] = sizeof(struct fi_cq_msg_entry),
	[FI_CQ_FORMAT_DATA] = sizeof(struct fi_cq_data_entry),
	[FI_CQ_FORMAT_TAGGED] = sizeof(struct fi_cq_tagged_entry)
};

struct fi_ops hook_debug_cq_fid_ops;
struct fi_ops_cq hook_debug_cq_ops;

static void hook_debug_cq_attr_log(struct hook_domain *dom,
				   struct fi_cq_attr *attr)
{
	HOOK_DEBUG_TRACE(dom->fabric, FI_LOG_CQ, "fi_cq_attr:\n");
	HOOK_DEBUG_TRACE(dom->fabric, FI_LOG_CQ, "\tsize: %zu\n", attr->size);
	HOOK_DEBUG_TRACE(dom->fabric, FI_LOG_CQ, "\tflags: %s\n", "TBD");
	HOOK_DEBUG_TRACE(dom->fabric, FI_LOG_CQ, "\tformat: %s\n", "TBD");
	HOOK_DEBUG_TRACE(dom->fabric, FI_LOG_CQ, "\twait_obj: %s\n", "TBD");
	HOOK_DEBUG_TRACE(dom->fabric, FI_LOG_CQ, "\tsignaling_vector: %d\n",
			 attr->signaling_vector);
	HOOK_DEBUG_TRACE(dom->fabric, FI_LOG_CQ, "\twait_cond: %s\n", "TBD");
	HOOK_DEBUG_TRACE(dom->fabric, FI_LOG_CQ, "\twait_set: %p\n",
			 (void *) attr->wait_set);
}

static int
hook_debug_cq_open(struct fid_domain *domain_fid, struct fi_cq_attr *attr,
		   struct fid_cq **cq, void *context)
{
	struct hook_domain *domain = container_of(domain_fid, struct hook_domain,
						  domain);
	struct hook_debug_cq *mycq;
	int ret;

	assert(!attr->flags);

	hook_debug_cq_attr_log(domain, attr);

	if ((config.track_sends || config.track_recvs) &&
	    (attr->format < FI_CQ_FORMAT_MSG)) {
		FI_WARN(&hook_debug_prov_ctx.prov, FI_LOG_CQ,
			"need FI_CQ_FORMAT_MSG or higher for tracking sends "
			"and(or) recvs\n");
		return -FI_EINVAL;
	}

	mycq = calloc(1, sizeof *mycq);
	if (!mycq)
		return -FI_EAGAIN;

	ret = hook_cq_init(domain_fid, attr, cq, context, &mycq->hook_cq);
	if (ret)
		goto err;

	FI_TRACE(hook_fabric_to_hprov(mycq->hook_cq.domain->fabric), FI_LOG_CQ,
		 "cq opened, fid: %p\n", (void *) &mycq->hook_cq.hcq->fid);

	mycq->hook_cq.cq.fid.ops = &hook_debug_cq_fid_ops;
	mycq->hook_cq.cq.ops = &hook_debug_cq_ops;
	mycq->format = attr->format;
	mycq->entry_size = cq_entry_size[attr->format];

	assert(mycq->entry_size);

	return 0;
err:
	hook_debug_cq_close(&mycq->hook_cq.cq.fid);
	return ret;
}

static int hook_debug_ep_close(struct fid *fid)
{
	struct hook_debug_ep *myep =
		container_of(fid, struct hook_debug_ep, hook_ep.ep.fid);
	int ret = 0;

	if (myep->tx_pool)
		ofi_bufpool_destroy(myep->tx_pool);

	if (myep->rx_pool)
		ofi_bufpool_destroy(myep->rx_pool);

	if (myep->hook_ep.hep)
		ret = fi_close(&myep->hook_ep.hep->fid);
	if (!ret)
		free(myep);
	return ret;
}

static int hook_debug_ep_bind(struct fid *fid, struct fid *bfid, uint64_t flags)
{
	struct fid *hfid, *hbfid;
	struct hook_cntr *cntr;
	struct hook_cq *cq;

	hfid = hook_to_hfid(fid);
	hbfid = hook_to_hfid(bfid);
	if (!hfid || !hbfid)
		return -FI_EINVAL;

	switch (bfid->fclass) {
	case FI_CLASS_CQ:
		cq = container_of(bfid, struct hook_cq, cq.fid);
		HOOK_DEBUG_TRACE(cq->domain->fabric, FI_LOG_EP_CTRL,
				 "cq: %p bind flags: %s\n", (void *) cq->hcq,
				 fi_tostr(&flags, FI_TYPE_CAPS));
		break;
	case FI_CLASS_CNTR:
		cntr = container_of(bfid, struct hook_cntr, cntr.fid);
		HOOK_DEBUG_TRACE(cntr->domain->fabric, FI_LOG_EP_CTRL,
				 "cntr: %p bind flags: %s\n",
				 (void *) cntr->hcntr,
				 fi_tostr(&flags, FI_TYPE_CAPS));
		break;
	}
	return hfid->ops->bind(hfid, hbfid, flags);
}

static void hook_debug_txrx_entry_init(struct ofi_bufpool_region *region,
				       void *buf)
{
	struct hook_debug_txrx_entry *entry = buf;
	entry->magic = OFI_MAGIC_64;
	entry->ep = region->pool->attr.context;
}

struct fi_ops hook_debug_ep_fid_ops;
static struct fi_ops_msg hook_debug_msg_ops = {
	.size 		= sizeof(struct fi_ops_msg),
	.recv 		= hook_debug_recv,
	.recvv 		= fi_no_msg_recvv,
	.recvmsg 	= hook_debug_recvmsg,
	.send		= hook_debug_send,
	.senddata 	= hook_debug_senddata,
	.sendv		= hook_debug_sendv,
	.sendmsg 	= hook_debug_sendmsg,
	.inject 	= hook_debug_inject,
	.injectdata 	= hook_debug_injectdata,
};

struct fi_ops_tagged hook_debug_tagged_ops = {
	.recv 		= hook_debug_trecv,
	.recvv 		= fi_no_tagged_recvv,
	.recvmsg 	= fi_no_tagged_recvmsg,
	.send 		= hook_debug_tsend,
	.senddata 	= hook_debug_tsenddata,
	.sendv		= hook_debug_tsendv,
	.sendmsg 	= hook_debug_tsendmsg,
	.inject 	= hook_debug_tinject,
	.injectdata	= hook_debug_tinjectdata,
};

static int
hook_debug_endpoint(struct fid_domain *domain, struct fi_info *info,
		    struct fid_ep **ep, void *context)
{
	struct hook_debug_ep *myep;
	struct ofi_bufpool_attr bufpool_attr = {
		.size		= sizeof(struct hook_debug_txrx_entry),
		.alignment	= 16,
		.max_cnt	= 0,
		.init_fn	= hook_debug_txrx_entry_init,
	};

	int ret = -FI_ENOMEM;

	if (info->domain_attr->threading != FI_THREAD_DOMAIN) {
		FI_WARN(&hook_debug_prov_ctx.prov, FI_LOG_CQ,
			"debug hooking provider doesn't support thread safety"
			"at this time\n");
		return -FI_EINVAL;
	}

	FI_TRACE(hook_to_hprov(&domain->fid), FI_LOG_EP_CTRL,
		 "tx_attr->size: %zu\n", info->tx_attr->size);
	FI_TRACE(hook_to_hprov(&domain->fid), FI_LOG_EP_CTRL,
		 "rx_attr->size: %zu\n", info->rx_attr->size);

	myep = calloc(1, sizeof *myep);
	if (!myep)
		return ret;

	bufpool_attr.context = myep;

	if (config.track_sends) {
		bufpool_attr.chunk_cnt = info->tx_attr->size;
		ret = ofi_bufpool_create_attr(&bufpool_attr, &myep->tx_pool);
		if (ret)
			goto err;
	}

	if (config.track_recvs) {
		bufpool_attr.chunk_cnt = info->rx_attr->size;
		ret = ofi_bufpool_create_attr(&bufpool_attr, &myep->rx_pool);
		if (ret)
			goto err;
	}

	ret = hook_endpoint_init(domain, info, ep, context, &myep->hook_ep);
	if (ret)
		goto err;

	FI_TRACE(hook_to_hprov(&myep->hook_ep.ep.fid), FI_LOG_EP_CTRL,
		 "endpoint opened, fid: %p\n", (void *) &myep->hook_ep.hep->fid);

	myep->hook_ep.ep.fid.ops = &hook_debug_ep_fid_ops;
	myep->hook_ep.ep.msg = &hook_debug_msg_ops;
	myep->hook_ep.ep.tagged = &hook_debug_tagged_ops;
	myep->tx_op_flags = info->tx_attr->op_flags;
	myep->rx_op_flags = info->rx_attr->op_flags;

	return 0;
err:
	hook_debug_ep_close(&myep->hook_ep.ep.fid);
	return ret;
}

/*
 * EQ
 */

static ssize_t hook_debug_eq_read(struct fid_eq *eq, uint32_t *event,
				  void *buf, size_t len, uint64_t flags)
{
	struct hook_debug_eq *myeq = container_of(eq, struct hook_debug_eq,
						  hook_eq.eq);
	ssize_t ret;

	ret = hook_eq_read(eq, event, buf, len, flags);
	if (ret > 0)
		ofi_atomic_inc64(&myeq->event_cntr[*event]);

	hook_debug_trace_exit_eq(myeq, "fi_eq_read", (ssize_t)ret);
	return ret;
}

static ssize_t hook_debug_eq_sread(struct fid_eq *eq, uint32_t *event,
				   void *buf, size_t len, int timeout,
				   uint64_t flags)
{
	struct hook_debug_eq *myeq = container_of(eq, struct hook_debug_eq,
						  hook_eq.eq);
	ssize_t ret;

	ret = hook_eq_sread(eq, event, buf, len, timeout, flags);
	if (ret > 0)
		ofi_atomic_inc64(&myeq->event_cntr[*event]);

	hook_debug_trace_exit_eq(myeq, "fi_eq_sread", (ssize_t)ret);
	return ret;
}

static int hook_debug_eq_close(struct fid *fid)
{
	struct hook_debug_eq *myeq = container_of(fid, struct hook_debug_eq,
						  hook_eq.eq.fid);
	int i, ret;

	HOOK_DEBUG_TRACE(myeq->hook_eq.fabric, FI_LOG_EQ, "EQ events:\n");

	for (i = 0; i < HOOK_DEBUG_EQ_EVENT_MAX; i++)
		HOOK_DEBUG_TRACE(myeq->hook_eq.fabric, FI_LOG_EQ,
				 "%-20s: %" PRIu64 "\n",
				 fi_tostr(&i, FI_TYPE_EQ_EVENT),
				 ofi_atomic_get64(&myeq->event_cntr[i]));

	ret = fi_close(&myeq->hook_eq.heq->fid);
	if (!ret)
		free(myeq);

	return ret;
}

static struct fi_ops_eq hook_debug_eq_ops;
static struct fi_ops hook_debug_eq_fid_ops;

static int
hook_debug_eq_open(struct fid_fabric *fabric, struct fi_eq_attr *attr,
		   struct fid_eq **eq, void *context)
{
	struct hook_debug_eq *myeq;
	int i, ret;

	myeq = calloc(1, sizeof *myeq);
	if (!myeq)
		return -FI_ENOMEM;

	ret = hook_eq_init(fabric, attr, eq, context, &myeq->hook_eq);
	if (ret) {
		free(myeq);
		return ret;
	}

	myeq->hook_eq.eq.ops = &hook_debug_eq_ops;
	myeq->hook_eq.eq.fid.ops = &hook_debug_eq_fid_ops;

	for (i = 0; i < HOOK_DEBUG_EQ_EVENT_MAX; i++)
		ofi_atomic_initialize64(&myeq->event_cntr[i], 0);

	return 0;
}

/*
 * Fabric
 */

struct fi_ops hook_debug_fabric_fid_ops;
static struct fi_ops_fabric hook_debug_fabric_ops;

static int hook_debug_fabric(struct fi_fabric_attr *attr,
			    struct fid_fabric **fabric, void *context)
{
	struct fi_provider *hprov = context;
	struct hook_fabric *fab;

	FI_TRACE(hprov, FI_LOG_FABRIC, "Installing debug hook\n");
	fab = calloc(1, sizeof *fab);
	if (!fab)
		return -FI_ENOMEM;

	hook_fabric_init(fab, HOOK_DEBUG, attr->fabric, hprov,
			 &hook_debug_fabric_fid_ops, &hook_debug_prov_ctx);
	*fabric = &fab->fabric;
	fab->fabric.ops = &hook_debug_fabric_ops;
	return 0;
}

struct hook_prov_ctx hook_debug_prov_ctx = {
	.prov = {
		.version = OFI_VERSION_DEF_PROV,
		/* We're a pass-through provider, so the fi_version is always the latest */
		.fi_version = OFI_VERSION_LATEST,
		.name = "ofi_hook_debug",
		.getinfo = NULL,
		.fabric = hook_debug_fabric,
		.cleanup = NULL,
	},
};

static uint64_t hook_debug_cntr_read(struct fid_cntr *cntr)
{
	struct hook_cntr *mycntr = container_of(cntr, struct hook_cntr, cntr);
	uint64_t ret;

	ret = fi_cntr_read(mycntr->hcntr);
	hook_debug_trace_exit_cntr(mycntr, "fi_cntr_read", (ssize_t)ret);
	return ret;
}


static int hook_debug_cntr_wait(struct fid_cntr *cntr, uint64_t threshold, int timeout)
{
	struct hook_cntr *mycntr = container_of(cntr, struct hook_cntr, cntr);
	int ret;

	HOOK_DEBUG_TRACE(mycntr->domain->fabric, FI_LOG_CNTR,
			 "cntr: %p, threshold: %" PRIu64 ", timeout: %d\n",
			 (void *) mycntr->hcntr, threshold, timeout);

	ret = fi_cntr_wait(mycntr->hcntr, threshold, timeout);

	hook_debug_trace_exit_cntr(mycntr, "fi_cntr_wait", (ssize_t)ret);
	return ret;
}

static struct fi_ops_cntr hook_debug_cntr_ops;

static int hook_debug_cntr_init(struct fid *fid)
{
	struct hook_cntr *mycntr = container_of(fid, struct hook_cntr, cntr.fid);
	HOOK_DEBUG_TRACE(mycntr->domain->fabric, FI_LOG_CNTR,
			 "fi_cntr_open: %p\n", (void *) mycntr->hcntr);
	mycntr->cntr.ops = &hook_debug_cntr_ops;
	return 0;
}

static struct fi_ops_domain hook_debug_domain_ops;

static int hook_debug_domain_init(struct fid *fid)
{
	struct fid_domain *domain = container_of(fid, struct fid_domain, fid);
	domain->ops = &hook_debug_domain_ops;
	return 0;
}

HOOK_DEBUG_INI
{
	// TODO explore adding a common hook_ini function that can initialize
	// the ops to common ones. Then override here.
	hook_debug_fabric_fid_ops = hook_fid_ops;
	hook_debug_fabric_ops = hook_fabric_ops;
	hook_debug_fabric_ops.eq_open = hook_debug_eq_open;

	hook_debug_eq_fid_ops = hook_fid_ops;
	hook_debug_eq_fid_ops.close = hook_debug_eq_close;
	hook_debug_eq_ops = hook_eq_ops;
	hook_debug_eq_ops.read = hook_debug_eq_read;
	hook_debug_eq_ops.sread = hook_debug_eq_sread;

	hook_debug_domain_ops = hook_domain_ops;
	hook_debug_domain_ops.cq_open = hook_debug_cq_open;
	hook_debug_domain_ops.endpoint = hook_debug_endpoint;

	hook_debug_cq_fid_ops = hook_fid_ops;
	hook_debug_cq_fid_ops.close = hook_debug_cq_close;

	hook_debug_cq_ops = hook_cq_ops;
	hook_debug_cq_ops.read = hook_debug_cq_read;
	hook_debug_cq_ops.readfrom = hook_debug_cq_readfrom;
	hook_debug_cq_ops.sread = fi_no_cq_sread;
	hook_debug_cq_ops.sreadfrom = fi_no_cq_sreadfrom;

	hook_debug_cntr_ops = hook_cntr_ops;
	hook_debug_cntr_ops.read = hook_debug_cntr_read;
	hook_debug_cntr_ops.add = fi_no_cntr_add;
	hook_debug_cntr_ops.set = fi_no_cntr_set;
	hook_debug_cntr_ops.wait = hook_debug_cntr_wait;

	hook_debug_ep_fid_ops = hook_fid_ops;
	hook_debug_ep_fid_ops.bind = hook_debug_ep_bind;
	hook_debug_ep_fid_ops.close = hook_debug_ep_close;

	hook_debug_prov_ctx.ini_fid[FI_CLASS_DOMAIN] = hook_debug_domain_init;
	hook_debug_prov_ctx.ini_fid[FI_CLASS_CNTR] = hook_debug_cntr_init;
	return &hook_debug_prov_ctx.prov;
}
