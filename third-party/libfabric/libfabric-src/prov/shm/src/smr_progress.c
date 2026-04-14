/*
 * Copyright (c) 2013-2020 Intel Corporation. All rights reserved
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

#include <stdlib.h>
#include <string.h>
#include <sys/uio.h>

#include "ofi_iov.h"
#include "ofi_hmem.h"
#include "ofi_atom.h"
#include "ofi_mb.h"
#include "ofi_mr.h"
#include "ofi_shm_p2p.h"
#include "smr.h"
#include "smr_dsa.h"

static inline void
smr_try_progress_to_sar(struct smr_ep *ep, struct smr_region *smr,
                        struct smr_freestack *sar_pool, struct smr_resp *resp,
                        struct smr_cmd *cmd, struct ofi_mr **mr,
			struct iovec *iov, size_t iov_count,
                        size_t *bytes_done, void *entry_ptr)
{
	if (*bytes_done < cmd->msg.hdr.size) {
		if (smr_env.use_dsa_sar && ofi_mr_all_host(mr, iov_count)) {
			(void) smr_dsa_copy_to_sar(ep, sar_pool, resp, cmd, iov,
					    iov_count, bytes_done, entry_ptr);
			return;
		} else {
			smr_copy_to_sar(sar_pool, resp, cmd, mr, iov, iov_count,
					bytes_done);
		}
	}
}

static inline void
smr_try_progress_from_sar(struct smr_ep *ep, struct smr_region *smr,
                          struct smr_freestack *sar_pool, struct smr_resp *resp,
                          struct smr_cmd *cmd, struct ofi_mr **mr,
			  struct iovec *iov, size_t iov_count,
                          size_t *bytes_done, void *entry_ptr)
{
	if (*bytes_done < cmd->msg.hdr.size) {
		if (smr_env.use_dsa_sar && ofi_mr_all_host(mr, iov_count)) {
			(void) smr_dsa_copy_from_sar(ep, sar_pool, resp, cmd,
					iov, iov_count, bytes_done, entry_ptr);
			return;
		} else {
			smr_copy_from_sar(sar_pool, resp, cmd, mr,
					  iov, iov_count, bytes_done);
		}
	}
}

static int smr_progress_resp_entry(struct smr_ep *ep, struct smr_resp *resp,
				   struct smr_tx_entry *pending, uint64_t *err)
{
	int i;
	struct smr_region *peer_smr;
	size_t inj_offset;
	struct smr_inject_buf *tx_buf = NULL;
	struct smr_sar_buf *sar_buf = NULL;
	uint8_t *src;
	ssize_t hmem_copy_ret;

	peer_smr = smr_peer_region(ep->region, pending->peer_id);

	switch (pending->cmd.msg.hdr.op_src) {
	case smr_src_iov:
		break;
	case smr_src_ipc:
		assert(pending->mr[0]);
		break;
	case smr_src_sar:
		sar_buf = smr_freestack_get_entry_from_index(
		    smr_sar_pool(peer_smr), pending->cmd.msg.data.sar[0]);
		if (pending->bytes_done == pending->cmd.msg.hdr.size &&
		    (resp->status == SMR_STATUS_SAR_EMPTY ||
		     resp->status == SMR_STATUS_SUCCESS)) {
			resp->status = SMR_STATUS_SUCCESS;
			break;
		}

		if (pending->cmd.msg.hdr.op == ofi_op_read_req)
			smr_try_progress_from_sar(ep, peer_smr,
					smr_sar_pool(peer_smr), resp,
					&pending->cmd, pending->mr, pending->iov,
				        pending->iov_count, &pending->bytes_done,
					pending);
		else
			smr_try_progress_to_sar(ep, peer_smr,
					smr_sar_pool(peer_smr), resp,
					&pending->cmd, pending->mr, pending->iov,
					pending->iov_count, &pending->bytes_done,
					pending);
		if (pending->bytes_done != pending->cmd.msg.hdr.size ||
		    resp->status != SMR_STATUS_SAR_EMPTY) {
			return -FI_EAGAIN;
		}

		resp->status = SMR_STATUS_SUCCESS;
		break;
	case smr_src_mmap:
		if (!pending->map_name)
			break;
		if (pending->cmd.msg.hdr.op == ofi_op_read_req) {
			if (!*err) {
				hmem_copy_ret =
					ofi_copy_to_mr_iov(pending->mr,
							   pending->iov,
							   pending->iov_count,
							   0, pending->map_ptr,
							   pending->cmd.msg.hdr.size);
				if (hmem_copy_ret < 0) {
					FI_WARN(&smr_prov, FI_LOG_EP_CTRL,
						"Copy from mmapped file failed with code %d\n",
						(int)(-hmem_copy_ret));
					*err = hmem_copy_ret;
				} else if (hmem_copy_ret != pending->cmd.msg.hdr.size) {
					FI_WARN(&smr_prov, FI_LOG_EP_CTRL,
						"Incomplete copy from mmapped file\n");
					*err = -FI_ETRUNC;
				} else {
					pending->bytes_done = (size_t) hmem_copy_ret;
				}
			}
			munmap(pending->map_ptr, pending->cmd.msg.hdr.size);
		}
		shm_unlink(pending->map_name->name);
		dlist_remove(&pending->map_name->entry);
		free(pending->map_name);
		pending->map_name = NULL;
		break;
	case smr_src_inject:
		inj_offset = (size_t) pending->cmd.msg.hdr.src_data;
		tx_buf = smr_get_ptr(peer_smr, inj_offset);
		if (*err || pending->bytes_done == pending->cmd.msg.hdr.size ||
		    pending->cmd.msg.hdr.op == ofi_op_atomic)
			break;

		src = pending->cmd.msg.hdr.op == ofi_op_atomic_compare ?
		      tx_buf->buf : tx_buf->data;
		hmem_copy_ret  = ofi_copy_to_mr_iov(pending->mr,
					pending->iov, pending->iov_count,
					0, src, pending->cmd.msg.hdr.size);

		if (hmem_copy_ret < 0) {
			FI_WARN(&smr_prov, FI_LOG_EP_CTRL,
				"RMA read/fetch failed with code %d\n",
				(int)(-hmem_copy_ret));
			*err = hmem_copy_ret;
		} else if (hmem_copy_ret != pending->cmd.msg.hdr.size) {
			FI_WARN(&smr_prov, FI_LOG_EP_CTRL,
				"Incomplete rma read/fetch buffer copied\n");
			*err = -FI_ETRUNC;
		} else {
			pending->bytes_done = (size_t) hmem_copy_ret;
		}
		break;
	default:
		FI_WARN(&smr_prov, FI_LOG_EP_CTRL,
			"unidentified operation type\n");
	}

	if (tx_buf) {
		smr_release_txbuf(peer_smr, tx_buf);
	} else if (sar_buf) {
		pthread_spin_lock(&peer_smr->lock);
		for (i = pending->cmd.msg.data.buf_batch_size - 1; i >= 0; i--) {
			smr_freestack_push_by_index(smr_sar_pool(peer_smr),
					pending->cmd.msg.data.sar[i]);
		}
		pthread_spin_unlock(&peer_smr->lock);
		smr_peer_data(ep->region)[pending->peer_id].sar_status = 0;
	}

	return FI_SUCCESS;
}

static void smr_progress_resp(struct smr_ep *ep)
{
	struct smr_resp *resp;
	struct smr_tx_entry *pending;
	int ret;

	ofi_genlock_lock(&ep->util_ep.lock);
	while (!ofi_cirque_isempty(smr_resp_queue(ep->region))) {
		resp = ofi_cirque_head(smr_resp_queue(ep->region));
		if (resp->status == SMR_STATUS_BUSY)
			break;

		pending = (struct smr_tx_entry *) resp->msg_id;
		if (smr_progress_resp_entry(ep, resp, pending, &resp->status))
			break;

		if (resp->status) {
			ret = smr_write_err_comp(ep->util_ep.tx_cq, pending->context,
					 pending->op_flags, pending->cmd.msg.hdr.tag,
					 resp->status);
		} else {
			ret = smr_complete_tx(ep, pending->context,
					  pending->cmd.msg.hdr.op, pending->op_flags);
		}
		if (ret) {
			FI_WARN(&smr_prov, FI_LOG_EP_CTRL,
				"unable to process tx completion\n");
			break;
		}
		ofi_freestack_push(ep->tx_fs, pending);
		ofi_cirque_discard(smr_resp_queue(ep->region));
	}
	ofi_genlock_unlock(&ep->util_ep.lock);
}

static int smr_progress_inline(struct smr_cmd *cmd, struct ofi_mr **mr,
			       struct iovec *iov, size_t iov_count,
			       size_t *total_len)
{
	ssize_t hmem_copy_ret;

	hmem_copy_ret = ofi_copy_to_mr_iov(mr, iov, iov_count, 0,
				cmd->msg.data.msg, cmd->msg.hdr.size);
	if (hmem_copy_ret < 0) {
		FI_WARN(&smr_prov, FI_LOG_EP_CTRL,
			"inline recv failed with code %d\n",
			(int)(-hmem_copy_ret));
		return hmem_copy_ret;
	} else if (hmem_copy_ret != cmd->msg.hdr.size) {
		FI_WARN(&smr_prov, FI_LOG_EP_CTRL,
			"inline recv truncated\n");
		return -FI_ETRUNC;
	}

	*total_len = hmem_copy_ret;

	return FI_SUCCESS;
}

static int smr_progress_inject(struct smr_cmd *cmd, struct ofi_mr **mr,
			       struct iovec *iov, size_t iov_count,
			       size_t *total_len, struct smr_ep *ep, int err)
{
	struct smr_inject_buf *tx_buf;
	size_t inj_offset;
	ssize_t hmem_copy_ret;

	assert(cmd->msg.hdr.op != ofi_op_read_req);

	inj_offset = (size_t) cmd->msg.hdr.src_data;
	tx_buf = smr_get_ptr(ep->region, inj_offset);

	if (err) {
		smr_release_txbuf(ep->region, tx_buf);
		return err;
	}

	hmem_copy_ret = ofi_copy_to_mr_iov(mr, iov, iov_count, 0, tx_buf->data,
	                                   cmd->msg.hdr.size);
	smr_release_txbuf(ep->region, tx_buf);

	if (hmem_copy_ret < 0) {
		FI_WARN(&smr_prov, FI_LOG_EP_CTRL,
			"inject recv failed with code %d\n",
			(int)(-hmem_copy_ret));
		return hmem_copy_ret;
	} else if (hmem_copy_ret != cmd->msg.hdr.size) {
		FI_WARN(&smr_prov, FI_LOG_EP_CTRL,
			"inject recv truncated\n");
		return -FI_ETRUNC;
	}

	*total_len = hmem_copy_ret;

	return FI_SUCCESS;
}

static int smr_progress_iov(struct smr_cmd *cmd, struct iovec *iov,
			    size_t iov_count, size_t *total_len,
			    struct smr_ep *ep)
{
	struct smr_region *peer_smr;
	struct ofi_xpmem_client *xpmem;
	struct smr_resp *resp;
	int ret;

	peer_smr = smr_peer_region(ep->region, cmd->msg.hdr.id);
	resp = smr_get_ptr(peer_smr, cmd->msg.hdr.src_data);

	xpmem = &smr_peer_data(ep->region)[cmd->msg.hdr.id].xpmem;

	ret = ofi_shm_p2p_copy(ep->p2p_type, iov, iov_count, cmd->msg.data.iov,
			       cmd->msg.data.iov_count, cmd->msg.hdr.size,
			       peer_smr->pid, cmd->msg.hdr.op == ofi_op_read_req,
			       xpmem);
	if (!ret)
		*total_len = cmd->msg.hdr.size;

	//Status must be set last (signals peer: op done, valid resp entry)
	resp->status = -ret;

	return ret;
}

static int smr_mmap_peer_copy(struct smr_ep *ep, struct smr_cmd *cmd,
			      struct ofi_mr **mr, struct iovec *iov,
			      size_t iov_count, size_t *total_len)
{
	char shm_name[SMR_NAME_MAX];
	void *mapped_ptr;
	int fd, num;
	int ret = 0;
	ssize_t hmem_copy_ret;

	num = smr_mmap_name(shm_name,
			ep->region->map->peers[cmd->msg.hdr.id].peer.name,
			cmd->msg.hdr.msg_id);
	if (num < 0) {
		FI_WARN(&smr_prov, FI_LOG_AV, "generating shm file name failed\n");
		return -errno;
	}

	fd = shm_open(shm_name, O_RDWR, S_IRUSR | S_IWUSR);
	if (fd < 0) {
		FI_WARN(&smr_prov, FI_LOG_AV, "shm_open error\n");
		return -errno;
	}

	mapped_ptr = mmap(NULL, cmd->msg.hdr.size, PROT_READ | PROT_WRITE,
			  MAP_SHARED, fd, 0);
	if (mapped_ptr == MAP_FAILED) {
		FI_WARN(&smr_prov, FI_LOG_AV, "mmap error %s\n", strerror(errno));
		ret = -errno;
		goto unlink_close;
	}

	if (cmd->msg.hdr.op == ofi_op_read_req) {
		hmem_copy_ret = ofi_copy_from_mr_iov(mapped_ptr,
					cmd->msg.hdr.size, mr, iov,
					iov_count, 0);
	} else {
		hmem_copy_ret = ofi_copy_to_mr_iov(mr, iov, iov_count, 0,
					mapped_ptr, cmd->msg.hdr.size);
	}

	if (hmem_copy_ret < 0) {
		FI_WARN(&smr_prov, FI_LOG_EP_CTRL,
			"mmap copy iov failed with code %d\n",
			(int)(-hmem_copy_ret));
		ret = hmem_copy_ret;
	} else if (hmem_copy_ret != cmd->msg.hdr.size) {
		FI_WARN(&smr_prov, FI_LOG_EP_CTRL,
			"mmap copy iov truncated\n");
		ret = -FI_ETRUNC;
	}

	*total_len = hmem_copy_ret;

	munmap(mapped_ptr, cmd->msg.hdr.size);
unlink_close:
	shm_unlink(shm_name);
	close(fd);
	return ret;
}

static int smr_progress_mmap(struct smr_cmd *cmd, struct ofi_mr **mr,
			     struct iovec *iov, size_t iov_count,
			     size_t *total_len, struct smr_ep *ep)
{
	struct smr_region *peer_smr;
	struct smr_resp *resp;
	int ret;

	peer_smr = smr_peer_region(ep->region, cmd->msg.hdr.id);
	resp = smr_get_ptr(peer_smr, cmd->msg.hdr.src_data);

	ret = smr_mmap_peer_copy(ep, cmd, mr, iov, iov_count, total_len);

	//Status must be set last (signals peer: op done, valid resp entry)
	resp->status = -ret;

	return -ret;
}

static struct smr_pend_entry *smr_progress_sar(struct smr_cmd *cmd,
			struct fi_peer_rx_entry *rx_entry, struct ofi_mr **mr,
			struct iovec *iov, size_t iov_count,
			size_t *total_len, struct smr_ep *ep)
{
	struct smr_region *peer_smr;
	struct smr_pend_entry *sar_entry;
	struct smr_resp *resp;
	struct iovec sar_iov[SMR_IOV_LIMIT];

	peer_smr = smr_peer_region(ep->region, cmd->msg.hdr.id);
	resp = smr_get_ptr(peer_smr, cmd->msg.hdr.src_data);

	/* Nothing to do for 0 byte transfer */
	if (!cmd->msg.hdr.size) {
		resp->status = SMR_STATUS_SUCCESS;
		return NULL;
	}

	memcpy(sar_iov, iov, sizeof(*iov) * iov_count);
	(void) ofi_truncate_iov(sar_iov, &iov_count, cmd->msg.hdr.size);

	sar_entry = ofi_buf_alloc(ep->pend_buf_pool);
	dlist_insert_tail(&sar_entry->entry, &ep->sar_list);

	if (cmd->msg.hdr.op == ofi_op_read_req)
		smr_try_progress_to_sar(ep, peer_smr, smr_sar_pool(ep->region),
				resp, cmd, mr, sar_iov, iov_count,
				total_len, sar_entry);
	else
		smr_try_progress_from_sar(ep, peer_smr,
				smr_sar_pool(ep->region), resp, cmd, mr,
				sar_iov, iov_count, total_len, sar_entry);

	if (*total_len == cmd->msg.hdr.size) {
		dlist_remove(&sar_entry->entry);
		ofi_buf_free(sar_entry);
		return NULL;
	}
	sar_entry->cmd = *cmd;
	sar_entry->cmd_ctx = NULL;
	sar_entry->bytes_done = *total_len;
	memcpy(sar_entry->iov, sar_iov, sizeof(*sar_iov) * iov_count);
	sar_entry->iov_count = iov_count;
	sar_entry->rx_entry = rx_entry ? rx_entry : NULL;
	if (mr)
		memcpy(sar_entry->mr, mr, sizeof(*mr) * iov_count);
	else
		memset(sar_entry->mr, 0, sizeof(*mr) * iov_count);

	*total_len = cmd->msg.hdr.size;
	return sar_entry;
}

