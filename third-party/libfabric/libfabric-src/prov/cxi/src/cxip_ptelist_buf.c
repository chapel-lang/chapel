/*
 * Copyright (c) 2018,2021-2023 Hewlett Packard Enterprise Development LP
 * SPDX-License-Identifier: BSD-2-Clause OR GPL-2.0-only
 */

#include "config.h"

#include <stdio.h>
#include <stdlib.h>
#include <ofi_list.h>
#include <ofi.h>

#include "cxip.h"

static const char*
cxip_ptelist_to_str(struct cxip_ptelist_bufpool *pool)
{
	return cxi_ptl_list_to_str(pool->attr.list_type);
}

static int cxip_ptelist_unlink_buf(struct cxip_ptelist_buf *buf)
{
	struct cxip_rxc_hpc *rxc = buf->rxc;
	int ret;

	ret = cxip_pte_unlink(rxc->base.rx_pte, buf->pool->attr.list_type,
			      buf->req->req_id, rxc->base.rx_cmdq);
	if (ret)
		RXC_DBG(rxc, "Failed to write command %d %s\n",
			ret, fi_strerror(-ret));

	return ret;
}

static int cxip_ptelist_link_buf(struct cxip_ptelist_buf *buf,
				 bool seq_restart)
{
	struct cxip_rxc_hpc *rxc = buf->rxc;
	uint32_t le_flags = C_LE_MANAGE_LOCAL | C_LE_NO_TRUNCATE |
		C_LE_UNRESTRICTED_BODY_RO | C_LE_OP_PUT |
		C_LE_UNRESTRICTED_END_RO | C_LE_EVENT_UNLINK_DISABLE;
	int ret;

	/* Match all eager, long sends */
	union cxip_match_bits mb = {
		.le_type = CXIP_LE_TYPE_RX
	};
	union cxip_match_bits ib = {
		.tag = ~0,
		.tx_id = ~0,
		.cq_data = 1,
		.tagged = 1,
		.match_comp = 1,
		.rdzv_done = 1,
	};

	if (!(buf->pool->attr.list_type == C_PTL_LIST_OVERFLOW &&
	      cxip_env.hybrid_preemptive))
		le_flags |= C_LE_EVENT_LINK_DISABLE;

	if (seq_restart)
		le_flags |= C_LE_RESTART_SEQ;

	RXC_DBG(rxc, "%s link buf %p num linked %u\n",
		cxip_ptelist_to_str(buf->pool), buf,
		ofi_atomic_get32(&buf->pool->bufs_linked));

	/* Reset request buffer stats used to know when the buffer is consumed.
	 */
	assert(dlist_empty(&buf->request.pending_ux_list));
	buf->unlink_length = -1;
	buf->cur_offset = 0;

	/* Take a request buffer reference for the link. */
	ret = cxip_pte_append(rxc->base.rx_pte,
			      CXI_VA_TO_IOVA(buf->md->md, buf->data),
			      buf->pool->attr.buf_size, buf->md->md->lac,
			      buf->pool->attr.list_type,
			      buf->req->req_id, mb.raw,
			      ib.raw, CXI_MATCH_ID_ANY,
			      buf->pool->attr.min_space_avail,
			      le_flags, NULL, rxc->base.rx_cmdq, true);
	if (ret) {
		RXC_WARN(rxc, "Failed to write %s append %d %s\n",
			 cxip_ptelist_to_str(buf->pool),
			 ret, fi_strerror(-ret));
		return ret;
	}

	dlist_remove(&buf->buf_entry);
	dlist_insert_tail(&buf->buf_entry,
			  &buf->pool->active_bufs);
	ofi_atomic_inc32(&buf->pool->bufs_linked);

	/* Reference taken until buffer is consumed or manually
	 * unlinked.
	 */
	cxip_ptelist_buf_get(buf);

	RXC_DBG(rxc, "APPEND %s buf %p num linked %u\n",
		cxip_ptelist_to_str(buf->pool), buf,
		ofi_atomic_get32(&buf->pool->bufs_linked));

	return ret;
}

/*
 * cxip_ptelist_buf_alloc() - Allocate a buffer for the Ptl buffer pool.
 */
