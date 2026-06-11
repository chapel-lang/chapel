/*
 * Copyright (c) Intel Corporation. All rights reserved
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

#include "smr.h"
#include "smr_dsa.h"
#include "ofi_atomic.h"
#include "ofi_mb.h"

static void smr_progress_overflow(struct smr_ep *ep)
{
	struct smr_cmd_entry *ce;
	struct smr_region *peer_smr;
	struct smr_cmd *cmd;
	int64_t pos;
	struct slist_entry *entry;
	int ret;

	entry = ep->overflow_list.head;
	while (entry) {
		cmd = (struct smr_cmd *) entry;
		peer_smr = smr_peer_region(ep, cmd->hdr.tx_id);
		ret = smr_cmd_queue_next(smr_cmd_queue(peer_smr), &ce, &pos);
		if (ret == -FI_ENOENT)
			return;

		ce->ptr = smr_local_to_peer(ep, peer_smr, cmd->hdr.tx_id,
					    cmd->hdr.rx_id, (uintptr_t) cmd);

		slist_remove_head(&ep->overflow_list);
		smr_cmd_queue_commit(ce, pos);
		entry = ep->overflow_list.head;
	}
}

void smr_try_send_cmd(struct smr_ep *ep, struct smr_cmd *cmd)
{
	cmd->hdr.entry = 0;
	slist_insert_tail((struct slist_entry *) &cmd->hdr.entry,
			  &ep->overflow_list);
	smr_progress_overflow(ep);
}

void smr_free_sar_bufs(struct smr_ep *ep, struct smr_cmd *cmd,
		       struct smr_pend_entry *pending)
{
	int i;

	for (i = cmd->data.buf_batch_size - 1; i >= 0; i--) {
		smr_freestack_push_by_index(smr_sar_pool(ep->region),
					    cmd->data.sar[i]);
	}
	smr_peer_data(ep->region)[cmd->hdr.tx_id].sar_status = SMR_SAR_FREE;
}

static int smr_progress_return_entry(struct smr_ep *ep, struct smr_cmd *cmd,
				     struct smr_pend_entry *pend)
{
	struct smr_inject_buf *tx_buf = NULL;
	uint8_t *src;
	ssize_t hmem_copy_ret;
	int ret = FI_SUCCESS;

	switch (cmd->hdr.proto) {
	case smr_proto_iov:
		break;
	case smr_proto_ipc:
		assert(pend->mr[0]);
		break;
	case smr_proto_sar:
		if (cmd->hdr.status) {
			smr_free_sar_bufs(ep, cmd, pend);
			return cmd->hdr.status;
		}

		if (cmd->hdr.op == ofi_op_read_req) {
			ret = pend->sar_copy_fn(ep, pend);
			if (ret == -FI_EBUSY)
				return -FI_EAGAIN;

			if (ret && ret != -FI_EBUSY)
				return ret;
			if (pend->bytes_done == cmd->hdr.size) {
				smr_free_sar_bufs(ep, cmd, pend);
				return FI_SUCCESS;
			}
			smr_peer_data(ep->region)[cmd->hdr.tx_id].sar_status =
							SMR_SAR_READY;
			smr_try_send_cmd(ep, cmd);
			return -FI_EAGAIN;
		}

		if (pend->bytes_done == cmd->hdr.size) {
			smr_free_sar_bufs(ep, cmd, pend);
			return FI_SUCCESS;
		}

		ret = pend->sar_copy_fn(ep, pend);
		if (ret && ret != -FI_EBUSY)
			return ret;

		smr_peer_data(ep->region)[cmd->hdr.tx_id].sar_status = ret ?
						SMR_SAR_BUSY : SMR_SAR_READY;
		smr_try_send_cmd(ep, cmd);
		return -FI_EAGAIN;
	case smr_proto_inject:
		tx_buf = smr_get_inject_buf(ep->region, cmd);
		if (pend) {
			if (pend->bytes_done != cmd->hdr.size &&
			    cmd->hdr.op != ofi_op_atomic) {
				src = cmd->hdr.op == ofi_op_atomic_compare ?
					tx_buf->buf : tx_buf->data;
				hmem_copy_ret  = ofi_copy_to_mr_iov(
							pend->mr, pend->iov,
							pend->iov_count,
							0, src, cmd->hdr.size);

				if (hmem_copy_ret < 0) {
					FI_WARN(&smr_prov, FI_LOG_EP_CTRL,
						"RMA read/fetch failed "
						"with code %d\n",
						(int)(-hmem_copy_ret));
					ret = hmem_copy_ret;
				} else if (hmem_copy_ret != cmd->hdr.size) {
					FI_WARN(&smr_prov, FI_LOG_EP_CTRL,
						"Incomplete rma read/fetch "
						"buffer copied\n");
					ret = -FI_ETRUNC;
				} else {
					pend->bytes_done =
						(size_t) hmem_copy_ret;
				}
			}
		}
		break;
	default:
		FI_WARN(&smr_prov, FI_LOG_EP_CTRL,
			"unidentified operation type\n");
	}

	return ret;
}

static void smr_progress_return(struct smr_ep *ep)
{
	struct smr_return_entry *queue_entry;
	struct smr_cmd *cmd;
	struct smr_pend_entry *pending;
	int64_t pos;
	int ret;

	while (1) {
		ret = smr_return_queue_head(smr_return_queue(ep->region),
					    &queue_entry, &pos);
		if (ret == -FI_ENOENT)
			break;

		cmd = (struct smr_cmd *) queue_entry->ptr;
		pending = (struct smr_pend_entry *) cmd->hdr.tx_ctx;

		ret = smr_progress_return_entry(ep, cmd, pending);
		if (ret != -FI_EAGAIN) {
			if (pending) {
				if (cmd->hdr.status) {
					ret = smr_write_err_comp(
							ep->util_ep.tx_cq,
							pending->comp_ctx,
							pending->comp_flags,
							cmd->hdr.tag,
							cmd->hdr.status);
				} else {
					ret = smr_complete_tx(
							ep, pending->comp_ctx,
							cmd->hdr.op,
							pending->comp_flags);
				}
				if (ret) {
					FI_WARN(&smr_prov, FI_LOG_EP_CTRL,
						"unable to process "
						"tx completion\n");
				}
				ofi_buf_free(pending);
			}
			smr_freestack_push(smr_cmd_stack(ep->region), cmd);
		}
		smr_return_queue_release(smr_return_queue(ep->region),
					 queue_entry, pos);
	}
}

static ssize_t smr_progress_inline(struct smr_ep *ep, struct smr_cmd *cmd,
				   struct fi_peer_rx_entry *rx_entry,
				   struct ofi_mr **mr, struct iovec *iov,
				   size_t iov_count)
{
	ssize_t ret;

	ret = ofi_copy_to_mr_iov(mr, iov, iov_count, 0, cmd->data.msg,
				 cmd->hdr.size);
	if (ret < 0) {
		FI_WARN(&smr_prov, FI_LOG_EP_CTRL,
			"inline recv failed with code %d\n", (int)(-ret));
		return ret;
	}
	if (ret != cmd->hdr.size) {
		FI_WARN(&smr_prov, FI_LOG_EP_CTRL, "inline recv truncated\n");
		return -FI_ETRUNC;
	}
	return FI_SUCCESS;
}

static ssize_t smr_progress_inject(struct smr_ep *ep, struct smr_cmd *cmd,
				   struct fi_peer_rx_entry *rx_entry,
				   struct ofi_mr **mr, struct iovec *iov,
				   size_t iov_count)
{
	struct smr_region *peer_smr;
	struct smr_inject_buf *tx_buf;
	ssize_t ret;

	peer_smr = smr_peer_region(ep, cmd->hdr.rx_id);
	tx_buf = smr_get_inject_buf(peer_smr, cmd);

	if (cmd->hdr.op == ofi_op_read_req) {
		ret = ofi_copy_from_mr_iov(tx_buf->data, cmd->hdr.size, mr,
					   iov, iov_count, 0);
	} else {
		ret = ofi_copy_to_mr_iov(mr, iov, iov_count, 0, tx_buf->data,
					 cmd->hdr.size);
	}

	if (ret < 0) {
		FI_WARN(&smr_prov, FI_LOG_EP_CTRL,
			"inject recv failed with code %lu\n", ret);
	} else if (ret != cmd->hdr.size) {
		FI_WARN(&smr_prov, FI_LOG_EP_CTRL,
			"inject recv truncated\n");
		ret = -FI_ETRUNC;
	} else {
		ret = FI_SUCCESS;
	}

	cmd->hdr.status = ret;
	return ret;
}

static ssize_t smr_progress_iov(struct smr_ep *ep, struct smr_cmd *cmd,
				struct fi_peer_rx_entry *rx_entry,
				struct ofi_mr **mr, struct iovec *iov,
				size_t iov_count)
{
	struct smr_region *peer_smr;
	struct ofi_xpmem_client *xpmem;
	int ret;

	peer_smr = smr_peer_region(ep, cmd->hdr.rx_id);

	xpmem = &smr_peer_data(ep->region)[cmd->hdr.rx_id].xpmem;

	ret = ofi_shm_p2p_copy(ep->p2p_type, iov, iov_count, cmd->data.iov,
			       cmd->data.iov_count, cmd->hdr.size,
			       peer_smr->pid, cmd->hdr.op == ofi_op_read_req,
			       xpmem);
	cmd->hdr.status = ret;
	return ret;
}

static int smr_buffer_sar(struct smr_ep *ep, struct smr_pend_entry *sar_entry,
			  struct smr_cmd_ctx *cmd_ctx)
{
	struct smr_region *peer_smr;
	struct smr_sar_buf *sar_buf;
	struct smr_unexp_buf *buf;
	struct smr_cmd *cmd = cmd_ctx->cmd;
	size_t bytes;
	int next_buf = 0;

	peer_smr = smr_peer_region(ep, cmd->hdr.rx_id);

	if (smr_peer_data(peer_smr)[cmd->hdr.tx_id].sar_status !=
	    SMR_SAR_READY)
		return -FI_EAGAIN;

	while (next_buf < cmd->data.buf_batch_size &&
	       sar_entry->bytes_done < cmd->hdr.size) {
		buf = ofi_buf_alloc(ep->unexp_buf_pool);
		if (!buf) {
			FI_WARN(&smr_prov, FI_LOG_EP_CTRL,
				"Error allocating buffer for unexpected SAR "
				"(-FI_ENOMEM)\n");
			return -FI_ENOMEM;
		}
		slist_insert_tail(&buf->entry, &cmd_ctx->buf_list);

		sar_buf = smr_freestack_get_entry_from_index(
						smr_sar_pool(peer_smr),
						cmd->data.sar[next_buf]);
		bytes = MIN(cmd->hdr.size - sar_entry->bytes_done,
			    SMR_SAR_SIZE);

		memcpy(buf->buf, sar_buf->buf, bytes);

		sar_entry->bytes_done += bytes;
		next_buf++;
	}
	return FI_SUCCESS;
}

static ssize_t smr_try_copy_rx_sar(struct smr_ep *ep,
				   struct smr_pend_entry *pend)
{
	ssize_t ret;

	ret = pend->sar_copy_fn(ep, pend);
	if (ret) {
		if (ret == -FI_EAGAIN)
			dlist_insert_tail(&pend->entry, &ep->async_cpy_list);
		else if (ret != -FI_EBUSY)
			pend->cmd->hdr.status = ret;
	}
	return ret;
}

static int smr_progress_pending_sar(struct smr_ep *ep, struct smr_cmd *cmd)
{
	struct smr_pend_entry *pend;
	int ret;

	pend = (struct smr_pend_entry *) cmd->hdr.rx_ctx;
	if (pend->rx_entry && pend->rx_entry->peer_context) {
		ret = smr_buffer_sar(ep, pend, pend->rx_entry->peer_context);
		if (ret == -FI_EAGAIN) {
			dlist_insert_tail(&pend->entry, &ep->async_cpy_list);
			return FI_SUCCESS;
		}
		cmd->hdr.status = ret;
		goto out;
	}

	ret = smr_try_copy_rx_sar(ep, pend);
	if (ret == -FI_EBUSY || ret == -FI_EAGAIN)
		return FI_SUCCESS;

	if (pend->bytes_done == cmd->hdr.size || pend->cmd->hdr.status) {
		if (pend->cmd->hdr.status) {
			ret = smr_write_err_comp(ep->util_ep.rx_cq,
						 pend->comp_ctx,
						 pend->comp_flags,
						 cmd->hdr.tag,
						 pend->cmd->hdr.status);
		} else {
			ret = smr_complete_rx(ep, pend->comp_ctx,
					      cmd->hdr.op,
					      pend->comp_flags,
					      pend->bytes_done,
					      pend->iov[0].iov_base,
					      cmd->hdr.rx_id, cmd->hdr.tag,
					      cmd->hdr.cq_data);
		}
		if (ret) {
			FI_WARN(&smr_prov, FI_LOG_EP_CTRL,
				"unable to process rx completion\n");
		}
		if (pend->rx_entry)
			ep->srx->owner_ops->free_entry(pend->rx_entry);

		ofi_buf_free(pend);
	}

out:
	smr_return_cmd(ep, cmd);
	return ret;
}

static int smr_progress_pending(struct smr_ep *ep, struct smr_cmd *cmd)
{
	switch (cmd->hdr.proto) {
	case smr_proto_sar:
		return smr_progress_pending_sar(ep, cmd);
	default:
		return -FI_EINVAL;
	}
}

static void smr_init_rx_pend(struct smr_pend_entry *pend, struct smr_cmd *cmd,
			     struct fi_peer_rx_entry *rx_entry,
			     struct ofi_mr **mr, struct iovec *iov,
			     size_t iov_count)
{
	pend->type = SMR_RX_ENTRY;
	if (rx_entry) {
		pend->comp_ctx = rx_entry->context;
		pend->comp_flags = smr_rx_cq_flags(rx_entry->flags,
						   cmd->hdr.op_flags);
	} else {
		pend->comp_ctx = NULL;
		pend->comp_flags = smr_rx_cq_flags(0, cmd->hdr.op_flags);
	}

	pend->cmd = cmd;

	pend->sar_dir = pend->cmd->hdr.op == ofi_op_read_req ?
			OFI_COPY_IOV_TO_BUF : OFI_COPY_BUF_TO_IOV;

	pend->bytes_done = 0;
	if (iov) {
		memcpy(pend->iov, iov, sizeof(*iov) * iov_count);
		pend->iov_count = iov_count;
		if (mr)
			memcpy(pend->mr, mr, sizeof(*mr) * iov_count);
		else
			memset(pend->mr, 0, sizeof(*mr) * iov_count);
	}
	pend->rx_entry = rx_entry;
}

static ssize_t smr_progress_sar(struct smr_ep *ep, struct smr_cmd *cmd,
				struct fi_peer_rx_entry *rx_entry,
				struct ofi_mr **mr, struct iovec *iov,
				size_t iov_count)
{
	struct smr_pend_entry *pend = NULL;
	struct iovec sar_iov[SMR_IOV_LIMIT];
	ssize_t ret = FI_SUCCESS;

	memcpy(sar_iov, iov, sizeof(*iov) * iov_count);
	(void) ofi_truncate_iov(sar_iov, &iov_count, cmd->hdr.size);

	pend = ofi_buf_alloc(ep->pend_pool);
	assert(pend);

	cmd->hdr.rx_ctx = (uintptr_t) pend;

	smr_init_rx_pend(pend, cmd, rx_entry, mr, sar_iov, iov_count);
	if (smr_env.use_dsa_sar && ofi_mr_all_host(pend->mr, pend->iov_count))
		pend->sar_copy_fn = &smr_dsa_copy_sar;
	else
		pend->sar_copy_fn = &smr_copy_sar;

	ret = smr_try_copy_rx_sar(ep, pend);

	if (pend->bytes_done == cmd->hdr.size || pend->cmd->hdr.status) {
		cmd->hdr.rx_ctx = 0;
		ofi_buf_free(pend);
		ret = FI_SUCCESS;
	}

	return ret;
}

static int smr_ipc_async_copy(struct smr_ep *ep, struct smr_cmd *cmd,
			      struct fi_peer_rx_entry *rx_entry,
			      struct ofi_mr_entry *mr_entry,
			      struct iovec *iov, size_t iov_count, void *ptr)
{
	struct smr_pend_entry *ipc_entry;
	enum fi_hmem_iface iface = cmd->data.ipc_info.iface;
	uint64_t device = cmd->data.ipc_info.device;
	int ret;

	ipc_entry = ofi_buf_alloc(ep->pend_pool);
	if (!ipc_entry)
		return -FI_ENOMEM;

	cmd->hdr.rx_ctx = (uintptr_t) ipc_entry;
	smr_init_rx_pend(ipc_entry, cmd, rx_entry, NULL, iov, iov_count);
	ipc_entry->ipc_entry = mr_entry;

	ret = ofi_create_async_copy_event(iface, device,
					  &ipc_entry->async_event);
	if (ret < 0)
		goto fail;

	if (cmd->hdr.op == ofi_op_read_req) {
		ret = ofi_async_copy_from_hmem_iov(ptr, cmd->hdr.size,
						   iface, device, iov,
						   iov_count, 0,
						   ipc_entry->async_event);
	} else {
		ret = ofi_async_copy_to_hmem_iov(iface, device, iov, iov_count,
						 0, ptr, cmd->hdr.size,
						 ipc_entry->async_event);
	}
	if (ret < 0)
		goto fail;

	dlist_insert_tail(&ipc_entry->entry, &ep->async_cpy_list);
	return FI_SUCCESS;

fail:
	ofi_buf_free(ipc_entry);
	return ret;
}

static ssize_t smr_progress_ipc(struct smr_ep *ep, struct smr_cmd *cmd,
				struct fi_peer_rx_entry *rx_entry,
				struct ofi_mr **mr, struct iovec *iov,
				size_t iov_count)
{
	void *ptr;
	int ret;
	struct ofi_mr_entry *mr_entry;
	struct smr_domain *domain;

	domain = container_of(ep->util_ep.domain, struct smr_domain,
			      util_domain);

	if (cmd->data.ipc_info.iface == FI_HMEM_ZE)
		ze_set_pid_fd((void **) &cmd->data.ipc_info.ipc_handle,
			      ep->map->peers[cmd->hdr.rx_id].pid_fd);

	//TODO disable IPC if more than 1 interface is initialized
	ret = ofi_ipc_cache_search(domain->ipc_cache, cmd->hdr.rx_id,
				   &cmd->data.ipc_info, &mr_entry);
	if (ret)
		goto out;

	ptr = (char *) (uintptr_t) mr_entry->info.mapped_addr +
		(uintptr_t) cmd->data.ipc_info.offset;

	if (cmd->data.ipc_info.iface == FI_HMEM_ROCR) {
		ret = smr_ipc_async_copy(ep, cmd, rx_entry, mr_entry, iov,
					 iov_count, ptr);
		if (ret)
			goto uncache;
		return FI_SUCCESS;
	}

	if (cmd->hdr.op == ofi_op_read_req) {
		ret = ofi_copy_from_hmem_iov(ptr, cmd->hdr.size,
					     cmd->data.ipc_info.iface,
					     cmd->data.ipc_info.device, iov,
					     iov_count, 0);
	} else {
		ret = ofi_copy_to_hmem_iov(cmd->data.ipc_info.iface,
					   cmd->data.ipc_info.device, iov,
					   iov_count, 0, ptr, cmd->hdr.size);
	}

	if (ret == cmd->hdr.size)
		ret = FI_SUCCESS;
	else if (ret > 0)
		ret = -FI_ETRUNC;
uncache:
	ofi_mr_cache_delete(domain->ipc_cache, mr_entry);
out:
	cmd->hdr.status = ret;
	return ret;
}

typedef ssize_t (*smr_progress_func)(
	struct smr_ep *ep, struct smr_cmd *cmd,
	struct fi_peer_rx_entry *rx_entry, struct ofi_mr **mr,
	struct iovec *iov, size_t iov_count);

static smr_progress_func smr_progress_ops[smr_proto_max] = {
	[smr_proto_inline] = &smr_progress_inline,
	[smr_proto_inject] = &smr_progress_inject,
	[smr_proto_iov] = &smr_progress_iov,
	[smr_proto_sar] = &smr_progress_sar,
	[smr_proto_ipc] = &smr_progress_ipc,
};

static void smr_do_atomic(struct smr_cmd *cmd, void *src, struct ofi_mr *dst_mr,
			  void *dst, void *cmp, enum fi_datatype datatype,
			  enum fi_op op, size_t cnt, uint16_t flags)
{
	char tmp_result[SMR_INJECT_SIZE];
	char tmp_dst[SMR_INJECT_SIZE];
	char *cpy_dst;
	int ret;

	if (dst_mr && dst_mr->iface != FI_HMEM_SYSTEM) {
		ret = ofi_copy_to_hmem(dst_mr->iface, dst_mr->device, tmp_dst,
				       dst, cnt * ofi_datatype_size(datatype));
		if (ret)
			FI_WARN(&smr_prov, FI_LOG_EP_DATA,
				"Error copying from device to host buffer\n");
		cpy_dst = tmp_dst;
	} else {
		cpy_dst = dst;
	}

	if (ofi_atomic_isswap_op(op)) {
		ofi_atomic_swap_handler(op, datatype, cpy_dst, src, cmp,
					tmp_result, cnt);
		memcpy(src, tmp_result, cnt * ofi_datatype_size(datatype));
	} else if (cmd->hdr.op == ofi_op_atomic_fetch ||
		   ofi_atomic_isreadwrite_op(op)) {
		ofi_atomic_readwrite_handler(op, datatype, cpy_dst, src,
					     tmp_result, cnt);
		memcpy(src, op == FI_ATOMIC_READ ? cpy_dst : tmp_result,
		       cnt * ofi_datatype_size(datatype));
	} else if (ofi_atomic_iswrite_op(op)) {
		ofi_atomic_write_handler(op, datatype, cpy_dst, src, cnt);
	} else {
		FI_WARN(&smr_prov, FI_LOG_EP_DATA,
			"invalid atomic operation\n");
	}

	if (cpy_dst != dst) {
		ret = ofi_copy_to_hmem(dst_mr->iface, dst_mr->device, dst,
				cpy_dst, cnt * ofi_datatype_size(datatype));
		if (ret)
			FI_WARN(&smr_prov, FI_LOG_EP_DATA,
				"Error copying result to device buffer\n");
	}
}

static int smr_progress_inline_atomic(struct smr_cmd *cmd, struct ofi_mr **mr,
				      struct fi_ioc *ioc, size_t ioc_count,
				      size_t *len)
{
	int i;
	uint8_t *src = cmd->data.msg;

	assert(cmd->hdr.op == ofi_op_atomic);

	for (i = *len = 0; i < ioc_count && *len < cmd->hdr.size; i++) {
		smr_do_atomic(cmd, &src[*len], mr[i], ioc[i].addr, NULL,
			      cmd->hdr.datatype, cmd->hdr.atomic_op,
			      ioc[i].count, cmd->hdr.op_flags);
		*len += ioc[i].count * ofi_datatype_size(cmd->hdr.datatype);
	}

	if (*len != cmd->hdr.size) {
		FI_WARN(&smr_prov, FI_LOG_EP_CTRL,
			"recv truncated");
		return -FI_ETRUNC;
	}
	return FI_SUCCESS;
}

static int smr_progress_inject_atomic(struct smr_cmd *cmd, struct ofi_mr **mr,
				      struct fi_ioc *ioc, size_t ioc_count,
				      size_t *len, struct smr_ep *ep)
{
	struct smr_inject_buf *tx_buf;
	uint8_t *src, *comp;
	int i;

	tx_buf = smr_get_inject_buf(smr_peer_region(ep, cmd->hdr.rx_id), cmd);

	switch (cmd->hdr.op) {
	case ofi_op_atomic_compare:
		src = tx_buf->buf;
		comp = tx_buf->comp;
		break;
	default:
		src = tx_buf->data;
		comp = NULL;
		break;
	}

	for (i = *len = 0; i < ioc_count && *len < cmd->hdr.size; i++) {
		smr_do_atomic(cmd, &src[*len], mr[i], ioc[i].addr,
			      comp ? &comp[*len] : NULL, cmd->hdr.datatype,
			      cmd->hdr.atomic_op, ioc[i].count,
			      cmd->hdr.op_flags);
		*len += ioc[i].count * ofi_datatype_size(cmd->hdr.datatype);
	}

	if (*len != cmd->hdr.size) {
		FI_WARN(&smr_prov, FI_LOG_EP_CTRL, "recv truncated");
		return -FI_ETRUNC;
	}

	return FI_SUCCESS;
}

static int smr_start_common(struct smr_ep *ep, struct smr_cmd *cmd,
			    struct fi_peer_rx_entry *rx_entry)
{
	struct smr_pend_entry *pend;
	uint64_t comp_flags;
	void *comp_buf;
	int ret;
	bool return_cmd = cmd->hdr.proto != smr_proto_inline;

	rx_entry->peer_context = NULL;
	assert (cmd->hdr.proto < smr_proto_max);
	ret = smr_progress_ops[cmd->hdr.proto](
					ep, cmd, rx_entry,
					(struct ofi_mr **) rx_entry->desc,
					rx_entry->iov, rx_entry->count);

	if (!cmd->hdr.rx_ctx) {
		comp_buf = rx_entry->iov[0].iov_base;
		comp_flags = smr_rx_cq_flags(rx_entry->flags,
					     cmd->hdr.op_flags);
		if (ret) {
			FI_WARN(&smr_prov, FI_LOG_EP_CTRL,
				"error processing op\n");
			ret = smr_write_err_comp(ep->util_ep.rx_cq,
						 rx_entry->context,
						 comp_flags, rx_entry->tag,
						 ret);
		} else {
			ret = smr_complete_rx(ep, rx_entry->context,
					      cmd->hdr.op, comp_flags,
					      cmd->hdr.size, comp_buf,
					      cmd->hdr.rx_id, cmd->hdr.tag,
					      cmd->hdr.cq_data);
		}
		if (ret) {
			FI_WARN(&smr_prov, FI_LOG_EP_CTRL,
				"unable to process rx completion\n");
		}
		ep->srx->owner_ops->free_entry(rx_entry);
	} else if (cmd->hdr.proto == smr_proto_sar) {
		pend = (struct smr_pend_entry *) cmd->hdr.rx_ctx;
		if (pend->sar_copy_fn == &smr_dsa_copy_sar)
			return_cmd = false;
	}

	if (return_cmd)
		smr_return_cmd(ep, cmd);
	return ret;
}

static int smr_copy_saved(struct smr_cmd_ctx *cmd_ctx,
			  struct fi_peer_rx_entry *rx_entry)
{
	struct smr_unexp_buf *buf;
	struct smr_pend_entry *sar_entry;
	size_t bytes = 0;
	uint64_t comp_flags;
	int ret;

	comp_flags = smr_rx_cq_flags(rx_entry->flags,
				     cmd_ctx->cmd->hdr.op_flags);
	while (!slist_empty(&cmd_ctx->buf_list)) {
		slist_remove_head_container(&cmd_ctx->buf_list,
					    struct smr_unexp_buf, buf, entry);

		bytes += ofi_copy_to_mr_iov((struct ofi_mr **) rx_entry->desc,
					    rx_entry->iov, rx_entry->count,
					    bytes, buf->buf,
					    MIN(cmd_ctx->cmd->hdr.size - bytes,
						SMR_SAR_SIZE));
		ofi_buf_free(buf);
	}

	if (bytes != cmd_ctx->cmd->hdr.size) {
		assert(cmd_ctx->cmd->hdr.proto == smr_proto_sar);
		sar_entry = (struct smr_pend_entry *) cmd_ctx->pend;
		sar_entry->comp_ctx = rx_entry->context;
		sar_entry->comp_flags = comp_flags;

		memcpy(sar_entry->iov, rx_entry->iov,
		       sizeof(*rx_entry->iov) * rx_entry->count);
		sar_entry->iov_count = rx_entry->count;
		(void) ofi_truncate_iov(sar_entry->iov,
					&sar_entry->iov_count,
					cmd_ctx->cmd->hdr.size);
		memcpy(sar_entry->mr, rx_entry->desc,
		       sizeof(*rx_entry->desc) * sar_entry->iov_count);

		if (smr_env.use_dsa_sar && ofi_mr_all_host(sar_entry->mr,
		    sar_entry->iov_count))
			sar_entry->sar_copy_fn = &smr_dsa_copy_sar;
		else
			sar_entry->sar_copy_fn = &smr_copy_sar;
		return FI_SUCCESS;
	}

	ret = smr_complete_rx(cmd_ctx->ep, rx_entry->context,
			      cmd_ctx->cmd->hdr.op, comp_flags,
			      bytes, rx_entry->iov[0].iov_base,
			      cmd_ctx->cmd->hdr.rx_id,
			      cmd_ctx->cmd->hdr.tag,
			      cmd_ctx->cmd->hdr.cq_data);
	if (ret) {
		FI_WARN(&smr_prov, FI_LOG_EP_CTRL,
			"unable to process rx completion\n");
		return ret;
	}
	cmd_ctx->ep->srx->owner_ops->free_entry(rx_entry);

	return FI_SUCCESS;
}

int smr_unexp_start(struct fi_peer_rx_entry *rx_entry)
{
	struct smr_cmd_ctx *cmd_ctx = rx_entry->peer_context;
	int ret = FI_SUCCESS;

	if (cmd_ctx->cmd->hdr.op_flags & SMR_BUFFER_RECV)
		ret = smr_copy_saved(cmd_ctx, rx_entry);
	else
		ret = smr_start_common(cmd_ctx->ep, cmd_ctx->cmd, rx_entry);

	ofi_buf_free(cmd_ctx);
	rx_entry->peer_context = NULL;

	return ret;
}

static void smr_progress_connreq(struct smr_ep *ep, struct smr_cmd *cmd)
{
	struct smr_region *peer_smr;
	int64_t idx = -1;
	int ret = 0;

	ofi_genlock_lock(&ep->util_ep.av->lock);
	smr_map_add(ep->map, (char *) cmd->data.msg, &idx);

	peer_smr = smr_peer_region(ep, idx);
	if (!peer_smr) {
		ret = smr_map_to_region(ep->map, idx);
		if (ret) {
			FI_WARN(&smr_prov, FI_LOG_EP_CTRL,
				"Could not map peer region\n");
			goto out;
		}
		peer_smr = smr_peer_region(ep, idx);
	}

	assert(peer_smr);
	if (peer_smr->pid != (int) cmd->hdr.cq_data) {
		/* TODO track and update/complete in error any transfers
		 * to or from old mapping
		 */
		ofi_genlock_lock(&ep->util_ep.av->lock);
		smr_unmap_region(ep->map, idx, false);
		smr_map_to_region(ep->map, idx);
		ofi_genlock_unlock(&ep->util_ep.av->lock);
		peer_smr = smr_peer_region(ep, idx);
	}

	smr_set_ipc_valid(ep, idx);
	smr_peer_data(ep->region)[idx].id = cmd->hdr.tx_id;
	smr_peer_data(ep->region)[idx].local_region = (uintptr_t) peer_smr;

	assert(ep->map->num_peers > 0);
	ep->region->max_sar_buf_per_peer = MIN(
		SMR_BUF_BATCH_MAX,
		SMR_MAX_PEERS / ep->map->num_peers);

	//set last to indicate to peer that setup is complete
	smr_peer_data(peer_smr)[cmd->hdr.tx_id].id = idx;
