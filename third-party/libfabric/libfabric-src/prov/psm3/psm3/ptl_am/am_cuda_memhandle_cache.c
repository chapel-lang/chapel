/*

  This file is provided under a dual BSD/GPLv2 license.  When using or
  redistributing this file, you may do so under either license.

  GPL LICENSE SUMMARY

  Copyright(c) 2016 Intel Corporation.

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

  Copyright(c) 2016 Intel Corporation.

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

#ifdef PSM_CUDA

#include "psm_user.h"
#include "am_cuda_memhandle_cache.h"

/*
 * rbtree cruft
 */
struct _cl_map_item;

typedef struct
{
	unsigned long		start;		 /* start virtual address */
	CUipcMemHandle		cuda_ipc_handle; /* cuda ipc mem handle */
	CUdeviceptr		cuda_ipc_dev_ptr;/* Cuda device pointer */
	psm2_epid_t             epid;
	struct _cl_map_item*	i_prev;	 /* idle queue previous */
	struct _cl_map_item*	i_next;	 /* idle queue next */
}__attribute__ ((aligned (128))) rbtree_cuda_memhandle_cache_mapitem_pl_t;

typedef struct {
	uint32_t		nelems;	/* number of elements in the cache */
} rbtree_cuda_memhandle_cache_map_pl_t;

static psm2_error_t am_cuda_memhandle_mpool_alloc(
					am_cuda_memhandle_cache_t cache, uint32_t memcache_size);

/*
 * Custom comparator
 */
typedef rbtree_cuda_memhandle_cache_mapitem_pl_t cuda_cache_item;

static int cuda_cache_key_cmp(const cuda_cache_item *a, const cuda_cache_item *b)
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

	// The sender has used cuMemGetAddressRange to normalize the address
	// so we can simply compare the start address of the allocation.
	// Note cuIpcOpenMemHandle only needs the start address as well, so we
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
#define RBTREE_MI_PL  rbtree_cuda_memhandle_cache_mapitem_pl_t
#define RBTREE_MAP_PL rbtree_cuda_memhandle_cache_map_pl_t
#define RBTREE_CMP(a,b) cuda_cache_key_cmp((a), (b))
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
struct am_cuda_memhandle_cache {
	cl_qmap_t map;
	mpool_t mpool;
	uint32_t size;
	psm2_mq_stats_t *stats;
};

static void print_cuda_memhandle_cache_stats(psm2_mq_stats_t *stats)
{
	_HFI_DBG("limit=%lu,maxelems=%lu,hit=%lu,miss=%lu,evict=%lu,remove=%lu,clear=%lu\n",
		stats->gpu_ipc_cache_limit, stats->gpu_ipc_cache_max_nelems,
		stats->gpu_ipc_cache_hit, stats->gpu_ipc_cache_miss,
		stats->gpu_ipc_cache_evict, stats->gpu_ipc_cache_remove,
		stats->gpu_ipc_cache_clear);
}

/*
 * This is the callback function when mempool are resized or destroyed.
 * Upon calling cache fini mpool is detroyed which in turn calls this callback
 * which helps in closing all memhandles.
 */
static void
psmi_cuda_memhandle_cache_alloc_func(int is_alloc, void* context, void* obj)
{
	cl_map_item_t* memcache_item = (cl_map_item_t*)obj;
	if (!is_alloc) {
		if(memcache_item->payload.start)
			PSMI_CUDA_CALL(cuIpcCloseMemHandle,
				       memcache_item->payload.cuda_ipc_dev_ptr);
	}
}

/*
 * Creating mempool for cuda memhandle cache nodes.
 */
static psm2_error_t
am_cuda_memhandle_mpool_alloc(am_cuda_memhandle_cache_t cache,
							uint32_t memcache_size)
{
	psm2_error_t err;
	if (memcache_size < 1)
		return PSM2_PARAM_ERR;

	cache->size = memcache_size;
	/* Creating a memory pool of size PSM3_CUDA_MEMCACHE_SIZE
	 * which includes the Root and NIL items
	 */
	cache->mpool = psm3_mpool_create_for_gpu(sizeof(cl_map_item_t),
					cache->size,
					cache->size, 0,
					UNDEFINED, NULL, NULL,
					psmi_cuda_memhandle_cache_alloc_func,
					NULL);
	if (cache->mpool == NULL) {
		err = psm3_handle_error(PSMI_EP_NORETURN, PSM2_NO_MEMORY,
				"Couldn't allocate CUDA host receive buffer pool");
		return err;
	}
	return PSM2_OK;
}

/*
 * allocate and initialize memhandle cache
 * including rbtree.
 */
