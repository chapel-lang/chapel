/*
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

#include "ofi_mr.h"

#if HAVE_CUDA

#include "ofi_hmem.h"

static int cuda_mm_subscribe(struct ofi_mem_monitor *monitor, const void *addr,
			     size_t len, union ofi_mr_hmem_info *hmem_info)
{
	CUresult ret;
	const char *errname, *errstr;

	ret = ofi_cuPointerGetAttribute(&hmem_info->cuda_id,
					CU_POINTER_ATTRIBUTE_BUFFER_ID,
					(CUdeviceptr)addr);
	if (ret == CUDA_SUCCESS) {
		FI_DBG(&core_prov, FI_LOG_MR,
		       "Assigned CUDA buffer ID %lu to buffer %p\n",
		       hmem_info->cuda_id, addr);
		return FI_SUCCESS;
	}

	ofi_cuGetErrorName(ret, &errname);
	ofi_cuGetErrorString(ret, &errstr);
	FI_WARN(&core_prov, FI_LOG_MR,
		"Failed to get CUDA buffer ID for buffer %p len %lu\n"
		"cuPointerGetAttribute() failed: %s:%s\n", addr, len,
		errname, errstr);

	return -FI_EFAULT;
}

static void cuda_mm_unsubscribe(struct ofi_mem_monitor *monitor,
				const void *addr, size_t len,
				union ofi_mr_hmem_info *hmem_info)
{
	/* no-op */
}

static bool cuda_mm_valid(struct ofi_mem_monitor *monitor,
			  const struct ofi_mr_info *info,
			  struct ofi_mr_entry *entry)
{
	uint64_t id;
	CUresult ret;
	const char *errname, *errstr;

	/* CUDA buffer IDs are associated for each CUDA monitor entry. If the
	 * device pages backing the device virtual address change, a different
	 * buffer ID is associated with this mapping.
	 */
	ret = ofi_cuPointerGetAttribute(&id, CU_POINTER_ATTRIBUTE_BUFFER_ID,
					(CUdeviceptr)entry->info.iov.iov_base);
	if (ret == CUDA_SUCCESS && entry->hmem_info.cuda_id == id) {
		FI_DBG(&core_prov, FI_LOG_MR,
		       "CUDA buffer ID %lu still valid for buffer %p\n",
		       entry->hmem_info.cuda_id, entry->info.iov.iov_base);
		return true;
	} else if (ret == CUDA_SUCCESS && entry->hmem_info.cuda_id != id) {
		FI_DBG(&core_prov, FI_LOG_MR,
		       "CUDA buffer ID %lu invalid for buffer %p\n",
		       entry->hmem_info.cuda_id, entry->info.iov.iov_base);
	} else {
		ofi_cuGetErrorName(ret, &errname);
		ofi_cuGetErrorString(ret, &errstr);
		FI_WARN(&core_prov, FI_LOG_MR,
			"Failed to get CUDA buffer ID for buffer %p len %lu\n"
			"cuPointerGetAttribute() failed: %s:%s\n",
			entry->info.iov.iov_base, entry->info.iov.iov_len,
			errname, errstr);
	}

	return false;
}

static int cuda_monitor_start(struct ofi_mem_monitor *monitor)
{
	/* no-op */
	return FI_SUCCESS;
}

#else

static int cuda_mm_subscribe(struct ofi_mem_monitor *monitor, const void *addr,
			     size_t len, union ofi_mr_hmem_info *hmem_info)
{
	return -FI_ENOSYS;
}

static void cuda_mm_unsubscribe(struct ofi_mem_monitor *monitor,
				const void *addr, size_t len,
				union ofi_mr_hmem_info *hmem_info)
{
}

static bool cuda_mm_valid(struct ofi_mem_monitor *monitor,
			  const struct ofi_mr_info *info,
			  struct ofi_mr_entry *entry)
{
	return false;
}

static int cuda_monitor_start(struct ofi_mem_monitor *monitor)
{
	return -FI_ENOSYS;
}

#endif /* HAVE_CUDA */

void cuda_monitor_stop(struct ofi_mem_monitor *monitor)
{
	/* no-op */
}

static struct ofi_mem_monitor cuda_mm = {
	.iface = FI_HMEM_CUDA,
	.init = ofi_monitor_init,
	.cleanup = ofi_monitor_cleanup,
	.start = cuda_monitor_start,
	.stop = cuda_monitor_stop,
	.subscribe = cuda_mm_subscribe,
	.unsubscribe = cuda_mm_unsubscribe,
	.valid = cuda_mm_valid,
	.name = "cuda",
};

struct ofi_mem_monitor *cuda_monitor = &cuda_mm;
