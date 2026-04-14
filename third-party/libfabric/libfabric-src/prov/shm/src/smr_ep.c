/*
 * Copyright (c) 2013-2021 Intel Corporation. All rights reserved
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
#include <sys/un.h>

#include "ofi_iov.h"
#include "ofi_hmem.h"
#include "ofi_mr.h"
#include "ofi_mb.h"
#include "smr_signal.h"
#include "smr.h"
#include "smr_dsa.h"
#include "ofi_xpmem.h"

extern struct fi_ops_msg smr_msg_ops, smr_no_recv_msg_ops;
extern struct fi_ops_tagged smr_tag_ops, smr_no_recv_tag_ops;
extern struct fi_ops_rma smr_rma_ops;
extern struct fi_ops_atomic smr_atomic_ops;
DEFINE_LIST(sock_name_list);
pthread_mutex_t sock_list_lock = PTHREAD_MUTEX_INITIALIZER;
int smr_global_ep_idx = 0;

static int smr_setname_internal(struct smr_ep *ep, void *addr, size_t addrlen)
{
	char *name;

	if (addrlen > SMR_NAME_MAX) {
		FI_WARN(&smr_prov, FI_LOG_EP_CTRL,
			"Addrlen exceeds max addrlen (%d)\n", SMR_NAME_MAX);
		return -FI_EINVAL;
	}

	if (ep->region) {
		FI_WARN(&smr_prov, FI_LOG_EP_CTRL,
			"Cannot set name after EP has been enabled\n");
		return -FI_EBUSY;
	}

	name = strdup(addr);
	if (!name)
		return -FI_ENOMEM;

	if (ep->name)
		free((void *) ep->name);
	ep->name = name;
	return 0;
}

int smr_setname(fid_t fid, void *addr, size_t addrlen)
{
	struct smr_ep *ep;

	ep = container_of(fid, struct smr_ep, util_ep.ep_fid.fid);
	ep->user_setname = true;

	return smr_setname_internal(ep, addr, addrlen);
}

int smr_getname(fid_t fid, void *addr, size_t *addrlen)
{
	struct smr_ep *ep;
	int ret = 0;

	ep = container_of(fid, struct smr_ep, util_ep.ep_fid.fid);
	if (!ep->name)
		return -FI_EADDRNOTAVAIL;

	if (!addr || *addrlen == 0 ||
	    snprintf(addr, *addrlen, "%s", ep->name) >= *addrlen)
		ret = -FI_ETOOSMALL;

	*addrlen = strlen(ep->name) + 1;

	if (!ret)
		((char *) addr)[*addrlen - 1] = '\0';

	return ret;
}

static struct fi_ops_cm smr_cm_ops = {
	.size = sizeof(struct fi_ops_cm),
	.setname = smr_setname,
	.getname = smr_getname,
	.getpeer = fi_no_getpeer,
	.connect = fi_no_connect,
	.listen = fi_no_listen,
	.accept = fi_no_accept,
	.reject = fi_no_reject,
	.shutdown = fi_no_shutdown,
};

int smr_ep_getopt(fid_t fid, int level, int optname, void *optval,
		  size_t *optlen)
{
	struct smr_ep *smr_ep =
		container_of(fid, struct smr_ep, util_ep.ep_fid);

	return smr_ep->srx->ep_fid.ops->getopt(&smr_ep->srx->ep_fid.fid, level,
					       optname, optval, optlen);
}

int smr_ep_setopt(fid_t fid, int level, int optname, const void *optval,
		  size_t optlen)
{
	struct smr_ep *smr_ep =
		container_of(fid, struct smr_ep, util_ep.ep_fid);

	if (level != FI_OPT_ENDPOINT)
		return -FI_ENOPROTOOPT;

	if (optname == FI_OPT_MIN_MULTI_RECV) {
		smr_ep->min_multi_recv_size = *(size_t *)optval;
		return FI_SUCCESS;
	}

	if (optname == FI_OPT_CUDA_API_PERMITTED) {
		if (!hmem_ops[FI_HMEM_CUDA].initialized) {
			FI_WARN(&smr_prov, FI_LOG_CORE,
				"Cannot set option FI_OPT_CUDA_API_PERMITTED when cuda library "
				"or cuda device not available\n");
			return -FI_EINVAL;
		}

		/* our CUDA support relies on the ability to call CUDA API */
		return *(bool *)optval ? FI_SUCCESS : -FI_EOPNOTSUPP;
	}

	return -FI_ENOPROTOOPT;
}

static ssize_t smr_ep_cancel(fid_t ep_fid, void *context)
{
	struct smr_ep *ep;

	ep = container_of(ep_fid, struct smr_ep, util_ep.ep_fid);
	return ep->srx->ep_fid.ops->cancel(&ep->srx->ep_fid.fid, context);
}

static struct fi_ops_ep smr_ep_ops = {
	.size = sizeof(struct fi_ops_ep),
	.cancel = smr_ep_cancel,
	.getopt = smr_ep_getopt,
	.setopt = smr_ep_setopt,
	.tx_ctx = fi_no_tx_ctx,
	.rx_ctx = fi_no_rx_ctx,
	.rx_size_left = fi_no_rx_size_left,
	.tx_size_left = fi_no_tx_size_left,
};

static void smr_send_name(struct smr_ep *ep, int64_t id)
{
	struct smr_region *peer_smr;
	struct smr_cmd_entry *ce;
	struct smr_inject_buf *tx_buf;
	int64_t pos;
	int ret;

	peer_smr = smr_peer_region(ep->region, id);

	if (smr_peer_data(ep->region)[id].name_sent)
		return;

	ret = smr_cmd_queue_next(smr_cmd_queue(peer_smr), &ce, &pos);
	if (ret == -FI_ENOENT)
		return;

	tx_buf = smr_get_txbuf(peer_smr);
	if (!tx_buf) {
		smr_cmd_queue_discard(ce, pos);
		return;
	}

	ce->cmd.msg.hdr.op = SMR_OP_MAX + ofi_ctrl_connreq;
	ce->cmd.msg.hdr.id = id;
	ce->cmd.msg.hdr.data = ep->region->pid;

	ce->cmd.msg.hdr.src_data = smr_get_offset(peer_smr, tx_buf);

	ce->cmd.msg.hdr.size = strlen(ep->name) + 1;
	memcpy(tx_buf->data, ep->name, ce->cmd.msg.hdr.size);

	smr_peer_data(ep->region)[id].name_sent = 1;
	smr_cmd_queue_commit(ce, pos);
}

