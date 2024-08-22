/*

  This file is provided under a dual BSD/GPLv2 license.  When using or
  redistributing this file, you may do so under either license.

  GPL LICENSE SUMMARY

  Copyright(c) 2022 Intel Corporation.

  This program is free software; you can redistribute it and/or modify
  it under the terms of version 2 of the GNU General Public License as
  published by the Free Software Foundation.

  This program is distributed in the hope that it will be useful, but
  WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  General Public License for more details.

  Contact Information:
  Intel Corporation, www.intel.com

  BSD LICENSE

  Copyright(c) 2022 Intel Corporation.

  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions
  are met:

    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in
      the documentation and/or other materials provided with the
      distribution.
    * Neither the name of Intel Corporation nor the names of its
      contributors may be used to endorse or promote products derived
      from this software without specific prior written permission.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

*/

#ifdef PSM_ONEAPI

#include "psm_user.h"
#include "psm_am_internal.h"
#include "am_oneapi_memhandle_cache.h"
#include <fcntl.h>
#include <unistd.h>
#ifdef HAVE_DRM
#include <sys/ioctl.h>
#include <drm/i915_drm.h>
#endif
#ifdef HAVE_LIBDRM
#include <sys/ioctl.h>
#include <libdrm/i915_drm.h>
#endif
#ifdef PSM_HAVE_PIDFD
#include <sys/syscall.h>
#endif

#if defined(HAVE_DRM) || defined(HAVE_LIBDRM)
/*
 * rbtree cruft
 */
struct _cl_map_item;

typedef struct
{
	unsigned long           start;           /* start(base) virtual address
						    in peer process */
	uint32_t                ze_handle;       /* Sender's GEM handle or fd */
	uint64_t                alloc_id;        /* ze alloc_id */
	void                    *buf_ptr;        /* buffer pointer in this
						    process */
	psm2_epid_t             epid;
	struct _cl_map_item*    i_prev;          /* idle queue previous */
	struct _cl_map_item*    i_next;          /* idle queue next */
	am_ze_memhandle_cache_t cache;           /* only for gem_handle close */
}__attribute__ ((aligned (128))) rbtree_ze_memhandle_cache_mapitem_pl_t;

typedef struct {
	uint32_t                nelems;          /* number of elements in the cache */
} rbtree_ze_memhandle_cache_map_pl_t;

static psm2_error_t am_ze_memhandle_mpool_alloc(
					am_ze_memhandle_cache_t cache, uint32_t memcache_size);
static void am_ze_memhandle_delete(void *buf_ptr);

/*
 * Custom comparator
 */
typedef rbtree_ze_memhandle_cache_mapitem_pl_t ze_cache_item;

static int ze_cache_key_cmp(const ze_cache_item *a, const ze_cache_item *b)
{
	// we use epid as part of cache key so multi-ep and multi-process jobs
	// can have a better cache hit rate.  In some cases we may end up with
	// cache entries for the same buffer with different epid's all within the
	// same multi-ep rank, but this does no harm other than to waste some
	// cache space.  By including epid in key_cmp we have a chance to have
	// separate cache entries for the same sbuf address in different
	// sender's GPU virtual address space.
	switch (psm3_epid_cmp_internal(a->epid, b->epid)) {
	case -1: return -1;
	case 1: return 1;
	default:
		break;
	}

	// The sender has used zeMemGetAddressRange to normalize the address
	// so we can simply compare the start address of the allocation.
	// Note zeMemOpenIpcHandle only needs the start address as well, so we
	// ignore length
	if (a->start < b->start)
		return -1;
	if (b->start < a->start)
		return 1;

	return 0;
}


/*
 * Necessary rbtree cruft
 */
