/*
 * Copyright (c) 2020 Intel Corporation. All rights reserved.
 * Copyright (c) 2021-2023 Hewlett Packard Enterprise Development LP
 * SPDX-License-Identifier: BSD-2-Clause OR GPL-2.0-only
 */

/*
 * Notes:
 *
 * To implement this as an extension of util_av_set requires that AV be an
 * extension of util_av, which it currently is not.
 *
 * The bulk of the util code is involved with a point-to-point implementaion of
 * collectives, and the util_av_set code is relatively trivial, and also has a
 * bad bug in util_av_set_diff().
 *
 * Our current plan is to implement only accelerated multicast operations in
 * libfabric, and leave all point-to-point implementations to the regular MPI
 * algorithms, which will (in general) be better optimized and tunable.
 *
 * At some future point, we can rework cxip_av to be an extension of util_av,
 * eliminate this code in favor of the util_coll code, with custom
 * implementations of the accelerated multicast operations.
 *
 */
#include "config.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/types.h>

#include <ofi_list.h>
#include <ofi.h>

#include "cxip.h"

int cxip_av_set_union(struct fid_av_set *dst, const struct fid_av_set *src)
{
	/* Must append to end */
	struct cxip_av_set *src_av_set;
	struct cxip_av_set *dst_av_set;
	size_t temp;
	int i,j;

	src_av_set = container_of(src, struct cxip_av_set, av_set_fid);
	dst_av_set = container_of(dst, struct cxip_av_set, av_set_fid);

	if (src_av_set->cxi_av != dst_av_set->cxi_av)
		return -FI_EINVAL;

	if (dst_av_set->mc_obj)
		return -FI_EPERM;

	/* New elements placed at end of dst */
	temp = dst_av_set->fi_addr_cnt;
	for (i = 0; i < src_av_set->fi_addr_cnt; i++) {
		for (j = 0; j < dst_av_set->fi_addr_cnt; j++) {
			if (dst_av_set->fi_addr_ary[j] ==
 		 	    src_av_set->fi_addr_ary[i]) {
				/* src[i] already in dst */
				break;
			}
		}
		if (j == dst_av_set->fi_addr_cnt) {
			/* src[i] gets added to end of dst */
			dst_av_set->fi_addr_ary[temp++] =
				src_av_set->fi_addr_ary[i];
		}
	}
	/* temp >= dst_av_set->fi_addr_cnt */
	dst_av_set->fi_addr_cnt = temp;
	return FI_SUCCESS;
}

int cxip_av_set_intersect(struct fid_av_set *dst, const struct fid_av_set *src)
{
	/* Must preserve order */
	struct cxip_av_set *src_av_set;
	struct cxip_av_set *dst_av_set;
	int i,j, temp;

	src_av_set = container_of(src, struct cxip_av_set, av_set_fid);
	dst_av_set = container_of(dst, struct cxip_av_set, av_set_fid);

	if (src_av_set->cxi_av != dst_av_set->cxi_av)
		return -FI_EINVAL;

	if (dst_av_set->mc_obj)
		return -FI_EPERM;

	/* Old elements removed from dst */
	temp = 0;
	for (i = 0; i < dst_av_set->fi_addr_cnt; i++) {
		for (j = 0; j < src_av_set->fi_addr_cnt; j++) {
			if (dst_av_set->fi_addr_ary[i] ==
 		 	    src_av_set->fi_addr_ary[j]) {
				/* dst[i] is in src, temp <= i */
				if (temp < i) {
					dst_av_set->fi_addr_ary[temp] =
						dst_av_set->fi_addr_ary[i];
				}
				temp++;
				break;
			}
		}
	}
	/* temp <= dst_av_set->fi_addr_cnt */
	dst_av_set->fi_addr_cnt = temp;
	return FI_SUCCESS;
}

