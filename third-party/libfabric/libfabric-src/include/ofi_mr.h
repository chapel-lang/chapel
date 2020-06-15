/*
 * Copyright (c) 2017-2019 Intel Corporation, Inc. All rights reserved.
 * Copyright (c) 2019 Amazon.com, Inc. or its affiliates. All rights reserved.
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

#ifndef _OFI_MR_H_
#define _OFI_MR_H_

#if HAVE_CONFIG_H
#  include <config.h>
#endif /* HAVE_CONFIG_H */

#include <inttypes.h>
#include <stdbool.h>

#include <ofi.h>
#include <ofi_atom.h>
#include <ofi_lock.h>
#include <ofi_list.h>
#include <ofi_tree.h>

struct ofi_mr_info {
	struct iovec iov;
};


#define OFI_MR_BASIC_MAP (FI_MR_ALLOCATED | FI_MR_PROV_KEY | FI_MR_VIRT_ADDR)

/* FI_LOCAL_MR is valid in pre-libfaric-1.5 and can be valid in
 * post-libfabric-1.5 */
static inline int ofi_mr_local(const struct fi_info *info)
{
	if (!info)
		return 0;

	if (!info->domain_attr)
		goto check_local_mr;

	if (info->domain_attr->mr_mode & FI_MR_LOCAL)
		return 1;

	if (info->domain_attr->mr_mode & ~(FI_MR_BASIC | FI_MR_SCALABLE))
		return 0;

check_local_mr:
	return (info->mode & FI_LOCAL_MR) ? 1 : 0;
}

#define OFI_MR_MODE_RMA_TARGET (FI_MR_RAW | FI_MR_VIRT_ADDR |			\
				 FI_MR_PROV_KEY | FI_MR_RMA_EVENT)

/* If the app sets FI_MR_LOCAL, we ignore FI_LOCAL_MR.  So, if the
 * app doesn't set FI_MR_LOCAL, we need to check for FI_LOCAL_MR.
 * The provider is assumed only to set FI_MR_LOCAL correctly.
 */
static inline uint64_t ofi_mr_get_prov_mode(uint32_t version,
					    const struct fi_info *user_info,
					    const struct fi_info *prov_info)
{
	if (FI_VERSION_LT(version, FI_VERSION(1, 5)) ||
	    (user_info->domain_attr &&
	     !(user_info->domain_attr->mr_mode & FI_MR_LOCAL))) {
		return (prov_info->domain_attr->mr_mode & FI_MR_LOCAL) ?
			prov_info->mode | FI_LOCAL_MR : prov_info->mode;
	} else {
		return prov_info->mode;
	}
}


/*
 * Memory notifier - Report memory mapping changes to address ranges
 */

struct ofi_mr_cache;

struct ofi_mem_monitor {
	pthread_mutex_t 		lock;
	struct dlist_entry		list;

	int (*subscribe)(struct ofi_mem_monitor *notifier,
			 const void *addr, size_t len);
	void (*unsubscribe)(struct ofi_mem_monitor *notifier,
			    const void *addr, size_t len);
};

void ofi_monitor_init(void);
void ofi_monitor_cleanup(void);
int ofi_monitor_add_cache(struct ofi_mem_monitor *monitor,
			   struct ofi_mr_cache *cache);
void ofi_monitor_del_cache(struct ofi_mr_cache *cache);
void ofi_monitor_notify(struct ofi_mem_monitor *monitor,
			const void *addr, size_t len);

int ofi_monitor_subscribe(struct ofi_mem_monitor *monitor,
			  const void *addr, size_t len);
void ofi_monitor_unsubscribe(struct ofi_mem_monitor *monitor,
			     const void *addr, size_t len);

extern struct ofi_mem_monitor *default_monitor;

/*
 * Userfault fd memory monitor
 */
struct ofi_uffd {
	struct ofi_mem_monitor		monitor;
	pthread_t			thread;
	int				fd;
};

int ofi_uffd_init(void);
void ofi_uffd_cleanup(void);

extern struct ofi_mem_monitor *uffd_monitor;

/*
 * Memory intercept call memory monitor
 */
struct ofi_memhooks {
	struct ofi_mem_monitor          monitor;
	struct dlist_entry		intercept_list;
};

int ofi_memhooks_init(void);
void ofi_memhooks_cleanup(void);

extern struct ofi_mem_monitor *memhooks_monitor;


/*
 * Used to store registered memory regions into a lookup map.  This
 * is used by the ofi_mr_xxx calls below, and may be accessed by a
 * provider when processing incoming RMA operations to verify that
 * a region has been registered for the specified operation.
 */

struct ofi_mr_map {
	const struct fi_provider *prov;
	struct ofi_rbmap	*rbtree;
	uint64_t		key;
	int			mode;
};

int ofi_mr_map_init(const struct fi_provider *in_prov, int mode,
		    struct ofi_mr_map *map);
void ofi_mr_map_close(struct ofi_mr_map *map);

int ofi_mr_map_insert(struct ofi_mr_map *map,
		      const struct fi_mr_attr *attr,
		      uint64_t *key, void *context);
