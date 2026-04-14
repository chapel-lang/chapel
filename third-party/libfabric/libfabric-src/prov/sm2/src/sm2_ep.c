/*
 * Copyright (c) Intel Corporation. All rights reserved
 * Copyright (c) Amazon.com, Inc. or its affiliates. All rights reserved.
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

#include "ofi_hmem.h"
#include "ofi_iov.h"
#include "ofi_mem.h"
#include "ofi_mr.h"
#include "sm2.h"
#include "sm2_fifo.h"

pthread_mutex_t sm2_ep_list_lock = PTHREAD_MUTEX_INITIALIZER;
extern struct fi_ops_msg sm2_msg_ops, sm2_no_recv_msg_ops;
extern struct fi_ops_tagged sm2_tag_ops, sm2_no_recv_tag_ops;
extern struct fi_ops_atomic sm2_atomic_ops;
int sm2_global_ep_idx = 0;

int sm2_setname(fid_t fid, void *addr, size_t addrlen)
{
	struct sm2_ep *ep;
	char *name;

	if (addrlen > OFI_NAME_MAX) {
		FI_WARN(&sm2_prov, FI_LOG_EP_CTRL,
			"Addrlen exceeds max addrlen (%d)\n", OFI_NAME_MAX);
		return -FI_EINVAL;
	}

	ep = container_of(fid, struct sm2_ep, util_ep.ep_fid.fid);

	name = strdup(addr);
	if (!name)
		return -FI_ENOMEM;

	if (ep->name)
		free((void *) ep->name);
	ep->name = name;
	return 0;
}

int sm2_getname(fid_t fid, void *addr, size_t *addrlen)
{
	struct sm2_ep *ep;
	int ret = 0;

	ep = container_of(fid, struct sm2_ep, util_ep.ep_fid.fid);
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

static struct fi_ops_cm sm2_cm_ops = {
	.size = sizeof(struct fi_ops_cm),
	.setname = sm2_setname,
	.getname = sm2_getname,
	.getpeer = fi_no_getpeer,
	.connect = fi_no_connect,
	.listen = fi_no_listen,
	.accept = fi_no_accept,
	.reject = fi_no_reject,
	.shutdown = fi_no_shutdown,
};

static int sm2_ep_getopt(fid_t fid, int level, int optname, void *optval,
			 size_t *optlen)
{
	struct sm2_ep *ep = container_of(fid, struct sm2_ep, util_ep.ep_fid);
	return ep->srx->ops->getopt(&ep->srx->fid, level, optname, optval,
				    optlen);
}

static int sm2_ep_setopt(fid_t fid, int level, int optname, const void *optval,
			 size_t optlen)
{
	struct sm2_ep *ep = container_of(fid, struct sm2_ep, util_ep.ep_fid);
	struct util_srx_ctx *srx;

	if (level != FI_OPT_ENDPOINT)
		return -FI_ENOPROTOOPT;

	if (optname == FI_OPT_MIN_MULTI_RECV) {
		srx = util_get_peer_srx(ep->srx)->ep_fid.fid.context;
		srx->min_multi_recv_size = *(size_t *) optval;
		return FI_SUCCESS;
	}

	if (optname == FI_OPT_CUDA_API_PERMITTED) {
		if (!hmem_ops[FI_HMEM_CUDA].initialized) {
			FI_WARN(&sm2_prov, FI_LOG_CORE,
				"Cannot set option FI_OPT_CUDA_API_PERMITTED "
				"when cuda library or cuda device not "
				"available\n");
			return -FI_EINVAL;
		}
		/* our CUDA support relies on the ability to call CUDA API */
		return *(bool *) optval ? FI_SUCCESS : -FI_EOPNOTSUPP;
	}
	return -FI_ENOPROTOOPT;
}

