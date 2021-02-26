/*
 * Copyright (c) 2019-2020 Amazon.com, Inc. or its affiliates.
 * All rights reserved.
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

#include "efa.h"
#include "rxr.h"
#include "rxr_rma.h"
#include "rxr_cntr.h"
#include "rxr_read.h"

int rxr_locate_iov_pos(struct iovec *iov, int iov_count, size_t offset,
		       int *iov_idx, size_t *iov_offset)
{
	int i;
	size_t curoffset;

	curoffset = 0;
	for (i = 0; i < iov_count; ++i) {
		if (offset >= curoffset &&
		    offset < curoffset + iov[i].iov_len) {
			*iov_idx = i;
			*iov_offset = offset - curoffset;
			return 0;
		}

		curoffset += iov[i].iov_len;
	}

	return -1;
}

int rxr_locate_rma_iov_pos(struct fi_rma_iov *rma_iov, int rma_iov_count, size_t offset,
			   int *rma_iov_idx, size_t *rma_iov_offset)
{
	int i;
	size_t curoffset;

	curoffset = 0;
	for (i = 0; i < rma_iov_count; ++i) {
		if (offset >= curoffset &&
		    offset < curoffset + rma_iov[i].len) {
			*rma_iov_idx = i;
			*rma_iov_offset = offset - curoffset;
			return 0;
		}

		curoffset += rma_iov[i].len;
	}

	return -1;
}

struct rxr_read_entry *rxr_read_alloc_entry(struct rxr_ep *ep, int entry_type, void *x_entry,
					    enum rxr_lower_ep_type lower_ep_type)
{
	struct rxr_tx_entry *tx_entry = NULL;
	struct rxr_rx_entry *rx_entry = NULL;
	struct rxr_read_entry *read_entry;
	int i, err;
	size_t total_iov_len, total_rma_iov_len;
	void **mr_desc;

	read_entry = ofi_buf_alloc(ep->read_entry_pool);
	if (OFI_UNLIKELY(!read_entry)) {
		FI_WARN(&rxr_prov, FI_LOG_EP_CTRL, "RDMA entries exhausted\n");
		return NULL;
	}

	read_entry->read_id = ofi_buf_index(read_entry);
	read_entry->state = RXR_RDMA_ENTRY_CREATED;
	read_entry->x_entry_type = entry_type;

	if (entry_type == RXR_TX_ENTRY) {
		tx_entry = (struct rxr_tx_entry *)x_entry;
		assert(tx_entry->op == ofi_op_read_req);
		read_entry->x_entry_id = tx_entry->tx_id;
		read_entry->addr = tx_entry->addr;

		read_entry->iov_count = tx_entry->iov_count;
		read_entry->iov = tx_entry->iov;

		read_entry->rma_iov_count = tx_entry->rma_iov_count;
		read_entry->rma_iov = tx_entry->rma_iov;

		total_iov_len = ofi_total_iov_len(tx_entry->iov, tx_entry->iov_count);
		total_rma_iov_len = ofi_total_rma_iov_len(tx_entry->rma_iov, tx_entry->rma_iov_count);
		read_entry->total_len = MIN(total_iov_len, total_rma_iov_len);
		mr_desc = tx_entry->desc;
	} else {
		rx_entry = (struct rxr_rx_entry *)x_entry;
		assert(rx_entry->op == ofi_op_write || rx_entry->op == ofi_op_msg ||
		       rx_entry->op == ofi_op_tagged);

		read_entry->x_entry_id = rx_entry->rx_id;
		read_entry->addr = rx_entry->addr;

		read_entry->iov_count = rx_entry->iov_count;
		read_entry->iov = rx_entry->iov;

		read_entry->rma_iov_count = rx_entry->rma_iov_count;
		read_entry->rma_iov = rx_entry->rma_iov;

		mr_desc = rx_entry->desc;
		total_iov_len = ofi_total_iov_len(rx_entry->iov, rx_entry->iov_count);
		total_rma_iov_len = ofi_total_rma_iov_len(rx_entry->rma_iov, rx_entry->rma_iov_count);
		read_entry->total_len = MIN(total_iov_len, total_rma_iov_len);
	}

	if (lower_ep_type == EFA_EP) {
		/* EFA provider need local buffer registration */
		for (i = 0; i < read_entry->iov_count; ++i) {
			if (mr_desc && mr_desc[i]) {
				read_entry->mr[i] = NULL;
				read_entry->mr_desc[i] = mr_desc[i];
			} else {
				err = fi_mr_reg(rxr_ep_domain(ep)->rdm_domain,
						read_entry->iov[i].iov_base, read_entry->iov[i].iov_len,
						FI_RECV, 0, 0, 0, &read_entry->mr[i], NULL);

				if (err == -FI_ENOMEM && efa_mr_cache_enable) {
					/* In this case, we will try registration one more time because
					 * mr cache will try to release MR when encountered error
					 */
					FI_WARN(&rxr_prov, FI_LOG_MR, "Unable to register MR buf for FI_ENOMEM!\n");
					FI_WARN(&rxr_prov, FI_LOG_MR, "Try again because MR cache will try release to release unused MR entry.\n");
					err = fi_mr_reg(rxr_ep_domain(ep)->rdm_domain,
							read_entry->iov[i].iov_base, read_entry->iov[i].iov_len,
							FI_RECV, 0, 0, 0, &read_entry->mr[i], NULL);
					if (!err)
						FI_WARN(&rxr_prov, FI_LOG_MR, "The 2nd attemp was successful!");
				}

				if (err) {
					FI_WARN(&rxr_prov, FI_LOG_MR, "Unable to register MR buf\n");
					return NULL;
				}

				read_entry->mr_desc[i] = fi_mr_desc(read_entry->mr[i]);
			}
		}
	} else {
		assert(lower_ep_type == SHM_EP);
		memset(read_entry->mr, 0, read_entry->iov_count * sizeof(struct fid_mr *));
		/* FI_MR_VIRT_ADDR is not being set, use 0-based offset instead. */
		if (!(shm_info->domain_attr->mr_mode & FI_MR_VIRT_ADDR)) {
			for (i = 0; i < read_entry->rma_iov_count; ++i)
				read_entry->rma_iov[i].addr = 0;
		}
	}

	read_entry->lower_ep_type = lower_ep_type;
	read_entry->bytes_submitted = 0;
	read_entry->bytes_finished = 0;
	return read_entry;
}

