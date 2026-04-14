/*
 * Copyright (c) 2022 ORNL. All rights reserved.
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

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <ctype.h>

#include <rdma/fi_errno.h>
#include "ofi_util.h"
#include "ofi.h"
#include "ofi_str.h"
#include "ofi_prov.h"
#include "ofi_perf.h"
#include "ofi_hmem.h"
#include "ofi_lock.h"
#include "rdma/fi_ext.h"
#include "ofi_iov.h"
#include "lnx.h"

int lnx_get_msg(struct fid_peer_srx *srx, struct fi_peer_match_attr *match,
		struct fi_peer_rx_entry **entry)
{
	return -FI_ENOSYS;
}

int lnx_queue_msg(struct fi_peer_rx_entry *entry)
{
	return -FI_ENOSYS;
}

void lnx_free_entry(struct fi_peer_rx_entry *entry)
{
	struct lnx_rx_entry *rx_entry;
	ofi_spin_t *bplock;

	rx_entry = container_of(entry, struct lnx_rx_entry, rx_entry);

	if (rx_entry->rx_global)
		bplock = &global_bplock;
	else
		bplock = &rx_entry->rx_lep->le_bplock;

	ofi_spin_lock(bplock);
	ofi_buf_free(rx_entry);
	ofi_spin_unlock(bplock);
}

static void
lnx_init_rx_entry(struct lnx_rx_entry *entry, const struct iovec *iov,
		  void **desc, size_t count, fi_addr_t addr, uint64_t tag,
		  uint64_t ignore, void *context, uint64_t flags)
{
	if (iov)
		memcpy(&entry->rx_iov, iov, sizeof(*iov) * count);
	if (desc)
		memcpy(entry->rx_desc, desc, sizeof(*desc) * count);

	entry->rx_entry.iov = entry->rx_iov;
	entry->rx_entry.desc = entry->rx_desc;
	entry->rx_entry.count = count;
	entry->rx_entry.addr = addr;
	entry->rx_entry.context = context;
	entry->rx_entry.tag = tag;
	entry->rx_entry.flags = flags | FI_TAGGED | FI_RECV;
	entry->rx_ignore = ignore;
}

static struct lnx_rx_entry *
get_rx_entry(struct lnx_ep *lep, const struct iovec *iov,
	     void **desc, size_t count, fi_addr_t addr,
	     uint64_t tag, uint64_t ignore, void *context,
	     uint64_t flags)
{
	struct lnx_rx_entry *rx_entry = NULL;
	ofi_spin_t *bplock;
	struct ofi_bufpool *bp;

	/* if lp is NULL, then we don't know where the message is going to
	 * come from, so allocate the rx_entry from a global pool
	 */
	if (!lep) {
		bp = global_recv_bp;
		bplock = &global_bplock;
	} else {
		bp = lep->le_recv_bp;
		bplock = &lep->le_bplock;
	}

	ofi_spin_lock(bplock);
	rx_entry = (struct lnx_rx_entry *)ofi_buf_alloc(bp);
	ofi_spin_unlock(bplock);
	if (rx_entry) {
		memset(rx_entry, 0, sizeof(*rx_entry));
		if (!lep)
			rx_entry->rx_global = true;
		rx_entry->rx_lep = lep;
		lnx_init_rx_entry(rx_entry, iov, desc, count, addr, tag,
				  ignore, context, flags);
	}

	return rx_entry;
}

static inline struct lnx_rx_entry *
lnx_find_first_match(struct lnx_queue *q, struct lnx_match_attr *match)
{
	struct lnx_rx_entry *rx_entry;

	rx_entry = (struct lnx_rx_entry *) dlist_find_first_match(
			&q->lq_queue, q->lq_match_func, match);

	return rx_entry;
}

static inline void lnx_update_queue_stats(struct lnx_queue *q, bool dq)
{
	if (dq)
		q->lq_size--;
	else
		q->lq_size++;

	if (q->lq_size > q->lq_max)
		q->lq_max = q->lq_size;

	q->lq_rolling_sum += q->lq_size;
	q->lq_count++;
	q->lq_rolling_avg = q->lq_rolling_sum / q->lq_count;
}

static inline struct lnx_rx_entry *
lnx_remove_first_match(struct lnx_queue *q, struct lnx_match_attr *match)
{
	struct lnx_rx_entry *rx_entry;

	rx_entry = (struct lnx_rx_entry *) dlist_remove_first_match(
			&q->lq_queue, q->lq_match_func, match);
	if (rx_entry)
		lnx_update_queue_stats(q, true);

	return rx_entry;
}

