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

static int ucx_domain_close(fid_t fid)
{
	struct ucx_domain *domain;
	int status;

	domain = container_of(fid, struct ucx_domain, u_domain.domain_fid.fid);

	ucp_cleanup(domain->context);
	status = ofi_domain_close( &domain->u_domain);
	if (!status)
		free(domain);

	return status;
}

static int ucx_dom_control(struct fid *fid, int command, void *arg)
{
	struct ucx_domain *domain = container_of(fid, struct ucx_domain,
						 u_domain.domain_fid.fid);
	struct ucx_mr_pkey *pkey;
	struct ucx_mr_rkey *rkey;
	struct fi_mr_map_raw *map_raw;

	switch (command) {
	case FI_MAP_RAW_MR:
		map_raw = arg;

		if (map_raw->key_size > FI_UCX_MAX_KEY_SIZE) {
			FI_WARN(&ucx_prov,FI_LOG_MR,
			        "map raw: key size (%ld) exceeds limit (%d).\n",
			        map_raw->key_size, FI_UCX_MAX_KEY_SIZE);
			return -FI_EINVAL;
		}

		pkey = malloc(sizeof(*pkey) + map_raw->key_size);
		if (!pkey)
			return -FI_ENOMEM;

		memcpy(pkey->pkey, map_raw->raw_key, map_raw->key_size);
		pkey->pkey_size = map_raw->key_size;
		pkey->signature = FI_UCX_PKEY_SIGNATURE;
		dlist_init(&pkey->rkey_list);
		*map_raw->key = (uint64_t)(uintptr_t)pkey;
		return FI_SUCCESS;

	case FI_UNMAP_KEY:
		pkey = (struct ucx_mr_pkey *)(uintptr_t) *(uint64_t *)arg;
		if (!pkey)
			return FI_SUCCESS;

		if (pkey->signature != FI_UCX_PKEY_SIGNATURE)
			return -FI_EINVAL;

		dlist_foreach_container(&pkey->rkey_list, struct ucx_mr_rkey,
					rkey, entry) {
			FI_DBG(&ucx_prov,FI_LOG_MR,
			       "UCX/RMA: removed key {%" PRIu64 ":%" PRIu64 "}\n",
			       rkey->id.owner_addr, rkey->id.key);
			HASH_DEL(domain->remote_keys, rkey);
			ucp_rkey_destroy(rkey->rkey);
			free(rkey);
		}
		pkey->signature = 0;
		free(pkey);
		return FI_SUCCESS;
	}

	return -FI_ENOSYS;
}

static struct fi_ops ucx_fi_ops = {
	.size = sizeof(struct fi_ops),
	.close = ucx_domain_close,
	.control = ucx_dom_control,
};

struct fi_ops_domain ucx_domain_ops = {
	.size = sizeof(struct fi_ops_domain),
	.av_open = ucx_av_open,
	.cq_open = ucx_cq_open,
	.endpoint = ucx_ep_open,
	.scalable_ep = fi_no_scalable_ep,
	.cntr_open = ucx_cntr_open,
	.poll_open = fi_poll_create,
	.stx_ctx = fi_no_stx_context,
	.srx_ctx = fi_no_srx_context,
	.query_atomic = fi_no_query_atomic,
};


static int ucx_mr_close(struct fid *fid)
{
	struct ucx_mr *mr;
	struct ofi_mr *omr;
	int ret;
	struct ucx_domain *domain;
	ucs_status_t status = UCS_OK;

	mr = container_of(fid, struct ucx_mr, omr.mr_fid.fid);
	omr = container_of(fid, struct ofi_mr, mr_fid.fid);

	ofi_genlock_lock(&omr->domain->lock);
	ret = ofi_mr_map_remove(&omr->domain->mr_map, omr->key);
	ofi_genlock_unlock(&omr->domain->lock);
	if (ret)
		return ret;

	domain = container_of(omr->domain, struct ucx_domain, u_domain);
	status = ucp_mem_unmap(domain->context, mr->memh);
	ofi_atomic_dec32(&omr->domain->ref);
	free(mr);

	return ucx_translate_errcode(status);
}

