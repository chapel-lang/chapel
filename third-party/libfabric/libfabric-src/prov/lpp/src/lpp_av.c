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

#include "lpp.h"

#define LPP_SOCKADDR_MAGIC (100)

struct lpp_sockaddr {
	uint8_t magic;
	uint8_t fabric_index;
	uint16_t host_index;
} __attribute__((packed));

static const struct fi_ops lpp_fi_av_ops = {
	.size      = sizeof(struct fi_ops),
	.close     = lpp_fi_av_close,
	.bind      = fi_no_bind,
	.control   = fi_no_control,
	.ops_open  = fi_no_ops_open,
};

static const struct fi_ops_av lpp_av_ops = {
	.size      = sizeof(struct fi_ops_av),
	.insert    = lpp_fi_av_insert,
	.insertsvc = fi_no_av_insertsvc,
	.insertsym = fi_no_av_insertsym,
	.remove    = fi_no_av_remove,
	.lookup    = lpp_fi_av_lookup,
	.straddr   = lpp_fi_av_straddr,
};

static const struct fi_av_attr lpp_default_av_attrs = {
	.type         = FI_AV_MAP,
	.rx_ctx_bits  = 0,
	.count        = 256,
	.ep_per_node  = 0,
	.name         = NULL,
	.map_addr     = NULL,
	.flags        = 0,
};

static int lpp_av_verify_attr(struct fi_av_attr *attr)
{
	// MPICH can run on MAP type.
	if (attr->type != FI_AV_MAP) {
		FI_WARN(&lpp_prov, FI_LOG_AV, "Wrong AV attribs type\n");
		return -FI_EINVAL;
	}

	// We don't suport events or symmetric.
	if ((attr->flags & (FI_EVENT | FI_SYMMETRIC)) != 0) {
		FI_WARN(&lpp_prov, FI_LOG_AV, "Wrong AV attribs flags\n");
		return -FI_EINVAL;
	}

	// MPICH doesn't demand named AVs.
	if ((attr->name != NULL) || (attr->map_addr != NULL)) {
		return -FI_ENOSYS;
	}

	return 0;
}

void lpp_make_sockaddr(uint8_t fabric_index, uint32_t host_index, uint16_t port,
		       struct sockaddr_in *sin, size_t *addrlen)
{
	struct lpp_sockaddr *lppaddr;

	sin->sin_family = AF_INET;
	sin->sin_port = htons(port);
	lppaddr = (struct lpp_sockaddr*)&sin->sin_addr;
	lppaddr->magic = LPP_SOCKADDR_MAGIC;
	lppaddr->fabric_index = fabric_index;
	lppaddr->host_index = host_index;
	*addrlen = sizeof(*sin);
}

int lpp_from_sockaddr(const struct sockaddr_in *sin, uint32_t *host_index,
		      uint16_t *port, bool checkaddr)
{
	struct lpp_sockaddr *lppaddr;

	if (checkaddr) {
		if (sin->sin_family != AF_INET) {
			FI_WARN(&lpp_prov, FI_LOG_AV, "invalid sin_family %d\n",
				sin->sin_family);
			return -FI_EINVAL;
		}

		lppaddr = (struct lpp_sockaddr *)&sin->sin_addr;

		if (lppaddr->magic != LPP_SOCKADDR_MAGIC) {
			FI_WARN(&lpp_prov, FI_LOG_AV,
				"invalid magic in addr (want %d, got %d)\n",
				LPP_SOCKADDR_MAGIC, lppaddr->magic);
			return -FI_EINVAL;
		}

		*host_index = lppaddr->host_index;
	} else {
		*host_index = 0;
	}
	*port = ntohs(sin->sin_port);

	return 0;
}

int lpp_fi_av_open(struct fid_domain *domain, struct fi_av_attr *attr,
		   struct fid_av **av, void *context)
{
	int			status = 0;
	struct lpp_av		*lpp_avp = NULL;
	struct util_domain	*util_domain;
	struct lpp_domain	*lpp_domainp = NULL;
	struct fi_av_attr	av_attrs = lpp_default_av_attrs;

	if (attr != NULL)
		av_attrs = *attr;

	if ((domain == NULL) || (av == NULL))
		return -FI_EINVAL;