static inline void
lnx_insert_rx_entry(struct lnx_queue *q, struct lnx_rx_entry *entry)
{
	dlist_insert_tail(&entry->entry, &q->lq_queue);
	lnx_update_queue_stats(q, false);
}

int lnx_queue_tag(struct fi_peer_rx_entry *entry)
{
	struct lnx_rx_entry *rx_entry;
	struct lnx_peer_srq *lnx_srq = (struct lnx_peer_srq*)entry->owner_context;

	rx_entry = container_of(entry, struct lnx_rx_entry, rx_entry);
	FI_DBG(&lnx_prov, FI_LOG_CORE,
		"addr = %lx tag = %lx ignore = 0 found\n",
		entry->addr, entry->tag);

	lnx_insert_rx_entry(&lnx_srq->lps_trecv.lqp_unexq, rx_entry);

	return 0;
}

int lnx_get_tag(struct fid_peer_srx *srx, struct fi_peer_match_attr *match,
		struct fi_peer_rx_entry **entry)
{
	struct lnx_match_attr match_attr = {0};
	struct lnx_peer_srq *lnx_srq;
	struct lnx_core_ep *cep;
	struct lnx_ep *lep;
	struct lnx_rx_entry *rx_entry;
	fi_addr_t addr = match->addr;
	uint64_t tag = match->tag;
	int rc = 0;

	cep = srx->ep_fid.fid.context;
	lep = cep->cep_parent;
	lnx_srq = &lep->le_srq;

	match_attr.lm_addr = addr;
	match_attr.lm_tag = tag;

	rx_entry = lnx_remove_first_match(&lnx_srq->lps_trecv.lqp_recvq,
					  &match_attr);
	if (rx_entry) {
		FI_DBG(&lnx_prov, FI_LOG_CORE,
		       "addr = %lx tag = %lx ignore = 0 found\n",
		       match_attr.lm_addr, tag);

		goto assign;
	}

	FI_DBG(&lnx_prov, FI_LOG_CORE,
	       "addr = %lx tag = %lx ignore = 0 not found\n",
	       match_attr.lm_addr, tag);

	rx_entry = get_rx_entry(lep, NULL, NULL, 0, addr, tag, 0, NULL,
				FI_TAGGED | FI_RECV);
	if (!rx_entry) {
		rc = -FI_ENOMEM;
		goto out;
	}

	rx_entry->rx_entry.owner_context = lnx_srq;
	rx_entry->rx_cep = cep;

	rc = -FI_ENOENT;

	cep->cep_t_stats.st_num_unexp_msgs++;

	goto finalize;

assign:
	cep->cep_t_stats.st_num_posted_recvs++;

	rx_entry->rx_entry.addr = lnx_get_core_addr(cep, addr);
	if (rx_entry->rx_entry.desc && *rx_entry->rx_entry.desc) {
		rc = lnx_mr_regattr_core(cep->cep_domain,
					 *rx_entry->rx_entry.desc,
					 rx_entry->rx_entry.desc);
		if (rc)
			return rc;
	}
finalize:
	rx_entry->rx_entry.msg_size = match->msg_size;
	*entry = &rx_entry->rx_entry;

out:
	return rc;
}

static int
lnx_discard(struct lnx_ep *lep, struct lnx_rx_entry *rx_entry, void *context)
{
	struct lnx_core_ep *cep = rx_entry->rx_cep;
	int rc;

	rc = cep->cep_srx.peer_ops->discard_tag(&rx_entry->rx_entry);
	if (rc) {
		FI_WARN(&lnx_prov, FI_LOG_CORE,
			"Error discarding message from %s\n",
			cep->cep_domain->cd_info->fabric_attr->name);
	}

	rc = ofi_cq_write(lep->le_ep.rx_cq, context,
			  rx_entry->rx_entry.flags,
			  rx_entry->rx_entry.msg_size, NULL,
			  rx_entry->rx_entry.cq_data,
			  rx_entry->rx_entry.tag);

	lnx_free_entry(&rx_entry->rx_entry);

	return rc;
}