int64_t smr_verify_peer(struct smr_ep *ep, fi_addr_t fi_addr)
{
	int64_t id;
	int ret;

	id = smr_addr_lookup(ep->util_ep.av, fi_addr);
	assert(id < SMR_MAX_PEERS);
	if (id < 0)
		return -1;

	if (smr_peer_data(ep->region)[id].addr.id >= 0)
		return id;

	if (!ep->region->map->peers[id].region) {
		ofi_spin_lock(&ep->region->map->lock);
		ret = smr_map_to_region(&smr_prov, ep->region->map, id);
		ofi_spin_unlock(&ep->region->map->lock);
		if (ret)
			return -1;
	}

	smr_send_name(ep, id);

	return -1;
}

void smr_format_pend_resp(struct smr_tx_entry *pend, struct smr_cmd *cmd,
			  void *context, struct ofi_mr **mr,
			  const struct iovec *iov, uint32_t iov_count,
			  uint64_t op_flags, int64_t id, struct smr_resp *resp)
{
	pend->cmd = *cmd;
	pend->context = context;
	memcpy(pend->iov, iov, sizeof(*iov) * iov_count);
	pend->iov_count = iov_count;
	pend->peer_id = id;
	pend->op_flags = op_flags;
	if (cmd->msg.hdr.op_src != smr_src_sar) {
		pend->bytes_done = 0;
		resp->status = FI_EBUSY;
	}

	if (mr)
		memcpy(pend->mr, mr, sizeof(*mr) * iov_count);
	else
		memset(pend->mr, 0, sizeof(*mr) * iov_count);

	resp->msg_id = (uint64_t) (uintptr_t) pend;
}

void smr_generic_format(struct smr_cmd *cmd, int64_t peer_id, uint32_t op,
			uint64_t tag, uint64_t data, uint64_t op_flags)
{
	cmd->msg.hdr.op = op;
	cmd->msg.hdr.op_flags = op == ofi_op_read_req ? SMR_RMA_REQ : 0;
	cmd->msg.hdr.tag = tag;
	cmd->msg.hdr.id = peer_id;
	cmd->msg.hdr.data = data;

	if (op_flags & FI_REMOTE_CQ_DATA)
		cmd->msg.hdr.op_flags |= SMR_REMOTE_CQ_DATA;
	if (op_flags & FI_COMPLETION)
		cmd->msg.hdr.op_flags |= SMR_TX_COMPLETION;
}

static void smr_format_inline(struct smr_cmd *cmd, struct ofi_mr **mr,
			      const struct iovec *iov, size_t count)
{
	cmd->msg.hdr.op_src = smr_src_inline;
	cmd->msg.hdr.size = ofi_copy_from_mr_iov(cmd->msg.data.msg,
						 SMR_MSG_DATA_LEN, mr,
						 iov, count, 0);
}

static void smr_format_inject(struct smr_cmd *cmd, struct ofi_mr **mr,
		const struct iovec *iov, size_t count, struct smr_region *smr,
		struct smr_inject_buf *tx_buf)
{
	cmd->msg.hdr.op_src = smr_src_inject;
	cmd->msg.hdr.src_data = smr_get_offset(smr, tx_buf);
	cmd->msg.hdr.size = ofi_copy_from_mr_iov(tx_buf->data, SMR_INJECT_SIZE,
						 mr, iov, count, 0);
}

static void smr_format_iov(struct smr_cmd *cmd, const struct iovec *iov,
		size_t count, size_t total_len, struct smr_region *smr,
		struct smr_resp *resp)
{
	cmd->msg.hdr.op_src = smr_src_iov;
	cmd->msg.hdr.src_data = smr_get_offset(smr, resp);
	cmd->msg.data.iov_count = count;
	cmd->msg.hdr.size = total_len;
	memcpy(cmd->msg.data.iov, iov, sizeof(*iov) * count);
}

static int smr_format_ipc(struct smr_cmd *cmd, void *ptr, size_t len,
			  struct smr_region *smr, struct smr_resp *resp,
			  enum fi_hmem_iface iface, uint64_t device)
{
	int ret;
	void *base;

	cmd->msg.hdr.op_src = smr_src_ipc;
	cmd->msg.hdr.src_data = smr_get_offset(smr, resp);
	cmd->msg.hdr.size = len;
	cmd->msg.data.ipc_info.iface = iface;
	cmd->msg.data.ipc_info.device = device;

	ret = ofi_hmem_get_base_addr(cmd->msg.data.ipc_info.iface, ptr,
				     len, &base,
				     &cmd->msg.data.ipc_info.base_length);
	if (ret)
		return ret;

	ret = ofi_hmem_get_handle(cmd->msg.data.ipc_info.iface, base,
				  cmd->msg.data.ipc_info.base_length,
				  (void **)&cmd->msg.data.ipc_info.ipc_handle);
	if (ret)
		return ret;

	cmd->msg.data.ipc_info.base_addr = (uintptr_t) base;
	cmd->msg.data.ipc_info.offset = (uintptr_t) ptr - (uintptr_t) base;

	return FI_SUCCESS;
}

static int smr_format_mmap(struct smr_ep *ep, struct smr_cmd *cmd,
		const struct iovec *iov, size_t count, size_t total_len,
		struct smr_tx_entry *pend, struct smr_resp *resp)
{
	void *mapped_ptr;
	int fd, ret, num;
	uint64_t msg_id;
	struct smr_ep_name *map_name;

	msg_id = ep->msg_id++;
	map_name = calloc(1, sizeof(*map_name));
	if (!map_name) {
		FI_WARN(&smr_prov, FI_LOG_EP_CTRL, "calloc error\n");
		return -FI_ENOMEM;
	}