static int
smr_ipc_async_copy(struct smr_ep *ep, void *ptr,
		   struct fi_peer_rx_entry *rx_entry,
		   struct iovec *iov, size_t iov_count,
		   struct ofi_mr_entry *mr_entry, struct smr_cmd *cmd,
		   struct smr_pend_entry **pend)
{
	struct smr_pend_entry *ipc_entry;
	enum fi_hmem_iface iface = cmd->msg.data.ipc_info.iface;
	uint64_t device = cmd->msg.data.ipc_info.device;
	int ret;

	ipc_entry = ofi_buf_alloc(ep->pend_buf_pool);
	if (!ipc_entry)
		return -FI_ENOMEM;

	ipc_entry->cmd = *cmd;
	ipc_entry->ipc_entry = mr_entry;
	ipc_entry->bytes_done = 0;
	memcpy(ipc_entry->iov, iov, sizeof(*iov) * iov_count);
	ipc_entry->iov_count = iov_count;
	ipc_entry->rx_entry = rx_entry;
	if (rx_entry) {
		ipc_entry->rx_entry->flags |= cmd->msg.hdr.op_flags;
		ipc_entry->rx_entry->flags &= ~SMR_MULTI_RECV;
	}

	ret = ofi_create_async_copy_event(iface, device,
					  &ipc_entry->async_event);
	if (ret < 0)
		goto fail;

