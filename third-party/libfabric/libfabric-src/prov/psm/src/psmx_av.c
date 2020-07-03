/*
 * Copyright (c) 2013-2017 Intel Corporation. All rights reserved.
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

#include "psmx.h"

static void psmx_set_epaddr_context(struct psmx_fid_domain *domain,
				    psm_epid_t epid, psm_epaddr_t epaddr)
{
	struct psmx_epaddr_context *context;

	context = (void *)psm_epaddr_getctxt(epaddr);
	if (context) {
		if (context->domain != domain || context->epid != epid) {
			FI_WARN(&psmx_prov, FI_LOG_AV,
				"domain or epid doesn't match\n");
			context = NULL;
		}
	}

	if (context)
		return;

	context = malloc(sizeof *context);
	if (!context) {
		FI_WARN(&psmx_prov, FI_LOG_AV,
			"cannot allocate context\n");
		return;
	}

	context->domain = domain;
	context->epid = epid;
	psm_epaddr_setctxt(epaddr, context);
}

int psmx_epid_to_epaddr(struct psmx_fid_domain *domain,
			psm_epid_t epid, psm_epaddr_t *epaddr)
{
	int err;
	psm_error_t errors;
	psm_epconn_t epconn;
	struct psmx_epaddr_context *context;

	err = psm_ep_epid_lookup(epid, &epconn);
	if (err == PSM_OK) {
		context = psm_epaddr_getctxt(epconn.addr);
		if (context && context->epid  == epid) {
			*epaddr = epconn.addr;
			return 0;
		}
	}

	err = psm_ep_connect(domain->psm_ep, 1, &epid, NULL, &errors, epaddr, 30*1e9);
	if (err != PSM_OK)
		return psmx_errno(err);

	psmx_set_epaddr_context(domain,epid,*epaddr);

	return 0;
}

static int psmx_av_check_table_size(struct psmx_fid_av *av, size_t count)
{
	size_t new_count;
	psm_epid_t *new_psm_epids;
	psm_epaddr_t *new_psm_epaddrs;

	new_count = av->count;
	while (new_count < av->last + count)
		new_count = new_count * 2 + 1;

	if ((new_count <= av->count) && av->psm_epids)
		return 0;

	new_psm_epids = realloc(av->psm_epids, new_count * sizeof(*new_psm_epids));
	if (!new_psm_epids)
		return -FI_ENOMEM;

	av->psm_epids = new_psm_epids;

	new_psm_epaddrs = realloc(av->psm_epaddrs, new_count * sizeof(*new_psm_epaddrs));
	if (!new_psm_epaddrs)
		return -FI_ENOMEM;

	av->psm_epaddrs = new_psm_epaddrs;
	av->count = new_count;
	return 0;
}

static void psmx_av_post_completion(struct psmx_fid_av *av, void *context,
				    uint64_t data, int prov_errno)
{
	if (prov_errno) {
		struct fi_eq_err_entry entry;
		entry.fid = &av->av.fid;
		entry.context = context;
		entry.data = data;
		entry.err = -psmx_errno(prov_errno);
		entry.prov_errno = prov_errno;
		entry.err_data = NULL;
		entry.err_data_size = 0;
		fi_eq_write(av->eq, FI_AV_COMPLETE, &entry, sizeof(entry),
			    UTIL_FLAG_ERROR);
	} else {
		struct fi_eq_entry entry;
		entry.fid = &av->av.fid;
		entry.context = context;
		entry.data = data;
		fi_eq_write(av->eq, FI_AV_COMPLETE, &entry, sizeof(entry), 0);
	}
}

static int psmx_av_insert(struct fid_av *av, const void *addr, size_t count,
			  fi_addr_t *fi_addr, uint64_t flags, void *context)
{
	struct psmx_fid_av *av_priv;
	psm_error_t *errors;
	int error_count = 0;
	int *mask;
	int i, j, ret;
	fi_addr_t *result = NULL;
	struct psmx_epaddr_context *epaddr_context;

	if (count && !addr) {
		FI_INFO(&psmx_prov, FI_LOG_AV,
			"the input address array is NULL.\n");
		return -FI_EINVAL;
	}

	av_priv = container_of(av, struct psmx_fid_av, av);

	if ((av_priv->flags & FI_EVENT) && !av_priv->eq)
		return -FI_ENOEQ;

	errors = (psm_error_t *) calloc(count, sizeof *errors);
	if (!errors)
		return -FI_ENOMEM;

	mask = (int *) calloc(count, sizeof *mask);
	if (!mask) {
		free(errors);
		return -FI_ENOMEM;
	}

	if (av_priv->type == FI_AV_TABLE) {
		if (psmx_av_check_table_size(av_priv, count)) {
			free(mask);
			free(errors);
			return -FI_ENOMEM;
		}

		for (i=0; i<count; i++)
			av_priv->psm_epids[av_priv->last + i] = ((psm_epid_t *)addr)[i];

		result = fi_addr;
		addr = (const void *)(av_priv->psm_epids + av_priv->last);
		fi_addr = (fi_addr_t *)(av_priv->psm_epaddrs + av_priv->last);
	}

	/* prevent connecting to the same ep twice, which is fatal in PSM */
	for (i=0; i<count; i++) {
		psm_epconn_t epconn;
		if (psm_ep_epid_lookup(((psm_epid_t *) addr)[i], &epconn) == PSM_OK) {
			epaddr_context = psm_epaddr_getctxt(epconn.addr);
			if (epaddr_context && epaddr_context->epid  == ((psm_epid_t *) addr)[i])
				((psm_epaddr_t *) fi_addr)[i] = epconn.addr;
			else
				mask[i] = 1;
		} else {
			mask[i] = 1;
		}
	}

	psm_ep_connect(av_priv->domain->psm_ep, count, 
			(psm_epid_t *) addr, mask, errors,
			(psm_epaddr_t *) fi_addr, 30*1e9);

	for (i=0; i<count; i++){
		if (!mask[i]) {
			errors[i] = PSM_OK;
			continue;
		}

		if (errors[i] == PSM_OK || errors[i] == PSM_EPID_ALREADY_CONNECTED) {
			psmx_set_epaddr_context(av_priv->domain,
						((psm_epid_t *) addr)[i],
						((psm_epaddr_t *) fi_addr)[i]);
			errors[i] = PSM_OK;
		} else {
			psm_epconn_t epconn;

			/* If duplicated addresses are passed to psm_ep_connect(), all but one will fail
			 * with error "Endpoint could not be reached". They should be treated as already
			 * connected.
			 */
			if (psm_ep_epid_lookup(((psm_epid_t *) addr)[i], &epconn) == PSM_OK) {
				epaddr_context = psm_epaddr_getctxt(epconn.addr);
				if (epaddr_context && epaddr_context->epid  == ((psm_epid_t *) addr)[i]) {
					((psm_epaddr_t *) fi_addr)[i] = epconn.addr;
					errors[i] = PSM_OK;
					continue;
				}
			}

			FI_INFO(&psmx_prov, FI_LOG_AV,
				"%d: psm_ep_connect returned %s. remote epid=%lx.\n",
				i, psm_error_get_string(errors[i]),
				((psm_epid_t *)addr)[i]);
			if (((psm_epid_t *)addr)[i] == 0)
				FI_INFO(&psmx_prov, FI_LOG_AV,
					"does the application depend on the provider"
					"to resolve IP address into endpoint id? if so"
					"check if the name server has started correctly"
					"at the other side.\n");
			fi_addr[i] = FI_ADDR_NOTAVAIL;
			error_count++;

			if (av_priv->flags & FI_EVENT)
				psmx_av_post_completion(av_priv, context, i, errors[i]);
		}
	}

	if (av_priv->type == FI_AV_TABLE) {
		/* NOTE: unresolved addresses are left in the AV table */
		if (result) {
			for (i=0; i<count; i++) {
				j = av_priv->last + i;
				if ((fi_addr_t)av_priv->psm_epaddrs[j] == FI_ADDR_NOTAVAIL)
					result[i] = FI_ADDR_NOTAVAIL;
				else
					result[i] = j;
			}
		}
		av_priv->last += count;
	}

	if (av_priv->flags & FI_EVENT) {
		psmx_av_post_completion(av_priv, context, count - error_count, 0);
		ret = 0;
	} else {
		if (flags & FI_SYNC_ERR) {
			int *fi_errors = context;
			for (i=0; i<count; i++)
				fi_errors[i] = psmx_errno(errors[i]);
		}
		ret = count - error_count;
	}

	free(mask);
	free(errors);
	return ret;
}

