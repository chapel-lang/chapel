/*
 * Copyright (c) 2021 Intel Corporation. All rights reserved.
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

#include "ofi_prov.h"
#include "ofi_iov.h"
#include "ofi_atomic.h"
#include "ofi_hmem.h"
#include "hook_prov.h"
#include "hook_hmem.h"

static int hook_hmem_iov_compare(struct ofi_rbmap *map, void *key, void *data)
{
	struct hook_hmem_desc *desc = data;
	struct iovec *iov = key;

	if (ofi_iov_shifted_left(iov, &desc->iov))
		return -1;
	if (ofi_iov_shifted_right(iov, &desc->iov))
		return 1;

	return 0;
}

static int hook_hmem_add_region(struct hook_hmem_domain *domain,
		const struct iovec *iov, struct hook_hmem_desc **hmem_desc)
{
	struct fi_mr_attr attr = {0};
	struct iovec base_iov;
	uint64_t iface, device, flags;
	int ret = 0;

	*hmem_desc = ofi_buf_alloc(domain->mr_pool);
	if (!*hmem_desc)
		return -FI_ENOMEM;

	iface = ofi_get_hmem_iface(iov->iov_base, &device, &flags);
	if (iface == FI_HMEM_SYSTEM) {
		(*hmem_desc)->desc = NULL;
		(*hmem_desc)->iov = *iov;
		goto out;
	}

	ret = ofi_hmem_get_base_addr(iface, iov->iov_base, iov->iov_len,
				     &base_iov.iov_base, &base_iov.iov_len);
	if (ret) {
		ofi_buf_free(*hmem_desc);
		return -FI_EINVAL;
	}

	attr.mr_iov = &base_iov;
	attr.iov_count = 1;
	attr.access = FI_SEND | FI_RECV | FI_READ | FI_WRITE |
		      FI_REMOTE_READ | FI_REMOTE_WRITE;
	attr.offset = 0;
	attr.requested_key = (uint64_t) *hmem_desc;
	attr.iface = iface;
	attr.device.reserved = device;

	ret = fi_mr_regattr(domain->hook_domain.hdomain, &attr,
			    (*hmem_desc)->flags, &(*hmem_desc)->mr_fid);
	if (ret) {
		ofi_buf_free(*hmem_desc);
		return ret;
	}

	(*hmem_desc)->desc = fi_mr_desc((*hmem_desc)->mr_fid);
	(*hmem_desc)->iov = base_iov;
out:
	(*hmem_desc)->count = 0;
	dlist_insert_tail(&(*hmem_desc)->entry, &domain->mr_list);
	return FI_SUCCESS;
}

static int hook_hmem_cache_mr(struct hook_hmem_domain *domain,
			      const struct iovec *iov,
			      struct hook_hmem_desc **hmem_desc)
{
	struct ofi_rbnode *node;
	int ret;

	ret = ofi_rbmap_insert(&domain->rbmap, (void *) iov, NULL, &node);
	if (!ret) {
		ret = hook_hmem_add_region(domain, iov,
				(struct hook_hmem_desc **) (&node->data));
		if (ret) {
			ofi_rbmap_delete(&domain->rbmap, node);
			return ret;
		}
	} else if (ret != -FI_EALREADY) {
		return ret;
	}

	*hmem_desc = (struct hook_hmem_desc *) (node->data);
	(*hmem_desc)->count++;

	return FI_SUCCESS;
}

static void hook_hmem_uncache_mr(struct hook_hmem_domain *domain,
				 const struct iovec *iov)
{
	struct ofi_rbnode *node;
	struct hook_hmem_desc *hmem_desc;

	node = ofi_rbmap_find(&domain->rbmap, (void *) iov);
	if (!node)
		return;

	hmem_desc = (struct hook_hmem_desc *) (node->data);
	if (--hmem_desc->count)
		return;

	ofi_rbmap_delete(&domain->rbmap, node);
	if (hmem_desc->desc)
		fi_close(&(hmem_desc->mr_fid)->fid);

	dlist_remove(&hmem_desc->entry);
	ofi_buf_free(hmem_desc);
}

static void hook_hmem_uncache_mr_iov(struct hook_hmem_domain *domain,
				     const struct iovec *iov, size_t count)
{
	int i;

	for (i = 0; i < count; i++)
		hook_hmem_uncache_mr(domain, &iov[i]);
}

static int hook_hmem_cache_mr_iov(struct hook_hmem_domain *domain,
			const struct iovec *iov, void **desc,
			size_t count, struct hook_hmem_desc **hmem_desc)
{
	int i, ret;

	for (i = 0; i < count; i++) {
		ret = hook_hmem_cache_mr(domain, &iov[i], &hmem_desc[i]);
		if (ret) {
			hook_hmem_uncache_mr_iov(domain, iov, i);
			return ret;
		}

		desc[i] = hmem_desc[i]->desc ? hmem_desc[i]->desc : desc[i];
	}
	return FI_SUCCESS;
}

static int hook_hmem_track(struct hook_ep *ep, const struct iovec *iov,
			   void **desc, size_t count, uint64_t flags,
			   void *app_ctx, struct hook_hmem_ctx **hmem_ctx)
{
	struct hook_hmem_domain *domain;
	int ret;

	domain = container_of(ep->domain, struct hook_hmem_domain, hook_domain);
	ofi_mutex_lock(&domain->lock);

	*hmem_ctx = ofi_buf_alloc(domain->ctx_pool);
	if (!*hmem_ctx) {
		ret = -FI_ENOMEM;
		goto unlock;
	}

	(*hmem_ctx)->app_ctx = app_ctx;
	(*hmem_ctx)->domain = domain;
	(*hmem_ctx)->flags = flags;

	ret = hook_hmem_cache_mr_iov(domain, iov, desc, count,
				     (*hmem_ctx)->hmem_desc);
	if (ret)
		goto free;

	(*hmem_ctx)->desc_count = count;
	(*hmem_ctx)->comp_count = 0;
	(*hmem_ctx)->comp_desc = NULL;
	(*hmem_ctx)->res_count = 0;
	(*hmem_ctx)->res_desc = NULL;
	ofi_mutex_unlock(&domain->lock);
	return FI_SUCCESS;

free:
	ofi_buf_free(*hmem_ctx);
unlock:
	ofi_mutex_unlock(&domain->lock);
	return ret;
}

static void hook_hmem_untrack(struct hook_hmem_ctx *hmem_ctx)
{
	int i;

	ofi_mutex_lock(&hmem_ctx->domain->lock);

	for (i = 0; i < hmem_ctx->desc_count; i++)
		hook_hmem_uncache_mr(hmem_ctx->domain,
				     &hmem_ctx->hmem_desc[i]->iov);

	for (i = 0; i < hmem_ctx->comp_count; i++)
		hook_hmem_uncache_mr(hmem_ctx->domain,
				     &hmem_ctx->comp_desc[i]->iov);

	for (i = 0; i < hmem_ctx->res_count; i++)
		hook_hmem_uncache_mr(hmem_ctx->domain,
				     &hmem_ctx->res_desc[i]->iov);

	ofi_buf_free(hmem_ctx);
	ofi_mutex_unlock(&hmem_ctx->domain->lock);
}

static int hook_hmem_track_atomic(struct hook_ep *ep, const struct fi_ioc *ioc,
		void **desc, size_t count, const struct fi_ioc *comp_ioc,
		void **comp_desc, size_t comp_count,
		const struct fi_ioc *res_ioc, void **res_desc, size_t res_count,
		enum fi_datatype datatype, uint64_t flags, void *app_ctx,
		struct hook_hmem_ctx **hmem_ctx)
{
	struct hook_hmem_domain *domain;
	struct iovec iov[HOOK_HMEM_IOV_LIMIT];
	struct iovec res_iov[HOOK_HMEM_IOV_LIMIT];
	struct iovec comp_iov[HOOK_HMEM_IOV_LIMIT];
	size_t dt_size = ofi_datatype_size(datatype);
	int ret;

	domain = container_of(ep->domain, struct hook_hmem_domain, hook_domain);
	ofi_mutex_lock(&domain->lock);

	*hmem_ctx = ofi_buf_alloc(domain->ctx_pool);
	if (!*hmem_ctx) {
		ret = -FI_ENOMEM;
		goto err1;
	}

	(*hmem_ctx)->app_ctx = app_ctx;
	(*hmem_ctx)->domain = domain;
	(*hmem_ctx)->flags = flags;

	if (count) {
		ofi_ioc_to_iov(ioc, iov, count, dt_size);
		ret = hook_hmem_cache_mr_iov(domain, iov, desc, count,
					     (*hmem_ctx)->hmem_desc);
		if (ret)
			goto err2;
	}

	if (comp_count) {
		(*hmem_ctx)->comp_desc = calloc(comp_count,
					sizeof(*(*hmem_ctx)->comp_desc));
		if ((*hmem_ctx)->comp_desc) {
			ret = -FI_ENOMEM;
			goto err3;
		}

		ofi_ioc_to_iov(comp_ioc, comp_iov, comp_count, dt_size);
		ret = hook_hmem_cache_mr_iov(domain, comp_iov, comp_desc,
					comp_count, (*hmem_ctx)->comp_desc);
		if (ret) {
			free((*hmem_ctx)->comp_desc);
			goto err3;
		}
		(*hmem_ctx)->comp_count = comp_count;
	} else {
		(*hmem_ctx)->comp_count = 0;
		(*hmem_ctx)->comp_desc = NULL;
	}

	if (res_count) {
		(*hmem_ctx)->res_desc = calloc(res_count,
					     sizeof(*(*hmem_ctx)->res_desc));
		if (!(*hmem_ctx)->res_desc) {
			ret = -FI_ENOMEM;
			goto err4;
		}

		ofi_ioc_to_iov(res_ioc, res_iov, res_count, dt_size);
		ret = hook_hmem_cache_mr_iov(domain, res_iov, res_desc,
					res_count, (*hmem_ctx)->res_desc);
		if (ret) {
			free((*hmem_ctx)->comp_desc);
			goto err4;
		}
		(*hmem_ctx)->res_count = res_count;
	} else {
		(*hmem_ctx)->res_count = 0;
		(*hmem_ctx)->res_desc = NULL;
	}

	ofi_mutex_unlock(&domain->lock);
	return FI_SUCCESS;

err4:
	if (comp_count) {
		hook_hmem_uncache_mr_iov(domain, comp_iov, comp_count);
		free((*hmem_ctx)->comp_desc);
	}
err3:
	if (count)
		hook_hmem_uncache_mr_iov(domain, iov, count);
err2:
	ofi_buf_free(*hmem_ctx);
err1:
	ofi_mutex_unlock(&domain->lock);
	return ret;
}

/*
 * atomic ops
 */

