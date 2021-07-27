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
#include "smr.h"

extern struct fi_ops_msg smr_msg_ops;
extern struct fi_ops_tagged smr_tagged_ops;
extern struct fi_ops_rma smr_rma_ops;
extern struct fi_ops_atomic smr_atomic_ops;
DEFINE_LIST(sock_name_list);
pthread_mutex_t sock_list_lock = PTHREAD_MUTEX_INITIALIZER;

int smr_setname(fid_t fid, void *addr, size_t addrlen)
{
	struct smr_ep *ep;
	char *name;

	ep = container_of(fid, struct smr_ep, util_ep.ep_fid.fid);
	name = strdup(addr);
	if (!name)
		return -FI_ENOMEM;

	if (ep->name)
		free((void *) ep->name);
	ep->name = name;
	return 0;
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

int smr_getopt(fid_t fid, int level, int optname,
	       void *optval, size_t *optlen)
{
	struct smr_ep *smr_ep =
		container_of(fid, struct smr_ep, util_ep.ep_fid);

	if ((level != FI_OPT_ENDPOINT) || (optname != FI_OPT_MIN_MULTI_RECV))
		return -FI_ENOPROTOOPT;

	*(size_t *)optval = smr_ep->min_multi_recv_size;
	*optlen = sizeof(size_t);

	return FI_SUCCESS;
}

int smr_setopt(fid_t fid, int level, int optname,
	       const void *optval, size_t optlen)
{
	struct smr_ep *smr_ep =
		container_of(fid, struct smr_ep, util_ep.ep_fid);

	if ((level != FI_OPT_ENDPOINT) || (optname != FI_OPT_MIN_MULTI_RECV))
		return -FI_ENOPROTOOPT;

	smr_ep->min_multi_recv_size = *(size_t *)optval;

	return FI_SUCCESS;
}

static int smr_match_recv_ctx(struct dlist_entry *item, const void *args)
{
	struct smr_rx_entry *pending_recv;

	pending_recv = container_of(item, struct smr_rx_entry, entry);
	return pending_recv->context == args;
}

static int smr_ep_cancel_recv(struct smr_ep *ep, struct smr_queue *queue,
			      void *context)
{
	struct smr_rx_entry *recv_entry;
	struct dlist_entry *entry;
	int ret = 0;

	fastlock_acquire(&ep->util_ep.rx_cq->cq_lock);
	entry = dlist_remove_first_match(&queue->list, smr_match_recv_ctx,
					 context);
	if (entry) {
		recv_entry = container_of(entry, struct smr_rx_entry, entry);
		ret = smr_complete_rx(ep, (void *) recv_entry->context, ofi_op_msg,
				  recv_entry->flags, 0,
				  NULL, recv_entry->peer_id,
				  recv_entry->tag, 0, FI_ECANCELED);
		ofi_freestack_push(ep->recv_fs, recv_entry);
		ret = ret ? ret : 1;
	}

	fastlock_release(&ep->util_ep.rx_cq->cq_lock);
	return ret;
}

static ssize_t smr_ep_cancel(fid_t ep_fid, void *context)
{
	struct smr_ep *ep;
	int ret;

	ep = container_of(ep_fid, struct smr_ep, util_ep.ep_fid);

	ret = smr_ep_cancel_recv(ep, &ep->trecv_queue, context);
	if (ret)
		return (ret < 0) ? ret : 0;

	ret = smr_ep_cancel_recv(ep, &ep->recv_queue, context);
	return (ret < 0) ? ret : 0;
}

static struct fi_ops_ep smr_ep_ops = {
	.size = sizeof(struct fi_ops_ep),
	.cancel = smr_ep_cancel,
	.getopt = smr_getopt,
	.setopt = smr_setopt,
	.tx_ctx = fi_no_tx_ctx,
	.rx_ctx = fi_no_rx_ctx,
	.rx_size_left = fi_no_rx_size_left,
	.tx_size_left = fi_no_tx_size_left,
};

static void smr_send_name(struct smr_ep *ep, int64_t id)
{
	struct smr_region *peer_smr;
	struct smr_cmd *cmd;
	struct smr_inject_buf *tx_buf;

	peer_smr = smr_peer_region(ep->region, id);

	fastlock_acquire(&peer_smr->lock);

	if (smr_peer_data(ep->region)[id].name_sent || !peer_smr->cmd_cnt)
		goto out;

	cmd = ofi_cirque_next(smr_cmd_queue(peer_smr));

	cmd->msg.hdr.op = SMR_OP_MAX + ofi_ctrl_connreq;
	cmd->msg.hdr.id = id;

	tx_buf = smr_freestack_pop(smr_inject_pool(peer_smr));
	cmd->msg.hdr.src_data = smr_get_offset(peer_smr, tx_buf);

	cmd->msg.hdr.size = strlen(smr_name(ep->region)) + 1;
	memcpy(tx_buf->data, smr_name(ep->region), cmd->msg.hdr.size);

	smr_peer_data(ep->region)[id].name_sent = 1;
	ofi_cirque_commit(smr_cmd_queue(peer_smr));
	peer_smr->cmd_cnt--;

out:
	fastlock_release(&peer_smr->lock);
}

int64_t smr_verify_peer(struct smr_ep *ep, fi_addr_t fi_addr)
{
	int64_t id;
	int ret;

	id = smr_addr_lookup(ep->util_ep.av, fi_addr);
	assert(id < SMR_MAX_PEERS);

	if (smr_peer_data(ep->region)[id].addr.id >= 0)
		return id;

	if (ep->region->map->peers[id].peer.id < 0) {
		ret = smr_map_to_region(&smr_prov, &ep->region->map->peers[id]);
		if (ret == -ENOENT)
			return -1;

	}

	smr_send_name(ep, id);

	return -1;
}

static int smr_match_msg(struct dlist_entry *item, const void *args)
{
	struct smr_match_attr *attr = (struct smr_match_attr *)args;
	struct smr_rx_entry *recv_entry;

	recv_entry = container_of(item, struct smr_rx_entry, entry);
	return smr_match_id(recv_entry->peer_id, attr->id);
}

static int smr_match_tagged(struct dlist_entry *item, const void *args)
{
	struct smr_match_attr *attr = (struct smr_match_attr *)args;
	struct smr_rx_entry *recv_entry;

	recv_entry = container_of(item, struct smr_rx_entry, entry);
	return smr_match_id(recv_entry->peer_id, attr->id) &&
	       smr_match_tag(recv_entry->tag, recv_entry->ignore, attr->tag);
}

static int smr_match_unexp_msg(struct dlist_entry *item, const void *args)
{
	struct smr_match_attr *attr = (struct smr_match_attr *)args;
	struct smr_unexp_msg *unexp_msg;

	unexp_msg = container_of(item, struct smr_unexp_msg, entry);
	assert(unexp_msg->cmd.msg.hdr.op == ofi_op_msg);
	return smr_match_id(unexp_msg->cmd.msg.hdr.id, attr->id);
}

static int smr_match_unexp_tagged(struct dlist_entry *item, const void *args)
{
	struct smr_match_attr *attr = (struct smr_match_attr *)args;
	struct smr_unexp_msg *unexp_msg;

	unexp_msg = container_of(item, struct smr_unexp_msg, entry);
	if (unexp_msg->cmd.msg.hdr.op == ofi_op_msg)
		return smr_match_id(unexp_msg->cmd.msg.hdr.id, attr->id);

	assert(unexp_msg->cmd.msg.hdr.op == ofi_op_tagged);
	return smr_match_id(unexp_msg->cmd.msg.hdr.id, attr->id) &&
	       smr_match_tag(unexp_msg->cmd.msg.hdr.tag, attr->ignore,
			     attr->tag);
}

static void smr_init_queue(struct smr_queue *queue,
			   dlist_func_t *match_func)
{
	dlist_init(&queue->list);
	queue->match_func = match_func;
}

void smr_format_pend_resp(struct smr_tx_entry *pend, struct smr_cmd *cmd,
			  void *context, enum fi_hmem_iface iface, uint64_t device,
			  const struct iovec *iov, uint32_t iov_count,
			  int64_t id, struct smr_resp *resp)
{
	pend->cmd = *cmd;
	pend->context = context;
	memcpy(pend->iov, iov, sizeof(*iov) * iov_count);
	pend->iov_count = iov_count;
	pend->peer_id = id;
	if (cmd->msg.hdr.op_src != smr_src_sar)
		pend->bytes_done = 0;

	pend->iface = iface;
	pend->device = device;

	resp->msg_id = (uint64_t) (uintptr_t) pend;
	resp->status = FI_EBUSY;
}

void smr_generic_format(struct smr_cmd *cmd, int64_t peer_id, uint32_t op,
			uint64_t tag, uint64_t data, uint64_t op_flags)
{
	cmd->msg.hdr.op = op;
	cmd->msg.hdr.op_flags = 0;
	cmd->msg.hdr.tag = tag;
	cmd->msg.hdr.id = peer_id;
	cmd->msg.hdr.data = data;

	if (op_flags & FI_REMOTE_CQ_DATA)
		cmd->msg.hdr.op_flags |= SMR_REMOTE_CQ_DATA;
	if (op_flags & FI_COMPLETION)
		cmd->msg.hdr.op_flags |= SMR_TX_COMPLETION;
}

void smr_format_inline(struct smr_cmd *cmd, enum fi_hmem_iface iface,
		       uint64_t device, const struct iovec *iov, size_t count)
{
	cmd->msg.hdr.op_src = smr_src_inline;
	cmd->msg.hdr.size = ofi_copy_from_hmem_iov(cmd->msg.data.msg,
						SMR_MSG_DATA_LEN, iface, device,
						iov, count, 0);
}

void smr_format_inject(struct smr_cmd *cmd, enum fi_hmem_iface iface, uint64_t device,
		       const struct iovec *iov, size_t count,
		       struct smr_region *smr, struct smr_inject_buf *tx_buf)
{
	cmd->msg.hdr.op_src = smr_src_inject;
	cmd->msg.hdr.src_data = smr_get_offset(smr, tx_buf);
	cmd->msg.hdr.size = ofi_copy_from_hmem_iov(tx_buf->data, SMR_INJECT_SIZE,
						   iface, device, iov, count, 0);
}

void smr_format_iov(struct smr_cmd *cmd, const struct iovec *iov, size_t count,
		    size_t total_len, struct smr_region *smr,
		    struct smr_resp *resp)
{
	cmd->msg.hdr.op_src = smr_src_iov;
	cmd->msg.hdr.src_data = smr_get_offset(smr, resp);
	cmd->msg.data.iov_count = count;
	cmd->msg.hdr.size = total_len;
	memcpy(cmd->msg.data.iov, iov, sizeof(*iov) * count);
}

int smr_format_ze_ipc(struct smr_ep *ep, int64_t id, struct smr_cmd *cmd,
		      const struct iovec *iov, uint64_t device,
		      size_t total_len, struct smr_region *smr,
		      struct smr_resp *resp, struct smr_tx_entry *pend)
{
	int ret;
	void *base;

	cmd->msg.hdr.op_src = smr_src_ipc;
	cmd->msg.hdr.src_data = smr_get_offset(smr, resp);
	cmd->msg.hdr.size = total_len;
	cmd->msg.data.ipc_info.iface = FI_HMEM_ZE;

	if (ep->sock_info->peers[id].state == SMR_CMAP_INIT)
		smr_ep_exchange_fds(ep, id);
	if (ep->sock_info->peers[id].state != SMR_CMAP_SUCCESS)
		return -FI_EAGAIN;

	ret = ze_hmem_get_base_addr(iov[0].iov_base, &base);
	if (ret)
		return ret;

	ret = ze_hmem_get_shared_handle(ep->sock_info->my_fds[device],
			base, &pend->fd,
			(void **) &cmd->msg.data.ipc_info.fd_handle);
	if (ret)
		return ret;

	cmd->msg.data.ipc_info.device = device;
	cmd->msg.data.ipc_info.offset = (char *) iov[0].iov_base -
					(char *) base;

	return FI_SUCCESS;
}

int smr_format_mmap(struct smr_ep *ep, struct smr_cmd *cmd,
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

size_t smr_copy_to_sar(struct smr_sar_msg *sar_msg, struct smr_resp *resp,
		       struct smr_cmd *cmd, enum fi_hmem_iface iface,
		       uint64_t device, const struct iovec *iov, size_t count,
		       size_t *bytes_done, int *next)
{
	size_t start = *bytes_done;

	if (sar_msg->sar[0].status == SMR_SAR_FREE && !*next) {
		*bytes_done += ofi_copy_from_hmem_iov(sar_msg->sar[0].buf,
					SMR_SAR_SIZE, iface, device,
					iov, count, *bytes_done);
		sar_msg->sar[0].status = SMR_SAR_READY;
		if (cmd->msg.hdr.op == ofi_op_read_req)
			resp->status = FI_SUCCESS;
		*next = 1;
	}

	if (*bytes_done < cmd->msg.hdr.size &&
	    sar_msg->sar[1].status == SMR_SAR_FREE && *next) {
		*bytes_done += ofi_copy_from_hmem_iov(sar_msg->sar[1].buf,
					SMR_SAR_SIZE, iface, device,
					iov, count, *bytes_done);
		sar_msg->sar[1].status = SMR_SAR_READY;
		if (cmd->msg.hdr.op == ofi_op_read_req)
			resp->status = FI_SUCCESS;
		*next = 0;
	}
	return *bytes_done - start;
}

size_t smr_copy_from_sar(struct smr_sar_msg *sar_msg, struct smr_resp *resp,
			 struct smr_cmd *cmd, enum fi_hmem_iface iface,
			 uint64_t device, const struct iovec *iov, size_t count,
			 size_t *bytes_done, int *next)
{
	size_t start = *bytes_done;

	if (sar_msg->sar[0].status == SMR_SAR_READY && !*next) {
		*bytes_done += ofi_copy_to_hmem_iov(iface, device, iov, count,
					*bytes_done, sar_msg->sar[0].buf,
					SMR_SAR_SIZE);
		sar_msg->sar[0].status = SMR_SAR_FREE;
		if (cmd->msg.hdr.op != ofi_op_read_req)
			resp->status = FI_SUCCESS;
		*next = 1;
	}

	if (*bytes_done < cmd->msg.hdr.size &&
	    sar_msg->sar[1].status == SMR_SAR_READY && *next) {
		*bytes_done += ofi_copy_to_hmem_iov(iface, device, iov, count,
					*bytes_done, sar_msg->sar[1].buf,
					SMR_SAR_SIZE);
		sar_msg->sar[1].status = SMR_SAR_FREE;
		if (cmd->msg.hdr.op != ofi_op_read_req)
			resp->status = FI_SUCCESS;
		*next = 0;
	}
	return *bytes_done - start;
}

void smr_format_sar(struct smr_cmd *cmd, enum fi_hmem_iface iface, uint64_t device,
		    const struct iovec *iov, size_t count,
		    size_t total_len, struct smr_region *smr,
		    struct smr_region *peer_smr, struct smr_sar_msg *sar_msg,
		    struct smr_tx_entry *pending, struct smr_resp *resp)
{
	cmd->msg.hdr.op_src = smr_src_sar;
	cmd->msg.hdr.src_data = smr_get_offset(smr, resp);
	cmd->msg.data.sar = smr_get_offset(peer_smr, sar_msg);
	cmd->msg.hdr.size = total_len;

	pending->bytes_done = 0;
	pending->next = 0;
	sar_msg->sar[0].status = SMR_SAR_FREE;
	sar_msg->sar[1].status = SMR_SAR_FREE;
	if (cmd->msg.hdr.op != ofi_op_read_req)
		smr_copy_to_sar(sar_msg, NULL, cmd, iface, device ,iov, count,
				&pending->bytes_done, &pending->next);
}

static void smr_cleanup_epoll(struct smr_sock_info *sock_info)
{
	fd_signal_free(&sock_info->signal);
	ofi_epoll_close(sock_info->epollfd);
}

static int smr_ep_close(struct fid *fid)
{
	struct smr_ep *ep;

	ep = container_of(fid, struct smr_ep, util_ep.ep_fid.fid);

	if (ep->sock_info) {
		fd_signal_set(&ep->sock_info->signal);
		pthread_join(ep->sock_info->listener_thread, NULL);
		close(ep->sock_info->listen_sock);
		unlink(ep->sock_info->name);
		smr_cleanup_epoll(ep->sock_info);
		free(ep->sock_info);
	}

	ofi_endpoint_close(&ep->util_ep);

	if (ep->region)
		smr_free(ep->region);

	smr_recv_fs_free(ep->recv_fs);
	smr_unexp_fs_free(ep->unexp_fs);
	smr_pend_fs_free(ep->pend_fs);
	smr_sar_fs_free(ep->sar_fs);
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

	if (flags & FI_TRANSMIT) {
		ep->tx_comp = cq->wait ? smr_tx_comp_signal : smr_tx_comp;
	}

	if (flags & FI_RECV) {
		if (cq->wait) {
			ep->rx_comp = (cq->domain->info_domain_caps & FI_SOURCE) ?
				      smr_rx_src_comp_signal :
				      smr_rx_comp_signal;
		} else {
			ep->rx_comp = (cq->domain->info_domain_caps & FI_SOURCE) ?
				      smr_rx_src_comp : smr_rx_comp;
		}
	}

	if (cq->wait) {
		ret = ofi_wait_add_fid(cq->wait, &ep->util_ep.ep_fid.fid, 0,
				       smr_ep_trywait);
		if (ret)
			return ret;
	}

	ret = fid_list_insert(&cq->ep_list,
			      &cq->ep_list_lock,
			      &ep->util_ep.ep_fid.fid);

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
	default:
		FI_WARN(&smr_prov, FI_LOG_EP_CTRL,
			"invalid fid class\n");
		ret = -FI_EINVAL;
		break;
	}
	return ret;
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
	       cmsg->cmsg_type == SCM_RIGHTS);
	memcpy(fds, CMSG_DATA(cmsg), ctrl_size);
out:
	free(ctrl_buf);
	return ret;
}