static int
lnx_peek(struct lnx_ep *lep, struct lnx_match_attr *match_attr, void *context,
	 uint64_t flags)
{
	int rc;
	struct lnx_rx_entry *rx_entry;
	struct lnx_peer_srq *lnx_srq = &lep->le_srq;

	rx_entry = lnx_find_first_match(&lnx_srq->lps_trecv.lqp_unexq,
					match_attr);
	if (!rx_entry) {
		FI_DBG(&lnx_prov, FI_LOG_CORE,
			"PEEK addr=%lx tag=%lx ignore=%lx\n",
			match_attr->lm_addr, match_attr->lm_tag,
			match_attr->lm_ignore);
		return ofi_cq_write_error_peek(lep->le_ep.rx_cq,
				match_attr->lm_tag, context);
	}

	rc = ofi_cq_write(lep->le_ep.rx_cq, context,
			  rx_entry->rx_entry.flags,
			  rx_entry->rx_entry.msg_size, NULL,
			  rx_entry->rx_entry.cq_data,
			  rx_entry->rx_entry.tag);

	if (flags & FI_DISCARD) {
		lnx_free_entry(&rx_entry->rx_entry);
		goto out;
	}

	if (flags & FI_CLAIM)
		((struct fi_context *)context)->internal[0] = rx_entry;

out:
	return rc;
}

/*
 * if lp is NULL, then we're attempting to receive from any peer so
 * matching the tag is the only thing that matters.
 *
 * if lp != NULL, then we're attempting to receive from a particular
 * peer. This peer can have multiple endpoints serviced by different core
 * providers.
 *
 * Therefore when we check the unexpected queue, we need to check
 * if we received any messages from any of the peer's addresses. If we
 * find one, then we kick the core provider associated with that
 * address to receive the message.
 *
 * If nothing is found on the unexpected messages, then add a receive
 * request on the SRQ; happens in the lnx_process_recv()
 */
static int lnx_process_recv(struct lnx_ep *lep, const struct iovec *iov, void *desc,
			fi_addr_t addr, size_t count, uint64_t tag,
			uint64_t ignore, void *context, uint64_t flags,
			bool tagged)
{
	struct lnx_peer_srq *lnx_srq = &lep->le_srq;
	struct lnx_rx_entry *rx_entry;
	struct lnx_match_attr match_attr;
	struct lnx_core_ep *cep;
	int rc = 0;
	fi_addr_t sub_addr, encoded_addr = lnx_encode_fi_addr(addr, 0);

	/* Matching format should always be in the encoded form */
	match_attr.lm_addr = (addr == FI_ADDR_UNSPEC) ||
		!(lep->le_ep.caps & FI_DIRECTED_RECV) ? FI_ADDR_UNSPEC :
		encoded_addr;
	match_attr.lm_ignore = ignore;
	match_attr.lm_tag = tag;

	if (flags & FI_PEEK)
		return lnx_peek(lep, &match_attr, context, flags);

	if (flags & FI_DISCARD) {
		rx_entry = (struct lnx_rx_entry *)
			(((struct fi_context *)context)->internal[0]);
		return lnx_discard(lep, rx_entry, context);
	}

	if (flags & FI_CLAIM) {
		rx_entry = (struct lnx_rx_entry *)
			(((struct fi_context *)context)->internal[0]);
	} else {
		rx_entry = lnx_remove_first_match(&lnx_srq->lps_trecv.lqp_unexq,
						&match_attr);
		if (!rx_entry) {
			FI_DBG(&lnx_prov, FI_LOG_CORE,
			"addr=%lx tag=%lx ignore=%lx buf=%p len=%lx not found\n",
			addr, tag, ignore, iov->iov_base, iov->iov_len);

			goto nomatch;
		}
	}

	FI_DBG(&lnx_prov, FI_LOG_CORE,
	       "addr=%lx tag=%lx ignore=%lx buf=%p len=%lx found\n",
	       addr, tag, ignore, iov->iov_base, iov->iov_len);

	/* match is found in the unexpected queue. call into the core
	 * provider to complete this message
	 */
	cep = rx_entry->rx_cep;
	sub_addr = lnx_get_core_addr(cep, rx_entry->rx_entry.addr);
	lnx_init_rx_entry(rx_entry, iov, desc, count, sub_addr,
			  tag, ignore, context,
			  rx_entry->rx_entry.flags | flags);
	rx_entry->rx_entry.msg_size = MIN(ofi_total_iov_len(iov, count),
				          rx_entry->rx_entry.msg_size);

	if (desc) {
		rc = lnx_mr_regattr_core(cep->cep_domain, desc,
					 rx_entry->rx_entry.desc);
		if (rc)
			return rc;
	}

	if (tagged)
		rc = cep->cep_srx.peer_ops->start_tag(&rx_entry->rx_entry);
	else
		rc = cep->cep_srx.peer_ops->start_msg(&rx_entry->rx_entry);

	if (rc == -FI_EINPROGRESS) {
		/* this is telling me that more messages can match the same
		 * rx_entry. So keep it on the queue
		 */
		FI_DBG(&lnx_prov, FI_LOG_CORE,
		       "addr = %lx tag = %lx ignore = %lx start_tag() in progress\n",
		       addr, tag, ignore);

		goto insert_recvq;
	} else if (rc) {
		FI_WARN(&lnx_prov, FI_LOG_CORE, "start tag failed with %d\n", rc);
	}

	FI_DBG(&lnx_prov, FI_LOG_CORE,
	       "addr = %lx tag = %lx ignore = %lx start_tag() success\n",
	       addr, tag, ignore);

	return 0;

nomatch:
	/* nothing on the unexpected queue, then allocate one and put it on
	 * the receive queue
	 */
	rx_entry = get_rx_entry(NULL, iov, (desc) ? &desc : NULL, count,
				match_attr.lm_addr, tag, ignore, context,
				flags);
	if (!rx_entry) {
		rc = -FI_ENOMEM;
		goto out;
	}

insert_recvq:
	lnx_insert_rx_entry(&lnx_srq->lps_trecv.lqp_recvq, rx_entry);

out:
	return rc;
}