static ssize_t hook_hmem_atomic_write(struct fid_ep *ep,
		const void *buf, size_t count, void *desc,
		fi_addr_t dest_addr, uint64_t addr, uint64_t key,
		enum fi_datatype datatype, enum fi_op op, void *context)
{
	struct hook_hmem_ep *hmem_ep;
	struct hook_hmem_ctx *hmem_ctx;
	void *hmem_desc = desc;
	struct fi_ioc ioc;
	int ret;

	hmem_ep = container_of(ep, struct hook_hmem_ep, hook_ep.ep);

	ioc.addr = (void *) buf;
	ioc.count = count;

	ret = hook_hmem_track_atomic(&hmem_ep->hook_ep, &ioc, &hmem_desc, 1,
				     NULL, NULL, 0, NULL, NULL, 0, datatype,
				     hmem_ep->tx_op_flags, context, &hmem_ctx);
	if (ret)
		return ret;

	ret = fi_atomic(hmem_ep->hook_ep.hep, buf, count, hmem_desc, dest_addr,
			addr, key, datatype, op, hmem_ctx);
	if (ret)
		hook_hmem_untrack(hmem_ctx);

	return ret;
}

static ssize_t hook_hmem_atomic_writev(struct fid_ep *ep,
		const struct fi_ioc *ioc, void **desc, size_t count,
		fi_addr_t dest_addr, uint64_t addr, uint64_t key,
		enum fi_datatype datatype, enum fi_op op, void *context)
{
	struct hook_hmem_ep *hmem_ep;
	struct hook_hmem_ctx *hmem_ctx;
	void *hmem_desc[HOOK_HMEM_IOV_LIMIT];
	int ret;

	hmem_ep = container_of(ep, struct hook_hmem_ep, hook_ep.ep);

	if (desc)
		memcpy(hmem_desc, desc, sizeof(*desc) * count);

	ret = hook_hmem_track_atomic(&hmem_ep->hook_ep, ioc, hmem_desc, count,
				     NULL, NULL, 0, NULL, NULL, 0, datatype,
				     hmem_ep->tx_op_flags, context, &hmem_ctx);
	if (ret)
		return ret;

	ret = fi_atomicv(hmem_ep->hook_ep.hep, ioc, hmem_desc, count, dest_addr,
			 addr, key, datatype, op, hmem_ctx);
	if (ret)
		hook_hmem_untrack(hmem_ctx);

	return ret;
}

static ssize_t hook_hmem_atomic_writemsg(struct fid_ep *ep,
		const struct fi_msg_atomic *msg, uint64_t flags)
{
	struct hook_hmem_ep *hmem_ep;
	struct hook_hmem_ctx *hmem_ctx;
	void *hmem_desc[HOOK_HMEM_IOV_LIMIT];
	struct fi_msg_atomic my_msg = *msg;
	int ret;

	hmem_ep = container_of(ep, struct hook_hmem_ep, hook_ep.ep);

	if (msg->desc)
		memcpy(hmem_desc, msg->desc,
		       sizeof(*msg->desc) * msg->iov_count);

	ret = hook_hmem_track_atomic(&hmem_ep->hook_ep, msg->msg_iov, hmem_desc,
			msg->iov_count, NULL, NULL, 0, NULL, NULL,
			0, msg->datatype, flags | hmem_ep->tx_msg_flags,
			msg->context, &hmem_ctx);
	if (ret)
		return ret;

	my_msg.desc = hmem_desc;
	my_msg.context = hmem_ctx;

	ret = fi_atomicmsg(hmem_ep->hook_ep.hep, &my_msg, flags);
	if (ret)
		hook_hmem_untrack(hmem_ctx);

	return ret;
}

static ssize_t hook_hmem_atomic_inject(struct fid_ep *ep,
		const void *buf, size_t count,
		fi_addr_t dest_addr, uint64_t addr, uint64_t key,
		enum fi_datatype datatype, enum fi_op op)
{
	struct hook_ep *hook_ep = container_of(ep, struct hook_ep, ep);

	return fi_inject_atomic(hook_ep->hep, buf, count, dest_addr,
				addr, key, datatype, op);
}

static ssize_t hook_hmem_atomic_readwrite(struct fid_ep *ep,
		const void *buf, size_t count, void *desc,
		void *result, void *result_desc,
		fi_addr_t dest_addr, uint64_t addr, uint64_t key,
		enum fi_datatype datatype, enum fi_op op, void *context)
{
	struct hook_hmem_ep *hmem_ep;
	struct hook_hmem_ctx *hmem_ctx;
	void *hmem_desc = desc;
	void *hmem_res_desc = result_desc;
	struct fi_ioc ioc, res_ioc;
	int ret;

	hmem_ep = container_of(ep, struct hook_hmem_ep, hook_ep.ep);

	ioc.addr = (void *) buf;
	ioc.count = count;

	res_ioc.addr = (void *) result;
	res_ioc.count = count;

	ret = hook_hmem_track_atomic(&hmem_ep->hook_ep, &ioc, &hmem_desc, 1,
			NULL, NULL, 0, &res_ioc, &hmem_res_desc, 1, datatype,
			hmem_ep->tx_op_flags, context, &hmem_ctx);
	if (ret)
		return ret;

	ret = fi_fetch_atomic(hmem_ep->hook_ep.hep, buf, count, &hmem_desc,
			      result, &hmem_res_desc, dest_addr,
			      addr, key, datatype, op, hmem_ctx);
	if (ret)
		hook_hmem_untrack(hmem_ctx);

	return ret;
}

static ssize_t hook_hmem_atomic_readwritev(struct fid_ep *ep,
		const struct fi_ioc *ioc, void **desc, size_t count,
		struct fi_ioc *resultv, void **result_desc,
		size_t result_count, fi_addr_t dest_addr,
		uint64_t addr, uint64_t key, enum fi_datatype datatype,
		enum fi_op op, void *context)
{
	struct hook_hmem_ep *hmem_ep;
	struct hook_hmem_ctx *hmem_ctx;
	void *hmem_desc[HOOK_HMEM_IOV_LIMIT];
	void *hmem_res_desc[HOOK_HMEM_IOV_LIMIT];
	int ret;

	hmem_ep = container_of(ep, struct hook_hmem_ep, hook_ep.ep);

	if (desc)
		memcpy(hmem_desc, desc, sizeof(*desc) * count);
	if (result_desc)
		memcpy(hmem_res_desc, result_desc,
		       sizeof(*result_desc) * result_count);

	ret = hook_hmem_track_atomic(&hmem_ep->hook_ep, ioc, hmem_desc, count,
			NULL, NULL, 0, resultv, hmem_res_desc, result_count,
			datatype, hmem_ep->tx_op_flags, context, &hmem_ctx);
	if (ret)
		return ret;

	ret = fi_fetch_atomicv(hmem_ep->hook_ep.hep, ioc, hmem_desc, count,
			       resultv, hmem_res_desc, result_count,
			       dest_addr, addr, key, datatype, op, hmem_ctx);
	if (ret)
		hook_hmem_untrack(hmem_ctx);