static void *smr_start_listener(void *args)
{
	struct smr_ep *ep = (struct smr_ep *) args;
	struct sockaddr_un sockaddr;
	void *ctx[SMR_MAX_PEERS + 1];
	int i, ret, poll_fds, sock = -1;
	int peer_fds[ZE_MAX_DEVICES];
	socklen_t len;
	int64_t id, peer_id;

	ep->region->flags |= SMR_FLAG_IPC_SOCK;
	while (1) {
		poll_fds = ofi_epoll_wait(ep->sock_info->epollfd, ctx,
					  SMR_MAX_PEERS + 1, -1);

		if (poll_fds < 0) {
			FI_WARN(&smr_prov, FI_LOG_EP_CTRL,
				"epoll error\n");
			continue;
		}

		for (i = 0; i < poll_fds; i++) {
			if (!ctx[i])
				goto out;

			sock = accept(ep->sock_info->listen_sock,
				      (struct sockaddr *) &sockaddr, &len);
			if (sock < 0) {
				FI_WARN(&smr_prov, FI_LOG_EP_CTRL,
					"accept error\n");
				continue;
			}

			FI_DBG(&smr_prov, FI_LOG_EP_CTRL,
			       "EP accepted connection request from %s\n",
			       sockaddr.sun_path);

			ret = smr_recvmsg_fd(sock, &id, peer_fds,
					     ep->sock_info->nfds);
			if (!ret) {
				memcpy(ep->sock_info->peers[id].device_fds,
				       peer_fds, sizeof(*peer_fds) *
				       ep->sock_info->nfds);

				peer_id = smr_peer_data(ep->region)[id].addr.id;
				ret = smr_sendmsg_fd(sock, id, peer_id,
						ep->sock_info->my_fds,
						ep->sock_info->nfds);
				ep->sock_info->peers[id].state =
					ret ? SMR_CMAP_FAILED :
					SMR_CMAP_SUCCESS;
			}

			close(sock);
			unlink(sockaddr.sun_path);
		}
	}
out:
	close(ep->sock_info->listen_sock);
	unlink(ep->sock_info->name);
	return NULL;
}

