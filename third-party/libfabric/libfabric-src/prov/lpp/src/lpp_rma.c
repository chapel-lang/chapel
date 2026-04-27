/*
 * Copyright (c) 2018-2024 GigaIO, Inc. All Rights Reserved.
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

#include "lpp.h"

ssize_t lpp_fi_readmsg(struct fid_ep *ep, const struct fi_msg_rma *msg,
		       uint64_t flags)
{
	struct lpp_ep	*lpp_ep = container_of(ep, struct lpp_ep, ep_fid);


	flags |= FI_RMA | FI_READ;
	flags |= lpp_cq_comp_flag(lpp_ep->transmit_bind_flags, flags);

	return lpp_rma_common(ep,
			      msg->msg_iov,
			      msg->iov_count,
			      msg->rma_iov,
			      msg->rma_iov_count,
			      msg->addr,
			      msg->context,
			      flags);
}

ssize_t lpp_fi_read(struct fid_ep *ep, void *buf, size_t len, void *desc,
		    fi_addr_t src_addr, uint64_t addr, uint64_t key,
		    void *context)
{
	size_t			local_iov_count = 1;
	size_t			remote_iov_count = 1;
	uint64_t		flags = FI_RMA | FI_READ;
	struct iovec		local_iov = { .iov_base = (void *)buf,
					      .iov_len = len };
	struct fi_rma_iov	remote_iov = { .addr = addr, .len = len,
					       .key = key };
	struct lpp_ep		*lpp_ep = container_of(ep, struct lpp_ep, ep_fid);

	flags |= lpp_cq_comp_flag(lpp_ep->transmit_bind_flags,
				  lpp_ep->stx->attr.op_flags);

	return lpp_rma_common(ep,
			      &local_iov,
			      local_iov_count,
			      &remote_iov,
			      remote_iov_count,
			      src_addr,
			      context,
			      flags);
}

ssize_t lpp_fi_write(struct fid_ep *ep, const void *buf, size_t len, void *desc,
					 fi_addr_t dest_addr, uint64_t addr, uint64_t key,
					 void *context)
{
	size_t			local_iov_count = 1;
	size_t			remote_iov_count = 1;
	uint64_t		flags = FI_RMA | FI_WRITE;
	struct iovec		local_iov = { .iov_base = (void *)buf,
					      .iov_len = len };
	struct fi_rma_iov	remote_iov = { .addr = addr,
					       .len = len,
					       .key = key };
	struct lpp_ep		*lpp_ep = container_of(ep, struct lpp_ep, ep_fid);

	flags |= lpp_cq_comp_flag(lpp_ep->transmit_bind_flags,
				  lpp_ep->stx->attr.op_flags);

	return lpp_rma_common(ep,
			      &local_iov,
			      local_iov_count,
			      &remote_iov,
			      remote_iov_count,
			      dest_addr,
			      context,
			      flags);
}

ssize_t lpp_fi_writemsg(struct fid_ep *ep, const struct fi_msg_rma *msg,
						uint64_t flags)
{
	struct lpp_ep	*lpp_ep = container_of(ep, struct lpp_ep, ep_fid);

	flags |= FI_RMA | FI_WRITE | flags;
	flags |= lpp_cq_comp_flag(lpp_ep->transmit_bind_flags, flags);

	return lpp_rma_common(ep,
			      msg->msg_iov,
			      msg->iov_count,
			      msg->rma_iov,
			      msg->rma_iov_count,
			      msg->addr,
			      msg->context,
			      flags);
}

ssize_t lpp_fi_inject_write(struct fid_ep *ep, const void *buf, size_t len,
			    fi_addr_t dest_addr, uint64_t addr, uint64_t key)
{
	size_t			local_iov_count = 1;
	size_t			remote_iov_count = 1;
	uint64_t		flags = FI_RMA | FI_WRITE | FI_INJECT;
	struct iovec		local_iov = { .iov_base = (void *)buf, .iov_len = len };
	struct fi_rma_iov	remote_iov = { .addr = addr, .len = len, .key = key };

	return lpp_rma_common(ep,
			      &local_iov,
			      local_iov_count,
			      &remote_iov,
			      remote_iov_count,
			      dest_addr,
			      NULL,
			      flags);
}
