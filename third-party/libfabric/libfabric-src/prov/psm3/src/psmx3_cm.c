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

#include "psmx3.h"

DIRECT_FN
STATIC int psmx3_cm_getname(fid_t fid, void *addr, size_t *addrlen)
{
	struct psmx3_fid_ep *ep;
	struct psmx3_fid_sep *sep;
	struct psmx3_ep_name epname;
	size_t	addr_size;
	int err = 0;

	ep = container_of(fid, struct psmx3_fid_ep, ep.fid);
	if (!ep->domain)
		return -FI_EBADF;

	memset(&epname, 0, sizeof(epname));

	if (ep->type == PSMX3_EP_REGULAR) {
		epname.epid = ep->rx ? ep->rx->psm2_epid : psm3_epid_zeroed();
		epname.type = ep->type;
	} else {
		sep = (struct psmx3_fid_sep *)ep;
		epname.epid = sep->ctxts[0].trx_ctxt->psm2_epid;
		epname.sep_id = sep->id;
		epname.type = sep->type;
	}

	if (ep->domain->addr_format == FI_ADDR_STR) {
		addr_size = *addrlen;
		ofi_straddr(addr, &addr_size, FI_ADDR_PSMX3, &epname);
	} else {
		addr_size = sizeof(epname);
		memcpy(addr, &epname, MIN(*addrlen, addr_size));
	}

	if (*addrlen < addr_size)
		err = -FI_ETOOSMALL;

	*addrlen = addr_size;
	return err;
}

struct fi_ops_cm psmx3_cm_ops = {
	.size = sizeof(struct fi_ops_cm),
	.setname = fi_no_setname,
	.getname = psmx3_cm_getname,
	.getpeer = fi_no_getpeer,
	.connect = fi_no_connect,
	.listen = fi_no_listen,
	.accept = fi_no_accept,
	.reject = fi_no_reject,
	.shutdown = fi_no_shutdown,
	.join = fi_no_join,
};

