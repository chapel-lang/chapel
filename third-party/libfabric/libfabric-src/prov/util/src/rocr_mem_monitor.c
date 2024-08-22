/*
 * Copyright (c) 2020 Hewlett Packard Enterprise Development LP
 * Copyright (c) 2020 Amazon.com, Inc. or its affiliates. All rights reserved.
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

#include "ofi_mr.h"

#if HAVE_ROCR

#include "ofi_tree.h"
#include "ofi_iov.h"
#include "ofi_hmem.h"

#include <hsa/hsa_ext_amd.h>

struct rocr_mm_entry {
	struct iovec iov;
	struct ofi_rbnode *node;
};

struct rocr_mm {
	struct ofi_mem_monitor mm;
	struct ofi_rbmap *dev_region_tree;
};

static int rocr_mm_start(struct ofi_mem_monitor *monitor);
static void rocr_mm_stop(struct ofi_mem_monitor *monitor);
static int rocr_mm_subscribe(struct ofi_mem_monitor *monitor, const void *addr,
			     size_t len, union ofi_mr_hmem_info *hmem_info);
static void rocr_mm_unsubscribe(struct ofi_mem_monitor *monitor,
				const void *addr, size_t len,
				union ofi_mr_hmem_info *hmem_info);
static bool rocr_mm_valid(struct ofi_mem_monitor *monitor,
			   const struct ofi_mr_info *info,
			   struct ofi_mr_entry *entry);

static struct rocr_mm rocr_mm = {
	.mm = {
		.iface = FI_HMEM_ROCR,
		.init = ofi_monitor_init,
		.cleanup = ofi_monitor_cleanup,
		.start = rocr_mm_start,
		.stop = rocr_mm_stop,
		.subscribe = rocr_mm_subscribe,
		.unsubscribe = rocr_mm_unsubscribe,
		.valid = rocr_mm_valid,
	},
};

struct ofi_mem_monitor *rocr_monitor = &rocr_mm.mm;

static int rocr_rbmap_compare(struct ofi_rbmap *map, void *key, void *data)
{
	struct rocr_mm_entry *entry = data;
	struct iovec *iov = key;

	if (ofi_iov_left(&entry->iov, iov))
		return -1;
	else if (ofi_iov_right(&entry->iov, iov))
		return 1;

	/* If this fails, the ROCR memory monitor failed to have a single ROCR
	 * memory monitor entry per user allocated ROCR memory region.
	 */
	assert(ofi_iov_within(iov, &entry->iov));

	return 0;
}

static struct rocr_mm_entry *rocr_mm_entry_get_root(void)
{
	struct ofi_rbnode *node;

	node = ofi_rbmap_get_root(rocr_mm.dev_region_tree);
	if (node)
		return node->data;
	return NULL;
}

/* ROCR memory monitor entry find works by finding the node within the device
 * region tree which contains the address within an entry's monitored range.
 * Thus, we only need an address instead of an address and length when
 * searching.
 */
static struct rocr_mm_entry *rocr_mm_entry_find(const void *addr)
{
	struct ofi_rbnode *node;
	struct iovec iov = {
		.iov_base = (void *) addr,
		.iov_len = 1,
	};

	node = ofi_rbmap_find(rocr_mm.dev_region_tree, (void *) &iov);
	if (node)
		return node->data;
	return NULL;
}

/* Pointer to ROCR memory monitor entry can never be returned as user data. This
 * could lead to use-after-free. Instead, address and length is always returned.
 * Unsubscribe will attempt to lookup the corresponding ROCR memory monitor
 * entry and will free the entry if found.
 */
static void rocr_mm_dealloc_cb(void *addr, void *user_data)
{
	size_t len = (size_t) user_data;

	pthread_rwlock_rdlock(&mm_list_rwlock);
	pthread_mutex_lock(&mm_lock);
	ofi_monitor_unsubscribe(rocr_monitor, addr, len, NULL);
	pthread_mutex_unlock(&mm_lock);
	pthread_rwlock_unlock(&mm_list_rwlock);
}

static void rocr_mm_entry_free(struct rocr_mm_entry *entry)
{
	hsa_status_t hsa_ret __attribute__((unused));

	FI_DBG(&core_prov, FI_LOG_MR,
	       "ROCR buffer address %p length %lu monitor entry freed\n",
	       entry->iov.iov_base, entry->iov.iov_len);

	/* Two return codes are expected. HSA_STATUS_SUCCESS is returned if the
	 * deallocation callback was not triggered and the entry is freed.
	 * HSA_STATUS_ERROR_INVALID_ARGUMENT is returned if the deallocation
	 * callback was triggered and the entry is freed. Any other return code
	 * puts the monitor in an unknown state and is fatal.
	 */
	hsa_ret = ofi_hsa_amd_dereg_dealloc_cb(entry->iov.iov_base,
					       rocr_mm_dealloc_cb);
	assert(hsa_ret == HSA_STATUS_SUCCESS ||
	       hsa_ret == HSA_STATUS_ERROR_INVALID_ARGUMENT);

	ofi_rbmap_delete(rocr_mm.dev_region_tree, entry->node);
	free(entry);
}

