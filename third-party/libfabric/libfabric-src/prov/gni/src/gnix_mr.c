/*
 * Copyright (c) 2015-2017 Cray Inc. All rights reserved.
 * Copyright (c) 2015-2018 Los Alamos National Security, LLC.
 *               All rights reserved.
 *
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

#include <stdlib.h>
#include <string.h>

#include "gnix.h"
#include "gnix_nic.h"
#include "gnix_util.h"
#include "gnix_mr.h"
#include "gnix_priv.h"

/* forward declarations */
static int __gnix_mr_refresh(struct gnix_fid_mem_desc *desc,
	uint64_t addr, uint64_t len);
static int fi_gnix_mr_close(fid_t fid);
static int fi_gnix_mr_control(struct fid *fid, int command, void *arg);

/* global declarations */
/* memory registration operations */
static struct fi_ops fi_gnix_mr_ops = {
	.size = sizeof(struct fi_ops),
	.close = fi_gnix_mr_close,
	.bind = fi_no_bind,
	.control = fi_gnix_mr_control,
	.ops_open = fi_no_ops_open,
};

/**
 * Sign extends the value passed into up to length parameter
 *
 * @param[in]  val  value to be sign extended
 * @param[in]  len  length to sign extend the value
 * @return          sign extended value to length, len
 */
static inline int64_t __sign_extend(
		uint64_t val,
		int len)
{
	int64_t m = 1UL << (len - 1);
	int64_t r = (val ^ m) - m;

	return r;
}

static inline void __print_mhdl(gni_mem_handle_t *mhdl)
{
	GNIX_INFO(FI_LOG_DOMAIN, "PRINT_MHDL:\n"
		"va=%016llx\n"
		"mdh=%d\n"
		"npages=%d\n"
		"pgsize=%d\n"
		"flags=%08llx\n"
		"crc=%08llx\n",
		GNI_MEMHNDL_GET_VA((*mhdl)),
		GNI_MEMHNDL_GET_MDH((*mhdl)),
		mhdl->qword2 & GNI_MEMHNDL_NPGS_MASK,
		(mhdl->qword2 >> 28) & GNI_MEMHNDL_PSIZE_MASK,
		GNI_MEMHNDL_GET_FLAGS((*mhdl)),
		mhdl->qword2 >> 56);
}

/**
 * Converts a key to a gni memory handle without calculating crc
 *
 * @param key   gnix memory registration key
 * @param mhdl  gni memory handle
 */
void _gnix_convert_key_to_mhdl_no_crc(
		gnix_mr_key_t *key,
		gni_mem_handle_t *mhdl)
{
	uint64_t va = key->pfn;
	uint8_t flags = 0;

	va = (uint64_t) __sign_extend(va << GNIX_MR_PAGE_SHIFT,
		GNIX_MR_VA_BITS);

	flags = (key->flags & GNIX_MR_FLAG_READONLY) ?
		GNI_MEMHNDL_ATTR_READONLY : 0;

	GNI_MEMHNDL_INIT((*mhdl));
	GNI_MEMHNDL_SET_PAGESIZE((*mhdl), GNIX_MR_PAGE_SHIFT);
	GNI_MEMHNDL_SET_NPAGES((*mhdl), GNI_MEMHNDL_NPGS_MASK);

	if (key->flags & GNIX_MR_FLAG_BASIC_REG) {
		va = key->pfn;
		va = (uint64_t) __sign_extend(va << GNIX_MR_PAGE_SHIFT,
			GNIX_MR_VA_BITS);

		GNI_MEMHNDL_SET_VA((*mhdl), va);
		GNI_MEMHNDL_SET_MDH((*mhdl), key->mdd);
	} else {
		GNI_MEMHNDL_SET_MDH((*mhdl), key->value);
		flags |= GNI_MEMHNDL_ATTR_VMDH;
	}
	GNI_MEMHNDL_SET_FLAGS((*mhdl), flags);
}

/**
 * Converts a key to a gni memory handle
 *
 * @param key   gnix memory registration key
 * @param mhdl  gni memory handle
 */
void _gnix_convert_key_to_mhdl(
	gnix_mr_key_t *key,
	gni_mem_handle_t *mhdl)
{
	_gnix_convert_key_to_mhdl_no_crc(key, mhdl);
	compiler_barrier();
	GNI_MEMHNDL_SET_CRC((*mhdl));
}

/**
 * Converts a gni memory handle to gnix memory registration key
 *
 * @param mhdl  gni memory handle
 * @return uint64_t representation of a gnix memory registration key
 */
uint64_t _gnix_convert_mhdl_to_key(gni_mem_handle_t *mhdl)
{
	gnix_mr_key_t key = {{{0}}};
	int flags = GNI_MEMHNDL_GET_FLAGS((*mhdl));

	/* VMDH handles do not have an address set */
	if (flags & GNI_MEMHNDL_ATTR_VMDH)
		return GNI_MEMHNDL_GET_MDH((*mhdl));

	key.pfn = GNI_MEMHNDL_GET_VA((*mhdl)) >> GNIX_MR_PAGE_SHIFT;
	key.mdd = GNI_MEMHNDL_GET_MDH((*mhdl));
	//key->format = GNI_MEMHNDL_NEW_FRMT((*mhdl));
	key.flags = GNIX_MR_FLAG_BASIC_REG;

	key.flags |= (flags & GNI_MEMHNDL_FLAG_READONLY) ?
		GNIX_MR_FLAG_READONLY : 0;


	return key.value;
}