ssize_t lnx_trecv(struct fid_ep *ep, void *buf, size_t len, void *desc,
		fi_addr_t src_addr, uint64_t tag, uint64_t ignore, void *context)
{
	struct lnx_ep *lep;
	const struct iovec iov = {.iov_base = buf, .iov_len = len};

	lep = container_of(ep, struct lnx_ep, le_ep.ep_fid.fid);
	if (!lep)
		return -FI_ENOSYS;

	return lnx_process_recv(lep, &iov, desc, src_addr, 1, tag, ignore,
				context, lnx_ep_rx_flags(lep), true);
}

ssize_t lnx_trecvv(struct fid_ep *ep, const struct iovec *iov, void **desc,
		size_t count, fi_addr_t src_addr, uint64_t tag, uint64_t ignore,
		void *context)
{
	void *mr_desc;
	struct lnx_ep *lep;

	lep = container_of(ep, struct lnx_ep, le_ep.ep_fid.fid);
	if (!lep)
		return -FI_ENOSYS;


	if (count == 0) {
		mr_desc = NULL;
	} else if (iov && count >= 1 &&
		   count <= lep->le_domain->ld_iov_limit) {
		mr_desc = desc ? desc[0] : NULL;
	} else {
		FI_WARN(&lnx_prov, FI_LOG_CORE, "Invalid IOV\n");
		return -FI_EINVAL;
	}

	return lnx_process_recv(lep, iov, mr_desc, src_addr, count, tag, ignore,
				context, lnx_ep_rx_flags(lep), true);
}

ssize_t lnx_trecvmsg(struct fid_ep *ep, const struct fi_msg_tagged *msg,
		     uint64_t flags)
{
	void *mr_desc;
	struct lnx_ep *lep;

	lep = container_of(ep, struct lnx_ep, le_ep.ep_fid.fid);
	if (!lep)
		return -FI_ENOSYS;

	if (msg->iov_count == 0) {
		mr_desc = NULL;
	} else if (msg->msg_iov && msg->iov_count >= 1 &&
		   msg->iov_count <= lep->le_domain->ld_iov_limit) {
		mr_desc = msg->desc ? msg->desc[0] : NULL;
	} else {
		FI_WARN(&lnx_prov, FI_LOG_CORE, "Invalid IOV\n");
		return -FI_EINVAL;
	}

	return lnx_process_recv(lep, msg->msg_iov, mr_desc, msg->addr, msg->iov_count,
				msg->tag, msg->ignore, msg->context,
				flags | lep->le_ep.rx_msg_flags, true);
}

