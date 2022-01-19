/*
 * Copyright (c) 2017-2019 Intel Corporation, Inc. All rights reserved.
 * Copyright (c) 2019-2021 Amazon.com, Inc. or its affiliates.
 *                         All rights reserved.
 * (C) Copyright 2020 Hewlett Packard Enterprise Development LP
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
#include <ofi_hmem.h>


int ofi_open_mr_cache(uint32_t version, void *attr, size_t attr_len,
		      uint64_t flags, struct fid **fid, void *context);

struct ofi_mr_info {
	struct iovec iov;
	enum fi_hmem_iface iface;
	uint64_t device;
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


/* Single lock used by all memory monitors and MR caches. */
extern pthread_mutex_t mm_lock;
/* The read-write lock is an additional lock used to protect the dlist_entry
 * list of ofi_mem_monitor. Due to the necessity of releasing the mm_lock
 * while walking the dlist in ofi_monitor_notify, we need a separate lock to
 * ensure thread safety. This must be a read-write lock because
 * ofi_monitor_notify may be recursive and cannot block multiple walks from
 * occurring at the same time.
 */
extern pthread_rwlock_t mm_list_rwlock;

/*
 * Memory notifier - Report memory mapping changes to address ranges
 */

struct ofi_mr_cache;

union ofi_mr_hmem_info {
	uint64_t cuda_id;
	uint64_t ze_id;
};

enum fi_mm_state {
	FI_MM_STATE_UNSPEC = 0,
	FI_MM_STATE_IDLE,
	FI_MM_STATE_STARTING,
	FI_MM_STATE_RUNNING,
	FI_MM_STATE_STOPPING,
};

struct ofi_mem_monitor {
	struct dlist_entry		list;
	enum fi_hmem_iface		iface;
	enum fi_mm_state                state;

	void (*init)(struct ofi_mem_monitor *monitor);
	void (*cleanup)(struct ofi_mem_monitor *monitor);
	int (*start)(struct ofi_mem_monitor *monitor);
	void (*stop)(struct ofi_mem_monitor *monitor);
	int (*subscribe)(struct ofi_mem_monitor *notifier,
			 const void *addr, size_t len,
			 union ofi_mr_hmem_info *hmem_info);
	void (*unsubscribe)(struct ofi_mem_monitor *notifier,
			    const void *addr, size_t len,
			    union ofi_mr_hmem_info *hmem_info);

	/* Valid is a memory monitor operation used to query a memory monitor to
	 * see if the memory monitor's view of the buffer is still valid. If the
	 * memory monitor's view of the buffer is no longer valid (e.g. the
	 * pages behind a given virtual address have changed), the buffer needs
	 * to be re-registered.
	 */
	bool (*valid)(struct ofi_mem_monitor *notifier, const void *addr,
		      size_t len, union ofi_mr_hmem_info *hmem_info);
};

void ofi_monitor_init(struct ofi_mem_monitor *monitor);
void ofi_monitor_cleanup(struct ofi_mem_monitor *monitor);
void ofi_monitors_init(void);
void ofi_monitors_cleanup(void);
int ofi_monitor_import(struct fid *fid);

int ofi_monitors_add_cache(struct ofi_mem_monitor **monitors,
			   struct ofi_mr_cache *cache);
void ofi_monitors_del_cache(struct ofi_mr_cache *cache);
void ofi_monitor_notify(struct ofi_mem_monitor *monitor,
			const void *addr, size_t len);
void ofi_monitor_flush(struct ofi_mem_monitor *monitor);

int ofi_monitor_subscribe(struct ofi_mem_monitor *monitor,
			  const void *addr, size_t len,
			  union ofi_mr_hmem_info *hmem_info);
void ofi_monitor_unsubscribe(struct ofi_mem_monitor *monitor,
			     const void *addr, size_t len,
			     union ofi_mr_hmem_info *hmem_info);

extern struct ofi_mem_monitor *default_monitor;
extern struct ofi_mem_monitor *default_cuda_monitor;
extern struct ofi_mem_monitor *default_rocr_monitor;
extern struct ofi_mem_monitor *default_ze_monitor;

/*
 * Userfault fd memory monitor
 */
struct ofi_uffd {
	struct ofi_mem_monitor		monitor;
	pthread_t			thread;
	int				fd;
};

extern struct ofi_mem_monitor *uffd_monitor;

/*
 * Memory intercept call memory monitor
 */
struct ofi_memhooks {
	struct ofi_mem_monitor          monitor;
	struct dlist_entry		intercept_list;
};

extern struct ofi_mem_monitor *memhooks_monitor;

extern struct ofi_mem_monitor *cuda_monitor;
extern struct ofi_mem_monitor *rocr_monitor;
extern struct ofi_mem_monitor *ze_monitor;
extern struct ofi_mem_monitor *import_monitor;

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
	enum fi_hmem_iface iface;
	uint64_t device;
};

void ofi_mr_update_attr(uint32_t user_version, uint64_t caps,
			const struct fi_mr_attr *user_attr,
			struct fi_mr_attr *cur_abi_attr);
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
	int				cuda_monitor_enabled;
	int				rocr_monitor_enabled;
	int				ze_monitor_enabled;
};

extern struct ofi_mr_cache_params	cache_params;

struct ofi_mr_entry {
	struct ofi_mr_info		info;
	struct ofi_rbnode		*node;
	int				use_cnt;
	struct dlist_entry		list_entry;
	union ofi_mr_hmem_info		hmem_info;
	uint8_t				data[];
};

#define OFI_HMEM_MAX 4

struct ofi_mr_cache {
	struct util_domain		*domain;
	struct ofi_mem_monitor		*monitors[OFI_HMEM_MAX];
	struct dlist_entry		notify_entries[OFI_HMEM_MAX];
	size_t				entry_data_size;

	struct ofi_rbmap		tree;
	struct dlist_entry		lru_list;
	struct dlist_entry		dead_region_list;
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

int ofi_mr_cache_init(struct util_domain *domain,
		      struct ofi_mem_monitor **monitors,
		      struct ofi_mr_cache *cache);
void ofi_mr_cache_cleanup(struct ofi_mr_cache *cache);

void ofi_mr_cache_notify(struct ofi_mr_cache *cache, const void *addr, size_t len);

static inline bool ofi_mr_cache_full(struct ofi_mr_cache *cache)
{
	return (cache->cached_cnt >= cache_params.max_cnt) ||
	       (cache->cached_size >= cache_params.max_size);
}

bool ofi_mr_cache_flush(struct ofi_mr_cache *cache, bool flush_lru);

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
