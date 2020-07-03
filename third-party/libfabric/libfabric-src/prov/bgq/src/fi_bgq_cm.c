/*
 * Copyright (C) 2016 by Argonne National Laboratory.
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
#include "rdma/bgq/fi_bgq.h"
#include <ofi_enosys.h>

int fi_bgq_getname(fid_t fid, void *addr, size_t *addrlen)
{

	if (*addrlen == 0) {
		*addrlen = 24;
		return 0;
	}

	if (!fid || !addr || !addrlen) {
		errno = FI_EINVAL;
		return -errno;
	}

	if (*addrlen < 24) {
		errno = FI_ETOOSMALL;
		return -errno;
	}

	char * addr_str;
	struct fi_bgq_ep *bgq_ep;
	struct fi_bgq_sep *bgq_sep;
	switch(fid->fclass) {
	case FI_CLASS_EP:
		bgq_ep = container_of(fid, struct fi_bgq_ep, ep_fid);
		addr_str = (char *) addr;
		sprintf(addr_str, "%u.%u.%u.%u.%u.%u",
			bgq_ep->domain->my_coords.a,
			bgq_ep->domain->my_coords.b,
			bgq_ep->domain->my_coords.c,
			bgq_ep->domain->my_coords.d,
			bgq_ep->domain->my_coords.e,
			bgq_ep->domain->my_coords.t);
		break;
	case FI_CLASS_SEP:
                bgq_sep = container_of(fid, struct fi_bgq_sep, ep_fid);
                addr_str = (char *) addr;
                sprintf(addr_str, "%u.%u.%u.%u.%u.%u",
                        bgq_sep->domain->my_coords.a,
                        bgq_sep->domain->my_coords.b,
                        bgq_sep->domain->my_coords.c,
                        bgq_sep->domain->my_coords.d,
                        bgq_sep->domain->my_coords.e,
                        bgq_sep->domain->my_coords.t);
                break;

	default:
		errno = FI_EINVAL;
		return -errno;
	}

	*addrlen = 24;

	return 0;
}

static struct fi_ops_cm fi_bgq_cm_ops = {
	.size		= sizeof(struct fi_ops_cm),
	.getname 	= fi_bgq_getname,
	.getpeer 	= fi_no_getpeer,
	.connect 	= fi_no_connect,
	.listen  	= fi_no_listen,
	.accept  	= fi_no_accept,
	.reject  	= fi_no_reject,
	.shutdown 	= fi_no_shutdown,
};

int fi_bgq_init_cm_ops(struct fid_ep *ep_fid, struct fi_info *info)
{
	ep_fid->cm	   = &fi_bgq_cm_ops;

	return 0;
}

int fi_bgq_finalize_cm_ops(struct fi_bgq_ep *bgq_ep)
{
	return 0;
}