ssize_t lnx_tsend(struct fid_ep *ep, const void *buf, size_t len, void *desc,
		fi_addr_t dest_addr, uint64_t tag, void *context)
{
	int rc;
	struct lnx_ep *lep;
	void *core_desc = NULL;
	struct lnx_core_ep *cep;
	fi_addr_t core_addr;

	lep = container_of(ep, struct lnx_ep, le_ep.ep_fid.fid);
	if (!lep)
		return -FI_ENOSYS;

	rc = lnx_select_send_endpoints(lep, dest_addr, &cep, &core_addr);
	if (rc)
		return rc;

	FI_DBG(&lnx_prov, FI_LOG_CORE,
	       "sending to %lx tag %lx buf %p len %ld\n",
	       core_addr, tag, buf, len);

	if (desc) {
		rc = lnx_mr_regattr_core(cep->cep_domain, desc, &core_desc);
		if (rc)
			return rc;
	}

	rc = fi_tsend(cep->cep_ep, buf, len, core_desc, core_addr, tag, context);

	if (!rc)
		cep->cep_t_stats.st_num_tsend++;

	return rc;
}

ssize_t lnx_tsendv(struct fid_ep *ep, const struct iovec *iov, void **desc,
		size_t count, fi_addr_t dest_addr, uint64_t tag, void *context)
{
	int rc;
	struct lnx_ep *lep;
	void *core_desc = NULL;
	struct lnx_core_ep *cep;
	fi_addr_t core_addr;

	if (count > 1)
		return -FI_EINVAL;

	lep = container_of(ep, struct lnx_ep, le_ep.ep_fid.fid);
	if (!lep)
		return -FI_ENOSYS;

	rc = lnx_select_send_endpoints(lep, dest_addr, &cep, &core_addr);
	if (rc)
		return rc;

	FI_DBG(&lnx_prov, FI_LOG_CORE,
	       "sending to %lx tag %lx buf %p len %ld\n",
	       core_addr, tag, iov->iov_base, iov->iov_len);

	if (desc && *desc) {
		rc = lnx_mr_regattr_core(cep->cep_domain, *desc, &core_desc);
		if (rc)
			return rc;
	}

	rc = fi_tsendv(cep->cep_ep, iov, (core_desc) ? &core_desc : NULL,
		       count, core_addr, tag, context);

	if (!rc)
		cep->cep_t_stats.st_num_tsendv++;

	return rc;
}

ssize_t lnx_tsendmsg(struct fid_ep *ep, const struct fi_msg_tagged *msg,
		uint64_t flags)
{
	int rc;
	struct lnx_ep *lep;
	void *core_desc = NULL;
	struct lnx_core_ep *cep;
	struct fi_msg_tagged core_msg;

	if (msg->iov_count > 1)
		return -FI_EINVAL;

	memcpy(&core_msg, msg, sizeof(*msg));

	lep = container_of(ep, struct lnx_ep, le_ep.ep_fid.fid);
	if (!lep)
		return -FI_ENOSYS;

	rc = lnx_select_send_endpoints(lep, core_msg.addr, &cep, &core_msg.addr);
	if (rc)
		return rc;

	FI_DBG(&lnx_prov, FI_LOG_CORE,
	       "sending to %lx tag %lx\n",
	       core_msg.addr, core_msg.tag);

	if (core_msg.desc && *core_msg.desc) {
		rc = lnx_mr_regattr_core(cep->cep_domain, *core_msg.desc, &core_desc);
		if (rc)
			return rc;
		core_msg.desc = &core_desc;
	}

	rc = fi_tsendmsg(cep->cep_ep, &core_msg, flags);

	if (!rc)
		cep->cep_t_stats.st_num_tsendmsg++;

	return rc;
}

ssize_t lnx_tinject(struct fid_ep *ep, const void *buf, size_t len,
		fi_addr_t dest_addr, uint64_t tag)
{
	int rc;
	struct lnx_ep *lep;
	struct lnx_core_ep *cep;
	fi_addr_t core_addr;

	lep = container_of(ep, struct lnx_ep, le_ep.ep_fid.fid);
	if (!lep)
		return -FI_ENOSYS;

	rc = lnx_select_send_endpoints(lep, dest_addr, &cep, &core_addr);
	if (rc)
		return rc;

	FI_DBG(&lnx_prov, FI_LOG_CORE,
	       "sending to %lx tag %lx buf %p len %ld\n",
	       core_addr, tag, buf, len);

	rc = fi_tinject(cep->cep_ep, buf, len, core_addr, tag);

	if (!rc)
		cep->cep_t_stats.st_num_tinject++;

	return rc;
}