	if (cmd->msg.hdr.op == ofi_op_read_req) {
		ret = ofi_async_copy_from_hmem_iov(ptr, cmd->msg.hdr.size,
						iface, device, iov, iov_count, 0,
						ipc_entry->async_event);
	} else {
		ret = ofi_async_copy_to_hmem_iov(iface, device, iov, iov_count, 0,
						ptr, cmd->msg.hdr.size,
						ipc_entry->async_event);
	}

	if (ret < 0)
		goto fail;

	dlist_insert_tail(&ipc_entry->entry, &ep->ipc_cpy_pend_list);
	*pend = ipc_entry;

	return FI_SUCCESS;

fail:
	ofi_buf_free(ipc_entry);
	return ret;
}

static struct smr_pend_entry *smr_progress_ipc(struct smr_cmd *cmd,
			struct fi_peer_rx_entry *rx_entry,
			struct ofi_mr **mr, struct iovec *iov,
			size_t iov_count, size_t *total_len,
			struct smr_ep *ep, int *err)
{
	struct smr_region *peer_smr;
	struct smr_resp *resp;
	void *ptr;
	int ret;
	ssize_t hmem_copy_ret;
	struct ofi_mr_entry *mr_entry;
	struct smr_domain *domain;
	struct smr_pend_entry *ipc_entry;

	domain = container_of(ep->util_ep.domain, struct smr_domain,
			      util_domain);