void rxr_read_release_entry(struct rxr_ep *ep, struct rxr_read_entry *read_entry)
{
	int i, err;

	for (i = 0; i < read_entry->iov_count; ++i) {
		if (read_entry->mr[i]) {
			err = fi_close((struct fid *)read_entry->mr[i]);
			if (err) {
				FI_WARN(&rxr_prov, FI_LOG_MR, "Unable to close mr\n");
				rxr_read_handle_error(ep, read_entry, err);
			}
		}
	}

#ifdef ENABLE_EFA_POISONING
	rxr_poison_mem_region((uint32_t *)read_entry, sizeof(struct rxr_read_entry));
#endif
	read_entry->state = RXR_RDMA_ENTRY_FREE;
	ofi_buf_free(read_entry);
}

int rxr_read_post_or_queue(struct rxr_ep *ep, int entry_type, void *x_entry)
{
	struct rxr_peer *peer;
	struct rxr_read_entry *read_entry;
	int err, lower_ep_type;

	if (entry_type == RXR_TX_ENTRY) {
		peer = rxr_ep_get_peer(ep, ((struct rxr_tx_entry *)x_entry)->addr);
	} else {
		assert(entry_type == RXR_RX_ENTRY);
		peer = rxr_ep_get_peer(ep, ((struct rxr_rx_entry *)x_entry)->addr);
	}

	assert(peer);
	lower_ep_type = (peer->is_local) ? SHM_EP : EFA_EP;
	read_entry = rxr_read_alloc_entry(ep, entry_type, x_entry, lower_ep_type);
	if (!read_entry) {
		FI_WARN(&rxr_prov, FI_LOG_CQ,
			"RDMA entries exhausted.\n");
		return -FI_ENOBUFS;
	}

	err = rxr_read_post(ep, read_entry);
	if (err == -FI_EAGAIN) {
		dlist_insert_tail(&read_entry->pending_entry, &ep->read_pending_list);
		read_entry->state = RXR_RDMA_ENTRY_PENDING;
		err = 0;
	} else if(err) {
		rxr_read_release_entry(ep, read_entry);
		FI_WARN(&rxr_prov, FI_LOG_CQ,
			"RDMA post read failed. errno=%d.\n", err);
	}

	return err;
}