	pthread_mutex_lock(&ep_list_lock);
	dlist_insert_tail(&map_name->entry, &ep_name_list);
	pthread_mutex_unlock(&ep_list_lock);
	num = smr_mmap_name(map_name->name, ep->name, msg_id);
	if (num < 0) {
		FI_WARN(&smr_prov, FI_LOG_AV, "generating shm file name failed\n");
		ret = -errno;
		goto remove_entry;
	}

	fd = shm_open(map_name->name, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
	if (fd < 0) {
		FI_WARN(&smr_prov, FI_LOG_EP_CTRL, "shm_open error\n");
		ret = -errno;
		goto remove_entry;
	}

	ret = ftruncate(fd, total_len);
	if (ret < 0) {
		FI_WARN(&smr_prov, FI_LOG_EP_CTRL, "ftruncate error\n");
		goto unlink_close;
	}

	mapped_ptr = mmap(NULL, total_len, PROT_READ | PROT_WRITE,
			  MAP_SHARED, fd, 0);
	if (mapped_ptr == MAP_FAILED) {
		FI_WARN(&smr_prov, FI_LOG_EP_CTRL, "mmap error\n");
		ret = -errno;
		goto unlink_close;
	}

	if (cmd->msg.hdr.op != ofi_op_read_req) {
		if (ofi_copy_from_iov(mapped_ptr, total_len, iov, count, 0)
		    != total_len) {
			FI_WARN(&smr_prov, FI_LOG_EP_CTRL, "copy from iov error\n");
			ret = -FI_EIO;
			goto munmap;
		}
		munmap(mapped_ptr, total_len);
	} else {
		pend->map_ptr = mapped_ptr;
	}

	cmd->msg.hdr.op_src = smr_src_mmap;
	cmd->msg.hdr.msg_id = msg_id;
	cmd->msg.hdr.src_data = smr_get_offset(ep->region, resp);
	cmd->msg.hdr.size = total_len;
	pend->map_name = map_name;

	close(fd);
	return 0;

munmap:
	munmap(mapped_ptr, total_len);
unlink_close:
	shm_unlink(map_name->name);
	close(fd);
remove_entry:
	dlist_remove(&map_name->entry);
	free(map_name);
	return ret;
}

size_t smr_copy_to_sar(struct smr_freestack *sar_pool, struct smr_resp *resp,
		       struct smr_cmd *cmd, struct ofi_mr **mr,
		       const struct iovec *iov, size_t count,
		       size_t *bytes_done)
{
	struct smr_sar_buf *sar_buf;
	size_t start = *bytes_done;
	int next_sar_buf = 0;

	if (resp->status != SMR_STATUS_SAR_EMPTY)
		return 0;

	while ((*bytes_done < cmd->msg.hdr.size) &&
			(next_sar_buf < cmd->msg.data.buf_batch_size)) {
		sar_buf = smr_freestack_get_entry_from_index(
				sar_pool, cmd->msg.data.sar[next_sar_buf]);

		*bytes_done += ofi_copy_from_mr_iov(
				sar_buf->buf, SMR_SAR_SIZE, mr, iov, count,
				*bytes_done);

		next_sar_buf++;
	}

	ofi_wmb();

	resp->status = SMR_STATUS_SAR_FULL;

	return *bytes_done - start;
}

size_t smr_copy_from_sar(struct smr_freestack *sar_pool, struct smr_resp *resp,
			 struct smr_cmd *cmd, struct ofi_mr **mr,
			 const struct iovec *iov, size_t count,
			 size_t *bytes_done)
{
	struct smr_sar_buf *sar_buf;
	size_t start = *bytes_done;
	int next_sar_buf = 0;

	if (resp->status != SMR_STATUS_SAR_FULL)
		return 0;

	while ((*bytes_done < cmd->msg.hdr.size) &&
			(next_sar_buf < cmd->msg.data.buf_batch_size)) {
		sar_buf = smr_freestack_get_entry_from_index(
				sar_pool, cmd->msg.data.sar[next_sar_buf]);

		*bytes_done += ofi_copy_to_mr_iov(mr, iov, count, *bytes_done,
				sar_buf->buf, SMR_SAR_SIZE);

		next_sar_buf++;
	}

	ofi_wmb();

	resp->status = SMR_STATUS_SAR_EMPTY;
	return *bytes_done - start;
}

static int smr_format_sar(struct smr_ep *ep, struct smr_cmd *cmd,
		   struct ofi_mr **mr, const struct iovec *iov, size_t count,
		   size_t total_len, struct smr_region *smr,
		   struct smr_region *peer_smr, int64_t id,
		   struct smr_tx_entry *pending, struct smr_resp *resp)
{
	int i, ret;
	uint32_t sar_needed;

	if (peer_smr->max_sar_buf_per_peer == 0)
		return -FI_EAGAIN;

	if (smr_peer_data(ep->region)[id].sar_status) {
		return -FI_EAGAIN;
	}

	sar_needed = (total_len + SMR_SAR_SIZE - 1) / SMR_SAR_SIZE;
	cmd->msg.data.buf_batch_size = MIN(SMR_BUF_BATCH_MAX,
			MIN(peer_smr->max_sar_buf_per_peer, sar_needed));

	pthread_spin_lock(&peer_smr->lock);
	for (i = 0; i < cmd->msg.data.buf_batch_size; i++) {
		if (smr_freestack_isempty(smr_sar_pool(peer_smr))) {
			cmd->msg.data.buf_batch_size = i;
			if (i == 0) {
				pthread_spin_unlock(&peer_smr->lock);
				return -FI_EAGAIN;
			}
			break;
		}

		cmd->msg.data.sar[i] =
			smr_freestack_pop_by_index(smr_sar_pool(peer_smr));
	}
	pthread_spin_unlock(&peer_smr->lock);

	resp->status = SMR_STATUS_SAR_EMPTY;
	cmd->msg.hdr.op_src = smr_src_sar;
	cmd->msg.hdr.src_data = smr_get_offset(smr, resp);
	cmd->msg.hdr.size = total_len;
	pending->bytes_done = 0;

	/* Nothing to copy for 0 byte transfer */
	if (!cmd->msg.hdr.size)
		goto out;

