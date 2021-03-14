/*
 * Copyright (c) 2016 Intel Corporation, Inc.  All rights reserved.
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

int ofi_fabric_close(struct util_fabric *fabric)
{
	if (ofi_atomic_get32(&fabric->ref))
		return -FI_EBUSY;

	ofi_fabric_remove(fabric);
	free((char *)fabric->name);
	fastlock_destroy(&fabric->lock);
	return 0;
}

int ofi_fabric_init(const struct fi_provider *prov,
		    const struct fi_fabric_attr *prov_attr,
		    const struct fi_fabric_attr *user_attr,
		    struct util_fabric *fabric, void *context)
{
	int ret;

	ret = ofi_check_fabric_attr(prov, prov_attr, user_attr);
	if (ret)
		return ret;

	fabric->prov = prov;
	ofi_atomic_initialize32(&fabric->ref, 0);
	dlist_init(&fabric->domain_list);
	fastlock_init(&fabric->lock);
	fabric->name = strdup(user_attr->name);
	if (!fabric->name)
		return -FI_ENOMEM;

	fabric->fabric_fid.fid.fclass = FI_CLASS_FABRIC;
	fabric->fabric_fid.fid.context = context;
	/*
	 * fabric ops set by provider
	 */
	ofi_fabric_insert(fabric);
	return 0;
}