#define RBTREE_MI_PL  rbtree_ze_memhandle_cache_mapitem_pl_t
#define RBTREE_MAP_PL rbtree_ze_memhandle_cache_map_pl_t
#define RBTREE_CMP(a,b) ze_cache_key_cmp((a), (b))
#define RBTREE_ASSERT                     psmi_assert
#define RBTREE_MAP_COUNT(PAYLOAD_PTR)     ((PAYLOAD_PTR)->nelems)
#define RBTREE_NO_EMIT_IPS_CL_QMAP_PREDECESSOR

#include "psm3_rbtree.h"
#include "psm3_rbtree.c"

/*
 * Convenience rbtree cruft
 */
#define NELEMS(cache)	((cache)->map.payload.nelems)

#define IHEAD(cache)	((cache)->map.root)
#define LAST(cache)	(IHEAD(cache)->payload.i_prev)
#define FIRST(cache)	(IHEAD(cache)->payload.i_next)
#define INEXT(x)	((x)->payload.i_next)
#define IPREV(x)	((x)->payload.i_prev)

/*
 * Actual module data
 */
struct am_ze_memhandle_cache {
	cl_qmap_t map;
	mpool_t mpool;
	uint32_t size;
	psm2_mq_stats_t *stats;
};

static void print_ze_memhandle_cache_stats(psm2_mq_stats_t *stats)
{
	_HFI_DBG("limit=%lu,maxelems=%lu,hit=%lu,miss=%lu,evict=%lu,remove=%lu,clear=%lu\n",
		stats->gpu_ipc_cache_limit, stats->gpu_ipc_cache_max_nelems,
		stats->gpu_ipc_cache_hit, stats->gpu_ipc_cache_miss,
		stats->gpu_ipc_cache_evict, stats->gpu_ipc_cache_remove,
		stats->gpu_ipc_cache_clear);
}

/*
 * This is the callback function when mempool are resized or destroyed.
 * Upon calling cache free mpool is destroyed which in turn calls this callback
 * which helps in closing all memhandles.
 * TBD - only called for !is_alloc when destroying so could avoid keeping
 * cache pointer in memcache_item.  But when GEM_CLOSE is not needed
 * memhandle_delete won't need destroyng flag and can remove cache pointer then
 */
static void
psmi_ze_memhandle_cache_alloc_func(int is_alloc, void* context, void* obj)
{
	cl_map_item_t* memcache_item = (cl_map_item_t*)obj;
	if (!is_alloc) {
		if(memcache_item->payload.start)
			am_ze_memhandle_delete(memcache_item->payload.buf_ptr);
	}
}

/*
 * Creating mempool for ze memhandle cache nodes.
 */
static psm2_error_t
am_ze_memhandle_mpool_alloc(am_ze_memhandle_cache_t cache,
							uint32_t memcache_size)
{
	psm2_error_t err;
	if (memcache_size < 1)
		return PSM2_PARAM_ERR;

	cache->size = memcache_size;
	/* Creating a memory pool of size PSM3_ONEAPI_MEMCACHE_SIZE
	 * which includes the Root and NIL items
	 */
	cache->mpool = psm3_mpool_create_for_gpu(sizeof(cl_map_item_t),
					cache->size,
					cache->size, 0,
					UNDEFINED, NULL, NULL,
					psmi_ze_memhandle_cache_alloc_func,
					NULL);
	if (cache->mpool == NULL) {
		err = psm3_handle_error(PSMI_EP_NORETURN, PSM2_NO_MEMORY,
				"Couldn't allocate ONEAPI host receive buffer pool");
		return err;
	}
	return PSM2_OK;
}

/*
 * allocate and initialize memhandle cache
 * including rbtree.
 */