static int psmx_av_remove(struct fid_av *av, fi_addr_t *fi_addr, size_t count,
			  uint64_t flags)
{
	int err = PSM_OK;

	return psmx_errno(err);
}

static int psmx_av_lookup(struct fid_av *av, fi_addr_t fi_addr, void *addr,
			  size_t *addrlen)
{
	struct psmx_fid_av *av_priv;
	struct psmx_epaddr_context *context;
	psm_epid_t epid;
	int idx;

	if (!addr || !addrlen)
		return -FI_EINVAL;

	av_priv = container_of(av, struct psmx_fid_av, av);

	if (av_priv->type == FI_AV_TABLE) {
		idx = (int)(int64_t)fi_addr;
		if (idx >= av_priv->last)
			return -FI_EINVAL;

		epid = av_priv->psm_epids[idx];
	} else {
		context = psm_epaddr_getctxt((void *)fi_addr);
		epid = context->epid;
	}

	if (*addrlen >= sizeof(epid))
		*(psm_epid_t *)addr = epid;
	else
		memcpy(addr, &epid, *addrlen);
	*addrlen = sizeof(epid);

	return 0;
}

static const char *psmx_av_straddr(struct fid_av *av, const void *addr,
				   char *buf, size_t *len)
{
	return ofi_straddr(buf, len, FI_ADDR_PSMX, addr);
}