	if (cmd->msg.hdr.op != ofi_op_read_req) {
		if (smr_env.use_dsa_sar && ofi_mr_all_host(mr, count)) {
			ret = smr_dsa_copy_to_sar(ep, smr_sar_pool(peer_smr),
					resp, cmd, iov,	count,
					&pending->bytes_done, pending);
			if (ret != FI_SUCCESS) {
				for (i = cmd->msg.data.buf_batch_size - 1;
				     i >= 0; i--) {
					smr_freestack_push_by_index(
					    smr_sar_pool(peer_smr),
					    cmd->msg.data.sar[i]);
				}
				return -FI_EAGAIN;
			}
		} else {
			smr_copy_to_sar(smr_sar_pool(peer_smr), resp, cmd,
					mr, iov, count, &pending->bytes_done);
		}
	}
out:
	smr_peer_data(smr)[id].sar_status = SMR_STATUS_SAR_FULL;
	return FI_SUCCESS;
}

int smr_select_proto(void **desc, size_t iov_count, bool vma_avail,
		     bool ipc_valid, uint32_t op, uint64_t total_len,
		     uint64_t op_flags)
{
	struct ofi_mr *smr_desc;
	enum fi_hmem_iface iface = FI_HMEM_SYSTEM;
	bool fastcopy_avail = false, use_ipc = false;

	/* Do not inline/inject if IPC is available so device to device
	 * transfer may occur if possible. */
	if (iov_count == 1 && desc && desc[0] && ipc_valid) {
		smr_desc = (struct ofi_mr *) *desc;
		iface = smr_desc->iface;
		use_ipc = ofi_hmem_is_ipc_enabled(iface) &&
				smr_desc->flags & FI_HMEM_DEVICE_ONLY &&
				!(op_flags & FI_INJECT);

		if (smr_desc->flags & OFI_HMEM_DATA_DEV_REG_HANDLE) {
			assert(smr_desc->hmem_data);
			fastcopy_avail = true;
		}
	}

	if (op == ofi_op_read_req) {
		if (use_ipc)
			return smr_src_ipc;
		if (vma_avail && FI_HMEM_SYSTEM == iface)
			return smr_src_iov;
		return smr_src_sar;
	}

	if (fastcopy_avail && total_len <= smr_env.max_gdrcopy_size)
		return total_len <= SMR_MSG_DATA_LEN ? smr_src_inline :
						       smr_src_inject;

	if (op_flags & FI_INJECT) {
		if (op_flags & FI_DELIVERY_COMPLETE)
			return smr_src_sar;
		return total_len <= SMR_MSG_DATA_LEN ?
				smr_src_inline : smr_src_inject;
	}

	if (use_ipc)
		return smr_src_ipc;

	if (total_len > SMR_INJECT_SIZE && vma_avail)
		return smr_src_iov;

	if (op_flags & FI_DELIVERY_COMPLETE)
		return smr_src_sar;

	if (total_len <= SMR_MSG_DATA_LEN)
		return smr_src_inline;

	if (total_len <= SMR_INJECT_SIZE)
		return smr_src_inject;

	if (total_len <= smr_env.sar_threshold)
		return smr_src_sar;

	return smr_src_mmap;
}

static ssize_t smr_do_inline(struct smr_ep *ep, struct smr_region *peer_smr, int64_t id,
			     int64_t peer_id, uint32_t op, uint64_t tag, uint64_t data,
			     uint64_t op_flags, struct ofi_mr **desc,
			     const struct iovec *iov, size_t iov_count, size_t total_len,
			     void *context, struct smr_cmd *cmd)
{
	smr_generic_format(cmd, peer_id, op, tag, data, op_flags);
	smr_format_inline(cmd, desc, iov, iov_count);

	return FI_SUCCESS;
}

static ssize_t smr_do_inject(struct smr_ep *ep, struct smr_region *peer_smr, int64_t id,
			     int64_t peer_id, uint32_t op, uint64_t tag, uint64_t data,
			     uint64_t op_flags, struct ofi_mr **desc,
			     const struct iovec *iov, size_t iov_count, size_t total_len,
			     void *context, struct smr_cmd *cmd)
{
	struct smr_inject_buf *tx_buf;

	tx_buf = smr_get_txbuf(peer_smr);
	if (!tx_buf)
		return -FI_EAGAIN;

	smr_generic_format(cmd, peer_id, op, tag, data, op_flags);
	smr_format_inject(cmd, desc, iov, iov_count, peer_smr, tx_buf);

	return FI_SUCCESS;
}

static ssize_t smr_do_iov(struct smr_ep *ep, struct smr_region *peer_smr, int64_t id,
			  int64_t peer_id, uint32_t op, uint64_t tag, uint64_t data,
			  uint64_t op_flags, struct ofi_mr **desc,
		          const struct iovec *iov, size_t iov_count, size_t total_len,
		          void *context, struct smr_cmd *cmd)
{
	struct smr_resp *resp;
	struct smr_tx_entry *pend;

	if (ofi_cirque_isfull(smr_resp_queue(ep->region)))
		return -FI_EAGAIN;

	resp = ofi_cirque_next(smr_resp_queue(ep->region));
	pend = ofi_freestack_pop(ep->tx_fs);

	smr_generic_format(cmd, peer_id, op, tag, data, op_flags);
	smr_format_iov(cmd, iov, iov_count, total_len, ep->region, resp);
	smr_format_pend_resp(pend, cmd, context, desc, iov,
			     iov_count, op_flags, id, resp);
	ofi_cirque_commit(smr_resp_queue(ep->region));

	return FI_SUCCESS;
}

static ssize_t smr_do_sar(struct smr_ep *ep, struct smr_region *peer_smr, int64_t id,
			  int64_t peer_id, uint32_t op, uint64_t tag, uint64_t data,
			  uint64_t op_flags, struct ofi_mr **desc,
		          const struct iovec *iov, size_t iov_count, size_t total_len,
		          void *context, struct smr_cmd *cmd)
{
	struct smr_resp *resp;
	struct smr_tx_entry *pend;
	int ret;

	if (ofi_cirque_isfull(smr_resp_queue(ep->region)))
		return -FI_EAGAIN;

	resp = ofi_cirque_next(smr_resp_queue(ep->region));
	pend = ofi_freestack_pop(ep->tx_fs);

	smr_generic_format(cmd, peer_id, op, tag, data, op_flags);
	ret = smr_format_sar(ep, cmd, desc, iov, iov_count, total_len,
			     ep->region, peer_smr, id, pend, resp);
	if (ret) {
		ofi_freestack_push(ep->tx_fs, pend);
		return ret;
	}

	smr_format_pend_resp(pend, cmd, context, desc, iov,
			     iov_count, op_flags, id, resp);
	ofi_cirque_commit(smr_resp_queue(ep->region));

	return FI_SUCCESS;
}

static ssize_t smr_do_ipc(struct smr_ep *ep, struct smr_region *peer_smr, int64_t id,
			  int64_t peer_id, uint32_t op, uint64_t tag, uint64_t data,
			  uint64_t op_flags, struct ofi_mr **desc,
		          const struct iovec *iov, size_t iov_count, size_t total_len,
		          void *context, struct smr_cmd *cmd)
{
	struct smr_resp *resp;
	struct smr_tx_entry *pend;
	int ret = -FI_EAGAIN;

	if (ofi_cirque_isfull(smr_resp_queue(ep->region)))
		return -FI_EAGAIN;

	resp = ofi_cirque_next(smr_resp_queue(ep->region));
	pend = ofi_freestack_pop(ep->tx_fs);

	smr_generic_format(cmd, peer_id, op, tag, data, op_flags);
	assert(iov_count == 1 && desc && desc[0]);
	ret = smr_format_ipc(cmd, iov[0].iov_base, total_len, ep->region,
			     resp, desc[0]->iface, desc[0]->device);

	if (ret) {
		FI_WARN_ONCE(&smr_prov, FI_LOG_EP_CTRL,
			     "unable to use IPC for msg, fallback to using SAR\n");
		ofi_freestack_push(ep->tx_fs, pend);
		return smr_do_sar(ep, peer_smr, id, peer_id, op, tag, data,
				  op_flags, desc, iov, iov_count,
				  total_len, context, cmd);
	}

	smr_format_pend_resp(pend, cmd, context, desc, iov,
			     iov_count, op_flags, id, resp);
	ofi_cirque_commit(smr_resp_queue(ep->region));

	return FI_SUCCESS;
}

static ssize_t smr_do_mmap(struct smr_ep *ep, struct smr_region *peer_smr, int64_t id,
			   int64_t peer_id, uint32_t op, uint64_t tag, uint64_t data,
			   uint64_t op_flags, struct ofi_mr **desc,
		           const struct iovec *iov, size_t iov_count, size_t total_len,
		           void *context, struct smr_cmd *cmd)
{
	struct smr_resp *resp;
	struct smr_tx_entry *pend;
	int ret;

	if (ofi_cirque_isfull(smr_resp_queue(ep->region)))
		return -FI_EAGAIN;

	resp = ofi_cirque_next(smr_resp_queue(ep->region));
	pend = ofi_freestack_pop(ep->tx_fs);

	smr_generic_format(cmd, peer_id, op, tag, data, op_flags);
	ret = smr_format_mmap(ep, cmd, iov, iov_count, total_len, pend, resp);
	if (ret) {
		ofi_freestack_push(ep->tx_fs, pend);
		return ret;
	}

	smr_format_pend_resp(pend, cmd, context, desc, iov,
			     iov_count, op_flags, id, resp);
	ofi_cirque_commit(smr_resp_queue(ep->region));

	return FI_SUCCESS;
}

smr_proto_func smr_proto_ops[smr_src_max] = {
	[smr_src_inline] = &smr_do_inline,
	[smr_src_inject] = &smr_do_inject,
	[smr_src_iov] = &smr_do_iov,
	[smr_src_mmap] = &smr_do_mmap,
	[smr_src_sar] = &smr_do_sar,
	[smr_src_ipc] = &smr_do_ipc,
};

static void smr_cleanup_epoll(struct smr_sock_info *sock_info)
{
	fd_signal_free(&sock_info->signal);
	ofi_epoll_close(sock_info->epollfd);
}

static void smr_free_sock_info(struct smr_ep *ep)
{
	int i, j;

	for (i = 0; i < SMR_MAX_PEERS; i++) {
		if (!ep->sock_info->peers[i].device_fds)
			continue;
		for (j = 0; j < ep->sock_info->nfds; j++)
			close(ep->sock_info->peers[i].device_fds[j]);
		free(ep->sock_info->peers[i].device_fds);
	}
	free(ep->sock_info);
	ep->sock_info = NULL;
}

static int smr_ep_close(struct fid *fid)
{
	struct smr_ep *ep;
	struct smr_pend_entry *pend;

	ep = container_of(fid, struct smr_ep, util_ep.ep_fid.fid);

	if (smr_env.use_dsa_sar)
		smr_dsa_context_cleanup(ep);

	if (ep->sock_info) {
		fd_signal_set(&ep->sock_info->signal);
		pthread_join(ep->sock_info->listener_thread, NULL);
		close(ep->sock_info->listen_sock);
		unlink(ep->sock_info->name);
		smr_cleanup_epoll(ep->sock_info);
		smr_free_sock_info(ep);
	}

	ofi_genlock_lock(&ep->util_ep.lock);
	while (!dlist_empty(&ep->sar_list)) {
		dlist_pop_front(&ep->sar_list, struct smr_pend_entry, pend,
				entry);
		if (pend->rx_entry && !pend->cmd_ctx)
			ep->srx->owner_ops->free_entry(pend->rx_entry);
		ofi_buf_free(pend);
	}
	ofi_genlock_unlock(&ep->util_ep.lock);

	if (ep->srx) {
		/* shm is an owner provider */
		if (ep->util_ep.ep_fid.msg != &smr_no_recv_msg_ops)
			(void) util_srx_close(&ep->srx->ep_fid.fid);
	}

	ofi_endpoint_close(&ep->util_ep);

	if (ep->region)
		smr_free(ep->region);

	if (ep->cmd_ctx_pool)
		ofi_bufpool_destroy(ep->cmd_ctx_pool);

	if (ep->unexp_buf_pool)
		ofi_bufpool_destroy(ep->unexp_buf_pool);

	if (ep->pend_buf_pool)
		ofi_bufpool_destroy(ep->pend_buf_pool);

	smr_tx_fs_free(ep->tx_fs);

	free((void *)ep->name);
	free(ep);
	return 0;
}

static int smr_ep_trywait(void *arg)
{
	struct smr_ep *ep;

	ep = container_of(arg, struct smr_ep, util_ep.ep_fid.fid);

	smr_ep_progress(&ep->util_ep);

	return FI_SUCCESS;
}

static int smr_ep_bind_cq(struct smr_ep *ep, struct util_cq *cq, uint64_t flags)
{
	int ret;

	ret = ofi_ep_bind_cq(&ep->util_ep, cq, flags);
	if (ret)
		return ret;

	if (cq->wait) {
		ret = ofi_wait_add_fid(cq->wait, &ep->util_ep.ep_fid.fid, 0,
				       smr_ep_trywait);
		if (ret)
			return ret;
	}

	return ret;
}

static int smr_ep_bind_cntr(struct smr_ep *ep, struct util_cntr *cntr, uint64_t flags)
{
	int ret;

	ret = ofi_ep_bind_cntr(&ep->util_ep, cntr, flags);
	if (ret)
		return ret;

	if (cntr->wait) {
		ret = ofi_wait_add_fid(cntr->wait, &ep->util_ep.ep_fid.fid, 0,
				       smr_ep_trywait);
		if (ret)
			return ret;
	}

	return FI_SUCCESS;
}

static int smr_sendmsg_fd(int sock, int64_t id, int64_t peer_id,
			  int *fds, int nfds)
{
	struct msghdr msg;
	struct cmsghdr *cmsg;
	struct iovec iov;
	char *ctrl_buf;
	size_t ctrl_size;
	int ret;

	ctrl_size = sizeof(*fds) * nfds;
	ctrl_buf = calloc(CMSG_SPACE(ctrl_size), 1);
	if (!ctrl_buf)
		return -FI_ENOMEM;

	iov.iov_base = &peer_id;
	iov.iov_len = sizeof(peer_id);

	memset(&msg, 0, sizeof(msg));
	msg.msg_control = ctrl_buf;
	msg.msg_controllen = CMSG_SPACE(ctrl_size);
	msg.msg_iov = &iov;
	msg.msg_iovlen = 1;

	cmsg = CMSG_FIRSTHDR(&msg);
	cmsg->cmsg_level = SOL_SOCKET;
	cmsg->cmsg_type = SCM_RIGHTS;
	cmsg->cmsg_len = CMSG_LEN(ctrl_size);
	memcpy(CMSG_DATA(cmsg), fds, ctrl_size);

	ret = sendmsg(sock, &msg, 0);
	if (ret == sizeof(peer_id)) {
		ret = FI_SUCCESS;
	} else {
		FI_WARN(&smr_prov, FI_LOG_EP_CTRL, "sendmsg error\n");
		ret = -FI_EIO;
	}

	free(ctrl_buf);
	return ret;
}

static int smr_recvmsg_fd(int sock, int64_t *peer_id, int *fds, int nfds)
{
	struct msghdr msg;
	struct cmsghdr *cmsg;
	struct iovec iov;
	char *ctrl_buf;
	size_t ctrl_size;
	int ret;

	ctrl_size = sizeof(*fds) * nfds;
	ctrl_buf = calloc(CMSG_SPACE(ctrl_size), 1);
	if (!ctrl_buf)
		return -FI_ENOMEM;

	iov.iov_base = peer_id;
	iov.iov_len = sizeof(*peer_id);

	memset(&msg, 0, sizeof(msg));
	msg.msg_control = ctrl_buf;
	msg.msg_controllen = CMSG_SPACE(ctrl_size);
	msg.msg_iov = &iov;
	msg.msg_iovlen = 1;

	ret = recvmsg(sock, &msg, 0);
	if (ret == sizeof(*peer_id)) {
		ret = FI_SUCCESS;
	} else {
		FI_WARN(&smr_prov, FI_LOG_EP_CTRL, "recvmsg error\n");
		ret = -FI_EIO;
		goto out;
	}

	assert(!(msg.msg_flags & (MSG_TRUNC | MSG_CTRUNC)));
	cmsg = CMSG_FIRSTHDR(&msg);
	assert(cmsg && cmsg->cmsg_len == CMSG_LEN(ctrl_size) &&
	       cmsg->cmsg_level == SOL_SOCKET &&
	       cmsg->cmsg_type == SCM_RIGHTS && CMSG_DATA(cmsg));
	memcpy(fds, CMSG_DATA(cmsg), ctrl_size);
out:
	free(ctrl_buf);
	return ret;
}

void smr_ep_exchange_fds(struct smr_ep *ep, int64_t id)
{
	struct smr_region *peer_smr = smr_peer_region(ep->region, id);
	struct sockaddr_un server_sockaddr = {0}, client_sockaddr = {0};
	int ret = -1, sock = -1;
	int64_t peer_id;

	if (peer_smr->pid == ep->region->pid ||
	    !(peer_smr->flags & SMR_FLAG_IPC_SOCK))
		goto out;

	sock = socket(AF_UNIX, SOCK_STREAM, 0);
	if (sock < 0)
		goto out;

	client_sockaddr.sun_family = AF_UNIX;

	ret = bind(sock, (struct sockaddr *) &client_sockaddr,
		  (socklen_t) sizeof(client_sockaddr));
	if (ret == -1) {
		if (errno != EADDRINUSE) {
			FI_WARN(&smr_prov, FI_LOG_EP_CTRL, "bind error\n");
			ep->sock_info->peers[id].state = SMR_CMAP_FAILED;
		}
		close(sock);
		return;
	}

	server_sockaddr.sun_family = AF_UNIX;

	ret = connect(sock, (struct sockaddr *) &server_sockaddr,
		      sizeof(server_sockaddr));
	if (ret == -1)
		goto cleanup;

	FI_DBG(&smr_prov, FI_LOG_EP_CTRL, "EP connected to UNIX socket %s\n",
	       server_sockaddr.sun_path);

	peer_id = smr_peer_data(ep->region)[id].addr.id;
	ret = smr_sendmsg_fd(sock, id, peer_id, ep->sock_info->my_fds,
			     ep->sock_info->nfds);
	if (ret)
		goto cleanup;

	if (!ep->sock_info->peers[id].device_fds) {
		ep->sock_info->peers[id].device_fds =
			calloc(ep->sock_info->nfds,
			       sizeof(*ep->sock_info->peers[id].device_fds));
		if (!ep->sock_info->peers[id].device_fds)
			goto cleanup;
	}
	ret = smr_recvmsg_fd(sock, &id, ep->sock_info->peers[id].device_fds,
			     ep->sock_info->nfds);
	if (ret)
		goto cleanup;

cleanup:
	close(sock);
	unlink(client_sockaddr.sun_path);
out:
	ep->sock_info->peers[id].state = ret ?
		SMR_CMAP_FAILED : SMR_CMAP_SUCCESS;
}

static int smr_discard(struct fi_peer_rx_entry *rx_entry)
{
	struct smr_cmd_ctx *cmd_ctx = rx_entry->peer_context;
	struct smr_region *peer_smr;
	struct smr_resp *resp;

	if (cmd_ctx->cmd.msg.hdr.src_data >= smr_src_iov) {
		peer_smr = smr_peer_region(cmd_ctx->ep->region,
					   cmd_ctx->cmd.msg.hdr.id);
		resp = smr_get_ptr(peer_smr, cmd_ctx->cmd.msg.hdr.src_data);
		resp->status = SMR_STATUS_SUCCESS;
	}

	ofi_buf_free(cmd_ctx);

	return FI_SUCCESS;
}

struct fi_ops_srx_peer smr_srx_peer_ops = {
	.size = sizeof(struct fi_ops_srx_peer),
	.start_msg = smr_unexp_start,
	.start_tag = smr_unexp_start,
	.discard_msg = smr_discard,
	.discard_tag = smr_discard,
};

static void smr_update(struct util_srx_ctx *srx, struct util_rx_entry *rx_entry)
{
	//no update needed - shm only used as the owner when not used as a peer
	//by another provider
}

static int smr_ep_bind(struct fid *ep_fid, struct fid *bfid, uint64_t flags)
{
	struct smr_ep *ep;
	struct util_av *av;
	int ret = 0;

	ep = container_of(ep_fid, struct smr_ep, util_ep.ep_fid.fid);
	switch (bfid->fclass) {
	case FI_CLASS_AV:
		av = container_of(bfid, struct util_av, av_fid.fid);
		ret = ofi_ep_bind_av(&ep->util_ep, av);
		if (ret) {
			FI_WARN(&smr_prov, FI_LOG_EP_CTRL,
				"duplicate AV binding\n");
			return -FI_EINVAL;
		}
		break;
	case FI_CLASS_CQ:
		ret = smr_ep_bind_cq(ep, container_of(bfid, struct util_cq,
						      cq_fid.fid), flags);
		break;
	case FI_CLASS_EQ:
		break;
	case FI_CLASS_CNTR:
		ret = smr_ep_bind_cntr(ep, container_of(bfid,
				struct util_cntr, cntr_fid.fid), flags);
		break;
	case FI_CLASS_SRX_CTX:
		ep->srx = (container_of(bfid, struct smr_domain, rx_ep.fid))->srx;
		break;
	default:
		FI_WARN(&smr_prov, FI_LOG_EP_CTRL, "invalid fid class\n");
		ret = -FI_EINVAL;
		break;
	}
	return ret;
}

static int smr_ep_ctrl(struct fid *fid, int command, void *arg)
{
	struct smr_attr attr;
	struct smr_domain *domain;
	struct smr_ep *ep;
	struct smr_av *av;
	struct fid_ep *srx;
	char tmp_name[SMR_NAME_MAX];
	int ret;

	ep = container_of(fid, struct smr_ep, util_ep.ep_fid.fid);
	av = container_of(ep->util_ep.av, struct smr_av, util_av);

	switch (command) {
	case FI_ENABLE:
		if ((ofi_needs_rx(ep->util_ep.caps) && !ep->util_ep.rx_cq) ||
		    (ofi_needs_tx(ep->util_ep.caps) && !ep->util_ep.tx_cq))
			return -FI_ENOCQ;
		if (!ep->util_ep.av)
			return -FI_ENOAV;

		attr.rx_count = ep->rx_size;
		attr.tx_count = ep->tx_size;
		attr.flags = ep->util_ep.caps & FI_HMEM ?
				SMR_FLAG_HMEM_ENABLED : 0;

create_shm:
		attr.name = smr_no_prefix(ep->name);
		ret = smr_create(&smr_prov, &av->smr_map, &attr, &ep->region);
		if (ret == -FI_EBUSY && !ep->user_setname) {
			snprintf(tmp_name, SMR_NAME_MAX - 1, "%s:%u",
				 ep->name, ofi_generate_seed());
			FI_WARN(&smr_prov, FI_LOG_EP_CTRL,
				"SMR %s busy, retry with name %s\n",
				ep->name, tmp_name);

			if (strcmp(tmp_name, ep->name) &&
			    !smr_setname_internal(ep, tmp_name, SMR_NAME_MAX))
				goto create_shm;
		}

		if (ep->util_ep.caps & FI_HMEM || smr_env.disable_cma) {
			ep->region->cma_cap_peer = SMR_VMA_CAP_OFF;
			ep->region->cma_cap_self = SMR_VMA_CAP_OFF;
		}

		if (ofi_hmem_any_ipc_enabled())
			ep->smr_progress_ipc_list = smr_progress_ipc_list;
		else
			ep->smr_progress_ipc_list = smr_progress_ipc_list_noop;

		if (!ep->srx) {
			domain = container_of(ep->util_ep.domain,
					      struct smr_domain,
					      util_domain.domain_fid);
			ret = util_ep_srx_context(&domain->util_domain,
					ep->rx_size, SMR_IOV_LIMIT,
					ep->min_multi_recv_size, &smr_update,
					&ep->util_ep.lock, &srx);
			if (ret)
				return ret;

			ep->srx = container_of(srx, struct fid_peer_srx,
					       ep_fid.fid);
			ep->srx->peer_ops = &smr_srx_peer_ops;

			ret = util_srx_bind(&ep->srx->ep_fid.fid,
					    &ep->util_ep.rx_cq->cq_fid.fid,
					    FI_RECV);
			if (ret)
				return ret;
		} else {
			ep->util_ep.ep_fid.msg = &smr_no_recv_msg_ops;
			ep->util_ep.ep_fid.tagged = &smr_no_recv_tag_ops;
		}
		smr_exchange_all_peers(ep->region);

		if (smr_env.use_dsa_sar)
			smr_dsa_context_init(ep);

		/* if XPMEM is on after exchanging peer info, then set the
		 * endpoint p2p to XPMEM so it can be used on the fast
		 * path
		 */
		if (ep->region->xpmem_cap_self == SMR_VMA_CAP_ON)
			ep->p2p_type = FI_SHM_P2P_XPMEM;

		break;
	default:
		return -FI_ENOSYS;
	}
	return ret;
}

static struct fi_ops smr_ep_fi_ops = {
	.size = sizeof(struct fi_ops),
	.close = smr_ep_close,
	.bind = smr_ep_bind,
	.control = smr_ep_ctrl,
	.ops_open = fi_no_ops_open,
};

static int smr_endpoint_name(struct smr_ep *ep, char *name, char *addr,
			     size_t addrlen)
{
	memset(name, 0, SMR_NAME_MAX);
	if (!addr || addrlen > SMR_NAME_MAX)
		return -FI_EINVAL;

	pthread_mutex_lock(&ep_list_lock);
	ep->ep_idx = smr_global_ep_idx++;
	pthread_mutex_unlock(&ep_list_lock);

	if (strstr(addr, SMR_PREFIX))
		snprintf(name, SMR_NAME_MAX - 1, "%s:%d:%d", addr, getuid(),
			 ep->ep_idx);
	else
		snprintf(name, SMR_NAME_MAX - 1, "%s", addr);

	return 0;
}

static void smr_init_sig_handlers(void)
{
	static bool sig_init = false;

	pthread_mutex_lock(&ep_list_lock);
	if (sig_init)
		goto out;

	/* Signal handlers to cleanup tmpfs files on an unclean shutdown */
	assert(SIGBUS < SIGRTMIN && SIGSEGV < SIGRTMIN
	       && SIGTERM < SIGRTMIN && SIGINT < SIGRTMIN);
	smr_reg_sig_handler(SIGBUS);
	smr_reg_sig_handler(SIGSEGV);
	smr_reg_sig_handler(SIGTERM);
	smr_reg_sig_handler(SIGINT);

	sig_init = true;
out:
	pthread_mutex_unlock(&ep_list_lock);
}

static int smr_create_pools(struct smr_ep *ep, struct fi_info *info)
{
	int ret;

	ret = ofi_bufpool_create(&ep->cmd_ctx_pool, sizeof(struct smr_cmd_ctx),
				 16, 0, info->rx_attr->size,
				 OFI_BUFPOOL_NO_TRACK);
	if (ret)
		goto err;

	ret = ofi_bufpool_grow(ep->cmd_ctx_pool);
	if (ret)
		goto free2;

	ret = ofi_bufpool_create(&ep->unexp_buf_pool,
				 sizeof(struct smr_unexp_buf),
				 16, 0, 4, OFI_BUFPOOL_NO_TRACK);
	if (ret)
		goto free2;

	ret = ofi_bufpool_create(&ep->pend_buf_pool,
				 sizeof(struct smr_pend_entry),
				 16, 0, 4, OFI_BUFPOOL_NO_TRACK);
	if (ret)
		goto free1;

	return FI_SUCCESS;
free1:
	ofi_bufpool_destroy(ep->unexp_buf_pool);
free2:
	ofi_bufpool_destroy(ep->cmd_ctx_pool);
err:
	FI_WARN(&smr_prov, FI_LOG_EP_CTRL,
		"Unable to allocate buf pools for EP\n");
	return ret;
}

int smr_endpoint(struct fid_domain *domain, struct fi_info *info,
		  struct fid_ep **ep_fid, void *context)
{
	struct smr_ep *ep;
	int ret;
	char name[SMR_NAME_MAX];