out:
	ofi_genlock_unlock(&ep->util_ep.av->lock);
}

static int smr_unexp_inline(struct smr_ep *ep, struct smr_cmd_ctx *cmd_ctx,
			    struct smr_cmd *cmd)
{
	cmd_ctx->cmd = &cmd_ctx->cmd_cpy;
	memcpy(&cmd_ctx->cmd_cpy, cmd,
		sizeof(cmd->hdr) + cmd->hdr.size);
	return FI_SUCCESS;
}

static int smr_unexp_inject(struct smr_ep *ep, struct smr_cmd_ctx *cmd_ctx,
			    struct smr_cmd *cmd)
{
	struct smr_region *peer_smr;
	struct smr_unexp_buf *buf;
	struct smr_inject_buf *tx_buf;
	int ret = FI_SUCCESS;

	if (!(cmd->hdr.op_flags & SMR_BUFFER_RECV)) {
		cmd_ctx->cmd = cmd;
		return FI_SUCCESS;
	}

	peer_smr = smr_peer_region(ep, cmd_ctx->cmd->hdr.rx_id);

	memcpy(&cmd_ctx->cmd_cpy, cmd, sizeof(cmd_ctx->cmd->hdr));
	cmd_ctx->cmd = &cmd_ctx->cmd_cpy;

