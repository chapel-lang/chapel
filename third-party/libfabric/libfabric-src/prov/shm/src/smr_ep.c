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
#include "smr_signal.h"
#include "ofi_mb.h"

extern struct fi_ops_msg smr_msg_ops, smr_no_recv_msg_ops;
extern struct fi_ops_tagged smr_tag_ops, smr_no_recv_tag_ops;
extern struct fi_ops_rma smr_rma_ops;
extern struct fi_ops_atomic smr_atomic_ops;
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
				"Cannot set option FI_OPT_CUDA_API_PERMITTED "
				"when cuda library or cuda device "
				"not available\n");
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
	int64_t pos;
	int ret;

	peer_smr = smr_peer_region(ep, id);

	if (smr_peer_data(ep->region)[id].name_sent)
		return;

	ret = smr_cmd_queue_next(smr_cmd_queue(peer_smr), &ce, &pos);
	if (ret == -FI_ENOENT)
		return;

	ce->ptr = smr_peer_to_peer(ep, peer_smr, id, (uintptr_t) &ce->cmd);
	ce->cmd.hdr.op = SMR_OP_MAX + ofi_ctrl_connreq;
	ce->cmd.hdr.tx_id = id;
	ce->cmd.hdr.cq_data = ep->region->pid;

	ce->cmd.hdr.size = strlen(ep->name) + 1;
	memcpy(ce->cmd.data.msg, ep->name, ce->cmd.hdr.size);

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

	if (smr_peer_data(ep->region)[id].id >= 0)
		return id;

	if (!ep->map->peers[id].region) {
		ofi_genlock_lock(&ep->util_ep.av->lock);
		ret = smr_map_to_region(ep->map, id);
		ofi_genlock_unlock(&ep->util_ep.av->lock);
		if (ret)
			return -1;
	}

	smr_send_name(ep, id);

	return -1;
}

void smr_format_tx_pend(struct smr_pend_entry *pend, struct smr_cmd *cmd,
			void *context, struct ofi_mr **mr,
			const struct iovec *iov, uint32_t iov_count,
			uint64_t op_flags)
{
	pend->type = SMR_TX_ENTRY;
	pend->cmd = cmd;
	pend->comp_ctx = context;
	pend->comp_flags = op_flags;

	memcpy(pend->iov, iov, sizeof(*iov) * iov_count);
	pend->iov_count = iov_count;
	pend->bytes_done = 0;

	if (mr)
		memcpy(pend->mr, mr, sizeof(*mr) * iov_count);
	else
		memset(pend->mr, 0, sizeof(*mr) * iov_count);
}

void smr_generic_format(struct smr_cmd *cmd, int64_t tx_id, int64_t rx_id,
			uint32_t op, uint64_t tag, uint64_t data,
			uint64_t op_flags)
{
	cmd->hdr.op = op;
	cmd->hdr.status = 0;
	cmd->hdr.op_flags = 0;
	cmd->hdr.tag = tag;
	cmd->hdr.tx_id = tx_id;
	cmd->hdr.rx_id = rx_id;
	cmd->hdr.cq_data = data;
	cmd->hdr.rx_ctx = 0;

	if (op_flags & FI_REMOTE_CQ_DATA)
		cmd->hdr.op_flags |= SMR_REMOTE_CQ_DATA;
}

static void smr_format_inline(struct smr_cmd *cmd, struct ofi_mr **mr,
			      const struct iovec *iov, size_t count)
{
	cmd->hdr.proto = smr_proto_inline;
	cmd->hdr.size = ofi_copy_from_mr_iov(cmd->data.msg, SMR_MSG_DATA_LEN,
					     mr, iov, count, 0);
}

static void smr_format_inject(struct smr_ep *ep, struct smr_cmd *cmd,
			      struct smr_pend_entry *pend)
{
	struct smr_inject_buf *tx_buf;

	tx_buf = smr_get_inject_buf(ep->region, cmd);