psm2_error_t am_ze_memhandle_cache_alloc(am_ze_memhandle_cache_t *cachep,
									uint32_t memcache_size,
									psm2_mq_stats_t *stats)
{
	cl_map_item_t *root = NULL, *nil_item = NULL;

	*cachep = (am_ze_memhandle_cache_t)psmi_calloc(
				NULL, UNDEFINED, 1, sizeof(**cachep));
	if (! *cachep)
		return PSM2_NO_MEMORY;

	psm2_error_t err = am_ze_memhandle_mpool_alloc(*cachep, memcache_size);
	if (err != PSM2_OK)
		return err;

	root = (cl_map_item_t *)psmi_calloc(NULL, UNDEFINED, 1, sizeof(cl_map_item_t));
	if (root == NULL) {
		err = PSM2_NO_MEMORY;
		goto fail;
	}
	nil_item = (cl_map_item_t *)psmi_calloc(NULL, UNDEFINED, 1, sizeof(cl_map_item_t));
	if (nil_item == NULL) {
		err = PSM2_NO_MEMORY;
		goto fail;
	}

	nil_item->payload.start = 0;
	nil_item->payload.epid = psm3_epid_zeroed_internal();
	ips_cl_qmap_init(&(*cachep)->map,root,nil_item);
	NELEMS(*cachep) = 0;

	 (*cachep)->stats = stats;

	stats->gpu_ipc_cache_limit = memcache_size;
	stats->gpu_ipc_cache_nelems = 0;
	stats->gpu_ipc_cache_max_nelems = 0;
	stats->gpu_ipc_cache_hit = 0;
	stats->gpu_ipc_cache_miss = 0;
	stats->gpu_ipc_cache_evict = 0;
	stats->gpu_ipc_cache_remove = 0;
	stats->gpu_ipc_cache_clear = 0;

	return PSM2_OK;

fail:
	if (nil_item)
		psmi_free(nil_item);
	if (root)
		psmi_free(root);
	if ((*cachep)->mpool)
		psm3_mpool_destroy((*cachep)->mpool);
	psmi_free(*cachep);
	return err;
}

void am_ze_memhandle_cache_free(am_ze_memhandle_cache_t cache)
{
	print_ze_memhandle_cache_stats(cache->stats);

	if (cache->map.nil_item)
		psmi_free(cache->map.nil_item);
	if (cache->map.root)
		psmi_free(cache->map.root);
	if (cache->mpool)
		psm3_mpool_destroy(cache->mpool);
	psmi_free(cache);
}

/*
 * Insert at the head of Idleq.
 */
static void
am_ze_idleq_insert(am_ze_memhandle_cache_t cache, cl_map_item_t* memcache_item)
{
	if (FIRST(cache) == NULL) {
		FIRST(cache) = memcache_item;
		LAST(cache) = memcache_item;
		return;
	}
	INEXT(FIRST(cache)) = memcache_item;
	IPREV(memcache_item) = FIRST(cache);
	FIRST(cache) = memcache_item;
	INEXT(FIRST(cache)) = NULL;
	return;
}

/*
 * Remove least recent used element.
 */
static void
am_ze_idleq_remove_last(am_ze_memhandle_cache_t cache, cl_map_item_t* memcache_item)
{
	if (!INEXT(memcache_item)) {
		LAST(cache) = NULL;
		FIRST(cache) = NULL;
	} else {
		LAST(cache) = INEXT(memcache_item);
		IPREV(LAST(cache)) = NULL;
	}
	// Null-out now-removed memcache_item's next and prev pointers out of
	// an abundance of caution
	INEXT(memcache_item) = IPREV(memcache_item) = NULL;
}

static void
am_ze_idleq_remove(am_ze_memhandle_cache_t cache, cl_map_item_t* memcache_item)
{
	if (LAST(cache) == memcache_item) {
		am_ze_idleq_remove_last(cache, memcache_item);
	} else if (FIRST(cache) == memcache_item) {
		FIRST(cache) = IPREV(memcache_item);
		INEXT(FIRST(cache)) = NULL;
	} else {
		INEXT(IPREV(memcache_item)) = INEXT(memcache_item);
		IPREV(INEXT(memcache_item)) = IPREV(memcache_item);
	}
	// Null-out now-removed memcache_item's next and prev pointers out of
	// an abundance of caution
	INEXT(memcache_item) = IPREV(memcache_item) = NULL;
}