static ssize_t sm2_ep_cancel(fid_t ep_fid, void *context)
{
	struct sm2_ep *ep;

	ep = container_of(ep_fid, struct sm2_ep, util_ep.ep_fid);
	return ep->srx->ops->cancel(&ep->srx->fid, context);
}

static struct fi_ops_ep sm2_ep_ops = {
	.size = sizeof(struct fi_ops_ep),
	.cancel = sm2_ep_cancel,
	.getopt = sm2_ep_getopt,
	.setopt = sm2_ep_setopt,
	.tx_ctx = fi_no_tx_ctx,
	.rx_ctx = fi_no_rx_ctx,
	.rx_size_left = fi_no_rx_size_left,
	.tx_size_left = fi_no_tx_size_left,
};

ssize_t sm2_verify_peer(struct sm2_ep *ep, fi_addr_t fi_addr, sm2_gid_t *gid)
{
	struct sm2_av *sm2_av;
	struct sm2_ep_allocation_entry *entries;

	*gid = *((sm2_gid_t *) ofi_av_get_addr(ep->util_ep.av, fi_addr));
	assert(*gid < SM2_MAX_UNIVERSE_SIZE);

	sm2_av = container_of(ep->util_ep.av, struct sm2_av, util_av);
	if (sm2_av->reverse_lookup[*gid] == FI_ADDR_NOTAVAIL)
		return -FI_EINVAL;

	entries = sm2_mmap_entries(ep->mmap);
	/* TODO... should this be atomic? */
	if (entries[*gid].startup_ready == false)
		return -FI_EAGAIN;

	/* TODO: Check for PID that changes */
	return 0;
}

static void sm2_format_inject(struct sm2_xfer_entry *xfer_entry,
			      struct ofi_mr **mr, const struct iovec *iov,
			      size_t count)
{
	xfer_entry->hdr.proto = sm2_proto_inject;
	xfer_entry->hdr.size = ofi_copy_from_mr_iov(
		xfer_entry->user_data, SM2_INJECT_SIZE, mr, iov, count, 0);
}

static ssize_t sm2_do_inject(struct sm2_ep *ep, struct sm2_region *peer_smr,
			     sm2_gid_t peer_gid, uint32_t op, uint64_t tag,
			     uint64_t data, uint64_t op_flags,
			     struct ofi_mr **mr, const struct iovec *iov,
			     size_t iov_count, size_t total_len, void *context)
{
	struct sm2_xfer_entry *xfer_entry;
	ssize_t ret;

	assert(total_len <= SM2_INJECT_SIZE);

	ret = sm2_pop_xfer_entry(ep, &xfer_entry);
	if (ret)
		return ret;

	sm2_generic_format(xfer_entry, ep->gid, op, tag, data, op_flags,
			   context);
	sm2_format_inject(xfer_entry, mr, iov, iov_count);

	sm2_fifo_write(ep, peer_gid, xfer_entry);
	return FI_SUCCESS;
}

static void sm2_format_cma(struct sm2_xfer_entry *xfer_entry,
			   const struct iovec *iov, size_t count)
{
	struct sm2_cma_data *cma_data =
		(struct sm2_cma_data *) xfer_entry->user_data;

	xfer_entry->hdr.proto = sm2_proto_cma;
	xfer_entry->hdr.size = ofi_total_iov_len(iov, count);
	cma_data->iov_count = count;
	memcpy(cma_data->iov, iov, sizeof(*iov) * count);
}

static ssize_t sm2_do_cma(struct sm2_ep *ep, struct sm2_region *peer_smr,
			  sm2_gid_t peer_gid, uint32_t op, uint64_t tag,
			  uint64_t data, uint64_t op_flags, struct ofi_mr **mr,
			  const struct iovec *iov, size_t iov_count,
			  size_t total_len, void *context)
{
	ssize_t ret;
	struct sm2_xfer_entry *xfer_entry;

	ret = sm2_pop_xfer_entry(ep, &xfer_entry);
	if (ret)
		return ret;

	sm2_generic_format(xfer_entry, ep->gid, op, tag, data, op_flags,
			   context);
	sm2_format_cma(xfer_entry, iov, iov_count);
	sm2_fifo_write(ep, peer_gid, xfer_entry);

	return FI_SUCCESS;
}