psm2_error_t am_cuda_memhandle_cache_alloc(am_cuda_memhandle_cache_t *cachep,
										uint32_t memcache_size,
										psm2_mq_stats_t *stats)
{
	cl_map_item_t *root = NULL, *nil_item = NULL;

	*cachep = (am_cuda_memhandle_cache_t)psmi_calloc(
				NULL, UNDEFINED, 1, sizeof(**cachep));
	if (! *cachep)
		return PSM2_NO_MEMORY;

	psm2_error_t err = am_cuda_memhandle_mpool_alloc(*cachep, memcache_size);
	if (err != PSM2_OK)
		goto fail;

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

void am_cuda_memhandle_cache_free(am_cuda_memhandle_cache_t cache)
{
	print_cuda_memhandle_cache_stats(cache->stats);

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
am_cuda_idleq_insert(am_cuda_memhandle_cache_t cache, cl_map_item_t* memcache_item)
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
am_cuda_idleq_remove_last(am_cuda_memhandle_cache_t cache, cl_map_item_t* memcache_item)
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
am_cuda_idleq_remove(am_cuda_memhandle_cache_t cache, cl_map_item_t* memcache_item)
{
	if (LAST(cache) == memcache_item) {
		am_cuda_idleq_remove_last(cache, memcache_item);
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
am_cuda_idleq_reorder(am_cuda_memhandle_cache_t cache, cl_map_item_t* memcache_item)
{
	if (FIRST(cache) == memcache_item && LAST(cache) == memcache_item ) {
		return;
	}
	am_cuda_idleq_remove(cache, memcache_item);
	am_cuda_idleq_insert(cache, memcache_item);
	return;
}

/*
 * After a successful cache hit, item is validated by doing a
 * memcmp on the handle stored and the handle we receive from the
 * sender. If the validation fails the item is removed from the idleq,
 * the rbtree, is put back into the mpool and cuIpcCloseMemHandle function
 * is called.
 * Cuda ipcMemHandles for distinct allocations are unique, even if the
 * allocation was at the same address.  So this check catches stale cache
 * entries.
 */
static psm2_error_t
am_cuda_memhandle_cache_validate(am_cuda_memhandle_cache_t cache,
				 cl_map_item_t* memcache_item,
				 uintptr_t sbuf, CUipcMemHandle* handle,
				 psm2_epid_t epid)
{
	psmi_assert(!psm3_epid_cmp_internal(epid, memcache_item->payload.epid));
	psmi_assert(sbuf == memcache_item->payload.start);
	if (0 == memcmp(handle, &memcache_item->payload.cuda_ipc_handle,
			 sizeof(CUipcMemHandle))) {
		return PSM2_OK;
	}
	_HFI_DBG("cache collision: new entry start=%lu\n", sbuf);

	cache->stats->gpu_ipc_cache_remove++;
	ips_cl_qmap_remove_item(&cache->map, memcache_item);
	cache->stats->gpu_ipc_cache_nelems--;
	PSMI_CUDA_CALL(cuIpcCloseMemHandle,
		       memcache_item->payload.cuda_ipc_dev_ptr);
	am_cuda_idleq_remove(cache, memcache_item);
	memset(memcache_item, 0, sizeof(*memcache_item));
	psm3_mpool_put(memcache_item);
	return PSM2_OK_NO_PROGRESS;
}

/*
 * Current eviction policy: Least Recently Used.
 */
static void
am_cuda_memhandle_cache_evict(am_cuda_memhandle_cache_t cache)
{
	cache->stats->gpu_ipc_cache_evict++;
	cl_map_item_t *p_item = LAST(cache);
	_HFI_VDBG("Removing (epid=%s,start=%lu,dev_ptr=0x%llX,it=%p) from cuda_memhandle_cachemap.\n",
			psm3_epid_fmt_internal(p_item->payload.epid, 0), p_item->payload.start,
			p_item->payload.cuda_ipc_dev_ptr, p_item);
	ips_cl_qmap_remove_item(&cache->map, p_item);
	cache->stats->gpu_ipc_cache_nelems--;
	PSMI_CUDA_CALL(cuIpcCloseMemHandle, p_item->payload.cuda_ipc_dev_ptr);
	am_cuda_idleq_remove_last(cache, p_item);
	memset(p_item, 0, sizeof(*p_item));
	psm3_mpool_put(p_item);
}

static psm2_error_t
am_cuda_memhandle_cache_register(am_cuda_memhandle_cache_t cache,
				 uintptr_t sbuf, CUipcMemHandle* handle,
				 psm2_epid_t epid,
				 CUdeviceptr cuda_ipc_dev_ptr)
{
	if (NELEMS(cache) == cache->size)
		am_cuda_memhandle_cache_evict(cache);

	cl_map_item_t* memcache_item = psm3_mpool_get(cache->mpool);
	/* memcache_item cannot be NULL as we evict
	 * before the call to mpool_get. Check has
	 * been fixed to help with klockwork analysis.
	 */
	if (memcache_item == NULL)
		return PSM2_NO_MEMORY;
	memcache_item->payload.start = sbuf;
	memcache_item->payload.cuda_ipc_handle = *handle;
	memcache_item->payload.cuda_ipc_dev_ptr = cuda_ipc_dev_ptr;
	memcache_item->payload.epid = epid;
	ips_cl_qmap_insert_item(&cache->map, memcache_item);
	cache->stats->gpu_ipc_cache_nelems++;
	if (cache->stats->gpu_ipc_cache_nelems > cache->stats->gpu_ipc_cache_max_nelems)
		cache->stats->gpu_ipc_cache_max_nelems = cache->stats->gpu_ipc_cache_nelems;
	am_cuda_idleq_insert(cache, memcache_item);
	return PSM2_OK;
}

static void am_cuda_memhandle_cache_clear(am_cuda_memhandle_cache_t cache)
{
	_HFI_DBG("Closing all handles, clearing cuda_memhandle_cachemap and idleq. NELEMS=%u\n", NELEMS(cache));
	while (NELEMS(cache)) {
		am_cuda_memhandle_cache_evict(cache);
	}
	cache->stats->gpu_ipc_cache_clear++;
	_HFI_DBG("Closed all handles, cleared cuda_memhandle_cachemap and idleq. NELEMS=%u\n", NELEMS(cache));
}

/*
 * The key used to search the cache is the senders buf address pointer and
 * epid.  The sender will have used cuMemGetAddressRange
 * to find the start of the memory containing the buffer (supplied as sbuf).
 * Upon match, we must validate the entry we find and may need to replace it.
 */
CUdeviceptr
am_cuda_memhandle_acquire(am_cuda_memhandle_cache_t cache,
				uintptr_t sbuf, CUipcMemHandle* handle,
				psm2_epid_t epid)
{
	_HFI_VDBG("sbuf=%lu,handle=%p,epid=%s\n",
			sbuf, handle, psm3_epid_fmt_internal(epid, 0));

	CUdeviceptr cuda_ipc_dev_ptr;
	if(! cache) {
		PSMI_CUDA_CALL(cuIpcOpenMemHandle, &cuda_ipc_dev_ptr,
				 *handle, CU_IPC_MEM_LAZY_ENABLE_PEER_ACCESS);
		return cuda_ipc_dev_ptr;
	}

	cuda_cache_item key = {
		.start = (unsigned long) sbuf,
		.epid = epid
	};

	/*
	 * preconditions:
	 *  1) buffer [start,epid) may or may not be in cachemap already
	 *  2) there are no duplicate entries in cachemap
	 * postconditions:
	 *  1) buffer is in cachemap with same handle, epid
	 *  2) there are no duplicate entries in cachemap
	 *
	 * The key used to search the cache is the senders buf address pointer
	 * and epid.
	 * Upon a succesful hit in the cache, additional validation is required
	 * as the handle could be stale.
	 */
	cl_map_item_t *p_item = ips_cl_qmap_searchv(&cache->map, &key);
	if (p_item->payload.start) {
		// confirm the entry for sbuf matches the handle and is not stale
		if (am_cuda_memhandle_cache_validate(cache, p_item, sbuf, handle, epid) == PSM2_OK) {
			cache->stats->gpu_ipc_cache_hit++;
			am_cuda_idleq_reorder(cache, p_item);
			return p_item->payload.cuda_ipc_dev_ptr;
		}

		// buffer found was stale am_cuda_memhandle_cache_validate()
		// closed and removed existing entry.
		// Should find no more duplicates
#ifdef PSM_DEBUG
		p_item = ips_cl_qmap_searchv(&cache->map, &key);
		psmi_assert(! p_item->payload.start);
#endif
	}
	cache->stats->gpu_ipc_cache_miss++;

	CUresult cudaerr;
	PSMI_CUDA_CALL_EXCEPT(CUDA_ERROR_ALREADY_MAPPED, cuIpcOpenMemHandle,
		&cuda_ipc_dev_ptr, *handle, CU_IPC_MEM_LAZY_ENABLE_PEER_ACCESS);

	if (cudaerr == CUDA_ERROR_ALREADY_MAPPED) {
		// remote memory already mapped. Close all handles, clear cache,
		// and try again
		am_cuda_memhandle_cache_clear(cache);
		PSMI_CUDA_CALL(cuIpcOpenMemHandle, &cuda_ipc_dev_ptr, *handle,
			CU_IPC_MEM_LAZY_ENABLE_PEER_ACCESS);
	}

	am_cuda_memhandle_cache_register(cache, sbuf, handle,
					   epid, cuda_ipc_dev_ptr);
	return cuda_ipc_dev_ptr;
}

void
am_cuda_memhandle_release(am_cuda_memhandle_cache_t cache,
						CUdeviceptr cuda_ipc_dev_ptr)
{
	if(! cache)
		PSMI_CUDA_CALL(cuIpcCloseMemHandle, cuda_ipc_dev_ptr);
	return;
}

#endif