	return ret;
}

static ssize_t hook_hmem_atomic_readwritemsg(struct fid_ep *ep,
		const struct fi_msg_atomic *msg,
		struct fi_ioc *resultv, void **result_desc,
		size_t result_count, uint64_t flags)
{
	struct hook_hmem_ep *hmem_ep;
	struct hook_hmem_ctx *hmem_ctx;
	void *hmem_desc[HOOK_HMEM_IOV_LIMIT];
	void *hmem_res_desc[HOOK_HMEM_IOV_LIMIT];
	struct fi_msg_atomic my_msg = *msg;
	int ret;

	hmem_ep = container_of(ep, struct hook_hmem_ep, hook_ep.ep);

	if (msg->desc)
		memcpy(hmem_desc, msg->desc,
		       sizeof(*msg->desc) * msg->iov_count);
	if (result_desc)
		memcpy(hmem_res_desc, result_desc,
		       sizeof(*result_desc) * result_count);

	ret = hook_hmem_track_atomic(&hmem_ep->hook_ep, msg->msg_iov, hmem_desc,
			msg->iov_count, NULL, NULL, 0, resultv, hmem_res_desc,
			result_count, msg->datatype,
			flags | hmem_ep->tx_msg_flags, msg->context, &hmem_ctx);
	if (ret)
		return ret;

	my_msg.desc = hmem_desc;
	my_msg.context = hmem_ctx;

	ret = fi_fetch_atomicmsg(hmem_ep->hook_ep.hep, &my_msg, resultv,
				 hmem_res_desc, result_count, flags);
	if (ret)
		hook_hmem_untrack(hmem_ctx);

	return ret;
}

static ssize_t hook_hmem_atomic_compwrite(struct fid_ep *ep,
		const void *buf, size_t count, void *desc,
		const void *compare, void *compare_desc,
		void *result, void *result_desc,
		fi_addr_t dest_addr, uint64_t addr, uint64_t key,
		enum fi_datatype datatype, enum fi_op op, void *context)
{
	struct hook_hmem_ep *hmem_ep;
	struct hook_hmem_ctx *hmem_ctx;
	void *hmem_desc = desc;
	void *hmem_comp_desc = compare_desc;
	void *hmem_res_desc = result_desc;
	struct fi_ioc ioc, comp_ioc, res_ioc;
	int ret;

	hmem_ep = container_of(ep, struct hook_hmem_ep, hook_ep.ep);

	ioc.addr = (void *) buf;
	ioc.count = count;

	comp_ioc.addr = (void *) compare;
	comp_ioc.count = count;

	res_ioc.addr = (void *) result;
	res_ioc.count = count;

	ret = hook_hmem_track_atomic(&hmem_ep->hook_ep, &ioc, &hmem_desc, 1,
			&comp_ioc, &hmem_comp_desc, 1, &res_ioc, &hmem_res_desc,
			1, datatype, hmem_ep->tx_op_flags, context, &hmem_ctx);
	if (ret)
		return ret;

	ret = fi_compare_atomic(hmem_ep->hook_ep.hep, buf, count, hmem_desc,
				compare, hmem_comp_desc, result, hmem_res_desc,
				dest_addr, addr, key, datatype, op, hmem_ctx);
	if (ret)
		hook_hmem_untrack(hmem_ctx);

	return ret;
}

static ssize_t hook_hmem_atomic_compwritev(struct fid_ep *ep,
		const struct fi_ioc *ioc, void **desc, size_t count,
		const struct fi_ioc *comparev, void **compare_desc,
		size_t compare_count, struct fi_ioc *resultv,
		void **result_desc, size_t result_count,
		fi_addr_t dest_addr, uint64_t addr, uint64_t key,
		enum fi_datatype datatype, enum fi_op op, void *context)
{
	struct hook_hmem_ep *hmem_ep;
	struct hook_hmem_ctx *hmem_ctx;
	void *hmem_desc[HOOK_HMEM_IOV_LIMIT];
	void *hmem_comp_desc[HOOK_HMEM_IOV_LIMIT];
	void *hmem_res_desc[HOOK_HMEM_IOV_LIMIT];
	int ret;

	hmem_ep = container_of(ep, struct hook_hmem_ep, hook_ep.ep);

	if (desc)
		memcpy(hmem_desc, desc, sizeof(*desc) * count);
	if (compare_desc)
		memcpy(hmem_comp_desc, compare_desc,
		       sizeof(*compare_desc) * compare_count);
	if (result_desc)
		memcpy(hmem_res_desc, result_desc,
		       sizeof(*result_desc) * result_count);

	ret = hook_hmem_track_atomic(&hmem_ep->hook_ep, ioc, hmem_desc, 1,
			comparev, hmem_comp_desc, compare_count, resultv,
			hmem_res_desc, 1, datatype, hmem_ep->tx_op_flags,
			context, &hmem_ctx);
	if (ret)
		return ret;

	ret = fi_compare_atomicv(hmem_ep->hook_ep.hep, ioc, desc, count,
				 comparev, hmem_comp_desc, compare_count,
				 resultv, hmem_res_desc, result_count,
				 dest_addr, addr, key, datatype, op, hmem_ctx);
	if (ret)
		hook_hmem_untrack(hmem_ctx);

	return ret;
}

static ssize_t hook_hmem_atomic_compwritemsg(struct fid_ep *ep,
		const struct fi_msg_atomic *msg,
		const struct fi_ioc *comparev, void **compare_desc,
		size_t compare_count, struct fi_ioc *resultv,
		void **result_desc, size_t result_count,
		uint64_t flags)
{
	struct hook_hmem_ep *hmem_ep;
	struct hook_hmem_ctx *hmem_ctx;
	void *hmem_desc[HOOK_HMEM_IOV_LIMIT];
	void *hmem_comp_desc[HOOK_HMEM_IOV_LIMIT];
	void *hmem_res_desc[HOOK_HMEM_IOV_LIMIT];
	struct fi_msg_atomic my_msg = *msg;
	int ret;

	hmem_ep = container_of(ep, struct hook_hmem_ep, hook_ep.ep);

	if (msg->desc)
		memcpy(hmem_desc, msg->desc,
		       sizeof(*msg->desc) * msg->iov_count);
	if (compare_desc)
		memcpy(hmem_comp_desc, compare_desc,
		       sizeof(*compare_desc) * compare_count);
	if (result_desc)
		memcpy(hmem_res_desc, result_desc,
		       sizeof(*result_desc) * result_count);

	ret = hook_hmem_track_atomic(&hmem_ep->hook_ep, msg->msg_iov, hmem_desc,
			msg->iov_count, comparev, hmem_comp_desc, compare_count,
			resultv, hmem_res_desc, result_count, msg->datatype,
			flags | hmem_ep->tx_msg_flags, msg->context, &hmem_ctx);
	if (ret)
		return ret;

	my_msg.desc = hmem_desc;
	my_msg.context = hmem_ctx;

	ret = fi_compare_atomicmsg(hmem_ep->hook_ep.hep, &my_msg, comparev,
				   hmem_comp_desc, compare_count, resultv,
				   hmem_res_desc, result_count, flags);
	if (ret)
		hook_hmem_untrack(hmem_ctx);

	return ret;
}

static int hook_hmem_atomic_writevalid(struct fid_ep *ep,
		enum fi_datatype datatype, enum fi_op op, size_t *count)
{
	struct hook_ep *hook_ep = container_of(ep, struct hook_ep, ep);

	return fi_atomicvalid(hook_ep->hep, datatype, op, count);
}

static int hook_hmem_atomic_readwritevalid(struct fid_ep *ep,
		enum fi_datatype datatype, enum fi_op op, size_t *count)
{
	struct hook_ep *hook_ep = container_of(ep, struct hook_ep, ep);

	return fi_fetch_atomicvalid(hook_ep->hep, datatype, op, count);
}

static int hook_hmem_atomic_compwritevalid(struct fid_ep *ep,
		enum fi_datatype datatype, enum fi_op op, size_t *count)
{
	struct hook_ep *hook_ep = container_of(ep, struct hook_ep, ep);

	return fi_compare_atomicvalid(hook_ep->hep, datatype, op, count);
}

struct fi_ops_atomic hook_hmem_atomic_ops = {
	.size = sizeof(struct fi_ops_atomic),
	.write = hook_hmem_atomic_write,
	.writev = hook_hmem_atomic_writev,
	.writemsg = hook_hmem_atomic_writemsg,
	.inject = hook_hmem_atomic_inject,
	.readwrite = hook_hmem_atomic_readwrite,
	.readwritev = hook_hmem_atomic_readwritev,
	.readwritemsg = hook_hmem_atomic_readwritemsg,
	.compwrite = hook_hmem_atomic_compwrite,
	.compwritev = hook_hmem_atomic_compwritev,
	.compwritemsg = hook_hmem_atomic_compwritemsg,
	.writevalid = hook_hmem_atomic_writevalid,
	.readwritevalid = hook_hmem_atomic_readwritevalid,
	.compwritevalid = hook_hmem_atomic_compwritevalid,
};