/**
 * Helper function to calculate the length of a potential registration
 * based on some rules of the registration cache.
 *
 * Registrations should be page aligned and contain all of page(s)
 *
 * @param address   base address of the registration
 * @param length    length of the registration
 * @param pagesize  assumed page size of the registration
 * @return length for the new registration
 */
static inline uint64_t __calculate_length(
		uint64_t address,
		uint64_t length,
		uint64_t pagesize)
{
	uint64_t baseaddr = address & ~(pagesize - 1);
	uint64_t reg_len = (address + length) - baseaddr;
	uint64_t pages = reg_len / pagesize;

	if (reg_len % pagesize != 0)
		pages += 1;

	return pages * pagesize;
}

int _gnix_mr_reg(struct fid *fid, const void *buf, size_t len,
			  uint64_t access, uint64_t offset,
			  uint64_t requested_key, uint64_t flags,
			  struct fid_mr **mr_o, void *context,
			  struct gnix_auth_key *auth_key,
			  int reserved)
{
	struct gnix_fid_mem_desc *mr = NULL;
	struct gnix_fid_domain *domain;
	int rc;
	uint64_t reg_addr, reg_len;
	struct _gnix_fi_reg_context fi_reg_context = {
			.access = access,
			.offset = offset,
			.requested_key = requested_key,
			.flags = flags,
			.context = context,
			.auth_key = auth_key,
			.reserved = reserved,
	};
	struct gnix_mr_cache_info *info;

	GNIX_TRACE(FI_LOG_MR, "\n");

	GNIX_INFO(FI_LOG_MR, "reg: buf=%p len=%llu\n", buf, len);

	/* Flags are reserved for future use and must be 0. */
	if (OFI_UNLIKELY(flags))
		return -FI_EBADFLAGS;

	/* The offset parameter is reserved for future use and must be 0.
	 *   Additionally, check for invalid pointers, bad access flags and the
	 *   correct fclass on associated fid
	 */
	if (offset || !buf || !mr_o || !access ||
		(access & ~(FI_READ | FI_WRITE | FI_RECV | FI_SEND |
						FI_REMOTE_READ |
						FI_REMOTE_WRITE)) ||
		(fid->fclass != FI_CLASS_DOMAIN))
		return -FI_EINVAL;

	domain = container_of(fid, struct gnix_fid_domain, domain_fid.fid);

	if (auth_key->using_vmdh && !reserved &&
		requested_key >= auth_key->attr.user_key_limit)
		return -FI_EKEYREJECTED;

	if (!reserved && auth_key->using_vmdh) {
		/* adjust requested key by rank offset */
		fi_reg_context.requested_key += auth_key->key_offset;
		GNIX_DEBUG(FI_LOG_DOMAIN,
			"user requested key %d, but adjusting by "
			"rank offset as key %d\n",
			requested_key, fi_reg_context.requested_key);
	}

	if (auth_key->using_vmdh && !reserved &&
		requested_key < auth_key->attr.user_key_limit) {
		rc = _gnix_test_and_set_bit(auth_key->user,
				fi_reg_context.requested_key);
		if (rc) {
			GNIX_WARN(FI_LOG_DOMAIN, "key already in use, key=%d\n",
				fi_reg_context.requested_key);
			return -FI_ENOKEY;
		}
	}

	/* if this is a provider registration using VMDH and 0 was provided
	 * as the key, pick any available */
	if (auth_key->using_vmdh && reserved && !requested_key) {
		requested_key = _gnix_get_next_reserved_key(auth_key);
		if (requested_key <= 0)
			return -FI_ENOKEY;
		fi_reg_context.requested_key = requested_key;
	}

	info = &domain->mr_cache_info[auth_key->ptag];

	reg_addr = ((uint64_t) buf) & ~((1 << GNIX_MR_PAGE_SHIFT) - 1);
	reg_len = __calculate_length((uint64_t) buf, len,
			1 << GNIX_MR_PAGE_SHIFT);

	/* call cache register op to retrieve the right entry */
	fastlock_acquire(&info->mr_cache_lock);
	if (OFI_UNLIKELY(!domain->mr_ops))
		_gnix_open_cache(domain, GNIX_DEFAULT_CACHE_TYPE);

	if (OFI_UNLIKELY(!domain->mr_ops->is_init(domain, auth_key))) {
		rc = domain->mr_ops->init(domain, auth_key);
		if (rc != FI_SUCCESS) {
			fastlock_release(&info->mr_cache_lock);
			goto err;
		}
	}

	rc = domain->mr_ops->reg_mr(domain,
			(uint64_t) reg_addr, reg_len, &fi_reg_context,
			(void **) &mr);
	fastlock_release(&info->mr_cache_lock);

	/* check retcode */
	if (OFI_UNLIKELY(rc != FI_SUCCESS))
		goto err;

	/* md.mr_fid */
	mr->mr_fid.mem_desc = mr;
	mr->mr_fid.fid.fclass = FI_CLASS_MR;
	mr->mr_fid.fid.context = context;
	mr->mr_fid.fid.ops = &fi_gnix_mr_ops;

	/* setup internal key structure */
	mr->mr_fid.key = _gnix_convert_mhdl_to_key(&mr->mem_hndl);
	if (!reserved && auth_key->using_vmdh) {
		/* When using scalable, the key is a virtual index to the
		   vmdh table */
		mr->mr_fid.key = requested_key;
	}
	mr->auth_key = auth_key;

	if (reserved && auth_key->using_vmdh) {
		rc = __gnix_mr_refresh(mr, reg_addr, reg_len);
		if (rc != FI_SUCCESS)
			GNIX_FATAL(FI_LOG_MR,
				"failed to enabled internal provider registration, ret=%d",
				rc);
	}

	_gnix_ref_get(mr->domain);

	/* set up mr_o out pointer */
	*mr_o = &mr->mr_fid;
	return FI_SUCCESS;

err:
	return rc;
}

