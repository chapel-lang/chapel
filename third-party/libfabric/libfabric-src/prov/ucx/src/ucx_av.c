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

static int ucx_av_write_event(struct ucx_av *av, uint64_t data, int err,
			      void *context)
{
	struct fi_eq_err_entry entry;
	size_t size;
	uint64_t flags;

	entry.fid = &av->av.fid;
	entry.context = context;
	entry.data = data;

	if (err) {
		entry.err = err;
		size = sizeof(struct fi_eq_err_entry);
		flags = UTIL_FLAG_ERROR;
	} else {
		size = sizeof(struct fi_eq_entry);
		flags = 0;
	}

	fi_eq_write(&av->eq->eq_fid, FI_AV_COMPLETE, &entry, size, flags);
	return FI_SUCCESS;
}

static int ucx_av_remove(struct fid_av *fi_av, fi_addr_t *fi_addr, size_t count,
			 uint64_t flags)
{
	struct ucx_av *av;
	struct ucx_ave *ep_ave;
	int i;

	av = container_of(fi_av, struct ucx_av, av);
	if (av->async && !av->eq)
		return -FI_ENOEQ;

	for (i = 0; i < count; ++i) {
		ep_ave = (struct ucx_ave *) fi_addr[i];
		ucp_ep_destroy((ucp_ep_h) ep_ave->uep);
	}
	return FI_SUCCESS;
}

static inline int ucx_av_resolve_if_addr(const struct sockaddr *saddr,
					 char **address)
{
	char peer_host[INET_ADDRSTRLEN] = {0};
	char peer_serv[INET_ADDRSTRLEN] = {0};
	int intserv, peer_host_len, peer_serv_len;
	int rv;

	peer_host_len = peer_serv_len = INET_ADDRSTRLEN;
	rv = getnameinfo(saddr, sizeof(struct sockaddr_in),
			 peer_host, peer_host_len,
			 peer_serv, peer_serv_len,
			 NI_NUMERICSERV|NI_NUMERICHOST);
	if (0 != rv) {
		FI_WARN(&ucx_prov, FI_LOG_CORE,
			"Unable to resolve address: %s \n",
			gai_strerror(rv));
		return -FI_EINVAL;
	}

	intserv = atoi(peer_serv);
	(*address) = ofi_ns_resolve_name(&ucx_descriptor.name_serv,
					 peer_host, &intserv);
	if (!(*address)) {
		FI_WARN(&ucx_prov, FI_LOG_CORE,
			"Unable to resolve address: %s:%s\n",
			peer_host, peer_serv);
		return -FI_EINVAL;
	}
	return FI_SUCCESS;
}

static int ucx_av_insert(struct fid_av *fi_av, const void *addr, size_t count,
			 fi_addr_t *fi_addr, uint64_t flags, void *context)
{
	struct ucx_av *av;
	struct ucx_ep *ep;
	size_t i;
	ucs_status_t status = UCS_OK;
	int added = 0;

	av = container_of(fi_av, struct ucx_av, av);
	ep = av->ep;

	if ((av->async) && (!av->eq))
		return -FI_ENOEQ;

	if (!ofi_array_at(&av->ave_array, av->count + count))
		return -FI_ENOMEM;

	if (ucx_descriptor.enable_spawn) {
		if (!ofi_array_at(&av->addr_array, av->count + count))
			return -FI_ENOMEM;
	}

	for (i = 0; i < count ; ++i) {
		struct ucx_ave *ep_ave;
		void *ep_addr;
		ucp_ep_params_t ep_params = { 0 };
		struct sockaddr_in *sock_addr = (struct sockaddr_in *) addr;

		if (ucx_descriptor.use_ns) {
			if (ucx_av_resolve_if_addr(
				(struct sockaddr*) &sock_addr[i],
				(char**) &ep_params.address) != FI_SUCCESS)
				break;

		} else {
			ep_params.address =
				(const ucp_address_t *)
					&((const char *) addr)[i*av->addr_len];
		}
		ep_params.field_mask = UCP_EP_PARAM_FIELD_REMOTE_ADDRESS;

		FI_INFO(&ucx_prov, FI_LOG_CORE,
			"Try to insert address #%zd, offset=%zd (size=%zd) fi_addr=%p\n",
			i, i * av->addr_len, count, fi_addr);

		ep_ave = ofi_array_at(&av->ave_array, av->count);

		if (ucx_descriptor.enable_spawn) {
			ep_addr = ofi_array_at(&av->addr_array, av->count);
			memcpy(ep_addr, &((const char *) addr)[i*av->addr_len],
			       av->addr_len);
			ep_ave->addr = ep_addr;
		} else {
			ep_ave->addr = NULL;
		}

		fi_addr[i] = (fi_addr_t)ep_ave;

		status = ucp_ep_create(ep->worker, &ep_params, &ep_ave->uep);
		if (status == UCS_OK) {
			FI_INFO(&ucx_prov, FI_LOG_CORE, "address inserted\n");
			added++;
			av->count++;
		} else {
			if (av->eq) {
				ucx_av_write_event(av, i,
						   ucx_translate_errcode(status),
						   context);
			}
			break;
		}
	}

	if (av->eq) {
		ucx_av_write_event(av, added, 0, context);
		count = 0;
	} else {
		count = added;
	}
	return count;
}