static struct cxip_ptelist_buf*
cxip_ptelist_buf_alloc(struct cxip_ptelist_bufpool *pool)
{
	struct cxip_rxc_hpc *rxc = pool->rxc;
	struct cxip_ptelist_buf *buf;
	int ret;

	buf = calloc(1, sizeof(*buf));
	if (!buf)
		goto err;

	buf->data = aligned_alloc(pool->buf_alignment, pool->attr.buf_size);
	if (!buf->data)
		goto err_free_buf;

	if (rxc->base.hmem && !cxip_env.disable_host_register) {
		ret = ofi_hmem_host_register(buf->data, pool->attr.buf_size);
		if (ret) {
			RXC_WARN(rxc,
				 "Failed to register buffer with HMEM: %d:%s\n",
				 ret, fi_strerror(-ret));
			goto err_free_data_buf;
		}
	}

	ret = cxip_map(rxc->base.domain, buf->data, pool->attr.buf_size,
		       OFI_MR_NOCACHE, &buf->md);
	if (ret)
		goto err_unreg_buf;

	buf->req = cxip_evtq_req_alloc(&rxc->base.rx_evtq, true, buf);
	if (!buf->req)
		goto err_unmap_buf;

	buf->pool = pool;
	buf->req->cb = pool->attr.ptelist_cb;
	buf->rxc = rxc;
	buf->le_type = CXIP_LE_TYPE_RX;

	if (pool->attr.list_type == C_PTL_LIST_REQUEST)
		buf->req->type = CXIP_REQ_RBUF;
	else
		buf->req->type = CXIP_REQ_OFLOW;

	ofi_atomic_initialize32(&buf->refcount, 0);
	dlist_init(&buf->request.pending_ux_list);
	dlist_init(&buf->buf_entry);
	ofi_atomic_inc32(&pool->bufs_allocated);

	RXC_DBG(rxc, "Allocated %s buf %p num alloc %u\n",
		cxip_ptelist_to_str(buf->pool), buf,
		ofi_atomic_get32(&pool->bufs_allocated));

	return buf;

err_unmap_buf:
	cxip_unmap(buf->md);
err_unreg_buf:
	if (rxc->base.hmem && !cxip_env.disable_host_register)
		ofi_hmem_host_unregister(buf);
err_free_data_buf:
	free(buf->data);
err_free_buf:
	free(buf);
err:
	return NULL;
}

static void cxip_ptelist_buf_free(struct cxip_ptelist_buf *buf)
{
	struct cxip_ux_send *ux;
	struct dlist_entry *tmp;
	struct cxip_rxc_hpc *rxc = buf->rxc;

	/* Sanity check making sure the buffer was properly removed before
	 * freeing.
	 */
	assert(dlist_empty(&buf->buf_entry));

	if (buf->pool->attr.list_type == C_PTL_LIST_REQUEST) {
		dlist_foreach_container_safe(&buf->request.pending_ux_list,
					     struct cxip_ux_send,
					     ux, rxc_entry, tmp) {
			dlist_remove(&ux->rxc_entry);
			_cxip_req_buf_ux_free(ux, false);
		}
	}

	if (ofi_atomic_get32(&buf->refcount) != 0)
		RXC_FATAL(rxc, "%s buf %p non-zero refcount %u\n",
			  cxip_ptelist_to_str(buf->pool), buf,
			  ofi_atomic_get32(&buf->refcount));
	cxip_evtq_req_free(buf->req);
	cxip_unmap(buf->md);
	if (rxc->base.hmem && !cxip_env.disable_host_register)
		ofi_hmem_host_unregister(buf->data);

	ofi_atomic_dec32(&buf->pool->bufs_allocated);

	RXC_DBG(rxc, "Freeing %s buf %p num_alloc %u\n",
		cxip_ptelist_to_str(buf->pool), buf,
		ofi_atomic_get32(&buf->pool->bufs_allocated));
	free(buf->data);
	free(buf);
}