static void
am_ze_idleq_reorder(am_ze_memhandle_cache_t cache, cl_map_item_t* memcache_item)
{
	if (FIRST(cache) == memcache_item && LAST(cache) == memcache_item ) {
		return;
	}
	am_ze_idleq_remove(cache, memcache_item);
	am_ze_idleq_insert(cache, memcache_item);
	return;
}

/*
 * After a successful cache hit, item is validated by doing a
 * memcmp on the handle stored and the handle we receive from the
 * sender. If the validation fails the item is removed from the idleq,
 * the rbtree, is put back into the mpool and ZeMemCloseIpcHandle function
 * is called.
 * Level Zero's alloc_id will be unique per allocation, even if the allocation
 * was at the same address.  In some cases, but not always, the ipc_handle
 * will also be different.  So we validate both, although just checking alloc_id
 * would be sufficient.
 */

static psm2_error_t
am_ze_memhandle_cache_validate(am_ze_memhandle_cache_t cache,
			       cl_map_item_t* memcache_item,
			       uintptr_t sbuf, uint32_t handle,
			       psm2_epid_t epid, uint64_t alloc_id)
{
	psmi_assert(!psm3_epid_cmp_internal(epid, memcache_item->payload.epid));
	psmi_assert(sbuf == memcache_item->payload.start);
	if (handle == memcache_item->payload.ze_handle &&
	    alloc_id == memcache_item->payload.alloc_id) {
		return PSM2_OK;
	}
	_HFI_DBG("cache remove stale entry: new start=%lu,handle=%u,alloc_id=%lu\n",
		 sbuf, handle, alloc_id);

	cache->stats->gpu_ipc_cache_remove++;
	ips_cl_qmap_remove_item(&cache->map, memcache_item);
	cache->stats->gpu_ipc_cache_nelems--;
	am_ze_memhandle_delete(memcache_item->payload.buf_ptr);
	am_ze_idleq_remove(cache, memcache_item);
	memset(memcache_item, 0, sizeof(*memcache_item));
	psm3_mpool_put(memcache_item);
	return PSM2_OK_NO_PROGRESS;
}

/*
 * Current eviction policy: Least Recently Used.
 */
static void
am_ze_memhandle_cache_evict(am_ze_memhandle_cache_t cache)
{
	cache->stats->gpu_ipc_cache_evict++;
	cl_map_item_t *p_item = LAST(cache);
	_HFI_VDBG("Removing (epid=%s,start=%lu,dev_ptr=%p,it=%p) from ze_memhandle_cachemap.\n",
			psm3_epid_fmt_internal(p_item->payload.epid, 0), p_item->payload.start,
			p_item->payload.buf_ptr, p_item);
	ips_cl_qmap_remove_item(&cache->map, p_item);
	cache->stats->gpu_ipc_cache_nelems--;
	am_ze_memhandle_delete(p_item->payload.buf_ptr);
	am_ze_idleq_remove_last(cache, p_item);
	memset(p_item, 0, sizeof(*p_item));
	psm3_mpool_put(p_item);
}