	peer_smr = smr_peer_region(ep->region, cmd->msg.hdr.id);
	resp = smr_get_ptr(peer_smr, cmd->msg.hdr.src_data);

	if (cmd->msg.data.ipc_info.iface == FI_HMEM_ZE)
		ze_set_pid_fd((void **) &cmd->msg.data.ipc_info.ipc_handle,
			      ep->region->map->peers[cmd->msg.hdr.id].pid_fd);

	//TODO disable IPC if more than 1 interface is initialized
	ret = ofi_ipc_cache_search(domain->ipc_cache, cmd->msg.hdr.id,
				   &cmd->msg.data.ipc_info, &mr_entry);
	if (ret)
		goto out;

	ptr = (char *) (uintptr_t) mr_entry->info.mapped_addr +
		(uintptr_t) cmd->msg.data.ipc_info.offset;

	if (cmd->msg.data.ipc_info.iface == FI_HMEM_ROCR) {
		*total_len = 0;
		ipc_entry = NULL;
		resp->status = SMR_STATUS_BUSY;

		ret = smr_ipc_async_copy(ep, (char*)ptr, rx_entry, iov,
					 iov_count, mr_entry, cmd,
					 &ipc_entry);
		if (ret)
			resp->status = -ret;

		return ipc_entry;
	}

	if (cmd->msg.hdr.op == ofi_op_read_req) {
		hmem_copy_ret = ofi_copy_from_hmem_iov(ptr, cmd->msg.hdr.size,
					cmd->msg.data.ipc_info.iface,
					cmd->msg.data.ipc_info.device, iov,
					iov_count, 0);
	} else {
		hmem_copy_ret = ofi_copy_to_hmem_iov(cmd->msg.data.ipc_info.iface,
					cmd->msg.data.ipc_info.device, iov,
					iov_count, 0, ptr, cmd->msg.hdr.size);
	}

	ofi_mr_cache_delete(domain->ipc_cache, mr_entry);

	if (hmem_copy_ret < 0)
		*err = hmem_copy_ret;
	else if (hmem_copy_ret != cmd->msg.hdr.size)
		*err = -FI_ETRUNC;
	else
		*err = FI_SUCCESS;

	*total_len = hmem_copy_ret;

out:
	//Status must be set last (signals peer: op done, valid resp entry)
	resp->status = -ret;

	return NULL;
}

static void smr_do_atomic(void *src, struct ofi_mr *dst_mr, void *dst,
			  void *cmp, enum fi_datatype datatype, enum fi_op op,
			  size_t cnt, uint16_t flags)
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
	} else if (flags & SMR_RMA_REQ && ofi_atomic_isreadwrite_op(op)) {
		ofi_atomic_readwrite_handler(op, datatype, cpy_dst, src,
					     tmp_result, cnt);
	} else if (ofi_atomic_iswrite_op(op)) {
		ofi_atomic_write_handler(op, datatype, cpy_dst, src, cnt);
	} else {
		FI_WARN(&smr_prov, FI_LOG_EP_DATA,
			"invalid atomic operation\n");
	}

	if (flags & SMR_RMA_REQ)
		memcpy(src, op == FI_ATOMIC_READ ? cpy_dst : tmp_result,
		       cnt * ofi_datatype_size(datatype));

	if (cpy_dst != dst) {
		ret = ofi_copy_to_hmem(dst_mr->iface, dst_mr->device, dst,
				cpy_dst, cnt * ofi_datatype_size(datatype));
		if (ret)
			FI_WARN(&smr_prov, FI_LOG_EP_DATA,
				"Error copying result to device buffer\n");
	}
}

static int smr_progress_inline_atomic(struct smr_cmd *cmd, struct ofi_mr **mr,
			struct fi_ioc *ioc, size_t ioc_count, size_t *len)
{
	int i;
	uint8_t *src = cmd->msg.data.msg;

	assert(cmd->msg.hdr.op == ofi_op_atomic);

	for (i = *len = 0; i < ioc_count && *len < cmd->msg.hdr.size; i++) {
		smr_do_atomic(&src[*len], mr[i], ioc[i].addr, NULL,
			      cmd->msg.hdr.datatype, cmd->msg.hdr.atomic_op,
			      ioc[i].count, cmd->msg.hdr.op_flags);
		*len += ioc[i].count * ofi_datatype_size(cmd->msg.hdr.datatype);
	}

	if (*len != cmd->msg.hdr.size) {
		FI_WARN(&smr_prov, FI_LOG_EP_CTRL,
			"recv truncated");
		return -FI_ETRUNC;
	}
	return FI_SUCCESS;
}

static int smr_progress_inject_atomic(struct smr_cmd *cmd, struct ofi_mr **mr,
			struct fi_ioc *ioc, size_t ioc_count, size_t *len,
			struct smr_ep *ep, int err)
{
	struct smr_inject_buf *tx_buf;
	size_t inj_offset;
	uint8_t *src, *comp;
	int i;

	inj_offset = (size_t) cmd->msg.hdr.src_data;
	tx_buf = smr_get_ptr(ep->region, inj_offset);
	if (err)
		goto out;

	switch (cmd->msg.hdr.op) {
	case ofi_op_atomic_compare:
		src = tx_buf->buf;
		comp = tx_buf->comp;
		break;
	default:
		src = tx_buf->data;
		comp = NULL;
		break;
	}

	for (i = *len = 0; i < ioc_count && *len < cmd->msg.hdr.size; i++) {
		smr_do_atomic(&src[*len], mr[i], ioc[i].addr,
			      comp ? &comp[*len] : NULL, cmd->msg.hdr.datatype,
			      cmd->msg.hdr.atomic_op, ioc[i].count,
			      cmd->msg.hdr.op_flags);
		*len += ioc[i].count * ofi_datatype_size(cmd->msg.hdr.datatype);
	}

	if (*len != cmd->msg.hdr.size) {
		FI_WARN(&smr_prov, FI_LOG_EP_CTRL,
			"recv truncated");
		err = -FI_ETRUNC;
	}

out:
	if (!(cmd->msg.hdr.op_flags & SMR_RMA_REQ))
		smr_release_txbuf(ep->region, tx_buf);

	return err;
}

static int smr_start_common(struct smr_ep *ep, struct smr_cmd *cmd,
		struct fi_peer_rx_entry *rx_entry)
{
	struct smr_pend_entry *pend = NULL;
	size_t total_len = 0;
	uint64_t comp_flags;
	void *comp_buf;
	int ret;
	int err = 0;