	buf = ofi_buf_alloc(ep->unexp_buf_pool);
	if (!buf) {
		ret = -FI_ENOMEM;
		cmd->hdr.status = ret;
		goto out;
	}

	tx_buf = smr_get_inject_buf(peer_smr, cmd);
	memcpy(buf->buf, tx_buf->buf, cmd_ctx->cmd->hdr.size);
	slist_insert_tail(&buf->entry, &cmd_ctx->buf_list);
out:
	smr_return_cmd(ep, cmd);
	return ret;
}

static int smr_unexp_iov(struct smr_ep *ep, struct smr_cmd_ctx *cmd_ctx,
			 struct smr_cmd *cmd)
{
	struct smr_unexp_buf *buf;
	size_t bytes = 0;
	struct iovec iov;
	int ret = FI_SUCCESS;

	if (!(cmd->hdr.op_flags & SMR_BUFFER_RECV)) {
		cmd_ctx->cmd = cmd;
		return FI_SUCCESS;
	}

	memcpy(&cmd_ctx->cmd_cpy, cmd, sizeof(cmd_ctx->cmd->hdr));
	cmd_ctx->cmd = &cmd_ctx->cmd_cpy;

	while (bytes < cmd_ctx->cmd->hdr.size) {
		buf = ofi_buf_alloc(ep->unexp_buf_pool);
		if (!buf) {
			ret = -FI_ENOMEM;
			goto out;
		}

		iov.iov_base = buf->buf;
		iov.iov_len = SMR_SAR_SIZE;

		cmd->hdr.size = MIN(cmd_ctx->cmd->hdr.size - bytes,
				    SMR_SAR_SIZE);
		ret = smr_progress_iov(ep, cmd, NULL, NULL, &iov, 1);
		if (ret) {
			ofi_buf_free(buf);
			goto out;
		}

		bytes += cmd->hdr.size;
		ofi_consume_iov(cmd->data.iov, &cmd->data.iov_count,
				cmd->hdr.size);

		slist_insert_tail(&buf->entry, &cmd_ctx->buf_list);
	}
out:
	cmd->hdr.status = ret;
	smr_return_cmd(ep, cmd);
	return ret;
}