static int smr_init_epoll(struct smr_sock_info *sock_info)
{
	int ret;

	ret = ofi_epoll_create(&sock_info->epollfd);
	if (ret < 0)
		return ret;

	ret = fd_signal_init(&sock_info->signal);
	if (ret < 0)
		goto err2;

	ret = ofi_epoll_add(sock_info->epollfd,
	                    sock_info->signal.fd[FI_READ_FD],
	                    OFI_EPOLL_IN, NULL);
	if (ret != 0)
		goto err1;

	ret = ofi_epoll_add(sock_info->epollfd, sock_info->listen_sock,
			    OFI_EPOLL_IN, sock_info);
	if (ret != 0)
		goto err1;

	return FI_SUCCESS;
err1:
	ofi_epoll_close(sock_info->epollfd);
err2:
	fd_signal_free(&sock_info->signal);
	return ret;
}

void smr_ep_exchange_fds(struct smr_ep *ep, int64_t id)
{
	struct smr_region *peer_smr = smr_peer_region(ep->region, id);
	struct sockaddr_un server_sockaddr = {0}, client_sockaddr = {0};
	char *name1, *name2;
	int ret = -1, sock = -1;
	int64_t peer_id;
	int peer_fds[ZE_MAX_DEVICES];

	if (peer_smr->pid == ep->region->pid ||
	    !(peer_smr->flags & SMR_FLAG_IPC_SOCK))
		goto out;

	sock = socket(AF_UNIX, SOCK_STREAM, 0);
	if (sock < 0)
		goto out;

	if (strcmp(smr_sock_name(ep->region), smr_sock_name(peer_smr)) < 1) {
		name1 = smr_sock_name(ep->region);
		name2 = smr_sock_name(peer_smr);
	} else {
		name1 = smr_sock_name(peer_smr);
		name2 = smr_sock_name(ep->region);
	}
	client_sockaddr.sun_family = AF_UNIX;
	snprintf(client_sockaddr.sun_path, SMR_SOCK_NAME_MAX, "%s%s:%s",
		 SMR_ZE_SOCK_PATH, name1, name2);

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
	snprintf(server_sockaddr.sun_path, SMR_SOCK_NAME_MAX, "%s%s",
		 SMR_ZE_SOCK_PATH, smr_sock_name(peer_smr));

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

	ret = smr_recvmsg_fd(sock, &id, peer_fds, ep->sock_info->nfds);
	if (ret)
		goto cleanup;

	memcpy(ep->sock_info->peers[id].device_fds, peer_fds,
	       sizeof(*peer_fds) * ep->sock_info->nfds);

cleanup:
	close(sock);
	unlink(client_sockaddr.sun_path);
out:
	ep->sock_info->peers[id].state = ret ?
		SMR_CMAP_FAILED : SMR_CMAP_SUCCESS;
}

