/*
 * Copyright (c) 2014-2019, Cisco Systems, Inc. All rights reserved.
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
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include "config.h"

#include <asm/types.h>
#include <assert.h>
#include <errno.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <ofi_epoll.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <rdma/fabric.h>
#include <rdma/fi_cm.h>
#include <rdma/fi_domain.h>
#include <rdma/fi_endpoint.h>
#include <rdma/fi_rma.h>
#include <rdma/fi_errno.h>
#include "ofi.h"
#include "ofi_enosys.h"

#include "usdf.h"
#include "usdf_endpoint.h"
#include "usdf_cm.h"

int
usdf_endpoint_open(struct fid_domain *domain, struct fi_info *info,
	    struct fid_ep **ep_o, void *context)
{
	USDF_TRACE_SYS(EP_CTRL, "\n");

	switch (info->ep_attr->type) {
	case FI_EP_DGRAM:
		return usdf_ep_dgram_open(domain, info, ep_o, context);
	default:
		return -FI_ENODEV;
	}
}

int usdf_ep_getopt_connected(fid_t fid, int level, int optname, void *optval,
		size_t *optlen)
{
	size_t *cm_size;
	size_t dest_size;

	USDF_TRACE_SYS(EP_CTRL, "\n");

	if (!optval || !optlen)
		return -FI_EINVAL;

	if (level != FI_OPT_ENDPOINT)
		return -FI_ENOPROTOOPT;

	switch (optname) {
	case FI_OPT_CM_DATA_SIZE:
		dest_size = *optlen;
		*optlen = sizeof(*cm_size);

		if (dest_size < sizeof(*cm_size))
			return -FI_ETOOSMALL;

		cm_size = optval;
		*cm_size = USDF_MAX_CONN_DATA;
		break;
	default:
		return -FI_ENOPROTOOPT;
	}

	return FI_SUCCESS;
}

int usdf_ep_getopt_unconnected(fid_t fid, int level, int optname, void *optval,
		size_t *optlen)
{
	USDF_TRACE_SYS(EP_CTRL, "\n");

	return -FI_ENOPROTOOPT;
}


int usdf_ep_setopt(fid_t fid, int level, int optname, const void *optval,
		size_t optlen)
{
	USDF_TRACE_SYS(EP_CTRL, "\n");

	return -FI_ENOPROTOOPT;
}