static int smr_unexp_sar(struct smr_ep *ep, struct smr_cmd_ctx *cmd_ctx,
			 struct smr_cmd *cmd)
{
	struct fi_peer_rx_entry *rx_entry = cmd_ctx->rx_entry;
	struct smr_pend_entry *sar_entry;
	int ret;

	cmd->hdr.op_flags |= SMR_BUFFER_RECV;

	cmd_ctx->cmd = &cmd_ctx->cmd_cpy;
	memcpy(&cmd_ctx->cmd_cpy, cmd,
		sizeof(cmd->hdr) + sizeof(cmd->data));

	if (cmd->hdr.size) {
		sar_entry = ofi_buf_alloc(ep->pend_pool);
		if (!sar_entry) {
			ofi_buf_free(cmd_ctx);
			ret = -FI_ENOMEM;
			cmd->hdr.status = ret;
			goto out;
		}
		cmd->hdr.rx_ctx = (uintptr_t) sar_entry;

		smr_init_rx_pend(sar_entry, cmd, rx_entry, NULL, NULL, 0);
		cmd_ctx->pend = sar_entry;

		ret = smr_buffer_sar(ep, sar_entry,
				     sar_entry->rx_entry->peer_context);
		if (ret == -FI_EAGAIN) {
			dlist_insert_tail(&sar_entry->entry,
					  &ep->async_cpy_list);
			return FI_SUCCESS;
		}
		cmd->hdr.status = ret;
	}
out:
	smr_return_cmd(ep, cmd);
	return FI_SUCCESS;
}