	switch (cmd->msg.hdr.op_src) {
	case smr_src_inline:
		err = smr_progress_inline(cmd,
				(struct ofi_mr **) rx_entry->desc,
				rx_entry->iov, rx_entry->count, &total_len);
		break;
	case smr_src_inject:
		err = smr_progress_inject(cmd,
				(struct ofi_mr **) rx_entry->desc,
				rx_entry->iov, rx_entry->count, &total_len,
				ep, 0);
		break;
	case smr_src_iov:
		err = smr_progress_iov(cmd, rx_entry->iov, rx_entry->count,
				       &total_len, ep);
		break;
	case smr_src_mmap:
		err = smr_progress_mmap(cmd, (struct ofi_mr **) rx_entry->desc,
					rx_entry->iov, rx_entry->count,
					&total_len, ep);
		break;
	case smr_src_sar:
		pend = smr_progress_sar(cmd, rx_entry,
				       (struct ofi_mr **) rx_entry->desc,
				       rx_entry->iov, rx_entry->count,
				       &total_len, ep);
		break;
	case smr_src_ipc:
		pend = smr_progress_ipc(cmd, rx_entry,
				(struct ofi_mr **) rx_entry->desc,
				rx_entry->iov, rx_entry->count,
				&total_len, ep, &err);
		break;
	default:
		FI_WARN(&smr_prov, FI_LOG_EP_CTRL,
			"unidentified operation type\n");
		err = -FI_EINVAL;
	}

	if (!pend) {
		comp_buf = rx_entry->iov[0].iov_base;
		comp_flags = smr_rx_cq_flags(rx_entry->flags,
					     cmd->msg.hdr.op_flags);
		if (err) {
			FI_WARN(&smr_prov, FI_LOG_EP_CTRL,
				"error processing op\n");
			ret = smr_write_err_comp(ep->util_ep.rx_cq,
						 rx_entry->context,
						 comp_flags, rx_entry->tag,
						 -err);
		} else {
			ret = smr_complete_rx(ep, rx_entry->context, cmd->msg.hdr.op,
					      comp_flags, total_len, comp_buf,
					      cmd->msg.hdr.id, cmd->msg.hdr.tag,
					      cmd->msg.hdr.data);
		}
		if (ret) {
			FI_WARN(&smr_prov, FI_LOG_EP_CTRL,
				"unable to process rx completion\n");
		}
		ep->srx->owner_ops->free_entry(rx_entry);
	}

	return 0;
}

static int smr_copy_saved(struct smr_cmd_ctx *cmd_ctx,
		struct fi_peer_rx_entry *rx_entry)
{
	struct smr_unexp_buf *sar_buf;
	size_t bytes = 0;
	uint64_t comp_flags;
	int ret;

	while (!slist_empty(&cmd_ctx->buf_list)) {
		slist_remove_head_container(&cmd_ctx->buf_list,
				struct smr_unexp_buf, sar_buf, entry);

		bytes += ofi_copy_to_mr_iov((struct ofi_mr **) rx_entry->desc,
				rx_entry->iov, rx_entry->count, bytes,
				sar_buf->buf,
				MIN(cmd_ctx->cmd.msg.hdr.size - bytes,
					SMR_SAR_SIZE));
		ofi_buf_free(sar_buf);
	}
	if (bytes != cmd_ctx->cmd.msg.hdr.size) {
		assert(cmd_ctx->sar_entry);
		cmd_ctx->sar_entry->cmd_ctx = NULL;
		cmd_ctx->sar_entry->rx_entry = rx_entry;
		memcpy(cmd_ctx->sar_entry->iov, rx_entry->iov,
		       sizeof(*rx_entry->iov) * rx_entry->count);
		cmd_ctx->sar_entry->iov_count = rx_entry->count;
		(void) ofi_truncate_iov(cmd_ctx->sar_entry->iov,
					&cmd_ctx->sar_entry->iov_count,
					cmd_ctx->cmd.msg.hdr.size);
		memcpy(cmd_ctx->sar_entry->mr, rx_entry->desc,
		       sizeof(*rx_entry->desc) * cmd_ctx->sar_entry->iov_count);
		return FI_SUCCESS;
	}
	assert(!cmd_ctx->sar_entry);

	comp_flags = smr_rx_cq_flags(rx_entry->flags,
				     cmd_ctx->cmd.msg.hdr.op_flags);

	ret = smr_complete_rx(cmd_ctx->ep, rx_entry->context,
			      cmd_ctx->cmd.msg.hdr.op, comp_flags,
			      bytes, rx_entry->iov[0].iov_base,
			      cmd_ctx->cmd.msg.hdr.id,
			      cmd_ctx->cmd.msg.hdr.tag,
			      cmd_ctx->cmd.msg.hdr.data);
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
	int ret;

	if (cmd_ctx->cmd.msg.hdr.op_src == smr_src_sar ||
	    cmd_ctx->cmd.msg.hdr.op_src == smr_src_inject)
		ret = smr_copy_saved(cmd_ctx, rx_entry);
	else
		ret = smr_start_common(cmd_ctx->ep, &cmd_ctx->cmd, rx_entry);

	ofi_buf_free(cmd_ctx);

	return ret;
}

static void smr_progress_connreq(struct smr_ep *ep, struct smr_cmd *cmd)
{
	struct smr_region *peer_smr;
	struct smr_inject_buf *tx_buf;
	size_t inj_offset;
	int64_t idx = -1;
	int ret = 0;

	inj_offset = (size_t) cmd->msg.hdr.src_data;
	tx_buf = smr_get_ptr(ep->region, inj_offset);

	ret = smr_map_add(&smr_prov, ep->region->map,
			  (char *) tx_buf->data, &idx);
	if (ret || idx < 0) {
		FI_WARN(&smr_prov, FI_LOG_EP_CTRL,
			"Error processing mapping request\n");
		return;
	}

	peer_smr = smr_peer_region(ep->region, idx);
	if (!peer_smr) {
		ofi_spin_lock(&ep->region->map->lock);
		ret = smr_map_to_region(&smr_prov, ep->region->map, idx);
		ofi_spin_unlock(&ep->region->map->lock);
		if (ret) {
			FI_WARN(&smr_prov, FI_LOG_EP_CTRL,
				"Could not map peer region\n");
			return;
		}
		peer_smr = smr_peer_region(ep->region, idx);
	}

	assert(peer_smr);
	if (peer_smr->pid != (int) cmd->msg.hdr.data) {
		/* TODO track and update/complete in error any transfers
		 * to or from old mapping
		 */
		ofi_spin_lock(&ep->region->map->lock);
		smr_unmap_region(&smr_prov, ep->region->map, idx, false);
		smr_map_to_region(&smr_prov, ep->region->map, idx);
		ofi_spin_unlock(&ep->region->map->lock);
		peer_smr = smr_peer_region(ep->region, idx);
	}

	smr_set_ipc_valid(ep->region, idx);
	smr_peer_data(peer_smr)[cmd->msg.hdr.id].addr.id = idx;
	smr_peer_data(ep->region)[idx].addr.id = cmd->msg.hdr.id;

	smr_release_txbuf(ep->region, tx_buf);
	assert(ep->region->map->num_peers > 0);
	ep->region->max_sar_buf_per_peer = SMR_MAX_PEERS /
		ep->region->map->num_peers;
}