/* Each ROCR memory monitor entry is sized to the entire user-allocated ROCR
 * memory region. A single deallocation callback is registered for the memory
 * region. This callback is called when the user frees the ROCR memory region.
 */
static int rocr_mm_entry_alloc(const void *addr, struct rocr_mm_entry **entry)
{
	hsa_amd_pointer_info_t hsa_info = {
		.size = sizeof(hsa_info),
	};
	hsa_status_t hsa_ret;
	int ret;

	*entry = malloc(sizeof(**entry));
	if (!*entry) {
		ret = -FI_ENOMEM;
		goto err;
	}

	/* Determine full ROCR memory region size. */
	hsa_ret = ofi_hsa_amd_pointer_info((void *) addr, &hsa_info, NULL, NULL,
					   NULL);
	if (hsa_ret != HSA_STATUS_SUCCESS) {
		FI_WARN(&core_prov, FI_LOG_CORE,
			"Failed to perform hsa_amd_pointer_info: %s\n",
			ofi_hsa_status_to_string(hsa_ret));
		ret = -FI_EIO;
		goto err_free_entry;
	}

	if (hsa_info.type != HSA_EXT_POINTER_TYPE_HSA) {
		FI_WARN(&core_prov, FI_LOG_CORE,
			"Cannot monitor non-HSA allocated memory\n");
		ret = -FI_EINVAL;
		goto err_free_entry;
	}

	(*entry)->iov.iov_base = hsa_info.agentBaseAddress;
	(*entry)->iov.iov_len = hsa_info.sizeInBytes;

	ret = ofi_rbmap_insert(rocr_mm.dev_region_tree,
			       (void *) &(*entry)->iov,
			       (void *) *entry, &(*entry)->node);
	if (ret) {
		FI_WARN(&core_prov, FI_LOG_MR,
			"Failed to insert into RB tree: %s\n", strerror(ret));
		goto err_free_entry;
	}

	/* Register a deallocation callback for this ROCR memory region. */
	hsa_ret = ofi_hsa_amd_reg_dealloc_cb(hsa_info.agentBaseAddress,
					     rocr_mm_dealloc_cb,
					     (void *) hsa_info.sizeInBytes);
	if (hsa_ret != HSA_STATUS_SUCCESS) {
		FI_WARN(&core_prov, FI_LOG_CORE,
			"Failed to perform hsa_amd_register_deallocation_callback: %s\n",
			ofi_hsa_status_to_string(hsa_ret));

		ret = -FI_EIO;
		goto err_rbmap_delete_entry;
	}

	FI_DBG(&core_prov, FI_LOG_MR,
	       "ROCR buffer address %p length %lu monitor entry allocated\n",
	       hsa_info.agentBaseAddress, hsa_info.sizeInBytes);

	return FI_SUCCESS;

err_rbmap_delete_entry:
	ofi_rbmap_delete(rocr_mm.dev_region_tree, (*entry)->node);
err_free_entry:
	free(*entry);
err:
	*entry = NULL;
	return ret;
}

static int rocr_mm_start(struct ofi_mem_monitor *monitor)
{
	rocr_mm.dev_region_tree = ofi_rbmap_create(rocr_rbmap_compare);
	if (!rocr_mm.dev_region_tree)
		return -FI_ENOMEM;
	return FI_SUCCESS;
}

static void rocr_mm_stop(struct ofi_mem_monitor *monitor)
{
	struct rocr_mm_entry *entry;

	while ((entry = rocr_mm_entry_get_root()))
		rocr_mm_entry_free(entry);

	assert(ofi_rbmap_empty(rocr_mm.dev_region_tree));

	ofi_rbmap_destroy(rocr_mm.dev_region_tree);
}

static void rocr_mm_unsubscribe(struct ofi_mem_monitor *monitor,
				const void *addr, size_t len,
				union ofi_mr_hmem_info *hmem_info)
{
	struct rocr_mm_entry *entry;
	size_t cur_len = len;
	void *cur_addr = (void *) addr;
	void *next_addr;