DIRECT_FN int gnix_mr_reg(struct fid *fid, const void *buf, size_t len,
	uint64_t access, uint64_t offset,
	uint64_t requested_key, uint64_t flags,
	struct fid_mr **mr, void *context)
{
	const struct iovec mr_iov = {
		.iov_base = (void *) buf,
		.iov_len = len,
	};
	const struct fi_mr_attr attr = {
		.mr_iov = &mr_iov,
		.iov_count = 1,
		.access = access,
		.offset = offset,
		.requested_key = requested_key,
		.context = context,
		.auth_key = NULL,
		.auth_key_size = 0,
	};

	return gnix_mr_regattr(fid, &attr, flags, mr);
}

DIRECT_FN int gnix_mr_regv(struct fid *fid, const struct iovec *iov,
	size_t count, uint64_t access,
	uint64_t offset, uint64_t requested_key,
	uint64_t flags, struct fid_mr **mr, void *context)
{
	const struct fi_mr_attr attr = {
		.mr_iov = iov,
		.iov_count = count,
		.access = access,
		.offset = offset,
		.requested_key = requested_key,
		.context = context,
		.auth_key = NULL,
		.auth_key_size = 0,
	};

	return gnix_mr_regattr(fid, &attr, flags, mr);
}

DIRECT_FN int gnix_mr_regattr(struct fid *fid, const struct fi_mr_attr *attr,
	uint64_t flags, struct fid_mr **mr)
{
	struct gnix_fid_domain *domain = container_of(fid,
		struct gnix_fid_domain, domain_fid.fid);
	struct gnix_auth_key *auth_key;

	if (!attr)
		return -FI_EINVAL;
	if (!attr->mr_iov || !attr->iov_count)
		return -FI_EINVAL;

	if (domain->mr_iov_limit < attr->iov_count)
		return -FI_EOPNOTSUPP;

	if (FI_VERSION_LT(domain->fabric->fab_fid.api_version,
		FI_VERSION(1, 5)) &&
		(attr->auth_key || attr->auth_key_size))
		return -FI_EINVAL;

	if (attr->auth_key_size) {
		auth_key = GNIX_GET_AUTH_KEY(attr->auth_key,
			attr->auth_key_size,
			domain->using_vmdh);
		if (!auth_key)
			return -FI_EINVAL;
	} else {
		auth_key = domain->auth_key;
	}

	if (attr->iov_count == 1)
		return _gnix_mr_reg(fid, attr->mr_iov[0].iov_base,
			attr->mr_iov[0].iov_len, attr->access, attr->offset,
			attr->requested_key, flags, mr, attr->context,
			auth_key, GNIX_USER_REG);

	/* regv limited to one iov at this time */
	return -FI_EOPNOTSUPP;
}

static int __gnix_mr_refresh(struct gnix_fid_mem_desc *desc,
		uint64_t addr, uint64_t len)
{
		gni_return_t grc;


	fastlock_acquire(&desc->nic->lock);
	grc = GNI_MemRegister(desc->nic->gni_nic_hndl, addr,
		len, NULL, GNI_MEM_UPDATE_REGION,
		desc->mr_fid.key, &desc->mem_hndl);
	if (grc != FI_SUCCESS)
		GNIX_WARN(FI_LOG_MR,
			"failed GNI_MemRegister with UPDATE REGION, "
			" addr=%p len=%x key=%d grc=%d\n",
			addr, len, desc->mr_fid.key, grc);
	fastlock_release(&desc->nic->lock);

	return (grc != 0) ? -FI_EINVAL : FI_SUCCESS;
}

static int __gnix_mr_refresh_iov(struct fid *fid, void *arg)
{
	struct fi_mr_modify *modify = (struct fi_mr_modify *) arg;
	int ret = FI_SUCCESS;
	struct gnix_fid_mem_desc *desc;
	uint64_t aligned_addr;
	uint64_t aligned_len;
	uint64_t addr;
	uint64_t len;
	int i;

	GNIX_TRACE(FI_LOG_MR, "\n");

	desc = container_of(fid, struct gnix_fid_mem_desc, mr_fid);

	/* assume that no one is going to attempt to update a MR at the
	 * same time that they might try to deregister a mr
	 *
	 * For the record, that would be REALLY silly application behavior
	 */
	if (!modify->attr.mr_iov || modify->attr.iov_count == 0) {
		GNIX_DEBUG(FI_LOG_DOMAIN,
			"cannot provide null iov or 0 iov_count\n");
		return -FI_EINVAL;
	}

	if (!desc->auth_key->using_vmdh) {
		GNIX_DEBUG(FI_LOG_DOMAIN,
			"cannot use refresh with non-vmdh registrations\n");
		return -FI_EINVAL;
	}

	for (i = 0; i < modify->attr.iov_count; i++) {
		addr = (uint64_t) modify->attr.mr_iov[i].iov_base;
		len = (uint64_t) modify->attr.mr_iov[i].iov_len;

		aligned_addr = addr & ~0xfff;
		aligned_len = addr + len - aligned_addr;
		aligned_len += (((addr + len) & 0xfff) ?
			(0x1000 - ((addr + len) & 0xfff)) : 0);

		ret = __gnix_mr_refresh(desc, aligned_addr, aligned_len);
		if (ret) {
			GNIX_WARN(FI_LOG_DOMAIN,
				"failed to refresh IOV %d, addr=%p len=%x\n",
				i, aligned_addr, aligned_len);
			return ret;
		}
	}

	return FI_SUCCESS;
}

