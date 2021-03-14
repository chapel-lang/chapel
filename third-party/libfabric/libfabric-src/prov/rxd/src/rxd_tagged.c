/*
 * Copyright (c) 2013-2018 Intel Corporation. All rights reserved.
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
#include <ofi_mem.h>
#include <ofi_iov.h>
#include "rxd.h"

ssize_t rxd_ep_trecv(struct fid_ep *ep_fid, void *buf, size_t len, void *desc,
	fi_addr_t src_addr, uint64_t tag, uint64_t ignore, void *context)
{
	struct rxd_ep *ep;
	struct iovec msg_iov;

	ep = container_of(ep_fid, struct rxd_ep, util_ep.ep_fid.fid);

	msg_iov.iov_base = (void *) buf;
	msg_iov.iov_len = len;

	return rxd_ep_generic_recvmsg(ep, &msg_iov, 1, src_addr, tag, ignore,
				      context, RXD_TAGGED,
				      ep->rx_flags | RXD_TAG_HDR, 0);
}

ssize_t rxd_ep_trecvv(struct fid_ep *ep_fid, const struct iovec *iov, void **desc,
	size_t count, fi_addr_t src_addr, uint64_t tag, uint64_t ignore,
	void *context)
{
	struct rxd_ep *ep;

	ep = container_of(ep_fid, struct rxd_ep, util_ep.ep_fid.fid);

	return rxd_ep_generic_recvmsg(ep, iov, count, src_addr, tag, ignore,
				      context, RXD_TAGGED,
				      ep->rx_flags | RXD_TAG_HDR, 0);
}

ssize_t rxd_ep_trecvmsg(struct fid_ep *ep_fid, const struct fi_msg_tagged *msg,
	uint64_t flags)
{
	struct rxd_ep *ep;

	ep = container_of(ep_fid, struct rxd_ep, util_ep.ep_fid.fid);

	return rxd_ep_generic_recvmsg(ep, msg->msg_iov, msg->iov_count, msg->addr,
				      msg->tag, msg->ignore, msg->context,
				      RXD_TAGGED, rxd_rx_flags(flags |
				      ep->util_ep.rx_msg_flags) | RXD_TAG_HDR, flags);
}

ssize_t rxd_ep_tsend(struct fid_ep *ep_fid, const void *buf, size_t len,
	void *desc, fi_addr_t dest_addr, uint64_t tag, void *context)
{
	struct rxd_ep *ep;
	struct iovec msg_iov;

	ep = container_of(ep_fid, struct rxd_ep, util_ep.ep_fid.fid);

	msg_iov.iov_base = (void *) buf;
	msg_iov.iov_len = len;

	return rxd_ep_generic_sendmsg(ep, &msg_iov, 1, dest_addr, tag,
				      0, context, RXD_TAGGED,
				      ep->tx_flags | RXD_TAG_HDR);
}

ssize_t rxd_ep_tsendv(struct fid_ep *ep_fid, const struct iovec *iov,
	void **desc, size_t count, fi_addr_t dest_addr, uint64_t tag,
	void *context)
{
	struct rxd_ep *ep;

	ep = container_of(ep_fid, struct rxd_ep, util_ep.ep_fid.fid);

	return rxd_ep_generic_sendmsg(ep, iov, count, dest_addr, tag,
				      0, context, RXD_TAGGED,
				      ep->tx_flags | RXD_TAG_HDR);
}

ssize_t rxd_ep_tsendmsg(struct fid_ep *ep_fid, const struct fi_msg_tagged *msg,
			uint64_t flags)
{
	struct rxd_ep *ep;

	ep = container_of(ep_fid, struct rxd_ep, util_ep.ep_fid.fid);

	return rxd_ep_generic_sendmsg(ep, msg->msg_iov, msg->iov_count,
				      msg->addr, msg->tag, msg->data, msg->context,
				      RXD_TAGGED, rxd_tx_flags(flags |
				      ep->util_ep.tx_msg_flags) | RXD_TAG_HDR);
}

ssize_t rxd_ep_tinject(struct fid_ep *ep_fid, const void *buf, size_t len,
		       fi_addr_t dest_addr, uint64_t tag)
{
	struct rxd_ep *ep;
	struct iovec iov;

	ep = container_of(ep_fid, struct rxd_ep, util_ep.ep_fid.fid);

	iov.iov_base = (void *) buf;
	iov.iov_len = len;

	return rxd_ep_generic_inject(ep, &iov, 1, dest_addr, tag, 0,
				     RXD_TAGGED, RXD_NO_TX_COMP | RXD_INJECT |
				     RXD_TAG_HDR);
}

ssize_t rxd_ep_tsenddata(struct fid_ep *ep_fid, const void *buf, size_t len,
		         void *desc, uint64_t data, fi_addr_t dest_addr,
		         uint64_t tag, void *context)
{
	struct rxd_ep *ep;
	struct iovec iov;

	ep = container_of(ep_fid, struct rxd_ep, util_ep.ep_fid.fid);

	iov.iov_base = (void *) buf;
	iov.iov_len = len;

	return rxd_ep_generic_sendmsg(ep, &iov, 1, dest_addr, tag, data, context,
				      RXD_TAGGED, ep->tx_flags |
				      RXD_REMOTE_CQ_DATA | RXD_TAG_HDR);
}

ssize_t rxd_ep_tinjectdata(struct fid_ep *ep_fid, const void *buf, size_t len,
			   uint64_t data, fi_addr_t dest_addr, uint64_t tag)
{
	struct rxd_ep *ep;
	struct iovec iov;

	ep = container_of(ep_fid, struct rxd_ep, util_ep.ep_fid.fid);

	iov.iov_base = (void *) buf;
	iov.iov_len = len;

	return rxd_ep_generic_inject(ep, &iov, 1, dest_addr, tag, data, RXD_TAGGED,
				     RXD_NO_TX_COMP | RXD_INJECT |
				     RXD_REMOTE_CQ_DATA | RXD_TAG_HDR);
}

struct fi_ops_tagged rxd_ops_tagged = {
	.size = sizeof(struct fi_ops_tagged),
	.recv = rxd_ep_trecv,
	.recvv = rxd_ep_trecvv,
	.recvmsg = rxd_ep_trecvmsg,
	.send = rxd_ep_tsend,
	.sendv = rxd_ep_tsendv,
	.sendmsg = rxd_ep_tsendmsg,
	.inject = rxd_ep_tinject,
	.senddata = rxd_ep_tsenddata,
	.injectdata = rxd_ep_tinjectdata,
};
