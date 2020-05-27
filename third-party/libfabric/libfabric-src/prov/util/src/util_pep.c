/*
 * Copyright (c) 2017 Intel Corporation, Inc.  All rights reserved.
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

#include <stdlib.h>
#include <string.h>

#include <ofi_enosys.h>
#include <ofi_util.h>

int ofi_pep_bind_eq(struct util_pep *pep, struct util_eq *eq, uint64_t flags)
{
	if (flags) {
		FI_WARN(pep->fabric->prov, FI_LOG_EP_CTRL, "Invalid flags\n");
		return -FI_EINVAL;
	}

	if (pep->fabric != eq->fabric) {
		FI_WARN(pep->fabric->prov, FI_LOG_EP_CTRL,
			"Cannot bind Passive EP and EQ on different fabrics\n");
		return -FI_EINVAL;
	}

	pep->eq = eq;
	ofi_atomic_inc32(&pep->eq->ref);
	return 0;
}

int ofi_pep_init(struct fid_fabric *fabric,  struct fi_info *info,
		 struct util_pep *pep, void *context)
{
	pep->pep_fid.fid.fclass = FI_CLASS_PEP;
	pep->fabric = container_of(fabric, struct util_fabric, fabric_fid);
	pep->pep_fid.fid.context = context;

	return 0;
}

int ofi_pep_close(struct util_pep *pep)
{
	if (pep->eq)
		ofi_atomic_dec32(&pep->eq->ref);

	return 0;
}