	cmd->hdr.proto = smr_proto_inject;
	if (cmd->hdr.op != ofi_op_read_req) {
		cmd->hdr.size = ofi_copy_from_mr_iov(tx_buf->data,
						     SMR_INJECT_SIZE,
						     pend->mr, pend->iov,
						     pend->iov_count, 0);
		pend->bytes_done = cmd->hdr.size;
	} else {
		cmd->hdr.size = ofi_total_iov_len(pend->iov, pend->iov_count);
		pend->bytes_done = 0;
	}
}

static void smr_format_iov(struct smr_cmd *cmd, struct smr_pend_entry *pend)
{
	cmd->hdr.proto = smr_proto_iov;
	cmd->data.iov_count = pend->iov_count;
	cmd->hdr.size = ofi_total_iov_len(pend->iov, pend->iov_count);
	memcpy(cmd->data.iov, pend->iov, sizeof(*pend->iov) * pend->iov_count);
}

static int smr_format_ipc(struct smr_cmd *cmd, void *ptr, size_t len,
			  struct smr_region *smr,
			  enum fi_hmem_iface iface, uint64_t device)
{
	int ret;
	void *base;
	size_t base_length;

	cmd->hdr.proto = smr_proto_ipc;
	cmd->hdr.size = len;
	cmd->data.ipc_info.iface = iface;
	cmd->data.ipc_info.device = device;

	ret = ofi_hmem_get_base_addr(cmd->data.ipc_info.iface, ptr,
				     len, &base, &base_length);
	if (ret)
		return ret;

	cmd->data.ipc_info.base_length = (uint64_t)base_length;

	ret = ofi_hmem_get_handle(cmd->data.ipc_info.iface, base,
				  cmd->data.ipc_info.base_length,
				  (void **)&cmd->data.ipc_info.ipc_handle);
	if (ret)
		return ret;

	cmd->data.ipc_info.base_addr = (uintptr_t) base;
	cmd->data.ipc_info.offset = (uintptr_t) ptr - (uintptr_t) base;

	return FI_SUCCESS;
}

ssize_t smr_copy_sar(struct smr_ep *ep, struct smr_pend_entry *pend)
{
	struct smr_freestack *sar_pool;
	struct smr_sar_buf *sar_buf;
	int next_sar_buf = 0;

	sar_pool = smr_pend_sar_pool(ep, pend);
	while (pend->bytes_done < pend->cmd->hdr.size &&
	       next_sar_buf < pend->cmd->data.buf_batch_size) {
		sar_buf = smr_freestack_get_entry_from_index(
				sar_pool, pend->cmd->data.sar[next_sar_buf]);

		pend->bytes_done += ofi_copy_mr_iov(
					pend->mr, pend->iov, pend->iov_count,
					pend->bytes_done, sar_buf->buf,
					SMR_SAR_SIZE, pend->sar_dir);
		next_sar_buf++;
	}

	return FI_SUCCESS;
}

static int smr_format_sar(struct smr_ep *ep, struct smr_cmd *cmd,
			  struct ofi_mr **mr, const struct iovec *iov,
			  size_t count, size_t total_len,
			  struct smr_region *smr, struct smr_region *peer_smr,
			  struct smr_pend_entry *pend)
{
	int i, ret;

	if (ep->region->max_sar_buf_per_peer == 0 ||
	    smr_peer_data(ep->region)[cmd->hdr.tx_id].sar_status)
		return -FI_EAGAIN;

	cmd->data.buf_batch_size = MIN3(
				ep->region->max_sar_buf_per_peer,
				(total_len + SMR_SAR_SIZE - 1) / SMR_SAR_SIZE,
				smr_freestack_avail(smr_sar_pool(ep->region)));

	for (i = 0; i < cmd->data.buf_batch_size; i++) {
		cmd->data.sar[i] =
			smr_freestack_pop_by_index(smr_sar_pool(ep->region));
	}

	cmd->hdr.proto = smr_proto_sar;
	cmd->hdr.size = total_len;

