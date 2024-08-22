/*
 * Copyright (c) 2016-2023 Intel Corporation. All rights reserved.
 *
 * This software is available to you under a choice of one of two
 * licenses.  You may choose to be licensed under the terms of the GNU
 * General Public License (GPL) Version 2, available from the file
 * COPYING in the main directory of this source tree, or the
 * OpenFabrics.org BSD license below:
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
#include "ucx.h"
#include <inttypes.h>

static int ucx_cm_getname_ucx_format(fid_t fid, void *addr, size_t *addrlen)
{
	ucs_status_t status = UCS_OK;
	void *addr_local = NULL;
	size_t addr_len_local;
	struct ucx_ep* ep;
	int ofi_status = FI_SUCCESS;

	ep = container_of(fid, struct ucx_ep, ep.ep_fid.fid);

	status = ucp_worker_get_address(ep->worker,
					(ucp_address_t **) &addr_local,
					(size_t*) &addr_len_local);
	if (status != UCS_OK) {
		FI_WARN(&ucx_prov, FI_LOG_CORE,
			"ucp_worker_get_address error!\n");
		return ucx_translate_errcode(status);
	}

	if (addr_len_local > FI_UCX_MAX_NAME_LEN) {
		FI_WARN(&ucx_prov, FI_LOG_CORE,
			"Address returned by UCX is too long %"PRIu64"\n",
			addr_len_local);
		return -FI_EINVAL;
	}

	if ((*addrlen) < FI_UCX_MAX_NAME_LEN) {
		FI_WARN(&ucx_prov, FI_LOG_CORE,
			"Buffer storage for ep address is too small %"PRIu64
			" instead of %d [%s]\n",
			*addrlen, FI_UCX_MAX_NAME_LEN, (char *)addr_local);
		ofi_status = -FI_ETOOSMALL;
	}
	FI_INFO(&ucx_prov, FI_LOG_CORE, "Loaded UCP address: [%"PRIu64"]...\n",
		addr_len_local);

	if (addr_local != NULL)
		memcpy(addr, addr_local, ((*addrlen < addr_len_local) ?
						*addrlen : addr_len_local));

	*addrlen = FI_UCX_MAX_NAME_LEN;
	ucp_worker_release_address(ep->worker, (ucp_address_t *) addr_local);
	return ofi_status;
}

static int ucx_cm_getname_ai_format(fid_t fid, void *addr, size_t *addrlen)
{
	int ofi_status = FI_SUCCESS;
	struct ucx_ep* ep = container_of(fid, struct ucx_ep, ep.ep_fid.fid);

	if (ep->addr) {
		if (ep->addr_len > *addrlen) {
			ofi_status = -FI_ETOOSMALL;
			FI_WARN(&ucx_prov, FI_LOG_EP_CTRL,
				"addrlen expected: %"PRIu64", got: %"PRIu64"\n",
				ep->addr_len, *addrlen);
		} else {
			memcpy(addr, ep->addr, ep->addr_len);
		}
		*addrlen = ep->addr_len;
	} else {
		char *hostname = ucx_descriptor.localhost;
		int service = (((getpid() & 0xFFFF)));
		struct addrinfo hints = {
			.ai_family = AF_INET,
			.ai_socktype = SOCK_STREAM,
			.ai_protocol = IPPROTO_TCP,
		};
		struct addrinfo *res;

		if (getaddrinfo(hostname, NULL, &hints, &res) != 0) {
			FI_WARN(&ucx_prov, FI_LOG_CORE,
				"Unable to resolve hostname:%s\n", hostname);
			return -FI_EAVAIL;
		}
		FI_INFO(&ucx_prov, FI_LOG_CORE,
			"Loaded IPv4 address: [%jd]%s:%d\n",
			(intmax_t) res->ai_addrlen, hostname, service);

		if (res->ai_addrlen > *addrlen) {
			ofi_status = -FI_ETOOSMALL;
			FI_WARN(&ucx_prov, FI_LOG_EP_CTRL,
				"addrlen expected: %jd, got: %"PRIu64"\n",
				(intmax_t) res->ai_addrlen, *addrlen);
		} else {
			memcpy(addr, res->ai_addr, res->ai_addrlen);
			((struct sockaddr_in *) addr)->sin_port =
				htons((short)service);
		}

		*addrlen = res->ai_addrlen;

		freeaddrinfo(res);
	}

	return ofi_status;
}

static int ucx_cm_getname(fid_t fid, void *addr, size_t *addrlen)
{
	return ucx_descriptor.use_ns ?
		ucx_cm_getname_ai_format(fid, addr, addrlen) :
		ucx_cm_getname_ucx_format(fid, addr, addrlen);
}

struct fi_ops_cm ucx_cm_ops = {
	.size = sizeof(struct fi_ops_cm),
	.getname = ucx_cm_getname,
	.getpeer = fi_no_getpeer,
	.connect = fi_no_connect,
	.listen = fi_no_listen,
	.accept = fi_no_accept,
	.reject = fi_no_reject,
	.shutdown = fi_no_shutdown,
};