ssize_t lnx_tsenddata(struct fid_ep *ep, const void *buf, size_t len, void *desc,
		uint64_t data, fi_addr_t dest_addr, uint64_t tag, void *context)
{
	int rc;
	struct lnx_ep *lep;
	struct lnx_core_ep *cep;
	fi_addr_t core_addr;
	void *core_desc = desc;

	lep = container_of(ep, struct lnx_ep, le_ep.ep_fid.fid);
	if (!lep)
		return -FI_ENOSYS;

	rc = lnx_select_send_endpoints(lep, dest_addr, &cep, &core_addr);
	if (rc)
		return rc;

	FI_DBG(&lnx_prov, FI_LOG_CORE,
	       "sending to %lx tag %lx buf %p len %ld\n",
	       core_addr, tag, buf, len);

	if (desc) {
		rc = lnx_mr_regattr_core(cep->cep_domain, desc, &core_desc);
		if (rc)
			return rc;
	}

	rc = fi_tsenddata(cep->cep_ep, buf, len, core_desc,
			  data, core_addr, tag, context);

	if (!rc)
		cep->cep_t_stats.st_num_tsenddata++;

	return rc;
}

ssize_t lnx_tinjectdata(struct fid_ep *ep, const void *buf, size_t len,
			uint64_t data, fi_addr_t dest_addr, uint64_t tag)
{
	int rc;
	struct lnx_ep *lep;
	struct lnx_core_ep *cep;
	fi_addr_t core_addr;

	lep = container_of(ep, struct lnx_ep, le_ep.ep_fid.fid);
	if (!lep)
		return -FI_ENOSYS;

	rc = lnx_select_send_endpoints(lep, dest_addr, &cep, &core_addr);
	if (rc)
		return rc;

	FI_DBG(&lnx_prov, FI_LOG_CORE,
	       "sending to %lx tag %lx buf %p len %ld\n",
	       core_addr, tag, buf, len);

	rc = fi_tinjectdata(cep->cep_ep, buf, len, data, core_addr, tag);

	if (!rc)
		cep->cep_t_stats.st_num_tinjectdata++;

	return rc;
}

struct fi_ops_tagged lnx_tagged_ops = {
	.size = sizeof(struct fi_ops_tagged),
	.recv = lnx_trecv,
	.recvv = lnx_trecvv,
	.recvmsg = lnx_trecvmsg,
	.send = lnx_tsend,
	.sendv = lnx_tsendv,
	.sendmsg = lnx_tsendmsg,
	.inject = lnx_tinject,
	.senddata = lnx_tsenddata,
	.injectdata = lnx_tinjectdata,
};

struct fi_ops_msg lnx_msg_ops = {
	.size = sizeof(struct fi_ops_msg),
	.recv = fi_no_msg_recv,
	.recvv = fi_no_msg_recvv,
	.recvmsg = fi_no_msg_recvmsg,
	.send = fi_no_msg_send,
	.sendv = fi_no_msg_sendv,
	.sendmsg = fi_no_msg_sendmsg,
	.inject = fi_no_msg_inject,
	.senddata = fi_no_msg_senddata,
	.injectdata = fi_no_msg_injectdata,
};

struct fi_ops_rma lnx_rma_ops = {
	.size = sizeof(struct fi_ops_rma),
	.read = fi_no_rma_read,
	.readv = fi_no_rma_readv,
	.readmsg = fi_no_rma_readmsg,
	.write = fi_no_rma_write,
	.writev = fi_no_rma_writev,
	.writemsg = fi_no_rma_writemsg,
	.inject = fi_no_rma_inject,
	.writedata = fi_no_rma_writedata,
	.injectdata = fi_no_rma_injectdata,
};

struct fi_ops_atomic lnx_atomic_ops = {
	.size = sizeof(struct fi_ops_atomic),
	.write = fi_no_atomic_write,
	.writev = fi_no_atomic_writev,
	.writemsg = fi_no_atomic_writemsg,
	.inject = fi_no_atomic_inject,
	.readwrite = fi_no_atomic_readwrite,
	.readwritev = fi_no_atomic_readwritev,
	.readwritemsg = fi_no_atomic_readwritemsg,
	.compwrite = fi_no_atomic_compwrite,
	.compwritev = fi_no_atomic_compwritev,
	.compwritemsg = fi_no_atomic_compwritemsg,
	.writevalid = fi_no_atomic_writevalid,
	.readwritevalid = fi_no_atomic_readwritevalid,
	.compwritevalid = fi_no_atomic_compwritevalid,
};