static int fi_gnix_mr_control(struct fid *fid, int command, void *arg)
{
	int ret;
	struct gnix_fid_mem_desc *desc;

	desc = container_of(fid, struct gnix_fid_mem_desc, mr_fid);
	if (desc->mr_fid.fid.fclass != FI_CLASS_MR) {
		GNIX_WARN(FI_LOG_DOMAIN, "invalid fid\n");
		return -FI_EINVAL;
	}

	switch (command) {
	case FI_REFRESH:
		ret = __gnix_mr_refresh_iov(fid, arg);
		break;
	default:
		ret = -FI_EOPNOTSUPP;
		break;
	}

	return ret;
}

/**
 * Closes and deallocates a libfabric memory registration in the internal cache
 *
 * @param[in]  fid  libfabric memory registration fid
 *
 * @return     FI_SUCCESS on success
 *             -FI_EINVAL on invalid fid
 *             -FI_NOENT when there isn't a matching registration for the
 *               provided fid
 *             Otherwise, GNI_RC_* ret codes converted to FI_* err codes
 */
static int fi_gnix_mr_close(fid_t fid)
{
	struct gnix_fid_mem_desc *mr;
	gni_return_t ret;
	struct gnix_fid_domain *domain;
	struct gnix_mr_cache_info *info;
	int requested_key;
	struct gnix_auth_key *auth_key;

	GNIX_TRACE(FI_LOG_MR, "\n");

	if (OFI_UNLIKELY(fid->fclass != FI_CLASS_MR))
		return -FI_EINVAL;

	mr = container_of(fid, struct gnix_fid_mem_desc, mr_fid.fid);

	auth_key = mr->auth_key;
	domain = mr->domain;
	requested_key = fi_mr_key(&mr->mr_fid);
	info = &domain->mr_cache_info[mr->auth_key->ptag];

	/* call cache deregister op */
	fastlock_acquire(&info->mr_cache_lock);
	ret = domain->mr_ops->dereg_mr(domain, mr);
	fastlock_release(&info->mr_cache_lock);

	/* check retcode */
	if (OFI_LIKELY(ret == FI_SUCCESS)) {
		/* release references to the domain and nic */
		_gnix_ref_put(domain);
		if (auth_key->using_vmdh) {
			if (requested_key < auth_key->attr.user_key_limit)
				_gnix_test_and_clear_bit(auth_key->user,
					requested_key);
			else {
				ret = _gnix_release_reserved_key(auth_key,
					requested_key);
				if (ret != FI_SUCCESS) {
					GNIX_WARN(FI_LOG_DOMAIN,
						"failed to release reserved key, "
						"rc=%d key=%d\n",
						ret, requested_key);
				}
			}
		}
	} else {
		GNIX_INFO(FI_LOG_MR, "failed to deregister memory, "
			  "ret=%i\n", ret);
	}

	return ret;
}

static inline void *__gnix_generic_register(
		struct gnix_fid_domain *domain,
		struct gnix_fid_mem_desc *md,
		void *address,
		size_t length,
		gni_cq_handle_t dst_cq_hndl,
		int flags,
		int vmdh_index,
		struct gnix_auth_key *auth_key)
{
	struct gnix_nic *nic;
	struct gnix_nic_attr nic_attr = {0};
	gni_return_t grc = GNI_RC_SUCCESS;
	int rc;
	struct gnix_auth_key *info;

	pthread_mutex_lock(&gnix_nic_list_lock);

	/* If the nic list is empty, create a nic */
	if (OFI_UNLIKELY((dlist_empty(&gnix_nic_list_ptag[auth_key->ptag])))) {
		/* release the lock because we are not checking the list after
			this point. Additionally, gnix_nic_alloc takes the
			lock to add the nic. */
		pthread_mutex_unlock(&gnix_nic_list_lock);
		nic_attr.auth_key = auth_key;

		rc = gnix_nic_alloc(domain, &nic_attr, &nic);
		if (rc) {
			GNIX_INFO(FI_LOG_MR,
				  "could not allocate nic to do mr_reg,"
				  " ret=%i\n", rc);
			return NULL;
		}
	} else {
		nic = dlist_first_entry(&gnix_nic_list_ptag[auth_key->ptag],
			struct gnix_nic, ptag_nic_list);
		if (OFI_UNLIKELY(nic == NULL)) {
			GNIX_ERR(FI_LOG_MR, "Failed to find nic on "
				"ptag list\n");
			pthread_mutex_unlock(&gnix_nic_list_lock);
			return NULL;
		}
		_gnix_ref_get(nic);
		pthread_mutex_unlock(&gnix_nic_list_lock);
	}