static int ucx_mr_control(struct fid *fid, int command, void *arg)
{
	struct ucx_mr *ucx_mr;
	struct ucx_domain *domain;
	struct fi_mr_raw_attr *raw_attr = arg;
	size_t tmp_size = 0;
	void * tmp = NULL;
	ucs_status_t status;

	if (command != FI_GET_RAW_MR)
		return -FI_ENOSYS;

	ucx_mr = container_of(fid, struct ucx_mr, omr.mr_fid.fid);
	domain = container_of(ucx_mr->omr.domain, struct ucx_domain,
			      u_domain);

	status = ucp_rkey_pack(domain->context, ucx_mr->memh, &tmp,
			       &tmp_size);
	if (status != UCS_OK)
		return ucx_translate_errcode(status);

	if (*raw_attr->key_size < tmp_size) {
		ucp_rkey_buffer_release(tmp);
		FI_WARN(&ucx_prov, FI_LOG_MR,
			"Raw key buffer is too small: input %lu, needed %lu\n",
			*raw_attr->key_size, tmp_size);
		*raw_attr->key_size = tmp_size;
		return -FI_ETOOSMALL;
	}

	memcpy(raw_attr->raw_key, tmp, tmp_size);
	*raw_attr->key_size = tmp_size;
	ucp_rkey_buffer_release(tmp);

	/* TODO: should (*raw_attr->base_addr) be set? unused for now. */
	return FI_SUCCESS;
}

static struct fi_ops ucx_mr_fi_ops = {
	.size = sizeof(struct fi_ops),
	.close = ucx_mr_close,
	.control = ucx_mr_control,
	.ops_open = fi_no_ops_open,
};

static inline void ucx_update_memtype_cache(enum fi_hmem_iface iface,
					    void *addr, size_t len)
{
	void *base;
	size_t size;
	ucs_memory_info_t mem_info;

	if (iface == FI_HMEM_SYSTEM)
		return;

	if (ofi_hmem_get_base_addr(iface, addr, len, &base, &size))
		return;

	if (ucs_memtype_cache_lookup(base, size, &mem_info) != UCS_ERR_NO_ELEM)
		return;

	/*
	 * Now we know that the address range is not in the memtype cache. The
	 * reason could be:
	 * (1) No hook has been installed for the memory allocator being used;
	 * (2) The hook has not been installed properly. For example, when the
	 *     allocator is obtained via dlsym();
	 * (3) The memory is allocated before the hooks are installed.
	 *
	 * We only need to add the range to the memtype cache. The exact value
	 * of memtype will be updated next time a lookup is performed within
	 * this range.
	 */
	ucs_memtype_cache_update(base, size, UCS_MEMORY_TYPE_UNKNOWN,
				 UCS_SYS_DEVICE_ID_UNKNOWN);
}

static int ucx_mr_regattr(struct fid *fid, const struct fi_mr_attr *attr,
			  uint64_t flags, struct fid_mr **mr_fid)
{
	struct ucx_domain *m_domain;
	struct util_domain *domain;
	struct ucx_mr *ucx_mr;
	struct ofi_mr *mr;
	ucp_mem_map_params_t um_params;
	uint64_t key;
	int ret = 0;
	ucs_status_t status = UCS_OK;

	if (fid->fclass != FI_CLASS_DOMAIN || !attr || attr->iov_count <= 0)
		return -FI_EINVAL;

	if (flags & FI_MR_DMABUF)
		return -FI_EINVAL;

	domain = container_of(fid, struct util_domain, domain_fid.fid);
	m_domain = container_of(domain, struct ucx_domain, u_domain);
	ucx_mr = calloc(1, sizeof(*ucx_mr));
	if (!ucx_mr)
		return -FI_ENOMEM;
	mr = &ucx_mr->omr;

	ofi_genlock_lock(&domain->lock);

	mr->mr_fid.fid.fclass = FI_CLASS_MR;
	mr->mr_fid.fid.context = attr->context;
	mr->mr_fid.fid.ops = &ucx_mr_fi_ops;
	mr->domain = domain;
	mr->flags = flags;

	um_params.field_mask = UCP_MEM_MAP_PARAM_FIELD_ADDRESS |
			       UCP_MEM_MAP_PARAM_FIELD_LENGTH |
			       UCP_MEM_MAP_PARAM_FIELD_FLAGS;
	um_params.address = attr->mr_iov->iov_base;
	um_params.length = attr->mr_iov->iov_len;
	um_params.flags = 0;

	ucx_update_memtype_cache(attr->iface, attr->mr_iov->iov_base,
				 attr->mr_iov->iov_len);

	status = ucp_mem_map(m_domain->context, &um_params, &ucx_mr->memh);
	if (status != UCS_OK) {
		ret = ucx_translate_errcode(status);
		free(ucx_mr);
		goto out;
	}

	ret = ofi_mr_map_insert(&domain->mr_map, attr, &key, mr, flags);
	if (ret) {
		ucp_mem_unmap(m_domain->context, ucx_mr->memh);
		free(ucx_mr);
		goto out;
	}

	mr->mr_fid.key = mr->key = key;
	mr->mr_fid.mem_desc = (void *)(uintptr_t) key;

	*mr_fid = &mr->mr_fid;
	ofi_atomic_inc32(&domain->ref);

out:
	ofi_genlock_unlock(&domain->lock);
	return ret;
}