/*
 * non-tagged message ops
 */

static ssize_t hook_hmem_msg_recv(struct fid_ep *ep, void *buf, size_t len,
		void *desc, fi_addr_t src_addr, void *context)
{
	struct hook_hmem_ep *hmem_ep;
	struct hook_hmem_ctx *hmem_ctx;
	void *hmem_desc = desc;
	struct iovec iov;
	int ret;

	hmem_ep = container_of(ep, struct hook_hmem_ep, hook_ep.ep);

	iov.iov_base = buf;
	iov.iov_len = len;

	ret = hook_hmem_track(&hmem_ep->hook_ep, &iov, &hmem_desc, 1,
			      hmem_ep->rx_op_flags, context, &hmem_ctx);
	if (ret)
		return ret;

	ret = fi_recv(hmem_ep->hook_ep.hep, buf, len, hmem_desc, src_addr,
		      hmem_ctx);
	if (ret)
		hook_hmem_untrack(hmem_ctx);

	return ret;
}

static ssize_t hook_hmem_msg_recvv(struct fid_ep *ep, const struct iovec *iov,
		void **desc, size_t count, fi_addr_t src_addr, void *context)
{
	struct hook_hmem_ep *hmem_ep;
	struct hook_hmem_ctx *hmem_ctx;
	void *hmem_desc[HOOK_HMEM_IOV_LIMIT];
	int ret;

	hmem_ep = container_of(ep, struct hook_hmem_ep, hook_ep.ep);

	if (desc)
		memcpy(hmem_desc, desc, sizeof(*desc) * count);

	ret = hook_hmem_track(&hmem_ep->hook_ep, iov, hmem_desc, count,
			      hmem_ep->rx_op_flags, context, &hmem_ctx);
	if (ret)
		return ret;

	ret = fi_recvv(hmem_ep->hook_ep.hep, iov, hmem_desc, count, src_addr,
		       hmem_ctx);
	if (ret)
		hook_hmem_untrack(hmem_ctx);

	return ret;
}

static ssize_t hook_hmem_msg_recvmsg(struct fid_ep *ep,
		const struct fi_msg *msg, uint64_t flags)
{
	struct hook_hmem_ep *hmem_ep;
	struct hook_hmem_ctx *hmem_ctx;
	void *hmem_desc[HOOK_HMEM_IOV_LIMIT];
	struct fi_msg my_msg = *msg;
	int ret;

	hmem_ep = container_of(ep, struct hook_hmem_ep, hook_ep.ep);

	ret = hook_hmem_track(&hmem_ep->hook_ep, msg->msg_iov, hmem_desc,
			      msg->iov_count, flags | hmem_ep->rx_msg_flags,
			      msg->context, &hmem_ctx);
	if (ret)
		return ret;

	my_msg.desc = hmem_desc;
	my_msg.context = hmem_ctx;

	ret = fi_recvmsg(hmem_ep->hook_ep.hep, &my_msg, flags);
	if (ret)
		hook_hmem_untrack(hmem_ctx);

	return ret;
}

static ssize_t hook_hmem_msg_send(struct fid_ep *ep, const void *buf,
		size_t len, void *desc, fi_addr_t dest_addr, void *context)
{
	struct hook_hmem_ep *hmem_ep;
	struct hook_hmem_ctx *hmem_ctx;
	void *hmem_desc = desc;
	struct iovec iov;
	int ret;

	hmem_ep = container_of(ep, struct hook_hmem_ep, hook_ep.ep);

	iov.iov_base = (void *) buf;
	iov.iov_len = len;

	ret = hook_hmem_track(&hmem_ep->hook_ep, &iov, &hmem_desc, 1,
			      hmem_ep->tx_op_flags, context, &hmem_ctx);
	if (ret)
		return ret;

	ret = fi_send(hmem_ep->hook_ep.hep, buf, len, hmem_desc, dest_addr,
		      hmem_ctx);
	if (ret)
		hook_hmem_untrack(hmem_ctx);

	return ret;
}

static ssize_t hook_hmem_msg_sendv(struct fid_ep *ep, const struct iovec *iov,
		void **desc, size_t count, fi_addr_t dest_addr, void *context)
{
	struct hook_hmem_ep *hmem_ep;
	struct hook_hmem_ctx *hmem_ctx;
	void *hmem_desc[HOOK_HMEM_IOV_LIMIT];
	int ret;

	hmem_ep = container_of(ep, struct hook_hmem_ep, hook_ep.ep);

	if (desc)
		memcpy(hmem_desc, desc, sizeof(*desc) * count);

	ret = hook_hmem_track(&hmem_ep->hook_ep, iov, hmem_desc, count,
			      hmem_ep->tx_op_flags, context, &hmem_ctx);
	if (ret)
		return ret;

	ret = fi_sendv(hmem_ep->hook_ep.hep, iov, hmem_desc, count, dest_addr,
		       hmem_ctx);
	if (ret)
		hook_hmem_untrack(hmem_ctx);

	return ret;
}

static ssize_t hook_hmem_msg_sendmsg(struct fid_ep *ep,
		const struct fi_msg *msg, uint64_t flags)
{
	struct hook_hmem_ep *hmem_ep;
	struct hook_hmem_ctx *hmem_ctx;
	void *hmem_desc[HOOK_HMEM_IOV_LIMIT];
	struct fi_msg my_msg = *msg;
	int ret;

	hmem_ep = container_of(ep, struct hook_hmem_ep, hook_ep.ep);

	if (msg->desc)
		memcpy(hmem_desc, msg->desc,
		       sizeof(*msg->desc) * msg->iov_count);

	ret = hook_hmem_track(&hmem_ep->hook_ep, msg->msg_iov, hmem_desc,
			      msg->iov_count, flags | hmem_ep->tx_msg_flags,
			      msg->context, &hmem_ctx);
	if (ret)
		return ret;

	my_msg.desc = hmem_desc;
	my_msg.context = hmem_ctx;

	ret = fi_sendmsg(hmem_ep->hook_ep.hep, &my_msg, flags);
	if (ret)
		hook_hmem_untrack(hmem_ctx);

	return ret;
}

static ssize_t hook_hmem_msg_inject(struct fid_ep *ep, const void *buf,
		size_t len, fi_addr_t dest_addr)
{
	struct hook_ep *hook_ep = container_of(ep, struct hook_ep, ep);

	return fi_inject(hook_ep->hep, buf, len, dest_addr);
}

static ssize_t hook_hmem_msg_senddata(struct fid_ep *ep, const void *buf,
		size_t len, void *desc, uint64_t data, fi_addr_t dest_addr,
		void *context)
{
	struct hook_hmem_ep *hmem_ep;
	struct hook_hmem_ctx *hmem_ctx;
	void *hmem_desc = desc;
	struct iovec iov;
	int ret;

	hmem_ep = container_of(ep, struct hook_hmem_ep, hook_ep.ep);

	iov.iov_base = (void *) buf;
	iov.iov_len = len;

	ret = hook_hmem_track(&hmem_ep->hook_ep, &iov, &hmem_desc, 1,
			      hmem_ep->tx_op_flags, context, &hmem_ctx);
	if (ret)
		return ret;

	ret = fi_senddata(hmem_ep->hook_ep.hep, buf, len, hmem_desc, data,
			  dest_addr, hmem_ctx);
	if (ret)
		hook_hmem_untrack(hmem_ctx);

	return ret;
}

static ssize_t hook_hmem_msg_injectdata(struct fid_ep *ep, const void *buf,
		size_t len, uint64_t data, fi_addr_t dest_addr)
{
	struct hook_ep *hook_ep = container_of(ep, struct hook_ep, ep);

	return fi_injectdata(hook_ep->hep, buf, len, data, dest_addr);
}

static struct fi_ops_msg hook_hmem_msg_ops = {
	.size = sizeof(struct fi_ops_msg),
	.recv = hook_hmem_msg_recv,
	.recvv = hook_hmem_msg_recvv,
	.recvmsg = hook_hmem_msg_recvmsg,
	.send = hook_hmem_msg_send,
	.sendv = hook_hmem_msg_sendv,
	.sendmsg = hook_hmem_msg_sendmsg,
	.inject = hook_hmem_msg_inject,
	.senddata = hook_hmem_msg_senddata,
	.injectdata = hook_hmem_msg_injectdata,
};

/*
 * rma ops
 */

