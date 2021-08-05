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

/*
 * rxr_read_prepare_pkt_entry_mr() ensure pkt_entry's memory is registered.
 *
 * For a packet entry whose memory is not registered, it will reserve a pkt entry
 * from rx_readcopy_pkt_pool and copy data their.
 *
 * Return value:
 *
 *     On success, return 0
 *     On pack entry reservation failure, return -FI_EAGAIN
 */
static
ssize_t rxr_read_prepare_pkt_entry_mr(struct rxr_ep *ep, struct rxr_read_entry *read_entry)
{
	size_t pkt_offset;
	struct rxr_pkt_entry *pkt_entry;
	struct rxr_pkt_entry *pkt_entry_copy;

	assert(read_entry->context_type == RXR_READ_CONTEXT_PKT_ENTRY);
	/*
	 * In this case, target buffer is data in a pkt_entry, so rma_iov_count must be 1.
	 */
	assert(read_entry->rma_iov_count == 1);

	pkt_entry = read_entry->context;
	if (pkt_entry->mr) {
		assert(read_entry->rma_iov[0].key == fi_mr_key(pkt_entry->mr));
		return 0;
	}

	/* only ooo and unexp packet entry's memory is not registered with device */
	assert(pkt_entry->type == RXR_PKT_ENTRY_OOO ||
	       pkt_entry->type == RXR_PKT_ENTRY_UNEXP);

	pkt_offset = (char *)read_entry->rma_iov[0].addr - (char *)pkt_entry->pkt;
	assert(pkt_offset > sizeof(struct rxr_base_hdr));

	pkt_entry_copy = rxr_pkt_entry_clone(ep, ep->rx_readcopy_pkt_pool,
					     pkt_entry, RXR_PKT_ENTRY_READ_COPY);
	if (!pkt_entry_copy) {
		FI_WARN(&rxr_prov, FI_LOG_CQ,
			"readcopy pkt pool exhausted! Set FI_EFA_READCOPY_POOL_SIZE to a higher value!");
		return -FI_EAGAIN;
	}

	rxr_pkt_entry_release_rx(ep, pkt_entry);

	assert(pkt_entry_copy->mr);
	read_entry->context = pkt_entry_copy;
	read_entry->rma_iov[0].addr = (uint64_t)pkt_entry_copy->pkt + pkt_offset;
	read_entry->rma_iov[0].key = fi_mr_key(pkt_entry_copy->mr);

	return 0;
}

/*
 * rxr_read_mr_reg register the memory of local buffer if application did not
 * provide descriptor.
 * It is called by rxr_read_post().
 * On success, it return 0.
 * If memory registration failed with -FI_ENOMEM, it will return -FI_EAGAIN.
 * If memory registration failed with other error, it will return the error code.
 */
ssize_t rxr_read_mr_reg(struct rxr_ep *ep, struct rxr_read_entry *read_entry)
{
	size_t i;
	int err;

	for (i = 0; i < read_entry->iov_count; ++i) {
		if (read_entry->mr_desc[i] || read_entry->mr[i]) {
			continue;
		}

		err = fi_mr_reg(rxr_ep_domain(ep)->rdm_domain,
				read_entry->iov[i].iov_base, read_entry->iov[i].iov_len,
				FI_RECV, 0, 0, 0, &read_entry->mr[i], NULL);

		if (err) {
			/* If registration failed with -FI_ENOMEM, we return -FI_EAGAIN.
			 * This read entry will be put into a queue.
			 *
			 * The progress engine will progress other message transfers, which
			 * will release registrations. Thus, when the progress engine call this
			 * function again later, there will be registrations available.
			 *
			 * All registration opened here will be closed during release of
			 * the read_entry.
			 */
			FI_WARN(&rxr_prov, FI_LOG_MR, "Unable to register MR buf for read!\n");
			if (err == -FI_ENOMEM)
				err = -FI_EAGAIN;
			return err;
		}

		read_entry->mr_desc[i] = fi_mr_desc(read_entry->mr[i]);
	}

	return 0;
}

/* rxr_read_alloc_entry allocates a read entry.
 * It is called by rxr_read_post_or_queue().
 * Input:
 *   x_entry: can be a tx_entry or an rx_entry.
 *            If x_entry is tx_entry, application called fi_read().
 *            If x_entry is rx_entry, read message protocol is being used.
 *   lower_ep_type: EFA_EP or SHM_EP
 * Return:
 *   On success, return the pointer of allocated read_entry
 *   Otherwise, return NULL
 */