static void cxip_ptelist_buf_dlist_free(struct dlist_entry *head)
{
	struct cxip_ptelist_buf *buf;
	struct dlist_entry *tmp;

	dlist_foreach_container_safe(head, struct cxip_ptelist_buf, buf,
				     buf_entry, tmp) {
		dlist_remove_init(&buf->buf_entry);
		cxip_ptelist_buf_free(buf);
	}
}

void cxip_ptelist_buf_link_err(struct cxip_ptelist_buf *buf,
			       int rc_link_error)
{
	struct cxip_rxc_hpc *rxc = buf->pool->rxc;

	RXC_WARN(rxc, "%s buffer %p link error %s\n",
		 cxip_ptelist_to_str(buf->pool),
		 buf, cxi_rc_to_str(rc_link_error));

	assert(rc_link_error == C_RC_NO_SPACE);

	cxip_ptelist_buf_put(buf, false);
	ofi_atomic_dec32(&buf->pool->bufs_linked);

	/* We are running out of LE resources, do not repost
	 * immediately.
	 */
	assert(ofi_atomic_get32(&buf->refcount) == 0);
	dlist_remove(&buf->buf_entry);
	dlist_insert_tail(&buf->buf_entry, &buf->pool->free_bufs);
	ofi_atomic_inc32(&buf->pool->bufs_free);
}

void cxip_ptelist_buf_unlink(struct cxip_ptelist_buf *buf)
{
	struct cxip_ptelist_bufpool *pool = buf->pool;

	cxip_ptelist_buf_put(buf, false);
	ofi_atomic_dec32(&pool->bufs_linked);

	RXC_DBG(pool->rxc, "%s buffer unlink\n", cxip_ptelist_to_str(pool));
}

int cxip_ptelist_bufpool_init(struct cxip_rxc_hpc *rxc,
			      struct cxip_ptelist_bufpool **pool,
			      struct cxip_ptelist_bufpool_attr *attr)
{
	int i;
	struct cxip_ptelist_buf *buf;
	struct dlist_entry tmp_buf_list;
	struct dlist_entry *tmp;
	struct cxip_ptelist_bufpool *_pool;
	int ret;
	size_t buf_size;


	if (attr->list_type != C_PTL_LIST_REQUEST &&
	    attr->list_type != C_PTL_LIST_OVERFLOW)
		return -FI_EINVAL;

	_pool = calloc(1, sizeof(*_pool));
	if (!_pool)
		return -FI_ENOMEM;

	_pool->buf_alignment = ofi_get_page_size();

	buf_size = roundup(attr->buf_size, _pool->buf_alignment);
	if (attr->buf_size != buf_size)
		RXC_INFO(rxc,
			 "Aligning buf size to %lu: prev_size=%lu new_size=%lu\n",
			 _pool->buf_alignment, attr->buf_size, buf_size);
	attr->buf_size = buf_size;

	_pool->attr = *attr;
	_pool->rxc = rxc;
	dlist_init(&_pool->active_bufs);
	dlist_init(&_pool->consumed_bufs);
	dlist_init(&_pool->free_bufs);
	ofi_atomic_initialize32(&_pool->bufs_linked, 0);
	ofi_atomic_initialize32(&_pool->bufs_allocated, 0);
	ofi_atomic_initialize32(&_pool->bufs_free, 0);

	dlist_init(&tmp_buf_list);

	for (i = 0; i < _pool->attr.min_posted; i++) {
		buf = cxip_ptelist_buf_alloc(_pool);
		if (!buf) {
			ret = -FI_ENOMEM;
			goto err_free_bufs;
		}

		dlist_insert_tail(&buf->buf_entry, &tmp_buf_list);
	}

	/* Since this is called during RXC initialization, RXQ CMDQ should be
	 * empty. Thus, linking a request buffer should not fail.
	 */
	dlist_foreach_container_safe(&tmp_buf_list, struct cxip_ptelist_buf,
				     buf, buf_entry, tmp) {
		ret = cxip_ptelist_link_buf(buf, false);
		if (ret != FI_SUCCESS)
			CXIP_FATAL("Failed to link request buffer: %d %s\n",
				   ret, fi_strerror(-ret));
	}

	*pool = _pool;
	return FI_SUCCESS;

err_free_bufs:
	cxip_ptelist_buf_dlist_free(&tmp_buf_list);

