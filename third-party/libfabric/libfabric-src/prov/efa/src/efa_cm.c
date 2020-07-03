/*
 * Copyright (c) 2013-2015 Intel Corporation, Inc.  All rights reserved.
 * Copyright (c) 2017-2019 Amazon.com, Inc. or its affiliates. All rights reserved.
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

#include "config.h"
#include "efa.h"

static int efa_copy_addr(void *dst_addr, size_t *dst_addrlen, void *src_addr)
{
	size_t len = MIN(*dst_addrlen, EFA_EP_ADDR_LEN);

	memcpy(dst_addr, src_addr, len);
	*dst_addrlen = EFA_EP_ADDR_LEN;

	return (len == EFA_EP_ADDR_LEN) ? 0 : -FI_ETOOSMALL;
}

static int efa_ep_getname(fid_t ep_fid, void *addr, size_t *addrlen)
{
	struct efa_ep_addr *ep_addr;
	struct efa_ep *ep;
	char str[INET6_ADDRSTRLEN] = {};

	ep = container_of(ep_fid, struct efa_ep, util_ep.ep_fid);

	ep_addr = (struct efa_ep_addr *)ep->src_addr;
	ep_addr->qpn = ep->qp->qp_num;
	ep_addr->pad = 0;
	ep_addr->qkey = ep->qp->qkey;

	inet_ntop(AF_INET6, ep_addr->raw, str, INET6_ADDRSTRLEN);

	EFA_INFO(FI_LOG_EP_CTRL, "EP addr: GID[%s] QP[%d] QKEY[%d] (length %zu)\n",
		 str, ep_addr->qpn, ep_addr->qkey, *addrlen);

	return efa_copy_addr(addr, addrlen, ep_addr);
}

struct fi_ops_cm efa_ep_cm_ops = {
	.size = sizeof(struct fi_ops_cm),
	.setname = fi_no_setname,
	.getname = efa_ep_getname,
	.getpeer = fi_no_getpeer,
	.connect = fi_no_connect,
	.listen = fi_no_listen,
	.accept = fi_no_accept,
	.reject = fi_no_reject,
	.shutdown = fi_no_shutdown,
	.join = fi_no_join,
};