static psm2_error_t
am_ze_memhandle_cache_register(am_ze_memhandle_cache_t cache,
			       uintptr_t sbuf, uint32_t handle,
			       psm2_epid_t epid,
			       void *buf_ptr, uint64_t alloc_id)
{
	if (NELEMS(cache) == cache->size)
		am_ze_memhandle_cache_evict(cache);

	cl_map_item_t* memcache_item = psm3_mpool_get(cache->mpool);
	/* memcache_item cannot be NULL as we evict
	 * before the call to mpool_get. Check has
	 * been fixed to help with klockwork analysis.
	 */
	if (memcache_item == NULL)
		return PSM2_NO_MEMORY;
	memcache_item->payload.start = sbuf;
	memcache_item->payload.ze_handle = handle;
	memcache_item->payload.buf_ptr = buf_ptr;
	memcache_item->payload.alloc_id = alloc_id;
	memcache_item->payload.epid = epid;
	memcache_item->payload.cache = cache;
	ips_cl_qmap_insert_item(&cache->map, memcache_item);
	cache->stats->gpu_ipc_cache_nelems++;
	if (cache->stats->gpu_ipc_cache_nelems > cache->stats->gpu_ipc_cache_max_nelems)
		cache->stats->gpu_ipc_cache_max_nelems = cache->stats->gpu_ipc_cache_nelems;
	am_ze_idleq_insert(cache, memcache_item);
	_HFI_VDBG("registered: handle %u sbuf 0x%lx ptr %p alloc_id %lu\n",
		  handle, sbuf, buf_ptr, alloc_id);
	return PSM2_OK;
}

#ifndef PSM_HAVE_PIDFD
static inline psm2_error_t am_ze_prepare_fds_for_ipc_import(
		uint32_t gem_handle, int device_index, int *ipc_fd,
		psm2_epaddr_t epaddr)
{
	am_epaddr_t *am_epaddr = (am_epaddr_t*)epaddr;
	int fd;
	struct drm_prime_handle open_fd = {0, 0, -1};

	if (device_index >= num_ze_devices) {
		_HFI_ERROR("am_ze_memhandle_acquire received invalid device_index from peer: %d\n",
			device_index);
		psm3_handle_error(PSMI_EP_NORETURN, PSM2_INTERNAL_ERR,
			"device_index "
			"invalid - received from peer: %d",
			device_index);
		return PSM2_INTERNAL_ERR;
	}
	fd = am_epaddr->peer_fds[device_index];
	cur_ze_dev = &ze_devices[device_index];
	open_fd.flags = DRM_CLOEXEC | DRM_RDWR;
	open_fd.handle = gem_handle;
	if (ioctl(fd, DRM_IOCTL_PRIME_HANDLE_TO_FD, &open_fd) < 0) {
		_HFI_ERROR("ioctl failed for DRM_IOCTL_PRIME_HANDLE_TO_FD: %s\n", strerror(errno));
		psm3_handle_error(PSMI_EP_NORETURN, PSM2_INTERNAL_ERR,
			"ioctl "
			"failed for DRM_IOCTL_PRIME_HANDLE_TO_FD errno=%d",
			errno);
		return PSM2_INTERNAL_ERR;
	}
	*ipc_fd = open_fd.fd;

	return PSM2_OK;
}
#else
static inline psm2_error_t am_ze_prepare_fds_for_ipc_import(
		uint32_t handle, int device_index, int *ipc_fd,
		psm2_epaddr_t epaddr)
{
	int fd;
	am_epaddr_t *am_epaddr = (am_epaddr_t *)epaddr;

	fd = syscall(__NR_pidfd_getfd, am_epaddr->pidfd, handle, 0);
	if (fd < 0) {
		_HFI_ERROR("pidfd_getfd failed %d: %s\n", fd, strerror(errno));
		psm3_handle_error(PSMI_EP_NORETURN, PSM2_INTERNAL_ERR,
			"pidfd_getfd failed errno=%d (%s)",
			errno, strerror(errno));
		return PSM2_INTERNAL_ERR;
	}
	*ipc_fd = fd;

	return PSM2_OK;
}
#endif /* PSM_HAVE_PIDFD */
#endif /* defined(HAVE_DRM) || defined(HAVE_LIBDRM) */