	return ret;
}

void cxip_ptelist_bufpool_fini(struct cxip_ptelist_bufpool *pool)
{
	struct cxip_rxc_hpc *rxc = pool->rxc;
	struct cxip_ptelist_buf *buf;
	int ret;

	assert(rxc->base.rx_pte->state == C_PTLTE_DISABLED);

	RXC_INFO(rxc, "Number of %s buffers allocated %d\n",
		 cxip_ptelist_to_str(pool),
		 ofi_atomic_get32(&pool->bufs_allocated));

	/* All request buffers are split between the active and consumed list.
	 * Only active buffers need to be unlinked.
	 */
	dlist_foreach_container(&pool->active_bufs, struct cxip_ptelist_buf,
				buf, buf_entry) {
		ret = cxip_ptelist_unlink_buf(buf);
		if (ret != FI_SUCCESS)
			CXIP_FATAL("PtlTE %d failed to unlink %s buf %d %s\n",
				   rxc->base.rx_pte->pte->ptn,
				   cxip_ptelist_to_str(pool), ret,
				   fi_strerror(-ret));
	}

	do {
		cxip_evtq_progress(&rxc->base.rx_evtq);
	} while (ofi_atomic_get32(&pool->bufs_linked));

	cxip_ptelist_buf_dlist_free(&pool->active_bufs);
	cxip_ptelist_buf_dlist_free(&pool->consumed_bufs);
	cxip_ptelist_buf_dlist_free(&pool->free_bufs);

	assert(ofi_atomic_get32(&pool->bufs_allocated) == 0);

	assert(pool);
	free(pool);
}

/*
 * cxip_ptelist_buf_replenish() - Replenish PtlTE overflow or request list
 * buffers.
 *
 * Caller must hold ep_obj->lock.
 */
int cxip_ptelist_buf_replenish(struct cxip_ptelist_bufpool *pool,
			       bool seq_restart)
{
	struct cxip_rxc_hpc *rxc = pool->rxc;
	struct cxip_ptelist_buf *buf;
	int bufs_added = 0;
	int ret = FI_SUCCESS;

	if (rxc->base.msg_offload &&
	    pool->attr.list_type == C_PTL_LIST_REQUEST)
		return FI_SUCCESS;

	while (ofi_atomic_get32(&pool->bufs_linked) < pool->attr.min_posted) {

		/* Always prefer to use a free buffer for which
		 * reposting was deferred or an append failed.
		 */
		if (!dlist_empty(&pool->free_bufs)) {
			dlist_pop_front(&pool->free_bufs,
					struct cxip_ptelist_buf, buf,
					buf_entry);
			ofi_atomic_dec32(&buf->pool->bufs_free);

			RXC_DBG(rxc, "%s LINK REPOST buf %p\n",
				cxip_ptelist_to_str(pool), buf);
		} else {
			buf = cxip_ptelist_buf_alloc(pool);

			RXC_DBG(rxc, "%s LINK NEW buf %p\n",
				cxip_ptelist_to_str(pool), buf);

		}

		if (!buf) {
			RXC_WARN(rxc, "%s buffer allocation err\n",
				 cxip_ptelist_to_str(pool));
			break;
		}

		RXC_DBG(rxc, "Link %s buf entry %p\n",
			cxip_ptelist_to_str(pool), buf);

		ret = cxip_ptelist_link_buf(buf, !bufs_added);
		if (ret) {
			RXC_WARN(rxc, "%s append failure %d %s\n",
				 cxip_ptelist_to_str(pool), ret,
				 fi_strerror(-ret));

			dlist_insert_tail(&buf->buf_entry,
					  &pool->free_bufs);
			ofi_atomic_inc32(&pool->bufs_free);
			break;
		}
		bufs_added++;
	}

	/* If no buffer appended, check for fatal conditions. */
	if (!bufs_added) {
		if (ofi_atomic_get32(&pool->bufs_linked) < 1)
			RXC_FATAL(rxc, "%s buffer list exhausted\n",
				  cxip_ptelist_to_str(pool));
	}

	RXC_DBG(rxc, "%s current bufs alloc %u, num linked %u\n",
		cxip_ptelist_to_str(pool),
		ofi_atomic_get32(&pool->bufs_allocated),
		ofi_atomic_get32(&pool->bufs_linked));

	return ret;
}

