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

#include "netdir.h"
#include "netdir_ov.h"
#include "netdir_iface.h"

#include "rdma/fabric.h"
#include "ofi_util.h"

static int ofi_nd_cntr_close(struct fid *fid);
static uint64_t ofi_nd_cntr_read(struct fid_cntr *cntr);
static uint64_t ofi_nd_cntr_readerr(struct fid_cntr *cntr);
static int ofi_nd_cntr_add(struct fid_cntr *cntr, uint64_t value);
static int ofi_nd_cntr_set(struct fid_cntr *cntr, uint64_t value);
static int ofi_nd_cntr_wait(struct fid_cntr *cntr,
			    uint64_t threshold, int timeout);

static struct fi_ops ofi_nd_fi_ops = {
	.size = sizeof(ofi_nd_fi_ops),
	.close = ofi_nd_cntr_close,
	.bind = fi_no_bind,
	.control = fi_no_control,
	.ops_open = fi_no_ops_open,
};

static struct fid ofi_nd_fid = {
	.fclass = FI_CLASS_CNTR,
	.context = NULL,
	.ops = &ofi_nd_fi_ops
};

static struct fi_ops_cntr ofi_nd_cntr_ops = {
	.size = sizeof(ofi_nd_cntr_ops),
	.read = ofi_nd_cntr_read,
	.readerr = ofi_nd_cntr_readerr,
	.add = ofi_nd_cntr_add,
	.set = ofi_nd_cntr_set,
	.wait = ofi_nd_cntr_wait
};

static int ofi_nd_cntr_close(struct fid *fid)
{
	assert(fid->fclass == FI_CLASS_CNTR);
	if (fid->fclass != FI_CLASS_CQ)
		return -FI_EINVAL;

	struct nd_cntr *cntr = container_of(fid, struct nd_cntr, fid.fid);

	free(cntr);

	return FI_SUCCESS;
}

int ofi_nd_cntr_open(struct fid_domain *pdomain, struct fi_cntr_attr *attr,
		     struct fid_cntr **pcntr, void *context)
{
	OFI_UNUSED(context);

	assert(pdomain);
	assert(pdomain->fid.fclass == FI_CLASS_DOMAIN);

	if (attr) {
		if (attr->wait_obj != FI_WAIT_NONE &&
		    attr->wait_obj != FI_WAIT_UNSPEC)
			return -FI_EBADFLAGS;
	}

	struct nd_cntr *cntr = (struct nd_cntr*)calloc(1, sizeof(*cntr));
	if (!cntr)
		return -FI_ENOMEM;

	struct nd_cntr def = {
		.fid = {
			.fid = ofi_nd_fid,
			.ops = &ofi_nd_cntr_ops
		},
	};

	*cntr = def;

	*pcntr = &cntr->fid;

	return FI_SUCCESS;
}

static uint64_t ofi_nd_cntr_read(struct fid_cntr *pcntr)
{
	assert(pcntr);
	assert(pcntr->fid.fclass == FI_CLASS_CNTR);

	struct nd_cntr *cntr = container_of(pcntr, struct nd_cntr, fid);
	return cntr->counter;
}

static uint64_t ofi_nd_cntr_readerr(struct fid_cntr *pcntr)
{
	assert(pcntr);
	assert(pcntr->fid.fclass == FI_CLASS_CNTR);

	struct nd_cntr *cntr = container_of(pcntr, struct nd_cntr, fid);
	return cntr->err;
}

static int ofi_nd_cntr_add(struct fid_cntr *pcntr, uint64_t value)
{
	assert(pcntr);
	assert(pcntr->fid.fclass == FI_CLASS_CNTR);

	if (pcntr->fid.fclass != FI_CLASS_CNTR)
		return -FI_EINVAL;

	struct nd_cntr *cntr = container_of(pcntr, struct nd_cntr, fid);

	cntr->counter += value;
	WakeByAddressAll((void*)&cntr->counter);

	return FI_SUCCESS;
}

static int ofi_nd_cntr_set(struct fid_cntr *pcntr, uint64_t value)
{
	assert(pcntr);
	assert(pcntr->fid.fclass == FI_CLASS_CNTR);

	if (pcntr->fid.fclass != FI_CLASS_CNTR)
		return -FI_EINVAL;

	struct nd_cntr *cntr = container_of(pcntr, struct nd_cntr, fid);

	cntr->counter = value;
	WakeByAddressAll((void*)&cntr->counter);

	return FI_SUCCESS;
}

static int ofi_nd_cntr_wait(struct fid_cntr *pcntr, 
			    uint64_t threshold, int timeout)
{
	assert(pcntr);
	assert(pcntr->fid.fclass == FI_CLASS_CNTR);

	if (pcntr->fid.fclass != FI_CLASS_CNTR)
		return -FI_EINVAL;

	struct nd_cntr *cntr = container_of(pcntr, struct nd_cntr, fid);

	/* process corner timeouts separately to optimize */
	if (!timeout) {	/* no wait */
		return (cntr->counter >= (LONGLONG)threshold) ?
			FI_SUCCESS : -FI_ETIMEDOUT;
	}
	else if (timeout < 0) {	/* infinite wait */
		while (cntr->counter < (LONG64)threshold) {
			LONG64 val = cntr->counter;
			WaitOnAddress(&cntr->counter, &val,
				      sizeof(val), INFINITE);
		}
		return FI_SUCCESS;
	}
	else { /* timeout wait */
		OFI_ND_TIMEOUT_INIT(timeout);

		do {
			if (cntr->counter >= (LONG64)threshold)
				return FI_SUCCESS;
			LONG64 val = cntr->counter;
			WaitOnAddress(&cntr->counter, &val,
				      sizeof(val), timeout);
		} while (!OFI_ND_TIMEDOUT());
	}

	return FI_SUCCESS;
}

#endif /* _WIN32 */