#if defined(HAVE_DRM) || defined(HAVE_LIBDRM)
static void *am_ze_import_ipc_buf(uint32_t fd, uint8_t alloc_type)
{
	ze_external_memory_import_fd_t import_desc = {};
	void *ze_ipc_buf = NULL;

	import_desc.stype = ZE_STRUCTURE_TYPE_EXTERNAL_MEMORY_IMPORT_FD;
	import_desc.flags = ZE_EXTERNAL_MEMORY_TYPE_FLAG_DMA_BUF;
	import_desc.fd = fd;

	switch(alloc_type) {
	case ZE_MEMORY_TYPE_HOST:
	{
		ze_host_mem_alloc_desc_t host_desc = {};

		host_desc.stype = ZE_STRUCTURE_TYPE_HOST_MEM_ALLOC_DESC;
		host_desc.pNext = &import_desc;
		/* size & alignment are not used since this is an import.*/
		PSMI_ONEAPI_ZE_CALL(zeMemAllocHost, ze_context, &host_desc,
				    0, 0, &ze_ipc_buf);
	}
		break;
	case ZE_MEMORY_TYPE_DEVICE:
	{
		ze_device_mem_alloc_desc_t dev_desc = {};

		dev_desc.stype = ZE_STRUCTURE_TYPE_DEVICE_MEM_ALLOC_DESC;
		dev_desc.pNext = &import_desc;
		/* size & alignment are not used since this is an import. */
		PSMI_ONEAPI_ZE_CALL(zeMemAllocDevice, ze_context, &dev_desc,
				    0, 0, cur_ze_dev->dev, &ze_ipc_buf);
	}
		break;
	default:
		_HFI_ERROR("Invalid alloc_type %u for fd %u\n",
			   alloc_type, fd);
		return NULL;
	}

	return ze_ipc_buf;
}
#endif /* defined(HAVE_DRM) || defined(HAVE_LIBDRM) */

/*
 * The key used to search the cache is the senders buf address pointer and
 * epid.  The sender will have used zeMemGetAddressRange
 * to find the start of the memory containing the buffer (supplied as sbuf)
 * Upon match, we must validate the entry we find and may need to replace it.
 */