static ssize_t sm2_format_ipc(struct sm2_xfer_entry *xfer_entry, void *ptr,
			      size_t len, enum fi_hmem_iface iface,
			      uint64_t device)
{
	void *base;
	ssize_t ret;
	struct ipc_info *ipc_info = (struct ipc_info *) xfer_entry->user_data;

	xfer_entry->hdr.proto = sm2_proto_ipc;
	xfer_entry->hdr.size = len;
	ipc_info->iface = iface;
	ipc_info->device = device;

	ret = ofi_hmem_get_base_addr(ipc_info->iface, ptr, len, &base,
				     &ipc_info->base_length);
	if (ret)
		return ret;

	ret = ofi_hmem_get_handle(ipc_info->iface, base, ipc_info->base_length,
				  (void **) &ipc_info->ipc_handle);
	if (ret)
		return ret;

	ipc_info->base_addr = (uintptr_t) base;
	ipc_info->offset = (uintptr_t) ptr - (uintptr_t) base;

	return FI_SUCCESS;
}

static ssize_t sm2_do_ipc(struct sm2_ep *ep, struct sm2_region *peer_smr,
			  sm2_gid_t peer_gid, uint32_t op, uint64_t tag,
			  uint64_t data, uint64_t op_flags, struct ofi_mr **mr,
			  const struct iovec *iov, size_t iov_count,
			  size_t total_len, void *context)
{
	struct sm2_xfer_entry *xfer_entry;
	ssize_t ret;

	ret = sm2_pop_xfer_entry(ep, &xfer_entry);
	if (ret)
		return ret;

	sm2_generic_format(xfer_entry, ep->gid, op, tag, data, op_flags,
			   context);
	ret = sm2_format_ipc(xfer_entry, iov[0].iov_base, total_len,
			     mr[0]->iface, mr[0]->device);

	if (ret) {
		FI_WARN(&sm2_prov, FI_LOG_EP_CTRL,
			"Error generating IPC header information\n");
		smr_freestack_push(sm2_freestack(ep->self_region), xfer_entry);
		return ret;
	}

	sm2_fifo_write(ep, peer_gid, xfer_entry);
	return FI_SUCCESS;
}

static void cleanup_shm_resources(struct sm2_ep *ep)
{
	struct sm2_xfer_entry *xfer_entry;
	bool retry = true;

	/* Return all free queue entries in queue without processing them */
return_incoming:
	while (NULL != (xfer_entry = sm2_fifo_read(ep))) {
		if (xfer_entry->hdr.proto_flags & SM2_RETURN) {
			smr_freestack_push(sm2_freestack(ep->self_region),
					   xfer_entry);
		} else {
			/* TODO Tell other side that we haven't processed their
			 * message, just returned xfer_entry */
			xfer_entry->hdr.proto_flags &= ~SM2_GENERATE_COMPLETION;
			sm2_fifo_write_back(ep, xfer_entry);
		}
	}

	if (smr_freestack_isfull(sm2_freestack(ep->self_region))) {
		/* TODO Set head/tail of FIFO queue to show peers we aren't
		   accepting new entires */
		FI_INFO(&sm2_prov, FI_LOG_EP_CTRL,
			"All xfer_entry's acquired for map[%d], clean "
			"shutdown\n",
			ep->gid);
	} else {
		if (retry) {
			retry = false;
			sleep(1);
			goto return_incoming;
		}

		FI_WARN(&sm2_prov, FI_LOG_EP_CTRL,
			"Shutting down map[%d] without all xfer_entry's, messy "
			"shutdown, map[%d] will not be used again (zombie'd) "
			"until SHM file is cleared\n",
			ep->gid, ep->gid);
	}
}