	if (cmd->hdr.op != ofi_op_read_req) {
		ret = pend->sar_copy_fn(ep, pend);
		if (ret < 0 && ret != -FI_EBUSY) {
			for (i = cmd->data.buf_batch_size - 1; i >= 0; i--) {
				smr_freestack_push_by_index(
						smr_sar_pool(ep->region),
						cmd->data.sar[i]);
			}
			return -FI_EAGAIN;
		}
		smr_peer_data(ep->region)[cmd->hdr.tx_id].sar_status =
			      pend->sar_copy_fn == &smr_copy_sar ?
			      SMR_SAR_READY : SMR_SAR_BUSY;
	} else {
		smr_peer_data(ep->region)[cmd->hdr.tx_id].sar_status =
								SMR_SAR_READY;
	}

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
			return smr_proto_ipc;
		if (total_len <= SMR_INJECT_SIZE)
			return smr_proto_inject;
		if (vma_avail && FI_HMEM_SYSTEM == iface)
			return smr_proto_iov;
		return smr_proto_sar;
	}

	if (fastcopy_avail && total_len <= smr_env.max_gdrcopy_size)
		return total_len <= SMR_MSG_DATA_LEN ? smr_proto_inline :
						       smr_proto_inject;

	if (use_ipc && !(op_flags & FI_INJECT))
		return smr_proto_ipc;

	if (op_flags & FI_INJECT || total_len <= SMR_INJECT_SIZE) {
		if (op_flags & FI_DELIVERY_COMPLETE)
			return smr_proto_inject;
		return total_len <= SMR_MSG_DATA_LEN ?
				smr_proto_inline : smr_proto_inject;
	}

	return vma_avail ? smr_proto_iov: smr_proto_sar;
}

static ssize_t smr_do_inline(struct smr_ep *ep, struct smr_region *peer_smr,
			     int64_t tx_id, int64_t rx_id, uint32_t op,
			     uint64_t tag, uint64_t data, uint64_t op_flags,
			     struct ofi_mr **desc, const struct iovec *iov,
			     size_t iov_count, size_t total_len, void *context,
			     struct smr_cmd *cmd)
{
	cmd->hdr.tx_ctx = 0;
	smr_generic_format(cmd, tx_id, rx_id, op, tag, data, op_flags);
	smr_format_inline(cmd, desc, iov, iov_count);

	return FI_SUCCESS;
}

static ssize_t smr_do_inject(struct smr_ep *ep, struct smr_region *peer_smr,
			     int64_t tx_id, int64_t rx_id, uint32_t op,
			     uint64_t tag, uint64_t data, uint64_t op_flags,
			     struct ofi_mr **desc, const struct iovec *iov,
			     size_t iov_count, size_t total_len, void *context,
			     struct smr_cmd *cmd)
{
	struct smr_pend_entry *pend;

	pend = ofi_buf_alloc(ep->pend_pool);
	assert(pend);

	cmd->hdr.tx_ctx = (uintptr_t) pend;
	smr_format_tx_pend(pend, cmd, context, desc, iov, iov_count, op_flags);

	smr_generic_format(cmd, tx_id, rx_id, op, tag, data, op_flags);
	smr_format_inject(ep, cmd, pend);

	if (smr_freestack_avail(smr_cmd_stack(ep->region)) <=
	    smr_env.buffer_threshold)
		cmd->hdr.op_flags |= SMR_BUFFER_RECV;

	return FI_SUCCESS;
}

static ssize_t smr_do_iov(struct smr_ep *ep, struct smr_region *peer_smr,
			  int64_t tx_id, int64_t rx_id, uint32_t op,
			  uint64_t tag, uint64_t data, uint64_t op_flags,
			  struct ofi_mr **desc, const struct iovec *iov,
			  size_t iov_count, size_t total_len, void *context,
			  struct smr_cmd *cmd)
{
	struct smr_pend_entry *pend;

	pend = ofi_buf_alloc(ep->pend_pool);
	assert(pend);

	cmd->hdr.tx_ctx = (uintptr_t) pend;
	smr_format_tx_pend(pend, cmd, context, desc, iov, iov_count, op_flags);

	smr_generic_format(cmd, tx_id, rx_id, op, tag, data, op_flags);
	smr_format_iov(cmd, pend);

	if (smr_freestack_avail(smr_cmd_stack(ep->region)) <=
	    smr_env.buffer_threshold)
		cmd->hdr.op_flags |= SMR_BUFFER_RECV;

	return FI_SUCCESS;
}

