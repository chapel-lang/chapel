/*
 * (C) Copyright 2021 Intel Corporation
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

#if HAVE_ZE

#include "ofi_hmem.h"

static int ze_mm_subscribe(struct ofi_mem_monitor *monitor, const void *addr,
			   size_t len, union ofi_mr_hmem_info *hmem_info)
{
	return ze_hmem_get_id(addr, &hmem_info->ze_id);
}

static void ze_mm_unsubscribe(struct ofi_mem_monitor *monitor,
			      const void *addr, size_t len,
			      union ofi_mr_hmem_info *hmem_info)
{
	/* no-op */
}

static bool ze_mm_valid(struct ofi_mem_monitor *monitor,
			const struct ofi_mr_info *info,
			struct ofi_mr_entry *entry)
{
	uint64_t id;
	int ret;

	ret = ze_hmem_get_id(entry->info.iov.iov_base, &id);
	if (ret)
		return false;


	return id == entry->hmem_info.ze_id;
}

static int ze_monitor_start(struct ofi_mem_monitor *monitor)
{
	/* no-op */
	return FI_SUCCESS;
}

#else

static int ze_mm_subscribe(struct ofi_mem_monitor *monitor, const void *addr,
			   size_t len, union ofi_mr_hmem_info *hmem_info)
{
	return -FI_ENOSYS;
}

static void ze_mm_unsubscribe(struct ofi_mem_monitor *monitor,
			      const void *addr, size_t len,
			      union ofi_mr_hmem_info *hmem_info)
{
}

static bool ze_mm_valid(struct ofi_mem_monitor *monitor,
			const struct ofi_mr_info *info,
			struct ofi_mr_entry *entry)
{
	return false;
}

static int ze_monitor_start(struct ofi_mem_monitor *monitor)
{
	return -FI_ENOSYS;
}

#endif /* HAVE_ZE */

void ze_monitor_stop(struct ofi_mem_monitor *monitor)
{
	/* no-op */
}

static struct ofi_mem_monitor ze_mm = {
	.iface = FI_HMEM_ZE,
	.init = ofi_monitor_init,
	.cleanup = ofi_monitor_cleanup,
	.start = ze_monitor_start,
	.stop = ze_monitor_stop,
	.subscribe = ze_mm_subscribe,
	.unsubscribe = ze_mm_unsubscribe,
	.valid = ze_mm_valid,
	.name = "ze",
};

struct ofi_mem_monitor *ze_monitor = &ze_mm;