	COND_ACQUIRE(nic->requires_lock, &nic->lock);
	if (nic->using_vmdh && !nic->mdd_resources_set) {
		info = auth_key;
		assert(info);

		grc = GNI_SetMddResources(nic->gni_nic_hndl,
				(info->attr.prov_key_limit +
				 info->attr.user_key_limit));
		assert(grc == GNI_RC_SUCCESS);

		nic->mdd_resources_set = 1;
	}

	GNIX_DEBUG(FI_LOG_MR,
		"Params: hndl=%p addr=%p length=%d dst_cq_hndl=%p flags=%08x "
		"vmdh_index=%d mem_hndl=%p md=%p\n",
		nic->gni_nic_hndl, address, length, dst_cq_hndl, flags,
		vmdh_index, &md->mem_hndl, md);

	grc = GNI_MemRegister(nic->gni_nic_hndl, (uint64_t) address,
				  length,	dst_cq_hndl, flags,
				  vmdh_index, &md->mem_hndl);
	COND_RELEASE(nic->requires_lock, &nic->lock);

	if (OFI_UNLIKELY(grc != GNI_RC_SUCCESS)) {
		GNIX_INFO(FI_LOG_MR, "failed to register memory with uGNI, "
			  "ret=%s\n", gni_err_str[grc]);
		_gnix_ref_put(nic);

		return NULL;
	}

	/* set up the mem desc */
	md->nic = nic;
	md->domain = domain;

	/* take references on domain */
	_gnix_ref_get(md->domain);

	return md;
}

static void *__gnix_register_region(
		void *handle,
		void *address,
		size_t length,
		struct _gnix_fi_reg_context *fi_reg_context,
		void *context)
{
	struct gnix_fid_mem_desc *md = (struct gnix_fid_mem_desc *) handle;
	struct gnix_fid_domain *domain = context;
	gni_cq_handle_t dst_cq_hndl = NULL;
	int flags = 0;
	int vmdh_index = -1;

	/* If network would be able to write to this buffer, use read-write */
	if (fi_reg_context->access & (FI_RECV | FI_READ | FI_REMOTE_WRITE))
		flags |= GNI_MEM_READWRITE;
	else
		flags |= GNI_MEM_READ_ONLY;

	if (domain->using_vmdh) {
		flags |= GNI_MEM_USE_VMDH | GNI_MEM_RESERVE_REGION;
		vmdh_index = fi_reg_context->requested_key;
	}

	GNIX_DEBUG(FI_LOG_MR, "addr %p len %d flags 0x%x\n", address, length,
		   flags);
	return __gnix_generic_register(domain, md, address, length, dst_cq_hndl,
			flags, vmdh_index, fi_reg_context->auth_key);
}

static int __gnix_deregister_region(
		void *handle,
		void *context)
{
	struct gnix_fid_mem_desc *mr = (struct gnix_fid_mem_desc *) handle;
	gni_return_t ret = GNI_RC_SUCCESS;
	struct gnix_fid_domain *domain;
	struct gnix_nic *nic;

	domain = mr->domain;
	nic = mr->nic;

	GNIX_DEBUG(FI_LOG_MR,
		"Params: deregister md=%p\n", handle);
	COND_ACQUIRE(nic->requires_lock, &nic->lock);
	ret = GNI_MemDeregister(nic->gni_nic_hndl, &mr->mem_hndl);
	COND_RELEASE(nic->requires_lock, &nic->lock);
	if (ret == GNI_RC_SUCCESS) {
		/* release reference to domain */
		_gnix_ref_put(domain);

		/* release reference to nic */
		_gnix_ref_put(nic);
	} else {
		GNIX_INFO(FI_LOG_MR, "failed to deregister memory"
			  " region, entry=%p ret=%i\n", handle, ret);
	}

	return ret;
}

/**
 * Associates a registered memory region with a completion counter.
 *
 * @param[in] fid		the memory region
 * @param[in] bfid		the fabric identifier for the memory region
 * @param[in] flags		flags to apply to the registration
 *
 * @return FI_SUCCESS		Upon successfully registering the memory region
 * @return -FI_ENOSYS		If binding of the memory region is not supported
 * @return -FI_EBADFLAGS	If the flags are not supported
 * @return -FI_EKEYREJECTED	If the key is not available
 * @return -FI_ENOKEY		If the key is already in use
 */
DIRECT_FN int gnix_mr_bind(fid_t fid, struct fid *bfid, uint64_t flags)
{
	return -FI_ENOSYS;
}

static int __gnix_destruct_registration(void *context)
{
	return GNI_RC_SUCCESS;
}

#ifdef HAVE_UDREG
void *__udreg_register(void *addr, uint64_t length, void *context)
{
	struct gnix_fid_mem_desc *md;
	struct gnix_mr_cache_info *info = (struct gnix_mr_cache_info *) context;
	struct gnix_auth_key *auth_key = info->auth_key;
	struct gnix_fid_domain *domain = info->domain;

	/* Allocate an udreg info block for this registration. */
	md = calloc(1, sizeof(*md));
	if (!md) {
		GNIX_WARN(FI_LOG_MR,
			"failed to allocate memory for registration\n");
		return NULL;
	}

	GNIX_INFO(FI_LOG_MR, "info=%p auth_key=%p\n",
		info, auth_key);
	GNIX_INFO(FI_LOG_MR, "ptag=%d\n", auth_key->ptag);

	return __gnix_generic_register(domain, md, addr, length, NULL,
		GNI_MEM_READWRITE, -1, auth_key);
}

