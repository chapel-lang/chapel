/*
 * Copyright (c) 2017 Intel Corporation. All rights reserved.
 *
 * This software is available to you under a choice of one of two
 * licenses.  You may choose to be licensed under the terms of the GNU
 * General Public License (GPL) Version 2, available from the file
 * COPYING in the main directory of this source tree, or the
 * BSD license below:
 *
 *	   Redistribution and use in source and binary forms, with or
 *	   without modification, are permitted provided that the following
 *	   conditions are met:
 *
 *		- Redistributions of source code must retain the above
 *		  copyright notice, this list of conditions and the following
 *		  disclaimer.
 *
 *		- Redistributions in binary form must reproduce the above
 *		  copyright notice, this list of conditions and the following
 *		  disclaimer in the documentation and/or other materials
 *		  provided with the distribution.
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

#include <rdma/fi_errno.h>

#include <ofi_prov.h>
#include "tcpx.h"
#include <poll.h>

#include <sys/types.h>
#include <ifaddrs.h>
#include <net/if.h>
#include <ofi_util.h>

struct fi_ops_fabric tcpx_fabric_ops = {
	.size = sizeof(struct fi_ops_fabric),
	.domain = tcpx_domain_open,
	.passive_ep = tcpx_passive_ep,
	.eq_open = tcpx_eq_create,
	.wait_open = ofi_wait_fd_open,
	.trywait = ofi_trywait
};

static int tcpx_fabric_close(fid_t fid)
{
	int ret;
	struct tcpx_fabric *tcpx_fabric;

	tcpx_fabric = container_of(fid, struct tcpx_fabric,
				   util_fabric.fabric_fid.fid);

	ret = ofi_fabric_close(&tcpx_fabric->util_fabric);
	if (ret)
		return ret;

	free(tcpx_fabric);
	return 0;
}

struct fi_ops tcpx_fabric_fi_ops = {
	.size = sizeof(struct fi_ops),
	.close = tcpx_fabric_close,
	.bind = fi_no_bind,
	.control = fi_no_control,
	.ops_open = fi_no_ops_open,
};

int tcpx_create_fabric(struct fi_fabric_attr *attr, struct fid_fabric **fabric,
		       void *context)
{
	struct tcpx_fabric *tcpx_fabric;
	int ret;

	tcpx_fabric = calloc(1, sizeof(*tcpx_fabric));
	if (!tcpx_fabric)
		return -FI_ENOMEM;

	ret = ofi_fabric_init(&tcpx_prov, tcpx_info.fabric_attr, attr,
			      &tcpx_fabric->util_fabric, context);
	if (ret) {
		free(tcpx_fabric);
		return ret;
	}

	*fabric = &tcpx_fabric->util_fabric.fabric_fid;
	(*fabric)->fid.ops = &tcpx_fabric_fi_ops;
	(*fabric)->ops = &tcpx_fabric_ops;

	return 0;
}