static int smr_unexp_ipc(struct smr_ep *ep, struct smr_cmd_ctx *cmd_ctx,
			 struct smr_cmd *cmd)
{
	struct smr_unexp_buf *buf;
	size_t bytes = 0;
	size_t total_size;
	struct iovec iov;
	int ret = FI_SUCCESS;;

	if (!(cmd->hdr.op_flags & SMR_BUFFER_RECV)) {
		cmd_ctx->cmd = cmd;
		return FI_SUCCESS;
	}

	cmd_ctx->cmd = &cmd_ctx->cmd_cpy;
	memcpy(&cmd_ctx->cmd_cpy, cmd,
		sizeof(cmd->hdr) + sizeof(cmd->data));

	total_size = cmd_ctx->cmd->hdr.size;

	while (bytes < total_size) {
		buf = ofi_buf_alloc(ep->unexp_buf_pool);
		if (!buf) {
			ret = -FI_ENOMEM;
			goto out;
		}

		iov.iov_base = buf->buf;
		iov.iov_len = SMR_SAR_SIZE;

		cmd_ctx->cmd->hdr.size = MIN(total_size - bytes, SMR_SAR_SIZE);
		cmd_ctx->cmd->data.ipc_info.offset = cmd->data.ipc_info.offset + bytes;

		ret = smr_progress_ipc(ep, cmd_ctx->cmd, NULL, NULL, &iov, 1);
		if (ret) {
			ofi_buf_free(buf);
			goto out;
		}

		bytes += cmd_ctx->cmd->hdr.size;
		ofi_consume_iov(cmd->data.iov, &cmd->data.iov_count,
				SMR_SAR_SIZE);

		slist_insert_tail(&buf->entry, &cmd_ctx->buf_list);
	}

