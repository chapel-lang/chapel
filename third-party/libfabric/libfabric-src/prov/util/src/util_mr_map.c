/*
 * Copyright (c) 2016 Intel Corporation, Inc.  All rights reserved.
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
#include <config.h>
#include <stdlib.h>
#include <ofi_enosys.h>
#include <ofi_util.h>
#include <ofi_mr.h>
#include <assert.h>


static struct fi_mr_attr *
dup_mr_attr(const struct fi_mr_attr *attr)
{
	struct fi_mr_attr *dup_attr;

	dup_attr = calloc(1, sizeof(*attr) +
			     sizeof(*attr->mr_iov) * attr->iov_count);
	if (!dup_attr)
		return NULL;

	*dup_attr = *attr;
	dup_attr->mr_iov = (struct iovec *) (dup_attr + 1);
	memcpy((void *) dup_attr->mr_iov, attr->mr_iov,
		sizeof(*attr->mr_iov) * attr->iov_count);

	return dup_attr;
}

int ofi_mr_map_insert(struct ofi_mr_map *map, const struct fi_mr_attr *attr,
		      uint64_t *key, void *context)
{
	struct fi_mr_attr *item;
	int ret;

	item = dup_mr_attr(attr);
	if (!item)
		return -FI_ENOMEM;

	if (!(map->mode & FI_MR_VIRT_ADDR))
		item->offset = (uintptr_t) attr->mr_iov[0].iov_base;

	if (map->mode & FI_MR_PROV_KEY)
		item->requested_key = map->key++;

	ret = ofi_rbmap_insert(map->rbtree, &item->requested_key, item, NULL);
	if (ret) {
		if (ret == -FI_EALREADY)
			ret = -FI_ENOKEY;
		goto err;
	}
	*key = item->requested_key;
	item->context = context;

	return 0;
err:
	free(item);
	return ret;
}

void *ofi_mr_map_get(struct ofi_mr_map *map, uint64_t key)
{
	struct fi_mr_attr *attr;
	struct ofi_rbnode *node;

	node = ofi_rbmap_find(map->rbtree, &key);
	if (!node)
		return NULL;

	attr = node->data;
	return attr->context;
}

int ofi_mr_map_verify(struct ofi_mr_map *map, uintptr_t *io_addr,
		      size_t len, uint64_t key, uint64_t access,
		      void **context)
{
	struct fi_mr_attr *attr;
	struct ofi_rbnode *node;
	void *addr;

	node = ofi_rbmap_find(map->rbtree, &key);
	if (!node)
		return -FI_EINVAL;

	attr = node->data;
	assert(attr);

	if ((access & attr->access) != access) {
		FI_DBG(map->prov, FI_LOG_MR, "verify_addr: invalid access\n");
		return -FI_EACCES;
	}

	addr = (void *) (*io_addr + (uintptr_t) attr->offset);

	if ((addr < attr->mr_iov[0].iov_base) ||
	    (((char *) addr + len) > ((char *) attr->mr_iov[0].iov_base +
			    	      attr->mr_iov[0].iov_len))) {
		return -FI_EACCES;
	}

	if (context)
		*context = attr->context;
	*io_addr = (uintptr_t) addr;
	return 0;
}

int ofi_mr_map_remove(struct ofi_mr_map *map, uint64_t key)
{
	struct ofi_rbnode *node;
	struct fi_mr_attr *attr;

	node = ofi_rbmap_find(map->rbtree, &key);
	if (!node)
		return -FI_ENOKEY;

	attr = node->data;
	ofi_rbmap_delete(map->rbtree, node);
	free(attr);

	return 0;
}

/* assumes uint64_t keys */
static int compare_mr_keys(struct ofi_rbmap *rbtree,
			   void *key, void *data)
{
	struct fi_mr_attr *attr = data;
	uint64_t mrkey = *((uint64_t *) key);

	return (mrkey < attr->requested_key) ? -1 :
	       (mrkey > attr->requested_key);
}