static int sm2_ep_close(struct fid *fid)
{
	struct sm2_ep *ep =
		container_of(fid, struct sm2_ep, util_ep.ep_fid.fid);

	cleanup_shm_resources(ep);

	if (ep->srx && ep->util_ep.ep_fid.msg != &sm2_no_recv_msg_ops)
		(void) util_srx_close(&ep->srx->fid);

	ofi_endpoint_close(&ep->util_ep);

	/* Set our PID to 0 in the regions map if our free queue entry stack is
	 * full This will allow other entries re-use us or shrink file.
	 */
	/* TODO Do we want to mark our entry as zombie now if we don't have all
	   our xfer_entry? */
	if (smr_freestack_isfull(sm2_freestack(ep->self_region))) {
		sm2_file_lock(ep->mmap);
		sm2_entry_free(ep->mmap, ep->gid);
		sm2_file_unlock(ep->mmap);
	}

	if (ep->xfer_ctx_pool)
		ofi_bufpool_destroy(ep->xfer_ctx_pool);

	free((void *) ep->name);
	free(ep);
	return 0;
}

static int sm2_ep_trywait(void *arg)
{
	struct sm2_ep *ep;

	ep = container_of(arg, struct sm2_ep, util_ep.ep_fid.fid);

	sm2_ep_progress(&ep->util_ep);

	return FI_SUCCESS;
}

static int sm2_ep_bind_cq(struct sm2_ep *ep, struct util_cq *cq, uint64_t flags)
{
	int ret;

	ret = ofi_ep_bind_cq(&ep->util_ep, cq, flags);
	if (ret)
		return ret;

	if (cq->wait) {
		ret = ofi_wait_add_fid(cq->wait, &ep->util_ep.ep_fid.fid, 0,
				       sm2_ep_trywait);
		if (ret)
			return ret;
	}

	return ret;
}

static int sm2_ep_bind_cntr(struct sm2_ep *ep, struct util_cntr *cntr,
			    uint64_t flags)
{
	int ret;

	ret = ofi_ep_bind_cntr(&ep->util_ep, cntr, flags);
	if (ret)
		return ret;

	if (cntr->wait) {
		ret = ofi_wait_add_fid(cntr->wait, &ep->util_ep.ep_fid.fid, 0,
				       sm2_ep_trywait);
		if (ret)
			return ret;
	}

	return FI_SUCCESS;
}

static int sm2_ep_bind(struct fid *ep_fid, struct fid *bfid, uint64_t flags)
{
	struct sm2_ep *ep;
	struct util_av *av;
	int ret = 0;

	ep = container_of(ep_fid, struct sm2_ep, util_ep.ep_fid.fid);
	switch (bfid->fclass) {
	case FI_CLASS_AV:
		av = container_of(bfid, struct util_av, av_fid.fid);
		ret = ofi_ep_bind_av(&ep->util_ep, av);
		if (ret) {
			FI_WARN(&sm2_prov, FI_LOG_EP_CTRL,
				"Duplicate AV binding\n");
			return -FI_EINVAL;
		}
		break;
	case FI_CLASS_CQ:
		ret = sm2_ep_bind_cq(
			ep, container_of(bfid, struct util_cq, cq_fid.fid),
			flags);
		break;
	case FI_CLASS_EQ:
		break;
	case FI_CLASS_CNTR:
		ret = sm2_ep_bind_cntr(
			ep, container_of(bfid, struct util_cntr, cntr_fid.fid),
			flags);
		break;
	case FI_CLASS_SRX_CTX:
		ep->srx = container_of(bfid, struct fid_ep, fid);
		break;
	default:
		FI_WARN(&sm2_prov, FI_LOG_EP_CTRL, "Invalid fid class\n");
		ret = -FI_EINVAL;
		break;
	}
	return ret;
}