	/* Restore original total size */
	cmd_ctx->cmd->hdr.size = total_size;

out:
	cmd->hdr.status = ret;
	smr_return_cmd(ep, cmd);
	return ret;
}

typedef int (*smr_unexp_func)(struct smr_ep *ep, struct smr_cmd_ctx *cmd_ctx,
			      struct smr_cmd *cmd);

static smr_unexp_func smr_unexp_ops[smr_proto_max] = {
	[smr_proto_inline] = &smr_unexp_inline,
	[smr_proto_inject] = &smr_unexp_inject,
	[smr_proto_iov] = &smr_unexp_iov,
	[smr_proto_sar] = &smr_unexp_sar,
	[smr_proto_ipc] = &smr_unexp_ipc,
};

static int smr_alloc_cmd_ctx(struct smr_ep *ep,
			     struct fi_peer_rx_entry *rx_entry,
			     struct smr_cmd *cmd)
{
	struct smr_cmd_ctx *cmd_ctx;
	int ret;

	cmd_ctx = ofi_buf_alloc(ep->cmd_ctx_pool);
	if (!cmd_ctx) {
		FI_WARN(&smr_prov, FI_LOG_EP_CTRL,
			"Error allocating cmd ctx\n");
		return -FI_ENOMEM;
	}
	cmd_ctx->ep = ep;

	rx_entry->peer_context = cmd_ctx;
	cmd_ctx->rx_entry = rx_entry;
	cmd_ctx->cmd = cmd;
	slist_init(&cmd_ctx->buf_list);

	rx_entry->msg_size = cmd->hdr.size;
	if (cmd->hdr.op_flags & SMR_REMOTE_CQ_DATA) {
		rx_entry->flags |= FI_REMOTE_CQ_DATA;
		rx_entry->cq_data = cmd->hdr.cq_data;
	}

	ret = smr_unexp_ops[cmd->hdr.proto](ep, cmd_ctx, cmd);
	if (ret) {
		FI_WARN(&smr_prov, FI_LOG_EP_CTRL,
			"Error processing unexpected command\n");
		ofi_buf_free(cmd_ctx);
		return ret;
	}