struct rxr_read_entry *rxr_read_alloc_entry(struct rxr_ep *ep, int entry_type, void *x_entry,
					    enum rxr_lower_ep_type lower_ep_type)
{
	struct rxr_tx_entry *tx_entry = NULL;
	struct rxr_rx_entry *rx_entry = NULL;
	struct rxr_read_entry *read_entry;
	int i;
	size_t total_iov_len, total_rma_iov_len;

	read_entry = ofi_buf_alloc(ep->read_entry_pool);
	if (OFI_UNLIKELY(!read_entry)) {
		FI_WARN(&rxr_prov, FI_LOG_EP_CTRL, "RDMA entries exhausted\n");
		return NULL;
	}

	read_entry->type = RXR_READ_ENTRY;
	read_entry->read_id = ofi_buf_index(read_entry);
	read_entry->state = RXR_RDMA_ENTRY_CREATED;

	if (entry_type == RXR_TX_ENTRY) {
		tx_entry = (struct rxr_tx_entry *)x_entry;
		assert(tx_entry->op == ofi_op_read_req);

		read_entry->context_type = RXR_READ_CONTEXT_TX_ENTRY;
		read_entry->context = tx_entry;
		read_entry->addr = tx_entry->addr;

		read_entry->iov_count = tx_entry->iov_count;
		memcpy(read_entry->iov, tx_entry->iov,
		       tx_entry->iov_count * sizeof(struct iovec));

		read_entry->rma_iov_count = tx_entry->rma_iov_count;
		memcpy(read_entry->rma_iov, tx_entry->rma_iov,
		       tx_entry->rma_iov_count * sizeof(struct fi_rma_iov));

		total_iov_len = ofi_total_iov_len(tx_entry->iov, tx_entry->iov_count);
		total_rma_iov_len = ofi_total_rma_iov_len(tx_entry->rma_iov, tx_entry->rma_iov_count);
		read_entry->total_len = MIN(total_iov_len, total_rma_iov_len);

		if (tx_entry->desc) {
			memcpy(read_entry->mr_desc, tx_entry->desc,
			       read_entry->iov_count * sizeof(void *));
		}

	} else {
		rx_entry = (struct rxr_rx_entry *)x_entry;
		assert(rx_entry->op == ofi_op_write || rx_entry->op == ofi_op_msg ||
		       rx_entry->op == ofi_op_tagged);

		read_entry->context_type = RXR_READ_CONTEXT_RX_ENTRY;
		read_entry->context = rx_entry;
		read_entry->addr = rx_entry->addr;

		read_entry->iov_count = rx_entry->iov_count;
		memcpy(read_entry->iov, rx_entry->iov,
		       rx_entry->iov_count * sizeof(struct iovec));

		read_entry->rma_iov_count = rx_entry->rma_iov_count;
		memcpy(read_entry->rma_iov, rx_entry->rma_iov,
		       rx_entry->rma_iov_count * sizeof(struct fi_rma_iov));

		total_iov_len = ofi_total_iov_len(rx_entry->iov, rx_entry->iov_count);
		total_rma_iov_len = ofi_total_rma_iov_len(rx_entry->rma_iov, rx_entry->rma_iov_count);
		read_entry->total_len = MIN(total_iov_len, total_rma_iov_len);

		if (rx_entry->desc) {
			memcpy(read_entry->mr_desc, rx_entry->desc,
			       read_entry->iov_count * sizeof(void *));
		}
	}

	memset(read_entry->mr, 0, read_entry->iov_count * sizeof(struct fid_mr *));