static int ucx_mr_regv(struct fid *fid, const struct iovec *iov,
		       size_t count, uint64_t access, uint64_t offset,
		       uint64_t requested_key, uint64_t flags,
		       struct fid_mr **mr_fid, void *context)
{
	struct fi_mr_attr attr;

	attr.mr_iov = iov;
	attr.iov_count = count;
	attr.access = access;
	attr.offset = offset;
	attr.requested_key = requested_key;
	attr.context = context;
	attr.iface = FI_HMEM_SYSTEM;
	return ucx_mr_regattr(fid, &attr, flags, mr_fid);
}

static int ucx_mr_reg(struct fid *fid, const void *buf, size_t len,
		      uint64_t access, uint64_t offset, uint64_t requested_key,
		      uint64_t flags, struct fid_mr **mr_fid, void *context)
{
	struct iovec iov;

	iov.iov_base = (void *) buf;
	iov.iov_len = len;
	return ucx_mr_regv(fid, &iov, 1, access, offset, requested_key, flags,
			   mr_fid, context);
}

struct fi_ops_mr ucx_mr_ops = {
	.size = sizeof(struct fi_ops_mr),
	.reg = ucx_mr_reg,
	.regv = ucx_mr_regv,
	.regattr = ucx_mr_regattr,
};

static void ucx_req_reset(void *request)
{
	struct ucx_request* mreq = (struct ucx_request*)request;

	mreq->type = UCX_REQ_UNSPEC;
	mreq->cq = NULL;
	mreq->ep = NULL;
}

int ucx_domain_open(struct fid_fabric *fabric, struct fi_info *info,
		    struct fid_domain **fid, void *context)
{
	ucs_status_t status = UCS_OK;
	int ofi_status;
	struct ucx_domain* domain;
	size_t univ_size;
	ucp_params_t params = {
		.features = UCP_FEATURE_TAG | UCP_FEATURE_RMA,
		.request_size = sizeof(struct ucx_request),
		.request_init = ucx_req_reset,
		.field_mask = UCP_PARAM_FIELD_FEATURES |
			      UCP_PARAM_FIELD_REQUEST_SIZE |
			      UCP_PARAM_FIELD_REQUEST_INIT,
	};

	if (!info->domain_attr->name ||
	    strcmp(info->domain_attr->name, FI_UCX_FABRIC_NAME))
		return -FI_EINVAL;

	ofi_status = ofi_prov_check_info(&ucx_util_prov,
					 fabric->api_version,
					 info);
	if (ofi_status)
		return ofi_status;

	domain = calloc(1, sizeof(struct ucx_domain));
	if (!domain)
		return -ENOMEM;

	ofi_status = fi_param_get_size_t(NULL, "universe_size", &univ_size);
	if (ofi_status) {
		params.estimated_num_eps = univ_size;
		params.field_mask |= UCP_PARAM_FIELD_ESTIMATED_NUM_EPS;
	}

	ofi_status = ofi_domain_init(fabric, info,
				     &(domain->u_domain), context, 0);
	if (ofi_status)
		goto domain_free;

	status = ucp_init_version(FI_UCX_VERSION_MAJOR, FI_UCX_VERSION_MINOR,
				  &params, ucx_descriptor.config,
				  &domain->context);
	if (status != UCS_OK) {
		ofi_status = ucx_translate_errcode(status);
		goto destroy_domain;
	}

	domain->u_domain.domain_fid.fid.ops = &ucx_fi_ops;
	domain->u_domain.domain_fid.ops = &ucx_domain_ops;
	domain->u_domain.domain_fid.mr = &ucx_mr_ops;

	*fid = &(domain->u_domain.domain_fid);
	return FI_SUCCESS;

destroy_domain:
	ofi_domain_close(&(domain->u_domain));

domain_free:
	free(domain);
	if (!ofi_status) {
		ofi_status = FI_ENETUNREACH;
	}
	return ofi_status;
}