/*
 * If a provider or app whose version is < 1.5, calls this function and passes
 * FI_MR_UNSPEC as mode, it would be treated as MR scalable.
 */
int ofi_mr_map_init(const struct fi_provider *prov, int mode,
		    struct ofi_mr_map *map)
{
	map->rbtree = ofi_rbmap_create(compare_mr_keys);
	if (!map->rbtree)
		return -FI_ENOMEM;

	switch (mode) {
	case FI_MR_BASIC:
		map->mode = OFI_MR_BASIC_MAP;
		break;
	case FI_MR_SCALABLE:
		map->mode = 0;
		break;
	default:
		map->mode = mode;
	}
	map->prov = prov;
	map->key = 1;

	return 0;
}

void ofi_mr_map_close(struct ofi_mr_map *map)
{
	ofi_rbmap_destroy(map->rbtree);
}

int ofi_mr_close(struct fid *fid)
{
	struct ofi_mr *mr;
	int ret;

	mr = container_of(fid, struct ofi_mr, mr_fid.fid);

	fastlock_acquire(&mr->domain->lock);
	ret = ofi_mr_map_remove(&mr->domain->mr_map, mr->key);
	fastlock_release(&mr->domain->lock);
	if (ret)
		return ret;

	ofi_atomic_dec32(&mr->domain->ref);
	free(mr);
	return 0;
}

static struct fi_ops ofi_mr_fi_ops = {
	.size = sizeof(struct fi_ops),
	.close = ofi_mr_close,
	.bind = fi_no_bind,
	.control = fi_no_control,
	.ops_open = fi_no_ops_open
};

int ofi_mr_regattr(struct fid *fid, const struct fi_mr_attr *attr,
		   uint64_t flags, struct fid_mr **mr_fid)
{
	struct util_domain *domain;
	struct ofi_mr *mr;
	uint64_t key;
	int ret = 0;

	if (fid->fclass != FI_CLASS_DOMAIN || !attr || attr->iov_count <= 0)
		return -FI_EINVAL;

	domain = container_of(fid, struct util_domain, domain_fid.fid);
	mr = calloc(1, sizeof(*mr));
	if (!mr)
		return -FI_ENOMEM;

	fastlock_acquire(&domain->lock);

	mr->mr_fid.fid.fclass = FI_CLASS_MR;
	mr->mr_fid.fid.context = attr->context;
	mr->mr_fid.fid.ops = &ofi_mr_fi_ops;
	mr->domain = domain;
	mr->flags = flags;

	ret = ofi_mr_map_insert(&domain->mr_map, attr, &key, mr);
	if (ret) {
		free(mr);
		goto out;
	}

	mr->mr_fid.key = mr->key = key;
	mr->mr_fid.mem_desc = (void *) (uintptr_t) key;

	*mr_fid = &mr->mr_fid;
	ofi_atomic_inc32(&domain->ref);

out:
	fastlock_release(&domain->lock);
	return ret;
}

int ofi_mr_regv(struct fid *fid, const struct iovec *iov,
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
	return ofi_mr_regattr(fid, &attr, flags, mr_fid);
}

int ofi_mr_reg(struct fid *fid, const void *buf, size_t len,
	       uint64_t access, uint64_t offset, uint64_t requested_key,
	       uint64_t flags, struct fid_mr **mr_fid, void *context)
{
	struct iovec iov;

	iov.iov_base = (void *) buf;
	iov.iov_len = len;
	return ofi_mr_regv(fid, &iov, 1, access, offset, requested_key, flags,
			   mr_fid, context);
}

int ofi_mr_verify(struct ofi_mr_map *map, ssize_t len,
		  uintptr_t *addr, uint64_t key, uint64_t access)
{
	struct util_domain *domain;
	int ret;

	domain = container_of(map, struct util_domain, mr_map);
	fastlock_acquire(&domain->lock);
	ret = ofi_mr_map_verify(&domain->mr_map, addr, len,
				key, access, NULL);
	fastlock_release(&domain->lock);
	return ret;
}