static ssize_t hook_hmem_rma_read(struct fid_ep *ep, void *buf, size_t len,
		void *desc, fi_addr_t src_addr, uint64_t addr, uint64_t key,
		void *context)
{
	struct hook_hmem_ep *hmem_ep;
	struct hook_hmem_ctx *hmem_ctx;
	void *hmem_desc = desc;
	struct iovec iov;
	int ret;

	hmem_ep = container_of(ep, struct hook_hmem_ep, hook_ep.ep);

	iov.iov_base = buf;
	iov.iov_len = len;

	ret = hook_hmem_track(&hmem_ep->hook_ep, &iov, &hmem_desc, 1,
			      hmem_ep->tx_op_flags, context, &hmem_ctx);
	if (ret)
		return ret;

	ret = fi_read(hmem_ep->hook_ep.hep, buf, len, hmem_desc, src_addr, addr,
		      key, hmem_ctx);
	if (ret)
		hook_hmem_untrack(hmem_ctx);

	return ret;
}

static ssize_t hook_hmem_rma_readv(struct fid_ep *ep, const struct iovec *iov,
		void **desc, size_t count, fi_addr_t src_addr, uint64_t addr,
		uint64_t key, void *context)
{
	struct hook_hmem_ep *hmem_ep;
	struct hook_hmem_ctx *hmem_ctx;
	void *hmem_desc[HOOK_HMEM_IOV_LIMIT];
	int ret;

	hmem_ep = container_of(ep, struct hook_hmem_ep, hook_ep.ep);

	if (desc)
		memcpy(hmem_desc, desc, sizeof(*desc) * count);

	ret = hook_hmem_track(&hmem_ep->hook_ep, iov, hmem_desc, count,
			      hmem_ep->tx_op_flags, context, &hmem_ctx);
	if (ret)
		return ret;

	ret = fi_readv(hmem_ep->hook_ep.hep, iov, hmem_desc, count, src_addr,
		       addr, key, hmem_ctx);
	if (ret)
		hook_hmem_untrack(hmem_ctx);

	return ret;
}

static ssize_t hook_hmem_rma_readmsg(struct fid_ep *ep,
		const struct fi_msg_rma *msg, uint64_t flags)
{
	struct hook_hmem_ep *hmem_ep;
	struct hook_hmem_ctx *hmem_ctx;
	void *hmem_desc[HOOK_HMEM_IOV_LIMIT];
	struct fi_msg_rma my_msg = *msg;
	int ret;

	hmem_ep = container_of(ep, struct hook_hmem_ep, hook_ep.ep);

	if (msg->desc)
		memcpy(hmem_desc, msg->desc,
		       sizeof(*msg->desc) * msg->iov_count);

	ret = hook_hmem_track(&hmem_ep->hook_ep, msg->msg_iov, hmem_desc,
			      msg->iov_count, flags | hmem_ep->tx_msg_flags,
			      msg->context, &hmem_ctx);
	if (ret)
		return ret;

	my_msg.desc = hmem_desc;
	my_msg.context = hmem_ctx;

	ret = fi_readmsg(hmem_ep->hook_ep.hep, &my_msg, flags);
	if (ret)
		hook_hmem_untrack(hmem_ctx);

	return ret;
}

static ssize_t hook_hmem_rma_write(struct fid_ep *ep, const void *buf,
		size_t len, void *desc, fi_addr_t dest_addr, uint64_t addr,
		uint64_t key, void *context)
{
	struct hook_hmem_ep *hmem_ep;
	struct hook_hmem_ctx *hmem_ctx;
	void *hmem_desc = desc;
	struct iovec iov;
	int ret;

	hmem_ep = container_of(ep, struct hook_hmem_ep, hook_ep.ep);

	iov.iov_base = (void *) buf;
	iov.iov_len = len;

	ret = hook_hmem_track(&hmem_ep->hook_ep, &iov, &hmem_desc, 1,
			      hmem_ep->tx_op_flags, context, &hmem_ctx);
	if (ret)
		return ret;

	ret = fi_write(hmem_ep->hook_ep.hep, buf, len, hmem_desc, dest_addr,
		       addr, key, hmem_ctx);
	if (ret)
		hook_hmem_untrack(hmem_ctx);

	return ret;
}

static ssize_t hook_hmem_rma_writev(struct fid_ep *ep, const struct iovec *iov,
		void **desc, size_t count, fi_addr_t dest_addr, uint64_t addr,
		uint64_t key, void *context)
{
	struct hook_hmem_ep *hmem_ep;
	struct hook_hmem_ctx *hmem_ctx;
	void *hmem_desc[HOOK_HMEM_IOV_LIMIT];
	int ret;

	hmem_ep = container_of(ep, struct hook_hmem_ep, hook_ep.ep);

	if (desc)
		memcpy(hmem_desc, desc, sizeof(*desc) * count);

	ret = hook_hmem_track(&hmem_ep->hook_ep, iov, hmem_desc, count,
			      hmem_ep->tx_op_flags, context, &hmem_ctx);
	if (ret)
		return ret;

	ret = fi_writev(hmem_ep->hook_ep.hep, iov, hmem_desc, count, dest_addr,
			addr, key, hmem_ctx);
	if (ret)
		hook_hmem_untrack(hmem_ctx);

	return ret;
}

static ssize_t hook_hmem_rma_writemsg(struct fid_ep *ep,
		const struct fi_msg_rma *msg, uint64_t flags)
{
	struct hook_hmem_ep *hmem_ep;
	struct hook_hmem_ctx *hmem_ctx;
	void *hmem_desc[HOOK_HMEM_IOV_LIMIT];
	struct fi_msg_rma my_msg = *msg;
	int ret;

	hmem_ep = container_of(ep, struct hook_hmem_ep, hook_ep.ep);

	if (msg->desc)
		memcpy(hmem_desc, msg->desc,
		       sizeof(*msg->desc) * msg->iov_count);

	ret = hook_hmem_track(&hmem_ep->hook_ep, msg->msg_iov, hmem_desc,
			      msg->iov_count, flags | hmem_ep->tx_msg_flags,
			      msg->context, &hmem_ctx);
	if (ret)
		return ret;

	my_msg.desc = hmem_desc;
	my_msg.context = hmem_ctx;

	ret = fi_writemsg(hmem_ep->hook_ep.hep, &my_msg, flags);
	if (ret)
		hook_hmem_untrack(hmem_ctx);

	return ret;
}

static ssize_t hook_hmem_rma_inject(struct fid_ep *ep, const void *buf,
		size_t len, fi_addr_t dest_addr, uint64_t addr, uint64_t key)
{
	struct hook_ep *hook_ep = container_of(ep, struct hook_ep, ep);

	return fi_inject_write(hook_ep->hep, buf, len, dest_addr, addr, key);
}

static ssize_t hook_hmem_rma_writedata(struct fid_ep *ep, const void *buf,
		size_t len, void *desc, uint64_t data, fi_addr_t dest_addr,
		uint64_t addr, uint64_t key, void *context)
{
	struct hook_hmem_ep *hmem_ep;
	struct hook_hmem_ctx *hmem_ctx;
	void *hmem_desc = desc;
	struct iovec iov;
	int ret;

	hmem_ep = container_of(ep, struct hook_hmem_ep, hook_ep.ep);

	iov.iov_base = (void *) buf;
	iov.iov_len = len;

	ret = hook_hmem_track(&hmem_ep->hook_ep, &iov, &hmem_desc, 1,
			      hmem_ep->tx_op_flags, context, &hmem_ctx);
	if (ret)
		return ret;

	ret = fi_writedata(hmem_ep->hook_ep.hep, buf, len, hmem_desc, data,
			   dest_addr, addr, key, hmem_ctx);
	if (ret)
		hook_hmem_untrack(hmem_ctx);

	return ret;
}

static ssize_t hook_hmem_rma_injectdata(struct fid_ep *ep, const void *buf,
		size_t len, uint64_t data, fi_addr_t dest_addr, uint64_t addr,
		uint64_t key)
{
	struct hook_ep *hook_ep = container_of(ep, struct hook_ep, ep);

	return fi_inject_writedata(hook_ep->hep, buf, len, data, dest_addr,
				   addr, key);
}

static struct fi_ops_rma hook_hmem_rma_ops = {
	.size = sizeof(struct fi_ops_rma),
	.read = hook_hmem_rma_read,
	.readv = hook_hmem_rma_readv,
	.readmsg = hook_hmem_rma_readmsg,
	.write = hook_hmem_rma_write,
	.writev = hook_hmem_rma_writev,
	.writemsg = hook_hmem_rma_writemsg,
	.inject = hook_hmem_rma_inject,
	.writedata = hook_hmem_rma_writedata,
	.injectdata = hook_hmem_rma_injectdata,
};


/*
 * tagged message ops
 */