static int smr_alloc_cmd_ctx(struct smr_ep *ep,
		struct fi_peer_rx_entry *rx_entry, struct smr_cmd *cmd)
{
	struct smr_cmd_ctx *cmd_ctx;
	struct smr_pend_entry *sar_entry;
	struct smr_inject_buf *tx_buf;
	struct smr_unexp_buf *buf;

	cmd_ctx = ofi_buf_alloc(ep->cmd_ctx_pool);
	if (!cmd_ctx) {
		FI_WARN(&smr_prov, FI_LOG_EP_CTRL,
			"Error allocating cmd ctx\n");
		return -FI_ENOMEM;
	}
	cmd_ctx->ep = ep;
	cmd_ctx->rx_entry = rx_entry;

	rx_entry->msg_size = cmd->msg.hdr.size;
	if (cmd->msg.hdr.op_flags & SMR_REMOTE_CQ_DATA) {
		rx_entry->flags |= FI_REMOTE_CQ_DATA;
		rx_entry->cq_data = cmd->msg.hdr.data;
	}

	if (cmd->msg.hdr.op_src == smr_src_inline) {
		memcpy(&cmd_ctx->cmd, cmd, sizeof(cmd->msg.hdr) + cmd->msg.hdr.size);
	} else if (cmd->msg.hdr.op_src == smr_src_inject) {
		memcpy(&cmd_ctx->cmd, cmd, sizeof(cmd->msg.hdr));
		buf = ofi_buf_alloc(ep->unexp_buf_pool);
		if (!buf) {
			FI_WARN(&smr_prov, FI_LOG_EP_CTRL,
				"Error allocating buffer\n");
			ofi_buf_free(cmd_ctx);
			return -FI_ENOMEM;
		}
		cmd_ctx->sar_entry = NULL;
		slist_init(&cmd_ctx->buf_list);
		slist_insert_tail(&buf->entry, &cmd_ctx->buf_list);
		tx_buf = smr_get_ptr(ep->region, (size_t) cmd->msg.hdr.src_data);
		memcpy(buf->buf, tx_buf->buf, cmd->msg.hdr.size);
		smr_release_txbuf(ep->region, tx_buf);
	} else if (cmd->msg.hdr.op_src == smr_src_sar) {
		memcpy(&cmd_ctx->cmd, cmd, sizeof(*cmd));
		slist_init(&cmd_ctx->buf_list);

		if (cmd->msg.hdr.size) {
			sar_entry = ofi_buf_alloc(ep->pend_buf_pool);
			if (!sar_entry) {
				FI_WARN(&smr_prov, FI_LOG_EP_CTRL,
					"Error allocating sar entry\n");
				ofi_buf_free(cmd_ctx);
				return -FI_ENOMEM;
			}

			memcpy(&sar_entry->cmd, cmd, sizeof(*cmd));
			sar_entry->cmd_ctx = cmd_ctx;
			sar_entry->bytes_done = 0;
			sar_entry->rx_entry = rx_entry;

			dlist_insert_tail(&sar_entry->entry, &ep->sar_list);

			cmd_ctx->sar_entry = sar_entry;
		}
	} else {
		memcpy(&cmd_ctx->cmd, cmd, sizeof(*cmd));
	}

	rx_entry->peer_context = cmd_ctx;
	return FI_SUCCESS;
}