static int psmx_av_close(fid_t fid)
{
	struct psmx_fid_av *av;

	av = container_of(fid, struct psmx_fid_av, av.fid);
	psmx_domain_release(av->domain);
	if (av->psm_epids)
		free(av->psm_epids);
	if (av->psm_epaddrs)
		free(av->psm_epaddrs);
	free(av);
	return 0;
}

static int psmx_av_bind(struct fid *fid, struct fid *bfid, uint64_t flags)
{
	struct psmx_fid_av *av;

	av = container_of(fid, struct psmx_fid_av, av.fid);

	if (!bfid)
		return -FI_EINVAL;

	switch (bfid->fclass) {
	case FI_CLASS_EQ:
		av->eq = (struct fid_eq *)bfid;
		break;

	default:
		return -FI_ENOSYS;
	}

	return 0;
}

static struct fi_ops psmx_fi_ops = {
	.size = sizeof(struct fi_ops),
	.close = psmx_av_close,
	.bind = psmx_av_bind,
	.control = fi_no_control,
	.ops_open = fi_no_ops_open,
};

static struct fi_ops_av psmx_av_ops = {
	.size = sizeof(struct fi_ops_av),
	.insert = psmx_av_insert,
	.insertsvc = fi_no_av_insertsvc,
	.insertsym = fi_no_av_insertsym,
	.remove = psmx_av_remove,
	.lookup = psmx_av_lookup,
	.straddr = psmx_av_straddr,
};

int psmx_av_open(struct fid_domain *domain, struct fi_av_attr *attr,
		 struct fid_av **av, void *context)
{
	struct psmx_fid_domain *domain_priv;
	struct psmx_fid_av *av_priv;
	int type = FI_AV_MAP;
	size_t count = 64;
	uint64_t flags = 0;

	domain_priv = container_of(domain, struct psmx_fid_domain,
				   util_domain.domain_fid);

	if (attr) {
		switch (attr->type) {
		case FI_AV_UNSPEC:
			break;

		case FI_AV_MAP:
		case FI_AV_TABLE:
			type = attr->type;
			break;
		default:
			FI_INFO(&psmx_prov, FI_LOG_AV,
				"attr->type=%d, supported=%d %d\n",
				attr->type, FI_AV_MAP, FI_AV_TABLE);
			return -FI_EINVAL;
		}

		count = attr->count;
		flags = attr->flags;

		if (flags & (FI_READ | FI_SYMMETRIC)) {
			FI_INFO(&psmx_prov, FI_LOG_AV,
				"attr->flags=%"PRIu64", supported=%llu\n",
				attr->flags, FI_EVENT);
			return -FI_ENOSYS;
		}

		if (attr->name) {
			FI_INFO(&psmx_prov, FI_LOG_AV,
				"attr->name=%s, named AV is not supported\n",
				attr->name);
			return -FI_ENOSYS;
		}
	}

	av_priv = (struct psmx_fid_av *) calloc(1, sizeof *av_priv);
	if (!av_priv)
		return -FI_ENOMEM;

	psmx_domain_acquire(domain_priv);

	av_priv->domain = domain_priv;
	av_priv->type = type;
	av_priv->addrlen = sizeof(psm_epaddr_t);
	av_priv->count = count;
	av_priv->flags = flags;

	av_priv->av.fid.fclass = FI_CLASS_AV;
	av_priv->av.fid.context = context;
	av_priv->av.fid.ops = &psmx_fi_ops;
	av_priv->av.ops = &psmx_av_ops;

	*av = &av_priv->av;
	if (attr)
		attr->type = type;

	return 0;
}