static ssize_t hook_hmem_tagged_recv(struct fid_ep *ep, void *buf, size_t len,
		void *desc, fi_addr_t src_addr, uint64_t tag, uint64_t ignore,
		void *context)
{
	struct hook_hmem_ep *hmem_ep;
	struct hook_hmem_ctx *hmem_ctx;
	void *hmem_desc = desc;
	struct iovec iov;
	int ret;

	hmem_ep = container_of(ep, struct hook_hmem_ep, hook_ep.ep);

	iov.iov_base = buf;
	iov.iov_len = len;

	ret = hook_hmem_track(&hmem_ep->hook_ep, &iov, &hmem_desc, 1,
			      hmem_ep->rx_op_flags, context, &hmem_ctx);
	if (ret)
		return ret;

	ret = fi_trecv(hmem_ep->hook_ep.hep, buf, len, hmem_desc, src_addr,
		       tag, ignore, hmem_ctx);
	if (ret)
		hook_hmem_untrack(hmem_ctx);

	return ret;
}

static ssize_t hook_hmem_tagged_recvv(struct fid_ep *ep,
		const struct iovec *iov, void **desc, size_t count,
		fi_addr_t src_addr, uint64_t tag, uint64_t ignore,
		void *context)
{
	struct hook_hmem_ep *hmem_ep;
	struct hook_hmem_ctx *hmem_ctx;
	void *hmem_desc[HOOK_HMEM_IOV_LIMIT];
	int ret;

	hmem_ep = container_of(ep, struct hook_hmem_ep, hook_ep.ep);

	if (desc)
		memcpy(hmem_desc, desc, sizeof(*desc) * count);

	ret = hook_hmem_track(&hmem_ep->hook_ep, iov, hmem_desc, count,
			      hmem_ep->rx_op_flags, context, &hmem_ctx);
	if (ret)
		return ret;

	ret = fi_trecvv(hmem_ep->hook_ep.hep, iov, hmem_desc, count, src_addr,
			tag, ignore, hmem_ctx);
	if (ret)
		hook_hmem_untrack(hmem_ctx);

	return ret;
}

static ssize_t hook_hmem_tagged_recvmsg(struct fid_ep *ep,
		const struct fi_msg_tagged *msg, uint64_t flags)
{
	struct hook_hmem_ep *hmem_ep;
	struct hook_hmem_ctx *hmem_ctx;
	void *hmem_desc[HOOK_HMEM_IOV_LIMIT];
	struct fi_msg_tagged my_msg = *msg;
	int ret;

	hmem_ep = container_of(ep, struct hook_hmem_ep, hook_ep.ep);

	if (msg->desc)
		memcpy(hmem_desc, msg->desc,
		       sizeof(*msg->desc) * msg->iov_count);

	ret = hook_hmem_track(&hmem_ep->hook_ep, msg->msg_iov, hmem_desc,
			      msg->iov_count, flags | hmem_ep->rx_msg_flags,
			      msg->context, &hmem_ctx);
	if (ret)
		return ret;

	my_msg.desc = hmem_desc;
	my_msg.context = hmem_ctx;

	ret = fi_trecvmsg(hmem_ep->hook_ep.hep, &my_msg, flags);
	if (ret)
		hook_hmem_untrack(hmem_ctx);

	return ret;
}

static ssize_t hook_hmem_tagged_send(struct fid_ep *ep, const void *buf,
		size_t len, void *desc, fi_addr_t dest_addr, uint64_t tag,
		void *context)
{
	struct hook_hmem_ep *hmem_ep;
	struct hook_hmem_ctx *hmem_ctx;
	void *hmem_desc = desc;
	struct iovec iov;
	int ret;

	hmem_ep = container_of(ep, struct hook_hmem_ep, hook_ep.ep);

	iov.iov_base = (void *) buf;
	iov.iov_len = len;

	ret = hook_hmem_track(&hmem_ep->hook_ep, &iov, &hmem_desc, 1,
			      hmem_ep->tx_op_flags, context, &hmem_ctx);
	if (ret)
		return ret;

	ret = fi_tsend(hmem_ep->hook_ep.hep, buf, len, hmem_desc, dest_addr,
		       tag, hmem_ctx);
	if (ret)
		hook_hmem_untrack(hmem_ctx);

	return ret;
}

static ssize_t hook_hmem_tagged_sendv(struct fid_ep *ep,
		const struct iovec *iov, void **desc, size_t count,
		fi_addr_t dest_addr, uint64_t tag, void *context)
{
	struct hook_hmem_ep *hmem_ep;
	struct hook_hmem_ctx *hmem_ctx;
	void *hmem_desc[HOOK_HMEM_IOV_LIMIT];
	int ret;

	hmem_ep = container_of(ep, struct hook_hmem_ep, hook_ep.ep);

	if (desc)
		memcpy(hmem_desc, desc, sizeof(*desc) * count);

	ret = hook_hmem_track(&hmem_ep->hook_ep, iov, hmem_desc, count,
			      hmem_ep->tx_op_flags, context, &hmem_ctx);
	if (ret)
		return ret;

	ret = fi_tsendv(hmem_ep->hook_ep.hep, iov, hmem_desc, count, dest_addr,
			tag, hmem_ctx);
	if (ret)
		hook_hmem_untrack(hmem_ctx);

	return ret;
}

static ssize_t hook_hmem_tagged_sendmsg(struct fid_ep *ep,
		const struct fi_msg_tagged *msg, uint64_t flags)
{
	struct hook_hmem_ep *hmem_ep;
	struct hook_hmem_ctx *hmem_ctx;
	void *hmem_desc[HOOK_HMEM_IOV_LIMIT];
	struct fi_msg_tagged my_msg = *msg;
	int ret;

	hmem_ep = container_of(ep, struct hook_hmem_ep, hook_ep.ep);

	if (msg->desc)
		memcpy(hmem_desc, msg->desc,
		       sizeof(*msg->desc) * msg->iov_count);

	ret = hook_hmem_track(&hmem_ep->hook_ep, msg->msg_iov, hmem_desc,
			      msg->iov_count, flags | hmem_ep->tx_msg_flags,
			      msg->context, &hmem_ctx);
	if (ret)
		return ret;

	my_msg.desc = hmem_desc;
	my_msg.context = hmem_ctx;

	ret = fi_tsendmsg(hmem_ep->hook_ep.hep, &my_msg, flags);
	if (ret)
		hook_hmem_untrack(hmem_ctx);

	return ret;
}

static ssize_t hook_hmem_tagged_inject(struct fid_ep *ep, const void *buf,
		size_t len, fi_addr_t dest_addr, uint64_t tag)
{
	struct hook_ep *hook_ep = container_of(ep, struct hook_ep, ep);

	return fi_tinject(hook_ep->hep, buf, len, dest_addr, tag);
}

static ssize_t hook_hmem_tagged_senddata(struct fid_ep *ep, const void *buf,
		size_t len, void *desc, uint64_t data, fi_addr_t dest_addr,
		uint64_t tag, void *context)
{
	struct hook_hmem_ep *hmem_ep;
	struct hook_hmem_ctx *hmem_ctx;
	void *hmem_desc = desc;
	struct iovec iov;
	int ret;

	hmem_ep = container_of(ep, struct hook_hmem_ep, hook_ep.ep);

	iov.iov_base = (void *) buf;
	iov.iov_len = len;

	ret = hook_hmem_track(&hmem_ep->hook_ep, &iov, &hmem_desc, 1,
			      hmem_ep->tx_op_flags, context, &hmem_ctx);
	if (ret)
		return ret;

	ret = fi_tsenddata(hmem_ep->hook_ep.hep, buf, len, hmem_desc, data,
			    dest_addr, tag, hmem_ctx);
	if (ret)
		hook_hmem_untrack(hmem_ctx);

	return ret;
}

static ssize_t hook_hmem_tagged_injectdata(struct fid_ep *ep, const void *buf,
		size_t len, uint64_t data, fi_addr_t dest_addr, uint64_t tag)
{
	struct hook_ep *hook_ep = container_of(ep, struct hook_ep, ep);

	return fi_tinjectdata(hook_ep->hep, buf, len, data, dest_addr, tag);
}

static struct fi_ops_tagged hook_hmem_tagged_ops = {
	.size = sizeof(struct fi_ops_tagged),
	.recv = hook_hmem_tagged_recv,
	.recvv = hook_hmem_tagged_recvv,
	.recvmsg = hook_hmem_tagged_recvmsg,
	.send = hook_hmem_tagged_send,
	.sendv = hook_hmem_tagged_sendv,
	.sendmsg = hook_hmem_tagged_sendmsg,
	.inject = hook_hmem_tagged_inject,
	.senddata = hook_hmem_tagged_senddata,
	.injectdata = hook_hmem_tagged_injectdata,
};


/*
 * completion queue ops
 */

static size_t cq_entry_size[] = {
	[FI_CQ_FORMAT_UNSPEC] = 0,
	[FI_CQ_FORMAT_CONTEXT] = sizeof(struct fi_cq_entry),
	[FI_CQ_FORMAT_MSG] = sizeof(struct fi_cq_msg_entry),
	[FI_CQ_FORMAT_DATA] = sizeof(struct fi_cq_data_entry),
	[FI_CQ_FORMAT_TAGGED] = sizeof(struct fi_cq_tagged_entry)
};

