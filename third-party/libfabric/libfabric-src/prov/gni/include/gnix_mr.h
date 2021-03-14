/*
 * Copyright (c) 2015-2017 Cray Inc. All rights reserved.
 * Copyright (c) 2015 Los Alamos National Security, LLC. All rights reserved.
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

#ifndef GNIX_MR_H_
#define GNIX_MR_H_

#ifdef HAVE_UDREG
#include <udreg_pub.h>
#endif

/* global includes */
#include "rdma/fi_domain.h"

/* provider includes */
#include "gnix_priv.h"
#include "gnix_mr_cache.h"

#define GNIX_USER_REG 0
#define GNIX_PROV_REG 1

#define GNIX_MR_PAGE_SHIFT 12
#define GNIX_MR_PFN_BITS 37
#define GNIX_MR_MDD_BITS 12
#define GNIX_MR_FMT_BITS 1
#define GNIX_MR_FLAG_BITS 2
#define GNIX_MR_VA_BITS (GNIX_MR_PFN_BITS + GNIX_MR_PAGE_SHIFT)
#define GNIX_MR_KEY_BITS (GNIX_MR_PFN_BITS + GNIX_MR_MDD_BITS)
#define GNIX_MR_RESERVED_BITS \
	(GNIX_MR_KEY_BITS + GNIX_MR_FLAG_BITS + GNIX_MR_FMT_BITS)
#define GNIX_MR_PADDING_LENGTH (64 - GNIX_MR_RESERVED_BITS)

/* TODO: optimize to save space by using a union to combine the two
 * independent sets of data
 */
struct gnix_mr_cache_info {
	/* used only with internal mr cache */
	gnix_mr_cache_t *mr_cache_rw;
	gnix_mr_cache_t *mr_cache_ro;

	/* used only with udreg */
	struct udreg_cache *udreg_cache;
	struct gnix_fid_domain *domain;
	struct gnix_auth_key *auth_key;

	fastlock_t mr_cache_lock;
	int inuse;
};

enum {
	GNIX_MR_FLAG_READONLY = 1 << 0,
	GNIX_MR_FLAG_BASIC_REG = 1 << 1,
};

enum {
	GNIX_MR_TYPE_INTERNAL = 0,
	GNIX_MR_TYPE_UDREG,
	GNIX_MR_TYPE_NONE,
	GNIX_MR_MAX_TYPE,
};

#define GNIX_DEFAULT_CACHE_TYPE GNIX_MR_TYPE_INTERNAL

/* forward declarations */
struct gnix_fid_domain;
struct gnix_nic;

/**
 * @brief gnix memory descriptor object for use with fi_mr_reg
 *
 * @var   mr_fid    libfabric memory region descriptor
 * @var   domain    gnix domain associated with this memory region
 * @var   mem_hndl  gni memory handle for the memory region
 * @var   nic       gnix nic associated with this memory region
 * @var   key       gnix memory cache key associated with this memory region
 */
struct gnix_fid_mem_desc {
	struct fid_mr mr_fid;
	struct gnix_fid_domain *domain;
	gni_mem_handle_t mem_hndl;
	struct gnix_nic *nic;
	struct gnix_auth_key *auth_key;
#ifdef HAVE_UDREG
	udreg_entry_t *entry;
#endif
};

/**
 * @brief gnix memory region key
 *
 * @var   pfn      prefix of the virtual address
 * @var   mdd      index for the mdd
 * @var   format   flag for determining whether new mdd format is used
 * @var   flags    set of bits for passing flags such as read-only
 * @var   padding  reserved bits, unused for now
 */
typedef struct gnix_mr_key {
	union {
		struct {
			uint64_t pfn: GNIX_MR_PFN_BITS;
			uint64_t mdd: GNIX_MR_MDD_BITS;
			uint64_t format : GNIX_MR_FMT_BITS;
			uint64_t flags : GNIX_MR_FLAG_BITS;
			uint64_t padding: GNIX_MR_PADDING_LENGTH;
		};
		uint64_t value;
	};
} gnix_mr_key_t;

/**
 *
 */
struct gnix_mr_ops {
	int (*init)(struct gnix_fid_domain *domain,
			struct gnix_auth_key *auth_key);
	int (*is_init)(struct gnix_fid_domain *domain,
			struct gnix_auth_key *auth_key);
	int (*reg_mr)(struct gnix_fid_domain *domain, uint64_t address,
			uint64_t length, struct _gnix_fi_reg_context *fi_reg_context,
			void **handle);
	int (*dereg_mr)(struct gnix_fid_domain *domain,
			struct gnix_fid_mem_desc *md);
	int (*destroy_cache)(struct gnix_fid_domain *domain,
			struct gnix_mr_cache_info *info);
	int (*flush_cache)(struct gnix_fid_domain *domain);
};


/**
 * @brief Converts a libfabric key to a gni memory handle, skipping memory
 *        handle CRC generation.
 *
 * @param[in]     key   libfabric memory region key
 * @param[in,out] mhdl  gni memory handle
 */
void _gnix_convert_key_to_mhdl_no_crc(
		gnix_mr_key_t    *key,
		gni_mem_handle_t *mhdl);

/**
 * @brief Converts a libfabric key to a gni memory handle
 *
 * @param[in]     key   libfabric memory region key
 * @param[in,out] mhdl  gni memory handle
 */
void _gnix_convert_key_to_mhdl(
		gnix_mr_key_t    *key,
		gni_mem_handle_t *mhdl);

#define _GNIX_CONVERT_MR_KEY(scalable, offset, convert_func, key, mhdl) \
	do { \
		if (scalable) { \
			gnix_mr_key_t _gnix_mr_key = { \
				.value = ((gnix_mr_key_t *) (key))->value + (offset), \
			}; \
			convert_func(&_gnix_mr_key, (mhdl)); \
		} else { \
			convert_func((gnix_mr_key_t *) (key), (mhdl)); \
		} \
	} while (0)

/**
 * @brief Converts a gni memory handle to a libfabric key
 *
 * @param[in]     mhdl  gni memory handle
 * @return              fi_mr_key to be used by remote EPs.
 */
uint64_t _gnix_convert_mhdl_to_key(gni_mem_handle_t *mhdl);

/* initializes mr cache for a given domain */
int _gnix_open_cache(struct gnix_fid_domain *domain, int type);

/* destroys mr cache for a given domain */
int _gnix_close_cache(struct gnix_fid_domain *domain,
	struct gnix_mr_cache_info *info);

/* flushes the memory registration cache for a given domain */
int _gnix_flush_registration_cache(struct gnix_fid_domain *domain);


/** 
 * used for internal registrations,
 *
 * @param fid  endpoint fid
 * @param buf            buffer to register
 * @param len            length of buffer to register
 * @param access         access permissions
 * @param offset         registration offset 
 * @param requested_key  key requested for new registration
 * @param flags          registration flags
 * @param mr_o           pointer to returned registration
 * @param context        context to associate with registration
 * @param auth_key       authorization key to associate with registration
 * @param reserved       1 if provider registration, 0 otherwise
 *
 * @note  Set reserved to 0 for a user registration
 * @note  Set reserved to 1 for a provider registration 
 */
int _gnix_mr_reg(struct fid *fid, const void *buf, size_t len,
			  uint64_t access, uint64_t offset,
			  uint64_t requested_key, uint64_t flags,
			  struct fid_mr **mr_o, void *context,
			  struct gnix_auth_key *auth_key,
			  int reserved);

extern gnix_mr_cache_attr_t _gnix_default_mr_cache_attr;

#endif /* GNIX_MR_H_ */
