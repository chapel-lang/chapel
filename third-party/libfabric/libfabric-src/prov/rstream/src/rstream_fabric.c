/*
 * Copyright (c) 2017-2018 Intel Corporation. All rights reserved.
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

#include "rstream.h"


static int rstream_fabric_close(fid_t fid)
{
	struct rstream_fabric *rstream_fabric =
		container_of(fid, struct rstream_fabric,
		util_fabric.fabric_fid.fid);
	int ret;

	ret = fi_close(&rstream_fabric->msg_fabric->fid);
	if (ret)
		return ret;

	ret = ofi_fabric_close(&rstream_fabric->util_fabric);
	if (ret)
		return ret;

	free(rstream_fabric);
	return 0;
}

static int rstream_control(struct fid *fid, int command, void *arg)
{
	return -FI_ENOSYS;
}

int rstream_trywait(struct fid_fabric *fabric, struct fid **fids, int count)
{
	int ret;
	struct rstream_ep *rstream_ep;
	struct rstream_fabric *rstream_fabric;
	int num_fids = 1;
	struct fid *rstream_fids[num_fids];

	if (count != num_fids)
		return -FI_ENOSYS;

	if (fids[0]->fclass == FI_CLASS_EP) {
		rstream_ep = container_of(fids[0], struct rstream_ep,
			util_ep.ep_fid.fid);
		rstream_fabric = container_of(fabric, struct rstream_fabric,
			util_fabric.fabric_fid);
		rstream_fids[0] = &rstream_ep->cq->fid;
		ret = fi_trywait(rstream_fabric->msg_fabric, rstream_fids,
			num_fids);
		return ret;
	}

	return -FI_EINVAL;
}

static struct fi_ops rstream_fabric_fi_ops = {
	.size = sizeof(struct fi_ops),
	.close = rstream_fabric_close,
	.bind = fi_no_bind,
	.control = rstream_control,
	.ops_open = fi_no_ops_open,
};

static struct fi_ops_fabric rstream_fabric_ops = {
	.size = sizeof(struct fi_ops_fabric),
	.domain = rstream_domain_open,
	.passive_ep = rstream_passive_ep,
	.eq_open = rstream_eq_open,
	.wait_open = fi_no_wait_open,
	.trywait = rstream_trywait
};

int rstream_fabric_open(struct fi_fabric_attr *attr, struct fid_fabric **fabric,
			   void *context)
{
	struct rstream_fabric *rstream_fabric;
	int ret;
	struct fi_info *info = NULL;

	rstream_fabric = calloc(1, sizeof(*rstream_fabric));
	if (!rstream_fabric)
		return -FI_ENOMEM;

	ret = ofi_fabric_init(&rstream_prov, &rstream_fabric_attr, attr,
			&rstream_fabric->util_fabric, context);
	if (ret)
		goto err1;

	ret = ofi_get_core_info_fabric(&rstream_prov, attr, &info);
	if (ret) {
		FI_WARN(&rstream_prov, FI_LOG_FABRIC, "core info failed\n");
		ret = -FI_EINVAL;
		goto err1;
	}

	ret = fi_fabric(info->fabric_attr, &rstream_fabric->msg_fabric, context);
	if (ret) {
		FI_WARN(&rstream_prov, FI_LOG_FABRIC, "fi_fabric failed\n");
		ret = -FI_EINVAL;
		goto err1;
	}

	*fabric = &rstream_fabric->util_fabric.fabric_fid;
	(*fabric)->fid.ops = &rstream_fabric_fi_ops;
	(*fabric)->ops = &rstream_fabric_ops;

	fi_freeinfo(info);
	return 0;
err1:
	free(rstream_fabric);
	if (info)
		fi_freeinfo(info);

	return ret;
}