static ssize_t hook_hmem_cq_read_internal(struct hook_cq *cq, size_t size,
					  void *buf, size_t count)
{
	struct fi_cq_entry *read_entry, *write_entry;
	struct hook_hmem_ctx *hmem_ctx;
	ssize_t write_idx;
	int read_idx;

	for (read_idx = write_idx = 0; read_idx < count;
	     read_idx++, buf = (char *) buf + size) {
		read_entry = (struct fi_cq_entry *) buf;
		hmem_ctx = (struct hook_hmem_ctx *) read_entry->op_context;
		read_entry->op_context = hmem_ctx->app_ctx;
		if (hmem_ctx->flags & (FI_COMPLETION | FI_REMOTE_CQ_DATA)) {
			if (read_idx != write_idx) {
				write_entry = (struct fi_cq_entry *)
					((char *) buf + (size * write_idx));
				memcpy(write_entry, read_entry, size);
			}
			write_idx++;
		}
		hook_hmem_untrack(hmem_ctx);
	}

	return write_idx;
}

static ssize_t hook_hmem_cq_read(struct fid_cq *cq, void *buf, size_t count)
{
	struct hook_cq *my_cq = container_of(cq, struct hook_cq, cq);
	ssize_t ret;

	ret = fi_cq_read(my_cq->hcq, buf, count);
	if (ret < 0)
		return ret;

	return hook_hmem_cq_read_internal(my_cq, cq_entry_size[my_cq->format],
					  buf, ret);
}

static ssize_t hook_hmem_cq_readerr(struct fid_cq *cq,
		struct fi_cq_err_entry *buf, uint64_t flags)
{
	struct hook_cq *my_cq = container_of(cq, struct hook_cq, cq);
	ssize_t ret;

	ret = fi_cq_readerr(my_cq->hcq, buf, flags);
	if (ret < 0)
		return ret;

	return hook_hmem_cq_read_internal(my_cq, sizeof(*buf), buf, ret);
}

static ssize_t hook_hmem_cq_readfrom(struct fid_cq *cq, void *buf, size_t count,
		fi_addr_t *src_addr)
{
	struct hook_cq *my_cq = container_of(cq, struct hook_cq, cq);
	ssize_t ret;

	ret = fi_cq_readfrom(my_cq->hcq, buf, count, src_addr);
	if (ret < 0)
		return ret;

	return hook_hmem_cq_read_internal(my_cq, cq_entry_size[my_cq->format],
					  buf, ret);
}

static ssize_t hook_hmem_cq_sread(struct fid_cq *cq, void *buf, size_t count,
		const void *cond, int timeout)
{
	struct hook_cq *my_cq = container_of(cq, struct hook_cq, cq);
	ssize_t ret;

	ret = fi_cq_sread(my_cq->hcq, buf, count, cond, timeout);
	if (ret < 0)
		return ret;

	return hook_hmem_cq_read_internal(my_cq, cq_entry_size[my_cq->format],
					  buf, ret);
}

static ssize_t hook_hmem_cq_sreadfrom(struct fid_cq *cq, void *buf,
		size_t count, fi_addr_t *src_addr, const void *cond,
		int timeout)
{
	struct hook_cq *my_cq = container_of(cq, struct hook_cq, cq);
	ssize_t ret;

	ret = fi_cq_sreadfrom(my_cq->hcq, buf, count, src_addr, cond, timeout);
	if (ret < 0)
		return ret;

	return hook_hmem_cq_read_internal(my_cq, cq_entry_size[my_cq->format],
					  buf, ret);
}

static int hook_hmem_cq_signal(struct fid_cq *cq)
{
	struct hook_cq *my_cq = container_of(cq, struct hook_cq, cq);

	return fi_cq_signal(my_cq->hcq);
}

struct fi_ops_cq hook_hmem_cq_ops = {
	.size = sizeof(struct fi_ops_cq),
	.read = hook_hmem_cq_read,
	.readfrom = hook_hmem_cq_readfrom,
	.readerr = hook_hmem_cq_readerr,
	.sread = hook_hmem_cq_sread,
	.sreadfrom = hook_hmem_cq_sreadfrom,
	.signal = hook_hmem_cq_signal,
	.strerror = hook_cq_strerror,
};

/*
 * counter ops
 */

static uint64_t hook_hmem_cntr_read(struct fid_cntr *cntr)
{
	struct hook_cntr *my_cntr = container_of(cntr, struct hook_cntr, cntr);

	return fi_cntr_read(my_cntr->hcntr);
}

static uint64_t hook_hmem_cntr_readerr(struct fid_cntr *cntr)
{
	struct hook_cntr *my_cntr = container_of(cntr, struct hook_cntr, cntr);

	return fi_cntr_readerr(my_cntr->hcntr);
}

static int hook_hmem_cntr_add(struct fid_cntr *cntr, uint64_t value)
{
	struct hook_cntr *my_cntr = container_of(cntr, struct hook_cntr, cntr);

	return fi_cntr_add(my_cntr->hcntr, value);
}

static int hook_hmem_cntr_set(struct fid_cntr *cntr, uint64_t value)
{
	struct hook_cntr *my_cntr = container_of(cntr, struct hook_cntr, cntr);

	return fi_cntr_set(my_cntr->hcntr, value);
}

static int hook_hmem_cntr_wait(struct fid_cntr *cntr, uint64_t threshold,
			     int timeout)
{
	struct hook_cntr *my_cntr = container_of(cntr, struct hook_cntr, cntr);

	return fi_cntr_wait(my_cntr->hcntr, threshold, timeout);
}

static int hook_hmem_cntr_adderr(struct fid_cntr *cntr, uint64_t value)
{
	struct hook_cntr *my_cntr = container_of(cntr, struct hook_cntr, cntr);

	return fi_cntr_adderr(my_cntr->hcntr, value);
}

static int hook_hmem_cntr_seterr(struct fid_cntr *cntr, uint64_t value)
{
	struct hook_cntr *my_cntr = container_of(cntr, struct hook_cntr, cntr);

	return fi_cntr_seterr(my_cntr->hcntr, value);
}

struct fi_ops_cntr hook_hmem_cntr_ops = {
	.size = sizeof(struct fi_ops_cntr),
	.read = hook_hmem_cntr_read,
	.readerr = hook_hmem_cntr_readerr,
	.add = hook_hmem_cntr_add,
	.set = hook_hmem_cntr_set,
	.wait = hook_hmem_cntr_wait,
	.adderr = hook_hmem_cntr_adderr,
	.seterr = hook_hmem_cntr_seterr,
};


/*
 * memory region
 */

static int hook_hmem_mr_regattr(struct fid *fid, const struct fi_mr_attr *attr,
		uint64_t flags, struct fid_mr **mr)
{
	struct hook_domain *domain;
	struct fi_mr_attr my_attr = *attr;
	struct hook_mr *my_mr;
	uint64_t my_flags;
	int ret, i;

	domain = container_of(fid, struct hook_domain, domain.fid);
	my_mr = calloc(1, sizeof(*my_mr));
	if (!my_mr)
		return -FI_ENOMEM;

	my_mr->domain = domain;
	my_mr->mr.fid.fclass = FI_CLASS_MR;
	my_mr->mr.fid.context = attr->context;
	my_mr->mr.fid.ops = &hook_fid_ops;

	for (i = 0; i < attr->iov_count; i++)
		my_attr.iface = ofi_get_hmem_iface(attr->mr_iov[i].iov_base,
					&my_attr.device.reserved, &my_flags);

	ret = fi_mr_regattr(domain->hdomain, &my_attr, flags | my_flags,
			    &my_mr->hmr);
	if (ret) {
		free(my_mr);
	} else {
		my_mr->mr.mem_desc = my_mr->hmr->mem_desc;
		my_mr->mr.key = my_mr->hmr->key;
		*mr = &my_mr->mr;
	}

	return ret;
}

static int hook_hmem_mr_regv(struct fid *fid, const struct iovec *iov,
		size_t count, uint64_t access, uint64_t offset,
		uint64_t requested_key, uint64_t flags, struct fid_mr **mr,
		void *context)
{
	struct fi_mr_attr attr;

	attr.mr_iov = iov;
	attr.iov_count = count;
	attr.access = access;
	attr.offset = offset;
	attr.requested_key = requested_key;
	attr.context = context;
	attr.auth_key_size = 0;
	attr.auth_key = NULL;

	/* hook_hmem_mr_regattr will determine the correct iface,
	 * this silences warnings about uninitialized fields
	 */
	attr.iface = FI_HMEM_SYSTEM;
	attr.device.reserved = 0;
	attr.hmem_data = NULL;

	return hook_hmem_mr_regattr(fid, &attr, flags, mr);
}