int ofi_mr_map_remove(struct ofi_mr_map *map, uint64_t key);
void *ofi_mr_map_get(struct ofi_mr_map *map,  uint64_t key);

int ofi_mr_map_verify(struct ofi_mr_map *map, uintptr_t *io_addr,
		      size_t len, uint64_t key, uint64_t access,
		      void **context);

/*
 * These calls may be used be providers to implement software memory
 * registration.
 */

struct ofi_mr {
	struct fid_mr mr_fid;
	struct util_domain *domain;
	uint64_t key;
	uint64_t flags;
};

int ofi_mr_close(struct fid *fid);
int ofi_mr_regattr(struct fid *fid, const struct fi_mr_attr *attr,
		   uint64_t flags, struct fid_mr **mr_fid);
int ofi_mr_regv(struct fid *fid, const struct iovec *iov,
		size_t count, uint64_t access, uint64_t offset,
		uint64_t requested_key, uint64_t flags,
		struct fid_mr **mr_fid, void *context);
int ofi_mr_reg(struct fid *fid, const void *buf, size_t len,
	       uint64_t access, uint64_t offset, uint64_t requested_key,
	       uint64_t flags, struct fid_mr **mr_fid, void *context);
int ofi_mr_verify(struct ofi_mr_map *map, ssize_t len,
		  uintptr_t *addr, uint64_t key, uint64_t access);

/*
 * Memory registration cache
 */

struct ofi_mr_cache_params {
	size_t				max_cnt;
	size_t				max_size;
	char *				monitor;
};

extern struct ofi_mr_cache_params	cache_params;

struct ofi_mr_entry {
	struct ofi_mr_info		info;
	void				*storage_context;
	unsigned int			subscribed:1;
	int				use_cnt;
	struct dlist_entry		list_entry;
	uint8_t				data[];
};

enum ofi_mr_storage_type {
	OFI_MR_STORAGE_DEFAULT = 0,
	OFI_MR_STORAGE_RBT,
	OFI_MR_STORAGE_USER,
};

struct ofi_mr_storage {
	enum ofi_mr_storage_type	type;
	void				*storage;

	struct ofi_mr_entry *		(*find)(struct ofi_mr_storage *storage,
						const struct ofi_mr_info *key);
	struct ofi_mr_entry *		(*overlap)(struct ofi_mr_storage *storage,
						const struct iovec *key);
	int				(*insert)(struct ofi_mr_storage *storage,
						struct ofi_mr_info *key,
						struct ofi_mr_entry *entry);
	int				(*erase)(struct ofi_mr_storage *storage,
						struct ofi_mr_entry *entry);
	void				(*destroy)(struct ofi_mr_storage *storage);
};

struct ofi_mr_cache {
	struct util_domain		*domain;
	struct ofi_mem_monitor		*monitor;
	struct dlist_entry		notify_entry;
	size_t				entry_data_size;

	struct ofi_mr_storage		storage;
	struct dlist_entry		lru_list;
	struct dlist_entry		flush_list;
	pthread_mutex_t 		lock;

	size_t				cached_cnt;
	size_t				cached_size;
	size_t				uncached_cnt;
	size_t				uncached_size;
	size_t				search_cnt;
	size_t				delete_cnt;
	size_t				hit_cnt;
	size_t				notify_cnt;
	struct ofi_bufpool		*entry_pool;

	int				(*add_region)(struct ofi_mr_cache *cache,
						      struct ofi_mr_entry *entry);
	void				(*delete_region)(struct ofi_mr_cache *cache,
							 struct ofi_mr_entry *entry);
};

int ofi_mr_cache_init(struct util_domain *domain, struct ofi_mem_monitor *monitor,
		      struct ofi_mr_cache *cache);
void ofi_mr_cache_cleanup(struct ofi_mr_cache *cache);

void ofi_mr_cache_notify(struct ofi_mr_cache *cache, const void *addr, size_t len);

bool ofi_mr_cache_flush(struct ofi_mr_cache *cache);
int ofi_mr_cache_search(struct ofi_mr_cache *cache, const struct fi_mr_attr *attr,
			struct ofi_mr_entry **entry);
/**
 * Given an attr (with an iov range), if the iov range is already registered,
 * return the corresponding ofi_mr_entry. Otherwise, return NULL.
 * The caller must call ofi_mr_cache_delete on the entry before cleanup if
 * the returned entry is not NULL.
 *
 * @param[in]	cache		The cache the entry belongs to
 * @param[in]	attr		Information about the region to search
 *
 * @returns	entry		The registered entry corresponding to the
 *				region described in attr
 * @returns	NULL		The region described in attr is not registered
 *				with the cache.
 */
struct ofi_mr_entry *ofi_mr_cache_find(struct ofi_mr_cache *cache,
				       const struct fi_mr_attr *attr);
int ofi_mr_cache_reg(struct ofi_mr_cache *cache, const struct fi_mr_attr *attr,
		     struct ofi_mr_entry **entry);
void ofi_mr_cache_delete(struct ofi_mr_cache *cache, struct ofi_mr_entry *entry);


#endif /* _OFI_MR_H_ */