static int sm2_discard(struct fi_peer_rx_entry *rx_entry)
{
	struct sm2_xfer_ctx *xfer_ctx = rx_entry->peer_context;

	ofi_genlock_lock(&xfer_ctx->ep->util_ep.lock);
	ofi_buf_free(xfer_ctx);
	ofi_genlock_unlock(&xfer_ctx->ep->util_ep.lock);
	return FI_SUCCESS;
}

struct fi_ops_srx_peer sm2_srx_peer_ops = {
	.size = sizeof(struct fi_ops_srx_peer),
	.start_msg = sm2_unexp_start,
	.start_tag = sm2_unexp_start,
	.discard_msg = sm2_discard,
	.discard_tag = sm2_discard,
};

static void sm2_update(struct util_srx_ctx *srx, struct util_rx_entry *rx_entry)
{
	// no update needed - sm2 only used as the owner when not used as a peer
	// by another provider
}

int sm2_srx_context(struct fid_domain *domain, struct fi_rx_attr *attr,
		    struct fid_ep **rx_ep, void *context)
{
	struct sm2_domain *sm2_domain;

	sm2_domain =
		container_of(domain, struct sm2_domain, util_domain.domain_fid);

	if (attr->op_flags & FI_PEER) {
		sm2_domain->srx =
			((struct fi_peer_srx_context *) (context))->srx;
		sm2_domain->srx->peer_ops = &sm2_srx_peer_ops;
		return FI_SUCCESS;
	}
	FI_WARN(&sm2_prov, FI_LOG_EP_CTRL,
		"shared srx only supported with FI_PEER flag\n");
	return -FI_EINVAL;
}

static int sm2_ep_ctrl(struct fid *fid, int command, void *arg)
{
	struct sm2_attr attr;
	struct sm2_domain *domain;
	struct sm2_ep *ep;
	struct sm2_av *av;
	struct fid_peer_srx *srx;
	int ret;
	sm2_gid_t self_gid;

	ep = container_of(fid, struct sm2_ep, util_ep.ep_fid.fid);
	av = container_of(ep->util_ep.av, struct sm2_av, util_av);

	switch (command) {
	case FI_ENABLE:
		if ((ofi_needs_rx(ep->util_ep.caps) && !ep->util_ep.rx_cq) ||
		    (ofi_needs_tx(ep->util_ep.caps) && !ep->util_ep.tx_cq))
			return -FI_ENOCQ;
		if (!ep->util_ep.av)
			return -FI_ENOAV;

		attr.name = ep->name;
		attr.flags = 0;

		ret = sm2_create(&sm2_prov, &attr, &av->mmap, &self_gid);
		ep->gid = self_gid;
		ep->mmap = &av->mmap;
		ep->self_region = sm2_mmap_ep_region(ep->mmap, ep->gid);

		if (ret)
			return ret;

		if (!ep->srx) {
			domain = container_of(ep->util_ep.domain,
					      struct sm2_domain,
					      util_domain.domain_fid);
			ret = util_ep_srx_context(&domain->util_domain,
						  ep->rx_size, SM2_IOV_LIMIT,
						  SM2_INJECT_SIZE, &sm2_update,
						  &ep->util_ep.lock, &ep->srx);
			if (ret)
				return ret;

			util_get_peer_srx(ep->srx)->peer_ops =
				&sm2_srx_peer_ops;
			ret = util_srx_bind(&ep->srx->fid,
					    &ep->util_ep.rx_cq->cq_fid.fid,
					    FI_RECV);
			if (ret)
				return ret;
		} else {
			srx = calloc(1, sizeof(*srx));
			srx->peer_ops = &sm2_srx_peer_ops;
			srx->owner_ops = sm2_get_peer_srx(ep)->owner_ops;
			srx->ep_fid.fid.context =
				sm2_get_peer_srx(ep)->ep_fid.fid.context;
			ep->srx = &srx->ep_fid;
			ep->util_ep.ep_fid.msg = &sm2_no_recv_msg_ops;
			ep->util_ep.ep_fid.tagged = &sm2_no_recv_tag_ops;
		}

		break;
	default:
		return -FI_ENOSYS;
	}
	return ret;
}