void cxip_ptelist_buf_get(struct cxip_ptelist_buf *buf)
{
	ofi_atomic_inc32(&buf->refcount);

	RXC_DBG(buf->rxc, "%s GET buf %p refcnt %u\n",
		cxip_ptelist_to_str(buf->pool),
		buf, ofi_atomic_get32(&buf->refcount));
}

void cxip_ptelist_buf_put(struct cxip_ptelist_buf *buf, bool repost)
{
	int ret;
	int refcount = ofi_atomic_dec32(&buf->refcount);

	RXC_DBG(buf->rxc, "%s PUT buf %p refcnt %u repost %d\n",
		cxip_ptelist_to_str(buf->pool), buf, refcount, repost);

	if (refcount < 0) {
		RXC_FATAL(buf->rxc, "%s buffer refcount underflow %d\n",
			  cxip_ptelist_to_str(buf->pool), refcount);
		/* not needed */
		return;
	}

	if (refcount == 0 && repost) {

		/* Overflow buffers should just be freed if no longer
		 * in hardware RX match mode.
		 */
		if (buf->pool->attr.list_type == C_PTL_LIST_OVERFLOW &&
		    (!buf->rxc->base.msg_offload ||
		     buf->rxc->base.state != RXC_ENABLED))
			goto free_buf;

		if (buf->pool->attr.list_type == C_PTL_LIST_REQUEST &&
		    buf->rxc->base.state != RXC_ENABLED_SOFTWARE)
			goto skip_repost;

		/* Limit immediate repost if already sufficient */
		if (ofi_atomic_get32(&buf->pool->bufs_linked) <
		    buf->pool->attr.max_posted) {

			do {
				ret = cxip_ptelist_link_buf(buf, false);
			} while (ret == -FI_EAGAIN);

			if (ret != FI_SUCCESS)
				RXC_FATAL(buf->rxc,
					  "Fatal %s buf link err %d %s",
					  cxip_ptelist_to_str(buf->pool),
					  ret, fi_strerror(-ret));

			return;
		}

skip_repost:
		/* To avoid thrashing on buffer allocation, cache
		 * free buffers until a sufficient number are kept
		 * for reuse. This will help bursty traffic from
		 * holding on to unnecessary buffers.
		 */
		if (!buf->pool->attr.max_cached ||
		    (ofi_atomic_get32(&buf->pool->bufs_linked) +
		     ofi_atomic_get32(&buf->pool->bufs_free) <
		     buf->pool->attr.max_cached)) {

			dlist_remove(&buf->buf_entry);
			dlist_insert_tail(&buf->buf_entry,
					&buf->pool->free_bufs);
			ofi_atomic_inc32(&buf->pool->bufs_free);

			return;
		}

free_buf:
		dlist_remove_init(&buf->buf_entry);
		cxip_ptelist_buf_free(buf);
	}
}

void cxip_ptelist_buf_consumed(struct cxip_ptelist_buf *buf)
{
	RXC_DBG(buf->rxc, "%s CONSUMED off %ld len %ld buf %p\n",
		cxip_ptelist_to_str(buf->pool), buf->cur_offset,
		buf->unlink_length, buf);

	dlist_remove(&buf->buf_entry);
	dlist_insert_tail(&buf->buf_entry,
			  &buf->pool->consumed_bufs);

	/* Since buffer is consumed, return reference
	 * taken during the initial linking.
	 */
	cxip_ptelist_buf_put(buf, true);
}

void _cxip_req_buf_ux_free(struct cxip_ux_send *ux, bool repost)
{
	struct cxip_ptelist_buf *buf = ux->req->req_ctx;

	assert(ux->req->type == CXIP_REQ_RBUF);

	cxip_ptelist_buf_put(buf, repost);
	free(ux);

	RXC_DBG(buf->rxc, "%s buf %p ux %p\n",
		cxip_ptelist_to_str(buf->pool), buf, ux);
}