uint32_t __udreg_deregister(void *registration, void *context)
{
	gni_return_t grc;

	grc = __gnix_deregister_region(registration, NULL);

	free(registration);

	return (grc == GNI_RC_SUCCESS) ? 0 : 1;
}

/* Called via dreg when a cache is destroyed. */
void __udreg_cache_destructor(void *context)
{
	/*  Nothing needed here. */
}

static int __udreg_init(struct gnix_fid_domain *domain,
		struct gnix_auth_key *auth_key)
{
	int ret = FI_SUCCESS;
	udreg_return_t urc;
	struct gnix_mr_cache_info *info =
		GNIX_GET_MR_CACHE_INFO(domain, auth_key);

	udreg_cache_attr_t udreg_cache_attr = {
		.cache_name =           {"gnix_app_cache"},
		.max_entries =          domain->udreg_reg_limit,
		.modes =                UDREG_CC_MODE_USE_LARGE_PAGES,
		.debug_mode =           0,
		.debug_rank =           0,
		.reg_context =          (void *) info,
		.dreg_context =         (void *) domain,
		.destructor_context =   (void *) domain,
		.device_reg_func =      __udreg_register,
		.device_dereg_func =    __udreg_deregister,
		.destructor_callback =  __udreg_cache_destructor,
	};

	if (domain->mr_cache_attr.lazy_deregistration)
		udreg_cache_attr.modes |= (UDREG_CC_MODE_USE_LAZY_DEREG | UDREG_CC_MODE_USE_KERNEL_CACHE);

	/*
	 * Create a udreg cache for application memory registrations.
	 */
	urc = UDREG_CacheCreate(&udreg_cache_attr);
	if (urc != UDREG_RC_SUCCESS) {
		GNIX_WARN(FI_LOG_MR,
				"Could not initialize udreg application cache, urc=%d\n",
				urc);
		switch (urc) {
		case UDREG_RC_INVALID_PARAM:
			ret = -FI_EINVAL;
			goto err;
			break;
		case UDREG_RC_ERROR_NO_DEVICE:
			ret = -FI_ENODEV;
			goto err;
			break;
		case UDREG_RC_NO_SPACE:
			ret = -FI_ENOSPC;
			goto err;
			break;
		default:
			ret = -FI_EINVAL;
			goto err;
			break;
		}
	}

	urc = UDREG_CacheAccess(udreg_cache_attr.cache_name,
		&info->udreg_cache);
	if (urc != UDREG_RC_SUCCESS) {
		GNIX_WARN(FI_LOG_MR,
				"Could not access udreg application cache, urc=%d",
				urc);
		switch (urc) {
		case UDREG_RC_INVALID_PARAM:
			ret = -FI_EINVAL;
			goto err;
			break;
		case UDREG_RC_NO_MATCH:
			ret = -FI_ENODEV;
			goto err;
			break;
		default:
			ret = -FI_EINVAL;
			goto err;
			break;
		}
	}

	info->inuse = 1;
	info->auth_key = auth_key;
	GNIX_INFO(FI_LOG_MR, "info=%p auth_key=%p ptag=%d\n",
		info, info->auth_key, auth_key->ptag);

err:
	return ret;
}

static int __udreg_is_init(struct gnix_fid_domain *domain,
		struct gnix_auth_key *auth_key)
{
	struct gnix_mr_cache_info *info =
		GNIX_GET_MR_CACHE_INFO(domain, auth_key);

	return info->inuse;
}

static int __udreg_reg_mr(
		struct gnix_fid_domain     *domain,
		uint64_t                    address,
		uint64_t                    length,
		struct _gnix_fi_reg_context *fi_reg_context,
		void                        **handle) {

	udreg_return_t urc;
	udreg_entry_t *udreg_entry;
	struct gnix_fid_mem_desc *md;
	struct gnix_mr_cache_info *info =
		GNIX_GET_MR_CACHE_INFO(domain,
			fi_reg_context->auth_key);

	urc = UDREG_Register(info->udreg_cache, (void *) address,
			length, &udreg_entry);
	if (OFI_UNLIKELY(urc != UDREG_RC_SUCCESS))
		return -FI_EIO;

	md = udreg_entry->device_data;
	md->entry = udreg_entry;

	*handle = md;

	return FI_SUCCESS;
}

static int __udreg_dereg_mr(struct gnix_fid_domain *domain,
		struct gnix_fid_mem_desc *md)
{
	udreg_return_t urc;
	struct gnix_mr_cache_info *info =
		GNIX_GET_MR_CACHE_INFO(domain,
			md->auth_key);

	urc = UDREG_Unregister(info->udreg_cache,
			(udreg_entry_t *) md->entry);
	if (urc != UDREG_RC_SUCCESS) {
		GNIX_WARN(FI_LOG_MR, "UDREG_Unregister() returned %d\n", urc);
		return -FI_ENOENT;
	}

