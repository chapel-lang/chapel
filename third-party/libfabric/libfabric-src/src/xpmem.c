/*
 * (C) Copyright 2023 UT-Battelle, LLC. All rights reserved.
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

#if HAVE_CONFIG_H
#include <config.h>
#endif

#include <rdma/fi_errno.h>
#include <ofi_xpmem.h>
#include <ofi_mem.h>
#include <ofi_mr.h>
#include <ofi.h>
#include <stdio.h>

#define XPMEM_DEFAULT_MEMCPY_CHUNK_SIZE 262144
struct ofi_xpmem *xpmem = NULL;

#if HAVE_XPMEM
/* global cache for use with xpmem */
static struct ofi_mr_cache *xpmem_cache;

int ofi_xpmem_init(void)
{
	/* Any attachment that goes past the Linux TASK_SIZE will always
	 * fail. To prevent this we need to determine the value of
	 * TASK_SIZE. On x86_64 the value was hard-coded in sm to be
	 * 0x7ffffffffffful but this approach does not work with AARCH64
	 * (and possibly other architectures). Since there is really no
	 * way to directly determine the value we can (in all cases?) look
	 * through the mapping for this process to determine what the
	 * largest address is.  This should be the top of the stack. No
	 * heap allocations should be larger than this value.  Since the
	 * largest address may differ between processes the value must be
	 * shared as part of the modex and stored in the endpoint. */
	int ret = 0;
	char buffer[1024];
	uintptr_t address_max = 0;
	FILE *fh;
	uintptr_t high;
	char *tmp;

	fi_param_define(&core_prov, "xpmem_memcpy_chunksize", FI_PARAM_SIZE_T,
			"Maximum size used for a single memcpy call. "
			"(default: %d)", XPMEM_DEFAULT_MEMCPY_CHUNK_SIZE);

	xpmem = calloc(sizeof(*xpmem), 1);
	if (!xpmem)
		return -FI_ENOMEM;

	fh = fopen("/proc/self/maps", "r");
	if (NULL == fh) {
		FI_WARN(&core_prov, FI_LOG_CORE,
				"could not open /proc/self/maps for reading");
		ret = -FI_EIO;
		goto fail;
	}

	while (fgets(buffer, sizeof(buffer), fh)) {
		/* each line of /proc/self/maps starts with low-high in
		 * hexidecimal (without a 0x) */
		(void) strtoul(buffer, &tmp, 16);
		high = strtoul(tmp + 1, NULL, 16);
		if (address_max < high)
			address_max = high;
	}
	fclose(fh);

	if (address_max == 0) {
		FI_WARN(&core_prov, FI_LOG_CORE,
			"could not determine the address max");
		ret = -FI_EIO;
		goto fail;
	}

	/* save the calcuated maximum */
	xpmem->pinfo.address_max = address_max - 1;

	/* export the process virtual address space for use with xpmem */
	xpmem->pinfo.seg_id = xpmem_make(0, XPMEM_MAXADDR_SIZE, XPMEM_PERMIT_MODE,
					 (void *) 0666);
	if (xpmem->pinfo.seg_id == -1) {
		FI_INFO(&core_prov, FI_LOG_CORE,
			"Failed to export process virtual address space for use with xpmem\n");
		ret = -FI_ENODATA;
		goto fail;
	}

	ret = fi_param_get_size_t(&core_prov, "xpmem_memcpy_chunksize",
				  &xpmem->memcpy_chunk_size);
	if (ret)
		xpmem->memcpy_chunk_size = XPMEM_DEFAULT_MEMCPY_CHUNK_SIZE;

	ret = ofi_xpmem_cache_open(&xpmem_cache);
	if (ret)
		FI_WARN(&core_prov, FI_LOG_CORE,
			"could not open xpmem cache");

	return ret;

fail:
	free(xpmem);
	xpmem = NULL;
	return ret;
}

int ofi_xpmem_cleanup(void)
{
	int ret = 0;

	if (xpmem_cache)
		ofi_xpmem_cache_destroy(xpmem_cache);

	if (xpmem)
		ret = xpmem_remove(xpmem->pinfo.seg_id);

	return (ret) ? -FI_EINVAL : ret;
}

static inline void xpmem_memcpy(void *dst, void *src, size_t size)
{
	size_t copy_size;

	while (size > 0) {
		copy_size = MIN(size, xpmem->memcpy_chunk_size);
		memcpy(dst, src, copy_size);
		dst = (void *) ((uintptr_t) dst + copy_size);
		src = (void *) ((uintptr_t) src + copy_size);
		size -= copy_size;
	}
}

int ofi_xpmem_copy(struct iovec *local, unsigned long local_cnt,
                   struct iovec *remote, unsigned long remote_cnt,
                   size_t total, pid_t pid, bool write, void *user_data)
{
	int ret, i;
	struct iovec iov;
	uint64_t offset, copy_len, delta;
	void *mapped_addr;
	struct ofi_mr_entry *mr_entry;
	long page_size = ofi_get_page_size();

	assert(local_cnt == remote_cnt);

	for (i = 0; i < remote_cnt; i++) {
		iov.iov_base = (void *) ofi_get_page_start(remote[i].iov_base,
							   page_size);
		iov.iov_len =
			(uintptr_t) ofi_get_page_end(
					(void*)((uintptr_t)remote[i].iov_base +
					remote[i].iov_len), page_size) -
					(uintptr_t)iov.iov_base;

		ret = ofi_xpmem_cache_search(xpmem_cache, &iov, pid, &mr_entry,
					     (struct ofi_xpmem_client *)user_data);
		if (ret)
			return ret;

		delta = (uintptr_t) iov.iov_base -
			(uintptr_t) mr_entry->info.iov.iov_base;
		offset = (uintptr_t)((uintptr_t) remote[i].iov_base -
				     (uintptr_t) iov.iov_base);

		mapped_addr = (char*) (uintptr_t)mr_entry->info.mapped_addr +
				offset + delta;

		copy_len = (local[i].iov_len <= iov.iov_len - offset) ?
				local[i].iov_len : iov.iov_len - offset;

		if (write)
			xpmem_memcpy(mapped_addr, local[i].iov_base,
				     copy_len);
		else
			xpmem_memcpy(local[i].iov_base, mapped_addr,
				     copy_len);

		ofi_mr_cache_delete(xpmem_cache, mr_entry);
	}

	return 0;
}

int ofi_xpmem_enable(struct ofi_xpmem_pinfo *peer,
		     struct ofi_xpmem_client *xpmem)
{
	xpmem->apid = xpmem_get(peer->seg_id,
				XPMEM_RDWR, XPMEM_PERMIT_MODE, (void *) 0666);
	if (xpmem->apid == -1)
		return -FI_ENOSYS;

	return FI_SUCCESS;
}

void ofi_xpmem_release(struct ofi_xpmem_client *xpmem)
{
	xpmem_release(xpmem->apid);
}

#else

int ofi_xpmem_init(void)
{
	return -FI_ENOSYS;
}

int ofi_xpmem_cleanup(void)
{
	return -FI_ENOSYS;
}

int ofi_xpmem_copy(struct iovec *local, unsigned long local_cnt,
                   struct iovec *remote, unsigned long remote_cnt,
                   size_t total, pid_t pid, bool write, void *user_data)
{
	return -FI_ENOSYS;
}

int ofi_xpmem_enable(struct ofi_xpmem_pinfo *peer,
		     struct ofi_xpmem_client *xpmem)
{
	return -FI_ENOSYS;
}

void ofi_xpmem_release(struct ofi_xpmem_client *xpmem)
{
}

#endif /* HAVE_XPMEM */