static void smr_init_ipc_socket(struct smr_ep *ep)
{
	struct smr_domain *domain;
	struct smr_sock_name *sock_name;
	struct sockaddr_un sockaddr = {0};
	int ret;

	ep->sock_info = calloc(1, sizeof(*ep->sock_info));
	if (!ep->sock_info)
		goto err_out;

	ep->sock_info->listen_sock = socket(AF_UNIX, SOCK_STREAM, 0);
	if (ep->sock_info->listen_sock < 0)
		goto free;

	domain = container_of(ep->util_ep.domain, struct smr_domain, util_domain);
	snprintf(smr_sock_name(ep->region), SMR_SOCK_NAME_MAX,
		 "%ld:%d:%d", (long) ep->region->pid, domain->dom_idx,
		 ep->ep_idx);

	sockaddr.sun_family = AF_UNIX;
	snprintf(sockaddr.sun_path, SMR_SOCK_NAME_MAX,
		 "%s%s", SMR_ZE_SOCK_PATH, smr_sock_name(ep->region));

	ret = bind(ep->sock_info->listen_sock, (struct sockaddr *) &sockaddr,
		   (socklen_t) sizeof(sockaddr));
	if (ret)
		goto close;

	ret = listen(ep->sock_info->listen_sock, SMR_MAX_PEERS);
	if (ret)
		goto close;

	FI_DBG(&smr_prov, FI_LOG_EP_CTRL, "EP listening on UNIX socket %s\n",
	       sockaddr.sun_path);

	ret = smr_init_epoll(ep->sock_info);
	if (ret)
		goto close;

	sock_name = calloc(1, sizeof(*sock_name));
	if (!sock_name)
		goto cleanup;

	memcpy(sock_name->name, sockaddr.sun_path, strlen(sockaddr.sun_path));
	memcpy(ep->sock_info->name, sockaddr.sun_path,
	       strlen(sockaddr.sun_path));

	pthread_mutex_lock(&sock_list_lock);
	dlist_insert_tail(&sock_name->entry, &sock_name_list);
	pthread_mutex_unlock(&sock_list_lock);

	ep->sock_info->my_fds = ze_hmem_get_dev_fds(&ep->sock_info->nfds);
	ret = pthread_create(&ep->sock_info->listener_thread, NULL,
			     &smr_start_listener, ep);
	if (ret)
		goto remove;

	return;

remove:
	pthread_mutex_lock(&sock_list_lock);
	dlist_remove(&sock_name->entry);
	pthread_mutex_unlock(&sock_list_lock);
	free(sock_name);
cleanup:
	smr_cleanup_epoll(ep->sock_info);
close:
	close(ep->sock_info->listen_sock);
	unlink(sockaddr.sun_path);
free:
	free(ep->sock_info);
	ep->sock_info = NULL;
err_out:
	FI_WARN(&smr_prov, FI_LOG_EP_CTRL, "Unable to initialize IPC socket."
		"Defaulting to SAR for device transfers\n");
}