	return urc;
}

static int __udreg_close(struct gnix_fid_domain *domain,
		struct gnix_mr_cache_info *info)
{
	udreg_return_t ret;

	if (!info->inuse)
		return FI_SUCCESS; /* nothing to close */

	if (info->udreg_cache) {
		ret = UDREG_CacheRelease(info->udreg_cache);
		if (OFI_UNLIKELY(ret != UDREG_RC_SUCCESS))
			GNIX_FATAL(FI_LOG_DOMAIN, "failed to release from "
					"mr cache during domain destruct, dom=%p rc=%d\n",
					domain, ret);

		ret = UDREG_CacheDestroy(info->udreg_cache);
		if (OFI_UNLIKELY(ret != UDREG_RC_SUCCESS))
			GNIX_FATAL(FI_LOG_DOMAIN, "failed to destroy mr "
					"cache during domain destruct, dom=%p rc=%d\n",
					domain, ret);
	}

	info->inuse = 0;

	return FI_SUCCESS;
}
#else
static int __udreg_init(struct gnix_fid_domain *domain,
		struct gnix_auth_key *auth_key)
{
	return -FI_ENOSYS;
}

static int __udreg_is_init(struct gnix_fid_domain *domain
		struct gnix_auth_key *auth_key)
{
	return FI_SUCCESS;
}

static int __udreg_reg_mr(struct gnix_fid_domain *domain,
		uint64_t                    address,
		uint64_t                    length,
		struct _gnix_fi_reg_context *fi_reg_context,
		void                        **handle) {

	return -FI_ENOSYS;
}

static int __udreg_dereg_mr(struct gnix_fid_domain *domain,
		struct gnix_fid_mem_desc *md)
{
	return -FI_ENOSYS;
}

static int __udreg_close(struct gnix_fid_domain *domain,
		struct gnix_mr_cache_info *info)
{
	return FI_SUCCESS;
}
#endif

struct gnix_mr_ops udreg_mr_ops = {
	.init = __udreg_init,
	.is_init = __udreg_is_init,
	.reg_mr = __udreg_reg_mr,
	.dereg_mr = __udreg_dereg_mr,
	.destroy_cache = __udreg_close,
	.flush_cache = NULL, /* UDREG doesn't support cache flush */
};

static int __cache_init(struct gnix_fid_domain *domain,
		struct gnix_auth_key *auth_key)
{
	int ret;
	struct gnix_mr_cache_info *info =
		GNIX_GET_MR_CACHE_INFO(domain, auth_key);

#if !HAVE_KDREG
	domain->mr_cache_attr.lazy_deregistration = 0;
#endif

	ret = _gnix_mr_cache_init(&info->mr_cache_ro,
			&domain->mr_cache_attr);

	if (ret)
		return ret;

	ret = _gnix_mr_cache_init(&info->mr_cache_rw,
				&domain->mr_cache_attr);

	if (ret == FI_SUCCESS)
		info->inuse = 1;

	return ret;
}

static int __cache_is_init(struct gnix_fid_domain *domain,
		struct gnix_auth_key *auth_key)
{
	struct gnix_mr_cache_info *info =
		GNIX_GET_MR_CACHE_INFO(domain, auth_key);

	return info->inuse;
}

static int __cache_reg_mr(
		struct gnix_fid_domain      *domain,
		uint64_t                    address,
		uint64_t                    length,
		struct _gnix_fi_reg_context *fi_reg_context,
		void                        **handle)
{
	struct gnix_mr_cache *cache;
	struct gnix_auth_key *auth_key = fi_reg_context->auth_key;
	struct gnix_mr_cache_info *info =
		GNIX_GET_MR_CACHE_INFO(domain, auth_key);

	if (fi_reg_context->access & (FI_RECV | FI_READ | FI_REMOTE_WRITE))
		cache = info->mr_cache_rw;
	else
		cache = info->mr_cache_ro;

	return _gnix_mr_cache_register(cache, address, length,
			fi_reg_context, handle);
}

static int __cache_dereg_mr(struct gnix_fid_domain *domain,
		struct gnix_fid_mem_desc *md)
{
	gnix_mr_cache_t *cache;
	struct gnix_mr_cache_info *info = GNIX_GET_MR_CACHE_INFO(domain,
			md->auth_key);

	if (GNI_MEMHNDL_GET_FLAGS((md->mem_hndl)) &
	    GNI_MEMHNDL_FLAG_READONLY)
		cache = info->mr_cache_ro;
	else
		cache = info->mr_cache_rw;

	return _gnix_mr_cache_deregister(cache, md);
}

static int __cache_close(struct gnix_fid_domain *domain,
		struct gnix_mr_cache_info *info)
{
	int ret;

	if (!info->inuse)
		return FI_SUCCESS;

	if (info->mr_cache_ro) {
		ret = _gnix_mr_cache_destroy(info->mr_cache_ro);
		if (ret != FI_SUCCESS)
			GNIX_FATAL(FI_LOG_DOMAIN, "failed to destroy ro mr "
					"cache dom=%p ret=%d\n",
					domain, ret);
	}

	if (info->mr_cache_rw) {
		ret = _gnix_mr_cache_destroy(info->mr_cache_rw);
		if (ret != FI_SUCCESS)
			GNIX_FATAL(FI_LOG_DOMAIN, "failed to destroy rw mr "
					"cache dom=%p ret=%d\n",
					domain, ret);
	}