int rxr_read_init_iov(struct rxr_ep *ep,
		      struct rxr_tx_entry *tx_entry,
		      struct fi_rma_iov *read_iov)
{
	int i, err;
	struct fid_mr *mr;
	struct rxr_peer *peer;

	peer = rxr_ep_get_peer(ep, tx_entry->addr);
	assert(peer);

	for (i = 0; i < tx_entry->iov_count; ++i) {
		read_iov[i].addr = (uint64_t)tx_entry->iov[i].iov_base;
		read_iov[i].len = tx_entry->iov[i].iov_len;
	}

	if (tx_entry->desc[0]) {
		for (i = 0; i < tx_entry->iov_count; ++i) {
			mr = (struct fid_mr *)tx_entry->desc[i];
			read_iov[i].key = fi_mr_key(mr);
		}
	} else {
		/* note mr could be been set by an unsucessful rxr_ep_post_ctrl */
		if (!tx_entry->mr[0]) {
			for (i = 0; i < tx_entry->iov_count; ++i) {
				assert(!tx_entry->mr[i]);

				if (peer->is_local)
					err = efa_mr_reg_shm(rxr_ep_domain(ep)->rdm_domain,
							     tx_entry->iov + i,
							     FI_REMOTE_READ, &tx_entry->mr[i]);
				else
					err = fi_mr_regv(rxr_ep_domain(ep)->rdm_domain,
							 tx_entry->iov + i, 1,
							 FI_REMOTE_READ,
							 0, 0, 0, &tx_entry->mr[i], NULL);
				if (err) {
					FI_WARN(&rxr_prov, FI_LOG_MR,
						"Unable to register MR buf %p as FI_REMOTE_READ",
						tx_entry->iov[i].iov_base);
					return err;
				}
			}
		}

		for (i = 0; i < tx_entry->iov_count; ++i) {
			assert(tx_entry->mr[i]);
			read_iov[i].key = fi_mr_key(tx_entry->mr[i]);
		}
	}

	return 0;
}