	dlist_insert_tail(&cmd_ctx->entry, &ep->unexp_cmd_list);
	return FI_SUCCESS;
}

static int smr_progress_cmd_msg(struct smr_ep *ep, struct smr_cmd *cmd)
{
	struct fi_peer_match_attr attr;
	struct fi_peer_rx_entry *rx_entry;
	int ret;

	if (cmd->hdr.rx_ctx)
		return smr_progress_pending(ep, cmd);

	attr.addr = ep->map->peers[cmd->hdr.rx_id].fiaddr;
	attr.msg_size = cmd->hdr.size;
	attr.tag = cmd->hdr.tag;
	if (cmd->hdr.op == ofi_op_tagged) {
		ret = ep->srx->owner_ops->get_tag(ep->srx, &attr, &rx_entry);
		if (ret == -FI_ENOENT) {
			ret = smr_alloc_cmd_ctx(ep, rx_entry, cmd);
			if (ret) {
				ep->srx->owner_ops->free_entry(rx_entry);
				return ret;
			}

			ret = ep->srx->owner_ops->queue_tag(rx_entry);
			if (ret) {
				ep->srx->owner_ops->free_entry(rx_entry);
				return ret;
			}
			goto out;
		}
	} else {
		ret = ep->srx->owner_ops->get_msg(ep->srx, &attr, &rx_entry);
		if (ret == -FI_ENOENT) {
			ret = smr_alloc_cmd_ctx(ep, rx_entry, cmd);
			if (ret) {
				ep->srx->owner_ops->free_entry(rx_entry);
				return ret;
			}

			ret = ep->srx->owner_ops->queue_msg(rx_entry);
			if (ret) {
				ep->srx->owner_ops->free_entry(rx_entry);
				return ret;
			}
			goto out;
		}
	}
	if (ret) {
		FI_WARN(&smr_prov, FI_LOG_EP_CTRL, "Error getting rx_entry\n");
		return ret;
	}
	ret = smr_start_common(ep, cmd, rx_entry);

out:
	return ret < 0 ? ret : 0;
}

static int smr_progress_cmd_rma(struct smr_ep *ep, struct smr_cmd *cmd)
{
	struct smr_domain *domain;
	struct iovec iov[SMR_IOV_LIMIT];
	struct fi_rma_iov *rma_iov;
	size_t iov_count;
	int ret = 0;
	struct ofi_mr *mr[SMR_IOV_LIMIT];
	struct smr_pend_entry *pend;
	bool return_cmd = cmd->hdr.proto != smr_proto_inline;

	if (cmd->hdr.rx_ctx)
		return smr_progress_pending(ep, cmd);

	domain = container_of(ep->util_ep.domain, struct smr_domain,
			      util_domain);

	ofi_genlock_lock(&domain->util_domain.lock);
	for (iov_count = 0; iov_count < cmd->rma.rma_count; iov_count++) {
		rma_iov = &cmd->rma.rma_iov[iov_count];
		ret = ofi_mr_map_verify(&domain->util_domain.mr_map,
					(uintptr_t *) &(rma_iov->addr),
					rma_iov->len, rma_iov->key,
					ofi_rx_mr_reg_flags(cmd->hdr.op, 0),
					(void **) &mr[iov_count]);
		if (ret)
			break;

		iov[iov_count].iov_base = (void *)(uintptr_t) rma_iov->addr;
		iov[iov_count].iov_len = rma_iov->len;
	}
	ofi_genlock_unlock(&domain->util_domain.lock);

	if (ret)
		goto out;

	assert(cmd->hdr.proto < smr_proto_max);
	ret = smr_progress_ops[cmd->hdr.proto](ep, cmd, NULL, mr, iov,
					       iov_count);

	if (cmd->hdr.rx_ctx) {
		if (cmd->hdr.proto == smr_proto_sar) {
			pend = (struct smr_pend_entry *) cmd->hdr.rx_ctx;
			if (pend->sar_copy_fn == &smr_dsa_copy_sar)
				return_cmd = false;
		}
		goto out;
	}

	if (ret) {
		FI_WARN(&smr_prov, FI_LOG_EP_CTRL,
			"error processing rma op\n");
		ret = smr_write_err_comp(ep->util_ep.rx_cq, NULL,
					 smr_rx_cq_flags(0, cmd->hdr.op_flags),
					 0, ret);
	} else {
		ret = smr_complete_rx(ep, NULL, cmd->hdr.op,
				      smr_rx_cq_flags(0, cmd->hdr.op_flags),
				      cmd->hdr.size,
				      iov_count ? iov[0].iov_base : NULL,
				      cmd->hdr.rx_id, 0, cmd->hdr.cq_data);
	}
	if (ret) {
		FI_WARN(&smr_prov, FI_LOG_EP_CTRL,
			"unable to process rx completion\n");
	}

out:
	if (return_cmd)
		smr_return_cmd(ep, cmd);
	return ret;
}

static int smr_progress_cmd_atomic(struct smr_ep *ep, struct smr_cmd *cmd)
{
	struct smr_domain *domain;
	struct ofi_mr *mr[SMR_IOV_LIMIT];
	struct fi_ioc ioc[SMR_IOV_LIMIT];
	struct fi_rma_ioc *rma_ioc;
	size_t ioc_count, dt_size, total_len = 0;
	int err = 0, ret = 0;

	domain = container_of(ep->util_ep.domain, struct smr_domain,
			      util_domain);

	dt_size = ofi_datatype_size(cmd->hdr.datatype);
	ofi_genlock_lock(&domain->util_domain.lock);
	for (ioc_count = 0; ioc_count < cmd->rma.rma_count; ioc_count++) {
		rma_ioc = &cmd->rma.rma_ioc[ioc_count];
		ret = ofi_mr_map_verify(&domain->util_domain.mr_map,
					(uintptr_t *) &(rma_ioc->addr),
					rma_ioc->count * dt_size,
					rma_ioc->key,
					ofi_rx_mr_reg_flags(cmd->hdr.op,
							    cmd->hdr.atomic_op),
					(void **) &mr[ioc_count]);
		if (ret)
			break;

		ioc[ioc_count].addr = (void *)(uintptr_t) rma_ioc->addr;
		ioc[ioc_count].count = rma_ioc->count;
	}
	ofi_genlock_unlock(&domain->util_domain.lock);

	if (ret)
		goto out;

	switch (cmd->hdr.proto) {
	case smr_proto_inline:
		err = smr_progress_inline_atomic(cmd, mr, ioc, ioc_count,
						 &total_len);
		break;
	case smr_proto_inject:
		err = smr_progress_inject_atomic(cmd, mr, ioc, ioc_count,
						 &total_len, ep);
		break;
	default:
		FI_WARN(&smr_prov, FI_LOG_EP_CTRL,
			"unidentified operation type\n");
		err = -FI_EINVAL;
	}
	cmd->hdr.status = -err;

	if (err) {
		FI_WARN(&smr_prov, FI_LOG_EP_CTRL,
			"error processing atomic op\n");
		ret = smr_write_err_comp(ep->util_ep.rx_cq, NULL,
					 smr_rx_cq_flags(0,
					 cmd->hdr.op_flags), 0, err);
	} else {
		ret = smr_complete_rx(ep, NULL, cmd->hdr.op,
				      smr_rx_cq_flags(0,
				      cmd->hdr.op_flags), total_len,
				      ioc_count ? ioc[0].addr : NULL,
				      cmd->hdr.rx_id, 0, cmd->hdr.cq_data);
	}
	if (ret) {
		FI_WARN(&smr_prov, FI_LOG_EP_CTRL,
			"unable to process rx completion\n");
		err = ret;
	}

out:
	if (cmd->hdr.proto != smr_proto_inline)
		smr_return_cmd(ep, cmd);
	return err;
}