	if (lower_ep_type == SHM_EP) {
		assert(lower_ep_type == SHM_EP);
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

static inline
int rxr_read_post_or_queue(struct rxr_ep *ep, struct rxr_read_entry *read_entry)
{
	int err;

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

int rxr_read_post_remote_read_or_queue(struct rxr_ep *ep, int entry_type, void *x_entry)
{
	struct rxr_peer *peer;
	struct rxr_read_entry *read_entry;
	int lower_ep_type;

	if (entry_type == RXR_TX_ENTRY) {
		peer = rxr_ep_get_peer(ep, ((struct rxr_tx_entry *)x_entry)->addr);
	} else {
		assert(entry_type == RXR_RX_ENTRY);
		peer = rxr_ep_get_peer(ep, ((struct rxr_rx_entry *)x_entry)->addr);
	}

	lower_ep_type = (peer->is_local) ? SHM_EP : EFA_EP;
	read_entry = rxr_read_alloc_entry(ep, entry_type, x_entry, lower_ep_type);
	if (!read_entry) {
		FI_WARN(&rxr_prov, FI_LOG_CQ,
			"RDMA entries exhausted.\n");
		return -FI_ENOBUFS;
	}

	return rxr_read_post_or_queue(ep, read_entry);
}

int rxr_read_post_local_read_or_queue(struct rxr_ep *ep,
				      struct rxr_rx_entry *rx_entry,
				      size_t data_offset,
				      struct rxr_pkt_entry *pkt_entry,
				      char *data, size_t data_size)
{
	int err;
	struct rxr_read_entry *read_entry;

	read_entry = ofi_buf_alloc(ep->read_entry_pool);
	if (!read_entry) {
		FI_WARN(&rxr_prov, FI_LOG_EP_CTRL, "RDMA entries exhausted\n");
		return -FI_ENOBUFS;
	}

	read_entry->read_id = ofi_buf_index(read_entry);
	read_entry->lower_ep_type = EFA_EP;
	read_entry->context_type = RXR_READ_CONTEXT_PKT_ENTRY;
	read_entry->context = pkt_entry;
	read_entry->state = RXR_RDMA_ENTRY_CREATED;
	read_entry->addr = FI_ADDR_NOTAVAIL;
	read_entry->total_len = data_size;
	read_entry->bytes_submitted = 0;
	read_entry->bytes_finished = 0;

	/* setup rma_iov */
	read_entry->rma_iov_count = 1;
	read_entry->rma_iov[0].addr = (uint64_t)data;
	read_entry->rma_iov[0].len = data_size;
	read_entry->rma_iov[0].key = (pkt_entry->mr) ? fi_mr_key(pkt_entry->mr) : 0;

	/* setup iov */
	assert(pkt_entry->x_entry == rx_entry);
	assert(rx_entry->desc && efa_ep_is_cuda_mr(rx_entry->desc[0]));
	read_entry->iov_count = rx_entry->iov_count;
	memcpy(read_entry->iov, rx_entry->iov, rx_entry->iov_count * sizeof(struct iovec));
	memcpy(read_entry->mr_desc, rx_entry->desc, rx_entry->iov_count * sizeof(void *));
	ofi_consume_iov_desc(read_entry->iov, read_entry->mr_desc, &read_entry->iov_count, data_offset);
	if (read_entry->iov_count == 0) {
		FI_WARN(&rxr_prov, FI_LOG_CQ,
			"data_offset %ld out of range\n",
			data_offset);
		ofi_buf_free(read_entry);
		return -FI_ETRUNC;
	}

	assert(efa_ep_is_cuda_mr(read_entry->mr_desc[0]));
	err = ofi_truncate_iov(read_entry->iov, &read_entry->iov_count, data_size);
	if (err) {
		FI_WARN(&rxr_prov, FI_LOG_CQ,
			"data_offset %ld data_size %ld out of range\n",
			data_offset, data_size);
		ofi_buf_free(read_entry);
		return -FI_ETRUNC;
	}

	return rxr_read_post_or_queue(ep, read_entry);
}

int rxr_read_init_iov(struct rxr_ep *ep,
		      struct rxr_tx_entry *tx_entry,
		      struct fi_rma_iov *read_iov)
{
	int i, err;
	struct fid_mr *mr;
	struct rxr_peer *peer;

	peer = rxr_ep_get_peer(ep, tx_entry->addr);

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
	bool self_comm;
	size_t iov_offset = 0, rma_iov_offset = 0;
	size_t total_iov_len, total_rma_iov_len, max_read_size;
	struct rxr_pkt_entry *pkt_entry;
	struct iovec iov;
	struct fi_rma_iov rma_iov;
	struct fi_msg_rma msg;
	struct efa_ep *efa_ep;
	struct rxr_peer *peer;
	fi_addr_t shm_fiaddr = FI_ADDR_NOTAVAIL;

	assert(read_entry->iov_count > 0);
	assert(read_entry->rma_iov_count > 0);
	assert(read_entry->bytes_submitted < read_entry->total_len);

	if (read_entry->context_type == RXR_READ_CONTEXT_PKT_ENTRY) {
		assert(read_entry->lower_ep_type == EFA_EP);
		ret = rxr_read_prepare_pkt_entry_mr(ep, read_entry);
		if (ret)
			return ret;
	}

	if (read_entry->lower_ep_type == EFA_EP) {
		ret = rxr_read_mr_reg(ep, read_entry);
		if (ret)
			return ret;
	}

	peer = rxr_ep_get_peer(ep, read_entry->addr);

	if (read_entry->lower_ep_type == SHM_EP)
		shm_fiaddr = peer->shm_fiaddr;

	max_read_size = (read_entry->lower_ep_type == EFA_EP) ?
				efa_max_rdma_size(ep->rdm_ep) : SIZE_MAX;
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

		if (ep->tx_pending == ep->max_outstanding_tx)
			return -FI_EAGAIN;

		assert(iov_idx < read_entry->iov_count);
		assert(iov_offset < read_entry->iov[iov_idx].iov_len);
		assert(rma_iov_idx < read_entry->rma_iov_count);
		assert(rma_iov_offset < read_entry->rma_iov[rma_iov_idx].len);

		iov.iov_base = (char *)read_entry->iov[iov_idx].iov_base + iov_offset;
		iov.iov_len = read_entry->iov[iov_idx].iov_len - iov_offset;

		rma_iov.addr = (uintptr_t)read_entry->rma_iov[rma_iov_idx].addr + rma_iov_offset;
		rma_iov.len = read_entry->rma_iov[rma_iov_idx].len - rma_iov_offset;
		rma_iov.key = read_entry->rma_iov[rma_iov_idx].key;

		iov.iov_len = MIN(iov.iov_len, rma_iov.len);
		if (read_entry->lower_ep_type == EFA_EP)
			iov.iov_len = MIN(iov.iov_len, rxr_env.efa_read_segment_size);
		iov.iov_len = MIN(iov.iov_len, max_read_size);
		rma_iov.len = iov.iov_len;

		/* because fi_send uses a pkt_entry as context
		 * we had to use a pkt_entry as context too
		 */
		if (read_entry->lower_ep_type == SHM_EP)
			pkt_entry = rxr_pkt_entry_alloc(ep, ep->tx_pkt_shm_pool);
		else
			pkt_entry = rxr_pkt_entry_alloc(ep, ep->tx_pkt_efa_pool);

		if (OFI_UNLIKELY(!pkt_entry))
			return -FI_EAGAIN;

		rxr_pkt_init_read_context(ep, read_entry, iov.iov_len, pkt_entry);

		memset(&msg, 0, sizeof(msg));
		msg.msg_iov = &iov;
		msg.desc = &read_entry->mr_desc[iov_idx];
		msg.iov_count = 1;
		msg.rma_iov = &rma_iov;
		msg.rma_iov_count = 1;
		msg.context = pkt_entry;

		if (read_entry->lower_ep_type == SHM_EP) {
			msg.addr = shm_fiaddr;
			ret = fi_readmsg(ep->shm_ep, &msg, 0);
		} else {
			efa_ep = container_of(ep->rdm_ep, struct efa_ep, util_ep.ep_fid);
			msg.addr = read_entry->addr;
			self_comm = (read_entry->context_type == RXR_READ_CONTEXT_PKT_ENTRY);
			ret = efa_rma_post_read(efa_ep, &msg, 0, self_comm);
		}

		if (OFI_UNLIKELY(ret)) {
			rxr_pkt_entry_release_tx(ep, pkt_entry);
			return ret;
		}

		if (read_entry->context_type == RXR_READ_CONTEXT_PKT_ENTRY) {
			assert(read_entry->lower_ep_type == EFA_EP);
			/* read from self, no peer */
			ep->tx_pending++;
		} else if (read_entry->lower_ep_type == EFA_EP) {
			rxr_ep_inc_tx_pending(ep, peer);
		}

		read_entry->bytes_submitted += iov.iov_len;

		iov_offset += iov.iov_len;
		assert(iov_offset <= read_entry->iov[iov_idx].iov_len);
		if (iov_offset == read_entry->iov[iov_idx].iov_len) {
			iov_idx += 1;
			iov_offset = 0;
		}

		rma_iov_offset += rma_iov.len;
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

	if (read_entry->context_type == RXR_READ_CONTEXT_TX_ENTRY) {
		tx_entry = read_entry->context;
		ret = rxr_cq_handle_tx_error(ep, tx_entry, ret);
	} else {
		assert(read_entry->context_type == RXR_READ_CONTEXT_RX_ENTRY);
		rx_entry = read_entry->context;
		ret = rxr_cq_handle_rx_error(ep, rx_entry, ret);
	}

	if (read_entry->state == RXR_RDMA_ENTRY_PENDING)
		dlist_remove(&read_entry->pending_entry);
	return ret;
}

