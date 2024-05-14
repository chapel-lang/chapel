/*
 * (C) Copyright (c) 2022 UT-Battelle, LLC. All rights reserved.
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

#include "ofi_hmem.h"

static bool rocr_ipc_monitor_valid(struct ofi_mem_monitor *monitor,
			const struct ofi_mr_info *info,
			struct ofi_mr_entry *entry)
{
	return (memcmp((void **)&info->handle,
		(void **)&entry->info.handle,
		sizeof(hsa_amd_ipc_memory_t)) == 0);
}

#else

static bool rocr_ipc_monitor_valid(struct ofi_mem_monitor *monitor,
			const struct ofi_mr_info *info,
			struct ofi_mr_entry *entry)
{
	return false;
}

#endif /* HAVE_ROCR */

static struct ofi_mem_monitor rocr_ipc_monitor_ = {
	.init = ofi_monitor_init,
	.cleanup = ofi_monitor_cleanup,
	.start = ofi_monitor_start_no_op,
	.stop = ofi_monitor_stop_no_op,
	.subscribe = ofi_monitor_subscribe_no_op,
	.unsubscribe = ofi_monitor_unsubscribe_no_op,
	.valid = rocr_ipc_monitor_valid,
	.name = "rocr_ipc",
};

struct ofi_mem_monitor *rocr_ipc_monitor = &rocr_ipc_monitor_;