static void smr_progress_cmd(struct smr_ep *ep)
{
	struct smr_cmd_entry *ce;
	struct smr_cmd *cmd;
	int ret = 0;
	int64_t pos;

	while (1) {
		ret = smr_cmd_queue_head(smr_cmd_queue(ep->region), &ce, &pos);
		if (ret == -FI_ENOENT)
			break;

		cmd = (struct smr_cmd *) ce->ptr;
		switch (cmd->hdr.op) {
		case ofi_op_msg:
		case ofi_op_tagged:
			ret = smr_progress_cmd_msg(ep, cmd);
			break;
		case ofi_op_write:
		case ofi_op_read_req:
			ret = smr_progress_cmd_rma(ep, cmd);
			break;
		case ofi_op_write_async:
		case ofi_op_read_async:
			ofi_ep_peer_rx_cntr_inc(&ep->util_ep, cmd->hdr.op);
			break;
		case ofi_op_atomic:
		case ofi_op_atomic_fetch:
		case ofi_op_atomic_compare:
			ret = smr_progress_cmd_atomic(ep, cmd);
			break;
		case SMR_OP_MAX + ofi_ctrl_connreq:
			smr_progress_connreq(ep, cmd);
			break;
		default:
			FI_WARN(&smr_prov, FI_LOG_EP_CTRL,
				"unidentified operation type\n");
			ret = -FI_EINVAL;
		}
		smr_cmd_queue_release(smr_cmd_queue(ep->region), ce, pos);
		if (ret) {
			if (ret != -FI_EAGAIN) {
				FI_WARN(&smr_prov, FI_LOG_EP_CTRL,
					"error processing command\n");
			}
			break;
		}
	}
}

static void smr_progress_async_ipc(struct smr_ep *ep,
				   struct smr_pend_entry *ipc_entry)
{
	struct smr_domain *domain;
	enum fi_hmem_iface iface;
	uint64_t device;
	int ret;

	domain = container_of(ep->util_ep.domain, struct smr_domain,
			      util_domain);

	iface = ipc_entry->cmd->data.ipc_info.iface;
	device = ipc_entry->cmd->data.ipc_info.device;

	if (ofi_async_copy_query(iface, ipc_entry->async_event))
		return;

	ofi_mr_cache_delete(domain->ipc_cache, ipc_entry->ipc_entry);
	ofi_free_async_copy_event(iface, device, ipc_entry->async_event);

	ret = smr_complete_rx(ep, ipc_entry->comp_ctx, ipc_entry->cmd->hdr.op,
			      ipc_entry->comp_flags, ipc_entry->cmd->hdr.size,
			      ipc_entry->iov[0].iov_base,
			      ipc_entry->cmd->hdr.rx_id,
			      ipc_entry->cmd->hdr.tag,
			      ipc_entry->cmd->hdr.cq_data);
	if (ret) {
		FI_WARN(&smr_prov, FI_LOG_EP_CTRL,
			"unable to process rx completion\n");
	}
	if (ipc_entry->rx_entry)
		ep->srx->owner_ops->free_entry(ipc_entry->rx_entry);

	smr_return_cmd(ep, ipc_entry->cmd);
	dlist_remove(&ipc_entry->entry);
	ofi_buf_free(ipc_entry);
}

static void smr_progress_async_sar(struct smr_ep *ep,
				   struct smr_pend_entry *pend)
{
	ssize_t ret;

	if (pend->rx_entry && pend->rx_entry->peer_context) {
		ret = smr_buffer_sar(ep, pend, pend->rx_entry->peer_context);
		if (ret == -FI_EAGAIN)
			return;
		pend->cmd->hdr.status = ret;
		dlist_remove(&pend->entry);
		smr_return_cmd(ep, pend->cmd);
		return;
	}

	ret = pend->sar_copy_fn(ep, pend);
	if (ret) {
		if (ret == -FI_EAGAIN)
			return;
		/* -FI_EBUSY indicates copy was submitted successfully but will
		 * complete asynchronously through DSA progress
		 */
		if (ret == -FI_EBUSY) {
			dlist_remove(&pend->entry);
			return;
		}
		pend->cmd->hdr.status = ret;
	}
}

void smr_progress_async(struct smr_ep *ep)
{
	struct smr_pend_entry *async_entry;
	struct dlist_entry *tmp;

	dlist_foreach_container_safe(&ep->async_cpy_list,
				     struct smr_pend_entry,
				     async_entry, entry, tmp) {
		switch (async_entry->cmd->hdr.proto) {
		case smr_proto_ipc:
			smr_progress_async_ipc(ep, async_entry);
			break;
		case smr_proto_sar:
			smr_progress_async_sar(ep, async_entry);
			break;
		default:
			FI_WARN(&smr_prov, FI_LOG_EP_CTRL,
				"unidentified operation type\n");
			assert(0);
		}
	}
}

void smr_ep_progress(struct util_ep *util_ep)
{
	struct smr_ep *ep;

	ep = container_of(util_ep, struct smr_ep, util_ep);

	if (!ep->region)
		return;

	/* ep->util_ep.lock is used to serialize the message/tag matching.
	 * We keep the lock until the matching is complete. This will
	 * ensure that commands are matched in the order they are
	 * received, if there are multiple progress threads.
	 *
	 * This lock should be low cost because it's only used by this
	 * single process. It is also optimized to be a noop if
	 * multi-threading is disabled.
	 *
	 * Other processes are free to post on the queue without the need
	 * for locking the queue.
	 */
	ofi_genlock_lock(&ep->util_ep.lock);

	if (smr_env.use_dsa_sar)
		smr_dsa_progress(ep);

	smr_progress_return(ep);

	if (!slist_empty(&ep->overflow_list))
		smr_progress_overflow(ep);

	smr_progress_cmd(ep);

	/* always drive forward the ipc list since the completion is
	 * independent of any action by the provider */
	ep->smr_progress_async(ep);

	ofi_genlock_unlock(&ep->util_ep.lock);
}