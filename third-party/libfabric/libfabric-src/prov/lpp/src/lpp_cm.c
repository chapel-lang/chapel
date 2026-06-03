/*
 * Copyright (c) 2018-2024 GigaIO, Inc. All Rights Reserved.
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

#include <arpa/inet.h>

#include "lpp.h"

int lpp_fi_getname(fid_t fid, void *addr, size_t *addrlen)
{
	struct lpp_ep *lpp_epp = container_of(fid, struct lpp_ep, ep_fid);
	struct lpp_domain *domain = lpp_epp->domain;
	struct sockaddr_in *sin;

	if (fid->fclass != FI_CLASS_EP)
		return -FI_EINVAL;

	if (!lpp_epp->is_enabled) {
		FI_WARN(&lpp_prov, FI_LOG_EP_CTRL,
		    "endpoint is not enabled, cannot get name\n");
		return -FI_EINVAL;
	}

	if (*addrlen < sizeof(*sin)) {
		*addrlen = sizeof(*sin);
		return -FI_ETOOSMALL;
	}

	sin = (struct sockaddr_in *)addr;
	lpp_make_sockaddr(0, domain->devinfo.host_index, lpp_epp->port, sin,
			  addrlen);

	return 0;
}

int lpp_fi_setname(fid_t fid, void *addr, size_t addrlen)
{
	struct lpp_ep *lpp_epp = container_of(fid, struct lpp_ep, ep_fid);
	struct sockaddr_in *sin;
	uint32_t host_index;

	if (fid->fclass != FI_CLASS_EP)
		return -FI_EINVAL;

	if (lpp_epp->is_enabled) {
		FI_WARN(&lpp_prov, FI_LOG_EP_CTRL,
			"endpoint enabled, cannot set name\n");
		return -FI_EINVAL;
	}

	if (addrlen != sizeof(*sin)) {
		FI_WARN(&lpp_prov, FI_LOG_EP_CTRL,
			"addrlen %zu doesn't match expected %zu\n", addrlen,
			sizeof(*sin));
		return -FI_EINVAL;
	}

	sin = (struct sockaddr_in *)addr;

	// TODO: do we care about the value of host_index?
	return lpp_from_sockaddr(sin, &host_index, &lpp_epp->port, false);
}