	/* The user unsubscribe region may span multiple ROCR memory regions.
	 * Each ROCR memory region needs to be freed and MR caches notified.
	 */
	while (cur_len) {
		entry = rocr_mm_entry_find(cur_addr);
		if (!entry)
			break;

		ofi_monitor_notify(rocr_monitor, entry->iov.iov_base,
				   entry->iov.iov_len);

		FI_DBG(&core_prov, FI_LOG_MR,
		       "ROCR buffer address %p length %lu unsubscribed\n",
		       entry->iov.iov_base, entry->iov.iov_len);

		next_addr = (void *) ((uintptr_t) ofi_iov_end(&entry->iov) + 1);

		rocr_mm_entry_free(entry);

		cur_len -= MIN((uintptr_t) next_addr - (uintptr_t) cur_addr,
			       cur_len);
		cur_addr = next_addr;
	}

	if (cur_len)
		FI_WARN(&core_prov, FI_LOG_MR,
			"Failed to completely unsubscribe from address %p length %lu\n",
			addr, len);
}

/* Subscribe is designed to monitor entire ROCR memory regions even if the user
 * subscribe region is smaller. All ROCR memory regions are inserted into an RB
 * tree for tracking. Future subscriptions will always reuse RB tree entries if
 * possible.
 *
 * RB tree entries can be removed in two different ways:
 * 1. An unsubscribe against the memory region occurs. This will occur when ROCR
 *    invokes the deregistration callback.
 * 2. The ROCR memory monitor is stopped.
 *
 * Note: The ROCR memory monitor does not impose a limit on the number of ROCR
 * memory regions which can be monitored.
 */
static int rocr_mm_subscribe(struct ofi_mem_monitor *monitor, const void *addr,
			     size_t len, union ofi_mr_hmem_info *hmem_info)
{
	struct rocr_mm_entry *entry;
	int ret = FI_SUCCESS;
	size_t cur_len = len;
	void *cur_addr = (void *) addr;
	void *next_addr;

	/* The user subscribe region may span multiple ROCR memory regions. For
	 * this case, each ROCR memory region needs to be monitored. This
	 * requires allocating a ROCR memory monitor entry for each ROCR memory
	 * region.
	 */
	while (cur_len) {
		entry = rocr_mm_entry_find(cur_addr);
		if (entry) {
			FI_DBG(&core_prov, FI_LOG_MR,
			"Reusing monitored ROCR buffer address %p length %lu\n",
			entry->iov.iov_base, entry->iov.iov_len);
		} else {
			/* On error, previous allocated entries are not cleaned
			 * up. This is harmless since these entries will either
			 * be cleaned up when the user frees the ROCR memory
			 * region or the memory monitor is stopped.
			 */
			ret = rocr_mm_entry_alloc(cur_addr, &entry);
			if (ret != FI_SUCCESS)
				break;
		}

		next_addr = (void *) ((uintptr_t) ofi_iov_end(&entry->iov) + 1);
		cur_len -= MIN((uintptr_t) next_addr - (uintptr_t) cur_addr,
			       cur_len);
		cur_addr = next_addr;
	}

	FI_LOG(&core_prov, ret ? FI_LOG_WARN : FI_LOG_DEBUG, FI_LOG_MR,
	       "ROCR buffer address %p length %lu subscribe status: %s\n", addr,
	       len, fi_strerror(-ret));

	return ret;
}

static bool rocr_mm_valid(struct ofi_mem_monitor *monitor,
			  const struct ofi_mr_info *info,
			  struct ofi_mr_entry *entry)
{
	/* no-op */
	return true;
}

#else

static int rocr_mm_start(struct ofi_mem_monitor *monitor)
{
	return -FI_ENOSYS;
}

static void rocr_mm_stop(struct ofi_mem_monitor *monitor)
{
}

static int rocr_mm_subscribe(struct ofi_mem_monitor *monitor, const void *addr,
			     size_t len, union ofi_mr_hmem_info *hmem_info)
{
	return -FI_ENOSYS;
}

static void rocr_mm_unsubscribe(struct ofi_mem_monitor *monitor,
				const void *addr, size_t len,
				union ofi_mr_hmem_info *hmem_info)
{
}

static bool rocr_mm_valid(struct ofi_mem_monitor *monitor,
			  const struct ofi_mr_info *info,
			  struct ofi_mr_entry *entry)
{
	return false;
}

static struct ofi_mem_monitor rocr_mm = {
	.iface = FI_HMEM_ROCR,
	.init = ofi_monitor_init,
	.cleanup = ofi_monitor_cleanup,
	.start = rocr_mm_start,
	.stop = rocr_mm_stop,
	.subscribe = rocr_mm_subscribe,
	.unsubscribe = rocr_mm_unsubscribe,
	.valid = rocr_mm_valid,
	.name = "rocr",
};

struct ofi_mem_monitor *rocr_monitor = &rocr_mm;

#endif /* HAVE_ROCR */
