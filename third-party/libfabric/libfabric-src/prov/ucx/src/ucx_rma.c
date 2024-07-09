/*
 * Copyright (c) 2019-2023 Intel Corporation. All rights reserved.
 *
 * This software is available to you under a choice of one of two
 * licenses.  You may choose to be licensed under the terms of the GNU
 * General Public License (GPL) Version 2, available from the file
 * COPYING in the main directory of this source tree, or the
 * OpenFabrics.org BSD license below:
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

#include "ucx.h"
#include "ofi_util.h"

#define UCX_DO_READ 0
#define UCX_DO_WRITE 1

static ssize_t ucx_proc_rma_msg(struct fid_ep *ep,
				const struct fi_msg_rma *msg,
				uint64_t flags, int op_type);

static ssize_t ucx_write(struct fid_ep *ep, const void *buf, size_t len,
			 void *desc, fi_addr_t dest_addr, uint64_t addr,
			 uint64_t key, void *context)
{
	struct ucx_ep *u_ep = container_of(ep, struct ucx_ep, ep.ep_fid);
	struct iovec iov = {
		.iov_base = (void *)buf,
		.iov_len = len,
	};
	struct fi_rma_iov rma_iov = {
		.addr = addr,
		.len = len,
		.key = key,
	};
	struct fi_msg_rma msg = {
		.msg_iov = &iov,
		.desc = desc,
		.iov_count = 1,
		.addr = dest_addr,
		.rma_iov = &rma_iov,
		.rma_iov_count = 1,
		.context = context,
		.data = 0,
	};

	return ucx_proc_rma_msg(ep, &msg, u_ep->ep.tx_op_flags, UCX_DO_WRITE);
}

static ssize_t ucx_writev(struct fid_ep *ep, const struct iovec *iov,
			  void **desc, size_t count, fi_addr_t dest_addr,
			  uint64_t addr, uint64_t key, void *context)
{
	if (count != 1) {
		return -FI_EINVAL;
	} else {
		struct ucx_ep *u_ep = container_of(ep, struct ucx_ep, ep.ep_fid);
		struct fi_rma_iov rma_iov = {
			.addr = addr,
			.len = iov[0].iov_len,
			.key = key,
		};
		struct fi_msg_rma msg = {
			.msg_iov = iov,
			.desc = desc,
			.iov_count = 1,
			.addr = dest_addr,
			.rma_iov = &rma_iov,
			.rma_iov_count = 1,
			.context = context,
			.data = 0,
		};

		return ucx_proc_rma_msg(ep, &msg, u_ep->ep.tx_op_flags,
					UCX_DO_WRITE);
	}
}

static ssize_t ucx_read(struct fid_ep *ep, void *buf, size_t len,
			void *desc, fi_addr_t src_addr, uint64_t addr,
			uint64_t key, void *context)
{
	struct ucx_ep *u_ep = container_of(ep, struct ucx_ep, ep.ep_fid);
	struct iovec iov = {
		.iov_base = buf,
		.iov_len = len,
	};
	struct fi_rma_iov rma_iov = {
		.addr = addr,
		.len = len,
		.key = key,
	};
	struct fi_msg_rma msg = {
		.msg_iov = &iov,
		.desc = desc,
		.iov_count = 1,
		.addr = src_addr,
		.rma_iov = &rma_iov,
		.rma_iov_count = 1,
		.context = context,
		.data = 0,
	};

	return ucx_proc_rma_msg(ep, &msg, u_ep->ep.tx_op_flags, UCX_DO_READ);
}

static ssize_t ucx_readv(struct fid_ep *ep, const struct iovec *iov,
			 void **desc, size_t count, fi_addr_t src_addr,
			 uint64_t addr, uint64_t key, void *context)
{
	if (count != 1) {
		return -FI_EINVAL;
	} else {
		struct ucx_ep *u_ep = container_of(ep, struct ucx_ep, ep.ep_fid);
		struct fi_rma_iov rma_iov = {
			.addr = addr,
			.len = iov[0].iov_len,
			.key = key,
		};
		struct fi_msg_rma msg = {
			.msg_iov = iov,
			.desc = desc,
			.iov_count = 1,
			.addr = src_addr,
			.rma_iov = &rma_iov,
			.rma_iov_count = 1,
			.context = context,
			.data = 0,
		};

		return ucx_proc_rma_msg(ep, &msg, u_ep->ep.tx_op_flags,
					UCX_DO_READ);
	}
}

static ssize_t ucx_writemsg(struct fid_ep *ep,
			    const struct fi_msg_rma *msg,
			    uint64_t flags)
{
	return ucx_proc_rma_msg(ep, msg, flags, UCX_DO_WRITE);
}

static ssize_t ucx_readmsg(struct fid_ep *ep,
			   const struct fi_msg_rma *msg,
			   uint64_t flags)
{
	return ucx_proc_rma_msg(ep, msg, flags, UCX_DO_READ);
}

void ucx_rma_callback(void *request, ucs_status_t status)
{
	struct ucx_request *ucx_req = request;

	if (status == UCS_OK){
		if (ucx_req->type == UCX_REQ_WRITE)
			ofi_ep_cntr_inc(&(ucx_req->ep->ep), CNTR_WR);
		else
			ofi_ep_cntr_inc(&(ucx_req->ep->ep), CNTR_RD);

		if (ucx_req->completion.flags & FI_COMPLETION)
			ofi_cq_write(ucx_req->ep->ep.tx_cq,
				     ucx_req->completion.op_context,
				     ucx_req->completion.flags,
				     0, NULL, 0, 0);
	} else {
		FI_DBG( &ucx_prov,FI_LOG_CORE,
			"RMA completion error %s\n",
			ucs_status_string(status));
		struct util_cntr *cntr = (ucx_req->type == UCX_REQ_WRITE) ?
						ucx_req->ep->ep.cntrs[CNTR_WR] :
						ucx_req->ep->ep.cntrs[CNTR_RD];
		if (cntr)
			cntr->cntr_fid.ops->adderr(&cntr->cntr_fid, 1);

		if (ucx_req->completion.flags & FI_COMPLETION) {
			ucx_write_error_completion(ucx_req->ep->ep.tx_cq,
						   ucx_req->completion.op_context,
						   ucx_req->completion.flags,
						   status,
						   -ucx_translate_errcode(status),
						   0, ucx_req->completion.tag);
		}
	}
	ucx_req_release(request);
}

static struct ucx_mr_rkey *ucx_get_rkey(struct ucx_ep *ep,
					fi_addr_t owner_addr, uint64_t key)
{
	ucs_status_t status;
	struct ucx_mr_pkey *pkey;
	struct ucx_mr_rkey tmp_rkey, *rkey;
	struct ucx_domain *domain;

	tmp_rkey.id.owner_addr = owner_addr;
	tmp_rkey.id.key = key;

	domain = container_of(ep->ep.domain, struct ucx_domain, u_domain);
	HASH_FIND(hh, domain->remote_keys, &tmp_rkey.id, sizeof(tmp_rkey.id),
		  rkey);
	if (!rkey) {
		struct ucx_ave *ep_ave = (struct ucx_ave *)owner_addr;

		pkey = (struct ucx_mr_pkey *)tmp_rkey.id.key;
		if (!pkey || pkey->signature != FI_UCX_PKEY_SIGNATURE) {
			FI_DBG(&ucx_prov,FI_LOG_CORE,
			       "UCX/RMA: invalid key {%" PRIu64 ":%" PRIu64 "}\n",
			       tmp_rkey.id.owner_addr, tmp_rkey.id.key);
			return NULL;
		}

		rkey = malloc(sizeof(struct ucx_mr_rkey));
		if (!rkey)
			return NULL;
		status = ucp_ep_rkey_unpack(ep_ave->uep, pkey->pkey,
					    &(rkey->rkey));
		if (status != UCS_OK) {
			FI_DBG(&ucx_prov,FI_LOG_CORE,
			       "UCX/RMA: failed to add key {%" PRIu64 ":%" PRIu64 "}\n",
			       tmp_rkey.id.owner_addr, tmp_rkey.id.key);
			free(rkey);
			return NULL;
		}

		rkey->id = tmp_rkey.id;
		HASH_ADD(hh, domain->remote_keys, id, sizeof(rkey->id), rkey);
		dlist_insert_before(&rkey->entry, &pkey->rkey_list);

		FI_DBG(&ucx_prov,FI_LOG_CORE,
		       "UCX/RMA: added key {%" PRIu64 ":%" PRIu64 "}\n",
		       rkey->id.owner_addr, rkey->id.key);
	}

	return rkey;
}

static ssize_t ucx_proc_rma_msg(struct fid_ep *ep,
				const struct fi_msg_rma *msg,
				uint64_t flags, int op_type)
{
	struct ucx_ep* u_ep;
	ucp_ep_h dst_ep;
	ucs_status_ptr_t status = NULL;
	struct ucx_mr_rkey *rkey;
	struct ucx_request *req;

	u_ep = container_of(ep, struct ucx_ep, ep.ep_fid);
	dst_ep = UCX_GET_UCP_EP(u_ep, msg->addr);

	if (msg->rma_iov_count > 1 || msg->iov_count > 1) {
		FI_DBG(&ucx_prov,FI_LOG_CORE,
		       "UCX/RMA: unsupported IOV len. Local: %lu, Remote %lu",
		       msg->iov_count,
		       msg->rma_iov_count);
		return -FI_EINVAL;
	}

	rkey = ucx_get_rkey(u_ep, msg->addr, msg->rma_iov[0].key);
	if (!rkey)
		return -FI_EINVAL;

	if (op_type == UCX_DO_READ) {
		status = ucp_get_nb(dst_ep,
				    msg->msg_iov[0].iov_base,
				    msg->msg_iov[0].iov_len,
				    msg->rma_iov[0].addr,
				    rkey->rkey, ucx_rma_callback);
	} else {
		status = ucp_put_nb(dst_ep,
				    msg->msg_iov[0].iov_base,
				    msg->msg_iov[0].iov_len,
				    msg->rma_iov[0].addr,
				    rkey->rkey, ucx_rma_callback);
	}

	if (status == UCS_OK) {
		if (op_type == UCX_DO_WRITE)
			ofi_ep_cntr_inc(&u_ep->ep, CNTR_WR);
		else
			ofi_ep_cntr_inc(&u_ep->ep, CNTR_RD);

		if (flags & FI_COMPLETION)
			ofi_cq_write(u_ep->ep.tx_cq, msg->context, flags,
				     0, NULL, 0, 0);
		return FI_SUCCESS;
	}

	if (UCS_PTR_IS_ERR(status)) {
		FI_DBG(&ucx_prov,FI_LOG_CORE,
		       "Write operation returns error: %s",
		       ucs_status_string(*(ucs_status_t*)status));
		return ucx_translate_errcode(*(ucs_status_t*)status);
	}

	req = (struct ucx_request *)status;
	req->cq = NULL;
	req->ep = u_ep;
	req->completion.op_context = msg->context;
	req->completion.flags = flags;
	req->type = (UCX_DO_READ == op_type) ? UCX_REQ_READ : UCX_REQ_WRITE;
	return FI_SUCCESS;
}

ssize_t ucx_inject_write(struct fid_ep *ep, const void *buf, size_t len,
			 fi_addr_t dest_addr, uint64_t addr, uint64_t key)
{
	struct ucx_ep* u_ep;
	ucp_ep_h dst_ep;
	ucs_status_ptr_t status = NULL;
	ucs_status_t ret = UCS_OK;
	struct ucx_mr_rkey *rkey;

	u_ep = container_of(ep, struct ucx_ep, ep.ep_fid);
	dst_ep = UCX_GET_UCP_EP(u_ep, dest_addr);

	rkey = ucx_get_rkey(u_ep, dest_addr, key);
	if (!rkey)
		return -FI_EINVAL;

	status = ucp_put_nb(dst_ep, buf, len, addr, rkey->rkey,
			    ucx_send_callback_no_compl);

	if (status != UCS_OK) {
		if (UCS_PTR_IS_ERR(status))
			return ucx_translate_errcode(UCS_PTR_STATUS(status));

		while ((ret = ucp_request_check_status(status)) ==
		       UCS_INPROGRESS)
			ucp_worker_progress(u_ep->worker);
	}

	ofi_ep_cntr_inc(&(u_ep->ep), CNTR_WR);

	return ucx_translate_errcode(ret);
}

struct fi_ops_rma ucx_rma_ops = {
	.size = sizeof(struct fi_ops_rma),
	.read = ucx_read,
	.readv = ucx_readv,
	.readmsg = ucx_readmsg,
	.write = ucx_write,
	.writev = ucx_writev,
	.writemsg = ucx_writemsg,
	.inject = ucx_inject_write,
	.writedata = fi_no_rma_writedata,
	.injectdata = fi_no_rma_injectdata,
};
