/*
 * Copyright (c) 2022 Intel Corporation. All rights reserved.
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

#include "coll.h"

static int coll_getname(fid_t fid, void *addr, size_t *addrlen)
{
        struct coll_ep *coll_ep;

        coll_ep = container_of(fid, struct coll_ep, util_ep.ep_fid.fid);
        return fi_getname(&coll_ep->peer_ep->fid, addr, addrlen);
}

static struct fi_ops_cm coll_ops_cm = {
	.size = sizeof(struct fi_ops_cm),
	.setname = fi_no_setname,
	.getname = coll_getname,
	.getpeer = fi_no_getpeer,
	.connect = fi_no_connect,
	.listen = fi_no_listen,
	.accept = fi_no_accept,
	.reject = fi_no_reject,
	.shutdown = fi_no_shutdown,
	.join = coll_join_collective,
};

static struct fi_ops_ep coll_ops_ep = {
	.size = sizeof(struct fi_ops_ep),
	.cancel = fi_no_cancel,
	.getopt = fi_no_getopt,
	.setopt = fi_no_setopt,
	.tx_ctx = fi_no_tx_ctx,
	.rx_ctx = fi_no_rx_ctx,
	.rx_size_left = fi_no_rx_size_left,
	.tx_size_left = fi_no_tx_size_left,
};

static struct fi_ops_collective coll_ops_collective = {
	.size = sizeof(struct fi_ops_collective),
	.barrier = coll_ep_barrier,
	.barrier2 = coll_ep_barrier2,
	.broadcast = coll_ep_broadcast,
	.alltoall = fi_coll_no_alltoall,
	.allreduce = coll_ep_allreduce,
	.allgather = coll_ep_allgather,
	.reduce_scatter = fi_coll_no_reduce_scatter,
	.reduce = fi_coll_no_reduce,
	.scatter = coll_ep_scatter,
	.gather = fi_coll_no_gather,
	.msg = fi_coll_no_msg,
};

static int coll_ep_close(struct fid *fid)
{
	struct coll_ep *ep;

	ep = container_of(fid, struct coll_ep, util_ep.ep_fid.fid);

	ofi_endpoint_close(&ep->util_ep);
	fi_freeinfo(ep->peer_info);
	fi_freeinfo(ep->coll_info);
	free(ep);
	return 0;
}

static int coll_ep_ctrl(struct fid *fid, int command, void *arg)
{
	if (command == FI_ENABLE)
		return 0;

	return -FI_ENOSYS;
}

static struct fi_ops coll_ep_fi_ops = {
	.size = sizeof(struct fi_ops),
	.close = coll_ep_close,
	.bind = ofi_ep_fid_bind,
	.control = coll_ep_ctrl,
	.ops_open = fi_no_ops_open,
};

static struct fi_ops_transfer_peer coll_ep_peer_xfer_ops = {
	.size = sizeof(struct fi_ops_transfer_peer),
	.complete = coll_peer_xfer_complete,
	.comperr = coll_peer_xfer_error,
};

int coll_endpoint(struct fid_domain *domain, struct fi_info *info,
		  struct fid_ep **ep_fid, void *context)
{
	struct coll_ep *ep;
	struct fi_peer_transfer_context *peer_context = context;
	int ret;

	if (!info || !(info->mode & FI_PEER_TRANSFER)) {
		FI_WARN(&coll_prov, FI_LOG_CORE,
			"FI_PEER_TRANSFER mode required\n");
		return -EINVAL;
	}

	if (!peer_context || peer_context->size < sizeof(*peer_context)) {
		FI_WARN(&coll_prov, FI_LOG_CORE,
			"Invalid peer transfer context\n");
		return -EINVAL;
	}

	ep = calloc(1, sizeof(*ep));
	if (!ep)
		return -FI_ENOMEM;

	ep->coll_info = fi_dupinfo(info);
	if (!ep->coll_info) {
		ret = -FI_ENOMEM;
		goto err;
	}

	ep->peer_info = fi_dupinfo(peer_context->info);
	if (!ep->peer_info) {
		ret = -FI_ENOMEM;
		goto err;
	}

	ep->peer_ep = peer_context->ep;

	ret = ofi_endpoint_init(domain, &coll_util_prov, info,
				&ep->util_ep, context,
				&coll_ep_progress);

	if (ret)
		goto err;

	peer_context->peer_ops = &coll_ep_peer_xfer_ops;

	*ep_fid = &ep->util_ep.ep_fid;
	(*ep_fid)->fid.ops = &coll_ep_fi_ops;
	(*ep_fid)->ops = &coll_ops_ep;
	(*ep_fid)->cm = &coll_ops_cm;
	(*ep_fid)->collective = &coll_ops_collective;

	return 0;

err:
	fi_freeinfo(ep->peer_info);
	fi_freeinfo(ep->coll_info);
	free(ep);
	return ret;
}