static struct fi_ops sm2_ep_fi_ops = {
	.size = sizeof(struct fi_ops),
	.close = sm2_ep_close,
	.bind = sm2_ep_bind,
	.control = sm2_ep_ctrl,
	.ops_open = fi_no_ops_open,
};

static int sm2_endpoint_name(struct sm2_ep *ep, char *name, char *addr,
			     size_t addrlen)
{
	memset(name, 0, OFI_NAME_MAX);
	if (!addr || addrlen > OFI_NAME_MAX)
		return -FI_EINVAL;

	pthread_mutex_lock(&sm2_ep_list_lock);
	ep->ep_idx = sm2_global_ep_idx++;
	pthread_mutex_unlock(&sm2_ep_list_lock);

	if (strstr(addr, SM2_PREFIX)) {
		snprintf(name, OFI_NAME_MAX - 1, "%s:%d:%d", addr, getuid(),
			 ep->ep_idx);
	} else {
		/* this is an fi_ns:// address.*/
		snprintf(name, OFI_NAME_MAX - 1, "%s", addr);
	}

	return 0;
}

int sm2_endpoint(struct fid_domain *domain, struct fi_info *info,
		 struct fid_ep **ep_fid, void *context)
{
	struct sm2_ep *ep;
	int ret;
	char name[OFI_NAME_MAX];

	ep = calloc(1, sizeof(*ep));
	if (!ep)
		return -FI_ENOMEM;

	ret = sm2_endpoint_name(ep, name, info->src_addr, info->src_addrlen);

	if (ret)
		goto ep;
	ret = sm2_setname(&ep->util_ep.ep_fid.fid, name, OFI_NAME_MAX);
	if (ret)
		goto ep;

	ep->tx_size = info->tx_attr->size;
	ep->rx_size = info->rx_attr->size;
	ret = ofi_endpoint_init(domain, &sm2_util_prov, info, &ep->util_ep,
				context, sm2_ep_progress);
	if (ret)
		goto name;

	ep->util_ep.ep_fid.msg = &sm2_msg_ops;
	ep->util_ep.ep_fid.tagged = &sm2_tag_ops;

	ret = ofi_bufpool_create(&ep->xfer_ctx_pool,
				 sizeof(struct sm2_xfer_ctx), 16, 0,
				 info->rx_attr->size, OFI_BUFPOOL_NO_TRACK);
	if (ret || ofi_bufpool_grow(ep->xfer_ctx_pool)) {
		FI_WARN(&sm2_prov, FI_LOG_EP_CTRL,
			"Unable to create xfer_entry ctx pool\n");
		ret = -FI_ENOMEM;
		goto close;
	}

	ep->util_ep.ep_fid.fid.ops = &sm2_ep_fi_ops;
	ep->util_ep.ep_fid.ops = &sm2_ep_ops;
	ep->util_ep.ep_fid.cm = &sm2_cm_ops;
	ep->util_ep.ep_fid.rma = NULL;
	ep->util_ep.ep_fid.atomic = &sm2_atomic_ops;

	*ep_fid = &ep->util_ep.ep_fid;
	return 0;

close:
	(void) ofi_endpoint_close(&ep->util_ep);
name:
	free((void *) ep->name);
ep:
	free(ep);
	return ret;
}

sm2_proto_func sm2_proto_ops[sm2_proto_max] = {
	[sm2_proto_inject] = &sm2_do_inject,
	[sm2_proto_cma] = &sm2_do_cma,
	[sm2_proto_ipc] = &sm2_do_ipc,
};