int rxr_read_post(struct rxr_ep *ep, struct rxr_read_entry *read_entry)
{
	int ret;
	int iov_idx = 0, rma_iov_idx = 0;
	void *iov_ptr, *rma_iov_ptr;
	struct rxr_peer *peer;
	struct rxr_pkt_entry *pkt_entry;
	size_t iov_offset = 0, rma_iov_offset = 0;
	size_t total_iov_len, total_rma_iov_len;
	size_t segsize, max_iov_segsize, max_rma_iov_segsize, max_read_size;
	struct fid_ep *lower_ep;
	fi_addr_t lower_ep_addr;

	assert(read_entry->iov_count > 0);
	assert(read_entry->rma_iov_count > 0);
	assert(read_entry->bytes_submitted < read_entry->total_len);

	peer = rxr_ep_get_peer(ep, read_entry->addr);
	if (read_entry->lower_ep_type == EFA_EP) {
		max_read_size = efa_max_rdma_size(ep->rdm_ep);
		lower_ep = ep->rdm_ep;
		lower_ep_addr = read_entry->addr;
	} else {
		max_read_size = SIZE_MAX;
		lower_ep = ep->shm_ep;
		lower_ep_addr = peer->shm_fiaddr;
	}
	assert(max_read_size > 0);

	ret = rxr_locate_iov_pos(read_entry->iov, read_entry->iov_count,
				 read_entry->bytes_submitted,
				 &iov_idx, &iov_offset);
	assert(ret == 0);

	ret = rxr_locate_rma_iov_pos(read_entry->rma_iov, read_entry->rma_iov_count,
				     read_entry->bytes_submitted,
				     &rma_iov_idx, &rma_iov_offset);
	assert(ret == 0);

	total_iov_len = ofi_total_iov_len(read_entry->iov, read_entry->iov_count);
	total_rma_iov_len = ofi_total_rma_iov_len(read_entry->rma_iov, read_entry->rma_iov_count);
	assert(read_entry->total_len == MIN(total_iov_len, total_rma_iov_len));

	while (read_entry->bytes_submitted < read_entry->total_len) {
		assert(iov_idx < read_entry->iov_count);
		assert(iov_offset < read_entry->iov[iov_idx].iov_len);
		assert(rma_iov_idx < read_entry->rma_iov_count);
		assert(rma_iov_offset < read_entry->rma_iov[rma_iov_idx].len);

		iov_ptr = (char *)read_entry->iov[iov_idx].iov_base + iov_offset;
		rma_iov_ptr = (char *)read_entry->rma_iov[rma_iov_idx].addr + rma_iov_offset;

		max_iov_segsize = read_entry->iov[iov_idx].iov_len - iov_offset;
		max_rma_iov_segsize = read_entry->rma_iov[rma_iov_idx].len - rma_iov_offset;
		segsize = MIN(max_iov_segsize, max_rma_iov_segsize);
		if (read_entry->lower_ep_type == EFA_EP)
			segsize = MIN(segsize, rxr_env.efa_read_segment_size);
		segsize = MIN(segsize, max_read_size);

		/* because fi_send uses a pkt_entry as context
		 * we had to use a pkt_entry as context too
		 */
		if (read_entry->lower_ep_type == SHM_EP)
			pkt_entry = rxr_pkt_entry_alloc(ep, ep->tx_pkt_shm_pool);
		else
			pkt_entry = rxr_pkt_entry_alloc(ep, ep->tx_pkt_efa_pool);

		if (OFI_UNLIKELY(!pkt_entry))
			return -FI_EAGAIN;

		rxr_pkt_init_read_context(ep, read_entry, segsize, pkt_entry);

		ret = fi_read(lower_ep,
			      iov_ptr, segsize, read_entry->mr_desc[iov_idx],
			      lower_ep_addr,
			      (uint64_t)rma_iov_ptr, read_entry->rma_iov[rma_iov_idx].key,
			      pkt_entry);

		if (OFI_UNLIKELY(ret)) {
			rxr_pkt_entry_release_tx(ep, pkt_entry);
			return ret;
		}

		if (!peer->is_local)
			rxr_ep_inc_tx_pending(ep, peer);
		read_entry->bytes_submitted += segsize;

		iov_offset += segsize;
		assert(iov_offset <= read_entry->iov[iov_idx].iov_len);
		if (iov_offset == read_entry->iov[iov_idx].iov_len) {
			iov_idx += 1;
			iov_offset = 0;
		}

		rma_iov_offset += segsize;
		assert(rma_iov_offset <= read_entry->rma_iov[rma_iov_idx].len);
		if (rma_iov_offset == read_entry->rma_iov[rma_iov_idx].len) {
			rma_iov_idx += 1;
			rma_iov_offset = 0;
		}
	}

	if (read_entry->total_len == total_iov_len) {
		assert(iov_idx == read_entry->iov_count);
		assert(iov_offset == 0);
	}

	if (read_entry->total_len == total_rma_iov_len) {
		assert(rma_iov_idx == read_entry->rma_iov_count);
		assert(rma_iov_offset == 0);
	}

	return 0;
}

int rxr_read_handle_error(struct rxr_ep *ep, struct rxr_read_entry *read_entry, int ret)
{
	struct rxr_tx_entry *tx_entry;
	struct rxr_rx_entry *rx_entry;

	if (read_entry->x_entry_type == RXR_TX_ENTRY) {
		tx_entry = ofi_bufpool_get_ibuf(ep->tx_entry_pool, read_entry->x_entry_id);
		ret = rxr_cq_handle_tx_error(ep, tx_entry, ret);
	} else {
		assert(read_entry->x_entry_type == RXR_RX_ENTRY);
		rx_entry = ofi_bufpool_get_ibuf(ep->rx_entry_pool, read_entry->x_entry_id);
		ret = rxr_cq_handle_rx_error(ep, rx_entry, ret);
	}

	dlist_remove(&read_entry->pending_entry);
	return ret;
}

