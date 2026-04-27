/*
 * Copyright (c) 2019-2023 GigaIO, Inc. All Rights Reserved.
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

#include "lpp.h"
#include "ofi_hmem.h"

#if HAVE_CUDA || HAVE_ROCR

/* lpp_hmem_ptr() - Checks if a pointer is associated with an hmem_iface
 * @lpp_domain:	The lpp_domain that wants to use the memory
 * @ptr:	The memory in question
 * @iface:	Returns the fi_hmem_iface the ptr belongs to
 *
 *		If no hmem_ifaces are enabled, then FI_HMEM_SYSTEM is returned
 *		Only 1 interface can be configured at a time, i.e. systems with
 *		a mix of Nvidia and AMD gpus won't work.
 */
int lpp_hmem_ptr(struct lpp_domain *lpp_domain, const void *ptr,
		 enum fi_hmem_iface *iface)
{
	if (!iface)
		return -FI_EINVAL;

	*iface = FI_HMEM_SYSTEM;

	if (lpp_domain->enabled_iface == FI_HMEM_SYSTEM)
		return 0;

	if (lpp_domain->enabled_iface == FI_HMEM_ROCR &&
	    rocr_is_addr_valid(ptr, NULL, NULL)) {
		*iface = FI_HMEM_ROCR;
	} else if (lpp_domain->enabled_iface == FI_HMEM_CUDA
		   && cuda_is_addr_valid(ptr, NULL, NULL)) {
		int ret = cuda_set_sync_memops((void *)ptr);
		if (ret)
			return ret;

		*iface = FI_HMEM_CUDA;
	}

	return 0;
}
#endif // HAVE_CUDA || HAVE_ROCR

int lpp_hmem_init(struct lpp_domain *lpp_domainp)
{
	lpp_domainp->enabled_iface = FI_HMEM_SYSTEM;
	if (ofi_hmem_is_initialized(FI_HMEM_ROCR))
		lpp_domainp->enabled_iface = FI_HMEM_ROCR;
	else if (ofi_hmem_is_initialized(FI_HMEM_CUDA))
		lpp_domainp->enabled_iface = FI_HMEM_CUDA;

	return 0;
}