static int smr_ep_ctrl(struct fid *fid, int command, void *arg)
{
	struct smr_attr attr;
	struct smr_ep *ep;
	struct smr_av *av;
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

		attr.name = ep->name;
		attr.rx_count = ep->rx_size;
		attr.tx_count = ep->tx_size;
		ret = smr_create(&smr_prov, av->smr_map, &attr, &ep->region);
		if (ret)
			return ret;

		if (ep->util_ep.caps & FI_HMEM) {
			ep->region->cma_cap_peer = SMR_CMA_CAP_OFF;
			ep->region->cma_cap_self = SMR_CMA_CAP_OFF;
			if (ze_hmem_p2p_enabled())
				smr_init_ipc_socket(ep);
		}

		smr_exchange_all_peers(ep->region);
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

static int smr_endpoint_name(char *name, char *addr, size_t addrlen,
			     int dom_idx, int ep_idx)
{
	const char *start;
	memset(name, 0, SMR_NAME_MAX);
	if (!addr || addrlen > SMR_NAME_MAX)
		return -FI_EINVAL;

	start = smr_no_prefix((const char *) addr);
	if (strstr(addr, SMR_PREFIX) || dom_idx || ep_idx)
		snprintf(name, SMR_NAME_MAX - 1, "%s:%d:%d:%d", start, getuid(),
			 dom_idx, ep_idx);
	else
		snprintf(name, SMR_NAME_MAX - 1, "%s", start);

	return 0;
}

int smr_endpoint(struct fid_domain *domain, struct fi_info *info,
		  struct fid_ep **ep_fid, void *context)
{
	struct smr_ep *ep;
	struct smr_domain *smr_domain;
	int ret;
	char name[SMR_NAME_MAX];

	ep = calloc(1, sizeof(*ep));
	if (!ep)
		return -FI_ENOMEM;

	smr_domain = container_of(domain, struct smr_domain, util_domain.domain_fid);

	fastlock_acquire(&smr_domain->util_domain.lock);
	ep->ep_idx = smr_domain->ep_idx++;
	fastlock_release(&smr_domain->util_domain.lock);
	ret = smr_endpoint_name(name, info->src_addr, info->src_addrlen,
			        smr_domain->dom_idx, ep->ep_idx);
	if (ret)
		goto err2;
	ret = smr_setname(&ep->util_ep.ep_fid.fid, name, SMR_NAME_MAX);
	if (ret)
		goto err2;

	ep->rx_size = info->rx_attr->size;
	ep->tx_size = info->tx_attr->size;
	ret = ofi_endpoint_init(domain, &smr_util_prov, info, &ep->util_ep, context,
				smr_ep_progress);
	if (ret)
		goto err1;

	ep->recv_fs = smr_recv_fs_create(info->rx_attr->size, NULL, NULL);
	ep->unexp_fs = smr_unexp_fs_create(info->rx_attr->size, NULL, NULL);
	ep->pend_fs = smr_pend_fs_create(info->tx_attr->size, NULL, NULL);
	ep->sar_fs = smr_sar_fs_create(info->rx_attr->size, NULL, NULL);
	smr_init_queue(&ep->recv_queue, smr_match_msg);
	smr_init_queue(&ep->trecv_queue, smr_match_tagged);
	smr_init_queue(&ep->unexp_msg_queue, smr_match_unexp_msg);
	smr_init_queue(&ep->unexp_tagged_queue, smr_match_unexp_tagged);
	dlist_init(&ep->sar_list);

	ep->min_multi_recv_size = SMR_INJECT_SIZE;

	ep->util_ep.ep_fid.fid.ops = &smr_ep_fi_ops;
	ep->util_ep.ep_fid.ops = &smr_ep_ops;
	ep->util_ep.ep_fid.cm = &smr_cm_ops;
	ep->util_ep.ep_fid.msg = &smr_msg_ops;
	ep->util_ep.ep_fid.tagged = &smr_tagged_ops;
	ep->util_ep.ep_fid.rma = &smr_rma_ops;
	ep->util_ep.ep_fid.atomic = &smr_atomic_ops;

	*ep_fid = &ep->util_ep.ep_fid;
	return 0;

err1:
	free((void *)ep->name);
err2:
	free(ep);
	return ret;
}