static int ucx_av_close(fid_t fid)
{
	struct ucx_av *av;

	av = container_of(fid, struct ucx_av, av);
	ofi_array_destroy(&av->addr_array);
	ofi_array_destroy(&av->ave_array);
	free(av);
	return FI_SUCCESS;
}

static int ucx_av_bind(struct fid *fid, struct fid *bfid, uint64_t flags)
{
	struct ucx_av *av;
	struct util_eq *eq;

	av = container_of(fid, struct ucx_av, av.fid);
	if (!av->async || bfid->fclass != FI_CLASS_EQ) {
		FI_WARN(&ucx_prov, FI_LOG_EP_CTRL,
			"Only EQ can be bound to AV, "
			"and the AV must be asynchronous\n");
		return -FI_EINVAL;
	}
	eq = container_of(bfid, struct util_eq, eq_fid.fid);
	av->eq = eq;
	return FI_SUCCESS;
}

static int ucx_av_lookup(struct fid_av *av, fi_addr_t fi_addr, void *addr,
			 size_t *addrlen)
{
	struct ucx_ave *ave;
	struct ucx_av *mav;
	size_t realsz;

	ave = (struct ucx_ave*) fi_addr;
	mav = container_of(av, struct ucx_av, av.fid);
	realsz = MIN(*addrlen, mav->addr_len);
	memcpy(addr, ave->addr, realsz);
	*addrlen = mav->addr_len;
	return FI_SUCCESS;
}

static struct fi_ops ucx_fi_ops = {
	.size = sizeof(struct fi_ops),
	.close = ucx_av_close,
	.bind = ucx_av_bind,
};

static struct fi_ops_av ucx_av_ops = {
	.size = sizeof(struct fi_ops_av),
	.insert = ucx_av_insert,
	.remove = ucx_av_remove,
	.lookup = ucx_av_lookup,
};

int ucx_av_open(struct fid_domain *fi_domain, struct fi_av_attr *attr,
		struct fid_av **fi_av, void *context)
{
	struct ucx_domain *domain;
	struct ucx_av *av;
	int type = FI_AV_MAP;
	int is_async = 0;

	domain = container_of(fi_domain, struct ucx_domain, u_domain.domain_fid);

	if (attr) {
		switch (attr->type) {
		case FI_AV_MAP:
			type = attr->type;
			break;
		case FI_AV_UNSPEC:
			/* Set FI_AV_MAP by default */
			type = FI_AV_MAP;
			break;
		default:
			return -EINVAL;
		}

		if (attr->name || attr->map_addr)
			return -EINVAL;

		if (attr->flags & FI_EVENT)
			is_async = 1;
	}

	av = (struct ucx_av *) calloc(1, sizeof(struct ucx_av));
	if (!av)
		return -ENOMEM;

	av->domain = domain;
	av->async = is_async;
	av->type = type;
	av->eq = NULL;

	if (ucx_descriptor.use_ns)
		av->addr_len = sizeof(struct sockaddr_in);
	else
		av->addr_len = FI_UCX_MAX_NAME_LEN;

	ofi_array_init(&av->ave_array, sizeof(struct ucx_ave), NULL);
	ofi_array_init(&av->addr_array, av->addr_len, NULL);

	av->av.fid.fclass = FI_CLASS_AV;
	av->av.fid.context = context;
	av->av.fid.ops = &ucx_fi_ops;
	av->av.ops = &ucx_av_ops;

	*fi_av = &av->av;
	return FI_SUCCESS;
}
