/*
 * Copyright (c) 2019-2022 Intel Corporation. All rights reserved.
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

#include "coll.h"

static int
coll_av_set_union(struct fid_av_set *dst, const struct fid_av_set *src)
{
	struct util_av_set *src_av_set;
	struct util_av_set *dst_av_set;
	int i,j;

	src_av_set = container_of(src, struct util_av_set, av_set_fid);
	dst_av_set = container_of(dst, struct util_av_set, av_set_fid);

	assert(src_av_set->av == dst_av_set->av);

	for (i = 0; i < src_av_set->fi_addr_count; i++) {
		for (j = 0; j < dst_av_set->fi_addr_count; j++) {
			if (dst_av_set->fi_addr_array[j] ==
			    src_av_set->fi_addr_array[i])
				break;
		}
		if (j == dst_av_set->fi_addr_count) {
			if (dst_av_set->fi_addr_count >=
			    dst_av_set->max_array_size) {
				FI_INFO(dst_av_set->av->prov, FI_LOG_AV,
					"destination AV is full\n");
				return -FI_ENOMEM;
			}

			dst_av_set->fi_addr_array[dst_av_set->fi_addr_count++] =
						src_av_set->fi_addr_array[i];
			FI_DBG(dst_av_set->av->prov, FI_LOG_AV,
				"Adding fi_addr: %" PRIu64 "\n",
				src_av_set->fi_addr_array[i]);
		}
	}
	return FI_SUCCESS;
}

static int
coll_av_set_intersect(struct fid_av_set *dst, const struct fid_av_set *src)
{
	struct util_av_set *src_av_set;
	struct util_av_set *dst_av_set;
	int i,j, temp;

	src_av_set = container_of(src, struct util_av_set, av_set_fid);
	dst_av_set = container_of(dst, struct util_av_set, av_set_fid);

	assert(src_av_set->av == dst_av_set->av);

	temp = 0;
	for (i = 0; i < src_av_set->fi_addr_count; i++) {
		for (j = temp; j < dst_av_set->fi_addr_count; j++) {
			if (dst_av_set->fi_addr_array[j] ==
			    src_av_set->fi_addr_array[i]) {
				dst_av_set->fi_addr_array[temp++] =
					dst_av_set->fi_addr_array[j];
				break;
			}
		}
	}
	dst_av_set->fi_addr_count = temp;
	return FI_SUCCESS;
}

static int
coll_av_set_diff(struct fid_av_set *dst, const struct fid_av_set *src)
{

	struct util_av_set *src_av_set;
	struct util_av_set *dst_av_set;
	int i,j;
	size_t temp;

	src_av_set = container_of(src, struct util_av_set, av_set_fid);
	dst_av_set = container_of(dst, struct util_av_set, av_set_fid);

	assert(src_av_set->av == dst_av_set->av);

	temp = dst_av_set->fi_addr_count;
	for (i = 0; i < src_av_set->fi_addr_count; i++) {
		for (j = 0; j < temp; j++) {
			if (dst_av_set->fi_addr_array[j] ==
			    src_av_set->fi_addr_array[i]) {
				dst_av_set->fi_addr_array[--temp] =
					dst_av_set->fi_addr_array[j];
				break;
			}
		}
	}
	dst_av_set->fi_addr_count = temp;
	return FI_SUCCESS;
}

static int coll_av_set_insert(struct fid_av_set *set, fi_addr_t addr)
{
	struct util_av_set *av_set;
	int i;

	av_set = container_of(set, struct util_av_set, av_set_fid);
	if (av_set->fi_addr_count >= av_set->max_array_size)  {
		FI_INFO(av_set->av->prov, FI_LOG_AV, "AV set full\n");
		return -FI_ENOMEM;
	}

	for (i = 0; i < av_set->fi_addr_count; i++) {
		if (av_set->fi_addr_array[i] == addr)
			return -FI_EINVAL;
	}

	av_set->fi_addr_array[av_set->fi_addr_count++] = addr;
	FI_DBG(av_set->av->prov, FI_LOG_AV,
		"fi_addr: %" PRIu64 "\n", addr);
	return FI_SUCCESS;
}

static int coll_av_set_remove(struct fid_av_set *set, fi_addr_t addr)
{
	struct util_av_set *av_set;
	int i;

	av_set = container_of(set, struct util_av_set, av_set_fid);

	for (i = 0; i < av_set->fi_addr_count; i++) {
		if (av_set->fi_addr_array[i] == addr) {
			av_set->fi_addr_array[i] =
				av_set->fi_addr_array[--av_set->fi_addr_count];
			return FI_SUCCESS;
		}
	}
	return -FI_EINVAL;
}

static int coll_av_set_addr(struct fid_av_set *set, fi_addr_t *coll_addr)
{
	struct util_av_set *av_set;

	av_set = container_of(set, struct util_av_set, av_set_fid);

	*coll_addr = (uintptr_t) &av_set->coll_mc;

	return FI_SUCCESS;
}

static struct fi_ops_av_set coll_av_set_ops = {
	.set_union	= coll_av_set_union,
	.intersect	= coll_av_set_intersect,
	.diff		= coll_av_set_diff,
	.insert		= coll_av_set_insert,
	.remove		= coll_av_set_remove,
	.addr		= coll_av_set_addr,
};

static int coll_av_set_close(struct fid *fid)
{
	struct util_av_set *av_set;

	av_set = container_of(fid, struct util_av_set, av_set_fid.fid);
	if (ofi_atomic_get32(&av_set->ref) > 0)
		return -FI_EBUSY;

	ofi_atomic_dec32(&av_set->av->ref);
	free(av_set->fi_addr_array);
	free(av_set);
	return FI_SUCCESS;
}

static struct fi_ops coll_av_set_fi_ops = {
	.size = sizeof(struct fi_ops),
	.close = coll_av_set_close,
	.bind = fi_no_bind,
	.control = fi_no_control,
	.ops_open = fi_no_ops_open,
};

int coll_av_set(struct fid_av *av_fid, struct fi_av_set_attr *attr,
		struct fid_av_set **av_set_fid, void *context)
{
	struct util_av_set *av_set;
	struct coll_av *av;
	struct fid_peer_av *peer_av;
	struct fi_av_attr av_attr;
	uint64_t i;
	int ret;

	av = container_of(av_fid, struct coll_av, util_av.av_fid);
	peer_av  = av->peer_av;

	ret = peer_av->owner_ops->query(peer_av, &av_attr);
	if (ret)
		return ret;

	av_set = calloc(1, sizeof(*av_set));
	if (!av_set)
		return -FI_ENOMEM;

	ret = ofi_mutex_init(&av_set->lock);
	if (ret)
		goto err1;

	av_set->max_array_size = attr->count ? attr->count : av_attr.count;
	av_set->fi_addr_array = calloc(av_set->max_array_size,
				       sizeof(*av_set->fi_addr_array));
	if (!av_set->fi_addr_array) {
		ret = -FI_ENOMEM;
		goto err2;
	}

	if (attr->start_addr != FI_ADDR_NOTAVAIL &&
	    attr->end_addr != FI_ADDR_NOTAVAIL) {
		for (i = attr->start_addr; i <= attr->end_addr;
		     i += attr->stride) {
			if (av_set->fi_addr_count >= av_set->max_array_size) {
				FI_WARN(av_set->av->prov, FI_LOG_AV,
					"AV set size (%zu) not large enough\n",
					av_set->max_array_size);
				ret = -FI_EINVAL;
				goto err3;
			}

			av_set->fi_addr_array[av_set->fi_addr_count++] = i;
		}
	} else if (attr->start_addr != attr->end_addr) {
		FI_WARN(av_set->av->prov, FI_LOG_AV,
			"AV set start and end addr must both be set to"
			"FI_ADDR_NOTAVAIL when creating empty AV set\n");
		ret = -FI_EINVAL;
		goto err3;
	}

	ofi_atomic_initialize32(&av_set->ref, 0);
	av_set->coll_mc.av_set = av_set;
	av_set->av_set_fid.ops = &coll_av_set_ops;
	av_set->av_set_fid.fid.fclass = FI_CLASS_AV_SET;
	av_set->av_set_fid.fid.context = context;
	av_set->av_set_fid.fid.ops = &coll_av_set_fi_ops;
	av_set->av = &av->util_av;
	ofi_atomic_inc32(&av_set->av->ref);

	*av_set_fid = &av_set->av_set_fid;
	return FI_SUCCESS;

err3:
	free(av_set->fi_addr_array);
err2:
	ofi_mutex_destroy(&av_set->lock);
err1:
	free(av_set);
	return ret;
}