	info->inuse = 0;
	return FI_SUCCESS;
}

static int __cache_flush(struct gnix_fid_domain *domain)
{
	int ret = FI_SUCCESS;
	int i;
	struct gnix_mr_cache_info *info;

	fastlock_acquire(&domain->mr_cache_lock);

	for (i = 0; i < 256; i++) {
		info = &domain->mr_cache_info[i];

		fastlock_acquire(&info->mr_cache_lock);
		if (!info->inuse) {
			fastlock_release(&info->mr_cache_lock);
			continue;
		}

		ret = _gnix_mr_cache_flush(info->mr_cache_ro);
		if (ret) {
			fastlock_release(&info->mr_cache_lock);
			break;
		}

		ret = _gnix_mr_cache_flush(info->mr_cache_rw);
		if (ret) {
			fastlock_release(&info->mr_cache_lock);
			break;
		}

		fastlock_release(&info->mr_cache_lock);
	}

	fastlock_release(&domain->mr_cache_lock);

	return ret;
}

struct gnix_mr_ops cache_mr_ops = {
	.init = __cache_init,
	.is_init = __cache_is_init,
	.reg_mr = __cache_reg_mr,
	.dereg_mr = __cache_dereg_mr,
	.destroy_cache = __cache_close,
	.flush_cache = __cache_flush,
};

static int __basic_mr_init(struct gnix_fid_domain *domain,
		struct gnix_auth_key *auth_key)
{
	struct gnix_mr_cache_info *info = domain->mr_cache_info;

	info->inuse = 1;
	return FI_SUCCESS;
}

static int __basic_mr_is_init(struct gnix_fid_domain *domain,
		struct gnix_auth_key *auth_key)
{
	struct gnix_mr_cache_info *info = domain->mr_cache_info;

	return info->inuse;
}

static int __basic_mr_reg_mr(
		struct gnix_fid_domain      *domain,
		uint64_t                    address,
		uint64_t                    length,
		struct _gnix_fi_reg_context *fi_reg_context,
		void                        **handle)
{
	struct gnix_fid_mem_desc *md, *ret;

	md = calloc(1, sizeof(*md));
	if (!md) {
		GNIX_WARN(FI_LOG_MR, "failed to allocate memory");
		return -FI_ENOMEM;
	}

	ret = __gnix_register_region((void *) md, (void *) address, length,
			fi_reg_context, (void *) domain);
	if (!ret) {
		GNIX_WARN(FI_LOG_MR, "failed to register memory");
		free(md);
		return -FI_ENOSPC;
	}

	*handle = (void *) md;

	return FI_SUCCESS;
}

static int __basic_mr_dereg_mr(struct gnix_fid_domain *domain,
		struct gnix_fid_mem_desc *md)
{
	int ret;

	ret = __gnix_deregister_region((void *) md, NULL);
	if (ret == FI_SUCCESS)
		free((void *) md);

	return ret;
}

struct gnix_mr_ops basic_mr_ops = {
	.init = __basic_mr_init,
	.is_init = __basic_mr_is_init,
	.reg_mr = __basic_mr_reg_mr,
	.dereg_mr = __basic_mr_dereg_mr,
	.flush_cache = NULL, /* unsupported since there is no caching here */
};

int _gnix_open_cache(struct gnix_fid_domain *domain, int type)
{
	if (type < 0 || type >= GNIX_MR_MAX_TYPE)
		return -FI_EINVAL;

	if (domain->mr_ops && domain->mr_ops->is_init(domain, domain->auth_key))
		return -FI_EBUSY;

	switch(type) {
	case GNIX_MR_TYPE_UDREG:
		domain->mr_ops = &udreg_mr_ops;
		break;
	case GNIX_MR_TYPE_NONE:
		domain->mr_ops = &basic_mr_ops;
		break;
	default:
		domain->mr_ops = &cache_mr_ops;
		break;
	}

	domain->mr_cache_type = type;

	return FI_SUCCESS;
}

int _gnix_flush_registration_cache(struct gnix_fid_domain *domain)
{
	if (domain->mr_ops && domain->mr_ops->flush_cache)
		return domain->mr_ops->flush_cache(domain);

	return FI_SUCCESS;  /* if no flush was present, silently pass */
}

int _gnix_close_cache(struct gnix_fid_domain *domain,
		struct gnix_mr_cache_info *info)
{
	/* if the domain isn't being destructed by close, we need to check the
	 * cache again. This isn't a likely case. Destroy must succeed since we
	 * are in the destruct path */
	if (domain->mr_ops && domain->mr_ops->destroy_cache)
		return domain->mr_ops->destroy_cache(domain, info);

	return FI_SUCCESS;
}

gnix_mr_cache_attr_t _gnix_default_mr_cache_attr = {
		.soft_reg_limit      = 4096,
		.hard_reg_limit      = -1,
		.hard_stale_limit    = 128,
#if HAVE_KDREG
		.lazy_deregistration = 1,
#else
		.lazy_deregistration = 0,
#endif
		.reg_callback        = __gnix_register_region,
		.dereg_callback      = __gnix_deregister_region,
		.destruct_callback   = __gnix_destruct_registration,
		.elem_size           = sizeof(struct gnix_fid_mem_desc),
};