int cxip_av_set_diff(struct fid_av_set *dst, const struct fid_av_set *src)
{
	/* Must preserve order */
	struct cxip_av_set *src_av_set;
	struct cxip_av_set *dst_av_set;
	int i,j, temp;

	src_av_set = container_of(src, struct cxip_av_set, av_set_fid);
	dst_av_set = container_of(dst, struct cxip_av_set, av_set_fid);

	if (src_av_set->cxi_av != dst_av_set->cxi_av)
		return -FI_EINVAL;

	if (dst_av_set->mc_obj)
		return -FI_EPERM;

	/* Old elements removed from dst */
	temp = 0;
	for (i = 0; i < dst_av_set->fi_addr_cnt; i++) {
		for (j = 0; j < src_av_set->fi_addr_cnt; j++) {
			if (dst_av_set->fi_addr_ary[i] ==
			    src_av_set->fi_addr_ary[j])
				break;
		}
		if (j == src_av_set->fi_addr_cnt) {
			/* temp <= i */
			if (temp < dst_av_set->fi_addr_cnt) {
				dst_av_set->fi_addr_ary[temp] =
					dst_av_set->fi_addr_ary[i];
			}
			temp++;
		}
	}
	/* temp <= dst_av_set->fi_addr_cnt */
	dst_av_set->fi_addr_cnt = temp;
	return FI_SUCCESS;
}

int cxip_av_set_insert(struct fid_av_set *set, fi_addr_t addr)
{
	/* Must append to end */
	struct cxip_av_set *av_set_obj;
	int i;

	av_set_obj = container_of(set, struct cxip_av_set, av_set_fid);

	if (av_set_obj->mc_obj)
		return -FI_EPERM;

	/* Do not insert duplicates */
	for (i = 0; i < av_set_obj->fi_addr_cnt; i++) {
		if (av_set_obj->fi_addr_ary[i] == addr)
			return -FI_EINVAL;
	}
	/* Append new value */
	av_set_obj->fi_addr_ary[av_set_obj->fi_addr_cnt++] = addr;
	return FI_SUCCESS;
}

int cxip_av_set_remove(struct fid_av_set *set, fi_addr_t addr)
{
	/* Must preserve ordering */
	struct cxip_av_set *av_set_obj;
	int i;

	av_set_obj = container_of(set, struct cxip_av_set, av_set_fid);

	if (av_set_obj->mc_obj)
		return -FI_EPERM;

	for (i = 0; i < av_set_obj->fi_addr_cnt; i++) {
		if (av_set_obj->fi_addr_ary[i] == addr)
			break;
	}
	if (i == av_set_obj->fi_addr_cnt)
		return -FI_EINVAL;

	for (i++; i < av_set_obj->fi_addr_cnt; i++)
		av_set_obj->fi_addr_ary[i-1] = av_set_obj->fi_addr_ary[i];
	av_set_obj->fi_addr_cnt--;
	return FI_SUCCESS;
}

int cxip_av_set_addr(struct fid_av_set *set, fi_addr_t *coll_addr)
{
	*coll_addr = FI_ADDR_NOTAVAIL;
	return FI_SUCCESS;
}

int cxip_close_av_set(struct fid *fid)
{
	struct cxip_av_set *cxi_av_set;

	cxi_av_set = container_of(fid, struct cxip_av_set, av_set_fid.fid);
	if (cxi_av_set->mc_obj)
		return -FI_EBUSY;

	ofi_atomic_dec32(&cxi_av_set->cxi_av->ref);

	free(cxi_av_set->fi_addr_ary);
	free(cxi_av_set);
	return FI_SUCCESS;
}

static struct fi_ops_av_set cxip_av_set_ops= {
	.set_union = cxip_av_set_union,
	.intersect = cxip_av_set_intersect,
	.diff = cxip_av_set_diff,
	.insert = cxip_av_set_insert,
	.remove = cxip_av_set_remove,
	.addr = cxip_av_set_addr
};

static struct fi_ops cxip_av_set_fid_ops = {
	.size = sizeof(struct fi_ops),
	.close = cxip_close_av_set,
	.bind = fi_no_bind,
	.control = fi_no_control,
	.ops_open = fi_no_ops_open,
};

static inline int fi_addr_is_valid(struct cxip_av *av, fi_addr_t fi_addr)
{
	return cxip_av_lookup_addr(av, fi_addr, NULL) == FI_SUCCESS;
}