	smr_init_sig_handlers();

	ep = calloc(1, sizeof(*ep));
	if (!ep)
		return -FI_ENOMEM;

	ret = smr_endpoint_name(ep, name, info->src_addr, info->src_addrlen);
	if (ret)
		goto free;

	ret = smr_setname_internal(ep, name, SMR_NAME_MAX);
	if (ret)
		goto free;

	ep->rx_size = info->rx_attr->size;
	ep->tx_size = info->tx_attr->size;
	ret = ofi_endpoint_init(domain, &smr_util_prov, info, &ep->util_ep, context,
				smr_ep_progress);
	if (ret)
		goto name;

	ep->util_ep.ep_fid.msg = &smr_msg_ops;
	ep->util_ep.ep_fid.tagged = &smr_tag_ops;

	ret = smr_create_pools(ep, info);
	if (ret)
		goto ep;

	ep->tx_fs = smr_tx_fs_create(info->tx_attr->size, NULL, NULL);

	dlist_init(&ep->sar_list);
	dlist_init(&ep->ipc_cpy_pend_list);

	ep->min_multi_recv_size = SMR_INJECT_SIZE;

	ep->util_ep.ep_fid.fid.ops = &smr_ep_fi_ops;
	ep->util_ep.ep_fid.ops = &smr_ep_ops;
	ep->util_ep.ep_fid.cm = &smr_cm_ops;
	ep->util_ep.ep_fid.rma = &smr_rma_ops;
	ep->util_ep.ep_fid.atomic = &smr_atomic_ops;

	*ep_fid = &ep->util_ep.ep_fid;

	/* default to CMA for p2p */
	ep->p2p_type = FI_SHM_P2P_CMA;
	return 0;
ep:
	ofi_endpoint_close(&ep->util_ep);
name:
	free((void *)ep->name);
free:
	free(ep);
	return ret;
}