void *
am_ze_memhandle_acquire(am_ze_memhandle_cache_t cache,
			uintptr_t sbuf, uint32_t handle,
			psm2_epaddr_t epaddr, int device_index,
			uint64_t alloc_id, uint8_t alloc_type)
{
	void *buf_ptr = NULL;
	psm2_epid_t epid = epaddr->epid;
#if defined(HAVE_DRM) || defined(HAVE_LIBDRM)
	int ipc_fd = -1;
#endif
	_HFI_VDBG("sbuf=%lu,handle=%u,epid=%s\n",
		  sbuf, handle, psm3_epid_fmt_internal(epid, 0));
#if defined(HAVE_DRM) || defined(HAVE_LIBDRM)

	if (!cache) {
		if (am_ze_prepare_fds_for_ipc_import(handle, device_index, &ipc_fd,
						     epaddr) == PSM2_OK) {
			buf_ptr = am_ze_import_ipc_buf(ipc_fd, alloc_type);
			if (ipc_fd >= 0) {
				if (close(ipc_fd) < 0) {
					_HFI_ERROR("close failed for ipc_fd: %s\n", strerror(errno));
					psm3_handle_error(PSMI_EP_NORETURN, PSM2_INTERNAL_ERR,
						"close "
						"failed for ipc_fd %d errno=%d",
						ipc_fd, errno);
					return NULL;
				}
			}
		}
		return buf_ptr;
	}

	ze_cache_item key = {
		.start = (unsigned long) sbuf,
		.epid = epid
	};

	/*
	 * preconditions:
	 *  1) buffer [start,epid) may or may not be in cache->map already
	 *  2) there are no duplicate entries in cache->map
	 * postconditions:
	 *  1) buffer is in cache->map with same handle, epid, alloc_id
	 *  2) there are no duplicate entries in cache->map
	 *
	 * The key used to search the cache is the senders buf address pointer
	 * and epid.
	 * Upon a succesful hit in the cache, additional validation is required
	 * as the handle or alloc_id could be stale.
	 */
	cl_map_item_t *p_item = ips_cl_qmap_searchv(&cache->map, &key);
	if (p_item->payload.start) {
		// confirm the entry for sbuf matches the handle and is not stale
		if (am_ze_memhandle_cache_validate(cache, p_item, sbuf, handle,
						   epid, alloc_id) ==
						   PSM2_OK) {
			cache->stats->gpu_ipc_cache_hit++;
			am_ze_idleq_reorder(cache, p_item);
			return p_item->payload.buf_ptr;
		}

		// buffer found was stale am_oneapi_memhandle_cache_validate()
		// closed and removed existing entry.
		// Should find no more duplicates
#ifdef PSM_DEBUG
		p_item = ips_cl_qmap_searchv(&cache->map, &key);
		psmi_assert(! p_item->payload.start);
#endif
	}
	cache->stats->gpu_ipc_cache_miss++;

	if (am_ze_prepare_fds_for_ipc_import(handle, device_index, &ipc_fd,
					     epaddr) == PSM2_OK) {
		buf_ptr = am_ze_import_ipc_buf(ipc_fd, alloc_type);
		if (ipc_fd >= 0) {
			if (close(ipc_fd) < 0) {
				_HFI_ERROR("close failed for ipc_fd: %s\n", strerror(errno));
				psm3_handle_error(PSMI_EP_NORETURN, PSM2_INTERNAL_ERR,
					"close "
					"failed for ipc_fd %d errno=%d",
					ipc_fd, errno);
				return NULL;
			}
		}
		if (!buf_ptr)
			return NULL;
	} else {
		return NULL;
	}

	am_ze_memhandle_cache_register(cache, sbuf, handle, epid, buf_ptr,
				       alloc_id);
	return buf_ptr;
#else // if no drm, set up to return NULL as oneapi ipc handles don't work without drm
	buf_ptr = NULL;
	return buf_ptr;
#endif // defined(HAVE_DRM) || defined(HAVE_LIBDRM)

}

#if defined(HAVE_DRM) || defined(HAVE_LIBDRM)
void am_ze_memhandle_delete(void *buf_ptr)
{
	/* Release the reference to the buffer */
	PSMI_ONEAPI_ZE_CALL(zeMemFree, ze_context, buf_ptr);

#ifndef PSM_HAVE_PIDFD
	/*
	 * If pidfd is not used, we need to call GEM_CLOSE ioctl to remove the
	 * GEM handle from the handle cache of the peer device file's
	 * private file data in the kernel to avoid handle leak. However, we
	 * will have a potential risk condition that will fail a later request:
	 * (1) 3 requests with buf1, buf2, and buf1 are sent from sender side.
	 *     Requests 1 and 3 uses the same buffer and therefore have the
	 *     same gem_handle1.
	 * (2) buf1 is received and put into cache;
	 * (3) buf2 is received and buf1 is evicted from cache due to some
	 *     condition (small cache size). As a result, gem_handle1 is closed
	 *     through GEM_CLOSE ioctl. buf2 is put into cache.
	 * (4) Request 3 (with buf1) is received and HANDLE_TO_FD ioctl will
	 *     fail because the gem_handle has been removed from peer device
	 *     file's handle cache.
	 * For this reason, we prefer to leak the GEM handle over calling
	 * GEM_CLOSE.
	 */
#endif
}
#endif /* HAVE_DRM or HAVE_LIBDRM */

void
am_ze_memhandle_release(am_ze_memhandle_cache_t cache,
			void *buf_ptr)
{
#if defined(HAVE_DRM) || defined(HAVE_LIBDRM)
	if (!cache)
		am_ze_memhandle_delete(buf_ptr);
#endif // defined(HAVE_DRM) || defined(HAVE_LIBDRM)
	return;
}

#endif /* PSM_ONEAPI */
