/*
 * Copyright (c) 2018 Intel Corporation. All rights reserved.
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

#include "ofi_hook.h"


static int hook_setname(fid_t fid, void *addr, size_t addrlen)
{
	return fi_setname(hook_to_hfid(fid), addr, addrlen);
}

static int hook_getname(fid_t fid, void *addr, size_t *addrlen)
{
	return fi_getname(hook_to_hfid(fid), addr, addrlen);
}

static int hook_getpeer(struct fid_ep *ep, void *addr, size_t *addrlen)
{
	struct hook_ep *myep = container_of(ep, struct hook_ep, ep);

	return fi_getpeer(myep->hep, addr, addrlen);
}

static int hook_connect(struct fid_ep *ep, const void *addr,
			const void *param, size_t paramlen)
{
	struct hook_ep *myep = container_of(ep, struct hook_ep, ep);

	return fi_connect(myep->hep, addr, param, paramlen);
}

static int hook_listen(struct fid_pep *pep)
{
	struct hook_pep *mypep = container_of(pep, struct hook_pep, pep);

	return fi_listen(mypep->hpep);
}

static int hook_accept(struct fid_ep *ep, const void *param, size_t paramlen)
{
	struct hook_ep *myep = container_of(ep, struct hook_ep, ep);

	return fi_accept(myep->hep, param, paramlen);
}

static int hook_reject(struct fid_pep *pep, fid_t handle,
		       const void *param, size_t paramlen)
{
	struct hook_pep *mypep = container_of(pep, struct hook_pep, pep);

	return fi_reject(mypep->hpep, handle, param, paramlen);
}

static int hook_shutdown(struct fid_ep *ep, uint64_t flags)
{
	struct hook_ep *myep = container_of(ep, struct hook_ep, ep);

	return fi_shutdown(myep->hep, flags);
}

static int hook_join(struct fid_ep *ep, const void *addr, uint64_t flags,
		     struct fid_mc **mc, void *context)
{
	struct hook_ep *myep = container_of(ep, struct hook_ep, ep);

	return fi_join(myep->hep, addr, flags, mc, context);
}

struct fi_ops_cm hook_cm_ops = {
	.size = sizeof(struct fi_ops_cm),
	.setname = hook_setname,
	.getname = hook_getname,
	.getpeer = hook_getpeer,
	.connect = hook_connect,
	.listen = hook_listen,
	.accept = hook_accept,
	.reject = hook_reject,
	.shutdown = hook_shutdown,
	.join = hook_join,
};