static int smr_progress_cmd_msg(struct smr_ep *ep, struct smr_cmd *cmd)
{
	struct fi_peer_match_attr attr;
	struct fi_peer_rx_entry *rx_entry;
	int ret;

	attr.addr = ep->region->map->peers[cmd->msg.hdr.id].fiaddr;
	attr.msg_size = cmd->msg.hdr.size;
	attr.tag = cmd->msg.hdr.tag;
	if (cmd->msg.hdr.op == ofi_op_tagged) {
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

static int smr_progress_cmd_rma(struct smr_ep *ep, struct smr_cmd *cmd,
				struct smr_cmd *rma_cmd)
{
	struct smr_region *peer_smr;
	struct smr_domain *domain;
	struct smr_resp *resp;
	struct iovec iov[SMR_IOV_LIMIT];
	size_t iov_count;
	size_t total_len = 0;
	int err = 0, ret = 0;
	struct ofi_mr *mr[SMR_IOV_LIMIT];

	domain = container_of(ep->util_ep.domain, struct smr_domain,
			      util_domain);

	ofi_genlock_lock(&domain->util_domain.lock);
	for (iov_count = 0; iov_count < rma_cmd->rma.rma_count; iov_count++) {
		ret = ofi_mr_map_verify(&domain->util_domain.mr_map,
				(uintptr_t *) &(rma_cmd->rma.rma_iov[iov_count].addr),
				rma_cmd->rma.rma_iov[iov_count].len,
				rma_cmd->rma.rma_iov[iov_count].key,
				ofi_rx_mr_reg_flags(cmd->msg.hdr.op, 0),
				(void **) &mr[iov_count]);
		if (ret)
			break;

		iov[iov_count].iov_base = (void *) rma_cmd->rma.rma_iov[iov_count].addr;
		iov[iov_count].iov_len = rma_cmd->rma.rma_iov[iov_count].len;
	}
	ofi_genlock_unlock(&domain->util_domain.lock);

	if (ret)
		goto out;

	switch (cmd->msg.hdr.op_src) {
	case smr_src_inline:
		err = smr_progress_inline(cmd, mr, iov, iov_count, &total_len);
		break;
	case smr_src_inject:
		err = smr_progress_inject(cmd, mr, iov, iov_count, &total_len,
					  ep, ret);
		if (cmd->msg.hdr.op == ofi_op_read_req && cmd->msg.hdr.data) {
			peer_smr = smr_peer_region(ep->region, cmd->msg.hdr.id);
			resp = smr_get_ptr(peer_smr, cmd->msg.hdr.data);
			resp->status = -err;
		}
		break;
	case smr_src_iov:
		err = smr_progress_iov(cmd, iov, iov_count, &total_len, ep);
		break;
	case smr_src_mmap:
		err = smr_progress_mmap(cmd, mr, iov, iov_count, &total_len,
					ep);
		break;
	case smr_src_sar:
		if (smr_progress_sar(cmd, NULL, mr, iov, iov_count, &total_len,
				     ep))
			return ret;
		break;
	case smr_src_ipc:
		if (smr_progress_ipc(cmd, NULL, mr, iov, iov_count, &total_len,
				     ep, &ret))
			return ret;
		break;
	default:
		FI_WARN(&smr_prov, FI_LOG_EP_CTRL,
			"unidentified operation type\n");
		err = -FI_EINVAL;
	}

	if (err) {
		FI_WARN(&smr_prov, FI_LOG_EP_CTRL,
			"error processing rma op\n");
		ret = smr_write_err_comp(ep->util_ep.rx_cq, NULL,
				smr_rx_cq_flags(0, cmd->msg.hdr.op_flags),
				0, -err);
	} else {
		ret = smr_complete_rx(ep, (void *) cmd->msg.hdr.msg_id,
				cmd->msg.hdr.op, smr_rx_cq_flags(0,
				cmd->msg.hdr.op_flags), total_len,
				iov_count ? iov[0].iov_base : NULL,
				cmd->msg.hdr.id, 0, cmd->msg.hdr.data);
	}
	if (ret) {
		FI_WARN(&smr_prov, FI_LOG_EP_CTRL,
		"unable to process rx completion\n");
	}

out:
	return ret;
}

static int smr_progress_cmd_atomic(struct smr_ep *ep, struct smr_cmd *cmd,
				   struct smr_cmd *rma_cmd)
{
	struct smr_region *peer_smr;
	struct smr_domain *domain;
	struct smr_resp *resp;
	struct ofi_mr *mr[SMR_IOV_LIMIT];
	struct fi_ioc ioc[SMR_IOV_LIMIT];
	size_t ioc_count;
	size_t total_len = 0;
	int err = 0, ret = 0;

	domain = container_of(ep->util_ep.domain, struct smr_domain,
			      util_domain);

	ofi_genlock_lock(&domain->util_domain.lock);
	for (ioc_count = 0; ioc_count < rma_cmd->rma.rma_count; ioc_count++) {
		ret = ofi_mr_map_verify(&domain->util_domain.mr_map,
				(uintptr_t *) &(rma_cmd->rma.rma_ioc[ioc_count].addr),
				rma_cmd->rma.rma_ioc[ioc_count].count *
				ofi_datatype_size(cmd->msg.hdr.datatype),
				rma_cmd->rma.rma_ioc[ioc_count].key,
				ofi_rx_mr_reg_flags(cmd->msg.hdr.op,
				cmd->msg.hdr.atomic_op),
				(void **) &mr[ioc_count]);
		if (ret)
			break;

		ioc[ioc_count].addr = (void *) rma_cmd->rma.rma_ioc[ioc_count].addr;
		ioc[ioc_count].count = rma_cmd->rma.rma_ioc[ioc_count].count;
	}
	ofi_genlock_unlock(&domain->util_domain.lock);

	if (ret)
		goto out;

	switch (cmd->msg.hdr.op_src) {
	case smr_src_inline:
		err = smr_progress_inline_atomic(cmd, mr, ioc, ioc_count,
						 &total_len);
		break;
	case smr_src_inject:
		err = smr_progress_inject_atomic(cmd, mr, ioc, ioc_count,
						 &total_len, ep, ret);
		break;
	default:
		FI_WARN(&smr_prov, FI_LOG_EP_CTRL,
			"unidentified operation type\n");
		err = -FI_EINVAL;
	}
	if (cmd->msg.hdr.data) {
		peer_smr = smr_peer_region(ep->region, cmd->msg.hdr.id);
		resp = smr_get_ptr(peer_smr, cmd->msg.hdr.data);
		/*
		 * smr_do_atomic will do memcpy when flags has SMR_RMA_REQ.
		 * Add a memory barrier before updating resp status to ensure
		 * the buffer is ready before the status update.
		 */
		if (cmd->msg.hdr.op_flags & SMR_RMA_REQ)
			ofi_wmb();
		resp->status = -err;
	}

	if (err) {
		FI_WARN(&smr_prov, FI_LOG_EP_CTRL,
			"error processing atomic op\n");
		ret = smr_write_err_comp(ep->util_ep.rx_cq, NULL,
				smr_rx_cq_flags(0, cmd->msg.hdr.op_flags),
				0, err);
	} else {
		ret = smr_complete_rx(ep, NULL, cmd->msg.hdr.op,
				      smr_rx_cq_flags(0, cmd->msg.hdr.op_flags),
				      total_len, ioc_count ? ioc[0].addr : NULL,
				      cmd->msg.hdr.id, 0, cmd->msg.hdr.data);
	}
	if (ret) {
		FI_WARN(&smr_prov, FI_LOG_EP_CTRL,
			"unable to process rx completion\n");
		err = ret;
	}

out:
	return err;
}

static void smr_progress_cmd(struct smr_ep *ep)
{
	struct smr_cmd_entry *ce;
	int ret = 0;
	int64_t pos;

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
	while (1) {
		ret = smr_cmd_queue_head(smr_cmd_queue(ep->region), &ce, &pos);
		if (ret == -FI_ENOENT)
			break;
		switch (ce->cmd.msg.hdr.op) {
		case ofi_op_msg:
		case ofi_op_tagged:
			ret = smr_progress_cmd_msg(ep, &ce->cmd);
			break;
		case ofi_op_write:
		case ofi_op_read_req:
			ret = smr_progress_cmd_rma(ep, &ce->cmd,
				&ce->rma_cmd);
			break;
		case ofi_op_write_async:
		case ofi_op_read_async:
			ofi_ep_peer_rx_cntr_inc(&ep->util_ep,
						ce->cmd.msg.hdr.op);
			break;
		case ofi_op_atomic:
		case ofi_op_atomic_fetch:
		case ofi_op_atomic_compare:
			ret = smr_progress_cmd_atomic(ep, &ce->cmd,
				&ce->rma_cmd);
			break;
		case SMR_OP_MAX + ofi_ctrl_connreq:
			smr_progress_connreq(ep, &ce->cmd);
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
	ofi_genlock_unlock(&ep->util_ep.lock);
}

void smr_progress_ipc_list(struct smr_ep *ep)
{
	struct smr_pend_entry *ipc_entry;
	struct smr_region *peer_smr;
	struct smr_domain *domain;
	enum fi_hmem_iface iface;
	struct dlist_entry *tmp;
	struct smr_resp *resp;
	uint64_t device;
	uint64_t flags;
	void *context;
	int ret;

	domain = container_of(ep->util_ep.domain, struct smr_domain,
			      util_domain);

	/* after the synchronize all operations should be complete */
	dlist_foreach_container_safe(&ep->ipc_cpy_pend_list,
				     struct smr_pend_entry,
				     ipc_entry, entry, tmp) {
		iface = ipc_entry->cmd.msg.data.ipc_info.iface;
		device = ipc_entry->cmd.msg.data.ipc_info.device;
		peer_smr = smr_peer_region(ep->region, ipc_entry->cmd.msg.hdr.id);
		resp = smr_get_ptr(peer_smr, ipc_entry->cmd.msg.hdr.src_data);

		if (ofi_async_copy_query(iface, ipc_entry->async_event))
			continue;

		if (ipc_entry->rx_entry) {
			context = ipc_entry->rx_entry->context;
			flags = smr_rx_cq_flags(ipc_entry->rx_entry->flags,
					ipc_entry->cmd.msg.hdr.op_flags);
		} else {
			context = NULL;
			flags = smr_rx_cq_flags(0, ipc_entry->cmd.msg.hdr.op_flags);
		}

		ret = smr_complete_rx(ep, context, ipc_entry->cmd.msg.hdr.op,
				flags, ipc_entry->cmd.msg.hdr.size,
				ipc_entry->iov[0].iov_base,
				ipc_entry->cmd.msg.hdr.id,
				ipc_entry->cmd.msg.hdr.tag,
				ipc_entry->cmd.msg.hdr.data);
		if (ret) {
			FI_WARN(&smr_prov, FI_LOG_EP_CTRL,
				"unable to process rx completion\n");
		}

		/* indicate that the operation is completed only after we
		 * have confirmed that the write has finished. This is to
		 * ensure that the tx_complete occurs after the sending
		 * buffer is now free to be reused
		 */
		resp->status = SMR_STATUS_SUCCESS;

		ofi_mr_cache_delete(domain->ipc_cache, ipc_entry->ipc_entry);
		ofi_free_async_copy_event(iface, device,
					  ipc_entry->async_event);
		dlist_remove(&ipc_entry->entry);
		if (ipc_entry->rx_entry)
			ep->srx->owner_ops->free_entry(ipc_entry->rx_entry);
		ofi_buf_free(ipc_entry);
	}
}

static void smr_buffer_sar(struct smr_ep *ep, struct smr_region *peer_smr,
		      struct smr_resp *resp, struct smr_pend_entry *sar_entry)
{
	struct smr_sar_buf *sar_buf;
	struct smr_unexp_buf *buf;
	size_t bytes;
	int next_buf = 0;

	while (next_buf < sar_entry->cmd.msg.data.buf_batch_size &&
	       sar_entry->bytes_done < sar_entry->cmd.msg.hdr.size) {
		buf = ofi_buf_alloc(ep->unexp_buf_pool);
		if (!buf) {
			FI_WARN(&smr_prov, FI_LOG_EP_CTRL,
				"Error allocating buffer for unexpected SAR "
				"(-FI_ENOMEM)\n");
			return;
		}
		slist_insert_tail(&buf->entry,
			&sar_entry->cmd_ctx->buf_list);

		sar_buf = smr_freestack_get_entry_from_index(
				smr_sar_pool(ep->region),
				sar_entry->cmd.msg.data.sar[next_buf]);
		bytes = MIN(sar_entry->cmd.msg.hdr.size -
				sar_entry->bytes_done,
				SMR_SAR_SIZE);

		memcpy(buf->buf, sar_buf->buf, bytes);

		sar_entry->bytes_done += bytes;
		next_buf++;
	}
	ofi_wmb();
	resp->status = SMR_STATUS_SAR_EMPTY;
}

static void smr_progress_sar_list(struct smr_ep *ep)
{
	struct smr_region *peer_smr;
	struct smr_pend_entry *sar_entry;
	struct smr_resp *resp;
	struct dlist_entry *tmp;
	void *comp_ctx;
	uint64_t comp_flags;
	int ret;

	ofi_genlock_lock(&ep->util_ep.lock);
	dlist_foreach_container_safe(&ep->sar_list, struct smr_pend_entry,
				     sar_entry, entry, tmp) {
		peer_smr = smr_peer_region(ep->region, sar_entry->cmd.msg.hdr.id);
		resp = smr_get_ptr(peer_smr, sar_entry->cmd.msg.hdr.src_data);
		if (sar_entry->cmd.msg.hdr.op == ofi_op_read_req) {
			smr_try_progress_to_sar(ep, peer_smr, smr_sar_pool(ep->region),
					resp, &sar_entry->cmd, sar_entry->mr,
					sar_entry->iov, sar_entry->iov_count,
					&sar_entry->bytes_done, sar_entry);
		} else {
			if (sar_entry->cmd_ctx) {
				if (resp->status != SMR_STATUS_SAR_FULL)
					continue;
				smr_buffer_sar(ep, peer_smr, resp, sar_entry);
			} else {
				smr_try_progress_from_sar(ep, peer_smr, smr_sar_pool(ep->region),
						resp, &sar_entry->cmd, sar_entry->mr,
						sar_entry->iov,
						sar_entry->iov_count,
						&sar_entry->bytes_done,
						sar_entry);
			}
		}

		if (sar_entry->bytes_done == sar_entry->cmd.msg.hdr.size) {
			if (sar_entry->cmd_ctx) {
				sar_entry->cmd_ctx->sar_entry = NULL;
				dlist_remove(&sar_entry->entry);
				ofi_buf_free(sar_entry);
				continue;
			}

			if (sar_entry->rx_entry) {
				comp_ctx = sar_entry->rx_entry->context;
				comp_flags = smr_rx_cq_flags(
						sar_entry->rx_entry->flags,
						sar_entry->cmd.msg.hdr.op_flags);
			} else {
				comp_ctx = NULL;
				comp_flags = smr_rx_cq_flags(0,
						sar_entry->cmd.msg.hdr.op_flags);
			}
			ret = smr_complete_rx(ep, comp_ctx,
					sar_entry->cmd.msg.hdr.op, comp_flags,
					sar_entry->bytes_done,
					sar_entry->iov[0].iov_base,
					sar_entry->cmd.msg.hdr.id,
					sar_entry->cmd.msg.hdr.tag,
					sar_entry->cmd.msg.hdr.data);
			if (ret) {
				FI_WARN(&smr_prov, FI_LOG_EP_CTRL,
					"unable to process rx completion\n");
			}
			if (sar_entry->rx_entry)
				ep->srx->owner_ops->free_entry(sar_entry->rx_entry);

			dlist_remove(&sar_entry->entry);
			ofi_buf_free(sar_entry);
		}
	}
	ofi_genlock_unlock(&ep->util_ep.lock);
}

void smr_ep_progress(struct util_ep *util_ep)
{
	struct smr_ep *ep;

	ep = container_of(util_ep, struct smr_ep, util_ep);

	if (!ep->region)
		return;

	if (smr_env.use_dsa_sar)
		smr_dsa_progress(ep);
	smr_progress_resp(ep);
	smr_progress_sar_list(ep);
	smr_progress_cmd(ep);

	/* always drive forward the ipc list since the completion is
	 * independent of any action by the provider */
	ep->smr_progress_ipc_list(ep);
}