static int hook_hmem_mr_reg(struct fid *fid, const void *buf, size_t len,
		uint64_t access, uint64_t offset, uint64_t requested_key,
		uint64_t flags, struct fid_mr **mr, void *context)
{
	struct iovec iov;

	iov.iov_base = (void *) buf;
	iov.iov_len = len;
	return hook_hmem_mr_regv(fid, &iov, 1, access, offset, requested_key,
				 flags, mr, context);
}

static struct fi_ops_mr hook_hmem_mr_ops = {
	.size = sizeof(struct fi_ops_mr),
	.reg = hook_hmem_mr_reg,
	.regv = hook_hmem_mr_regv,
	.regattr = hook_hmem_mr_regattr,
};

/*
 * initialization
 */

static int hook_hmem_cq_init(struct fid *fid)
{
	struct fid_cq *cq = container_of(fid, struct fid_cq, fid);
	cq->ops = &hook_hmem_cq_ops;
	return 0;
}

static int hook_hmem_cntr_init(struct fid *fid)
{
	struct fid_cntr *cntr = container_of(fid, struct fid_cntr, fid);
	cntr->ops = &hook_hmem_cntr_ops;
	return 0;
}

struct hook_prov_ctx hook_hmem_prov_ctx;

static struct fi_ops_fabric hook_hmem_fabric_ops;

static int hook_hmem_fabric(struct fi_fabric_attr *attr,
			    struct fid_fabric **fabric, void *context)
{
	struct fi_provider *hprov = context;
	struct hook_fabric *fab;

	FI_TRACE(hprov, FI_LOG_FABRIC, "Installing HMEM hook\n");
	fab = calloc(1, sizeof *fab);
	if (!fab)
		return -FI_ENOMEM;

	hook_fabric_init(fab, HOOK_HMEM, attr->fabric, hprov,
			 &hook_fid_ops, &hook_hmem_prov_ctx);
	*fabric = &fab->fabric;
	fab->fabric.ops = &hook_hmem_fabric_ops;

	return 0;
}

struct hook_prov_ctx hook_hmem_prov_ctx = {
	.prov = {
		.version = OFI_VERSION_DEF_PROV,
		/* We're a pass-through provider
		   so the fi_version is always the latest */
		.fi_version = FI_VERSION(FI_MAJOR_VERSION, FI_MINOR_VERSION),
		.name = "ofi_hook_hmem",
		.getinfo = NULL,
		.fabric = hook_hmem_fabric,
		.cleanup = NULL,
	},
};

static int hook_hmem_domain_close(struct fid *fid)
{
	struct hook_hmem_domain *hmem_domain;
	struct hook_hmem_desc *hmem_desc;
	int ret;

	hmem_domain = container_of(fid, struct hook_hmem_domain,
				   hook_domain.domain);

	while (!dlist_empty(&hmem_domain->mr_list)) {
		dlist_pop_front(&hmem_domain->mr_list, struct hook_hmem_desc,
				hmem_desc, entry);
		if (hmem_desc->desc)
			fi_close(&(hmem_desc->mr_fid)->fid);
		dlist_remove(&hmem_desc->entry);
		ofi_buf_free(hmem_desc);
	}

	ofi_rbmap_cleanup(&hmem_domain->rbmap);

	ret = fi_close(&hmem_domain->hook_domain.hdomain->fid);
	if (ret)
		return ret;

	ofi_bufpool_destroy(hmem_domain->mr_pool);
	ofi_bufpool_destroy(hmem_domain->ctx_pool);
	ofi_mutex_destroy(&hmem_domain->lock);

	free(hmem_domain);
	return 0;
}

static int hook_hmem_bind(struct fid *fid, struct fid *bfid, uint64_t flags)
{
	struct hook_hmem_ep *hmem_ep = container_of(fid, struct hook_hmem_ep,
						hook_ep.ep);
	struct fid *hfid, *hbfid;

	hfid = hook_to_hfid(fid);
	hbfid = hook_to_hfid(bfid);
	if (!hfid || !hbfid)
		return -FI_EINVAL;

	if (hbfid->fclass == FI_CLASS_CQ) {
		if (flags & FI_TRANSMIT) {
			if (!(flags & FI_SELECTIVE_COMPLETION)) {
				hmem_ep->tx_op_flags |= FI_COMPLETION;
				hmem_ep->tx_msg_flags = FI_COMPLETION;
			}
		}

		if (flags & FI_RECV) {
			if (!(flags & FI_SELECTIVE_COMPLETION)) {
				hmem_ep->rx_op_flags |= FI_COMPLETION;
				hmem_ep->rx_msg_flags = FI_COMPLETION;
			}
		}
		flags &= ~FI_SELECTIVE_COMPLETION;
	}

	return hfid->ops->bind(hfid, hbfid, flags);
}

struct fi_ops hook_hmem_ep_fid_ops = {
	.size = sizeof(struct fi_ops),
	.close = hook_close,
	.bind = hook_hmem_bind,
	.control = hook_control,
	.ops_open = hook_ops_open,
};

static int hook_hmem_endpoint(struct fid_domain *domain, struct fi_info *info,
			      struct fid_ep **ep, void *context)
{
	struct hook_hmem_ep *hmem_ep;
	int ret;

	hmem_ep = calloc(1, sizeof(*hmem_ep));
	if (!hmem_ep)
		return -FI_ENOMEM;

	hmem_ep->tx_op_flags = info->tx_attr->op_flags;
	hmem_ep->rx_op_flags = info->rx_attr->op_flags;

	ret = hook_endpoint_init(domain, info, ep, context, &hmem_ep->hook_ep);
	if (ret)
		free(hmem_ep);

	(*ep)->msg = &hook_hmem_msg_ops;
	(*ep)->rma = &hook_hmem_rma_ops;
	(*ep)->tagged = &hook_hmem_tagged_ops;
	(*ep)->fid.ops = &hook_hmem_ep_fid_ops;

	return ret;
}

static struct fi_ops hook_hmem_domain_fid_ops = {
	.size = sizeof(struct fi_ops),
	.close = hook_hmem_domain_close,
	.bind = hook_bind,
	.control = hook_control,
	.ops_open = hook_ops_open,
};

static struct fi_ops_domain hook_hmem_domain_ops = {
	.size = sizeof(struct fi_ops_domain),
	.av_open = hook_av_open,
	.cq_open = hook_cq_open,
	.endpoint = hook_hmem_endpoint,
	.scalable_ep = hook_scalable_ep,
	.cntr_open = hook_cntr_open,
	.poll_open = hook_poll_open,
	.stx_ctx = hook_stx_ctx,
	.srx_ctx = hook_srx_ctx,
	.query_atomic = hook_query_atomic,
	.query_collective = hook_query_collective,
};

static int hook_hmem_domain(struct fid_fabric *fabric, struct fi_info *info,
			    struct fid_domain **domain, void *context)
{
	struct hook_hmem_domain *hmem_domain;
	int ret;

	hmem_domain = calloc(1, sizeof(*hmem_domain));
	if (!hmem_domain)
		return -FI_ENOMEM;

	ret = hook_domain_init(fabric, info, domain, context,
			       &hmem_domain->hook_domain);
	if (ret)
		goto out;

	(*domain)->mr = &hook_hmem_mr_ops;
	(*domain)->fid.ops = &hook_hmem_domain_fid_ops;
        (*domain)->ops = &hook_hmem_domain_ops;

	ret = ofi_bufpool_create(&hmem_domain->mr_pool,
				 sizeof(struct hook_hmem_desc),
				 16, 0, 0, 0);
	if (ret) {
		hook_close(&(*domain)->fid);
		goto out;
	}

	ret = ofi_bufpool_create(&hmem_domain->ctx_pool,
				 sizeof(struct hook_hmem_ctx),
				 16, 0, 0, OFI_BUFPOOL_NO_TRACK);
	if (ret) {
		ofi_bufpool_destroy(hmem_domain->mr_pool);
		hook_close(&(*domain)->fid);
		goto out;
	}

	hmem_domain->mr_mode = info->domain_attr->mr_mode;
	ofi_rbmap_init(&hmem_domain->rbmap, hook_hmem_iov_compare);
	dlist_init(&hmem_domain->mr_list);
	ofi_mutex_init(&hmem_domain->lock);

	return 0;
out:
	free(hmem_domain);
	return ret;
}

HOOK_HMEM_INI
{
#ifdef HAVE_HOOK_HMEM_DL
	ofi_hmem_init();
#endif
	hook_hmem_fabric_ops = hook_fabric_ops;
	hook_hmem_fabric_ops.domain = hook_hmem_domain;

	hook_hmem_prov_ctx.ini_fid[FI_CLASS_CQ] = hook_hmem_cq_init;
	hook_hmem_prov_ctx.ini_fid[FI_CLASS_CNTR] = hook_hmem_cntr_init;

	return &hook_hmem_prov_ctx.prov;
}
