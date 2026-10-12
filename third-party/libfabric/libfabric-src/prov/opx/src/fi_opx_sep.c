/*
 * Copyright (C) 2016 by Argonne National Laboratory.
 * Copyright (C) 2023-2025 Cornelis Networks.
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
#include <ofi.h>

#include "rdma/opx/fi_opx.h"
#include "rdma/opx/fi_opx_domain.h"
#include "rdma/opx/fi_opx_endpoint.h"

#include <ofi.h>
#include <ofi_enosys.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <errno.h>
#include <unistd.h>
#include <limits.h>

/* forward declaration */
int fi_opx_endpoint_rx_tx(struct fid_domain *dom, struct fi_info *info, struct fid_ep **ep, void *context);

static int fi_opx_close_sep(fid_t fid)
{
	int		   ret;
	struct fi_opx_sep *opx_sep = container_of(fid, struct fi_opx_sep, ep_fid);

	ret = fi_opx_fid_check(fid, FI_CLASS_SEP, "scalable endpoint");
	if (ret) {
		return ret;
	}

	fi_opx_finalize_cm_ops(&opx_sep->ep_fid.fid);

	ret = fi_opx_ref_dec(&opx_sep->av->ref_cnt, "address vector");
	if (ret) {
		return ret;
	}

	// TODO:  Are these cleaned up
	ret = fi_opx_ref_dec(&opx_sep->ref_cnt, "rx");
	ret = fi_opx_ref_dec(&opx_sep->ref_cnt, "tx");

	ret = fi_opx_ref_finalize(&opx_sep->ref_cnt, "scalable endpoint");
	if (ret) {
		return ret;
	}

	ret = fi_opx_ref_dec(&opx_sep->domain->ref_cnt, "domain");
	if (ret) {
		return ret;
	}

	free(opx_sep->info->ep_attr);
	opx_sep->info->ep_attr = NULL;
	free(opx_sep->info);
	opx_sep->info = NULL;
	void *memptr  = opx_sep->memptr;
	free(memptr);
	// opx_sep (the object passed in as fid) is now unusable

	return 0;
}

static int fi_opx_control_sep(fid_t fid, int command, void *arg)
{
	struct fid_ep *ep __attribute__((unused));
	ep = container_of(fid, struct fid_ep, fid);
	return 0;
}

static int fi_opx_bind_sep(struct fid *fid, struct fid *bfid, uint64_t flags)
{
	int		   ret	   = 0;
	struct fi_opx_sep *opx_sep = container_of(fid, struct fi_opx_sep, ep_fid);
	struct fi_opx_av  *opx_av;

	if (!fid || !bfid) {
		errno = FI_EINVAL;
		return -errno;
	}

	switch (bfid->fclass) {
	case FI_CLASS_AV:
		opx_av = container_of(bfid, struct fi_opx_av, av_fid);
		fi_opx_ref_inc(&opx_av->ref_cnt, "address vector");
		opx_sep->av = opx_av;
		break;
	default:
		errno = FI_ENOSYS;
		return -errno;
	}

	return ret;
}

static struct fi_ops fi_opx_fi_ops = {.size	= sizeof(struct fi_ops),
				      .close	= fi_opx_close_sep,
				      .bind	= fi_opx_bind_sep,
				      .control	= fi_opx_control_sep,
				      .ops_open = fi_no_ops_open};

static struct fi_ops_ep fi_opx_sep_ops = {.size		= sizeof(struct fi_ops_ep),
					  .cancel	= fi_no_cancel,
					  .getopt	= fi_no_getopt,
					  .setopt	= fi_no_setopt,
					  .tx_ctx	= fi_no_tx_ctx,
					  .rx_ctx	= fi_no_rx_ctx,
					  .rx_size_left = fi_no_rx_size_left,
					  .tx_size_left = fi_no_tx_size_left};

int fi_opx_scalable_ep(struct fid_domain *domain, struct fi_info *info, struct fid_ep **sep, void *context)
{
	struct fi_opx_sep *opx_sep = NULL;

	if (!info || !domain) {
		errno = FI_EINVAL;
		goto err;
	}

	void *memptr = NULL;
	memptr	     = malloc(sizeof(struct fi_opx_sep) + L2_CACHE_LINE_SIZE);
	if (!memptr) {
		errno = FI_ENOMEM;
		goto err;
	}
	memset(memptr, 0, sizeof(struct fi_opx_sep) + L2_CACHE_LINE_SIZE);
	opx_sep		= (struct fi_opx_sep *) (((uintptr_t) memptr + L2_CACHE_LINE_SIZE) & ~(L2_CACHE_LINE_SIZE - 1));
	opx_sep->memptr = memptr;

	opx_sep->domain = (struct fi_opx_domain *) domain;

	opx_sep->ep_fid.fid.fclass  = FI_CLASS_SEP;
	opx_sep->ep_fid.fid.context = context;
	opx_sep->ep_fid.fid.ops	    = &fi_opx_fi_ops;
	opx_sep->ep_fid.ops	    = &fi_opx_sep_ops;

	opx_sep->info = calloc(1, sizeof(struct fi_info));
	if (!opx_sep->info) {
		errno = FI_ENOMEM;
		goto err;
	}
	memcpy(opx_sep->info, info, sizeof(struct fi_info));
	opx_sep->info->next    = NULL;
	opx_sep->info->ep_attr = calloc(1, sizeof(struct fi_ep_attr));
	if (!opx_sep->info->ep_attr) {
		errno = FI_ENOMEM;
		goto err;
	}
	memcpy(opx_sep->info->ep_attr, info->ep_attr, sizeof(struct fi_ep_attr));

	/*
	 * fi_endpoint.3
	 *
	 * "tx_ctx_cnt - Transmit Context Count
	 * 	Number of transmit contexts to associate with the endpoint. If
	 * 	not specified (0), 1 context will be assigned if the endpoint
	 * 	supports outbound transfers."
	 */
	if (0 == opx_sep->info->ep_attr->tx_ctx_cnt) {
		opx_sep->info->ep_attr->tx_ctx_cnt = 1;
	}

	/*
	 * fi_endpoint.3
	 *
	 * "rx_ctx_cnt - Receive Context Count
	 * 	Number of receive contexts to associate with the endpoint. If
	 * 	not specified, 1 context will be assigned if the endpoint
	 * 	supports inbound transfers."
	 */
	if (0 == opx_sep->info->ep_attr->rx_ctx_cnt) {
		opx_sep->info->ep_attr->rx_ctx_cnt = 1;
	}

	unsigned i;
	for (i = 0; i < FI_OPX_ADDR_SEP_RX_MAX; ++i) {
		opx_sep->ep[i] = NULL;
	}

	int ret = fi_opx_init_cm_ops(&opx_sep->ep_fid.fid, info);
	if (ret) {
		goto err;
	}

	fi_opx_ref_init(&opx_sep->ref_cnt, "scalable endpoint");
	fi_opx_ref_inc(&opx_sep->domain->ref_cnt, "domain");

	*sep = &opx_sep->ep_fid;

	return 0;
err:
	if (opx_sep) {
		fi_opx_finalize_cm_ops(&opx_sep->ep_fid.fid);
		if (opx_sep->info) {
			if (opx_sep->info->ep_attr) {
				free(opx_sep->info->ep_attr);
				opx_sep->info->ep_attr = NULL;
			}
			free(opx_sep->info);
			opx_sep->info = NULL;
		}
		memptr = opx_sep->memptr;
		free(memptr);
		opx_sep = NULL;
	}
	return -errno;
}