	if (status = lpp_av_verify_attr(&av_attrs), status != 0)
		return status;

	util_domain = container_of(domain, struct util_domain, domain_fid);
	lpp_domainp = container_of(util_domain, struct lpp_domain, util_domain);
	if (lpp_domainp->util_domain.av_type != FI_AV_MAP) {
		FI_WARN(&lpp_prov, FI_LOG_AV,
			"Wrong AV type %d\n", lpp_domainp->util_domain.av_type);
		return -FI_EINVAL;
	}

	if (lpp_avp = calloc(1, sizeof(struct lpp_av)), lpp_avp == NULL)
		return -FI_ENOMEM;

	lpp_avp->av_fid.fid.fclass = FI_CLASS_AV;
	lpp_avp->av_fid.fid.context = context;
	lpp_avp->av_fid.fid.ops = (struct fi_ops *)&lpp_fi_av_ops;
	lpp_avp->av_fid.ops = (struct fi_ops_av *)&lpp_av_ops;

	lpp_avp->domain = lpp_domainp;
	lpp_avp->attr = av_attrs;
	lpp_avp->context = context;

	lpp_avp->map_name = NULL;

	ofi_atomic_initialize32(&lpp_avp->ref_count, 0);
	ofi_atomic_inc32(&lpp_avp->domain->util_domain.ref);

	*av = &lpp_avp->av_fid;
	return 0;
}

int lpp_fi_av_insert(struct fid_av *av, const void *addr,
		     size_t count, fi_addr_t *fi_addr, uint64_t flags,
		     void *context)
{
	size_t i;
	int status;
	int inserted = 0;
	union lpp_fi_addr_storage lpp_fi_addr;
	struct lpp_av *lpp_avp = NULL;
	struct sockaddr_in *sin;
	uint32_t host_index;
	uint16_t port;

	lpp_avp = container_of(av, struct lpp_av, av_fid);

	for (i = 0, sin = (struct sockaddr_in*)addr; i < count; i++, sin++) {
		fi_addr[i] = FI_ADDR_NOTAVAIL;

		status = lpp_from_sockaddr(sin, &host_index, &port, true);
		if (status != 0)
			continue;

		status = klpp_av_resolve(lpp_avp, host_index, port,
					 &lpp_fi_addr.fi_addr);
		if (status != 0) {
			FI_WARN(&lpp_prov, FI_LOG_AV,
				"KLPP returned %d when resolving name '%u:%u'\n",
				status, host_index, port);
			continue;
		}

		fi_addr[i] = lpp_fi_addr.raw;
		inserted++;
	}

	return inserted;
}

int lpp_fi_av_lookup(struct fid_av *av, fi_addr_t fi_addr, void *addr, size_t *addrlen)
{
	struct lpp_av *lpp_avp = container_of(av, struct lpp_av, av_fid);
	struct sockaddr_in *sin;
	union lpp_fi_addr_storage lpp_fi_addr;
	uint32_t host_index;
	uint16_t port;
	int ret;

	if (*addrlen < sizeof(*sin)) {
		*addrlen = sizeof(*sin);
		return -FI_ETOOSMALL;
	}

	lpp_fi_addr.raw = fi_addr;
	ret = klpp_av_lookup(lpp_avp, lpp_fi_addr.fi_addr, &host_index, &port);
	if (ret != 0) {
		FI_WARN(&lpp_prov, FI_LOG_AV, "klpp_av_lookup returned %d\n", ret);
		return ret;
	}

	sin = (struct sockaddr_in *)addr;
	lpp_make_sockaddr(0, host_index, port, sin, addrlen);

	return 0;
}

const char * lpp_fi_av_straddr(struct fid_av *av, const void *addr, char *buf, size_t *len)
{
	// Not used by MPICH, but required by libfabric
	return NULL;
}

int lpp_fi_av_close(struct fid *av)
{
	int status = -FI_EBUSY;
	struct lpp_av *lpp_avp = NULL;

	lpp_avp = container_of(av, struct lpp_av, av_fid);

	if (ofi_atomic_get32(&lpp_avp->ref_count) == 0) {
		ofi_atomic_dec32(&lpp_avp->domain->util_domain.ref);
		free(lpp_avp);
		status = 0;
	}

	return status;
}