int cxip_av_set(struct fid_av *av, struct fi_av_set_attr *attr,
	        struct fid_av_set **av_set_fid, void *context)
{
	struct cxip_av *cxi_av;
	struct cxip_av_set *cxi_set;
	bool abeg, aend;
	fi_addr_t start, end;
	size_t count, stride;
	fi_addr_t i, j;
	int ret;
	size_t max_size;
	size_t av_entry_count;

	cxi_av = container_of(av, struct cxip_av, av_fid);
	av_entry_count = cxip_av_entry_count(cxi_av);

	if (!attr)
		return -FI_EINVAL;

	/* We need the AV to stick around now */
	ofi_atomic_inc32(&cxi_av->ref);

	/* May change values below, don't alter struct */
	start = attr->start_addr;
	end = attr->end_addr;
	count = attr->count;
	stride = attr->stride;
	abeg = (start != FI_ADDR_NOTAVAIL);
	aend = (end != FI_ADDR_NOTAVAIL);

	/* Override everything for UNIVERSE flag */
	if (attr->flags & FI_UNIVERSE) {
		start = FI_ADDR_NOTAVAIL;
		end = FI_ADDR_NOTAVAIL;
		count = FI_ADDR_NOTAVAIL;
		stride = 1;
		abeg = false;
		aend = false;
	}

	/* Common error for these syntax tests */
	ret = -FI_EINVAL;

	/* Must specify both, or neither */
	if (abeg != aend)
		goto err0;

	/* Cannot specify a range for FI_AV_MAP */
	if (abeg && cxi_av->type == FI_AV_MAP)
		goto err0;

	/* Cannot specify a range for empty AV set */
	if (abeg && count == 0)
		goto err0;

	/* Comm_key data must match in our structure */
	if (attr->comm_key && attr->comm_key_size &&
	    attr->comm_key_size != sizeof(struct cxip_comm_key))
		goto err0;

	/* Must specify a range if non-sequential stride */
	if (!abeg && stride > 1)
		goto err0;

	/* Stride unspecified means sequential */
	if (stride == 0)
		stride = 1;

	/* Resolve undefined range and count */
	if (start == FI_ADDR_NOTAVAIL)
		start = 0;
	if (end == FI_ADDR_NOTAVAIL)
		end = 0;
	if (count > end - start + 1)
		count = end - start + 1;

	cxi_set = calloc(1,sizeof(*cxi_set));
	if (!cxi_set) {
		ret = -FI_ENOMEM;
		goto err0;
	}

	/* Allocate enough space to add all addresses */
	max_size = attr->count ?
		attr->count : MAX(ofi_universe_size, av_entry_count);
	cxi_set->fi_addr_ary = calloc(max_size,
				      sizeof(*cxi_set->fi_addr_ary));
	if (!cxi_set->fi_addr_ary) {
		ret = -FI_ENOMEM;
		goto err1;
	}

	/* Add address indices */
	for (i=0, j=start;
	     i < count && j <= end && j < av_entry_count;
	     i++, j+=stride) {
		/* Skip over invalid addresses as if not there */
		while (!fi_addr_is_valid(cxi_av, i)) {
		       if (++j >= av_entry_count)
			       break;
		}
		if (j >= av_entry_count)
			break;
		cxi_set->fi_addr_ary[i] = (fi_addr_t)j;
		cxi_set->fi_addr_cnt++;
	}

	/* copy comm_key from attributes, if present */
	if (attr->comm_key && attr->comm_key_size) {
		memcpy(&cxi_set->comm_key, attr->comm_key,
		       attr->comm_key_size);
	}

	cxi_set->av_set_fid.fid.fclass = FI_CLASS_AV_SET;
	cxi_set->av_set_fid.fid.context = context;
	cxi_set->av_set_fid.fid.ops = &cxip_av_set_fid_ops;
	cxi_set->av_set_fid.ops = &cxip_av_set_ops;
	cxi_set->cxi_av = cxi_av;

	*av_set_fid = &cxi_set->av_set_fid;

	return FI_SUCCESS;
err1:
	free(cxi_set);
err0:
	ofi_atomic_dec32(&cxi_av->ref);
	return ret;
}