static ssize_t smr_do_sar(struct smr_ep *ep, struct smr_region *peer_smr,
			  int64_t tx_id, int64_t rx_id, uint32_t op,
			  uint64_t tag, uint64_t data, uint64_t op_flags,
			  struct ofi_mr **desc, const struct iovec *iov,
			  size_t iov_count, size_t total_len, void *context,
			  struct smr_cmd *cmd)
{
	struct smr_pend_entry *pend;
	int ret;

	pend = ofi_buf_alloc(ep->pend_pool);
	assert(pend);

	cmd->hdr.tx_ctx = (uintptr_t) pend;
	smr_format_tx_pend(pend, cmd, context, desc, iov, iov_count, op_flags);

	pend->sar_dir = op == ofi_op_read_req ?
			OFI_COPY_BUF_TO_IOV : OFI_COPY_IOV_TO_BUF;

	if (smr_env.use_dsa_sar && ofi_mr_all_host(pend->mr, pend->iov_count))
		pend->sar_copy_fn = &smr_dsa_copy_sar;
	else
		pend->sar_copy_fn = &smr_copy_sar;

	smr_generic_format(cmd, tx_id, rx_id, op, tag, data, op_flags);
	ret = smr_format_sar(ep, cmd, desc, iov, iov_count, total_len,
			     ep->region, peer_smr, pend);
	if (ret)
		ofi_buf_free(pend);

	return ret;
}

static ssize_t smr_do_ipc(struct smr_ep *ep, struct smr_region *peer_smr,
			  int64_t tx_id, int64_t rx_id, uint32_t op,
			  uint64_t tag, uint64_t data, uint64_t op_flags,
			  struct ofi_mr **desc, const struct iovec *iov,
			  size_t iov_count, size_t total_len, void *context,
			  struct smr_cmd *cmd)
{
	struct smr_pend_entry *pend;
	int ret = -FI_EAGAIN;

	pend = ofi_buf_alloc(ep->pend_pool);
	assert(pend);

	cmd->hdr.tx_ctx = (uintptr_t) pend;
	smr_generic_format(cmd, tx_id, rx_id, op, tag, data, op_flags);
	assert(iov_count == 1 && desc && desc[0]);
	ret = smr_format_ipc(cmd, iov[0].iov_base, total_len, ep->region,
			     desc[0]->iface, desc[0]->device);

	if (ret) {
		FI_WARN_ONCE(&smr_prov, FI_LOG_EP_CTRL,
			     "unable to use IPC for msg, "
			     "fallback to using SAR\n");
		ofi_buf_free(pend);
		return smr_do_sar(ep, peer_smr, tx_id, rx_id, op, tag, data,
				  op_flags, desc, iov, iov_count,
				  total_len, context, cmd);
	}

	smr_format_tx_pend(pend, cmd, context, desc, iov, iov_count, op_flags);

	if (smr_freestack_avail(smr_cmd_stack(ep->region)) <=
	    smr_env.buffer_threshold)
		cmd->hdr.op_flags |= SMR_BUFFER_RECV;

	return FI_SUCCESS;
}

smr_send_func smr_send_ops[smr_proto_max] = {
	[smr_proto_inline] = &smr_do_inline,
	[smr_proto_inject] = &smr_do_inject,
	[smr_proto_iov] = &smr_do_iov,
	[smr_proto_sar] = &smr_do_sar,
	[smr_proto_ipc] = &smr_do_ipc,
};

