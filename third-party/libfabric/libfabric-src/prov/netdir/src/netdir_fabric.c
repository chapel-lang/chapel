/*
* Copyright (c) 2015-2016 Intel Corporation, Inc.  All rights reserved.
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

#ifdef _WIN32

#include <stdlib.h>
#include <malloc.h>

#include "netdir.h"
#include "ofi_util.h"
#include "ofi_enosys.h"
#include "rdma/fabric.h"

#include "netdir_ov.h"
#include "netdir_iface.h"

static int ofi_nd_fabric_close(fid_t fid);

static struct fi_ops ofi_nd_fi_ops = {
	.size = sizeof(ofi_nd_fi_ops),
	.close = ofi_nd_fabric_close,
	.bind = fi_no_bind,
	.control = fi_no_control,
	.ops_open = fi_no_ops_open,
};

static struct fid ofi_nd_fid = {
	.fclass = FI_CLASS_FABRIC,
	.context = NULL,
	.ops = &ofi_nd_fi_ops
};

static struct fi_ops_fabric ofi_nd_fabric_ops = {
	.size = sizeof(ofi_nd_fabric_ops),
	.domain = ofi_nd_domain_open,
	.passive_ep = ofi_nd_passive_endpoint,
	.eq_open = ofi_nd_eq_open,
	.wait_open = fi_no_wait_open,
	.trywait = fi_no_trywait
};

static int ofi_nd_fabric_close(fid_t fid)
{
	struct nd_fabric *fabric;
	fabric = container_of(fid, struct nd_fabric, fid.fid);
	free(fabric);
	/* due to issues in cleanup NetworkDirect on library
	   unload make clening here, on fabric close */
	ofi_nd_shutdown();
	return FI_SUCCESS;
}

int ofi_nd_fabric(struct fi_fabric_attr *attr, struct fid_fabric **fab,
		  void *context)
{
	OFI_UNUSED(context);

	if (attr) {
		if (attr->name && strcmp(attr->name, ofi_nd_prov.name))
			return -FI_EINVAL;
		if (attr->prov_name && strcmp(attr->prov_name, ofi_nd_prov.name))
			return -FI_EINVAL;
		if (attr->prov_version && attr->prov_version != ofi_nd_prov.version)
			return -FI_EINVAL;
	}

	struct nd_fabric *fabric = (struct nd_fabric*)calloc(1, sizeof(*fabric));
	if (!fabric)
		return -FI_ENOMEM;

	struct nd_fabric def = {
		.fid = {
			.fid = ofi_nd_fid,
			.ops = &ofi_nd_fabric_ops
		}
	};

	*fabric = def;

	*fab = &fabric->fid;

	fi_param_get_int(&ofi_nd_prov, "inlinethr", &gl_data.inline_thr);
	fi_param_get_int(&ofi_nd_prov, "prepostcnt", &gl_data.prepost_cnt);
	fi_param_get_int(&ofi_nd_prov, "prepostbufcnt", &gl_data.prepost_buf_cnt);

	gl_data.total_avail = gl_data.prepost_cnt * gl_data.prepost_buf_cnt;

	return FI_SUCCESS;
}

#endif /* _WIN32 */