static int smr_ep_close(struct fid *fid)
{
	struct smr_ep *ep;
	struct smr_pend_entry *pend;

	ep = container_of(fid, struct smr_ep, util_ep.ep_fid.fid);

	if (smr_env.use_dsa_sar)
		smr_dsa_context_cleanup(ep);

	ofi_genlock_lock(&ep->util_ep.lock);
	while (!dlist_empty(&ep->sar_list)) {
		dlist_pop_front(&ep->sar_list, struct smr_pend_entry, pend,
				entry);
		if (pend->rx_entry)
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

	if (ep->pend_pool)
		ofi_bufpool_destroy(ep->pend_pool);

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

static int smr_ep_bind_cntr(struct smr_ep *ep, struct util_cntr *cntr,
			    uint64_t flags)
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

static int smr_discard(struct fi_peer_rx_entry *rx_entry)
{
	struct smr_cmd_ctx *cmd_ctx = rx_entry->peer_context;
	struct smr_unexp_buf *sar_buf;

	switch (cmd_ctx->cmd->hdr.proto) {
	case smr_proto_inline:
		break;
	case smr_proto_sar:
		while (!slist_empty(&cmd_ctx->buf_list)) {
			slist_remove_head_container(
					&cmd_ctx->buf_list,
					struct smr_unexp_buf, sar_buf,
					entry);
			ofi_buf_free(sar_buf);
		}
		break;
	case smr_proto_inject:
	case smr_proto_iov:
	case smr_proto_ipc:
		smr_return_cmd(cmd_ctx->ep, cmd_ctx->cmd);
		break;
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
		ep->map = &container_of(av, struct smr_av, util_av)->smr_map;
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
		ep->srx = container_of(bfid, struct smr_domain, rx_ep.fid)->srx;
		break;
	default:
		FI_WARN(&smr_prov, FI_LOG_EP_CTRL, "invalid fid class\n");
		ret = -FI_EINVAL;
		break;
	}
	return ret;
}

static void smr_ep_map_all_peers(struct smr_ep *ep)
{
	int64_t i;

	ofi_genlock_lock(&ep->util_ep.av->lock);
	for (i = 0; i < SMR_MAX_PEERS; i++)
		smr_map_to_endpoint(ep, i);

	ofi_genlock_unlock(&ep->util_ep.av->lock);
}

static int smr_ep_ctrl(struct fid *fid, int command, void *arg)
{
	struct smr_attr attr;
	struct smr_domain *domain;
	struct smr_ep *ep;
	struct fid_ep *srx;
	char tmp_name[SMR_NAME_MAX];
	int ret;

	ep = container_of(fid, struct smr_ep, util_ep.ep_fid.fid);

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
		attr.flags |= smr_env.use_xpmem ? SMR_FLAG_XPMEM_ENABLED : 0;

create_shm:
		attr.name = smr_no_prefix(ep->name);
		ret = smr_create(&smr_prov, &attr, &ep->region);
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
			smr_set_vma_cap(&ep->region->peer_vma_caps,
					FI_SHM_P2P_CMA, false);
			smr_set_vma_cap(&ep->region->self_vma_caps,
					FI_SHM_P2P_CMA, false);
			ep->region->flags |= SMR_FLAG_CMA_INIT;
		}

		if (ofi_hmem_any_ipc_enabled()) {
			ep->smr_progress_async = smr_progress_async;
		} else {
#if SHM_HAVE_DSA
			ep->smr_progress_async = smr_progress_async;
#else
			ep->smr_progress_async =
						smr_progress_async_noop;
#endif
		}

		if (!ep->srx) {
			domain = container_of(ep->util_ep.domain,
					      struct smr_domain,
					      util_domain.domain_fid);
			ret = util_ep_srx_context(
					&domain->util_domain,
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
		smr_ep_map_all_peers(ep);

		if (smr_env.use_dsa_sar)
			smr_dsa_context_init(ep);

		/* if XPMEM is on after exchanging peer info, then set the
		 * endpoint p2p to XPMEM so it can be used on the fast path
		 */
		if (smr_get_vma_cap(ep->region->self_vma_caps,
				    FI_SHM_P2P_XPMEM))
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

	ret = ofi_bufpool_create(&ep->pend_pool,
				 sizeof(struct smr_pend_entry),
				 16, 0, ep->tx_size, OFI_BUFPOOL_NO_TRACK);
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
	ret = ofi_endpoint_init(domain, &smr_util_prov, info, &ep->util_ep,
				context, smr_ep_progress);
	if (ret)
		goto name;

	ep->util_ep.ep_fid.msg = &smr_msg_ops;
	ep->util_ep.ep_fid.tagged = &smr_tag_ops;

	ret = smr_create_pools(ep, info);
	if (ret)
		goto ep;

	dlist_init(&ep->sar_list);
	dlist_init(&ep->unexp_cmd_list);
	dlist_init(&ep->async_cpy_list);
	slist_init(&ep->overflow_list);

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
